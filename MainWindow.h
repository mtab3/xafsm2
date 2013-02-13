#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMessageBox>
#include <QTimer>
#include <QTime>

#include <math.h>

#include "Mccd.h"
#include "ui_MainWindow.h"
#include "SelMC.h"
#include "StarsSV.h"

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

class QFileDialog;

#define PI ( 3.1415926535 )
#define CC ( 2.99792458e8 )
#define hb ( 1.05457266e-34 )
#define EE ( 1.60217733e-19 )
#define E2A ( 2 * PI * hb * CC / EE * 1e10 )
#define KE2A ( 2 * PI * hb * CC / EE / 1000. * 1e10 )

/******************************************************************************/
/* Don't change these numbers ! without deep considerations.                  */
/* Window design of the program is assuming these values are as defined here. */
/******************************************************************************/
const int GOS = 4;            // Presetted Go To Positions
const int MaxBLKs = 8;        // Max Blocks
const int MaxSSDs = 19;       // Max SSD elements
/******************************************************************************/

enum MCASTARTRESUME { MCA_START, MCA_RESUME };

class MainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

public:
  MainWindow( QString myname );

private:

  Status* StatDisp;

  QString XAFSName;
  QString XAFSKey;
  QString XAFSTitle;

  /* Tab control */
  QVector<ViewCTRL*> ViewCtrls;

  /* ReadDef */
  QVector<AUnit *> AMotors;
  QVector<AUnit *> ASensors;
  void ReadDef( QString fname );
  QString nextItem( QString start, QString &item );
  QVector<MCCD*> mccd;
  void CheckDuplicateUID( void );
  void ExitByDuplicateUID( AUnit *a1, AUnit *a2 );

  /* cfg. */
  SelMC *selmc;
  StarsSV *starsSV;
  /* cfg. */

  double CurPosKeV;
  double nowCurrent;

  /* MCA */
  QString ROIStart[19];
  QString ROIEnd[19];
  int MCALength;
  bool inMCAMeas;
  bool validMCAData;
  int cMCACh;
  //  int oldMCACh;
  int MCAStage;
  ViewCTRL *cMCAViewC;
  MCAView *cMCAView;
  int *MCAData;
  MCASTARTRESUME StartResume;
  bool MCAClearRequest;
  QFileDialog *MCAFSel;

  /* ReadData */
  QVector<Data*> Datas;
  void setupReadDataArea( void );

  /* Special Units */
  AUnit *MMainTh;               // main Th ax
  AUnit *SI0, *SI1, *SFluo;       // I0, I1, and Fluorescence
  AUnit *EncMainTh;

  void InitAndIdentifyMotors( void );
  void InitAndIdentifySensors( void );

  QTimer *GoTimer, *MCATimer, *ScanTimer, *MonTimer, *MeasTimer, *MeasDarkTimer;

  Stars *s;
  double MonoCryD;

  MEASMODE MeasFileType;


  AtomNo SelectedA;
  PeriodicTable *PT;
  AbEN SelectedE;
  //  double CurPosKeV;      // Current Position of MonoChro in Energy (keV)
  QVector<QComboBox *> GoUnit;
  QVector<QLineEdit *> GoPosEdit;
  double GoPosKeV[ GOS ];

  void setupLogArea( void );
  void setupCommonArea( void );
  void setupSetupArea( void );
  void setupSetupSSDArea( void );
  void setupMeasArea( void );

  QStringList DriverList;

  MUnits mUnits;

  /* InterFace.cpp */ /**********************************************/

  void MoveCurThPosKeV( double keV ); // Move current Pos. of Mon. in keV
#if 0
  double GetCurPosKeV( void );   // Read out current Pos. of Mon. in keV
  int isFinishedCurMove( void ); 
  void MeasureICH( int devNo, double dwell );
  void MeasureSSD( double dwell );
  void MeasureAUX( int devNo, double dwell );
  void ReadOutTransData( double NowKeV );
  void ReadOutSSDData( double NowKeV );
  void ReadOutAUXData( int ch, double NowKeV );
  int isFinishedMeasICH( int devNo );
  int isFinishedMeasSSD( void );
  int isFinishedMeasAUX( int devNo );
