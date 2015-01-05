
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
	      int Loop, double damp, double prec1, double prec2 )
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
  double oldz = 10;
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
    // showV( "dp", PS, dp );
    
    // double adp = 0;
    for ( int i = 0; i < PS; i++ ) {
      p[i] += dp[i] * damp;
      // 停止判定「パラメータベクトルの変化が十分小さい?」
      // double adp0 = dp[i] / p[i];
      // adp += adp0 * adp0;
    }
    delete [] dp;
    setABC( p );
    // showV( "p", PS, p );

    // 停止判定「残差が十分小さい?」
    double z = 0, z0 = 0;
    for ( int i = 0; i < points; i++ ) {
      if ( e[i] > 0 ) {
	z += fabs( e[i] - f( x[i] ) );
	z0 += e[i];
      }
    }
    z /= z0;

    // stat.sprintf( "%d %g", loop, adp );
    stat.sprintf( "%d %g %g %g %d", loop, z, oldz, fabs(( z - oldz ) / z ), n );
    emit nowStat( stat );
    loop++;
    double rate = fabs(( z - oldz ) / z );
    if ( ( loop > Loop ) || ( z < prec1 ) || ( rate < prec2 ) ) {
      endf = true;
    }
    oldz = z;
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
