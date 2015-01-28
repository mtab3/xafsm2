
#include "S2DB.h"

S2DB::S2DB( QWidget *p ) : QFrame( p )
{
  setupUi( this );

  kev2pix = new KeV2Pix;
  
  Read = false;
  CBar->setAutoScale( AutoScaleB->isChecked() );
  S2DV->setCBar( CBar );

  MCAsDirSel = new QFileDialog;
  MCAsDirSel->setAcceptMode( QFileDialog::AcceptOpen );
  MCAsDirSel->setDirectory( QDir::currentPath() );
  qDebug() << "Cur Path" << QDir::currentPath();
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

  mapTimer = new QTimer;
  connect( mapTimer, SIGNAL( timeout() ), this, SLOT( mapNext() ), Qt::UniqueConnection );
  mapReading = false;
}

void S2DB::setParent( QWidget *p )
{
  parent = p;
  
  connect( S2DV, SIGNAL( AskMoveToPointedPosition( int, int ) ),
	   parent, SLOT( S2DMoveToPointedPosition( int, int ) ), Qt::UniqueConnection );
  connect( S2DV, SIGNAL( AskToChangeMCACh( int ) ),
	   parent, SLOT( S2DChangeMCACh( int ) ), Qt::UniqueConnection );
  connect( S2DV, SIGNAL( PointerMovedToNewPosition( int, int ) ),
	   this, SLOT( ShowInfoAtNewPosition( int, int ) ), Qt::UniqueConnection );
  connect( S2DV, SIGNAL( PointerMovedOnIntMCA() ),
	   this, SLOT( ShowIntMCA() ), Qt::UniqueConnection );
  connect( this, SIGNAL( ShowMCASpectrum( aMCASet *, aMCASet * ) ), 
	   parent, SLOT( ShowMCASpectrum( aMCASet *, aMCASet * ) ), Qt::UniqueConnection );
  connect( parent,
	   SIGNAL( ReCalcS2DMap0( QString *, QString *, QVector<QPushButton*>& ) ),
	   this,
	   SLOT( ReCalcMap( QString *, QString *, QVector<QPushButton*>& ) ), 
	   Qt::UniqueConnection );

  connect( this, SIGNAL( ShowMessage( QString, int ) ),
	   parent, SLOT( ShowMessageOnSBar( QString, int ) ), Qt::UniqueConnection );
}

void S2DB::LoadMCAs( const QString &name )
{
  mcaMapDir = name;
  emit askToGetNewMCAView( this );
}

void S2DB::getNewMCAMap( int length, int chs )
{
  int iX = S2Di.ps[0] + ( (S2Di.ScanMode == STEP) ? 0 : 1 );
  mcaMap.New( iX, S2Di.ps[1] + 1, length, chs );

  QDir dir( mcaMapDir );
  flist = dir.entryInfoList( QDir::Files, QDir::Name );

  mapTimer->setInterval( 10 );
  mapReading = false;
  mapTimer->start();
}

void S2DB::mapNext( void )
{
  if ( mapReading )
    return;

  QFileInfo fi;
  if ( flist.count() > 0 ) {
    fi = flist[0];
    flist.removeAt( 0 );
  } else {
    mapTimer->stop();
    return;
  }
  mapReading = true;

  QStringList cmps = fi.baseName().split( "-" );
  if ( cmps.count() > 1 ) {
    int x = cmps[ cmps.count() - 1 ].toInt();
    int y = cmps[ cmps.count() - 2 ].toInt();
    aMCASet *set = mcaMap.aPoint( x, y );
    emit ShowMessage( tr( "Reading MCA File : [%1]" ).arg( fi.baseName() ),
		      500 );
    set->load( fi.absoluteFilePath(), "" );
    if ( set->isValid() )
      set->correctE( kev2pix );
  }

  mapReading = false;
}


