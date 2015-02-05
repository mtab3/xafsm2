#ifndef MONINFO_H
#define MONINFO_H

#include <QVector>
#include <QTextStream>

#include "XafsM.h"
#include "AUnit.h"

class MonInfo
{
 public:

  QVector<AUnit*> Sensors;
  QStringList SensorNames;
  QStringList SensorUnits;
  double MeasTime;

  MonInfo( void );
  ~MonInfo( void );
  void save( QTextStream &out );
  bool load( QTextStream &in, QVector<AUnit*> &AMotors );
};

#endif
