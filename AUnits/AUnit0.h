#ifndef AUNIT_H
#define AUNIT_H

#include <math.h>
#include <QObject>

#include "../SMsg.h"
#include "../Stars.h"

class AUnit0 : public QObject
{
  Q_OBJECT

 protected:
  Stars *s;
  bool Enable;          // if the unit is enable on Stars server or not
  int ALine;            // the line number where the definition appears in .def file

  int LocalStage;

  QString GType;        // Motor, Sensor
  QString Type;         // PM, PZ, ENC, ...
  QString Type2;        // defined by code (not by def file)
  QString Uid;          // Uniq ID
  QString ID;           // MainTh, StageX, General, ...
  QString Name;         // Displayed name
  QString Dev;
  QString Ch;
  QString DevCh;        // Dev + "." + Ch
  QString Unit;         // metric unit "mm", "mA", ...
  double UPP;

  QString LastFunc;     // last function which enabled isBusy
  QString LastFunc2;    // last function which enabled isBusy2

  bool IsBusy;
  bool IsBusy2;
  int Busy2Count;
  
  QString Value;
  QStringList Values;
  QString LastValue;
  int ILastSetV;
  double DLastSetV;

  bool HasParent;
  AUnit0 *TheParent;
  QString PUid;

  bool Has2ndDev;
  QString Uid2;
  QString Dev2;
  QString Ch2;
  QString DevCh2;       // Dev + "." + Ch
  AUnit0 *The2ndDev;
  
public:
  AUnit0( QObject *parent = 0 );

  void Initialize( Stars *s );
  virtual void init( void ) {};

  void setEnable( bool enable );
  virtual void _setEnable( bool /*enable*/ ) {};
  void setALine( int aline ) { ALine = aline; };
  int aLine( void ) { return ALine; };
  bool isEnable( void ) { return Enable; };

  Stars *getStars( void ) { return s; };
  QString gType( void ) { return GType; };        // Motor, Sensor
  QString type( void ) { return Type; };          // PM, PZ, ENC, ...
  QString type2( void ) { return Type2; };
  QString uid( void ) { return Uid; };            // Uniq Uid
  QString uid2( void ) { return Uid2; };         /* return Uid2; */  // 2nd Uid
  QString id( void ) { return ID; };              // MainTh, StageX, General, ...
  QString name( void ) { return Name; };          // Displayed name
  QString dev( void ) { return Dev; };
  QString ch( void ) { return Ch; };
  QString devCh( void ) { return DevCh; };        // Driver + "." + Ch
  QString unit( void ) { return Unit; };          // metric unit "mm", "mA", ...
  double upp( void ) { return UPP; };

  void setStars( Stars *S ) { s = S; };
  void setGType( QString gtype ) { GType = gtype; }; // Motor, Sensor
  void setType( QString type ) { Type = type; };     // PM, PZ, ENC, ...
  void setUid( QString uid ) { Uid = uid; };         // Uniq Uid
  void set2ndUid( QString uid ) { Uid2 = uid; };
  void setID( QString id ) { ID = id; };
  void setName( QString name ) { Name = name; };
  void setDev( QString dev )
  { Dev = dev; if ( Ch != "" ) DevCh = makeDevCh( Dev, Ch ); };
  void setCh( QString ch )
  { Ch = ch; if ( Dev != "" ) DevCh = makeDevCh( Dev, Ch ); };
  void setUnit( QString unit ) { Unit = unit; };
  void setUPP( QString upp ) { UPP = upp.toDouble(); };

  QString makeDevCh( const QString &dev, const QString &ch );

  virtual void AskIsBusy( void ) {};
  void setIsBusy( bool busy ) { IsBusy = busy; emit ChangedIsBusy1( Dev ); };
  void setIsBusy2( bool busy ) { IsBusy2 = busy; emit ChangedIsBusy2( Dev ); };
  bool isBusy0( void ) { return IsBusy || IsBusy2; };
  bool isBusy( void ) { return IsBusy; };
  bool isBusy2( void ) { return IsBusy2; };
  void busy2On( QString drv, QString name );
  void busy2Off( QString drv );
  void setBusy2Count( int i ) { Busy2Count = i; };
  void clrBusy2Count( void ) { Busy2Count = 0; };
  int busy2Count( void ) { return Busy2Count; };

