
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "Data.h"
#include "ViewCtrl.h"

Data::Data( QWidget *p ) : QFrame( p )
{
  setupUi( this );

  FSDialog = new QFileDialog;

  FSDialog->setAcceptMode( QFileDialog::AcceptOpen );
  FSDialog->setDirectory( QDir::currentPath() );
  FSDialog->setFilter( "*.dat" );

  DColors << DColor01 << DColor02 << DColor03 << DColor04 << DColor05
	  << DColor06 << DColor07 << DColor08 << DColor09 << DColor10;

  DataTypeNames << "Measured" << "Scaned" << "Monitored" << "MCA" << "";

  connect( FileSelect, SIGNAL( clicked() ), FSDialog, SLOT( show() ) );
  connect( FSDialog, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( ShowFName( const QString & ) ) );
  connect( DataShow, SIGNAL( clicked() ), this, SLOT( StartToShowData() ) );
}

Data::~Data( void )
{
}

void Data::ShowFName( const QString &fname )
{
  SelectedFile->setText( fname );
  CheckFileType( fname );
}

void Data::CheckFileType( const QString &fname )
{
  QFile f( fname );

  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    DataType->setText( DataTypeNames[ dataType = NONDATA ] );
    emit showMessage( tr( "Can not open the file %1." ).arg( fname ), 2000 );
    return;
  }

  QTextStream in( &f );
  if ( in.atEnd() ) {
    f.close();
    DataType->setText( DataTypeNames[ dataType = NONDATA ] );
    emit showMessage( tr( "The file %1 is empty." ).arg( fname ), 2000 );
    return;
  }

  QString aline = in.readLine();
  f.close();

  QStringList Checks;
  Checks << "  9809     AichiSR"
    << "# XafsM2 Scan Data"
    << "# XafsM2 Monitor Data"
    << "# XafsM2 MCA Data";

  
  if ( aline.left( Checks[ MEASDATA ].length() ) == Checks[ MEASDATA ] ) {
    DataType->setText( DataTypeNames[ dataType = MEASDATA ] );
  } else if ( aline.left( Checks[ SCANDATA ].length() ) == Checks[ SCANDATA ] ) {
    DataType->setText( DataTypeNames[ dataType = SCANDATA ] );
  } else if ( aline.left( Checks[ MONDATA ].length() ) == Checks[ MONDATA ] ) {
    DataType->setText( DataTypeNames[ dataType = MONDATA ] );
  } else if ( aline.left( Checks[ MCADATA ].length() ) == Checks[ MCADATA ] ) {
    DataType->setText( DataTypeNames[ dataType = MCADATA ] );
  } else {
    DataType->setText( DataTypeNames[ dataType = NONDATA ] );
    emit showMessage( tr( "The file %1 is not avaliable to show." ).arg( fname ), 2000 );
  }
}

void Data::StartToShowData( void )
{
  emit AskToGetNewView( dataType );
}

void Data::GotNewView( QObject *to, ViewCTRL *view )
{
  QString fname;

  if ( (Data *)to != this ) {
    emit showMessage( tr( "No View is available." ), 2000 );
    return;
  }
  
  QFile f( fname = SelectedFile->text() );

  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    DataType->setText( DataTypeNames[ dataType = NONDATA ] );
    emit showMessage( tr( "Can not open the file %1." ).arg( fname ), 2000 );
    return;
  }

  QTextStream in( &f );

  switch( dataType ) {
  case MEASDATA: showMeasData( in, view ); break;
  case SCANDATA: showScanData( in, view ); break;
  case MONDATA:  showMonData( in, view );  break;
  case MCADATA:  showMCAData( in, view );  break;
  default: break;
  }


  f.close();
}

void Data::SetColor( int i, const QColor &c )
{
  DColors.at(i)->setStyleSheet( "background-color: " + c.name() );
}

void Data::showMeasData( QTextStream &in, ViewCTRL *viewC )
{
  QString line;
  QStringList vals;
  bool inBody = false;
  QString HeaderEnd = "    Offset";
  XYView *view = (XYView*)viewC->getView();

  if ( viewC->getNowDType() == NONDATA ) {
    view->SetWindow0( 1e300, 0, -1e300, 0 );
    viewC->setNowDType( MEASDATA );
    viewC->setNowVType( XYVIEW );
  }

  int L0 = view->getLines();
  int G0 = view->getGroups();
  view->setLineG( L0, 0 );
  for ( int i = L0+1; i < MAXLINES; i++ ) {  // 手抜き!! 何本線を引くかわからないので
                                          // 未定義部分を全部染めておく
    view->setLineG( i, 1 );
  }

  while ( ! in.atEnd() ) {
    line = in.readLine();
    if ( !inBody ) {
      if ( line.left( HeaderEnd.length() ) == HeaderEnd )
	inBody = true;
    } else {
      line = line.simplified();
      vals = line.split( QRegExp( "\\s" ) );
      int Vals = vals.count();
      for ( int i = 3; i < Vals - 2; i++ ) {
	view->NewPoint( L0+i-3, vals.at( Vals - 2 ).toDouble(), vals.at( i ).toDouble() );
      }
    }
  }
  int L = view->getLines();
  for ( int i = L0; i < L; i++ ) {
    SetColor( i - L0, view->getColor( view->getLineG( i ) ) );
  }
  view->update();
}

