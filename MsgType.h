#ifndef MSGTYPE_H
#define MSGTYPE_H

#include <QString>

enum MSGTYPE {
  LISTNODES,

  GETVALUE, 
  GETSPEEDSELECTED,
  SPEEDHIGH,
  SPEEDMIDDLE,
  SPEEDLOW, 
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
  SCANCW,
  SCANCCW,
  READ,
  ISBUSY,

  SETABSOLUTEVOLTAGE,
  SETABSOLUTEPOSITION,
  SETRELATIVEVOLTAGE,
  SETRELATIVEPOSITION,
  SETSPEEDHIGH,
  SETSPEEDMIDDLE,
  SETSPEEDLOW,

  SETSTOPMODE,
  SETTIMERPRESET,
  COUNTERRESET,
  COUNTSTART,

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

  EvCONNECTED,
  EvDISCONNECTED,
  EvCHANGEDVALUE,
  EvISBUSY,
  NOTMSG,
};


struct MsgTyp {
  QString T;
  MSGTYPE V;
};

extern const MsgTyp MsgT[];

#endif

