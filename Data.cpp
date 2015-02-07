
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "Head9809.h"
#include "Data.h"
#include "ViewCtrl.h"

Data::Data( QWidget *p ) : QFrame( p )
{
  setupUi( this );

  u = new Units;

  SettingL = 0;
  SettingC = QColor( 0, 0, 0 );
  FSDialog = new QFileDialog;
  CSDialog = new QColorDialog;

  QStringList filters;
  filters << "*.dat" << "*.*" << "*";

  FSDialog->setAcceptMode( QFileDialog::AcceptOpen );
  FSDialog->setDirectory( QDir::currentPath() );
  FSDialog->setNameFilters( filters );
  
  theViewC = NULL;
  theMCAView = NULL;
  theTYView = NULL;
  theXYView = NULL;
  XYLine0 = XYLines = 0;
  MCADataIsValid = false;
  MCALength = 0;
  S2DI.valid = false;

  DColors << DColor01 << DColor02 << DColor03 << DColor04 << DColor05
	  << DColor06 << DColor07 << DColor08 << DColor09 << DColor10;
  DColors << DColor11 << DColor12 << DColor13 << DColor14 << DColor15
	  << DColor16 << DColor17 << DColor18 << DColor19 << DColor20;
  DColors << DColor21 << DColor22 << DColor23 << DColor24 << DColor25
	  << DColor26 << DColor27 << DColor28 << DColor29 << DColor30;

  connect( FileSelect, SIGNAL( clicked() ), FSDialog, SLOT( show() ),
	   Qt::UniqueConnection );
  connect( FSDialog, SIGNAL( fileSelected( const QString & ) ),
	   this, SLOT( ShowFName( const QString & ) ),
	   Qt::UniqueConnection );
  connect( DataShow, SIGNAL( clicked() ), this, SLOT( StartToShowData() ),
	   Qt::UniqueConnection );
  for ( int i = 0; i < DColors.count(); i++ ) {
    connect( DColors.at(i), SIGNAL( clicked() ), this, SLOT( callCSDialog() ),
	     Qt::UniqueConnection );
  }
  connect( CSDialog, SIGNAL( colorSelected( const QColor & ) ),
	   this, SLOT( newColorSelected( const QColor & ) ),
	   Qt::UniqueConnection );
}

Data::~Data( void )
{
  delete u;
  delete FSDialog;
  delete CSDialog;
}

void Data::setDataRoot( const QString &dataRoot )
{
  FSDialog->setDirectory( dataRoot );
}

void Data::callCSDialog( void )
{
  SettingL = 0;
  for ( int i = 0; i < DColors.count(); i++ ) {
    if ( DColors.at(i) == sender() ) {
      SettingL = i;
      break;
    }
  }
  CSDialog->show();
}

void Data::newColorSelected( const QColor &c )
{
  SettingC = c;
  SetColor( SettingL, SettingC );
  emit GiveMeCurrentView();
}

void Data::GotCurrentView( void *view )
{
  if ( (XYView*)view == theXYView ) {
    if ( SettingL < XYLines ) {
      theXYView->SetColor( XYLine0 + SettingL, SettingC );
      theXYView->update();
    }
  }
}

void Data::ShowFName( const QString &fname )
{
  FName = fname;
  QString dfname = fname;

  SelectedFile->setText( dfname );
  CheckFileType( fname );
  emit AskToGetNewView( dataType, FSDialog->directory().absolutePath() );
}

