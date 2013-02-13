#include <QtGui>

#include "MainWindow.h"

void MainWindow::setupMeasArea( void )   /* 測定エリア */
{
  BLKstart << BLKs01 << BLKs02 << BLKs03 << BLKs04 << BLKs05
	   << BLKs06 << BLKs07 << BLKs08 << BLKs09;
  BLKstep  << BLKstep01 << BLKstep02 << BLKstep03 << BLKstep04 << BLKstep05
	   << BLKstep06 << BLKstep07 << BLKstep08;
  BLKdwell << BLKdwell01 << BLKdwell02 << BLKdwell03 << BLKdwell04 << BLKdwell05
	   << BLKdwell06 << BLKdwell07 << BLKdwell08;
  BLKpoints << BLKpoints01 << BLKpoints02 << BLKpoints03 << BLKpoints04 << BLKpoints05
	    << BLKpoints06 << BLKpoints07 << BLKpoints08;

  if ( SFluo == NULL ) 
    Use19chSSD->setEnabled( false );

  BLKUnit = KEV;
  ClearBLKs();
  ChangeBLKs( 4 );
  for ( int i = 0; i < UNITS; i++ ) {
    SelBLKUnit->addItem( UnitName[i].name );
  }
  SelBLKUnit->setCurrentIndex( BLKUnit );


  SelDFND = new QFileDialog;
  SelWBFND = new QFileDialog;
  SelRBFND = new QFileDialog;
  SelDFND->setAcceptMode( QFileDialog::AcceptSave );
  SelWBFND->setAcceptMode( QFileDialog::AcceptSave );
  SelRBFND->setAcceptMode( QFileDialog::AcceptOpen );
  SelDFND->setDirectory( QDir::currentPath() );
  SelWBFND->setDirectory( QDir::currentPath() );
  SelRBFND->setDirectory( QDir::currentPath() );
  SelDFND->setFilter( "*.dat" );
  SelWBFND->setFilter( "*.prm" );
  SelRBFND->setFilter( "*.prm" );
  OverWriteChecked = false;
  SelectedOrgName.clear();

  EditDFName->setText( "test.dat" );
  connect( EditDFName, SIGNAL( textEdited( const QString & ) ),
	   this, SLOT( isFileNameChanged( const QString & ) ) );

  OnFinishP->addItem( tr( "Return" ) );
  OnFinishP->addItem( tr( "Stay" ) );
  OnFinishP->setCurrentIndex( RETURN );

  QPushButton *tmpB;
  TP = 0;
  TT0 = 0;
  inMeas = 0;
  inPause = 0;
  MeasStage = 0;
  MeasPause->setEnabled( false );
  StopP = new QMessageBox;
  tmpB = StopP->addButton( tr( "Cancel" ), QMessageBox::RejectRole );
  StopP->addButton( tr( "OK" ), QMessageBox::AcceptRole );
  StopP->setText( tr( "<h1><center>Stop ?</center></h1>" ) );
  StopP->setWindowTitle( tr( "Stop or Continue" ) );
  StopP->setDefaultButton( tmpB );

  AskOverWrite = new QMessageBox;
  tmpB = AskOverWrite->addButton( tr( "Cancel" ), QMessageBox::RejectRole );
  AskOverWrite->addButton( tr( "OK" ), QMessageBox::AcceptRole );
  AskOverWrite->setWindowTitle( tr( "Over Write ?" ) );
  AskOverWrite->setDefaultButton( tmpB );

  MakeSureOfRangeSelect = new QMessageBox;
  tmpB = MakeSureOfRangeSelect->addButton( tr( "Cancel" ), QMessageBox::RejectRole );
  MakeSureOfRangeSelect->addButton( tr( "OK" ), QMessageBox::AcceptRole );
  MakeSureOfRangeSelect->setWindowTitle( tr( "Have you seleced ?" ) );
  MakeSureOfRangeSelect->setDefaultButton( tmpB );

  for ( int i = 0; i < ASensors.count(); i++ ) {
    QString name = ASensors.value(i)->getName(); 
    SelectI0->addItem( name );
    SelectI1->addItem( name );
    SelectAux1->addItem( name );
    SelectAux2->addItem( name );
    if ( ASensors.value(i)->getID() == "I0" )
      SelectI0->setCurrentIndex( i );
    if ( ASensors.value(i)->getID() == "I1" )
      SelectI1->setCurrentIndex( i );
    if ( ASensors.value(i)->getID() == "Aux1" )
      SelectAux1->setCurrentIndex( i );
    if ( ASensors.value(i)->getID() == "Aux2" )
      SelectAux2->setCurrentIndex( i );
  }
  UseI1->setChecked( true );

  ModeA1->addItem( "A1/I0" );
  ModeA1->addItem( "log(I0/A1)" );
  ModeA1->addItem( "log(-I0/A1)" );
  ModeA2->addItem( "A2/I0" );
  ModeA2->addItem( "log(I0/A2)" );
  ModeA2->addItem( "log(-I0/A2)" );

  for ( int i = 0; i < BLKstart.count(); i++ ) {
    connect( BLKstart.at(i), SIGNAL( editingFinished() ), this, SLOT(ChangeBLKstart()) );
  }
  for ( int i = 0; i < BLKstep.count(); i++ ) {
    connect( BLKstep.at(i), SIGNAL( editingFinished() ), this, SLOT(ChangeBLKstep()) );
  }
  for ( int i = 0; i < BLKdwell.count(); i++ ) {
    connect( BLKdwell.at(i), SIGNAL( editingFinished() ), this, SLOT(ChangeBLKdwell()) );
  }
  for ( int i = 0; i < BLKpoints.count(); i++ ) {
    connect( BLKpoints.at(i), SIGNAL( editingFinished() ),
	     this, SLOT(ChangeBLKpoints()) );
  }

  connect( SelBLKUnit, SIGNAL( currentIndexChanged( int ) ),
	   this, SLOT( ChangeBLKUnit( int ) ) );
  connect( SelBLKs, SIGNAL( valueChanged( int ) ), this, SLOT( ChangeBLKs( int ) ) );
  connect( StdEXAFS, SIGNAL( clicked() ), this, SLOT( SetStdEXAFSBLKs() ) );
  connect( StdXAFS, SIGNAL( clicked() ), this, SLOT( SetStdXAFSBLKs() ) );
  connect( StdXANES, SIGNAL( clicked() ), this, SLOT( SetStdXANESBLKs() ) );
  connect( DwellAll, SIGNAL( editingFinished() ), this, SLOT( SetDwells() ) );
  connect( SaveBLKs, SIGNAL( clicked() ), SelWBFND, SLOT( show() ) );
  connect( LoadBLKs, SIGNAL( clicked() ), SelRBFND, SLOT( show() ) );
  connect( SelWBFND, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( SelectedWBFN( const QString & ) ) );
  connect( SelRBFND, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( SelectedRBFN( const QString & ) ) );

  connect( SelDFName, SIGNAL( clicked() ), SelDFND, SLOT( show() ) );
  connect( SelDFND, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( SelectedNDFN( const QString & ) ) );

  connect( MeasStart, SIGNAL( clicked() ), this, SLOT( StartMeasurement() ) );
  connect( MeasPause, SIGNAL( clicked() ), this, SLOT( PauseMeasurement() ) );
  connect( StopP, SIGNAL( accepted() ), this, SLOT( SurelyStop() ) );
  connect( StopP, SIGNAL( rejected() ), this, SLOT( GoingOn() ) );
  connect( SelRPT, SIGNAL( valueChanged( int ) ), this, SLOT( NewRpt() ) );
  connect( AskOverWrite, SIGNAL( accepted() ), this, SLOT( OkOverWrite() ) );
  connect( AskOverWrite, SIGNAL( rejected() ), this, SLOT( SurelyStop() ) );
  connect( MakeSureOfRangeSelect, SIGNAL( accepted() ), this, SLOT( RangeSelOK() ) );
  connect( MakeSureOfRangeSelect, SIGNAL( rejected() ), this, SLOT( SurelyStop() ) );
  connect( MeasBackGround, SIGNAL( clicked() ), this, SLOT( MeasureDark() ) );

  inMeasDark = false;
  MeasDarkStage = 0;
  AskingShutterClose = false;
  AskingShutterOpen = false;
}

