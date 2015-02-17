#ifndef AUNITSC_H
#define AUNITSC_H

#include "AMotor.h"

class AUnitSC : public AMotor
{
  Q_OBJECT

 public:
  AUnitSC( void );
  void init0( void );

  void SetValue( double v );
  bool GetValue( void );
  void Stop( void );
  void AskIsBusy( void );

 private slots:
  void SetCurPos( SMsg msg );
  //  void SetIsBusyByMsg( SMsg msg );
};

#endif




