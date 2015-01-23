
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include "MCA.h"

#define COMMONTITLE "# XafsM2 MCA Data"

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

void aMCASet::writeHead( QTextStream &out )
{
  out << "# Ring Current : " << RINGCurrent << "\n";
  out << "# I0           : " << I0 << "\n";
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

//#define S2DFILEID "# XafsM2 MCA Data 

void aMCASet::load( QString fname, QString title )
{
  bool endf = false;
  int lc = 0;
  
  QFile f( fname );
  if ( f.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    QTextStream in( &f );
    while ( !endf && !in.atEnd() ) {
      QString line = in.readLine();
      if ( lc == 0 ) {
	QString Title = QString( "%1 %2" ).arg( COMMONTITLE ).arg( title );
	if ( line.left( QString( Title ).length() ) != QString( Title ) ) {
	  endf = true;
	}
      }
      lc++;
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
    
    for ( int i = 0; i < Length; i++ ) {
      out << i;
      for ( int j = 0; j < CHs; j++ ) {
	out << "\t" << Ch[ j ].E[ i ];
	out << "\t" << Ch[ j ].cnt[ i ];
      }
      out << "\n";
    }
#endif
    }
    f.close();
  }
}
