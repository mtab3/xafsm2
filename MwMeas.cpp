#include <QtGui>

#include "MainWindow.h"

void MainWindow::setupMeasArea( void )   /* 測定エリア */
/* setupSetupArea が先に実行されているというのが前提 */
{
  BLKstart << BLKs01 << BLKs02 << BLKs03 << BLKs04 << BLKs05
	   << BLKs06 << BLKs07 << BLKs08 << BLKs09;
  BLKstep  << BLKstep01 << BLKstep02 << BLKstep03 << BLKstep04 << BLKstep05
	   << BLKstep06 << BLKstep07 << BLKstep08;
  BLKdwell << BLKdwell01 << BLKdwell02 << BLKdwell03 << BLKdwell04 << BLKdwell05
	   << BLKdwell06 << BLKdwell07 << BLKdwell08;
  BLKpoints << BLKpoints01 << BLKpoints02 << BLKpoints03 << BLKpoints04 << BLKpoints05
	    << BLKpoints06 << BLKpoints07 << BLKpoints08;
  BLKlabels << BLKL01 << BLKL02 << BLKL03 << BLKL04 << BLKL05
	    << BLKL06 << BLKL07 << BLKL08 << BLKL09 << BLKLAll;

  if ( SFluo == NULL ) 
    Use19chSSD->setEnabled( false );

  BLKUnit = (UNIT)DefaultUnit;
  //  ClearBLKs();
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
  SelectedOrgName.clear();
  DFName00.clear();
  AutoModeFirst = true;
  MovingToNewSamplePosition = false;
  AutoModeComment.clear();

  defaultFileName = QDateTime::currentDateTime().toString( "yyMMdd" ) + "-test.dat";
  EditDFName->setText( defaultFileName );
  ShowMeasFileStatus( defaultFileName );

  OnFinishP->addItem( tr( "Return" ) );
  OnFinishP->addItem( tr( "Stay" ) );
  OnFinishP->setCurrentIndex( RETURN );

  QPushButton *tmpB;
  TP = 0;
  TT0 = 0;
  EstimatedMeasurementTimeInSec = 0;
  inMeas = false;
  inPause = false;
  MeasStage = false;
  FixedPositionMode = false;
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

  SetUpSensorComboBoxes();

  ModeA1->addItem( "A1/I0" );
  ModeA1->addItem( "log(I0/A1)" );
  ModeA1->addItem( "log(-I0/A1)" );
  ModeA1->addItem( "log(I1/A1)" );
  ModeA1->addItem( "log(-I1/A1)" );

  ModeA2->addItem( "A2/I0" );
  ModeA2->addItem( "log(I0/A2)" );
  ModeA2->addItem( "log(-I0/A2)" );
  ModeA2->addItem( "log(I1/A2)" );
  ModeA2->addItem( "log(-I1/A2)" );
  ModeA2->addItem( "log(A1/A2)" );
  ModeA2->addItem( "log(-A1/A2)" );

  I0Range->setEnabled( false );
  I1Range->setEnabled( false );
  A1Range->setEnabled( false );
  A2Range->setEnabled( false );
  newSensSelectedForI0( SelectI0->currentIndex() );
  newSensSelectedForI1( SelectI1->currentIndex() );
  newSensSelectedForA1( SelectAux1->currentIndex() );
  newSensSelectedForA2( SelectAux2->currentIndex() );

  connect( SelectI0, SIGNAL( currentIndexChanged( int ) ),
	   this, SLOT( newSensSelectedForI0( int ) ) );
  connect( SelectI1, SIGNAL( currentIndexChanged( int ) ),
	   this, SLOT( newSensSelectedForI1( int ) ) );
  connect( SelectAux1, SIGNAL( currentIndexChanged( int ) ),
	   this, SLOT( newSensSelectedForA1( int ) ) );
  connect( SelectAux2, SIGNAL( currentIndexChanged( int ) ),
	   this, SLOT( newSensSelectedForA2( int ) ) );
  connect( I0Range, SIGNAL( valueChanged( int ) ), this, SLOT( newI0Range( int ) ) );
  connect( I1Range, SIGNAL( valueChanged( int ) ), this, SLOT( newI1Range( int ) ) );
  connect( A1Range, SIGNAL( valueChanged( int ) ), this, SLOT( newA1Range( int ) ) );
  connect( A2Range, SIGNAL( valueChanged( int ) ), this, SLOT( newA2Range( int ) ) );

  for ( int i = 0; i < BLKstart.count(); i++ ) {
    connect( BLKstart[i], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKparams()) );
  }
  for ( int i = 0; i < BLKstep.count(); i++ ) {
    connect( BLKstep[i], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKparams()) );
    connect( BLKdwell[i], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKparams()));
    connect( BLKpoints[i], SIGNAL( editingFinished() ), this, SLOT(ChangeBLKpoints()) );
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

  connect( EditDFName, SIGNAL( editingFinished() ),
	   this, SLOT( CheckNewMeasFileName() ) );
  connect( SelDFName, SIGNAL( clicked() ), SelDFND, SLOT( show() ) );
  connect( SelDFND, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( SelectedNDFN( const QString & ) ) );

  connect( MeasStart, SIGNAL( clicked() ), this, SLOT( AutoMeasurement() ) );
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

  MeasView = NULL;

  connect( this, SIGNAL( SelectedSSD( int, bool ) ),
	      this, SLOT( ReCalcSSDTotal( int, bool ) ) );

  darkTable = new DarkTable;
  connect( ShowMeasuredBack, SIGNAL( clicked() ), this, SLOT( ShowMB() ) );

  int i0 = 0, i1 = 0;
  for ( int i = 0; i < ICLengths.count(); i++ ) {
    I0ChSelect->addItem( ICLengths[i]->Name );
    I1ChSelect->addItem( ICLengths[i]->Name );
    if ( ICLengths[i]->ID == "I0" )
      i0 = i;
    if ( ICLengths[i]->ID == "I1" )
      i1 = i;
  }
  I0ChSelect->setCurrentIndex( i0 );
  I1ChSelect->setCurrentIndex( i1 );
  SetNewGases();
  connect( I0ChSelect, SIGNAL( currentIndexChanged( int ) ),
	   this, SLOT( SetNewGases() ) );
  connect( I1ChSelect, SIGNAL( currentIndexChanged( int ) ),
	   this, SLOT( SetNewGases() ) );
  connect( ManTEkeV, SIGNAL( textChanged( const QString & ) ),
	   this, SLOT( SetNewGases() ) );

  connect( QXafsMode, SIGNAL( toggled( bool ) ), this, SLOT( ToggleQXafsMode( bool ) ) );

  // Auto mode   :  The parse can be done, on starting measurement
  connect( AutoMode, SIGNAL( editingFinished() ),
	   this, SLOT( ShowItemsForAutoMode() ) );
}

void MainWindow::CheckNewMeasFileName( void )
{
  ShowMeasFileStatus( EditDFName->text() );
}

void MainWindow::ShowMeasFileStatus( QString fname )
{
  QFileInfo InitialF( fname );  // デフォルトの測定ファイルの存在
  if ( InitialF.exists() || fname.simplified().isEmpty() ) {
    EditDFName->setStyleSheet( FSTATCOLORS[ NEW ][ OLD ] );  // data-new, name-old
  } else {
    EditDFName->setStyleSheet( FSTATCOLORS[ NEW ][ NEW ] );  // data-new, name-new
  }
}

