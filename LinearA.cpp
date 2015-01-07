
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "LinearA.h"

LA::LA( void )
{
}

LA::~LA( void )
{
}


double **LA::newM( int n )
{
  double **M = new double * [n];
  for ( int i = 0; i < n; i++ ) {
    M[i] = new double [n];
  }
  return M;
}

double *LA::newV( int n )
{
  double *V = new double [n];
  return V;
}

int *LA::newIV( int n )
{
  int *V = new int [n];
  return V;
}

void LA::deleteM( int n, double **M )
{
  for ( int i = 0; i < n; i++ ) {
    delete [] M[i];
  }
  delete [] M;
}

void LA::deleteV( double *V )
{
  delete [] V;
}

void LA::deleteIV( int *V )
{
  delete [] V;
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

void LA::showMi( const char *name, int n, double **M, int *idx )
{
  printf( "# %s = \n# ", name );
  
  for ( int i = 0; i < n; i++ ) {
    for ( int j = 0; j < n; j++ ) {
      printf( "%g ", M[idx[i]][j] );
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

void LA::showIV( const char *name, int n, int *V )
{
  printf( "# %s = \n# ", name );
  
  for ( int i = 0; i < n; i++ ) {
    printf( "%d ", V[i] );
  }
  printf( "\n# " );
  printf( "\n" );
}

void LA::showVi( const char *name, int n, double *V, int *idx )
{
  printf( "# %s = \n# ", name );
  
  for ( int i = 0; i < n; i++ ) {
    printf( "%g ", V[idx[i]] );
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

void LA::copyM( int n, double **s, double **d )
{
  for ( int i = 0; i < n; i++ )
    for ( int j = 0; j < n; j++ ) 
      d[i][j] = s[i][j];
}

void LA::copyV( int n, double *s, double *d )
{
  for ( int i = 0; i < n; i++ )
    d[i] = s[i];
}

void LA::mulMM( int n, double **M, double **N, double **R )
{
  for ( int i = 0; i < n; i++ ) {
    for ( int j = 0; j < n; j++ ) {
      R[i][j] = 0;
      for ( int k = 0; k < n; k++ ) {
	R[i][j] += M[i][k] * N[k][i];
      }
    }
  }
}

void LA::mulMV( int n, double **M, double *V, double *R )
{
  for ( int i = 0; i < n; i++ ) {
    R[i] = 0;
    for ( int j = 0; j < n; j++ ) {
      R[i] += M[i][j] * V[j];
    }
  }
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

void LA::makeLU( int n, double **M, double **L, double **U, int *idx )
{
  for ( int i = 0; i < n; i++ )
    idx[i] = i;

  copyM( n, M, L );
  for ( int i = 0; i < n; i++ ) {
    // i 番目が最大値になる行を見つける
    double max = -1e300;
    int maxl = i;
    for ( int j = i; j < n; j++ ) {
      if ( fabs( L[ idx[j] ][i] ) > maxl ) {
	maxl = j;
	max = fabs( L[ idx[j] ][i] );
      }
    }
    double tmp = idx[i];
    idx[i] = idx[maxl];
    idx[maxl] = tmp;

    // LU 分解 (1個の行列の中にパック)
    double a = L[idx[i]][i];
    for ( int j = i+1; j < n; j++ ) {
      L[idx[j]][i] /= a;
      double b = L[idx[j]][i];
      for ( int k = i+1; k < n; k++ ) {
	L[idx[j]][k] -= b * L[idx[i]][k];
      }
    }
  }
  
  // L と U に分けておく
  for ( int i = 0; i < n; i++ ) {
    for ( int j = 0; j < n; j++ ) {
      if ( j < i ) {
	U[idx[i]][j] = 0;
      } else {
	U[idx[i]][j] = L[idx[i]][j];
	L[idx[i]][j] = 0;
      }
      if ( i == j )
	L[idx[i]][j] = 1;
    }
  }
}

void LA::solveLV( int n, double **L, double *V, double *R, int *idx )
{
  copyV( n, V, R );
  for ( int i = 0; i < n - 1; i++ ) {
    R[idx[i]] /= L[idx[i]][i];         // L の対角は 1 なので不要のはず
    for ( int j = i + 1; j < n; j++ ) {
      R[idx[j]] -= R[idx[i]] * L[idx[j]][i];
    }
  }
}

void LA::solveUV( int n, double **U, double *V, double *R, int *idx )
{
  copyV( n, V, R );
  for ( int i = n - 1; i >= 0; i-- ) {
    R[idx[i]] /= U[idx[i]][i];
    for ( int j = i - 1; j >= 0; j-- ) {
      R[idx[j]] -= R[idx[i]] * U[idx[j]][i];
    }
  }
}
