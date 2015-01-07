
#include <stdlib.h>
#include <stdio.h>

#include "Gs.h"

bool Gs::fit( int points, double *x, double *e, double *p,
	      int Loop, double damp, double prec1, double prec2 )
{
  int GS = n;
  int PS = GS * 3;

  int *idx;
  double *op;
  // double **I;
  double **M, **L, **U;
  double *V, *R1, *R2;
  double *zs;
  double *rs, *r1s;
  
  // work の配列確保
  idx = new int [ PS ];
  op = new double [ PS ];
  M = new double * [ PS ];
  //  I = new double * [ PS ];
  L = new double * [ PS ];
  U = new double * [ PS ];
  V = new double [ PS ];
  R1 = new double [ PS ];
  R2 = new double [ PS ];
  for ( int i = 0; i < PS; i++ ) {
    M[i] = new double [ PS ];
    //    I[i] = new double [ PS ];
    L[i] = new double [ PS ];
    U[i] = new double [ PS ];
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
      //      la.setI( PS, I );
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
      la.makeLU( PS, M, L, U, idx );
      if ( la.checkMerr( PS, L ) ) throw (char*)"Err in calc I, L";
      if ( la.checkMerr( PS, L ) ) throw (char*)"Err in calc I, U";
      la.solveLV( PS, L, V, R1, idx );
      if ( la.checkVerr( PS, R1 ) ) throw (char*)"Err R1";
      la.solveUV( PS, U, R1, R2, idx );
      if ( la.checkVerr( PS, R2 ) ) throw (char*)"Err R2";
      
      la.copyV( PS, p, op );
      for ( int i = 0; i < PS; i++ ) {
	p[i] += R2[idx[i]] * damp;             // LU 分解型
      }
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
    //    printf( "Catch an err %s\n", err );
    //    la.showV( "Err Params", PS, p );
    //    la.copyV( PS, op, p );
    //    la.showV( "Recover Params", PS, p );
    setABC( p );
    stat.sprintf( "Fitting Error : %d", loop );
    emit nowStat( stat );
    return false;
  }
  //  la.showV( "Normal Params", PS, p );

  for ( int i = 0; i < PS; i++ ) {
    delete [] M[i];
    //    delete [] I[i];
    delete [] L[i];
    delete [] U[i];
  }
  delete [] idx;
  delete [] M;
  //  delete [] I;
  delete [] L;
  delete [] U;
  delete [] V;
  delete [] zs;
  delete [] op;
  delete [] rs;
  delete [] r1s;

  return true;
}
