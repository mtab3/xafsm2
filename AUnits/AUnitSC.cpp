
#include "AUnitSC.h"

AUnitSC::AUnitSC( void )
{
}

void AUnitSC::init0( void )
{
  s->SendCMD2( "Init", DevCh, "IsBusy" );
}

void AUnitSC::SetValue( double v )
{
  s->SendCMD2( Uid, DevCh, "SetValue", QString( "%1 1 0 0 0 0" )
	       .arg( ILastSetV = (int)v ) );
}

bool AUnitSC::GetValue( void )
{
  IsBusy2On( Dev, "GetValue" );
  s->SendCMD2( Uid, DevCh, "GetValue 0" );
  return false;
}

void AUnitSC::Stop( void )
{
  s->SendCMD2( Uid, DevCh, "Stop" );
}

void AUnitSC::AskIsBusy( void )
{
  s->SendCMD2( Uid, DevCh, "IsBusy" );
}

void AUnitSC::SetIsBusyByMsg( SMsg msg )
{
  if ( ( msg.From() == DevCh )
       && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
    IsBusy = ( msg.Val().toInt() == 1 );
    if ( IsBusy )
      LastFunc = "SetIsBusyByMsg";
    else
      LastFunc = "";
    emit ChangedIsBusy1( Dev );
  }
}

void AUnitSC::SetCurPos( SMsg msg )
{
  QString buf;
  
  if ( ( msg.From() == DevCh )
       && ( ( msg.Msgt() == GETVALUE ) || ( msg.Msgt() == EvCHANGEDVALUE )
            || ( msg.Msgt() == READ ) ) ) {
    if ( msg.Msgt() == EvCHANGEDVALUE ) {
      Value = msg.Val();
    } else {
      Value = msg.Vals().at(1);
    }
    emit newValue( Value );
    IsBusy2Off( Dev );
  }
}
