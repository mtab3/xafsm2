
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
    XMAPHead head = Heads[i];
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
  QFile f( fname );
  if ( f.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    QTextStream in( &f );
    load( in, title );
    f.close();
  }
}

void aMCASet::load( QTextStream &in, QString title )
{
  bool endf = false;
  int lc = 0;

  while ( !endf && !in.atEnd() ) {
    QString line = in.readLine();
    if ( lc == 0 ) {
      QString Title = QString( "%1 %2" ).arg( COMMONTITLE ).arg( title );
      Title = Title.simplified();
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
}

void aMCASet::correctE( KeV2Pix *k2p )
{
  // 測定時のエネルギーとピクセルの関係は、
  // 今のエネルギーとピクセルの関係とは違っている可能性があるので
  // 今のピクセルに対応するエネルギーでのカウント数を線形補完で求めておく
  aCH newCh;
  newCh.setLength( Length );
  for ( int ch = 0; ch < CHs; ch++ ) {
    for ( int i = 0; i < Length; i++ ) {
      double nowE = k2p->p2E( ch, i );
      newCh.E[i] = nowE;
      int j;
      for ( j = 1; j < Length - 1; j++ ) {
	if ( nowE < Ch[ch].E[j] ) {
	  break;
	}
      }
      if ( ( Ch[ch].E[j] != Ch[ch].E[j-1] ) && ( j < ( Length - 1 ) ) ) {
	int cnt = (int)(( nowE - Ch[ch].E[j-1] ) / ( Ch[ch].E[j] - Ch[ch].E[j-1] )
			* ( (int)Ch[ch].cnt[j] - (int)Ch[ch].cnt[j-1] )
			+ Ch[ch].cnt[j-1] );
	if ( cnt < 0 ) cnt = 0;
	newCh.cnt[i] = cnt;
      } else {
	newCh.cnt[i] = 0;
      }
    }
    for ( int i = 0; i < Length; i++ ) {
      Ch[ch].E[i] = newCh.E[i];
      Ch[ch].cnt[i] = newCh.cnt[i];
    }
  }
}

void aMCASet::copyCnt( int ch, quint32 *cnt )
{
  for ( int i = 0; i < Length; i++ ) {
    cnt[i] = Ch[ch].cnt[i];
  }
}


void aMCAMap::New( int ix, int iy, int length, int CHs )
{
  iX = ix;
  iY = iy;
  try {
    MCASets.resize( 0 );
    MCASets.resize( iX * iY );
  }
  catch(...) {
    MCASets.resize( 0 );
  }
  for ( int i = 0; i < MCASets.count(); i++ ) {
    MCASets[i].setSize( length, CHs );
  }
}

aMCASet *aMCAMap::aPoint( int ix, int iy )
{
  int i = iy * iX + ix;
  
  if ( ( i >= 0 )&&( i < MCASets.count() ) )
    return &(MCASets[ i ]);
  return NULL;
}

bool aMCAMap::valid( int ix, int iy )
{
  aMCASet *p = aPoint( ix, iy );
  if ( p == NULL )
    return false;
  return p->isValid();
}

aMCASet *aMCAMap::lastP( void )
{
  aMCASet *rv = NULL;
  
  for ( int i = MCASets.count() - 1; i >= 0; i-- ) {
    if ( MCASets[i].isValid() ) {
      rv = &(MCASets[i]);
      break;
    }
  }
  return rv;
}
