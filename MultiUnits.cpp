
#include "MultiUnits.h"

MUnits::MUnits( QObject *p ) : QObject( p )
{
  isParent = false;
}

void MUnits::clearUnits( void )
{
  for ( int i = 0; i < Units.count(); i++ ) {
    delete Units.at(i)->au;
  }
  Units.clear();
  for ( int i = 0; i < PUnits.count(); i++ ) {
    delete PUnits.at(i)->au;
  }
  PUnits.clear();
};

void MUnits::addUnit( AUnit *au, double dt )
{
  int i;
  MUElement *mue = new MUElement;
  mue->au = au;
  mue->dt = dt;
  Units << mue;

  if ( au->hasParent() ) {
    for ( i = 0; i < PUnits.count(); i++ ) {
      if ( PUnits.at(i)->au->getUid() == au->getPUid() )
	break;
    }
    if ( i < PUnits.count() ) {
      MUElement *pmue = new MUElement;
      pmue->au = au->getTheParent();
      pmue->dt = dt;
      PUnits << pmue;
    }
  }
}
