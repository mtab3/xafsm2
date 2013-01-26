
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "Data.h"
#include "ViewCtrl.h"

Data::Data( QWidget *p ) : QFrame( p )
{

  setupUi( this );

  SettingC = 0;
  FSDialog = new QFileDialog;
  CSDialog = new QColorDialog;

  QStringList filters;
  filters << "*.dat" << "*.*";

  FSDialog->setAcceptMode( QFileDialog::AcceptOpen );
  FSDialog->setDirectory( QDir::currentPath() );
  FSDialog->setNameFilters( filters );
  

  DColors << DColor01 << DColor02 << DColor03 << DColor04 << DColor05
	  << DColor06 << DColor07 << DColor08 << DColor09 << DColor10;
  DColors << DColor11 << DColor12 << DColor13 << DColor14 << DColor15
	  << DColor16 << DColor17 << DColor18 << DColor19 << DColor20;
  DColors << DColor21 << DColor22 << DColor23 << DColor24 << DColor25
	  << DColor26 << DColor27 << DColor28 << DColor29 << DColor30;

  DataTypeNames << "Measured" << "Scaned" << "Monitored" << "MCA" << "";

  connect( FileSelect, SIGNAL( clicked() ), FSDialog, SLOT( show() ) );
  connect( FSDialog, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( ShowFName( const QString & ) ) );
  connect( DataShow, SIGNAL( clicked() ), this, SLOT( StartToShowData() ) );
  for ( int i = 0; i < DColors.count(); i++ ) {
    connect( DColors.at(i), SIGNAL( clicked() ), this, SLOT( callCSDialog() ) );
  }
  connect( CSDialog, SIGNAL( colorSelected( const QColor & ) ),
	   this, SLOT( newColorSelected( const QColor & ) ) );
}

Data::~Data( void )
{
}

void Data::callCSDialog( void )
{
  SettingC = 0;
  for ( int i = 0; i < DColors.count(); i++ ) {
    if ( DColors.at(i) == sender() ) {
      SettingC = i;
      break;
    }
  }
  CSDialog->show();
}

void Data::newColorSelected( const QColor &c )
{
  SetColor( SettingC, c );
}

void Data::ShowFName( const QString &fname )
{
  FName = fname;
  QString dfname = fname;
#if 0
  QRect rec;
  QFontMetrics fm( SelectedFile->font() );
  rec = fm.boundingRect( dfname );
  qDebug() << "size of " << SelectedFile->width() << rec.width();
  while ( ( SelectedFile->width() - 10 )< rec.width() ) {
    dfname = dfname.mid( 1 );
    rec = fm.boundingRect( dfname );
    qDebug() << "size of " << SelectedFile->width() << rec.width();
  }
#endif
  SelectedFile->setText( dfname );
  CheckFileType( fname );
  StartToShowData();
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
  if ( (Data *)to != this ) {
    emit showMessage( tr( "No View is available." ), 2000 );
    return;
  }
  
  QFile f( FName );

  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    DataType->setText( DataTypeNames[ dataType = NONDATA ] );
    emit showMessage( tr( "Can not open the file %1." ).arg( FName ), 2000 );
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
    viewC->setNowDType( MEASDATA );
    viewC->setNowVType( XYVIEW );
    view->SetWindow0( 1e300, 0, -1e300, 0 );
    view->SetLeftName( "mu(E)" );
    view->SetRightName( "I0" );
  }

  int L0 = view->GetLines();

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
  int L = view->GetLines();
  for ( int i = L0; i < L; i++ ) {
    SetColor( i - L0, view->GetColor( i ) );
    view->SetLR( i, LEFT_AX );  // 一旦全部は左軸
    view->SetScaleType( i, FULLSCALE );
  }
  view->SetLR( L0, RIGHT_AX );  // 最初の一本だけ右軸 (I0)
  view->SetScaleType( L0, I0TYPE );
  view->SetLLine( L0+1 );    // デフォルトで情報表示される左軸の線はデータの1番め
  view->SetRLine( L0 );      // デフォルトで情報表示される右軸の線はデータの0番め (I0)

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
  view->SetAutoScale( true );

  int L0 = view->GetLines();

  if ( ! in.atEnd() ) line = in.readLine();
  if ( ! in.atEnd() ) line = in.readLine();  // 2行空読
  if ( ! in.atEnd() ) line = in.readLine();
  heads = line.split( '\t' );
  view->SetLineName( L0, heads.at( 1 ) );
  view->SetLR( L0, RIGHT_AX ); view->SetScaleType( 0, FULLSCALE );
  view->SetLineName( L0+1, heads.at( 2 ) );
  view->SetLR( L0+1, LEFT_AX ); view->SetScaleType( 1, FULLSCALE );

  view->SetXName( heads.at( 3 ) );
  view->SetXUnitName( heads.at( 4 ) );
  view->SetUpp( heads.at( 5 ).toDouble() );
  view->SetCenter( heads.at( 6 ).toDouble() );

  while ( ! in.atEnd() ) {
    line = in.readLine();
    line = line.simplified();
    vals = line.split( QRegExp( "\\s" ) );
    view->NewPoint( L0, vals.at( 0 ).toDouble(), vals.at( 1 ).toDouble() );
    view->NewPoint( L0+1, vals.at( 0 ).toDouble(), vals.at( 2 ).toDouble() );
  }
  int L = view->GetLines();
  for ( int i = L0; i < L; i++ ) {
    SetColor( i - L0, view->GetColor( i ) );
  }

  view->SetLLine( L0+1 );    // デフォルトで情報表示される左軸の線はデータの1番め
  view->SetRLine( L0 );      // デフォルトで情報表示される右軸の線はデータの0番め (I0)
  
  view->makeValid( true );
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
