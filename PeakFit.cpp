
#if 0

#include <math.h>
#include <stdio.h>

#include <QVector>
#include <QDebug>

#include "PeakFit.h"

void PeakFit::init( int n0, int l0 ) 
{
  N0 = n0;
  N = 3 * N0 + 2;
  L = l0;

  p = new double [ N ];
  np = new double [ N ];

  a = new double * [ N ];
  for ( int i = 0; i < N; i++ )
    a[i] = new double [ L ];

  at = new double * [ L ];
  for ( int i = 0; i < L; i++ )
    at[i] = new double [ N ];

  aa = new double * [ N ];
  for ( int i = 0; i < N; i++ )
    aa[i] = new double [ N ];

  I = new double * [ N ];
  for ( int i = 0; i < N; i++ )
    I[i] = new double [ N ];

  x = new double [ L ];
  y = new double [ L ];
  Y = new double [ L ];
  s = new double [ L ];
}

void PeakFit::setXYP0( QVector<double> x0, QVector<double> y0, QVector<double> p0 )
{
  for ( int i = 0; i < L; i++ ) {
    x[i] = x0[i];
    y[i] = y0[i];
  }

  printf( "Y0 = " );
  for( int i = 0; i < L; i++ ) {
    printf( "%g ", y[i] );
  }
  printf( "\n" );

  for ( int i = 0; i < N; i++ ) {
    p[i] = p0[i];
  }
}

QVector<double> PeakFit::getP( void )
{
  QVector<double> r;
  for ( int i = 0; i < N; i++ ) {
    r << p[i];
  }

  return r;
}

void PeakFit::release( void ) 
{
  delete p;
  delete np;

  for ( int i = 0; i < L; i++ ) 
    delete []at[i];
  delete []at;

  for ( int i = 0; i < N; i++ )
    delete []a[i];
  delete []a;

  for ( int i = 0; i < N; i++ ) 
    delete []aa[i];
  delete []aa;

  for ( int i = 0; i < N; i++ ) 
    delete []I[i];
  delete []I;

  delete []x;
  delete []y;
  delete []Y;
  delete []s;
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

#endif
