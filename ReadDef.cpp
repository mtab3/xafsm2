#include <QFile>
#include <QFileInfo>

#include "AUnits/AUnits.h"
#include "MainWindow.h"

#define LN "\n"    // only for Qt on MacOS X
#define BS "\\"     // only for Qt on MacOS X

void MainWindow::ReadDef( QString fname )
{
  QString fname0 = "XAFSM.def";
  if ( ! fname.isEmpty() ) {
    fname0 = fname;
  }
  qDebug() << QString( "File [%1] is selected as XafsM2 definition file." ).arg( fname0 );

  QFileInfo ff( fname0 );
  qDebug() << QString( "FilePath = [%1]" ).arg( ff.absoluteFilePath() );
  
  QFile f( fname0 );
  if ( !f.exists() ) {
    qDebug() << tr( "The file [%1] is not. Use default parameters." ).arg( fname0 );
    f.setFileName( fname0 = ":/XAFSM0.def" );
  }
  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    qDebug() << tr( "Error cannot open [%1]." ).arg( fname0 );
    return;
  }

  int i, j;
  int line = 0;
  QString aline;
  QString next;
  QString item;
  bool isMotor;
  QString gType;
  QString type;

  AUnit0 *NewUnit;
  ASensor *NewSensor;
  AMotor *NewMotor;

  QTextStream in( &f );
  while( !in.atEnd() ) {
    aline = in.readLine();
    line++;
    if ( !aline.isEmpty() && ( aline.at(0) != QChar( '#' ) ) ) {
//    next = nextItem( aline.simplified(), item );
      next = nextItem( aline, item );            // stop using 'simplified'
      gType = item;
      if ( ( gType == "MOTOR" ) || ( gType == "SENSOR" ) ) {
        next = nextItem( next, item ); type = item;
	NewUnit = NewNewUnit( type );

        if ( gType == "MOTOR" ) { // Motor か
	  NewMotor = (AMotor*)NewUnit;
          AMotors << NewMotor;
          NewUnit->setGType( "MOTOR" );
          isMotor = true;
        } else {
	  NewSensor = (ASensor*)NewUnit;
          ASensors << NewSensor;
          NewUnit->setGType( "SENSOR" );
          isMotor = false;
        }
        // 全 motor, sensor に共通の項目
	NewUnit->setType( type );
        NewUnit->setALine( line );
        next = nextItem( next, item ); NewUnit->setUid( item );
        next = nextItem( next, item ); NewUnit->setID( item );
        next = nextItem( next, item ); NewUnit->setName( item );
        next = nextItem( next, item ); NewUnit->setDev( item ); DriverList << item;
        next = nextItem( next, item ); NewUnit->setCh( item );
        next = nextItem( next, item ); NewUnit->setUnit( item );
	// NewUnit->setDevCh();
        // 名前の中の特殊文字の置換
        NewUnit->setName( LocalizedName( NewUnit->name() ) );

        // 以下、motor だけの項目
        if ( isMotor ) {
          // 全 motor 共通
          next = nextItem( next, item ); NewMotor->setUPP( item );
          next = nextItem( next, item ); NewMotor->setIsInt( item == "INT" );
          if (( type == "PM" )||( type == "SC" )) {
	    next = nextItem( next, item ); NewMotor->setCenter( item );
	  } else if ( type == "PZ" ) {
	    next = nextItem( next, item ); ((AUnitPZ*)NewMotor)->setMinV( item );
	    next = nextItem( next, item ); ((AUnitPZ*)NewMotor)->setMaxV( item );
	  } else if ( type == "AIOo" ) {
	    next = nextItem( next, item ); ((AUnitAIOo*)NewMotor)->setMinV( item );
	    next = nextItem( next, item ); ((AUnitAIOo*)NewMotor)->setMaxV( item );
	  } else {
            qDebug() << tr( "A::Undefined Unit type [%1]" ).arg( type );
          }
        } else {  // 以下、sensor だけの項目
          // 全 sensor 共通
          next = nextItem( next, item ); NewSensor->setHasParent( item == "YES" );
          next = nextItem( next, item ); NewSensor->setPUid( item );
          // 各 sensor 個別
          if ( type == "ENC" ) {
          } else if ( type == "ENC2" ) {
            next = nextItem( next, item );
            NewUnit->setUPP( item );
          } else if ( type == "PAM" ) {
          } else if ( type == "PAM2" ) {
          } else if ( type == "CNT" ) {
          } else if ( type == "OTC" ) {
          } else if (( type == "CNT2" )||( type == "OTC2" )) {
            next = nextItem( next, item );
            NewUnit->set2ndUid( item );             // 2nd ドライバの設定
            NewUnit->setHas2ndDev( true );          // 2nd ドライバフラグ
            NewSensor->setRangeSelectable( true );    // レンジ設定可能のフラグ
            next = nextItem( next, item );
            NewSensor->setRangeU( item.toInt() );     // レンジ上限値
            next = nextItem( next, item );
            NewSensor->setRangeL( item.toInt() );     // レンジ下限値
          } else if ( type == "SSDP" ) {
          } else if ( type == "SSD" ) {
          } else if ( type == "LSR" ) {
          } else if (( type == "DV" )||( type == "DV3" )) {
            next = nextItem( next, item );
            ((AUnitDV*)NewUnit)->setMaxIntTime( item.toDouble() );
          } else if ( type == "DV2" ) {
          } else if ( type == "CCG" ) {
          } else if ( type == "AIOi" ) {
          } else if ( type == "FP23" ) {
	  } else if ( type == "EPIC" ) {
          } else {
            qDebug() << tr( "B::Undefined Unit type [%1]" ).arg( type );
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
        QXafsOk += next.split( QRegExp( "\\s+" ) );
      } else if ( item == "NORMALOK" ) {
        NXafsOk += next.split( QRegExp( "\\s+" ) );
      } else if ( item == "CONTOK" ) {
        CScanOk += next.split( QRegExp( "\\s+" ) );
      } else if ( item == "MCAGAIN" ) {
        MCAGain *mcaGain = new MCAGain;
        next = nextItem( next, item ); mcaGain->ch = item.toInt();
        next = nextItem( next, item ); mcaGain->gain = item.toDouble();
        MCAGains << mcaGain;
      } else if ( item == "CHANGER" ) {
        Changer *newChanger = new Changer;
        next = nextItem( next, item ); newChanger->setID( item );
        next = nextItem( next, item ); newChanger->setName( item );
        next = nextItem( next, item ); newChanger->setUnitId1( item );
        next = nextItem( next, item ); newChanger->setUnitId2( item );
        next = nextItem( next, item ); newChanger->setHolders1( item.toInt() );
        next = nextItem( next, item ); newChanger->setHolders2( item.toInt() );
        next = nextItem( next, item ); newChanger->setCenter1( item.toDouble() );
        next = nextItem( next, item ); newChanger->setCenter2( item.toDouble() );
        next = nextItem( next, item ); newChanger->setSpacing1( item.toDouble() );
        next = nextItem( next, item ); newChanger->setSpacing2( item.toDouble() );
        next = nextItem( next, item ); newChanger->setDir1( item.toInt() );
        next = nextItem( next, item ); newChanger->setDir2( item.toInt() );
        next = nextItem( next, item ); newChanger->setWidth1( item.toInt() );
        next = nextItem( next, item ); newChanger->setWidth2( item.toInt() );
        newChanger->setName( LocalizedName( newChanger->name() ) );
        Changers << newChanger;
      } else if ( item == "SPEC_CHG" ) {
        SpecChanger *newSChanger = new SpecChanger;
        next = nextItem( next, item ); newSChanger->setID( item );
        next = nextItem( next, item ); newSChanger->setName( LocalizedName( item ) );
        next = nextItem( next, item ); newSChanger->setBaseChangerID( item );
        for ( i = 0; i < Changers.count(); i++ ) {
          if ( Changers[i]->id() == item )
            break;
        }
        if ( i < Changers.count() ) {
          newSChanger->setBaseChangerP( Changers[i] );
          SChangers << newSChanger;
        } else {
          qDebug()
              << QString( "There is no chager [%1] for the base of the changer [%2]" )
                 .arg( item ).arg( newSChanger->id() );
        }
      } else if ( item == "POS_NAME" ) {
        next = nextItem( next, item );
        for ( i = 0; i < SChangers.count(); i++ ) {
          if ( SChangers[i]->id() == item )
            break;
        }
        if ( i < SChangers.count() ) {
          SpecName *newSName = new SpecName;
          next = nextItem( next, item ); newSName->setPosition( item.toInt() );
          next = nextItem( next, item ); newSName->setName( LocalizedName( item ) );
          while ( next != "" ) {
            next = nextItem( next, item ); newSName->addAttrib( LocalizedName( item ) );
          }
          SChangers[i]->addSpecName( newSName );
        } else {
          qDebug()
              << QString( "There is no special chager [%1]"
                          " for the attributions [%2]" )
                 .arg( item ).arg( next );
        }
      } else if ( item == "CALIB_ENGS" ) {
        while ( next != "" ) {
          next = nextItem( next, item ); SSDCalibEnergys << item;
        }
      } else if ( item == "RW_DXMCENTER_CFG" ) {
        next = nextItem( next, item ); RWDXMCenterF = ( item.toInt() == 1 );
        next = nextItem( next, item ); DXMCenterFile = item;
      } else if ( item == "SPEEDS" ) {
        next = nextItem( next, item );
        for ( i = 0; i < AMotors.count(); i++ ) {
          if ( AMotors[i]->uid() == item )
            break;
        }
        if ( i < AMotors.count() ) {
          AMotors[i]->setHasSpeedsLine( true );
          next = nextItem( next, item ); AMotors[i]->setHighSpeed( item.toInt() );
          next = nextItem( next, item ); AMotors[i]->setHighestSpeed( item.toInt() );
          next = nextItem( next, item ); AMotors[i]->setAccRate( item.toInt() );
          next = nextItem( next, item ); AMotors[i]->setAccRateNo( item.toInt() );
        } else {
          qDebug() << tr( "Can not find a unit ID %1 for the SPEEDS line" ).
                      arg( item );
        }
      } else if ( item == "DEF_U_REAL" ) {   // use real unit as default unit not pulse
        next = nextItem( next, item );
        while( item != "" ) {
          if ( item[0] == '#' )
            break;
          DefUReals << item;
          next = nextItem( next, item );
        }
      } else if ( item == "DATAROOT" ) {    // define data-root
        next = nextItem( next, item );
        if ( item != "" ) {
          conds->setDataRoot( item );
        }
      } else if ( item == "MCA_CAN_SAVE_ALL" ) {
        // define if the MCA data can save on memory or not.
        next = nextItem( next, item );
        if ( item != "" ) {
          MCACanSaveAllOnMem = ( item.toInt() != 0 );
        }
      } else if ( item == "MAX_MCA_ENERGY" ) {  // define the maximum MCA display energy
        next = nextItem( next, item );
        if ( item != "" ) {
          MaxMCAEnergy = item.toDouble();
        }
      } else if ( item == "MSTAB" ) {  // define driver for monochromator controller via PEZ
        next = nextItem( next, item );
        MStabOk = true;
        MStabDrv = item;
        next = nextItem( next, item );
        MStabDelegate = item;
      } else if ( item == "SAVE_EACH_MCA_SPECTRUM" ) {
	next = nextItem( next, item );
	if ( item != "" ) {
	  conds->RecordEachMCASpectrumEnabled( item.toInt() == 1 );
	}
      } else if ( item == "ALARM" ) {
	QString uid, aon, aoff, msg;
	next = nextItem( next, item ); uid = item;
	next = nextItem( next, item ); aon = item;
	next = nextItem( next, item ); aoff = item;
	next = nextItem( next, item ); msg = item;
	alarms->addAlarm( uid, aon, aoff, msg );
      } else {
        qDebug() << tr( "Undefined Key word [%1]" ).arg( item );
      }
    }
  }

  //  qDebug() << DefUReals;

  DriverList.removeDuplicates();
  f.close();

  // Uid のダブりチェック
  CheckDuplicateUID();

  // 親ユニット有り、と宣言したセンサーに親ユニットのポインタを渡す。
  // また、2nd Driver があるユニット Uid2 を元に Driver2, Ch2, DevCh2 を設定
  // 全部の定義が終わってからやっているのは、親と宣言したユニットの定義が
  // 後から出てきても大丈夫にするため。
  for ( i = 0; i < ASensors.count(); i++ ) {
    if ( ASensors.at(i)->hasParent() ) {
      for ( j = 0; j < ASensors.count(); j++ ) {
        if ( ASensors[i]->pUid() == ASensors[j]->uid() ) {
          ASensors[i]->setTheParent( ASensors[j] );
          break;
        }
      }
      if ( j >= ASensors.count() ) {
        qDebug() << "can not find a parent for " << ASensors.at(i)->uid()
                 << "the name is " << ASensors[i]->pUid();
      }
    }
    if (( ASensors[i]->type() == "CNT2" )
        ||( ASensors[i]->type() == "OTC2" )) {
      for ( j = 0; j < ASensors.count(); j++ ) {
        if ( ASensors[i]->uid2() == ASensors[j]->uid() ) {
          ASensors[i]->setThe2ndDev( ASensors[j] );
          ASensors[i]->setDev2( ASensors[j]->dev() );
          ASensors[i]->setCh2( ASensors[j]->ch() );
	  //          ASensors[i]->set2ndDevCh();
          break;
        }
      }
      if ( j >= ASensors.count() ) {
        qDebug() << "can not find a 2nd Driver for " << ASensors.at(i)->uid()
                 << "the name is " << ASensors.at(i)->uid2();
      }
    }
  }

#if 0
  for ( int i = 0; i < ASensors.count(); i++ ) {
    qDebug() << i << ASensors[i]->uid() << ASensors[i]->name();
  }
#endif
  
}

AUnit0 *MainWindow::NewNewUnit( QString type )
{
  AUnit0 *rv = NULL;

  if ( type == "PM" )
    rv = (AUnit0*)new AUnitPM;
  if ( type == "PZ" )
    rv = (AUnit0*)new AUnitPZ;
  if ( type == "SC" )
    rv = (AUnit0*)new AUnitSC;
  if ( type == "AIOo" )
    rv = (AUnit0*)new AUnitAIOo;

  if ( type == "CNT" )
    rv = (AUnit0*)new AUnitCNT;
  if ( type == "PAM" )
    rv = (AUnit0*)new AUnitPAM;
  if ( type == "ENC" )
    rv = (AUnit0*)new AUnitENC;
  if ( type == "SSD" )
    rv = (AUnit0*)new AUnitXMAP;
  if ( type == "SSDP" )
    rv = (AUnit0*)new AUnitXMAP2;
  if ( type == "CNT2" )
    rv = (AUnit0*)new AUnitOTC;
  if ( type == "OTC" )
    rv = (AUnit0*)new AUnitOTC;
  if ( type == "OTC2" )
    rv = (AUnit0*)new AUnitOTC2;
  if ( type == "LSR" )
    rv = (AUnit0*)new AUnitLSR;
  if ( type == "DV" )
    rv = (AUnit0*)new AUnitDV;
  if ( type == "DV2" )
    rv = (AUnit0*)new AUnitDV2;
  if ( type == "DV3" )
    rv = (AUnit0*)new AUnitDV3;
  if ( type == "ENC2" )
    rv = (AUnit0*)new AUnitENC2;
  if ( type == "PAM2" )
    rv = (AUnit0*)new AUnitPAM2;
  if ( type == "CCG" )
    rv = (AUnit0*)new AUnitCCG;
  if ( type == "AIOi" )
    rv = (AUnit0*)new AUnitAIOi;
  if ( type == "FP23" )
    rv = (AUnit0*)new AUnitFP23;
  if ( type == "EPIC" )
    rv = (AUnit0*)new AUnitEPIC;

  if ( rv == NULL ) {
    rv = new AUnit0;
    qDebug() << QString( "Error Unknown type name [%1]" ).arg( type );
  }

  return rv;
}


// Uid の重複チェック
void MainWindow::CheckDuplicateUID( void )
{
  for ( int i = 0; i < AMotors.count(); i++ ) {
    for ( int j = 0; j < AMotors.count(); j++ ) {
      if ( i != j ) {
        if ( AMotors.at(i)->uid() == AMotors.at(j)->uid() ) {
          // Uid がダブってると致命的なので止まらないとしかたない
          ExitByDuplicateUID( AMotors.at(i), AMotors.at(j) );
        }
      }
    }
    for ( int j = 0; j < ASensors.count(); j++ ) {
      if ( AMotors.at(i)->uid() == ASensors.at(j)->uid() ) {
        ExitByDuplicateUID( AMotors.at(i), ASensors.at(j) );
      }
    }
  }
  for ( int i = 0; i < ASensors.count(); i++ ) {
    for ( int j = 0; j < ASensors.count(); j++ ) {
      if ( i != j ) {
        if ( ASensors.at(i)->uid() == ASensors.at(j)->uid() ) {
          ExitByDuplicateUID( ASensors.at(i), ASensors.at(j) );
        }
      }
    }
    for ( int j = 0; j < AMotors.count(); j++ ) {
      if ( ASensors.at(i)->uid() == AMotors.at(j)->uid() ) {
        ExitByDuplicateUID( ASensors.at(i), AMotors.at(j) );
      }
    }
  }
}

void MainWindow::ExitByDuplicateUID( AUnit0 *a1, AUnit0 *a2 )
{
  qDebug() << tr( "UIDs [%1](at line %2) and [%3](at line %4) are duplicated." )
              .arg( a1->uid() ).arg( a1->aLine() )
              .arg( a2->uid() ).arg( a2->aLine() );
  qDebug() << tr( "1st one is : Type[%1] Identifier[%2] Driver[%3] Node[%4]" )
              .arg( a1->type() ).arg( a1->id() ).arg( a1->dev() ).arg( a1->ch() );
  qDebug() << tr( "2nd one is : Type[%1] Identifier[%2] Driver[%3] Node[%4]" )
              .arg( a2->type() ).arg( a2->id() ).arg( a2->dev() ).arg( a2->ch() );
  exit( 0 );
}

QString MainWindow::nextItem( QString start, QString &item )
{
  QString rs;
  int s;
  int i = 0;

//  start = start.simplified();   // stop using 'simplified'
  if ( start.simplified().isEmpty() ) {
    item = "";
    return "";
  }

  // simplified が null でないことは確認してあるので、必ず何か not-space がある
  s = 0;
  while( ( start[s].isSpace() ) && ( s < start.length() - 1 ) ) s++;
  start = start.mid( s );
  // そこまでを切り落としてしまえば OK

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
  item.replace( QChar( '~' ), QChar( ' ' ) );

  return rs;
}