void Data::StartToShowData( void )
{
  emit AskToGetNewView( dataType, "" );
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

  if ( aline.left( FileIDs[ MEASDATA ].length() ) == FileIDs[ MEASDATA ] ) {
    DataType->setText( DataTypeNames[ dataType = MEASSHOW ] );
  } else if ( aline.left( FileIDs[ SCANDATA ].length() ) == FileIDs[ SCANDATA ] ) {
    DataType->setText( DataTypeNames[ dataType = SCANSHOW ] );
  } else if ( aline.left( FileIDs[ MONDATA ].length() ) == FileIDs[ MONDATA ] ) {
    DataType->setText( DataTypeNames[ dataType = MONSHOW ] );
  } else if ( aline.left( FileIDs[ MCADATA ].length() ) == FileIDs[ MCADATA ] ) {
    DataType->setText( DataTypeNames[ dataType = MCASHOW ] );
  } else if ( aline.left( FileIDs[ S2DDATA ].length() ) == FileIDs[ S2DDATA ] ) {
    DataType->setText( DataTypeNames[ dataType = S2DSHOW ] );
  } else {
    DataType->setText( DataTypeNames[ dataType = NONDATA ] );
    emit showMessage( tr( "The file %1 is not avaliable to show." ).arg( fname ), 2000 );
  }
}

void Data::GotNewView( ViewCTRL *viewC,
		       QVector<AUnit*> &AMotors, QVector<AUnit*> &ASensors )
{
  QFile f( FName );

  if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    DataType->setText( DataTypeNames[ dataType = NONDATA ] );
    emit showMessage( tr( "Can not open the file %1." ).arg( FName ), 2000 );
    return;
  }
  theViewC = viewC;

  QTextStream in( &f );

  switch( dataType ) {
#if 0
  case MEASSHOW: showMeasData( in ); break;
#endif
  case MONSHOW:  showMonData( in, ASensors );  break;
  case SCANSHOW: showScanData( in, AMotors, ASensors ); break;
  case MCASHOW:  showMCAData( in );  break;
  case S2DSHOW:  showS2DData( in, AMotors, ASensors );  break;
  default: break;
  }

  f.close();
}

void Data::SetColor( int i, const QColor &c )
{
  DColors.at(i)->setStyleSheet( "background-color: " + c.name() );
}

#if 0
void Data::showMeasData( QTextStream &in )
{
  QString HeaderEnd = "    Offset";
  QString line;
  QStringList vals;

  theXYView = (XYView*)(theViewC->getView());

  // なんか怪しげ showMeasData を復活させるなら要再検討
  if ( theViewC->getDType() == NONDATA ) {
    theViewC->setNowDType( MEASSHOW );
    theViewC->setNowVType( XYVIEW );
    theXYView->SetWindow0( 1e300, 0, -1e300, 0 );
    theXYView->SetLeftName( "mu(E)" );
    theXYView->SetRightName( "I0" );
  }

  int L0 = theXYView->GetLines();  // ここで描画をはじめる前にすでに描画されている線の数

  Head9809 head;
  if ( ! head.readFromStream( in ) ) {
    qDebug() << "Incomplete file as 9809 format";
    return;
  }

  //  int TrMode = head.getMode();
  // 混在測定を許すのでファイル全体モードは
  // あまり意味がない。

  // 最初の x(to go), x(enc), dwell は抜いたデータのモードだけ返って来る。
  QStringList ChModes0 = head.getChModes();
  int cols = ChModes0.count();
  bool aFluo = false;

  QVector<int> ChModes;    // 返ってきたモードを数字に直しておく
  for ( int i = 0; i < ChModes0.count(); i++ ) {
    ChModes << ChModes0[i].toInt();
  }
  int I0col = -1, I1col = -1;
  for ( int i = 0; i < cols; i++ ) {
    if ( ChModes[i] == I0 ) {         // 先に I0 を探さないと計算できない
      I0col = i + 3;
    }
    if ( ChModes[i] == TRANS ) {  // 一時期のバグで I0の mode 番号として 1 でなく、
                                  // 2 (本来は I1) を書いてしまってるファイルがあるので
                                  // I0 が見つからなくて I1 があるときは I1 を I0 と
                                  // みなす。
      I1col = i + 3;
    }
    if ( ChModes[i] == FLUO ) {       // 蛍光モードが少なくとも一チャンネルある
      aFluo = true;
    }
  }
  if ( I0col == -1 ) {                // I0 が見つからなければ表示できない
    if ( I1col == -1 ) {
      qDebug() << "No I0 data is found.";
      return;
    } else {
      I0col = I1col;
    }
  }

  bool dispf;
  double x, y, I00, I, totalf;
  while ( ! in.atEnd() ) {
    totalf = 0;
    line = in.readLine().simplified();
    vals = line.split( QRegExp( "\\s" ) );
    x = u->deg2keV( vals[0].toDouble() );
    I00 = vals.at( I0col ).toDouble();
    for ( int i = 0; i < cols; i++ ) {
      dispf = true;
      I = vals.at( i + 3 ).toDouble();
      switch( ChModes[i] ) {
      case I0:    y = I; break;
      case TRANS: y = ( I != 0 ) ?  log( fabs( I00 / I ) ) : 1; break;
      case FLUO:  y = ( I00 != 0 ) ? I / I00 : 1; totalf += y; break;
      default: dispf = false; break;
      }
      if ( dispf ) {
        theXYView->NewPoint( L0+i, x, y );
      }
    }
    if ( aFluo ) {
      theXYView->NewPoint( L0+cols, x, totalf );
    }
  }

  int L = theXYView->GetLines();       // ここで描画した線もいれた線の数
  for ( int i = L0; i < L; i++ ) {
    SetColor( i - L0, theXYView->GetColor( i ) );
    theXYView->SetLR( i, LEFT_AX );  // 一旦全部は左軸
    theXYView->SetScaleType( i, FULLSCALE );
  }
  theXYView->SetLR( L0, RIGHT_AX );  // 最初の一本だけ右軸 (I0)
  theXYView->SetScaleType( L0, I0TYPE );
  theXYView->SetLLine( L0+1 );  // デフォルトで情報表示される左軸の線はデータの1番め
  theXYView->SetRLine( L0 );    // デフォルトで情報表示される右軸の線はデータの0番め (I0)

  XYLine0 = L0;          // このクラスで管理する線の番号の最初と、
  XYLines = L - L0 + 1;  // その本数

  theXYView->update();
}
#endif

