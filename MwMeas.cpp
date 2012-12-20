#include <QtGui>

#include "MainWindow.h"

void MainWindow::setupMeasArea( void )   /* 測定エリア */
{
  BLKstart << BLKs01 << BLKs02 << BLKs03 << BLKs04 << BLKs05 << BLKs06 << BLKs07;
  BLKstep  << BLKstep01 << BLKstep02 << BLKstep03 << BLKstep04 << BLKstep05 << BLKstep06;
  BLKdwell << BLKdwell01 << BLKdwell02 << BLKdwell03
	   << BLKdwell04 << BLKdwell05 << BLKdwell06;
  BLKpoints << BLKpoints01 << BLKpoints02 << BLKpoints03
	    << BLKpoints04 << BLKpoints05 << BLKpoints06;

  BLKUnit = KEV;
  ClearBLKs();
  ChangeBLKs( 4 );
  for ( int i = 0; i < UNITS; i++ ) {
    SelBLKUnit->addItem( QString( UnitName[i].name ) );
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

  EditDFName->setText( "test.dat" );

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
#if 0
  UseI1->setAutoExclusive( false );
  Use19chSSD->setAutoExclusive( false );
  UseAux1->setAutoExclusive( false );
  UseAux2->setAutoExclusive( false );
#endif

  ModeA1->addItem( "log(I0/A1)" );
  ModeA1->addItem( "A1/I0" );
  ModeA2->addItem( "log(I0/A2)" );
  ModeA2->addItem( "A2/I0" );
  
  connect( BLKstart[0], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKstart00()) );
  connect( BLKstart[1], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKstart01()) );
  connect( BLKstart[2], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKstart02()) );
  connect( BLKstart[3], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKstart03()) );
  connect( BLKstart[4], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKstart04()) );
  connect( BLKstart[5], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKstart05()) );
  connect( BLKstart[6], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKstart06()) );

  connect( BLKstep[0], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKstep00()) );
  connect( BLKstep[1], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKstep01()) );
  connect( BLKstep[2], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKstep02()) );
  connect( BLKstep[3], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKstep03()) );
  connect( BLKstep[4], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKstep04()) );
  connect( BLKstep[5], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKstep05()) );

  connect( BLKdwell[0], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKdwell00()) );
  connect( BLKdwell[1], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKdwell01()) );
  connect( BLKdwell[2], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKdwell02()) );
  connect( BLKdwell[3], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKdwell03()) );
  connect( BLKdwell[4], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKdwell04()) );
  connect( BLKdwell[5], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKdwell05()) );

  connect( BLKpoints[0], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKpoints00()) );
  connect( BLKpoints[1], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKpoints01()) );
  connect( BLKpoints[2], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKpoints02()) );
  connect( BLKpoints[3], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKpoints03()) );
  connect( BLKpoints[4], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKpoints04()) );
  connect( BLKpoints[5], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKpoints05()) );

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
#if 0
  connect( ModeTrans, SIGNAL( clicked() ), this, SLOT( Mode2Tr() ) );
  connect( ModeFluo, SIGNAL( clicked() ), this, SLOT( Mode2Fl() ) );
  connect( ModeAux, SIGNAL( clicked() ), this, SLOT( Mode2Au() ) );
#endif


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

void MainWindow::ChangeBLKstart00( void ) { ChangeBLKstart( 0 ); }
void MainWindow::ChangeBLKstart01( void ) { ChangeBLKstart( 1 ); }
void MainWindow::ChangeBLKstart02( void ) { ChangeBLKstart( 2 ); }
void MainWindow::ChangeBLKstart03( void ) { ChangeBLKstart( 3 ); }
void MainWindow::ChangeBLKstart04( void ) { ChangeBLKstart( 4 ); }
void MainWindow::ChangeBLKstart05( void ) { ChangeBLKstart( 5 ); }
void MainWindow::ChangeBLKstart06( void ) { ChangeBLKstart( 6 ); }

void MainWindow::ChangeBLKstart( int i )
{
  BlockStart[i] = any2keV( BLKUnit, BLKstart[i]->text().toDouble() );
  ShowBLKs();
}

void MainWindow::ChangeBLKstep00( void ) { ChangeBLKstep( 0 ); }
void MainWindow::ChangeBLKstep01( void ) { ChangeBLKstep( 1 ); }
void MainWindow::ChangeBLKstep02( void ) { ChangeBLKstep( 2 ); }
void MainWindow::ChangeBLKstep03( void ) { ChangeBLKstep( 3 ); }
void MainWindow::ChangeBLKstep04( void ) { ChangeBLKstep( 4 ); }
void MainWindow::ChangeBLKstep05( void ) { ChangeBLKstep( 5 ); }

