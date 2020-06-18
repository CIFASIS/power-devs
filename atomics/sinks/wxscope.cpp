#include "wxscope.h"
void wxscope::init(double t,...) {
Sigma=1e12;
     pFrame=new PlotFrame();
     pFrame->Show(TRUE);
}
double wxscope::ta(double t) {
return Sigma;
}
void wxscope::dint(double t) {
Sigma=1e12;
}
void wxscope::dext(Event x, double t) {
pFrame->addpoint(x,t);
     Sigma=0;
}
Event wxscope::lambda(double t) {
return Event();
}
void wxscope::exit() {
pFrame->Refresh();
}
