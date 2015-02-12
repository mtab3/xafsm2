
#ifndef AUNITPAM_H
#define AUNITPAM_H

#include "ASensor.h"

class AUnitPAM : public ASensor
{
  Q_OBJECT

 public:
  AUnitPAM( void ) {};

  void init0( void );
  virtual void init00( void );

  bool isAutoRangeAvailable( void ) { return true; };
  bool GetValue( void );
};

class AUnitPAM2 : public AUnitPAM
{
  Q_OBJECT

 public:
  AUnitPAM2( void ) {};
  void init00( void );

};

#endif
