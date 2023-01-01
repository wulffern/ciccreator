//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2015-8-20
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
#include "core/consoleoutput.h"

namespace cIcCore{

ConsoleOutput::ConsoleOutput(QObject *parent) : QObject(parent)
{
  //output = new QTextStream(stdout);
  indent = 0;

}

ConsoleOutput::~ConsoleOutput()
{

}

void ConsoleOutput::startComment(QTextStream &out,AnsiColor color){
  out.setFieldWidth(indent);
  out.setFieldAlignment(QTextStream::AlignRight);
  out << " ";
  out.setFieldWidth(0);
#ifndef _WIN32
  out << "\033[0;" << color << "m";
#endif
  out.setFieldAlignment(QTextStream::AlignLeft);
  out.setFieldWidth(20);
  out.setPadChar(' ');
}

void ConsoleOutput::endComment(QTextStream &out){
  out.setFieldWidth(1);
#ifndef _WIN32
  out << "\033[0m" ;
#endif
  out << Qt::endl;
}

 void ConsoleOutput::commentStartClass(QString cell){
   QTextStream out(stdout);
   out.setFieldWidth(0);
#ifndef _WIN32
   out << "\033[1;" << green << "m";
#endif
   out.setFieldAlignment(QTextStream::AlignLeft);
   out.setFieldWidth(20);
   out.setPadChar(' ');
   out <<  cell;
   out.setFieldWidth(1);
#ifndef _WIN32
    out << "\033[0m" ;
#endif
   out << Qt::endl;

 }

 void ConsoleOutput::comment(QString str, AnsiColor color){
   QTextStream out(stdout);
   out.setFieldWidth(indent);
   out.setFieldAlignment(QTextStream::AlignRight);
   out << " ";
   out.setFieldWidth(0);
#ifndef _WIN32
   out << "\033[0;" << color << "m";
#endif
   out.setFieldAlignment(QTextStream::AlignLeft);
   out.setFieldWidth(0);
   out.setPadChar(' ');
   out <<  str;
   this->endComment(out);

 }

    void ConsoleOutput::error(QString str)
    {

        int indent_old = indent;

        indent = 0;
        
        this->comment("ERROR: " + str,ConsoleOutput::red);

        indent = indent_old;

    }
    

void ConsoleOutput::commentInvokeMethod(QString cell, QString theme, QString method){
  QTextStream out(stdout);
  this->startComment(out);
  out <<  cell << theme << method;
  this->endComment(out);

}

void ConsoleOutput::commentSetProperty(QString cell, QString theme, QString property){
  QTextStream out(stdout);
  this->startComment(out);
  out <<  cell << theme << ("Property: " + property);
  this->endComment(out);
}

void ConsoleOutput::errorMethodNotFound(QString cell, QString theme, QString method){
  QTextStream out(stdout);
  this->startComment(out,red);
  out <<  cell << theme << method;
  this->endComment(out);
}



void ConsoleOutput::increaseIndent(){
  indent++;
    indent++;

}


void ConsoleOutput::decreaseIndent(){
  indent--;
    indent--;
     QTextStream out(stdout);
   this->endComment(out);

}

}
