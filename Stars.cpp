
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
    return false;          // �R�l�N�V�������͂�Ȃ��� false
  }

  // �R�}���h�𑗂낤�Ƃ���Ƃ��Ƀf�[�^�����Ă���S�~�Ƃ��Ď̂Ă�
  // �����ŃC�x���g������\���͂��邪�A�C�x���g�̈����͕ʓr�l���� event
  int n;
  while( ss->canReadLine() ) {
    n = ss->readLine( ssRBuf, 4000 );
    ssRBuf[n] = '\0';
    printf( "Discard (Before CMD snd): [%s]\n", ssRBuf );
  }

  // �R�}���h���`
  strcpy( ssWBuf, dev );
  strcat( ssWBuf, " " );
  strcat( ssWBuf, cmd1 );
  if ( cmd2 != NULL ) {
    strcat( ssWBuf, " " );
    strcat( ssWBuf, cmd2 );
  }
  strcat( ssWBuf, "\n" );

  // �R�}���h���M
  ss->write( ssWBuf );

  return true;
}

bool Stars::SetSpeed( const char *dev, int num )  // �{���͂���� AskAns �ɂ���?
{
  char buf[ 256 ];

#if 0
  sprintf( buf, "%d", num );
  if ( SendCMD( dev, "SetSpeed", buf ) ) {
    //    QString rv = SendCMD( MONO1 );  // ��ǂ݁B�����ŃG���[���E���ׂ�
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

bool Stars::SetValue( const char *dev, const char *val )  // �{���͂���� AskAns �ɂ���?
{
#if 0
  if ( SendCMD( dev, "SetValue", val ) ) {
    //    QString rv = SendCMD( MONO1 );  // ��ǂ݁B�����ŃG���[���E���ׂ�
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

// �����̊֐����Ăђ����Ă�
bool Stars::SetValue( const char *dev, int num )
{
  char buf[ 256 ];

  sprintf( buf, "%d", num );
  return SetValue( dev, buf );
}

// �����̊֐����Ăђ����Ă�
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

  // �������`�F�b�N���鎞�Ɋ��҂��鉞���̌`
  strcpy( ssIBuf, dev );
  strcat( ssIBuf, ">" );
  strcat( ssIBuf, MYNAME );
  strcat( ssIBuf, " @" );
  strcat( ssIBuf, cmd );
  iSize = strlen( ssIBuf );

  while ( trying == 1 ) {
    // �^�C���A�E�g��ׂ��ēǂݍ���
    if ( !ss->waitForReadyRead( 1000 ) ) {
      // 1 �b���ǂ����ǂ����͕ʂɂ��āA�^�C���A�E�g�ɑ΂��ă��g���C�͂��Ȃ�
      return (char *)ERROR;
    }

    // �f�[�^�̓ǂݍ���
    int n = ss->readLine( ssRBuf, 4000 );
    ssRBuf[n] = '\0';

    // ���҂��鉞���̌`���ǂ����̃`�F�b�N�A����Ă���΍ēx�f�[�^�҂�
    // (�R�}���h�̍đ��͂��Ȃ�)
    // �����ŃC�x���g������\���͂��邪�A�C�x���g�̈����͕ʓr�l���� event
    if ( strncmp( ssRBuf, ssIBuf, iSize ) == 0 )
      trying = 0;
  }
 
  if ( ssRBuf[ strlen( ssRBuf )-1 ] == '\n' ) // ������ LF �͏���
    ssRBuf[ strlen( ssRBuf )-1 ] = '\0';
  char *p = ssRBuf + iSize;              // �Ăяo���R�}���h�̕����ƒ���̃X�y�[�X�͏���
  while ( isspace( *p ) ) p++;

  return p;
}

void Stars::Stop( const char *dev )
{
  SendCMD( dev, "Stop" );
}
