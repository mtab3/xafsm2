#ifndef USINGUNIT_H
#define USINGUNIT_H

#include <QObject>
#include "AUnit.h"

class UsingUnit : public QObject
{
  Q_OBJECT

  QString User;
  AUnit *Unit;

 public:
  UsingUnit( QString user, AUnit *unit ) { User = user; Unit = unit; };

  bool isUnit( AUnit *unit ) { return ( Unit == unit ); };
  bool isParentUnit( AUnit *unit )
  {
    if ( Unit->hasParent() && unit->hasParent() )
      return ( Unit->getTheParent() == unit->getTheParent() );
    return false;
  };
  bool isUniq( QString user, AUnit *unit )
  { return (( user == User )&&( unit == Unit )); };
  QString user( void ) { return User; };
};

class UsingUnits : public QObject
{
  Q_OBJECT

  QVector<UsingUnit*> uUnits;

 public:
  UsingUnits() {};

  QString user( AUnit *unit ) {
    // 指定されたユニットが直接使われていたらその使用者を返す
    for ( int i = 0; i < uUnits.count(); i++ ) {
      if ( uUnits[i]->isUnit( unit ) ) {
	return uUnits[i]->user();
      }
    }
    // 指定されたユニットの親ユニットが直接使われていたらその使用者を返す
    for ( int i = 0; i < uUnits.count(); i++ ) {
      if ( uUnits[i]->isParentUnit( unit ) ) {
	return uUnits[i]->user();
      }
    }
    return "";
  }

  bool addAnUnit( QString user, AUnit *unit ) {
    for ( int i = 0; i < uUnits.count(); i++ ) {
      if ( uUnits[i]->isUnit( unit ) )
	return false;
    }
    uUnits << new UsingUnit( user, unit );
    return true;
  };

  bool removeAnUnit( QString user, AUnit *unit ) {
    for ( int i = 0; i < uUnits.count(); i++ ) {
      if ( uUnits[i]->isUniq( user, unit ) ) {
	uUnits.remove( i );
	return true;
      }
    }
    return false;
  };

  void removeUnits( QString user ) {
    for ( int i = uUnits.count() - 1; i >= 0; i-- ) {
      if ( uUnits[i]->user() == user ) {
	uUnits.remove( i );
      }
    }
  }

  bool isAnyOneUsedBy( QString user ) {
    for ( int i = 0; i < uUnits.count(); i++ ) {
      if ( uUnits[i]->user() == user )
	return true;
    }
    return false;
  }
};

#endif
