
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
  bool InitSensor( void );

  bool isAutoRangeAvailable( void ) { return true; };
  bool GetValue( void );
  void SetRange( int range );
  virtual void _SetRange( int range );
  virtual bool _GetValue( void );
  double SetTime( double dtime );
  virtual void _SetTime( double t );
};

class AUnitPAM2 : public AUnitPAM
{
  Q_OBJECT

 public:
  AUnitPAM2( void ) {};
  void init00( void );

  void _SetRange( int range );
  bool _GetValue( void );
  void _SetTime( double t );

 private slots:
  void RcvAnsGetValueOfDriver( SMsg msg );
};

#endif
