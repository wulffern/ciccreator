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
  out << "\033[0;" << color << "m";
  out.setFieldAlignment(QTextStream::AlignLeft);
  out.setFieldWidth(20);
  out.setPadChar(' ');
}

void ConsoleOutput::endComment(QTextStream &out){
  out.setFieldWidth(1);
  out << "\033[0m" ;
  out << endl;
}

 void ConsoleOutput::commentStartClass(QString cell){
   QTextStream out(stdout);
   out.setFieldWidth(0);
   out << "\033[1;" << green << "m";
   out.setFieldAlignment(QTextStream::AlignLeft);
   out.setFieldWidth(20);
   out.setPadChar(' ');
   out <<  cell;
   out.setFieldWidth(1);
    out << "\033[0m" ;
   out << endl;

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
  out <<  cell << theme << property;
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
