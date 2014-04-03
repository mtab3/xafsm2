#ifndef SCANINFO_H
#define SCANINFO_H

#include <QVector>

#include "XafsM.h"
#include "AUnit.h"

struct ScanInfo
{
 public:

  bool valid;
  AUnit *am;
  AUnit *as;
  AUnit *as0;
  RELABS relabs;
  bool normalize;
  int showUnit;
  QString unitName;
  double upp;
  int speed;
  double origin;
  double offset;
  QString sx0, ex0, dx0, dwell0;
  double sx, ex, dx, dwell;
};

#endif