void MainWindow::ChangeBLKstep( int i )
{
  double step;

  step = BLKstep[i]->text().toDouble();
  BlockPoints[i]
    = fabs(( keV2any(BLKUnit, BlockStart[i+1]) - keV2any(BLKUnit, BlockStart[i]) )
	   /step )+0.5;
  ShowBLKs();
}

void MainWindow::ChangeBLKpoints00( void ) { ChangeBLKpoints( 0 ); }
void MainWindow::ChangeBLKpoints01( void ) { ChangeBLKpoints( 1 ); }
void MainWindow::ChangeBLKpoints02( void ) { ChangeBLKpoints( 2 ); }
void MainWindow::ChangeBLKpoints03( void ) { ChangeBLKpoints( 3 ); }
void MainWindow::ChangeBLKpoints04( void ) { ChangeBLKpoints( 4 ); }
void MainWindow::ChangeBLKpoints05( void ) { ChangeBLKpoints( 5 ); }

void MainWindow::ChangeBLKpoints( int i )
{
  BlockPoints[i] = BLKpoints[i]->text().toDouble();
  ShowBLKs();
}

void MainWindow::ChangeBLKdwell00( void ) { ChangeBLKdwell( 0 ); }
void MainWindow::ChangeBLKdwell01( void ) { ChangeBLKdwell( 1 ); }
void MainWindow::ChangeBLKdwell02( void ) { ChangeBLKdwell( 2 ); }
void MainWindow::ChangeBLKdwell03( void ) { ChangeBLKdwell( 3 ); }
void MainWindow::ChangeBLKdwell04( void ) { ChangeBLKdwell( 4 ); }
void MainWindow::ChangeBLKdwell05( void ) { ChangeBLKdwell( 5 ); }

