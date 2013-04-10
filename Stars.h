#ifndef STARS_H
#define STARS_H

#include <QTcpSocket>
#include <QStringList>
#include <QString>

#include "SMsg.h"

#define ERROR   "Error!!"        // string を返す関数のエラー通知用

// デバイス名の定義
//#define MYNAME      "BLC"       // 自分が名乗る名前 config で変えられる様にするべき


enum CONNECTIONSTAGE { CSTAGE0, CSTAGE1, CSTAGEEND };

#define STARSSERVER "localhost"      // config で変えられるようにするべき
#define STARSPORT   ( 6057 )         // config で変えられるようにするべき

class Stars : public QObject 
{
  Q_OBJECT

  QString MyNameOnStars;

  CONNECTIONSTAGE ConnectionStage;

  QString StarsServer;
  qint16  StarsSPort;

  QStringList keys;
  int MaxKeys;
  QTcpSocket *ss;
  bool newSetting;

  void ReadHistory( void );

public:
  Stars( void );
  ~Stars( void );
  void ReadStarsKeys( QString SelectedName, QString DefaultName );
  QString GetKey( int nkey );

  void MakeConnection( void );
  bool SendCMD( QString dev, QString cmd1, QString cmd2 = "" );
  bool SendCMD2( QString fromCh, QString dev, QString cmd1, QString cmd2 = "" );

public slots:
  void SetNewSVAddress( const QString &item );
  void SetNewSVPort( const QString &item );
  void ReceiveMessageFromStars( void );
  void ReConnect( void );
  void AskStatus( void );

signals:
  void ConnectingServer( QString SSVA );
  void ConnectingPort( qint16 SSVP );
  void ConnectionIsReady( void );
  void SSisActive( bool f );

  void AskShowStat( QString msg, int time );
  void AskRecord( QString msg );
  void RecordSSVHistoryA( const QString &item );
  void RecordSSVHistoryP( const QString &item );
  void ReConnected( void );
  void ReceiveError( SMsg msg );

  void AnsListNodes( SMsg msg );

  void AnsRemote( SMsg msg );
  void AnsGetValue( SMsg msg );
  void AnsSetSpeed( SMsg msg );
  void AnsGetSpeed( SMsg msg );
  void AnsSetHighSpeed( SMsg msg );
  void AnsStop( SMsg msg );
  void AnsReset( SMsg msg );
  void AnsSetDataFormat( SMsg msg );
  void AnsSetZeroCheck( SMsg msg );
  void AnsSetAutoRange( SMsg msg );
  void AnsSetNPLCycles( SMsg msg );
  void AnsRun( SMsg msg );
  void AnsSetRange( SMsg msg );
  void AnsGetRange( SMsg msg );
  void AnsScanCw( SMsg msg );
  void AnsScanCcw( SMsg msg );
  void AnsRead( SMsg msg );
  void AnsIsBusy( SMsg msg );

  void AnsSetAbsoluteVoltage( SMsg msg );
  void AnsSetAbsolutePosition( SMsg msg );
  void AnsSetRelativeVoltage( SMsg msg );
  void AnsSetRelativePosition( SMsg msg );

  void AnsSetStopMode( SMsg msg );
  void AnsSetTimerPreset( SMsg msg );
  void AnsCounterReset( SMsg msg );
  void AnsCountStart( SMsg msg );

  void AnsSetPresetType( SMsg msg );
  void AnsSetPresetValue( SMsg msg );
  void AnsGetValues( SMsg msg );
  void AnsRunStart( SMsg msg );
  void AnsRunStop( SMsg msg );
  void AnsResume( SMsg msg );
  void AnsGetRealTime( SMsg msg );
  void AnsGetLiveTime( SMsg msg );
  void AnsGetStatus( SMsg msg );
  void AnsSetROI( SMsg msg );
  void AnsSetROIs( SMsg msg );
  void AnsSetCalibration( SMsg msg );
  void AnsSetThreshold( SMsg msg );
  void AnsSetPeakingTime( SMsg msg );
  void AnsSetDynamicRange( SMsg msg );
  void AnsGetCalibration( SMsg msg );
  void AnsGetThreshold( SMsg msg );
  void AnsGetPeakingTime( SMsg msg );
  void AnsGetDynamicRange( SMsg msg );
  void AnsGetPreAMPGain( SMsg msg );
  void AnsGetMCALength( SMsg msg );
  void AnsGetMCA( SMsg msg );
  void AnsGetStatistics( SMsg msg );
  void AnsGetDataLinkCh( SMsg msg );
  void AnsGetMCAs( SMsg msg );
  void AnsSetMode( SMsg msg );
  void AnsSetCountPreset( SMsg msg );

  void EvConnected( SMsg msg );
  void EvDisconnected( SMsg msg );
  void EvChangedValue( SMsg msg );
  void EvIsBusy( SMsg msg );
  void EvReportCurrent( SMsg msg );
};

#endif
