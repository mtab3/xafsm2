
#ifndef AUNITDV_H
#define AUNITDV_H

#include "ASensor.h"

class AUnitDV : public ASensor
{
  Q_OBJECT

 protected:
  int points;           // Measured Data Points : 34410
  bool HasMaxIntTime;
  double MaxIntTime;    // Maximum integration time
  
 public:
  AUnitDV( void );
  void init0( void );
  virtual void init00( void );
  virtual void init000( void ) {};
  bool InitSensor( void );
  virtual bool _InitSensor( void );
  
  double SetTime( double dtime ) { return _SetTime( dtime ); };
  virtual double _SetTime( double dtime );
  bool GetValue0( void ) { return _GetValue0(); };
  virtual bool _GetValue0( void ) { return false; };

  bool hasMaxIntTime( void ) { return HasMaxIntTime; };
  double maxIntTime( void ) { return MaxIntTime; };
  void setMaxIntTime( double t ) { MaxIntTime = t; HasMaxIntTime = true; };
  int Points( void ) { return points; };
  
  bool QStart( void );
  bool QRead( void );
  bool QEnd( void );

  void SetTriggerDelay( double time );
  void SetSamplingSource( QString source );
  void SetTriggerSource( QString source );
  void SetTriggerCounts( int counts );
  void SetTriggerSlope( QString type );
  void GetDataPoints( void );
  void ReadDataPoints( int datas );
  void Abort( void );

 private slots:
   //  void SetIsBusyByMsg( SMsg msg );
  void RcvQGetData( SMsg msg );
};

class AUnitDV2 : public AUnitDV
{
  Q_OBJECT

 public:
  AUnitDV2( void ) {};
  void init00( void );
  bool _InitSensor( void );
  double _SetTime( double dtime );

  void AskIsBusy( void );
};

class AUnitDV3 : public AUnitDV
{
  Q_OBJECT

 public:
  AUnitDV3( void ) {};
  void init000( void );

  bool _GetValue0( void );
  double _SetTime( double dtime );

private slots:
  void SetIsBusyByMsg( SMsg msg );
};

#endif




