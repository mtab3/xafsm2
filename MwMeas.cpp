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
  SSD << SSDE01 << SSDE02 << SSDE03 << SSDE04 << SSDE05
      << SSDE06 << SSDE07 << SSDE08 << SSDE09 << SSDE10
      << SSDE11 << SSDE12 << SSDE13 << SSDE14 << SSDE15
      << SSDE16 << SSDE17 << SSDE18 << SSDE19;

  BLKUnit = KEV;
  ClearBLKs();
  ChangeBLKs( 4 );
  for ( int i = 0; i < UNITS; i++ ) {
    SelBLKUnit->addItem( QString( UnitName[i].name ) );
  }
  SelBLKUnit->setCurrentIndex( BLKUnit );

  for ( int i = 0; i < MaxSSDs; i++ ) {
    SSD[i]->setStyleSheet( "background-color: #eeffee" );
    SSD[i]->setToolTip( tr( "Active" ) );
  }
  //  SetSSDactive( true );

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
  MeasID = 0;
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

  connect( SSD[0],  SIGNAL( clicked() ), this, SLOT( SelSSDs00() ) );
  connect( SSD[1],  SIGNAL( clicked() ), this, SLOT( SelSSDs01() ) );
  connect( SSD[2],  SIGNAL( clicked() ), this, SLOT( SelSSDs02() ) );
  connect( SSD[3],  SIGNAL( clicked() ), this, SLOT( SelSSDs03() ) );
  connect( SSD[4],  SIGNAL( clicked() ), this, SLOT( SelSSDs04() ) );
  connect( SSD[5],  SIGNAL( clicked() ), this, SLOT( SelSSDs05() ) );
  connect( SSD[6],  SIGNAL( clicked() ), this, SLOT( SelSSDs06() ) );
  connect( SSD[7],  SIGNAL( clicked() ), this, SLOT( SelSSDs07() ) );
  connect( SSD[8],  SIGNAL( clicked() ), this, SLOT( SelSSDs08() ) );
  connect( SSD[9],  SIGNAL( clicked() ), this, SLOT( SelSSDs09() ) );
  connect( SSD[10], SIGNAL( clicked() ), this, SLOT( SelSSDs10() ) );
  connect( SSD[11], SIGNAL( clicked() ), this, SLOT( SelSSDs11() ) );
  connect( SSD[12], SIGNAL( clicked() ), this, SLOT( SelSSDs12() ) );
  connect( SSD[13], SIGNAL( clicked() ), this, SLOT( SelSSDs13() ) );
  connect( SSD[14], SIGNAL( clicked() ), this, SLOT( SelSSDs14() ) );
  connect( SSD[15], SIGNAL( clicked() ), this, SLOT( SelSSDs15() ) );
  connect( SSD[16], SIGNAL( clicked() ), this, SLOT( SelSSDs16() ) );
  connect( SSD[17], SIGNAL( clicked() ), this, SLOT( SelSSDs17() ) );
  connect( SSD[18], SIGNAL( clicked() ), this, SLOT( SelSSDs18() ) );

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

  BlockStart[0] = Eg - 0.50;
  BlockStart[1] = Eg - 0.05;
  BlockStart[2] = Eg + 0.10;
  BlockStart[3] = Eg + 0.50;
  BlockStart[4] = Eg + 1.10;
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

#if 0
void MainWindow::Mode2Tr( void )
{
  MeasMode = TRANS;
  SetSSDactive( false );
}

void MainWindow::Mode2Fl( void )
{
  MeasMode = FLUO;
  SetSSDactive( true );
}

void MainWindow::Mode2Au( void )
{
  MeasMode = AUX;
  SetSSDactive( false );
}
#endif

#if 0
void MainWindow::SetSSDactive( bool active )
{
  for ( int i = 0; i < MaxSSDs; i++ ) {
    SSD[i]->setEnabled( active );
  }
}
#endif