void Data::showScanData( QTextStream &in, ViewCTRL *viewC )
{
  QStringList heads, vals;
  QString line;
  
  XYView *view = (XYView*)viewC->getView();

  if ( viewC->getNowDType() == NONDATA ) {
    view->SetWindow0( 1e300, 0, -1e300, 0 );
    viewC->setNowDType( SCANDATA );
    viewC->setNowVType( XYVIEW );
  }

  int L0 = view->getLines();
  int G0 = view->getGroups();
  view->setLineG( L0, 0 );
  for ( int i = L0+1; i < MAXLINES; i++ ) {  // 手抜き!! 何本線を引くかわからないので
                                          // 未定義部分を全部染めておく
    view->setLineG( i, 1 );
  }

  if ( ! in.atEnd() ) line = in.readLine();
  if ( ! in.atEnd() ) line = in.readLine();  // 2行空読
  if ( ! in.atEnd() ) line = in.readLine();
  heads = line.split( '\t' );
  view->SetGName( 0, heads.at( 1 ) );
  view->SetGName( 1, heads.at( 2 ) );
  view->SetXName( heads.at( 3 ) );
  view->setUpp( heads.at( 5 ).toDouble() );
  view->setCenter( heads.at( 6 ).toDouble() );

  while ( ! in.atEnd() ) {
    line = in.readLine();
    line = line.simplified();
    vals = line.split( QRegExp( "\\s" ) );
    int Vals = vals.count();
    for ( int i = 1; i < Vals; i++ ) {
      view->NewPoint( L0+(i-1), vals.at( 0 ).toDouble(), vals.at( i ).toDouble() );
    }
  }
  int L = view->getLines();
  for ( int i = L0; i < L; i++ ) {
    SetColor( i - L0, view->getColor( view->getLineG( i ) ) );
  }

  view->update();
}

void Data::showMonData( QTextStream &in, ViewCTRL *viewC )
{
  QStringList heads, vals;
  QString line;
  double Values[ MaxMon ];
  
  TYView *view = (TYView*)viewC->getView();

  if ( viewC->getNowDType() == NONDATA ) {
    viewC->setNowDType( MONDATA );
    viewC->setNowVType( TYVIEW );
  }
  view->SetMonScale( 0 );
  view->makeValid( true );

  if ( ! in.atEnd() ) line = in.readLine();
  if ( ! in.atEnd() ) line = in.readLine();  // 2行空読
  if ( ! in.atEnd() ) line = in.readLine();
  heads = line.split( '\t' );
  for ( int i = 2; i < heads.count(); i++ ) {
    view->SetLName( i-2, heads.at( i ) );
  }
  //  view->SetXName( heads.at( 1 ) );
  view->ClearDataR();
  view->SetLines( heads.count() - 2 );

  for ( int i = 0; i < MaxMon; i++ )
    Values[i] = 0;

  while ( ! in.atEnd() ) {
    line = in.readLine();
    line = line.simplified();
    vals = line.split( QRegExp( "\\s" ) );
    int Vals = vals.count();
    for ( int i = 1; i < Vals - 3; i++ ) {
      Values[i-1] = vals.at( i ).toDouble();
    }
    view->NewPointR( vals.at(0).toDouble()*1000, Values[0], Values[1], Values[2] );
  }
  int L = view->GetLines();
  for ( int i = 0; i < L; i++ ) {
    SetColor( i, view->getColor( i ) );
  }

  view->update();
}

void Data::showMCAData( QTextStream &in, ViewCTRL *viewC )
{
  QStringList heads, vals;
  QString line;

  MCAView *view = (MCAView*)viewC->getView();

  if ( viewC->getNowDType() == NONDATA ) {
    viewC->setNowDType( MCADATA );
    viewC->setNowVType( MCAVIEW );
  }
  view->makeValid( true );

  if ( ! in.atEnd() ) line = in.readLine();
  if ( ! in.atEnd() ) line = in.readLine();
  if ( ! in.atEnd() ) line = in.readLine();  // 3行空読
  if ( ! in.atEnd() ) line = in.readLine();
  heads = line.split( QRegExp( "\\s" ) );
  int MCALength = heads.at(1).toInt();
  int MCACh = heads.at(2).toInt();
  double RealTime = heads.at(3).toDouble();
  double LiveTime = heads.at(4).toDouble();
  int ROIs = heads.at(5).toInt();
  int ROIe = heads.at(6).toInt();

  int *MCA = view->setMCAdataPointer( MCALength );
  view->SetRealTime( RealTime );
  view->SetLiveTime( LiveTime );
  view->SetMCACh( MCACh );
  view->setROI( ROIs, ROIe );
  view->makeValid( true );

  int cnt = 0;
  while (( ! in.atEnd() )&&( cnt < MCALength )) {
    line = in.readLine();
    line = line.simplified();
    vals = line.split( QRegExp( "\\s" ) );
    MCA[cnt] = vals.at(1).toInt();
    cnt++;
  }
  view->update();
}
