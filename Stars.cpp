
#include <QTextStream>
#include <QFile>

#include "Stars.h"

/********************/
/** サーバとの接続 **/

Stars::Stars( void ) : QObject()
{
  ss = NULL;
  emit SSisActive( false );
  StarsServer = STARSSERVER;
  StarsSPort  = STARSPORT;
  newSetting = true;
  ConnectionStage = CSTAGE0;
}

Stars::~Stars( void ) 
{
  if ( ss != NULL ) {
    ss->disconnectFromHost();
    delete ss;
    ss = NULL;
    emit SSisActive( false );
  }
}

void Stars::SetNewSVAddress( const QString &item )
{
  StarsServer = item;
  
  if ( ss != NULL ) {
    ss->disconnectFromHost();
    delete ss;
    ss = NULL;
    newSetting = true;
    emit SSisActive( false );
  }

  emit RecordSSVHistoryA( item );
}

void Stars::SetNewSVPort( const QString &item )
{
  StarsSPort = item.toInt();

  if ( ss != NULL ) {
    ss->disconnectFromHost();
    delete ss;
    ss = NULL;
    newSetting = true;
    emit SSisActive( false );
  }

  emit RecordSSVHistoryP( item );
}

void Stars::ReadStarsKeys( QString SelectedName, QString DefaultName )
{
  bool FileOK = false;

  MyNameOnStars = DefaultName;

  MaxKeys = 0;

  QFile file;
  if ( SelectedName != "" ) {
    file.setFileName( SelectedName + ".key" );
    if ( file.open( QIODevice::ReadOnly ) ) {
      FileOK = true;
    } else {
      qDebug() << tr( "Cannot open [%1.key]" ).arg( SelectedName );
    }
  }

  if ( ! FileOK ) {
    file.setFileName( ":" + DefaultName + ".key" );
    if ( !file.open( QIODevice::ReadOnly ) ) {
      qDebug() << tr( "Cannot open both [%1.key] and [:%2.key]" )
	.arg( SelectedName ).arg( DefaultName );
      exit( 1 );
    }
  }

  QString item;
  QTextStream in( &file );

  while( !in.atEnd() ) {
    in >> item;
    if ( !item.isEmpty() ) {
      keys << item;
    }
  }
  MaxKeys = keys.count();

  file.close();
}

QString Stars::GetKey( int nkey )
{
  int key = ( nkey - ((int)( nkey / MaxKeys )) * MaxKeys );
  return keys[ key ];
}

void Stars::ReConnect( void )
{
  if ( ss != NULL ) {
    ss->disconnectFromHost();
    delete ss;
    ss = NULL;
    newSetting = true;
    emit SSisActive( false );
  }

  emit ReConnected();
}

void Stars::AskStatus( void )
{
  if ( ss == NULL ) {
    emit SSisActive( false );
  } else {
    emit SSisActive( ss->state() == QAbstractSocket::ConnectedState );
  }
  emit ConnectingServer( StarsServer );
  emit ConnectingPort( StarsSPort );
}

/** サーバとの接続 **/
/********************/

/************************/
/** 低レベルの通信関数 **/

void Stars::MakeConnection( void )
{
  if ( ( ss == NULL ) || ( ss->state() != QAbstractSocket::ConnectedState ) ){
    if ( newSetting == true ) {  // 同じアドレス設定での接続試行は一回だけ
      newSetting = false;
      if ( ss == NULL )
	ss = new QTcpSocket;
      
      emit ConnectingServer( StarsServer );
      emit ConnectingPort( StarsSPort );
      emit AskRecord( tr( "Connecting Stars Server [%1] [%2] as [%3]" )
		      .arg( StarsServer ).arg( StarsSPort ).arg( MyNameOnStars ) );
      ConnectionStage = CSTAGE0;
      connect( ss, SIGNAL( readyRead( void ) ),
	       this, SLOT( ReceiveMessageFromStars( void ) ) );
      ss->connectToHost( StarsServer, StarsSPort );
    }
  }
}

