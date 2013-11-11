
#ifndef PEAKFIT_H
#define PEAKFIT_H

#include <QObject>
#include "MCAPeak.h"

class PeakFit : public QObject
{
  Q_OBJECT

  bool dataValid;
  
  QVector<MCAPeak> *peaks;
  int len, Ps, Ps3;
  double *x, *y;
  double **lines;
  double *p1, *p2, *dp;

  double Zansa( double *p );
  void genZ1( double *p, double *z1 );
  void genZ2( double *p, double **z2 );
  void getdp( double *p, double *dp, double *z1, double **z22 );
  bool invZ2( int d, double **z2, double **z22 );

  void initPs( double *p );
  void backPs( double *p );

  void GenPeaks();
  double A( double *p, int i0 );
  double B( double *p, int i0 );
  double C( double *p, int i0 );
  double x0( double *p, int x, int i0 );
  double x2( double *p, int x, int i0 );
  double Y00( double *p, int x, int i0 );
  double Y0( double *p, int x, int i0 );
  double Y( double *p, int x );
  double dY0( double *p, int x, int i );
  double ddY0( double *p, int x, int i, int j );

 public:
  PeakFit( QObject *p = NULL ) ;

  void init( QVector<MCAPeak> *Peaks, int DLength, double *X, double *Y );
  double **fit( int *peaks, bool doFitting );

};

#endif



