#ifndef SCANINFO_H
#define SCANINFO_H

#include <QVector>
#include <QTextStream>

#include "XafsM.h"
#include "AUnit.h"

class ScanInfo
{
 public:

  bool valid;

  AUnit *am;
  QString amName;

  MSPEED speed;
  int showUnit;
  double upp;
  QString unitName;
  RELABS relabs;

  QString sx0, ex0, dx0, dt0;
  double sx, ex, dx, dt;

  AUnit *as;
  AUnit *as0;
  QString asName;
  QString as0Name;
  bool normalize;

  double origin;
  double offset;

  ScanInfo( void );
  ~ScanInfo( void );
  void save( QTextStream &out );
  bool load( QTextStream &in, QVector<AUnit*> &AMotors );
};

#endif
