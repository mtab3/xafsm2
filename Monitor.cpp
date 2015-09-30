#include "MainWindow.h"

#include <QFile>
#include <QTextStream>

static int lastETime = -1;

void MainWindow::setLastETime( int let )
{
  lastETime = let;
}

void MainWindow::MonSequence( void )
{
  if ( mMonUnits.isBusy() )
    return;
  
  double monInt = MonIntT->text().toDouble();
  if (( MonStage == 5 )&&(  monInt > 0 )) {  // インターバル時間の指定があるばあい
    if ( lastETime > 0 ) {  // 前回の計測から指定の時間経っていなければ
                            // 次の計測ステージ(5)に進まない
      if ( (double)( MonTime.elapsed() - lastETime ) / 1000. < monInt )
	return;
    }
  }

  switch( MonStage ) {
    /* 
       0: 値の読み出しと表示
       1: nct08 を使う時: 計測開始の前準備
          それ以外: 計測開始
       2: nct08 を使う時: 計測開始
    */
  case 0:
    mMonUnits.clearStage();
    MonStage = 1;
    break;
  case 1:
    if ( mMonUnits.init() == false ) {  // true :: initializing
      mMonUnits.clearStage();
      MonStage = 2;
    }
    break;
  case 2: 
    mMonUnits.setDwellTime();
    if ( mMonUnits.isParent() ) {
      MonStage = 3;
    } else {
      MonStage = 4;
    }
    break;
  case 3:
    if ( mMonUnits.getValue0() == false ) { // only for counters and SSDs
      mMonUnits.clearStage();
      MonStage = 4;
    }
    break;
  case 4:
    if ( mMonUnits.getValue() == false ) {  // true :: Getting
      mMonUnits.clearStage();
      MonStage = 5;
    }
    break;
  case 5:
    int etime;
    mMonUnits.readValue( MeasVals, MeasCPSs, false );   // false :: not correct dark
    MonitorView->NewPointR( etime = MonTime.elapsed(),
			    MeasVals, mMonUnits.count() );
    MonitorView->update();
    lastETime = etime;
    
    if ( conds->isRecordAllSSDChs() ) {
      if ( SFluo != NULL ) {
	QFile f( "ssd.dat" );
	f.open( QIODevice::Append | QIODevice::Text );
	QTextStream out( &f );
	QVector<quint64> CinROI = SFluo->getCountsInROI();
	QVector<quint64> CAll =  SFluo->getCountsAll();
	QVector<quint64> TotalE = SFluo->getCountsAll();
	QVector<double> ICRs = SFluo->getICRs();
	QString buf;
	out << T->elapsed();
	for ( int i = 0; i < CinROI.count(); i++ )
	  buf += QString( " %1" ).arg( CinROI[i] );
	for ( int i = 0; i < CAll.count(); i++ )
	  buf += QString( " %1" ).arg( CAll[i] );
	for ( int i = 0; i < TotalE.count(); i++ )
	  buf += QString( " %1" ).arg( TotalE[i] );
	for ( int i = 0; i < ICRs.count(); i++ )
	  buf += QString( " %1" ).arg( ICRs[i] );
	out << buf << "\n";
	f.close();
      }
    }

    if ( monRecF ) {
      MonFile.setFileName( MonRecFile->text() );
      if ( MonFile.open( QIODevice::Append | QIODevice::Text ) ) {
	MonOut.setDevice( &MonFile );
	
	MonOut << (double)etime/1000;
	for ( int i = 0; i < mMonUnits.count(); i++ ) {
	  MonOut << "\t" << QString( "%1" ).arg( MeasVals[i], 11, 'f', 7 );
	}
	if ( SLS != NULL ) 
	  MonOut << "\t # " << SLS->value();
	MonOut << "\t # " << QDateTime::currentDateTime().toString( "yy/MM/dd hh:mm:ss" );
	MonOut << "\n";
      }
      MonFile.close();
    }
    MonStage = 10;
    if ( MinPause ) {
      MonStage = 99;          // PauseStage
    }
    // don't break
  case 10:                     // This label is resume point from pausing
    MonitorView->update();
    if ( !MinPause ) {
      MonStage = 3;
    }
    break;
  case 99:
    if ( !MinPause ) {
      MonStage = 3;
    }
    break;
  }
}

