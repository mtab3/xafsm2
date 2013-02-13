#include <QMessageBox>
#include "MultiUnits.h"

MUnits::MUnits( QObject *p ) : QObject( p )
{
}

void MUnits::clearUnits( void )
{
  for ( int i = 0; i < Units.count(); i++ ) {
    delete Units.at(i);
  }
  Units.clear();
  for ( int i = 0; i < PUnits.count(); i++ ) {
    delete PUnits.at(i);
  }
  PUnits.clear();
};

void MUnits::addUnit( AUnit *au )
{
  int i;
  MUElement *mue = new MUElement;
  mue->au = au;
  Units << mue;

  if ( au->hasParent() ) {
    for ( i = 0; i < PUnits.count(); i++ ) {
      if ( PUnits.at(i)->au->getUid() == au->getPUid() )
	break;
    }
    if ( i >= PUnits.count() ) {
      MUElement *pmue = new MUElement;
      pmue->au = au->getTheParent();
      PUnits << pmue;
    }
  }
}

void MUnits::setDwellTimes( double dt )
{
  for ( int i = 0; i < Units.count(); i++ ) {
    Units.at(i)->dt = dt;
  }
  for ( int i = 0; i < PUnits.count(); i++ ) {
    PUnits.at(i)->dt = dt;
  }
}

bool MUnits::isBusy( void )
{
  bool ff = false;

  for ( int i = 0; i < PUnits.count(); i++ ) {
    ff |= PUnits.at(i)->au->isBusy() || PUnits.at(i)->au->isBusy2();
  }
  for ( int i = 0; i < Units.count(); i++ ) {
    ff |= Units.at(i)->au->isBusy() || Units.at(i)->au->isBusy2();
  }

  return ff;
}

void MUnits::clearStage( void )
{
  for ( int i = 0; i < PUnits.count(); i++ ) {
    PUnits.at(i)->au->InitLocalStage();
  }
  for ( int i = 0; i < Units.count(); i++ ) {
    Units.at(i)->au->InitLocalStage();
  }
}

bool MUnits::init( void )
{
  bool ff = false;

  for ( int i = 0; i < PUnits.count(); i++ ) {
    ff |= PUnits.at(i)->au->InitSensor();
  }
  for ( int i = 0; i < Units.count(); i++ ) {
    ff |= Units.at(i)->au->InitSensor();
  }

  return ff;
}

void MUnits::setDwellTime( void )  // これもホントは返答を待つ形にするべき
{
  double rv;

  for ( int i = 0; i < PUnits.count(); i++ ) {
    if ( ( rv = PUnits.at(i)->au->SetTime( PUnits.at(i)->dt ) ) != PUnits.at(i)->dt ) {
      // 設定しようとした値と実際に設定された値が違ってたら
      QMessageBox *msg1 = new QMessageBox;
      msg1->setModal( false );
      msg1->setText( tr( "Dwell time was set [%1] for [%2],"
			 " though tried to be as [%3]." )
		     .arg( rv ).arg( PUnits.at(i)->au->getName() )
		     .arg( PUnits.at(i)->dt ) );
      msg1->setWindowTitle( tr( "Warning on dwell time" ) );
      connect( msg1, SIGNAL( buttonClicked( QAbstractButton * ) ),
	       this, SLOT( ShownMessage( QAbstractButton * ) ) );
      msg1->show();
    }
  }
  for ( int i = 0; i < Units.count(); i++ ) {
    //    if ( ! Units.at(i)->au->hasParent() ) {
    if ( ( rv = Units.at(i)->au->SetTime( Units.at(i)->dt ) ) != Units.at(i)->dt ) {
      // 設定しようとした値と実際に設定された値が違ってたら
      QMessageBox *msg1 = new QMessageBox;
      msg1->setModal( false );
      msg1->setText( tr( "Dwell time was set [%1] for [%2],"
			 " though tried to be as [%3]." )
		     .arg( rv ).arg( Units.at(i)->au->getName() )
		     .arg( Units.at(i)->dt ) );
      msg1->setWindowTitle( tr( "Warning on dwell time" ) );
      connect( msg1, SIGNAL( buttonClicked( QAbstractButton * ) ),
	       this, SLOT( ShownMessage( QAbstractButton * ) ) );
      msg1->show();
    }
    // }
  }
}

void MUnits::ShownMessage( QAbstractButton * )
{
  QMessageBox *msg1 = (QMessageBox *)sender();
  disconnect( msg1, SIGNAL( buttonClicked( QAbstractButton * ) ),
	      this, SLOT( ShownMessage( QAbstractButton * ) ) );
}

bool MUnits::getValue0( void )
{
  bool ff = false;

  for ( int i = 0; i < PUnits.count(); i++ ) {
    ff |= PUnits.at(i)->au->GetValue0();
  }

  return ff;
}

bool MUnits::getValue( void )
{
  bool ff = false;

  for ( int i = 0; i < Units.count(); i++ ) {
    ff |= Units.at(i)->au->GetValue();
  }

  return ff;
}

void MUnits::readValue( double *rvs, bool correctBack )
// 登録されているユニットの現在値を前詰めの配列で返す
{
  for ( int i = 0; i < Units.count(); i++ ) {
#if 0                          // values().at(0) is total of all 19 CHs
    if ( Units.at(i)->au->getType() == "SSD" ) {
      rvs[i] = Units.at(i)->au->values().at(0).toDouble();
    } else {
      rvs[i] = Units.at(i)->au->value().toDouble();
    }
#else
    rvs[i] = Units.at(i)->au->value().toDouble();
    if ( correctBack )
      rvs[i] -= Units.at(i)->au->getDark() * Units.at(i)->au->GetSetTime();
#endif
  }
}

bool MUnits::getMCA( int ch )
{
  bool ff = false;

  for ( int i = 0; i < Units.count(); i++ ) {
    ff |= Units.at(i)->au->GetMCA( ch );
  }
  
  return ff;
}
