
#include <math.h>

#include "ScanInfo.h"

ScanInfo::ScanInfo( void )
{
  valid = false;

  am = NULL;
  
  sx0 = "-100";
  ex0 =  "100";
  dx0 =   "10";
  dt0 =  "1.0";
  sx = sx0.toDouble();
  ex = ex0.toDouble();
  dx = dx0.toDouble();
  dt = dt0.toDouble();

  relabs = REL;
  speed = MIDDLE;
  showUnit = 0;
  unitName = "Pulse";
  upp = 1;

  as = as0 = NULL;
  normalize = false;

  origin = 0;
  offset = 0;
}

ScanInfo::~ScanInfo( void )
{
}

void ScanInfo::save( QTextStream &out )
{
  out << "# UnitNames\t"
      << am->getName() << "\t" << as->getName() << "\t" << as0->getName() << "\n";
  out << "# UnitIDs\t"
      << am->getUid() << "\t" << as->getUid() << "\t" << as0->getUid() << "\n";
  out << "# Normalize\t" << ( ( normalize ) ? 1 : 0 ) << "\n";

  out << "# MetricUnit\t" << unitName << "\n";
  out << "# Unit/Pulse\t" << upp << "\n";
  out << "# Orig.Point\t" << origin << "\n";
  out << "# Offset\t"     << offset << "\n";
  out << "# Start/End/Step\t"  << sx0 << "\t" << ex0 << "\t" << dx0 << "\n";
  out << "# RelAbs\t" << relabs << "\n";
  out << "# Speed\t" << speed << "\n";
  out << "# DwellTime\t" << dt0 << "\n";
  out << "\n";
}

bool ScanInfo::load( QTextStream &in, QVector<AUnit*> &AMotors )
{
  bool f = false;
  
  am = as = as0 = NULL;
  while( !in.atEnd() ) {
    QString line = in.readLine();
    if ( line.count() < 2 ) break;
    if ( line.left( 1 ) != "#" ) break;
    if ( line.left( 2 ) != "# " ) continue;
    QStringList vals = line.mid( 2 ).split( "\t" );
    if ( vals.count() < 2 ) continue;
    if ( vals[0] == "UnitNames" ) {
      f = true;
      if ( vals.count() > 1 ) amName = vals[1]; 
      if ( vals.count() > 2 ) asName = vals[2]; 
      if ( vals.count() > 3 ) as0Name = vals[3]; 
    }
    if ( vals[0] == "UnitIDs" ) {
      if ( vals.count() >= 4 ) {
	for ( int j = 0; j < AMotors.count(); j++ ) {
	  if ( AMotors[j]->getUid() == vals[1] ) {
	    am = AMotors[j];
	    break;
	  }
	}
	for ( int j = 0; j < AMotors.count(); j++ ) {
	  if ( AMotors[j]->getUid() == vals[2] ) {
	    as = AMotors[j];
	    break;
	  }
	}
	for ( int j = 0; j < AMotors.count(); j++ ) {
	  if ( AMotors[j]->getUid() == vals[3] ) {
	    as0 = AMotors[j];
	    break;
	  }
	}
      }
    }
    if ( vals[0] == "Normalize" )
      normalize = ( vals[1].toInt() == 1 );
    if ( vals[0] == "MetricUnit" )
      unitName = vals[1];
    if ( vals[0] == "Unit/Pulse" )
      upp = vals[1].toDouble();
    if ( vals[0] == "Orig.Point" )
      origin = vals[1].toDouble();
    if ( vals[0] == "Offset" )
      offset = vals[1].toDouble();
    if ( vals[0] == "Start/End/Step" ) {
      if ( vals.count() >= 4 ) {
	sx0 = vals[1];
	ex0 = vals[2];
	dx0 = vals[3];
      }
    }
    if ( vals[0] == "RelAbs" )
      relabs = (RELABS)(vals[1].toInt());
    if ( vals[0] == "Speed" )
      speed = (MSPEED)(vals[1].toInt());
    if ( vals[0] == "DwellTime" )
      dt0 = vals[1];
  }

  if ( am != NULL ) {
    sx = am->any2p( sx0.toDouble(), showUnit, relabs );
    ex = am->any2p( ex0.toDouble(), showUnit, relabs );
  }
  if ( upp != 0 )
    dx = fabs( dx0.toDouble() / upp );
  if ( sx > ex )
    dx = -dx;
  dt = dt0.toDouble();

  return f;
}
