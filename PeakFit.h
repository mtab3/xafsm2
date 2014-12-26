
#if 0

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
  //  double *line;
  double *A, *B, *C;
  double *p1, *p2, *dp;
  double *line;

  double Zansa( void );
  void genZ1( double *z1 );
  void genZ2( double **z2 );
  void getdp( double *dp, double *z1, double **z22 );
  bool invZ2( int d, double **z2, double **z22 );

  void initPs( double *p );
  void backPs( double *p );
  void GenPeaks( void );

  //  void GenPeaks();
  double x0( int x, int i0 );
  double x2( int x, int i0 );
  double Y00( int x, int i0 );
  double Y0( int x, int i0 );
  double Y( int x );
  double dY0( int x, int i, int j );   // i-th paramset j = 0, 1, 2 -- A, B, C
  double ddY0( int x, int i, int j );

 public:
  PeakFit( QObject *p = NULL ) ;

  void init( QVector<MCAPeak> *Peaks, int DLength, double *X, double *Y );
  void fit( bool doFitting, double *line );
};

#endif

#endif


