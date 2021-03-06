#include <QDebug>

#include "MonInfo.h"

MonInfo::MonInfo( void )
{
  Sensors.clear();
  SensorNames.clear();
  SensorUnits.clear();
  MeasTime = 1;
  valid = false;
}

MonInfo::~MonInfo( void )
{
}

void MonInfo::save( QTextStream &out )
{
  out << QString( "# %1\t%2\n" ).arg( MEASTIME ).arg( MeasTime );
  for ( int i = 0; i < Sensors.count(); i++ ) {
    out << QString( "# %1\t%2\t%3\t%4\n" ).arg( MONSENSOR )
      .arg( Sensors[i]->uid() )
      .arg( Sensors[i]->unit() )
      .arg( Sensors[i]->name() );
  }
  out << "\n";
}

bool MonInfo::load( QTextStream &in, QVector<ASensor*> &ASensors )
{
  bool f = false;

  Sensors.clear();
  SensorNames.clear();
  SensorUnits.clear();

  MeasTime = 1;
  while( !in.atEnd() ) {
    QString line = in.readLine();
    if ( line.count() < 2 ) break;
    if ( line.left( 1 ) != "#" ) break;
    if ( line.left( 2 ) != "# " ) continue;
    load0( line, ASensors, f );
  }

  return f;
}

void MonInfo::load0( QString line, QVector<ASensor*> &ASensors, bool &f )
{
  QStringList vals = line.mid( 2 ).split( "\t" );
  if ( vals.count() < 2 ) return ;
  if ( vals[0] == MONSENSOR ) {
    f = true;
    ASensor *as = NULL;
    QString uid, unit, name;
    if ( vals.count() > 1 ) uid = vals[1]; 
    if ( vals.count() > 2 ) unit = vals[2]; 
    if ( vals.count() > 3 ) name = vals[3]; 
    for ( int j = 0; j < ASensors.count(); j++ ) {
      if ( ASensors[j]->uid() == uid ) {
	as = ASensors[j];
	break;
      }
    }
    Sensors << as;
    SensorNames << name;
    SensorUnits << unit;
  }
  if ( vals[0] == MEASTIME )
    MeasTime = vals[1].toDouble();
  
  if ( vals[0] == "sec" ) {  // Old format
    for ( int i = 1; i < vals.count(); i++ ) {
      QString item = vals[i];
      int i1 = item.indexOf( '[' );
      int i2 = item.lastIndexOf( ']' );
      QString name, unit;
      if ( i1 >= 0) {
	name = item.left( i1 );
	if (( i2 >= 0 ) && ( i2 > i1 )) {
	  unit = item.mid( i1+1, i2 - i1 - 1 );
	}
      } else {
	name = item;
      }
      Sensors << NULL;
      SensorNames << name;
      SensorUnits << unit;
    }
  }
}

void MonInfo::show( void )
{
  qDebug() << "---- Stat of MonInfo ----";
  qDebug() << "valid" << valid;
  for ( int i = 0; i < Sensors.count(); i++ ) {
    qDebug() << i << Sensors[i]->uid() << SensorNames[i] << SensorUnits[i];
  }
  qDebug() << "MeasTime" << MeasTime;
}