void MainWindow::SetUpSensorComboBoxes( void )
{
  bool findQXafsI0, findQXafsI1;
  findQXafsI0 = findQXafsI1 = false;

  while ( I0Sensors.count() > 0 ) I0Sensors.remove( 0 );
  while ( I1Sensors.count() > 0 ) I1Sensors.remove( 0 );
  while ( A1Sensors.count() > 0 ) A1Sensors.remove( 0 );
  while ( A2Sensors.count() > 0 ) A2Sensors.remove( 0 );
  while ( SelectI0->count() > 0 ) SelectI0->removeItem( 0 );
  while ( SelectI1->count() > 0 ) SelectI1->removeItem( 0 );
  while ( SelectAux1->count() > 0 ) SelectAux1->removeItem( 0 );
  while ( SelectAux2->count() > 0 ) SelectAux2->removeItem( 0 );

  for ( int i = 0; i < ASensors.count(); i++ ) {
    if ( ASensors[i]->getID() == "QXAFS-I0" ) findQXafsI0 = true;
    if ( ASensors[i]->getID() == "QXAFS-I1" ) findQXafsI1 = true;

    bool addOk = false;
    QStringList CheckList = ( QXafsMode->isChecked() ) ? QXafsOk : NXafsOk;
    QString type = ASensors[i]->getType();
    for ( int j = 0; j < CheckList.count(); j++ ) {
      if ( CheckList[j] == type ) {
	addOk = true;
	break;
      }
    }
    if ( addOk ) {
      QString name = ASensors.value(i)->getName();
      
      SelectI0->addItem( name );  I0Sensors << ASensors[i];
      SelectI1->addItem( name );  I1Sensors << ASensors[i];
      if ( ASensors.at(i) != SFluo ) {
	SelectAux1->addItem( name );   A1Sensors << ASensors[i];
      }
      if ( ASensors.at(i) != SFluo ) {
	SelectAux2->addItem( name );   A2Sensors << ASensors[i];
      }
      
      if ( ASensors.value(i)->getID() == "I0" )
	SelectI0->setCurrentIndex( i );
      if ( ASensors.value(i)->getID() == "I1" )
	SelectI1->setCurrentIndex( i );
      if ( ASensors.value(i)->getID() == "Aux1" )
	SelectAux1->setCurrentIndex( i );
      if ( ASensors.value(i)->getID() == "Aux2" )
	SelectAux2->setCurrentIndex( i );
    }
  }
  UseI1->setChecked( true );
  if ( (!findQXafsI0)||(!findQXafsI1) ) {
    isQXafsModeAvailable = false;
    if ( ! isQXafsModeAvailable ) {
      QXafsMode->setChecked( false );
      QXafsMode->setEnabled( false );
    }
  }
}


void MainWindow::newSensSelectedForI0( int index )
{
  bool found = false;
  for ( int i = 0; i < SensWithRange.count(); i++ ) {
    if (( I0Sensors[ index ] == SensWithRange[i] )
	&&( !I0Sensors[ index ]->isAutoRange() )) {
      int nowR = SensWithRange[i]->getRange();
      found = true;
      I0Range->setEnabled( true );
      I0Range->setRange( SensWithRange[i]->getRangeL(),
			 SensWithRange[i]->getRangeU() );
      SensWithRange[i]->setRange( nowR );
      I0Range->setValue( SensWithRange[i]->getRange() );
      break;
    }
  }
  if ( !found ) {
    I0Range->setEnabled( false );
    I0Range->setRange( 0, 0 );
    I0Range->setValue( 0 );
  }
}

void MainWindow::newSensSelectedForI1( int index )
{
  bool found = false;
  for ( int i = 0; i < SensWithRange.count(); i++ ) {
    if (( I1Sensors[ index ] == SensWithRange[i] )
	&&( !I1Sensors[ index ]->isAutoRange() )) {
      int nowR = SensWithRange[i]->getRange();
      found = true;
      I1Range->setEnabled( true );
      I1Range->setRange( SensWithRange.at(i)->getRangeL(),
			 SensWithRange.at(i)->getRangeU() );
      SensWithRange[i]->setRange( nowR );
      I1Range->setValue( SensWithRange.at(i)->getRange() );
      break;
    }
  }
  if ( !found ) {
    I1Range->setEnabled( false );
    I1Range->setRange( 0, 0 );
    I1Range->setValue( 0 );
  }
}

void MainWindow::newSensSelectedForA1( int index )
{
  bool found = false;
  for ( int i = 0; i < SensWithRange.count(); i++ ) {
    if (( A1Sensors[ index ] == SensWithRange[i] )
	&&( !A1Sensors[ index ]->isAutoRange() )) {
      int nowR = SensWithRange[i]->getRange();
      found = true;
      A1Range->setEnabled( true );
      A1Range->setRange( SensWithRange.at(i)->getRangeL(),
			 SensWithRange.at(i)->getRangeU() );
      SensWithRange[i]->setRange( nowR );
      A1Range->setValue( SensWithRange.at(i)->getRange() );
      break;
    }
  }
  if ( !found ) {
    A1Range->setEnabled( false );
    A1Range->setRange( 0, 0 );
    A1Range->setValue( 0 );
  }
}

void MainWindow::newSensSelectedForA2( int index )
{
  bool found = false;
  for ( int i = 0; i < SensWithRange.count(); i++ ) {
    if (( A2Sensors[ index ] == SensWithRange[i] )
	&&( !A2Sensors[ index ]->isAutoRange() )) {
      int nowR = SensWithRange[i]->getRange();
      found = true;
      A2Range->setEnabled( true );
      A2Range->setRange( SensWithRange.at(i)->getRangeL(),
			 SensWithRange.at(i)->getRangeU() );
      SensWithRange[i]->setRange( nowR );
      A2Range->setValue( SensWithRange.at(i)->getRange() );
      break;
    }
  }
  if ( !found ) {
    A2Range->setEnabled( false );
    A2Range->setRange( 0, 0 );
    A2Range->setValue( 0 );
  }
}

void MainWindow::newI0Range( int newR )
{
  I0Sensors[ SelectI0->currentIndex() ]->setRange( newR );
  if ( SensWithRange[ SelSensToSetRange->currentIndex() ] 
       == I0Sensors[ SelectI0->currentIndex() ] ) {
    RangeSelect->setValue( newR );
  }
}

void MainWindow::newI1Range( int newR )
{
  I1Sensors[ SelectI1->currentIndex() ]->setRange( newR );
  if ( SensWithRange[ SelSensToSetRange->currentIndex() ] 
       == I1Sensors[ SelectI1->currentIndex() ] ) {
    RangeSelect->setValue( newR );
  }
}

void MainWindow::newA1Range( int newR )
{
  A1Sensors[ SelectAux1->currentIndex() ]->setRange( newR );
  if ( SensWithRange[ SelSensToSetRange->currentIndex() ] 
       == A1Sensors[ SelectAux1->currentIndex() ] ) {
    RangeSelect->setValue( newR );
  }
}

void MainWindow::newA2Range( int newR )
{
  A2Sensors[ SelectAux2->currentIndex() ]->setRange( newR );
  if ( SensWithRange[ SelSensToSetRange->currentIndex() ] 
       == A2Sensors[ SelectAux2->currentIndex() ] ) {
    RangeSelect->setValue( newR );
  }
}


