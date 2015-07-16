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

    Subckt::Subckt(){

    }



    Subckt::Subckt(QList<QString> buffer){

    }

    Subckt::~Subckt(){

    }

    Subckt::Subckt(const Subckt&){

    }

    void Subckt::parse(QList<QString> subckt_buffer,int line){

      this->setLineNumber(line);
      //Get first subckt line, and remove last subckt line (.ends)
      QString firstLine = subckt_buffer.first();

      //Get subckt name
      QRegularExpression re_subckt_name("^\\s*.subckt\\s+(\\S+)");
      re_subckt_name.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
      QRegularExpressionMatch m_subckt_name = re_subckt_name.match(firstLine);
      if(m_subckt_name.hasMatch()){
         this->setName(m_subckt_name.captured(1));
        //  qWarning() << "Parsing " << this->name();
        }else{
          qWarning() << "Could not parse subcktname on line" << this->lineNumber() << ": " << firstLine;
        }

      //TODO: figure something out to make ports, should probably be an object, maybe with storage for rectangles, and connections
      // to instances. Things not connected to port should be added as an wire object.

      QRegularExpression re_nodes("\\s*(\\S+)+");

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
        if(this->_instances.contains(inst->name())){
           qWarning() << "Error: " << this->name() << " already contains an " << inst->name();
          }
        this->_instances[inst->name()] = inst;
        instance_line_number++;
        }

    }

}
