#ifndef MSGTYPE_H
#define MSGTYPE_H

#include <QString>

enum MSGTYPE {
  LISTNODES,

  // PM16C and keithley
  GETVALUE, 
  GETSPEEDSELECTED,
  SPEEDHIGH,
  SPEEDMIDDLE,
  SPEEDLOW, 
  SETHIGHSPEED, 
  GETHIGHSPEED, 
  SETTIMINGOUTMODE,
  SETTIMINGOUTSTART,
  SETTIMINGOUTEND,
  SETTIMINGOUTINTERVAL,
  SETTIMINGOUTREADY,
  SELECT,
  STOP,
  REMOTE,
  STOPEMERGENCY,
  STANDBY,
  RESET,
  SETDATAFORMAT,
  SETZEROCHECK,
  SETAUTORANGE,
  SETNPLCYCLES,
  RUN,
  SETRANGE,
  GETRANGE,
  SCANCW,
  SCANCCW,
  READ,
  ISBUSY,

  // piezo
  SETABSOLUTEVOLTAGE,
  SETABSOLUTEPOSITION,
  SETRELATIVEVOLTAGE,
  SETRELATIVEPOSITION,
  SETSPEEDHIGH,
  SETSPEEDMIDDLE,
  SETSPEEDLOW,

  // NCT08
  SETSTOPMODE,
  SETTIMERPRESET,
  COUNTERRESET,
  COUNTSTART,

  // MCA SSD
  SETPRESETTYPE,
  SETPRESETVALUE,
  GETVALUES,
  RUNSTART,
  RUNSTOP,
  RESUME,
  GETREALTIME,
  GETLIVETIME,
  GETSTATUS,
  SETROI,
  SETROIS,
  SETCALIBRATION,
  SETTHRESHOLD,
  SETPEAKINGTIME,
  SETDYNAMICRANGE,
  GETCALIBRATION,
  GETTHRESHOLD,
  GETPEAKINGTIME,
  GETDYNAMICRANGE,
  GETPREAMPGAIN,
  GETMCALENGTH,
  GETMCA,
  GETSTATISTICS,
  GETDATALINKCH,
  GETMCAS,

  // ortec974
  // RESET,          //  already is
  SETMODE,
  SETCOUNTPRESET,
  // COUNTERRESET,   // already is 
  // RUN,            // already is
  // STOP,           // already is
  // GETVALUE,       // already is
  // _EvChangedIsBusy  // already is
  // _EvChangedValue   // already is

  // 34410
  SETAPERTURE,
  SETTRIGGERDELAY,
  SETSAMPLINGSOURCE,
  SETTRIGGERSOURCE,
  SETTRIGGERCOUNTS,
  SETTRIGGERSLOPE,
  GETDATAPOINTS,
  READDATAPOINTS,
  ABORT,
  SETAUTOZERO,
  RECORD,
  QINITIALIZE,
  QGETDATA,
  QFINALIZE,

  // ENC2
  GETDATA,
  GETSTAT,
  TRIGGER,

  EvCONNECTED,
  EvDISCONNECTED,
  EvCHANGEDVALUE,
  EvISBUSY,
  EvREPORTCURRENT,
  EvREPORTINJECTION,
  NOTMSG,
};


struct MsgTyp {
  QString T;
  MSGTYPE V;
};

extern const MsgTyp MsgT[];

#endif

