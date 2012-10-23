#ifndef MSGTYPE_H
#define MSGTYPE_H

#include <QString>

enum MSGTYPE {
  GETVALUE,                  // 0
  GETSPEEDSELECTED,
  SPEEDHIGH,
  SPEEDMIDDLE,
  SPEEDLOW,                  // 4
  STOP,
  REMOTE,
  STOPEMERGENCY,
  STANDBY,
  RESET,                     // 9
  SETDATAFORMAT,
  SETZEROCHECK,
  SETAUTORANGE,
  SETNPLCYCLES,
  RUN,                       // 14
  SCANCW,
  SCANCCW,
  READ,
  ISBUSY,

  SETABSOLUTEVOLTAGE,        // 19
  SETABSOLUTEPOSITION,
  SETRELATIVEVOLTAGE,
  SETRELATIVEPOSITION,
  SETSPEEDHIGH,
  SETSPEEDMIDDLE,            // 24
  SETSPEEDLOW,

  SETSTOPMODE,
  SETTIMERPRESET,
  COUNTERRESET,
  COUNTSTART,                // 29

  EvCHANGEDVALUE,
  EvISBUSY,
  NOTMSG,                    // 32
};

extern const QString MsgType[];
extern MSGTYPE MsgTypeV[];

#endif

