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

#include "spice/subcktinstance.h"

namespace cIcSpice{

    SubcktInstance::SubcktInstance(){

    }

    SubcktInstance::SubcktInstance(QString buffer){

    }

    SubcktInstance::~SubcktInstance(){

    }

    SubcktInstance::SubcktInstance(const SubcktInstance&){

    }

    void SubcktInstance::parse(QString buffer,int line_number){

        this->setLineNumber(line_number);
        this->spiceStr().append(buffer);

        //Remove parameters
        QRegularExpression re_params("\\s+(\\S+)\\s*=\\s*(\\S+)");
        QRegularExpressionMatchIterator it= re_params.globalMatch(buffer);
        while (it.hasNext()) {
            QRegularExpressionMatch m_params = it.next();
             this->properties()[m_params.captured(1)] = m_params.captured(2);
        }


        buffer.replace(re_params,"");

        //Split on space
        const QRegularExpression re_space("\\s+");
        QStringList nodes = buffer.split(re_space);

        //First element should be instance name
        this->setName(nodes.first());
        nodes.removeFirst();

        QRegularExpression re_group("(\\S+)(\\d\\S+)");
        QRegularExpressionMatch m_group = re_group.match(this->name());
        if(m_group.hasMatch()){
          this->_group_name = m_group.captured(1);
            this->_group_tag = m_group.captured(2);
          }
        //Last element should be subckt name
        this->_subckt_name = nodes.last();
        nodes.removeLast();

        //The rest should be nodes
        this->setNodes(nodes);

    }

}
