
#ifndef PEAKFIT_H
#define PEAKFIT_H

#include <QObject>
#include "MCAPeak.h"

class PeakFit : public QObject
{
  Q_OBJECT

  bool dataValid;
  
  QVector<MCAPeak> *peaks;
  int len, ps;
  double *x, *y;
  double **lines;

  void GenPeaks();

 public:
  PeakFit( QObject *p = NULL ) ;

  void init( QVector<MCAPeak> *Peaks, int DLength, double *X, double *Y );
  double **fit( int *peaks );

};

#endif
