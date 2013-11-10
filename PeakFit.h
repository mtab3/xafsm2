
#ifndef PEAKFIT_H
#define PEAKFIT_H

#include <QObject>
#include "MCAPeak.h"

class PeakFit : public QObject
{
  Q_OBJECT

  bool dataValid;
  
  QVector<MCAPeak> *peaks;
  int len, ps;
  double *x, *y;
  double **lines;
  double *p1, *p2;


  double Zansa( void );
  void genZ1( double *z1 );
  void genZ2( double *z2 );
  void newP( double *p1, double *p2, double *z1, double *z22 );
  void invZ2( double *z2, double *z22 );

  void initPs( double *p );
  void GenPeaks();
  double A( int i0 );
  double B( int i0 );
  double C( int i0 );
  double x0( int x, int i0 );
  double x2( int x, int i0 );
  double Y00( int x, int i0 );
  double Y0( int x, int i0 );
  double Y( int x );
  double dY0( int x, int i );
  double ddY0( int x, int i, int j );

 public:
  PeakFit( QObject *p = NULL ) ;

  void init( QVector<MCAPeak> *Peaks, int DLength, double *X, double *Y );
  double **fit( int *peaks );

};

#endif



