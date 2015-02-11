
#ifndef ASENSOR_H
#define ASENSOR_H

#include "AUnit0.h"

class ASensor : public AUnit0
{
  Q_OBJECT

  double setTime;       // Actually set time;
  double setDarkTime;   // Actually set time;
  double Dark;                 // back ground value normalized for 1 sec

  bool autoRange;
  bool RangeSelectable; // if range is selectable for the unit
  int RangeU;           // Upper range limit
  int RangeL;           // Lower range limit
  int SelectedRange;

 public:
  ASensor( void );

  void init( Stars *s );
  virtual void init0( Stars *s );

  virtual bool InitSensor( void );
  virtual double SetTime( double dtime );   // in sec
  double GetSetTime( void ) { return setTime; };   // actual set time
  void setDark( double dark ) { Dark = dark; emit newDark( Dark ); };
  double getDark( void ) { return Dark; };

  /* AutoRange 可能なデバイスでは true */
  virtual bool GetRange( void );
  virtual void SetRange( int range );
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
  
  /* QXAFS 対応 */
  virtual bool QStart( void ) { return false; };
  virtual bool QRead( void ) { return false; };
  virtual bool QEnd( void ) { return false; };

  /* 連続スキャン対応 */
  virtual bool Close( void ) { return false; };

 signals:
  void newDark( double dark );
};

#endif