void MainWindow::ClearBLKs( void )
{
  for ( int i = 0; i < MaxBLKs+1; i++ ) {
    BlockStart[i] = 0;
    BlockPoints[i] = 0;
    BlockDwell[i] = 0;
  }
}

void MainWindow::ChangeBLKUnit( int i )
{
  BLKUnit = (UNIT)i;
  ShowBLKs();
}

void MainWindow::ChangeBLKs( int newBLKs )
{
  if ( SelBLKs->value() != newBLKs ) {
    SelBLKs->setValue( newBLKs );
  }
  Blocks = newBLKs;
  BLKstart[0]->setEnabled( true );
  for ( int i = 0; i < Blocks; i++ ) {
    BLKstart[i+1]->setEnabled( true );
    BLKstep[i]->setEnabled( true );
    BLKdwell[i]->setEnabled( true );
    BLKpoints[i]->setEnabled( true );
  }
  for ( int i = Blocks; i < MaxBLKs; i++ ) {
    BLKstart[i+1]->setEnabled( false );
    BLKstep[i]->setEnabled( false );
    BLKdwell[i]->setEnabled( false );
    BLKpoints[i]->setEnabled( false );
  }
  ShowBLKs();
}

void MainWindow::SetStdEXAFSBLKs( void )
{
  double Eg = ManTEkeV->text().toDouble();

  BlockStart[0] = Eg - 0.30;
  BlockStart[1] = Eg - 0.05;
  BlockStart[2] = Eg + 0.10;
  BlockStart[3] = Eg + 0.50;
  BlockStart[4] = Eg + 1.20;
  for ( int i = 5; i < MaxBLKs+1; i++ )
    BlockStart[i] = 0;

  BlockPoints[0] = 70;
  BlockPoints[1] = 150;
  BlockPoints[2] = 160;
  BlockPoints[3] = 100;
  for ( int i = 4; i < MaxBLKs; i++ )
    BlockPoints[i] = 0;

  BlockDwell[0] = 1.0;
  BlockDwell[1] = 1.0;
  BlockDwell[2] = 1.0;
  BlockDwell[3] = 1.0;
  for ( int i = 4; i < MaxBLKs; i++ )
    BlockDwell[i] = 0;

  ChangeBLKs( 4 );
  ShowBLKs();
}

