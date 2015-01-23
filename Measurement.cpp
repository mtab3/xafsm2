#include "MainWindow.h"

void MainWindow::MeasSequence( void )
{
  double Delta;
  bool a1, a2;

  if ( inMeasDark ) return;
  if ( AskingOverwrite ) return;

  if ( MStabOk && MPSet.TuneAtEachStep && MMStab->isBusy() ) {
    return;
  }

  if ( MPSet.qXafsMode ) {
    ShowQXafsProgress();
  }

  if ( ( a1 = isBusyMotorInMeas() ) || ( a2 = mUnits.isBusy() ) ) {
    return;
  }
  if ( MovingToNewSamplePosition ) {
    if ( Changers[ ChangerSelect->currentIndex() ]->unit1()->isBusy()
	 || Changers[ ChangerSelect->currentIndex() ]->unit2()->isBusy() )
      return;
    MovingToNewSamplePosition = false;
  }

  NowTimeDisp->setText( QDateTime::currentDateTime().toString("yy.MM.dd hh:mm:ss") );

  if ( MPSet.qXafsMode ) {
    QXafsMeasSequence();
    return;
  }

  switch( MeasStage ) {
    /* 
       0: 測定開始 Repeat = 0
       1: Block = 0
       2: Step = 0, setDwellTime
       3: Goto a Position with a Block and a Step
       4: Prepare to trigger Sensors (only for cnt08)
       5: Trigger Sensors (for all)
       6: Read out Sensors
       10: Draw (Resume point from 99:)
          Step++; if ( Step < MaxStep ) goto 3
          Block++; if ( Block < MaxBlock ) goto 2
          Repeat++; if ( Repeat < MaxRepeat ) toto 1
          when reach here, finish.
       99: pause の時用のステージ
    */
  case 0:
    if ( AutoModeFirst )
      TouchDelegateFile();
    if ( AutoModeButton->isChecked() ) {
      CurrentRpt->setText( QString( "%1 - %2" ).arg( 1 ).arg( MeasA+1 ) );
    } else {
      CurrentRpt->setText( QString::number( 1 ) );
    }
    CurrentPnt->setText( QString::number( 1 ) );
    WriteInfoFile();
    mUnits.clearStage();
    MeasView->SetWindow0( u->any2keV( SBLKUnit, SBlockStartAsDisp[0] ), 0,
			  u->any2keV( SBLKUnit, SBlockStartAsDisp[ SBlocks ] ), 0 );
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
    if ( !FixedPositionMode ) {
      if ( SMeasInDeg ) {
	Delta = SBlockStartInDeg[MeasB+1] - SBlockStartInDeg[MeasB];
	GoToKeV = u->deg2keV( Delta/SBlockPoints[MeasB]*MeasS + SBlockStartInDeg[MeasB] );
      } else {
	Delta = SBlockStartAsDisp[MeasB+1] - SBlockStartAsDisp[MeasB];
	GoToKeV = u->any2keV( SBLKUnit, Delta / SBlockPoints[MeasB] * MeasS
			      + SBlockStartAsDisp[MeasB] );
      }
      MoveCurThPosKeV( GoToKeV );     // 軸の移動
    }
    mUnits.clearStage();
    if ( MStabOk && MPSet.TuneAtEachStep ) {
      MeasStage = 41;
    } else {
      if ( mUnits.isParent() )
	MeasStage = 5;
      else
	MeasStage = 6;
    }
    break;
  case 41:
    if ( MMStab != NULL ) {
      qDebug() << "Tune Abs " << MPSet.TuneESAbs << "Tune Quick " << MPSet.TuneESQuick;
      if ( MPSet.TuneESAbs ) {
	if ( MPSet.TuneESQuick ) {
	  qDebug() << "Abs Quick";
	  MMStab->GoMaxAbsQ( MPSet.TuneESStart, MPSet.TuneESEnd,
			     MPSet.TuneESSteps, MPSet.TuneESQuickTime );
	} else {
	  qDebug() << "Abs Normal";
	  MMStab->GoMaxAbs( MPSet.TuneESStart, MPSet.TuneESEnd, MPSet.TuneESSteps );
	}
      } else {
	if ( MPSet.TuneESQuick ) {
	  qDebug() << "Rel Quick";
	  MMStab->GoMaxRelQ( MPSet.TuneESStart, MPSet.TuneESSteps, MPSet.TuneESQuickTime );
	} else {
	  qDebug() << "Rel Normal";
	  MMStab->GoMaxRel( MPSet.TuneESStart, MPSet.TuneESSteps );
	}
      }
    }
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
    if ( inPause ) {
      MeasStage = 99;          // PauseStage
    }
    // don't break
  case 10:                     // This label is resume point from pausing
    MeasView->update();
    if ( conds->I0ShouldBeChecked() && ( MeasCPSs[0] < conds->I0Threshold() ) )
      MeasS--;
    MeasS++;
    if ( !inPause ) {
      if ( MeasS < SBlockPoints[ MeasB ] ) {
        MeasStage = 4;
      } else if ( MeasB < SBlocks-1 ) {
        MeasB++;
        MeasStage = 3;
      } else if ( MeasR < SelRPT->value()-1 ) {
        NewLogMsg( QString( tr( "Meas: Repeat %1" ) ).arg( MeasR + 1 ) );
        WriteHeader2( MeasR );
	SaveI0inMPSet();
        ClearXViewScreenForMeas( MeasView );
        PlayGoOnSound();
        WriteInfoFile2();
        MeasR++;
	if ( AutoModeButton->isChecked() ) {
	  CurrentRpt->setText( QString( "%1 - %2" ).arg( MeasR + 1 ).arg( MeasA+1 ) );
	} else {
	  CurrentRpt->setText( QString::number( MeasR + 1 ) );
	}
	if ( MPSet.isSFluo ) 
	  if ( ! MCACanSaveAllOnMem )
	    // 全部セーブできない時は、1スキャン終わったら
	    // 次のスキャンに備えてメモリクリア (直近の1スキャン分だけ覚えておく)
	    XafsMCAMap.New( MPSet.totalPoints, 1, MCALength, 19 );
                               // SelRPT->value() --> 1
        MeasStage = 2;
      } else {               // 終了
	UUnits.clear( MEAS_ID );
	CheckNewMeasFileName();
        statusbar->showMessage( tr( "The Measurement has Finished" ), 4000 );
        NewLogMsg( QString( tr( "Meas: Finished" ) ) );
        WriteHeader2( MeasR );
	SaveI0inMPSet();
        PlayEndingSound();
        WriteInfoFile2();
        MeasTimer->stop();
        inMeas = false;
	MPSet.normallyFinished = true;
        MeasStart->setText( tr( "Start" ) );
        MeasStart->setStyleSheet( NormalB );
        MeasPause->setEnabled( false );
        onMeasFinishWorks();
      }
    }
    break;
  case 99:
    if ( !inPause )
      MeasStage = 10;
    break;
  }
}

