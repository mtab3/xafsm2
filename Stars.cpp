
#include <QTextStream>
#include <QFile>

#include "Stars.h"

/********************/
/** サーバとの接続 **/

Stars::Stars( void ) : QObject()
{
  ss = NULL;
  StarsServer = STARSSERVER;
  StarsSPort  = STARSPORT;
  newSetting = true;
}

void Stars::SetNewSVAddress( const QString &item )
{
  StarsServer = item;
  
  if ( ss != NULL ) {
    ss->disconnectFromHost();
    delete ss;
    ss = NULL;
    newSetting = true;
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
  }

  emit RecordSSVHistoryP( item );
}

void Stars::ReadStarsKeys( QString SelectedName, QString DefaultName )
{
  bool FileOK = false;

  MyNameOnStars = DefaultName;

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

  MaxKeys = 0;
  foreach( QString i, keys ) {
    MaxKeys++;
  }

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
  }

  emit ReConnected();
}

/** サーバとの接続 **/
/********************/

/************************/
/** 低レベルの通信関数 **/

void Stars::MakeConnection( void )
{
#if 0
  QByteArray RBuf;
  QString WBuf;
#endif

  qDebug() << "Make Connection";

  if ( ( ss == NULL ) || ( ss->state() != QAbstractSocket::ConnectedState ) ){

    qDebug() << "Makeing Connection";

    if ( newSetting == true ) {  // 同じアドレス設定での接続試行は一回だけ

      qDebug() << "Makeing Connection 1";

      newSetting = false;
      if ( ss == NULL )
	ss = new QTcpSocket;
      
      emit AskRecord( tr( "Connecting Stars Server [%1] [%2] as [%3]" ).
		      arg( StarsServer ).arg( StarsSPort ).arg( MyNameOnStars ) );
      ConnectionStage = CSTAGE0;
      connect( ss, SIGNAL( readyRead( void ) ),
	       this, SLOT( ReceiveMessageFromStars( void ) ) );

      qDebug() << "Makeing Connection 1" << StarsServer << StarsSPort;

      ss->connectToHost( StarsServer, StarsSPort );
    }
  }
}

void Stars::ReceiveMessageFromStars( void )
{
  SMsg smsg;
  QByteArray RBuf;
  QString WBuf;

  qDebug() << "ReceiveMessage " << ConnectionStage;

  switch( ConnectionStage ) {
  case CSTAGE0: 
    RBuf = ss->readLine( 4000 );
    RBuf = RBuf.simplified();
    WBuf = tr( "%1 %2\n" ).arg( MyNameOnStars ).arg( GetKey( RBuf.toInt() ) );
    qDebug() << WBuf;
    ConnectionStage = CSTAGE1;
    ss->write( WBuf.toAscii() );
    break;
  case CSTAGE1:
    while( ss->canReadLine() )       // ここで応答のメッセージをチェックするべき
      RBuf = ss->readLine( 4000 );
    ConnectionStage = CSTAGEEND;
    emit AskRecord( tr( "Success to connect." ) );
    emit ConnectionIsReady();
    break;
  case CSTAGEEND:
    while( ss->canReadLine() ) {
      RBuf = ss->readLine( 4000 );
      RBuf = RBuf.simplified();
      
      switch( smsg.ParseMsg( RBuf ) ) {
      case RES_MSG:
	emit AskRecord( tr( "Receive an answer from Stars [%1]" ).arg( RBuf.data() ) );
	switch( smsg.Msgt() ) {
	case GETVALUE:
	  emit AnsGetValue( smsg ); break;
	case GETSPEEDSELECTED:
	  emit AnsGetSpeed( smsg ); break;
	case SPEEDHIGH:
	case SPEEDMIDDLE:
	case SPEEDLOW:
	  emit AnsSetSpeed( smsg ); break;
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

#if 0
bool Stars::SendCMD( QString dev, QString cmd1, QString cmd2 )
{
  if ( MakeConnection() == false ) {
    emit AskRecord( tr( "Failure to connect Stars." ) );
    return false;          // コネクションをはれないと false
  }

  if ( ConnectionStage != CSTAGEEND )
    return;

  QString Cmd = dev + " " + cmd1;
  if ( !cmd2.isEmpty() )
    Cmd += " " + cmd2;
  Cmd += "\n";

  emit AskRecord( tr( "Sending a message [%1] to Stars" ).arg( Cmd ) );
  ss->write( Cmd.toAscii() );

  return true;
}
#endif

bool Stars::SendCMD2( QString fromCh, QString dev, QString cmd1, QString cmd2 )
{
#if 0
  if ( MakeConnection() == false ) {
    emit AskRecord( tr( "Failure to connect Stars." ) );
    return false;          // コネクションをはれないと false
  }
#endif

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