void MainWindow::SetStdXAFSBLKs( void )
{
  double Eg = ManTEkeV->text().toDouble();

  BlockStart[0] = Eg - 0.30;
  BlockStart[1] = Eg - 0.04;
  BlockStart[2] = Eg + 0.05;
  BlockStart[3] = Eg + 0.50;
  BlockStart[4] = Eg + 1.10;
  for ( int i = 5; i < MaxBLKs+1; i++ )
    BlockStart[i] = 0;

  BlockPoints[0] = 40;
  BlockPoints[1] = 300;
  BlockPoints[2] = 180;
  BlockPoints[3] = 100;
  for ( int i = 4; i < MaxBLKs; i++ )
    BlockPoints[i] = 0;

  BlockDwell[0] = 1.0;
  BlockDwell[1] = 1.0;
  BlockDwell[2] = 1.0;
  BlockDwell[3] = 1.0;
  for ( int i = 4; i < MaxBLKs; i++ )
    BlockDwell[i] = 0;

  ChangeBLKs( 4 );
  ShowBLKs();
}

void MainWindow::SetStdXANESBLKs( void )
{
  double Eg = ManTEkeV->text().toDouble();

  BlockStart[0] = Eg - 0.03;
  BlockStart[1] = Eg + 0.07;
  for ( int i = 2; i < MaxBLKs+1; i++ )
    BlockStart[i] = 0;

  BlockPoints[0] = 300;
  for ( int i = 1; i < MaxBLKs; i++ )
    BlockPoints[i] = 0;

  BlockDwell[0] = 1.0;
  for ( int i = 1; i < MaxBLKs; i++ )
    BlockDwell[i] = 0;

  ChangeBLKs( 1 );
  ShowBLKs();
}

