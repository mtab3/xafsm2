#include <QtGui>

#include "MainWindow.h"

#define BLNAME    ( "BL5S1" )

int MainWindow::GetDFName0()
{
  DFName0 = EditDFName->text();

  if ( DFName0.isEmpty() ) {
    return 0;
  }

  if ( DFName0.toUpper().lastIndexOf( ".DAT" ) == DFName0.length() - 4 ) {
    DFName0 = DFName0.left( DFName0.length() - 4 );
  }

  return 1;
}

void MainWindow::SetDFName( int i )
{
  char buf[ 100 ];

  if ( i == 0 ) {
    DFName = DFName0 + ".dat";
  } else {
    sprintf( buf, ".%03d", i );
    DFName = DFName0 + QString( buf );
  }
}

QString MainWindow::fixS( QString s, int l )
{
  QString r;

  r.fill( ' ', l - s.size() );
  s += r;
  s.left( l );

  return s;
}

void MainWindow::WriteHeader( int Rpt )
{
  SetDFName( Rpt );   // Generate a file name with repitation number

  QFile file( DFName );
  if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;

  // Writing fixed headers
  QTextStream out(&file);

  out << " " << fixS( " 9809     AichiSR", 20 ) << fixS( BLNAME, 5 ) << endl;
  out << " " << fixS( DFName, 14 ) << "  "
      << QDateTime::currentDateTime().toString("yy.MM.dd hh:mm") << " - "
      << "%001%" << endl; // 置換用のマーク 001 番目 14桁終了日時 "yy.MM.dd hh:mm"
  out << " " << fixS( EditCMT->text(), 80 ) << endl;
  out << " " << "Ring : " << "  1.2 GeV"
      << QString( "  %1 mA - " ).arg( nowCurrent, 6, 'f', 1, ' ' )
      << "%002%"          // 置換用のマーク 002 番目 実数6.1リング電流
      << " mA" << endl;
  out << " " << "Mono :   " << fixS( mccd[ selMC->MC() ].MCName, 10 )   // Si(111)
      << QString( "    D= %1 A    " ).arg( mccd[ selMC->MC() ].d, 8, 'f', 5, ' ' )
      << QString( "Initial angle=%1 deg" ).arg( keV2deg( InitialKeV ), 9, 'f', 5, ' ' ) 
      << endl;
  out << " " << fixS( BLNAME, 5 ) << "    "
      << fixS( CMode[ MeasFileType ], 13 ) << QString( "(%1)" ).arg( MeasFileType, 2 )
      << QString( "   Repitition=%1" ).arg( MeasR + 1, 3 )
      << QString( "     Points=%1" ).arg( TP, 5 ) << endl;

  if ( SBLKUnit == DEG ) {
    out << " "
	<< QString( "Param file : %1  angle axis (1)     Block =%2" )
      .arg( fixS( "DUMMYNAME.prm", 14 ) ).arg( Blocks, 2 ) << endl;
    out << endl;
    out << " "
	<< "Block      Init-ang  final-ang     Step/deg     Time/s       Num" << endl;
    for ( int i = 0; i < Blocks; i++ ) {
      out << " " << QString( "%1     %2%3%4%5%6" )
	.arg( i+1, 5 )
	.arg( SBlockStart[i], 10, 'f', 5 )
	.arg( SBlockStart[i+1], 10, 'f', 5 )
	.arg( SBlockStep[i], 13, 'f', 6 )
	.arg( SBlockDwell[i], 11, 'f', 2 )
	.arg( SBlockPoints[i], 10 )
	  << endl;
    }
  } else {
    out << " "
	<< QString( "Param file : %1  energy axis (2)     Block =%2" )
      .arg( fixS( "DUMMYNAME.prm", 14 ) ).arg( Blocks, 2 ) << endl;
    out << endl;
    out << " "
	<< "Block      Init-Eng  final-Eng     Step/eV     Time/s       Num" << endl;
    for ( int i = 0; i < Blocks; i++ ) {
      out << " " << QString( "%1     %2%3%4%5%6" )
	.arg( i+1, 5 )
	.arg( any2keV( SBLKUnit, SBlockStart[i] ) * 1000, 10, 'f', 2 )
	.arg( any2keV( SBLKUnit, SBlockStart[i+1] ) * 1000, 10, 'f', 2 )
	.arg( any2keV( SBLKUnit, SBlockStep[i] ) * 1000, 13, 'f', 2 )
	.arg( SBlockDwell[i], 11, 'f', 2 )
	.arg( SBlockPoints[i], 10 )
	  << endl;
    }
  }
  out << " " << QString( "SCALE( 2)     NDCH =%1" ).arg( MeasChNo, 2 ) << endl;
  switch( MeasFileType ) {
  case TRANS:
    out << " Angle(c) Angle(o) time/s";
    for ( int i = 0; i < MeasChNo; i++ )
      out << QString( "%1" ).arg( i+1, 10 );
    out << endl;
    out << QString( "      Mode         0         0"
		    "%1%2" ).arg( 1, 10 ).arg( 2, 10 ) << endl;
    out << QString( "    Offset         0         0"
		    "%1%2" ).arg( 0.0, 10, 'f', 3 ).arg( 0.0, 10, 'f', 3 ) << endl;
    // offset は真面目に扱ってない
    break;
  case FLUO:
    out << " Angle(c) Angle(o) time/s";
    for ( int i = 0; i < MeasChNo; i++ )
      out << QString( "%1" ).arg( i+1, 10 );
    for ( int i = 0; i < MeasChNo; i++ )
      out << QString( "%1" ).arg( i+1+MeasChNo, 10 );
    out << endl;
    out << QString( "      Mode         0         0" );
    for ( int i = 0; i < MeasChNo; i++ ) {
      out << QString( "%1" ).arg( 2 );
      // 真面目に書いてない (I0 を含む各チャンネルのモードを書かないといけない)
    }
    for ( int i = 0; i < MeasChNo; i++ ) {
      out << QString( "%1" ).arg( 2 );
      // 真面目に書いてない (ICRであることと、リセット回数であることを書かないといけない)
    }
    out << endl;
    out << QString( "    Offset         0         0" );
    for ( int i = 0; i < MeasChNo; i++ ) {
      out << QString( "%1" ).arg( 0.0, 10, 'f', 3 );
    }
    out << endl;
    // offset は真面目に扱ってない
    break;
  case AUX:
  case EXTRA:  // FLUO との違いは ICR や リセット回数の欄が無いこと
    out << " Angle(c) Angle(o) time/s";
    for ( int i = 0; i < MeasChNo; i++ )
      out << QString( "%1" ).arg( i+1, 10 );
    for ( int i = 0; i < MeasChNo; i++ )
      out << QString( "%1" ).arg( i+1+MeasChNo, 10 );
    out << endl;
    out << QString( "      Mode         0         0" );
    for ( int i = 0; i < MeasChNo; i++ ) {
      out << QString( "%1" ).arg( 2 );
      // 真面目に書いてない (I0 を含む各チャンネルのモードを書かないといけない)
    }
    out << endl;
    out << QString( "    Offset         0         0" );
    for ( int i = 0; i < MeasChNo; i++ ) {
      out << QString( "%1" ).arg( 0.0, 10, 'f', 3 );
    }
    out << endl;
    // offset は真面目に扱ってない
    break;
  default:
    qDebug() << "Unknown Measuremet type";
  }

  file.close();
}

void MainWindow::RecordData( void )
{
  SetDFName( MeasR );
  QFile file( DFName );
  if ( file.open( QIODevice::Append | QIODevice::Text ) ) {
    QTextStream out( &file );
    QString buf;
    for ( int i = 1; i < MCHANNELS; i++ ) {
      if ( MeasSensF[i] ) {
        buf.sprintf("%10.5f%10.5f%10.2f%10.0f%10.0f%10.0f",
                    keV2deg( GoToKeV ), EncMainTh->value().toDouble(),
		    NowDwell, MeasVals[ MC_I0 ], MeasVals[i], GoToKeV );
        out << buf;
      }
    }
    // 野村フォーマットだとホントはエネルギーは書かない
    // 本来2つめはエンコーダ
//    out << GoToKeV
//        << " " << keV2deg( GoToKeV ) << " " << keV2deg( GoToKeV )
//        << " " << NowDwell << " " << MeasVals[ MC_I0 ];

//    for ( int i = 1; i < MCHANNELS; i++ ) {
//      if ( MeasSensF[i] ) {
//        out << " " << MeasVals[i];
//      }
//    }
    out << endl;
    file.close();
  }

}

