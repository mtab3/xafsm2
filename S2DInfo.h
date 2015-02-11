#ifndef S2DINFO_H
#define S2DINFO_H

#include <QVector>

#include "XafsM.h"
#include "AUnits/AMotor.h"

enum DIRECTION { FORWARD, BACKWARD };
enum SCANMODE { STEP, QCONT, RCONT };

class S2DInfo
{
 public:

  bool valid;
  SCANMODE ScanMode;
  int motors;
  bool Use3rdAx;
  bool ScanBothDir;
  bool isSFluo;
  DIRECTION startDir; 
  QVector<AMotor*> unit;
  QVector<bool> used;
  QVector<int> now;
  QVector<int> i, ps;
  QVector<double> sx, ex, dx;
  double Dwell;
  QString MCAFile;
  QString SaveFile;
  int pps;               // use only in RCONT
  
  S2DInfo( void );
  void save( QTextStream &out );
  void load( QTextStream &in, QVector<AMotor*> &AMotors );
};

#endif
