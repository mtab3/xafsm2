
#include <stdio.h>

#include "MsgType.h"

const QString MsgType[] = {
  "@GetValue",                 // 0
  "@GetSpeedSelected",
  "@SpeedHigh",
  "@SpeedMiddle",
  "@SpeedLow",                 // 4
  "@Stop",
  "@Remote",
  "@StopEmergency",
  "@Standby",
  "@Reset",                    // 9
  "@SetDataFormatElements",
  "@SetZeroCheckEnable",
  "@SetAutoRangeEnable",
  "@SetNPLCycles",
  "@Run",                      // 14
  "@ScanCw",
  "@ScanCcw",
  "@Read",
  "@IsBusy",

  "@SetAbsoluteVoltage",       // 19
  "@SetAbsolutePosition",
  "@SetRelativeVoltage",
  "@SetRelativePosition",
  "@SetSpeedHigh",
  "@SetSpeedMiddle",           // 24
  "@SetSpeedLow",

  "@SetStopMode",
  "@SetTimerPreset",
  "@CounterReset",
  "@CountStart",               // 29

  "_ChangedValue",
  "_ChangedIsBusy",
  "",
};

MSGTYPE MsgTypeV[] = {
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

  EvCHANGEDVALUE,
  EvISBUSY,
  NOTMSG,
};
