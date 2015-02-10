void AUnit::SetValue( double v )
{
  s->SendCMD2( Uid, DevCh, "SetValue", QString::number( dlastSetV = v ) );
}
