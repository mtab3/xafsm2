
#include "AUnitMStab.h"

AUnitMStab::AUnitMStab( void )
{
}

void AUnitMStab::CloseShutter( bool close )   // MStab ���ü졣�����ʥǥХ����Ǥ��ꤨ��
{
  if ( Type == "AIOo" ) {
    if ( close ) {
      s->SendCMD2( Uid, Driver, "ShutterOff 1" );
    } else {
      s->SendCMD2( Uid, Driver, "ShutterOff 0" );
    }
  }
}