void MainWindow::onMeasFinishWorks( void )
{
  MPSet.finalRpt = MeasR + 1;
  MPSet.finalPnt = MeasP;
  qDebug() << "Final Rpt and Pnt " << MeasR << MeasP;
  SelRealTime->setChecked( SvSelRealTime );
  SelLiveTime->setChecked( SvSelLiveTime );
  MeasPause->setEnabled( false );
  MeasViewC->setIsDeletable( true );
  if ( OnFinishP->currentIndex() == (int)RETURN ) {
    MoveCurThPosKeV( InitialKeV );
    if ( AutoModeButton->isChecked() ) {
      connect( MMainTh, SIGNAL( ChangedIsBusy1( QString ) ),
	       this, SLOT( AutoXAFSSequence() ),
	       Qt::UniqueConnection );
    }
  } else {
    emit ChangerNext();
  }
}

bool MainWindow::isBusyMotorInMeas( void )
{
  bool rf = MMainTh->isBusy() || MMainTh->isBusy2();
  if ( MDTh1 != NULL )
    rf = rf || MDTh1->isBusy() || MDTh1->isBusy2();
  return  rf;
}

void MainWindow::SetDispMeasModes( void )
{
  int DLC = 0;     // display line count
  int DG = 0;      // display group
  int MUC = 0;     // multi unit count

  MeasView->SetRLine( 0 );            // まず、0 番目のラインを右軸に表示
  MeasView->SetLLine( 1 );            //       1 番目のラインを左軸に表示

  MeasView->SetLR( DLC, RIGHT_AX );                        // I0 
  MeasView->SetScaleType( DLC, I0TYPE );
  MeasView->SetLineName( DLC, mUnits.at( MUC )->getName() );
  MeasView->SetDG( DLC, DG++ );          // I0 は スケーリングのグループわけでは 0 
  DLC++;
  MUC++;
  if ( UseI1->isChecked() ) {  // I1 に対して線は 2 本
    MeasView->SetLLine( DLC );
    MeasView->SetLR( DLC, LEFT_AX );
    MeasView->SetScaleType( DLC, FULLSCALE );
    MeasView->SetLineName( DLC, mUnits.at( MUC )->getName() );
    MeasView->SetDG( DLC, DG++ );   // 生の I1 の表示は独立スケール
    DLC++;
    MeasView->SetLR( DLC, LEFT_AX );                     // mu
    MeasView->SetScaleType( DLC, FULLSCALE );
    MeasView->SetLineName( DLC, tr( "mu" ) );
    MeasView->SetDG( DLC, DG++ );     // mu も独立スケール
    DLC++;
    MUC++;
  }
  if ( Use19chSSD->isChecked() ) {
    MeasView->SetLR( DLC, LEFT_AX );
    MeasView->SetScaleType( DLC, FULLSCALE );
    MeasView->SetLineName( DLC, mUnits.at( MUC )->getName() );
    MeasView->SetDG( DLC, DG++ );    // ステップの時、基本的には各線は独立スケール
    DLC++;
    for ( int j = 0; j < MaxSSDs; j++ ) {
      MeasView->SetLR( DLC, LEFT_AX );
      MeasView->SetScaleType( DLC, FULLSCALE );
      MeasView->SetLineName( DLC, QString( "SSD %1" ).arg( j ) );
      MeasView->SetDG( DLC, DG );    // SSD の各チャンネルだけ同一スケールグループ
      DLC++;
    }
    MUC++;
  }
  if ( UseAux1->isChecked() ) {
    qDebug() << "meas-disp-mode for Aux1 " << DLC << MeasDispMode[ DLC ];
    if ( MeasDispMode[ DLC ] == TRANS ) { // 透過の時は一つの Aux の出力で 2本の線
      MeasView->SetLLine( DLC );
      MeasView->SetLR( DLC, LEFT_AX );                   // I1
      MeasView->SetScaleType( DLC, FULLSCALE );
      MeasView->SetLineName( DLC, mUnits.at( MUC )->getName() );
      MeasView->SetDG( DLC, DG++ );   // 生の I1 の表示は独立スケール
      DLC++;
      MeasView->SetLR( DLC, LEFT_AX );                     // mu
      MeasView->SetScaleType( DLC, FULLSCALE );
      MeasView->SetLineName( DLC, tr( "mu2" ) );
      MeasView->SetDG( DLC, DG++ );     // mu も独立スケール
      DLC++;
      MUC++;
    } else {
      MeasView->SetLR( DLC, LEFT_AX );
      MeasView->SetScaleType( DLC, FULLSCALE );
      MeasView->SetLineName( DLC, mUnits.at( MUC )->getName() );
      MeasView->SetDG( DLC, DG++ );    // ステップの時、基本的には各線は独立スケール
      DLC++;
      MUC++;
    }
  }
  if ( UseAux2->isChecked() ) {
    qDebug() << "meas-disp-mode for Aux1 " << DLC << MeasDispMode[ DLC ];
    if ( MeasDispMode[ DLC ] == TRANS ) { // 透過の時は一つの Aux の出力で 2本の線
      MeasView->SetLLine( DLC );
      MeasView->SetLR( DLC, LEFT_AX );                   // I1
      MeasView->SetScaleType( DLC, FULLSCALE );
      MeasView->SetLineName( DLC, mUnits.at( MUC )->getName() );
      MeasView->SetDG( DLC, DG++ );   // 生の I1 の表示は独立スケール
      DLC++;
      MeasView->SetLR( DLC, LEFT_AX );                     // mu
      MeasView->SetScaleType( DLC, FULLSCALE );
      MeasView->SetLineName( DLC, tr( "mu3" ) );
      MeasView->SetDG( DLC, DG++ );     // mu も独立スケール
      DLC++;
      MUC++;
    } else {
      MeasView->SetLR( DLC, LEFT_AX );
      MeasView->SetScaleType( DLC, FULLSCALE );
      MeasView->SetLineName( DLC, mUnits.at( MUC )->getName() );
      MeasView->SetDG( DLC, DG++ );    // ステップの時、基本的には各線は独立スケール
      DLC++;
      MUC++;
    }
  }
}

