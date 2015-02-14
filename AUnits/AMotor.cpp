
#include "AMotor.h"

AMotor::AMotor( void )
{
  Center = 0;

  HasSpeedsLine = false;
  HasSetMaxS = false;
  MaxS = 3000;      // max speed (pps) $B:G=i$K@_Dj$5$l$F$$$?%*%j%8%J%k$N%9%T!<%I(B
  MaxMaxS = 3000;   // $B5v$5$l$k:G9b$N%9%T!<%I(B
  HighS = 3000;
  MiddleS = 1000;
  LowS = 500;
  IsInt = false;
}

void AMotor::init( void )
{
  // SetValue $B$O(B Ok: $B$G$b(B Er: $B$G$bL5;k$9$k!#(B(ClrBusy$B$b$7$J$$(B)
  connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ),
	   Qt::UniqueConnection );

  s->SendCMD2( "Init", DevCh, "GetValue" );

  init0();  // $B99$K8DJL$N%f%K%C%H$N=i4|2=$r9T$&$?$a$N2>A[4X?t(B
}
