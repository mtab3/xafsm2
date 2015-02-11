
#include "AUnitXMAP.h"

AUnitXMAP::AUnitXMAP( void )
{
}

bool AUnitXMAP::InitSensor( void )
{
  bool rv = false;

  QString ROIs = "";
  switch( LocalStage ) {
  case 0:
    IsBusy2On( Driver, "InitSensor-c0" );
    s->SendCMD2( "Init", Driver, "RunStop" );
    LocalStage++;
    rv = true;
    break;
  case 1:
    IsBusy2On( Driver, "InitSensor-c1" );
    s->SendCMD2( "Init", Driver, "SetPresetType", SSDPresetType );
    LocalStage++;
    rv = true;
    break;
  case 2:
    IsBusy2On( Driver, "InitSensor-c2" );
    ROIs = ROIStart[0] + " " + ROIEnd[0];
    for ( int i = 1; i < MaxSSDs; i++ ) {
      ROIs += " " + ROIStart[i] + " " + ROIEnd[i];
    }
    s->SendCMD2( "Init", Driver, "SetROIs", ROIs );
    LocalStage++;
    rv = false;
    break;
  }

  return rv;
}

void AUnitXMAP::init0( Stars *s )
{
  connect( s, SIGNAL( AnsSetPresetType( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetPresetValue( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsRunStart( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsRunStop( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGetValues( SMsg ) ), this, SLOT( ReceiveValues( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsSetROIs( SMsg ) ), this, SLOT( ClrBusy( SMsg ) ),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGetStatistics( SMsg ) ), this, SLOT( ReactGetStat( SMsg )),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGetRealTime( SMsg )), this, SLOT( ReactGetRealTime( SMsg )),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGetLiveTime( SMsg )), this, SLOT( ReactGetLiveTime( SMsg )),
	   Qt::UniqueConnection );
  connect( s, SIGNAL( AnsGetDataLinkCh( SMsg ) ),
	   this, SLOT( ReactGetDataLinkCh( SMsg ) ),
	   Qt::UniqueConnection );

  s->SendCMD2( "Init", DevCh, "IsBusy" );
  s->SendCMD2( "Init", Driver, "RunStop" );
  s->SendCMD2( "Init", Driver, "GetDataLinkCh" );
}


double AUnitXMAP::SetTime( double dtime ) // in sec, この関数は、複数ステップ化できない
{
  IsBusy2On( Driver, "SetTime" );
  s->SendCMD2( Uid, Driver, "RunStop" );   // コマンド連続発行可能か? いちおういけてる
  s->SendCMD2( Uid, DevCh, "SetPresetValue", QString::number( dtime ) );
  setTime = dtime;
}

bool AUnit::GetValue( void )
{
  IsBusy2On( Driver, "GetValue" );
  // 変則 : この IsBusy2 は @GetMCAs Ok: を受けても消さない
  //        data-link 経由で完全なデータをもらった時に消す
  //    s->SendCMD2( Uid, Driver, "GetValues" );    // new mcas
  s->SendCMD2( Uid, Driver, "GetMCAs" );

  return false;
}

void AUnitXMAP::RunStart( void )
{
  s->SendCMD2( Uid, Driver, "RunStart" );
}

void AUnitXMAP::RunStop( void )
{
  s->SendCMD2( Uid, Driver, "RunStop" );
}

void AUnitXMAP::RunResume( void )
{
  s->SendCMD2( Uid, Driver, "Resume" );
}

bool AUnitXMAP::GetMCAs( void )
{
  IsBusy2On( Driver2, "GetMCAs" );
  // 変則 : この IsBusy2 は @GetMCAs Ok: を受けても消さない
  //        data-link 経由で完全なデータをもらった時に消す
  s->SendCMD2( Uid, DevCh, QString( "GetMCAs" ) );

  return false;
}

void AUnitXMAP::getMCALength( SMsg msg )
{
  if ( msg.From() == Driver ) {  //   // Check !!!!! DevCh/Drv
    MCALength = msg.Val().toInt();
  }
}

bool AUnitXMAP::GetStat( void )
{
  bool rv = false;

  IsBusy2On( Driver, "GetStat" );
  s->SendCMD2( Uid, Driver, "GetStatistics" );

  return rv;
}

void AUnitXMAP::ReactGetStat( SMsg msg )
{
  if ( ( msg.From() == DevCh ) || ( msg.From() == Driver ) ) {  // Check !!!!! DevCh/Drv
    IsBusy2Off( Driver );
    MCAStats = msg.Vals();
  }
}

double AUnitXMAP::stat( int ch, STATELM i )
{
  double rv = 0;

  if ( MCAStats.count() >= 140 ) {
    rv = MCAStats.at( ch * 7 + (int)i ).toDouble();
  }

  return rv;
}

double AUnitXMAP::stat( STATELM i )
{
  return theParent->stat( Ch.toInt(), i );
}

#if 0
bool AUnitXMAP::SetRealTime( double val )
{
  bool rv = false;

  if ( Type == "SSDP" ) {
    IsBusy2On( Driver, "SetRealTime1" );
    s->SendCMD2( Uid, DevCh, "SetRealTime", QString::number( val ) );
    rv = false;
  }

  return rv;
}
#endif

bool AUnitXMAP::SetRealTime( int ch, double val )
{
  bool rv = false;

  IsBusy2On( Driver, "SetRealTime2" );
  s->SendCMD2( Uid, Driver, "SetRealTime",
	       QString::number( ch ) + " " + QString::number( val ) );

  return rv;
}

bool AUnitXMAP::GetRealTime( int ch )
{
  bool rv = false;

  IsBusy2On( Driver, "GetRealTime" );
  s->SendCMD2( Uid, Driver, "GetRealTime", QString::number( ch ) );

  return rv;
}

void AUnitXMAP::ReactGetRealTime( SMsg msg )
{
  int ch;

  if ( ( msg.From() == DevCh ) || ( msg.From() == Driver ) ) {  // Check !!!!! DevCh/Drv
    IsBusy2Off( Driver );
    MCARealTime[ ch = msg.Vals().at(0).toInt() ] = msg.Vals().at(1).toDouble();
    emit ReceivedNewMCARealTime( ch );
  }
}

double AUnitXMAP::realTime( int ch )
{
  return MCARealTime[ ch ];
}

#if 0
bool AUnitXMAP::SetLiveTime( double val )
{
  bool rv = false;

  if ( Type == "SSDP" ) {
    IsBusy2On( Driver, "SetLiveTime1" );
    s->SendCMD2( Uid, DevCh, "SetLiveTime", QString::number( val ) );
    rv = false;
  }

  return rv;
}
#endif

bool AUnitXMAP::SetLiveTime( int ch, double val )
{
  bool rv = false;

  IsBusy2On( Driver, "SetLiveTime2" );
  s->SendCMD2( Uid, Driver, "SetLiveTime",
	       QString::number( ch ) + " " + QString::number( val ) );

  return rv;
}

bool AUnitXMAP::GetLiveTime( int ch )
{
  bool rv = false;

  IsBusy2On( Driver, "GetLiveTime2" );
  s->SendCMD2( Uid, Driver, "GetLiveTime", QString::number( ch ) );

  return rv;
}

void AUnitXMAP::ReactGetLiveTime( SMsg msg )
{
  int ch;

  if ( ( msg.From() == DevCh ) || ( msg.From() == Driver ) ) {  // Check !!!!! DevCh/Drv
    IsBusy2Off( Driver );
    MCALiveTime[ ch = msg.Vals().at(0).toInt() ] = msg.Vals().at(1).toDouble();
    emit ReceivedNewMCALiveTime( ch );
  }
}

double AUnitXMAP::liveTime( int ch )
{
  return MCALiveTime[ ch ];
}

void AUnitXMAP::SetLowLimit( int ch, int llpix )
{
  if ( ch < MaxSSDs ) {
    //    MCALowLimit[ ch ] = llpix;
    s->SendCMD2( "SSDSetting", Driver,
		 QString( "SetLLimit %1 %2" ).arg( ch ).arg( llpix ) );
  } else {
    qDebug() << "Setting LowLimit the ch " << ch << "is too big";
  }
}

void AUnitXMAP::ReactGetDataLinkCh( SMsg msg )
{
  if ( msg.From() == Driver ) {
    if ( msg.Vals().count() == 2 ) {
      IsBusy2Off( Driver );
      QString NewDataLinkHostName = msg.Vals().at(0);
      int NewDataLinkHostPort = msg.Vals().at(1).toInt();
      if ( ( ! ConnectedToSSDServer ) || 
	   ( ( NewDataLinkHostName != DataLinkHostName )
	     &&( NewDataLinkHostPort != DataLinkHostPort ) ) ) {
	DataLinkHostName = NewDataLinkHostName;
	DataLinkHostPort = NewDataLinkHostPort;
	ConnectedToSSDServer = true;
	ConnectToDataLinkServer( DataLinkHostName, DataLinkHostPort );
	qDebug() << "Connect to SSD server" << DataLinkHostName << DataLinkHostPort;
      }
    }
  }
}

void AUnitXMAP::setDark( void )
{
  Dark = Value.toDouble() / ( ( setTime != 0 ) ? setTime : 1 );
  DarkCountsInROI.clear();
  DarkCountsAll.clear();
  DarkTotalEvents.clear();
  for ( int i = 0; i < CountsInROI.count(); i++ ) {
    DarkCountsInROI << CountsInROI.at(i) / ( ( setTime != 0 ) ? setTime : 1 );
    DarkCountsAll << CountsAll.at(i) / ( ( setTime != 0 ) ? setTime : 1 );
    DarkTotalEvents << TotalEvents.at(i) / ( ( setTime != 0 ) ? setTime : 1 );
  }
  DarkICRs = ICRs;
  setDarkTime = setTime;
  emit newDark( Dark );
}


quint32 AUnitXMAP::getAMCAdata( int ch, int pixel )
{
  if ( !MCAsReady )
    return 0;
  return *((quint32 *)( MCAs + AMCABUF * ch + MCAHEAD ) + pixel );
}

quint32 *AUnitXMAP::getAMCA( int ch )
{
  if ( !MCAsReady )
    return NULL;
  return (quint32 *)( MCAs + AMCABUF * ch + MCAHEAD );
}

MCAHead AUnitXMAP::getAMCAHead( int ch )
{
  MCAHead rv;

  if ( !MCAsReady )
    return rv;
  rv.ch       = *(qint64*)( MCAs + AMCABUF * ch +  0 );
  rv.stat     = *(qint64*)( MCAs + AMCABUF * ch +  8 );
  rv.len      = *(qint64*)( MCAs + AMCABUF * ch + 16 );
  rv.realTime = *(double*)( MCAs + AMCABUF * ch + 24 );
  rv.liveTime = *(double*)( MCAs + AMCABUF * ch + 32 );
  rv.icr      = *(double*)( MCAs + AMCABUF * ch + 40 );
  return rv;
}

void AUnitXMAP::setGain( int ch, double gain )
{
  s->SendCMD2( Uid, Driver, QString( "SetPreAMPGain %1 %2" ).arg( ch ).arg( gain ) );
}


/* 連続スキャン対応 */

// 連続スキャンの後にノーマルモードに戻す
bool AUnitCNT::Close( void )
{
  IsBusy2On( Driver, "GetValue0c0" );
  s->SendCMD2( Uid, Driver, "RunStop" );

  return false;
}
