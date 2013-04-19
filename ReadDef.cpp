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

  int line = 0;
  QString aline;
  QString next;
  QString item;
  bool isMotor;
  QString type;

  AUnit *NewUnit;

  QTextStream in( &f );
  while( !in.atEnd() ) {
    aline = in.readLine();
    line++;
    if ( !aline.isEmpty() && ( aline.at(0) != QChar( '#' ) ) ) {
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
	NewUnit->setALine( line );
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
	  if (( type == "PM" )||( type == "SC" )) {
	    next = nextItem( next, item ); NewUnit->setCenter( item );
	  } else if ( type == "PZ" ) {
	    next = nextItem( next, item ); NewUnit->setMinV( item );
	    next = nextItem( next, item ); NewUnit->setMaxV( item );
	  } else {
	    qDebug() << tr( "::Undefined Unit type [%1]" ).arg( type );
	  }
	} else {  // 以下、sensor だけの項目
	  // 全 sensor 共通
	  next = nextItem( next, item ); NewUnit->setHasParent( item == "YES" );
	  next = nextItem( next, item ); NewUnit->setParent( item );
	  // 各 sensor 個別
	  if ( type == "ENC" ) {
	  } else if ( type == "ENC2" ) {
	    next = nextItem( next, item );
	    NewUnit->setUPP( item );
	  } else if ( type == "PAM" ) {
	  } else if ( type == "CNT" ) {
	  } else if ( type == "OTC" ) {
	  } else if (( type == "CNT2" )||( type == "OTC2" )) {
	    next = nextItem( next, item );
	    NewUnit->set2ndUid( item );             // 2nd ドライバの設定
	    NewUnit->setHas2ndDriver( true );       // 2nd ドライバフラグ
	    NewUnit->setRangeSelectable( true );    // レンジ設定可能のフラグ
	    next = nextItem( next, item );
	    NewUnit->setRangeU( item.toInt() );     // レンジ上限値
	    next = nextItem( next, item );
	    NewUnit->setRangeL( item.toInt() );     // レンジ下限値
	  } else if ( type == "SSDP" ) {
	  } else if ( type == "SSD" ) {
	  } else if ( type == "LSR" ) {
	  } else if ( type == "DV" ) {
	  } else if ( type == "DV2" ) {
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
      } else if ( item == "AGROUP" ) {
	AtomGroup *ag = new AtomGroup;
	next = nextItem( next, item ); ag->groupId = item.toInt();
	next = nextItem( next, item ); ag->startAtom = item;
	next = nextItem( next, item ); ag->endAtom = item;
	next = nextItem( next, item ); ag->groupColor = item;
	AtomGroups << ag;
      } else if ( item == "GAS" ) {
	Gas *gas = new Gas;
	next = nextItem( next, item ); gas->Name = item;
	next = nextItem( next, item );
	while ( item != "" ) {
	  GasComp *gascomp = new GasComp;
	  gascomp->GasForm = item; next = nextItem( next, item ); 
	  gascomp->comp = item.toDouble(); next = nextItem( next, item ); 
	  gas->GasComps << gascomp;
	}
	Gases << gas;
      } else if ( item == "ICLENGTH" ) {
	IonChLength *iclen = new IonChLength;
	next = nextItem( next, item ); iclen->Name = item;
	next = nextItem( next, item ); iclen->ID = item;
	next = nextItem( next, item ); iclen->length = item.toDouble();
	next = nextItem( next, item ); iclen->UName = item;
	ICLengths << iclen;
      } else if ( item == "ENERGYRANGE" ) {
	next = nextItem( next, item ); MinEnergyInEV = item.toDouble();
	next = nextItem( next, item ); MaxEnergyInEV = item.toDouble();
      } else if ( item == "DEFAULTUNIT" ) {
	next = nextItem( next, item ); DefaultUnit = item.toInt();
      } else if ( item == "QXAFSMODE" ) {
	next = nextItem( next, item ); isQXafsModeAvailable = item.toInt();
	next = nextItem( next, item ); OrigHSpeed = item.toInt();
	next = nextItem( next, item ); MaxHSpeed = item.toInt();
	next = nextItem( next, item ); LowSpeed = item.toInt();
	next = nextItem( next, item ); RunUpRate = item.toDouble();
      } else if ( item == "QXAFSOK" ) {
	QXafsOk = next.split( QRegExp( "\\s+" ) );
      } else if ( item == "NORMALOK" ) {
	NXafsOk = next.split( QRegExp( "\\s+" ) );
      } else {
	qDebug() << tr( "Undefined Key word [%1]" ).arg( item );
      }
    }
  }

  DriverList.removeDuplicates();
  f.close();

  // Uid のダブりチェック
  CheckDuplicateUID();

  int i, j;   // 親ユニット有り、と宣言したセンサーに親ユニットのポインタを渡す。
              // また、2nd Driver があるユニット Uid2 を元に Driver2, Ch2, DevCh2 を設定
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
    if ( ASensors.at(i)->getType() == "CNT2" ) {
      for ( j = 0; j < ASensors.count(); j++ ) {
	if ( ASensors.at(i)->get2ndUid() == ASensors.at(j)->getUid() ) {
	  ASensors.at(i)->set2ndDriver( ASensors.at(j)->getDriver() );
	  ASensors.at(i)->set2ndCh( ASensors.at(j)->getCh() );
	  ASensors.at(i)->set2ndDevCh();
	  break;
	}
      }
      if ( j >= ASensors.count() ) {
	qDebug() << "can not find a 2nd Driver for " << ASensors.at(i)->getUid()
		 << "the name is " << ASensors.at(i)->get2ndUid();
      }
    }
  }
}

