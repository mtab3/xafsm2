
#include "ASensor.h"

ASensor::ASensor( void )
{
  setTime = 1;       // Actually set time;
  setDarkTime = 1;   // Actually set time;
  Dark = 0;          // back ground value normalized for 1 sec

  autoRange = false;
  RangeSelectable = false;
  RangeU = 0;           // Upper range limit
  RangeL = 0;           // Lower range limit
  SelectedRange = 0;
}

void ASensor::init( void )
{
  connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ),
	   Qt::UniqueConnection );
  init0();
}
