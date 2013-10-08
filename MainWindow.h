#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <QFileDialog>

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
#include "S2DView.h"
#include "Status.h"
#include "AtomGroup.h"
#include "Conditions.h"
#include "KeV2Pix.h"
#include "FluoDBase.h"
#include "DarkTable.h"
#include "Changer.h"
#include "cBar.h"
#include "UsingUnits.h"
#include "TuningTable.h"

enum DIRECTION { FORWARD, BACKWARD };
enum MCASTARTRESUME { MCA_START, MCA_RESUME };
enum ENCORPM { XENC, XPM };
enum OLDNEW { OLD, NEW };

#define MEAS_ID "XAFS Measurement"
#define GOMOTOR_ID "Motor Motion"
#define GOKEV_ID "Monochromator Motion"
#define SCAN_ID "Scan"
#define MONITOR_ID "Monitor"
#define MCA_ID  "MCA Monitor"
#define S2D_ID  "2D Scan"

#define DXMCENTERFILE0 "DXMCenter.cfg"

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

  QVector<AtomGroup*> AtomGroups;
  UsingUnits UUnits;

  /* Tab control */
  QVector<ViewCTRL*> ViewCtrls;

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

  /* cfg. */
  SelMC2 *selmc;
  StarsSV2 *starsSV;
  Conditions *conds;
  Units *u;
  /* cfg. */

  //  bool isAnyOtherProcess( void );
  
  /* MCA */
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
  int *MCAData;
  MCASTARTRESUME StartResume;
  bool MCAClearRequest;
  QFileDialog *MCAFSel;
  PeriodicTable *PT2;
  QString NonSelC, SelectC;
  QDir mcaDir;
  QFileInfo BaseFile;
  QVector<MCAGain*> MCAGains;
  QVector<MCAPeak> *MCAPeaks;

  /* ReadData */
  QVector<Data*> Datas;
  void setupReadDataArea( void );

  /* Special Units */
  AUnit *MMainTh;                 // main Th ax
  AUnit *MDTh1;                   // Delta Theta 1 ax
  AUnit *SI0, *SI1, *SFluo, *SLS;  // I0, I1, and Fluorescence, LS
  AUnit *EncMainTh, *Enc2;
  //  AUnit *ChangerX, *ChangerZ;

  QVector<Changer*> Changers;
  AUnit *movingC1, *movingC2;

  void InitAndIdentifyMotors( void );
  void InitAndIdentifySensors( void );

  QTimer *GoTimer, *MCATimer, *ScanTimer, *MonTimer, *MeasTimer, *MeasDarkTimer;
  QTimer *S2DTimer;

  Stars *s;

  MEASMODE MeasFileType;

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
  int MovingM;           // Moving motor ID
  int MovingS;           // Moving motor Speed
  //  RELABS GoMRelAbs, SPSRelAbs;
  int SPSSelU;           // Selected SPS Unit
  double SPSUPP;         // Unit per puls
  bool inSPSing;
  int ScanStage;
  int ScanMotor, ScanSensor;
  double ScanOrigin, ScanSP, ScanEP, ScanSTP;
  int NowScanP;
  ViewCTRL *ScanViewC;
  XYView *ScanView;
  QFileDialog *scanFSel;
  int SFluoLine;
  bool isSI1, isSFluo;
  //  bool useFixedDelta;
  double SettingMainTh;
  void SetMainThCenter( void );

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
  bool MonSensF[ 3 ];

  // Scan 2D
  bool S2DStepF, S2DRealScanF;
  DIRECTION S2DScanDir;
  QFileDialog *S2DFileSel;
  DIRECTION ReversedDir( DIRECTION d )
  { if ( d == FORWARD ) return BACKWARD; return FORWARD; }

  QVector<QComboBox*> S2DAxis;
  QVector<QLabel *> S2DCurPos;
  QVector<QLabel *> S2DUnits;
  QVector<QLineEdit *> S2DStarts;
  QVector<QLineEdit *> S2DEnds;
  QVector<QLineEdit *> S2DSteps;
  QVector<QLineEdit *> S2DPoints;
  QVector<RelAbs *> S2DRelAbs;
  QVector<bool> S2DMotorUse;
  QVector<AUnit *> S2DOkSensors;
  QVector<AUnit *> S2DOkMotors;
  QVector<AUnit *> S2DMotors;
  bool inS2D;
  int S2DStage;
  QVector<int> S2Dnow;
  QVector<int> S2Di, S2Dps;
  QVector<double> S2Dsx, S2Dex, S2Ddx;
  double S2DVals[ 10 ], S2DCPSs[ 10 ];
  QString S2DFile;
  bool isS2DSFluo;
  double S2DLastV;

  void newAx0( int ax, int motor );
  void S2DStop0( void );
  void SetupS2DParams( void );
  void S2DWriteHead( void );
  void S2DWriteBody( double v );
  void S2DWriteBody2( void );
  void S2DWriteBlankLine( void );
  void S2DWriteTail( void );

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
  QString defaultFileName;

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
  bool inMeas, inPause, SinPause;
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
  bool AskingOverwrite;
  bool MakingSureOfRangeSelect;
  ViewCTRL *SetUpNewView( VTYPE vtype );
  void ClearXViewScreenForMeas( XYView *view );
  int GetDFName0( void );
  void SetDFName( int rpt );
  double MeasVals[ MCHANNELS ];
  double MeasCPSs[ MCHANNELS ];
  MEASMODE MeasDispMode[ MCHANNELS ];
  int MeasDispPol[ MCHANNELS ];   // polarity
  double NowDwell;
  bool inMeasDark;
  bool AskingShutterClose;
  bool AskingShutterOpen;
  int MeasDarkStage;

  void ShowMeasFileStatus( QString fname );
  void ShowTotal( void );
  void CpBlock2SBlock( void );
  bool CheckBlockRange( void );
  void WriteInfoFile( void );
  void WriteInfoFile2( void );
  void WriteHeader( int Rpt );
  void WriteHeader2( int Rpt );
  void WriteHeaderCore( void );
  void WriteHeaderCore2( void );
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
  void SetDFName2( int rpt, DIRECTION dir );
  void MakeDelegateFile( void );
  void WriteQHeader( int rpt, DIRECTION dir );
  void WriteQHeader2( int rpt, DIRECTION dir );
  void WriteQBody( void );
  int findMini( QStringList &v1, QStringList &v2, QStringList &v3 );
  int findMini( QStringList &v1, QStringList &v2,
                QStringList &v3, QStringList &v4 );
  void DispQSpectrum( int g );
  void QXafsFinish0( void );
  void QXafsFinish( void );
  bool CheckOkList( AUnit *as, QStringList OkList );
  bool theSensorIsAvailable( AUnit *as );
  void ShowQTime( double dtime, double WidthInPuls );

