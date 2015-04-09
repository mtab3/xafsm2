/* DV/DV2 は DV の方が QXAFS 用で、DV2 が NORMAL 用 */
/* DV3 */

#include "AUnitDV.h"

AUnitDV::AUnitDV( void )
{
  points = 0;
  HasMaxIntTime = false;
  MaxIntTime = 1000000;   // 十分大きい
}

void AUnitDV::init0( void )
{
  connect( s, SIGNAL( AnsReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );

  init00();
}

void AUnitDV::init00( void )
{
  connect( s, SIGNAL( AnsQInitialize( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsQGetData( SMsg ) ), this, SLOT( RcvQGetData( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsQFinalize( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  init000();
}

void AUnitDV2::init00( void )
{
  connect( s, SIGNAL( AnsSetAperture( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetAutoZero( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
}

void AUnitDV3::init000( void )
{
  connect( s, SIGNAL( AnsSetTimerPreset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsCounterReset( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsCountStart( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
}

bool AUnitDV::InitSensor( void )
{
  return _InitSensor();
}

bool AUnitDV::_InitSensor( void )
{
  busy2On( Dev, "InitSensor-c0" );
  s->SendCMD2( "Scan", DevCh, "Reset", "" );
  return false;
}

bool AUnitDV2::_InitSensor( void )
{
  bool rv = false;
  
  switch( LocalStage ) {
  case 0:
    busy2On( Dev, "InitSensor-c0" );
    s->SendCMD2( "Scan", DevCh, "Reset", "" );
    LocalStage++;
    rv = true;
    break;
  case 1:
    busy2On( Dev, "InitSensor-c1" );
    s->SendCMD2( "Scan", DevCh, "SetAutoZero", "OFF" );
    LocalStage++;
    rv = false;
    break;
  default:
    rv = false;
  }

  return rv;
}

void AUnitDV2::AskIsBusy( void )
{
  s->SendCMD2( Uid, DevCh, "IsBusy" );
}

void AUnitDV3::SetIsBusyByMsg( SMsg msg )
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

bool AUnitDV::QStart( void )
{
  busy2On( Dev, "Start" );
  s->SendCMD2( Uid, DevCh, "qInitialize", QString::number( setTime ) );
  return false;
}

bool AUnitDV::QRead( void )
{
  busy2On( Dev, "Read" );
  s->SendCMD2( Uid, DevCh, "qGetData" );

  return false;
}

bool AUnitDV::QEnd( void )
{
  busy2On( Dev, "End" );
  s->SendCMD2( Uid, DevCh, "qFinalize" );

  return false;
}

double AUnitDV::_SetTime( double dtime ) // in sec // この関数は、複数ステップ化できない
{
  if ( dtime < 0.0001 ) dtime = 0.0001;
  if ( dtime > 1.0 ) dtime = 1.0;
  if (( HasMaxIntTime )&&( dtime > MaxIntTime )) { dtime = MaxIntTime; };
  setTime = dtime;

  qDebug() << "setting time in DV " << dtime << setTime;

  return setTime;
}

double AUnitDV2::_SetTime( double dtime )
{
  if ( dtime < 0.0001 ) dtime = 0.0001;
  if ( dtime > 1.0 ) dtime = 1.0;
  if (( HasMaxIntTime )&&( dtime > MaxIntTime )) { dtime = MaxIntTime; };

  busy2On( Dev, "SetAperture" );
  s->SendCMD2( Uid, DevCh, "SetAperture", QString( "%1" ).arg( dtime ) );
  setTime = dtime;

  return setTime;
}

double AUnitDV3::_SetTime( double dtime )
{
  busy2On( Dev, "SetTimerPreset" );
  long int ltime = dtime * 1e6;
  s->SendCMD2( Uid, Dev, "SetTimerPreset", QString( "%1" ).arg( ltime ) );
  setTime = dtime;

  return setTime;
}

void AUnitDV::SetTriggerDelay( double time )  // 使っていない
{
  busy2On( Dev, "SetTriggerDelay" );
  s->SendCMD2( Uid, DevCh, "SetTriggerDelay", QString::number( time ) );
}

void AUnitDV::SetSamplingSource( QString source ) //source := TIM, IMM // 使ってない
{
  busy2On( Dev, "SetSamplingSource" );
  s->SendCMD2( Uid, DevCh, "SetSamplingSource", source );
}

void AUnitDV::SetTriggerSource( QString source ) //source := IMM, EXT, BUS //使ってない
{
  busy2On( Dev, "SetTriggerSource" );
  s->SendCMD2( Uid, DevCh, "SetTriggerSource", source );
}

void AUnitDV::SetTriggerCounts( int counts )   
{
  busy2On( Dev, "SetTriggerCounts" );
  
  QString arg;
  if ( counts > 0 )
    arg = QString::number( counts );
  else
    arg = "INF";
  
  s->SendCMD2( Uid, DevCh, "SetTriggerCounts", arg );
}

void AUnitDV::SetTriggerSlope( QString type ) // POS : rising-edge
{
  busy2On( Dev, "SetTriggerSlope" );
  s->SendCMD2( Uid, DevCh, "SetTriggerSlope", type );
}

void AUnitDV::GetDataPoints( void )
{
  busy2On( Dev, "GetDataPoints" );
  s->SendCMD2( Uid, DevCh, "GetDataPoints" );
}

void AUnitDV::ReadDataPoints( int datas )
{
  busy2On( Dev, "ReadDataPoints" );
  s->SendCMD2( Uid, DevCh, "GetDataPoints", QString::number( datas ) );
}

void AUnitDV::Abort( void )
{
  busy2On( Dev, "Abort" );
  s->SendCMD2( Uid, DevCh, "Abort" );
}

void AUnitDV::RcvQGetData( SMsg msg )
{
  if ( ( ( msg.From() == DevCh )||( msg.From() == Dev ) )
       && ( ( msg.Msgt() == GETDATAPOINTS )
	    || ( msg.Msgt() == QGETDATA )
	    || ( msg.Msgt() == GETDATA ) ) ) {
    
    Values = msg.Vals();
    emit NewQData();
    busy2Off( Dev );
  }
}

bool AUnitDV3::_GetValue0( void )  // 値読み出しコマンドの前に何か必要なタイプの場合
{
  bool rv = false;

  qDebug() << "in getValue0 of DV3" << LocalStage;
  
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
    s->SendCMD2( Uid, Dev, "CountStart" );
    LocalStage++;
    rv = false;
    break;
  }

  return rv;
}
