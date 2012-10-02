#ifndef PERIODICT_H
#define PERIODICT_H

#include <QSignalMapper>

#include "ui_PeriodicT.h"

enum PTWHENSELECT { PT_CLOSE, PT_HIDE, PT_STAY, PTWHENSELECTS };

class PeriodicTable : public QMainWindow, private Ui::PeriodicT
{
  Q_OBJECT

public:
  PeriodicTable();

private:
  QSignalMapper *PBMap;

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

private slots:
  void SetPBs( void );
  void doPB( int i );
  void PTClose( void );


signals:
  /******* Result Action Signals ********/
  void AtomSelected( int i );
  void PTClosing( int i );
};

#endif
