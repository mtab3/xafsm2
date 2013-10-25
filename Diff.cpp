
#include "Diff.h"

#include <stdio.h>

// y   : ��ʬ�����ؿ����ͤ���
// y1  : ��ʬ�������
// len : �ؿ��������
// w   : ��ʬ��Ȥ�Ȥ��νŤ�
// ws  : �Ťߤ�����
//
// y1[i] = ��{j=0}{j<ws} ( y[i+(j+1)]*w[j] - y[i-(j+1)]*w[j] )
//       w ����ʬ�����濴���оݤȹͤ�����¦������Ϳ����
//       w �������ʬ���� w ��ɬ�� 0 �ȹͤ��������ʤ�

void Diff( double *y, double *y1, int len,
	   const double *w, int ws,
	   double *min, double *max )
{
  *min = 1e300;
  *max = -1e300;
  
  for ( int i = 0; i < len; i++ ) {
    y1[ i ] = 0;
    for ( int j = 0; j < ws; j++ ) {
      int j1 = i + ( j + 1 );
      int j2 = i - ( j + 1 );
      if ( j1 < len ) y1[i] += w[j] * y[ j1 ];
      if ( j2 >= 0 )  y1[i] -= w[j] * y[ j2 ];
    }
    if ( y1[i] > *max ) *max = y1[i];
    if ( y1[i] < *min ) *min = y1[i];
  }
}

void Diff2( double *y, double *y1, double *y2, int len,
	    const double *w1, int ws1, const double *w2, int ws2,
	    double *min1, double *max1, double *min2, double *max2 )
{
  Diff( y, y1, len, w1, ws1, min1, max1 );
  Diff( y1, y2, len, w2, ws2, min2, max2 );
  for ( int i = 0; i < len; i++ ) {
    printf( "diff %d %g %g %g\n", i, y[i], y1[i], y2[i] );
  }
  printf( "     min max %g %g %g %g\n", *min1, *max1, *min2, *max2 );
}
