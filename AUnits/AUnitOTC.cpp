
#include "AUnitOTC.h"

AUnitOTC::AUnitOTC( void )
{
}

AUnitOTC2::AUnitOTC2( void )
{
}

void AUnitOTC::init0( void )
{
  connect( s, SIGNAL( AnsReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetMode( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetCountPreset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsCounterReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	     Qt::UniqueConnection );
  connect( s, SIGNAL( AnsRun( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsStop( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );

  s->SendCMD2( Uid, Dev, "IsBusy" );
  s->SendCMD2( Uid, Dev, "Reset" );
  s->SendCMD2( Uid, Dev, "SetMode", "0" );

  init00();
}

void AUnitOTC::init00( void )
{
}

void AUnitOTC2::init00( void )
{
  connect( s, SIGNAL( AnsReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetAutoRange( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetZeroCheck( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetRange( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGetRange( SMsg ) ), this, SLOT( ReactGetRange( SMsg ) ),
	   Qt::UniqueConnection );
}

bool AUnitOTC::InitSensor( void )
{
  return _InitSensor();
}

bool AUnitOTC::_InitSensor( void )
{
  return false;
}

bool AUnitOTC2::_InitSensor( void )
{
  if ( The2ndDev == NULL ) {
    qDebug() << "InitSensor:: The2ndDev is not initialized" << Uid << Name;
    return false;
  }
  
  bool rv = false;
  // CNT2, OTC2 $B$N$H$-(B $B%+%&%s%?$N8~$3$&$K$D$J$,$k$N$O(B
  // keithley $B$J$N$G$=$lMQ$N=hM}$r$7$F$*$/(B
  QString Type2 = The2ndDev->type();
  switch( LocalStage ) {
  case 0:
    busy2On( Dev2, "InitSensor-c0" );
    s->SendCMD2( Uid, DevCh2, "Reset", "" );
    LocalStage++;
    rv = true;
    break;
  case 1:
    busy2On( Dev2, "InitSensor-c1" );
    if ( autoRange ) {
      if ( Type2 == "PAM" )
	s->SendCMD2( Uid, DevCh2, "SetAutoRangeEnable", "1" );
      if ( Type2 == "PAM2" )
	s->SendCMD2( Uid, Dev2, "SetAutoRangeEnable " + Ch2, "1" );
      LocalStage = 3;
    } else {
      if ( Type2 == "PAM" )
	s->SendCMD2( Uid, DevCh2, "SetAutoRangeEnable", "0" );
      if ( Type2 == "PAM2" )
	s->SendCMD2( Uid, Dev2, "SetAutoRangeEnable " + Ch2, "0" );
      LocalStage = 2;
    }
    rv = true;
    break;
  case 2:
    busy2On( Dev2, "InitSensor-c2" );
    if ( Type2 == "PAM" ) {
      s->SendCMD2( Uid, DevCh2, "SetRange", QString( "2E%1" ).arg( SelectedRange ) );
      LocalStage++;
      rv = true;
    }
    if ( Type2 == "PAM2" ) {
      s->SendCMD2( Uid, Dev2, "SetRange " + Ch2,
		   QString( "2E%1" ).arg( SelectedRange ) );
      LocalStage+=2;     // PAM2 $B$N;~$O!"(BLocalStage == 3 $B$r$H$P$9(B
      rv = false;
    }
    break;
  case 3:
    busy2On( Dev2, "InitSensor-c3" );
    s->SendCMD2( Uid, DevCh2, "SetZeroCheckEnable", "0" );
    rv = false;
    LocalStage++;
    break;
  default:
    rv = false;
  }

  return rv;
}



void AUnitOTC::AskIsBusy( void )
{
  _AskIsBusy();
}

void AUnitOTC::_AskIsBusy( void )
{
  s->SendCMD2( Uid, DevCh, "IsBusy" );
}

void AUnitOTC2::_AskIsBusy( void )
{
}

void AUnitOTC::SetIsBusyByMsg( SMsg msg )
{
  if ( ( msg.From() == Dev )   // Check !!!!! DevCh/Drv
       && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
    IsBusy = ( msg.Val().toInt() == 1 );
    if ( IsBusy )
      LastFunc = "SetIsBusyByMsg";
    else
      LastFunc = "";
    emit ChangedIsBusy1( Dev );
  }
}

bool AUnitOTC::GetValue0( void )  // $BCMFI$_=P$7%3%^%s%I$NA0$K2?$+I,MW$J%?%$%W$N>l9g(B
{
  bool rv = false;

  switch( LocalStage ) {
  case 0:
    busy2On( Dev, "GetValue0c0" );
    s->SendCMD2( Uid, Dev, "CounterReset" );
    LocalStage++;
    rv = true;
    break;
  case 1:
    busy2On( Dev, "GetValue0c1" );
    IsBusy = true;
    LastFunc = "GetValue0c1";
    emit ChangedIsBusy1( Dev );
    s->SendCMD2( Uid, Dev, "Run" );
    LocalStage++;
    rv = false;
    break;
  }

  return rv;
}

bool AUnitOTC2::GetRange( void ) // CNT2, OTC2
{
  QString Type2 = The2ndDev->type();
  busy2On( Dev2, "GetRange" );
  if ( Type2 == "PAM" )
    s->SendCMD2( Uid, DevCh2, QString( "GetRange" ) );
  if ( Type2 == "PAM2" )
    s->SendCMD2( Uid, Dev2, QString( "GetRange " ) + Ch2 );

  return false;
}

void AUnitOTC2::ReactGetRange( SMsg msg )  // CNT2, OTC2
{
  double range = RangeL;
  if ( ( msg.From() == DevCh2 ) || ( msg.From() == Dev2 ) ) {
    QString Type2 = The2ndDev->type();
    if ( Type2 == "PAM" ) {
      range = log10( msg.Vals().at(0).toDouble() / 2.1 );
    }
    if ( Type2 == "PAM2" ) {
      if ( msg.Vals().at(0).toInt() == Ch2.toInt() ) {
	range = log10( msg.Vals().at(1).toDouble() / 2.1 );
      } else {
	return;
      }
    }
    
    busy2Off( Dev2 );
    if ( range > RangeU ) range = RangeU;
    if ( range < RangeL ) range = RangeL;
    emit AskedNowRange( (int)range );
  }
}

double AUnitOTC::SetTime( double dtime ) // in sec // $B$3$N4X?t$O!"J#?t%9%F%C%W2=$G$-$J$$(B
{
  int M, N;

  busy2On( Dev, "SetTime" );
  N = log10( dtime * 10 );
  M = ceil( dtime / pow( 10., N - 1 ) );
  s->SendCMD2( Uid, Dev, "SetCountPreset", QString( "%1,%2" ).arg( M ).arg( N ) );
  setTime = M * pow( 10, N ) * 0.1;  // $B$3$l$GICC10L$NIaDL$N;~4V$KLa$C$F$k(B

  return setTime;
}

void AUnitOTC2::SetRange( int range )
{
  if ( The2ndDev == NULL ) {
    qDebug() << "SetRange : The2ndDev is not initialized" << Uid << Name;
    return;
  }
  
  busy2On( Dev2, "SetRange" );
  // CNT2, OTC2 $B$N$H$-(B $B%+%&%s%?$N8~$3$&$K$D$J$,$k$N$O(B
  // keithley ( PAM/PAM2 )$B$J$N$G$=$lMQ$N=hM}$r$7$F$*$/(B
  QString Type2 = The2ndDev->type();
  if ( Type2 == "PAM" ) {
    s->SendCMD2( Uid, DevCh2, "SetRange", QString( "2E%1" ).arg( range ) );
  }
  if ( Type2 == "PAM2" ) {
    s->SendCMD2( Uid, Dev2, "SetRange " + Ch2,
		 QString( "2E%1" ).arg( range ) );
  }
  SelectedRange = range;
}
