
#ifndef AUNITCCG_H
#define AUNITCCG_H

#include "ASensor.h"

class AUnitCCG : public ASensor
{
  Q_OBJECT

 public:
  AUnitCCG( void );

  void init0( void );

 private slots:
   //  void SetCurPos( SMsg msg );
};

#endif