  virtual void SetValue( double /* v */ ) {};
  virtual bool GetValue0( void ) { return false; };
  virtual bool GetValue02( void ) { return false; };
  virtual bool GetValue( void );
  QString value( void ) { return Value; };
  QString lastValue( void ) { return LastValue; };
  void setLastValue( QString v ) { LastValue = v; };
  QStringList values( void ) { return Values; };

  void initLocalStage( void ) { LocalStage = 0; };

  // parent : $B40A4$KDj5A$5$l(B Uid $B$r;}$D%G%P%$%9!V?F%G%P%$%9!W(B
  // $BNc$($P(B nct08 $B$NJ#?t$N%A%c%s%M%k$O%P%i%P%i$K%+%&%s%H%9%?!<%H%9%H%C%W$G$-$J$$!#(B
  // ch1 $B0J30$N%A%c%s%M%k$N%9%?!<%H%9%H%C%W$bA4It(B ch1 $B$K$^$H$a$FG$$;$k$?$a$K(B
  // ch1 $B$r(B parent $B$H$7!"B>$N(B ch $B$OA4It(B(ch1$B<+?.$b(B) ch1 $B$r?F$K;}$D(B
  void setHasParent( bool hasParent ) { HasParent = hasParent; };
  bool hasParent( void ) { return HasParent; };
  void setPUid( QString puid ) { PUid = puid; };
  QString pUid( void ) { return PUid; };
  //  void setParent( QString pUid ) { PUid = pUid; };
  void setTheParent( AUnit0 *p ) { TheParent = p; };
  AUnit0 *theParent( void ) { return TheParent; };
  
  // Dev2, Ch2, DevCh2 : $B%f%K%C%H$NDj5A$K(B2$B$D$N%I%i%$%P$,I,MW$J;~!"$=$N(B2$B$DL\$N%I%i%$%P(B
  // $BNc$($P(BKeithley $B$rEEN.(B/$BEE05%"%s%W$H$7$F;HMQ$7$F!"$=$N=PNO$rNc$($P%+%&%s%?$G?^$k>l9g(B
  // Keithley $B$NJ}$r%3%s%H%m!<%k$9$k$H%l%s%8@_Dj$,$G$-$k%+%&%s%?$N$h$&$K07$($k(B
  // $B$3$N;~%a%$%s$N%G%P%$%9$O%+%&%s%?$@$,!"(BKeithley $B$r(B2nd$B%I%i%$%P$H$7$F;XDj$9$k(B
  void setHas2ndDev( bool has2ndDev ) { Has2ndDev = has2ndDev; };
  bool has2ndDev( void ) { return Has2ndDev; };
  AUnit0 *the2ndDev( void ) { return The2ndDev; };
  void setThe2ndDev( AUnit0 *dev2 ) { The2ndDev = dev2; };
  QString dev2( void ) { return Dev2; };
  QString ch2( void ) { return Ch2; };
  QString devCh2( void ) { return DevCh2; };
  void setDev2( QString dev )
  { Dev2 = dev; if ( Ch2 != "" ) DevCh2 = makeDevCh( Dev2, Ch2 ); };
  void setCh2( QString ch )
  { Ch2 = ch; if ( Dev2 != "" ) DevCh2 = makeDevCh( Dev2, Ch2 ); };

  QString lastFunc( void ) { return LastFunc; };
  QString lastFunc2( void ) { return LastFunc2; };

 private slots:
  virtual void SetIsBusyByMsg( SMsg /* msg */ );
  void ClrBusy( SMsg msg );
  virtual void SetCurPos( SMsg /* msg */ );
  
 signals:
  void ChangedIsBusy1( QString Drv );
  void ChangedIsBusy2( QString Drv );
  void ChangedBusy2Count( QString Drv );
  void Enabled( QString Dev, bool enable );
  void NewValue( QString value );
  void NewQData( void );
  void LogMsg( QString msg );

#if 0
  //  void CountFinished( void );
  //  void newValues( void );
  void NewCountsInROI( QVector<int> );
  void NewCountsAll( QVector<int> );
  void NewTotalEvents( QVector<int> );
  void NewICRs( QVector<double> );
  //  void newDataPoints( int points );
  void Alarm( QString uid, QString msg );
#endif
};

#endif
