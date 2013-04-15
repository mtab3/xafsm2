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
    buf.sprintf( ".%03d", rpt );
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
  out << "#" << " Dir       : " << ( ( dir == FORWARD ) ? "Forward" : "Backward" ) << endl;
  out << "#" << " Points    : " << SBlockPoints[0] << endl;
  out << "#" << " ScanTime  : " << SBlockDwell[0] << endl;
  out << "#" << " Date      : " << QDateTime::currentDateTime().toString("yy.MM.dd hh:mm") << endl;
  out << "#" << " Ring Cur. : " << SLS->value().toDouble() << "[mA]" << endl;
  file.close();
}

void MainWindow::WriteQBody( void )
{
  QStringList vals0 = mUnits.at(0)->values();
  QStringList vals1 = mUnits.at(1)->values();
  int num0 = vals0[0].toInt();
  int num1 = vals1[0].toInt();
  int num00 = vals0.count() - 1;
  int num10 = vals1.count() - 1;

  int num = num0;     // I0 �� I ��¬��������Ʊ���Ϥ�������äƤ�����ä�����
  if ( num < num1 )  num = num1;
  if ( num < num00 ) num = num00;  // �ºݤ������Ƥ����ǡ�����������٤Ƥߤ�
  if ( num < num10 ) num = num10;  // �Ȥˤ������֤��ä�����

  qDebug() << QString( "writing a file [%1]" ).arg( DFName );
  QFile file( DFName );
  if ( !file.open( QIODevice::Append | QIODevice::Text ) )
    return;

  QTextStream out( &file );
  int p = QXafsSP0;
  int d = QXafsInterval;
  int c = MMainTh->getCenter();
  double upp = MMainTh->getUPP();
  double deg;
  
  for ( int i = 0; i < num; i++ ) {
    deg = ( p - c ) * upp;
    p += d;
    out << deg << "\t" << deg << "\t"              // �����ϥ��󥳡����ɤߤ�ľ��
	<< vals0[i+1].toDouble() << "\t" << vals1[i+1].toDouble() << endl;
  }

  file.close();
}
