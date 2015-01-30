
#include <QDebug>

#include "XafsM.h"
#include "Conditions.h"
#include "gitlog.h"

Conditions::Conditions( void ) : QFrame()
{
  setupUi( this );

  DataRoot0 = "";
  
  VersionInfo->setText( QString( "Ver. %1, Compiled Date : %2, Time : %3" )
			.arg( VERSION )
			.arg( __DATE__ )
			.arg( __TIME__ ) );

  connect( CalibDXMPMC, SIGNAL( clicked() ), this, SIGNAL( SetDXMPMC() ),
	   Qt::UniqueConnection );
  connect( SaveDTh1TTable, SIGNAL( clicked() ), this, SIGNAL( AskToSaveDTh1TTable() ),
	   Qt::UniqueConnection );
  connect( ShowDTh1TTable, SIGNAL( clicked() ), this, SIGNAL( AskToShowDTh1TTable() ),
	   Qt::UniqueConnection );
  connect( SelectDiffType1, SIGNAL( currentIndexChanged( int ) ),
	   this, SIGNAL( NewDiff1( int ) ), Qt::UniqueConnection );
  connect( SelectDiffType2, SIGNAL( currentIndexChanged( int ) ),
	   this, SIGNAL( NewDiff2( int ) ), Qt::UniqueConnection );
}

void Conditions::setVersionInfo( QString ver, QString date, QString time )
{
  QStringList GitLogs = QString( GITLOGS ).split( "\n" );
  
  VersionInfo
    ->setText( QString( "Ver. %1, Compiled Date : %2, Time : %3, Branch : %4\n" )
	       .arg( ver )
	       .arg( date )
	       .arg( time )
	       .arg( BRANCH_NAME )
	       );
  for ( int i = 0; i < GitLogs.count(); i++ ) {
    ShowGitLogs->append( GitLogs[i].simplified() );
  }
  QTextCursor qtc = ShowGitLogs->textCursor();
  qtc.movePosition( QTextCursor::Start );
  ShowGitLogs->setTextCursor( qtc );
}

void Conditions::setupDataRoot( void )
{
  DataRootSelect = new QFileDialog;
  DataRootSelect->setFileMode( QFileDialog::Directory );

  connect( DataRoot, SIGNAL( textChanged( const QString & ) ),
	   this, SIGNAL( newDataRoot( const QString & ) ),
	   Qt::UniqueConnection );

  if ( DataRoot0 != "" ) {          // DataRoot の指定があった
    QFileInfo droot( DataRoot0 );
    if ( droot.exists() ) {   // 指定された名前があった
      if ( !droot.isDir() ) { // 指定された名前がファイルだった (ディレクトリではなかった)
	DataRoot0 = "";      // 指定は無かったことにする
      }
    } else {                  // 指定された名前が無のディレクトリを作るように努力する
      QDir newdir;
      if ( ! newdir.mkpath( DataRoot0 ) ) { // 作れなかったら
	DataRoot0 = "";       // 指定は無かったことにする
      }
    }
  }
  if ( DataRoot0 == "" ) { // 上の if の中で DataRoot0="" にする可能性があるので改めて訊く
    DataRoot0 = QDir::currentPath(); 
  }

  DataRootSelect->setDirectory( DataRoot0 );
  DataRoot->setText( DataRoot0 );  // この signal で newDataRoot を呼ぶ
//  EditDFName->setText( QFileInfo( DataRoot0, EditDFName->text() ).filePath() );

  connect( SelDataRoot, SIGNAL( clicked() ), DataRootSelect, SLOT( show() ),
	   Qt::UniqueConnection );
  connect( DataRootSelect, SIGNAL( directoryEntered( const QString & ) ),
	   DataRoot, SLOT( setText( const QString & ) ),
	   Qt::UniqueConnection );
}

bool Conditions::isEncAsTh( void )
{
  return SelEncAsTh->isChecked();
}

bool Conditions::isAddInfos( void )
{
  return SelAddInfos->isChecked();
}

bool Conditions::isRecordAllSSDChs( void ) 
{
  return RecordAllSSDChs->isChecked();
}

bool Conditions::isRecordEachMCASpectrum( void ) 
{
  return RecordEachMCASpectrum->isChecked();
}

void Conditions::RecordEachMCASpectrumEnabled( bool f ) 
{
  return RecordEachMCASpectrum->setChecked( f );
}

bool Conditions::isMakeInfo( void ) 
{
  return MakeInfoFile->isChecked();
}

bool Conditions::isMeasInDeg( void )
{
  return SelStepInDegree->isChecked();
}

bool Conditions::isUseDTh1TTable( void )
{
  return UseDTh1TTable->isChecked();
}

int Conditions::Diff1Type( void )
{
  return SelectDiffType1->currentIndex();
}

int Conditions::Diff2Type( void )
{
  return SelectDiffType2->currentIndex();
}

bool Conditions::tuneESAbs( void )
{
  return TuneESAbs->isChecked();
}

double Conditions::tuneESStart( void )
{
  return TuneESStart->text().toDouble();
}

double Conditions::tuneESEnd( void )
{
  return TuneESEnd->text().toDouble();
}

int Conditions::tuneESSteps( void )
{
  return TuneESSteps->text().toInt();
}

bool Conditions::tuneESQuick( void )
{
  return QuickScanToGoMax->isChecked();
}

double Conditions::tuneESQuickTime( void )
{
  return QuickScanTimeToGoMax->text().toDouble();
}

bool Conditions::I0ShouldBeChecked( void )
{
  return CheckI0BeforeMove->isChecked();
}

double Conditions::I0Threshold( void )
{
  return I0ThresholdInput->text().toDouble();
}
