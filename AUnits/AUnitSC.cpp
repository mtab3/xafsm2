
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

void AUnitSC::Stop( void )
{
  s->SendCMD2( Uid, DevCh, "Stop" );
}

void AUnitSC::AskIsBusy( void )
{
  s->SendCMD2( Uid, DevCh, "IsBusy" );
}
