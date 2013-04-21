#include "MainWindow.h"

void MainWindow::MeasSequence( void )
{
  double Delta;
  bool a1, a2;

  if ( inMeasDark ) return;
  if (( AskingOverwrite )||( MakingSureOfRangeSelect )) return;
  if ( ( a1 = isBusyMotorInMeas() ) || ( a2 = mUnits.isBusy() ) ) return;

  NowTimeDisp->setText( QDateTime::currentDateTime().toString("yy.MM.dd hh:mm:ss") );

  if ( QXafsMode->isChecked() ) {
    QXafsMeasSequence();
    return;
  }

  switch( MeasStage ) {
    /* 
       0: 測定開始 Repeat = 0
       1: Block = 0
       2: Step = 0, setDwellTIme
       3: Goto a Position with a Block and a Step
       4: prepare to triger Sensors (only for cnt08)
       5: Triger Sensors (for all)
       6: Read out Sensors
       10: Draw (Resume point from 99:)
          Step++; if ( Step < MaxStep ) goto 3
          Block++; if ( Block < MaxBlock ) goto 2
          Repeat++; if ( Repeat < MaxRepeat ) toto 1
          when reach here, finish.
       99: pause の時用のステージ
    */
  case 0:
    CurrentRpt->setText( QString::number( 1 ) );
    CurrentPnt->setText( QString::number( 1 ) );
    WriteInfoFile();
    mUnits.clearStage();
    MeasView->SetWindow0( SBlockStart[0], 0, SBlockStart[ SBlocks ], 0 );
    statusbar->showMessage( tr( "Start Measurement!" ) );
    MeasStage = 1;
  case 1:
    if ( mUnits.init() == false ) {  // true :: initializing
      MeasR = 0;    // Measurement Repeat count
      mUnits.clearStage();
      MeasStage = 2;
    }
    break;
  case 2:
    MeasB = 0;    // Measurement Block count
    MeasP = 0;    // Measurement point count
    statusbar->showMessage( tr( "Writing Header." ), 2000 );
    WriteHeader( MeasR );
    MeasStage = 3;
    // break;       MeasStage == 2 の動作はレスポンスを待つ必要なし
  case 3: 
    MeasS = 0;    // Measurement Step count in each block
    mUnits.setDwellTimes( NowDwell = SBlockDwell[ MeasB ] );
    mUnits.setDwellTime();
    MeasStage = 4;
    // break;       MeasStage == 3 もレスポンスを待つ必要なし
    //              (ここで操作したのはセンサーで, Stage == 4 でセンサーを操作しないから)
  case 4:
    Delta = u->keV2any( SBLKUnit, SBlockStart[MeasB+1] )
      - u->keV2any( SBLKUnit, SBlockStart[MeasB] );
    GoToKeV = u->any2keV( SBLKUnit, Delta / SBlockPoints[MeasB] * MeasS
		       + u->keV2any( SBLKUnit, SBlockStart[MeasB] ) );
    MoveCurThPosKeV( GoToKeV );     // 軸の移動
    mUnits.clearStage();
    if ( mUnits.isParent() )
      MeasStage = 5;
    else
      MeasStage = 6;
    break;
  case 5:
    if ( mUnits.getValue0() == false ) { // only for counters
      mUnits.clearStage();
      MeasStage = 6;
    }
    break;
  case 6:
    if ( mUnits.getValue() == false ) {  // true :: Getting
      mUnits.clearStage();
      MeasStage = 7;
    }
    break;
  case 7:
    mUnits.readValue( MeasVals, MeasCPSs, true );  // true : correct dark
    DispMeasDatas();
    RecordData();
    MeasP++;
    CurrentPnt->setText( QString::number( MeasP + 1 ) );
    MeasStage = 10;
    if ( inPause == 1 ) {
      MeasStage = 99;          // PauseStage
    }
    // don't break
  case 10:                     // This label is resume point from pausing
    MeasView->update();
    MeasS++;
    if ( inPause == 0 ) {
      if ( MeasS < SBlockPoints[ MeasB ] ) {
	MeasStage = 4;
      } else if ( MeasB < SBlocks-1 ) {
	MeasB++;
	MeasStage = 3;
      } else if ( MeasR < SelRPT->value()-1 ) {
	NewLogMsg( QString( tr( "Meas: Repeat %1" ) ).arg( MeasR + 1 ) );
	ClearXViewScreenForMeas( MeasView );
	WriteHeader2( MeasR );
	WriteInfoFile2();
	MeasR++;
	CurrentRpt->setText( QString::number( MeasR + 1 ) );
	MeasStage = 2;
      } else {               // 終了
	statusbar->showMessage( tr( "The Measurement has Finished" ), 4000 );
	NewLogMsg( QString( tr( "Meas: Finished" ) ) );
	WriteHeader2( MeasR );
	WriteInfoFile2();
	MeasTimer->stop();
	inMeas = 0;
	MeasStart->setText( tr( "Start" ) );
	MeasStart
	  ->setStyleSheet( "background-color: "
			   "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 "
			   "rgba(225, 235, 225, 255), stop:1 "
			   "rgba(255, 255, 255, 255));" );
	MeasPause->setEnabled( false );
	onMeasFinishWorks();
      }
    }
    break;
  case 99:
    if ( inPause == 0 )
      MeasStage = 10;
    break;
  }
}

