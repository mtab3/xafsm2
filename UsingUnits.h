#ifndef USINGUNIT_H
#define USINGUNIT_H

#include <QObject>
#include "AUnit.h"

class UsingUnit : public QObject
{
  Q_OBJECT

  QString ID;
  AUnit *Unit;

 public:
  UsingUnit( QString id, AUnit *u ) { ID = id; Unit = u; };

  bool isUnit( AUnit *u ) { return ( Unit == u ); };
  bool isUniq( QString id, AUnit *u ) { return ( ( id == ID )&&( Unit == u ) ); };
  QString id( void ) { return ID; };
};

class UsingUnits : public QObject
{
  Q_OBJECT

  QVector<UsingUnit*> UUnits;

 public:
  UsingUnits() {};

  QString isTheUnitInUse( AUnit *Unit ) {
    for ( int i = 0; i < UUnits.count(); i++ ) {
      if ( UUnits[i]->isUnit( Unit ) ) {
	return UUnits[i]->id();
      }
    }
    return "";
  }

  void addUnit( QString ID, AUnit *Unit ) {
    UUnits << new UsingUnit( ID, Unit );
  };

  bool deleteUnit( QString ID, AUnit *Unit ) {
    for ( int i = 0; i < UUnits.count(); i++ ) {
      if ( UUnits[i]->isUniq( ID, Unit ) ) {
	UUnits.remove( i );
	return true;
      }
    }
    return false;
  };

  void clear( QString ID ) {
    qDebug() << "1:UUcount" << UUnits.count();
    for ( int i = UUnits.count() - 1; i >= 0; i-- ) {
      if ( UUnits[i]->id() == ID ) {
	UUnits.remove( i );
      }
    }
    qDebug() << "2:UUcount" << UUnits.count();
    for ( int i = 0; i < UUnits.count(); i++ ) {
      qDebug() << "remain " << UUnits[i]->id();
    }
  }
};

#endif
