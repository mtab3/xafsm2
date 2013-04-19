#include <QtGui>
#include <math.h>

#include "MainWindow.h"

#define BLNAME    ( "BL5S1" )

void MainWindow::SetDFName2( int rpt, DIRECTION dir )
{
  QString buf;

  if ( rpt == 1 ) {
    if ( dir == FORWARD ) {
      DFName = DFName0 + "-f" + ".dat";
    } else {
      DFName = DFName0 + "-b" + ".dat";
    }
  } else {
    buf.sprintf( ".%03d", rpt - 1 );
    if ( dir == FORWARD ) {
      DFName = DFName0 + "-f" + buf;
    } else {
      DFName = DFName0 + "-b" + buf;
    }
  }
}

void MainWindow::WriteQHeader( int rpt, DIRECTION dir )
{
  SetDFName2( rpt, dir );   // Generate a file name with repitation number
  
  QFile file( DFName );
  if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;
  
  // Writing fixed headers
  QTextStream out(&file);

  out << "#" << " 1304     AichiSR QXAFS" << endl;
  out << "#" << " Start     : " << SBlockStart[0] << endl;
  out << "#" << " End       : " << SBlockStart[1] << endl;
  out << "#" << " Dir       : " << (( dir == FORWARD ) ? "Forward" : "Backward") << endl;
  out << "#" << " Points    : " << SBlockPoints[0] << endl;
  out << "#" << " ScanTime  : " << SBlockDwell[0] << endl;
  out << "#" << " Date      : " << QDateTime::currentDateTime().toString("yy.MM.dd hh:mm") << endl;
  if ( SLS != NULL ) 
    out << "#" << " Ring Cur. : " << SLS->value().toDouble() << "[mA]" << endl;

  file.close();
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

void MainWindow::WriteQBody( void )
{
  QStringList vals0 = mUnits.at(0)->values();
  QStringList vals1 = mUnits.at(1)->values();
  QStringList vals2;
  if ( Enc2 != NULL )
    vals2 = Enc2->values();
  else 
    vals2.clear();

  int num = findMini( vals0, vals1, vals2 );

  qDebug() << QString( "writing a file [%1]" ).arg( DFName );
  QFile file( DFName );
  if ( !file.open( QIODevice::Append | QIODevice::Text ) )
    return;

  QTextStream out( &file );

  int p = QXafsSP0;
  int d = QXafsInterval;
  int c = MMainTh->getCenter();
  double upp = MMainTh->getUPP();
  double deg, deg2;
  double upp2 = 0;
  if ( Enc2 != NULL ) {
    upp2 = Enc2->getUPP();
  }
  
  for ( int i = 0; i < num; i++ ) {
    deg = ( p - c ) * upp;
    p += d;
    if ( Enc2 == NULL ) {
      deg2 = deg;
    } else {
      deg2 = EncValue0.toDouble() + ( vals2[i+1].toInt() - Enc2Value0.toInt() ) * upp2;
    }
    out << deg << "\t" << deg2 << "\t"              // 片方はエンコーダ読みに直す
	<< vals0[i+1].toDouble() << "\t" << vals1[i+1].toDouble() << endl;
  }

  file.close();
}
