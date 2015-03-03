
#ifndef ASENSOR_H
#define ASENSOR_H

#include "AUnit0.h"

class ASensor : public AUnit0
{
  Q_OBJECT

 protected:
  double setTime;       // Actually set time;
  double setDarkTime;   // Actually set time;
  double Dark;                 // back ground value normalized for 1 sec

  bool autoRange;
  bool RangeSelectable; // if range is selectable for the unit
  int RangeU;           // Upper range limit
  int RangeL;           // Lower range limit
  int SelectedRange;

 public:
  ASensor( QObject *p = NULL );

  void init( void );
  virtual void init0( void ) {};

  virtual bool InitSensor( void ) { return true; };
  virtual double SetTime( double dtime ) { return dtime; };   // in sec
  double getSetTime( void ) { return setTime; };   // actual set time
  void setDark( double dark ) { Dark = dark; emit newDark( Dark ); };
  double getDark( void ) { return Dark; };

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
  void newDark( double dark );
  void AskedNowRange( int r );
};

#endif
