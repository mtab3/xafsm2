#ifndef AUNITPM_H
#define AUNITPM_H

#include "AMotor.h"

class AUnitPM : public AMotor
{
  Q_OBJECT

  int AccRate;   // 加減速レート ( AccRage ms/1000pps )
  int AccRateNo; // 対応する加減速レートの PM16C のテーブル番号

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
  
  int accRate( void ) { return AccRate; };    // 加減速レート
  void setAccRate( int r ) { AccRate = r; };
  int accRateNo( void ) { return AccRateNo; }; // 加減速レートのテーブル番号
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



