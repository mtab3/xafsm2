
#include "AUnitXMAP.h"

AUnitXMAP::AUnitXMAP( void )
{
  MCALength = 2048;    // !!
  
  ConnectedToSSDServer = false;
  hasConnected = false;
  DataLinkHostName = "";
  DataLinkHostPort = 0;
  dLink = NULL;
  dLinkStream = NULL;
  dLinkCount = 0;

  MCAs0 = NULL;
  MCAs = NULL;
  MCAsReady = false;

  SSDPresetType = "REAL";   // for MCA/SSD
  ROIStart = ROIEnd = NULL;

  MCARealTime.clear();
  MCALiveTime.clear();
  SSDUsingCh.clear();
  CountsInROI.clear();
  CountsAll.clear();
  TotalEvents.clear();
  ICRs.clear();
  DarkCountsInROI.clear();
  DarkCountsAll.clear();
  DarkTotalEvents.clear();
  DarkICRs.clear();
  for ( int i = 0; i < MaxSSDs; i++ ) {
    MCARealTime << 0;
    MCALiveTime << 0;
    SSDUsingCh << true;
    CountsInROI << 0;
    CountsAll << 0;
    TotalEvents << 0;
    ICRs << 0;
    DarkCountsInROI << 0;
    DarkCountsAll << 0;
    DarkTotalEvents << 0;
    DarkICRs << 0;
  }

  MCAStats.clear();
}

bool AUnitXMAP::InitSensor( void )
{
  bool rv = false;

  QString ROIs = "";
  switch( LocalStage ) {
  case 0:
    IsBusy2On( Dev, "InitSensor-c0" );
    s->SendCMD2( "Init", Dev, "RunStop" );
    LocalStage++;
    rv = true;
    break;
  case 1:
    IsBusy2On( Dev, "InitSensor-c1" );
    s->SendCMD2( "Init", Dev, "SetPresetType", SSDPresetType );
    LocalStage++;
    rv = true;
    break;
  case 2:
    IsBusy2On( Dev, "InitSensor-c2" );
    ROIs = ROIStart[0] + " " + ROIEnd[0];
    for ( int i = 1; i < MaxSSDs; i++ ) {
      ROIs += " " + ROIStart[i] + " " + ROIEnd[i];
    }
    s->SendCMD2( "Init", Dev, "SetROIs", ROIs );
    LocalStage++;
    rv = false;
    break;
  }

  return rv;
}

void AUnitXMAP::init0( void )
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
  s->SendCMD2( "Init", Dev, "RunStop" );
  s->SendCMD2( "Init", Dev, "GetDataLinkCh" );
}

void AUnitXMAP::ConnectToXMAPDataLinkServer( QString host, qint16 port )
{
  if ( !hasConnected ) {
    hasConnected = true;
    qDebug() << "data link server" << host << port;
    if ( dLink != NULL ) delete dLink;
    dLink = new QTcpSocket;
    if ( dLinkStream != NULL ) delete dLinkStream;
    dLinkStream = new QDataStream( dLink );
    if ( MCAs0 != NULL )
      delete [] MCAs0;
    MCAs0 = new char [ XMAPBUFSIZE ];
    dLinkCount = 0;
    MCAsReady = false;  // MCAs のバッファに有効なデータが無い
    connect( dLink, SIGNAL( readyRead() ), this, SLOT( receiveMCAs() ),
	     Qt::UniqueConnection );
    dLink->connectToHost( host, port );
  }
}

void AUnitXMAP::_setEnable( bool /*enable*/ )
{
  ConnectedToSSDServer = false;
  // 本当にこれでいいか stars との接続が on/off されると
  // かならず、裏でつながっているダイレクとのコネクションが切れたことにされる
}

double AUnitXMAP::SetTime( double dtime ) // in sec, この関数は、複数ステップ化できない
{
  IsBusy2On( Dev, "SetTime" );
  s->SendCMD2( Uid, Dev, "RunStop" );   // コマンド連続発行可能か? いちおういけてる
  s->SendCMD2( Uid, DevCh, "SetPresetValue", QString::number( dtime ) );
  setTime = dtime;

  return setTime;
}