void MainWindow::SetNewGases( void )
{
  I0Recommend->clear();
  double trans, near = 100;
  int Rec = 0;
  for ( int i = 0; i < Gases.count(); i++ ) {
    double mut = calcMuT( I0ChSelect->currentIndex(), i, ManTEkeV->text().toDouble() );
    trans = exp( -mut );
    I0Recommend
      ->addItem( QString( "%1 :  %2" ).arg( Gases[i]->Name ).arg( trans, 5, 'f', 3 ) );
    if ( fabs( trans - 0.9 ) < near ) {
      near = fabs( trans - 0.9 );
      Rec = i;
    }
  }
  I0Recommend->setCurrentIndex( Rec );
  near = 100;
  Rec = 0;
  I1Recommend->clear();
  for ( int i = 0; i < Gases.count(); i++ ) {
    double mut = calcMuT( I1ChSelect->currentIndex(), i, ManTEkeV->text().toDouble() );
    trans = exp( -mut );
    I1Recommend
      ->addItem( QString( "%1: %2" ).arg( Gases[i]->Name ).arg( trans, 5, 'f', 3 ) );
    if ( fabs( trans - 0.1 ) < near ) {
      near = fabs( trans - 0.1 );
      Rec = i;
    }
  }
  I1Recommend->setCurrentIndex( Rec );
}

void MainWindow::ShowMB( void )
{
  QTableWidgetItem *item;

  darkTable->clearItems();

  darkTable->setRowCol( ASensors.count(), 2 );
  for ( int i = 0; i < ASensors.count(); i++ ) {
    item = new QTableWidgetItem ( ASensors.at( i )->getName() );
    darkTable->setItem( i, 0, item );
    item = new QTableWidgetItem ( QString::number( ASensors.at( i )->getDark() ) );
    item->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
    darkTable->setItem( i, 1, item );
  }
  darkTable->show();
}

void MainWindow::ChangeBLKUnit( int unit )
{
  QString buf;
  double s;
  int i;

  for ( i = 0; i < MaxBLKs + 1; i++ ) {
    s = u->keV2any( (UNIT)unit, u->any2keV( BLKUnit, BLKstart[i]->text().toDouble() ) );
    if ( ! QXafsMode->isChecked() ) {
      buf.sprintf( UnitName[unit].form, s );
      BLKstart[i]->setText( buf );
    } else {
      BLKstart[i]->setText( QString::number( s ) );
    }
  }
  for ( i = 0; i < MaxBLKs; i++ ) {
    BLKstep[i]->setText( "0" );
  }

  BLKUnit = (UNIT)unit;
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
  QString buf;
  double Eg = ManTEkeV->text().toDouble();

  if ( QXafsMode->isChecked() ) {

    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, Eg - 0.30 ) );
    BLKstart[0]->setText( buf );
    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, Eg + 1.20 ) );
    BLKstart[1]->setText( buf );
    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, 0.0 ) );
    for ( int i = 2; i < MaxBLKs+1; i++ )
      BLKstart[i]->setText( buf );
    
    buf.sprintf( "% 4d", 600 );
    BLKpoints[0]->setText( buf );
    buf.sprintf( "% 4d", 0 );
    for ( int i = 1; i < MaxBLKs; i++ )
      BLKpoints[i]->setText( buf );

    // dwell の設定は後
    buf.sprintf( "% 5.2f", 0.0 );
    for ( int i = 1; i < MaxBLKs; i++ )
      BLKdwell[i]->setText( buf );

  } else {
    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, Eg - 0.30 ) );
    BLKstart[0]->setText( buf );
    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, Eg - 0.05 ) );
    BLKstart[1]->setText( buf );
    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, Eg + 0.10 ) );
    BLKstart[2]->setText( buf );
    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, Eg + 0.50 ) );
    BLKstart[3]->setText( buf );
    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, Eg + 1.20 ) );
    BLKstart[4]->setText( buf );
    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, 0.0 ) );
    for ( int i = 5; i < MaxBLKs+1; i++ )
      BLKstart[i]->setText( buf );

    buf.sprintf( "% 4d", 0 );
    for ( int i = 0; i < MaxBLKs; i++ )
      BLKstep[i]->setText( buf );

    buf.sprintf( "% 4d",  70 ); BLKpoints[0]->setText( buf );
    buf.sprintf( "% 4d", 150 ); BLKpoints[1]->setText( buf );
    buf.sprintf( "% 4d", 160 ); BLKpoints[2]->setText( buf );
    buf.sprintf( "% 4d", 100 ); BLKpoints[3]->setText( buf );
    buf.sprintf( "% 4d",   0 ); 
    for ( int i = 4; i < MaxBLKs; i++ )
      BLKpoints[i]->setText( buf );
    
    buf.sprintf( "% 5.2f", 1.0 );
    BLKdwell[0]->setText( buf );
    BLKdwell[1]->setText( buf );
    BLKdwell[2]->setText( buf );
    BLKdwell[3]->setText( buf );
    buf.sprintf( "% 5.2f", 0.0 );
    for ( int i = 4; i < MaxBLKs; i++ )
      BLKdwell[i]->setText( buf );
    
    ChangeBLKs( 4 );
  }
  ShowBLKs();

  if ( QXafsMode->isChecked() ) {
    CheckQXafsParams();     // dwell は最小時間にセットされる
    ShowBLKs();
  }
}

void MainWindow::SetStdXAFSBLKs( void )
{
  QString buf;
  double Eg = ManTEkeV->text().toDouble();
    
  if ( QXafsMode->isChecked() ) {   // QXAFS

    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, Eg - 0.30 ) );
    BLKstart[0]->setText( buf );
    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, Eg + 1.10 ) );
    BLKstart[1]->setText( buf );
    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, 0 ) );
    for ( int i = 2; i < MaxBLKs+1; i++ )
      BLKstart[i]->setText( buf );

    buf.sprintf( "% 4d", 0 );
    for ( int i = 0; i < MaxBLKs; i++ )
      BLKstep[i]->setText( buf );

    buf.sprintf( "% 4d", 600 );
    BLKpoints[0]->setText( buf );
    buf.sprintf( "% 4d", 0 );
    for ( int i = 1; i < MaxBLKs; i++ )
      BLKpoints[i]->setText( buf );

    // dwell の設定は後
    buf.sprintf( "% 5.2f", 0.0 );
    for ( int i = 1; i < MaxBLKs; i++ )
      BLKdwell[i]->setText( buf );

  } else {                          // Normal XAFS
    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, Eg - 0.30 ) );
    BLKstart[0]->setText( buf );
    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, Eg - 0.04 ) );
    BLKstart[1]->setText( buf );
    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, Eg + 0.05 ) );
    BLKstart[2]->setText( buf );
    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, Eg + 0.50 ) );
    BLKstart[3]->setText( buf );
    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, Eg + 1.10 ) );
    BLKstart[4]->setText( buf );
    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, 0.0 ) );
    for ( int i = 5; i < MaxBLKs+1; i++ )
      BLKstart[i]->setText( buf );

    buf.sprintf( "% 4d", 0 );
    for ( int i = 0; i < MaxBLKs; i++ )
      BLKstep[i]->setText( buf );

    buf.sprintf( "% 4d",  40 ); BLKpoints[0]->setText( buf );
    buf.sprintf( "% 4d", 300 ); BLKpoints[1]->setText( buf );
    buf.sprintf( "% 4d", 180 ); BLKpoints[2]->setText( buf );
    buf.sprintf( "% 4d", 100 ); BLKpoints[3]->setText( buf );
    buf.sprintf( "% 4d", 0 );
    for ( int i = 4; i < MaxBLKs; i++ )
      BLKpoints[i]->setText( buf );

    buf.sprintf( "% 5.2f", 1.0 );
    BLKdwell[0]->setText( buf );
    BLKdwell[1]->setText( buf );
    BLKdwell[2]->setText( buf );
    BLKdwell[3]->setText( buf );
    buf.sprintf( "% 5.2f", 0.0 );
    for ( int i = 4; i < MaxBLKs; i++ )
      BLKdwell[i]->setText( buf );
    
    ChangeBLKs( 4 );
  }
  ShowBLKs();

  if ( QXafsMode->isChecked() ) {
    CheckQXafsParams();       // dwell が可能な最短にセットされる
    ShowBLKs();
  }
}