void MainWindow::ShowBLKs( void )
{
  QString buf;
  int i;

  for ( i = 0; i < MaxBLKs; i++ ) {
    buf.sprintf( UnitName[ BLKUnit ].form, keV2any( BLKUnit, BlockStart[i] ) );
    BLKstart[i]->setText( buf );
    if ( BlockPoints[i] > 0 ) {
      buf.sprintf( UnitName[ BLKUnit ].form,
	       ( keV2any(BLKUnit, BlockStart[i+1]) - keV2any(BLKUnit, BlockStart[i]) )
	       / BlockPoints[ i ] );
      BLKstep[i]->setText( buf );
    } else {
      BLKstep[i]->setText( "0" );
    }
    buf.sprintf( "% 5.1f", BlockDwell[i] );
    BLKdwell[i]->setText( buf );
    buf.sprintf( "% 4d", BlockPoints[i] );
    BLKpoints[i]->setText( buf );
  }
  buf.sprintf( UnitName[ BLKUnit ].form, keV2any( BLKUnit, BlockStart[i] ) );
  BLKstart[i]->setText( buf );

  ShowTotal();
#if 0
  for ( ; i < MaxBLKs; i++ ) {
    BLKstart[i+1]->setText( "" );
    BLKstep[i]->setText( "" );
    BLKdwell[i]->setText( "" );
    BLKpoints[i]->setText( "" );
  }
#endif
}

void MainWindow::NewRpt( void )
{
  ShowTotal();
}

void MainWindow::ShowTotal( void )
{
  QString buf;

  TP = 0;
  TT0 = 0;
  for ( int i = 0; i < Blocks; i++ ) {
    TP += BlockPoints[i];
    TT0 += BlockPoints[i] * BlockDwell[i];
  } 
  double TT = TT0;
  buf.sprintf( "%4d", TP * SelRPT->value() );
  TPoints->setText( tr( "Points: " ) + buf );
  TT *= SelRPT->value();
  int Th = (int)( TT / 3600 );
  TT -= Th * 3600;
  int Tm = (int)( TT / 60 );
  TT -= Tm * 60;
  int Ts = (int)TT;
  TT -= Ts;
  buf.sprintf( "%02d:%02d:%02d.%02d", Th, Tm, Ts, (int)(TT*100) );
  TTime->setText( tr( "Time: " ) + buf );
}

void MainWindow::ChangeBLKstart( void )
{
  for ( int i = 0; i < BLKstart.count(); i++ ) {
    if ( BLKstart.at(i) == sender() ) {
      BlockStart[i] = any2keV( BLKUnit, BLKstart[i]->text().toDouble() );
      double step = BLKstep[i]->text().toDouble();
      if ( step != 0 ) {
	BlockPoints[i]
	  = fabs(( keV2any(BLKUnit, BlockStart[i+1]) - keV2any(BLKUnit, BlockStart[i]) )
		 /step )+0.5;
      }
      if ( i > 0 ) {
	double step = BLKstep[i-1]->text().toDouble();
	if ( step != 0 ) {
	  BlockPoints[i-1]
	    = fabs(( keV2any(BLKUnit, BlockStart[i]) - keV2any(BLKUnit, BlockStart[i-1]) )
		   /step )+0.5;
	}
      }
      ShowBLKs();
    }
  }
}

void MainWindow::ChangeBLKstep( void )
{
  double step;

  for ( int i = 0; i < BLKstep.count(); i++ ) {
    if ( BLKstep.at(i) == sender() ) {
      step = BLKstep[i]->text().toDouble();
      if ( step != 0 ) {
	BlockPoints[i]
	  = fabs(( keV2any(BLKUnit, BlockStart[i+1]) - keV2any(BLKUnit, BlockStart[i]) )
		 /step )+0.5;
      }
      ShowBLKs();
    }
  }
}

