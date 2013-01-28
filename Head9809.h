#ifndef HEAD9809_H
#define HEAD9809_H

#include <QtGui>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTextStream>

class Head9809 : public QObject
{
  Q_OBJECT

  bool valid;

  QString ID;
  QStringList Location;
  QString FileName, DateTime1, DateTime2;
  QString Sample;
  QString Energy, Current1, Current2;
  QString CrystName, SpacingD, InitAngle;
  QString BeamLine, ModeName, ModeId, Repetition, Points;
  QString ParamFName, AngleOrEnergy;    // 1 : Angle, 2 : Energy
  bool Angle;
  QString Blocks;

  QStringList BLKNos, BLKStarts, BLKEnds, BLKSteps, BLKTimes, BLKNums;
  QString DevName, DevType, DevChannels;
  QStringList Modes;
  QStringList Offsets;

 public:
  Head9809();

  bool readFromStream( QTextStream &in );

  int getMode( void ) { return ModeId.toInt(); };
  QStringList getChModes( void ) { return Modes; };
};

#endif       // HEAD9809_H
