
#ifndef AUNITCNT_H
#define AUNITCNT_H

#include "ASensor.h"

class AUnitCNT : public ASensor
{
  Q_OBJECT

 public:
  AUnitCNT( void ) {};
  void init0( void );
  virtual void init00( void );
  bool InitSensor( void );
  virtual bool _InitSensor( void );
  
  void AskIsBusy( void );
  double SetTime( double dtime );
  virtual void _AskIsBusy( void );
  bool GetValue0( void );
  bool GetValue02( void );

  bool Close( void );

 private slots:
   void SetIsBusyByMsg( SMsg msg );
};

class AUnitCNT2 : public AUnitCNT
{
  Q_OBJECT

 public:
  AUnitCNT2( void ) {};

  void init00( void );
  bool _InitSensor( void );

  void _AskIsBusy( void ) {};

  bool isAutoRangeAvailable( void ) { return true; }
  bool GetRange( void );
  void SetRange( int range );

 private slots:
  void ReactGetRange( SMsg msg );

};

#endif
