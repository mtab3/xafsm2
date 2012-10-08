#include <QWidget>

#include "MainWindow.h"
#include "Stars.h"

/* 以下 Stars とのインターフェイスのラッパ関数 */

void MainWindow::MoveCurThPosKeV( double keV ) // 分光器の移動指令(keV単位で位置指定)
{
  MMainTh->setIsBusy( true );
  MMainTh->SetValue( keV2deg( keV ) * MMainTh->getUPP() + MMainTh->getCenter() );
}

#if 0
void MainWindow::ReadOutTransData( double NowKeV ) // 透過法の測定結果を得て記録する
{
  double I0 = atof( sks->GetValue( "ion-ch", IONCH0 - IONCH0 ) );
  double I = atof( sks->GetValue( "ion-ch", IONCH1 - IONCH0 ) );
  NowView->NewPoint( 0, NowKeV, I0 );
  NowView->NewPoint( 1, NowKeV, log( I0/I ) );
}

void MainWindow::ReadOutSSDData( double NowKeV ) // 蛍光法の測定結果を得て記録する
{
  double I0 = atof( sks->GetValue( "ion-ch", IONCH0 - IONCH0 ) );
  QString rv = sks->GetValue( "SSD" );
  QStringList rvs = rv.simplified().split( QChar( ' ' ) );
  printf( "SSD rv[0] = %f, rv = %s\n", rvs[0].toDouble(), rv.toAscii().data() );

  double sum = 0;
  NowView->NewPoint( 0, NowKeV, I0 );
  for ( int i = 0; i < 19; i++ ) {
    sum += rvs[i].toDouble();
    NowView->NewPoint( i+2, NowKeV, rvs[i].toDouble() / I0 );
  }
  NowView->NewPoint( 1, NowKeV, sum / I0 );
}

void MainWindow::ReadOutAUXData( int devNo, double NowKeV )
                                         // 外部入力を使った測定結果を得て記録する
{
  double I0 = atof( sks->GetValue( "ion-ch", IONCH0 - IONCH0 ) );
  double I = atof( sks->GetValue( "aux-in", devNo - AUXCH0 ) );
  NowView->NewPoint( 0, NowKeV, I0 );
  NowView->NewPoint( 1, NowKeV, log( I0/I ) );
}
#endif
