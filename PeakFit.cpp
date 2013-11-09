
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
}
