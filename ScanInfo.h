#ifndef SCANINFO_H
#define SCANINFO_H

#include <QVector>
#include <QTextStream>

#include "XafsM.h"
#include "AUnit.h"
#include "MonInfo.h"

#define UNITNAMES      "UnitNames"
#define UNITIDS        "UnitIDs"
#define NORMALIZE      "Normalize"
#define METRICUNIT     "MetricUnit"
#define UNIT_PULSE     "Unit/Pulse"
#define ORIG_POINT     "Orig.Point"
#define OFFSET         "Offset"
#define START_END_STEP "Start/End/Step"
#define RELABSSEL      "RelAbs"
#define SPEED          "Speed"
#define SCANDWELLTIME  "DwellTime"
#define USEMONITORS    "UseMonitors"

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

  QString goingP;
  QString goingPinU;
  
  AUnit *as;
  AUnit *as0;
  QString asName;
  QString as0Name;
  bool normalize;

  double origin;
  double offset;

  bool UseMonitors;
  MonInfo mi;

  ScanInfo( void );
  ~ScanInfo( void );
  void save( QTextStream &out );
  bool load( QTextStream &in, QVector<AUnit*> &AMotors, QVector<AUnit*> &ASensors );
  void show( void );
};

#endif