void MainWindow::DispMeasDatas( void )  // mUnits->readValue の段階でダーク補正済み
{
  int DLC0;
  int DLC = 0;   // display line count
  int MUC = 0;

  double I0, I1, A1, I00, V0, V;
  double sum;

  I1 = A1 = 0;
  I0 = MeasCPSs[ MUC ];
  MeasView->NewPoint( DLC, GoToKeV, I0 );
  DLC++;
  MUC++;
  if ( UseI1->isChecked() ) {
    I1 = V0 = MeasCPSs[ MUC ];
    MeasView->NewPoint( DLC, GoToKeV, V0 );   // I の値も表示する
    DLC++;
    if ( V0 < 1e-10 ) V0 = 1e-10;
    if ( ( V = ( I0 / V0 * MeasDispPol[ MUC ] ) ) > 0 ) {
        MeasView->NewPoint( DLC, GoToKeV, log( V ) );
      } else {
        MeasView->NewPoint( DLC, GoToKeV, 0 );
      }
    DLC++;
    MUC++;
  }
  if ( Use19chSSD->isChecked() ) {
    if ( I0 < 1e-20 )
      I0 = 1e-20;
    DLC0 = DLC;     // 合計表示は後回しにして、先に個別チャンネルの表示
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
    MUC++;
  }
  if ( UseAux1->isChecked() ) {
    A1 = V0 = MeasCPSs[ MUC ];
    MeasView->NewPoint( DLC, GoToKeV, V0 );   // I の値も表示する
    DLC++;
    if ( MeasDispMode[ MUC ] == TRANS ) {
      I00 = I0;
      if (( ModeA1->currentIndex() == 3 )||( ModeA1->currentIndex() == 4 ))
	I00 = I1;
      if ( fabs( V0 ) < 1e-10 ) V0 = 1e-10;
      if ( ( V = ( I00 / V0 * MeasDispPol[ MUC ] ) ) > 0 ) {
	MeasView->NewPoint( DLC, GoToKeV, log( V ) );
      } else {
	MeasView->NewPoint( DLC, GoToKeV, 0 );
      }
      DLC++;
    } else {  // MeasDispMode == FLUO
      if ( I0 < 1e-20 )
        I0 = 1e-20;
      MeasView->NewPoint( DLC, GoToKeV, V0/I0 );
      DLC++;
    }
    MUC++;
  }
  if ( UseAux2->isChecked() ) {
    V0 = MeasCPSs[ MUC ];
    if ( MeasDispMode[ MUC ] == TRANS ) {
      I00 = I0;
      if (( ModeA2->currentIndex() == 3 )||( ModeA2->currentIndex() == 4 ))
	I00 = I1;
      if (( ModeA2->currentIndex() == 5 )||( ModeA2->currentIndex() == 6 ))
	I00 = A1;
      MeasView->NewPoint( DLC, GoToKeV, V0 );   // I の値も表示する
      DLC++;
      if ( V0 < 1e-10 ) V0 = 1e-10;
      if ( ( V = ( I00 / V0 * MeasDispPol[ MUC ] ) ) > 0 ) {
	MeasView->NewPoint( DLC, GoToKeV, log( V ) );
      } else {
	MeasView->NewPoint( DLC, GoToKeV, 0 );
      }
      DLC++;
    } else {  // MeasDispMode == FLUO
      if ( I0 < 1e-20 )
        I0 = 1e-20;
      MeasView->NewPoint( DLC, GoToKeV, V0/I0 );
      DLC++;
    }
    MUC++;
  }

#if 0
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
#endif
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

void MainWindow::SaveI0inMPSet( void )
{
  if ( ! MPSet.isSFluo )
    return;

  double *i0 = new double [ MPSet.totalPoints ];

  for ( int i = 0; i < MPSet.totalPoints; i++ ) {
    i0[i] = MeasView->GetY( 0, i );
  }
  MPSet.i0s << i0;
}
