#include <parser.h>
#include <data/atomic.h>
#include <data/parameter.h>
#include <data/coupled.h>

unsigned int depth=0;

string tab(unsigned int t)
{
  string res="";
  for (unsigned i=0;i<t;i++)
  {
    res = res + "    ";
  }
  return res;
}

void printParameter(std::ostream &out,const Parameter *p);

string typeToString(Line::PointType pt)
{
	switch(pt)
	{
		case Line::PORT:
			return "Prt";
		case Line::NODE:
			return "Pnt";
		case Line::COMPONENT:
			return "Cmp";
		case Line::CROSS:
			return "NoC";
	}
}

std::ostream &operator<<(std::ostream &out, const Line &l)
{
	out << tab(depth) << "Line" << endl;
	out << tab(depth+1) << "{" << endl;
	out << tab(depth+1) << "Source = " << typeToString(l.sourceType()) << " ;  " 
											<< l.sourceAt(0) << " ; "
											<< ( l.sourceAt(1) > 0 ?  " " : "")
											<< l.sourceAt(1) << " ; "
											<< l.sourceAt(2) << endl;
	out << tab(depth+1) << "Sink = " << typeToString(l.sinkType()) << " ;  " 
											<< l.sinkAt(0) << " ; "
											<< ( l.sinkAt(1) > 0 ?  " " : "")
											<< l.sinkAt(1) << " ; "
											<< l.sinkAt(2) << endl;
	out << tab(depth+1) << "PointX = ";
	for(unsigned int i=0;i<l.pointCount();i++) {
		out << l.pointX(i)*TWIPS_TO_PIXEL;
		if (i+1!=l.pointCount())
			out << " ; ";
	}
	out << endl;
	out << tab(depth+1) << "PointY = " ;
	for(unsigned int i=0;i<l.pointCount();i++) {
		out << l.pointY(i)*TWIPS_TO_PIXEL;
		if (i+1!=l.pointCount())
			out << " ; ";
	}
	out << endl;
	out << tab(depth+1) << "}" << endl;
	return out;
}

std::ostream &operator<<(std::ostream &out, const Point &p)
{
	out << tab(depth) << "Point" << endl;
	out << tab(depth+1) << "{" << endl;
	out << tab(depth+1) << "ConectedLines = ";
	for(int i=0;i<p.lineCount();i++) {
		out << p.lineAt(i);
		if (i+1!=p.lineCount())
			out << " ; ";
	}
	out << endl;
	out << tab(depth+1) << "ConectedExtrems = ";
	for(int i=0;i<p.extremsCount();i++) {
		out << (!p.extremsAt(i) ? "Dst" : "Org" );
		if (i+1!=p.extremsCount())
			out << " ; ";
	}
	out << endl;
	out << tab(depth+1) << "Position = " << p.const_graphic().x()*TWIPS_TO_PIXEL << " ; " 
																			 << p.const_graphic().y()*TWIPS_TO_PIXEL << endl;
	out << tab(depth+1) << "}" << endl;
	return out;
}

std::ostream &operator<<(std::ostream &out, const Atomic &a)
{
  unsigned ldepth = depth;
  out << tab(ldepth) << "Atomic" << endl;
  out << tab(ldepth+1) << "{" << endl;
  out << tab(ldepth+1) << "Name = " << a.name() << endl;
  out << tab(ldepth+1) << "Ports = " << a.inPorts() << " ; " << a.outPorts() << endl;
  out << tab(ldepth+1) << "Path = " << a.path() << endl;
  out << tab(ldepth+1) << "Description = " << a.description() << endl;
  depth++;
  out << (a.const_graphic());
  depth--;
  out << tab(ldepth+1) << "Parameters" << endl;
  out << tab(ldepth+2) << "{" << endl;
  depth+=2;
  for (int p=0; p<a.parameterCount();p++) {
    printParameter(out,a.parameter(p));
  }
  depth-=2;
  out << tab(ldepth+2) << "}" << endl;
  vector<string> extra = a.getExtra();
  if (extra.size()) {
    out << tab(ldepth+2) << "Extra" << endl;
    out << tab(ldepth+3) << "{" << endl;
    for (int i=0;i<extra.size();i++)
      out << tab(ldepth+4) << extra[i] << endl;
    out << tab(ldepth+3)<< "}" << endl;
  
  }
 
  out << tab(ldepth+1) << "}" << endl;
  return out;
}

std::istream &operator>>(istream &in, Atomic &a)
{
  a.setName("Test");
  a.graphic().setWidth(40);
  a.graphic().setHeight(40);
  a.graphic().setDirection(Graphic::RIGHT);
  a.setInports(3);
  a.setOutports(3);
  return in;
}


void printParameter(std::ostream &out,const Parameter *p)
{
  if (p->isString()) {
    const StringParameter *sp = dynamic_cast<const StringParameter*>(p);
    out << tab(depth) << sp->name() << " = Str; " << sp->strEscapedValue() << " ; " << sp->description() << "" << endl;
  }
  if (p->isValue()) {
    const ValueParameter *vp = dynamic_cast<const ValueParameter*>(p);
    out << tab(depth) << vp->name() <<  " = Val; " << vp->value() << " ; " << vp->description() << "" << endl;
  }
  if (p->isList()) {
    const ListParameter *lp = dynamic_cast<const ListParameter*>(p);
    out << tab(depth) << lp->name() << " = Lst; " << lp->selected() << "\%";
    vector<string> l = lp->list();
    vector<string>::iterator i=l.begin();
    for(;i!=l.end();i++)
      out << "" << i->c_str() << "\%" ;
    out << " ; " << lp->description() << "" << endl;
  }
}

