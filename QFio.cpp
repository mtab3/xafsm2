#include <QtGui>
#include <math.h>

#include "MainWindow.h"

#define BLNAME    ( "BL5S1" )

void MainWindow::SetDFName2( int rpt, DIRECTION dir, int type )
{
  QString buf;
  QString ext;

  switch( type ) {
  case 0: // normal type
    ext = "";
    break;
  case 1: // simulate Step-Scan
    ext = "-S";
    break;
  }
  if ( SvSelExtPattern || ( SelRPT->text().toInt() == 1 ) ) {
    if ( rpt == 1 ) {
      if ( dir == FORWARD ) {
        DFName = DFName0 + DFName00 + "-f" + ext + ".dat";
      } else {
        DFName = DFName0 + DFName00 + "-b" + ext + ".dat";
      }
    } else {
      buf.sprintf( ".%04d", rpt - 1 );
      if ( dir == FORWARD ) {
        DFName = DFName0 + DFName00 + "-f" + ext + buf;
      } else {
        DFName = DFName0 + DFName00 + "-b" + ext + buf;
      }
    }
  } else {
    buf.sprintf( "-%04d.dat", rpt - 1 );
    if ( dir == FORWARD ) {
      DFName = DFName0 + DFName00 + "-f" + ext + buf;
    } else {
      DFName = DFName0 + DFName00 + "-b" + ext + buf;
    }
  }
}

void MainWindow::MakeDelegateFile( void )
{
  QFile file( DFName0 + ".dat" );
  if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;
  
  // Writing fixed headers
  QTextStream out(&file);

  out << "#" << " 1304     AichiSR QXAFS base file" << endl;
  out << "#" << " Unit      : " << UnitName[ SBLKUnit ].name;
  out << "#" << " Start     : " << SBlockStartAsDisp[0] << endl;
  out << "#" << " End       : " << SBlockStartAsDisp[1] << endl;
  out << "#" << " Dir       : " << ( (QMeasOnBackward->isChecked())
				     ? "Both(Forward and Backward" : "Single(Forward)" )
                                << endl;
  out << "#" << " Points    : " << SBlockPoints[0] << endl;
  out << "#" << " MeasTime  : " << QString::number( QXafsDwellTime, 'f', 10 )
                                << "[s]" << endl;
  out << "#" << " SweepTime : " << SBlockDwell[0] << "[s]" << endl;
  out << "#" << " Repeats   : " << SelRPT->value() << "[cycles]" << endl;
  out << "#" << " Date      : " << QDateTime::currentDateTime()
                                     .toString("yy.MM.dd hh:mm") << endl;
  if ( SLS != NULL ) 
    out << "#" << " Ring Cur. : " << SLS->value().toDouble() << "[mA]" << endl;

  out << "#" << endl;
  out << "#" << " ---- QXafs PM setting parameters ---- " << endl;
  out << "#" << " Motion start point : " << QXafsSP << endl;
  out << "#" << " Motion end point   : " << QXafsEP << endl;
  out << "#" << " Pulse start point  : " << QXafsSP0 << endl;
  out << "#" << " Pulse stop point   : " << QXafsEP0 << endl;
  out << "#" << " Pulse interval     : " << QXafsInterval << endl;
  out << "#" << " Pulse number       : " << QXafsPoints << endl;
  out << "#" << " Pulse dwell time   : " << QXafsDwellTime << endl;
  out << "#" << " Run Up Time        : " << RunUpTime << endl;
  out << "#" << endl;

  file.close();
}

void MainWindow::WriteQHeader( int rpt, DIRECTION dir )
{
  // 通常のQXAFSのデータファイル
  SetDFName2( rpt, dir, 0 );   // Generate a file name with repitation number

  double sblkdwell = SBlockDwell[0];
  SBlockDwell[0] = QXafsDwellTime;

  WriteHeaderCore();

  SBlockDwell[0] = sblkdwell;

  // Step Scan 型のファイル
  if ( SvSaveQDataAsStepScan ) {
    SetDFName2( rpt, dir, 1 );   // Generate a file name with repitation number
    WriteHeaderCore();
  }
}

void MainWindow::WriteQHeader2( int rpt, DIRECTION dir )
{
  // 通常のQXAFSのデータファイル
  SetDFName2( rpt, dir, 0 );   // Generate a file name with repitation number
  WriteHeaderCore2();

  // Step Scan 型のファイル
  if ( SvSaveQDataAsStepScan ) {
    SetDFName2( rpt, dir, 1 );   // Generate a file name with repitation number
    WriteHeaderCore2();
  }
}

