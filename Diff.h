#ifndef DIFF_H
#define DIFF_H

enum WTYPE { WT1 = 0, WT2, WT3, WTEND };

void Diff( double *y, double *y1, int len, 
	   WTYPE wtype, 
	   double *min, double *max );
void Diff2( double *y, double *y1, double *y2, int len,
	    WTYPE wtype1, WTYPE wtype2,
	    double *min1, double *max1, double *min2, double *max2 );

const int WS0 = 1;
const double W0[ WS0 ] = { 1 };
const int WS1 = 2;
const double W1[ WS1 ] = { 2, 1 };
const int WS2 = 5;
const double W2[ WS2 ] = { 60, 30, 20, 15, 12 };

const int WTS = WTEND;
const int WTs[ WTS ] = { WS0, WS1, WS2 };
extern const double *Ws[ WTS ];

#endif
