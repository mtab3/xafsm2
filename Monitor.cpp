#include "MainWindow.h"

void MainWindow::MonSequence( void )
{
  if ( mUnits.isBusy() )
    return;

  switch( MonStage ) {
    /* 
       0: �ͤ��ɤ߽Ф���ɽ��
       1: nct08 ��Ȥ���: ��¬���Ϥ�������
          ����ʳ�: ��¬����
       2: nct08 ��Ȥ���: ��¬����
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
    mUnits.readValue( MeasVals );
    MonitorView->NewPointR( etime = MonTime.elapsed(),
			    MeasVals[0], MeasVals[1], MeasVals[2] );
    MonitorView->ReDraw();
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
    MonitorView->ReDraw();
    if ( inPause == 0 ) {
      MonStage = 3;
    }
    break;
  }
}