void MainWindow::WriteQBody( int rpt, DIRECTION dir )
{
  // 通常のQXAFSのデータファイル
  SetDFName2( rpt, dir, 0 );   // Generate a file name with repitation number
  WriteQBody1( dir );

  // Step Scan 型のファイル
  if ( SvSaveQDataAsStepScan ) {
    SetDFName2( rpt, dir, 1 );   // Generate a file name with repitation number
    WriteQBody2( dir );
  }
}

// 通常の QXAFS のデータファイル
// EIB741なし(Enc2なし)の場合は正しく動かない気がする
void MainWindow::WriteQBody1( DIRECTION /* dir */ ) // こっちは本当に dir は不要のはず
{
  int Us = mUnits.count();

  QStringList valsEnc;
  if ( Enc2 != NULL ) {
    valsEnc = Enc2->values();
    Us -= 1;
  } else {
    valsEnc.clear();
  }

  QVector<QStringList> vals;
  QVector<double> dark;
  int num = 100000000;
  for ( int i = 0; i < Us; i++ ) {
    vals << mUnits.at(i)->values();
    dark << mUnits.at(i)->getDark() * QXafsDwellTime;
    if ( num > vals[i][0].toInt() )
      num = vals[i][0].toInt();
  }
  if ( num > valsEnc[0].toInt() )
    num = valsEnc[0].toInt();

  NewLogMsg( tr( "QXafs data points [%1]." )
             .arg( num ) );

  QFile file( DFName );
  if ( !file.open( QIODevice::Append | QIODevice::Text ) ) {
    NewLogMsg( tr( "Can't open QXafs data file [%1] to write data body." )
               .arg( DFName ) );
    return;
  }

  QTextStream out( &file );

  int p = QXafsSP0;
  int d = QXafsInterval;
  int c = MMainTh->getCenter();
  double upp = MMainTh->getUPP();
  double encV0 = EncValue0.toDouble();
  double enc2V0 = Enc2Value0.toDouble();
  double deg, deg2;
  double upp2 = 0;
  QString buf, buf2;

  if ( Enc2 != NULL ) {
    upp2 = Enc2->getUPP();
  }

  // dir == FORWARD と dir == BACKWARD で違うはず !!!!
  // 大丈夫だった。 p は正しい始点、 d は符号有りなので OK
  for ( int i = 0; i < num; i++ ) {
    deg = ( p - d * i - c ) * upp; // pm16c14 のパルス値から計算
    deg2 = ( Enc2 == NULL ) ? deg : 
      encV0 + ( valsEnc[i+1].toInt() - enc2V0 ) * upp2;

    buf.sprintf( "%10.5f" "%10.5f" "%10.4f", deg, deg2, QXafsDwellTime );

    for ( int j = 0; j < Us; j++ ) {
      buf2.sprintf( " %9.6f", vals[j][i+1].toDouble() - dark[j] );  // 8.7 --> 9.6
      buf += buf2;
    }
    out << buf << endl;
  }

  file.close();
}

