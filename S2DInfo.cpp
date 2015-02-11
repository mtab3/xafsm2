
#include "S2DInfo.h"

S2DInfo::S2DInfo( void )
{
  valid = false;

  motors = 3;
  ScanBothDir = false;
  Use3rdAx = false;
  isSFluo = false;
  unit << NULL << NULL << NULL;
  used << true << true << false;
  ps << 0 << 0 << 0;
  now << 0 << 0 << 0;
  sx << 0 << 0 << 0;
  ex << 0 << 0 << 0;
  dx << 0 << 0 << 0;
  i << 0 << 0 << 0;
  ScanMode = STEP;
}

#define SCANMODE  "Scan Mode"
#define SCANDIR   "Scan dir"
#define AXIS      "Axis"
#define DWELLTIME "Dwell Time"
#define MOTORS    "Motors"
#define USE3RDAX  "Use 3rd Ax"
#define ISSFLUO   "Meas. Fluo."
#define UNITS     "Units"
#define USED      "Using Flags"

#define STEPSCAN  "Step Scan"
#define QUASICONT "Quasi Continuous Scan"
#define REALCONT  "Real Continuous Scan"

#define SCANBOTH   "Both"
#define SCANSINGLE "Single"

#define YES   "Yes"
#define NO    "No"
#define NONE  "None"
#define TRUE  "T"
#define FALSE "F"

void S2DInfo::save( QTextStream &out )
{
  out << "# " << SCANMODE << " : ";
  switch( ScanMode ) {
  case STEP: out << STEPSCAN << endl; break;
  case QCONT: out << QUASICONT << endl; break;
  case RCONT: out << REALCONT << endl; break;
  }
  out << "# " << SCANDIR << " : " << ( ScanBothDir ? SCANBOTH : SCANSINGLE ) << endl;

  for ( int i = 0; i < motors; i++ ) {
    if ( used[i] ) {
      out << "# " << AXIS
	  << QString( " %1    : " ).arg( i, 1 )
	  << QString( " %1" ).arg( sx[i], 10 )
	  << QString( " %1" ).arg( ex[i], 10 )
	  << QString( " %1" ).arg( dx[i], 10 )
	  << QString( " %1" ).arg( ps[i], 10 )
	  << " : " << unit[i]->getName() << endl;
    }
  }

  out << "# " << DWELLTIME << QString( " : %1" ).arg( Dwell ) << endl;

  out << "# " << MOTORS << QString( " : %1" ).arg( motors ) << endl;
  out << "# " << USE3RDAX << QString( " : %1" ).arg( Use3rdAx ? YES : NO ) << endl;
  out << "# " << ISSFLUO << QString( " : %1" ).arg( isSFluo ? YES : NO ) << endl;
  out << "# " << UNITS << " :";
  for ( int i = 0; i < motors; i++ ) {
    if ( unit[i] != NULL ) {
      out << QString( " %1" ).arg( unit[i]->getUid() );
    } else {
      out << " None";
    }
  }
  out << endl;
  out << "# " << USED << " :";
  for ( int i = 0; i < motors; i++ ) {
    out << QString( " %1" ).arg( used[i] ? TRUE : FALSE );
  }
  out << endl;

  out << "#" << endl << endl;
}

void S2DInfo::load( QTextStream &in, QVector<AMotor*> &AMotors )
{
  while( ! in.atEnd() ) {
    QString line = in.readLine().simplified();
    if ( line.length() == 0 )
      break;
    if ( line[0] != '#' )
      break;

    int cp = line.indexOf( ':' );
    QString val = "";
    if ( cp >= 0 ) 
      val = line.mid( cp + 2 );
    QStringList vals = val.simplified().split( QRegExp( "\\s+" ) );
    
    if ( line.mid( 2, QString( SCANMODE ).length() ) == QString( SCANMODE ) ) {
      if ( val.left( QString( STEPSCAN  ).length() ) == STEPSCAN  ) ScanMode = STEP;
      if ( val.left( QString( QUASICONT ).length() ) == QUASICONT ) ScanMode = QCONT;
      if ( val.left( QString( REALCONT  ).length() ) == REALCONT  ) ScanMode = RCONT;
      qDebug() << SCANMODE << ScanMode;
      valid = true;
    }
    if ( line.mid( 2, QString( SCANDIR ).length() ) == QString( SCANDIR ) ) {
      if ( val.left( QString( SCANBOTH   ).length() ) == SCANBOTH   ) ScanBothDir = true;
      if ( val.left( QString( SCANSINGLE ).length() ) == SCANSINGLE ) ScanBothDir = false;
      //      qDebug() << SCANDIR << ScanBothDir;
      valid = true;
    }
    if ( line.mid( 2, QString( AXIS ).length() ) == QString( AXIS ) ) {
      int i = line.mid( 2 + QString( AXIS ).length() + 1, 2 ).toInt();
      if ( vals.count() >= 4 ) {
	sx[i] = vals[0].toDouble();
	ex[i] = vals[1].toDouble();
	dx[i] = vals[2].toDouble();
	ps[i] = vals[3].toInt();
	valid = true;
      }
      //      qDebug() << AXIS << i << sx[i] << ex[i] << dx[i] << ps[i];
    }
    if ( line.mid( 2, QString( DWELLTIME ).length() ) == QString( DWELLTIME ) ) {
      if ( vals.count() >= 1 ) {
	Dwell = vals[0].toDouble();
	valid = true;
      }
      //      qDebug() << DWELLTIME << Dwell;
    }
    if ( line.mid( 2, QString( MOTORS ).length() ) == QString( MOTORS ) ) {
      if ( vals.count() >= 1 ) {
	motors = vals[0].toInt();
	valid = true;
      }
      //      qDebug() << MOTORS << motors;
    }
    if ( line.mid( 2, QString( USE3RDAX ).length() ) == QString( USE3RDAX ) ) {
      if ( vals.count() >= 1 ) {
	if ( vals[0] == YES ) Use3rdAx = true;
	if ( vals[0] == NO )  Use3rdAx = false;
      }
      //      qDebug() << USE3RDAX << Use3rdAx;
    }
    if ( line.mid( 2, QString( ISSFLUO ).length() ) == QString( ISSFLUO ) ) {
      if ( vals.count() >= 1 ) {
	if ( vals[0] == YES ) isSFluo = true;
	if ( vals[0] == NO )  isSFluo = false;
      }
      //      qDebug() << ISSFLUO << isSFluo;
    }
    if ( line.mid( 2, QString( UNITS ).length() ) == QString( UNITS ) ) {
      for ( int i = 0; ( i < vals.count() ) && ( i < motors ); i++ ) {
	for ( int j = 0; j < AMotors.count(); j++ ) {
	  if ( AMotors[j]->getUid() == vals[i] ) {
	    unit[i] = AMotors[j];
	  }
	}
	//	qDebug() << UNITS << i << unit[i]->getUid();
      }
    }
    if ( line.mid( 2, QString( USED ).length() ) == QString( USED ) ) {
      for ( int i = 0; ( i < vals.count() ) && ( i < motors ); i++ ) {
	if ( vals[i] == TRUE  ) used[i] = true;
	if ( vals[i] == FALSE ) used[i] = false;
	//	qDebug() << USED << i << used[i];
      }
    }
  }
}
