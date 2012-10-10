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
  QFile file( DFName.toAscii() );
  if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;
  // Writing fixed headers
  QTextStream out(&file);
  out << " 9809      ChubuSR  BL5S1 " << endl;
  out << " " + QFileInfo(DFName0).fileName() + ".dat         "
    + QDateTime::currentDateTime().toString("yy.MM.dd hh:mm") + " - " << endl;
  out << EditCMT->text() << endl;
  out << " Ring :   1.2 GeV    " << "300.0 mA - " << "300.0 mA" << endl;
  out << " Mono :   Si(111)     D= 3.13551 A    Initial angle="
      << " "  << "11.00000" << " deg" << endl;
  out << " BL5S1    " << "Transmission" << "( 2)" << "   Repitition= "
      << MeasR << "   Points= " << TP << endl;

  out << " Param file : ";
  out << "PARAMFILENAME";
  out << "   angle axis (";
  out << "PARAMUNIT";
  out << ") Block = ";
  out << Blocks << endl;

  out << endl;
  out << " Block      Init-Ang  Final-Ang     Step/deg     Time/s       Num" << endl;
  QString buf;
  for (int i = 0; i < Blocks; i++ ) {
      buf.sprintf( "     %d       %7.5f    %7.5f     %7.6f       %3.2f       %3d",
		   i+1,
		   SBlockStart[i],
		   SBlockStart[i+1],
		   BLKstep[i]->text().toDouble(),
		   SBlockDwell[i],
		   SBlockPoints[i]);
      out << buf << endl;
  }
  buf.sprintf(" Ortec( 0)      NDCH= 2");
  out << buf << endl;
  //  switch( SMeasMode ) {
  switch( TRANS ) {                     // !!!!!!!!!!!!!!!!! very hard coded !!!! 
  case TRANS:
    out << "  Angle(c)  Angle(o)   time/s          2         3" << endl;
    out << "      Mode         0        0          1         2" << endl;
    buf.sprintf("    Offset         0        0   %8.0f  %8.0f", 1.0, 2.0);
    out << buf << endl;
    break;
  case FLUO:
    out << "  Angle(c)  Angle(o)   time/s "
      "         1         2         3         4         5"
      "         6         7         8         9        10"
      "        11        12        13        14        15"
      "        16        17        18        19        20"
      "         1         2         3         4         5"
      "         6         7         8         9        10"
      "        11        12        13        14        15"
      "        16        17        18        19        20"
	<< endl; // hardcoded by H.Asakura

    out << "      Mode         0        0 ";
    for (int i = 0; i < 20; i++){
      // buf.sprintf("%10.0f", SMeasMode);
      buf.sprintf("%10d", 1);
      out << buf;
    }
    for (int i = 0; i < 20; i++){
      buf.sprintf("%10d", 100);
      out << buf;
    }
    out << endl;
    // Loop!
    out << "    Offset         0        0 ";
    for (int i = 0; i < 20; i++){
      buf.sprintf(" %9.3f", 1.0);
      out << buf;
    }

    out << endl;
    break;
  case AUX:
    // temporary
    out << "  Angle(c)  Angle(o)   time/s          2         3" << endl;
    out << "      Mode         0        0          1         2" << endl;
    buf.sprintf("    Offset         0        0   %8.0f  %8.0f", 1.0, 2.0);
    out << buf << endl;
    break;
  }
  file.close();
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


