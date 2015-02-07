#include "MonInfo.h"

MonInfo::MonInfo( void )
{
  Sensors.clear();
  SensorNames.clear();
  SensorUnits.clear();
  MeasTime = 1;
}

MonInfo::~MonInfo( void )
{
}

void MonInfo::save( QTextStream &out )
{
  out << QString( "# DwellTime\t%1\n" ).arg( MeasTime );
  for ( int i = 0; i < Sensors.count(); i++ ) {
    out << QString( "# Sensor\t%1\t%2\t%3\n" )
      .arg( Sensors[i]->getUid() )
      .arg( Sensors[i]->getUnit() )
      .arg( Sensors[i]->getName() );
  }
  out << "\n";
}

bool MonInfo::load( QTextStream &in, QVector<AUnit*> &ASensors )
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
    QStringList vals = line.mid( 2 ).split( "\t" );
    if ( vals.count() < 2 ) continue;
    if ( vals[0] == "Sensor" ) {
      f = true;
      AUnit *as = NULL;
      QString uid, unit, name;
      if ( vals.count() > 1 ) uid = vals[1]; 
      if ( vals.count() > 2 ) unit = vals[2]; 
      if ( vals.count() > 3 ) name = vals[3]; 
      for ( int j = 0; j < ASensors.count(); j++ ) {
	if ( ASensors[j]->getUid() == uid ) {
	  as = ASensors[j];
	  break;
	}
      }
      Sensors << as;
      SensorNames << name;
      SensorUnits << unit;
    }
    if ( vals[0] == "DwellTime" )
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

  return f;
}
