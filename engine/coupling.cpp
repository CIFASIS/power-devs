/****************************************************************************
**
**  Copyright (C) 2009 Facultad de Ciencia Exactas Ingeniería y Agrimensura
**		       Universidad Nacional de Rosario - Argentina.
**  Contact: PowerDEVS Information (kofman@fceia.unr.edu.ar, fbergero@fceia.unr.edu.ar)
**
**  This file is part of PowerDEVS.
**
**  PowerDEVS is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  PowerDEVS is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with PowerDEVS.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include <cmath>
#include "coupling.h"
#include "event.h"
#include "engine.h"
#include "pdevslib.h"

using namespace std;

Coupling::Coupling(const char *name):Simulator(name)
{

}

Coupling::Coupling()
{

};
Coupling::~Coupling()
{
	for(unsigned int i=0;i<Dsize;i++) 
		delete D[i];
	delete []D;

	for(unsigned int i=0;i<ICsize;i++) 
		delete IC[i];
	delete []IC;

	for(unsigned int i=0;i<EICsize;i++) 
		delete EIC[i];
	delete []EIC;

	for(unsigned int i=0;i<EOCsize;i++) 
		delete EOC[i];
	delete []EOC;
}

void Coupling::init(Time t, ...){
	for (unsigned int i=0;i<Dsize;i++)
		childs.push_back(D[i]);
	IC_Index.clear();
	IC_Index.resize(Dsize+1);
	for (unsigned int i=0;i<ICsize;i++) 
		IC_Index[IC[i]->childSource].push_back(i);
	for(unsigned int i=0;i<childs.size();i++) 
		childs[i]->imessage(this, i, t);
	heap.create(D,Dsize);
};

void Coupling::exit(){
	for(unsigned int i=0;i<childs.size();i++)
		childs[i]->exit();
};


Event Coupling::lambda(Time t){
	DEBUG(LAMBDA, this, "[%s] Emiting Output function of child %s at %g\n",name,childs[transitionChild]->name,t);
	output=childs[transitionChild]->lambdamessage(t);
	if (output.isNotNull()){
		propagate(output,t);
	};
	Event ret;
	ret.setNullEvent();
	if (output.isInterrupted()) {
		ret.setInterrupted();
	}
	return ret;
};

void Coupling::dint(Time t){
	DEBUG(DINT, this, "[%s] Execution Internal Transition of child %s at %g\n",name,childs[transitionChild]->name,t);
	childs[transitionChild]->dintmessage(t);
	heap.update(transitionChild);
};

void Coupling::dext(Event x, Time t){
	Port port=x.port;
	for(unsigned int i=0;i<EICsize;i++){
		if (EIC[i]->sourcePort==port){
			DEBUG(DEXT, this,"[%s] Forwarding Event from port %d to %s at %g\n",name,port,childs[transitionChild]->name,t);
			x.port=EIC[i]->sinkPort;
			childs[EIC[i]->childSink]->dextmessage(x, t);
			heap.update(EIC[i]->childSink);
		};
	};
};

double Coupling::ta(Time t){
  if (Dsize==0) // No childs
    return 10e20;
	transitionChild=heap.next();
	tn=D[transitionChild]->tn;
  /*
	tn=10e50;
	for(i=0;i<Dsize;i++){
		DEBUG(TA,this,"[%s] Child %s has tn=%g at %g\n",name,D[i]->name,D[i]->tn,t);
		if ((D[i]->tn)<tn){
			tn=D[i]->tn;
			transitionChild=i;
		};
	};
	if (transitionChild!=heap.next())
	{
		heap.dump();
	}
	if (transitionChild<0)
	{
		tn=10e50;
		return tn-t;
	}
	tn=D[transitionChild]->tn;
  */
	DEBUG(TA,this,"[%s] Next event is at %g from child %s at %g\n",name,tn,D[transitionChild]->name,t);
	return tn-t;
};

void Coupling::setup(Simulator** Da, int Ds, Connection** ICa, int ICs, Connection** EICa, int EICs, Connection** EOCa, int EOCs){
	D=Da;
	Dsize=Ds;
	IC=ICa;
	ICsize=ICs;
	EIC=EICa;
	EICsize=EICs;
	EOC=EOCa;
	EOCsize=EOCs;
};

