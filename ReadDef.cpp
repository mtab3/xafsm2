#include <QFile>
#include <QFileInfo>

#include "MainWindow.h"

#define LN "\n"    // only for Qt on MacOS X
#define BS "\\"     // only for Qt on MacOS X

void MainWindow::ReadDef( QString fname )
{
  QString fname0 = "XAFSM.def";
  if ( ! fname.isEmpty() ) {
    fname0 = fname;
  }

  qDebug() << tr( "File [%1] is selected as XafsM2 definition file." ).arg( fname0 );

  QFileInfo ff( fname0 );
  qDebug() << tr( "FilePath = [%1]" ).arg( ff.absoluteFilePath() );
  
  QFile f( fname0 );
  if ( !f.exists() ) {
    qDebug() << tr( "The file [%1] is not. Use default parameters." ).arg( fname0 );
    f.setFileName( fname0 = ":/XAFSM0.def" );
  }
  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    qDebug() << tr( "Error cannot open [%1]." ).arg( fname0 );
    return;
  }

  QString aline;
  QString next;
  QString item;
  QString type;

  AUnit *NewUnit;

  QTextStream in( &f );
  while( !in.atEnd() ) {
    aline = in.readLine();
    if ( !aline.isEmpty() ) {
      //      next = aline.simplified();
      next = nextItem( aline.simplified(), item );
      NewUnit = new AUnit;
      if ( ( item == "MOTOR" ) || ( item == "SENSOR" ) ) {
	if ( item == "MOTOR" )
	  AMotors << NewUnit;
	else
	  ASensors << NewUnit;
	next = nextItem( next, item ); NewUnit->setType( type = item );
	next = nextItem( next, item ); NewUnit->setID( item );
	next = nextItem( next, item ); NewUnit->setName( item );
	next = nextItem( next, item ); NewUnit->setDriver( item );
	next = nextItem( next, item ); NewUnit->setCh( item );
	next = nextItem( next, item ); NewUnit->setUnit( item );
	NewUnit->setDevCh();
	if ( type == "PM" ) {
	  next = nextItem( next, item ); NewUnit->setPPU( item );
	  next = nextItem( next, item ); NewUnit->setCenter( item );
	} else if ( type == "PZ" ) {
	  next = nextItem( next, item ); NewUnit->setMinV( item );
	  next = nextItem( next, item ); NewUnit->setMaxV( item );
	} else if ( type == "PAM" ) {
	} else if ( type == "CNT" ) {
	} else if ( type == "SSDP" ) {
	} else if ( type == "SSD" ) {
	} else {
	  qDebug() << tr( "Undefined Unit type [%1]" ).arg( item );
	}
	NewUnit->show();
      } else if ( item == "#" ) {
	qDebug() << "Comment line"; // do nothing but not an error
      } else {
	qDebug() << tr( "Undefined Key word [%1]" ).arg( item );
      }
    }
  }

  f.close();
}

QString MainWindow::nextItem( QString start, QString &item )
{
  QString rs;
  int i;

  start = start.simplified();
  if ( start.isEmpty() ) {
    item = "";
    return "";
  }

  if ( start.at(0) == '"') {
    if ( ( i = start.indexOf( '"', 1 ) ) < 0 ) {
      item = start.mid( 1 );
      rs = "";
    } else {
      item = start.mid( 1, i - 1 );
      rs = start.mid( i + 1 );
    }
  } else {
    if ( ( i = start.indexOf( " " ) ) < 0 ) {
      item = start;
      rs = "";
    } else {
      item = start.mid( 0, i );
      rs = start.mid( i + 1 );
    }
  }

  return rs;
}
