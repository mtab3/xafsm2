#include <QtGui>

#include "MainWindow.h"

int MainWindow::GetDFName0()
{
  DFName0 = EditDFName->text();

  if ( DFName0.isEmpty() ) {
    return 0;
  }

  printf( "DFName00 = [%s]\n", DFName0.toAscii().data() );
  if ( DFName0.toUpper().lastIndexOf( ".DAT" ) == DFName0.length() - 4 ) {
    DFName0 = DFName0.left( DFName0.length() - 4 );
    printf( "trans DFName0 = [%s]\n", DFName0.toAscii().data() );
  }
  printf( "DFName0 = [%s]\n", DFName0.toAscii().data() );

  return 1;
}

void MainWindow::SetDFName( int i )
{
  char buf[ 256 ];

  if ( i == 0 ) {
    DFName = DFName0 + ".dat";
  } else {
    sprintf( buf, ".%03d", i );
    DFName = DFName0 + QString( buf );
  }
}

void MainWindow::WriteHeader( int Rpt )
{
  SetDFName( Rpt );
  printf( "DFName = [%s]\n", DFName.toAscii().data() );
}

void MainWindow::RecordDataTrans( int device, double r )
{
  printf( "Record Data: %d %g\n", device, r );
}

void MainWindow::RecordDataSSD( int device, double r )
{
  printf( "Record Data: %d %g\n", device, r );
}

void MainWindow::RecordDataAUX( int device, double r )
{
  printf( "Record Data: %d %g\n", device, r );
}

