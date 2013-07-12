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

int MainWindow::findMini( QStringList &v1, QStringList &v2, QStringList &v3 )
{
  int num1 = v1[0].toInt();
  int num2 = v2[0].toInt();
  int num3 = v3[0].toInt();
  int num10 = v1.count() - 1;
  int num20 = v2.count() - 1;
  int num30 = v3.count() - 1;

  int rnum = num1;
  if ( num2 < rnum ) rnum = num2;
  if ( num10 < rnum ) rnum = num10;
  if ( num20 < rnum ) rnum = num20;
  if (( num3 > 1 )&&( num3 < rnum )) rnum = num3;
  if (( num30 > 1 )&&( num30 < rnum )) rnum = num30;

  return rnum;
}

int MainWindow::findMini( QStringList &v1, QStringList &v2,
                          QStringList &v3, QStringList &v4 )
{
  int num1 = v1[0].toInt();
  int num2 = v2[0].toInt();
  int num3 = v3[0].toInt();
  int num4 = v4[0].toInt();
  int num10 = v1.count() - 1;
  int num20 = v2.count() - 1;
  int num30 = v3.count() - 1;
  int num40 = v4.count() - 1;

  QList<int> list;

  list << num1 << num2 << num3 << num4
       << num10 << num20 << num30 << num40;

  qDebug() << list.at(0);
  qSort( list.begin(), list.end() );
  qDebug() << list.at(0);

  return list.at(0);
}

// EIB741なし(Enc2なし)の場合は正しく動かない気がする
void MainWindow::WriteQBody( void )
{
  int Us = mUnits.count();

  QStringList vals0 = mUnits.at(0)->values();
  QStringList vals1 = mUnits.at(1)->values();
  QStringList vals2;
  QStringList valsEnc;

  if ( Enc2 != NULL )
    valsEnc = Enc2->values();
  else 
    valsEnc.clear();

  int num;
  if ( Us > 3 ) {
    vals2 =  mUnits.at(2)->values();
    num = findMini( vals0, vals1, vals2, valsEnc );
  } else {
    num = findMini( vals0, vals1, valsEnc );
  }

  NewLogMsg( tr( "QXafs data points [%1]." )
             .arg( num ) );

  double dark0 = mUnits.at(0)->getDark() * QXafsDwellTime;
  double dark1 = mUnits.at(1)->getDark() * QXafsDwellTime;
  double dark2 = 0;
  if ( Us > 3 )
    dark2 = mUnits.at(1)->getDark() * QXafsDwellTime;

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
  double i0, i1, i2;
  QString buf;

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
    i0 = vals0[i+1].toDouble() - dark0;
    i1 = vals1[i+1].toDouble() - dark1;
    if ( Us > 3 ) {
      i2 = vals2[i+1].toDouble() - dark2;
      buf.sprintf( "%10.5f" "%10.5f" "%10.4f" " %8.7f" " %8.7f" " %8.7f",
		   deg, deg2, QXafsDwellTime, i0, i1, i2 );
    } else {
      buf.sprintf( "%10.5f" "%10.5f" "%10.4f" " %8.7f" " %8.7f",
		   deg, deg2, QXafsDwellTime, i0, i1 );
    }
    out << buf << endl;
  }

  file.close();
}
