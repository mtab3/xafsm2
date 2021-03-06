#ifndef KEV2PIX_H
#define KEV2PIX_H

#include <QObject>
#include <QVector>

#include "AUnits/AUnitXMAP.h"

class KeV2Pix : public QObject
{
  Q_OBJECT

 public:
  KeV2Pix();

  bool isValid( void ) { return valid; };
  void MakeUpAB( int len, int chs, int dim, QString fname );
  const QVector<double>& getAB( int i );
  int getDim( void ) { return dim; };
  double p2E( int ch, int p );
  int E2p( int ch, double E );

 private:
  bool valid;
  int dim;
  int MCALen;
  int MCAChs;
  QVector< QVector<double> > ab;   // 現状 ab は、Pixel -> keV への変換行列
  QVector< QVector<double> > ba;   // ba は、keV -> Pixel への変換行列にする予定。

  bool calcAB( QVector<double> &x, QVector<double> &y, double *ab );
};

#endif
