
#include "Head9809.h"

Head9809::Head9809( void )
{
  valid = false;
}

bool Head9809::readFromStream( QTextStream &in )
{
  valid = false;

  QString line;
  QString HeaderEnd = "    Offset";

  qDebug() << "in read header 9809";
  if ( in.atEnd() ) return false;
  line = in.readLine().mid(1);                     // 1 行目
  ID = line.left(5).simplified();
  Location = line.mid( 10 ).split( QRegExp( "\\s+" ) );
  qDebug() << "Location : " << Location;
  if ( ID != "9809" ) return false;

  if ( in.atEnd() ) return false;
  line = in.readLine().mid(1);                     // 2 行目
  FileName = line.mid( 0, 14 ).simplified();
  DateTime1 = line.mid( 16, 14 ).simplified();
  DateTime2 = line.mid( 33, 14 ).simplified();
  qDebug() << "name dates" << FileName << DateTime1 << DateTime2;

  if ( in.atEnd() ) return false;
  line = in.readLine().mid(1);                     // 3 行目
  Sample = line.mid( 0, 80 ).simplified();
  qDebug() << "Sample description " << Sample;

  if ( in.atEnd() ) return false;
  line = in.readLine().mid(1);                     // 4 行目
  Energy = line.mid( 7, 5 ).simplified();
  Current1 = line.mid( 18, 6 ).simplified();
  Current2 = line.mid( 30, 6 ).simplified();
  qDebug() << "Eng. Cur. Cur." << Energy << Current1 << Current2;

  if ( in.atEnd() ) return false;
  line = in.readLine().mid(1);                     // 5 行目
  CrystName = line.mid( 9, 10 ).simplified();
  SpacingD = line.mid( 26, 8 ).simplified();
  InitAngle = line.mid( 54, 9 ).simplified();
  qDebug() << "Cryst D initAngle " << CrystName << SpacingD << InitAngle;

  if ( in.atEnd() ) return false;
  line = in.readLine().mid(1);                     // 6 行目
  BeamLine = line.mid( 0, 5 ).simplified();
  ModeName = line.mid( 9, 13 ).simplified();
  ModeId = line.mid( 23, 2 ).simplified();
  Repetition = line.mid( 40, 3 ).simplified();
  Points = line.mid( 55, 5 ).simplified();
  qDebug() << "BL ModeName Mode Rep. Points"
	   << BeamLine << ModeName << ModeId << Repetition << Points;

  if ( in.atEnd() ) return false;
  line = in.readLine().mid(1);                     // 7 行目
  ParamFName = line.mid( 13, 14 ).simplified();
  AngleOrEnergy = line.mid( 42, 1 ).simplified();
  if ( AngleOrEnergy == "1" ) {
    Angle = true;
  } else {
    Angle = false;
  }
  Blocks = line.mid( 56, 5 ).simplified();
  qDebug() << "PFName AorE isAorE Blocks "
	   << ParamFName << AngleOrEnergy << Angle << Blocks;

  if ( in.atEnd() ) return false;
  line = in.readLine().mid(1);                     // 8 行目
  if ( in.atEnd() ) return false;
  line = in.readLine().mid(1);                     // 9 行目

  for ( int i = 0; i < Blocks.toInt(); i++ ) {
    if ( in.atEnd() ) return false;
    line = in.readLine().mid(1);
    BLKNos    << line.mid( 0, 5 ).simplified();
    BLKStarts << line.mid( 10, 10 ).simplified();
    BLKEnds   << line.mid( 20, 10 ).simplified();
    BLKSteps  << line.mid( 30, 13 ).simplified();
    BLKTimes  << line.mid( 43, 11 ).simplified();
    BLKNums   << line.mid( 54, 10 ).simplified();
    qDebug()  << BLKNos.at(i) << BLKStarts.at(i) << BLKEnds.at(i)
	      << BLKSteps.at(i) << BLKTimes.at(i) << BLKNums.at(i);
  }

  if ( in.atEnd() ) return false;
  line = in.readLine().mid(1);
  DevName = line.mid( 0, 5 ).simplified();
  DevType = line.mid( 6, 2 ).simplified();
  DevChannels = line.mid( 20, 2 ).simplified();
  qDebug() << "DevName DevID DevChannels " << DevName << DevType << DevChannels;

  if ( in.atEnd() ) return false;
  line = in.readLine().mid(1);                     // comment line

  if ( in.atEnd() ) return false;
  line = in.readLine().mid(1);                     // Mode Line
  line = line.mid( 30 );                           // 最初の 30桁 (Mode 0 0) は読み飛ばす
  for ( int i = 0; i < line.length() / 10; i++ ) {
    Modes << line.mid( i*10, 10 ).simplified();
  }
  qDebug() << "Modes " << Modes;

  if ( in.atEnd() ) return false;
  line = in.readLine().mid(1);                   // Offset Line
  line = line.mid( 30 );                         // 最初の 30桁 (Offset 0 0) は読み飛ばす
  for ( int i = 0; i < line.length() / 10; i++ ) {
    Offsets << line.mid( i*10, 10 ).simplified();
  }
  qDebug() << "Offsets " << Offsets;
  
  return true;
}



