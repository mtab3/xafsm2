
#include "S2DB.h"

S2DB::S2DB( QWidget *p ) : QFrame( p )
{
  setupUi( this );

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
	   SIGNAL( ReCalcS2DMap0( bool, QDir, QString, bool ) ),
	   this,
	   SLOT( ReCalcMap( bool, QDir, QString, bool ) ), 
	   Qt::UniqueConnection );
}

void S2DB::LoadMCAs( const QString &name )
{
  mcaMapDir = name;
  emit askToGetNewMCAView( this );
}

void S2DB::gotNewMCAView( MCAView *mcav, int length, int chs )
{
  qDebug() << S2DI.ps[0] << S2DI.ps[1] << length << chs;
  mcaMap.New( S2DI.ps[0], S2DI.ps[1], length, chs );

  QDir dir( mcaMapDir );
  QFileInfoList flist = dir.entryInfoList( QDir::Files, QDir::Name );
  for ( int i = 0; i < flist.count(); i++ ) {
    int x = i % S2DI.ps[0];
    int y = i / S2DI.ps[0];
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

void S2DB::ReCalcMap( bool onMem, QDir dir, QString base, bool use3rdAx )
{
  QFileInfo mcaFile;
  double sum = 0;
  double lastsum = 0;

  if ( S2DI.ScanMode == STEP ) {
    for ( int i = 0; i <= S2DI.ps[1]; i++ ) {
      for ( int j = 0; j < S2DI.ps[0]; j++ ) {
	if ( onMem ) {
	  sum = ReCalcAMapPointOnMem( j, i );
	} else {
	  mcaFile = GenerateMCAFileName( dir, base, use3rdAx, j, i, S2DI.ps[2] );
	  sum = ReCalcAMapPoint( mcaFile.canonicalFilePath() );
	}
	if ( sum > 0 ) {
	  S2DV->setData( j, i, sum );
	} else {
	  return;
	}
	lastsum = sum;
      }
    }
  } else {
    for ( int i = 0; i <= S2DI.ps[1]; i++ ) {
      if (( S2DI.ScanBothDir ) && (( i % 2 ) == 1 )) {
	for ( int j = S2DI.ps[0]; j >= 0; j-- ) {
	  if ( onMem ) {
	    sum = ReCalcAMapPointOnMem( j, i );
	  } else {
	    mcaFile = GenerateMCAFileName( dir, base, use3rdAx, j, i, S2DI.ps[2] );
	    sum = ReCalcAMapPoint( mcaFile.canonicalFilePath() );
	  }
	  if ( ( sum > 0 ) && ( j < S2DI.ps[0] ) ) {
	    S2DV->setData( j, i, sum - lastsum );
	  }
	  if ( sum < 0 )
	    return;
	  lastsum = sum;
	}
      } else {
	for ( int j = 0; j <= S2DI.ps[0]; j++ ) {
	  if ( onMem ) {
	    sum = ReCalcAMapPointOnMem( j, i );
	  } else {
	    mcaFile = GenerateMCAFileName( dir, base, use3rdAx, j, i, S2DI.ps[2] );
	    sum = ReCalcAMapPoint( mcaFile.canonicalFilePath() );
	  }
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
}

double S2DB::ReCalcAMapPoint( QString fname )
{
  double sum = 0;

  QStringList vals;
  QFile f( fname );
  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) )
    return -1.;

  QTextStream in( &f );
  double eng;
  double cnt;
  QVector<double> ss;
  QVector<double> es;
  for ( int i = 0; i < MaxSSDs; i++ ) {
    ss << kev2pix->p2E( i, ROIStart[ i ].toDouble() );
    es << kev2pix->p2E( i, ROIEnd[ i ].toDouble() );
  }
  while( !in.atEnd() ) {
    vals = in.readLine().simplified().split( QRegExp( "\\s+" ) );
    if (( vals[0] != "#" )&&( vals.count() >= 36 )) {
      for ( int i = 0; i < MaxSSDs; i++ ) {
	if ( SSDbs2[i]->isChecked() == PBTrue ) {
	  eng = vals[i*2+1].toDouble();
	  cnt = vals[i*2+2].toDouble();
	  if (( eng >= ss[i] )&&( eng <= es[i] )) {
	    sum += cnt;
	  }
	}
      }
    }
  }
  f.close();

  return sum;
}

double S2DB::ReCalcAMapPointOnMem( int ix, int iy )
{
  double sum = 0;

  QVector<double> ss;
  QVector<double> es;
  for ( int i = 0; i < MaxSSDs; i++ ) {
    ss << kev2pix->p2E( i, ROIStart[ i ].toDouble() );
    es << kev2pix->p2E( i, ROIEnd[ i ].toDouble() );
  }

  aMCASet *set = map->aPoint( ix, iy );
  if ( ( set != NULL )&&( set->isValid() ) ) {
    for ( int ch = 0; ch < SAVEMCACh; ch++ ) {
      if ( SSDbs2[ ch ]->isChecked() == PBTrue ) {
	double *E = set->Ch[ ch ].E;
	quint32 *cnt = set->Ch[ ch ].cnt;
	for ( int i = 0; i < MCALength; i++ ) {
	  if (( E[i] >= ss[ch] )&&( E[i] <= es[ch] )) {
	    sum += cnt[i];
	  }
	}
      }
    }
  }
  
  return sum;
}

QFileInfo S2DB::GenerateMCAFileName( QDir dir, QString base, bool use3rdAx,
					int i1, int i2, int i3 )
{
  QFileInfo BaseFile( base );
  QFileInfo mcaFile;

  if ( use3rdAx ) {
    mcaFile = QFileInfo( dir,
			 QString( "%1-%2-%3-%4.dat" )
			 .arg( BaseFile.baseName() )
			 .arg( i1, 4, 10, QChar( '0' ) )
			 .arg( i2, 4, 10, QChar( '0' ) )
			 .arg( i3, 4, 10, QChar( '0' ) ) );
  } else {
    mcaFile = QFileInfo( dir,
			 QString( "%1-%2-%3.dat" )
			 .arg( BaseFile.baseName() )
			 .arg( i1, 4, 10, QChar( '0' ) )
			 .arg( i2, 4, 10, QChar( '0' ) ) );
  }

  return mcaFile;
}

