
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

  double Center;
  bool IsInt;

  bool HasSpeedsLine;
  bool HaveSetMaxS;
  int MaxS;      // max speed (pps) 最初に設定されていたオリジナルのスピード
  int MaxMaxS;   // 許される最高のスピード
  int HighS;
  int MiddleS;
  int LowS;
  
 public:
  AMotor( void );

  void init( Stars *s );
  virtual void init0( Stars *s );
  
  virtual void SetValue( double v );
  virtual void Stop( void );
  bool checkNewVal( void )
  {
    bool rv = ( value() != lastValue() );
    setLastValue( value() );
    return rv;
  };

  double u2p( double u ) { return u / getUPP() + Center; };
  double p2u( double p ) { return ( p - Center ) * getUPP(); };
  double any2p( double a, int selU, RELABS ra ) {
    return a / ( ( selU == 0 ) ? 1 : getUPP() )
      + ( ( ra == REL ) ? value().toDouble() : ( ( selU == 0 ) ? 0 : Center ) );
  }
  double getCenter( void ) { return Center; };
  void setCenter( QString center ) { Center = center.toDouble(); };
  void setCenter( double center ) { Center = center; };
  double metricValue( void ) { return ( value().toDouble() - Center ) * getUPP(); };
  void setIsInt( bool isInt ) { IsInt = isInt; };

  /* SPeed 設定関連 */
  virtual void SetSpeed( MSPEED speed );
  virtual void SetHighSpeed( int speed );
  virtual void SetMiddleSpeed( int speed );
  virtual void SetLowSpeed( int speed );
  bool hasSpeedsLine( void ) { return HasSpeedsLine; };
  void setHasSpeedsLine( bool f ) { HasSpeedsLine = f; };
  int highSpeed( void ) { return MaxS; };     // オリジナルのハイスピード
  void setHighSpeed( int h ) { MaxS = h; };
  int highestSpeed( void ) { return ( HasSpeedsLine ) ? MaxMaxS : MaxS; };// 許容最高速度
  void setHighestSpeed( int h ) { MaxMaxS = h; };
  int getHighS( void ) { return HighS; };
  int getMiddleS( void ) { return MiddleS; };
  int getLowS( void ) { return LowS; };
  virtual void AskHighSpeed( void );
  virtual void AskMiddleSpeed( void );
  virtual void AskLowSpeed( void );
  
  /* MStab 可の Unit は下記の関数を実装するべし */
  virtual void CloseShutter( bool /*close*/ ) {};
  virtual void GoMaxAbs( double /*start*/, double /*end*/, int /*steps*/ ) {};
  virtual void GoMaxAbsQ( double /*start*/, double /*end*/, int /*steps*/, double /*time*/ ) {};
  virtual void GoMaxRel( double /*width*/, int /*steps*/ ) {};
  virtual void GoMaxRelQ( double /*width*/, int /*steps*/, double /*time*/ ) {};

  /* PM16C だけかも */
  virtual void AssignDispCh( int ch );  // ch : 0 - 3 --> 'A' -- 'D'
  /* PM16C で QXAFS の為に */
  virtual void SetTimingOutMode( int mode );
  // 0 - 5 :: 0: none, 1: cont., 2: 200ns, 3: 10us, 4: 100us, 5: 1ms
  // 34410 triggers rising edge and requires 1us or longer
  // for EB741 2us is long enough
  virtual void SetTimingOutStart( int /*startP*/ ) {};      // start pos. of timing out
  virtual void SetTimingOutEnd( int /*endP*/ ) {};          // end pos. of timing out
  virtual void SetTimingOutInterval( int /*interval*/ ) {}; // timing out interval
  virtual void SetTimingOutReady( int /*ready*/ ) {};       // timing out ready
  virtual int accRate( void ) { return 1; };                // 加減速レート
  virtual void setAccRate( int /*r*/ ) {};
  virtual int accRateNo( void ) { return 0; };              // 加減速レートのテーブル番号
  virtual void setAccRateNo( int /*n*/ ) {};
  
  virtual void setMaxV( QString /*maxv*/ ) {};
  virtual void setMinV( QString /*minv*/ ) {};
};

#endif
