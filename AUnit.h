#ifndef AMOTOR_H
#define AMOTOR_H

#include <QObject>

#include "XafsM.h"
#include "Stars.h"
#include "MCAHead.h"

#define MCAHEAD    ( 6 * 8 )              // 6 values * 8 bytes
// qint64  ch, stat, mcaLength
// qreal64 realTime, liveTime, icr
#define AMCABUF    ( MCAHEAD + 2048 * 4 ) // MCAHEAD + 2048 pixels * 4byte
#define MCABUFSIZE ( AMCABUF * 19 )       // AMCABUF * 19 ch

enum STATELM { STAT_REALTIME, STAT_TRG_LIVETIME, STAT_ENGY_LIVETIME, STAT_TRIGGERS,
	       STAT_EVENTS, STAT_ICR, STAT_OCR };

class AUnit : public QObject
{
  Q_OBJECT

  Stars *s;
  bool Enable;          // if the unit is enable on Stars server or not
  int aLine;            // the line number where the definition appears in .def file
  bool autoRange;
  bool ConnectedToSSDServer;

  QString GType;        // Motor, Sensor
  QString Type;         // PM, PZ, ENC, ...
  QString Uid;          // Uniq ID
  QString Uid2;         // 2nd Uid
  QString ID;           // MainTh, StageX, General, ...
  QString Name;         // Displayed name
  QString Driver;
  QString Driver2;      // 2nd Driver name
  QString Ch;
  QString Ch2;          // 2nd Ch;
  QString DevCh;        // Driver + "." + Ch
  QString DevCh2;       // 2nd DevCh
  QString Unit;         // metric unit "mm", "mA", ...
  QString LastFunc;     // last function which enabled isBusy
  QString LastFunc2;    // last function which enabled isBusy2
  double UPP;           // Unit per Puls : only for PM
  bool IsInt;           // if the controlling unit is integer or not.
  bool HasParent;       // if the unit have parent (group leader).
  QString PUid;         // the Uid of the parenet
  AUnit *theParent;     // the parent
  AUnit *the2ndDriver;  // the 2nd Driver unit
  bool Has2ndDriver;    // if the unit has second driver or not.
  bool RangeSelectable; // if range is selectable for the unit
  int RangeU;           // Upper range limit
  int RangeL;           // Lower range limit
  int SelectedRange;
  double setTime;       // Actually set time;
  double setDarkTime;   // Actually set time;
  int points;           // Measured Data Points : 34410

  double MaxIntTime;    // Maximum integration time
  bool HasMaxIntTime;

  double Center;        // Center position in puls : only for PM

  double MaxV;          // only for PZ
  double MinV;          // only for PZ

  bool HasSpeedsLine;
  int MaxS;      // max speed (pps) 最初に設定されていたオリジナルのスピード
  int MaxMaxS;   // 許される最高のスピード
  int AccRate;   // 加減速レート ( AccRage ms/1000pps )
  int AccRateNo; // 対応する加減速レートの PM16C のテーブル番号

  bool hasConnected;
  QTcpSocket *dLink;
  QDataStream *dLinkStream;
  int dLinkCount;
  char *MCAs0, *MCAs;
  bool MCAsReady;    // MCAs に有効なデータがある true, 無い false

  QString DataLinkHostName;
  qint16 DataLinkHostPort;
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

  bool IsBusy;
  bool IsBusy2;
  int Busy2Count;
  QString Value;
  double Dark;                 // back ground value normalized for 1 sec
  QString lastVal;
  quint64 MCALength;
  QStringList Values;
  QStringList MCAValues;
  QStringList MCAStats;
  double MCARealTime[ MaxSSDs + 1 ];
  double MCALiveTime[ MaxSSDs + 1 ];
  //  int MCALowLimit[ MaxSSDs + 1 ];
  bool SSDUsingCh[ MaxSSDs + 1 ];       // Only 19 is necessary, 20 is only for safety.

  int LocalStage;
  int ilastSetV;
  double dlastSetV;

