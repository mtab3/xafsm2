
#include <math.h>
#include <stdio.h>

#include <QVector>
#include <QDebug>

#include "PeakFit.h"

PeakFit::PeakFit( QObject *pp ) : QObject( pp )
{
  dataValid = false;
  len = 0;
  ps = 0;
  x = y = NULL;
  lines = NULL;
}

void PeakFit::init( QVector<MCAPeak> *Peaks, int DLength, double *X, double *Y )
{
  if ( lines != NULL ) {
    for ( int i = 0; i < ps + 1; i++ )
      delete [] lines[i];
    delete [] lines;
  }

  peaks = Peaks;
  ps = peaks->count();

  len = DLength;
  x = X;
  y = Y;

  lines = new double* [ ps + 1 ];
  for ( int i = 0; i < ps + 1; i++ ) {
    lines[i] = new double [ len ];
  }

  dataValid = true;
}

double **PeakFit::fit( int *Ps )
{
  *Ps = ps;

  GenPeaks();

  return lines;
}

void PeakFit::GenPeaks( void )
{
  for ( int j = 0; j < len; j++ ) {
    lines[0][j] = 0;
    for ( int i = 0; i < ps; i++ ) {
      double x0 = (*peaks)[i].center;
      double w = (*peaks)[i].center - (*peaks)[i].start;
      lines[i+1][j] = (*peaks)[i].peakH
	* exp( - ( j - x0 ) * ( j - x0 ) / ( w * w ) * 4 );
      lines[0][j] += lines[i+1][j];
    }
  }
}
