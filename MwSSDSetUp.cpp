
#include "MainWindow.h"

void MainWindow::setupSFluoRelated( void )
{
  connect( SSFluo0, SIGNAL( ReCalcXAFSWithMCA() ), this, SLOT( ReCalcXAFSWithMCA() ),
	   Qt::UniqueConnection );
  connect( SSFluo0, SIGNAL( ReCalcS2DMap( SetUpSFluo * ) ),
	   this, SLOT( ReCalcS2DMap( SetUpSFluo * ) ),
	   Qt::UniqueConnection );
  connect( SSFluo0, SIGNAL( ShowMessage( QString, int ) ),
	   statusbar, SLOT( showMessage( QString, int ) ), Qt::UniqueConnection );
  connect( SSFluo0, SIGNAL( addAnUsingUnit( QString, AUnit0 * ) ),
	   this, SLOT( addAnUsingUnit( QString, AUnit0 * ) ), Qt::UniqueConnection );
  connect( SSFluo0, SIGNAL( removeUsingUnits( QString, AUnit0 * ) ),
	   this, SLOT( removeUsingUnits( QString, AUnit0 * ) ), Qt::UniqueConnection );
  SSFluo0->setFDBase( fdbase );
  
  // Calibration Tab
  // D.T. Calib.
  if ( SChangers.count() > 0 ) {
    for ( int i = 0; i < SChangers.count(); i++ ) {
      AttChSelect->addItem( SChangers[i]->name() );
    }
    NewAttenCh( 0 );
  } else {
    AttChSelect->setEnabled( false );
    AttenSelect->setEnabled( false );
    AttenMoveTo->setEnabled( false );
    DTAutoCalib->setEnabled( false );
  }
  //  AttenDx = AttenDy = 0;
  connect( AttChSelect, SIGNAL( currentIndexChanged( int ) ),
	   this, SLOT( NewAttenCh( int ) ), Qt::UniqueConnection );
  connect( AttenMoveTo, SIGNAL( clicked() ), this, SLOT( NewAttenPos() ),
	   Qt::UniqueConnection );
  connect( DTAutoCalib, SIGNAL( clicked() ), this, SLOT( DTAutoCalibStart() ),
	   Qt::UniqueConnection );

  // SSD Energy Calib.
  if ( SSDCalibEnergys.count() > 0 ) {
    for ( int i = 0; i < SSDCalibEnergys.count(); i++ ) {
      EnergySelect->addItem( SSDCalibEnergys[i] );
    }
  } else {
    EnergySelect->setEnabled( false );
    EnergyMoveTo->setEnabled( false );
    // SSDEngAutoCalib->setEnabled( false ); // energy 選択のコンボボックスに値がなくても
                                           // スクリプトがあれば動けるので false にしない
  }
  connect( EnergyMoveTo, SIGNAL( clicked() ),
	   this, SLOT( MoveToNewCaribEnergy() ), Qt::UniqueConnection );
  connect( SSDEngAutoCalib, SIGNAL( clicked() ), this, SLOT( SSDEngAutoCalibStart() ),
	   Qt::UniqueConnection );
}

void MainWindow::addAnUsingUnit( QString id, AUnit0 *unit )
{
  UUnits.addAnUnit( id, unit );
}

void MainWindow::removeUsingUnits( QString id, AUnit0 *unit )
{
  UUnits.removeUnits( id, unit );
}

void MainWindow::ReCalcS2DMap( SetUpSFluo *ssfluo )
{
  //  if ( ( ! S2DI.valid ) || inMeas || inMCAMeas || inS2D ) {
  if ( inMeas || inS2D || ssfluo->isInMeas() ) {
    statusbar->showMessage( tr( "ROI cannot change while XAFS, S2D or MCA measurements" ), 2000 );
  }
  S2DBase->setS2DI( S2DI );
  emit ReCalcS2DMap0( ssfluo->roiStart(), ssfluo->roiEnd(), ssfluo->selBs2() );
}

void MainWindow::MoveToNewCaribEnergy( void )
{
  MoveCurThPosKeV( EnergySelect->currentText().toDouble() );
}

void MainWindow::NewAttenPos( void )
{
  SpecChanger *sc = SChangers[ AttChSelect->currentIndex() ];

  Changer *changer = Changers[ ChangerSelect->currentIndex() ];
  movingSC1 = changer->unit1();
  movingSC2 = changer->unit2();
  connect( movingSC1, SIGNAL( ChangedIsBusy1( QString ) ),
	   this, SLOT( SChangerReached( QString ) ),
	   Qt::UniqueConnection );
  connect( movingSC2, SIGNAL( ChangedIsBusy1( QString ) ),
	   this, SLOT( SChangerReached( QString ) ),
	   Qt::UniqueConnection );

  moveToTarget( sc->baseChangerP(),
		sc->specName( AttenSelect->currentIndex() )->position(),
		AttDx->text().toDouble(), AttDy->text().toDouble() );

  AttenMoveTo->setText( tr( "Moving" ) );
  AttenMoveTo->setStyleSheet( InActive );
  AttenMoveTo->setEnabled( false );
}

void MainWindow::NewAttenCh( int att )
{
  SpecChanger *c = SChangers[ att ];

  AttenSelect->clear();
  for ( int i = 0; i < c->specNames(); i++ ) {
    AttenSelect->addItem( c->specName(i)->name() );
  }
}

void MainWindow::newCalibration( void )
{
  if ( SSFluo0->isInMeas() )
    return;
  if ( SFluo == NULL )
    return;

  MCAView *view;   // ここでは view は直接使わないが、MCAPeaks は view 内部へのポインタ
  if ( ViewCtrls[ ViewTab->currentIndex() ]->getVType() == MCAVIEW ) {
    if ( ( view = (MCAView*)ViewCtrls[ ViewTab->currentIndex() ]->getView() ) != NULL ) {
      //(*(SSFluo0->peaks()))[ MCAPeakList->currentIndex() ].BinE;
      double oldE = SSFluo0->currentPeakE();
      if ( oldE <= 0 ) return;
      // 新旧の エネルギー比
      double ratio = SSFluo0->calibE() / oldE;
      if ( ratio <= 0 ) return;
      // 一回の入力で、このルーチンに複数回入ってくるとおかしくなるので
      // ある数値で一旦設定したら、入力欄自体をクリアしてしまう
      // PeakCalibrate->setText( "" );
      SSFluo0->clearCalibE();
      // gain の設定は何故か逆
      SFluo->setGain( SSFluo0->mcaCh(), SSFluo0->gain() / ratio );
      // 設定したゲインの読み出し
      s->SendCMD2( "SetUpMCA", SFluo->dev(),
		   "GetPreAMPGain", QString::number( SSFluo0->mcaCh() ) );
    }
  }
}