void Data::showScanData( QTextStream &in,
			 QVector<AUnit*> &AMotors, QVector<AUnit*> &ASensors )
{
  QStringList heads, vals;
  QString line;

  theXYView = (XYView*)theViewC->getView();
  theXYView->SetWindow0( 1e300, 0, -1e300, 0 );
  theXYView->SetAutoScale( true );

  int L0 = theXYView->GetLines();

  ScanInfo si = theXYView->getSInfo();
  si.load( in, AMotors, ASensors );
  si.show();
  
  theXYView->SetLineName( L0, si.asName );
  theXYView->SetLR( L0, RIGHT_AX ); theXYView->SetScaleType( 0, FULLSCALE );
  theXYView->SetLineName( L0+1, si.as0Name );
  theXYView->SetLR( L0+1, LEFT_AX ); theXYView->SetScaleType( 1, FULLSCALE );
  
  theXYView->SetXName( si.amName );
  theXYView->SetXUnitName( si.unitName );
  theXYView->SetUpp( si.upp );
  theXYView->SetCenter( si.origin );

  int lines = 2 + si.mi.Sensors.count();
  while ( ! in.atEnd() ) {
    line = in.readLine();
    line = line.simplified();
    if ( line.count() < 2 ) continue;
    if ( line[0] == '#' ) continue;
    vals = line.split( QRegExp( "\\s" ) );
    for ( int i = 0; i < lines; i++ ) {
      if ( vals.count() > i + 1 ) {
	theXYView->NewPoint( i, vals[0].toDouble(), vals[i+1].toDouble() );
      }
    }
  }

  int L = theXYView->GetLines();
  for ( int i = L0; i < L; i++ ) {
    SetColor( i - L0, theXYView->GetColor( i ) );
  }
  
  theXYView->SetLLine( L0+1 ); // デフォルトで情報表示される左軸の線はデータの1番め
  theXYView->SetRLine( L0 );   // デフォルトで情報表示される右軸の線はデータの0番め (I0)

  XYLine0 = L0;
  XYLines = 2;
  
  theXYView->makeValid( true );
  theXYView->update();
}

