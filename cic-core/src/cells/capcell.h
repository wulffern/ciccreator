//====================================================================
//        Copyright (c) 2016 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2016-9-3
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




#ifndef CIC_CELLS_CAPCELL_H
#define CIC_CELLS_CAPCELL_H

#include "core/layoutcell.h"

namespace cIcCells{

    class CapCell: public cIcCore::LayoutCell
    {
        Q_OBJECT

        public:
            virtual void place();
            virtual Rect calcBoundingRect();
            void addContacts(QString name, QString node,int y,QList<int> array, Rect* r);
            Rect* getAvssConnectRect(Rect* rect);

            Q_INVOKABLE
            /**
             * @brief Use Metal 3
             */
            void usem3(QJsonValue obj);

            Q_INVOKABLE
            /**
             * @brief Use Metal 5 shield
             */
            void usem5(QJsonValue obj);

            Q_INVOKABLE
            /**
             * @brief Increase multiplier height
             */
            void heightIncreaseMult(QJsonValue obj);

        private:
            int msw = 0;
            int xorg = 0;
            int yMax = 0;
            bool usem3_ = true;
            bool usem5_ = false;
            int heightIncreaseMult_ = 1;


    };


};
Q_DECLARE_METATYPE(cIcCells::CapCell)

#endif
