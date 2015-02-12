
#include "AUnitPM.h"

void AUnitPM::init0( void )
{
  s->SendCMD2( "Init", DevCh, "IsBusy" );

  connect( s, SIGNAL(AnsSetHighSpeed( SMsg )), this, SLOT(ClrBusy( SMsg )),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsSetMiddleSpeed( SMsg )), this, SLOT(ClrBusy( SMsg )),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsSetLowSpeed( SMsg )), this, SLOT(ClrBusy( SMsg )),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsSetTimingOutMode( SMsg )), this, SLOT(ClrBusy( SMsg )),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsSetTimingOutStart( SMsg )), this, SLOT(ClrBusy( SMsg )),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsSetTimingOutEnd( SMsg )), this, SLOT(ClrBusy( SMsg )),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsSetTimingOutInterval( SMsg )), this, SLOT(ClrBusy( SMsg )),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsSetTimingOutReady( SMsg )), this, SLOT(ClrBusy( SMsg )),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsSelect( SMsg )), this, SLOT(ClrBusy( SMsg )),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsGetHighSpeed( SMsg )), this, SLOT(RcvHighSpeed( SMsg )),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsGetMiddleSpeed( SMsg )), this, SLOT(RcvMiddleSpeed( SMsg )),
	   Qt::UniqueConnection );
  connect( s, SIGNAL(AnsGetLowSpeed( SMsg )), this, SLOT(RcvLowSpeed( SMsg )),
	   Qt::UniqueConnection );
}

void AUnitPM::SetValue( double v )
{
  IsBusy = true;
  emit ChangedIsBusy1( DevCh );
  s->SendCMD2( Uid, DevCh, "SetValue", QString::number( ILastSetV = (int)v ) );
}

void AUnitPM::SetSpeed( MSPEED speed )
{
  QString cmd = "SpeedLow";

  //  IsBusy2 = true;    // setspeed に対する応答は無視するので isBusy2 もセットしない
  switch( speed ) {
  case LOW: cmd = "SpeedLow"; break;
  case MIDDLE: cmd = "SpeedMiddle"; break;
  case HIGH: cmd = "SpeedHigh"; break;
  default: cmd = "SpeedLow"; break;
  }
  s->SendCMD2( Uid, DevCh, cmd );
}

void AUnitPM::SetHighSpeed( int speed )
{
  IsBusy2On( Dev, "SetHighSpeed" );
  QString cmd = QString( "SetHighSpeed %1" ).arg( speed );
  s->SendCMD2( Uid, DevCh, cmd );
}

void AUnitPM::SetMiddleSpeed( int speed )
{
  IsBusy2On( Dev, "SetMiddleSpeed" );
  QString cmd = QString( "SetMiddleSpeed %1" ).arg( speed );
  s->SendCMD2( Uid, DevCh, cmd );
}

void AUnitPM::SetLowSpeed( int speed )
{
  IsBusy2On( Dev, "SetLowSpeed" );
  QString cmd = QString( "SetLowSpeed %1" ).arg( speed );
  s->SendCMD2( Uid, DevCh, cmd );
}

void AUnitPM::Stop( void )
{
  s->SendCMD2( Uid, DevCh, "Stop" );
}

void AUnitPM::AskHighSpeed( void )
{
  s->SendCMD2( "Init", DevCh, "GetHighSpeed" );
}

void AUnitPM::AskMiddleSpeed( void )
{
  s->SendCMD2( "Init", DevCh, "GetMiddleSpeed" );
}

void AUnitPM::AskLowSpeed( void )
{
  s->SendCMD2( "Init", DevCh, "GetLowSpeed" );
}

void AUnitPM::AssignDispCh( int ch )
{
  IsBusy2On( Dev, "AssignDispCh" );
  int num = Ch.toInt();
  if ( Ch.left( 2 ).toUpper() == "CH" )
    num = Ch.mid( 2 ).toInt();
  QString cmd = QString( "Select %1 %2" ).arg( QChar( 'A' + ch ) ).arg( num );
  s->SendCMD2( Uid, Dev, cmd );
}

void AUnitPM::SetTimingOutMode( int mode )
{
  IsBusy2On( Dev, "SetTimingOutMode" );
  QString cmd = QString( "SetTimingOutMode %1" ).arg( mode );
  s->SendCMD2( Uid, DevCh, cmd );
}

void AUnitPM::SetTimingOutStart( int startP )
{
  IsBusy2On( Dev, "SetTimingOutStart" );
  QString cmd = QString( "SetTimingOutStart %1" ).arg( startP );
  s->SendCMD2( Uid, DevCh, cmd );
}

void AUnitPM::SetTimingOutEnd( int endP )
{
  IsBusy2On( Dev, "SetTimingOutEnd" );
  QString cmd = QString( "SetTimingOutEnd %1" ).arg( endP );
  s->SendCMD2( Uid, DevCh, cmd );
}

void AUnitPM::SetTimingOutInterval( int interval )
{
  IsBusy2On( Dev, "SetTimingOutInterval" );
  QString cmd = QString( "SetTimingOutInterval %1" ).arg( interval );
  s->SendCMD2( Uid, DevCh, cmd );
}

void AUnitPM::SetTimingOutReady( int ready )
{
  IsBusy2On( Dev, "SetTimingOutReady" );
  QString cmd = QString( "SetTimingOutReady %1" ).arg( ready );
  s->SendCMD2( Uid, DevCh, cmd );
}

void AUnitPM::AskIsBusy( void )
{
  s->SendCMD2( Uid, DevCh, "IsBusy" );
}
