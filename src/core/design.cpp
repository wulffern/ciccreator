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
        qRegisterMetaType<cIcCore::Cell>("cIcCore::Cell");
		qRegisterMetaType<cIcCore::LayoutCell>("cIcCore::LayoutCell");
        qRegisterMetaType<cIcCore::PatternTile>("cIcCore::PatternTile");
        qRegisterMetaType<cIcCore::PatternTransistor>("cIcCore::PatternTransistor");
        qRegisterMetaType<cIcCore::PatternTransistor>("cIcCore::PatternCapacitor");
        qRegisterMetaType<cIcCore::PatternTransistor>("cIcCore::PatternCapacitor");
        cellTranslator["Gds::GdsPatternTransistor"] = "cIcCore::PatternTransistor";
        cellTranslator["Gds::GdsPatternCapacitor"] = "cIcCore::PatternCapacitor";
        cellTranslator["Gds::GdsPatternCapacitorGnd"] = "cIcCore::PatternCapacitor";
        cellTranslator["Layout::LayoutDigitalCell"] = "cIcCore::LayoutCell";
        nameTranslator["type"] = "mosType";
    }


    void Design::findAllParents(QList<Cell *> reverse_parents,QString inh)
    {

    }

    void Design::runAllMethods(QString jname, Cell *c, QJsonObject jobj){
        QJsonValue jo = jobj[jname];
        if(jo.isUndefined()) return;

        QJsonObject job = jo.toObject();
        this->runIfObjectCanMethods(c,job);

    }

    void Design::runIfObjectCanMethods(Cell * c, QJsonObject jobj){

        const QMetaObject * mobj = c->metaObject();

        //Make a hash of the existing methods
        QHash<QString,QMetaMethod> methods;
        for(int i =0;i<mobj->methodCount();i++){
            QMetaMethod  m = mobj->method(i);
            methods[m.name()] = m;
        }
        QHash<QString,QMetaProperty> properties;
        for(int i =0;i<mobj->propertyCount();i++){
            QMetaProperty  p = c->metaObject()->property(i);
            properties[p.name()] = p;
        }


        //Search throught the json file and find methods that can be run
        QRegularExpression re("^new|class|name|before.*|after.*");
        //qWarning() << "Expression"  <<   re.isValid();
        foreach( QString key, jobj.keys()){
            if(re.match(key).hasMatch()){ continue;}

            if(nameTranslator.contains(key)){
                key = nameTranslator[key];
            }

            if(methods.contains(key)){
                QMetaMethod m = methods[key];
                QJsonArray arg = jobj[key].toArray();
                m.invoke(c,Qt::DirectConnection, Q_ARG(QJsonArray, arg));
            }else if(key.endsWith("s")  && methods.contains(key.left(key.length()-1))){

                //Iterate over array if function exists without the "s" at the end
                QMetaMethod m = methods[key.left(key.length()-1)];
                QJsonArray arg = jobj[key].toArray();
                foreach(QJsonValue v, arg){
                    QJsonArray ar1 = v.toArray();
                    m.invoke(c,Qt::DirectConnection, Q_ARG(QJsonArray, ar1));
                }

            }else if(properties.contains(key)){
                QMetaProperty p = properties[key];
                QJsonValue v = jobj[key];

                p.write(c,v.toVariant());

            }else{
//         qWarning() << "Could not find method " << key << " on " << c->metaObject()->className();
            }
        }

    }

    void Design::createCell(QString cl, QJsonObject jobj){

        QList<Cell *> reverse_parents;
        if(jobj.contains("inherit")){
            QJsonValue inh = jobj["inherit"];
            this->findAllParents(reverse_parents,inh.toString());
        }



		//TODO: implement inherit parents

//    #- Inherit class type from parent
        if (jobj.contains("class")  && reverse_parents.count() > 0) {
//			foreach my $parent (@reverse_parents) {
//        if ($parent->{class}) {
//          $class = $parent->{class};
//        }
//      }
        }

//    my @parents = reverse(@reverse_parents);

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
            Cell * c  = reinterpret_cast<Cell*>(vp);
            QString name = jobj["name"].toString();

            c->setName(name);

	    qWarning() << "Making " << name;

			
			
            //TODO: Make a "runAllMethods" equivalent

            this->runAllMethods("afterNew",c,jobj);

            //TODO: Run instance methods from parent

            //Run instancemethods
            this->runIfObjectCanMethods(c,jobj);

            //TODO: Add spice parsing before place
            cIcSpice::Subckt * ckt = _spice_parser.getSubckt(name);
            c->setSubckt(ckt);

			//TODO: How will I separate out instances from cells that should be printed to bottom?
			
            //Place
            this->runAllMethods("beforePlace",c,jobj);
            c->place();
            this->runAllMethods("afterPlace",c,jobj);

            //Route
            this->runAllMethods("beforeRoute",c,jobj);
            c->route();
            this->runAllMethods("afterRoute",c,jobj);

            //Paint
            this->runAllMethods("beforePaint",c,jobj);
            c->paint();
            this->runAllMethods("afterPaint",c,jobj);
            c->addAllPorts();

            this->add(c);
            Cell::addCell(c);



            // qWarning() << "Found class " <<obj->className()  << " " << obj->methodCount();

        }else{

            qWarning() << "Class not found " << cl << "\n";
        }