void Coupling::setup(Simulator** Da, int Ds, Connection** ICa, int ICs){
	D=Da;
	Dsize=Ds;
	IC=ICa;
	ICsize=ICs;
	EIC=NULL;
	EICsize=0;
	EOC=NULL;
	EOCsize=0;
};

void Coupling::propagate(Event x, Time t){
	output=x;
	Port p=output.port;
	unsigned int i;
	if(x.isNotNull()){
		for(i=0;i<IC_Index[transitionChild].size();i++){
			int ic=IC_Index[transitionChild][i];
			if (IC[ic]->sourcePort==p){
				output.port=IC[ic]->sinkPort;
				childs[IC[ic]->childSink]->dextmessage(output, t);
				heap.update(IC[ic]->childSink);
				DEBUG(PROP,this,"[%s] Forwarding Event from %s(%d) to %s(%d) at %g\n",
					name,childs[transitionChild]->name,p,childs[IC[ic]->childSink]->name,IC[ic]->sinkPort,t);
			};
		};
		for(i=0;i<EOCsize;i++){
			if (EOC[i]->childSource==transitionChild && EOC[i]->sourcePort==p){
				output.port=EOC[i]->sinkPort;
				father->propagate(output, t);
			};
		};
	};
}

void Coupling::changemytn(Time  tnext, unsigned int child){
	DEBUG(TA,this,"[%s] Changing tn (if necesary) of %s to %g\n",name,childs[child]->name,childs[child]->tn);
	if (tn>tnext || child==transitionChild || (tn==tnext && child<transitionChild)){
		ta(0);
		DEBUG(TA,this,"[%s] Changing tn  of %s to %g\n",name,childs[child]->name,childs[child]->tn);
		if (father!=NULL)
			father->changemytn(tnext, myself);
	};
	heap.update(child);
}

void ChildHeap::create(Simulator **Ds, unsigned int s)
{
	D=Ds;
	size=s;
	unsigned log_size = ((int)ceil(log2(size)))+1;
  if (size==0)
    log_size=0;
	heap.resize(log_size);
	for (unsigned int i=0; i<log_size; i++)
	{
		if (i+1!=log_size)
			s+=(s % 2);
		heap[i].resize(s);
		s/=2;
	}	
	for (unsigned int i=0; i<log_size; i++) {
		heap[i][heap[i].size()-1].tn = 1e100;
	}
	for (unsigned int i=0; i<size; i++) {
		heap[0][i].tn = D[i]->tn;
		heap[0][i].model = i;
	}
	for (unsigned int i=0; log_size>0 && i<log_size-1; i++) {
		for (unsigned int j=0; j<heap[i].size(); j+=2) {
			if (heap[i][j].tn < heap[i][j+1].tn)
			{
				heap[i+1][j/2] = heap[i][j];
			} else if (heap[i][j].tn > heap[i][j+1].tn) {
				heap[i+1][j/2] = heap[i][j+1];
			} else { 
				if (heap[i][j].model<heap[i][j+1].model) {
					heap[i+1][j/2] = heap[i][j];
				} else {
					heap[i+1][j/2] = heap[i][j+1];
				}
			}
		}

	}
}

void ChildHeap::update(unsigned int child)
{
	heap[0][child].tn = D[child]->tn;
	unsigned level=0,p = child,o;
	bool b = true;
	while (b && level<heap.size()-1)
	{
		b=false;
		o=(p % 2 == 0 ? p+1 : p-1);
		if (heap[level][p].tn < heap[level][o].tn)
		{

			heap[level+1][p/2] = heap[level][p];
			b = true;
		} else if (heap[level][p].tn == heap[level][o].tn) {
			int mn = p<o ? p : o;
			heap[level+1][p/2] = heap[level][mn];
			b = true;
		}
		else if (heap[level][o].model!=heap[level+1][p/2].model) {
			heap[level+1][p/2] = heap[level][o];
			b = true;
		}
		level++;
		p/=2;
	}
}

void ChildHeap::dump()
{
	unsigned log_size = ((int)ceil(log2(size)))+1;
	for (unsigned int i=0;i<log_size;i++) {
		for (unsigned int j=0;j<heap[i].size();j++)
		{
			printf("(%d,%g) ",heap[i][j].model, heap[i][j].tn);
		}
		printf("\n");
	}
}
int ChildHeap::next()
{
	if (heap.size())
		return heap[heap.size()-1][0].model;
	return -1;
}