void Data::showMonData( QTextStream &in, QVector<AUnit*> &ASensors )
{
  QStringList heads, vals;
  QString line;
  double Values[ MaxMon ];
  
  theTYView = (TYView*)theViewC->getView();
#if 0
  if ( theViewC->getNowDType() == NONDATA ) {
    theViewC->setNowDType( MONSHOW );
    theViewC->setNowVType( TYVIEW );
  }
#endif
  theTYView->SetMonScale( 0 );
  theTYView->makeValid( true );

  MonInfo monInfo;
  monInfo.load( in, ASensors );
  monInfo.show();
#if 0
  if ( ! in.atEnd() ) line = in.readLine();
  if ( ! in.atEnd() ) line = in.readLine();  // 2行空読
  if ( ! in.atEnd() ) line = in.readLine();
  heads = line.split( '\t' );
  for ( int i = 2; i < heads.count(); i++ ) {
    theTYView->SetLName( i-2, heads.at( i ) );
  }
#endif
  int Lines = monInfo.SensorNames.count();
  for ( int i = 0; i < Lines; i++ ) {
    theTYView->SetLName( i, monInfo.SensorNames[i] );
  }
  //  theTYView->SetXName( heads.at( 1 ) );
  theTYView->ClearDataR();
  theTYView->SetLines( Lines );

  for ( int i = 0; i < MaxMon; i++ )
    Values[i] = 0;

  while ( ! in.atEnd() ) {
    line = in.readLine();
    line = line.simplified();
    vals = line.split( QRegExp( "\\s" ) );
    int Vals = vals.count();
    int i;
    for ( i = 0; i < Vals; i++ ) {
      if ( vals[i][0] == QChar( '#' ) ) break;
    }
    if ( i < Vals )
      Vals = i;
    for ( int i = 1; i < Vals; i++ ) {
      Values[i-1] = vals.at( i ).toDouble();
    }
    theTYView->NewPointR( vals.at(0).toDouble()*1000, Values, Vals - 1 );
  }
  int L = theTYView->GetLines();
  for ( int i = 0; i < L; i++ ) {
    SetColor( i, theTYView->getColor( i ) );
  }

  theTYView->update();
}

void Data::showMCAData( QTextStream &in )
{
  MCALength = 2048;

  QStringList vals;
  theMCAView = (MCAView*)theViewC->getView();

  KeV2Pix *k2p = theMCAView->keV2Pix();
  cMCACh = 0;
#if 0
  if ( theViewC->getNowDType() == NONDATA ) {
    theViewC->setNowDType( MCASHOW );
    theViewC->setNowVType( MCAVIEW );
  }
#endif
  theMCAView->makeValid( true );
  cMCA = theMCAView->setMCAdataPointer( MCALength );

  MCADataIsValid = false;
  //  getNewMCAs( MCALength );
  aMCA.setSize( MCALength, MaxSSDs );
  
  aMCA.load( in, "" );
#if 0  
  while ( ! in.atEnd() ) {
    vals = in.readLine().simplified().split( QRegExp( "\\s" ) );
    if ( vals[0][0] != '#' ) {
      int i = vals[0].toInt();
      for ( int ch = 0; ch < MaxSSDs; ch++ ) {
	double E = vals[ ch * 2 + 1 ].toDouble();
	int count = vals[ ch * 2 + 2 ].toInt();
	MCAEs[ch][i] = E;      // これは測定時にそのピクセルに対応していたエネルギー
	MCAs0[ch][i] = count;
      }
    }
  }
#endif

  if ( ! aMCA.isValid() )
    return;
  aMCA.correctE( k2p );
#if 0  
  // 測定時のエネルギーとピクセルの関係は、
  // 今のエネルギーとピクセルの関係とは違っている可能性があるので
  // 今のピクセルに対応するエネルギーでのカウント数を線形補完で求めておく
  for ( int i = 0; i < MCALength; i++ ) {
    for ( int ch = 0; ch < MaxSSDs; ch++ ) {
      double nowE = k2p->p2E( ch, i );
      int j;
      for ( j = 1; j < MCALength - 1; j++ ) {
	if ( nowE < MCAEs[ch][j] ) {
	  break;
	}
      }
      MCAs[ch][i] = (int)(( nowE - MCAEs[ch][j-1] ) / ( MCAEs[ch][j] - MCAEs[ch][j-1] )
			  * ( (int)MCAs0[ch][j] - (int)MCAs0[ch][j-1] )
			  + MCAs0[ch][j-1] );
    }
  }
#endif

  aMCA.copyCnt( cMCACh, cMCA );
#if 0
  for ( int i = 0; i < MCALength; i++ ) {
    cMCA[i] = MCAs[cMCACh][i];
  }
#endif

  emit setMCACh( cMCACh );
  theMCAView->SetMCACh( cMCACh );
  MCADataIsValid = true;
  theMCAView->update();
}

