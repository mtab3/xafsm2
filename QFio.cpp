#include <QtGui>
#include <math.h>

#include "MainWindow.h"

#define BLNAME    ( "BL5S1" )

void MainWindow::SetDFName2( int rpt, DIRECTION dir )
{
  QString buf;

  if ( SvSelExtPattern || ( SelRPT->text().toInt() == 1 ) ) {
    if ( rpt == 1 ) {
      if ( dir == FORWARD ) {
        DFName = DFName0 + DFName00 + "-f" + ".dat";
      } else {
        DFName = DFName0 + DFName00 + "-b" + ".dat";
      }
    } else {
      buf.sprintf( ".%04d", rpt - 1 );
      if ( dir == FORWARD ) {
        DFName = DFName0 + DFName00 + "-f" + buf;
      } else {
        DFName = DFName0 + DFName00 + "-b" + buf;
      }
    }
  } else {
    buf.sprintf( "-%04d.dat", rpt - 1 );
    if ( dir == FORWARD ) {
      DFName = DFName0 + DFName00 + "-f" + buf;
    } else {
      DFName = DFName0 + DFName00 + "-b" + buf;
    }
  }
}

void MainWindow::MakeDelegateFile( void )
{
  QFile file( DFName0 + ".dat" );
  if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;
  
  // Writing fixed headers
  QTextStream out(&file);

  out << "#" << " 1304     AichiSR QXAFS base file" << endl;
  out << "#" << " Unit      : " << UnitName[ SBLKUnit ].name;
  out << "#" << " Start     : " << SBlockStartAsDisp[0] << endl;
  out << "#" << " End       : " << SBlockStartAsDisp[1] << endl;
  out << "#" << " Dir       : " << ( (QMeasOnBackward->isChecked())
				     ? "Both(Forward and Backward" : "Single(Forward)" )
                                << endl;
  out << "#" << " Points    : " << SBlockPoints[0] << endl;
  out << "#" << " MeasTime  : " << QString::number( QXafsDwellTime, 'f', 10 )
                                << "[s]" << endl;
  out << "#" << " SweepTime : " << SBlockDwell[0] << "[s]" << endl;
  out << "#" << " Repeats   : " << SelRPT->value() << "[cycles]" << endl;
  out << "#" << " Date      : " << QDateTime::currentDateTime()
                                     .toString("yy.MM.dd hh:mm") << endl;
  if ( SLS != NULL ) 
    out << "#" << " Ring Cur. : " << SLS->value().toDouble() << "[mA]" << endl;

  out << "#" << endl;
  out << "#" << " ---- QXafs PM setting parameters ---- " << endl;
  out << "#" << " Motion start point : " << QXafsSP << endl;
  out << "#" << " Motion end point   : " << QXafsEP << endl;
  out << "#" << " Pulse start point  : " << QXafsSP0 << endl;
  out << "#" << " Pulse stop point   : " << QXafsEP0 << endl;
  out << "#" << " Pulse interval     : " << QXafsInterval << endl;
  out << "#" << " Pulse number       : " << QXafsPoints << endl;
  out << "#" << " Pulse dwell time   : " << QXafsDwellTime << endl;
  out << "#" << " Run Up Time        : " << RunUpTime << endl;
  out << "#" << endl;

  file.close();
}

void MainWindow::WriteQHeader( int rpt, DIRECTION dir )
{
  SetDFName2( rpt, dir );   // Generate a file name with repitation number

  double sblkdwell = SBlockDwell[0];
  SBlockDwell[0] = QXafsDwellTime;

  WriteHeaderCore();

  SBlockDwell[0] = sblkdwell;
}

void MainWindow::WriteQHeader2( int rpt, DIRECTION dir )
{
  SetDFName2( rpt, dir );   // Generate a file name with repitation number

  WriteHeaderCore2();
}

// EIB741なし(Enc2なし)の場合は正しく動かない気がする
void MainWindow::WriteQBody( void )
{
  int Us = mUnits.count();

  QStringList valsEnc;
  if ( Enc2 != NULL ) {
    valsEnc = Enc2->values();
    Us -= 1;
  } else {
    valsEnc.clear();
  }

  QVector<QStringList> vals;
  QVector<double> dark;
  int num = 100000000;
  for ( int i = 0; i < Us; i++ ) {
    vals << mUnits.at(i)->values();
    dark << mUnits.at(i)->getDark() * QXafsDwellTime;
    if ( num > vals[i][0].toInt() )
      num = vals[i][0].toInt();
  }
  if ( num > valsEnc[0].toInt() )
    num = valsEnc[0].toInt();

  NewLogMsg( tr( "QXafs data points [%1]." )
             .arg( num ) );

  QFile file( DFName );
  if ( !file.open( QIODevice::Append | QIODevice::Text ) ) {
    NewLogMsg( tr( "Can't open QXafs data file [%1] to write data body." )
               .arg( DFName ) );
    return;
  }

  QTextStream out( &file );

  int p = QXafsSP0;
  int d = QXafsInterval;
  int c = MMainTh->getCenter();
  double upp = MMainTh->getUPP();
  double deg, deg2;
  double upp2 = 0;
  QString buf, buf2;

  if ( Enc2 != NULL ) {
    upp2 = Enc2->getUPP();
  }

  for ( int i = 0; i < num; i++ ) {
    deg = ( p - c ) * upp; // pm16c14 のパルス値から計算
    // p += d;
    p -= d;
    if ( Enc2 == NULL ) {
      deg2 = deg;
    } else {
      // EIB741 が使える時はエンコーダ値
      deg2 = EncValue0.toDouble() + ( valsEnc[i+1].toInt() - Enc2Value0.toInt() ) * upp2;
    }
    buf.sprintf( "%10.5f" "%10.5f" "%10.4f", deg, deg2, QXafsDwellTime );

    for ( int j = 0; j < Us; j++ ) {
      buf2.sprintf( " %9.6f", vals[j][i+1].toDouble() - dark[j] );  // 8.7 --> 9.6
      buf += buf2;
    }
    out << buf << endl;
  }

  file.close();
}
