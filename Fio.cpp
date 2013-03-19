#include <QtGui>
#include <math.h>

#include "MainWindow.h"

#define BLNAME    ( "BL5S1" )

int MainWindow::GetDFName0()
{
  DFName0 = EditDFName->text();

  if ( DFName0.isEmpty() ) {
    return 0;
  }

  if ( DFName0.toUpper().lastIndexOf( ".DAT" ) == DFName0.length() - 4 ) {
    DFName0 = DFName0.left( DFName0.length() - 4 );
  }

  return 1;
}

void MainWindow::SetDFName( int i )
{
  QString buf;

  if ( i == 0 ) {
    DFName = DFName0 + ".dat";
  } else {
    buf.sprintf( ".%03d", i );
    DFName = DFName0 + buf;
  }
}

QString MainWindow::fixS( QString s, int l )
{
  QString r;

  r.fill( ' ', l - s.size() );
  s += r;
  s.left( l );

  return s;
}

void MainWindow::WriteHeader( int Rpt )
{
  bool newf = conds->isUse1303Format();
  QString NFM = "";   // new file mark

  int cnt;
  SetDFName( Rpt );   // Generate a file name with repitation number
  
  QFile file( DFName );
  if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;
  
  // Writing fixed headers
  QTextStream out(&file);

  if ( newf ) {
    out << "##  1303" << endl;
    out << "### An extended 9809 format file." << endl;
    out << "### To convert the 1303 format file to 9809 format file," << endl;
    out << "###   1) When a line is started with single '#'," << endl;
    out << "###        the one character should be deleted," << endl;
    out << "###   2) When a line is started with '##' or '###'," << endl;
    out << "###        remove the line simply," << endl;
    out << "###   3) When '###' appears at the middle of a line," << endl;
    out << "###        the rest of the line including the '###' should be omitted.";
    out << endl;
    NFM = "#";
  }

  out << NFM << " " << fixS( " 9809     AichiSR", 20 ) << fixS( BLNAME, 5 ) << endl;

  out << NFM << " " << fixS( DFName, 14 ) << "  "
      << QDateTime::currentDateTime().toString("yy.MM.dd hh:mm") << " - "
      << "%001%" << endl; // �ִ��ѤΥޡ��� 001 ���� 14�彪λ���� "yy.MM.dd hh:mm"
  out << " " << fixS( EditCMT->text(), 80 ) << endl;

  out << NFM << " " << "Ring : " << "  1.2 GeV"
      << QString( "  %1 mA - " ).arg( SLS->value().toDouble(), 6, 'f', 1, ' ' )
      << "%002%"          // �ִ��ѤΥޡ��� 002 ���� �¿�6.1�����ή
      << " mA" << endl;

  out << NFM << " " << "Mono :   " << fixS( mccd[ selmc->MC() ]->getMCName(), 10 )
      << QString( "    D= %1 A    " ).arg( mccd[ selmc->MC() ]->getD(), 8, 'f', 5, ' ' )
      << QString( "Initial angle=%1 deg" ).arg(u->keV2deg( InitialKeV ), 9, 'f', 5, ' ') 
      << endl;

  out << NFM << " " << fixS( BLNAME, 5 ) << "    "
      << fixS( CMode[ MeasFileType ], 13 ) << QString( "(%1)" ).arg( MeasFileType, 2 )
      << QString( "   Repitition=%1" ).arg( MeasR + 1, 3 )
      << QString( "     Points=%1" ).arg( TP, 5 ) << endl;
  
  if ( SBLKUnit == DEG ) {
    out << NFM << " "
        << QString( "Param file : %1  angle axis (1)     Block =%2" )
                    .arg( fixS( "DUMMYNAME.prm", 14 ) ).arg( Blocks, 2 ) << endl;
    out << endl;

    out << NFM << " "
        << "Block      Init-ang  final-ang     Step/deg     Time/s       Num" << endl;

    for ( int i = 0; i < Blocks; i++ ) {
      out << NFM << " "
          << QString( "%1     %2%3%4%5%6" )
	     .arg( i+1, 5 )
	     .arg( SBlockStart[i], 10, 'f', 5 )
             .arg( SBlockStart[i+1], 10, 'f', 5 )
             .arg( SBlockStep[i], 13, 'f', 6 )
             .arg( SBlockDwell[i], 11, 'f', 2 )
             .arg( SBlockPoints[i], 10 )
          << endl;
    }
  } else {
    out << NFM << " "
        << QString( "Param file : %1  energy axis (2)     Block =%2" )
                    .arg( fixS( "DUMMYNAME.prm", 14 ) ).arg( Blocks, 2 ) << endl;
    out << endl;

    out << NFM << " "
        << "Block      Init-Eng  final-Eng     Step/eV     Time/s       Num" << endl;
    for ( int i = 0; i < Blocks; i++ ) {
      out << NFM << " "
          << QString( "%1     %2%3%4%5%6" )
             .arg( i+1, 5 )
             .arg( u->any2keV( SBLKUnit, SBlockStart[i] ) * 1000, 10, 'f', 2 )
             .arg( u->any2keV( SBLKUnit, SBlockStart[i+1] ) * 1000, 10, 'f', 2 )
             .arg( u->any2keV( SBLKUnit, SBlockStep[i] ) * 1000, 13, 'f', 2 )
             .arg( SBlockDwell[i], 11, 'f', 2 )
             .arg( SBlockPoints[i], 10 )
          << endl;
    }
  }
  out << NFM << " " << QString( "SCALE( 2)     NDCH =%1" ).arg( MeasChNo, 2 ) << endl;

  QVector<double> darks;
  switch( MeasFileType ) {
  case TRANS:
    out << NFM << "  Angle(c)  Angle(o)    time/s";
    for ( int i = 0; i < MeasChNo; i++ )
      out << QString( "%1" ).arg( i+1, 10 );
    out << endl;

    out << NFM << QString( "      Mode         0         0"
		    "%1%2" ).arg( 1, 10 ).arg( 2, 10 ) << endl;

    out << NFM << QString( "    Offset         0         0"
		    "%1%2" ).arg( mUnits.at(0)->getDark(), 10, 'f', 3 )
                            .arg( mUnits.at(1)->getDark(), 10, 'f', 3 ) << endl;
    break;

  case FLUO:    // FLUO �� EXTRA �ϰ��٤�Ʊ��(���)�ˤʤä��Τˡ�
    // I0 �ΰ��֤��Ѥ��ʤ��Ȥ����ʤ����Ȥ�Ƚ�����ʤΤǤޤ�ʬΥ��
    out << NFM << "  Angle(c)  Angle(o)    time/s";
    // FLUO �λ� mUnits �����Ǥ��¤Ӥ�ɬ�� I0, 19ch SSD �ˤʤäƤ�Ϥ�
    for ( int j = 0; j < MaxSSDs; j++ ) {
      out << QString( "%1" ).arg( j+1, 10 );  // 19ch SSD
    }
    out << QString( "%1" ).arg( MaxSSDs + 1, 10 );    // I0
    for ( int j = 0; j < MaxSSDs; j++ ) {
      // ICR  19ch SSD ���ֹ�Ȥ�����
      out << QString( "%1" ).arg( j+1, 10 );
    }
    out << QString( "%1" ).arg( MaxSSDs + 1, 10 );    // resets
    out << endl;

    out << NFM << QString( "      Mode         0         0" );    // Modes Line
    for ( int j = 0; j < MaxSSDs; j++ ) {
      out << QString( "%1" ).arg( FLUO, 10 );  // 19ch SSD
    }
    out << QString( "%1" ).arg( 1, 10 );       // I0 : I0 �Ϸ���Ǥ��� 1
    for ( int j = 0; j < MaxSSDs; j++ ) {
      out << QString( "%1" ).arg( FLUO + 100, 10 );  // ICR
    }
    out << QString( "%1" ).arg( 101, 10 );     // resets
    out << endl;

    out << NFM << QString( "    Offset         0         0" ); //Offsets Line(per socond)
    darks = SFluo->getDarkCountsInROI();
    for ( int j = 0; j < MaxSSDs; j++ ) {            // 19ch SSD -- in ROI
      out << QString( "%1" ).arg( darks[j], 10, 'f', 3 );
    }
    out << QString( "%1" ).arg( mUnits.at(0)->getDark(), 10, 'f', 3 ); // I0
    darks = SFluo->getDarkICRs();
    for ( int j = 0; j < MaxSSDs; j++ ) {             // 19ch SSD -- ICR
      out << QString( "%1" ).arg( darks[j], 10, 'f', 3 );
    }
    out << QString( "%1" ).arg( 0., 10, 'f', 3 );      // �ꥻ�åȲ�� : 0 �ˤ��Ƥ� !!
    out << endl;

    break;

    // EXTRA �����ݤ�������
    // (�ä� MeasDispMode ����������Ǥ�Ȥ��Τ� 19 ch ʬ��ĥ����櫓�ˤ����ʤ�)
    // Use19chSSD->isChecked() == true �ʤ顢19ch SSD ��Ȥ���
    // ���λ� UseI1->isChecked() == true ���ɤ�����
    // 19ch ʬ�Υǡ�����Ÿ���������֤��Ѥ�롣
    // ����Ū�˹ͤ��Ƥߤ��
    // 19ch SSD : No.   ��ñ
    //    MeasChNo ʬ i �롼�פ�󤷤ơ�
    //    ��ľ�� MeasDispMode[i] �Ȥ� Offsets[i] �Ȥ���񤱤Ф���
    // 19ch SSD : Yes.
    //   I1 : No.
    //     ���١����١����֡�I0��19ch ʬŸ����19chʬ ICR���ꥻ�åȲ��������¾�Ĥ�
    //   I1 : Yes.
    //     ���١����١����֡�I0��I1��19ch ʬŸ����19chʬ ICR���ꥻ�åȲ��������¾�Ĥ�
    // �ˤʤ롣

  case EXTRA:
    out << NFM << "  Angle(c)  Angle(o)    time/s";
    cnt = 1;
    for ( int i = 0; i < mUnits.count(); i++ ) {
      if ( mUnits.at(i) != SFluo ) {
        out << QString( "%1" ).arg( cnt, 10 );
      } else {
        for ( int j = 0; j < MaxSSDs; j++ ) {
          out << QString( "%1" ).arg( cnt, 10 );  // 19ch SSD
          cnt++;
        }
        out << QString( "%1" ).arg( 0, 10 );    // resets
        cnt++;
        for ( int j = 0; j < MaxSSDs; j++ ) {
          out << QString( "%1" ).arg( cnt - MaxSSDs - 2, 10 );
          // ICR  19ch SSD ���ֹ�Ȥ�����
          cnt++;
        }
        cnt--;
      }
      cnt++;
    }
    out << endl;

    out << NFM << QString( "      Mode         0         0" );    // Modes Line
    for ( int i = 0; i < mUnits.count(); i++ ) {
      if ( mUnits.at(i) != SFluo ) {
        out << QString( "%1" ).arg( MeasDispMode[i], 10 );
      } else {
        for ( int j = 0; j < MaxSSDs; j++ ) {
          out << QString( "%1" ).arg( FLUO, 10 );  // 19ch SSD
        }
        out << QString( "%1" ).arg( 5, 10 );    // resets
        for ( int j = 0; j < MaxSSDs; j++ ) {
          out << QString( "%1" ).arg( 5, 10 );  // ICR
        }
      }
    }
    out << endl;

    out << NFM << QString( "    Offset         0         0" ); //Offsets Line(per socond)
    for ( int i = 0; i < mUnits.count(); i++ ) {
      if ( mUnits.at(i) != SFluo ) {
        out << QString( "%1" ).arg( mUnits.at(i)->getDark(), 10, 'f', 3 );
      } else {
        QVector<double> darks;
        darks = SFluo->getDarkCountsInROI();
        for ( int j = 0; j < MaxSSDs; j++ ) {            // 19ch SSD -- in ROI
          out << QString( "%1" ).arg( darks[j], 10, 'f', 3 );
        }
        out << QString( "%1" ).arg( 0., 10, 'f', 3 );      // �ꥻ�åȲ�� : 0 �ˤ��Ƥ� !!
        darks = SFluo->getDarkICRs();
        for ( int j = 0; j < MaxSSDs; j++ ) {             // 19ch SSD -- ICR
          out << QString( "%1" ).arg( darks[j], 10, 'f', 3 );
        }
      }
    }
    out << endl;

    break;
  default:
    qDebug() << "Unknown Measuremet type";
  }

  if ( newf ) {
    out << "## Channel Names:";
    for ( int i = 0; i < mUnits.count(); i++ )
      out << QString( " \"%1\"" ).arg( mUnits.at(i)->getName() );
    out << endl;
    
    out << "## Select Button Names:";
    for ( int i = 0; i < GSBs.count(); i++ )
      out << QString( " \"%1\"" ).arg( GSBs[i]->text() );
    out << endl;
    
    out << "## Select Button State:";
    for ( int i = 0; i < GSBs.count(); i++ )
      out << QString( " \"%1\"" ).arg( GSBs[i]->isChecked() );
    out << endl;

    if ( isSFluo ) {
      out << "## Sum up channel : " << SFluoLine << " :";
      for ( int i = 0; i < 19; i++ ) {
        out << " " << SFluoLine + i + 1;
      }
      out << endl;
    }
  }

  file.close();
}