 private:
  bool TypeCHK( int pm, int pz, int cnt, int pam, int enc, int ssd, int ssdp,
		int cnt2, int sc, int otc, int otc2, int lsr, int dv, int dv2, int enc2,
    int pam2, int ccg, int aioi, int aioo, int fp23 );
  void ConnectToDataLinkServer( QString host, qint16 port );

 private slots:
  void receiveMCAs( void );

public:
  AUnit( QObject *parent = 0 );

  void Initialize( Stars *S );
  void setEnable( bool enable );
  void setALine( int aline ) { aLine = aline; };
  bool isEnable( void ) { return Enable; };
  void setGType( QString gtype ) { GType = gtype; };
  void setStars( Stars *S ) { s = S; };
  void setType( QString type ) { Type = type; };
  void setID( QString id ) { ID = id; };
  void setUid( QString uid ) { Uid = uid; };
  void set2ndUid( QString uid ) { Uid2 = uid; };
  void setName( QString name ) { Name = name; };
  void setDriver( QString driver ) { Driver = driver; };
  void setCh( QString ch ) { Ch = ch; };
  void setDevCh( void ) {
    if ( Ch.isEmpty() )
      DevCh = Driver;
    else 
      DevCh = Driver + "." + Ch;
  }
  void set2ndDriver( QString driver ) { Driver2 = driver; };
  void set2ndCh( QString ch ) { Ch2 = ch; };
  void set2ndDevCh( void ) {
    if ( Ch2.isEmpty() )
      DevCh2 = Driver2;
    else 
      DevCh2 = Driver2 + "." + Ch2;
  }
  void setUnit( QString unit ) { Unit = unit; };
  void setIsBusy( bool busy ) { IsBusy = busy; emit ChangedIsBusy1( Driver ); };
  void setIsBusy2( bool busy ) { IsBusy2 = busy; emit ChangedIsBusy2( Driver ); };
  void setIsInt( bool isInt ) { IsInt = isInt; };
  void setHasParent( bool hasParent ) { HasParent = hasParent; };
  void setParent( QString pUid ) { PUid = pUid; };
  void setTheParent( AUnit *p ) { theParent = p; };
  void setThe2ndDriver( AUnit *p ) { the2ndDriver = p; };
  void setHas2ndDriver( bool has2ndDriver ) { Has2ndDriver = has2ndDriver; };
  void setSSDPresetType( QString type ) { SSDPresetType = type; };
  QString getSSDPresetType( void ) { return SSDPresetType; };
  void setROIs( QString *roistart, QString *roiend )
  { ROIStart = roistart; ROIEnd = roiend; };
  void setSSDUsingCh( int i, bool f ) { if ( i < MaxSSDs ) SSDUsingCh[i] = f; };
  void setRangeSelectable( bool f ) { RangeSelectable = f; };
  void setRangeU( int upper ) { RangeU = upper; };
  void setRangeL( int lower ) { RangeL = lower; };
  void setRange( int r ) { SelectedRange = r; };
  void setDark( double dark ) { Dark = dark; emit newDark( Dark ); };
  void setDark( void )
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
  bool isAutoRangeAvailable( void );
  bool isAutoRange( void ) { return autoRange; };
  void setAutoRange( bool ar ) { autoRange = ar; };
  void setGain( int ch, double gain );

  bool checkNewVal( void )
  {
    bool rv = ( Value != lastVal );
    lastVal = Value;
    return rv;
  };

  double u2p( double u ) { return u / UPP + Center; };
  double p2u( double p ) { return ( p - Center ) * UPP; };
  double any2p( double a, int selU, RELABS ra ) {
    return a / ( ( selU == 0 ) ? 1 : UPP )
      + ( ( ra == REL ) ? Value.toDouble() : ( ( selU == 0 ) ? 0 : Center ) );
  }

  // only for PM
  void setUPP( QString upp ) { UPP = upp.toDouble(); };
  void setCenter( QString center ) { Center = center.toDouble(); };
  void setCenter( double center ) { Center = center; };

  // only for PZ
  void setMaxV( QString maxv ) { MaxV = maxv.toDouble(); };
  void setMinV( QString minv ) { MinV = minv.toDouble(); };

  // only for DV
  void setMaxIntTime( double t ) { MaxIntTime = t; HasMaxIntTime = true; };