void MainWindow::ChangeBLKpoints( void )
{
  for ( int i = 0; i < BLKpoints.count(); i++ ) {
    if ( BLKpoints.at(i) == sender() ) {
      BlockPoints[i] = BLKpoints[i]->text().toDouble();
      ShowBLKs();
    }
  }
}

void MainWindow::ChangeBLKdwell( void )
{
  for ( int i = 0; i < BLKdwell.count(); i++ ) {
    if ( BLKdwell.at(i) == sender() ) {
      BlockDwell[i] = BLKdwell[i]->text().toDouble();
      ShowBLKs();
    }
  }
}

void MainWindow::SetDwells( void )
{
  for ( int i = 0; i < Blocks; i++ ) {
    BlockDwell[i] = DwellAll->text().toDouble();
  }
  ShowBLKs();
}

void MainWindow::SelectedNDFN( const QString &fname )
{
  EditDFName->setText( fname );   // ここではファイル名をセットするだけ。
                                  // Start 時に書き出す。
  OverWriteChecked = true;
  SelectedOrgName = fname;
}

void MainWindow::isFileNameChanged( const QString &fname )
{
  if ( fname != SelectedOrgName ) {
    OverWriteChecked = false;
  }
}

void MainWindow::SelectedWBFN( const QString &fname )
{
  QFile f( fname );
  if ( !f.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;

  QTextStream out( &f );

  out << "N " << Blocks << endl;
  out << "U " << BLKUnit << endl;
  for ( int i = 0; i < MaxBLKs+1; i++ ) {
    out << "B " << i << " " << BlockStart[i] << " "
	<< BlockPoints[i] << " " << BlockDwell[i] << endl;
  }

  f.close();
}

void MainWindow::SelectedRBFN( const QString &fname )
{
  QFile f( fname );
  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) )
    return;

  QTextStream in( &f );

  QRegExp sep("\\s+");
  while (!in.atEnd()) {
    int i;
    QString line = in.readLine();

    if ( line[0] == QChar( 'N' ) ) {
      Blocks = line.section( sep, 1, 1 ).toInt();
    }
    if ( line[0] == QChar( 'U' ) ) {
      BLKUnit = (UNIT)line.section( sep, 1, 1 ).toInt();
    }
    if ( line[0] == QChar( 'B' ) ) {
      i = line.section( sep, 1, 1 ).toInt();
      BlockStart[i] = line.section( sep, 2, 2 ).toDouble();
      BlockPoints[i] = line.section( sep, 3, 3 ).toInt();
      BlockDwell[i] = line.section( sep, 4, 4 ).toDouble();
    }
  }

  f.close();
  ShowBLKs();
}

void MainWindow::ClearXViewScreenForMeas( XYView *view )
{
  view->Clear();
  view->SetRightName( tr( "I0" ) );
  view->SetLeftName( tr( "mu(E)" ) );
  view->SetXName( tr( "Energy" ) );
  view->SetXUnitName( tr( "[keV]" ) );
  view->makeValid( true );
}

bool MainWindow::CheckDetectorSelection( void )
{
  int NoOfSelectedSens = 0;

  MeasFileType = EXTRA;    // 実質 AUX タイプは無い

  if ( UseI1->isChecked() ) NoOfSelectedSens++;
  if ( Use19chSSD->isChecked() ) NoOfSelectedSens++;
  if ( UseAux1->isChecked() ) NoOfSelectedSens++;
  if ( UseAux2->isChecked() ) NoOfSelectedSens++;

  if ( NoOfSelectedSens == 0 )  // I0 以外に一つはセンサが選ばれていなければダメ
    return false;

  if ( NoOfSelectedSens == 1 ) {  // 選ばれたのが一個だけの場合、モードが決まる
    if ( UseI1->isChecked() ) {
      AUnit *as = ASensors.value( SelectI1->currentIndex() );
      if (( as->getType() == "CNT" )||( as->getType() == "CNT2" )
	  ||( as->getType() == "OTC" )||( as->getType() == "OTC2" )) {
	MeasFileType = TRANS;
      }  // その他は EXTRA
    }
    if ( Use19chSSD->isChecked() ) {
      MeasFileType = FLUO;
    }
    if ( UseAux1->isChecked() ) {
      if ( MeasDispMode[ MC_AUX1 ] == TRANS ) {
	MeasFileType = TRANS;
      } else {
	MeasFileType = EXTRA;
      }
    }
    if ( UseAux2->isChecked() ) {
      if ( MeasDispMode[ MC_AUX2 ] == TRANS ) {
	MeasFileType = TRANS;
      } else {
	MeasFileType = EXTRA;
      }
    }
  }

  return true;
}

