#ifndef AUNITENC_H
#define AUNITENC_H

#include "ASensor.h"

class AUnitENC : public ASensor
{
  Q_OBJECT

 public:
  AUnitENC( void );

  void init0( void );
  virtual void init00( void );
  void SetValue( double v );
};

class AUnitENC2 :public AUnitENC
{
  Q_OBJECT

 public:
  AUnitENC2( void );
  void init00( void );

  void AskIsBusy( void );
  bool QStart( void );
  bool QRead( void );
};

#endif




