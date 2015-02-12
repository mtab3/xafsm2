
#include "AMotor.h"

AMotor::AMotor( void )
{
  Center = 0;

  HasSpeedsLine = false;
  HaveSetMaxS = false;
  MaxS = 3000;      // max speed (pps) 最初に設定されていたオリジナルのスピード
  MaxMaxS = 3000;   // 許される最高のスピード
  HighS = 3000;
  MiddleS = 1000;
  LowS = 500;
}

void AMotor::init( void )
{
  // SetValue は Ok: でも Er: でも無視する。(ClrBusyもしない)
  connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ),
	   Qt::UniqueConnection );

  s->SendCMD2( "Init", DevCh, "GetValue" );

  init0();
}
