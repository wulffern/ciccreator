//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-4-6
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

#include "core/rules.h"

namespace cIcCore{

    Rules * Rules::myRules_;

    Rules::Rules(){
        gamma_ = 50;
        grid_ = 5;
        spiceunit_ = 1e-6;

    }

    Rules::Rules(const Rules&){

    }

    double Rules::spiceUnit(){
        return spiceunit_;
    }


    void Rules::loadRules(QString filename){

        ConsoleOutput* console = new ConsoleOutput();
        QString val;
        QFile file;


        
        file.setFileName(filename);


        if(!file.exists()){
            console->error("Can't find file '" + filename + "'" ) ;
            throw "Die";
            
        }
        
        
        file.open(QIODevice::ReadOnly | QIODevice::Text);
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

            console->error("Error[" + filename + "]:" +  err.errorString() + " at line " + QString("%1").arg(line_count)) ;
            throw "Die";
        }

        QJsonObject obj = d.object();
        Rules::myRules_ = new Rules();
        Rules::myRules_->setRules(obj);

    }

    Device * Rules::getDevice(QString dev){
        if(devices_.contains(dev)){
            return devices_[dev];
        }
        return 0;
    }

    double Rules::toMicron(int val){
        double d = ((double)val)/(1.0e4);
        return d;
    }

    Layer * Rules::getLayer(QString name){
        if(this->layers_.contains(name)){

            Layer * l = this->layers_[name];
            return l;
        }else{
            return this->layers_["PR"];
        }
    }

    bool Rules::isLayerBeforeLayer(QString layer1, QString layer2){
        QString previousLayer  = this->getPreviousLayer(layer2);

        if(previousLayer== ""){
            return false;
        }if(previousLayer == layer1){
            return true;
        }else{
            return this->isLayerBeforeLayer(layer1,previousLayer);

        }
    }

    QList<Layer *> Rules::getConnectStack(QString layer1, QString layer2){

        QList<Layer*> stack;
        QString start;
        QString stop;


        if(this->isLayerBeforeLayer(layer1,layer2)){
            start = layer1;
            stop = layer2;
        }else if(this->isLayerBeforeLayer(layer2,layer1)){
            start = layer2;
            stop = layer1;
        }else if(layer1 == "OD"){
            start = layer1;
            stop = layer2;
        }else if(layer2 == "OD"){
            start = layer2;
            stop = layer1;
        } else{
            qDebug() << "No connect rules that tie " << layer1 << " to " << layer2;
            return stack;
        }

        QString current = start;
        int counter = 100;
        while(current != stop){
            stack.append(this->getLayer(current));
            current = this->getNextLayer(current);
            counter--;
            if(counter < 0){
                break;


            }
        }
        stack.append(this->getLayer(stop));

//        qDebug() << stack;


        return stack;
    }


    QString Rules::getNextLayer(QString lay){
        if(layers_.contains(lay)){
            return layers_[lay]->next;
        }else{
            qWarning() << "Error: Could not find next layer for " << lay;
        }
        return "";
    }

    QString Rules::getPreviousLayer(QString lay){
        if(layers_.contains(lay)){
            return layers_[lay]->previous;
        }else{
            qWarning() << "Error: Could not find next layer for " << lay;
        }
        return "";
    }



    void Rules::setRules(QJsonObject job){

        QJsonObject tech = job["technology"].toObject();
        if(tech.contains("gamma"))
            gamma_ = tech["gamma"].toInt();
        if(tech.contains("grid"))
            grid_ = tech["grid"].toInt();
        if(tech.contains("spiceunit"))
            spiceunit_ = tech["spiceunit"].toDouble();
        QJsonObject devices = tech["devices"].toObject();
        foreach(QString key, devices.keys()){
            devices_[key] = new Device();
            QJsonObject dev = devices[key].toObject();
            devices_[key]->name = dev["name"].toString();
            devices_[key]->devicetype = dev["devicetype"].toString();
            QJsonArray ar = dev["ports"].toArray();
            foreach(QJsonValue p, ar){
                devices_[key]->ports.append(p.toString());
            }

        }

        QJsonObject layers = job["layers"].toObject();
        foreach(QString layer, layers.keys()){
            QJsonObject lay = layers[layer].toObject();
            Layer *ln = new Layer();
            ln->name = layer;

            QJsonValue dtv = lay["datatype"];
            if(dtv.isArray()){
                QJsonArray dtva = dtv.toArray();
                foreach ( const QJsonValue & value, dtva){
                    QJsonArray dtvc = value.toArray();
                    QString dts = dtvc[0].toString();
                    int dti = dtvc[1].toInt();
                    if(dts == "default"){
                        ln->datatype = dti;
                    }else{
                        ln->datatypes.insert(dts, dti);
                    }



                }

            }else{
                ln->datatype = lay["datatype"].toInt();
            }




            ln->number = lay["number"].toInt();

            if(lay.contains("material")){
                QString mat = lay["material"].toString();
                if(mat == "diffusion"){
                    ln->material = ln->diffusion;
                }
                else if(mat == "metal"){
                    ln->material = ln->metal;
                }
                else if(mat == "poly"){
                    ln->material = ln->poly;
                }
                else if(mat == "cut"){
                    ln->material = ln->cut;
                }
                else if(mat == "metalres"){
                    ln->material = ln->metalres;
                }
                else if(mat == "marker"){
                    ln->material = Layer::marker;
                }
                else if(mat == "implant"){
                    ln->material = Layer::implant;
                }
                else {
                    ln->material = ln->other;
                }
            }

            if(lay.contains("previous")){
                ln->previous = lay["previous"].toString();
            }
            if(lay.contains("next")){
                ln->next = lay["next"].toString();
            }
            if(lay.contains("pin")){
                ln->pin = lay["pin"].toString();
            }
            if(lay.contains("res")){
                ln->res = lay["res"].toString();

            }
            if(lay.contains("color")){
                ln->color = lay["color"].toString();
            }
            if(lay.contains("fill")){
                QString fill = lay["fill"].toString();
                if(fill.startsWith("nofill")){
                    ln->nofill = true;
                }
            }
            this->layers_[layer] = ln;

        }



        QJsonObject r = job["rules"].toObject();
        foreach(QString layer, r.keys()){
            QJsonObject vral = r[layer].toObject();


            foreach(QString name,vral.keys()){
                qreal v = vral[name].toDouble();
                this->rules_[layer][name] = v;
                //  if(this->rules_.contains(layer)){
                //  QMap<QString,qreal> rl = this->rules_[name];
                //  rl[name] = v;
                // }

                //rules_ = v;
            }
        }

        //}

        // rules_ = job["rules"].toObject();
        // layers_ = job["layers"].toObject();
        // technology_ = job["technology"].toObject();
    }
    QString Rules::removeDataType(QString layer)
    {
        QStringList sl = layer.split("|");
        return sl[0];

    }

    QString Rules::getDataType(QString layer)
    {
        QStringList sl = layer.split("|");
        if(sl.count() > 1){
            return  sl[1];
        }else{
            return "";
        }
    }



    bool Rules::hasRule(QString layer, QString rule){
        QString layerstr = removeDataType(layer);
        if(rules_.contains(layerstr)){
            if(rules_[layerstr].contains(rule)){
                return true;
            }
        }
        return false;
    }

    qreal Rules::get(QString layer, QString rule){
        qreal v = 0;
        QString layerstr = removeDataType(layer);
        if(rules_.contains(layerstr)){
            QMap<QString,qreal> lay =  rules_[layerstr];

            if(!lay.contains(rule)){
                qDebug() << "Could not find rule "<< layerstr << rule ;
            }else{
                v = lay[rule]*gamma_;

            }
        }else{
            qDebug() << "Could not find rule " << rule << " for layer " << layer << ", no such layer";
        }

        return v;
    }

    QString Rules::layerToColor(QString name){
        QString layerstr = removeDataType(name);
        if(this->layers_.contains(layerstr)){
            return this->layers_[layerstr]->color;
        }

        qWarning() << "Error (layerToColor)   : Layer " << layerstr << "not found in rule file";
        return "";
    }

    int Rules::layerToNumber(QString name){
        name = removeDataType(name);
        if(this->layers_.contains(name)){
            return this->layers_[name]->number;
        }

        qWarning() << "Error (layerToNumber)   : Layer " << name << "not found in rule file";
        return 1;
    }

    int Rules::layerToDataType(QString name){
        QString datatype = getDataType(name);

        int dt = 0;

        QString layer = removeDataType(name);
        if(this->layers_.contains(layer)){
            Layer * l = this->layers_[layer];
            if(datatype != ""){
                dt =  l->datatypes[datatype];
            }else{
                dt =  l->datatype;
            }



        }else{
            qWarning() << "Error (layerToDataType) : Layer " << name << "not found in rule file";
        }



        return dt;
    }
}
