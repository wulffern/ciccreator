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

#ifndef CIC_DECORATOR_CONNECTSD_H
#define CIC_DECORATOR_CONNECTSD_H

#include "decorator/layoutcelldecorator.h"
#include "core/patterntransistor.h"

namespace cIcCore{

    /*! Decorator to find all transistors of same group, and connect
     *  the source to the drain. It serves a similar purpose as
     *  overlapping source and drain, like what is usually done in i.e
     *  NAND cell
     */
    class ConnectSourceDrain : public LayoutCellDecorator
    {
        Q_OBJECT

    public:
        ConnectSourceDrain(){};
        ~ConnectSourceDrain(){};
        ConnectSourceDrain(const ConnectSourceDrain&){};
        virtual void parseOptions();
        virtual void beforeRoute();

        QString layer;
        QString routeType;
        QString options;
        
        
    };
    

};


Q_DECLARE_METATYPE(cIcCore::ConnectSourceDrain)

#endif
