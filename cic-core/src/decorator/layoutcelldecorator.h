//====================================================================
//        Copyright (c) 2018 Carsten Wulff Software, Norway 
// ===================================================================
// Created       : wulff at 2018-3-11
// ===================================================================
//  The MIT License (MIT)
// 
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
// 
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
// 
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//  
//====================================================================

#ifndef CIC_DECORATOR_LAYOUTCELL_H
#define CIC_DECORATOR_LAYOUTCELL_H

#include "core/layoutcell.h"

namespace cIcCore{

    class LayoutCellDecorator: public QObject
    {
        Q_OBJECT

    public:
        LayoutCellDecorator();
        LayoutCellDecorator(const LayoutCellDecorator &);
        LayoutCellDecorator(LayoutCell* lc);
        ~LayoutCellDecorator();

        void setOptions(QJsonValue & job)
        {
            jsonval = job;
            parseOptions();
        }
        
        void setCell(LayoutCell* lc)
        {
            layoutcell = lc;

        }
        virtual void parseOptions(){};

        virtual void afterNew(){};
        virtual void beforePlace(){};
        virtual void afterPlace(){};        
        virtual void beforeRoute(){};
        virtual void afterRoute(){};
        virtual void place(){};
        virtual void route(){};
        virtual void paint(){};
        virtual void beforePaint(){};
        virtual void afterPaint(){};
        

    protected:
        LayoutCell * layoutcell;
        QJsonValue  jsonval;
        
    };

}

Q_DECLARE_METATYPE(cIcCore::LayoutCellDecorator)
#endif