// Step Scan 型のデータファイル
// EIB741なし(Enc2なし)の場合は正しく動かない気がする
void MainWindow::WriteQBody2( DIRECTION /* dir */ )
// 結局 dir はいらんかった ?
// そんなことはない !!!! ほんとうは使わないとダメ (いまは backward の時におかしい)
{
  int Us = mUnits.count();

  QStringList valsEnc;
  if ( Enc2 != NULL ) {
    valsEnc = Enc2->values();
    Us -= 1;
  } else {
    valsEnc.clear();
  }

  QVector<QStringList> vals;
  QVector<double> dark;
  int num = 100000000;
  for ( int i = 0; i < Us; i++ ) {
    vals << mUnits.at(i)->values();
    dark << mUnits.at(i)->getDark() * QXafsDwellTime;
    if ( num > vals[i][0].toInt() )
      num = vals[i][0].toInt();
  }
  if ( num > valsEnc[0].toInt() )
    num = valsEnc[0].toInt();

  QFile file( DFName );
  if ( !file.open( QIODevice::Append | QIODevice::Text ) ) {
    NewLogMsg( tr( "Can't open QXafs data file as Step Scan [%1] to write data body." )
               .arg( DFName ) );
    return;
  }

  QTextStream out( &file );

  int p = QXafsSP0;
  int d = QXafsInterval;
  int c = MMainTh->getCenter();
  double upp = MMainTh->getUPP();
  double deg, deg2;
  double upp2 = 0;
  QString buf, buf2;

  if ( Enc2 != NULL ) {
    upp2 = Enc2->getUPP();
  }

#if 0
  SMeasInDeg = conds->isMeasInDeg();
  SBlocks = Blocks;
  SBLKUnit = BLKUnit;
  for ( i = 0; i < MaxBLKs; i++ ) {
    SBlockStartAsDisp[i] = BLKstart[i]->text().toDouble();
    SBlockStartInDeg[i] = u->any2deg( BLKUnit, SBlockStartAsDisp[i] );
    SBlockStepAsDisp[i] = BLKstep[i]->text().toDouble();
    SBlockPoints[i] = BLKpoints[i]->text().toInt();
    SBlockDwell[i] = BLKdwell[i]->text().toDouble();
  }
  SBlockStartAsDisp[i] = BLKstart[i]->text().toDouble();
  SBlockStartInDeg[i] = u->any2deg( BLKUnit, SBlockStartAsDisp[i] );
  for ( int i = 0; i < MaxBLKs; i++ ) {
    SBlockStepInDeg[i]
      = ( SBlockStartInDeg[i+1] - SBlockStartInDeg[i] ) / SBlockPoints[i];
  }
  SBlockStepInDeg[ MaxBLKs ] = 0;
#endif

  double encV0 = EncValue0.toDouble();
  double enc2V0 = Enc2Value0.toDouble();
  double xs, xe, dx, x0, x1;
  int blk, ps;

  int TotalPoints = 0;
  for ( int i = 0; i < SBlocks; i++ ) {
    TotalPoints += SBlockPoints[i];
  }

  double Deg[ TotalPoints ];
  double Deg2[ TotalPoints ];
  double DTime[ TotalPoints ];
  QVector<double> boxes[ TotalPoints ];
  double pinbox[ TotalPoints ];
  for ( int i = 0; i < TotalPoints; i++ ) {
    for ( int j = 0; j < Us; j++ ) {
      boxes[i][j] = 0;
    }
    DTime[i] = 0;
    pinbox[i] = 0;
  }

  xs = xe = dx = 0;
  for ( int i = 0; i < num; i++ ) {
    deg = ( p - d * i - c ) * upp; // pm16c14 のパルス値から計算
    i++;
    deg2 = ( Enc2 == NULL ) ? deg : 
      encV0 + ( valsEnc[i+1].toInt() - enc2V0 ) * upp2;
    if ( SMeasInDeg ) {
      x0 = deg;
      x1 = deg2;
    } else {
      x0 = u->deg2any( SBLKUnit, deg );
      x1 = u->deg2any( SBLKUnit, deg2 );
    }
    
    int SumPoints = 0;
    for ( blk = 0; blk < SBlocks; blk++ ) {
      if ( SMeasInDeg ) {
	xs = SBlockStartInDeg[ blk ];
	xe = SBlockStartInDeg[ blk + 1 ];
	dx = fabs( SBlockStepInDeg[ blk ] );
      } else {
	xs = SBlockStartAsDisp[ blk ];
	xe = SBlockStartAsDisp[ blk + 1 ];
	dx = fabs( SBlockStepAsDisp[ blk ] );
      }
      if ( xs > xe ) 
	dx = -dx;
      
      if ( xs < xe ) {
	if (( x1 >= xs )&&( x1 < xe ))
	  break;
      } else {
	if (( x1 <= xs )&&( x1 > xe ))
	  break;
      }
      SumPoints += SBlockPoints[ blk ];
    }
    for ( ps = 0; ps < SBlockPoints[ blk ]; ps++ ) {
      if ( xs < xe ) {
	if (( x1 >= ( xs + dx * ps ))&&( x1 < ( xs + dx * ( ps + 1 ) ) ))
	  break;
      } else {
	if (( x1 > ( xs + dx * ( ps + 1 ) ))&&( x1 <= ( xs + dx * ps ) ))
	  break;
      }
    }
    pinbox[ ps + SumPoints ]++;   // points in the box
    Deg[ ps + SumPoints ] = deg;
    Deg2[ ps + SumPoints ] = deg2;
    DTime[ ps + SumPoints ] += QXafsDwellTime;
    for ( int j = 0; j < Us; j++ ) {
      boxes[ ps + SumPoints ][ j ] += vals[j][i+1].toDouble() - dark[j];
    }
  }

  file.close();
}