//    my $obj = new $class ($self->cast($c->{new}));
//    $obj->hasPR(1);
//    my $name = $c->{name};
//    if ($obj->can("name")) {
//      $obj->name($c->{name});
//    }
//    my $sname = sprintf("%-40s ",$name,$class);
//    $self->comment($sname);
//    $self->startIndent();

//    #- Check if it's an instance and load spice file
//    if ($obj->can("gdslib")) {
//      $obj->gdslib($self->gdslib);
//      $obj->schlib($self->schlib);

//      my $spice;
//      if ($c->{spice}) {
//        #- If the cell has embedded spice
//        my $buffer = join("\n",$self->cast($c->{spice}));
//        my @match = $buffer =~ m/\$\{([^\}]+)\}/g;
//        foreach my $m (@match) {
//          if ($c->{$m}) {
//            my $str = $c->{$m};
//            $buffer =~ s/\$\{$m\}/$str/g;
//          }
//        }
//        $self->parser->readSpiceBuffer($buffer);

//        $spice = $self->parser->getSubckt($name,"nowarn");
//      } else {
//        #- Try to read the spice from the read spice files
//        $spice  = $self->parser->getSubckt($name,"nowarn");
//        if (!$spice && scalar(@reverse_parents)) {
//          #- Read the spice from parents
//          my $inherit = $reverse_parents[0]->{name};
//          $spice = $self->parser->getSubckt($inherit);
//          if (exists($c->{spiceRegex})) {
//            my $buffer = $spice->{buffer};
//            $buffer =~ s/$inherit/$name/ig;
//            foreach my $regex (@{$c->{spiceRegex}}) {
//              my ($from,$to) = @{$regex};
//              $buffer =~ s/$from/$to/ig;
//            }
//            $self->parser->readSpiceBuffer($buffer);
//            #       print $buffer."\n";
//            $spice = $self->parser->getSubckt($name,"nowarn");
//          }
//        }
//      }

//      $obj->subckt($spice);
//    }

//    $self->runAllMethods("afterNew",$c,$obj,@parents);

//    #- Run instance methods from parents
//    foreach my $parent (@parents) {
//      $self->runIfObjectCanMethods($parent,$obj);
//    }

//    #- Run instance methods
//    $self->runIfObjectCanMethods($c,$obj);

//    #- Place
//    $self->runAllMethods("beforePlace",$c,$obj,@parents);
//    $obj->place() if $obj->can("place");
//    $self->runAllMethods("afterPlace",$c,$obj,@parents);

//    #- Route
//    $self->runAllMethods("beforeRoute",$c,$obj,@parents);
//    $obj->route() if $obj->can("route");
//    $self->runAllMethods("afterRoute",$c,$obj,@parents);


//    #- Paint
//    $self->runAllMethods("beforePaint",$c,$obj,@parents);
//    $obj->paint() if $obj->can("paint");
//    $self->runAllMethods("afterPaint",$c,$obj,@parents);

//    if ($obj->can("addAllPorts")) {
//      $obj->addAllPorts();
//    }

//    if ($obj->can("gdslib")) {
//      $self->gdslib->addInstance($obj);
//    } else {
//      if ($obj->can("schObject") && $obj->schObject) {
//        $self->schlib->addChild($obj->schObject);
//      }
//      $self->gdslib->addChild($obj);
//    }


//    $self->endIndent();

        //  this->_designs(name,obj)

    }

    void Design::read(QString filename){
        QString val;
        QFile file;
        qWarning() << filename << "\n";
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
        QJsonValue cells = obj.take("cells");
        if(cells.isArray()){
            //Run through the array of design cells, and try an match objects
            QJsonArray cellArray  = cells.toArray();
            foreach (const QJsonValue & value, cellArray) {
                QJsonObject c = value.toObject();
                QJsonValue v = c["class"];

                //Class name is defined
                if(v.isString()){
                    this->createCell(v.toString(),c);
                }
            }
        }else{
            qWarning() << "Could not find 'cells' array in json file\n";
        }

    }


};
