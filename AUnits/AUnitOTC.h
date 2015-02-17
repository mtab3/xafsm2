
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
  bool InitSensor( void );
  virtual bool _InitSensor( void );

  bool GetValue0( void );
  void AskIsBusy( void );
  virtual void _AskIsBusy( void );
  double SetTime( double dtime );

 private slots:
  void SetIsBusyByMsg( SMsg msg );
};

class AUnitOTC2 : public AUnitOTC
{
  Q_OBJECT

 public:
  AUnitOTC2( void );
  void init00( void );
  bool _InitSensor( void );
  
  void _AskIsBusy( void );
  bool isAutoRangeAvailable( void ) { return true; };
  bool GetRange( void );
  void SetRange( int range );

 private slots:
  void ReactGetRange( SMsg msg );
};





#endif
