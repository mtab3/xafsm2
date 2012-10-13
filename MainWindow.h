#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMessageBox>

#include <math.h>

#include "ui_MainWindow.h"
#include "SelMC.h"
#include "StarsSV.h"

#include "XafsM.h"
#include "Atoms.h"
#include "Stars.h"
#include "PeriodicT.h"
#include "AUnit.h"

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
const int MaxBLKs = 6;        // Max Blocks
const int MaxSSDs = 19;       // Max SSD elements
/******************************************************************************/

struct DRVDef {
  QString name;
  QString comment;
};

class MainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

public:
  MainWindow( QString myname );

private:

  QString XAFSName;
  QString XAFSKey;
  QString XAFSTitle;

  /* ReadDef */
  QVector<AUnit *> AMotors;
  QVector<AUnit *> ASensors;
  void ReadDef( QString fname );
  QString nextItem( QString start, QString &item );

  /* cfg. */
  SelMC *selMC;
  StarsSV *starsSV;
  /* cfg. */

  double CurPosKeV;
  double nowCurrent;

  /* Special Units */
  AUnit *MMainTh;               // main Th ax
  AUnit *SI0, *SI1, *SFluo;       // I0, I1, and Fluorescence
  void InitAndIdentifyMotors( void );
  void InitAndIdentifySensors( void );

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
  void setupMeasArea( void );

  QVector<DRVDef *> DriverList;

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

  void timerEvent( QTimerEvent *event );
  //  int WatchCurPosID;

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

  void GoMAtPuls( int Pos );
  void GoMStop0( void );
  void ShowTAE( void );
  void GetNewGo( int i );
  void ShowGo( int i );
  void SetNewGos( void );
  void SPSStart( int AbsRel );
  void SPSStop0( void );
  void ReadOutScanData( void ); // ( int NowP );
  void ShowRelAbs( void );

  int inMMove;
  int MoveID;            // Timer ID
  int MovingM;           // Moving motor ID
  int MovingS;           // Moving motor Speed
  RELABS RelAbs;
  int SPSMon;            // SPS-ing monitor
  int SPSID;             // Timer ID
  int inSPSing;
  int ScanStage;
  int ScanSP, ScanEP, ScanSTP;
  double ScanDW;
  int NowScanP;
  XView *SPSView;
  QString RadioBOn;
  QString RadioBOff;

  bool MeasCntIs;
  int MeasCntNo;
  int MeasChNo;
  bool setupMDispFirstTime;

  int inMonitor;
  int MonID;
  int MonStage;
  int MonDev;
  double MonMeasTime;
  double NewMonV, OldMonV;
  double NewMonV0, OldMonV0;
  int MonUsedOldV;
  int MonUsedOldV0;
  XView *MonView;

  QVector<QLineEdit *> BLKstart;
  QVector<QLineEdit *> BLKstep;
  QVector<QLineEdit *> BLKdwell;
  QVector<QLineEdit *> BLKpoints;

  QVector<QPushButton *> SSD;
  //  MEASMODE MeasMode;
  QFileDialog *SelDFND;
  QFileDialog *SelWBFND;
  QFileDialog *SelRBFND;
  QFileDialog *SelLFND;

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

  int SelectedSSD[ MaxSSDs ];

  void ChangeBLKstart( int i );
  void ChangeBLKstep( int i );
  void ChangeBLKpoints( int i );
  void ChangeBLKdwell( int i );
  void ClearBLKs( void );
  void ShowBLKs( void );
  void WriteBF( void );

  void SelSSDs( int i );
  //  void SetSSDactive( bool active );

  QString fixS( QString s, int l );
  QString DFName0, DFName;
  int TP;
  double TT0;
  int inMeas, inPause, SinPause;
  int inMoveTh;
  int MeasStage, SMeasStage;
  int MeasID;
  int MeasR, MeasB, MeasS;
  double GoToKeV;
  double InitialKeV;
  double r[ 100 ];
  QMessageBox *StopP;
  void ClearNowView( void );
  int GetDFName0( void );
  void SetDFName( int i );
  AUnit *MeasSens[ 5 ];
  bool MeasSensF[ 5 ];
  bool OneOfTheSensorIsCounter;
  AUnit *TheCounter;

  void ShowTotal( void );
  void CpBlock2SBlock( void );
  void WriteHeader( int Rpt );
  void RecordDataTrans( int devieNo, double r );
  void RecordDataSSD( int devieNo, double r );
  void RecordDataAUX( int devieNo, double r );

  QString NewLFName( void );

  QVector<XView *> XViews;
  XView *NowView;
  void setupView( void );

  /* in Timer.cc */
  void WatchPos( void );
  void MotorMove( void );
  void MeasSequence( void );
  void SPSSequence( void );
  void MonSequence( void );
  bool CheckDetectorSelection( void );

  bool isBusyMotorInMeas( void );
  bool isBusySensorInMeas( void );
  void ClearSensorStages( void );
  bool InitSensors( void );
  void SetDwellTime( double dtime );
  bool GetSensValues0( void );
  bool GetSensValues( void );
  void ReadSensValues( void );

