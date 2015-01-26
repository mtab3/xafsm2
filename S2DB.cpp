
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
}

void S2DB::LoadMCAs( const QString &name )
{
  mcaMapDir = name;
  emit askToGetNewMCAView( this );
}

void S2DB::gotNewMCAView( MCAView *mcav, int length, int chs )
{
  qDebug() << S2Di.ps[0] << S2Di.ps[1] << length << chs;
  mcaMap.New( S2Di.ps[0], S2Di.ps[1], length, chs );

  QDir dir( mcaMapDir );
  QFileInfoList flist = dir.entryInfoList( QDir::Files, QDir::Name );
  for ( int i = 0; i < flist.count(); i++ ) {
    int x = i % S2Di.ps[0];
    int y = i / S2Di.ps[0];
    mcaMap.aPoint( x, y )->load( flist[i].absoluteFilePath(), "" );
  }
}


void S2DB::ShowInfoAtNewPosition( int ix, int iy )
{
  emit ShowMCASpectrum( mcaMap.aPoint( ix, iy ), mcaMap.aPoint( ix+1, iy ) );
}

void S2DB::ShowIntMCA( void )
{
  emit ShowMCASpectrum( mcaMap.lastP(), NULL );
}

void S2DB::ReCalcMap( QString *RS, QString *RE, QVector<QPushButton*> &ssdbs2 )
{
  QFileInfo mcaFile;
  double sum = 0;
  double lastsum = 0;

  qDebug() << "a";
  if ( S2Di.ScanMode == STEP ) {
    qDebug() << "stp";
    for ( int i = 0; i <= S2Di.ps[1]; i++ ) {
      for ( int j = 0; j < S2Di.ps[0]; j++ ) {
	sum = ReCalcAMapPointOnMem( j, i, RS, RE, ssdbs2 );
	if ( sum > 0 ) {
	  S2DV->setData( j, i, sum );
	} else {
	  return;
	}
	lastsum = sum;
      }
    }
  } else {
    qDebug() << "n-stp";
    for ( int i = 0; i <= S2Di.ps[1]; i++ ) {
      if (( S2Di.ScanBothDir ) && (( i % 2 ) == 1 )) {
	qDebug() << "both";
	for ( int j = S2Di.ps[0]; j >= 0; j-- ) {
	  sum = ReCalcAMapPointOnMem( j, i, RS, RE, ssdbs2 );
	  if ( ( sum > 0 ) && ( j < S2Di.ps[0] ) ) {
	    S2DV->setData( j, i, sum - lastsum );
	  }
	  if ( sum < 0 )
	    return;
	  lastsum = sum;
	}
      } else {
	qDebug() << "single";
	for ( int j = 0; j <= S2Di.ps[0]; j++ ) {
	  sum = ReCalcAMapPointOnMem( j, i, RS, RE, ssdbs2 );
	  if ( ( sum > 0 ) && ( j > 0 ) ) {
	    S2DV->setData( j - 1, i, sum - lastsum );
	  }
	  if ( sum < 0 )
	    return;
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

