
#ifndef AUNITOTC_H
#define AUNITOTC_H

#include "ASensor.h"

class AUnitOTC : public ASensor
{
  Q_OBJECT

 public:
  AUnitOTC( void );
  void init0( void );
  virtual void init00( void );

  void AskIsBusy( void );
  virtual void _AskIsBusy( void );
};

class AUnitOTC2 : public AUnitOTC
{
  Q_OBJECT

 public:
  AUnitOTC2( void );
  void init00( void );
  
  void _AskIsBusy( void );
  bool isAutoRangeAvailable( void ) { return true; };
  bool GetRange( void );

 private slots:
  void ReactGetRange( SMsg msg );
};





#endif