private slots:
  void InitializeUnitsAgain( void );

  void ShowMessageOnSBar( QString msg, int time );
  void SetNewLatticeConstant( double LC ) { MonoCryD = LC; };

  void ShowCurThPos( SMsg msg );
  void ShowCurMotorPos( SMsg msg );

  void MMRel( void );
  void MMAbs( void );

  void NewSelA( int i );
  void OpenPT( void );
  void AtomSelectedByPT( int i );
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
  void GoMAt( void );
  void GoMStop( void );
  void SPSAS( void );
  void SPSRS( void );
  void Monitor( void );

  void ChangeBLKUnit( int i );
  void ChangeBLKs( int i );

  void ChangeBLKstart00( void );
  void ChangeBLKstart01( void );
  void ChangeBLKstart02( void );
  void ChangeBLKstart03( void );
  void ChangeBLKstart04( void );
  void ChangeBLKstart05( void );
  void ChangeBLKstart06( void );

  void ChangeBLKstep00( void );
  void ChangeBLKstep01( void );
  void ChangeBLKstep02( void );
  void ChangeBLKstep03( void );
  void ChangeBLKstep04( void );
  void ChangeBLKstep05( void );

  void ChangeBLKpoints00( void );
  void ChangeBLKpoints01( void );
  void ChangeBLKpoints02( void );
  void ChangeBLKpoints03( void );
  void ChangeBLKpoints04( void );
  void ChangeBLKpoints05( void );

  void ChangeBLKdwell00( void );
  void ChangeBLKdwell01( void );
  void ChangeBLKdwell02( void );
  void ChangeBLKdwell03( void );
  void ChangeBLKdwell04( void );
  void ChangeBLKdwell05( void );

  void SetStdEXAFSBLKs( void );
  void SetStdXANESBLKs( void );
  void SetDwells( void );
  void SelectedWBFN( const QString &fname );
  void SelectedRBFN( const QString &fname );

#if 0
  void Mode2Tr( void );
  void Mode2Fl( void );
  void Mode2Au( void );
#endif

  void SelSSDs00( void );
  void SelSSDs01( void );
  void SelSSDs02( void );
  void SelSSDs03( void );
  void SelSSDs04( void );
  void SelSSDs05( void );
  void SelSSDs06( void );
  void SelSSDs07( void );
  void SelSSDs08( void );
  void SelSSDs09( void );
  void SelSSDs10( void );
  void SelSSDs11( void );
  void SelSSDs12( void );
  void SelSSDs13( void );
  void SelSSDs14( void );
  void SelSSDs15( void );
  void SelSSDs16( void );
  void SelSSDs17( void );
  void SelSSDs18( void );

  void SelectedNDFN( const QString &fname );
  void NewRpt( void );
  
  void StartMeasurement( void );
  void PauseMeasurement( void );
  void SurelyStop( void );
  void GoingOn( void );

  void NewLogMsg( QString msg );
  void SelLFN( void );
  void SetNewLFName( const QString &name );
  void AddLogComment( void );
};

#endif
