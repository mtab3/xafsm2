#ifndef S2DINFO_H
#define S2DINFO_H

#include <QVector>

#include "AUnit.h"

struct S2DInfo
{
 public:
  int motors;
  bool Use3rdAx;
  bool ScanBothDir;
  QVector<AUnit*> unit;
  QVector<bool> used;
  QVector<int> now;
  QVector<int> i, ps;
  QVector<double> sx, ex, dx;
  double Dwell;
};

#endif
