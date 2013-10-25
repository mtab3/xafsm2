#ifndef DIFF_H
#define DIFF_H

void Diff( double *y, double *y1, int len, const double *w, int ws,
	   double *min, double *max );
void Diff2( double *y, double *y1, double *y2, int len,
	    const double *w1, int ws1, const double *w2, int ws2,
	    double *min1, double *max1, double *min2, double *max2 );

const int WS0 = 2;
const double W0[ WS0 ] = { 1, 2 };
const int WS1 = 5;
const double W1[ WS1 ] = { 60, 30, 20, 15, 12 };

#endif
