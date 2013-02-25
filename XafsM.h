#ifndef XAFSM_H
#define XAFSM_H

/******************************************************************************/
/* Don't change these numbers ! without deep considerations.                  */
/* Window design of the program is assuming these values are as defined here. */
/******************************************************************************/
const int GOS = 4;            // Presetted Go To Positions
const int MaxBLKs = 8;        // Max Blocks
const int MaxSSDs = 19;       // Max SSD elements
/******************************************************************************/

extern QString DefFileName;
extern bool newFluoMode;

//const int TicPDiv = 50;   // ÉÇÉjÉ^Å[éûÇÃ 1årê¸ä‘Ç…ë™íËÇ∑ÇÈâÒêî
const int DivPScr = 6;    // Ç±ÇÃï”ÇÇ©Ç¶ÇÈÇ∆Ç´ÇÕ XView.h ÇÃ RingMax Ç…Ç‡íçà”
struct MSCALED {
  int MSid;
  const QString MSName;
  const int div;
  const int dispDiv;
  const QString unit;
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

enum MEASMODE{ I0 = 1, TRANS, FLUO, EYIELD, EXTRA, MEASMODES };
extern const QString CMode[ MEASMODES + 1 ];

enum MEASCHNAME{ MC_I0, MC_I1, MC_SSD, MC_AUX1, MC_AUX2, MCHANNELS };
enum MEASCHMODE{ MCM_I0, MCM_TRANS, MCM_FLUO, MCM_EY, MCMODES };

enum MSPEED { LOW, MIDDLE, HIGH, MSPEEDS };
struct MSPEEDD {
  MSPEED MSid;
  const QString MSName;
};
const MSPEEDD MSpeeds[ MSPEEDS ] = {
  { LOW,    "Low" },
  { MIDDLE, "Midium" },
  { HIGH,   "High" },
};

struct UNITNAME {
  QString name;
  const char *form;
};
enum UNIT { EV, KEV, ANGS, DEG, UNITS };
const UNITNAME UnitName[ UNITS ] = {
  { "eV",  (const char*)"% 7.1f", },
  { "keV", (const char*)"% 7.4f", },
  { "A",   (const char*)"% 7.4f", },
  { "deg", (const char*)"% 7.4f", },
};

enum ONFIN { RETURN, STAY, ONFINS };

enum RELABS { REL, ABS };

#endif
