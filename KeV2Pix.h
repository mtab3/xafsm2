#ifndef KEV2PIX_H
#define KEV2PIX_H

#include <QObject>
#include <QVector>

#include "XafsM.h"

class KeV2Pix : public QObject
{
  Q_OBJECT

 public:
  KeV2Pix();
  
  void MakeUpAB( int dim );
  const QVector<double>& getAB( int i );
  int getDim( void ) { return dim; };
  double p2E( int i, int p );
  int E2p( int i, double E );
  void setMCALen( int mcaLen ) { MCALen = mcaLen; };

 private:
  int dim;
  int MCALen;
  QVector<double> ab[ MaxSSDs ];

  bool calcAB( QVector<double> &x, QVector<double> &y, double *ab );
};

#endif
