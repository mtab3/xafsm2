
#include <stdio.h>
#include <math.h>

#include "LinearA.h"

LA::LA( void )
{
}

LA::~LA( void )
{
}

void LA::clearM( int n, double **M )
{
  for ( int i = 0; i < n; i++ ) {
    for ( int j = 0; j < n; j++ ) {
      M[i][j] = 0;
    }
  }
}

void LA::clearV( int n, double *V )
{
  for ( int i = 0; i < n; i++ ) {
    V[i] = 0;
  }
}

void LA::setI( int n, double **M )
{
  for ( int i = 0; i < n; i++ ) {
    for ( int j = 0; j < n; j++ ) {
      M[i][j] = 0;
    }
    M[i][i] = 1;
  }
}

void LA::showM( const char *name, int n, double **M )
{
  printf( "# %s = \n# ", name );
  
  for ( int i = 0; i < n; i++ ) {
    for ( int j = 0; j < n; j++ ) {
      printf( "%g ", M[i][j] );
    }
    printf( "\n# " );
  }
  printf( "\n" );
}

void LA::showV( const char *name, int n, double *V )
{
  printf( "# %s = \n# ", name );
  
  for ( int i = 0; i < n; i++ ) {
    printf( "%g ", V[i] );
  }
  printf( "\n# " );
  printf( "\n" );
}

bool LA::checkMerr( int n, double **M )
{
  for ( int i = 0; i < n; i++ ) {
    for ( int j = 0; j < n; j++ ) {
      if ( isnan( M[i][j] ) )
	return true;
    }
  }
  return false;
}

bool LA::checkVerr( int n, double *V )
{
  for ( int i = 0; i < n; i++ ) {
    if ( isnan( V[i] ) )
      return true;
  }
  return false;
}

void LA::copyV( int n, double *s, double *d )
{
  for ( int i = 0; i < n; i++ )
    d[i] = s[i];
}

void LA::invert( int PS, double **M, double **I )
{
  bool *ff;
  ff = new bool [ PS ];
  for ( int i = 0; i < PS; i++ ) {
    ff[i] = true;
  }
  
  for ( int i = 0; i < PS; i++ ) {
    double max = -1e300;
    int ii = -1;
    for ( int j = 0; j < PS; j++ ) {
      if ( ff[j] ) {
	if ( fabs( M[j][j] ) > max ) {
	  max = fabs( M[j][j] );
	  ii = j;
	}
      }
    }
    if ( ii < 0 ) {
      for ( ii = 0; ii < PS; ii++ ) {
	if ( ff[ii] )
	  break;
      }
    }
    ff[ii] = false;
    
    double a = M[ii][ii];
    if ( a != 0 ) {
      for ( int j = 0; j < PS; j++ ) {
	M[ii][j] /= a;
	I[ii][j] /= a;
      }
      for ( int j = 0; j < PS; j++ ) {
	if ( ff[j] ) {
	  double b = M[j][ii];
	  for ( int k = 0; k < PS; k++ ) {
	    M[j][k] -= M[ii][k] * b;
	    I[j][k] -= I[ii][k] * b;
	  }
	}
      }
    }
  }
}

void LA::makeLU( int n, double **M, int *L )
{
  for ( int i = 0; i < n; i++ )
    L[i] = i;

  for ( int i = 0; i < n; i++ ) {
    double max = -1e300;
    int maxl = i;
    for ( int j = i; j < n; j++ ) {
      if ( fabs( M[ L[j] ][i] ) > maxl ) {
	maxl = j;
	max = fabs( M[ L[j] ][i] );
      }
    }
    double tmp = L[i];
    L[i] = L[maxl];
    L[maxl] = tmp;
  }

  showM( "LU M ", n, M );
  for ( int i = 0; i < n; i++ )
    printf( "%d ", L[i] );
  printf( "\n" );
}
