
#include "AUnitPM.h"

AUnitPM::AUnitPM( void )
{
  AccRate = 100;    // 加減速レート ( AccRage ms/1000pps )
  AccRateNo = 24;   // 対応する加減速レートの PM16C のテーブル番号
}

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
  s->SendCMD2( Uid, DevCh, "GetHighSpeed" );
}

void AUnitPM::AskMiddleSpeed( void )
{
  s->SendCMD2( Uid, DevCh, "GetMiddleSpeed" );
}

void AUnitPM::AskLowSpeed( void )
{
  s->SendCMD2( Uid, DevCh, "GetLowSpeed" );
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

void AUnitPM::SetIsBusyByMsg( SMsg msg )
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

void AUnitPM::RcvHighSpeed( SMsg msg )
{
  if ( ( ( msg.From() == DevCh )||( msg.From() == Dev ) )  // Check !!!!! DevCh/Drv
       && ( ( msg.Msgt() == GETHIGHSPEED ) ) ) {
    HighS = msg.Val().toInt();
    if ( ! HasSetMaxS ) {
      MaxS = HighS;
      HasSetMaxS = true;
    }
    IsBusy2Off( Dev );
    emit gotHighS( HighS );
  }
}

void AUnitPM::RcvMiddleSpeed( SMsg msg )
{
  if ( ( ( msg.From() == DevCh )||( msg.From() == Dev ) )  // Check !!!!! DevCh/Drv
       && ( ( msg.Msgt() == GETMIDDLESPEED ) ) ) {
    MiddleS = msg.Val().toInt();
    IsBusy2Off( Dev );
    emit gotMiddleS( MiddleS );
  }
}

void AUnitPM::RcvLowSpeed( SMsg msg )
{
  if ( ( ( msg.From() == DevCh )||( msg.From() == Dev ) )  // Check !!!!! DevCh/Drv
       && ( ( msg.Msgt() == GETLOWSPEED ) ) ) {
    LowS = msg.Val().toInt();
    IsBusy2Off( Dev );
    emit gotLowS( LowS );
  }
}
