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
        qRegisterMetaType<cIcCore::Rect>("cIcCore::Rect");
        qRegisterMetaType<cIcCore::Text>("cIcCore::Text");
        qRegisterMetaType<cIcCore::Port>("cIcCore::Port");
        qRegisterMetaType<cIcCore::LayoutCell>("cIcCore::LayoutCell");
        qRegisterMetaType<cIcCore::PatternTile>("cIcCore::PatternTile");
        qRegisterMetaType<cIcCore::PatternTransistor>("cIcCore::PatternTransistor");
        qRegisterMetaType<cIcCore::PatternCapacitor>("cIcCore::PatternCapacitor");
        qRegisterMetaType<cIcCore::LayoutRotateCell>("cIcCore::LayoutRotateCell");
        qRegisterMetaType<cIcCells::SAR>("cIcCells::SAR");
        qRegisterMetaType<cIcCells::CapCell>("cIcCells::CapCell");
        qRegisterMetaType<cIcCells::PhCapCell>("cIcCells::PhCapCell");
        qRegisterMetaType<cIcCells::CDAC>("cIcCells::CDAC");
        qRegisterMetaType<cIcCore::PatternHighResistor>("cIcCore::PatternHighResistor");
        qRegisterMetaType<cIcCore::PatternResistor>("cIcCore::PatternResistor");
        qRegisterMetaType<cIcCore::ConnectSourceDrain>("ConnectSourceDrain");
        qRegisterMetaType<cIcCells::CapCellV2>("cIcCells::CapCellV2");


        //Translate from Perl names to c++ names
        cellTranslator["Gds::GdsPatternTransistor"] = "cIcCore::PatternTransistor";
        cellTranslator["Gds::GdsPatternHighResistor"] = "cIcCore::PatternHighResistor";
        cellTranslator["Gds::GdsPatternResistor"] = "cIcCore::PatternResistor";
        cellTranslator["Gds::GdsPatternCapacitor"] = "cIcCore::PatternCapacitor";
        cellTranslator["Gds::GdsPatternCapacitorGnd"] = "cIcCore::PatternCapacitor";
        cellTranslator["Layout::LayoutDigitalCell"] = "cIcCore::LayoutCell";
        cellTranslator["LayoutCell"] = "cIcCore::LayoutCell";
        cellTranslator["Layout::LayoutRotateCell"] = "cIcCore::LayoutRotateCell";
        cellTranslator["Layout::LayoutSARCDAC"] = "cIcCells::SAR";
        cellTranslator["Layout::LayoutCDACSmall"] = "cIcCells::CDAC";
        cellTranslator["Layout::LayoutCapCellSmall"] = "cIcCells::CapCell";
        nameTranslator["type"] = "mosType";
        console = new ConsoleOutput();
        readError = false;
        ignoreSetYoffsetHalf = false;

    }

    bool Design::hasTopCells(){
        if(topcells_.count() > 0){
            return true;
        }
        return false;
    }
    void Design::setPrefix(QString prefix){
        prefix_ = prefix;
    }

    bool fexists(const char *filename)
    {
        if (FILE *file = fopen(filename, "r")) {
            fclose(file);
            return true;
        } else {
            return false;
        }

    }

    void Design::addIncludePath(QString filename)
    {
        _includePaths.append(filename);
    }


    bool Design::readCells(QString filename)
    {
        // Read file
        QString spice_file = filename;
        spice_file.replace(".json",".spi");
        _spice_parser.parseFile(spice_file);

        console->comment("Reading '" + filename + "'",ConsoleOutput::green);
        QJsonObject obj = this->readJson( filename);

        if(readError){
            return false;
        }

        //Read options
        QJsonValue options = obj["options"];
        if(options.isObject()){
            QJsonObject opt = options.toObject();
            if(opt.contains("ignoreSetYoffsetHalf")){
                ignoreSetYoffsetHalf = opt["ignoreSetYoffsetHalf"].toBool();
            }
            if(opt.contains("prefix")){
                this->prefix_ = opt["prefix"].toString();
            }

            if(opt.contains("topcells")){
                QJsonArray a = opt["topcells"].toArray();
                    foreach(const QJsonValue & v, a){
                        topcells_.append(v.toString());
                    }
            }

        }


        //Read patterns
        QJsonValue patterns = obj["patterns"];
        if(patterns.isObject()){
            QJsonObject pat = patterns.toObject();
            foreach(const QString & str, pat.keys()){
                QStringList lst;
                QJsonValue jv = pat[str];
                if(jv.isArray()){

                    QJsonArray a = jv.toArray();
                    foreach(const QJsonValue & v, a){
                        lst.append(v.toString());

                    }
                    PatternTile::Patterns[str] = lst;
                }
            }

        }

        //Read libraries
        QJsonValue library = obj["library"];
        if(library.isArray()){
            QJsonArray libraryArray  = library.toArray();
            foreach (const QJsonValue & value, libraryArray) {
                QString libfile = value.toString();

                bool fileNotFound = true;

                if(fexists(libfile.toStdString().c_str())){
                    console->comment("Reading library '" + libfile + "'",ConsoleOutput::green);
                    this->readJsonFile(libfile);
                    fileNotFound = false;
                }

                if(fileNotFound){
                    console->error("Could not find file '" + libfile + "'");
                    return false;
                }

            }

        }



        //Read includes
        QJsonValue include = obj["include"];
        if(include.isArray()){
            QJsonArray includeArray  = include.toArray();
            foreach (const QJsonValue & value, includeArray) {
                QString incfile = value.toString();

                bool fileNotFound = true;

                if(fexists(incfile.toStdString().c_str())){
                    if(this->readCells(incfile))
                    {
                        fileNotFound = false;

                    }
                }

                foreach (QString incpath,_includePaths){
                    QString incf = QString("%1/%2").arg(incpath).arg(incfile);

                    if(fexists(incf.toStdString().c_str())){
                        if(this->readCells(incf)){
                            fileNotFound = false;
                        }

                    }

                }

                if(fileNotFound){
                    console->error("Could not find file '" + incfile + "'");
                    return false;
                }

            }

        }

        QJsonValue cells = obj["cells"];
        if(cells.isArray()){
            //Run through the array of design cells, and try an match objects
            QJsonArray cellArray  = cells.toArray();
            foreach (const QJsonValue & value, cellArray) {
                try{
                QJsonObject c = value.toObject();
                QJsonValue name = c["name"];
                _cells[name.toString()] = c;

                this->createCell(c);
                }catch(const std::exception &exc){
                    console->error(exc.what());
                    throw exc;
                }
            }

        }else{
            console->error("Could not find 'cells' array in json file\n");
            return false;

        }
        return true;
    }


    bool Design::read(QString filename){

        bool retval = true;


        if(readCells(filename)){
            //Import all cuts, and put them on top
            foreach(Cut* cut,Cut::getCuts()){
                Cell::addCell(cut);

                this->add(cut);
                _cell_names.insert(0,cut->name());
            }

            //If topcells is set, then mark which cells have been used
            foreach(QString s,topcells_){
                Cell * c = Cell::getCell(s);
                if(c){
                    c->setUsed(true);
                    c->updateUsedChildren();
                }

            }

        }else{
            retval = false;

        }

        return retval;
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

            //Apply spice-regex
            if(jobj.contains("spiceRegex")){

                QJsonArray jarr = jobj["spiceRegex"].toArray();
                foreach(QJsonValue rval, jarr){
                    QJsonArray reg_arr = rval.toArray();
                    QString from = reg_arr[0].toString();
                    QString to = reg_arr[1].toString();
                    strlist.replaceInStrings(QRegularExpression(from),to);
                }
            }

            _spice_parser.parseSubckt(0,strlist);
            ckt = _spice_parser.getSubckt(name);

        }


        if(ckt == NULL){ //Try to find a parent with a subckt
            int count = reverse_parents->count();
            for(int i=count-1;i>=0; i--){
                QJsonObject parent  = reverse_parents->at(i);
                cIcSpice::Subckt * ckt_parent = _spice_parser.getSubckt(parent["name"].toString());


                if(ckt_parent == NULL) continue;

                QStringList strlist = ckt_parent->spiceStr();
                strlist.replaceInStrings(parent["name"].toString(),name);

                _spice_parser.parseSubckt(0,strlist);
                ckt = _spice_parser.getSubckt(name);

                if(ckt){
                    break;
                }
            }

        }

        //Apply spice-regex
        if(ckt && jobj.contains("spiceRegex")){

            QStringList strlist = ckt->spiceStr();

            QJsonArray jarr = jobj["spiceRegex"].toArray();
            foreach(QJsonValue rval, jarr){
                QJsonArray reg_arr = rval.toArray();
                QString from = reg_arr[0].toString();
                QString to = reg_arr[1].toString();

                strlist.replaceInStrings(QRegularExpression(from),to);
            }


            _spice_parser.parseSubckt(0,strlist);
            ckt = _spice_parser.getSubckt(name);
        }



        return ckt;
    }

    void Design::createCell(QJsonObject jobj){


        QString cl  = jobj["class"].toString();
        QString name = jobj["name"].toString();
        if(name == ""){
            //- Skip error message if a cell has no name and class and contains comment
            if(!jobj.contains("comment"))
                qDebug() << "Error: no class name defined";
            return;
        }


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
        }

        //- Set default class name
        if (cl == "" ) {
            cl = "cIcCore::LayoutCell";

        }


        QList<LayoutCellDecorator*> decorators;

        //Find decorators
        if( jobj.contains("decorator") ){
            QJsonArray ar = jobj["decorator"].toArray();
            foreach(QJsonValue dcjv, ar){
                QJsonObject djob = dcjv.toObject();
                QString decorator = djob.keys()[0];
                QJsonValue jv = djob[decorator];

                //Make the object, let's see how that works in Qt
                int id = QMetaType::type(decorator.toUtf8().data());
                if(id != 0){
                    void* vp = QMetaType::create(id);
                    LayoutCellDecorator * c  = static_cast<LayoutCellDecorator*>(vp);
                    c->setOptions(jv);
                    decorators.append(c);
                }
            }
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

            foreach(auto lcd, decorators){
                if(c->isLayoutCell()){
                    lcd->setCell(static_cast<LayoutCell*>(c));
                    lcd->afterNew();
                }
            }
            //- Run instancemethods
            this->runParentsIfObjectCanMethods(c,reverse_parents);
            this->runIfObjectCanMethods(c,jobj);

            if(ckt){
                c->setSubckt(ckt);
            }

            //- Place
            this->runAllParentMethods("beforePlace",c,reverse_parents);
            this->runAllMethods("beforePlace",c,jobj);
            foreach(auto lcd, decorators){
                if(c->isLayoutCell()){
                    lcd->setCell(static_cast<LayoutCell*>(c));
                    lcd->beforePlace();
                }
            }
            c->place();
            foreach(auto lcd, decorators){
                if(c->isLayoutCell()){
                    lcd->setCell(static_cast<LayoutCell*>(c));
                    lcd->place();
                }
            }
            this->runAllParentMethods("afterPlace",c,reverse_parents);
            this->runAllMethods("afterPlace",c,jobj);
            foreach(auto lcd, decorators){
                if(c->isLayoutCell()){
                    lcd->setCell(static_cast<LayoutCell*>(c));
                    lcd->afterPlace();
                }
            }

            //- Route
            this->runAllParentMethods("beforeRoute",c,reverse_parents);
            this->runAllMethods("beforeRoute",c,jobj);
            foreach(auto lcd, decorators){
                if(c->isLayoutCell()){
                    lcd->setCell(static_cast<LayoutCell*>(c));
                    lcd->beforeRoute();
                }
            }
            c->route();
            this->runAllParentMethods("afterRoute",c,reverse_parents);
            this->runAllMethods("afterRoute",c,jobj);
            foreach(auto lcd, decorators){
                if(c->isLayoutCell()){
                    lcd->setCell(static_cast<LayoutCell*>(c));
                    lcd->afterRoute();
                }
            }
            c->addAllPorts();

            //- Paint
            this->runAllParentMethods("beforePaint",c,reverse_parents);
            this->runAllMethods("beforePaint",c,jobj);
            foreach(auto lcd, decorators){
                if(c->isLayoutCell()){
                    lcd->setCell(static_cast<LayoutCell*>(c));
                    lcd->beforePaint();
                }
            }
            c->paint();
            foreach(auto lcd, decorators){
                if(c->isLayoutCell()){
                    lcd->setCell(static_cast<LayoutCell*>(c));
                    lcd->paint();
                }
            }
            this->runAllParentMethods("afterPaint",c,reverse_parents);
            this->runAllMethods("afterPaint",c,jobj);
            foreach(auto lcd, decorators){
                if(c->isLayoutCell()){
                    lcd->setCell(static_cast<LayoutCell*>(c));
                    lcd->afterPaint();
                }
            }


            this->add(c);
            Cell::addCell(c);
            _cell_names.append(c->name());
            ckt = c->subckt();


            //Make sure this subckt is added to all subckts
            if(ckt){
                ckt->addSubckt();
            }

            console->decreaseIndent();
        }else{
            qDebug() << "Error(design.cpp): did not find " << cl;
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
//          job["name"] = jobj["name"].toString();
            this->runIfObjectCanMethods(c,job,jname);

        }else if (jo.isArray()){
            QJsonArray job = jo.toArray();
            foreach(QJsonValue cjv, job){

                QJsonObject ljob = cjv.toObject();
                ljob["name"] = name;
//              cjo["name"] = jobj["name"].toString();
                this->runIfObjectCanMethods(c,ljob,jname);
            }

        }

    }

    void Design::runParentsIfObjectCanMethods(Cell * c, QList<QJsonObject> * parents, QString theme){
        int count = parents->count();
        for(int i=0;i<count;i++){
            QJsonObject par = parents->at(i);
//          par["name"] = c->name();
            this->runIfObjectCanMethods(c,par,theme);
        }

    }

    void Design::runMethod(QJsonValue v, QMetaMethod m, Cell* c)
    {

        if(v.isArray()){
            QJsonArray ar1 = v.toArray();
            m.invoke(c,Qt::DirectConnection, Q_ARG(QJsonArray, ar1));
        }
        else if(v.isObject()){
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
        QRegularExpression re("^new|inherit|leech|class|name|before.*|after.*|comment|decorator|spiceRegex");


        foreach( QString key, jobj.keys()){
            if(re.match(key).hasMatch()){ continue;}
            if( ignoreSetYoffsetHalf && key == "setYoffsetHalf"){continue;}



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

    void Design::fromJson(QJsonObject obj){

        QJsonArray ar =  obj["cells"].toArray();

        foreach(QJsonValue v,ar){

            QJsonObject o = v.toObject();
            QString cl = o["class"].toString();
            Cell *c = NULL;


            //Make the object, let's see how that works in Qt
            int id = QMetaType::type(cl.toUtf8().data());
            if(id != 0){
                void* vp = QMetaType::create(id);
                c  = static_cast<Cell*>(vp);

            }

            if(c){
                c->fromJson(o);
                c->setLibCell(true);
                c->setLibPath(obj["libpath"].toString());
                this->add(c);
                Cell::addCell(c);
                _cell_names.append(c->name());
                auto ckt = c->subckt();
                if(ckt){
                    ckt->addSubckt();
                }
                c->updateBoundingRect();

            }else{
                console->comment("Unknown cell " + cl,ConsoleOutput::red);
            }
        }

    }


    QJsonObject Design::toJson(){
        QJsonObject o;

        QJsonArray ar;

        for(int i=0;i<_cell_names.count();i++){

            Cell * c = Cell::getCell(_cell_names[i]);
            if(c){
                c->setPrefix(this->prefix_);
                if(topcells_.count() > 0 and !c->isUsed()){
                    //Skip if cell is not used and topcell has been set
                    continue;
                }else{
                    QJsonObject lo = c->toJson();
                    ar.append(lo);
                }
            }
        }
        o["cells"] = ar;
        return o;
    }


    QJsonObject Design::readJson(QString filename){

        QFile file;
        file.setFileName(filename);
        if(!file.exists()){
            console->comment("Could not find file '" + filename + "'",ConsoleOutput::red);
            throw "Die";

            QJsonObject obj;
            return obj;
        }

        QString val;
        QStringList valList;
        QList<int> lineList;


        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            int linecount = 0;
            QTextStream in(&file);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                linecount++;
                if(line.contains(QRegularExpression("^\\s*//"))) continue;
                val.append(line);
                valList.append(line);
                lineList.append(linecount);
            }

            file.close();
        }

        QJsonParseError err;
        QJsonDocument d = QJsonDocument::fromJson(val.toUtf8(),&err);
        if(QJsonParseError::NoError != err.error ){
            readError = true;

            QString verr = val.mid(0,err.offset);
            int charcount =0;
            int line_count = 0;

            foreach(QString s, valList){
                charcount += s.length();
                if(charcount < err.offset){
                    line_count += 1;
                }

            }
            line_count -=3;


            QString error("%1%2%3%4\n%5\n%6\n%7\n%8\n%9");
            console->comment(error.arg("JSON ERROR (line ")
                             .arg(lineList[line_count+2])
                             .arg("): ")
                             .arg(err.errorString())
                             .arg("   " + valList[line_count])
                             .arg("   " + valList[line_count+1])
                             .arg("-> " + valList[line_count+2])
                             .arg("   " + valList[line_count+3])
                             .arg("   " + valList[line_count+4])
                             ,ConsoleOutput::red);
        }
        QJsonObject obj = d.object();
        return obj;

    }

    void Design::readJsonFile(QString filename){

        QJsonObject obj = this->readJson(filename);
        obj["libpath"] = filename.replace(".cic","");
        this->fromJson(obj);
    }


    void Design::writeJsonFile(QString filename,QJsonObject info){

        QFile file;
        file.setFileName(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QJsonObject o = this->toJson();
        o["info"] = info;

        QJsonDocument d(o);
        file.write(d.toJson());
        file.close();
    }






};
