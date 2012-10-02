#ifndef STARS_H
#define STARS_H

#include <QTcpSocket>
#include <QStringList>
#include <QString>

#define ERROR   "Error!!"        // string ��Ԃ��֐��̃G���[�ʒm�p

// �f�o�C�X���̒�`
#define MYNAME      "XafsM"          // ����������閼�O config �ŕς�����l�ɂ���ׂ�
#define MONO1       "monochro1"      // ������ (1)

#define STARSSERVER "localhost"      // config �ŕς�����悤�ɂ���ׂ�
#define STARSPORT   ( 6057 )         // config �ŕς�����悤�ɂ���ׂ�

#define SSMAXBUF    ( 4096 )

class Stars : public QObject 
{
  Q_OBJECT

  QString StarsServer;
  qint16  StarsSPort;

  QString clName;
  QStringList keys;
  int MaxKeys;
  QTcpSocket *ss;

  char ssWBuf[ SSMAXBUF ];  // ���M�R�}���h�`���p
  char ssRBuf[ SSMAXBUF ];  // ��M�p
  char ssIBuf[ SSMAXBUF ];  // ������Ɨp

  void ReadHistory( void );

public:
  Stars( void );
  void ReadStarsKeys( QString CLName );
  QString GetKey( int nkey );
  bool MakeConnection( void );

  bool SendCMD( const char *dev, const char *cmd1, const char *cmd2 ); // �����p
  char *AskAns( const char *dev, const char *cmd ); // �����p

  bool SetValue( const char *dev, const char *val );
  bool SetValue( const char *dev, int num );
  bool SetValue( const char *dev, double num );
  bool SetSpeed( const char *dev, int num );
  char *GetValue( const char *dev, int ch = -1 );
  int IsBusy( const char *dev, int ch = -1 );
  void StartMeas( const char *dev, double dwell, int ch = -1 );
  void Stop( const char *dev );

public slots:
  void SetNewSVAddress( const QString &item );
  void SetNewSVPort( const QString &item );

signals:
  void aMessage( QString msg, int time );
  void RecordSSVHistoryA( const QString &item );
  void RecordSSVHistoryP( const QString &item );
};

#endif
