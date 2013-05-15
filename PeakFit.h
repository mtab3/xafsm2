
#if 0

#ifndef PEAKFIT_H
#define PEAKFIT_H

#include <QObject>

class PeakFit : public QObject
{
  Q_OBJECT

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
  PeakFit( void ) {};

  void init( int n0, int l0 );
  void release( void );
  void calcNewP( double gamma );
  void setXYP0( QVector<double> x, QVector<double> y, QVector<double> p );
  QVector<double> getP( void );
  double Norm( void ) { return norm; };
};

#endif

#endif