bool AUnitXMAP::GetValue( void )
{
  IsBusy2On( Dev, "GetValue" );
  // 変則 : この IsBusy2 は @GetMCAs Ok: を受けても消さない
  //        data-link 経由で完全なデータをもらった時に消す
  //    s->SendCMD2( Uid, Dev, "GetValues" );    // new mcas
  s->SendCMD2( Uid, Dev, "GetMCAs" );

  return false;
}

bool AUnitXMAP::GetValue0( void )  // 値読み出しコマンドの前に何か必要なタイプの場合
{
  bool rv = false;

  switch( LocalStage ) {
  case 0:
    IsBusy2On( Dev, "GetValue0c0" );
    s->SendCMD2( Uid, Dev, "RunStop" );
    rv = true;
    LocalStage++;
    break;
  case 1:
    IsBusy2On( Dev, "GetValue0c1" );
    IsBusy = true;
    LastFunc = "GetValue0c1";
    emit ChangedIsBusy1( Dev );
    s->SendCMD2( Uid, Dev, "RunStart" );
    rv = false;
    LocalStage++;
    break;
  }

  return rv;
}

// 値読み出しコマンドの前に何か必要なタイプの場合
// 別バージョン、presetTime 等の終了条件無しにしてある
// 連続スキャン (差分で値を見る)モード用
bool AUnitXMAP::GetValue02( void )
{
  bool rv = false;

  switch( LocalStage ) {
  case 0:
    IsBusy2On( Dev, "GetValue0c0" );
    s->SendCMD2( Uid, Dev, "SetPresetType", "NONE" );
    rv = true;
    LocalStage++;
    break;
  case 1:
    IsBusy2On( Dev, "GetValue0c1" );
    s->SendCMD2( Uid, Dev, "RunStop" );
    rv = true;
    LocalStage++;
    break;
  case 2:
    IsBusy2On( Dev, "GetValue0c2" );
    IsBusy = true;
    LastFunc = "GetValue0c1";
    emit ChangedIsBusy1( Dev );
    s->SendCMD2( Uid, Dev, "RunStart" );
    rv = false;
    LocalStage++;
    break;
  }
  return rv;
}

/* 連続スキャン対応 */
// 連続スキャンの後にノーマルモードに戻す
bool AUnitXMAP::Close( void )
{
  IsBusy2On( Dev, "GetValue0c0" );
  s->SendCMD2( Uid, Dev, "RunStop" );

  return false;
}

void AUnitXMAP::RunStart( void )
{
  s->SendCMD2( Uid, Dev, "RunStart" );
}

void AUnitXMAP::RunStop( void )
{
  s->SendCMD2( Uid, Dev, "RunStop" );
}

void AUnitXMAP::RunResume( void )
{
  s->SendCMD2( Uid, Dev, "Resume" );
}

bool AUnitXMAP::GetMCAs( void )
{
  IsBusy2On( Dev2, "GetMCAs" );
  // 変則 : この IsBusy2 は @GetMCAs Ok: を受けても消さない
  //        data-link 経由で完全なデータをもらった時に消す
  s->SendCMD2( Uid, DevCh, QString( "GetMCAs" ) );

  return false;
}

void AUnitXMAP::getMCALength( SMsg msg )
{
  if ( msg.From() == Dev ) {  //   // Check !!!!! DevCh/Drv
    MCALength = msg.Val().toInt();
  }
}

bool AUnitXMAP::GetStat( void )
{
  bool rv = false;

  IsBusy2On( Dev, "GetStat" );
  s->SendCMD2( Uid, Dev, "GetStatistics" );

  return rv;
}

