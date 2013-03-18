
#include <QDebug>

#include "GSBStats.h"

void GSBStats::init( int n )
{
  clear();
  for ( int i = 0; i < n; i++ ) {
    addAStat( "", PBFalse );
  }
}

void GSBStats::addAStat( QString Label, bool Stat )
{
  aGSBS *bStat = new aGSBS;

  bStat->label = Label;
  bStat->stat = Stat;

  GSBSs << bStat;
}

void GSBStats::clear( void )
{
  for ( int i = 0; i < GSBSs.count(); i++ ) {
    if ( GSBSs[i] != NULL ) {
      delete GSBSs[i];
      GSBSs[i] = NULL;
    }
  }
  GSBSs.clear();
}

void GSBStats::setAStat( int i, QString Label, bool Stat )
{
  if ( i < GSBSs.count() ) {
    GSBSs[i]->label = Label;
    GSBSs[i]->stat = Stat;
  }
}

void GSBStats::setAStat( int i, bool Stat )
{
  if ( i < GSBSs.count() ) {
    GSBSs[i]->stat = Stat;
  }
}


int GSBStats::count( void )
{
  return GSBSs.count();
}

QString GSBStats::getALabel( int i )
{
  if ( i < GSBSs.count() ) {
    return GSBSs[i]->label;
  }

  return "";
}

bool GSBStats::getAStat( int i )
{
  if ( i < GSBSs.count() ) {
    return GSBSs[i]->stat;
  }

  return PBFalse;
}

QStringList GSBStats::labels( void )
{
  QStringList rv;

  for ( int i = 0; i < GSBSs.count(); i++ ) {
    rv << GSBSs[i]->label;
  }

  return rv;
}

QVector<bool> GSBStats::flags( void )
{
  QVector<bool> rv;

  for ( int i = 0; i < GSBSs.count(); i++ ) {
    rv << GSBSs[i]->stat;
  }

  return rv;
}
