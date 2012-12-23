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
  bool isMotor;
  QString type;

  AUnit *NewUnit;

  QTextStream in( &f );
  while( !in.atEnd() ) {
    aline = in.readLine();
    if ( !aline.isEmpty() && aline.at(0) != QChar( '#' ) ) {
      //      next = aline.simplified();
      next = nextItem( aline.simplified(), item );
      NewUnit = new AUnit;
      if ( ( item == "MOTOR" ) || ( item == "SENSOR" ) ) {
	if ( item == "MOTOR" ) { // Motor か
	  AMotors << NewUnit;
	  NewUnit->setGType( "MOTOR" );
	  isMotor = true;
	} else {
	  ASensors << NewUnit;  // Sensor か
	  NewUnit->setGType( "SENSOR" );
	  isMotor = false;
	}
	// 全 motor, sensor に共通の項目
	next = nextItem( next, item ); NewUnit->setType( type = item );
	next = nextItem( next, item ); NewUnit->setUid( item );
	next = nextItem( next, item ); NewUnit->setID( item );
	next = nextItem( next, item ); NewUnit->setName( item );
	next = nextItem( next, item ); NewUnit->setDriver( item ); DriverList << item;
	next = nextItem( next, item ); NewUnit->setCh( item );
	next = nextItem( next, item ); NewUnit->setUnit( item );
	NewUnit->setDevCh();
	// 以下、motor だけの項目
	if ( isMotor ) {
	  // 全 motor 共通
	  next = nextItem( next, item ); NewUnit->setUPP( item );
	  next = nextItem( next, item ); NewUnit->setIsInt( item == "INT" );
	  // 各 motor 個別
	  if ( type == "PM" ) {
	    next = nextItem( next, item ); NewUnit->setCenter( item );
	  } else if ( type == "PZ" ) {
	    next = nextItem( next, item ); NewUnit->setMinV( item );
	    next = nextItem( next, item ); NewUnit->setMaxV( item );
	  } else {
	    qDebug() << tr( "::Undefined Unit type [%1]" ).arg( type );
	  }
	} else {  // 以下、各 sensor だけの項目
	  // 全 sensor 共通
	  next = nextItem( next, item ); NewUnit->setHasParent( item == "YES" );
	  next = nextItem( next, item ); NewUnit->setParent( item );
	  // 各 sensor 個別
	  if ( type == "ENC" ) {
	  } else if ( type == "PAM" ) {
	  } else if ( type == "CNT" ) {
	  } else if ( type == "SSDP" ) {
	  } else if ( type == "SSD" ) {
	  } else {
	    qDebug() << tr( "::Undefined Unit type [%1]" ).arg( type );
	  }
	}
	//	NewUnit->show();
      } else if ( item == "#" ) {
	// qDebug() << "Comment line"; // do nothing but not an error
      } else if ( item == "XAFSName" ) {
        next = nextItem( next, item );
        XAFSName = item;
      } else if ( item == "XAFSKey" ) {
        next = nextItem( next, item );
        XAFSKey = item;
      } else if ( item == "XAFSTitle" ) {
	next = nextItem( next, item );
	XAFSTitle = item;
      } else if ( item == "CRYSTAL" ) {
	QString CName;
	QString LatticeC;
	next = nextItem( next, item ); CName = item;
	next = nextItem( next, item ); LatticeC = item;
	mccd << new MCCD( CName, LatticeC );
      } else {
	qDebug() << tr( "Undefined Key word [%1]" ).arg( item );
      }
    }
  }

  DriverList.removeDuplicates();
  for ( int i = 0; i < DriverList.count(); i++ ) {
    qDebug() << "Driver " << DriverList.at(i);
  }

  f.close();

  int i, j;   // 親ユニット有り、と宣言したセンサーに親ユニットのポインタを渡す。
              // 全部の定義が終わってからやっているのは、親と宣言したユニットの定義が
              // 後から出てきても大丈夫にするため。
  for ( i = 0; i < ASensors.count(); i++ ) {
    if ( ASensors.at(i)->hasParent() ) {
      for ( j = 0; j < ASensors.count(); j++ ) {
	if ( ASensors.at(i)->getPUid() == ASensors.at(j)->getUid() ) {
	  ASensors.at(i)->setTheParent( ASensors.at(j) );
	  break;
	}
      }
      if ( j >= ASensors.count() ) {
	qDebug() << "can not find a parent for " << ASensors.at(i)->getUid()
		 << "the name is " << ASensors.at(i)->getPUid();
      }
    }
  }
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
