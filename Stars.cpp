
#include <QTextStream>
#include <QFile>

#include "Stars.h"

Stars::Stars( void ) : QObject()
{
  ss = NULL;
  StarsServer = STARSSERVER;
  StarsSPort  = STARSPORT;
}

void Stars::SetNewSVAddress( const QString &item )
{
  StarsServer = item;
  ss->disconnectFromHost();
  delete ss;
  ss = NULL;

  emit RecordSSVHistoryA( item );
}

void Stars::SetNewSVPort( const QString &item )
{
  StarsSPort = item.toInt();
  ss->disconnectFromHost();
  delete ss;
  ss = NULL;

  emit RecordSSVHistoryP( item );
}

void Stars::ReadStarsKeys( QString CLName )
{
  clName = CLName;

  QFile file( ":" + CLName + ".key" );
  if ( !file.open( QIODevice::ReadOnly ) ) {
    printf( "Cannot open [%s]\n", CLName.toAscii().data() );
    exit( 1 );
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
    //    printf( "keys [%s]\n", i.toAscii().data() );
  }
  //  printf( "Keys = [%d]\n", MaxKeys );

  file.close();
}

QString Stars::GetKey( int nkey )
{
  int key = ( nkey - ((int)( nkey / MaxKeys )) * MaxKeys );
  return keys[ key ];
}

bool Stars::MakeConnection( void ) 
{
  if ( ss == NULL ) {
    ss = new QTcpSocket;
    ss->connectToHost( StarsServer, StarsSPort );
    if ( !ss->waitForReadyRead( 1000 ) ) {
      return false;
    }
    ss->readLine( ssRBuf, 4000 );
    sprintf( ssWBuf, "%s %s\n", MYNAME, GetKey( atoi( ssRBuf ) ).toAscii().data() );
    ss->write( ssWBuf );
    if ( !ss->waitForReadyRead( 1000 ) ) {
      return false;
    }
    ss->readLine( ssRBuf, 4000 );
  }
  return true;
}

bool Stars::SendCMD( const char *dev, const char *cmd1, const char *cmd2 = NULL )
{
  if ( MakeConnection() == false ) {
    emit aMessage( tr( "Connection Error." ), 1000 );
    return false;          // コネクションをはれないと false
  }

  // コマンドを送ろうとするときにデータが来てたらゴミとして捨てる
  // ここでイベントが入る可能性はあるが、イベントの扱いは別途考える event
  int n;
  while( ss->canReadLine() ) {
    n = ss->readLine( ssRBuf, 4000 );
    ssRBuf[n] = '\0';
    printf( "Discard (Before CMD snd): [%s]\n", ssRBuf );
  }

  // コマンド成形
  strcpy( ssWBuf, dev );
  strcat( ssWBuf, " " );
  strcat( ssWBuf, cmd1 );
  if ( cmd2 != NULL ) {
    strcat( ssWBuf, " " );
    strcat( ssWBuf, cmd2 );
  }
  strcat( ssWBuf, "\n" );

  // コマンド送信
  ss->write( ssWBuf );

  return true;
}

bool Stars::SetSpeed( const char *dev, int num )  // 本当はこれも AskAns にする?
{
  char buf[ 256 ];

#if 0
  sprintf( buf, "%d", num );
  if ( SendCMD( dev, "SetSpeed", buf ) ) {
    //    QString rv = SendCMD( MONO1 );  // 空読み。ここでエラーを拾うべき
    //    printf( "Ans.SV: [%s]\n", rv.trimmed().toAscii().data() );
    return true;
  }
  return false;
#else

  char *rv;
  sprintf( buf, "SetSpeed %d", num );
  rv = AskAns( dev, buf );
  if ( rv == (char*)ERROR )
    return false;
  return true;

#endif

}

bool Stars::SetValue( const char *dev, const char *val )  // 本当はこれも AskAns にする?
{
#if 0
  if ( SendCMD( dev, "SetValue", val ) ) {
    //    QString rv = SendCMD( MONO1 );  // 空読み。ここでエラーを拾うべき
    //    printf( "Ans.SV: [%s]\n", rv.trimmed().toAscii().data() );
    return true;
  }
  return false;
#else
  char buf[ 4096 ];
  char *rv;

  strcpy( buf, "SetValue " );
  strcat( buf, val );
  rv = AskAns( dev, buf );
  if ( rv == (char*)ERROR )
    return false;
  return true;
#endif
}

// 同名の関数を呼び直してる
bool Stars::SetValue( const char *dev, int num )
{
  char buf[ 256 ];

  sprintf( buf, "%d", num );
  return SetValue( dev, buf );
}

// 同名の関数を呼び直してる
bool Stars::SetValue( const char *dev, double num )
{
  char buf[ 256 ];

  sprintf( buf, "%f", num );
  return SetValue( dev, buf );
}

int Stars::IsBusy( const char *dev, int ch )
{
  char cbuf[ 100 ];
  char *rv;

  if ( ch == -1 )
    rv = AskAns( dev, "IsBusy" );
  else {
    sprintf( cbuf, "IsBusy %d", ch );
    rv = AskAns( dev, cbuf );
  }

  if ( strncmp( rv, ERROR, strlen( ERROR ) ) == 0 ) {
    return -1;
  }
  return ( atoi( rv ) );
}

char *Stars::GetValue( const char *dev, int ch )
{
  char cbuf[ 100 ];
  char *rv;

  if ( ch == -1 )
    rv = AskAns( dev, "GetValue" );
  else {
    sprintf( cbuf, "GetValue %d", ch );
    rv = AskAns( dev, cbuf );
  }

  return rv;
}

void Stars::StartMeas( const char *dev, double dwell, int ch )
{
  char cbuf[ 100 ];

  if ( ch == -1 ) {
    sprintf( cbuf, "StartMeas %f", dwell );
    AskAns( dev, "StartMeas " );
  } else {
    sprintf( cbuf, "StartMeas %d %f", ch, dwell );
    AskAns( dev, cbuf );
  }
}

char *Stars::AskAns( const char *dev, const char *cmd )
{
  int trying = 1;
  int iSize;

  SendCMD( dev, cmd );

  // 応答をチェックする時に期待する応答の形
  strcpy( ssIBuf, dev );
  strcat( ssIBuf, ">" );
  strcat( ssIBuf, MYNAME );
  strcat( ssIBuf, " @" );
  strcat( ssIBuf, cmd );
  iSize = strlen( ssIBuf );

  while ( trying == 1 ) {
    // タイムアウトを儲けて読み込み
    if ( !ss->waitForReadyRead( 1000 ) ) {
      // 1 秒が良いかどうかは別にして、タイムアウトに対してリトライはしない
      return (char *)ERROR;
    }

    // データの読み込み
    int n = ss->readLine( ssRBuf, 4000 );
    ssRBuf[n] = '\0';

    // 期待する応答の形かどうかのチェック、違っていれば再度データ待ち
    // (コマンドの再送はしない)
    // ここでイベントが入る可能性はあるが、イベントの扱いは別途考える event
    if ( strncmp( ssRBuf, ssIBuf, iSize ) == 0 )
      trying = 0;
  }
 
  if ( ssRBuf[ strlen( ssRBuf )-1 ] == '\n' ) // 末尾の LF は消す
    ssRBuf[ strlen( ssRBuf )-1 ] = '\0';
  char *p = ssRBuf + iSize;              // 呼び出しコマンドの復唱と直後のスペースは消す
  while ( isspace( *p ) ) p++;

  return p;
}

void Stars::Stop( const char *dev )
{
  SendCMD( dev, "Stop" );
}
