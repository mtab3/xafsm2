
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
  
  // 測定値の大きさが計測時間に比例するかしないか。true ならする
  void setMeasIntegral( bool f ) { measIntegral = f; };
  /* この virtual 3つは結構重要 !!!! */
  // カウンタやSSDの様に、積分形の計測器で、計測時間が延びると
  // 測定値が計測時間に比例して大きくなるタイプの計測器は、
  // ここで書かれた Default の定義で OK
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
  // 電圧計や電流計の様に、計測時間が長くなっても
  // 測定結果の信頼性は上がるが、値そのものは計測時間に直接依存しないタイプの
  // 計測器は再定義の必要あり
  // その際は
#if 0     // Never turn on here !
  void SetDark( double dark, double /* dtime */ ) { Dark = dark; emit NewDark( Dark ); };
  double GetDark( double /* dtime */ ) { return Dark; };
  double convert2perTime( double val, double /* time */ ) { return val; };
#endif
  // になるはず
  // もしくは、そのタイプのユニットの初期化の中で setMeasIntegral( false );
  // すれば良い
  
  /* AutoRange 可能なデバイスでは true */
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
  
  /* QXAFS 対応 */
  virtual bool QStart( void ) { return false; };
  virtual bool QRead( void ) { return false; };
  virtual bool QEnd( void ) { return false; };

  /* 連続スキャン対応 */
  virtual bool Close( void ) { return false; };

 signals:
  void NewDark( double dark );
  void AskedNowRange( int r );
};

#endif
