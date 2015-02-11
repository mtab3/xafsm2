
AUnitSC::AUnitSC( void )
{
}

void AUnitSC::init0( Stars *s )
{
  s->SendCMD2( "Init", DevCh, "IsBusy" );
}

void AUnitSC::SetValue( double v )
{
  s->SendCMD2( Uid, DevCh, "SetValue", QString( "%1 1 0 0 0 0" )
	       .arg( ilastSetV = (int)v ) );
}

void AUnitPM::Stop( void )
{
  s->SendCMD2( Uid, DevCh, "Stop" );
}
