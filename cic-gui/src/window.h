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

#ifndef CIC_GUI_WINDOW_H
#define CIC_GUI_WINDOW_H

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QListWidget>
#include <QSlider>
#include <QGridLayout>
#include <QSplitter>
#include <QScrollArea>
#include <QShortcut>
#include "cic-core.h"
#include "renderarea.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QSlider;
QT_END_NAMESPACE


namespace cIcGui{
    using namespace cIcCore;
    //using cIcCore;

    class Window : public QWidget
    {
        Q_OBJECT
    public:
        explicit Window(QWidget *parent = 0);
        void loadDesign(Design *d);
        void loadFile(QString f);
        ~Window();

    signals:

    public slots:
        void shapeSelected(int index);
        void zoomChanged(int zoom);
        void layerClicked(QModelIndex index);
        void reloadFile();
		void saveRenderImage();

    protected:
        bool eventFilter(QObject *obj, QEvent *event);
    private:

        RenderArea *originalRenderArea;
        QScrollArea *scroll;
        QComboBox *shapeComboBox;
        QListWidget * listCells;
        QListWidget * listLayers;
        QSlider *zoom;
        Design * designs;
        QSplitter * splitter;
        QString filename;
        QWidget * leftside;
        QPushButton *saveImage;
        QShortcut *shift_z;
        QShortcut *ctrl_z;
        QShortcut *shift_r;


    };

}

#endif // WINDOW_H
