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
#include "Status.h"
#include "AtomGroup.h"
#include "Conditions.h"
#include "KeV2Pix.h"
#include "FluoDBase.h"
#include "DarkTable.h"

enum MCASTARTRESUME { MCA_START, MCA_RESUME };
enum ENCORPM { XENC, XPM };
enum OLDNEW { OLD, NEW };

class MainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

public:
  MainWindow( QString myname );

private:

  QTime *T;
  Status* StatDisp;

  QString XAFSName;
  QString XAFSKey;
  QString XAFSTitle;

  QVector<AtomGroup*> AtomGroups;

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

  /* cfg. */
  SelMC2 *selmc;
  StarsSV2 *starsSV;
  Conditions *conds;
  Units *u;
  /* cfg. */
  
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

  /* ReadData */
  QVector<Data*> Datas;
  void setupReadDataArea( void );

  /* Special Units */
  AUnit *MMainTh;                 // main Th ax
  AUnit *SI0, *SI1, *SFluo, *SLS;  // I0, I1, and Fluorescence, LS
  AUnit *EncMainTh;

  void InitAndIdentifyMotors( void );
  void InitAndIdentifySensors( void );

  QTimer *GoTimer, *MCATimer, *ScanTimer, *MonTimer, *MeasTimer, *MeasDarkTimer;

  Stars *s;

  MEASMODE MeasFileType;


  AtomNo SelectedA;
  PeriodicTable *PT;
  AbEN SelectedE;
  //  double CurPosKeV;      // Current Position of MonoChro in Energy (keV)
  QVector<QComboBox *> GoUnit;
  QVector<QLineEdit *> GoPosEdit;
  double GoPosKeV[ GOS ];
  double oldDeg;             // ShowCurThPos での重複実行を避けるため。
  bool AllInited, MotorsInited, SensorsInited;

  void setupLogArea( void );
  void setupCommonArea( void );
  void setupSetupArea( void );
  void setupSetupSSDArea( void );
  void setupMeasArea( void );

  QStringList DriverList;

  MUnits mUnits;

  /***********************************************/

  double SelectedCurPosDeg( ENCORPM EncOrPM );
  //  QString viewEncOrPM( ENCORPM EncOrPM )
  //  { return ( EncOrPM == XENC ) ? "Encorder" : "PM"; };
  // return Mono. Chro. deg. measured by selected way
  void MoveCurThPosKeV( double keV ); // Move current Pos. of Mon. in keV

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

  void GoMAtPuls( double Pos );
  void GoMStop0( void );
  void ShowTAE( void );
  void GetNewGo( int i );
  void ShowGo( int i );
  void SetNewGos( void );
  void ScanStop0( void );
  void ReadOutScanData( void ); // ( int NowP );
  void ShowGoMRelAbs( void );
  void ShowSPSRelAbs( void );

  void ShowGoMSpeed( void );
  MSPEED GoMSpeed;

  int inMMove;
  int MovingM;           // Moving motor ID
  int MovingS;           // Moving motor Speed
  RELABS GoMRelAbs, SPSRelAbs;
  int SPSSelU;           // Selected SPS Unit
  double SPSUPP;         // Unit per puls
  int inSPSing;
  int ScanStage;
  int ScanMotor, ScanSensor;
  double ScanOrigin, ScanSP, ScanEP, ScanSTP;
  int NowScanP;
  ViewCTRL *ScanViewC;
  XYView *ScanView;
  QFileDialog *scanFSel;
  int SFluoLine;
  bool isSI1, isSFluo;

  //  bool MeasCntIs;
  //  int MeasCntNo;
  int MeasChNo;
  bool setupMDispFirstTime;

  QTime MonTime;
  bool monRecF;
  QFile MonFile;
  QTextStream MonOut;
  int inMonitor;
  int MonStage;
  int MonDev;
  ViewCTRL *MonitorViewC;
  TYView *MonitorView;
  QFileDialog *monFSel;
  bool MonSensF[ 3 ];

  QVector<AUnit*> SensWithRange;

  QVector<QLineEdit *> BLKstart;
  QVector<QLineEdit *> BLKstep;
  QVector<QLineEdit *> BLKdwell;
  QVector<QLineEdit *> BLKpoints;

  QVector<QPushButton *> SSDbs;
  QVector<QPushButton *> SSDbs2;
  QVector<QPushButton *> GSBs;
  //  MEASMODE MeasMode;
  QFileDialog *SelDFND;
  QFileDialog *SelWBFND;
  QFileDialog *SelRBFND;
  QFileDialog *SelLFND;
  QString SelectedOrgName;
  bool OverWriteChecked;

  UNIT BLKUnit;
  int Blocks;
  double BlockStart[ MaxBLKs + 1 ];
  double BlockDwell[ MaxBLKs + 1 ];
  int BlockPoints[ MaxBLKs + 1 ];

  UNIT SBLKUnit;
  int SBlocks;
  double SBlockStart[ MaxBLKs + 1 ];
  double SBlockStep[ MaxBLKs + 1 ];
  double SBlockDwell[ MaxBLKs + 1 ];
  int SBlockPoints[ MaxBLKs + 1 ];
  int SensorUseF[ 4 ];       // 0: I1, 1: SSD Total, 2: Aux1, 3: Aux2
  int EstimatedMeasurementTimeInSec;

  void ClearBLKs( void );
  void ShowBLKs( void );
  void WriteBF( void );
  DarkTable *darkTable;

  void SelSSDs( int i );
  //  void SetSSDactive( bool active );

  QVector<int> ChModes;

  QVector<AUnit *> I0Sensors, I1Sensors, A1Sensors, A2Sensors;
  QString fixS( QString s, int l );
  QString DFName0, DFName;
  int TP;
  double TT0;
  int inMeas, inPause, SinPause;
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
  void SetDFName( int i );
  double MeasVals[ MCHANNELS ];
  double MeasCPSs[ MCHANNELS ];
  MEASMODE MeasDispMode[ MCHANNELS ];
  int MeasDispPol[ MCHANNELS ];   // polarity
  double NowDwell;
  bool inMeasDark;
  bool AskingShutterClose;
  bool AskingShutterOpen;
  int MeasDarkStage;

  void ShowTotal( void );
  void CpBlock2SBlock( void );
  void WriteHeader( int Rpt );
  void WriteHeader2( int Rpt );
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

