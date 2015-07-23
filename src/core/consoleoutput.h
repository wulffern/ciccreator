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
  void commentSetProperty(QString cell, QString theme, QString property);
  void errorMethodNotFound(QString cell, QString theme, QString method);
  void increaseIndent();
  void decreaseIndent();

  private:
  int indent;
  void startComment(QTextStream &out, AnsiColor color = green);
  void endComment(QTextStream &out);

signals:

public slots:
};

}

#endif // CONSOLEOUTPUT_H