void MainWindow::SetStdXANESBLKs( void )
{
  QString buf;
  double Eg = ManTEkeV->text().toDouble();

  buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, Eg - 0.03 ) );
  BLKstart[0]->setText( buf );
  buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, Eg + 0.07 ) );
  BLKstart[1]->setText( buf );
  buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, 0 ) );
  for ( int i = 2; i < MaxBLKs+1; i++ )
    BLKstart[i]->setText( buf );

  buf.sprintf( "% 4d", 0 );
  for ( int i = 0; i < MaxBLKs; i++ )
    BLKstep[i]->setText( buf );
  
  buf.sprintf( "% 4d", 300 );
  BLKpoints[0]->setText( buf );
  buf.sprintf( "% 4d", 0 );
  for ( int i = 1; i < MaxBLKs; i++ )
    BLKpoints[i]->setText( buf );

  if ( ! QXafsMode->isChecked() ) {
    buf.sprintf( "% 5.2f", 1.0 );
    BLKdwell[0]->setText( buf );
    buf.sprintf( "% 5.2f", 0.0 );
    for ( int i = 1; i < MaxBLKs; i++ )
      BLKdwell[i]->setText( buf );
  }    

  ChangeBLKs( 1 );
  ShowBLKs();

  if ( QXafsMode->isChecked() ) {
    CheckQXafsParams();   // dwell が最短時間にセットされる
    ShowBLKs();
  }
}

void MainWindow::ShowBLKs( void )
{
  double step, width;
  int points;
  QString buf;
  int i;

  // ブロック指定の各行について
  for ( i = 0; i < MaxBLKs; i++ ) {
    // 区間の幅と刻幅を信じて、点数を計算し直す。
    width = fabs( BLKstart[i+1]->text().toDouble() - BLKstart[i]->text().toDouble() );
    step = BLKstep[i]->text().toDouble();
    points = BLKpoints[i]->text().toInt();
    if ( step != 0 ) {
      buf.sprintf( "% 4d", points = (int)( abs( width / step ) + 0.5) );
      BLKpoints[i]->setText( buf );
      if ( points > 0 )
	if ( !QXafsMode->isChecked() ) {
	  buf.sprintf( UnitName[ BLKUnit ].form, width / points );
	} else {
	  BLKstep[i]->setText( QString::number( width / points ) );
	}
    } else {	// もし刻幅が 0 の場合
      // 点数が正の数なら、逆に区間幅と点数から刻幅を計算する
      if ( points > 0 ) {
	step = width / points;
	if ( !QXafsMode->isChecked() ) {
	  buf.sprintf( UnitName[ BLKUnit ].form, step );
	  BLKstep[i]->setText( buf );
	} else {
	  BLKstep[i]->setText( QString::number( step ) );
	}
      } else {
	BLKstep[i]->setText( "0" );
      }
    }

    // Noraml XAFS の場合は、表示のフォーマットを直す
    buf.sprintf( "% 4d", BLKpoints[i]->text().toInt() );
    BLKpoints[i]->setText( buf );
    if ( ! QXafsMode->isChecked() ) {
      buf.sprintf( UnitName[ BLKUnit ].form, BLKstart[i]->text().toDouble() );
      BLKstart[i]->setText( buf );
      buf.sprintf( "% 5.2f", BLKdwell[i]->text().toDouble() );
      BLKdwell[i]->setText( buf );
    }
  }
  buf.sprintf( UnitName[ BLKUnit ].form, BLKstart[i]->text().toDouble() );
  BLKstart[i]->setText( buf );

  ShowTotal();
}


void MainWindow::NewRpt( void )
{
  ShowTotal();
}

void MainWindow::ShowTotal( void )  // ShowBlock の中からと、反復回数変更時に呼ばれる
{
  QString buf;

  if ( ! QXafsMode->isChecked() ) {  // 通常モード
    TP = 0;                     // 測定の合計点数と、単純積算時間を数える
    TT0 = 0;
    for ( int i = 0; i < Blocks; i++ ) {
      TP += BLKpoints[i]->text().toInt();
      TT0 += BLKpoints[i]->text().toInt() * BLKdwell[i]->text().toDouble();
    } 

    double TT = TT0 + TP * 360. / 480.;    // Cu-Ka で 480点測定に6分余分にかかる?
    buf.sprintf( "%4d", TP * SelRPT->value() );
    TPoints->setText( tr( "Points: " ) + buf );
    TT *= SelRPT->value();
    EstimatedMeasurementTimeInSec = TT;
    int Th = (int)( TT / 3600 );
    TT -= Th * 3600;
    int Tm = (int)( TT / 60 );
    TT -= Tm * 60;
    int Ts = (int)TT;
    TT -= Ts;
    //  buf.sprintf( "%02d:%02d:%02d.%02d", Th, Tm, Ts, (int)(TT*100) );
    buf.sprintf( "%02d:%02d:%02d", Th, Tm, Ts );  // 秒以下の精度は不要
    TTime->setText( tr( "Time: " ) + buf );
  } else {    // QXAFS モード
    //    SelRPT->value() * ( ( 
    TP = BLKpoints[0]->text().toInt();        // 測定の合計点数と、単純積算時間を数える
    TT0 = BLKdwell[0]->text().toDouble();     //  

    //    RunUpTime = ( HSpeed - LowSpeed ) * RunUpRate / 1000;  // HSpeed までの加速にかかる時間
  }
}

void MainWindow::ChangeBLKparams( void )
{
  if ( QXafsMode->isChecked() ) CheckQXafsParams();
  ShowBLKs();
}

void MainWindow::ChangeBLKpoints( void )
{
  int points;
  double step, width;
  QString buf;

  for ( int i = 0; i < BLKpoints.count(); i++ ) {
    if ( BLKpoints[i] == sender() ) {
      width = BLKstart[i+1]->text().toDouble() - BLKstart[i]->text().toDouble();
      points = abs( BLKpoints[i]->text().toDouble() );
      buf.sprintf( "% 4d", points );
      BLKpoints[i]->setText( buf );
      if ( points > 0 ) {
	step = width / points;
	if ( !QXafsMode->isChecked() ) {
	  buf.sprintf( UnitName[BLKUnit].form, step );
	  BLKstep[i]->setText( buf );
	} else {
	  BLKstep[i]->setText( QString::number( step ) );
	}
      }
    }
  }
  if ( QXafsMode->isChecked() ) CheckQXafsParams();
  ShowTotal();
}

void MainWindow::SetDwells( void )
{
  for ( int i = 0; i < Blocks; i++ ) {
    BLKdwell[i]->setText( DwellAll->text() );
  }
  if ( QXafsMode->isChecked() ) CheckQXafsParams();
  ShowBLKs();
}

void MainWindow::SelectedNDFN( const QString &fname )
{
  EditDFName->setText( fname );   // ここではファイル名をセットするだけ。
                                  // Start 時に書き出す。
  SelectedOrgName = fname;
  ShowMeasFileStatus( fname );
}