void AUnitXMAP::ReactGetStat( SMsg msg )
{
  if ( ( msg.From() == DevCh ) || ( msg.From() == Dev ) ) {  // Check !!!!! DevCh/Drv
    IsBusy2Off( Dev );
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

bool AUnitXMAP::SetRealTime( int ch, double val )
{
  bool rv = false;

  IsBusy2On( Dev, "SetRealTime2" );
  s->SendCMD2( Uid, Dev, "SetRealTime",
	       QString::number( ch ) + " " + QString::number( val ) );

  return rv;
}

bool AUnitXMAP::GetRealTime( int ch )
{
  bool rv = false;

  IsBusy2On( Dev, "GetRealTime" );
  s->SendCMD2( Uid, Dev, "GetRealTime", QString::number( ch ) );

  return rv;
}

void AUnitXMAP::ReactGetRealTime( SMsg msg )
{
  int ch;

  if ( ( msg.From() == DevCh ) || ( msg.From() == Dev ) ) {  // Check !!!!! DevCh/Drv
    IsBusy2Off( Dev );
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
    IsBusy2On( Dev, "SetLiveTime1" );
    s->SendCMD2( Uid, DevCh, "SetLiveTime", QString::number( val ) );
    rv = false;
  }

  return rv;
}
#endif

bool AUnitXMAP::SetLiveTime( int ch, double val )
{
  bool rv = false;

  IsBusy2On( Dev, "SetLiveTime2" );
  s->SendCMD2( Uid, Dev, "SetLiveTime",
	       QString::number( ch ) + " " + QString::number( val ) );

  return rv;
}

bool AUnitXMAP::GetLiveTime( int ch )
{
  bool rv = false;

  IsBusy2On( Dev, "GetLiveTime2" );
  s->SendCMD2( Uid, Dev, "GetLiveTime", QString::number( ch ) );

  return rv;
}

void AUnitXMAP::ReactGetLiveTime( SMsg msg )
{
  int ch;

  if ( ( msg.From() == DevCh ) || ( msg.From() == Dev ) ) {  // Check !!!!! DevCh/Drv
    IsBusy2Off( Dev );
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
    s->SendCMD2( "SSDSetting", Dev,
		 QString( "SetLLimit %1 %2" ).arg( ch ).arg( llpix ) );
  } else {
    qDebug() << "Setting LowLimit the ch " << ch << "is too big";
  }
}

void AUnitXMAP::ReactGetDataLinkCh( SMsg msg )
{
  if ( msg.From() == Dev ) {
    if ( msg.Vals().count() == 2 ) {
      IsBusy2Off( Dev );
      QString NewDataLinkHostName = msg.Vals().at(0);
      int NewDataLinkHostPort = msg.Vals().at(1).toInt();
      if ( ( ! ConnectedToSSDServer ) || 
	   ( ( NewDataLinkHostName != DataLinkHostName )
	     &&( NewDataLinkHostPort != DataLinkHostPort ) ) ) {
	DataLinkHostName = NewDataLinkHostName;
	DataLinkHostPort = NewDataLinkHostPort;
	ConnectedToSSDServer = true;
	ConnectToXMAPDataLinkServer( DataLinkHostName, DataLinkHostPort );
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
  return *((quint32 *)( MCAs + AXMAPBUF * ch + XMAPHEAD ) + pixel );
}

quint32 *AUnitXMAP::getAMCA( int ch )
{
  if ( !MCAsReady )
    return NULL;
  return (quint32 *)( MCAs + AXMAPBUF * ch + XMAPHEAD );
}

XMAPHead AUnitXMAP::getAMCAHead( int ch )
{
  XMAPHead rv;

  if ( !MCAsReady )
    return rv;
  rv.ch       = *(qint64*)( MCAs + AXMAPBUF * ch +  0 );
  rv.stat     = *(qint64*)( MCAs + AXMAPBUF * ch +  8 );
  rv.len      = *(qint64*)( MCAs + AXMAPBUF * ch + 16 );
  rv.realTime = *(double*)( MCAs + AXMAPBUF * ch + 24 );
  rv.liveTime = *(double*)( MCAs + AXMAPBUF * ch + 32 );
  rv.icr      = *(double*)( MCAs + AXMAPBUF * ch + 40 );
  return rv;
}

void AUnitXMAP::setGain( int ch, double gain )
{
  s->SendCMD2( Uid, Dev, QString( "SetPreAMPGain %1 %2" ).arg( ch ).arg( gain ) );
}

void AUnitXMAP::ReceiveValues( SMsg msg )
{
  QString buf;

  CountsInROI.clear();
  CountsAll.clear();
  TotalEvents.clear();
  ICRs.clear();

  if ( ( msg.From() == Dev ) && ( msg.Msgt() == GETVALUES ) ) { // Check !!!!! DevCh/Drv
    int sum = 0;
    for ( int i = 0; i < MaxSSDs; i++ ) {
      if ( SSDUsingCh[i] ) {
	sum += msg.Vals().at( i + 1 ).toInt();
      }
    }
    Value = QString::number( sum );
    for ( int i = 0; i < MaxSSDs; i++ ) {
      CountsInROI << msg.Vals().at( i + 1 ).toInt();
      CountsAll   << msg.Vals().at( i + 1 + MaxSSDs ).toInt();
      TotalEvents << msg.Vals().at( i + 1 + MaxSSDs * 2 ).toInt();
      ICRs        << msg.Vals().at( i + 1 + MaxSSDs * 3 ).toDouble();
    }
    
    Values = msg.Vals();
    
    emit newValue( Value );
    IsBusy2Off( Dev );
  }
}

void AUnitXMAP::SetIsBusyByMsg( SMsg msg )
{
  if ( ( msg.From() == Dev )
       && ( ( msg.Msgt() == ISBUSY ) || ( msg.Msgt() == EvISBUSY ) ) ) {
    IsBusy = ( msg.Val().toInt() == 1 );
    if ( IsBusy )
      LastFunc = "SetIsBusyByMsg";
    else
      LastFunc = "";
    emit ChangedIsBusy1( Dev );
  }
}

void AUnitXMAP::receiveMCAs( void )
{
  uint bytes0, bytes;

  bytes0 = dLink->bytesAvailable();
  // 今届いた分を全部読んでもバッファサイズより小さいなら
  if ( dLinkCount + bytes0 <= XMAPBUFSIZE )
    bytes = bytes0;                    // 全部読む
  else
    bytes = XMAPBUFSIZE - dLinkCount;   // 大きいなら、読める分だけ読む

  bytes = dLinkStream->readRawData( MCAs0 + dLinkCount, bytes );
  dLinkCount += bytes;

  if ( dLinkCount >= XMAPBUFSIZE ) {
    IsBusy2Off( Dev );
    dLinkCount = 0;
    if ( MCAs != NULL ) delete [] MCAs;
    MCAs = MCAs0;              // 読み込みが完成したバッファ(MCAs0)を
                               // 最新のデータが置かれたバッファ(MCAs)に移し
    MCAs0 = new char [ XMAPBUFSIZE ];
                               // MCAs0 は次のデータを受けるために新しくする
    MCAsReady = true;          // MCAs のバッファに有効なデータがある

    CountsInROI.clear();
    CountsAll.clear();
    TotalEvents.clear();
    ICRs.clear();

    quint64 sum = 0;
    quint64 countsAll, countsInROI;
    for ( int i = 0; i < MaxSSDs; i++ ) {
      quint32 *aMCA = getAMCA( i );
      countsAll = countsInROI = 0;
      for ( int j = 0; j < (int)MCALength; j++ ) {
	if ( ( j >= ROIStart[i].toInt() )&&( j <= ROIEnd[i].toInt() ) )
	  countsInROI += aMCA[j];
	countsAll += aMCA[j];
      }
      CountsAll << countsAll;
      sum += countsInROI;
      CountsInROI << countsInROI;
    }

    Value = QString::number( sum );
    for ( int i = 0; i < MaxSSDs; i++ ) {
      TotalEvents << 0;
      ICRs        << getAMCAHead( i ).icr;
    }
    emit LogMsg( "emitted New MCAs" );
    emit NewMCAsAvailable( MCAs );
    emit newValue( Value );
  }
}