  int getALine( void ) { return aLine; };
  QString getGType( void ) { return GType; };
  QString getType( void ) { return Type; };
  QString getID( void ) { return ID; };
  QString getUid( void ) { return Uid; };
  QString get2ndUid( void ) { return Uid2; };
  QString getName( void ) { return Name; };
  QString getDriver( void ) { return Driver; };
  QString getCh( void ) { return Ch; };
  QString getDevCh( void ) { return DevCh; };
  QString get2ndDriver( void ) { return Driver2; };
  QString get2ndCh( void ) { return Ch2; };
  QString get2ndDevCh( void ) { return DevCh2; };
  QString getUnit( void ) { return Unit; };
  bool isBusy0( void ) { return IsBusy || IsBusy2; };
  bool isBusy( void ) { return IsBusy; };
  bool isBusy2( void ) { return IsBusy2; };
  QString value( void ) { return Value; };
  double metricValue( void ) { return ( Value.toDouble() - Center ) * UPP; };
  QStringList values( void ) { return Values; };
  QStringList MCAvalues( void ) { return MCAValues; };
  double getUPP( void ) { return UPP; };
  bool isInt( void ) { return IsInt; };
  bool hasParent( void ) { return HasParent; };
  bool has2ndDriver( void ) { return Has2ndDriver; };
  QString getPUid( void ) { return PUid; };
  AUnit *getTheParent( void ) { return theParent; };
  double stat( int ch, STATELM i );
  double stat( STATELM i );
  double realTime( int ch );
  double liveTime( int ch );
  void SetLowLimit( int ch, int llpix );
  bool getSSDUsingCh( int i )
  { if ( i < MaxSSDs ) return SSDUsingCh[i]; else return false; };
  bool isRangeSelectable( void ) { return RangeSelectable; };
  int getRangeU( void ) { return RangeU; };
  int getRangeL( void ) { return RangeL; };
  int getRange( void ) { return SelectedRange; };
  double getDark( void ) { return Dark; };
  QString lastFunc( void ) { return LastFunc; };
  QString lastFunc2( void ) { return LastFunc2; };
  int Points( void ) { return points; };

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
  MCAHead getAMCAHead( int ch );

  int getILastSetV( void ) { return ilastSetV; };
  double getDLastSetV( void ) { return dlastSetV; };

  // only for PM
  double getCenter( void ) { return Center; };
  // only for PZ
  double getMaxV( void ) { return MaxV; };
  double getMinV( void ) { return MinV; };
  int highSpeed( void ) { return MaxS; };     // オリジナルのハイスピード
  void setHighSpeed( int h ) { MaxS = h; };
  int highestSpeed( void ) { return ( HasSpeedsLine ) ? MaxMaxS : MaxS; };// 許容最高速度
  void setHighestSpeed( int h ) { MaxMaxS = h; };
  int accRate( void ) { return AccRate; };    // 加減速レート
  void setAccRate( int r ) { AccRate = r; };
  int accRateNo( void ) { return AccRateNo; }; // 加減速レートのテーブル番号
  void setAccRateNo( int n ) { AccRateNo = n; };
  bool hasSpeedsLine( void ) { return HasSpeedsLine; };
  void setHasSpeedsLine( bool f ) { HasSpeedsLine = f; };

  // only for DV
  bool hasMaxIntTime( void ) { return HasMaxIntTime; };
  double maxIntTime( void ) { return MaxIntTime; };

  void show( void );   // mainly for debugging

  // wrapper functions of stars communication
  void InitLocalStage( void );
  bool InitSensor( void );
  bool GetValue0( void );
  bool GetValue02( void );
  bool GetValue( void );
  void SetValue( double v );
  bool Close( void );
  void RunStart( void );
  void RunStop( void );
  void RunResume( void );
  void AskIsBusy( void );
  bool QStart( void );                 // QXAFS
  bool QRead( void );                 // QXAFS
  bool QEnd( void );                  // QXAFS
  void SetSpeed( MSPEED speed );
  void SetHighSpeed( int speed );
  void AssignDispCh( int ch );  // ch : 0 - 3 --> 'A' -- 'D'
  void SetTimingOutMode( int mode );
  // 0 - 5 :: 0: none, 1: cont., 2: 200ns, 3: 10us, 4: 100us, 5: 1ms
  // 34410 triggers rising edge and requires 1us or longer
  // for EB741 2us is long enough
  void SetTimingOutStart( int startP );   // start position of timing out
  void SetTimingOutEnd( int endP );       // end position of timing out
  void SetTimingOutInterval( int interval );  // timing out interval
  void SetTimingOutReady( int ready );  // timing out ready
  double SetTime( double dtime );   // in sec
  void Stop( void );

