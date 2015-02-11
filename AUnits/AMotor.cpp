
#include "AMotor.h"

AMotor::AMotor( void )
{
  Center = 0;
  UPP = 1;

  HasSpeedsLine = false;
  HaveSetMaxS = false;
  MaxS = 3000;      // max speed (pps) 最初に設定されていたオリジナルのスピード
  MaxMaxS = 3000;   // 許される最高のスピード
  HighS = 3000;
  MiddleS = 1000;
  LowS = 500;
}

void AMotor::init( Stars *s )
{
  //  PM  PZ SC  AIOo 
  //   1,  1, 1,  1
  //  PZ は当てはまらないが、connect しておいても悪いことは無いはず
  connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ),
	   Qt::UniqueConnection );
  // SetValue は Ok: でも Er: でも無視する。(ClrBusyもしない)

  s->SendCMD2( "Init", DevCh, "GetValue" );

  init0( s );
}
