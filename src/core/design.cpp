//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-1-26
// ===================================================================
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//====================================================================
#include "core/design.h"


namespace cIcCore{

    Design::Design(){

		//Need to register celltypes for reflection later on
        qRegisterMetaType<cIcCore::Cell>("cIcCore::Cell");
        qRegisterMetaType<cIcCore::LayoutCell>("cIcCore::LayoutCell");
        qRegisterMetaType<cIcCore::PatternTile>("cIcCore::PatternTile");
        qRegisterMetaType<cIcCore::PatternTransistor>("cIcCore::PatternTransistor");
        qRegisterMetaType<cIcCore::PatternCapacitor>("cIcCore::PatternCapacitor");
        qRegisterMetaType<cIcCore::LayoutCell>("cIcCore::LayoutCell");
        cellTranslator["Gds::GdsPatternTransistor"] = "cIcCore::PatternTransistor";
        cellTranslator["Gds::GdsPatternCapacitor"] = "cIcCore::PatternCapacitor";
        cellTranslator["Gds::GdsPatternCapacitorGnd"] = "cIcCore::PatternCapacitor";
        cellTranslator["Layout::LayoutDigitalCell"] = "cIcCore::LayoutCell";
        nameTranslator["type"] = "mosType";
        console = new ConsoleOutput();

    }

	void Design::read(QString filename){
        QString val;
        QFile file;
        file.setFileName(filename);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        //      QStringList buffer;
        //    QString line = in.readLine();
        //    int line_number = 1;
        //    while (!line.isNull()) {
        //            buffer.append(line);
        //            line_number++;
        //            line = in.readLine();
        //     }

        val = file.readAll();
        file.close();

        QString spice_file = filename;
        spice_file.replace(".json",".spi");
        _spice_parser.parseFile(spice_file);

        QJsonParseError err;

        QJsonDocument d = QJsonDocument::fromJson(val.toUtf8(),&err);
        if(QJsonParseError::NoError != err.error ){
            QString verr = val.mid(0,err.offset);
            int position = 0;
            int index = verr.indexOf("\n",position+1);
            int line_count = 1;
            while(index > 0){
                position = index;
                line_count++;
                index = verr.indexOf("\n",position+1);
            }

            qWarning() << err.errorString() << " at line " << line_count ;
        }
        QJsonObject obj = d.object();
        QJsonValue cells = obj["cells"];
        if(cells.isArray()){
            //Run through the array of design cells, and try an match objects
            QJsonArray cellArray  = cells.toArray();
            foreach (const QJsonValue & value, cellArray) {
                QJsonObject c = value.toObject();
                QJsonValue name = c["name"];
                _cells[name.toString()] = c;

                //                QJsonValue v = c["class"];
                //Class name is defined
                //               if(v.isString()){
                this->createCell(c);
                //               }
            }

            //Import all cuts, and put them on top
            foreach(Cut* cut,Cut::getCuts()){
                Cell::addCell(cut);
                this->add(cut);
                _cell_names.insert(0,cut->name());
              }

        }else{
            qWarning() << "Could not find 'cells' array in json file\n";
        }

    }

	    void Design::createCell(QJsonObject jobj){

        QString cl  = jobj["class"].toString();
        QList<QJsonObject> * reverse_parents = new QList<QJsonObject>();
        if(jobj.contains("inherit")){
            QJsonValue inh = jobj["inherit"];
            this->findAllParents(reverse_parents,inh.toString());
        }

        //    #- Inherit class type from parent
        if (jobj.contains("class")  && reverse_parents->count() > 0) {
            int count = reverse_parents->count();
            for(int i=0;i<count; i++){
                //   foreach(QJsonObject parent, reverse_parents){
                QJsonObject parent  = reverse_parents->at(i);
                if(parent.contains("class")){
                    cl = parent["class"].toString();
                }
            }
        }

        //    my @parents = reverse(@reverse_parents);

        if( jobj.contains("leech") ){
            QString leech = jobj["leech"].toString();
            if( _cells.contains(leech) ){
                reverse_parents->push_back( _cells[leech] );
            }

        }

        //    #Inherit, but don't find older parents
        //    my @leech = split(",",$c->{leech});
        //    foreach my $leech (@leech) {
        //      my $cell = $self->{knowncells}->{$leech};
        //      if ($cell) {
        //        push(@parents,$cell);
        //      }
        //    }



        if (cl == "" ) {
            cl = "cIcCore::Cell";
        }



        //Translate the class name if it exists in the translator
        if(cellTranslator.contains(cl)){
            cl = cellTranslator[cl];
        }

        //Make the object, let's see how that works in Qt
        int id = QMetaType::type(cl.toUtf8().data());
        if(id != 0){
            void* vp = QMetaType::create(id);

            Cell * c  = static_cast<Cell*>(vp);
            QString name = jobj["name"].toString();
            c->setName(name);
            console->increaseIndent();
            console->commentStartClass(name);

            this->runAllParentMethods("afterNew",c,reverse_parents);
            this->runAllMethods("afterNew",c,jobj);

            //Run instancemethods
            this->runParentsIfObjectCanMethods(c,reverse_parents);
            this->runIfObjectCanMethods(c,jobj);

            //TODO: Add spice parsing before place
            cIcSpice::Subckt * ckt = _spice_parser.getSubckt(name);
            c->setSubckt(ckt);
            //TODO: How will I separate out instances from cells that should be printed to bottom?

            //Place
            this->runAllParentMethods("beforePlace",c,reverse_parents);
            this->runAllMethods("beforePlace",c,jobj);
            c->place();
            this->runAllParentMethods("afterPlace",c,reverse_parents);
            this->runAllMethods("afterPlace",c,jobj);

            //Route
            this->runAllParentMethods("beforeRoute",c,reverse_parents);
            this->runAllMethods("beforeRoute",c,jobj);
            c->route();
            this->runAllParentMethods("afterRoute",c,reverse_parents);
            this->runAllMethods("afterRoute",c,jobj);

            //Paint
            this->runAllParentMethods("beforePaint",c,reverse_parents);
            this->runAllMethods("beforePaint",c,jobj);
            c->paint();
            this->runAllParentMethods("afterPaint",c,reverse_parents);
            this->runAllMethods("afterPaint",c,jobj);
            c->addAllPorts();

            this->add(c);
            Cell::addCell(c);
            _cell_names.append(c->name());
            console->decreaseIndent();

        }else{

        }

    }

