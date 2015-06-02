#ifndef SETUPSFLUO_H
#define SETUPSFLUO_H

#include <QFileDialog>
#include <QVector>
#include <QStringList>
#include <QTimer>

#include "ui_SetUpSFluo.h"

#include "XafsM.h"
#include "MCAPeak.h"
#include "MCAView.h"
#include "enums.h"
#include "PeriodicT.h"
#include "FluoDBase.h"
#include "AUnits/AUnitSFluo.h"
#include "AUnits/MCAMap.h"

#define MCA_ID  "MCA Monitor"

class SetUpSFluo : public QWidget, private Ui::SetUpSFluo
{
  Q_OBJECT

  QString k2pFile;
  QString llFile;
  QString pagFile;
  AUnitSFluo *SFluo0;
  QString DType;
  Stars *s;
  PeriodicTable *PT2;
  FluoDBase *fdbase;
  MCAView *mcaView;
  KeV2Pix *k2p;
  QVector<QStringList> *FSTATMsgs;
  QVector<MCAPeak> *MCAPeaks;       // MCAView からコピー

  bool inMCAMeas;
  //  bool validMCAData;
  MCASTARTRESUME StartResume;
  bool MCAClearRequest;
  int MCAStage;
  QTimer *MCATimer;
  bool hvControllable;

  quint32 *MCAData;
  int cMCACh;

  bool MwSSDGotMCALen;
  double MaxMCAEnergy;
  bool MCAPreAMPGainHasSet;

  QFileDialog *MCAFSel;
  OLDNEW MCADataStat;
  OLDNEW MCANameStat;
  
  QString *ROIStart;
  QString *ROIEnd;

  QString NonSelC, SelectC;
  bool svRT, svLT;

 public:
  SetUpSFluo( QWidget *p = NULL );

  void setDType( QString dtype ) { DType = dtype; };
  void setUpMCAView( MCAView *view );
  void setK2PFile( QString name ) { k2pFile = name; };
  void setLLFile( QString name ) { llFile = name; };
  void setPAGFile( QString name ) { pagFile = name; };
  AUnitSFluo *sFluo( void ) { return SFluo0; };
  KeV2Pix *K2P( void ) { return k2p; };
  SelectCh *selBs2( void ) { return SELBs2; };
  MCAView *McaView( void ) { return mcaView; };
  quint32 *McaData( void ) { return MCAData; };
  QString *roiStart( void ) { return ROIStart; };
  QString *roiEnd( void ) { return ROIEnd; };
  int mcaCh( void ) { return MCACh->text().toInt(); };
  int cCh( void ) { return cMCACh; };
  int length( void ) { return SFluo0->length(); };
  void setMCACh( int ch ) { MCACh->setValue( ch ); };
  //  QPushButton *B_ROIsetAll( void ) { return ROIsetAll; };
  QRadioButton *B_SelRealTime( void ) { return SelRealTime; };
  QRadioButton *B_SelLiveTime( void ) { return SelLiveTime; };
  void saveTimeModes( void )
  { svRT = SelRealTime->isChecked(); svLT = SelLiveTime->isChecked(); };
  void recoverTimeModes( void )
  { SelRealTime->setChecked( svRT ); SelLiveTime->setChecked( svLT ); };
  QRadioButton *B_ShowAlwaysSelElm( void ) { return ShowAlwaysSelElm; };
  QRadioButton *B_AutoROIsetAll( void ) { return AutoROIsetAll; };
  bool isInMeas( void ) { return inMCAMeas; };
  // double currentPeakE( void ) { return (*MCAPeaks)[ MCAPeakList->currentIndex() ].BinE; };
  // double calibE( void ) { return PeakCalibrate->text().toDouble(); };
  // void clearCalibE( void ) { PeakCalibrate->setText( "" ); };
  void setupSetupSFluo( Stars *S, QVector<QStringList> *fStatMsgs );
  //  double gain( void ) { return GainInput->text().toDouble(); };
  void setViewStats( void ) {
    mcaView->setLog( SetDisplayLog->isChecked() );
    mcaView->setShowElements( DispElmNames->isChecked() );
    mcaView->setShowElementsAlways( ShowAlwaysSelElm->isChecked() );
    mcaView->setShowElementsEnergy( ShowElmEnergy->isChecked() );
  }
  bool DispElmNamesIsChecked( void ) { return DispElmNames->isChecked(); };
  bool ShowAlwaysSelElmIsChecked( void ) { return ShowAlwaysSelElm->isChecked(); }
  bool ShowElmEnergyIsChecked( void ) { return ShowElmEnergy->isChecked(); };
  bool ShowDiffIsChecked( void ) { return ShowDiff->isChecked(); };
  bool ShowSmoothedIsChecked( void ) { return ShowSmoothed->isChecked(); };
  bool FitToRawIsChecked( void ) { return FitToRaw->isChecked(); };
  bool SetDisplayLogIsChecked( void ) { return SetDisplayLog->isChecked(); }
  double maxMCAEnergy( void ) { return MaxMCAEnergy; }
  int maxLoop( void ) { return MaxLoop->text().toInt(); };
  double dampFact( void ) { return DampFact->text().toDouble(); };
  double prec1( void ) { return Prec1->text().toDouble(); };
  double prec2( void ) { return Prec2->text().toDouble(); };