void Stars::ReceiveMessageFromStars( void )
{
  SMsg smsg;
  QByteArray RBuf;
  QString WBuf;
  bool OkF;

  switch( ConnectionStage ) {
  case CSTAGE0: 
    RBuf = ss->readLine( 160000 ); // Stars のバッファは現在 160kbyte
                                   // QXafs では 1点16byte のデータが来るので
                                   // 10,000点が max
    RBuf = RBuf.simplified();
    WBuf = tr( "%1 %2\n" ).arg( MyNameOnStars ).arg( GetKey( RBuf.toInt() ) );
    ConnectionStage = CSTAGE1;
    ss->write( WBuf.toAscii() );
    break;
  case CSTAGE1:
    OkF = false;
    while( ss->canReadLine() ) {      // ここで応答のメッセージをチェックするべき
      RBuf = ss->readLine( 4000 );
      if ( RBuf.indexOf( "Ok:" ) >= 0 )
	OkF = true;
    }
    if ( OkF ) {
      ConnectionStage = CSTAGEEND;
      emit AskRecord( tr( "Success to connect." ) );
      emit ConnectionIsReady();
      emit SSisActive( true );
    }
    break;
  case CSTAGEEND:
    while( ss->canReadLine() ) {
      RBuf = ss->readLine( 160000 );
      // 160000 / 8000 = 20  dummy MCA でも 1チャンネルの数字が 20byte 以下なら大丈夫
      RBuf = RBuf.simplified();
      
      switch( smsg.ParseMsg( RBuf ) ) {
      case RES_MSG:
	emit AskRecord( tr( "Receive an answer from Stars [%1]" ).arg( RBuf.data() ) );
	if ( smsg.Val() == "Er:" ) {
	  emit ReceiveError( smsg ); break;
	}
	switch( smsg.Msgt() ) {
	case LISTNODES:
	  emit AnsListNodes( smsg ); break;
	case GETVALUE:
	  emit AnsGetValue( smsg ); break;
	case GETSPEEDSELECTED:
	  emit AnsGetSpeed( smsg ); break;
	case SPEEDHIGH:
	case SPEEDMIDDLE:
	case SPEEDLOW:
	  emit AnsSetSpeed( smsg ); break;
	case SETHIGHSPEED:
	  emit AnsSetHighSpeed( smsg ); break;
	case SETTIMINGOUTMODE:
	  emit AnsSetTimingOutMode( smsg ); break;
	case SETTIMINGOUTSTART:
	  emit AnsSetTimingOutStart( smsg ); break;
	case SETTIMINGOUTEND:
	  emit AnsSetTimingOutEnd( smsg ); break;
	case SETTIMINGOUTINTERVAL:
	  emit AnsSetTimingOutInterval( smsg ); break;
	case SETTIMINGOUTREADY:
	  emit AnsSetTimingOutReady( smsg ); break;
	case SELECT: break;
	  emit AnsSelect( smsg ); break;
	case REMOTE: break;
	  emit AnsRemote( smsg ); break;
	case STOP:
	  emit AnsStop( smsg ); break;
	case RESET:
	  emit AnsReset( smsg ); break;
	case SETDATAFORMAT:
	  emit AnsSetDataFormat( smsg ); break;
	case SETZEROCHECK:
	  emit AnsSetZeroCheck( smsg ); break;
	case SETAUTORANGE:
	  emit AnsSetAutoRange( smsg ); break;
	case SETNPLCYCLES:
	  emit AnsSetNPLCycles( smsg ); break;
	case RUN:
	  emit AnsRun( smsg ); break;
	case SETRANGE:
	  emit AnsSetRange( smsg ); break;
	case GETRANGE:
	  emit AnsGetRange( smsg ); break;
	case SCANCW:
	  emit AnsScanCw( smsg ); break;
	case SCANCCW:
	  emit AnsScanCcw( smsg ); break;
	case READ:
	  emit AnsRead( smsg ); break;
	case ISBUSY:
	  emit AnsIsBusy( smsg ); break;
	case SETABSOLUTEVOLTAGE:
	  emit AnsSetAbsoluteVoltage( smsg ); break;
	case SETABSOLUTEPOSITION:
	  emit AnsSetAbsolutePosition( smsg ); break;
	case SETRELATIVEVOLTAGE:
	  emit AnsSetRelativeVoltage( smsg ); break;
	case SETRELATIVEPOSITION:
	  emit AnsSetRelativePosition( smsg ); break;
	case SETSPEEDHIGH:
	case SETSPEEDMIDDLE:
	case SETSPEEDLOW:
	  emit AnsSetSpeed( smsg ); break;
	case SETSTOPMODE:
	  emit AnsSetStopMode( smsg ); break;
	case SETTIMERPRESET:
	  emit AnsSetTimerPreset( smsg ); break;
	case COUNTERRESET:
	  emit AnsCounterReset( smsg ); break;
	case COUNTSTART:
	  emit AnsCountStart( smsg ); break;
	case SETPRESETTYPE:
	  emit AnsSetPresetType( smsg ); break;
	case SETPRESETVALUE:
	  emit AnsSetPresetValue( smsg ); break;
	case GETVALUES:
	  emit AnsGetValues( smsg ); break;
	case RUNSTART:
	  emit AnsRunStart( smsg ); break;
	case RUNSTOP:
	  emit AnsRunStop( smsg ); break;
	case RESUME:
	  emit AnsResume( smsg ); break;
	case GETREALTIME:
	  emit AnsGetRealTime( smsg ); break;
	case GETLIVETIME:
	  emit AnsGetLiveTime( smsg ); break;
	case GETSTATUS:
	  emit AnsGetStatus( smsg ); break;
	case SETROI:
	  emit AnsSetROI( smsg ); break;
	case SETROIS:
	  emit AnsSetROIs( smsg ); break;
	case SETCALIBRATION:
	  emit AnsSetCalibration( smsg ); break;
	case SETTHRESHOLD:
	  emit AnsSetThreshold( smsg ); break;
	case SETPEAKINGTIME:
	  emit AnsSetPeakingTime( smsg ); break;
	case SETDYNAMICRANGE:
	  emit AnsSetDynamicRange( smsg ); break;
	case GETCALIBRATION:
	  emit AnsGetCalibration( smsg ); break;
	case GETTHRESHOLD:
	  emit AnsGetThreshold( smsg ); break;
	case GETPEAKINGTIME:
	  emit AnsGetPeakingTime( smsg ); break;
	case GETDYNAMICRANGE:
	  emit AnsGetDynamicRange( smsg ); break;
	case GETPREAMPGAIN:
	  emit AnsGetPreAMPGain( smsg ); break;
	case GETMCALENGTH:
	  emit AnsGetMCALength( smsg ); break;
	case GETMCA:
	  emit AnsGetMCA( smsg ); break;
	case GETSTATISTICS:
	  emit AnsGetStatistics( smsg ); break;
	case GETDATALINKCH:
	  emit AnsGetDataLinkCh( smsg ); break;
	case GETMCAS:
	  emit AnsGetMCAs( smsg ); break;
	case SETMODE:
	  emit AnsSetMode( smsg ); break;
	case SETCOUNTPRESET:
	  emit AnsSetCountPreset( smsg ); break;
	case SETAPERTURE:
	  emit AnsSetAperture( smsg ); break;
	case SETTRIGGERDELAY:
	  emit AnsSetTriggerDelay( smsg ); break;
	case SETSAMPLINGSOURCE:
	  emit AnsSetSamplingSource( smsg ); break;
	case SETTRIGGERSOURCE:
	  emit AnsSetTriggerSource( smsg ); break;
	case SETTRIGGERCOUNTS:
	  emit AnsSetTriggerCounts( smsg ); break;
	case SETTRIGGERSLOPE:
	  emit AnsSetTriggerSlope( smsg ); break;
	case GETDATAPOINTS:
	  emit AnsGetDataPoints( smsg ); break;
	case READDATAPOINTS:
	  emit AnsReadDataPoints( smsg ); break;
	case ABORT:
	  emit AnsAbort( smsg ); break;
	case SETAUTOZERO:
	  emit AnsSetAutoZero( smsg ); break;
	case RECORD:
	  emit AnsRecord( smsg ); break;
	case QINITIALIZE:
	  emit AnsQInitialize( smsg ); break;
	case QGETDATA:
	  emit AnsQGetData( smsg ); break;
	case QFINALIZE:
	  emit AnsQFinalize( smsg ); break;
	default:
	  break;
	}
	break;
      case RES_EVENT:
	emit AskRecord( tr( "Receive an event from Stars [%1]" ).arg( RBuf.data() ) );
	switch( smsg.Msgt() ) {
	case EvCHANGEDVALUE: 
	  emit EvChangedValue( smsg ); break;
	case EvISBUSY: 
	  emit EvIsBusy( smsg ); break;
	case EvCONNECTED: 
	  emit EvConnected( smsg ); break;
	case EvDISCONNECTED: 
	  emit EvDisconnected( smsg ); break;
	case EvREPORTCURRENT: 
	  emit EvReportCurrent( smsg ); break;
	default: 
	  break;
	}
	break;
      default:
	emit AskRecord( tr( "Receive an unricognized message from Stars [%1]" )
			.arg( RBuf.data() ) );
	qDebug() << tr( "Others:  [%1] -> [%2] : [%3] [%4]\n" )
	  .arg( smsg.From() ).arg( smsg.To() ).arg( smsg.Msg() ).arg( smsg.Val() );
      }
    }
  }
}

bool Stars::SendCMD2( QString fromCh, QString dev, QString cmd1, QString cmd2 )
{
  if ( ConnectionStage != CSTAGEEND )
    return false;

  QString Cmd = MyNameOnStars + "." + fromCh + ">" + dev + " " + cmd1;
  if ( !cmd2.isEmpty() )
    Cmd += " " + cmd2;
  Cmd += "\n";

  emit AskRecord( tr( "Sending a message [%1] to Stars" ).arg( Cmd ) );
  ss->write( Cmd.toAscii() );

  return true;
}

/** 低レベルの通信関数 **/
/************************/

