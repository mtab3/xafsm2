#include "TuningTable.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QTableWidgetItem>

TuningTable::TuningTable( void ) : QObject()
{
  tuneTable = new TuneTable;
  connect( tuneTable, SIGNAL( Ok() ), this, SLOT( EditedTuneTable() ) );

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
}

void TuningTable::ClearPoints( void )
{
  while( Points.count() > 0 ) {
    Points.remove( 0 );
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

void TuningTable::ShowTuneTable( void )
{
  QTableWidgetItem *item;

  tuneTable->clearItems();

  tuneTable->setRowCol( Points.count(), 2 );
  for ( int i = 0; i < Points.count(); i++ ) {
    item = new QTableWidgetItem ( QString::number( Points[i].eng ) );
    tuneTable->setItem( i, 0, item );
    item = new QTableWidgetItem ( QString::number( Points[i].pulse ) );
    item->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
    tuneTable->setItem( i, 1, item );
  }
  tuneTable->show();
}

void TuningTable::EditedTuneTable( void )
{
  ClearPoints();
  double eng;

  for ( int i = 0; i < tuneTable->rowCount(); i++ ) {
    eng = tuneTable->getItem( i, 0 )->text().toDouble();
    if ( eng > 0 ) {
      AddAPoint0( eng, tuneTable->getItem( i, 1 )->text().toInt() );
    }
  }
  qSort( Points.begin(), Points.end() );
}
