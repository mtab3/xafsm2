#ifndef AUNITPM_H
#define AUNITPM_H

#include "AMotor.h"

class AUnitPM : public AMotor
{
  Q_OBJECT

  int AccRate;   // $B2C8:B.%l!<%H(B ( AccRage ms/1000pps )
  int AccRateNo; // $BBP1~$9$k2C8:B.%l!<%H$N(B PM16C $B$N%F!<%V%kHV9f(B

 public:
  AUnitPM( void );
  void init0( void );

  void SetValue( double v );
  void SetSpeed( MSPEED speed );
  void SetHighSpeed( int speed );
  void SetMiddleSpeed( int speed );
  void SetLowSpeed( int speed );
  void Stop( void );
  void AskHighSpeed( void );
  void AskMiddleSpeed( void );
  void AskLowSpeed( void );
  void AssignDispCh( int ch );
  void SetTimingOutMode( int mode );
  void SetTimingOutStart( int startP );
  void SetTimingOutEnd( int endP );
  void SetTimingOutInterval( int interval );
  void SetTimingOutReady( int ready );
  void AskIsBusy( void );
  
  int accRate( void ) { return AccRate; };    // $B2C8:B.%l!<%H(B
  void setAccRate( int r ) { AccRate = r; };
  int accRateNo( void ) { return AccRateNo; }; // $B2C8:B.%l!<%H$N%F!<%V%kHV9f(B
  void setAccRateNo( int n ) { AccRateNo = n; };

 private slots:
   //  void SetIsBusyByMsg( SMsg msg );
  void RcvHighSpeed( SMsg msg );
  void RcvMiddleSpeed( SMsg msg );
  void RcvLowSpeed( SMsg msg );
  //  void RcvAnsGetValueOfDriver( SMsg msg );

 signals:
  void gotHighS( int s );
  void gotMiddleS( int s );
  void gotLowS( int s );
};

#endif



