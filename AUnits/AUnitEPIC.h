
#ifndef AUNITEPIC_H
#define AUNITEPIC_H

#include "ASensor.h"

class AUnitEPIC : public ASensor
{
  Q_OBJECT

 public:
  AUnitEPIC( void );
  void init0( void );
};

#endif
