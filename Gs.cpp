
#include <stdio.h>

#include "Gs.h"

void showM( const char *name, int n, double **M )
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

void showV( const char *name, int n, double *V )
{
  printf( "# %s = \n# ", name );
  
  for ( int i = 0; i < n; i++ ) {
    printf( "%g ", V[i] );
  }
  printf( "\n# " );
  printf( "\n" );
}

void Gs::fit( int points, double *x, double *e, double *p,
	      int Loop, double damp, double prec )
{
  int GS = n;
  int PS = GS * 3;

  double **M, **I;
  double *V;
  bool *ff;

  M = new double * [ PS ];
  I = new double * [ PS ];
  V = new double [ PS ];
  for ( int i = 0; i < PS; i++ ) {
    M[i] = new double [ PS ];
    I[i] = new double [ PS ];
  }
  ff = new bool [ PS ];

  setABC( p );
  bool endf = false;
  int loop = 0;
  while ( ! endf ) {
    for ( int i = 0; i < PS; i++ ) {
      for ( int j = 0; j < PS; j++ ) {
	M[i][j] = 0;
	I[i][j] = 0;
      }
      V[i] = 0;
      I[i][i] = 1;
      ff[i] = true;
    }
    
    for ( int i = 0; i < points; i++ ) {
      for ( int j = 0; j < PS; j++ ) {
	for ( int k = 0; k < PS; k++ ) {
	  M[j][k] += dp( k, x[i] ) * dp( j, x[i] );
	}
	V[j] += ( e[i] - f( x[i] ) ) * dp( j, x[i] );
      }
    }
    //    showM( "M", PS, M );
    //    showV( "V", PS, V );
    
    for ( int i = 0; i < PS; i++ ) {
      double max = -1e300;
      int ii = -1;
      for ( int j = 0; j < PS; j++ ) {
	if ( ff[j] ) {
	  if ( M[j][j] > max ) {
	    max = M[j][j];
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
    //    showM( "M", PS, M );
    //    showM( "I", PS, I );
    
    double *dp;
    dp = new double [ PS ];
    for ( int i = 0; i < PS; i++ ) {
      dp[i] = 0;
      for ( int j = 0; j < PS; j++ ) {
	dp[i] += I[i][j] * V[j];
      }
    }
    //    showV( "dp", PS, dp );
    
    double adp = 0, ap = 0;
    for ( int i = 0; i < PS; i++ ) {
      p[i] += dp[i] * damp;
      adp += dp[i] * dp[i];
      ap += p[i] * p[i];
    }
    delete [] dp;
    setABC( p );
    //    showV( "p", PS, p );

    stat.sprintf( "# %d %g\n", loop, adp / ap );
    emit nowStat( stat );
    loop++;
    if ( ( loop > Loop ) || ( ( adp / ap ) < prec ) ) {
      endf = true;
    }
  }

  for ( int i = 0; i < PS; i++ ) {
    delete [] M[i];
    delete [] I[i];
  }
  delete [] M;
  delete [] I;
  delete [] V;
  delete [] ff;
}
