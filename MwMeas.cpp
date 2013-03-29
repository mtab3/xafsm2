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
  EstimatedMeasurementTimeInSec = 0;
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
  UseI1->setChecked( true );


  ModeA1->addItem( "A1/I0" );
  ModeA1->addItem( "log(I0/A1)" );
  ModeA1->addItem( "log(-I0/A1)" );
  ModeA2->addItem( "A2/I0" );
  ModeA2->addItem( "log(I0/A2)" );
  ModeA2->addItem( "log(-I0/A2)" );

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
    connect( BLKstart.at(i), SIGNAL( editingFinished() ), this, SLOT(ChangeBLKstart()) );
  }
  for ( int i = 0; i < BLKstep.count(); i++ ) {
    connect( BLKstep.at(i), SIGNAL( editingFinished() ), this, SLOT(ChangeBLKstep()) );
  }
  for ( int i = 0; i < BLKdwell.count(); i++ ) {
    connect( BLKdwell.at(i), SIGNAL( editingFinished() ), this, SLOT( ChangeBLKdwell()));
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
    buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, BlockStart[i] ) );
    BLKstart[i]->setText( buf );
    if ( BlockPoints[i] > 0 ) {
      buf.sprintf( UnitName[ BLKUnit ].form,
	       ( u->keV2any(BLKUnit, BlockStart[i+1])
		 - u->keV2any(BLKUnit, BlockStart[i]) )
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
  buf.sprintf( UnitName[ BLKUnit ].form, u->keV2any( BLKUnit, BlockStart[i] ) );
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
  if ( SelRPT->value() > 30 )
    SelRPT->setValue( 30 );
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
}

void MainWindow::ChangeBLKstart( void )
{
  for ( int i = 0; i < BLKstart.count(); i++ ) {
    if ( BLKstart.at(i) == sender() ) {
      BlockStart[i] = u->any2keV( BLKUnit, BLKstart[i]->text().toDouble() );
      double step = BLKstep[i]->text().toDouble();
      if ( step != 0 ) {
	BlockPoints[i]
	  = fabs(( u->keV2any(BLKUnit, BlockStart[i+1])
		   - u->keV2any(BLKUnit, BlockStart[i]) )
		 /step )+0.5;
      }
      if ( i > 0 ) {
	double step = BLKstep[i-1]->text().toDouble();
	if ( step != 0 ) {
	  BlockPoints[i-1]
	    = fabs(( u->keV2any(BLKUnit, BlockStart[i])
		     - u->keV2any(BLKUnit, BlockStart[i-1]) )
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
	  = fabs(( u->keV2any(BLKUnit, BlockStart[i+1])
		   - u->keV2any(BLKUnit, BlockStart[i]) )
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
      AUnit *as = I1Sensors[ SelectI1->currentIndex() ];
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
  // ・測定対象の検出器
  // ・表示されるもの
  // ・ファイルに記録するもの
  // という 3 つの似てるけど違うものがある
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

  EncOrPM = ( ( SelThEncorder->isChecked() ) ? XENC : XPM );
  SFluoLine = -1;
  isSFluo = isSI1 = false;

  if ( inMeas == 0 ) {           // 既に測定が進行中でなければ
    if ( MMainTh->isBusy() ) {   // 分光器が回ってたらダメ
      statusbar->showMessage( tr( "Monochro is moving!" ), 2000 );
      return;
    }
    if ( ! MMainTh->isEnable() ) {   // 分光器の制御系が繋がってなかったらダメ
      statusbar->showMessage( tr( "Meas cannot Start : (%1) is disabled" )
			      .arg( MMainTh->getName() ), 2000 );
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

    if ( Use19chSSD->isChecked() ) {   // 19ch 使うときは MCA の測定中はダメ
      if ( inMCAMeas ) {
	QString msg = tr( "Meas cannot Start : in MCA measurement" );
	statusbar->showMessage( msg, 2000 );
	NewLogMsg( msg );
	return;
      }
    }

    bool OneOfSensIsRangeSelectable = false;
    QString theNames = "";
    int LC = 0;    // mUnits に登録するユニットに対応したカウント
    //    int DLC = 0;   // 表示するラインに対応したカウント
    mUnits.clearUnits();
    clearGSBs();              // ボタンの表示をクリア
    aGSBS aGsb;
    QVector<aGSBS> GSBSs;

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
      aGsb.stat = PBFalse; aGsb.label = "I1"; GSBSs << aGsb;
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
	aGsb.stat = PBFalse; aGsb.label = QString::number( i ); GSBSs << aGsb;
      }
    }
    if ( UseAux1->isChecked() ) {
      MeasDispMode[ LC ] = ( ModeA1->currentIndex() == 0 ) ? FLUO : TRANS;
      MeasDispPol[ LC ] = ( ModeA1->currentIndex() == 2 ) ? -1 : 1;
      mUnits.addUnit( A1Sensors[ SelectAux1->currentIndex() ] );
      LC++;
      aGsb.stat = PBTrue;  aGsb.label = "A1"; GSBSs << aGsb;
    }
    if ( UseAux2->isChecked() ) {
      MeasDispMode[ LC ] = ( ModeA2->currentIndex() == 0 ) ? FLUO : TRANS;
      MeasDispPol[ LC ] = ( ModeA1->currentIndex() == 2 ) ? -1 : 1;
      mUnits.addUnit( A2Sensors[ SelectAux2->currentIndex() ] );
      LC++;
      aGsb.stat = PBTrue;  aGsb.label = "A2"; GSBSs << aGsb;
    }

    for ( int i = 0; i < mUnits.count(); i++ ) {
      as = mUnits.at(i);
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

#if 0
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
#endif

    if ( ( MeasViewC = SetUpNewView( XYVIEW ) ) == NULL ) {
      // グラフ表示領域が確保できないとダメ
      return;
    }
    ViewTab->setTabText( ViewTab->currentIndex(), "XAFS" );
    MeasViewC->setNowDType( MEASDATA );
    MeasView = (XYView*)(MeasViewC->getView());
    ClearXViewScreenForMeas( MeasView );
    for ( int i = 0; i < GSBs.count(); i++ ) {
      MeasView->ChooseAG( i, GSBs[i]->isChecked() == PBTrue );
    }
    MeasViewC->setGSBStats( GSBSs );
    ShowButtonsForCurrentTab();

    BaseFile = QFileInfo( DFName0 + ".dat" );  // 必要なら測定ファイルの上書き確認
    if ( ! OverWriteChecked && BaseFile.exists() ) {
      AskOverWrite
	->setText( tr( "File [%1] Over Write ?" )
			     .arg( DFName0 + ".dat" ) );
      AskOverWrite->show();
      AskingOverwrite = true;
    } else {
      AskingOverwrite = false;
    }

    NewLogMsg( tr( "Meas: Start %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]" )
	       .arg( u->deg2keV( SelectedCurPosDeg( XENC ) ) )
	       .arg( SelectedCurPosDeg( XENC ) )
	       .arg( u->deg2keV(SelectedCurPosDeg( XPM ) ) )
	       .arg( SelectedCurPosDeg( XPM ) ) );
    InitialKeV = u->deg2keV( SelectedCurPosDeg( XPM ) ); // 戻る場所はパスモータの現在位置
    inMeas = 1;
    MeasStart->setText( tr( "Stop" ) );
    MeasStart->setStyleSheet( InActive );
    MeasPause->setEnabled( true );
    
    MeasChNo = mUnits.count();         // 測定のチャンネル数
    // 19ch SSD を使う場合、上では 1つと数えているので 18 追加
    if ( Use19chSSD->isChecked() ) {
      MeasChNo += ( MaxSSDs -1 );
    }
#if 0
    MeasView->SetRLine( 0 );
    MeasView->SetLLine( 1 );
    MeasView->SetLR( 0, RIGHT_AX );
    MeasView->SetScaleType( 0, I0TYPE );
    MeasView->SetLineName( 0, mUnits.at(0)->getName() );
    for ( int j = 1; j < mUnits.count(); j++ ) {
      MeasView->SetLR( j, LEFT_AX );
      MeasView->SetScaleType( j, FULLSCALE );
      MeasView->SetLineName( j, mUnits.at(j)->getName() );
    }
#endif
    SetDispMeasModes();
    CpBlock2SBlock();

    if ( isSFluo && RecordMCASpectra->isChecked() ) {
      mcaDir = QDir( BaseFile.canonicalPath() );
      mcaDir.mkdir( BaseFile.baseName() );
      mcaDir.cd( BaseFile.baseName() );
      qDebug() << "the place " << mcaDir.path();
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
    inPause = 1;
    MeasPause->setText( tr( "Resume" ) );
    MeasPause->setStyleSheet( InActive );
    MeasPause->setEnabled( false );
    MeasStart->setEnabled( false );
  }
}

void MainWindow::SurelyStop( void )
{
  if ( inMeasDark ) {
    MeasDarkTimer->stop();
    inMeasDark = false;
    statusbar->showMessage( "", 0 );
    MeasBackGround->setText( tr( "Measure Background" ) );
    MeasBackGround->setStyleSheet( NormalB );
    MeasDarkStage = 0;
  }
  NewLogMsg( tr( "Meas: Stopped %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]" )
	     .arg( u->deg2keV( SelectedCurPosDeg( XENC ) ) )
	     .arg( SelectedCurPosDeg( XENC ) )
	     .arg( u->deg2keV(SelectedCurPosDeg( XPM ) ) )
	     .arg( SelectedCurPosDeg( XPM ) ) );
  statusbar->showMessage( tr( "The Measurement is Stopped" ), 4000 );
  MeasTimer->stop();
  inMeas = 0;
  MeasStart->setText( tr( "Start" ) );
  MeasStart->setStyleSheet( NormalB );
  MeasStart->setEnabled( true );
  inPause = 0;
  MeasPause->setText( tr( "Pause" ) );
  MeasPause->setStyleSheet( NormalB );
  onMeasFinishWorks();
}

void MainWindow::GoingOn( void )
{
  MeasStart->setEnabled( true );
  MeasPause->setEnabled( true );
  if ( SinPause == 1 ) {
    NewLogMsg( tr( "Meas: Pausing %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]" )
	       .arg( u->deg2keV( SelectedCurPosDeg( XENC ) ) )
	       .arg( SelectedCurPosDeg( XENC ) )
	       .arg( u->deg2keV(SelectedCurPosDeg( XPM ) ) )
	       .arg( SelectedCurPosDeg( XPM ) ) );
    inPause = 1;
  } else {
    NewLogMsg( tr( "Meas: Resume %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]" )
	       .arg( u->deg2keV( SelectedCurPosDeg( XENC ) ) )
	       .arg( SelectedCurPosDeg( XENC ) )
	       .arg( u->deg2keV(SelectedCurPosDeg( XPM ) ) )
	       .arg( SelectedCurPosDeg( XPM ) ) );
    inPause = 0;
    MeasPause->setText( tr( "Pause" ) );
    MeasPause->setStyleSheet( NormalB );
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
    NewLogMsg( tr( "Meas: Pause %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]" )
	       .arg( u->deg2keV( SelectedCurPosDeg( XENC ) ) )
	       .arg( SelectedCurPosDeg( XENC ) )
	       .arg( u->deg2keV(SelectedCurPosDeg( XPM ) ) )
	       .arg( SelectedCurPosDeg( XPM ) ) );
    inPause = 1;
    MeasPause->setText( tr( "Resume" ) );
    MeasPause->setStyleSheet( InActive );
  } else {
    NewLogMsg( tr( "Meas: Resume %1 keV (%2 deg) [enc] %3 keV (%4 deg) [PM]" )
	       .arg( u->deg2keV( SelectedCurPosDeg( XENC ) ) )
	       .arg( SelectedCurPosDeg( XENC ) )
	       .arg( u->deg2keV(SelectedCurPosDeg( XPM ) ) )
	       .arg( SelectedCurPosDeg( XPM ) ) );
    inPause = 0;
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

