//#define OPEN_DEFAULT_FILE


#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QWidget>
#include <QHBoxLayout>


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{

  ui->setupUi(this);

  QHBoxLayout *layout = new QHBoxLayout;
  view = new CaptureView();
  layout->addWidget(view);
  
  QWidget *widget = new QWidget;
  widget->setLayout(layout);
  
  setCentralWidget(widget);
     
  //  QGraphicsView * view = ui->view;

  setWindowTitle(tr("cSchematic"));
  setUnifiedTitleAndToolBarOnMac(true);
  setupMenuBar();

}

void MainWindow::actionTriggered(QAction *action)
{
  qDebug("action '%s' triggered", action->text().toLocal8Bit().data());
}

void MainWindow::setupMenuBar()
{
  fileMenu = menuBar()->addMenu(tr("&File"));

QAction*  action = fileMenu->addAction(tr("New"));
  action->setShortcut(tr("Ctrl+N","New"));
  connect(action, SIGNAL(triggered()), this, SLOT(newFile()));


  action = fileMenu->addAction(tr("Save"));
  action->setShortcut(tr("Ctrl+S","Save"));
  connect(action, SIGNAL(triggered()), this, SLOT(saveFile()));

  action = fileMenu->addAction(tr("Open"));
  action->setShortcut(tr("Ctrl+O","Open"));
  connect(action, SIGNAL(triggered()), this, SLOT(openFile()));


  fileMenu->addSeparator();
  action = fileMenu->addAction(tr("&Quit"), this, SLOT(close()));
  action->setShortcut(tr("Ctrl+W"));

  editMenu = menuBar()->addMenu(tr("Edit"));

  QUndoStack * m_undoStack = view->scene()->undoStack();
  QAction* undoAction = m_undoStack->createUndoAction( this );
  QAction* redoAction = m_undoStack->createRedoAction( this );
  undoAction->setShortcut( QKeySequence::Undo );
  redoAction->setShortcut( QKeySequence::Redo );
  editMenu->addAction( undoAction );
  editMenu->addAction( redoAction );

  viewMenu = menuBar()->addMenu(tr("View"));

  action = viewMenu->addAction(tr("Zoom in"));
  action->setShortcut(tr("+","Zoom in"));
  connect(action, SIGNAL(triggered()), view, SLOT(zoomIn()));

  action = viewMenu->addAction(tr("Zoom out"));
  action->setShortcut(tr("-","Zoom out"));
  connect(action, SIGNAL(triggered()), view, SLOT(zoomOut()));


       //Scene actions
         action = editMenu->addAction(tr("Line"));
         action->setShortcut(tr("Ctrl+L","Add Line"));
         connect(action, SIGNAL(triggered()),this,SLOT(setSceneModeAddCommentLine()));

         action = editMenu->addAction(tr("Box"));
         action->setShortcut(tr("Ctrl+R","Add Box"));
         connect(action, SIGNAL(triggered()),this,SLOT(setSceneModeAddCommentBox()));

         action = editMenu->addAction(tr("Text"));
         action->setShortcut(tr("Ctrl+T","Add Text"));
         connect(action, SIGNAL(triggered()),this,SLOT(setSceneModeAddCommentText()));
         action = editMenu->addAction(tr("Net"));
         action->setShortcut(tr("Ctrl+W","Add Net"));
         connect(action, SIGNAL(triggered()),this,SLOT(setSceneModeAddNet()));

         action = editMenu->addAction(tr("Symbol"));
         action->setShortcut(tr("Ctrl+I","Add Symbol"));
         connect(action, SIGNAL(triggered()),this,SLOT(setSceneModeAddSymbol()));

         action = editMenu->addAction(tr("Default"));
         action->setShortcut(tr("Esc","Default Mode"));
         connect(action, SIGNAL(triggered()),this,SLOT(setSceneModeDefaultMode()));
  
}

void MainWindow::newFile(){
  view->scene()->undoStack()->clear();
  view->scene()->clear();
}

void MainWindow::saveFile(){
   gEdaFileWriter gdw;
   QString outfile("/Users/wulff/test.sch");
//   gdw.writeFile(outfile,view->scene()->dataSource());
}

void MainWindow::openFile(){
  //      QString str = "/Users/wulff/svnwork/wulff/trunk/research/cSchematic/example/sym/analog/quad_opamp-1.sym";
  // QString str = "/Users/wulff/svnwork/wulff/trunk/research/cSchematic/example/pmos.sym";
  //    QString str = "/Users/wulff/svnwork/wulff/trunk/research/cSchematic/example/test1.sch";
  //  QString str = "/Users/wulff/svnwork/wulff/trunk/research/cSchematic/example/sept.sch";
  //     QString str = "/Users/wulff/test.sch";
  //    QString str = "/Users/wulff/svnwork/wulff/trunk/research/cSchematic/example/nsfscopebuffer.sch";
#ifndef OPEN_DEFAULT_FILE
 QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 "",
                                                 tr("cSchematic files (*.sch *.sym)"));
#else
 QString fileName = "/Users/wulff/svnwork/wulff/trunk/research/cSchematic/example/sept.sch";
#endif
 
 view->openFile(fileName);
}

void MainWindow::setSceneModeAddCommentLine(){
  view->scene()->setSceneMode(CaptureScene::AddCommentLine);
}

void MainWindow::setSceneModeAddCommentBox(){
  view->scene()->setSceneMode(CaptureScene::AddCommentBox);
}

void MainWindow::setSceneModeDefaultMode(){
  view->scene()->setSceneMode(CaptureScene::DefaultMode);
}

void MainWindow::setSceneModeAddCommentText(){
  view->scene()->setSceneMode(CaptureScene::AddCommentText);
}

void MainWindow::setSceneModeAddSymbol(){
  view->scene()->setSceneMode(CaptureScene::AddSymbol);
}

void MainWindow::setSceneModeAddNet(){
  view->scene()->setSceneMode(CaptureScene::AddNet);
}



MainWindow::~MainWindow()
{
  delete ui;
}
