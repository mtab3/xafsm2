
#ifndef AUNITSFLUO_H
#define AUNITSFLUO_H

#include "ASensor.h"
#include "XMAPHead.h"

enum STATELM { STAT_REALTIME, STAT_TRG_LIVETIME, STAT_ENGY_LIVETIME, STAT_TRIGGERS,
	       STAT_EVENTS, STAT_ICR, STAT_OCR };

// #define AXMAPBUF    ( XMAPHEAD + 2048 * 4 ) // MCAHEAD + 2048 pixels * 4byte
// #define XMAPBUFSIZE ( AXMAPBUF * 19 )       // AMCABUF * 19 ch

class AUnitSFluo : public ASensor
{
  Q_OBJECT

  bool connectingDLink;

  int SSDChs;
  quint64 mcaLength;
  int AChBufSize;
  int AllChBufSize;
  
  QString DataLinkHostName;
  qint16 DataLinkHostPort;
  QTcpSocket *dLink;
  QDataStream *dLinkStream;
  int dLinkCount;

  char *MCAs0, *MCAs;
  bool MCAsReady;    // MCAs に有効なデータがある true, 無い false

  QString SSDPresetType;
  QString *ROIStart, *ROIEnd;
  QVector<quint64> CountsInROI;
  QVector<quint64> CountsAll;
  QVector<quint64> TotalEvents;
  QVector<double> ICRs;
  QVector<double> DarkCountsInROI;    // per second
  QVector<double> DarkCountsAll;      // per second
  QVector<double> DarkTotalEvents;    // per second
  QVector<double> DarkICRs;           // per second
  QStringList MCAStats;
  QVector<double> MCARealTime;
  QVector<double> MCALiveTime;
  QVector<bool> SSDUsingCh;

  void resetVectors( int chs );
  void setBufSize( void );
  
 public:
  AUnitSFluo( QObject *p = NULL );

  bool InitSensor( void );
  void init0( void );
  void _setEnable( bool enable );
  void ConnectToSFluoDataLinkServer( QString host, qint16 port );
  bool isSFluo( void ) { return true; };

  bool GetValue0( void );
  bool GetValue02( void );
  void RunStart( void );
  void RunStop( void );
  void RunResume( void );
  void setLength( int len );
  void setChs( int chs );
  int length( void ) { return mcaLength; };
  int chs( void ) { return SSDChs; };

  void setSSDPresetType( QString type ) { SSDPresetType = type; };
  QString getSSDPresetType( void ) { return SSDPresetType; };

  double SetTime( double dtime );
  bool GetMCAs( void );
  bool GetStat( void );
  //  bool SetRealTime( double val );  // xmap2
  //  bool SetLiveTime( double val );  // xmap2
  bool SetRealTime( int ch, double val );
  bool SetLiveTime( int ch, double val );
  bool GetRealTime( int ch );
  bool GetLiveTime( int ch );

  void SetDark( double val, double dtime );
  
  QVector<quint64> getCountsInROI( void ) { return CountsInROI; };
  QVector<quint64> getCountsAll( void ) { return CountsAll; };
  QVector<quint64> getTotalEvents( void ) { return TotalEvents; };
  QVector<double> getICRs( void ) { return ICRs; };
  QVector<double> getDarkCountsInROI( void ) { return DarkCountsInROI; };
  QVector<double> getDarkCountsAll( void ) { return DarkCountsAll; };
  QVector<double> getDarkTotalEvents( void ) { return DarkTotalEvents; };
  QVector<double> getDarkICRs( void ) { return DarkICRs; };
  quint32 *getAMCA( int ch );
  quint32 getAMCAdata( int ch, int pixel );
  XMAPHead getAMCAHead( int ch );
  
  void setGain( int ch, double gain );
  void setROIs( QString *roistart, QString *roiend )
  { ROIStart = roistart; ROIEnd = roiend; };

  void SetLowLimit( int ch, int llpix );
  void setSSDUsingCh( int i, bool f )
  { if ( i < SSDChs ) SSDUsingCh[i] = f; };
  bool getSSDUsingCh( int i )
  { if ( i < SSDChs ) return SSDUsingCh[i]; else return false; };

  double realTime( int ch );
  double liveTime( int ch );

  double stat( int ch, STATELM i );
  bool GetValue( void );
  bool Close( void );
  
 public slots:
  void SetIsBusyByMsg( SMsg msg );
  void getMCALength( SMsg msg );
  void ReactGetStat( SMsg msg );
  void ReactGetRealTime( SMsg msg );
  void ReactGetLiveTime( SMsg msg );
  void ReactGetDataLinkCh( SMsg msg );
  void ReceiveValues( SMsg msg );
  void receiveMCAs( void );

 signals:
  void ReceivedNewMCARealTime( int i );
  void ReceivedNewMCALiveTime( int i );
  void DataLinkServerIsReady( QString host, qint16 port );
  void NewMCAsAvailable( char *MCAs );
};

#endif







