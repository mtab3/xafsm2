
#include "AUnitXMAP.h"

AUnitXMAP::AUnitXMAP( void )
{
  SSDChs = 19;
  McaLength = 2048;    // !!

  connectingDLink = false;   // new
  
  DataLinkHostName = "";
  DataLinkHostPort = 0;
  dLink = NULL;
  dLinkStream = NULL;
  dLinkCount = 0;
  resetVectors( SSDChs );
  
  MCAs0 = NULL;
  MCAs = NULL;
  MCAsReady = false;

  SSDPresetType = "REAL";   // for MCA/SSD
  ROIStart = ROIEnd = NULL;


  MCAStats.clear();
}

void AUnitXMAP::resetVectors( int chs )
{
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
  for ( int i = 0; i < chs; i++ ) {
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
}

void AUnitXMAP::_setEnable( bool /*enable*/ )
{
  connectingDLink = false;    
  // $B$3$N(B setEnable $B$KB3$$$F(B init0 $B$,8F$P$l$k$O$:$J$N$G!"$3$3$G$d$i$J$/$F$bNI$$$O$:(B
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

  s->SendCMD2( Uid, DevCh, "IsBusy" );
  s->SendCMD2( Uid, Dev, "RunStop" );

  connectingDLink = false;          // new
  s->SendCMD2( Uid, Dev, "GetDataLinkCh" );
}

void AUnitXMAP::ReactGetDataLinkCh( SMsg msg )
{
  if ( msg.From() == Dev ) {
    if ( msg.Vals().count() == 2 ) {
      busy2Off( Dev );
      DataLinkHostName = msg.Vals().at(0);
      DataLinkHostPort = msg.Vals().at(1).toInt();
      ConnectToXMAPDataLinkServer( DataLinkHostName, DataLinkHostPort );
      qDebug() << "Connecting to SSD server" << DataLinkHostName << DataLinkHostPort;
    }
  }
}

void AUnitXMAP::ConnectToXMAPDataLinkServer( QString host, qint16 port )
{
  if ( !connectingDLink ) {
    connectingDLink = true;                          // new
    qDebug() << "data link server" << host << port;
    if ( dLink != NULL ) delete dLink;
    dLink = new QTcpSocket;
    if ( dLinkStream != NULL ) delete dLinkStream;
    dLinkStream = new QDataStream( dLink );
    if ( MCAs0 != NULL )
      delete [] MCAs0;
    MCAs0 = new char [ XMAPBUFSIZE ];
    dLinkCount = 0;
    MCAsReady = false;  // MCAs $B$N%P%C%U%!$KM-8z$J%G!<%?$,L5$$(B
    connect( dLink, SIGNAL( readyRead() ), this, SLOT( receiveMCAs() ),
	     Qt::UniqueConnection );
    dLink->connectToHost( host, port );
  }
}

bool AUnitXMAP::InitSensor( void )
{
  bool rv = false;

  QString ROIs = "";
  switch( LocalStage ) {
  case 0:
    busy2On( Dev, "InitSensor-c0" );
    s->SendCMD2( Uid, Dev, "RunStop" );
    LocalStage++;
    rv = true;
    break;
  case 1:
    busy2On( Dev, "InitSensor-c1" );
    s->SendCMD2( Uid, Dev, "SetPresetType", SSDPresetType );
    LocalStage++;
    rv = true;
    break;
  case 2:
    busy2On( Dev, "InitSensor-c2" );
    ROIs = ROIStart[0] + " " + ROIEnd[0];
    for ( int i = 1; i < chs(); i++ ) {
      ROIs += " " + ROIStart[i] + " " + ROIEnd[i];
    }
    s->SendCMD2( Uid, Dev, "SetROIs", ROIs );
    LocalStage++;
    rv = false;
    break;
  }

  return rv;
}

double AUnitXMAP::SetTime( double dtime ) // in sec, $B$3$N4X?t$O!"J#?t%9%F%C%W2=$G$-$J$$(B
{
  busy2On( Dev, "SetTime" );
  s->SendCMD2( Uid, Dev, "RunStop" );   // $B%3%^%s%IO"B3H/9T2DG=$+(B? $B$$$A$*$&$$$1$F$k(B
  s->SendCMD2( Uid, DevCh, "SetPresetValue", QString::number( dtime ) );
  setTime = dtime;

  return setTime;
}

bool AUnitXMAP::GetValue( void )
{
  busy2On( Dev, "GetValue" );
  // $BJQB'(B : $B$3$N(B IsBusy2 $B$O(B @GetMCAs Ok: $B$r<u$1$F$b>C$5$J$$(B
  //        data-link $B7PM3$G40A4$J%G!<%?$r$b$i$C$?;~$K>C$9(B
  //    s->SendCMD2( Uid, Dev, "GetValues" );    // new mcas
  s->SendCMD2( Uid, Dev, "GetMCAs" );

  return false;
}

bool AUnitXMAP::GetValue0( void )  // $BCMFI$_=P$7%3%^%s%I$NA0$K2?$+I,MW$J%?%$%W$N>l9g(B
{
  bool rv = false;

  switch( LocalStage ) {
  case 0:
    busy2On( Dev, "GetValue0c0" );
    s->SendCMD2( Uid, Dev, "RunStop" );
    rv = true;
    LocalStage++;
    break;
  case 1:
    busy2On( Dev, "GetValue0c1" );
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

// $BCMFI$_=P$7%3%^%s%I$NA0$K2?$+I,MW$J%?%$%W$N>l9g(B
// $BJL%P!<%8%g%s!"(BpresetTime $BEy$N=*N;>r7oL5$7$K$7$F$"$k(B
// $BO"B3%9%-%c%s(B ($B:9J,$GCM$r8+$k(B)$B%b!<%IMQ(B
bool AUnitXMAP::GetValue02( void )
{
  bool rv = false;

  switch( LocalStage ) {
  case 0:
    busy2On( Dev, "GetValue0c0" );
    s->SendCMD2( Uid, Dev, "SetPresetType", "NONE" );
    rv = true;
    LocalStage++;
    break;
  case 1:
    busy2On( Dev, "GetValue0c1" );
    s->SendCMD2( Uid, Dev, "RunStop" );
    rv = true;
    LocalStage++;
    break;
  case 2:
    busy2On( Dev, "GetValue0c2" );
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

/* $BO"B3%9%-%c%sBP1~(B */
// $BO"B3%9%-%c%s$N8e$K%N!<%^%k%b!<%I$KLa$9(B
bool AUnitXMAP::Close( void )
{
  busy2On( Dev, "GetValue0c0" );
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
  busy2On( Dev2, "GetMCAs" );
  // $BJQB'(B : $B$3$N(B IsBusy2 $B$O(B @GetMCAs Ok: $B$r<u$1$F$b>C$5$J$$(B
  //        data-link $B7PM3$G40A4$J%G!<%?$r$b$i$C$?;~$K>C$9(B
  s->SendCMD2( Uid, DevCh, QString( "GetMCAs" ) );

  return false;
}

void AUnitXMAP::getMCALength( SMsg msg )
{
  if ( msg.From() == Dev ) {  //   // Check !!!!! DevCh/Drv
    McaLength = msg.Val().toInt();
  }
}

bool AUnitXMAP::GetStat( void )
{
  bool rv = false;

  busy2On( Dev, "GetStat" );
  s->SendCMD2( Uid, Dev, "GetStatistics" );

  return rv;
}

void AUnitXMAP::ReactGetStat( SMsg msg )
{
  if ( ( msg.From() == DevCh ) || ( msg.From() == Dev ) ) {  // Check !!!!! DevCh/Drv
    busy2Off( Dev );
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

  busy2On( Dev, "SetRealTime2" );
  s->SendCMD2( Uid, Dev, "SetRealTime",
	       QString::number( ch ) + " " + QString::number( val ) );

  return rv;
}

bool AUnitXMAP::GetRealTime( int ch )
{
  bool rv = false;

  busy2On( Dev, "GetRealTime" );
  s->SendCMD2( Uid, Dev, "GetRealTime", QString::number( ch ) );

  return rv;
}

void AUnitXMAP::ReactGetRealTime( SMsg msg )
{
  int ch;

  if ( ( msg.From() == DevCh ) || ( msg.From() == Dev ) ) {  // Check !!!!! DevCh/Drv
    busy2Off( Dev );
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
    busy2On( Dev, "SetLiveTime1" );
    s->SendCMD2( Uid, DevCh, "SetLiveTime", QString::number( val ) );
    rv = false;
  }

  return rv;
}
#endif

bool AUnitXMAP::SetLiveTime( int ch, double val )
{
  bool rv = false;

  busy2On( Dev, "SetLiveTime2" );
  s->SendCMD2( Uid, Dev, "SetLiveTime",
	       QString::number( ch ) + " " + QString::number( val ) );

  return rv;
}

bool AUnitXMAP::GetLiveTime( int ch )
{
  bool rv = false;

  busy2On( Dev, "GetLiveTime2" );
  s->SendCMD2( Uid, Dev, "GetLiveTime", QString::number( ch ) );

  return rv;
}

void AUnitXMAP::ReactGetLiveTime( SMsg msg )
{
  int ch;

  if ( ( msg.From() == DevCh ) || ( msg.From() == Dev ) ) {  // Check !!!!! DevCh/Drv
    busy2Off( Dev );
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
  if ( ch < chs() ) {
    //    MCALowLimit[ ch ] = llpix;
    s->SendCMD2( Uid, Dev,
		 QString( "SetLLimit %1 %2" ).arg( ch ).arg( llpix ) );
  } else {
    qDebug() << "Setting LowLimit the ch " << ch << "is too big";
  }
}

void AUnitXMAP::SetDark( double /* val */, double /* time */ )
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
  emit NewDark( Dark );
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

void AUnitXMAP::readGain( int ch )
{
  s->SendCMD2( Uid, Dev, QString( "GetPreAMPGain %1" ).arg( ch ) );
}

void AUnitXMAP::getMCASettings( int ch )
{
  s->SendCMD2( Uid, Dev, "GetPeakingTime", QString::number( ch ) );
  s->SendCMD2( Uid, Dev, "GetThreshold", QString::number( ch ) );
  s->SendCMD2( Uid, Dev, "GetCalibration", QString::number( ch ) );
  s->SendCMD2( Uid, Dev, "GetDynamicRange", QString::number( ch ) );
  s->SendCMD2( Uid, Dev, "GetPreAMPGain", QString::number( ch ) );

  GetMCAs();
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
    for ( int i = 0; i < length(); i++ ) {
      if ( SSDUsingCh[i] ) {
	sum += msg.Vals().at( i + 1 ).toInt();
      }
    }
    Value = QString::number( sum );
    for ( int i = 0; i < chs(); i++ ) {
      CountsInROI << msg.Vals().at( i + 1 ).toInt();
      CountsAll   << msg.Vals().at( i + 1 + chs() ).toInt();
      TotalEvents << msg.Vals().at( i + 1 + chs() * 2 ).toInt();
      ICRs        << msg.Vals().at( i + 1 + chs() * 3 ).toDouble();
    }
    
    Values = msg.Vals();
    
    emit NewValue( Value );
    busy2Off( Dev );
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
  // $B:#FO$$$?J,$rA4ItFI$s$G$b%P%C%U%!%5%$%:$h$j>.$5$$$J$i(B
  if ( dLinkCount + bytes0 <= XMAPBUFSIZE )
    bytes = bytes0;                    // $BA4ItFI$`(B
  else
    bytes = XMAPBUFSIZE - dLinkCount;   // $BBg$-$$$J$i!"FI$a$kJ,$@$1FI$`(B

  bytes = dLinkStream->readRawData( MCAs0 + dLinkCount, bytes );
  dLinkCount += bytes;

  if ( dLinkCount >= XMAPBUFSIZE ) {
    busy2Off( Dev );
    dLinkCount = 0;
    if ( MCAs != NULL ) delete [] MCAs;
    MCAs = MCAs0;              // $BFI$_9~$_$,40@.$7$?%P%C%U%!(B(MCAs0)$B$r(B
                               // $B:G?7$N%G!<%?$,CV$+$l$?%P%C%U%!(B(MCAs)$B$K0\$7(B
    MCAs0 = new char [ XMAPBUFSIZE ];
                               // MCAs0 $B$O<!$N%G!<%?$r<u$1$k$?$a$K?7$7$/$9$k(B
    MCAsReady = true;          // MCAs $B$N%P%C%U%!$KM-8z$J%G!<%?$,$"$k(B

    CountsInROI.clear();
    CountsAll.clear();
    TotalEvents.clear();
    ICRs.clear();

    quint64 sum = 0;
    quint64 countsAll, countsInROI;
    for ( int i = 0; i < chs(); i++ ) {
      quint32 *aMCA = getAMCA( i );
      countsAll = countsInROI = 0;
      for ( int j = 0; j < length(); j++ ) {
	if ( ( j >= ROIStart[i].toInt() )&&( j <= ROIEnd[i].toInt() ) )
	  countsInROI += aMCA[j];
	countsAll += aMCA[j];
      }
      CountsAll << countsAll;
      sum += countsInROI;
      CountsInROI << countsInROI;
    }

    Value = QString::number( sum );
    for ( int i = 0; i < chs(); i++ ) {
      TotalEvents << 0;
      ICRs        << getAMCAHead( i ).icr;
    }
    emit NewMCAsAvailable( MCAs );
    emit NewValue( Value );
  }
}

