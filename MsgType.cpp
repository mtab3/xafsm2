
#include <stdio.h>

#include "MsgType.h"

const MsgTyp MsgT[] = {
  { "@listnodes", LISTNODES },

  { "@GetValue", GETVALUE },
  { "@GetSpeedSelected", GETSPEEDSELECTED },
  { "@SpeedHigh", SPEEDHIGH },
  { "@SpeedMiddle", SPEEDMIDDLE },
  { "@SpeedLow", SPEEDLOW },
  { "@SetHighSpeed", SETHIGHSPEED },
  { "@GetHighSpeed", GETHIGHSPEED },

  { "@SetTimingOutMode", SETTIMINGOUTMODE },
  { "@SetTimingOutStart", SETTIMINGOUTSTART },
  { "@SetTimingOutEnd", SETTIMINGOUTEND },
  { "@SetTimingOutInterval", SETTIMINGOUTINTERVAL },
  { "@SetTimingOutReady", SETTIMINGOUTREADY },
  { "@Select", SELECT },

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
  { "@SetRange", SETRANGE },
  { "@GetRange", GETRANGE },
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
  { "@RunStart", RUNSTART },
  { "@RunStop", RUNSTART },

  { "@Resume", RESUME },
  { "@GetRealTime", GETREALTIME },
  { "@GetLiveTime", GETLIVETIME },
  { "@GetStatus", GETSTATUS },
  { "@SetROI", SETROI },
  { "@SetROIs", SETROIS },
  { "@SetCalibration", SETCALIBRATION },
  { "@SetThreshold", SETTHRESHOLD },
  { "@SetPeakingTime", SETPEAKINGTIME },
  { "@SetDynamicRange", SETDYNAMICRANGE },
  { "@GetCalibration", GETCALIBRATION },
  { "@GetThreshold", GETTHRESHOLD },
  { "@GetPeakingTime", GETPEAKINGTIME },
  { "@GetDynamicRange", GETDYNAMICRANGE },
  { "@GetPreAMPGain", GETPREAMPGAIN },
  { "@GetMCALength", GETMCALENGTH },
  { "@GetMCA", GETMCA },
  { "@GetStatistics", GETSTATISTICS },

  { "@SetMode", SETMODE },
  { "@SetCountPreset", SETCOUNTPRESET },

  { "@GetDataLinkCh", GETDATALINKCH },
  { "@GetMCAs", GETMCAS },

  { "@SetAperture", SETAPERTURE },
  { "@SetTriggerDelay", SETTRIGGERDELAY },
  { "@SetSamplingSource", SETSAMPLINGSOURCE },
  { "@SetTriggerSource", SETTRIGGERSOURCE },

  { "@SetTriggerCounts", SETTRIGGERCOUNTS },
  { "@SetTriggerSlope", SETTRIGGERSLOPE },
  { "@GetDataPoints", GETDATAPOINTS },
  { "@ReadDataPoints", READDATAPOINTS },
  { "@Abort", ABORT },
  { "@SetAutoZero", SETAUTOZERO },
  { "@Record", SETAUTOZERO },
  { "@qInitialize", QINITIALIZE },
  { "@qGetData", QGETDATA },
  { "@qFinalize", QFINALIZE },

  { "@StandBy", STANDBY },
  { "@GetData", GETDATA },
  { "@GetStat", GETSTAT },
  { "@Trigger", TRIGGER },

  { "@GoMaxAbs", GOMAXABS },
  { "@GoMaxRel", GOMAXREL },
  { "@ShutterOff", SHUTTEROFF },

  { "_Connected", EvCONNECTED },
  { "_Disconnected", EvDISCONNECTED },
  { "_ChangedValue", EvCHANGEDVALUE },
  { "_ChangedIsBusy", EvISBUSY },

  { "_ReportCurrent", EvREPORTCURRENT },
  { "_IsBusy", EvREPORTINJECTION },
  // IsBusy という名前のイベントだけど、実際にはこれを発行するのは LS.Injector 

  { "", NOTMSG },
};
