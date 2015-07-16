
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include "MCAMap.h"

#define COMMONTITLE_0 "XafsM2 MCA Data"
#define COMMONTITLE   "XafsM2 MCA Ver. 1" // '...Data Ver.1' とすると _0 にも一致する

void aMCASet::save( QString fname, QString title, QString uid )
{
  QFile f( fname );
  if ( f.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
    QTextStream out( &f );
    out << QString( "# %1 %2\n" ).arg( COMMONTITLE ).arg( title );
    out << "# " << QDateTime::currentDateTime().toString( "yy/MM/dd hh:mm:ss" ) << "\n";

    writeHead( out, uid );
    writeData( out );

    f.close();
  }
}

#define SFLUOID     "Sensor ID   "
#define RINGCURRENT "Ring Current"
#define I0VALUE     "I0          "
#define MCALENGTH   "MCA Length  "
#define MCACHS      "MCA Channels"
#define MCACHINFO   "MCA Ch Info "
#define NUMOFELMS   "Sel. Elems. "
#define ELEMENT     "A Element   "

void aMCASet::writeHead( QTextStream &out, QString uid )
{
  out << "# " << SFLUOID     << " : " << uid << "\n";
  out << "# " << RINGCURRENT << " : " << RINGCurrent << "\n";
  out << "# " << I0VALUE     << " : " << I0 << "\n";
  out << "# " << MCALENGTH << " : " << Length << "\n";
  out << "# " << MCACHS    << " : " << CHs    << "\n";
  out << "## Channel Status Length RealTime LiveTime ICR ROI-Start ROI-End\n";
  for ( int i = 0; i < CHs; i++ ) {
    XMAPHead head = Heads[i];
    qDebug() << "Heads ch stat" << i << Heads[i].stat;
    out << "# " << MCACHINFO << " : "
	<< head.ch << "\t" << head.stat << "\t" << head.len << "\t"
	<< head.realTime << "\t" << head.liveTime << "\t" << head.icr << "\t"
	<< ROIStart[i] << "\t" << ROIEnd[i] << "\n";
  }
  out << "## Selected elements list\n";
  out << "# " << NUMOFELMS << " : " << Elms.count() << "\n";
  for ( int i = 0; i < Elms.count(); i++ ) {
    out << "# " << ELEMENT << " : " << i << Elms[i] << "\n";
  }
  out << "\n";
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
  valid = false;

  int hType = 0;
  if  ( !in.atEnd() ) {
    QString line = in.readLine();
    if ( fc.CheckHeadLine( COMMONTITLE_0, title, line, 2 ) ) hType = 1;   // old
    if ( fc.CheckHeadLine( COMMONTITLE,   title, line, 2 ) ) hType = 2;   // new
  }

  switch( hType ) {
  case 0: return;
  case 1: loadHeader0( in ); break;
  case 2: loadHeader( in ); break;
  default: return;
  }
  int lc = 0;

  while ( !in.atEnd() ) {
    QString line = in.readLine();
    QStringList vals = line.simplified().split( QRegExp( "\\s+" ) );
    if ( vals.count() >= ( CHs * 2 + 1 ) ) {
      int i = vals[0].toInt();
      for ( int j = 0; j < CHs; j++ ) {
        Ch[j].E[i] = vals[j * 2 + 1].toDouble();
        Ch[j].cnt[i] = vals[j * 2 + 2].toInt();
      }
    }
    lc++;
  }
  if ( lc > 0 )
    valid = true;
}

void aMCASet::loadHeader( QTextStream &in )
{
  int Elements = 0;
  int length = 0;
  int chs = 0;
  Elms.clear();
  while ( !in.atEnd() ) {
    QString line = in.readLine();
    QString val;
    if ( ( line.count() > 0 ) && ( line[0] == '#' ) ) {
      if ( fc.CheckItem( RINGCURRENT, line, 2, val ) ) RINGCurrent = val.toDouble();
      if ( fc.CheckItem( I0VALUE,     line, 2, val ) ) I0          = val.toDouble();
      if ( fc.CheckItem( MCALENGTH,   line, 2, val ) ) length      = val.toInt();
      if ( fc.CheckItem( MCACHS,      line, 2, val ) ) chs         = val.toInt();
      if ( fc.CheckItem( MCACHINFO,   line, 2, val ) ) {
	QStringList vals = val.simplified().split( QRegExp( "\\s+" ) );
	if ( vals.count() >= 7 ) {
	  int ch = vals[0].toInt();
	  Heads[ ch ].ch = ch;
	  Heads[ ch ].len = vals[1].toInt();
	  Heads[ ch ].realTime = vals[2].toDouble();
	  Heads[ ch ].liveTime = vals[3].toDouble();
	  Heads[ ch ].icr = vals[4].toDouble();
	  ROIStart[ ch ] = vals[5];
	  ROIEnd[ ch ] = vals[6];
	}
      }
      if ( fc.CheckItem( NUMOFELMS,   line, 2, val ) ) Elements    = val.toInt();
      if ( fc.CheckItem( ELEMENT,     line, 2, val ) ) {
	QStringList vals = val.simplified().split( QRegExp( "\\s+" ) );
	if ( vals.count() >= 2 ) { Elms << vals[1]; }
      }
      // length と chs が確定した時点で即 setSize を呼ぶ
      // これを呼んでおかないと、ROI とか、MCA 個別の情報を入れる配列が無い
      // or 後から呼ぶと情報をセットした配列が消される。
      if (( length > 0 )&&( chs > 0 )) {
	setSize( length, chs );
      }
    } else {
      return;
    }
  }
}

void aMCASet::loadHeader0( QTextStream &in )
{
  int maxCh = 0;
  int maxL = 0;
  while ( !in.atEnd() ) {
    QString line = in.readLine();
    QStringList vals = line.simplified().split( QRegExp( "\\s+" ) );
    if ( ( vals.count() > 0 ) && ( vals[0] == "#" ) ) {
      if ( line.mid( 2, QString( RINGCURRENT ).length() ) == QString( RINGCURRENT ) ) {
	RINGCurrent = line.mid( 2 + QString( RINGCURRENT ).length() + 3 ).toDouble();
      }
      if ( line.mid( 2, QString( I0VALUE ).length() ) == QString( I0VALUE ) ) {
	I0 = line.mid( 2 + QString( I0VALUE ).length() + 3 ).toDouble();
      }
      if ( vals.count() >= 7 ) {
	int ch = vals[1].toInt();
	int L = vals[3].toInt();
	if ( ch > maxCh ) maxCh = ch;
	if ( L > maxL ) maxL = L;
      }
    } else {
      setSize( maxL, maxCh + 1 );
      return;
    }
  }
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