#endif

  /* Dummy Functions for Dummy Interface */
  void DummyDelayMotionStart( double keV );

  /* InterFace.cpp */ /**********************************************/

  double eV2deg( double eV ) {
    double tmp = eV2a( eV )/( 2.* MonoCryD );
    return ( fabs( tmp ) > 1 ) ? -1 : asin( tmp )/(PI)*180.;
  }
  double keV2deg( double eV ) {
    double tmp = keV2a( eV )/( 2.* MonoCryD );
    return ( fabs( tmp ) > 1 ) ? -1 : asin( tmp )/(PI)*180.;
  }
  double deg2keV( double deg ) { return a2keV( sin( deg / 180. * PI ) * 2.*MonoCryD ); }
  double deg2eV( double deg ) { return a2eV( sin( deg / 180. * PI ) * 2.*MonoCryD ); }
  double eV2a( double eV ) { return E2A / eV; }
  double keV2a( double keV ) { return KE2A / keV; }
  double a2eV( double a ) { return E2A / a; }
  double a2keV( double a ) { return KE2A / a; }
  double keV2any( UNIT i, double keV ) {
    switch( (int)i ) {
    case EV: return keV * 1000;
    case KEV: return keV;
    case DEG: return keV2deg( keV );
    case ANGS: return keV2a( keV );
    }
    return 0;
  }
  double any2keV( UNIT i, double any ) {
    switch( (int)i ) {
    case EV: return any / 1000.;
    case KEV: return any;
    case DEG: return deg2keV( any );
    case ANGS: return a2keV( any );
    }
    return 0;
  }

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

  QString RadioBOn;
  QString RadioBOff;

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

  QVector<AUnit*> SensWithRange;

  QVector<QLineEdit *> BLKstart;
  QVector<QLineEdit *> BLKstep;
  QVector<QLineEdit *> BLKdwell;
  QVector<QLineEdit *> BLKpoints;

  QVector<QPushButton *> SSDbs;
  QVector<QPushButton *> SSDbs2;
  QString SSDActive;
  QString SSDnotActive;
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

  void ClearBLKs( void );
  void ShowBLKs( void );
  void WriteBF( void );

  void SelSSDs( int i );
  //  void SetSSDactive( bool active );

  QVector<double> Offsets;
  QVector<int> ChModes;

  QString fixS( QString s, int l );
  QString DFName0, DFName;
  int TP;
  double TT0;
  int inMeas, inPause, SinPause;
  int cMeasTab;          // Tab No. on which the current measurement result is displayed
  int inMoveTh;
  int MeasStage, SMeasStage;
  int MeasR, MeasB, MeasS;
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
  MEASMODE MeasDispMode[ MCHANNELS ];
  int MeasDispPol[ MCHANNELS ];   // polarity
  bool MeasSensF[ MCHANNELS ];
  double NowDwell;
  bool inMeasDark;
  bool AskingShutterClose;
  bool AskingShutterOpen;
  int MeasDarkStage;

  void ShowTotal( void );
  void CpBlock2SBlock( void );
  void WriteHeader( int Rpt );
  void WriteHeader2( int Rpt );
  void DispMeasDatas( void );
  void RecordData( void );

  QString NewLFName( void );

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

  void ShowMessageOnSBar( QString msg, int time );
  void SetNewLatticeConstant( double LC ) { MonoCryD = LC; };

  bool MeasureDark( void );

  void ChangeBLKstart( void );
  void ChangeBLKstep( void );
  void ChangeBLKpoints( void );
  void ChangeBLKdwell( void );

  //  double CurrentAngle( void );
  void ShowCurThPos( void );
  void ShowCurMotorPos( SMsg msg );

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
  void ShowNewMCAStat( void );
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
  
  void StartMeasurement( void );
  void PauseMeasurement( void );
  void SurelyStop( void );
  void GoingOn( void );
  void OkOverWrite( void );
  void RangeSelOK( void );
  void isFileNameChanged( const QString & );

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

 signals:
  //  void GiveNewView( QObject *to, ViewCTRL *view );
};

#endif
