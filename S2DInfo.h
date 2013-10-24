#ifndef S2DINFO_H
#define S2DINFO_H

#include <QVector>

#include "XafsM.h"
#include "AUnit.h"

struct S2DInfo
{
 public:

  bool valid;
  SCANMODE ScanMode;
  int motors;
  bool Use3rdAx;
  bool ScanBothDir;
  bool isSFluo;
  DIRECTION startDir; 
  QVector<AUnit*> unit;
  QVector<bool> used;
  QVector<int> now;
  QVector<int> i, ps;
  QVector<double> sx, ex, dx;
  double Dwell;
  QString MCAFile;
  QString SaveFile;
  int pps;               // use only in RCONT
};

#endif
