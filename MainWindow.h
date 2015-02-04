#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <QFileDialog>
#include <QPrintDialog>

#include <math.h>

#include "ui_MainWindow.h"

#include "Mccd.h"
#include "SelMC2.h"
#include "StarsSV2.h"
#include "SelMC2.h"
#include "Units.h"
#include "ViewCtrl.h"
#include "XafsM.h"
#include "Atoms.h"
#include "Stars.h"
#include "PeriodicT.h"
#include "AUnit.h"
#include "MultiUnits.h"
#include "XYView.h"
#include "TYView.h"
#include "MCAView.h"
#include "S2DB.h"
#include "S2DView.h"
#include "Status.h"
#include "AtomGroup.h"
#include "Conditions.h"
#include "PMConditions.h"
#include "KeV2Pix.h"
#include "FluoDBase.h"
#include "DarkTable.h"
#include "Changer.h"
#include "SpecChanger.h"
#include "cBar.h"
#include "UsingUnits.h"
#include "TuningTable.h"
#include "S2DInfo.h"
#include "MCA.h"
#include "ScanInfo.h"
#include "BlockInfo.h"
#include "CheckUnits.h"
#include "MeasParamSet.h"
#include "ScanInfo.h"
#include "MonLine.h"
#include "Alarm.h"

#define MEAS_ID "XAFS Measurement"
#define GOMOTOR_ID "Motor Motion"
#define GOKEV_ID "Monochromator Motion"
#define SCAN_ID "Scan"
#define MONITOR_ID "Monitor"
#define MCA_ID  "MCA Monitor"
#define S2D_ID  "2D Scan"

#define XAFS_M_START   "StartMeas"
#define XAFS_M_END     "EndMeas"
//#define XAFS_M_STOP    "StopMeas"      // suspend measurement
//#define XAFS_M_RESUME  "ResumeMeas"
#define QXAFS_M_START  "StartQMeas"
#define QXAFS_M_END    "EndQMeas"

#define DXMCENTERFILE0 "DXMCenter.cfg"

#define SAVEMCACh        ( 19 )
#define S2D_END_STAGE    ( 99 )

struct AutoModeParam {
  int num;
  double dx, dz;
};

class MainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

public:
  MainWindow( QString myname );
  void InitSize( void );