void MainWindow::SelSSDs00( void ) { SelSSDs(  0 ); }
void MainWindow::SelSSDs01( void ) { SelSSDs(  1 ); }
void MainWindow::SelSSDs02( void ) { SelSSDs(  2 ); }
void MainWindow::SelSSDs03( void ) { SelSSDs(  3 ); }
void MainWindow::SelSSDs04( void ) { SelSSDs(  4 ); }
void MainWindow::SelSSDs05( void ) { SelSSDs(  5 ); }
void MainWindow::SelSSDs06( void ) { SelSSDs(  6 ); }
void MainWindow::SelSSDs07( void ) { SelSSDs(  7 ); }
void MainWindow::SelSSDs08( void ) { SelSSDs(  8 ); }
void MainWindow::SelSSDs09( void ) { SelSSDs(  9 ); }
void MainWindow::SelSSDs10( void ) { SelSSDs( 10 ); }
void MainWindow::SelSSDs11( void ) { SelSSDs( 11 ); }
void MainWindow::SelSSDs12( void ) { SelSSDs( 12 ); }
void MainWindow::SelSSDs13( void ) { SelSSDs( 13 ); }
void MainWindow::SelSSDs14( void ) { SelSSDs( 14 ); }
void MainWindow::SelSSDs15( void ) { SelSSDs( 15 ); }
void MainWindow::SelSSDs16( void ) { SelSSDs( 16 ); }
void MainWindow::SelSSDs17( void ) { SelSSDs( 17 ); }
void MainWindow::SelSSDs18( void ) { SelSSDs( 18 ); }

void MainWindow::SelSSDs( int i )
{
  if ( SSD[i]->isChecked() ) {
    SelectedSSD[i] = 0;
    SSD[i]->setStyleSheet( "background-color: #aabbaa" );
    SSD[i]->setToolTip( tr( "Inactive" ) );
  } else {
    SelectedSSD[i] = 1;
    SSD[i]->setStyleSheet( "background-color: #eeffee" );
    SSD[i]->setToolTip( tr( "Active" ) );
  }
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
  NowView = XViews[ ViewTab->currentIndex() ];
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
    if ( MMainTh->getIsBusy() ) {
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

    MeasDispMode[ MC_I0 ] = TRANS;     // I0 にモードはないのでダミー
    MeasDispMode[ MC_I1 ] = TRANS;     // I1 は TRANS に固定
    MeasDispMode[ MC_SSD ] = FLUO;      // SSD は FLUO に固定
    MeasDispMode[ MC_AUX1 ] = ( ModeA1->currentIndex() == 0 ) ? TRANS : FLUO;
    MeasDispMode[ MC_AUX2 ] = ( ModeA2->currentIndex() == 0 ) ? TRANS : FLUO;

    NewLogMsg( QString( tr( "Meas: Start (%1 keV)\n" ) )
	       .arg( CurPosKeV ) );
    InitialKeV = CurPosKeV;
    inMeas = 1;
    MeasStart->setText( tr( "Stop" ) );
    MeasStart->setStyleSheet( "background-color: yellow" );
    MeasPause->setEnabled( true );
    
    MeasSens[ MC_I0 ] = ASensors.value( SelectI0->currentIndex() );
    MeasSens[ MC_I1 ] = ASensors.value( SelectI1->currentIndex() );
    MeasSens[ MC_SSD ] = SFluo;
    MeasSens[ MC_AUX1 ] = ASensors.value( SelectAux1->currentIndex() );
    MeasSens[ MC_AUX2 ] = ASensors.value( SelectAux2->currentIndex() );
    
    for ( int i = 0; i < MCHANNELS; i++ )
      MeasSensF[i] = false;
    MeasSensF[ MC_I0 ] = true;
    MeasSensF[ MC_I1 ] = UseI1->isChecked();
    MeasSensF[ MC_SSD ] = Use19chSSD->isChecked();
    MeasSensF[ MC_AUX1 ] = UseAux1->isChecked();
    MeasSensF[ MC_AUX2 ] = UseAux2->isChecked();
    
    OneOfTheSensorIsCounter = false;   // 使おうとするディテクタの中にカウンタがあるか
    TheCounter = 0;       // そのデテクタの番号
    MeasChNo = 0;         // 測定のチャンネル数
    for ( int i = 0; i < 5; i++ ) {
      if ( MeasSensF[i] ) {
	MeasChNo++;
	if ( MeasSens[i]->getType() == "CNT" ) {
	  OneOfTheSensorIsCounter = true;
	  TheCounter = MeasSens[i];
	  break;
	}
      }
    }
    if ( MeasSensF[2] )
      MeasChNo += 18; // 19ch SSD を使う場合、上では 1つと数えているので 18 追加
    
    CpBlock2SBlock();
    MeasStage = 0;
    ClearNowView();
    MeasID = startTimer( 100 );
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
  killTimer( MeasID );
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
