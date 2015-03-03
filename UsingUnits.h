#ifndef USINGUNIT_H
#define USINGUNIT_H

#include <QObject>
#include "AUnits/AUnits.h"

class UsingUnit : public QObject
{
  Q_OBJECT

  QString User;
  AUnit0 *Unit;

 public:
  UsingUnit( QString user, AUnit0 *unit ) { User = user; Unit = unit; };

  AUnit0 *unit( void ) { return Unit; };
  bool isUnit( AUnit0 *unit ) { return ( Unit == unit ); };
  bool isParentUnit( AUnit0 *unit )
  {
    if ( Unit->hasParent() && unit->hasParent() )
      return ( Unit->theParent() == unit->theParent() );
    return false;
  };
  bool isUniq( QString user, AUnit0 *unit )
  { return (( user == User )&&( unit == Unit )); };
  QString user( void ) { return User; };
};

class UsingUnits : public QObject
{
  Q_OBJECT

  QVector<UsingUnit*> uUnits;

 public:
  UsingUnits() {};

  QString user( AUnit0 *unit ) {
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

  bool addAnUnit( QString user, AUnit0 *unit ) {
    for ( int i = 0; i < uUnits.count(); i++ ) {
      if ( uUnits[i]->isUnit( unit ) )
	return false;
    }
    uUnits << new UsingUnit( user, unit );
    unit->setUsing( true );
    unit->setUserName( user );
    return true;
  };

  bool removeAnUnit( QString user, AUnit0 *unit ) {
    for ( int i = 0; i < uUnits.count(); i++ ) {
      if ( uUnits[i]->isUniq( user, unit ) ) {
	uUnits.remove( i );
	unit->setUsing( false );
	unit->setUserName( "" );
	return true;
      }
    }
    return false;
  };

  void removeUnits( QString user ) {
    for ( int i = uUnits.count() - 1; i >= 0; i-- ) {
      if ( uUnits[i]->user() == user ) {
	uUnits[i]->unit()->setUsing( false );
	uUnits[i]->unit()->setUserName( "" );
	uUnits.remove( i );
      }
    }
  }
  void removeUnits( QString user, AUnit0 *unit ) {
    for ( int i = uUnits.count() - 1; i >= 0; i-- ) {
      if (( uUnits[i]->user() == user )&&( uUnits[i]->unit() == unit )) {
	uUnits[i]->unit()->setUsing( false );
	uUnits[i]->unit()->setUserName( "" );
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
