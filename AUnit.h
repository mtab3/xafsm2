#ifndef AMOTOR_H
#define AMOTOR_H

#include <QObject>

#include "XafsM.h"
#include "Stars.h"

enum STATELM { STAT_REALTIME, STAT_TRG_LIVETIME, STAT_ENGY_LIVETIME, STAT_TRIGGERS,
	       STAT_EVENTS, STAT_ICR, STAT_OCR };

class AUnit : public QObject
{
  Q_OBJECT

  Stars *s;
  bool Enable;          // if the unit is enable on Stars server or not
  int aLine;            // the line number where the definition appears in .def file
  bool autoRange;

  QString GType;        // Motor, Sensor
  QString Type;         // PM, PZ, ENC, ...
  QString Uid;          // Uniq ID
  QString Uid2;         // 2nd Uid
  QString ID;           // MainTh, StageX, General, ...
  QString Name;         // Displayed name
  QString Driver;
  QString Driver2;      // 2nd Driver
  QString Ch;
  QString Ch2;          // 2nd Ch;
  QString DevCh;        // Driver + "." + Ch
  QString DevCh2;       // 2nd DevCh
  QString Unit;         // metric unit "mm", "mA", ...
  double UPP;           // Unit per Puls : only for PM
  bool IsInt;           // if the controlling unit is integer or not.
  bool HasParent;       // if the unit have parent (group leader).
  QString PUid;         // the Uid of the parenet
  AUnit *theParent;     // the parent
  bool Has2ndDriver;    // if the unit has second driver or not.
  bool RangeSelectable; // if range is selectable for the unit
  int RangeU;           // Upper range limit
  int RangeL;           // Lower range limit
  int SelectedRange;
  double setTime;       // Actually set time;
  double setDarkTime;   // Actually set time;

  double Center;        // Center position in puls : only for PM

  double MaxV;          // only for PZ
  double MinV;          // only for PZ

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
  QVector<int> CountsInROI;
  QVector<int> CountsAll;
  QVector<int> TotalEvents;
  QVector<double> ICRs;
  QVector<double> DarkCountsInROI;    // per second
  QVector<double> DarkCountsAll;      // per second
  QVector<double> DarkTotalEvents;    // per second
  QVector<double> DarkICRs;           // per second

  bool IsBusy;
  bool IsBusy2;
  QString Value;
  double Dark;                 // back ground value normalized for 1 sec
  QString lastVal;
  QStringList Values;
  QStringList MCAValues;
  QStringList MCAStats;
  double MCARealTime[ MaxSSDs + 1 ];
  double MCALiveTime[ MaxSSDs + 1 ];
  bool SSDUsingCh[ MaxSSDs + 1 ];       // Only 19 is necessary, 20 is only for safety.

  int LocalStage;
  int ilastSetV;
  double dlastSetV;

 private:
  bool TypeCHK( int pm, int pz, int cnt, int pam, int enc,
		int ssd, int ssdp, int cnt2, int sc, int otc, int otc2, int lsr );
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

  // only for PZ
  void setMaxV( QString maxv ) { MaxV = maxv.toDouble(); };
  void setMinV( QString minv ) { MinV = minv.toDouble(); };

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
  bool isBusy( void ) { return IsBusy; };
  bool isBusy2( void ) { return IsBusy2; };
  QString value( void ) { return Value; };
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
  bool getSSDUsingCh( int i )
  { if ( i < MaxSSDs ) return SSDUsingCh[i]; else return false; };
  bool isRangeSelectable( void ) { return RangeSelectable; };
  int getRangeU( void ) { return RangeU; };
  int getRangeL( void ) { return RangeL; };
  int getRange( void ) { return SelectedRange; };
  double getDark( void ) { return Dark; };

  QVector<int> getCountsInROI( void ) { return CountsInROI; };
  QVector<int> getCountsAll( void ) { return CountsAll; };
  QVector<int> getTotalEvents( void ) { return TotalEvents; };
  QVector<double> getICRs( void ) { return ICRs; };
  QVector<double> getDarkCountsInROI( void ) { return DarkCountsInROI; };
  QVector<double> getDarkCountsAll( void ) { return DarkCountsAll; };
  QVector<double> getDarkTotalEvents( void ) { return DarkTotalEvents; };
  QVector<double> getDarkICRs( void ) { return DarkICRs; };
  unsigned long *getAMCA( int ch );
  unsigned long *getAMCAHead( int ch );
  unsigned long getAMCAdata( int ch, int pixel );

  int getILastSetV( void ) { return ilastSetV; };
  double getDLastSetV( void ) { return dlastSetV; };

  // only for PM
  double getCenter( void ) { return Center; };
  // only for PZ
  double getMaxV( void ) { return MaxV; };
  double getMinV( void ) { return MinV; };

  void show( void );   // mainly for debugging

  // wrapper functions of stars communication
  void InitLocalStage( void );
  bool InitSensor( void );
  bool GetValue0( void );
  bool GetValue( void );
  void SetValue( double v );
  void RunStart( void );
  void RunStop( void );
  void RunResume( void );
  void AskIsBusy( void );
  void SetSpeed( MSPEED speed );
  double SetTime( double dtime );   // in sec
  void Stop( void );
  bool GetMCA( int ch );
  bool GetMCAs( void );
  bool GetStat( void );
  bool SetRealTime( double val );
  bool SetLiveTime( double val );
  bool SetRealTime( int ch, double val );
  bool SetLiveTime( int ch, double val );
  bool GetRealTime( int ch );
  bool GetLiveTime( int ch );
  bool GetRange( void );
  double GetSetTime( void ) { return setTime; };   // actual set time

public slots:
  void ClrBusy( SMsg msg );
  void SetIsBusyByMsg( SMsg msg );
  void SetCurPos( SMsg msg );
  void ReceiveValues( SMsg msg );

  void ReactGetMCA( SMsg msg );
  void ReactGetStat( SMsg msg );
  void ReactGetRealTime( SMsg msg );
  void ReactGetLiveTime( SMsg msg );
  void ReactGetRange( SMsg msg );
  void OnReportCurrent( SMsg msg );
  void ReactGetDataLinkCh( SMsg msg );

  void getNewValue( QString v );   // only for SSD childlen
  void getNewDark( double d );     // only for SSD childlen

signals:
  //  void CountFinished( void );
  void newValue( QString value );
  void newDark( double dark );
  void newCountsInROI( QVector<int> );
  void newCountsAll( QVector<int> );
  void newTotalEvents( QVector<int> );
  void newICRs( QVector<double> );

  void Enabled( QString Drv, bool flag );
  void ChangedIsBusy1( QString Drv );
  void ChangedIsBusy2( QString Drv );
  void AskedNowRange( int r );
  void ReceivedNewMCAValue( void );
  void ReceivedNewMCARealTime( int i );
  void ReceivedNewMCALiveTime( int i );
  void NewRingCurrent( QString val, QStringList vals );
  void DataLinkServerIsReady( QString host, qint16 port );
  void NewMCAsAvailable( char *MCAs );
};

#endif


