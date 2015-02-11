void AUnitENC::SetValue( double v )
{
  s->SendCMD2( Uid, DevCh, "SetValue", QString::number( dlastSetV = v ) );
}

void AUnitENC::init0( Stars *s )
{
  connect( s, SIGNAL(AnsGetStat( SMsg )), this, SLOT( RcvStat( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsTrigger( SMsg )), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsStandBy( SMsg )), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsGetData( SMsg )), this, SLOT( RcvQGetData( SMsg ) ),
	   Qt::UniqueConnection );

  init00( s );
}

void AUnitENC::init00( Stars *s )
{
  connect( s, SIGNAL( EvChangedValue( SMsg ) ), this, SLOT( SetCurPos( SMsg ) ),
	   Qt::UniqueConnection );
  s->SendCMD2( "Init", DevCh, "IsBusy" );
  s->SendCMD2( "Init", DevCh, "GetValue" );
}

void AUnitENC2::init00( Stars *s )
{
}

bool AUnitENC2::QStart( void )
{
  IsBusy2On( Driver, "Start" );
  s->SendCMD2( Uid, DevCh, "StandBy" );
  return false;
}

bool AUnit::QRead( void )
{
  IsBusy2On( Driver, "Read" );
  s->SendCMD2( Uid, DevCh, "GetData" );

  return false;
}
