
#include <stdio.h>

#include "MsgType.h"

const MsgTyp MsgT[] = {
  { "@GetValue", GETVALUE },
  { "@GetSpeedSelected", GETSPEEDSELECTED },
  { "@SpeedHigh", SPEEDHIGH },
  { "@SpeedMiddle", SPEEDMIDDLE },
  { "@SpeedLow", SPEEDLOW },
  { "@Stop", STOP },
  { "@Remote", REMOTE },
  { "@StopEmergency", STOPEMERGENCY },
  { "@Standby", STANDBY },
  { "@Reset", RESET },
  { "@SetDataFormatElements", SETDATAFORMAT },
  { "@SetZeroCheckEnable", SETZEROCHECK },
  { "@SetAutoRangeEnable", SETAUTORANGE },
  { "@SetNPLCycles", SETNPLCYCLES },
  { "@Run", RUN },
  { "@ScanCw", SCANCW },
  { "@ScanCcw", SCANCCW },
  { "@Read", READ },
  { "@IsBusy", ISBUSY },

  { "@SetAbsoluteVoltage", SETABSOLUTEVOLTAGE },
  { "@SetAbsolutePosition", SETABSOLUTEPOSITION },
  { "@SetRelativeVoltage", SETRELATIVEVOLTAGE },
  { "@SetRelativePosition", SETRELATIVEPOSITION },
  { "@SetSpeedHigh", SETSPEEDHIGH },
  { "@SetSpeedMiddle", SETSPEEDMIDDLE },
  { "@SetSpeedLow", SETSPEEDLOW },

  { "@SetStopMode", SETSTOPMODE },
  { "@SetTimerPreset", SETTIMERPRESET },
  { "@CounterReset", COUNTERRESET },
  { "@CountStart", COUNTSTART },

  { "@SetPresetType", SETPRESETTYPE },
  { "@SetPresetValue", SETPRESETVALUE },
  { "@GetValues", GETVALUES },
  { "@Start", SMSG_START },

  { "_ChangedValue", EvCHANGEDVALUE },
  { "_ChangedIsBusy", EvISBUSY },

  { "", NOTMSG },
};