ostream &operator<<(ostream &out, const Graphic g)
{
  out << tab(depth) << "Graphic" << endl;
  out << tab(depth+1) << "{" << endl ; 
  out << tab(depth+1) << "Position = " << (int)(g.x()*TWIPS_TO_PIXEL) << " ; " << (int)(g.y()*TWIPS_TO_PIXEL) << endl ; 
  out << tab(depth+1) << "Dimension = " << (int)(g.width()*TWIPS_TO_PIXEL) << " ; " << (int)(g.height()*TWIPS_TO_PIXEL) << endl ; 
  out << tab(depth+1) << "Direction = " << g.directionWord() << endl; 
  out << tab(depth+1) << "Color = " << g.color() << endl; 
  out << tab(depth+1) << "Icon = " << g.icon() << "" << endl; 
  out << tab(depth+1) << "}" << endl; 
  return out;
}

void printGraphicCoupled(ostream &out, const Graphic g)
{
  out << tab(depth) << "Graphic" << endl;
  out << tab(depth+1) << "{" << endl ; 
  out << tab(depth+1) << "Position = " << (int)(g.x()*TWIPS_TO_PIXEL) << "; " << (int)(g.y()*TWIPS_TO_PIXEL) << endl ; 
  out << tab(depth+1) << "Dimension = " << (int)(g.width()*TWIPS_TO_PIXEL) << "; " << (int)(g.height()*TWIPS_TO_PIXEL) << endl ; 
  out << tab(depth+1) << "Direction = " << g.directionWord() << endl; 
  out << tab(depth+1) << "Color = " << (int)g.color() << endl; 
  out << tab(depth+1) << "Icon = " << g.icon() << "" << endl; 
  out << tab(depth+1) << "Window = 5000; 5000; 5000; 5000" << endl; 
  out << tab(depth+1) << "}" << endl; 
}


ostream &operator<< (ostream &out, const Port& p)
{
  out << tab(depth) << (p.type()==Port::Inport ? "Inport" : "Outport") << endl;
  out << tab(depth+1) << "{" << endl;
  out << tab(depth+1) << "Name = " << p.name() << "" << endl;
  out << tab(depth+1) << "CoupledPort = " <<  p.coupledPort() << "" << endl;
  out << tab(depth+1) << "Description = " << p.desc() << "" << endl;
  out << tab(depth+1) << "Graphic" << endl;
  out << tab(depth+2) << "{" << endl;
  out << tab(depth+2) << "Position = " << (int)p.const_graphic().x()*TWIPS_TO_PIXEL << " ; " << (int)p.const_graphic().y()*TWIPS_TO_PIXEL << endl;
  out << tab(depth+2) << "Dimension = " << (int)p.const_graphic().width()*TWIPS_TO_PIXEL << endl;
  out << tab(depth+2) << "Direction = " << p.const_graphic().directionWord() << endl;
  out << tab(depth+2) << "}" << endl;
  out << tab(depth+1) << "}" << endl;
  return out;
}

istream &operator>> (istream &in, Port& p)
{
  p.setName("Test");
  p.graphic().setDirection(Graphic::RIGHT);
  p.graphic().setWidth(40);
  p.graphic().setHeight(40);
  p.setPortNumber(3);
  return in;
}


ostream &operator<<(ostream &out, const Coupled &c)
{
  unsigned ldepth = depth;
  out << tab(ldepth) << "Coupled" << endl;
  out << tab(ldepth+1) << "{" << endl;
  out << tab(ldepth+1) << "Type = " << (c.father()==NULL ? "Root" : "Coordinator" )<< endl;
  out << tab(ldepth+1) << "Name = " << c.name() << endl;
  out << tab(ldepth+1) << "Ports = " << c.inPorts() << "; " << c.outPorts() << endl;
  out << tab(ldepth+1) << "Description = " << c.description() << endl;
  depth++;
  printGraphicCoupled(out,c.const_graphic());
  depth--;
  out << tab(ldepth+1) << "Parameters" << endl;
  out << tab(ldepth+2) << "{" << endl;
  depth+=2;
  for (int p=0; p<c.parameterCount();p++) {
    printParameter(out,c.parameter(p));
  }
  depth-=2;
  out << tab(ldepth+2) << "}" << endl;
  out << tab(ldepth+1) << "System" << endl;
  out << tab(ldepth+2) << "{" << endl;
  depth+=2;
  for(int port=0;port<c.portCount();port++)
  {
    out << *c.port(port);
  }
  for(int child=0;child<c.childCount();child++)
  {
    Model *m=c.child(child);
    if (m->isAtomic())
    {
      Atomic *a = dynamic_cast<Atomic*>(m);
      out << *a;
    } else {
      Coupled *c = dynamic_cast<Coupled*>(m);
      out << *c;
    }
  }
 for(unsigned int point=0;point<c.pointCount();point++)
  {
    out << *c.pointAt(point);
  }
  for(unsigned int line=0;line<c.lineCount();line++)
  {
    out << *c.lineAt(line);
  }
  vector<string> extra = c.getExtra();
  if (extra.size()) {
    out << tab(ldepth+2) << "Extra" << endl;
    out << tab(ldepth+3) << "{" << endl;
    for (int i=0;i<extra.size();i++)
      out << tab(ldepth+4) << extra[i] << endl;
    out << tab(ldepth+3)<< "}" << endl;
  
  }
  out << tab(ldepth+2) << "}" << endl;
  out << tab(ldepth+1) << "}" << endl;
  return out;
}
