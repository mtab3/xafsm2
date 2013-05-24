#include "MainWindow.h"

void MainWindow::AutoMeasurement( void )
{
  MeasA = 0;
  AutoModeFirst = true;   // 通常/AutoMode にかかわらず 1 回目の測定フラグは立てておく
                          // ファイルの上書きチェックを制御するため
  if ( AutoModeButton->isChecked() ) {
    if ( ! ParseAutoMode() ) {
      statusbar->showMessage( tr( "Auto mode parameters are illigal."),
                              2000 );
      return;
    }
    if ( AutoModeParams.count() == 0 ) {
      statusbar->showMessage( tr( "Auto mode parameters are not set."),
                              2000 );
      return;
    }
    // In automode, a user should choose "Use Measured" to prevent stopping.
    if ( MeasBackBeforeMeas->isChecked() ) {
      statusbar->showMessage( tr( "Please choose \"Use Measured\""),
                              2000 );
      return;
    }
    // AutoMode->setDisabled( true );  // Now, there is no need to set it to be disabled
    MeasA = 0;
    // AutoModeParams.remove( 0 );
    moveToTarget( AutoModeParams[ MeasA ].num,
                  AutoModeParams[ MeasA ].dx,
                  AutoModeParams[ MeasA ].dz );
  }
  StartMeasurement();
}

void MainWindow::AutoSequence( void )
{
  AutoModeFirst = false;

  //  qDebug() << "AutoSequence";
  //  qDebug() << "MeasA" << MeasA
  //           << "AutoModeParams.count()" << AutoModeParams.count();

  if ( ( MeasA + 1 ) >=  AutoModeParams.count() ) {
    ChangerX->SetValue( 0 );
    ChangerZ->SetValue( 0 );
    disconnect( MMainTh, SIGNAL( ChangedIsBusy1( QString ) ),
                this, SLOT( AutoSequence() ) );
  } else if ( !(MMainTh->isBusy()) ) {
    MeasA++;
    disconnect( MMainTh, SIGNAL( ChangedIsBusy1( QString ) ),
                this, SLOT( AutoSequence() ) );
    moveToTarget( AutoModeParams[ MeasA ].num,
                  AutoModeParams[ MeasA].dx,
                  AutoModeParams[ MeasA ].dz );
    StartMeasurement();
  }
}

#define ChangerXSpacing  ( 50.0 )        // mm
#define ChangerZSpacing  ( 45.0 )        // mm

void MainWindow::SetNewChangerCenter( void )
{
  int target = ChangerCurrentHolder->value();
  int ix = ( target - 1 ) % 3 - 1;    // -1, 0, 1
  int iz = ( target - 1 ) / 3 - 1;    // -1, 0, 1
  int nowx = ChangerX->value().toInt();
  int nowz = ChangerZ->value().toInt();
  ChangerX->setCenter( nowx - ix * ChangerXSpacing / ChangerX->getUPP() );
  ChangerZ->setCenter( nowz - iz * ChangerZSpacing / ChangerZ->getUPP() );
}

void MainWindow::moveToTarget( int target, double dx, double dz )
{
  qDebug() << "target" << "dx" << "dz";
  qDebug() << target << dx << dz;
  MovingToNewSamplePosition = true;   // このフラグで移動中の測定をブロックする
  int ix = ( target - 1 ) % 3 - 1;    // -1, 0, 1
  int iz = ( target - 1 ) / 3 - 1;    // -1, 0, 1
  int targetx = ChangerX->u2p( ChangerXSpacing * ix * -1 ) + dx / ChangerX->getUPP();
  int targetz = ChangerZ->u2p( ChangerZSpacing * iz * -1 ) + dz / ChangerZ->getUPP();
  DFName00 = QString("_%1").arg( MeasA );
  AutoModeComment = QString( "Sample No. %1 +%2[mm] +%3[mm]" ).arg( target ).arg( dx ).arg( dz );
  ChangerX->SetValue( targetx );
  ChangerZ->SetValue( targetz );
}

