#include "MainWindow.h"

#include <QFile>
#include <QTextStream>

void MainWindow::MonSequence( void )
{
  if ( mUnits.isBusy() )
    return;

  switch( MonStage ) {
    /* 
       0: 値の読み出しと表示
       1: nct08 を使う時: 計測開始の前準備
          それ以外: 計測開始
       2: nct08 を使う時: 計測開始
    */
  case 0:
    mUnits.clearStage();
    MonStage = 1;
    break;
  case 1:
    if ( mUnits.init() == false ) {  // true :: initializing
      mUnits.clearStage();
      MonStage = 2;
    }
    break;
  case 2: 
    mUnits.setDwellTime();
    if ( mUnits.isParent() ) {
      MonStage = 3;
    } else {
      MonStage = 4;
    }
    break;
  case 3:
    if ( mUnits.getValue0() == false ) { // only for counters and SSDs
      mUnits.clearStage();
      MonStage = 4;
    }
    break;
  case 4:
    if ( mUnits.getValue() == false ) {  // true :: Getting
      mUnits.clearStage();
      MonStage = 5;
    }
    break;
  case 5:
    int etime;
    mUnits.readValue( MeasVals, MeasCPSs, false );   // false :: not correct dark
    MonitorView->NewPointR( etime = MonTime.elapsed(),
			    MeasVals[0], MeasVals[1], MeasVals[2] );
    MonitorView->update();

    if ( conds->isRecordAllSSDChs() ) {
      if ( SFluo != NULL ) {
	QFile f( "ssd.dat" );
	f.open( QIODevice::Append | QIODevice::Text );
	QTextStream out( &f );
	QVector<unsigned long> CinROI = SFluo->getCountsInROI();
	QVector<unsigned long> CAll =  SFluo->getCountsAll();
	QVector<unsigned long> TotalE = SFluo->getCountsAll();
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
      MonOut << (double)etime/1000;
      for ( int i = 0; i < mUnits.count(); i++ ) {
	MonOut << "\t" << MeasVals[i];
      }
      MonOut << "\t # " << QDateTime::currentDateTime().toString( "yy/MM/dd hh:mm:ss" );
      MonOut << "\n";
    }
    MonStage = 10;
#if 0
    if ( inPause == 1 ) {
      MonStage = 99;          // PauseStage
    }
#endif
    // don't break
  case 10:                     // This label is resume point from pausing
    MonitorView->update();
    if ( inPause == 0 ) {
      MonStage = 3;
    }
    break;
  }
}

