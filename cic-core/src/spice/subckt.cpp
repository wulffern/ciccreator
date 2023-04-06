//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-7-6
// ===================================================================
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
//====================================================================
#include "spice/subckt.h"

namespace cIcSpice{

    QMap<QString,Subckt*> Subckt::_allsubckt;

    Subckt::Subckt(){

    }

    Subckt::Subckt(QList<QString> buffer){

    }

    Subckt::~Subckt(){

    }

    Subckt::Subckt(const Subckt&){

    }

    SubcktInstance* Subckt::getInstance(QString name){

        foreach(SubcktInstance* i, _instances){
            //cout << i->name();
            if(i->name() == name){
                return i;
            }
        }
    }



    Subckt* Subckt::getInstanceSubckt(SubcktInstance* inst){
        Subckt * ckt = NULL;
        if(_allsubckt.contains(inst->subcktName())){
            ckt = _allsubckt[inst->subcktName()];
        }
        return ckt;
    }

    void Subckt::fromJson(QJsonObject o)
    {
        SpiceObject::fromJson(o);

        this->setName(o["name"].toString());
        QJsonArray insts = o["instances"].toArray();
        foreach(auto i, insts){
            SubcktInstance * is = new SubcktInstance();
            is->fromJson(i.toObject());
            this->add(is);

        }
        QJsonArray dev = o["devices"].toArray();
        foreach(auto d, dev){
            auto od = d.toObject();
            //TODO: Parse devices
            if(od["class"] == "cIcSpice::Mosfet"){
                Mosfet * m = new Mosfet();
                m->fromJson(od);
                this->add(m);
            }else if(od["class"] == "cIcSpice::Resistor"){
                Resistor * r = new Resistor();
                r->fromJson(od);
                this->add(r);
            }else{
                qDebug() << "Unknown device class " << o["class"];
            }
        }

        QJsonArray prop = o["properties"].toArray();
        foreach(auto p, prop){

            //TODO parse properties
        }

    }


    QJsonObject Subckt::toJson()
    {
        QJsonObject o = SpiceObject::toJson();
        QJsonArray ar;
        foreach(SubcktInstance* i, _instances){
            i->setPrefix(this->prefix_);
            QJsonObject oi = i->toJson();
            ar.append(oi);
        }

        QJsonArray ar1;
        foreach(SpiceDevice* i, _devices){
            i->setPrefix(this->prefix_);
            QJsonObject oi = i->toJson();
            ar1.append(oi);
        }


        o.remove("deviceName");

        o["name"] = this->prefix_ + name();
        o["instances"] = ar;
        o["devices"] = ar1;
        return o;

    }


    void Subckt::parse(QList<QString> subckt_buffer,int line){
        _spice_str = subckt_buffer;
        this->setLineNumber(line);

        //Get first subckt line, and remove last subckt line (.ends)
        QString firstLine = subckt_buffer.first();

        //Get subckt name
        QRegularExpression re_subckt_name("^\\s*.subckt\\s+(\\S+)");
        re_subckt_name.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch m_subckt_name = re_subckt_name.match(firstLine);
        if(m_subckt_name.hasMatch()){
            this->setName(m_subckt_name.captured(1));
            firstLine.replace(re_subckt_name,"");
        }else{
            qWarning() << "Could not parse subcktname on line" << this->lineNumber() << ": " << firstLine;
        }

        _allsubckt[this->name()] = this;

        firstLine = firstLine.trimmed();

        //Remove parameters
        QRegularExpression re_params("\\s+(\\S+)\\s*=\\s*(\\S+)");
        QRegularExpressionMatchIterator it= re_params.globalMatch(firstLine);
        while (it.hasNext()) {

            QRegularExpressionMatch m_params = it.next();

            //- TODO do I want to use parameters for anything on subckt??
            //_properties[m_params.captured(1)] = m_params.captured(2);
        }


        firstLine.replace(re_params,"");


        //Split on space
        const QRegularExpression re_space("\\s+");
        QStringList nodes = firstLine.split(re_space);
        this->setNodes(nodes);

        subckt_buffer.removeFirst();
        subckt_buffer.removeLast();

        int instance_line_number = line;
        QRegularExpression re_ignore("(^\\s*$)|(^\\s*\\*)");
        foreach(QString line, subckt_buffer){
            QRegularExpressionMatch m_ignore = re_ignore.match(line);
            if(m_ignore.hasMatch())
                continue;

            SubcktInstance * inst = new SubcktInstance();



            inst->parse(line,instance_line_number);
            if(this->_inst_index.contains(inst->name())){
                qWarning() << "Error: " << this->name() << " already contains an " << inst->name();
            }
            this->_instances.append(inst);
            this->_inst_index[inst->name()] = this->_instances.count() -1;

            // Make paralell devices
            if(inst->properties().contains("M")){
                int count  = inst->properties()["M"].toInt();
                QString name = inst->name();
                inst->setName(inst->name() + "0");
                for(int i=1;i<count;i++){
                    SubcktInstance * inst_mult = new SubcktInstance();
                    inst_mult->parse(line,instance_line_number);
                    inst_mult->setName(QString("%1%2").arg(name).arg(i));

                    if(this->_inst_index.contains(inst_mult->name())){
                        qWarning() << "Error: " << this->name() << " already contains an " << inst_mult->name();
                    }
                    this->_instances.append(inst_mult);
                    this->_inst_index[inst_mult->name()] = this->_instances.count() -1;
                }
            }

            //Make series devices. Only works for mosfets with
            // D G S B nodes. Assume that bulk and gate is shared.
            if(inst->properties().contains("S")){

                QList<QString> nodes = inst->nodes();
                //check for 4 nodes, and maybe mos status
                if(nodes.count() != 4){
                    qDebug() << "Error: Series parameter (S) can only be used for devices with 4 nodes";
                }else{

                    QString top_node = nodes[0];
                    //fix first instance nodes
                    nodes[0] += "_S0";
                    inst->setNodes(nodes);
                    QString prev_node = nodes[0];

                    //Make series instances
                    int count  = inst->properties()["S"].toInt();
                    QString name = inst->name();
                    inst->setName(inst->name() + "_S0");
                    SubcktInstance * inst_ser = 0;
                    for(int i=1;i<count;i++){
                        inst_ser = new SubcktInstance();
                        inst_ser->parse(line,instance_line_number);
                        inst_ser->setName(QString("%1_S%2").arg(name).arg(i));
                        auto inodes = inst_ser->nodes();

                        inodes[2] = prev_node;
                        inodes[0] = QString("%1_S%2").arg(inodes[0]).arg(i);
                        inst_ser->setNodes(inodes);
                        prev_node= inodes[0];

                        if(this->_inst_index.contains(inst_ser->name())){
                            qWarning() << "Error: " << this->name() << " already contains an " << inst_ser->name();
                        }
                        this->_instances.append(inst_ser);
                        this->_inst_index[inst_ser->name()] = this->_instances.count() -1;
                    }
                    if(inst_ser){
                        auto inodes = inst_ser->nodes();
                        inodes[0] = top_node;
                        inst_ser->setNodes(inodes);
                    }
                }
            }

            instance_line_number++;
        }
    }
}
