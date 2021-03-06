
#include <QMessageBox>
#include "MultiUnits.h"

MUnits::MUnits( QObject *p ) : QObject( p )
{
  OneByOne = false;
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
}

void MUnits::addUnit( ASensor *au )
{
  int i;
  MUElement *mue = new MUElement;
  mue->au = au;
  Units << mue;

  if ( au->hasParent() ) {
    for ( i = 0; i < PUnits.count(); i++ ) {
      if ( PUnits.at(i)->au->uid() == au->pUid() )
	break;
    }
    if ( i >= PUnits.count() ) {
      MUElement *pmue = new MUElement;
      pmue->au = (ASensor*)(au->theParent());
      PUnits << pmue;
    }
  }
}

void MUnits::setDwellTimes( double dt )   // 親子全員に同じ時間を配る
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

bool MUnits::isBusy2( void )
{
  bool ff = false;

  for ( int i = 0; i < PUnits.count(); i++ ) {
    ff |= PUnits.at(i)->au->isBusy2();
  }
  for ( int i = 0; i < Units.count(); i++ ) {
    ff |= Units.at(i)->au->isBusy2();
  }

  return ff;
}

void MUnits::clearStage( void )
{
  for ( int i = 0; i < PUnits.count(); i++ ) {
    PUnits.at(i)->au->initLocalStage();
  }
  for ( int i = 0; i < Units.count(); i++ ) {
    Units.at(i)->au->initLocalStage();
  }
}

void MUnits::clearDoneF( void )
{
  for ( int i = 0; i < Units.count(); i++ ) {
    Units.at(i)->done = false;
  }
}

// 親ユニットを持った QXAFS 可能なユニットが出てくるまで親ユニットのことは気にしない
bool MUnits::QStart( void )   // QXAFS
{
  bool ff = false;

  for ( int i = 0; i < Units.count(); i++ ) {
    ff |= Units.at(i)->au->QStart();
  }

  return ff;
}

// 親ユニットを持った QXAFS 可能なユニットが出てくるまで親ユニットのことは気にしない
bool MUnits::QRead( void )   // QXAFS
{
  bool f;
  bool ff = false;

  for ( int i = 0; i < Units.count(); i++ ) {
    if ( OneByOne ) {
      if ( ! Units.at(i)->done ) {
	ff = Units.at(i)->au->QRead();
	Units.at(i)->done = true;
	break;
      }
    } else {
      ff |= ( f = Units.at(i)->au->QRead() );
    }
  }

  return ff;
}

// 親ユニットを持った QXAFS 可能なユニットが出てくるまで親ユニットのことは気にしない
bool MUnits::QEnd( void )   // QXAFS
{
  bool ff = false;

  for ( int i = 0; i < Units.count(); i++ ) {
    ff |= Units.at(i)->au->QEnd();
  }

  return ff;
}


bool MUnits::init( void )
{
  bool ff = false;

  for ( int i = 0; i < PUnits.count(); i++ ) {
    ff |= PUnits.at(i)->au->InitSensor();
  }
  for ( int i = 0; i < Units.count(); i++ ) {
    if (( Units.at(i)->au->hasParent() )&&( Units.at(i)->au->type() == "PAM2" ))
      continue;
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
		     .arg( rv ).arg( PUnits.at(i)->au->name() )
		     .arg( PUnits.at(i)->dt ) );
      msg1->setWindowTitle( tr( "Warning on dwell time" ) );
      connect( msg1, SIGNAL( buttonClicked( QAbstractButton * ) ),
	       this, SLOT( ShownMessage( QAbstractButton * ) ),
	       Qt::UniqueConnection );
      msg1->show();
    }
  }
  for ( int i = 0; i < Units.count(); i++ ) {
    if (( Units.at(i)->au->hasParent() )&&( Units.at(i)->au->type() == "PAM2" ))
      continue;
    if ( ( rv = Units.at(i)->au->SetTime( Units.at(i)->dt ) ) != Units.at(i)->dt ) {
      // 設定しようとした値と実際に設定された値が違ってたら
      QMessageBox *msg1 = new QMessageBox;
      msg1->setModal( false );
      msg1->setText( tr( "Dwell time was set [%1] for [%2],"
			 " though tried to be as [%3]." )
		     .arg( rv ).arg( Units.at(i)->au->name() )
		     .arg( Units.at(i)->dt ) );
      msg1->setWindowTitle( tr( "Warning on dwell time" ) );
      connect( msg1, SIGNAL( buttonClicked( QAbstractButton * ) ),
	       this, SLOT( ShownMessage( QAbstractButton * ) ),
	       Qt::UniqueConnection );
      msg1->show();
    }
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

  // qDebug() << "mUnits getValue0 " << PUnits.count();
  for ( int i = 0; i < PUnits.count(); i++ ) {
    ff |= PUnits.at(i)->au->GetValue0();
  }

  return ff;
}

bool MUnits::getValue02( void )
{
  bool ff = false;

  for ( int i = 0; i < PUnits.count(); i++ ) {
    ff |= PUnits.at(i)->au->GetValue02();
  }

  return ff;
}


bool MUnits::getValue( void )
{
  bool ff = false;

  for ( int i = 0; i < PUnits.count(); i++ ) {
    if ( PUnits.at(i)->au->type() == "PAM2" )
      ff |= PUnits.at(i)->au->GetValue();
  }
  for ( int i = 0; i < Units.count(); i++ ) {
    // qDebug() << "mGetValue " << i << Units.at(i)->au->uid() << Units.at(i)->au->name() << Units.at(i)->au->type() << Units.at(i)->au->type2();
    if ( Units.at(i)->au->type() != "PAM2" )
      ff |= Units.at(i)->au->GetValue();
  }

  return ff;
}

bool MUnits::Close( void )
{
  bool ff = false;

  for ( int i = 0; i < PUnits.count(); i++ ) {
    ff |= PUnits.at(i)->au->Close();
  }

  return ff;
}

void MUnits::readValue( double *rvs, double *cps, bool correctBack )
// 登録されているユニットの現在値を前詰めの配列で返す
{
  ASensor *as;
  for ( int i = 0; i < Units.count(); i++ ) {
    as = Units.at(i)->au;
    rvs[i] = as->value().toDouble();
    if ( correctBack )
      rvs[i] -= as->GetDark( as->getSetTime() );
    cps[i] = as->convert2perTime( rvs[i], as->getSetTime() );
  }
}
