#ifndef AMOTOR_H
#define AMOTOR_H

#include <QObject>

#include "Stars.h"

class AUnit : public QObject
{
  Q_OBJECT

  Stars *s;

  QString Type;         // PM, PZ, ENC, ...
  QString ID;           // MainTh, StageX, General, ...
  QString UID;          // Uniq ID ( ID + number )
  QString Name;         // Disiplayed name
  QString Driver;
  QString Ch;
  QString DevCh;        // Driver + "." + Ch
  QString Unit;         // metric unit "mm", "mA", ...

  double UPP;           // Unit per Puls : only for PM
  double Center;        // Center position in puls : only for PM

  double MaxV;          // only for PZ
  double MinV;          // only for PZ

  bool isBusy;
  QString Value;

public:
  AUnit( QObject *parent = 0 );

  void setStars( Stars *S ) { s = S; };
  void setType( QString type ) { Type = type; };
  void setID( QString id ) { ID = id; };
  void setUniqID( QString num ) { UID = "N" + num + "N" + ID; };
  void setName( QString name ) { Name = name; };
  void setDriver( QString driver ) { Driver = driver; };
  void setCh( QString ch ) { Ch = ch; };
  void setDevCh( void ) {
    if ( Ch.isEmpty() )
      DevCh = Driver;
    else 
      DevCh = Driver + "." + Ch;
  }
  void setUnit( QString unit ) { Unit = unit; };

  // only for PM
  void setUPP( QString upp ) { UPP = upp.toDouble(); };
  void setCenter( QString center ) { Center = center.toDouble(); };

  // only for PZ
  void setMaxV( QString maxv ) { MaxV = maxv.toDouble(); };
  void setMinV( QString minv ) { MinV = minv.toDouble(); };

  QString getType( void ) { return Type; };
  QString getID( void ) { return ID; };
  QString getUID( void ) { return UID; };
  QString getName( void ) { return Name; };
  QString getDriver( void ) { return Driver; };
  QString getCh( void ) { return Ch; };
  QString getDevCh( void ) { return DevCh; };
  QString getUnit( void ) { return Unit; };
  bool getIsBusy( void ) { return isBusy; };
  QString value( void ) { return Value; };

  // only for PM
  double getUPP( void ) { return UPP; };
  double getCenter( void ) { return Center; };
  // only for PZ
  double getMaxV( void ) { return MaxV; };
  double getMinV( void ) { return MinV; };


  void show( void );   // mainly for debugging

  // wrapper functions of stars communication
  void GetValue( void );
  void SetValue( double v );
  void AskIsBusy( void );
  void SetSpeed( MSPEED speed );
  void Stop( void );

public slots:
  void SetCurPos( SMsg msg );
  void SetIsBusy( SMsg msg );
};

#endif


