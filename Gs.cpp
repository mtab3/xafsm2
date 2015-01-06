
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

bool checkMerr( int n, double **M )
{
  for ( int i = 0; i < n; i++ ) {
    for ( int j = 0; j < n; j++ ) {
      if ( isnan( M[i][j] ) )
	return true;
    }
  }
  return false;
}

bool checkVerr( int n, double *V )
{
  for ( int i = 0; i < n; i++ ) {
    if ( isnan( V[i] ) )
      return true;
  }
  return false;
}

void copyV( int n, double *s, double *d )
{
  for ( int i = 0; i < n; i++ )
    d[i] = s[i];
}

bool Gs::fit( int points, double *x, double *e, double *p,
	      int Loop, double damp, double prec1, double prec2 )
{
  int GS = n;
  int PS = GS * 3;

  double *op;
  double **M, **I;
  double *V;
  bool *ff;
  double *zs;
  double *rs, *r1s;

  // work の配列確保
  op = new double [ PS ];
  M = new double * [ PS ];
  I = new double * [ PS ];
  V = new double [ PS ];
  for ( int i = 0; i < PS; i++ ) {
    M[i] = new double [ PS ];
    I[i] = new double [ PS ];
  }
  ff = new bool [ PS ];
  zs = new double [ Loop + 10 ];
  rs = new double [ Loop + 10 ];
  r1s = new double [ Loop + 10 ];
  
  setABC( p );          // 初期値のパラメータセット
  bool endf = false;
  int loop = 0;

  copyV( PS, p, op );
  try {
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
      if ( checkMerr( n, M ) ) throw (char*)"Err in M";
      if ( checkVerr( n, V ) ) throw (char*)"Err in V";
      
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
      if ( checkMerr( n, M ) ) throw (char*)"Err in calc I, M";
      if ( checkMerr( n, I ) ) throw (char*)"Err in calc I, I";
      
      double *dp;
      dp = new double [ PS ];
      for ( int i = 0; i < PS; i++ ) {
	dp[i] = 0;
	for ( int j = 0; j < PS; j++ ) {
	  dp[i] += I[i][j] * V[j];
	}
      }
      if ( checkVerr( n, V ) ) throw (char*)"Err in dp";
      
      copyV( PS, p, op );
      for ( int i = 0; i < PS; i++ ) {
	p[i] += dp[i] * damp;
	// 停止判定「パラメータベクトルの変化が十分小さい?」
	// double adp0 = dp[i] / p[i];
	// adp += adp0 * adp0;
      }
      delete [] dp;
      setABC( p );
      
      // 停止判定「残差が十分小さい?」
      double z = 0, z0 = 0;
      for ( int i = 0; i < points; i++ ) {
	if ( e[i] > 0 ) {
	  z += fabs( e[i] - f( x[i] ) );
	  z0 += e[i];
	}
      }
      z /= z0;
      zs[loop] = z;
      
      if ( loop > 0 ) {
	double oz = zs[loop-1];
	double r = ( z != 0 ) ? fabs( ( z - oz ) / z ) : 1;
	rs[loop] = r;
	if ( loop > 1 ) {
	  double oor = rs[loop-1];
	  double r1 = r - oor;
	  r1s[loop] = r1;
	  if ( loop > 2 ) {
	    double or1 = r1s[loop-1];
	    double r2 = r1 - or1;
	    stat.sprintf( "%d %g %g %g %g %d", loop, z, r, r1, r2, n );
	    emit nowStat( stat );
	    if ( ( loop > Loop ) || ( z < prec1 ) || ( fabs( r2 ) < prec2 ) ) {
	      endf = true;
	    }
	}
	}
      }
      loop++;
    }
  }
  catch( char *err ) {
    printf( "Catch an err %s\n", err );
    showV( "Err Params", PS, p );
    copyV( PS, op, p );
    showV( "Recover Params", PS, p );
    setABC( p );
    stat.sprintf( "Fitting Error : %1", loop );
    emit nowStat( stat );
    return false;
  }
  showV( "Normal Params", PS, p );

  for ( int i = 0; i < PS; i++ ) {
    delete [] M[i];
    delete [] I[i];
  }
  delete [] M;
  delete [] I;
  delete [] V;
  delete [] ff;
  delete [] zs;
  delete [] op;
  delete [] rs;
  delete [] r1s;

  return true;
}
