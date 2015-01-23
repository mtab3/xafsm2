
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include "MCA.h"

void aMCASet::save( QString fname, QStringList &Elms )
{
  QFile f( fname );
  if ( f.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
    QTextStream out( &f );
    out << "# XafsM2 MCA Data measured by 2D Scan\n";
    out << "# " << QDateTime::currentDateTime().toString( "yy/MM/dd hh:mm:ss" )
	<< "\n";
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
    f.close();
  }
}
