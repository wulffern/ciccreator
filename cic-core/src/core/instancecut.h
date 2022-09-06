//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-7-14
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
#ifndef CIC_CORE_INSTANCECUT_H
#define CIC_CORE_INSTANCECUT_H

#include "instance.h"


namespace cIcCore{

  class InstanceCut : public Instance
  {

    Q_OBJECT

  public:
      InstanceCut():Instance(){};
      explicit InstanceCut(const Instance& inst):Instance(inst){};
      ~InstanceCut(){};
  };

}

Q_DECLARE_METATYPE(cIcCore::InstanceCut)

#endif // INSTANCECUT_H
