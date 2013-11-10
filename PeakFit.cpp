
#include <math.h>
#include <stdio.h>

#include <QVector>
#include <QDebug>

#include "PeakFit.h"

PeakFit::PeakFit( QObject *pp ) : QObject( pp )
{
  dataValid = false;
  len = 0;
  ps = 0;
  x = y = NULL;
  lines = NULL;
  p1 = p2 = NULL;
}

void PeakFit::init( QVector<MCAPeak> *Peaks, int DLength, double *X, double *Y )
{
  if ( lines != NULL ) {
    for ( int i = 0; i < ps + 1; i++ )
      delete [] lines[i];
    delete [] lines;
  }

  peaks = Peaks;
  ps = peaks->count();

  len = DLength;
  x = X;
  y = Y;

  lines = new double* [ ps + 1 ];
  for ( int i = 0; i < ps + 1; i++ ) {
    lines[i] = new double [ len ];
  }

  dataValid = true;
}

double **PeakFit::fit( int *Ps )
{
  *Ps = ps;

  if ( p1 != NULL ) delete [] p1;
  p1 = new double [ ps * 3 ];
  if ( p2 != NULL ) delete [] p2;
  p2 = new double [ ps * 3 ];

  double z, z0;
  double z1[ ps * 3 ];
  double z2[ ps * 3 * ps * 3 ];
  double z22[ ps * 3 * ps * 3 ];

  z = z0 = 0;
  initPs( p1 );

  // 残差 z : これを最小にするのが目的
  z = Zansa();
  while( fabs( ( z - z0 ) / z ) > 0.001 ) {
    z0 = z;
    genZ1( z1 );
    genZ2( z2 );
    invZ2( z2, z22 );
    newP( p1, p2, z1, z22 );
    z = Zansa();
  }

  GenPeaks();
  return lines;
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

void PeakFit::newP( double *p1, double *p2, double *z1, double *z22 )
{
}

void PeakFit::invZ2( double *z2, double *z22 )
{
}

void PeakFit::genZ1( double *z1 )
{
  for ( int i = 0; i < ps * 3; i++ ) {
    z1[i] = 0;
    for ( int x = 0; x < len; x++ ) {
      z1[i] += dY0( x, i );
    }
  }
}

void PeakFit::genZ2( double *z2 )
{
  int k;
  for ( int i = 0; i < ps * 3; i++ ) {
    for ( int j = 0; j < ps * 3; j++ ) {
      k = i * ps * 3 + j;
      z2[k] = 0;
      for ( int x = 0; x < len; x++ ) {
	z2[k] += ddY0( x, i, j );
      }
    }
  }
}

double PeakFit::A( int i0 )          { return p1[i0*3]; }
double PeakFit::B( int i0 )          { return p1[i0*3+1]; }
double PeakFit::C( int i0 )          { return p1[i0*3+2]; }
double PeakFit::x0( int x, int i0 )  { return ( x - B(i0) ); }
double PeakFit::x2( int x, int i0 )  { return x0( x, i0 )*x0( x, i0 ); }
double PeakFit::Y00( int x, int i0 ) { return exp( -x2( x, i0 ) * C(i0) * 4 ); }
double PeakFit::Y0( int x, int i0 )  { return A(i0) * Y00( x, i0 ); }

double PeakFit::Y( int x )
{
  double y = 0;
  for ( int i = 0; i < ps; i++ )
    y += Y0( x, i );
  return y;
}

double PeakFit::dY0( int x, int i )
{
  double y = 0;
  int i0 = (int)( i / 3 );
  int i1 = i % 3;
  
  switch( i1 ) {
  case 0: y = Y00( x, i0 ); break;
  case 1: y = Y0( x, i0 ) * 8 * C(i0) * x0( x, i0 ); break;
  case 2: y = -Y0( x, i0 ) * 4 * x2( x, i0 ); break;
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
    case 1: y = Y00( x, i0 ) * 8 * C(i0) * x0( x, i0 ); break;
    case 2: y = -Y00( x, i0 ) * 4 * x2( x, i0 ); break;
    } break;
  case 1:
    switch( j1 ) {
    case 0: y = Y00( x, i0 )*8*C(i0)*x0( x, i0 ); break;
    case 1: y = dY0( x, i0*3+1 )*8*C(i0)*x0( x, i0 ) - Y0( x, i0 )*8*C(i0); break;
    case 2: y = dY0( x, i0*3+2 )*8*C(i0)*x0( x, i0 ) + Y0( x, i0 )*8*x0( x, i0 ); break;
    } break;
  case 2:
    switch( j1 ) {
    case 0: y = -Y00( x, i0 ) * 4 * x2( x, i0 ); break;
    case 1: y = dY0( x, i0*3+2 )*8*C(i0)*x0( x, i0 ) + Y0( x, i0 )*8*x0( x, i0 ); break;
    case 2: y = -dY0( x, i0*3+2 ) * 4 * x2( x, i0 ); break;
    } break;
  }

  return y;
}

void PeakFit::initPs( double *p )
{
  for ( int i = 0; i < ps; i++ ) {
    p[i*3]   = (*peaks)[i].peakH0;
    p[i*3+1] = (*peaks)[i].center;
    p[i*3+2] = 1. / ( (*peaks)[i].center - (*peaks)[i].start );
    p[i*3+2] *= p[i*3+2];
  }
}

void PeakFit::GenPeaks( void )
{
  for ( int j = 0; j < len; j++ ) {
    lines[0][j] = 0;
    for ( int i = 0; i < ps; i++ ) {
      double x0 = (*peaks)[i].center;
      double w = (*peaks)[i].center - (*peaks)[i].start;
      lines[i+1][j] = (*peaks)[i].peakH0
	* exp( - ( j - x0 ) * ( j - x0 ) / ( w * w ) * 4 );
      lines[0][j] += lines[i+1][j];
    }
  }
}
