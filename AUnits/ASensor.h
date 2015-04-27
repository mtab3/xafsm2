
#ifndef ASENSOR_H
#define ASENSOR_H

#include "AUnit0.h"

class ASensor : public AUnit0
{
  Q_OBJECT

 protected:
  bool measIntegral;
  double setTime;       // Actually set time;
  double setDarkTime;   // Actually set time;
  double Dark;                 // back ground value normalized for 1 sec

  bool autoRange;
  bool RangeSelectable; // if range is selectable for the unit
  int RangeU;           // Upper range limit
  int RangeL;           // Lower range limit
  int SelectedRange;

  bool HasMaxIntTime;   // for QXAFS devices (default should be false)
  double MaxIntTime;    // Maximum integration time

 public:
  ASensor( void );

  void init( void );
  virtual void init0( void ) {};

  virtual bool InitSensor( void ) { return false; };
  virtual double SetTime( double dtime ) { return dtime; };   // in sec
  double getSetTime( void ) { return setTime; };   // actual set time

  bool hasMaxIntTime( void ) { return HasMaxIntTime; };
  double maxIntTime( void ) { return MaxIntTime; };
  void setMaxIntTime( double t ) { MaxIntTime = t; HasMaxIntTime = true; };
  
  // $BB,DjCM$NBg$-$5$,7WB,;~4V$KHfNc$9$k$+$7$J$$$+!#(Btrue $B$J$i$9$k(B
  void setMeasIntegral( bool f ) { measIntegral = f; };
  /* $B$3$N(B virtual 3$B$D$O7k9==EMW(B !!!! */
  // $B%+%&%s%?$d(BSSD$B$NMM$K!"@QJ,7A$N7WB,4o$G!"7WB,;~4V$,1d$S$k$H(B
  // $BB,DjCM$,7WB,;~4V$KHfNc$7$FBg$-$/$J$k%?%$%W$N7WB,4o$O!"(B
  // $B$3$3$G=q$+$l$?(B Default $B$NDj5A$G(B OK
  virtual void SetDark( double dark, double dtime )
  { if ( measIntegral )
      { Dark = dark / dtime; }
    else
      { Dark = dark; }
    emit NewDark( Dark );
  };
  virtual double GetDark( double dtime )
  { return ( measIntegral ) ? Dark * dtime : Dark; };
  virtual double convert2perTime( double val, double time )
  { return ( measIntegral ) ? val / time : val; };
  // $BEE057W$dEEN.7W$NMM$K!"7WB,;~4V$,D9$/$J$C$F$b(B
  // $BB,Dj7k2L$N?.Mj@-$O>e$,$k$,!"CM$=$N$b$N$O7WB,;~4V$KD>@\0MB8$7$J$$%?%$%W$N(B
  // $B7WB,4o$O:FDj5A$NI,MW$"$j(B
  // $B$=$N:]$O(B
#if 0     // Never turn on here !
  void SetDark( double dark, double /* dtime */ ) { Dark = dark; emit NewDark( Dark ); };
  double GetDark( double /* dtime */ ) { return Dark; };
  double convert2perTime( double val, double /* time */ ) { return val; };
#endif
  // $B$K$J$k$O$:(B
  // $B$b$7$/$O!"$=$N%?%$%W$N%f%K%C%H$N=i4|2=$NCf$G(B setMeasIntegral( false );
  // $B$9$l$PNI$$(B
  
  /* AutoRange $B2DG=$J%G%P%$%9$G$O(B true */
  virtual bool GetRange( void ) { return false; };
  virtual void SetRange( int /* range */ ) {};
  virtual bool isAutoRangeAvailable( void ) { return false; };
  bool isAutoRange( void ) { return autoRange; };
  void setAutoRange( bool ar ) { autoRange = ar; };
  int getRangeU( void ) { return RangeU; };
  int getRangeL( void ) { return RangeL; };
  int getRange( void ) { return SelectedRange; };
  void setRangeSelectable( bool f ) { RangeSelectable = f; };
  bool isRangeSelectable( void ) { return RangeSelectable; };
  void setRange( int r ) { SelectedRange = r; };
  void setRangeU( int upper ) { RangeU = upper; };
  void setRangeL( int lower ) { RangeL = lower; };
  
  /* QXAFS $BBP1~(B */
  virtual bool QStart( void ) { return false; };
  virtual bool QRead( void ) { return false; };
  virtual bool QEnd( void ) { return false; };

  /* $BO"B3%9%-%c%sBP1~(B */
  virtual bool Close( void ) { return false; };

 signals:
  void NewDark( double dark );
  void AskedNowRange( int r );
};

#endif
