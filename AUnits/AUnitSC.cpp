
AUnitSC::AUnitSC( void )
{
}

void AUnitSC::SetValue( double v )
{
  s->SendCMD2( Uid, DevCh, "SetValue", QString( "%1 1 0 0 0 0" )
	       .arg( ilastSetV = (int)v ) );
}
