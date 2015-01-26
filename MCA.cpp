
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include "MCA.h"

#define COMMONTITLE "XafsM2 MCA Data"

void aMCASet::save( QString fname, QString title )
{
  QFile f( fname );
  if ( f.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
    QTextStream out( &f );
    out << QString( "# %1 %2\n" ).arg( COMMONTITLE ).arg( title );
    out << "# " << QDateTime::currentDateTime().toString( "yy/MM/dd hh:mm:ss" ) << "\n";
    
    writeHead( out );
    writeData( out );

    f.close();
  }
}

#define RINGCURRENT "Ring Current"
#define I0VALUE     "I0          "

void aMCASet::writeHead( QTextStream &out )
{
  out << "# " << RINGCURRENT << " : " << RINGCurrent << "\n";
  out << "# " << I0VALUE     << " : " << I0 << "\n";
  out << "# Channel Status Length RealTime LiveTime ICR ROI-Start ROI-End\n";
  for ( int i = 0; i < CHs; i++ ) {
    MCAHead head = Heads[i];
    out << "# " << head.ch << "\t" << head.stat << "\t" << head.len << "\t"
	<< head.realTime << "\t" << head.liveTime << "\t" << head.icr << "\t"
	<< ROIStart[i] << "\t" << ROIEnd[i] << "\n";
  }
  out << "# Selected elements list\n";
  for ( int i = 0; i < Elms.count(); i++ ) {
    out << QString( "# Element %1 %2\n" ).arg( i ).arg( Elms[i] );
  }
}

void aMCASet::writeData( QTextStream &out )
{
  for ( int i = 0; i < Length; i++ ) {
    out << i;
    for ( int j = 0; j < CHs; j++ ) {
      out << "\t" << Ch[j].E[i];
      out << "\t" << Ch[j].cnt[i];
    }
    out << "\n";
  }
}

void aMCASet::load( QString fname, QString title )
{
  bool endf = false;
  int lc = 0;
  qDebug() << "Fname " << fname;
  QFile f( fname );
  if ( f.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    QTextStream in( &f );
    while ( !endf && !in.atEnd() ) {
      QString line = in.readLine();
      if ( lc == 0 ) {
	QString Title = QString( "%1 %2" ).arg( COMMONTITLE ).arg( title );
	if ( line.mid( 2, QString( Title ).length() ) != QString( Title ) ) {
	  endf = true;
	}
      }
      if ( lc > 1 ) {
	if ( ( line.count() > 0 ) && ( line[0] == '#' ) ) {
	  if ( line.mid( 2, QString( RINGCURRENT ).length() ) == QString( RINGCURRENT ) ) {
	    RINGCurrent = line.mid( 2 + QString( RINGCURRENT ).length() + 3 ).toDouble();
	  }
	  if ( line.mid( 2, QString( I0VALUE ).length() ) == QString( I0VALUE ) ) {
	    RINGCurrent = line.mid( 2 + QString( I0VALUE ).length() + 3 ).toDouble();
	  }
	} else {
	  QStringList vals = line.simplified().split( QRegExp( "\\s+" ) );
	  if ( vals.count() >= ( CHs * 2 + 1 ) ) {
	    int i = vals[0].toInt();
	    for ( int j = 0; j < CHs; j++ ) {
	      Ch[j].E[i] = vals[j * 2 + 1].toDouble();
	      Ch[j].cnt[i] = vals[j * 2 + 2].toInt();
	    }
	  }
	}
      }
      lc++;
    }
    if ( lc > 1 ) valid = true;
    f.close();
  }
}


#if 0
    out << "# Ring Current : " << RINGCurrent << "\n";
    out << "# I0           : " << I0 << "\n";
    out << "# Channel Status Length RealTime LiveTime ICR\n";
    for ( int i = 0; i < Length; i++ ) {
      MCAHead head = Heads[i];
      out << "# " << head.ch << "\t" << head.stat << "\t" << head.len << "\t"
	  << head.realTime << "\t" << head.liveTime << "\t" << head.icr << "\n";
    }
    
    out << "# Selected elements list\n";
    for ( int i = 0; i < Elms.count(); i++ ) {
      out << QString( "# Element %1 %2\n" ).arg( i ).arg( Elms[i] );
    }
#endif


void aMCAMap::New( int ix, int iy, int length, int CHs )
{
  if ( Points != NULL ) {
    delete [] Points;
  }
  iX = ix;
  iY = iy;
  try {
    Points = new aMCASet[ iX * iY ];
  }
  catch(...) {
    Points = NULL;
  }
  if ( Points != NULL ) {
    for ( int i = 0; i < iX * iY; i++ ) {
      Points[i].setSize( length, CHs );
    }
  }
}

aMCASet *aMCAMap::aPoint( int ix, int iy )
{
  if ( Points == NULL )
    return NULL;
  if (( ix < iX )&&( iy < iY ))
    return &(Points[ iy * iX + ix ]);
  return NULL;
}

aMCASet *aMCAMap::lastP( void )
{
  aMCASet *rv = NULL;
  for ( int i = iX * iY - 1; i >= 0; i++ ) {
    if ( Points[i].isValid() ) {
      qDebug() << "Selected set " << i << iX << iY << iX * iY;
      rv = &(Points[i]);
      break;
    }
  }
  return rv;
}