void MainWindow::SelectedWBFN( const QString &fname )
{
  QFile f( fname );
  if ( !f.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;

  int i;
  QTextStream out( &f );

  // 一見、行の並びは自由に見えるが、B より後に U が来ると
  // (かつ、その時の表示単位と U での単位が違っていると)
  // B で読み込んだ値を 表示単位から U 指定単位に変換してしまうため
  // おかしくなる。
  // 従って必ず、U は B より先でないとダメ
  out << "N " << Blocks << endl;
  out << "U " << BLKUnit << endl;
  for ( i = 0; i < MaxBLKs; i++ ) {
    out << "B " << i << " " << BLKstart[i]->text() << " "
	<< BLKpoints[i]->text() << " " << BLKdwell[i]->text() << endl;
  }
  out << "B " << i << " " << BLKstart[i]->text() << " " << 0 << " " << 0 << endl;

  f.close();
}

void MainWindow::SelectedRBFN( const QString &fname )
{
  QFile f( fname );
  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) )
    return;

  QTextStream in( &f );

  // 一見、行の並びは自由に見えるが、B より後に U が来ると
  // (かつ、その時の表示単位と U での単位が違っていると)
  // B で読み込んだ値を 表示単位から U 指定単位に変換してしまうため
  // おかしくなる。
  // 従って必ず、U は B より先でないとダメ
  QRegExp sep("\\s+");
  while (!in.atEnd()) {
    int i;
    QString line = in.readLine();

    if ( line[0] == QChar( 'N' ) ) {
      Blocks = line.section( sep, 1, 1 ).toInt();
      SelBLKs->setValue( Blocks );
    }
    if ( line[0] == QChar( 'U' ) ) {
      BLKUnit = (UNIT)line.section( sep, 1, 1 ).toInt();
      SelBLKUnit->setCurrentIndex( BLKUnit );
    }
    if ( line[0] == QChar( 'B' ) ) {
      i = line.section( sep, 1, 1 ).toInt();
      if ( i < MaxBLKs + 1 )
	BLKstart[i]->setText( line.section( sep, 2, 2 ) );
      if ( i < MaxBLKs ) {
	BLKstep[i]->setText( "0" );
	BLKpoints[i]->setText( line.section( sep, 3, 3 ) );
	BLKdwell[i]->setText( line.section( sep, 4, 4 ) );
      }
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

  if ( NoOfSelectedSens == 0 ) { // I0 以外に一つはセンサが選ばれていなければダメ
    statusbar->showMessage( tr( "At least 1 detector should be selected, except I0." ),
			    2000 );
    return false;
  }

  if ( NoOfSelectedSens == 1 ) {  // 選ばれたのが一個だけの場合、モードが決まる
    if ( UseI1->isChecked() ) {
      AUnit *as = I1Sensors[ SelectI1->currentIndex() ];
      if (( as->getType() == "CNT" )||( as->getType() == "CNT2" )
          ||( as->getType() == "OTC" )||( as->getType() == "OTC2" )
          ||( as->getType() == "DV" )||( as->getType() == "DV2" )) {
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

  if ( UseAux1->isChecked() ) {
    if ( ( ModeA1->currentIndex() == 3 )||( ModeA1->currentIndex() == 4 ) )
      if ( ! UseI1->isChecked() ) {
	statusbar->showMessage( tr( "I1 should be selected for the measurement." ),
				2000 );
	return false;
      }
  }
  if ( UseAux2->isChecked() ) {
    if ( ( ModeA2->currentIndex() == 3 )||( ModeA2->currentIndex() == 4 ) )
      if ( ! UseI1->isChecked() ) {
	statusbar->showMessage( tr( "I1 should be selected for the measurement." ),
				2000 );
	return false;
      }
    if ( ( ModeA2->currentIndex() == 5 )||( ModeA2->currentIndex() == 6 ) )
      if ( ! UseAux1->isChecked() ) {
	statusbar->showMessage( tr( "Aux1 should be selected for the measurement." ),
				2000 );
	return false;
      }
  }

  return true;
}

void MainWindow::StartMeasurement( void )
{
  // ・測定対象の検出器
  // ・表示されるもの
  // ・ファイルに記録するもの
  // という 3 つの似てるけど違うものがあるF
  // 例えば、I0 と 19ch-SSD で測定する場合
  // ・測定対象の検出器       : I0, SFluo (19ch を束ねた検出器)  : mUnits で管理
  // ・表示するもの           : I0, Total (19ch の合計), 各チャンネル(19個)
  //                 元ネタになるデータそのものは mUnits で管理されてる
  //                 SFluo が何番目に来るかだけ記録しておけば後は、
  //                 ちょっとした例外処理でなんとかなりそうな気がする
  // ・ファイルに記録するもの : I0, 各チャンネル(19個)
  //                 元ネタになるデータそのものは mUnits で管理されてる
  //                 mUnits を流用し SFluo を 19 チャンネルに展開することで対応できてる
  // 
  AUnit *as;

  // 将来の変更
  // ノーマル XAFS の時、使用する検出器には ノーマル XAFS OK のフラグが立ってるもの
  // だけが選べるようにする。

  if ( !inMeas ) {           // 既に測定が進行中でなければ
    EncOrPM = ( ( SelThEncorder->isChecked() ) ? XENC : XPM );
    SFluoLine = -1;
    isSFluo = isSI1 = false;

    QString User;
    if ( ( User = UUnits.isTheUnitInUse( MMainTh ) ) != "" ) {
      // 分光器が他のことに使われたらダメ
      statusbar->showMessage( tr( "Monochro is used by the process %1!" )
			      .arg( User ), 2000 );
      return;
    }
    if ( MMainTh->isBusy() ) {   // 分光器が回ってたらダメ
      statusbar->showMessage( tr( "Monochro is moving!" ), 2000 );
      return;
    }
    if ( ! MMainTh->isEnable() ) {   // 分光器の制御系が繋がってなかったらダメ
      statusbar->showMessage( tr( "Meas cannot Start : (%1) is disabled" )
			      .arg( MMainTh->getName() ), 2000 );
    }
    if ( ! CheckBlockRange() ) {  // ブロック指定のエネルギーレンジが範囲外だったらダメ
      statusbar->showMessage( "The block parameter is out of range.", 2000 );
      return;
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
      return;
    }
    if ( Use19chSSD->isChecked() ) {   // 19ch 使うときは MCA の測定中はダメ
      if ( inMCAMeas ) {
        QString msg = tr( "Meas cannot Start : in MCA measurement" );
        statusbar->showMessage( msg, 2000 );
        NewLogMsg( msg );
        return;
      }
    }

    if ( QXafsMode->isChecked() ) {     // QXafs モードの時の追加チェック
      if ( BLKpoints[0]->text().toInt() > 9990 ) {    // 測定点数が 9990 を超えてたらダメ
        statusbar->showMessage( tr( "Measured points are too many.  "
                                    "It should be less than 9990 in QXAFS mode." ),
                                2000 );
        return;
      }
      if ( ! UseI1->isChecked() ) {     // 今 QXafs は透過専用なので、I1 は必須
        statusbar->showMessage( tr( "I1 must be selected for QXAFS" ), 2000 );
        return;
      }
      if ( Use19chSSD->isChecked() ) {     // 今 QXafs は透過専用なので、SSDは使えない
        statusbar->showMessage( tr( "19ch SSD can not be used for QXAFS" ), 2000 );
        return;
      }
#if 0
      if ( UseAux1->isChecked() || UseAux2->isChecked() ) {
        // 今 QXafs で AUX は使えない
        statusbar
            ->showMessage( tr( "Aux1 and 2 can not be used for QXAFS" ), 2000 );
      }
      // これは将来変える 「Q mode 可能」というフラグが立ってれば OKにする
      if ( I0Sensors[ SelectI0->currentIndex() ]->getID() != "QXAFS-I0" ) {
        statusbar
            ->showMessage( tr( "Selected I0 Sensor can not be used for QXAFS" ), 2000 );
        return;
      }
      // これは将来変える 「Q mode 可能」というフラグが立ってれば OKにする
      if ( I1Sensors[ SelectI1->currentIndex() ]->getID() != "QXAFS-I1" ) {
        statusbar
            ->showMessage( tr( "Selected I1 Sensor can not be used for QXAFS" ), 2000 );
        return;
      }
#endif

#if 0   // 多分不要 (QXAFS と AutoMode は両立するはず)
    } else if ( AutoModeButton->isChecked() ) { // Auto mode
      if ( QXafsMode->isChecked() ) {
        statusbar->showMessage( tr( "Auto mode cannot be used with QXAFS mode" ),
                                2000 );
        return;
      }
#endif

    } else {   // Normal モード時専用のチェック
      int TotalPoints = 0;
      for ( int i = 0; i < Blocks; i++ ) {
        TotalPoints += BLKpoints[i]->text().toInt();
      }
      if ( TotalPoints > 1999 ) {
        statusbar->showMessage( tr( "Measured points are too many.    "
                                "It should be less than 2000 in normal XAFS mode." ) );
        return;
      }
    }

    // この下の諸々諸々諸々諸々諸々諸々諸々諸々諸々諸々諸々諸々の設定が
    // QXAFS の時も必要かどうか、逆に QXAFS に必要な設定が全部できてるかは
    // 要確認

    bool OneOfSensIsRangeSelectable = false;
    QString theNames = "";
    int LC = 0;    // mUnits に登録するユニットに対応したカウント
    //    int DLC = 0;   // 表示するラインに対応したカウント
    mUnits.clearUnits();
    clearGSBs();              // ボタンの表示をクリア
    aGSBS aGsb;
    QVector<aGSBS> GSBSs;

    SvSelRealTime = SelRealTime->isChecked();
    SvSelLiveTime = SelLiveTime->isChecked();
    SvSelExtPattern = SelExtPattern->isChecked();

    MeasDispMode[ LC ] = I0;        // I0 にモードはないのでダミー
    MeasDispPol[ LC ] = 1;          // polarity +
    mUnits.addUnit( I0Sensors[ SelectI0->currentIndex() ] );
    LC++; 
    aGsb.stat = PBTrue; aGsb.label = "I0"; GSBSs << aGsb;

    if ( UseI1->isChecked() ) {
      MeasDispMode[ LC ] = TRANS;     // I1 は TRANS に固定
      MeasDispPol[ LC ] = 1;          // polarity +
      mUnits.addUnit( I1Sensors[ SelectI1->currentIndex() ] );
      LC++;
      isSI1 = true;
      aGsb.stat = PBFalse; aGsb.label = tr( "I1" ); GSBSs << aGsb;
      aGsb.stat = PBTrue;  aGsb.label = tr( "mu" ); GSBSs << aGsb;
    }
    if ( Use19chSSD->isChecked() ) {
      MeasDispMode[ LC ] = FLUO;      // SSD は FLUO に固定
      MeasDispPol[ LC ] = 1;          // polarity +
      mUnits.addUnit( SFluo );
      LC++;
      isSFluo = true;
      SFluoLine = GSBSs.count();
      aGsb.stat = PBTrue;  aGsb.label = "FL"; GSBSs << aGsb;
      for ( int i = 0; i < MaxSSDs; i++ ) {
        aGsb.stat = PBFalse;
        aGsb.label = QString::number( i );
        GSBSs << aGsb;
      }
      SFluo->setSSDPresetType( "REAL" );   // SSD を使った XAFS 測定は強制的に Real Time
      SelRealTime->setChecked( true );
      SelLiveTime->setChecked( false );
    }
    if ( UseAux1->isChecked() ) {
      // 0 以外は全部 TRANS。ちょっと荒っぽい
      if ( ModeA1->currentIndex() == 0 ) { // 蛍光
	MeasDispMode[ LC ] =  FLUO;
      } else {  // 透過
	MeasDispMode[ LC ] = TRANS;
      }
      MeasDispPol[ LC ] = 1;
      if ( ModeA1->currentIndex() == 2 ) MeasDispPol[ LC ] = -1;
      if ( ModeA1->currentIndex() == 4 ) MeasDispPol[ LC ] = -1;
      mUnits.addUnit( A1Sensors[ SelectAux1->currentIndex() ] );
      LC++;
      aGsb.stat = PBFalse; aGsb.label = "A1"; GSBSs << aGsb;
      aGsb.stat = PBTrue;  aGsb.label = "mu2"; GSBSs << aGsb;
    }
    if ( UseAux2->isChecked() ) {
      // 0 以外は全部 TRANS。ちょっと荒っぽい
      if ( ModeA2->currentIndex() == 0 ) { // 蛍光
	MeasDispMode[ LC ] = FLUO;
      } else {
	MeasDispMode[ LC ] = TRANS;
      }
      MeasDispPol[ LC ] = 1;
      if ( ( ModeA2->currentIndex() == 2 )
	   ||( ModeA2->currentIndex() == 4 )
	   ||( ModeA2->currentIndex() == 6 ) )
	MeasDispPol[ LC ] = -1;
      mUnits.addUnit( A2Sensors[ SelectAux2->currentIndex() ] );
      LC++;
      aGsb.stat = PBFalse; aGsb.label = "A2";     GSBSs << aGsb;
      aGsb.stat = PBTrue;  aGsb.label = "mu3"; GSBSs << aGsb;
    }
    if ( QXafsMode->isChecked() ) {
      if ( Enc2 != NULL ) {
        mUnits.addUnit( Enc2 );
      }
      mUnits.setOneByOne( false );
      // 検出器(主に a34410a)の設定を一個ずつ順番にやるかどうか
      // 必要かもと思われたこともあったけど、結局、順番にやるモードを使ったことはない。
    }

    for ( int i = 0; i < mUnits.count(); i++ ) {
      as = mUnits.at(i);
      if ( ! theSensorIsAvailable( as ) ) {  // QXafs / NXafs モードで使えるかどうか
        QString msg;
        if ( QXafsMode->isChecked() ) {
          msg = tr( "The sensor [%1] can not use for the QXafs." ).arg( as->getName() );
        } else {
          msg = tr( "The sensor [%1] can not use for the Normal Xafs." )
              .arg( as->getName() );
        }
        statusbar->showMessage( msg, 2000 );
        NewLogMsg( msg );
        return;
      }

      if ( ( User = UUnits.isTheUnitInUse( as ) ) != "" ) {
	// 計測器が他のことに使われてたらダメ
	statusbar->showMessage( tr( "The Sensor [%1] is used by the process %2!" )
				.arg( as->getName() ).arg( User ), 2000 );
	return;
      }
      if ( ! as->isEnable() ) { // 指定されたセンサーが Stars 経由で生きていないとダメ
        QString msg = tr( "Meas cannot Start : (%1) is disabled" ).arg( as->getName() );
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

    // 同じユニットを2回使っていないか確認。
    for ( int i = 0; i < mUnits.count(); i++ ) {
      for ( int j = i+1; j < mUnits.count(); j++ ) {
	if ( mUnits.at(i) == mUnits.at(j) ) {
	  QString msg
	    = tr( "Identical sensor [%1:%2,%3:%4]"
		  "is used as different inputs, like I0 and I1." )
	    .arg( i )
	    .arg( mUnits.at(i)->getName() )
	    .arg( j )
	    .arg( mUnits.at(j)->getName() );
	  statusbar->showMessage( msg, 2000 );
            NewLogMsg( msg );
            return;
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

    MakingSureOfRangeSelect = false;

    if ( MeasBackBeforeMeas->isChecked() ) {// 測定前にバックグラウンド測定指定があった
      if ( ! MeasureDark() )                // 正常に測れなければだめ
        return;
    }

    if ( ( MeasViewC = SetUpNewView( XYVIEW ) ) == NULL ) {
      // グラフ表示領域が確保できないとダメ
      return;
    }
    if ( QXafsMode->isChecked() )
      ViewTab->setTabText( ViewTab->currentIndex(), "QXAFS" );
    else 
      ViewTab->setTabText( ViewTab->currentIndex(), "XAFS" );

    MeasViewC->setNowDType( MEASDATA );
    MeasView = (XYView*)(MeasViewC->getView());
    ClearXViewScreenForMeas( MeasView );
    if ( QXafsMode->isChecked() ) {
      MeasView->SetQXafsMode( true );
      if ( ! UseAux1->isChecked() ) {
	MeasView->SetGroupLines( 3 );
      } else {
	MeasView->SetGroupLines( 5 );
      }
    } else {
      MeasView->SetQXafsMode( false );
    }
    for ( int i = 0; i < GSBs.count(); i++ ) {
      MeasView->ChooseAG( i, GSBs[i]->isChecked() == PBTrue );
    }
    MeasViewC->setGSBStats( GSBSs );
    ShowButtonsForCurrentTab();

    if ( AutoModeFirst ) {  // AutoMode: off か AutoMode の 1回目に true
      BaseFile = QFileInfo( DFName0 + ".dat" );  // 必要なら測定ファイルの上書き確認
      if ( BaseFile.exists() ) {
        AskOverWrite->setText( tr( "File [%1] Over Write ?" )
                               .arg( DFName0 + ".dat" ) );
        AskOverWrite->show();
        AskingOverwrite = true;  // ここで出した確認ダイアログをクローズするときにクリア
      } else {
        AskingOverwrite = false;
      }
    }

    NewLogMsg( tr( "Meas: Start %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]" )
               .arg( u->deg2keV( SelectedCurPosDeg( XENC ) ) )
               .arg( SelectedCurPosDeg( XENC ) )
               .arg( u->deg2keV(SelectedCurPosDeg( XPM ) ) )
               .arg( SelectedCurPosDeg( XPM ) ) );
    InitialKeV = u->deg2keV( SelectedCurPosDeg( XPM ) ); // 戻る場所はパスモータの現在位置

    inMeas = true;
    MeasStart->setText( tr( "Stop" ) );
    MeasStart->setStyleSheet( InActive );
    MeasPause->setEnabled( true );
    
    MeasChNo = mUnits.count();         // 測定のチャンネル数
    // 19ch SSD を使う場合、上では 1つと数えているので 18 追加
    if ( Use19chSSD->isChecked() ) {
      MeasChNo += ( MaxSSDs -1 );
    }
    if ( QXafsMode->isChecked() && ( Enc2 != NULL ) ) {
      MeasChNo -= 1;
    }

    SetDispMeasModes();
    CpBlock2SBlock();
    if ( ( SBlocks == 1 ) && ( BLKpoints[0]->text().toInt() == 1 ) )
      FixedPositionMode = true;
    else
      FixedPositionMode = false;

    if ( isSFluo && RecordMCASpectra->isChecked() ) {
      mcaDir = QDir( BaseFile.canonicalPath() );
      mcaDir.mkdir( BaseFile.baseName() );
      mcaDir.cd( BaseFile.baseName() );
    }

    // 測定に使う MaitnTh と検出器の登録。
    // *************************************************************************
    // これ以降に XAFS 測定をやめるときは UUnits.clear() が必要。!!!!!!!!!!!!!!!
    // *************************************************************************
    UUnits.addUnit( MEAS_ID, MMainTh );
    for ( int i = 0; i < mUnits.count(); i++ ) {
      UUnits.addUnit( MEAS_ID, mUnits.at(i) );
    }

    StartTimeDisp->setText( QDateTime::currentDateTime().toString("yy.MM.dd hh:mm:ss") );
    NowTimeDisp->setText( QDateTime::currentDateTime().toString("yy.MM.dd hh:mm:ss") );
    EndTimeDisp->setText( QDateTime::currentDateTime()
                          .addSecs( EstimatedMeasurementTimeInSec )
                          .toString("yy.MM.dd hh:mm:ss") );
    MeasStage = 0;
    //    ClearMeasView();
    MeasViewC->setIsDeletable( false );
    MeasTimer->start( 30 );
  } else {
    StopP->show();
    SinPause = inPause;
    NewLogMsg( tr( "Meas: Break %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]" )
               .arg( u->deg2keV( SelectedCurPosDeg( XENC ) ) )
               .arg( SelectedCurPosDeg( XENC ) )
               .arg( u->deg2keV(SelectedCurPosDeg( XPM ) ) )
               .arg( SelectedCurPosDeg( XPM ) ) );
    inPause = true;
    MeasPause->setText( tr( "Resume" ) );
    MeasPause->setStyleSheet( InActive );
    MeasPause->setEnabled( false );
    MeasStart->setEnabled( false );
  }
}

// Ok リストに名前があるか  // 同じ関数が MultiUnit にもある !
bool MainWindow::CheckOkList( AUnit *as, QStringList OkList )
{
  int j;
  for ( j = 0; j < OkList.count(); j++ ) {
    if ( as->getType() == OkList[j] )
      break;
  }
  if ( j >= OkList.count() ) { // 指定されたセンサー type はリストにない
    return false;
  }
  return true;
}

// 現在のモード ( QXafs or NXafs ) で使えるか
bool MainWindow::theSensorIsAvailable( AUnit *as )
{
  if ( QXafsMode->isChecked() ) {
    return CheckOkList( as, QXafsOk );
  } else {
    return CheckOkList( as, NXafsOk );
  }
  return true;  // ここに来ることは無い
}

void MainWindow::SurelyStop( void )
{
  // the MeasA counter for auto mode is set to the last one.
  // If the measurement is stopped, ALL measurements will be skipped.
  MeasA = AutoModeParams.count();
  if ( inMeasDark ) {
    MeasDarkTimer->stop();
    inMeasDark = false;
    statusbar->showMessage( "", 0 );
    MeasBackGround->setText( tr( "Measure Background" ) );
    MeasBackGround->setStyleSheet( NormalB );
    MeasDarkStage = 0;
  }
  UUnits.clear( MEAS_ID );
  CheckNewMeasFileName();
  NewLogMsg( tr( "Meas: Stopped %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]" )
	     .arg( u->deg2keV( SelectedCurPosDeg( XENC ) ) )
	     .arg( SelectedCurPosDeg( XENC ) )
       .arg( u->deg2keV( SelectedCurPosDeg( XPM ) ) )
	     .arg( SelectedCurPosDeg( XPM ) ) );
  statusbar->showMessage( tr( "The Measurement is Stopped" ), 4000 );
  MeasTimer->stop();
  inMeas = false;
  MeasStart->setText( tr( "Start" ) );
  MeasStart->setStyleSheet( NormalB );
  MeasStart->setEnabled( true );
  inPause = false;
  MeasPause->setText( tr( "Pause" ) );
  MeasPause->setStyleSheet( NormalB );
  if ( QXafsMode->isChecked() )
    QXafsFinish0();
  onMeasFinishWorks();
}

void MainWindow::GoingOn( void )
{
  MeasStart->setEnabled( true );
  MeasPause->setEnabled( true );
  if ( SinPause ) {
    NewLogMsg( tr( "Meas: Pausing %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]" )
	       .arg( u->deg2keV( SelectedCurPosDeg( XENC ) ) )
	       .arg( SelectedCurPosDeg( XENC ) )
	       .arg( u->deg2keV(SelectedCurPosDeg( XPM ) ) )
	       .arg( SelectedCurPosDeg( XPM ) ) );
    inPause = true;
  } else {
    NewLogMsg( tr( "Meas: Resume %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]" )
	       .arg( u->deg2keV( SelectedCurPosDeg( XENC ) ) )
	       .arg( SelectedCurPosDeg( XENC ) )
	       .arg( u->deg2keV(SelectedCurPosDeg( XPM ) ) )
	       .arg( SelectedCurPosDeg( XPM ) ) );
    inPause = false;
    MeasPause->setText( tr( "Pause" ) );
    MeasPause->setStyleSheet( NormalB );
  }
}

void MainWindow::CpBlock2SBlock( void )
{
  SMeasInDeg = conds->isMeasInDeg();
  SBlocks = Blocks;
  SBLKUnit = BLKUnit;
  for ( int i = 0; i < MaxBLKs; i++ ) {
    SBlockStartAsDisp[i] = BLKstart[i]->text().toDouble();
    SBlockStartInDeg[i] = u->any2deg( BLKUnit, SBlockStartAsDisp[i] );
    SBlockStepAsDisp[i] = BLKstep[i]->text().toDouble();
    SBlockPoints[i] = BLKpoints[i]->text().toInt();
    SBlockDwell[i] = BLKdwell[i]->text().toDouble();
  }
  for ( int i = 0; i < MaxBLKs - 1; i++ ) {
    SBlockStepInDeg[i]
      = ( SBlockStartInDeg[i+1] - SBlockStartInDeg[i] ) / SBlockPoints[i];
  }
  SBlockStepInDeg[ MaxBLKs - 1 ] = 0;
}

bool MainWindow::CheckBlockRange( void )
{
  for ( int i = 0; i <= Blocks; i++ ) {
    if (( u->any2keV( BLKUnit, BLKstart[i]->text().toDouble())*1000 < MinEnergyInEV )
	||( u->any2keV( BLKUnit, BLKstart[i]->text().toDouble())*1000 > MaxEnergyInEV )) {
    statusbar
      ->showMessage( tr( "The block definitin [%1]eV is out of range. [%2]-[%3]eV" )
		     .arg( u->keV2any( EV, BLKstart[i]->text().toDouble() ) )
		     .arg( MinEnergyInEV )
		     .arg( MaxEnergyInEV ), 2000 );

      return false;
    }
  }
  return true;
}

void MainWindow::PauseMeasurement( void )
{
  if ( !inPause ) {
    NewLogMsg( tr( "Meas: Pause %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]" )
	       .arg( u->deg2keV( SelectedCurPosDeg( XENC ) ) )
	       .arg( SelectedCurPosDeg( XENC ) )
	       .arg( u->deg2keV(SelectedCurPosDeg( XPM ) ) )
	       .arg( SelectedCurPosDeg( XPM ) ) );
    inPause = true;
    MeasPause->setText( tr( "Resume" ) );
    MeasPause->setStyleSheet( InActive );
  } else {
    NewLogMsg( tr( "Meas: Resume %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]" )
	       .arg( u->deg2keV( SelectedCurPosDeg( XENC ) ) )
	       .arg( SelectedCurPosDeg( XENC ) )
	       .arg( u->deg2keV(SelectedCurPosDeg( XPM ) ) )
	       .arg( SelectedCurPosDeg( XPM ) ) );
    inPause = false;
    MeasPause->setText( tr( "Pause" ) );
    MeasPause->setStyleSheet( NormalB );
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


void MainWindow::ShowItemsForAutoMode( void )
{
  ParseAutoMode();
  ItemsForAutoMode->setText( QString::number( AutoModeParams.count() ) );
}

bool MainWindow::ParseAutoMode( void )
{
  AutoModeParams.clear();

  QStringList options;
  QString pLine = AutoMode->text();

  // "*" は特別な意味を持たせるので元の記号列に含まれていたらエラー
  if ( pLine.indexOf( "*" ) >= 0 )
    return false;
  // パラメータ列に含まれる "(", ")" ペアは別に切り出しておく。(代わりに "*" を置く)
  int s, e;
  while ( ( s = pLine.indexOf( "(" ) ) >= 0 ) {
    if ( ( e = pLine.indexOf( ")" ) ) < 0 )
      return false;
    options << pLine.mid( s+1, e-s-1 );
    pLine = pLine.left( s ) + "*" + pLine.mid( e+1 );
  }

  // 「,」(カンマ)と「 」(スペース、空白文字)の両方を区切りとして認める。
  // 全て、「 」空白1個に置き換える。
  QString parameter = pLine.replace( QRegExp( "[\\s,]" ), " " ).simplified();
  if ( parameter.isEmpty() )
    return false;

  QStringList prms = parameter.split( QRegExp( "[\\s]" ) );

  int start, end;
  double dx, dz;
  int p;
  AutoModeParam amp;
  for ( int i = 0; i < prms.count(); i++ ) {
    dx = dz = 0;
    if ( ( p = prms[i].indexOf( "*" ) ) >= 0 ) {
      // "*" がパラメータの末端以外にあったらエラー
      prms[i][p] = ' ';
      if ( p != ( prms[i].length() - 1 ) )
	return false;
      // オプション文字列が "," を含んでいなければエラー
      // "," が先頭でもエラー
      if ( ( p = options[0].indexOf( "," ) ) <= 0 )
	return false;
      // "," が末尾でもエラー
      if ( p == ( options[0].length() - 1 ) )
	return false;
      // "," が２つあってもエラーだけどそのチェックはまだしてない
      dx = options[0].left( p ).toDouble();
      dz = options[0].mid( p+1 ).toDouble();
      options.removeAt( 0 );
    }
    if ( prms[i].indexOf( "-" ) >= 0 ) {
      QStringList series = prms[i].split("-");
      if ( series.count() > 1 ) {                 // "-" だけ、はとりあえず無視
	start = ( series.at(0).isEmpty() ) ? 1 : series.at(0).toInt();
	end = ( series.at(1).isEmpty() ) ? 9 : series.at(1).toInt();
	int diff = abs( end - start );
	int sign = 1;
	if ( start > end ) sign = -1;
	for ( int j = 0; j < diff + 1; j++ ) {
	  amp.num = start + j * sign;
	  amp.dx = dx;
	  amp.dz = dz;
	  AutoModeParams << amp;
	}
      }
    } else {
      amp.num = prms[i].toInt();
      amp.dx = dx;
      amp.dz = dz;
      AutoModeParams << amp;
    }
  }

  return true;
}