void MainWindow::StartMeasurement( void )
{
  AUnit *as;

  if ( inMeas == 0 ) {           // 既に測定が進行中でなければ
    if ( MMainTh->isBusy() ) {   // 分光器が回ってたらダメ
      statusbar->showMessage( tr( "Monochro is moving!" ), 2000 );
      return;
    }
    if ( ! MMainTh->isEnable() ) {   // 分光器の制御系が繋がってなかったらダメ
      QString msg = tr( "Scan cannot Start : (%1) is disabled" )
	.arg( MMainTh->getName() );
      statusbar->showMessage( msg, 2000 );
      NewLogMsg( msg );
    }

    if ( ( TP <= 0 ) || ( TT0 <= 0 ) ) {   // 測定点数等ブロック指定がおかしかったらダメ
      statusbar->showMessage( tr( "Invalid block data." ), 2000 );
      return;
    }
    if ( GetDFName0() == 0 ) {  // データファイルが選択されていなかったらダメ
      statusbar->showMessage( tr( "Data File is not Selected!" ), 2000 );
      return;
    }
    if ( CheckDetectorSelection() == false ) { // I0 以外に一つは選ばれてないとダメ
      statusbar->showMessage( tr( "Detectors are not selected properly!" ), 2000 );
      return;
    }
    if ( ( MeasViewC = SetUpNewView( XYVIEW ) ) == NULL ) {
      // グラフ表示領域が確保できないとダメ
      return;
    }
    MeasViewC->setNowDType( MEASDATA );
    MeasView = (XYView*)(MeasViewC->getView());
    ClearXViewScreenForMeas( MeasView );

    bool OneOfSensIsRangeSelectable = false;
    QString theNames = "";
    int LC = 0;
    mUnits.clearUnits();
    for ( int i = 0; i < MCHANNELS; i++ )
      MeasSensF[i] = false;

    MeasSensF[ LC ] = true;
    MeasDispMode[ LC ] = TRANS;     // I0 にモードはないのでダミー
    MeasDispPol[ LC ] = 1;          // polarity +
    mUnits.addUnit( ASensors.value( SelectI0->currentIndex() ) );
    LC++;
    if ( MeasSensF[ LC ] = UseI1->isChecked() ) {
      MeasDispMode[ LC ] = TRANS;     // I1 は TRANS に固定
      MeasDispPol[ LC ] = 1;          // polarity +
      mUnits.addUnit( ASensors.value( SelectI1->currentIndex() ) );
      LC++;
    }
    if ( MeasSensF[ LC ] = Use19chSSD->isChecked() ) {
      MeasDispMode[ LC ] = FLUO;      // SSD は FLUO に固定
      MeasDispPol[ LC ] = 1;          // polarity +
      mUnits.addUnit( SFluo );
      LC++;
    }
    if ( MeasSensF[ LC ] = UseAux1->isChecked() ) {
      MeasDispMode[ LC ] = ( ModeA1->currentIndex() == 0 ) ? FLUO : TRANS;
      MeasDispPol[ LC ] = ( ModeA1->currentIndex() == 2 ) ? -1 : 1;
      mUnits.addUnit( ASensors.value( SelectAux1->currentIndex() ) );
      LC++;
    }
    if ( MeasSensF[ LC ] = UseAux2->isChecked() ) {
      MeasDispMode[ LC ] = ( ModeA2->currentIndex() == 0 ) ? FLUO : TRANS;
      MeasDispPol[ LC ] = ( ModeA1->currentIndex() == 2 ) ? -1 : 1;
      mUnits.addUnit( ASensors.value( SelectAux2->currentIndex() ) );
      LC++;
    }

    for ( int i = 0; i < mUnits.count(); i++ ) {
      as = mUnits.at(i);
      if ( ! as->isEnable() ) { // 指定されたセンサーが Stars 経由で生きていないとダメ
	QString msg = tr( "Scan cannot Start : (%1) is disabled" ).arg( as->getName() );
	statusbar->showMessage( msg, 2000 );
	NewLogMsg( msg );
	return;
      }
      if ( as->isRangeSelectable() ) {
	if ( ! as->isAutoRange() ) {
	  OneOfSensIsRangeSelectable = true;
	  theNames += " [" + as->getName() + "]";
	}
      }
    }

    // CNT2, OTC2 はカウンタの向こうに Keithley が繋がってる。
    // CNT2, OTC2 では Keithley をレンジ固定で、直接ではオートレンジで使うので
    // 両方を同時には測定に使えない
    for ( int i = 0; i < mUnits.count(); i++ ) {
      if (( mUnits.at(i)->getType() == "CNT2" )||( mUnits.at(i)->getType() == "OTC2" )) {
	for ( int j = 0; j < mUnits.count(); j++ ) {
	  if ( mUnits.at(i)->get2ndUid() == mUnits.at(j)->getUid() ) {
	    QString msg = tr( "Selected sensors [%1] and [%2] are conflicting." )
	      .arg( mUnits.at(i)->getName() )
	      .arg( mUnits.at(j)->getName() );
	    statusbar->showMessage( msg, 2000 );
	    NewLogMsg( msg );
	    return;
	  }
	}
      }
    }
    if ( OneOfSensIsRangeSelectable ) { // レンジ設定が必要なセンサが選ばれていたら
                                        // 設定済みかどうか確認する (測定開始をブロック)
      MakeSureOfRangeSelect
	->setText( tr( "The Sensor(s)%1 should be range selected.\n"
		       "Have you selected the range in 'Setup Condition'" )
		   .arg( theNames ) );
      MakeSureOfRangeSelect->show();
      MakingSureOfRangeSelect = true;
    } else {
      MakingSureOfRangeSelect = false;
    }

    if ( MeasBackBeforeMeas->isChecked() ) {// 測定前にバックグラウンド測定指定があった
      if ( ! MeasureDark() )                // 正常に測れなければだめ
	return;
    }
    Offsets.clear();
    for ( int i = 0; i < mUnits.count(); i++ ) {
      Offsets << mUnits.at(i)->getDark();
    }

    QFileInfo CheckFile( DFName0 + ".dat" );  // 必要なら測定ファイルの上書き確認
    if ( ! OverWriteChecked && CheckFile.exists() ) {
      AskOverWrite
	->setText( tr( "File [%1] Over Write ?" )
			     .arg( DFName0 + ".dat" ) );
      AskOverWrite->show();
      AskingOverwrite = true;
    } else {
      AskingOverwrite = false;
    }

    NewLogMsg( tr( "Meas: Start (%1 keV)" ).arg( CurPosKeV ) );
    InitialKeV = CurPosKeV;
    inMeas = 1;
    MeasStart->setText( tr( "Stop" ) );
    MeasStart->setStyleSheet( "background-color: yellow" );
    MeasPause->setEnabled( true );
    
    MeasChNo = mUnits.count();         // 測定のチャンネル数
    if ( Use19chSSD->isChecked() )
      MeasChNo += 18; // 19ch SSD を使う場合、上では 1つと数えているので 18 追加

    MeasView->SetRLine( 0 );
    MeasView->SetLLine( 1 );
    MeasView->SetLR( 0, RIGHT_AX );
    MeasView->SetScaleType( 0, I0TYPE );
    MeasView->SetLineName( 0, mUnits.at(0)->getName() );
    for ( int i = 1; i < mUnits.count(); i++ ) {
      MeasView->SetLR( i, LEFT_AX );
      MeasView->SetScaleType( i, FULLSCALE );
      MeasView->SetLineName( i, mUnits.at(i)->getName() );
    }

    CpBlock2SBlock();
    MeasStage = 0;
    //    ClearMeasView();
    MeasViewC->setIsDeletable( false );
    MeasTimer->start( 100 );
  } else {
    StopP->show();
    SinPause = inPause;
    NewLogMsg( tr( "Meas: Break (%1 keV)" ).arg( CurPosKeV ) );
    inPause = 1;
    MeasPause->setText( tr( "Resume" ) );
    MeasPause->setStyleSheet( "background-color: yellow" );
    MeasPause->setEnabled( false );
    MeasStart->setEnabled( false );
    MeasViewC->setIsDeletable( true );
  }
}

