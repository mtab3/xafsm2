#ifndef XAFSM_H
#define XAFSM_H

struct MCCD {
  const char *MCName;
  double d;
};
enum MCC { Si111, Si333, Ge111, Ge333, MCCS };
const MCCD mccd[ MCCS ] = {
  { "Si(111)", 3.1355316, },
  { "Si(333)", 1.0451772, },
  { "Ge(111)", 3.2663822, },
  { "Ge(333)", 1.0887941, },
};

#if 0
struct MOTORD {
  const int Mid;
  const char *MName;
  const char *devName;
  const char *devCh;
  int upstep;
  int downstep;
  int SPSstep;
  double SPSdwell;
  int SPSstart;
  int SPSend;
};

enum MONOTR { MONOCHRO1, MOTOR1, MOTOR2, MOTOR3, MOTOR4, MOTORS };
extern MOTORD Motors[ MOTORS ];

struct MDEVD {
  int MDid;
  const char *MDName;
  const char *devName;
  const int devCh;
  const int devCh2;
};
enum MEASDEVICE {
  IONCH0, IONCH1,
  SSDCH01, SSDCH02, SSDCH03, SSDCH04, SSDCH05,
  SSDCH06, SSDCH07, SSDCH08, SSDCH09, SSDCH10,
  SSDCH11, SSDCH12, SSDCH13, SSDCH14, SSDCH15,
  SSDCH16, SSDCH17, SSDCH18, SSDCH19,
  SSDALL,
  AUXCH0, AUXCH1,
  MEASDEVICES,
};
const MDEVD MDevs[ MEASDEVICES ] = {
  { IONCH0,  "Ion Ch 0",   "ion-ch",  0,  0 },
  { IONCH1,  "Ion Ch 1",   "ion-ch",  1,  1 },
  { SSDCH01, "SSD Ch 01",  "SSD",     0, -1 },
  { SSDCH02, "SSD Ch 02",  "SSD",     1, -1 },
  { SSDCH03, "SSD Ch 03",  "SSD",     2, -1 },
  { SSDCH04, "SSD Ch 04",  "SSD",     3, -1 },
  { SSDCH05, "SSD Ch 05",  "SSD",     4, -1 },
  { SSDCH06, "SSD Ch 06",  "SSD",     5, -1 },
  { SSDCH07, "SSD Ch 07",  "SSD",     6, -1 },
  { SSDCH08, "SSD Ch 08",  "SSD",     7, -1 },
  { SSDCH09, "SSD Ch 09",  "SSD",     8, -1 },
  { SSDCH10, "SSD Ch 10",  "SSD",     9, -1 },
  { SSDCH11, "SSD Ch 11",  "SSD",    10, -1 },
  { SSDCH12, "SSD Ch 12",  "SSD",    11, -1 },
  { SSDCH13, "SSD Ch 13",  "SSD",    12, -1 },
  { SSDCH14, "SSD Ch 14",  "SSD",    13, -1 },
  { SSDCH15, "SSD Ch 15",  "SSD",    14, -1 },
  { SSDCH16, "SSD Ch 16",  "SSD",    15, -1 },
  { SSDCH17, "SSD Ch 17",  "SSD",    16, -1 },
  { SSDCH18, "SSD Ch 18",  "SSD",    17, -1 },
  { SSDCH19, "SSD Ch 19",  "SSD",    18, -1 },
  { SSDALL,  "SSD All Ch", "SSD",    -1, -1 },
  { AUXCH0,  "Aux Ch 0",   "aux-in",  0,  0 },
  { AUXCH1,  "Aux Ch 1",   "aux-in",  1,  1 },
};
#endif

struct SPSMODED {   // Scan or Peak Search
  int SPSMid;
  const char *SPSMName;
};
enum SPSMODE { PEAKSEARCH, SCAN, SPSMODES };
const SPSMODED SPSModes[ SPSMODES ] = {
  { PEAKSEARCH, "Peak Search" },
  { SCAN, "Scan" },
};

const int TicPDiv = 10;   // ÉÇÉjÉ^Å[éûÇÃ 1årê¸ä‘Ç…ë™íËÇ∑ÇÈâÒêî
const int DivPScr = 6;    // Ç±ÇÃï”ÇÇ©Ç¶ÇÈÇ∆Ç´ÇÕ XView.h ÇÃ RingMax Ç…Ç‡íçà”
struct MSCALED {
  int MSid;
  const char *MSName;
  const int div;
  const int dispDiv;
  const char *unit;
};
enum MSCALE { S10, S30, M01, M02, M05, M10, M30, H01, H02, H05, MSCALES };
const MSCALED MScales[ MSCALES ] = {
  { S10, "10 s/div",       10,  10, "[s]" },
  { S30, "30 s/div",       30,  30, "[s]" },
  { M01, "60 s/div",       60,   1, "[m]" },
  { M02, " 2 m/div",     2*60,   2, "[m]" },
  { M05, " 5 m/div",     5*60,   5, "[m]" },
  { M10, "10 m/div",    10*60,  10, "[m]" },
  { M30, "30 m/div",    30*60,  30, "[m]" },
  { H01, "60 m/div",    60*60,   1, "[h]" },
  { H02, " 2 h/div",  2*60*60,   2, "[h]" },
  { H05, " 5 h/div",  5*60*60,   5, "[h]" },
};

enum MEASMODE{ TRANS, FLUO, AUX, MEASMODES };

enum MSPEED { LOW, MIDIUM, HIGH, MSPEEDS };
struct MSPEEDD {
  MSPEED MSid;
  const char *MSName;
};
const MSPEEDD MSpeeds[ MSPEEDS ] = {
  { LOW,    "Low" },
  { MIDIUM, "Midium" },
  { HIGH,   "High" },
};

struct UNITNAME {
  const char *name;
  const char *form;
};
enum UNIT { EV, KEV, ANGS, DEG, UNITS };
const UNITNAME UnitName[ UNITS ] = {
  { (const char*)"eV",  (const char*)"% 7.1f", },
  { (const char*)"keV", (const char*)"% 7.4f", },
  { (const char*)"A",   (const char*)"% 7.4f", },
  { (const char*)"deg", (const char*)"% 7.4f", },
};

enum ONFIN { RETURN, STAY, ONFINS };

enum RELABS { REL, ABS };

#endif