void MainWindow::onMeasFinishWorks( void )
{
  MeasPause->setEnabled( false );
  if ( OnFinishP->currentIndex() == (int)RETURN ) {
    MoveCurThPosKeV( InitialKeV );
  }
  MeasViewC->setIsDeletable( true );
}

bool MainWindow::isBusyMotorInMeas( void )
{
  return MMainTh->isBusy() || MMainTh->isBusy2();
}

void MainWindow::SetDispMeasModes( void )
{
  int i;
  int DLC = 0;   // display line count

  MeasView->SetRLine( 0 );            // まず、0 番目のラインを右軸に表示
  MeasView->SetLLine( 1 );            //       1 番目のラインを左軸に表示

  MeasView->SetLR( DLC, RIGHT_AX );                        // I0 
  MeasView->SetScaleType( DLC, I0TYPE );
  MeasView->SetLineName( DLC, mUnits.at(0)->getName() );
  DLC++;
  for ( i = 1; i < mUnits.count(); i++ ) {
    if ( MeasDispMode[i] == TRANS ) {
      if (( i == 1 )&&( isSI1 )) {
	MeasView->SetLLine( DLC );
        MeasView->SetLR( DLC, LEFT_AX );                   // I1
	MeasView->SetScaleType( DLC, FULLSCALE );
	MeasView->SetLineName( DLC, mUnits.at(i)->getName() );
        DLC++;
      }
      MeasView->SetLR( DLC, LEFT_AX );                     // mu
      MeasView->SetScaleType( DLC, FULLSCALE );
      MeasView->SetLineName( DLC, tr( "mu" ) );
      DLC++;
    } else {
      MeasView->SetLR( DLC, LEFT_AX );
      MeasView->SetScaleType( DLC, FULLSCALE );
      MeasView->SetLineName( DLC, mUnits.at(i)->getName() );
      DLC++;
      if ( ( isSFluo )&&( ( DLC - 1 ) == SFluoLine ) ) {
	for ( int j = 0; j < MaxSSDs; j++ ) {
	  MeasView->SetLR( DLC, LEFT_AX );
	  MeasView->SetScaleType( DLC, FULLSCALE );
	  MeasView->SetLineName( DLC, QString( "SSD %1" ).arg( j ) );
	  DLC++;
	}
      }
    }
  }
}

void MainWindow::DispMeasDatas( void )  // mUnits->readValue の段階でダーク補正済み
{
  double I0;
  double Val;
  int i;
  int DLC0;
  int DLC = 0;   // display line count
  double sum;

  I0 = MeasCPSs[ MC_I0 ];
  MeasView->NewPoint( DLC, GoToKeV, I0 );
  DLC++;
  for ( i = 1; i < mUnits.count(); i++ ) {
    Val = MeasCPSs[i];
    if ( MeasDispMode[i] == TRANS ) {
      if (( i == 1 )&&( isSI1 )) {
        MeasView->NewPoint( DLC, GoToKeV, MeasCPSs[1] );   // I の値も表示する
        DLC++;
      }
      if ( Val < 1e-10 )
        Val = 1e-10;
      if ( ( I0 / Val ) > 0 ) {
        MeasView->NewPoint( DLC, GoToKeV, log( I0/Val * MeasDispPol[i] ) );
        DLC++;
      } else {
        MeasView->NewPoint( DLC, GoToKeV, 0 );
        DLC++;
      }
    } else {  // MeasDispMode == FLUO
      if ( I0 < 1e-20 )
        I0 = 1e-20;
      if ( ( isSFluo )&&( DLC == SFluoLine ) ) {
	DLC0 = DLC;
	DLC++;
	QVector<quint64> vals = SFluo->getCountsInROI();
	QVector<double> darks = SFluo->getDarkCountsInROI();
	sum = 0;
	for ( int j = 0; j < MaxSSDs; j++ ) {
	  double v = ((double)vals[j] / SFluo->GetSetTime() - darks[j] ) / I0;
	  if ( SSDbs2[j]->isChecked() == PBTrue ) // 和を取るのは選択された SSD だけ
	    sum += v;
	  MeasView->NewPoint( DLC, GoToKeV, v );
	  DLC++;
	}
	MeasView->NewPoint( DLC0, GoToKeV, sum );
	// ここで Val は cps にしてあるので OK
      } else {
	MeasView->NewPoint( DLC, GoToKeV, Val/I0 );
	// ここで Val は cps にしてあるので OK
	DLC++;
      }
    }
  }
}

void MainWindow::ReCalcSSDTotal( int, bool )
{
  double sum[ MAXPOINTS ];
  double *y;

  if ( SFluoLine < 0 )                  // 19ch SSD を使った蛍光測定の場合だけ
    return;
  if ( MeasView == NULL )                // View が割り振られてなければ何もしない
    return;

  int points = MeasView->GetPoints( 0 );

  for ( int i = 0; i < points; i++ ) {
    sum[i] = 0;
  }

  for ( int l = 0; l < MaxSSDs; l++ ) {  // 選択し直された SSD の ch に関して
    if ( SSDbs2[l]->isChecked() == PBTrue ) {
      y = MeasView->GetYp( SFluoLine + 1 + l );
      for ( int i = 0; i < points; i++ ) {  // 合計をとりなおす
	sum[i] += y[i];
      }
    }
  }
  y = MeasView->GetYp( SFluoLine );
  for ( int i = 0; i < points; i++ ) {
    y[i] = sum[i];
  }
  MeasView->update();
}