void MainWindow::ChangeBLKdwell( int i )
{
  BlockDwell[i] = BLKdwell[i]->text().toDouble();
  ShowBLKs();
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

void MainWindow::ClearNowView( void )
{
  NowView = new XView;
  int cTab = ViewTab->currentIndex();
  if ( nowViews[ cTab ] != NULL ) {
    ViewBases.at( cTab )->layout()->removeWidget( (QWidget *)nowViews[ cTab ] );
    deleteView( cTab );
    nowViews[ cTab ] = (void *)NULL;
  }
  ViewBases.at( cTab )->layout()->addWidget( NowView );
  nowViews[ cTab ] = (void *)NowView;

  NowView->Clear();
  NowView->SetSLines( 0, 1 );
  NowView->SetLineF( RIGHT, LEFT,
		     LEFT, LEFT, LEFT, LEFT, LEFT, LEFT, LEFT, LEFT, LEFT,
		     LEFT, LEFT, LEFT, LEFT, LEFT, LEFT, LEFT, LEFT, LEFT, LEFT );
  NowView->SetScaleT( I0TYPE, FULLSCALE );
  NowView->SetLName( 0, tr( "I0" ) );
  NowView->SetLName( 1, tr( "mu(E)" ) );
  NowView->SetXName( tr( "[keV]" ) );
  NowView->makeValid( true );
  NowView->SetGType( XYPLOT );
}

bool MainWindow::CheckDetectorSelection( void )
{
  int NoOfSelectedSens = 0;

  MeasFileType = EXTRA;    // 実質 AUX タイプは無い

  if ( UseI1->isChecked() ) NoOfSelectedSens++;
  if ( Use19chSSD->isChecked() ) NoOfSelectedSens++;
  if ( UseAux1->isChecked() ) NoOfSelectedSens++;
  if ( UseAux2->isChecked() ) NoOfSelectedSens++;

  if ( NoOfSelectedSens == 0 )
    return false;

  if ( NoOfSelectedSens == 1 ) {
    if ( ( UseI1->isChecked()
	   && ( ASensors.value( SelectI1->currentIndex() )->getType() == "CNT" ) )
	 || ( UseAux1->isChecked() && ( MeasDispMode[ MC_AUX1 ] == TRANS ) )
	 || ( UseAux2->isChecked() && ( MeasDispMode[ MC_AUX2 ] == TRANS ) ) )
      MeasFileType = TRANS;
    if ( Use19chSSD->isChecked() 
	 || ( UseAux1->isChecked() && ( MeasDispMode[ MC_AUX1 ] == FLUO ) )
	 || ( UseAux2->isChecked() && ( MeasDispMode[ MC_AUX2 ] == FLUO ) ) )
      MeasFileType = FLUO;
  }

  return true;
}

void MainWindow::StartMeasurement( void )
{
  if ( inMeas == 0 ) {
    if ( MMainTh->isBusy() ) {
      statusbar->showMessage( tr( "Monochro is moving!" ), 2000 );
      return;
    }
    if ( ( TP <= 0 ) || ( TT0 <= 0 ) ) {
      statusbar->showMessage( tr( "Invalid block data." ), 2000 );
      return;
    }
    if ( GetDFName0() == 0 ) {
      statusbar->showMessage( tr( "Data File is not Selected!" ), 2000 );
      return;
    }
    if ( CheckDetectorSelection() == false ) {
      statusbar->showMessage( tr( "Detectors are not selected properly!" ), 2000 );
      return;
    }

    QFileInfo CheckFile( DFName0 + ".dat" );
    if ( CheckFile.exists() ) {
      AskOverWrite->setText( tr( "<h1><center>File [%1] Over Write ?</center></h1>" )
			     .arg( DFName ) );
      AskOverWrite->show();
      AskingOverwrite = true;
    } else {
      AskingOverwrite = false;
    }

    int LC = 0;

    mUnits.clearUnits();
    for ( int i = 0; i < MCHANNELS; i++ )
      MeasSensF[i] = false;

    MeasSensF[ LC ] = true;
    MeasDispMode[ LC ] = TRANS;     // I0 にモードはないのでダミー
    mUnits.addUnit( ASensors.value( SelectI0->currentIndex() ), 0 );
    LC++;
    if ( MeasSensF[ LC ] = UseI1->isChecked() ) {
      MeasDispMode[ LC ] = TRANS;     // I1 は TRANS に固定
      mUnits.addUnit( ASensors.value( SelectI1->currentIndex() ), 0 );
      LC++;
    }
    if ( MeasSensF[ LC ] = Use19chSSD->isChecked() ) {
      MeasDispMode[ LC ] = FLUO;      // SSD は FLUO に固定
      mUnits.addUnit( SFluo, 0 );
      LC++;
    }
    if ( MeasSensF[ LC ] = UseAux1->isChecked() ) {
      MeasDispMode[ LC ] = ( ModeA1->currentIndex() == 0 ) ? TRANS : FLUO;
      mUnits.addUnit( ASensors.value( SelectAux1->currentIndex() ), 0 );
      LC++;
    }
    if ( MeasSensF[ LC ] = UseAux2->isChecked() ) {
      MeasDispMode[ LC ] = ( ModeA2->currentIndex() == 0 ) ? TRANS : FLUO;
      mUnits.addUnit( ASensors.value( SelectAux2->currentIndex() ), 0 );
      LC++;
    }

    NewLogMsg( QString( tr( "Meas: Start (%1 keV)\n" ) )
	       .arg( CurPosKeV ) );
    InitialKeV = CurPosKeV;
    inMeas = 1;
    MeasStart->setText( tr( "Stop" ) );
    MeasStart->setStyleSheet( "background-color: yellow" );
    MeasPause->setEnabled( true );
    
    MeasChNo = mUnits.count();         // 測定のチャンネル数
    if ( Use19chSSD->isChecked() )
      MeasChNo += 18; // 19ch SSD を使う場合、上では 1つと数えているので 18 追加

    CpBlock2SBlock();
    MeasStage = 0;
    ClearNowView();

    MeasTimer->start( 100 );
  } else {
    StopP->show();
    SinPause = inPause;
    NewLogMsg( QString( tr( "Meas: Break (%1 keV)\n" ) ).arg( CurPosKeV ) );
    inPause = 1;
    MeasPause->setText( tr( "Resume" ) );
    MeasPause->setStyleSheet( "background-color: yellow" );
    MeasPause->setEnabled( false );
    MeasStart->setEnabled( false );
  }
}

void MainWindow::SurelyStop( void )
{
  NewLogMsg( QString( tr( "Meas: Stopped (%1 keV)\n" ) ).arg( CurPosKeV ) );
  statusbar->showMessage( tr( "The Measurement is Stopped" ), 4000 );
  MeasTimer->stop();
  inMeas = 0;
  MeasStart->setText( tr( "Start" ) );
  MeasStart->setStyleSheet( "" );
  MeasStart->setEnabled( true );
  inPause = 0;
  MeasPause->setText( tr( "Pause" ) );
  MeasPause->setStyleSheet( "" );
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
    NewLogMsg( QString( tr( "Meas: Pausing (%1 keV)\n" ) ).arg( CurPosKeV ) );
    inPause = 1;
  } else {
    NewLogMsg( QString( tr( "Measu: Resume (%1 keV)\n" ) ).arg( CurPosKeV ) );
    inPause = 0;
    MeasPause->setText( tr( "Pause" ) );
    MeasPause->setStyleSheet( "" );
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
    NewLogMsg( QString( tr( "Meas: Pause (%1 keV)\n" ) ).arg( CurPosKeV ) );
    inPause = 1;
    MeasPause->setText( tr( "Resume" ) );
    MeasPause->setStyleSheet( "background-color: yellow" );
  } else {
    NewLogMsg( QString( tr( "Meas: Resume (%1 keV)\n" ) ).arg( CurPosKeV ) );
    inPause = 0;
    MeasPause->setText( tr( "Pause" ) );
    MeasPause->setStyleSheet( "" );
  }
}

void MainWindow::OkOverWrite( void )
{
  AskingOverwrite = false;
}