void MainWindow::SurelyStop( void )
{
  if ( inMeasDark ) {
    MeasDarkTimer->stop();
    inMeasDark = false;
    statusbar->showMessage( "", 0 );
    MeasBackGround->setText( tr( "Measure Background" ) );
    MeasBackGround
      ->setStyleSheet( "background-color: "
		       "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 "
		       "rgba(225, 235, 225, 255), stop:1 "
		       "rgba(255, 255, 255, 255));" );
    MeasDarkStage = 0;
  }
  NewLogMsg( tr( "Meas: Stopped (%1 keV)" ).arg( CurPosKeV ) );
  statusbar->showMessage( tr( "The Measurement is Stopped" ), 4000 );
  MeasTimer->stop();
  inMeas = 0;
  MeasStart->setText( tr( "Start" ) );
  MeasStart
    ->setStyleSheet( "background-color: "
		     "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 "
		     "rgba(225, 235, 225, 255), stop:1 "
		     "rgba(255, 255, 255, 255));" );
  MeasStart->setEnabled( true );
  inPause = 0;
  MeasPause->setText( tr( "Pause" ) );
  MeasPause
    ->setStyleSheet( "background-color: "
		     "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 "
		     "rgba(225, 235, 225, 255), stop:1 "
		     "rgba(255, 255, 255, 255));" );
  MeasPause->setEnabled( false );
  if ( OnFinishP->currentIndex() == (int)RETURN ) {
    MoveCurThPosKeV( InitialKeV );
  }
}

