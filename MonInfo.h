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
  double MeasTime;

  MonInfo( void );
  ~MonInfo( void );
  void save( QTextStream &out );
  void load( QTextStream &in, QVector<AUnit*> &AMotors );
};

#endif
