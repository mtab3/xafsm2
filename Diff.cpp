
#include "Diff.h"

const double *Ws[ WTS ] = { W0, W1, W2 };

// y   : 微分される関数の値の列
// y1  : 微分した結果
// len : 関数列の点数
// w   : 微分をとるときの重み
// ws  : 重みの点数
//
// y1[i] = Σ{j=0}{j<ws} ( y[i+(j+1)]*w[j] - y[i-(j+1)]*w[j] )
//       w は微分点を中心に対象と考え、片側だけを与える
//       w さらに微分点の w は必ず 0 と考え、これも省く

void Diff( double *y, double *y1, int len, WTYPE wtype,
	   double *min, double *max )
{
  *min = 1e300;
  *max = -1e300;
  
  for ( int i = 0; i < len; i++ ) {
    y1[ i ] = 0;
    for ( int j = 0; j < WTs[ wtype ]; j++ ) {
      int j1 = i + ( j + 1 );
      int j2 = i - ( j + 1 );
      if ( j1 < len ) y1[i] += Ws[wtype][j] * y[ j1 ];
      if ( j2 >= 0 )  y1[i] -= Ws[wtype][j] * y[ j2 ];
    }
    if (( i > WTs[wtype] )&&( i < len - WTs[wtype] - 1 )) {
      if ( y1[i] > *max ) *max = y1[i];
      if ( y1[i] < *min ) *min = y1[i];
    }
  }
}

void Diff2( double *y, double *y1, double *y2, int len,
	    WTYPE wtype1, WTYPE wtype2,
	    double *min1, double *max1, double *min2, double *max2 )
{
  Diff( y, y1, len, wtype1, min1, max1 );
  Diff( y1, y2, len, wtype2, min2, max2 );
}