	/*
	  Search through JSON and find all parents
	*/
    void Design::findAllParents(QList<QJsonObject > *reverse_parents,QString inh)
    {
        if(_cells.contains(inh)){
            QJsonObject par = _cells[inh];
            if(par.contains("inherit")){
                this->findAllParents(reverse_parents,par["inherit"].toString());
            }
            reverse_parents->append(_cells[inh]);
        }

    }

    void Design::runAllParentMethods(QString jname, Cell *c, QList<QJsonObject> * parents){
        int count = parents->count();
        for(int i=0;i<count;i++){
            QJsonObject par = parents->at(i);
            this->runAllMethods(jname,c,par);
        }
    }

    void Design::runAllMethods(QString jname, Cell *c, QJsonObject jobj){
        QJsonValue jo = jobj[jname];
        if(jo.isUndefined()) return;

        QJsonObject job = jo.toObject();
        this->runIfObjectCanMethods(c,job,jname);

    }

    void Design::runParentsIfObjectCanMethods(Cell * c, QList<QJsonObject> * parents, QString theme){
        int count = parents->count();
        for(int i=0;i<count;i++){
            QJsonObject par = parents->at(i);
            this->runIfObjectCanMethods(c,par,theme);
        }

    }

    void Design::runIfObjectCanMethods(Cell * c, QJsonObject jobj, QString theme){

        const QMetaObject * mobj = c->metaObject();


        //Make a hash of the existing methods
        QMap<QString,QMetaMethod> methods;
        for(int i =0;i<mobj->methodCount();i++){
            QMetaMethod  m = mobj->method(i);
            methods[m.name()] = m;
        }
        QMap<QString,QMetaProperty> properties;
        for(int i =0;i<mobj->propertyCount();i++){
            QMetaProperty  p = c->metaObject()->property(i);
            properties[p.name()] = p;
        }

        QString c_name = jobj["name"].toString();
        if(c_name.compare("")== 0){
            c_name = c->name();
        }

        //Search throught the json file and find methods that can be run
        QRegularExpression re("^new|inherit|leech|class|name|before.*|after.*|comment");

        foreach( QString key, jobj.keys()){
            if(re.match(key).hasMatch()){ continue;}

            QString method_key = key;
            if(nameTranslator.contains(key)){
                method_key = nameTranslator[key];
            }

            if(methods.contains(method_key)){
                QMetaMethod m = methods[method_key];
                QJsonArray arg = jobj[key].toArray();
                m.invoke(c,Qt::DirectConnection, Q_ARG(QJsonArray, arg));
                console->commentInvokeMethod(c_name,theme, m.name());
            }else if(method_key.endsWith("s")  && methods.contains(method_key.left(key.length()-1))){

                console->commentInvokeMethod(c_name,theme, method_key);
                //Iterate over array if function exists without the "s" at the end
                QMetaMethod m = methods[method_key.left(key.length()-1)];
                QJsonArray arg = jobj[key].toArray();
                foreach(QJsonValue v, arg){
                    if(v.isArray()){
                        QJsonArray ar1 = v.toArray();
                        m.invoke(c,Qt::DirectConnection, Q_ARG(QJsonArray, ar1));
                    }else if(v.isObject()){
                        QJsonObject ar1 = v.toObject();
                        m.invoke(c,Qt::DirectConnection, Q_ARG(QJsonObject, ar1));
                    }
                }

            }else if(properties.contains(method_key)){
                QMetaProperty p = properties[method_key];
                QJsonValue v = jobj[key];
                p.write(c,v.toVariant());
                console->commentSetProperty(c_name,theme, p.name());

            }else{
                console->errorMethodNotFound(c_name,theme, key);
            }
        }

    }






};
