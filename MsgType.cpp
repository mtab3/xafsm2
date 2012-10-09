
#include <stdio.h>

#include "MsgType.h"

const QString MsgType[] = {
  "@GetValue",
  "@GetSpeedSelected",
  "@SpeedHigh",
  "@SpeedMiddle",
  "@SpeedLow",
  "@Stop",
  "@Remote",
  "@StopEmergency",
  "@Standby",
  "@Reset",
  "@SetDataFormatElements",
  "@SetZeroCheckEnable",
  "@SetAutoRangeEnable",
  "@SetNPLCycles",
  "@Run",
  "@ScanCw",
  "@ScanCcw",
  "@Read",
  "@IsBusy",

  "@SetAbsoluteVoltage",
  "@SetAbsolutePosition",
  "@SetRelativeVoltage",
  "@SetRelativePosition",
  "@SetSpeedHigh",
  "@SetSpeedMiddle",
  "@SetSpeedLow",

  "@SetStopMode",
  "@SetTimerPreset",
  "@CounterReset",
  "@CountStart",

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

  EvCHANGEDVALUE,
  EvISBUSY,
  NOTMSG,
};
