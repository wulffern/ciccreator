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
#include "core/instance.h"

namespace cIcCore{

  Instance::Instance():Cell()
  {
    _cell = 0;
  }

  Instance::Instance(const Instance& inst): Cell(inst)
  {
    _cell = 0;
  }


  Instance::~Instance()
  {

  }

  void Instance::setSubcktInstance(cIcSpice::SubcktInstance *inst){
	  ckt_inst_ = inst;
	  
	  _ports.clear();
	  cIcSpice::Subckt* ckt = cIcSpice::Subckt::getInstanceSubckt(inst);

	  //Add ports, but don't proceed if the subcircuit cannot be found
	  QStringList cktNodes;
	  Cell* cell = this->cell();

	  if(cell == NULL){ qWarning() << "Error: Could find cell" << inst->subcktName(); return;}

	  cIcSpice::SpiceObject * sobj = NULL;
	  if(cell){ sobj= cell->spiceObject();}

	  if(ckt){
	      cktNodes = ckt->nodes();
	    }else if(sobj){
	      cktNodes = sobj->nodes();
	    }else{
		 qWarning() << "Subckt " << inst->subcktName() << " not found";
	     return;
	    }

	  QStringList instNodes = inst->nodes();
	  
	  if(instNodes.count() != cktNodes.count()){
	      qWarning() << "Error: different number of nodes for " << inst->name() << "(" << instNodes.count() << ")" << " and " << inst->subcktName() << "(" << cktNodes.count() << ")";

	    }


	  for(int i=0;i<cktNodes.count();i++){
		  QString instNode = instNodes[i];
		  QString cktNode= cktNodes[i];
		  Port * instPort = new Port(instNode);
		  Port * cellPort = cell->getPort(cktNode);
		  if(cellPort){
		    instPort->setChild(cellPort,this);
		    this->add(instPort);
	    }
	  }
  }

  Rect Instance::calcBoundingRect(){
    Rect r = this->_cell->calcBoundingRect();
    //		r.moveTo(this->x1()+r.x1(), this->y1() + r.y1());
    r.moveTo(this->x(), this->y());

    return r;
  }

  Instance * Instance::getInstance(QString cell){

    Instance * c = new Instance();

    if(Cell::_allcells.contains(cell)){
        c->_cell  = _allcells[cell];
        c->setName(c->_cell->name());
        Rect r = c->_cell->calcBoundingRect();
        c->setLayer("PR");
        c->updateBoundingRect();
      }
    return c;
  }



}
