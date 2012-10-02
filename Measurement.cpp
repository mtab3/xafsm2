#include <QWidget>

#include "MainWindow.h"
#include "Stars.h"

/* 以下 Stars とのインターフェイス関数 */

double MainWindow::GetCurPosKeV( void ) // 分光器の現在位置の問い合わせ(keV単位で返答)
{                                 /* keV 単位の指定は分光器を交換したときなど問題かも */
  double rv;
  char *rrv;

  rv = atof( rrv = sks->GetValue( MONO1 ) );  // ここでエラーを拾うべき

  return rv;
}

void MainWindow::MoveCurPosManualKeV( double keV ) // 分光器の移動指令(keV単位で位置指定)
{
  if ( inMeas == 1 )
    return;
  if ( inMove == 0 ) {
    MoveCurPosKeV( keV );
  } else {
    // (現在位置を新しい移動先に指定 これで止まる? )
    NewLogMsg( QString( tr( "Mono: Stop Moving\n" ) ) );

    sks->Stop( MONO1 );
  }
}

void MainWindow::MoveCurPosKeV( double keV ) // 分光器の移動指令(keV単位で位置指定)
{
  if ( keV2deg( keV ) > 0 ) {
    if ( inMeas == 0 ) {
      NewLogMsg( QString( tr( "Mono: Move %1 to %2 keV\n" ) )
		 .arg( CurPosKeV ).arg( keV ) );
    }
    sks->SetValue( MONO1, keV );
  } else {
    statusbar->showMessage( tr( "Distination is out of range." ), 4000 );
  }
  inMove = 1;
}

int MainWindow::isFinishedCurMove( void )   // 分光器の移動終了の問い合わせ
{
  int rv = sks->IsBusy( MONO1 );
  if ( rv == -1 )
    statusbar->showMessage( tr( "Cannot get Monochro moving status." ), 5000 );

  return 1 - rv;                   // 今の戻り値 0 : Busy, 1: Ready, 2: Error
}

void MainWindow::MeasureICH( int devNo, double dwell ) // とある(測定)デバイスの値を
                                                       // dwell 時間(積分して)測定する
{
  sks->StartMeas( "ion-ch", dwell, devNo - IONCH0 );
}

void MainWindow::MeasureSSD( double dwell ) // とある(測定)デバイスの値を
                                                       // dwell 時間(積分して)測定する
{
  sks->StartMeas( "SSD", dwell );
}

void MainWindow::MeasureAUX( int devNo, double dwell ) // とある(測定)デバイスの値を
                                                       // dwell 時間(積分して)測定する
{
  sks->StartMeas( "aux-in", dwell, devNo - AUXCH0 );
}

int MainWindow::isFinishedMeasICH( int devNo )
                                              // とあるデバイスが指定された積分時間の
                                              // 測定を終えたかどうかの問い合わせ
{
  int rv = sks->IsBusy( "ion-ch", devNo - IONCH0 );
  if ( rv == -1 )
    statusbar->showMessage( tr( "Cannot get ion chamber measurement status." ), 5000 );

  return 1 - rv;                   // 今の戻り値 0 : Busy, 1: Ready, 2: Error
}

int MainWindow::isFinishedMeasSSD( void )
                                              // とあるデバイスが指定された積分時間の
                                              // 測定を終えたかどうかの問い合わせ
{
  int rv = sks->IsBusy( "SSD" );
  if ( rv == -1 )
    statusbar->showMessage( tr( "Cannot get ion chamber measurement status." ), 5000 );

  return 1 - rv;                   // 今の戻り値 0 : Busy, 1: Ready, 2: Error
}

int MainWindow::isFinishedMeasAUX( int devNo )
                                              // とあるデバイスが指定された積分時間の
                                              // 測定を終えたかどうかの問い合わせ
{
  int rv = sks->IsBusy( "aux-in", devNo );
  if ( rv == -1 )
    statusbar->showMessage( tr( "Cannot get ion chamber measurement status." ), 5000 );

  return 1 - rv;                   // 今の戻り値 0 : Busy, 1: Ready, 2: Error
}


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
