#ifndef CHECKUNITS_H
#define CHECKUNITS_H

#include <QObject>

#include "AUnits/AUnits.h"

struct CUnit {
  AUnit0 *au;
  bool b1f;
  bool b2f;
};

class CheckUnits : public QObject
{
  Q_OBJECT

  QVector<CUnit*> units;

public:
  CheckUnits() {};

  void clear( void );
  void add( AUnit0 *p, bool b1, bool b2 );
  bool isBusy( void );
};

#endif