#if 0
void Data::getNewMCAs( int MCALength )
{
  while( MCAs.count() > 0 ) {
    delete [] MCAs[0];
    MCAs.remove( 0 );
  }
  for ( int i = 0; i < MaxSSDs; i++ ) {
    quint32 *amca = new quint32 [ MCALength ];
    MCAs << amca;
  }

  while( MCAs0.count() > 0 ) {
    delete [] MCAs0[0];
    MCAs0.remove( 0 );
  }
  for ( int i = 0; i < MaxSSDs; i++ ) {
    quint32 *amca = new quint32 [ MCALength ];
    MCAs0 << amca;
  }

  while( MCAEs.count() > 0 ) {
    delete [] MCAEs[0];
    MCAEs.remove( 0 );
  }
  for ( int i = 0; i < MaxSSDs; i++ ) {
    double *amcaE = new double [ MCALength ];
    MCAEs << amcaE;
  }
}
#endif

void Data::SelectedNewMCACh( int ch ) 
{
  if ( ! MCADataIsValid ) return;

  cMCACh = ch;
  aMCA.copyCnt( cMCACh, cMCA );
#if 0
  for ( int i = 0; i < MCALength; i++ ) {
    cMCA[i] = MCAs[ cMCACh ][i];
  }
#endif
  theMCAView->SetMCACh( cMCACh );
  theMCAView->update();
}

void Data::showS2DData( QTextStream &in,
			QVector<AUnit*> &AMotors, QVector<AUnit*> & /* ASensors */ )
{
  QStringList HeadLine1, HeadLine2, vals;
  QString line;

  theS2DB = (S2DB*)(theViewC->getView());
  theS2DB->setLoadBHidden( false );
  theS2DView = (S2DView*)(theS2DB->getView());
#if 0
  if ( theViewC->getNowDType() == NONDATA ) {
    theViewC->setNowDType( S2DSHOW );
    theViewC->setNowVType( S2DVIEW );
    // theS2DView->SetWindow0( 1e300, 0, -1e300, 0 );
  }
#endif
  theS2DView->setRatioType( AS_SCREEN );

  S2DInfo s2di;
  s2di.load( in, AMotors );
  theS2DB->setS2DI( s2di );
  theS2DView->setRange( s2di.sx[0], s2di.sx[1] - s2di.dx[1] / 2,
			s2di.dx[0], s2di.dx[1],
			s2di.ps[0], s2di.ps[1] + 1 );
  int ix = 0, iy = 0;
  while ( ! in.atEnd() ) {
    vals = in.readLine().simplified().split( QRegExp( "\\s+" ) );
    if ( vals.count() > 0 ) {
      if ( vals[0][0] != '#' ) {
	if ( vals.count() >= 3 ) {
	  theS2DView->setData( ix, iy, vals[2].toDouble() );
	  ix++;
	} else {
	  ix = 0;
	  iy++;
	}
      }
    }
  }
}

