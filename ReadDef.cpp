#include <QFile>
#include <QFileInfo>

#include "enums.h"
#include "MainWindow.h"

#define LN "\n"    // only for Qt on MacOS X
#define BS "\\"     // only for Qt on MacOS X

void MainWindow::ReadDef( QString fname )
{
  QString fname0 = "XAFSMDEF.def";
  if ( fname.isEmpty() ) {

  } else {
    fname0 = fname;
  }

  qDebug() << tr( "File [%1] is selected as XafsM2 definition file." ).arg( fname0 );

  QFileInfo ff( fname0 );
  qDebug() << tr( "FilePath = [%1]" ).arg( ff.absoluteFilePath() );
  
  QFile f( fname0 );
  if ( !f.exists() ) {
    qDebug() << tr( "The file [%1] is not. Use default parameters." ).arg( fname0 );
    f.setFileName( fname0 = ":/XAFSMDEF0.def" );
  }
  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    qDebug() << tr( "Error cannot open [%1]." ).arg( fname0 );
    return;
  }

  PMCs = 0;           // MAX number of PM16C04
  int PMC = 0;            // current PMC No.
  int PMs = 0;            // Max number of PMs in current Unit
  int ENCs = 0;
  int PZs = 0;

  int PM = 0;             // current PM
  int ENC = 0;            // current Encoder
  int PZ = 0;             // current Piezo unit
  int UN = 0;             // current Unit No. (union of PM, ENC, and PZ)

  SENSORs = 0;
  SENSOR = 0;             // current SENSOR No.
  MOTOR = 0;              // current MOTOR No.

  UTYPE cUType = UT_NONE;
  
  QString aline;
  QString next;
  QString item;
  QString driver;
  int tmp;

  QTextStream in( &f );
  while( !in.atEnd() ) {
    aline = in.readLine();
    if ( !aline.isEmpty() ) {
      //      next = aline.simplified();
      next = nextItem( aline.simplified(), item );
      
      switch( GetCmdIndex( item ) ) {
      case 0:                                                // a comment line
        break;

        /* Unit や Grup とは別に PMC の名前を定義しておく   */
        /* 本来は、これも Unit ごとに指定するべきだが、        */
        /* 沢山でてくるので、簡単に一括で変更できるようにするため。 */
      case 1:                                                // PMCs
        next = nextItem( next, item );
        PMCs = item.toInt();
        break;
      case 2:                                                // PMC
        next = nextItem( next, item );
        if ( ( tmp = item.toInt() ) < PMCs ) {
          PMC = tmp;
        } else {
          printf( "select PMC No. %d is larger than max No. %d" LN, tmp, PMCs);
        }
        break;
      case 3:                                                // PMCName
        next = nextItem( next, item );
        PMCName[ PMC ] = item;
        break;

      case 11: // Group 内の PM の数
        next = nextItem( next, item );
        PMs = item.toInt();
        break;
      case 12: // ENCs // Group 内の ENC の数
        next = nextItem( next, item );
        ENCs = item.toInt();
        break;
      case 13: // PZs // Group 内の PZ の数
        next = nextItem( next, item );
        PZs = item.toInt();
        break;

      case 14: // PM // このキーワードで PM というタイプのユニットの定義が始まる
        next = nextItem( next, item );
        cUType = UT_PM;
        if ( ( tmp = item.toInt() ) < PMs ) {
          UN = PM = tmp;
        } else {
          printf( "selected PM No. %d is larger than max No. %d" LN, tmp, PMs );
        }
        break;

      case 15: // ENC // このキーワードで ENC というタイプのユニットの定義が始まる
        next = nextItem( next, item );
        cUType = UT_ENC;
        if ( ( tmp = item.toInt() ) < ENCs ) {
          UN = ENC = tmp;
        } else {
          printf( "selected ENC No. %d is larger than max No. %d\n", tmp, ENCs );
        }
        break;

      case 16: // PZ // このキーワードで PZ というタイプのユニットの定義が始まる
        next = nextItem( next, item );
        cUType = UT_PZ;
        if ( ( tmp = item.toInt() ) < PZs ) {
          UN = PZ = tmp;
        } else {
          printf( "selected PZ No. %d is larger than max No. %d\n", tmp, PZs );
        }
        break;

      case 17:                                               // UNIT NAME
        next = nextItem( next, item );
	switch( cUType ) {
	case UT_PM:
	  wPMs[ UN ]->setUnitName( item ); break;
	case UT_PZ:
	  wPZs[ UN ]->setUnitName( item ); break;
	case UT_ENC:
	  wENCs[ UN ]->setUnitName( item ); break;
	case UT_SENS:
	  Sensors[ UN ]->setUnitName( item ); break;
	default:
	  qDebug() << tr( "Undefined Unit Type [%1] for the name [%2]" )
	    .arg( cUType ).arg( item );
	  break;
	}
      case 18:                                               // DRIVER
        next = nextItem( next, item );
        if ( cUType == UT_PM )
          driver = PMCName[ item.toInt() ];
        else
          driver = item;
        switch ( cUType ) {
	case UT_PM:
	  wPMs[ UN ]->setDriver( item ); break;
	case UT_PZ:
	  wPZs[ UN ]->setDriver( item ); break;
	case UT_ENC:
	  wENCs[ UN ]->setDriver( item ); break;
	case UT_SENS:
	  Sensors[ UN ]->setDriver( item ); break;
	default:
	  qDebug() << tr( "Undefined Unit Type [%1] for the name [%2]" )
	    .arg( cUType ).arg( item );
	  break;
	}
        break;
      case 19:                                               // CH
        next = nextItem( next, item );
        switch ( cUType ) {
	case UT_PM:
	  wPMs[ UN ]->setCh( item ); break;
	case UT_PZ:
	  wPZs[ UN ]->setCh( item ); break;
	case UT_ENC:
	  wENCs[ UN ]->setCh( item ); break;
	case UT_SENS:
	  Sensors[ UN ]->setCh( item ); break;
	default:
	  qDebug() << tr( "Undefined Unit Type [%1] for the name [%2]" )
	    .arg( cUType ).arg( item );
	  break;
	}
        break;
      case 20:                                               // UNAME
        next = nextItem( next, item );
        switch ( cUType ) {
	case UT_PM:
	  wPMs[ UN ]->setMetricUnitName( item ); break;
	case UT_PZ:
	  wPZs[ UN ]->setMetricUnitName( item ); break;
	case UT_ENC:
	  wENCs[ UN ]->setMetricUnitName( item ); break;
	case UT_SENS:
	  Sensors[ UN ]->setMetricUnitName( item ); break;
	default:
	  qDebug() << tr( "Undefined Unit Type [%1] for the name [%2]" )
	    .arg( cUType ).arg( item );
	  break;
	}
        break;

      case 21:                                               // UPP
        next = nextItem( next, item );
        switch ( cUType ) {
	case UT_PM:
	  wPMs[ UN ]->setUPP( item ); break;
	case UT_PZ:
	  wPZs[ UN ]->setUPP( item ); break;
	case UT_ENC:
	  wENCs[ UN ]->setUPP( item ); break;
	case UT_SENS: break;
	default:
	  qDebug() << tr( "Undefined Unit Type [%1] for the name [%2]" )
	    .arg( cUType ).arg( item );
	  break;
	}
        break;

      case 23:                                               // CENTER
        next = nextItem( next, item );
        if ( item != "None" ) {
	  switch ( cUType ) {
	  case UT_PM:
	    wPMs[ UN ]->setCenter( item ); break;
	  case UT_PZ:
	    wPZs[ UN ]->setCenter( item ); break;
	  case UT_ENC:
	    wENCs[ UN ]->setCenter( item ); break;
	  case UT_SENS: break;
	  default:
	    qDebug() << tr( "Undefined Unit Type [%1] for the name [%2]" )
	      .arg( cUType ).arg( item );
	    break;
	  }
        } else {
	  switch ( cUType ) {
	  case UT_PM:
	    wPMs[ UN ]->setNoCenter( item ); break;
	  case UT_PZ:
	    wPZs[ UN ]->setNoCenter( item ); break;
	  case UT_ENC:
	    wENCs[ UN ]->setNoCenter( item ); break;
	  case UT_SENS: break;
	  default:
	    qDebug() << tr( "Undefined Unit Type [%1] for the name [%2]" )
	      .arg( cUType ).arg( item );
	    break;
	  }
        }
        break;

      case 29:                                              // SENS
        next = nextItem( next, item );
        cUType = UT_SENS;
        if ( ( tmp = item.toInt() ) < MAXSENSORS ) {
          UN = SENSOR = tmp;
          if ( SENSOR + 1 > SENSORs ) {
            SENSORs = SENSOR + 1;
          }
        } else {
          printf( "selected SENSOR No. %d is larger than max No. %d\n",
                  tmp, MAXSENSORS );
        }
        break;
      case 30:                                              // SENSTYPE
        next = nextItem( next, item );
        if ( item == "Keithley 6485" )
          Sensors[ SENSOR ]->setSType( KEITHLEY6485 );
        if ( item == "Ion Ch" )
          Sensors[ SENSOR ]->setSType( IONCH );
        if ( item == "DUMMYPEAK" )
          Sensors[ SENSOR ]->setSType( DUMMYPEAK );
        break;

      case 31:                                              // MAXV
        next = nextItem( next, item );
        switch( cUType ) {
	case UT_PZ:
	  wPZs[ UN ]->setMaxV( item ); break;
	default:
	  printf( "The keyword MAXV is not applicable for the unit\n" );
	  break;
        }
        break;

      case 32:                                              // MINV
        next = nextItem( next, item );
        switch( cUType ) {
	case UT_PZ:
	  wPZs[ UN ]->setMinV( item ); break;
	default:
	  printf( "The keyword MAXV is not applicable for the unit\n" );
	  break;
        }
        break;

      case 99: // END
        return;
      default:
        qDebug() << tr( "Undefined command [%1]" ).arg( item );
        break;
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

int MainWindow::GetCmdIndex( QString item )
{
  struct CMDLIST {
    const QString cmd;
    int index;

  } CMDs[ 100 ] = {
    { "#",              0, },

    { "PMCs",           1, },
    { "PMC",            2, },
    { "PMCNAME",        3, },

    { "PMs",            4, },
    { "ENCs",           5, },
    { "PZs",            6, },

    { "PM",             7, },
    { "ENC",            8, },
    { "PZ",             9, },

    { "UNITNAME",      10, },
    { "DRIVER",        11, },
    { "CH",            12, },

    { "UNAME",         13, },
    { "UPP",           14, },
    { "CENTER",        15, },
    { "MAXV",          16, },
    { "MINV",          17, },

    { "SENS",          18, },
    { "SENSTYPE",      19, },

    
    { "END",           99, },
    { "",              -1, },
  };
  
  if ( item.at(0) == '#' )
    return 0;            // a comment
  
  for ( int i = 1; CMDs[i].cmd != "END"; i++ ) {
    if ( CMDs[i].cmd == item )
      return CMDs[i].index;
  }
  qDebug() << tr( "undefined Line [%1]\n" ).arg( item );
  return -1;               // not a registered command
}
