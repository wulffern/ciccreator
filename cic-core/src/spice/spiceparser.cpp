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


#include "spice/spiceparser.h"

namespace cIcSpice{

    SpiceParser::SpiceParser(){

    }

    SpiceParser::~SpiceParser(){

    }

    SpiceParser::SpiceParser(const SpiceParser&){

    }


    void SpiceParser::parseSubckt(int line_number, QList<QString> subckt_buffer)
    {
        Subckt * ckt = new Subckt();
        ckt->parse(subckt_buffer,line_number);
        this->_subckt[ckt->name()] = ckt;
    }

    void SpiceParser::parseFile(QString filename){

        QFile file;
        file.setFileName(filename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){

            QTextStream in(&file);
            bool isSubckt = false;

            QRegularExpression re_subckt_start("^\\s*.subckt");
            re_subckt_start.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
            QRegularExpression re_subckt_end("^\\s*.ends");
            re_subckt_end.setPatternOptions(QRegularExpression::CaseInsensitiveOption);


            QRegularExpression re_plus("^\\s*\\+");

            int line_number = 0;
            QList<QString> subckt_buffer;
            while (!in.atEnd())
            {
                QString line = in.readLine();
                line = line.trimmed();
                line_number++;

                //Handle plus
                QRegularExpressionMatch m_hasPlus = re_plus.match(line);
                if(m_hasPlus.hasMatch()){
                    line.replace(re_plus,"");
                    subckt_buffer.last().append(line);
                    continue;
                }

                //Capture start of subckt
                QRegularExpressionMatch m_start = re_subckt_start.match(line);
                if(m_start.hasMatch()){
                    isSubckt = true;
                    subckt_buffer.clear();
                }

                if(isSubckt){
                    subckt_buffer.append(line);
                }

                //Capture end of subckt
                QRegularExpressionMatch m_end = re_subckt_end.match(line);
                if(m_end.hasMatch()){
                    isSubckt = false;
                    parseSubckt(line_number, subckt_buffer);
                }
            }
            file.close();

        }

    }

}
