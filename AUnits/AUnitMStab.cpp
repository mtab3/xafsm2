
#include "AUnitMStab.h"

AUnitMStab::AUnitMStab( void )
{
}

void AUnitMStab::CloseShutter( bool close )   // MStab は特殊。いろんなデバイスでありえる
{
  if ( Type == "AIOo" ) {
    if ( close ) {
      s->SendCMD2( Uid, Driver, "ShutterOff 1" );
    } else {
      s->SendCMD2( Uid, Driver, "ShutterOff 0" );
    }
  }
}
