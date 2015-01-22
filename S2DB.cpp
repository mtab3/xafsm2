
#include "S2DB.h"

S2DB::S2DB( QWidget *p ) : QFrame( p )
{
  setupUi( this );

  CBar->setAutoScale( AutoScaleB->isChecked() );
  S2DV->setCBar( CBar );

  MCAsDirSel = new QFileDialog;
  MCAsDirSel->setAcceptMode( QFileDialog::AcceptOpen );
  MCAsDirSel->setDirectory( QDir::currentPath() );
  MCAsDirSel->setNameFilter( "*" );
  MCAsDirSel->setFileMode( QFileDialog::Directory );
  MCAsDirSel->setConfirmOverwrite( false );

  connect( LoadMCAsB, SIGNAL( clicked() ), MCAsDirSel, SLOT( show() ),
	   Qt::UniqueConnection );
  connect( MCAsDirSel, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( LoadMCAs( const QString & ) ),
	   Qt::UniqueConnection );
  
#if 0
  connect( S2DPopUp, SIGNAL( clicked() ), this, SIGNAL( popup() ), Qt::UniqueConnection );
  connect( S2DPrintB, SIGNAL( clicked() ), this, SIGNAL( print() ), Qt::UniqueConnection );
#endif
  
  connect( AutoScaleB, SIGNAL( toggled( bool ) ), CBar, SLOT( setAutoScale( bool ) ), Qt::UniqueConnection );
  connect( zmax, SIGNAL( editingFinished() ), this, SLOT( newInputZmax() ), Qt::UniqueConnection );
  connect( zmin, SIGNAL( editingFinished() ), this, SLOT( newInputZmin() ), Qt::UniqueConnection );
  connect( S2DV, SIGNAL( newAutoZmax( double ) ), CBar, SLOT( newAutoZmax( double ) ), Qt::UniqueConnection );
  connect( S2DV, SIGNAL( newAutoZmin( double ) ), CBar, SLOT( newAutoZmin( double ) ), Qt::UniqueConnection );
  connect( CBar, SIGNAL( newZZ( QString, QString ) ), this, SLOT( newZZ( QString, QString ) ), Qt::UniqueConnection );
  connect( CBar, SIGNAL( newScale() ), S2DV, SLOT( update() ), Qt::UniqueConnection );
}




void S2DB::LoadMCAs( const QString &name )
{
  //  getNewMCAView();   // ここで確実に MCAData が有効になる
  //  S2DMCAMap.New( S2DI.ps[0]+1, S2DI.ps[1]+1 );


  qDebug() << "aaa";
}

#if 0
    S2DLastV = 0;
    S2DI.MCAFile = S2DI.SaveFile;
    if ( S2DI.MCAFile.isEmpty() )
      S2DI.MCAFile = QString( "S2DMCA0000.dat" );
    //    S2DWriteHead();
    S2DFileCheck();
    mUnits.clearStage();
    S2DStage = 0;
    S2DScanDir = FORWARD;

    S2DTimer->disconnect();
    switch ( S2DI.ScanMode ) {
    case STEP:
      connect( S2DTimer, SIGNAL( timeout() ), this, SLOT( S2DStepScanSequence() ),
	       Qt::UniqueConnection );
      break;
    case QCONT:
      connect( S2DTimer, SIGNAL( timeout() ),
	       this, SLOT( S2DQuasiContinuousScanSequence() ),
	       Qt::UniqueConnection );
      break;
    case RCONT:
      connect( S2DTimer, SIGNAL( timeout() ),
	       this, SLOT( S2DRealContinuousScanSequence() ),
	       Qt::UniqueConnection );
      break;
    default:
      qDebug() << "non-defined scan mode !";
      return;
    }
    S2DI.valid = true;
    S2DTimer->start( 10 );
  } else {
    S2DStop0();
  }
#endif