private slots:
  void Initialize( void );
  void InitializeUnitsAgain( void );
  void SendListNodes( void );
  void RcvListNodes( SMsg msg );
  void SomeDrvIsConnected( SMsg msg );
  void SomeDrvIsDisconnected( SMsg msg );
  void Hide( bool f );

  void ShowMessageOnSBar( QString msg, int time );
  //  void SetNewLatticeConstant( double LC ) { u->setD( LC ); };

  bool MeasureDark( void );

  void ChangeBLKstart( void );
  void ChangeBLKstep( void );
  void ChangeBLKpoints( void );
  void ChangeBLKdwell( const QString &dtime );

  //  double CurrentAngle( void );
  void ShowCurThPos( void );
  void ShowCurMotorPos( SMsg msg );
  void ShowNewRingCurrent( QString Val, QStringList Vals );

  void MMRel( void );
  void MMAbs( void );
  void SPSRel( void );
  void SPSAbs( void );
  void SetGoMSpeedH( void );
  void SetGoMSpeedM( void );
  void SetGoMSpeedL( void );
  void ScanStart( void );
  void saveScanData( void );

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

  void GetNewGos( void );
  void GoToPosKeV1( void ) { MoveCurThPosKeV( GoPosKeV[0] ); }
  void GoToPosKeV2( void ) { MoveCurThPosKeV( GoPosKeV[1] ); }
  void GoToPosKeV3( void ) { MoveCurThPosKeV( GoPosKeV[2] ); }
  void GoToPosKeV4( void ) { MoveCurThPosKeV( GoPosKeV[3] ); }
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
#if 0      // new mcas
  void ShowNewMCAStat( void );
#endif
  void ShowNewMCAStat( char *MCAs );
  void ShowNewMCARealTime( int ch );
  void ShowNewMCALiveTime( int ch );

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

  void StartMeasurement( void );
  void PauseMeasurement( void );
  void SurelyStop( void );
  void GoingOn( void );
  void OkOverWrite( void );
  void RangeSelOK( void );
  void isFileNameChanged( const QString & );
  void onMeasFinishWorks( void );
  void SelectAGB( bool f );
  void ReCalcSSDTotal( int i, bool f );
  void NoticeMCAViewSetDisplayLog( bool f );
  void NoticeMCAViewSetShowElements( bool f );
  void NoticeMCAViewShowAlwaysSelElm( bool f );
  void NoticeMCAViewShowElmEnergy( bool f );
  void moveToATab( int tab );
  void NoticeSelectedStats( int tab );
  void doPeakFit( void );

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

 signals:
  void SelectedSSD( int i, bool f );
  void SelectedAGB( int i, bool f );
  //  void GiveNewView( QObject *to, ViewCTRL *view );
};

#endif
