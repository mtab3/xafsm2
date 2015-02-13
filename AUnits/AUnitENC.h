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
  bool InitSensor( void );
  virtual bool _InitSensor( void );

  void SetValue( double v );
};

class AUnitENC2 :public AUnitENC
{
  Q_OBJECT

 public:
  AUnitENC2( void );
  void init00( void );
  bool _InitSensor( void );

  double SetTime( double dtime );
  void AskIsBusy( void );
  bool QStart( void );
  bool QRead( void );

 private slots:
   //  void SetIsBusyByMsg( SMsg msg );
  void RcvQGetData( SMsg msg );
  void RcvStat( SMsg msg );
};

#endif




