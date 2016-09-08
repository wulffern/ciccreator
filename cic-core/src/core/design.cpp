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
        qRegisterMetaType<cIcCore::PatternCapacitorGnd>("cIcCore::PatternCapacitorGnd");
        qRegisterMetaType<cIcCore::LayoutCell>("cIcCore::LayoutCell");
		qRegisterMetaType<cIcCore::LayoutRotateCell>("cIcCore::LayoutRotateCell");
		qRegisterMetaType<cIcCore::LayoutRotateCell>("cIcCore::LayoutRotateCell");
        qRegisterMetaType<cIcCells::SAR>("cIcCells::SAR");
        qRegisterMetaType<cIcCells::CapCell>("cIcCells::CapCell");
                qRegisterMetaType<cIcCells::CDAC>("cIcCells::CDAC");
        qRegisterMetaType<cIcCore::PatternResistor>("cIcCore::PatternResistor");


        //Translate from Perl names to c++ names
        cellTranslator["Gds::GdsPatternTransistor"] = "cIcCore::PatternTransistor";
        cellTranslator["Gds::GdsPatternCapacitor"] = "cIcCore::PatternCapacitor";
        cellTranslator["Gds::GdsPatternCapacitorGnd"] = "cIcCore::PatternCapacitorGnd";
        cellTranslator["Layout::LayoutDigitalCell"] = "cIcCore::LayoutCell";
        cellTranslator["Layout::LayoutRotateCell"] = "cIcCore::LayoutRotateCell";
        cellTranslator["Layout::LayoutSARCDAC"] = "cIcCells::SAR";
        cellTranslator["Layout::LayoutCDACSmall"] = "cIcCells::CDAC";
        cellTranslator["Layout::LayoutCapCellSmall"] = "cIcCells::CapCell";
        nameTranslator["type"] = "mosType";
        console = new ConsoleOutput();

    }

    void Design::read(QString filename){
        QString val;


        QString spice_file = filename;
        spice_file.replace(".json",".spi");
        _spice_parser.parseFile(spice_file);
        QJsonObject obj = this->readJson( filename);

        QJsonValue cells = obj["cells"];
        if(cells.isArray()){
            //Run through the array of design cells, and try an match objects
            QJsonArray cellArray  = cells.toArray();
            foreach (const QJsonValue & value, cellArray) {
                QJsonObject c = value.toObject();
                QJsonValue name = c["name"];
                _cells[name.toString()] = c;
                this->createCell(c);
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

    Subckt * Design::getSpiceSubckt(QJsonObject jobj, QList<QJsonObject>* reverse_parents, QString name)
    {
        cIcSpice::Subckt * ckt = _spice_parser.getSubckt(name);
        if(ckt == NULL && jobj.contains("spice")){
            QJsonArray ar = jobj["spice"].toArray();
            QStringList strlist;
            foreach(QJsonValue v,ar){
                strlist.append(v.toString());
            }
            _spice_parser.parseSubckt(0,strlist);
            ckt = _spice_parser.getSubckt(name);
        }


        if(ckt == NULL){ //Try to find a parent with a subckt
            int count = reverse_parents->count();
            for(int i=count-1;i>=0; i--){
                QJsonObject parent  = reverse_parents->at(i);
                cIcSpice::Subckt * ckt_parent = _spice_parser.getSubckt(parent["name"].toString());
                qDebug() << parent["name"];
                
                
                
                if(ckt_parent == NULL) continue;
                
                //Apply spice-regex
                if(jobj.contains("spiceRegex")){

                    QJsonArray jarr = jobj["spiceRegex"].toArray();
                    QStringList strlist = ckt_parent->spiceStr();
                    strlist.replaceInStrings(parent["name"].toString(),name);
                    foreach(QJsonValue rval, jarr){
                        QJsonArray reg_arr = rval.toArray();
                        QString from = reg_arr[0].toString();
                        QString to = reg_arr[1].toString();
                        strlist.replaceInStrings(QRegularExpression(from),to);
                    }
                    _spice_parser.parseSubckt(0,strlist);
                    ckt = _spice_parser.getSubckt(name);

                }else{
                    ckt = ckt_parent;
                }
                if(ckt){
                    break;
                }
            }

        }


        

        if(ckt == NULL){
            ckt = new cIcSpice::Subckt();
            //cerr << "Error: Could not find any subckt to match " << name.toStdString() << ", skipping\n";

        }

        return ckt;
    }

    void Design::createCell(QJsonObject jobj){


        QString cl  = jobj["class"].toString();
        QString name = jobj["name"].toString();
        if(name == "") return;

        //- Find all parents
        QList<QJsonObject> * reverse_parents = new QList<QJsonObject>();
        if(jobj.contains("inherit")){
            QJsonValue inh = jobj["inherit"];
            this->findAllParents(reverse_parents,inh.toString());
        }

        //- Inherit class type from parent
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

        //- Find cells we want to leach
        if( jobj.contains("leech") ){
            QString leech = jobj["leech"].toString();
            if( _cells.contains(leech) ){
                reverse_parents->push_back( _cells[leech] );
            }

        }

        //- Get subckt
        cIcSpice::Subckt* ckt = getSpiceSubckt(jobj, reverse_parents, name);


        //- Translate the class name if it exists in the translator
        if(cellTranslator.contains(cl)){
            cl = cellTranslator[cl];
        }else{
//            cerr << "Error(design.cpp): Unknown class " << cl.toStdString() << " for " << name.toStdString() <<  "\n";
        }

        //- Set default class name
        if (cl == "" ) {
            cl = "cIcCore::LayoutCell";
        }

        //Make the object, let's see how that works in Qt
        int id = QMetaType::type(cl.toUtf8().data());
        if(id != 0){
            void* vp = QMetaType::create(id);

            Cell * c  = static_cast<Cell*>(vp);
            c->setName(name);
            console->increaseIndent();
            console->commentStartClass(name);

            this->runAllParentMethods("afterNew",c,reverse_parents);
            this->runAllMethods("afterNew",c,jobj);

            //- Run instancemethods
            this->runParentsIfObjectCanMethods(c,reverse_parents);
            this->runIfObjectCanMethods(c,jobj);

            if(ckt){
                c->setSubckt(ckt);
            }

            //- Place
            this->runAllParentMethods("beforePlace",c,reverse_parents);
            this->runAllMethods("beforePlace",c,jobj);
            c->place();
            this->runAllParentMethods("afterPlace",c,reverse_parents);
            this->runAllMethods("afterPlace",c,jobj);

            //- Route
            this->runAllParentMethods("beforeRoute",c,reverse_parents);
            this->runAllMethods("beforeRoute",c,jobj);
            c->route();
            this->runAllParentMethods("afterRoute",c,reverse_parents);
            this->runAllMethods("afterRoute",c,jobj);

            c->addAllPorts();

            //- Paint
            this->runAllParentMethods("beforePaint",c,reverse_parents);
            this->runAllMethods("beforePaint",c,jobj);
            c->paint();
            this->runAllParentMethods("afterPaint",c,reverse_parents);
            this->runAllMethods("afterPaint",c,jobj);

            this->add(c);
            Cell::addCell(c);
            _cell_names.append(c->name());
            console->decreaseIndent();
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
                QString name = jobj["name"].toString();
        if(jo.isUndefined()) return;

        if(jo.isObject()){
            QJsonObject job = jo.toObject();
                        job["name"] = name;
//			job["name"] = jobj["name"].toString();
            this->runIfObjectCanMethods(c,job,jname);

	}else if (jo.isArray()){
			QJsonArray job = jo.toArray();
			foreach(QJsonValue cjv, job){

				QJsonObject job = cjv.toObject();
				job["name"] = name;
//				cjo["name"] = jobj["name"].toString();
				this->runIfObjectCanMethods(c,job,jname);
			}

	}

    }

    void Design::runParentsIfObjectCanMethods(Cell * c, QList<QJsonObject> * parents, QString theme){
        int count = parents->count();
        for(int i=0;i<count;i++){
            QJsonObject par = parents->at(i);
//			par["name"] = c->name();
            this->runIfObjectCanMethods(c,par,theme);
        }

    }

    void Design::runMethod(QJsonValue v, QMetaMethod m, Cell* c)
    {

        if(v.isArray()){
            QJsonArray ar1 = v.toArray();
            m.invoke(c,Qt::DirectConnection, Q_ARG(QJsonArray, ar1));
        }else if(v.isObject()){
            QJsonObject ar1 = v.toObject();
            m.invoke(c,Qt::DirectConnection, Q_ARG(QJsonObject, ar1));
        }else{
            m.invoke(c,Qt::DirectConnection,Q_ARG(QJsonValue, v));
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

        if(c_name.compare("") == 0){
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


                        //Invoke method
            if(methods.contains(method_key) && jobj.contains(method_key)){
                QMetaMethod method = methods[method_key];
                QJsonValue value = jobj[method_key];

                console->commentInvokeMethod(c_name,theme, method.name());

                this->runMethod(value, method, c);

            }else if(method_key.endsWith("s")  && methods.contains(method_key.left(key.length()-1))){
                console->commentInvokeMethod(c_name,theme, method_key);
                //Iterate over array if function exists without the "s" at the end
                QMetaMethod method = methods[method_key.left(key.length()-1)];
                QJsonArray arg = jobj[key].toArray();

                foreach(QJsonValue value, arg){
                    this->runMethod(value, method, c);
                }

            }else if(properties.contains(method_key)){
                QMetaProperty p = properties[method_key];
                QJsonValue v = jobj[key];
                p.write(c,v.toVariant());
                console->commentSetProperty(c_name,theme, p.name());

            }else{
                //Ignore symbol commands, they only apply to skill
                //Ignore rows command, not necessary in this version
                //Ignore composite command, it is no longer required
                //Ignore description and comment
                if(key != "symbol" && key != "rows" && key != "composite" && key != "comment" && key != "description"){
                    console->errorMethodNotFound(c_name,theme, key);
                }
            }
        }



    }

    void Design::fromJson(QJsonObject o){

        QJsonArray ar =  o["cells"].toArray();
        foreach(QJsonValue v,ar){
            QJsonObject o = v.toObject();
            QString cl = o["class"].toString();
            Cell *c;
            if(cl.contains(QRegularExpression("Don't have special case yet"))){
            }else{
                c = new LayoutCell();
            }

            if(c){
                c->fromJson(o);
                this->add(c);
                Cell::addCell(c);
                _cell_names.append(c->name());
            }
        }

    }




    QJsonObject Design::toJson(){
        QJsonObject o;

        QJsonArray ar;

        for(int i=0;i<_cell_names.count();i++){
            Cell * c = Cell::getCell(_cell_names[i]);
            if(c){
                QJsonObject o = c->toJson();
                ar.append(o);
            }
        }
        o["cells"] = ar;
        return o;
    }


    QJsonObject Design::readJson(QString filename){

        QFile file;
        file.setFileName(filename);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString val;
        val = file.readAll();
        file.close();
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

			QString error("%1%2%3%4");
			console->comment(error.arg("JSON ERROR (line ")
							 .arg(line_count)
							 .arg("): ")
							 .arg(err.errorString())
							 ,ConsoleOutput::red);
			//QTextStream out(stdout);
			//console->startComment(out,ConsoleOutput::red);
            //out << "JSON ERROR: " << err.errorString() << " at line " << line_count ;
			//console->endComment(out);
        }
        QJsonObject obj = d.object();
        return obj;

    }

    void Design::readJsonFile(QString filename){

        QJsonObject obj = this->readJson(filename);
        this->fromJson(obj);
    }


    void Design::writeJsonFile(QString filename){

        QFile file;
        file.setFileName(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QJsonObject o = this->toJson();

        QJsonDocument d(o);
        file.write(d.toJson());
        file.close();
    }






};