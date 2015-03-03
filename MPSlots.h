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
  void ReCalcS2DMap( void );
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
//  void setSelectedMCAFName( const QString &fname );
//  void newGain( void );
  void PopChangeMonLines( bool f );
  void PopDownMonLines( void );
  void newMovingAvr( void );

//  void ShowNewMCAStat( char *MCAs );
//  void ShowNewMCARealTime( int ch );
//  void ShowNewMCALiveTime( int ch );
  void saveMonData( void );
  void SaveMonInfo( MonInfo *set );
//  void gotNewPeakList( QVector<MCAPeak>* );
//  void newPSSens( void );
//  void SelectedShowDiff( bool f );
//  void SelectedPeakSearch( bool f );
//  void SelectedShowSmoothed( bool f );
//  void SelectedFitToRaw( bool f);
//  void PushedReFit( void );
//  void PushedClearMCAPeaks( void );
//  void SelectedLimitPSEnergy( bool f );
  void newCalibration( void );
//  void newMaxLoop( void );
//  void newDampFact( void );
//  void newPrec1( void );
//  void newPrec2( void );
  void SaveS2DMCAs( void );
  //  void S2DMCAWriteNext( void );
  void SaveMCADataOnMem( aMCASet *set, SetUpSFluo *sSFluo );
  void addAnUsingUnit( QString id, AUnit0 *unit );
  void removeUsingUnits( QString id, AUnit0 *unit );

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

//  void StartMCA( void );
//  void MCAChSelected( int i );
//  void showPeakingTime( SMsg mag );
//  void showThreshold( SMsg mas );
//  void showCalibration( SMsg mas );
//  void showDynamicRange( SMsg msg );
//  void showPreAMPGain( SMsg msg );
//  void getMCALen( SMsg msg );
//  void getMCASettings( int ch );
//  void newROIStart( const QString &newv );
//  void newROIEnd( const QString &newv );
//  void showCurrentValues( int, int );
//  void setNewROI( int, int );
//  void clearMCA( void );
//  void RealTimeIsSelected( void );
//  void LiveTimeIsSelected( void );
//  void saveMCAData( void );
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

//  void setPreAMPGains( void );
  //  void SelSSDs0( void );
  //  void SelSSDs20( void );
  void SelectedNDFN( const QString &fname );
  void NewRpt( void );
//  void setAllROIs( void );
//  void newMaxMCAEnergy( void );

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
  void newSSDChSelection( int i, bool f );  //ReCalcSSDTotal( int i, bool f );
  void moveToATab( int tab );
  void NoticeSelectedStats( int tab );
  //  void doPeakFit( void );
//  void ReadLowerLimitSetting( void );
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
//  void MCASequence( void );
  void MeasDarkSequence( void );

  void TryToNoticeCurrentView( void );
  void TryToGiveNewView( DATATYPE dtype, QString dir );
  //  void DeleteTheView( void );
  void ShowButtonsForCurrentTab( void );
  void ShowButtonsForATab( int i );

  void setEncNewTh( QString orig, QString newv );
  void SetNewGases( void );
  //  void showMCAs( void );
//  void nowFitStat( QString &stat );

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
  void ReCalcS2DMap( SetUpSFluo *ssfluo );

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

  void RcvEvAll( SMsg msg );
  void alarmOn( void );
  void alarmOff( void );
