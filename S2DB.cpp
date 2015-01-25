
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
	   parent, SLOT( S2DShowInfoAtNewPosition( int, int ) ), Qt::UniqueConnection );
  connect( S2DV, SIGNAL( PointerMovedOnIntMCA( int, int ) ),
	   parent, SLOT( S2DShowIntMCA( int, int ) ), Qt::UniqueConnection );
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
  QStringList flist = dir.entryList( QDir::Files, QDir::Name );
  for ( int i = 0; i < flist.count(); i++ ) {
    int x = i % S2DI.ps[0];
    int y = i / S2DI.ps[0];
    mcaMap.aPoint( x, y )->load( flist[i], "" );
  }
}

