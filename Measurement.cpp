#include <QWidget>

#include "MainWindow.h"
#include "Stars.h"

/* �ȉ� Stars �Ƃ̃C���^�[�t�F�C�X�֐� */

double MainWindow::GetCurPosKeV( void ) // ������̌��݈ʒu�̖₢���킹(keV�P�ʂŕԓ�)
{                                 /* keV �P�ʂ̎w��͕���������������Ƃ��Ȃǖ�肩�� */
  double rv;
  char *rrv;

  rv = atof( rrv = sks->GetValue( MONO1 ) );  // �����ŃG���[���E���ׂ�

  return rv;
}

void MainWindow::MoveCurPosManualKeV( double keV ) // ������̈ړ��w��(keV�P�ʂňʒu�w��)
{
  if ( inMeas == 1 )
    return;
  if ( inMove == 0 ) {
    MoveCurPosKeV( keV );
  } else {
    // (���݈ʒu��V�����ړ���Ɏw�� ����Ŏ~�܂�? )
    NewLogMsg( QString( tr( "Mono: Stop Moving\n" ) ) );

    sks->Stop( MONO1 );
  }
}

void MainWindow::MoveCurPosKeV( double keV ) // ������̈ړ��w��(keV�P�ʂňʒu�w��)
{
  if ( keV2deg( keV ) > 0 ) {
    if ( inMeas == 0 ) {
      NewLogMsg( QString( tr( "Mono: Move %1 to %2 keV\n" ) )
		 .arg( CurPosKeV ).arg( keV ) );
    }
    sks->SetValue( MONO1, keV );
  } else {
    statusbar->showMessage( tr( "Distination is out of range." ), 4000 );
  }
  inMove = 1;
}

int MainWindow::isFinishedCurMove( void )   // ������̈ړ��I���̖₢���킹
{
  int rv = sks->IsBusy( MONO1 );
  if ( rv == -1 )
    statusbar->showMessage( tr( "Cannot get Monochro moving status." ), 5000 );

  return 1 - rv;                   // ���̖߂�l 0 : Busy, 1: Ready, 2: Error
}

void MainWindow::MeasureICH( int devNo, double dwell ) // �Ƃ���(����)�f�o�C�X�̒l��
                                                       // dwell ����(�ϕ�����)���肷��
{
  sks->StartMeas( "ion-ch", dwell, devNo - IONCH0 );
}

void MainWindow::MeasureSSD( double dwell ) // �Ƃ���(����)�f�o�C�X�̒l��
                                                       // dwell ����(�ϕ�����)���肷��
{
  sks->StartMeas( "SSD", dwell );
}

void MainWindow::MeasureAUX( int devNo, double dwell ) // �Ƃ���(����)�f�o�C�X�̒l��
                                                       // dwell ����(�ϕ�����)���肷��
{
  sks->StartMeas( "aux-in", dwell, devNo - AUXCH0 );
}

int MainWindow::isFinishedMeasICH( int devNo )
                                              // �Ƃ���f�o�C�X���w�肳�ꂽ�ϕ����Ԃ�
                                              // ������I�������ǂ����̖₢���킹
{
  int rv = sks->IsBusy( "ion-ch", devNo - IONCH0 );
  if ( rv == -1 )
    statusbar->showMessage( tr( "Cannot get ion chamber measurement status." ), 5000 );

  return 1 - rv;                   // ���̖߂�l 0 : Busy, 1: Ready, 2: Error
}

int MainWindow::isFinishedMeasSSD( void )
                                              // �Ƃ���f�o�C�X���w�肳�ꂽ�ϕ����Ԃ�
                                              // ������I�������ǂ����̖₢���킹
{
  int rv = sks->IsBusy( "SSD" );
  if ( rv == -1 )
    statusbar->showMessage( tr( "Cannot get ion chamber measurement status." ), 5000 );

  return 1 - rv;                   // ���̖߂�l 0 : Busy, 1: Ready, 2: Error
}

int MainWindow::isFinishedMeasAUX( int devNo )
                                              // �Ƃ���f�o�C�X���w�肳�ꂽ�ϕ����Ԃ�
                                              // ������I�������ǂ����̖₢���킹
{
  int rv = sks->IsBusy( "aux-in", devNo );
  if ( rv == -1 )
    statusbar->showMessage( tr( "Cannot get ion chamber measurement status." ), 5000 );

  return 1 - rv;                   // ���̖߂�l 0 : Busy, 1: Ready, 2: Error
}


void MainWindow::ReadOutTransData( double NowKeV ) // ���ߖ@�̑��茋�ʂ𓾂ċL�^����
{
  double I0 = atof( sks->GetValue( "ion-ch", IONCH0 - IONCH0 ) );
  double I = atof( sks->GetValue( "ion-ch", IONCH1 - IONCH0 ) );
  NowView->NewPoint( 0, NowKeV, I0 );
  NowView->NewPoint( 1, NowKeV, log( I0/I ) );
}

void MainWindow::ReadOutSSDData( double NowKeV ) // �u���@�̑��茋�ʂ𓾂ċL�^����
{
  double I0 = atof( sks->GetValue( "ion-ch", IONCH0 - IONCH0 ) );
  QString rv = sks->GetValue( "SSD" );
  QStringList rvs = rv.simplified().split( QChar( ' ' ) );
  printf( "SSD rv[0] = %f, rv = %s\n", rvs[0].toDouble(), rv.toAscii().data() );

  double sum = 0;
  NowView->NewPoint( 0, NowKeV, I0 );
  for ( int i = 0; i < 19; i++ ) {
    sum += rvs[i].toDouble();
    NowView->NewPoint( i+2, NowKeV, rvs[i].toDouble() / I0 );
  }
  NowView->NewPoint( 1, NowKeV, sum / I0 );
}

void MainWindow::ReadOutAUXData( int devNo, double NowKeV )
                                         // �O�����͂��g�������茋�ʂ𓾂ċL�^����
{
  double I0 = atof( sks->GetValue( "ion-ch", IONCH0 - IONCH0 ) );
  double I = atof( sks->GetValue( "aux-in", devNo - AUXCH0 ) );
  NowView->NewPoint( 0, NowKeV, I0 );
  NowView->NewPoint( 1, NowKeV, log( I0/I ) );
}
