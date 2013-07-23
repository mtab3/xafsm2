#include "TuningTable.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QDateTime>

TuningTable::TuningTable( void ) : QObject()
{
  ReadTable( "TuningTable.txt" );
}

void TuningTable::ReadTable( QString fname )
{
  QFile f( fname );

  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) )
    return;

  QTextStream in( &f );

  while( !in.atEnd() ) {
    QString line = in.readLine().simplified();
    if ( ! line.isEmpty() ) {
      if ( line[0] != QChar( '#' ) ) {
	QStringList items = line.split( QRegExp( "\\s+" ) );
	if ( items.count() >= 2 ) {
	  AddAPoint0( items[0].toDouble(), items[1].toInt() );
	}
      } else {
	Comments << line;
      }
    }

  }

  f.close();
  qSort( Points.begin(), Points.end() );
}

void TuningTable::SaveTuneTable( void )
{
  QString fname
    = QDateTime::currentDateTime().toString( "yyMMdd" )
    + "-TuningTable.txt";

  QFile f( fname );
  if ( !f.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;

  QTextStream out( &f );
  for ( int i = 0; i < Comments.count(); i++ ) {
    out << Comments[i] << "\n";
  }
  for ( int i = 0; i < Points.count(); i++ ) {
    out << Points[i].eng << "\t" << Points[i].pulse << "\n";
  }

  f.close();
}

void TuningTable::AddAPoint0( double E, int p )
{
  aPoint point;
  point.eng = E;
  point.pulse = p;
  Points << point;
}

void TuningTable::AddAPoint( double E, int p )
{
  AddAPoint0( E, p );

  qSort( Points.begin(), Points.end() );

  for ( int i = 0; i < Points.count(); i++ ) {
    qDebug() << Points[i].eng << Points[i].pulse;
  }
}

bool TuningTable::isAvailable( void )
{
  return ( Points.count() > 0 );
}

int TuningTable::p( double keV )
{
  if ( Points.count() == 0 ) {
    return 0;
  }

  if ( keV < Points[0].eng )
    return Points[0].pulse;

  for ( int i = 1; i < Points.count(); i++ ) {
    if ( keV < Points[i].eng ) {
      return (int)( ( Points[i].pulse - Points[i-1].pulse )
		    * ( keV - Points[i-1].eng )/( Points[i].eng - Points[i-1].eng )
		    + Points[i-1].pulse );
    }
  }

  return Points[ Points.count() - 1 ].pulse;
}

