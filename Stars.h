#ifndef STARS_H
#define STARS_H

#include <QTcpSocket>
#include <QStringList>
#include <QString>

#define ERROR   "Error!!"        // string を返す関数のエラー通知用

// デバイス名の定義
#define MYNAME      "XafsM"          // 自分が名乗る名前 config で変えられる様にするべき
#define MONO1       "monochro1"      // 分光器 (1)

#define STARSSERVER "localhost"      // config で変えられるようにするべき
#define STARSPORT   ( 6057 )         // config で変えられるようにするべき

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

  char ssWBuf[ SSMAXBUF ];  // 送信コマンド形成用
  char ssRBuf[ SSMAXBUF ];  // 受信用
  char ssIBuf[ SSMAXBUF ];  // 内部作業用

  void ReadHistory( void );

public:
  Stars( void );
  void ReadStarsKeys( QString CLName );
  QString GetKey( int nkey );
  bool MakeConnection( void );

  bool SendCMD( const char *dev, const char *cmd1, const char *cmd2 ); // 内部用
  char *AskAns( const char *dev, const char *cmd ); // 内部用

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
