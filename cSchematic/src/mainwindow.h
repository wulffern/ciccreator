#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define OPEN_DEFAULT_FILE

#include <QMainWindow>
#include "schematic/captureview.h"
#include "geda/gEdaFileReader.h"
#include "geda/gEdaFileWriter.h"
#include <QWidget>
#include <QHBoxLayout>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QMenu * editMenu;
    QMenu * fileMenu;
    QMenu * viewMenu;
    CaptureView * view;
    QString currentFilename;
    

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	
    void actionTriggered(QAction *action);
    void setupMenuBar();

    public slots:
    //Actions
    void newFile();
    void saveFile();
    void openFile();
    void setSceneModeAddCommentLine();
    void setSceneModeAddCommentBox();
    void setSceneModeAddCommentText();
    void setSceneModeAddSymbol();
    void setSceneModeDefaultMode();
    void setSceneModeAddNet();
};

#endif // MAINWINDOW_H
