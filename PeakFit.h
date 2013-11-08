
#ifndef PEAKFIT_H
#define PEAKFIT_H

#include <QObject>

class PeakFit : public QObject
{
  Q_OBJECT

  bool dataValid;
  int N0;     // number of peaks
  int N;      // number of parameters
  int L;      // number of data points
  double norm;
  
  double *p;     // parameter vector pj (j=0,..,N)
  double *np;    // new parameter vector pj (j=0,..,N)
  double **a;    // dY/dp  ( at x = xi (i=0,..,L), p = pj (j=0,..,N) )
  double **at;   // transpose of a
  double **aa;   // a * at 
  double **I;     // invert of aa
  double *x, *y; // x: measured point, y: measured value
  double *Y, *s; // y: calculated value, s: residual

 public:
  PeakFit( QObject *p = NULL ) ;

  void init( int n0, int l0 );
  void calcNewP( double gamma );
  void setXYP0( double *x, double *y, double *p );
  double *getP( void );
  double Norm( void ) { return norm; };
};

#endif
