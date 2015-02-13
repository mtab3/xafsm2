
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
  int MaxS;      // max speed (pps) $B:G=i$K@_Dj$5$l$F$$$?%*%j%8%J%k$N%9%T!<%I(B
  int MaxMaxS;   // $B5v$5$l$k:G9b$N%9%T!<%I(B
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

  /* SPeed $B@_Dj4XO"(B */
  virtual void SetSpeed( MSPEED /* speed */ ) {};
  virtual void SetHighSpeed( int /* speed */ ) {};
  virtual void SetMiddleSpeed( int /* speed */ ) {};
  virtual void SetLowSpeed( int /* speed */ ) {};
  bool hasSpeedsLine( void ) { return HasSpeedsLine; };
  void setHasSpeedsLine( bool f ) { HasSpeedsLine = f; };
  int highSpeed( void ) { return MaxS; };     // $B%*%j%8%J%k$N%O%$%9%T!<%I(B
  void setHighSpeed( int h ) { MaxS = h; };
  int highestSpeed( void ) { return ( HasSpeedsLine ) ? MaxMaxS : MaxS; };// $B5vMF:G9bB.EY(B
  void setHighestSpeed( int h ) { MaxMaxS = h; };
  int getHighS( void ) { return HighS; };
  int getMiddleS( void ) { return MiddleS; };
  int getLowS( void ) { return LowS; };
  virtual void AskHighSpeed( void ) {};
  virtual void AskMiddleSpeed( void ) {};
  virtual void AskLowSpeed( void ) {};
  
  /* MStab $B2D$N(B Unit $B$O2<5-$N4X?t$r<BAu$9$k$Y$7(B */
  virtual void CloseShutter( bool /*close*/ ) {};
  virtual void GoMaxAbs( double /*start*/, double /*end*/, int /*steps*/ ) {};
  virtual void GoMaxAbsQ( double /*start*/, double /*end*/, int /*steps*/, double /*time*/ ) {};
  virtual void GoMaxRel( double /*width*/, int /*steps*/ ) {};
  virtual void GoMaxRelQ( double /*width*/, int /*steps*/, double /*time*/ ) {};

  /* PM16C $B$@$1$+$b(B */
  virtual void AssignDispCh( int /* ch */ ) {};  // ch : 0 - 3 --> 'A' -- 'D'
  /* PM16C $B$G(B QXAFS $B$N0Y$K(B */
  virtual void SetTimingOutMode( int /* mode */ ) {};
  // 0 - 5 :: 0: none, 1: cont., 2: 200ns, 3: 10us, 4: 100us, 5: 1ms
  // 34410 triggers rising edge and requires 1us or longer
  // for EB741 2us is long enough
  virtual void SetTimingOutStart( int /*startP*/ ) {};      // start pos. of timing out
  virtual void SetTimingOutEnd( int /*endP*/ ) {};          // end pos. of timing out
  virtual void SetTimingOutInterval( int /*interval*/ ) {}; // timing out interval
  virtual void SetTimingOutReady( int /*ready*/ ) {};       // timing out ready
  virtual int accRate( void ) { return 1; };                // $B2C8:B.%l!<%H(B
  virtual void setAccRate( int /*r*/ ) {};
  virtual int accRateNo( void ) { return 0; };              // $B2C8:B.%l!<%H$N%F!<%V%kHV9f(B
  virtual void setAccRateNo( int /*n*/ ) {};
  
  virtual void setMaxV( QString /*maxv*/ ) {};
  virtual void setMinV( QString /*minv*/ ) {};

 public slots:
};

#endif
