
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

  UseMonitors = false;
}

ScanInfo::~ScanInfo( void )
{
}

void ScanInfo::save( QTextStream &out )
{
  out << "# " << UNITNAMES << "\t"
      << am->getName() << "\t" << as->getName() << "\t" << as0->getName() << "\n";
  out << "# " << UNITIDS << "\t"
      << am->getUid() << "\t" << as->getUid() << "\t" << as0->getUid() << "\n";
  out << "# " << NORMALIZE << "\t" << ( ( normalize ) ? 1 : 0 ) << "\n";

  out << "# " << METRICUNIT << "\t" << unitName << "\n";
  out << "# " << UNIT_PULSE << "\t" << upp << "\n";
  out << "# " << ORIG_POINT << "\t" << origin << "\n";
  out << "# " << OFFSET << "\t" << offset << "\n";
  out << "# " << START_END_STEP << "\t"  << sx0 << "\t" << ex0 << "\t" << dx0 << "\n";
  out << "# " << RELABSSEL << "\t" << relabs << "\n";
  out << "# " << SPEED << "\t" << speed << "\n";
  out << "# " << SCANDWELLTIME << "\t" << dt0 << "\n";
  out << "# " << USEMONITORS << "\t" << ( UseMonitors ? 1 : 0 ) << "\n";
  mi.save( out );
  //  out << "\n";    // mi.save の中で出力される
}

bool ScanInfo::load( QTextStream &in,
		     QVector<AUnit*> &AMotors, QVector<AUnit*> &ASensors )
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
    if ( vals[0] == UNITNAMES ) {
      f = true;
      if ( vals.count() > 1 ) amName = vals[1]; 
      if ( vals.count() > 2 ) asName = vals[2]; 
      if ( vals.count() > 3 ) as0Name = vals[3]; 
    }
    if ( vals[0] == UNITIDS ) {
      if ( vals.count() >= 4 ) {
	for ( int j = 0; j < AMotors.count(); j++ ) {
	  if ( AMotors[j]->getUid() == vals[1] ) {
	    am = AMotors[j];
	    break;
	  }
	}
	for ( int j = 0; j < ASensors.count(); j++ ) {
	  if ( ASensors[j]->getUid() == vals[2] ) {
	    as = ASensors[j];
	    break;
	  }
	}
	for ( int j = 0; j < ASensors.count(); j++ ) {
	  if ( ASensors[j]->getUid() == vals[3] ) {
	    as0 = ASensors[j];
	    break;
	  }
	}
      }
    }
    if ( vals[0] == NORMALIZE )
      normalize = ( vals[1].toInt() == 1 );
    if ( vals[0] == METRICUNIT )
      unitName = vals[1];
    if ( vals[0] == UNIT_PULSE )
      upp = vals[1].toDouble();
    if ( vals[0] == ORIG_POINT )
      origin = vals[1].toDouble();
    if ( vals[0] == OFFSET )
      offset = vals[1].toDouble();
    if ( vals[0] == START_END_STEP ) {
      if ( vals.count() >= 4 ) {
	sx0 = vals[1];
	ex0 = vals[2];
	dx0 = vals[3];
      }
    }
    if ( vals[0] == RELABSSEL )
      relabs = (RELABS)(vals[1].toInt());
    if ( vals[0] == SPEED )
      speed = (MSPEED)(vals[1].toInt());
    if ( vals[0] == SCANDWELLTIME )
      dt0 = vals[1];
    if ( vals[0] == USEMONITORS )
      UseMonitors = ( vals[1].toInt() == 1 );
    bool dummyF;
    mi.load0( line, ASensors, dummyF );
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

void ScanInfo::show( void )
{
  qDebug() << "---- Stat of Scan Info ----";
  qDebug() << "valid" << valid;
  qDebug() << "motor" << am->getUid() << amName;
  qDebug() << "speed" << speed;
  qDebug() << "showUnit" << showUnit;
  qDebug() << "upp" << upp;
  qDebug() << "unit name" << unitName;
  qDebug() << "rel/abs" << relabs;
  qDebug() << "sx ex dx dt" << sx0 << ex0 << dx0 << dt0;
  qDebug() << "sx ex dx dt" << sx << ex << dx << dt;
  if ( as != NULL ) qDebug() << "sensor" << as->getUid() << asName;
  if ( as0 != NULL ) qDebug() << "sensor0" << as0->getUid() << as0Name;
  qDebug() << "normalize" << normalize;
  qDebug() << "origin offset" << origin << offset;
  qDebug() << "use monitors" << UseMonitors;
  mi.show();
}
