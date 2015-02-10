#ifndef MONINFO_H
#define MONINFO_H

#include <QVector>
#include <QTextStream>

#include "XafsM.h"
#include "AUnits/ASensor.h"

#define MEASTIME  "MeasTime"
#define MONSENSOR "Sensor"

class MonInfo
{
 public:

  bool valid;
  QVector<ASensor*> Sensors;
  QStringList SensorNames;
  QStringList SensorUnits;
  double MeasTime;

  MonInfo( void );
  ~MonInfo( void );
  void save( QTextStream &out );
  bool load( QTextStream &in, QVector<ASensor*> &ASensors );
  void load0( QString line, QVector<ASensor*> &ASensors, bool &f );
  void show( void );
};

#endif
