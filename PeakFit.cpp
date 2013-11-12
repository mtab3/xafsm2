
#include <math.h>
#include <stdio.h>

#include <QVector>
#include <QDebug>

#include "PeakFit.h"

PeakFit::PeakFit( QObject *pp ) : QObject( pp )
{
  dataValid = false;
  len = 0;
  Ps = Ps3 = 0;
  x = y = NULL;
  line = NULL;
  p1 = p2 = NULL;
  dp = NULL;
}

void PeakFit::init( QVector<MCAPeak> *Peaks, int DLength, double *X, double *Y )
{
  peaks = Peaks;
  Ps = peaks->count();
  Ps3 = Ps * 3;

  len = DLength;
  x = X;
  y = Y;

  dataValid = true;
}

void PeakFit::fit( bool f, double *l )
{
  line = l;

  if ( f ) {
    if ( p1 != NULL ) delete [] p1;
    p1 = new double [ Ps3 ];
    if ( p2 != NULL ) delete [] p2;
    p2 = new double [ Ps3 ];
    if ( dp != NULL ) delete [] dp;
    dp = new double [ Ps3 ];

    double z, z0;
    double z1[ Ps3 ];
    double **z2 = new double *[ Ps3 ];
    double **z22 = new double *[ Ps3 ];
    for ( int i = 0; i < Ps3; i++ ) {
      z2[i] = new double [ Ps3 ];
      z22[i] = new double [ Ps3 ];
    }
    
    z = z0 = 0;
    initPs( p1 );
    
    int cnt = 0;
    
    // 残差 z : これを最小(極小)にするのが目的
    // ホントは Zansa( p1 ) とするのが綺麗だけど、
    // Zansa の下請け関数に全部 p1 を配るのが面倒なので...
    z = Zansa();
    while( fabs( ( z - z0 ) / z ) > 0.001 ) {
      qDebug() << cnt++ << z;
      z0 = z;
      qDebug() << "a";
      genZ1( z1 );  // z1 は z をパラメータ p(i) で微分したベクトル。(z極小) == (z1=0)
      qDebug() << "b";
      genZ2( z2 );  // z2 は z1 をバラメータ p(j) で微分した行列
      qDebug() << "c";
      if ( ! invZ2( Ps3, z2, z22 ) ) // 逆行列が正常に計算できなかったら終了
	break;           
      // 単純ニュートン法で p(i)n+1 = p(i)n - z1/z2 に準じるが
      qDebug() << "d";
      getdp( dp, z1, z22 );  // そのままでは怖すぎるので制動をかける。
      qDebug() << "e";
      // 次のステップのパラメータ列
      double mul = 1.0; 
      do {
	for ( int i = 0; i < Ps3; i++ ) {
	  p2[i] = p1[i] - dp[i] * mul;
	}
	A = p2; B = p2 + Ps; C = p2 + Ps*2;
	z = Zansa();
	mul *= 0.9;
      } while (( z > z0 )&&( mul > 0.001 ));
      qDebug() << "f";
      for ( int i = 0; i < Ps3; i++ ) {
	p1[i] = p2[i];
      }
      qDebug() << "g";
      A = p1; B = p1 + Ps; C = p1 + Ps*2;
    }
    
    backPs( p1 );
    
    for ( int i = 0; i < Ps3; i++ ) {
      delete [] z2[i];
      delete [] z22[i];
    }
    delete [] z2;
    delete [] z22;
  }
  GenPeaks();
}

double PeakFit::Zansa( void )
{
  double z0, z = 0;
  for ( int i = 0; i < len; i++ ) {
    z0 = y[i] - Y(i);
    z += z0 * z0;
  }
  return z;
}

void PeakFit::getdp( double *dp, double *z1, double **z22 )
{
  // dp = -f/f'
  for ( int i = 0; i < Ps3; i++ ) {
    dp[i] = 0;
    for ( int j = 0; j < Ps3; j++ ) {
      dp[i] += z22[i][j] * z1[j];
    }
  }

  // 全力のニュートン法なら x(n+1) = x(n) - f(x(n))/f'(x(n)) だけど...
  // まともにやるのは怖すぎるので

  // p1 で規格化した dp のノルムを求める ( p1 の大きさを 1と思った時の dp の大きさ )
  double norm = 0;
  for ( int i = 0; i < Ps3; i++ ) {
    if ( p1[i] != 0 ) 
      norm += dp[i]/p1[i] * dp[i]/p1[i];
  }
  norm = sqrt( norm );
  // norm が大きすぎる時は(ニュートン法の1ステップでの変化が大きすぎる時は)
  // dp の norm の大きさが一定値になるように制限をかけてしまう。
  double R = 1.0;
  if ( norm > R ) {
    for ( int i = 0; i < Ps3; i++ ) {
      dp[i] = dp[i] / norm * R;
    }
  }
}

