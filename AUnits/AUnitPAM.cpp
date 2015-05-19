
#include "AUnitPAM.h"

void AUnitPAM::init0( void )
{
  setMeasIntegral( false );  // $BB,DjCM$NBg$-$5$O7WB,;~4V$KHfNc$7$J$$(B
  connect( s, SIGNAL( AnsReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetAutoRange( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetDataFormat( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetZeroCheck( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetNPLCycles( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );

  init00();
}

void AUnitPAM::init00( void )
{
  s->SendCMD2( Uid, DevCh, "IsBusy" );
}

void AUnitPAM2::init00( void )    // PAM $B$H(B PAM2 $B$G0c$C$F$k(B
{
  connect( s, SIGNAL( AnsRead( SMsg ) ),this, SLOT( RcvAnsGetValueOfDriver( SMsg ) ),
	   Qt::UniqueConnection );

  s->SendCMD2( Uid, Dev, "IsBusy" );
}

bool AUnitPAM::InitSensor( void )
{
  bool rv = false;

  QString dev;
  if ( Type == "PAM" ) { dev = Dev; } else { dev = Dev; } // !!
  switch( LocalStage ) {
  case 0:
    busy2On( Dev, "InitSensor-c0" );
    s->SendCMD2( Uid, dev, "Reset", "" );
    LocalStage++;
    rv = true;
    break;
  case 1:
    busy2On( Dev, "InitSensor-c1" );
    if ( Type == "PAM" ) 
      s->SendCMD2( Uid, dev, "SetAutoRangeEnable", "1" );
    if ( Type == "PAM2" ) 
      s->SendCMD2( Uid, dev, "SetAutoRangeEnable " + Ch, "1" );
    LocalStage++;
    rv = true;
    break;
  case 2:
    busy2On( Dev, "InitSensor-c2" );
    if ( Type == "PAM" )
      s->SendCMD2( Uid, dev, "SetDataFormatElements", "READ" );
    if ( Type == "PAM2" )
      s->SendCMD2( Uid, dev, "SetDataFormatElements", "CURR1,CURR2" );
    
    if ( Type == "PAM" ) {
      LocalStage++;
      rv = true;
    } else {
      rv = false;      // PAM2 $B$O(B ZeroCheck $B$N@_Dj$J$$$i$7$$(B
      LocalStage = 4;
    }
    break;
  case 3:
    busy2On( Dev, "InitSensor-c3" );
    s->SendCMD2( Uid, dev, "SetZeroCheckEnable", "0" );
    rv = false;
    LocalStage++;
    break;
  default:
    rv = false;
  }
  return rv;
}

bool AUnitPAM::GetValue( void )
{
  return _GetValue();
}
  
bool AUnitPAM::_GetValue( void )
{
  busy2On( Dev, "GetValue" );
  s->SendCMD2( Uid, DevCh, "Read" );

  return false;
}

bool AUnitPAM2::_GetValue( void )
{
  busy2On( Dev, "GetValue" );
  s->SendCMD2( Uid, Dev, "Read" );

  return false;
}

void AUnitPAM2::RcvAnsGetValueOfDriver( SMsg msg )  // driver $BL>$@$1$G8F$P$l$k>l9g(B
{
  if ( ( msg.From() == Dev ) && ( msg.Msgt() == READ ) ) {
    Values = msg.Val().split( QChar( ',' ) );
    Value = Values.at( Ch.toInt() ); // $B?F%I%i%$%P08$NJVEz$+$i<+J,MQ$NEz$($rA*$jJ,$1$k(B
    emit NewValue( Value );
    busy2Off( Dev );
  }
}

double AUnitPAM::SetTime( double dtime ) // in sec// $B$3$N4X?t$O!"J#?t%9%F%C%W2=$G$-$J$$(B
{
  double time;

  busy2On( Dev, "SetTime" );
  // 1 sec -> 1/60 sec
  time = dtime * 60;
  if ( time < 1 ) time = 1;
  if ( time > 40 ) time = 40;

  _SetTime( time );

  setTime = time / 60;    // $B$3$l$G!VIC!WC10L$NIaDL$N;~4V$KLa$C$F$k(B
  return setTime;
}

void AUnitPAM::_SetTime( double time )
{
  s->SendCMD2( Uid, DevCh, "SetNPLCycles", QString::number( time ) );
}

void AUnitPAM2::_SetTime( double time )
{
  s->SendCMD2( Uid, Dev, "SetNPLCycles " + Ch, QString::number( time ) );
}

void AUnitPAM::SetRange( int range )
{
  busy2On( Dev2, "SetRange" );
  _SetRange( range );
  SelectedRange = range;
}

void AUnitPAM::_SetRange( int range )
{
  s->SendCMD2( Uid, DevCh, "SetRange", QString( "2E%1" ).arg( range ) );
}

void AUnitPAM2::_SetRange( int range )
{
  s->SendCMD2( Uid, Dev, "SetRange " + Ch, QString( "2E%1" ).arg( range ) );
}
