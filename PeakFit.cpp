
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
  lines = NULL;
  p1 = p2 = NULL;
  dp = NULL;
}

void PeakFit::init( QVector<MCAPeak> *Peaks, int DLength, double *X, double *Y )
{
  if ( lines != NULL ) {
    for ( int i = 0; i < Ps + 1; i++ )
      delete [] lines[i];
    delete [] lines;
  }

  peaks = Peaks;
  Ps = peaks->count();
  Ps3 = Ps * 3;

  len = DLength;
  x = X;
  y = Y;

  lines = new double* [ Ps + 1 ];
  for ( int i = 0; i < Ps + 1; i++ ) {
    lines[i] = new double [ len ];
  }

  dataValid = true;
}

double **PeakFit::fit( int *ps, bool f )
{
  *ps = Ps;

  if ( f ) {
    qDebug() << "in fitting";

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
    
    // �ĺ� z : �����Ǿ�(�˾�)�ˤ���Τ���Ū
    // �ۥ�Ȥ� Zansa( p1 ) �Ȥ���Τ��������ɡ�
    // Zansa �β������ؿ������� p1 ���ۤ�Τ����ݤʤΤ�...
    z = Zansa( p1 );
    while( fabs( ( z - z0 ) / z ) > 0.01 ) {
      z0 = z;
      genZ1( p1, z1 );  // z1 �� z ��ѥ�᡼�� p(i) ����ʬ�����٥��ȥ롣(z�˾�) == (z1=0)
      genZ2( p1, z2 );  // z2 �� z1 ��Х�᡼�� p(j) ����ʬ��������
      if ( ! invZ2( Ps3, z2, z22 ) ) // �չ�������˷׻��Ǥ��ʤ��ä��齪λ
	break;           
      // ñ��˥塼�ȥ�ˡ�� p(i)n+1 = p(i)n - z1/z2 �˽स�뤬
      getdp( p1, dp, z1, z22 );  // ���ΤޤޤǤ��ݤ�����Τ���ư�򤫤��롣
      // ���Υ��ƥåפΥѥ�᡼����
      double mul = 1.0; 
      do {
	for ( int i = 0; i < Ps3; i++ ) {
	  p2[i] = p1[i] - dp[i] * mul;
	}
	z = Zansa( p2 );
	mul *= 0.9;
      } while (( z > z0 )&&( mul > 0.01 ));
      for ( int i = 0; i < Ps3; i++ ) {
	p1[i] = p2[i];
      }
      qDebug() << "nc " << cnt++;
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

  return lines;
}

double PeakFit::Zansa( double *p )
{
  double z0, z = 0;
  for ( int i = 0; i < len; i++ ) {
    z0 = y[i] - Y(p,i);
    z += z0 * z0;
  }
  return z;
}

void PeakFit::getdp( double *p1, double *dp, double *z1, double **z22 )
{
  // dp = -f/f'
  for ( int i = 0; i < Ps3; i++ ) {
    dp[i] = 0;
    for ( int j = 0; j < Ps3; j++ ) {
      dp[i] += z22[i][j] * z1[j];
    }
  }

  // ���ϤΥ˥塼�ȥ�ˡ�ʤ� x(n+1) = x(n) - f(x(n))/f'(x(n)) ������...
  // �ޤȤ�ˤ��Τ��ݤ�����Τ�

  // p1 �ǵ��ʲ����� dp �ΥΥ������ ( p1 ���礭���� 1�Ȼפä����� dp ���礭�� )
  double norm = 0;
  for ( int i = 0; i < Ps3; i++ ) {
    if ( p1[i] != 0 ) 
      norm += dp[i]/p1[i] * dp[i]/p1[i];
  }
  norm = sqrt( norm );
  // norm ���礭���������(�˥塼�ȥ�ˡ��1���ƥåפǤ��Ѳ����礭���������)
  // dp �� norm ���礭���������ͤˤʤ�褦�����¤򤫤��Ƥ��ޤ���
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

  while ( 1 ) {  // �г����Ǥ��礭���Ԥ�����֤�
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

void PeakFit::genZ1( double *p, double *z1 )
{
  for ( int i = 0; i < Ps3; i++ ) {
    z1[i] = 0;
    for ( int x = 0; x < len; x++ ) {
      z1[i] += dY0( p, x, i );
    }
  }
}

void PeakFit::genZ2( double *p, double **z2 )
{
  for ( int i = 0; i < Ps3; i++ ) {
    for ( int j = 0; j < Ps3; j++ ) {
      z2[i][j] = 0;
      for ( int x = 0; x < len; x++ ) {
	z2[i][j] += ddY0( p, x, i, j );
      }
    }
  }
}

double PeakFit::A( double *p, int i0 )          { return p[i0*3]; }
double PeakFit::B( double *p, int i0 )          { return p[i0*3+1]; }
double PeakFit::C( double *p, int i0 )          { return p[i0*3+2]; }
double PeakFit::x0( double *p, int x, int i0 )  { return ( x - B(p,i0) ); }
double PeakFit::x2( double *p, int x, int i0 )  { return x0( p, x, i0 )*x0( p, x, i0 ); }
double PeakFit::Y00( double *p, int x, int i0 ) { return exp(-x2( p,x,i0 )*C(p,i0)); }
double PeakFit::Y0( double *p, int x, int i0 )  { return A(p,i0) * Y00( p, x, i0 ); }

double PeakFit::Y( double *p, int x )
{
  double y = 0;
  for ( int i = 0; i < Ps; i++ )
    y += Y0( p, x, i );
  return y;
}

double PeakFit::dY0( double *p, int x, int i )
{
  double y = 0;
  int i0 = (int)( i / 3 );
  int i1 = i % 3;
  
  switch( i1 ) {
  case 0: y = Y00( p, x, i0 ); break;
  case 1: y = Y0( p, x, i0 ) * 2 * C(p,i0) * x0( p, x, i0 ); break;
  case 2: y = -Y0( p, x, i0 ) * x2( p, x, i0 ); break;
  }
  return y;
}

double PeakFit::ddY0( double *p, int x, int i, int j ) 
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
    case 1: y = Y00(p,x,i0)*2*C(p,i0)*x0(p,x,i0); break;
    case 2: y = -Y00(p,x,i0)*x2(p,x,i0); break;
    } break;
  case 1:
    switch( j1 ) {
    case 0: y = Y00(p,x,i0)*2*C(p,i0)*x0(p,x,i0); break;
    case 1: y = dY0(p,x,i0*3+1)*2*C(p,i0)*x0(p,x,i0) - Y0(p,x,i0)*2*C(p,i0); break;
    case 2: y = dY0(p,x,i0*3+2)*2*C(p,i0)*x0(p,x,i0) + Y0(p,x,i0)*2*x0(p,x,i0); break;
    } break;
  case 2:
    switch( j1 ) {
    case 0: y = -Y00(p,x,i0)*x2(p,x,i0); break;
    case 1: y = dY0(p,x,i0*3+2)*2*C(p,i0)*x0(p,x,i0) + Y0(p,x,i0)*2*x0(p,x,i0); break;
    case 2: y = -dY0(p,x,i0*3+2)*x2(p,x,i0); break;
    } break;
  }

  return y;
}

void PeakFit::initPs( double *p )
{
  for ( int i = 0; i < Ps; i++ ) {
    p[i*3]   = (*peaks)[i].peakH0;
    p[i*3+1] = (*peaks)[i].center;
    p[i*3+2] = 1. / ( (*peaks)[i].center - (*peaks)[i].start );
    p[i*3+2] *= p[i*3+2];
  }
}

void PeakFit::backPs( double *p )
{
  double w;
  for ( int i = 0; i < Ps; i++ ) {
    (*peaks)[i].peakH0 = p[i*3];
    (*peaks)[i].center = p[i*3+1];
    w = 1. / sqrt( p[i*3+2] );
    (*peaks)[i].start = (*peaks)[i].center - w;
    (*peaks)[i].end = (*peaks)[i].center + w;
  }
}

void PeakFit::GenPeaks( void )
{
  for ( int j = 0; j < len; j++ ) {
    lines[0][j] = 0;
    for ( int i = 0; i < Ps; i++ ) {
      double x0 = (*peaks)[i].center;
      double w = (*peaks)[i].center - (*peaks)[i].start;
      lines[i+1][j] = (*peaks)[i].peakH0
	* exp( - ( j - x0 ) * ( j - x0 ) / ( w * w ) );
      lines[0][j] += lines[i+1][j];
    }
  }
}