bool PeakFit::invZ2( int d, double **z2, double **z22 )
{
  for ( int i = 0; i < d; i++ ) {
    for ( int j = 0; j < d; j++ ) {
      z22[i][j] = ( i == j ) ? 1 : 0;
    }
  }

  bool f[ d ];
  for ( int i = 0; i < d; i++ ) f[i] = true;

  while ( 1 ) {  // 対角要素が大きい行から順番に
    double max = 0;
    int maxi = 0;
    int i;
    for ( i = 0; i < d; i++ ) {
      if ( f[i] ) {
	if ( fabs( z2[i][i] ) > max ) {
	  max = fabs( z2[i][i] );
	  maxi = i;
	}
      }
    }
    if ( i >= d )
      break;
    f[maxi] = false;

    double a, b;
    a = z2[maxi][maxi];
    if ( a == 0 )
      return false;
    for ( int x = 0; x < d; x++ ) {
      z2[maxi][x] /= a;
      z22[maxi][x] /= a;
    }
    for ( int y2 = 0; y2 < d; y2++ ) {
      if ( maxi != y2 ) {
	b = z2[y2][maxi];
	for ( int x = 0; x < d; x++ ) {
	  z2[y2][x] -= b * z2[maxi][x];
	  z22[y2][x] -= b * z22[maxi][x];
	}
      }
    }
  }
  
  return true;
}

void PeakFit::genZ1( double *z1 )
{
  for ( int i = 0; i < Ps; i++ ) {
    for ( int j = 0; j < 3; j++ ) {
      z1[i] = 0;
      for ( int x = 0; x < len; x++ ) {
	z1[i] += -( y[x] - Y( x ) ) * dY0( x, i, j );
      }
    }
  }
}

void PeakFit::genZ2( double **z2 )
{
  for ( int i = 0; i < Ps; i++ ) {
    for ( int j = 0; j < Ps; j++ ) {
      for ( int k = 0; k < 3; k++ ) {
	for ( int l = 0; l < 3; l++ ) {
	  z2[i][j] = 0;
	  for ( int x = 0; x < len; x++ ) {
	    z2[i][j] += dY0( x, i, k ) * dY0( x, j, l )
	      - ( y[x] - Y( x ) )*ddY0( x, i, j );
	  }
	}
      }
    }
  }
}

double PeakFit::x0( int x, int i0 )  { return ( x - A[i0] ); }
double PeakFit::x2( int x, int i0 )  { return x0( x, i0 )*x0( x, i0 ); }
double PeakFit::Y00( int x, int i0 ) { return exp( -x2( x, i0 ) * C[i0] ); }
double PeakFit::Y0( int x, int i0 )  { return A[i0] * Y00( x, i0 ); }

double PeakFit::Y( int x )
{
  double y = 0;
  for ( int i = 0; i < Ps; i++ )
    y += Y0( x, i );
  return y;
}

double PeakFit::dY0( int x, int i, int j )
{
  double y = 0;
  
  switch( j ) {
  case 0: y = Y00( x, i ); break;
  case 1: y = Y0( x, i ) * 2 * C[i] * x0( x, i ); break;
  case 2: y = -Y0( x, i ) * x2( x, i ); break;
  }
  return y;
}

double PeakFit::ddY0( int x, int i, int j ) 
{
  double y = 0;
  int i0 = ( int )( i / 3 );
  int i1 = i % 3;
  int j0 = ( int )( j / 3 );
  int j1 = j % 3;
  if ( i0 != j0 )
    return 0;

  switch( i1 ) {
  case 0:
    switch( j1 ) {
    case 0: y = 0; break;
    case 1: y = Y00( x, i0 ) * 2 * C[i0] * x0( x, i0 ); break;
    case 2: y = -Y00( x, i0 )*x2( x, i0 ); break;
    } break;
  case 1:
    switch( j1 ) {
    case 0: y = Y00( x, i0 )*2*C[i0] * x0( x, i0 ); break;
    case 1: y = dY0( x, i0, 1 )*2*C[i0] * x0( x, i0 ) - Y0( x, i0 )*2*C[i0]; break;
    case 2: y = dY0( x, i0, 2 )*2*C[i0] * x0( x, i0 ) + Y0( x, i0 )*2*x0( x, i0 ); break;
    } break;
  case 2:
    switch( j1 ) {
    case 0: y = -Y00( x, i0 ) * x2( x, i0 ); break;
    case 1: y = dY0( x, i0, 2 )*2*C[i0] * x0( x, i0 ) + Y0( x, i0 )*2*x0( x, i0 ); break;
    case 2: y = -dY0( x, i0, 2 )*x2( x, i0 ); break;
    } break;
  }

  return y;
}

void PeakFit::initPs( double *p )
{
  A = p;
  B = p + Ps;
  C = p + Ps * 2;
  for ( int i = 0; i < Ps; i++ ) {
    A[i]   = (*peaks)[i].peakH0;
    B[i] = (*peaks)[i].center;
    C[i] = 1. / ( (*peaks)[i].center - (*peaks)[i].start );
    C[i] *= C[i];
  }
}

void PeakFit::backPs( double *p )
{
  A = p;
  B = p + Ps;
  C = p + Ps * 2;

  double w;
  for ( int i = 0; i < Ps; i++ ) {
    (*peaks)[i].peakH0 = A[i];
    (*peaks)[i].center = B[i];
    w = 1. / sqrt( C[i] );
    (*peaks)[i].start = (*peaks)[i].center - w;
    (*peaks)[i].end = (*peaks)[i].center + w;
  }
}

void PeakFit::GenPeaks( void )
{
  for ( int j = 0; j < len; j++ ) {
    line[j] = 0;
    for ( int i = 0; i < Ps; i++ ) {
      double x0 = (*peaks)[i].center;
      double w = (*peaks)[i].center - (*peaks)[i].start;
      line[j] += (*peaks)[i].peakH0
	* exp( - ( j - x0 ) * ( j - x0 ) / ( w * w ) );
    }
  }
}
