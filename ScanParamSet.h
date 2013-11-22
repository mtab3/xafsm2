#ifndef SCANPARAMSET_H
#define SCANPARAMSET_H

struct ScanPSet {
  QString Start;
  QString End;
  QString Step;
  QString Dwell;
  int Unit;
  bool UnitSelected;
  int Speed;
  int Sensor;
  int Sensor0;
  bool Sens0Selected;

  ScanPSet() {
    Start = "-100";
    End =    "100";
    Step =    "10";
    Dwell =  "1.0";
    Unit = 0;
    Speed = 1;
    UnitSelected = false;
    Sensor = Sensor0 = 0;
    Sens0Selected = false;
  };
};

#endif