  void setFDBase( FluoDBase *fb );
  void setDirectory( QString root ) { if ( MCAFSel != NULL ) MCAFSel->setDirectory( root ); };
  void setMaxMCAEnergy( double e ) { MaxMCAEnergy = e; };
  QString K2PFile( void ) { return k2pFile; };
  void setHVControllable( bool f ) { hvControllable = f; };
					     
 private slots:
  void newMaxMCAEnergy( void );
  void newMaxLoop( void );
  void newDampFact( void );
  void newPrec1( void );
  void newPrec2( void );
  //  void MoveToNewCaribEnergy( void );  
  //  void NewAttenPos( void );
  //  void NewAttenCh( int att );
  //  void newCalibration( void );
  void ReadLowerLimitSetting( void );
  void SelectedLimitPSEnergy( bool f );
  //  void SelectedShowDiff( bool f );
  //  void SelectedPeakSearch( bool f );
  //  void SelectedShowSmoothed( bool f );
  //  void SelectedFitToRaw( bool f );
  //  void PushedReFit( void );
  //  void PushedClearMCAPeaks( void );
  //  void newPSSens( void );
  void newGain( void );
  void saveMCAData( void );
  void setSelectedMCAFName( const QString &fname );
  void RealTimeIsSelected( void );
  void LiveTimeIsSelected( void );
  void setROILen( void );  // 初期化の時に一回しか呼ばれないと信じる
  void newROIStart( const QString &newv );
  void newROIEnd( const QString &newv );
  void MCAChSelected( int i );
  void showPeakingTime( SMsg msg );
  void showThreshold( SMsg msg );
  void showCalibration( SMsg msg );
  void showDynamicRange( SMsg msg );
  void showPreAMPGain( SMsg msg );
  void StartMCA( void );
  void showCurrentValues( int atCur, int inROI );
  void setNewROI( int s, int e );
  void MCASequence( void );
  void ShowNewMCAStat( char * );
  void ShowNewMCARealTime( int ch );
  void ShowNewMCALiveTime( int ch );
  void clearMCA( void );
  void gotNewPeakList( QVector<MCAPeak> *peaks );
  void setPreAMPGains( void );
  void nowFitStat( QString &stat );
  void newCalibration( void );
  void setROIs( void );
  void showHVStat( int stat );

public slots:
  void GetMCASettings( int ch );
  void setAllROIs( void );

 signals:
  void ReCalcXAFSWithMCA( void );
  void ReCalcS2DMap( SetUpSFluo *ssfluo );
  void ShowMessage( QString msg, int time );
  void askToSaveMCAData( SetUpSFluo *ssfluo, QString fname );
  //  void NewMCACh( int ch );
  void addAnUsingUnit( QString id, AUnit0 *sfluo );
  void removeUsingUnits( QString id, AUnit0 *sfluo );

  void SignalMCAViewSetDisplayLog( bool f );
  void SignalMCAViewSetShowElements( bool f );
  void SignalMCAViewShowAlwaysSelElm( bool f );
  void SignalMCAViewShowElmEnergy( bool f );

  void showMyMCAView( SetUpSFluo *ssfluo );

  void askSetFDBase( FluoDBase *fb );
  void askSetMaxMCAEnergy( double maxe );
  void askSetMaxLoop( int loop );
  void askSetDampFact( double df );
  void askSetPrec1( double p1 );
  void askSetPrec2( double p2 );
  void askSetLimitPSEnergy( bool f );
  void askSetShowDiff( bool f );
  void askSetPeakSearch( bool f );
  void askSetShowSmoothed( bool f );
  void askSetFitToRaw( bool f );
  void askDoPeakFitWCPoints( void );
  void askClearMCAPeaks( void );
  void askSetROI( int rs, int re );
  void askSetMCACh( int ch );
};

#endif
