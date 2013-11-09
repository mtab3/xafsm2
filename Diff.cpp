
#include <stdlib.h>
#include "Diff.h"

// ñ�ʤ��ưʿ�� (-m <= i <= m)

void Smooth( bool intf, void *y, double *y1, int len, int m, double *W )
{
  double w;
  double y0;
  for ( int i = 0; i < len; i++ ) {
    double s = 0, ws = 0;
    for ( int j = -m; j <= m; j++ ) {
      int p = i+j;
      if (( p >= 0 )&&( p < len )) {
	w = ( W == NULL ) ? 1 : W[ abs( m ) ];
	ws += w;
	if ( intf ) {
	  y0 = (double)(((int *)y)[p]);
	} else {
	  y0 = (double)(((double *)y)[p]);
	}
	s += y0 * w;
      }
      if ( ws > 0 )
	y1[i] = s / ws;
      else 
	y1[i] = 0;
    }
  }
}

// y   : ��ʬ�����ؿ����ͤ���
// y1  : ��ʬ�������
// len : �ؿ��������
// w   : ��ʬ��Ȥ�Ȥ��νŤ�
// ws  : �Ťߤ�����
//
// y1[i] = ��{j=0}{j<ws} ( y[i+(j+1)]*w[j] - y[i-(j+1)]*w[j] )
//       w ����ʬ�����濴���оݤȹͤ�����¦������Ϳ����
//       w �������ʬ���� w ��ɬ�� 0 �ȹͤ��������ʤ�

const double *Ws[ WTS ] = { W0, W1, W2 };

void Diff( bool intf, void *y, double *y1, int len, WTYPE wtype,
	    double *min, double *max, int minI, int maxI )
{
  *min = 1e300;
  *max = -1e300;
  double wt = WsTotal[wtype];
  double y01, y02;
  
  for ( int i = 0; i < len; i++ ) {
    y1[ i ] = 0;
    for ( int j = 0; j < WTs[ wtype ]; j++ ) {
      int j1 = i + ( j + 1 );
      int j2 = i - ( j + 1 );
      if (( j1 < len )&&( j2 >= 0 )) {
	if ( intf ) {
	  y01 = (double)(((int *)y)[j1]);
	  y02 = (double)(((int *)y)[j2]);
	} else {
	  y01 = (double)(((double *)y)[j1]);
	  y02 = (double)(((double *)y)[j2]);
	}
	y1[i] += Ws[wtype][j] * y01;
	y1[i] -= Ws[wtype][j] * y02;
      }
    }
    y1[i] /= wt;
    if (( i > WTs[wtype] )&&( i < len - WTs[wtype] - 1 )&&( i >= minI)&&( i <= maxI )) {
      if ( y1[i] > *max ) *max = y1[i];
      if ( y1[i] < *min ) *min = y1[i];
    }
  }
}

void Diff2( bool intf, void *y, double *y1, double *y2, int len,
	    WTYPE wtype1, WTYPE wtype2,
	    double *min1, double *max1, double *min2, double *max2,
	    int minI, int maxI )
{
  Diff( intf, y, y1, len, wtype1, min1, max1, minI, maxI );
  Diff( false, y1, y2, len, wtype2, min2, max2, minI, maxI );
}

void Diff3( bool intf, void *y, double *y1, double *y2, double *y3, int len,
	    WTYPE wtype1, WTYPE wtype2, WTYPE wtype3, 
	    double *min1, double *max1,
	    double *min2, double *max2,
	    double *min3, double *max3,
	    int minI, int maxI )
{
  Diff( intf, y, y1, len, wtype1, min1, max1, minI, maxI );
  Diff( false, y1, y2, len, wtype2, min2, max2, minI, maxI );
  Diff( false, y2, y3, len, wtype3, min3, max3, minI, maxI );
}

void SDiff( bool intf, void *y, double *y1, int len, int sl, 
	    WTYPE wtype, double *min, double *max, int minI, int maxI )
{
  double *sy = new double [ len ];

  Smooth( intf, y, sy, len, sl, NULL );
  Diff( false, sy, y1, len, wtype, min, max, minI, maxI );

  delete [] sy;
}

void SDiff2( bool intf, void *y, double *y1, double *y2, int len, int sl,
	     WTYPE wtype1, WTYPE wtype2, 
	     double *min1, double *max1,
	     double *min2, double *max2,
	     int minI, int maxI )
{
  SDiff( intf, y, y1, len, sl, wtype1, min1, max1, minI, maxI );
  SDiff( false, y1, y2, len, sl, wtype2, min2, max2, minI, maxI );
}

void SDiff3( bool intf, void *y, double *y1, double *y2, double *y3, int len, int sl,
	       WTYPE wtype1, WTYPE wtype2, WTYPE wtype3, 
	       double *min1, double *max1,
	       double *min2, double *max2,
	       double *min3, double *max3,
	       int minI, int maxI )
{
  SDiff( intf, y, y1, len, sl, wtype1, min1, max1, minI, maxI );
  SDiff( false, y1, y2, len, sl, wtype2, min2, max2, minI, maxI );
  SDiff( false, y2, y3, len, sl, wtype3, min3, max3, minI, maxI );
}