private:
  QTime *T;
  Status* StatDisp;

  TuningTable *TTable;
  int OldDTh1;

  QString XAFSName;
  QString XAFSKey;
  QString XAFSTitle;

  QPrintDialog *PrintD;
  QPrintDialog *S2DPrintD;

  QVector<AtomGroup*> AtomGroups;
  UsingUnits UUnits;

  /* Tab control */
  QVector<ViewCTRL*> ViewCtrls;
  int ViewCount;

  /* ReadDef */
  QVector<AUnit *> AMotors;
  QVector<AUnit *> ASensors;
  void ReadDef( QString fname );
  QString nextItem( QString start, QString &item );
  QVector<MCCD*> mccd;
  QVector<Gas*> Gases;
  QVector<IonChLength*> ICLengths;
  void CheckDuplicateUID( void );
  void ExitByDuplicateUID( AUnit *a1, AUnit *a2 );
  double MinEnergyInEV, MaxEnergyInEV;
  int DefaultUnit;
  QString LocalizedName( QString name );
  Alarms *alarms;

  MeasPSet MPSet;
  void SetupMPSet( MeasPSet *aSet );
  void SaveI0inMPSet( void );

  /* cfg. */
  SelMC2 *selmc;
  StarsSV2 *starsSV;
  Conditions *conds;
  PMConditions *pmConds;
  Units *u;
  /* cfg. */

  //  bool isAnyOtherProcess( void );
  
  /* MCA */
  bool MCAPreAMPGainHasSet;
  KeV2Pix *kev2pix;
  FluoDBase *fdbase;
  double MCACorrect[ MaxSSDs ];
  QString ROIStart[ MaxSSDs ];
  QString ROIEnd[ MaxSSDs ];
  int MCALength;
  bool inMCAMeas;
  bool validMCAData;
  int cMCACh;
  //  int oldMCACh;
  int MCAStage;
  ViewCTRL *cMCAViewC;
  MCAView *cMCAView;
  int cMCAViewTabNo;
  quint32 *MCAData;
  MCASTARTRESUME StartResume;
  bool MCAClearRequest;
  bool MCACanSaveAllOnMem;
  double MaxMCAEnergy;
  QFileDialog *MCAFSel;
  PeriodicTable *PT2;
  QString NonSelC, SelectC;
  QDir mcaDir;
  QFileInfo BaseFile;
  QVector<MCAGain*> MCAGains;
  QVector<MCAPeak> *MCAPeaks;
  QStringList SSDCalibEnergys;
  //  double AttenDx, AttenDy;
  AUnit *movingSC1, *movingSC2;

  /* ReadData */
  QVector<Data*> Datas;
  void setupReadDataArea( void );

  /* Special Units */
  AUnit *MMainTh;                 // main Th ax
  AUnit *MDTh1;                   // Delta Theta 1 ax
  AUnit *SI0, *SI1, *SFluo, *SLS;  // I0, I1, and Fluorescence, LS
  AUnit *EncMainTh, *Enc2;
  AUnit *MMStab;

  bool MStabOk;
  QString MStabDrv;
  QString MStabDelegate;

  QVector<QString> DefUReals;
  QVector<Changer*> Changers;
  AUnit *movingC1, *movingC2;
  bool connecteNewValue2ShowChangerPosition;

  QVector<SpecChanger*> SChangers;

  void InitAndIdentifyMotors( void );
  void InitAndIdentifySensors( void );

  QTimer *GoTimer, *MCATimer, *ScanTimer, *MonTimer, *MeasTimer, *MeasDarkTimer;
  QTimer *S2DTimer, *S2DTimer2;

  Stars *s;

  MEASMODE MeasFileType, MeasFileType2;   // Type2 : Aux を使ってても頑張って判断する

  AtomNo SelectedA;
  PeriodicTable *PT;
  AbEN SelectedE;
  //  double CurPosKeV;      // Current Position of MonoChro in Energy (keV)
  QVector<QComboBox *> GoUnit;
  QVector<QLineEdit *> GoPosEdit;
  QVector<QPushButton *> GoTos;
  double GoPosKeV[ GOS ];
  double oldDeg;             // ShowCurThPos での重複実行を避けるため。
  bool AllInited, MotorsInited, SensorsInited;

  QVector<MonLine*> monLines;
  QVector<QRadioButton*> monSels;
  QVector<QComboBox*> monDevs;
  QVector<QLabel*> monVals;
  QDialog *MonLinesDialog;

  void setupLogArea( void );
  void setupCommonArea( void );
  void setupSetupArea( void );
  void setupChangerArea( void );
  void setupSetupSSDArea( void );
  void setupMeasArea( void );
  void setupScan2DArea( void );
  void setupWebView( void );
  void SetUpSensorComboBoxes( void );

  QStringList DriverList;

  MUnits mUnits;
  MUnits dUnits;

  /* Auto Sequence */
  QTimer *ASTimer;
  QFile ASFile;
  QTextStream ASin;
  QVector<QStringList> ASCMDs;
  int ASCMDi, ASCMDii;
  CheckUnits CheckMUnits, CheckSUnits;
  SpecChanger *ASSChanger;
  double ASSCDx, ASSCDy;
  MUnits ASMUnits;
  double ASMeasVals[ 100 ];
  double ASMeasCPSs[ 100 ];
  QMap<QString,QString> ASVals;
  QMap<QString,int> ASLabels;
  /* Auto Sequence */

  void getGassAbsTable( void );

  /***********************************************/

  double SelectedCurPosDeg( ENCORPM EncOrPM );
  //  QString viewEncOrPM( ENCORPM EncOrPM )
  //  { return ( EncOrPM == XENC ) ? "Encorder" : "PM"; };
  // return Mono. Chro. deg. measured by selected way
  void MoveCurThPosKeV( double keV ); // Move current Pos. of Mon. in keV
  void TuneDTh1( double keV );

  /***********************************************/

  QString FSTATMsgs[2][2];
  OLDNEW MeasDataStat;
  OLDNEW MeasNameStat;
  OLDNEW ScanDataStat;
  OLDNEW ScanNameStat;
  OLDNEW MonDataStat;
  OLDNEW MonNameStat;
  OLDNEW MCADataStat;
  OLDNEW MCANameStat;
  OLDNEW S2DDataStat;
  OLDNEW S2DNameStat;

  void GoMAtPuls( double Pos );
  void GoMStop0( void );
  void ShowTAE( void );
  void GetNewGo( int i );
  void ShowGo( int i );
  void SetNewGos( void );
  void ScanStop0( void );
  void ReadOutScanData( void ); // ( int NowP );

  void ShowGoMSpeed( void );
  MSPEED GoMSpeed;

  bool RWDXMCenterF;
  QString DXMCenterFile;

  bool inMMove;
  int SPSLastSelectedM;
  int MovingM;           // Moving motor ID
  //  int MovingS;           // Moving motor Speed
  //  RELABS GoMRelAbs, SPSRelAbs;
  //  int SPSSelU;           // Selected SPS Unit
  //  double SPSUPP;         // Unit per puls
  bool inSPSing;
  int ScanStage;
  //  int ScanMotor, ScanSensor;
  //  AUnit *SPSMovingMotor;
  //  double ScanOrigin, ScanSP, ScanEP, ScanSTP;
  //  ScanInfo SInfo;
  QVector<ScanInfo*> ScanIFs;    // 各モータ毎に状態を持つ
  int NowScanP;
  ViewCTRL *ScanViewC;
  XYView *ScanView;
  QFileDialog *scanFSel;
  int SFluoLine;
  bool isSI1, isSFluo;
  //  bool useFixedDelta;
  double SettingMainTh;
  void SetMainThCenter( void );
  bool UseDefUReal( AUnit *am );
  bool LoadScanInfo( ScanInfo *sif );
  void SaveScanInfo( ScanInfo *sif, AUnit *am = NULL );
  bool LastInIsPulsV;

  //  bool MeasCntIs;
  //  int MeasCntNo;
  int MeasChNo;
  bool setupMDispFirstTime;

  QTime MonTime;
  bool monRecF;
  QFile MonFile;
  QTextStream MonOut;
  bool inMonitor;
  int MonStage;
  int MonDev;
  ViewCTRL *MonitorViewC;
  TYView *MonitorView;
  QFileDialog *monFSel;

  // Scan 2D
  //  SCANMODE S2DScanMode0;
  DIRECTION S2DScanDir;
  QFileDialog *S2DFileSel;
  DIRECTION ReversedDir( DIRECTION d )
  { if ( d == FORWARD ) return BACKWARD; return FORWARD; }
  //  QDialog *S2DDialog;
  //  bool PoppingS2DDialog;
  S2DView *S2Dview;

  bool S2DFileCheckIsReady;
  QMessageBox *S2DAskOverWrite;
  QMessageBox *S2DAskOverWrite2;
  QVector<QComboBox*> S2DAxis;
  QVector<QLabel *> S2DCurPos;
  QVector<QLabel *> S2DUnits;
  QVector<QLineEdit *> S2DStarts;
  QVector<QLineEdit *> S2DEnds;
  QVector<QLineEdit *> S2DSteps;
  QVector<QLineEdit *> S2DPoints;
  QVector<RelAbs *> S2DRelAbs;
  //  QVector<bool> S2DMotorUse;
  QVector<AUnit *> S2DOkSensors;
  QVector<AUnit *> S2DOkMotors;
  QVector<AUnit *> S2DSelectedMotors;
  bool inS2D;
  int S2DStage;
  bool S2DInfoIsValid;
  S2DInfo S2DI;
  //  aMCAMap S2DMCAMap;
  aMCAMap XafsMCAMap;
  double S2DVals[ 10 ], S2DCPSs[ 10 ];
  double S2DLastV;

  void newAx0( int ax, int motor );
  void S2DStop0( void );
  void S2DStop00( void );
  void SetupS2DParams( void );
  void S2DWriteHead0( void );
  void S2DWriteHead( QTextStream &out );
  void S2DWriteHead2( QTextStream &out );
  void S2DWriteBody( double v );
  void S2DWriteBody2( int ix, int iy );
  void S2DWriteBlankLine( void );
  void S2DWriteTail( void );
  QFileInfo S2DGenerateMCAFileName( int i1, int i2, int i3 );
  double S2DReCalcAMapPoint( QString fname );
  double S2DReCalcAMapPointOnMem( int ix, int iy, aMCAMap *map );
  //  void S2DSaveMCAData( int ix, int iy, int iz );
  void S2DFileCheck( void );
  void SaveMCADataOnMem( aMCASet *set );
  
  QVector<AUnit*> SensWithRange;

  QVector<QLineEdit *> BLKstart;
  QVector<QLineEdit *> BLKstep;
  QVector<QLineEdit *> BLKdwell;
  QVector<QLineEdit *> BLKpoints;
  QVector<QLabel *> BLKlabels;

  QVector<QPushButton *> SSDbs;
  QVector<QPushButton *> SSDbs2;
  QVector<QPushButton *> GSBs;
  //  MEASMODE MeasMode;
  QFileDialog *SelDFND;
  QFileDialog *SelWBFND;
  QFileDialog *SelRBFND;
  QFileDialog *SelLFND;
  QString SelectedOrgName;

  UNIT BLKUnit;
  int Blocks;
  //  double BlockStart[ MaxBLKs + 1 ];
  //  double BlockDwell[ MaxBLKs + 1 ];
  //  int BlockPoints[ MaxBLKs + 1 ];

  BlockInfo NXAFSBInfo, QXAFSBInfo;

  bool SMeasInDeg;
  UNIT SBLKUnit;
  int SBlocks;
  double SBlockStartAsDisp[ MaxBLKs + 1 ];
  double SBlockStepAsDisp[ MaxBLKs + 1 ];
  double SBlockStartInDeg[ MaxBLKs + 1 ];
  double SBlockStepInDeg[ MaxBLKs + 1 ];
  double SBlockDwell[ MaxBLKs + 1 ];
  int SBlockPoints[ MaxBLKs + 1 ];
  int SensorUseF[ 4 ];       // 0: I1, 1: SSD Total, 2: Aux1, 3: Aux2
  int EstimatedMeasurementTimeInSec;
  bool SvSelRealTime, SvSelLiveTime;
  bool SvSelExtPattern;
  bool SvSaveQDataAsStepScan;
  QString defaultFileName;
  bool NBlockPisValid;

  bool NMeasInDeg;
  int NBlocks;
  UNIT NBLKUnit;
  double NBlockStartAsDisp[ MaxBLKs + 1 ];
  double NBlockStepAsDisp[ MaxBLKs + 1 ];
  double NBlockStartInDeg[ MaxBLKs + 1 ];
  double NBlockStepInDeg[ MaxBLKs + 1 ];
  double NBlockDwell[ MaxBLKs + 1 ];
  int NBlockPoints[ MaxBLKs + 1 ];

  //  void ClearBLKs( void );
  void ShowBLKs( void );
  void WriteBF( void );
  DarkTable *darkTable;

  void SelSSDs( int i );
  //  void SetSSDactive( bool active );

  QVector<int> ChModes;

  QVector<AUnit *> I0Sensors, I1Sensors, A1Sensors, A2Sensors;
  QString fixS( QString s, int l );
  QString DFName00, DFName0, DFName;
  int TP;
  double TT0;
  bool inMeas, inPause, MinPause, SinPause, inAutoSeq, inDTAutoCalib, inSSDEngAutoCalib;
  bool FixedPositionMode;
  int cMeasTab;       // Tab No. on which the current measurement result is displayed
  int inMoveTh;
  ENCORPM EncOrPM;    // Selected x-axis on the start of XAFS measurement
  int MeasStage, SMeasStage;
  int MeasR, MeasB, MeasS, MeasP;
  double GoToKeV;
  double InitialKeV;
  double r[ 100 ];
  QMessageBox *StopP;
  QMessageBox *AskOverWrite;
  QMessageBox *MakeSureOfRangeSelect;
  QMessageBox *NoticeHaveNotMeasDark;
  bool AskingOverwrite;
  bool MakingSureOfRangeSelect;
  bool haveMeasuredDark;
  bool NoticingHaveNotMeasDark;
  ViewCTRL *SetUpNewView( VTYPE vtype, DATATYPE dtype );
  void ClearXViewScreenForMeas( XYView *view );
  bool SetDFName0( QString fname );
  void SetDFName( int rpt, int rptMax, QString ext = "" );
  double MeasVals[ 100 ];
  double MeasCPSs[ 100 ];
  MEASMODE MeasDispMode[ 100 ];
  int MeasDispPol[ 100 ];   // polarity
  double NowDwell;
  bool UseAutoShutter;
  bool inMeasDark;
  bool AskingShutterClose;
  bool AskingShutterOpen;
  int MeasDarkStage;
  bool setRsRe( int &rs, int &re );
  
  void ShowMeasFileStatus( QString fname );
  void ShowTotal( void );
  void CpBlock2SBlock( void );
  bool CheckBlockRange( void );
  void WriteInfoFile( void );
  void WriteInfoFile2( void );
  void WriteHeader( int Rpt );
  void WriteHeader2( int Rpt );
  void WriteHeaderCore( bool SnotN );
  void WriteHeaderCore2( void );
  void WriteFLUOHeadSection( QTextStream &out, QVector<double>darks, double I0dark );
  void SetDispMeasModes( void );
  void DispMeasDatas( void );
  void RecordData( void );

  QString NewLFName( void );

  int oldCurrentView;
  ViewCTRL *MeasViewC;
  XYView *MeasView;
  void setupView( void );

  bool CheckDetectorSelection( void );

  bool isBusyMotorInMeas( void );
  void SetScanViewWindow( void );
  void ClearXViewScreenForScan( XYView *view );

  void SetEnableOfUnits( QString drv, bool enable );

  // Sound
  void PlayGoOnSound( void );
  void PlayEndingSound( void );
  void PlaySound( QString name, int times );

  // QXAFS
  QStringList QXafsOk, NXafsOk, CScanOk;
  bool isQXafsModeAvailable;
  int SaveNowBlocks, SaveSelectedI0, SaveSelectedI1;
  bool SaveUse19ChSSD, SaveUseAux1, SaveUseAux2;
  int OrigHSpeed, HSpeed, MaxHSpeed, LowSpeed;
  int QXafsSP0, QXafsSP, QXafsEP0, QXafsEP, QXafsInterval, QXafsPoints;
  double RunUpRate, RunUpTime, QXafsDwellTime;
  QString EncValue0, Enc2Value0;
  bool QIntervalBlock1, QIntervalBlock2;
  QTimer *QIntervalTimer1, *QIntervalTimer2;

  // Auto mode
  bool AutoModeFirst;
  bool MovingToNewSamplePosition;
  int MeasA;
  QVector<AutoModeParam> AutoModeParams;
  void TouchDelegateFile( void );
  QString AutoModeComment;

  void ShowQXafsProgress( void );
  void setupQXafsMode( void );
  void HideBLKs( bool f );
  void GetPM16CParamsForQXAFS( void );
  void SetUpMainThToGenerageTriggerSignal( int sp, int ep );

  void SetDFName2( int rpt, DIRECTION dir, int type );
  // type :: 0 normal Q-XAFS, 1 : Simulate Step Scan

  void MakeDelegateFile( void );
  void WriteQHeader( int rpt, DIRECTION dir );
  void WriteQHeader2( int rpt, DIRECTION dir );
  void WriteQBody( int rpt, DIRECTION dir );
  void WriteQBody1( DIRECTION dir );
  void WriteQBody2( DIRECTION dir );
  int findMini( QStringList &v1, QStringList &v2, QStringList &v3 );
  int findMini( QStringList &v1, QStringList &v2,
                QStringList &v3, QStringList &v4 );
  void DispQSpectrum( int g );
  void QXafsFinish0( void );
  void QXafsFinish( void );
  bool CheckOkList( AUnit *as, QStringList OkList );
  bool theSensorIsAvailable( AUnit *as );
  void ShowQTime( double dtime, double WidthInPuls );

  void getNewMCAView( void );

  /* Auto Sequence */
  void setupAutoSequence( void );
  void AutoSequence( QString fname );
  bool AutoSequenceShouldBeLocked( void );
  bool ASReadNextLine( QStringList &line );
  void AutoSequenceEnd( void );
  void ASReadSEQFile( QString fname );

  AUnit *smAm;
  int smStage;
				     
