
#include <stdio.h>

#include "Gs.h"

bool Gs::fit( int points, double *x, double *e, double *p,
	      int Loop, double damp, double prec1, double prec2 )
{
  int GS = n;
  int PS = GS * 3;

  int *L;
  double *op;
  double **M, **I;
  double *V;
  double *zs;
  double *rs, *r1s;
  
  // work の配列確保
  L = new int [ PS ];
  op = new double [ PS ];
  M = new double * [ PS ];
  I = new double * [ PS ];
  V = new double [ PS ];
  for ( int i = 0; i < PS; i++ ) {
    M[i] = new double [ PS ];
    I[i] = new double [ PS ];
  }
  zs = new double [ Loop + 10 ];
  rs = new double [ Loop + 10 ];
  r1s = new double [ Loop + 10 ];
  
  setABC( p );          // 初期値のパラメータセット
  bool endf = false;
  int loop = 0;

  la.copyV( PS, p, op );
  try {
    while ( ! endf ) {
      la.clearM( PS, M );
      la.setI( PS, I );
      la.clearV( PS, V );
      for ( int i = 0; i < points; i++ ) {
	for ( int j = 0; j < PS; j++ ) {
	  for ( int k = 0; k < PS; k++ ) {
	    M[j][k] += dp( k, x[i] ) * dp( j, x[i] );
	  }
	  V[j] += ( e[i] - f( x[i] ) ) * dp( j, x[i] );
	}
      }
      if ( la.checkMerr( PS, M ) ) throw (char*)"Err in M";
      if ( la.checkVerr( PS, V ) ) throw (char*)"Err in V";
      la.makeLU( PS, M, L );
      la.showM( "bM : ", PS, M );
      la.showM( "bI : ", PS, I );
      la.invert( PS, M, I );
      la.showM( "aM : ", PS, M );
      la.showM( "aI : ", PS, I );
      if ( la.checkMerr( PS, M ) ) throw (char*)"Err in calc I, M";
      if ( la.checkMerr( PS, I ) ) throw (char*)"Err in calc I, I";
      
      double *dp;
      dp = new double [ PS ];
      for ( int i = 0; i < PS; i++ ) {
	dp[i] = 0;
	for ( int j = 0; j < PS; j++ ) {
	  dp[i] += I[i][j] * V[j];
	}
      }
      if ( la.checkVerr( n, V ) ) throw (char*)"Err in dp";
      
      la.copyV( PS, p, op );
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
    la.showV( "Err Params", PS, p );
    la.copyV( PS, op, p );
    la.showV( "Recover Params", PS, p );
    setABC( p );
    stat.sprintf( "Fitting Error : %1", loop );
    emit nowStat( stat );
    return false;
  }
  la.showV( "Normal Params", PS, p );

  for ( int i = 0; i < PS; i++ ) {
    delete [] M[i];
    delete [] I[i];
  }
  delete [] L;
  delete [] M;
  delete [] I;
  delete [] V;
  delete [] zs;
  delete [] op;
  delete [] rs;
  delete [] r1s;

  return true;
}