void MainWindow::WriteHeader2( int Rpt )
// �������λ������λ���Ǥʤ��ȷ�ޤ�ʤ������إå��˽񤭲ä��롣
{
  SetDFName( Rpt );   // Generate a file name with repitation number

  QFile file( DFName );
  if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    return;

  // Writing fixed headers
  QTextStream in(&file);

  QStringList AllLines;
  QString line;
  int pos;

  while ( !in.atEnd() ) {
    line = in.readLine();
    if ( ( pos = line.indexOf( "%001%" ) ) > 0 ) {
      line = line.left( pos )
	+ QDateTime::currentDateTime().toString("yy.MM.dd hh:mm")
	+ line.mid( pos + 5 );
    }
    if ( ( pos = line.indexOf( "%002%" ) ) > 0 ) {
      line = line.left( pos )
	+ QString( "%1" ).arg( SLS->value().toDouble(), 6, 'f', 1, ' ' ) 
	+ line.mid( pos + 5 );
    }
    AllLines << line;
  }

  file.close();

  QFile file2( DFName );
  if ( !file2.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;

  // Writing fixed headers
  QTextStream out(&file2);
  for ( int i = 0; i < AllLines.count(); i++ ) {
    out << AllLines[i] << "\n";
  }

  file2.close();
}


// ���󥳡����γ��٤ǵ�Ͽ���뤫���򤷤����٤ǵ�Ͽ���뤫��
//     conds->isEncAsTh()
// �Ƿ�ޤ롣
// ���򤷤��Τ��ɤ��餫��
//     EncOrPM
// �Ǥ狼�롣
// ���ˤ��衢�������ͤ�Ǹ�˵�Ͽ���뤫�ɤ�����
//     conds->isAddInfos()
// �Ƿ�ޤ롣

void MainWindow::RecordData( void )
// MeasVals �� dark �����Ѥ� count
// dark �����Ѥ� cps ���ߤ������ MeasCPSs
// (dark �����������äƤ���Τ� Measurement �� readValue ����Ȥ�
//  dark �����Υ��ץ������դ��Ƥ��뤫��)
{
  SetDFName( MeasR );
  QFile file( DFName );
  double recTh;
  double encTh, PMTh;
  if ( file.open( QIODevice::Append | QIODevice::Text ) ) {
    QTextStream out( &file );
    QString buf;
    // �Ԥ���Ƭ�� ��ɸ���١����󥳡����ɤ߳��١�¬����֡�I0
    // Should be changed depending on the detector (ammeter or counter)

    encTh = SelectedCurPosDeg( XENC );  // ���󥳡�����
    PMTh = SelectedCurPosDeg( XPM );    // PM �Τ��줾��Ǹ�������
    // ���󥳡����γ��٤ǵ�Ͽ���뤫�����򤷤����٤ǵ�Ͽ���뤫����
    if ( conds->isEncAsTh() ) {
      recTh = encTh;
    } else {
      recTh = SelectedCurPosDeg( EncOrPM );
    }
    buf.sprintf( "%10.5f" " %9.5f" " %9.2f",
                 u->keV2deg( GoToKeV ), recTh, NowDwell );
    out << buf;

    if ( MeasFileType != FLUO ) {
      // I0 ���ͤ��������ɤ����ǡ���Ͽ���Υե����ޥåȤ��Ѥ��褦�Ȥ��Ƥ���
      if ( (int)(MeasVals[MC_I0]) == MeasVals[MC_I0] ) {
        buf.sprintf( " %9d",                 // ���� : %9d
                     (int)MeasVals[ MC_I0 ] );
      } else {
        buf.sprintf( " %9.6g",               // �¿� : %9.6g
                     MeasVals[ MC_I0 ] );
      }
      out << buf;
    }

    // ���θ��¬��ǡ������¤�
    for ( int i = 1; i < mUnits.count(); i++ ) {
      if ( mUnits.at(i) != SFluo ) {
        // Quick hack for pico-ammeter
        double v = mUnits.at(i)->value().toDouble();
        if ( (int)(v) == v ) {
          buf.sprintf(" %9d", (int)v );
        } else {
          buf.sprintf(" %9.6g", v );
        }
        out << buf;
      } else {
        QVector<int> vals = SFluo->getCountsInROI();           // vals �� count
        QVector<double> darks = SFluo->getDarkCountsInROI();   // darks �� cps 
        for ( int j = 0; j < MaxSSDs; j++ ) {   // 19ch SSD  in ROI
          buf.sprintf(" %9d",
		      (int)( vals[j] - ( darks[j] * SFluo->GetSetTime() ) ) );  // Orig.
	  // ���ꥸ�ʥ�(count)���ᤷ�����ե�����˽񤯤Τϥ�����Ȥ��ä��Ϥ�
	  // cps �ˤ�����Ǥ� by H.A. �ϴְ�äƤ���
	  // vals[j] �� count, darks[j] �� cps �ʤΤǡ�H.A ���ˤ��ʤ� NowDwell �ǳ��
	  // ����������ȡ�darks �˴ؤ��Ƥϡ����ٻ��֤�ݤ��Ƥޤ���äƤ뤳�Ȥˤʤ�Τ�
	  // vals ����֤ǳ�ä��ۤ���������
	  // ����ˤ� NowDwell �ϡ������ꤷ�褦�Ȥ������֡פʤΤ�
	  // �����ꤵ�줿���֡� GetSetTime() ��Ȥä��ۤ����ɤ�
//      (int)( ( vals[j] - ( darks[j] * SFluo->GetSetTime() ) ) * NowDwell ) ); // by H.A.
//      (int)( vals[j] / SFluo->GetSetTime() - darks[j] );  // by. M.T. (cps version)
          out << buf;
        }
        if ( MeasFileType == FLUO ) {
          // I0 ���ͤ��������ɤ����ǡ���Ͽ���Υե����ޥåȤ��Ѥ��褦�Ȥ��Ƥ���
          if ( (int)(MeasVals[MC_I0]) == MeasVals[MC_I0] ) {
            buf.sprintf( " %9d",                 // ���� : %9d
                         (int)MeasVals[ MC_I0 ] );
          } else {
            buf.sprintf( " %9.6g",               // �¿� : %9.6g
                         MeasVals[ MC_I0 ] );
          }
          out << buf;
        }
        QVector<double> icrs = SFluo->getICRs();
        for ( int j = 0; j < MaxSSDs; j++ ) {   // 19ch SSD  ICR ( per second )
          buf.sprintf(" %9d", (int)( icrs[j] * NowDwell ) );      // by H.A.
          out << buf;
        }
        buf.sprintf(" %9d", 0 );           // �ꥻ�åȲ�� : 0 �ˤ��Ƥ�
        out << buf;
      }
    }
    // �����˾����ɲá�
    // ���ꥨ�ͥ륮�������󥳡����ɤߤγ��١����󥳡������ɤߤΥ��ͥ륮��������
    //                 PM�׻��ѡ�PM�׻��ѤΥ��ͥ륮������
    //
    // �롼�����Ƭ���ɤ�Ǥ��ޤä��ͤ�ȤäƤ�Τϡ����Υ롼����¹����
    // ΢�� AUnit ������ value ���Ѥ�äƤ��ޤ��Τ�ٲ����ơ�(�ʤ��Ϥ�������)
    //
    if (( conds->isAddInfos() )||( conds->isUse1303Format() )) {
      buf.sprintf( " %9.5f" " %9.5f" " %9.5f" " %9.5f" " %9.5f",
		   GoToKeV,
		   encTh, u->deg2keV( encTh ),
		   PMTh, u->deg2keV( PMTh ) );
      if ( conds->isUse1303Format() )
        out << " ###";
      out << buf;
    }
    out << endl;
    file.close();
  }
}

