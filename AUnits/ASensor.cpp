
#include "ASensor.h"

ASensor::ASensor( QObject *p ) : AUnit0( p )
{
  measIntegral = true;  // 計測時間に比例して値が大きくなるタイプの
                        // 計測器(カウンタ、SSD)がデフォルト
  setTime = 1;       // Actually set time;
  setDarkTime = 1;   // Actually set time;
  Dark = 0;          // back ground value normalized for 1 sec

  autoRange = false;
  RangeSelectable = false;
  RangeU = 0;           // Upper range limit
  RangeL = 0;           // Lower range limit
  SelectedRange = 0;
  HasMaxIntTime = false; // 設定されていない
  MaxIntTime = 100000.;  // 十分大きい
}

void ASensor::init( void )
{
  connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ),
	   Qt::UniqueConnection );
  init0();
}
