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

#ifndef CONSOLEOUTPUT_H
#define CONSOLEOUTPUT_H

#include <QObject>
#include <QTextStream>

namespace cIcCore{

class ConsoleOutput : public QObject
{
  Q_OBJECT
public:
  explicit ConsoleOutput(QObject *parent = 0);
  ~ConsoleOutput();

  enum AnsiColor{ black = 30, red = 31, green = 32, brown = 33, blue = 34, magenta = 35, cyan = 36, lightgray = 37};

  void commentInvokeMethod(QString cell, QString theme, QString method);
  void commentStartClass(QString cell);
  void comment(QString cell, AnsiColor color = green);
    void error(QString str);
  void commentSetProperty(QString cell, QString theme, QString property);
  void errorMethodNotFound(QString cell, QString theme, QString method);
  void increaseIndent();
  void decreaseIndent();
  void startComment(QTextStream &out, AnsiColor color = green);
  void endComment(QTextStream &out);
	
  private:
  int indent;


signals:

public slots:
};

}

#endif // CONSOLEOUTPUT_H