void S2DB::ShowInfoAtNewPosition( int ix, int iy )
{
  if ( S2Di.ScanMode == STEP ) {
    emit ShowMCASpectrum( mcaMap.aPoint( ix, iy ), NULL );
  } else {
    if ( mcaMap.valid( ix+1, iy ) ) {
      emit ShowMCASpectrum( mcaMap.aPoint( ix, iy ), mcaMap.aPoint( ix+1, iy ) );
    } else {
      emit ShowMCASpectrum( mcaMap.aPoint( ix, iy ), mcaMap.aPoint( ix, iy ) );
    }
  }
}

void S2DB::ShowIntMCA( void )
{
  emit ShowMCASpectrum( mcaMap.lastP(), NULL );
}

void S2DB::ReCalcMap( QString *RS, QString *RE, QVector<QPushButton*> &ssdbs2 )
{
  if ( ! S2Di.valid )
    return;
  
  QFileInfo mcaFile;
  double sum = 0;
  double lastsum = 0;

  if ( S2Di.ScanMode == STEP ) {
    for ( int i = 0; i <= S2Di.ps[1]; i++ ) {
      for ( int j = 0; j < S2Di.ps[0]; j++ ) {
	if ( mcaMap.valid( j, i ) ) {
	  sum = ReCalcAMapPointOnMem( j, i, RS, RE, ssdbs2 );
	} else {
	  sum = 0;
	}
	if ( sum > 0 ) {
	  S2DV->setData( j, i, sum );
	} else {
	  return;
	}
	//	lastsum = sum;
      }
    }
  } else {
    for ( int i = 0; i <= S2Di.ps[1]; i++ ) {
      if (( S2Di.ScanBothDir ) && (( i % 2 ) == 1 )) {
	if ( mcaMap.valid( S2Di.ps[0], i ) ) {
	  lastsum = ReCalcAMapPointOnMem( S2Di.ps[0], i, RS, RE, ssdbs2 );
	} else {
	  lastsum = 0;
	}
	for ( int j = S2Di.ps[0]-1; j >= 0; j-- ) {
	  if ( mcaMap.valid( j, i ) ) {
	    sum = ReCalcAMapPointOnMem( j, i, RS, RE, ssdbs2 );
	  } else {
	    sum = lastsum;
	  }
	  S2DV->setData( j, i, sum - lastsum );
	  lastsum = sum;
	}
      } else {
	if ( mcaMap.valid( 0, i ) ) {
	  lastsum = ReCalcAMapPointOnMem( 0, i, RS, RE, ssdbs2 );
	} else {
	  lastsum = 0;
	}
	for ( int j = 1; j <= S2Di.ps[0]; j++ ) {
	  if ( mcaMap.valid( j, i ) ) {
	    sum = ReCalcAMapPointOnMem( j, i, RS, RE, ssdbs2 );
	  } else {
	    sum = lastsum;
	  }
	  S2DV->setData( j - 1, i, sum - lastsum );
	  lastsum = sum;
	}
      }
    }
  }
  S2DV->update();
}

double S2DB::ReCalcAMapPointOnMem( int ix, int iy,
				   QString *RS, QString *RE,
				   QVector<QPushButton*> &ssdbs2 )
{
  double sum = 0;

  QVector<double> ss;
  QVector<double> es;
  for ( int i = 0; i < MaxSSDs; i++ ) {
    ss << kev2pix->p2E( i, RS[ i ].toDouble() );
    es << kev2pix->p2E( i, RE[ i ].toDouble() );
  }

  aMCASet *set = mcaMap.aPoint( ix, iy );
  if ( ( set != NULL )&&( set->isValid() ) ) {
    for ( int ch = 0; ch < set->chs(); ch++ ) {
      if ( ssdbs2[ ch ]->isChecked() == PBTrue ) {
	double *E = set->Ch[ ch ].E;
	quint32 *cnt = set->Ch[ ch ].cnt;
	for ( int i = 0; i < set->length(); i++ ) {
	  if (( E[i] >= ss[ch] )&&( E[i] <= es[ch] )) {
	    sum += cnt[i];
	  }
	}
      }
    }
  }
  
  return sum;
}

