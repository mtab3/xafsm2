
#ifndef AMOTOR_H
#define AMOTOR_H

#include "../RelAbs.h"
#include "AUnit0.h"

enum MSPEED { LOW, MIDDLE, HIGH, MSPEEDS };
struct MSPEEDD {
  MSPEED MSid;
  const QString MSName;
};

const MSPEEDD MSpeeds[ MSPEEDS ] = {
  { LOW,    "Low" },
  { MIDDLE, "Middle" },
  { HIGH,   "High" },
};

class AMotor : public AUnit0
{
  Q_OBJECT

 protected:
  double Center;
  bool IsInt;

  bool HasSpeedsLine;
  bool HasSetMaxS;
  int MaxS;      // max speed (pps) 最初に設定されていたオリジナルのスピード
  int MaxMaxS;   // 許される最高のスピード
  int HighS;
  int MiddleS;
  int LowS;
  
 public:
  AMotor( void );

  void init( void );
  virtual void init0( void ) {};
  
  virtual void Stop( void ) {};
  bool checkNewVal( void )
  {
    bool rv = ( value() != lastValue() );
    setLastValue( value() );
    return rv;
  };

  double u2p( double u ) { return u / UPP + Center; };
  double p2u( double p ) { return ( p - Center ) * UPP; };
  double any2p( double a, int selU, RELABS ra ) {
    return a / ( ( selU == 0 ) ? 1 : UPP )
      + ( ( ra == REL ) ? Value.toDouble() : ( ( selU == 0 ) ? 0 : Center ) );
  }
  double getCenter( void ) { return Center; };
  void setCenter( QString center ) { Center = center.toDouble(); };
  void setCenter( double center ) { Center = center; };
  double metricValue( void ) { return ( Value.toDouble() - Center ) * UPP; };
  void setIsInt( bool isInt ) { IsInt = isInt; };
  bool isInt( void ) { return IsInt; };

  /* SPeed 設定関連 */
  virtual void SetSpeed( MSPEED /* speed */ ) {};
  virtual void SetHighSpeed( int /* speed */ ) {};
  virtual void SetMiddleSpeed( int /* speed */ ) {};
  virtual void SetLowSpeed( int /* speed */ ) {};
  bool hasSpeedsLine( void ) { return HasSpeedsLine; };
  void setHasSpeedsLine( bool f ) { HasSpeedsLine = f; };
  int highSpeed( void ) { return MaxS; };     // オリジナルのハイスピード
  void setHighSpeed( int h ) { MaxS = h; };
  int highestSpeed( void ) { return ( HasSpeedsLine ) ? MaxMaxS : MaxS; };// 許容最高速度
  void setHighestSpeed( int h ) { MaxMaxS = h; };
  int getHighS( void ) { return HighS; };
  int getMiddleS( void ) { return MiddleS; };
  int getLowS( void ) { return LowS; };
  virtual void AskHighSpeed( void ) {};
  virtual void AskMiddleSpeed( void ) {};
  virtual void AskLowSpeed( void ) {};

  /* MStab 可の Unit は下記の関数を実装するべし */
  virtual bool canBeMStab( void ) { return false; };
  virtual void CloseShutter( bool /*close*/ ) {};
  virtual void GoMaxAbs( double /*start*/, double /*end*/, int /*steps*/ ) {};
  virtual void GoMaxAbsQ( double /*start*/, double /*end*/, int /*steps*/, double /*time*/ ) {};
  virtual void GoMaxRel( double /*width*/, int /*steps*/ ) {};
  virtual void GoMaxRelQ( double /*width*/, int /*steps*/, double /*time*/ ) {};

  virtual void SetUpToGenerageTriggerSignal( int /*sp*/, int /*ep*/, int /*interval*/ ) {}; // QXafs
  virtual void TriggerOff( void ) {};
  virtual int accRate( void ) { return 1; };                // 加減速レート
  virtual void setAccRate( int /*r*/ ) {};
  virtual int accRateNo( void ) { return 0; };              // 加減速レートのテーブル番号
  virtual void setAccRateNo( int /*n*/ ) {};

 public slots:
};

#endif
