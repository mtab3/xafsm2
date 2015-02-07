#ifndef MONINFO_H
#define MONINFO_H

#include <QVector>
#include <QTextStream>

#include "XafsM.h"
#include "AUnit.h"

#define MEASTIME  "MeasTime"
#define MONSENSOR "Sensor"

class MonInfo
{
 public:

  bool valid;
  QVector<AUnit*> Sensors;
  QStringList SensorNames;
  QStringList SensorUnits;
  double MeasTime;

  MonInfo( void );
  ~MonInfo( void );
  void save( QTextStream &out );
  bool load( QTextStream &in, QVector<AUnit*> &ASensors );
  void load0( QString line, QVector<AUnit*> &ASensors, bool &f );
  void show( void );
};

#endif
