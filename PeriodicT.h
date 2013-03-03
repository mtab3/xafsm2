#ifndef PERIODICT_H
#define PERIODICT_H

#include <QSignalMapper>

#include "ui_PeriodicT.h"
#include "AtomGroup.h"

enum PTWHENSELECT { PT_CLOSE, PT_HIDE, PT_STAY, PTWHENSELECTS };

class PeriodicTable : public QMainWindow, private Ui::PeriodicT
{
  Q_OBJECT

public:
  PeriodicTable();

private:
  QSignalMapper *PBMap;

  bool isExclusive;
  int PBs;
  QPushButton *PB[ 150 ];
  int PTMask[ 150 ];       // 1 start
  int SelectedAtom;
  int WhenSelected;
  int WhenClosed;

public:
  /******* Control Functions *******/
  void SetActionOnSelected( PTWHENSELECT action ) { WhenSelected = action; };
  void SetActionOnClosed( PTWHENSELECT action ) { WhenClosed = action; };
  void SetPTMask( int i, int mask );
  void SetPTMasks( int s, int e, int mask );
  void SetAGColor( AtomGroup *ag );
  void SetCheckable( bool f );
  int Atoms( void ) { return PBs; };
  void SetAColor( int i, QString c )
  { PB[i]->setStyleSheet( "background-color: " + c ); };
  QStringList getSelectedAtoms( void );

public slots:
  void ShowAllNoneBs( bool show ) { AllNoneBs->setVisible( show ); };
  void setAll( void );
  void setNone( void );

private slots:
  void SetPBs( void );
  void doPB( int i );
  void PTClose( void );

signals:
  /******* Result Action Signals ********/
  void AtomSelected( int i );
  void AtomToggled( bool f, int i );
  void PTClosing( int i );
};

#endif
