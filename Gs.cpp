
#include "stdio.h"

#include "Gs.h"

void Gs::fit( int points, double *x, double *e, double *p, int Loop )
{
  int GS = n;
  int PS = GS * 3;

  double **M, **I;
  double *V;

  M = new double * [ PS ];
  I = new double * [ PS ];
  V = new double [ PS ];
  for ( int i = 0; i < PS; i++ ) {
    M[i] = new double [ PS ];
    I[i] = new double [ PS ];
  }
  
  setABW( p );
  for( int loop = 0; loop < Loop; loop++ ) {
    for ( int i = 0; i < PS; i++ ) {
      for ( int j = 0; j < PS; j++ ) {
	M[i][j] = 0;
	I[i][j] = 0;
      }
      V[i] = 0;
      I[i][i] = 1;
    }
    
    for ( int i = 0; i < points; i++ ) {
      for ( int j = 0; j < PS; j++ ) {
	for ( int k = 0; k < PS; k++ ) {
	  M[j][k] += dp( k, x[i] ) * dp( j, x[i] );
	}
	V[j] += ( e[i] - f( x[i] ) ) * dp( j, x[i] );
      }
    }

    printf( "MCALen = %d\n", points );
    printf( "E = \n" );
    for ( int i = 0; i < points; i++ ) {
      printf( "%f ", e[i] );
    }

    printf( "P = \n" );
    for ( int i = 0; i < PS && i < 60; i++ ) {
      printf( "%f\t", p[i] );
    }
    printf( "\n" );
    printf( "M = \n" );
    for ( int i = 0; i < PS; i++ ) {
      for ( int j = 0; j < PS; j++ ) {
	printf( "%g\t", M[i][j] );
      }
      printf( "\n" );
    }
    printf( "\n" );
    printf( "V = \n" );
    for ( int i = 0; i < PS; i++ ) {
      printf( "%g\t", V[i] );
    }
    printf( "\n" );


    
    
    for ( int i = 0; i < PS; i++ ) {
      double a = M[i][i];
      for ( int j = 0; j < PS; j++ ) {
	M[i][j] /= a;
	I[i][j] /= a;
      }
      for ( int j = 0; j < PS; j++ ) {
	if ( j != i ) {
	  double b = M[j][i];
	  for ( int k = 0; k < PS; k++ ) {
	    M[j][k] -= M[i][k] * b;
	    I[j][k] -= I[i][k] * b;
	  }
	}
      }
    }
    
    double *dp;
    dp = new double [ PS ];
    for ( int i = 0; i < PS; i++ ) {
      dp[i] = 0;
      for ( int j = 0; j < PS; j++ ) {
	dp[i] += I[i][j] * V[j];
      }
    }

    
    printf( "dP = \n" );
    for ( int i = 0; i < PS; i++ ) {
      printf( "%f\t", dp[i] );
    }
    printf( "\n" );
    printf( "I = \n" );
    for ( int i = 0; i < PS; i++ ) {
      for ( int j = 0; j < PS; j++ ) {
	printf( "%f\t", I[i][j] );
      }
      printf( "\n" );
    }
    printf( "\n" );

    
    
    for ( int i = 0; i < PS; i++ ) {
      p[i] += dp[i];
    }
    delete [] dp;

    setABW( p );
  }

  for ( int i = 0; i < PS; i++ ) {
    delete [] M[i];
    delete [] I[i];
  }
  delete [] M;
  delete [] I;
  delete [] V;
}
