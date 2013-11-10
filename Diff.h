#ifndef DIFF_H
#define DIFF_H

enum WTYPE { WT1 = 0, WT2, WT3, WTEND };

void Smooth( bool intf, void *y, double *y1, int len, int m, double *weight );

void Diff( bool intf,
	   void *y, double *y1, int len, 
	   WTYPE wtype, 
	   double *min, double *max,
	   int minI, int maxI );

void Diff2( bool intf, 
	    void *y, double *y1, double *y2, int len,
	    WTYPE wtype1, WTYPE wtype2,
	    double *min1, double *max1, double *min2, double *max2,
	    int minI, int maxI );

void Diff3( bool intf,
	    void *y, double *y1, double *y2, double *y3, int len,
	    WTYPE wtype1, WTYPE wtype2, WTYPE wtype3, 
	    double *min1, double *max1,
	    double *min2, double *max2,
	    double *min3, double *max3,
	    int minI, int maxI );

void SDiff( bool itnf, 
	    void *y, double *y1, int len, int smoothL,
	    WTYPE wtype, 
	    double *min, double *max,
	    int minI, int maxI );

void SDiff2( bool intf,
	     void *y, double *y1, double *y2, int len, int smoothL,
	     WTYPE wtype1, WTYPE wtype2, 
	     double *min1, double *max1,
	     double *min2, double *max2,
	     int minI, int maxI );

void SDiff3( bool intf,
	     void *y, double *y1, double *y2, double *y3, int len, int smoothL,
	     WTYPE wtype1, WTYPE wtype2, WTYPE wtype3, 
	     double *min1, double *max1,
	     double *min2, double *max2,
	     double *min3, double *max3,
	     int minI, int maxI );

void SDiff4( bool intf,
	     void *y, double *y1, double *y2, double *y3, double *y4,
	     int len, int smoothL,
	     WTYPE wtype1, WTYPE wtype2, WTYPE wtype3, WTYPE wtype4,
	     double *min1, double *max1,
	     double *min2, double *max2,
	     double *min3, double *max3,
	     double *min4, double *max4,
	     int minI, int maxI );

const int WS0 = 1;
const double W0[ WS0 ] = { 1 };
const int WS1 = 2;
const double W1[ WS1 ] = { 2, 1 };
const int WS2 = 5;
const double W2[ WS2 ] = { 60, 30, 20, 15, 12 };

const int WTS = WTEND;
const int WTs[ WTS ] = { WS0, WS1, WS2 };
extern const double *Ws[ WTS ];
const int WsTotal[ WTS ] = { 2, 6, 274 };

#endif
