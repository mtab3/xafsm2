#ifndef SCANINFO_H
#define SCANINFO_H

#include <QVector>

#include "XafsM.h"
#include "AUnit.h"

struct ScanInfo
{
 public:

  bool valid;
  AUnit *unit;
  double sx, ex, dx;
};

#endif