  // 3440
  void SetTriggerDelay( double time );
  void SetSamplingSource( QString source );
  void SetTriggerSource( QString source );
  void SetTriggerCounts( int count );
  void SetTriggerSlope( QString type );
  void GetDataPoints( void );
  void ReadDataPoints( int points );
  void Abort( void );
  void GoMaxAbs( double Start, double End, int Steps );
  void GoMaxAbsQ( double Start, double End, int Steps, double Time );
  void GoMaxRel( double Width, int Steps );
  void GoMaxRelQ( double Width, int Steps, double Time );

#if 0                   // new mcas
  bool GetMCA( int ch );
#endif
  bool GetMCAs( void );
  bool GetStat( void );
  bool SetRealTime( double val );
  bool SetLiveTime( double val );
  bool SetRealTime( int ch, double val );
  bool SetLiveTime( int ch, double val );
  bool GetRealTime( int ch );
  bool GetLiveTime( int ch );
  bool GetRange( void );
  void SetRange( int range );
  double GetSetTime( void ) { return setTime; };   // actual set time

  void IsBusy2On( QString drv, QString name );
  void IsBusy2Off( QString drv );
  void setBusy2Count( int i ) { Busy2Count = i; };
  void clrBusy2Count( void ) { Busy2Count = 0; };
  int busy2Count( void ) { return Busy2Count; };

public slots:
  void ClrBusy( SMsg msg );
  void SetIsBusyByMsg( SMsg msg );
  void SetCurPos( SMsg msg );
  void ReceiveValues( SMsg msg );
  void getMCALength( SMsg msg );
  void RcvAnsGetValueOfDriver( SMsg msg );

#if 0           // new mcas
  void ReactGetMCA( SMsg msg );
#endif
  void ReactGetStat( SMsg msg );
  void ReactGetRealTime( SMsg msg );
  void ReactGetLiveTime( SMsg msg );
  void ReactGetRange( SMsg msg );
  void OnReportCurrent( SMsg msg );
  void OnReportInjection( SMsg msg );
  void OnReportValue( SMsg msg );
  void ReactGetDataLinkCh( SMsg msg );

  //  void RcvDataPoints( SMsg msg );
  //  void RcvReadData( SMsg msg );
  void RcvStat( SMsg msg );
  void RcvQGetData( SMsg msg );
  void RcvHighSpeed( SMsg msg );

  void getNewValue( QString v );   // only for SSD childlen
  void getNewDark( double d );     // only for SSD childlen

  void CloseShutter( bool close );

signals:
  //  void CountFinished( void );
  void newValue( QString value );
  //  void newValues( void );
  void newDark( double dark );
  void newCountsInROI( QVector<int> );
  void newCountsAll( QVector<int> );
  void newTotalEvents( QVector<int> );
  void newICRs( QVector<double> );
  //  void newDataPoints( int points );
  void newQData( void );

  void Enabled( QString Drv, bool flag );
  void ChangedIsBusy1( QString Drv );
  void ChangedIsBusy2( QString Drv );
  void ChangedBusy2Count( QString Drv );
  void AskedNowRange( int r );
#if 0           // new mcas
  void ReceivedNewMCAValue( void );
#endif
  void ReceivedNewMCARealTime( int i );
  void ReceivedNewMCALiveTime( int i );
  void NewRingCurrent( QString val, QStringList vals );
  void NewInjectionReport( QString val, QStringList vals );
  void NewFP23Temperature( QString val );
  void DataLinkServerIsReady( QString host, qint16 port );
  void NewMCAsAvailable( char *MCAs );

  void LogMsg( QString msg );
};

#endif


