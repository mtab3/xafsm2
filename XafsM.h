#ifndef XAFSM_H
#define XAFSM_H

// #define VERSION "2.00"
#define VERSION "2.1 : 2D scan is ready"

#include <QString>
#include <QVector>

enum SCANMODE { STEP, QCONT, RCONT };
enum DIRECTION { FORWARD, BACKWARD };
enum MCASTARTRESUME { MCA_START, MCA_RESUME };
enum ENCORPM { XENC, XPM };
enum OLDNEW { OLD, NEW };

/******************************************************************************/
/* Don't change these numbers ! without deep considerations.                  */
/* Window design of the program is assuming these values are as defined here. */
/******************************************************************************/
const int GOS = 4;            // Presetted Go To Positions
const int MaxBLKs = 8;        // Max Blocks
const int MaxSSDs = 19;       // Max SSD elements
/******************************************************************************/

const QString RadioBOn = "background-color: rgb(250,250,200)";
const QString RadioBOff = "background-color: rgb(200,200,190)";
const QString NormalB = "background-color: "
		       "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 "
		       "rgba(225, 235, 225, 255), stop:1 "
                       "rgba(255, 255, 255, 255));";
const QString InActive = "background-color: rgb(250,250,180)"; 
const QString AlartRed = "background-color: rgb(250,200,200)";
const QString SSDActive = "background-color: #f0f0d0";
const QString SSDnotActive = "";
const QString EDITABLELINE = "background-color: rgb(255,255,255)";
const QString NONEDITABLELINE = "background-color: rgb(230,230,230)";
//const QString SSDActive = RadioBOn;
//const QString SSDnotActive = RadioBOff;

// ファイル名とデータの新旧に応じたステータスを色で表現
// data, file-name 
//  old     old    : 実質このステータスは使っていない
//                   全く何も問題がない
//                   --> 白系(色を付けるとしてもできるだけ淡色)
//  old     new    : 測定前でファイル名だけ入れた。
//                   あまり危険ではない(最悪古いデータが保存された別ファイルが出来るだけ)
//                   ---> 黄色系の色
//  new     old    : 測定後なのにファイル名が古い。
//                   かなり危険、前回の測定データを上書きで消す可能性あり
//                   ---> 赤系の色
//  new     new    : 危険性は低いがセーブし忘れるとデータを失うので注意を促す必要はある
//                   ---> 青/緑系の色 (できれば青系か)

const QString FILE_OLDOLD  = "background-color: rgb(250,255,240)"; // old-data, old-name
const QString FILE_NEWNAME = "background-color: rgb(250,250,190)"; // old-data, new-name
const QString FILE_NEWDATA = "background-color: rgb(255,200,200)"; // new-data, old-name
const QString FILE_NEWNEW  = "background-color: rgb(220,240,255)"; // new-data, new-name

const QString FSTATCOLORS[2][2] =
  { { FILE_OLDOLD, FILE_NEWNAME }, { FILE_NEWDATA, FILE_NEWNEW } };

#define PBTrue    ( false )   // PushButton を Checkable にすると、押した時、暗いのに
#define PBFalse   ( true  )   // true になるので、定数名で反転させて使う

enum ASType { EACHSCAN, SUMUPOFSCANS };   // after save type

extern QString DefFileName;
extern bool newFluoMode;

//const int TicPDiv = 50;   // モニター時の 1罫線間に測定する回数
const int DivPScr = 6;    // この辺をかえるときは XView.h の RingMax にも注意
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
  { MIDDLE, "Middle" },
  { HIGH,   "High" },
};


enum ONFIN { RETURN, STAY, ONFINS };

enum RELABS { REL, ABS };

struct GasComp {
  QString GasForm;
  double comp;
};

struct Gas {
  QString Name;
  QVector<GasComp*> GasComps;
};

struct IonChLength {
  QString Name;
  QString ID;
  double length;
  QString UName;
};

struct MCAGain {
  int ch;
  double gain;
};

#endif
