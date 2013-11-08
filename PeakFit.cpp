
#include <math.h>
#include <stdio.h>

#include <QVector>
#include <QDebug>

#include "PeakFit.h"

PeakFit::PeakFit( QObject *pp ) : QObject( pp )
{
  dataValid = false;

  p = np = NULL;
  a = at = aa = I = NULL;
  x = y = Y = s = NULL;
}

void PeakFit::init( int n0, int l0 )
{
  N0 = n0;
  N = 3 * N0 + 2;
  L = l0;

  if ( p != NULL )
    delete [] p;
  p = new double [ N ];

  if ( np != NULL )
    delete [] np;
  np = new double [ N ];

  if ( a != NULL ) {
    for ( int i = 0; i < N; i++ )
      if ( a[i] != NULL ) 
	delete [] a[i];
    delete [] a;
  }
  a = new double * [ N ];
  for ( int i = 0; i < N; i++ )
    a[i] = new double [ L ];

  if ( at != NULL ) {
    for ( int i = 0; i < L; i++ )
      if ( at[i] != NULL ) 
	delete [] at[i];
    delete [] at;
  }
  at = new double * [ L ];
  for ( int i = 0; i < L; i++ )
    at[i] = new double [ N ];
  
  if ( aa != NULL ) {
    for ( int i = 0; i < N; i++ )
      if ( aa[i] != NULL ) 
	delete [] aa[i];
    delete [] aa;
  }
  aa = new double * [ N ];
  for ( int i = 0; i < N; i++ )
    aa[i] = new double [ N ];

  if ( I != NULL ) {
    for ( int i = 0; i < N; i++ )
      if ( I[i] != NULL ) 
	delete [] I[i];
    delete [] I;
  }
  I = new double * [ N ];
  for ( int i = 0; i < N; i++ )
    I[i] = new double [ N ];

  if ( x != NULL )
    delete [] x;
  x = new double [ L ];

  if ( y != NULL )
    delete [] y;
  y = new double [ L ];

  if ( Y != NULL )
    delete [] Y;
  Y = new double [ L ];

  if ( s != NULL )
    delete [] s;
  s = new double [ L ];

  dataValid = true;
}

void PeakFit::setXYP0( double* x0, double *y0, double *p0 )
{
  for ( int i = 0; i < L; i++ ) {
    x[i] = x0[i];
    y[i] = y0[i];
  }

  for ( int i = 0; i < N; i++ ) {
    p[i] = p0[i];
  }
}

double *PeakFit::getP( void )
{
  return p;
}

void PeakFit::calcNewP( double gamma )
{
  qDebug() << "cc 1";
  
  for ( int i = 0; i < L; i++ ) {  // 残差ベクトル s を作る
    Y[i] = 0;
    for ( int j = 0; j < N0; j++ ) {
      Y[i] += p[3*j] * exp( -( x[i] - p[3*j+1] )*( x[i] - p[3*j+1] ) / p[3*j+2] );
    }
    Y[i] += p[3*N0] * x[i] + p[3*N0+1];
    s[i] = Y[i] - y[i];
  }

  printf( "Y = " );
  for( int i = 0; i < L; i++ ) {
    printf( "%g ", Y[i] );
  }
  printf( "\n" );
  printf( "s = " );
  for( int i = 0; i < L; i++ ) {
    printf( "%g ", s[i] );
  }
  printf( "\n" );


  qDebug() << "cc 2";

  for ( int i = 0; i < L; i++ ) {   // a と at を作る ガウス型
    for ( int j = 0; j < N0; j++ ) {
      at[i][j*3+0] = a[j*3+0][i]
	= exp( -( x[i] - p[3*j+1] )*( x[i] - p[3*j+1] ) / p[3*j+2] );
      at[i][j*3+1] = a[j*3+1][i]
	= a[j*3+0][i] * 2 * ( x[i] - p[3*j+1] ) / p[3*j+2];
      at[i][j*3+2] = a[j*3+2][i]
	= a[j*3+1][i] / 2 * ( x[i] - p[3*j+1] ) / p[3*j+2];
    }
    at[i][N0*3+0] = a[N0*3+0][i] = x[i];
    at[i][N0*3+1] = a[N0*3+1][i] = 1;
  }

  qDebug() << "cc 3 " << N << L;

  for ( int i = 0; i < N; i++ ) {    // aa = a * at と I (単位行列)を作る
    for ( int j = 0; j < N; j++ ) {
      a[i][j] = 0;
      for ( int k = 0; k < L; k++ ) {
	aa[i][j] += a[i][k] * at[k][j];
      }
      if ( i == j )
	I[i][j] = 1;
      else 
	I[i][j] = 0;
    }
  }

  for ( int i = 0; i < N; i++ ) {
    for ( int j = 0; j < N; j++ ) {
      printf( "%g ", aa[i][j] );
    }
    printf( "\n" );
  }

  qDebug() << "cc 4";

  for ( int i = 0; i < N; i++ ) {   // aa に減速係数を入れる
    aa[i][i] *= ( 1.0 - gamma );
  }

  qDebug() << "cc 5";

  for ( int i = 0; i < N; i++ ) {   // 掃き出しで aa の逆行列を作る
    double c = aa[i][i];
    for ( int j = 0; j < N; j++ ) {
      aa[i][j] /= c;
      I[i][j] /= c;
    }
    for ( int j = 0; j < N; j++ ) {
      if ( i != j ) {
	c = a[j][i];
	for ( int k = 0; k < N; k++ ) {
	  a[j][k] -= a[i][k] * c;
	  I[j][k] -= I[i][k] * c;
	}
      }
    }
  }                                 // I は(すでに壊れた)aa の逆行列

  for ( int i = 0; i < N; i++ ) {
    for ( int j = 0; j < N; j++ ) {
      printf( "%g ", I[i][j] );
    }
    printf( "\n" );
  }

  qDebug() << "cc 6";

  // 準備が終わったので Pn+1 = Pn - Inv(aa) x A x S の計算
  QVector<double> as, ias;
  for ( int i = 0; i < N; i++ ) {
    as << 0;
    ias << 0;
  }
  for ( int i = 0; i < N; i++ ) {
    for ( int j = 0; j < L; j++ ) {
      as[i] += a[i][j] * s[j];
    }
  }

  qDebug() << "cc 7";

  for ( int i = 0; i < N; i++ ) {
    for ( int j = 0; j < L; j++ ) {
      ias[i] += I[i][j] * as[j];
    }
  }

  qDebug() << "cc 8 " << as << ias;

  for ( int i = 0; i < N; i++ ) {
    np[i] = p[i] - ias[i];
  }

  qDebug() << "cc 9";

  norm = 0;
  for ( int i = 0; i < N; i++ ) {
    if ( fabs( p[i] - np[i] ) > norm )
      norm = fabs( p[i] - np[i] );
    p[i] = np[i];
  }

  qDebug() << "cc 10";
}
