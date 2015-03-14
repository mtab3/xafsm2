
#include "MainWindow.h"

void MainWindow::setupSFluoRelated( void )
{
  for ( int i = 0; i < SSFluos.count(); i++ ) {
    connect( SSFluos[i], SIGNAL( ReCalcXAFSWithMCA() ), this, SLOT( ReCalcXAFSWithMCA() ),
	     Qt::UniqueConnection );
    connect( SSFluos[i], SIGNAL( ReCalcS2DMap( SetUpSFluo * ) ),
	     this, SLOT( ReCalcS2DMap( SetUpSFluo * ) ),
	     Qt::UniqueConnection );
    connect( SSFluos[i], SIGNAL( ShowMessage( QString, int ) ),
	     statusbar, SLOT( showMessage( QString, int ) ), Qt::UniqueConnection );
    connect( SSFluos[i], SIGNAL( addAnUsingUnit( QString, AUnit0 * ) ),
	     this, SLOT( addAnUsingUnit( QString, AUnit0 * ) ), Qt::UniqueConnection );
    connect( SSFluos[i], SIGNAL( removeUsingUnits( QString, AUnit0 * ) ),
	     this, SLOT( removeUsingUnits( QString, AUnit0 * ) ), Qt::UniqueConnection );
    connect( SSFluos[i], SIGNAL( askToSaveMCAData( SetUpSFluo *, QString ) ),
	     this, SLOT( saveMCAData( SetUpSFluo *, QString ) ), Qt::UniqueConnection );

    // これは本当にここでいいか?? newCalibration が複数の SFluo を区別できるか?
#if 0
    connect( SSFluos[i], SIGNAL( newCalibration() ), this, SLOT( newCalibration() ),
	     Qt::UniqueConnection );
#endif
    
    SSFluos[i]->setFDBase( fdbase );
    SSFluos[i]->setupSetupSFluo( s, &FSTATMsgs );
  }
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

void MainWindow::saveMCAData( SetUpSFluo *ssfluo, QString fname )
{
  AUnitSFluo *sfluo = ssfluo->sFluo();
  aMCASet *set = new aMCASet;
  set->setSize( sfluo->length(), sfluo->chs() );
  SaveMCADataOnMem( set, ssfluo );
  //  saveMCAData0( MCARecFile->text(), set );
  set->save( fname, "measured by SSD set up", sfluo->uid() );
  delete set;
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
  // 対象の MCA が単純な蛍光スペクトル測定以外(XAFS, S2D)に使われていてたら
  // ROI の変更を認めない
  if (( ssfluo->isInMeas() )&&( ssfluo->sFluo()->userName() != MCA_ID )) {
    statusbar
      ->showMessage( tr( "ROI cannot be change while XAFS or S2D measurements" ), 2000 );
    return;
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