// Uid の重複チェック
void MainWindow::CheckDuplicateUID( void )
{
  for ( int i = 0; i < AMotors.count(); i++ ) {
    for ( int j = 0; j < AMotors.count(); j++ ) {
      if ( i != j ) {
	if ( AMotors.at(i)->getUid() == AMotors.at(j)->getUid() ) {
	  // Uid がダブってると致命的なので止まらないとしかたない
	  ExitByDuplicateUID( AMotors.at(i), AMotors.at(j) );
	}
      }
    }
    for ( int j = 0; j < ASensors.count(); j++ ) {
      if ( AMotors.at(i)->getUid() == ASensors.at(j)->getUid() ) {
	ExitByDuplicateUID( AMotors.at(i), ASensors.at(j) );
      }
    }
  }
  for ( int i = 0; i < ASensors.count(); i++ ) {
    for ( int j = 0; j < ASensors.count(); j++ ) {
      if ( i != j ) {
	if ( ASensors.at(i)->getUid() == ASensors.at(j)->getUid() ) {
	  ExitByDuplicateUID( ASensors.at(i), ASensors.at(j) );
	}
      }
    }
    for ( int j = 0; j < AMotors.count(); j++ ) {
      if ( ASensors.at(i)->getUid() == AMotors.at(j)->getUid() ) {
	ExitByDuplicateUID( ASensors.at(i), AMotors.at(j) );
      }
    }
  }
}

void MainWindow::ExitByDuplicateUID( AUnit *a1, AUnit *a2 )
{
  qDebug() << tr( "UIDs [%1](at line %2) and [%3](at line %4) are duplicated." )
    .arg( a1->getUid() ).arg( a1->getALine() )
    .arg( a2->getUid() ).arg( a2->getALine() );
  qDebug() << tr( "1st one is : Type[%1] Identifier[%2] Driver[%3] Node[%4]" )
    .arg( a1->getType() ).arg( a1->getID() ).arg( a1->getDriver() ).arg( a1->getCh() );
  qDebug() << tr( "2nd one is : Type[%1] Identifier[%2] Driver[%3] Node[%4]" )
    .arg( a2->getType() ).arg( a2->getID() ).arg( a2->getDriver() ).arg( a2->getCh() );
  exit( 0 );
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