private slots:

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
  void ScanStart( void );
  void saveScanData( void );

  void SetDXMPMC( void );

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
  void CheckNewMeasFileName();

  void GetNewGos( void );
#if 0
  void GoToPosKeV1( void ) { MoveCurThPosKeV( GoPosKeV[0] ); }
  void GoToPosKeV2( void ) { MoveCurThPosKeV( GoPosKeV[1] ); }
  void GoToPosKeV3( void ) { MoveCurThPosKeV( GoPosKeV[2] ); }
  void GoToPosKeV4( void ) { MoveCurThPosKeV( GoPosKeV[3] ); }
#endif
  void GoToPosKeV( void );
  void ToggleGoToButtons( QString );

  void NewMotor( void );
  void NewGoMotorPosPuls( const QString &val );
  void NewGoMotorPosUnit( const QString &val );
  void GoMAtP( void );
  void GoMStop( void );
  void Monitor( void );
  void newVI0( QString v );
  void newVS1( QString v );
  void newVS2( QString v );
  void setSelectedMonFName( const QString &fname );
  void setSelectedScanFName( const QString &fname );
  void setSelectedMCAFName( const QString &fname );
  void newGain( void );
#if 0      // new mcas
  void ShowNewMCAStat( void );
#endif
  void ShowNewMCAStat( char *MCAs );
  void ShowNewMCARealTime( int ch );
  void ShowNewMCALiveTime( int ch );
  void saveMonData( void );
  void gotNewPeakList( QVector<MCAPeak>* );
  void newPSSens( void );
  void SelectedShowDiff( bool f );
  void SelectedLimitPSEnergy( bool f );
  void newCalibration( void );

  void newSensSelected( int );
  void newRangeSelected( int );
  void askNowRange( void );
  void askNowRanges( void );
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
  void saveMCAData0( QString fname );
  void WriteMCAHead( QTextStream &out );
  void WriteMCAData( QTextStream &out );

  void ChangeBLKUnit( int i );
  void ChangeBLKs( int i );

  void SetStdEXAFSBLKs( void );
  void SetStdXAFSBLKs( void );
  void SetStdXANESBLKs( void );
  void SetDwells( void );
  void SelectedWBFN( const QString &fname );
  void SelectedRBFN( const QString &fname );

  void SelSSDs0( void );
  void SelSSDs20( void );
  void SelectedNDFN( const QString &fname );
  void NewRpt( void );
  void setAllROIs( void );

  void newSensSelectedForI0( int index );
  void newSensSelectedForI1( int index );
  void newSensSelectedForA1( int index );
  void newSensSelectedForA2( int index );
  void newI0Range( int newR );
  void newI1Range( int newR );
  void newA1Range( int newR );
  void newA2Range( int newR );

  void AutoMeasurement( void );
  void moveToTarget( int target, double dx, double dy );
  void AutoSequence( void );

  void StartMeasurement( void );
  void PauseMeasurement( void );
  void SurelyStop( void );
  void GoingOn( void );
  void OkOverWrite( void );
  void RangeSelOK( void );
  void onMeasFinishWorks( void );
  void SelectAGB( bool f );
  void ReCalcSSDTotal( int i, bool f );
  void NoticeMCAViewSetDisplayLog( bool f );
  void NoticeMCAViewSetShowElements( bool f );
  void NoticeMCAViewShowAlwaysSelElm( bool f );
  void NoticeMCAViewShowElmEnergy( bool f );
  void moveToATab( int tab );
  void NoticeSelectedStats( int tab );
  //  void doPeakFit( void );
  void ReadLowerLimitSetting( void );

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
  void TryToGiveNewView( DATATYPE dtype );
  void DeleteTheView( void );
  void ShowButtonsForCurrentTab( void );
  void ShowButtonsForATab( int i );

  void setEncNewTh( QString orig, QString newv );
  void SetNewGases( void );
  //  void showMCAs( void );

  // QXafs
  void ToggleQXafsMode( bool f );
  void QXafsMeasSequence( void );
  void CheckQXafsParams( void );
  void SetNewRPTLimit( void );
  void QIntervalTimeout1( void );
  void QIntervalTimeout2( void );

  // S2D Scan2D
  void newS2DFileSelected( const QString &fname );
  void showS2DNewAxValue( QString val );
  void newAx( int motor );
  void newS2DSteps( void );
  void newS2DPoints( void );
  void S2DScanStart( void );

  // ステップスキャン (検出器、モータともにステップ)
  void S2DStepScanSequence( void );
  // 擬似連続スキャン (検出器は連続、モータはステップ)
  void S2DQuasiContinuousScanSequence( void );
  // ホントの連続スキャン準備中
  void S2DRealContinuousScanSequence( void );

  void S2DNewChangerSelected( int i );
  void S2DSetUseChangers( bool f );

 signals:
  void SelectedSSD( int i, bool f );
  void SelectedAGB( int i, bool f );
  //  void GiveNewView( QObject *to, ViewCTRL *view );
  void NewEnergy( double e );
  void ChangerNext( void );
};

#endif
