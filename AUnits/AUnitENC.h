#ifndef AUNITENC_H
#define AUNITENC_H

#include "ASensor.h"

class AUnitENC : public ASensor
{
  Q_OBJECT

 public:
  AUnitENC( void );

  virtual void init00( Stars *s );
};

class AUnitENC2 :public AUnitENC
{
  Q_OBJECT

 public:
  AUnitENC2( void );
};

#endif