void MainWindow::GoingOn( void )
{
  MeasStart->setEnabled( true );
  MeasPause->setEnabled( true );
  if ( SinPause == 1 ) {
    NewLogMsg( tr( "Meas: Pausing (%1 keV)" ).arg( CurPosKeV ) );
    inPause = 1;
  } else {
    NewLogMsg( tr( "Measu: Resume (%1 keV)" ).arg( CurPosKeV ) );
    inPause = 0;
    MeasPause->setText( tr( "Pause" ) );
    MeasPause
      ->setStyleSheet( "background-color: "
		       "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 "
		       "rgba(225, 235, 225, 255), stop:1 "
		       "rgba(255, 255, 255, 255));" );
  }
}

void MainWindow::CpBlock2SBlock( void )
{
  SBlocks = Blocks;
  SBLKUnit = BLKUnit;
  for ( int i = 0; i < MaxBLKs; i++ ) {
    SBlockStart[i] = BlockStart[i];
    SBlockStep[i] = BLKstep[i]->text().toDouble();
    SBlockPoints[i] = BlockPoints[i];
    SBlockDwell[i] = BlockDwell[i];
  }
}

void MainWindow::PauseMeasurement( void )
{
  if ( inPause == 0 ) {
    NewLogMsg( tr( "Meas: Pause (%1 keV)" ).arg( CurPosKeV ) );
    inPause = 1;
    MeasPause->setText( tr( "Resume" ) );
    MeasPause->setStyleSheet( "background-color: yellow" );
  } else {
    NewLogMsg( tr( "Meas: Resume (%1 keV)" ).arg( CurPosKeV ) );
    inPause = 0;
    MeasPause->setText( tr( "Pause" ) );
    MeasPause
      ->setStyleSheet( "background-color: "
		       "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 "
		       "rgba(225, 235, 225, 255), stop:1 "
		       "rgba(255, 255, 255, 255));" );
  }
}

void MainWindow::OkOverWrite( void )
{
  AskingOverwrite = false;
}

void MainWindow::RangeSelOK( void )
{
  MakingSureOfRangeSelect = false;
}