private slots:
  // Main Part
  void Print( QPrinter *p );
  void newDataRoot( const QString &dataroot );

  // Auto mode
  bool ParseAutoMode( void );
  void ShowItemsForAutoMode( void );
  void SetNewChangerCenter( void );
  void ChangerGoToNewPosition( void );
  void NewChangerSelected( int i );
  void StartDatumChanger( void );
  void ChangerReached( QString );
  void ShowChangerPosition( QString );

  void Initialize( void );
  void InitializeUnitsAgain( void );
  void SendListNodes( void );
  void RcvListNodes( SMsg msg );
  void SomeDrvIsConnected( SMsg msg );
  void SomeDrvIsDisconnected( SMsg msg );
  void HideT( bool f );
  void HideB( bool f );

  void ShowMessageOnSBar( QString msg, int time );
  //  void SetNewLatticeConstant( double LC ) { u->setD( LC ); };

  bool MeasureDark( void );
  void AddNewDTh1TunePoint( void );

  void ChangeBLKparams( void );
  void ChangeBLKpoints( void );

  //  double CurrentAngle( void );
  void ShowCurThPos( void );
  void ShowCurMotorPos( SMsg msg );
  void ShowNewRingCurrent( QString Val, QStringList Vals );
  void ShowCurDTh1( void );

  void SetGoMSpeedH( void );
  void SetGoMSpeedM( void );
  void SetGoMSpeedL( void );
  //  void setSInfo( void );
  void ScanStart( void );
  void saveScanData( void );
  void SelectedAPointInScanArea( double x, double y );

  void SetDXMPMC( void );
  void ShowOtherOptions( void );

  void NewSelA( int i );
  void OpenPT( void );
  void NewSelE( int i );
  void ManSelTEdeg( void );
  void ManSelTEkeV( void );
  void SetAllGoUnits( int i );
  void ShowAllGos( void );
  void saveGSBs( int ch );
  void loadGSBs( int ch );
  void clearGSBs( void );
  void SetGSBFlags( QVector<bool> flgs );
  void SetGSBLabels( QStringList lbls );
  void ShowMB( void );
  void CheckNewMeasFileName( void );
  void ReCalcXAFSWithMCA( void );
  void AfterSaveXafs( void );

  void GetNewGos( void );
  void GoToPosKeV( void );
  void ToggleGoToButtons( QString );

  void NewMotor( void );
  void NewGoMotorPosPuls( const QString &val );
  void NewGoMotorPosUnit( const QString &val );
  void NewMMRelAbs( RELABS stat );
  void GoMAtP( void );
  void GoMStop( void );
  void Monitor( void );
  void PauseMonitor( void );
  void newVs( QString v );
  void setSelectedMonFName( const QString &fname );
  void setSelectedScanFName( const QString &fname );
  void setSelectedMCAFName( const QString &fname );
  void newGain( void );
  void PopChangeMonLines( bool f );
  void PopDownMonLines( void );
  void newMovingAvr( void );

  void ShowNewMCAStat( char *MCAs );
  void ShowNewMCARealTime( int ch );
  void ShowNewMCALiveTime( int ch );
  void saveMonData( void );
  void gotNewPeakList( QVector<MCAPeak>* );
  void newPSSens( void );
  void SelectedShowDiff( bool f );
  void SelectedPeakSearch( bool f );
  void SelectedShowSmoothed( bool f );
  void SelectedFitToRaw( bool f);
  void PushedReFit( void );
  void PushedClearMCAPeaks( void );
  void SelectedLimitPSEnergy( bool f );
  void newCalibration( void );
  void newMaxLoop( void );
  void newDampFact( void );
  void newPrec1( void );
  void newPrec2( void );
  void SaveS2DMCAs( void );
  //  void S2DMCAWriteNext( void );

  void newSensSelected( int );
  void newRangeSelected( int );
  void askNowRange( void );
  void askNowRanges( void );
  void askSetRange( void );
  void GotNowRange( int r );
  void SelAutoRange( bool Auto );
  void SetAutoRangeMode( int i );

  void ShowNewDark( double dark );
  void NewDarkChSelected( int i );
  void AskedToSetDark( void );

  void StartMCA( void );
  void MCAChSelected( int i );
  void showPeakingTime( SMsg mag );
  void showThreshold( SMsg mas );
  void showCalibration( SMsg mas );
  void showDynamicRange( SMsg msg );
  void showPreAMPGain( SMsg msg );
  void getMCALen( SMsg msg );
  void getMCASettings( int ch );
  void newROIStart( const QString &newv );
  void newROIEnd( const QString &newv );
  void showCurrentValues( int, int );
  void setNewROI( int, int );
  void clearMCA( void );
  void RealTimeIsSelected( void );
  void LiveTimeIsSelected( void );
  void saveMCAData( void );
  //  void saveMCAData0( QString fname, aMCASet *set );
  //  void WriteMCAHead( QTextStream &out, aMCASet *set );
  //  void WriteMCAData( QTextStream &out, aMCASet *set );

  void ChangeBLKUnit( int i );
  void ChangeBLKs( int i );

  void SetStdEXAFSBLKs( void );
  void SetStdXAFSBLKs( void );
  void SetStdXANESBLKs( void );
  void SetDwells( void );
  void SelectedWBFN( const QString &fname );
  void SelectedRBFN( const QString &fname );

  void setPreAMPGains( void );
  void SelSSDs0( void );
  void SelSSDs20( void );
  void SelectedNDFN( const QString &fname );
  void NewRpt( void );
  void setAllROIs( void );
  void newMaxMCAEnergy( void );

  void newSensSelectedForI0( int index );
  void newSensSelectedForI1( int index );
  void newSensSelectedForA1( int index );
  void newSensSelectedForA2( int index );
  void newI0Range( int newR );
  void newI1Range( int newR );
  void newA1Range( int newR );
  void newA2Range( int newR );

  void AutoMeasurement( void );
  void moveToTarget( Changer *c, int target, double dx, double dy );
  void AutoXAFSSequence( void );

  void StartMeasurement( void );
  void PauseMeasurement( void );
  void SurelyStop( void );
  void GoingOn( void );
  void OkOverWrite( void );
  void OkHaveNotMeasDark( void );
  void RangeSelOK( void );
  void onMeasFinishWorks( void );
  void SelectAGB( bool f );
  void ReCalcSSDTotal( int i, bool f );
  void moveToATab( int tab );
  void NoticeSelectedStats( int tab );
  //  void doPeakFit( void );
  void ReadLowerLimitSetting( void );
  void DTAutoCalibStart( void );
  void SSDEngAutoCalibStart( void );
  void MoveToNewCaribEnergy( void );
  void MoveInMeasView( int ix, double x );

  void NewAttenCh( int i );
  void NewAttenPos( void );
  void SChangerReached( QString );

  double calcMuT( int ch, int gas, double keV );
  double calcAMuT( int an, double keV );
  QVector<Element> ParseCompString( const QString& cmp );

  void NewLogMsg( QString msg );
  void SelLFN( void );
  void SetNewLFName( const QString &name );
  void AddLogComment( void );

  void MotorMove( void );
  void MeasSequence( void );
  void ScanSequence( void );
  void MonSequence( void );
  void MCASequence( void );
  void MeasDarkSequence( void );

  void TryToNoticeCurrentView( void );
  void TryToGiveNewView( DATATYPE dtype, QString dir );
  //  void DeleteTheView( void );
  void ShowButtonsForCurrentTab( void );
  void ShowButtonsForATab( int i );

  void setEncNewTh( QString orig, QString newv );
  void SetNewGases( void );
  //  void showMCAs( void );
  void nowFitStat( QString &stat );

  // QXafs
  void ToggleQXafsMode( bool f );
  void QXafsMeasSequence( void );
  void CheckQXafsParams( void );
  void SetNewRPTLimit( void );
  void QIntervalTimeout1( void );
  void QIntervalTimeout2( void );
  void ShowDeltaAtRefPoint( void );

  // S2D Scan2D
  void newS2DFileSelected( const QString &fname );
  void showS2DNewAxValue( QString val );
  void newAx( int motor );
  void newS2DSteps( void );
  void newS2DPoints( void );
  void S2DScanStart( void );
  void CheckS2DDwellTime( void );
  void SaveS2DResult0( void );
  void SaveS2DResult( void );
  //  void PopUpS2D( void );

  void S2DOkOverWrite( void );
  void S2DOkOverWrite2( void );
  void S2DScanNotStart( void );

  // ステップスキャン (検出器、モータともにステップ)
  void S2DStepScanSequence( void );
  // 擬似連続スキャン (検出器は連続、モータはステップ)
  void S2DQuasiContinuousScanSequence( void );
  // ホントの連続スキャン準備中
  void S2DRealContinuousScanSequence( void );

  void S2DNewChangerSelected( int i );
  void S2DSetUseChangers( bool f );
  void S2DRContScanMeas( void );
  void S2DNewScanValue( QString v );
  void S2DMoveToPointedPosition( int x, int y );

  //  void S2DShowInfoAtNewPosition( int x, int y, aMCASet *set );
  //  void S2DShowIntMCA( int x, int y, aMCASet *set );
  void ShowMCASpectrum( aMCASet *set1, aMCASet *set2 ); // 上の2つの関数をこの一つで担う
    
  void S2DChangeMCACh( int dCh );
  void S2DSetROIs( void );
  //  void S2DReCalcMap0( void );
  void ansToGetNewMCAView( S2DB *s2db );
  void ReCalcS2DMap( void );

  /* AutoSequence */
  void AutoSequence0( void );
  void DTAutoCalibFinished();
  void SSDEngAutoCalibFinished();

  /* AcceptTYView */
  void TYVUpScale( void );
  void TYVDownScale( void );

  void SpecialMove( void );           // 特殊動作だ !!
  void SpecialMoveCore( void );       // 特殊動作だ !!

  void SignalToStars( QString event );

  void onViewTabClosed( int i );
  void addAView( void );
  void *findAView( DATATYPE dtype );

  void alarmOn( void );
  void alarmOff( void );
  
 signals:
  void SelectedSSD( int i, bool f );
  void SelectedAGB( int i, bool f );
  //  void GiveNewView( QObject *to, ViewCTRL *view );
  void NewEnergy( double e );
  void ChangerNext( void );
  void NewROI( int s, int e );
  void NewDiff1( int i );
  void NewDiff2( int i );
  void NewMCACh( int ch );

  /* Auto Sequence */
  void AutoSequenceFinished( void );

  void SignalMCAViewSetDisplayLog( bool f );
  void SignalMCAViewSetShowElements( bool f );
  void SignalMCAViewShowAlwaysSelElm( bool f );
  void SignalMCAViewShowElmEnergy( bool f );

  void ReCalcS2DMap0( QString *RS, QString *RE, QVector<QPushButton*> &ssdbs2 );
};

#endif
