
#include <math.h>

#include "MainWindow.h"

#define BLNAME    ( "BL5S1" )

bool MainWindow::SetDFName0( QString fname )
{
  DFName0 = fname;          // EditDFName->text();
  
  if ( DFName0.isEmpty() ) {
    return false;
  }
  
  QFileInfo f = QFileInfo( DFName0 ); // path と basename 抽出に使う
  QFileInfo f2 = QFileInfo( f.absoluteDir().absolutePath(), f.baseName() );
  // f2 : path と basename の結合を Qt に任せる
  //      (顕に path + "/" + basename と書きたくない. unix "/", win "\" 問題回避 )
  DFName0 = f2.filePath();

  return true;
}

void MainWindow::SetDFName( int i, int rptMax, QString ext )
{
  QString buf;

  if ( SvSelExtPattern || ( rptMax == 1 ) ) {
    if ( i == 0 ) {
      DFName = DFName0 + DFName00 + ext + ".dat";
    } else {
      buf.sprintf( ".%03d", i );
      DFName = DFName0 + DFName00 + ext + buf;
    }
  } else {
    buf.sprintf( "-%03d", i );
    DFName = DFName0 + DFName00 + buf + ext + ".dat";
  }
}

QString MainWindow::fixS( QString s, int l )
{
  QString r;

  r.fill( ' ', l - s.size() );
  s += r;

  return s.left( l );
}

void MainWindow::WriteHeader( int Rpt )
{
  // Generate a file name with repitation number
  SetDFName( Rpt, SelRPT->text().toInt() );
  WriteHeaderCore( true );
}

void MainWindow::WriteHeaderCore( bool SnotN )
{
  int cnt, sfluop;
  QFile file( DFName );
  if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;
  
  // Writing fixed headers
  QTextStream out(&file);

  out << " " << fixS( FileIDs[ MEASDATA ], 20 ) << fixS( BLNAME, 5 ) << endl;

  out << " " << fixS( DFName, 14 ) << "  "
      << QDateTime::currentDateTime().toString("yy.MM.dd hh:mm") << " - "
      << "%001%" << endl; // 置換用のマーク 001 番目 14桁終了日時 "yy.MM.dd hh:mm"
  out << " " << fixS( EditCMT->text(), 80 ) << endl;

  out << " " << "Ring : " << "  1.2 GeV"
      << QString( "  %1 mA - " ).arg( ( SLS == NULL ) ? 0 : SLS->value().toDouble(), 6, 'f', 1, ' ' )
      << "%002%"          // 置換用のマーク 002 番目 実数6.1リング電流
      << " mA" << endl;

  out << " " << "Mono :   " << fixS( mccd[ selmc->MC() ]->getMCName(), 10 )
      << QString( "    D= %1 A    " ).arg( mccd[ selmc->MC() ]->getD(), 8, 'f', 5, ' ' )
      << QString( "Initial angle=%1 deg" ).arg(u->keV2deg( InitialKeV ), 9, 'f', 5, ' ')
      << endl;

  out << " " << fixS( BLNAME, 5 ) << "    "
      << fixS( CMode[ MeasFileType2 ], 13 ) << QString( "(%1)" ).arg( MeasFileType2, 2 )
      << QString( "   Repetition=%1" ).arg( MeasR , 3 )
      << QString( "     Points=%1" ).arg( TP, 5 ) << endl;

  UNIT Unit = ( SnotN ) ? SBLKUnit : NBLKUnit;
  int Blocks = ( SnotN ) ? SBlocks : NBlocks;
  if ( Unit == DEG ) {
    out << " "
        << QString( "Param file : %1  angle axis (1)    Block = %2" )
           .arg( fixS( "DUMMYNAME.prm", 14 ) ).arg( Blocks, 5 ) << endl;
    out << endl;

    out << " "
        << "Block      Init-ang  final-ang     Step/deg     Time/s       Num" << endl;

    for ( int i = 0; i < Blocks; i++ ) {
      if ( SnotN ) {
        out << " "
            << QString( "%1     %2%3%4%5%6" )
               .arg( i+1, 5 )
               .arg( SBlockStartAsDisp[i], 10, 'f', 5 )
               .arg( SBlockStartAsDisp[i+1], 10, 'f', 5 )
            .arg( SBlockStepAsDisp[i], 12, 'f', 6 )
            .arg( SBlockDwell[i], 11, 'f', 2 )
            .arg( SBlockPoints[i], 10 )
            << endl;
      } else {
        out << " "
            << QString( "%1     %2%3%4%5%6" )
               .arg( i+1, 5 )
               .arg( NBlockStartAsDisp[i], 10, 'f', 5 )
               .arg( NBlockStartAsDisp[i+1], 10, 'f', 5 )
            .arg( NBlockStepAsDisp[i], 12, 'f', 6 )
            .arg( NBlockDwell[i], 11, 'f', 2 )
            .arg( NBlockPoints[i], 10 )
            << endl;
      }
    }
  } else {
    out << " "
        << QString( "Param file : %1  energy axis (2)    Block = %2" )
           .arg( fixS( "DUMMYNAME.prm", 14 ) ).arg( Blocks, 5 ) << endl;
    out << endl;

    out << " "
        << "Block      Init-Eng  final-Eng     Step/eV     Time/s       Num" << endl;
    for ( int i = 0; i < Blocks; i++ ) {
      if ( SnotN ) {
        double s = u->any2keV( SBLKUnit, SBlockStartAsDisp[i] )*1000;
        double e = u->any2keV( SBLKUnit, SBlockStartAsDisp[i+1] )*1000;
        out << " "
            << QString( "%1     %2%3%4%5%6" )
               .arg( i+1, 5 )
               .arg( s, 10, 'f', 2 )
               .arg( e, 10, 'f', 2 )
               .arg( ( e - s )/SBlockPoints[i], 12, 'f', 2 )
               .arg( SBlockDwell[i], 11, 'f', 2 )
               .arg( SBlockPoints[i], 10 )
            << endl;
      } else {
        double s = u->any2keV( NBLKUnit, NBlockStartAsDisp[i] )*1000;
        double e = u->any2keV( NBLKUnit, NBlockStartAsDisp[i+1] )*1000;
        out << " "
            << QString( "%1     %2%3%4%5%6" )
               .arg( i+1, 5 )
               .arg( s, 10, 'f', 2 )
               .arg( e, 10, 'f', 2 )
               .arg( ( e - s )/NBlockPoints[i], 12, 'f', 2 )
               .arg( NBlockDwell[i], 11, 'f', 2 )
               .arg( NBlockPoints[i], 10 )
            << endl;
      }
    }
  }

  int Munits = mMeasUnits.count();
  if ( MPSet.qXafsMode && ( Enc2 != NULL ) ) {
    Munits -= 1;
  }

  QVector<double> darks;
  switch( MeasFileType2 ) {
  case TRANS:
    out << QString( " ORTEC( 0)     NDCH =%1" ).arg( 1 + MeasChNo, 2 )
        << endl;
    out << "  Angle(c)  Angle(o)    time/s";
    for ( int i = 0; i < MeasChNo; i++ ) {
      out << QString( "%1" ).arg( i+1, 10 );
    }
    out << endl;

    out << "      Mode         0         0";
    for ( int i = 0; i < MeasChNo; i++ ) {
      out << QString( "%1" ).arg( MeasDispMode[i], 10 );
    }
    out << endl;

    out << "    Offset         0         0";
    for ( int i = 0; i < MeasChNo; i++ ) {
      out << QString( "%1" ).arg( mMeasUnits.at(i)->GetDark( 1.0 ), 10, 'f', 3 );
      // For QXAFS with DMM in microV?
    }
    out << endl;
    break;

  case FLUO:    // FLUO と EXTRA は一度は同じ(一つ)になったのに、
    // SSD only
    if ( MPSet.isUsingSFluo() ) {
      //      darks = SFluo->getDarkCountsInROI();
      //      WriteFLUOHeadSection( out, darks, mMeasUnits.at(0)->getDark() );
      WriteFLUOHeadSection( out, mMeasUnits.at(0)->GetDark( 1.0 ) );
      break;
    } else {
      // 基本的には Lytle 検出器を想定している
      out << QString( " ORTEC( 3)     NDCH =%1" ).arg( 1 + MeasChNo, 2 )
          << endl;
      out << "  Angle(c)  Angle(o)    time/s";
      for ( int i = 0; i < MeasChNo; i++ ) {
        out << QString( "%1" ).arg( i+1, 10 );
      }
      out << endl;

      out << "      Mode         0         0";
      for ( int i = 0; i < MeasChNo; i++ ) {
        out << QString( "%1" ).arg( MeasDispMode[i], 10 );
      }
      out << endl;

      out << "    Offset         0         0";
      for ( int i = 0; i < MeasChNo; i++ ) {
        out << QString( "%1" ).arg( mMeasUnits.at(i)->GetDark( 1.0 ), 10, 'f', 3 );
      }
      out << endl;
      break;
    }

    // EXTRA は面倒くさい。
    // (特に MeasDispMode は描画の方でも使うので 19 ch 分拡張するわけにいかない)
    // Use19chSSD->isChecked() == true なら、19ch SSD を使う。
    // その時 UseI1->isChecked() == true かどうかで
    // 19ch 分のデータが展開される位置が変わる。
    // 具体的に考えてみると
    // 19ch SSD : No.   簡単
    //    MeasChNo 分 i ループを回して、
    //    素直に MeasDispMode[i] とか Offsets[i] とかを書けばいい
    // 19ch SSD : Yes.
    //   I1 : No.
    //     角度、角度、時間、I0、19ch 分展開、19ch分 ICR、リセット回数、その他残り
    //   I1 : Yes.
    //     角度、角度、時間、I0、I1、19ch 分展開、19ch分 ICR、リセット回数、その他残り
    // になる。

    // REX2000 のデータ変換で正しく変換できることを優先して、以下のように変更する
    // PF や SPring-8 で「実際に」出力されている形式にできるだけ準拠している
    // 具体的に考えてみると
    // 19ch SSD : No.   簡単
    //    MeasChNo 分 i ループを回して、
    //    素直に MeasDispMode[i] とか Offsets[i] とかを書けばいい
    // 19ch SSD : Yes.
    //   I1 : No.
    //     角度、角度、時間、19ch 分展開、I0、19ch分 ICR、リセット回数、その他残り
    //   I1 : Yes.
    //     角度、角度、時間、19ch 分展開、I0、19ch分 ICR、リセット回数、I1、その他残り
    // になる。

  case EXTRA:
    {
      int CHs = UsingSFluoCHs();
      if ( MPSet.isUsingSFluo() && MPSet.isI1 ) {
	out << QString( " CAMAC( 1)     NDCH =%1" ).arg( MeasChNo, 2 ) << endl;
	out << "  Angle(c)  Angle(o)    time/s";
	cnt = 1;
	for ( int i = 0; i < MPSet.isSFluos.count(); i++ ) {
	  if ( MPSet.isSFluos[i] ) {
	    for ( int j = 0; j < SFluos[i]->chs(); j++ ) {
	      out << QString( "%1" ).arg( cnt, 10 );  cnt++;
	    }
	  }
	}
	// I0
	out << QString( "%1" ).arg( cnt, 10 );  cnt++;
	// SFluo ICR
	int cnt2 = 1;
	for ( int i = 0; i < MPSet.isSFluos.count(); i++ ) {
	  if ( MPSet.isSFluos[i] ) {
	    for ( int j = 0; j < SFluos[i]->chs(); j++ ) {
	      out << QString( "%1" ).arg( cnt2, 10 );    cnt++; cnt2++;
	    }
	    // Reset
	    out << QString( "%1" ).arg( cnt2, 10 ); 	cnt++; cnt2++;
	  }
	}
	// Others
	for ( int i = 0; i < Munits; i++ ) {
	  if ( ( ! isASFluoUnit( mMeasUnits.at(i) ) ) && ( mMeasUnits.at(i) != SI0 ) ) {
	    out << QString( "%1" ).arg( cnt2, 10 );  cnt++; cnt2++;
	  }
	}
	out << endl;
	
	// Modes Line
	out << "      Mode         0         0";
	// 19ch ROI
	for ( int i = 0; i < CHs; i++ ) {
	  out << QString( "%1" ).arg( FLUO, 10 );
	}
	// I0
	out << QString( "%1" ).arg( I0, 10 );
	// 19ch ICR
	for ( int i = 0; i < MPSet.isSFluos.count(); i++ ) {
	  if ( MPSet.isSFluos[i] ) {
	    for ( int j = 0; j < SFluos[i]->chs(); j++ ) {
	      out << QString( "%1" ).arg( 103, 10 );
	    }
	    // Reset
	    out << QString( "%1" ).arg( 101, 10 );
	  }
	}
	// Others
	for ( int i = 0; i < Munits; i++ ) {
	  if ( ( ! isASFluoUnit( mMeasUnits.at(i) ) ) && ( mMeasUnits.at(i) != SI0 ) ) {
	    out << QString( "%1" ).arg( MeasDispMode[i], 10 );
	  }
	}
	out << endl;
	
	// Offsets Line (per second)
	out << "    Offset         0         0";
	// SFluo ROI
	//	QVector<double> darks;
	//	darks = SFluo->getDarkCountsInROI();
	for ( int i = 0; i < MPSet.isSFluos.count(); i++ ) {
	  if ( MPSet.isSFluos[i] ) {
	    for ( int j = 0; j < SFluos[i]->chs(); j++ ) {
	      out << QString( "%1" ).arg( SFluos[i]->getDarkCountsInROI()[j],
					  10, 'f', 3 );
	    }
	  }
	}
	// I0
	out << QString( "%1" ).arg( SI0->GetDark( 1.0 ), 10, 'f', 3 );
	// 19ch ICR
	//	darks = SFluo->getDarkICRs();
	for ( int i = 0; i < MPSet.isSFluos.count(); i++ ) {
	  if ( MPSet.isSFluos[i] ) {
	    for ( int j = 0; j < SFluos[i]->chs(); j++ ) {
	      out << QString( "%1" ).arg( SFluos[i]->getDarkICRs()[j], 10, 'f', 3 );
	    }
	    // Reset
	    out << QString( "%1" ).arg( 0., 10, 'f', 3 );
	  }
	}
	// Others
	for ( int i = 0; i < Munits; i++ ) {
	  if ( ( ! isASFluoUnit( mMeasUnits.at(i) ) ) && ( mMeasUnits.at(i) != SI0 ) ) {
	    out << QString( "%1" ).arg( mMeasUnits.at(i)->GetDark( 1.0 ), 10, 'f', 3 );
	  }
	}
	out << endl;
	
      } else {
	out << QString( " SCALE( 2)     NDCH =%1" ).arg( MeasChNo, 2 ) << endl;
	out << "  Angle(c)  Angle(o)    time/s";
	cnt = 1;
	sfluop = 0;
	while( ( ! MPSet.isSFluos[sfluop] )&&( sfluop < MPSet.isSFluos.count() ) )
	  sfluop++;
	for ( int i = 0; i < Munits; i++ ) {
	  if ( ! isASFluoUnit( mMeasUnits.at(i) ) ) {
	    out << QString( "%1" ).arg( cnt, 10 );
	  } else {
	    for ( int j = 0; j < SFluos[sfluop]->chs(); j++ ) {
	      out << QString( "%1" ).arg( cnt, 10 );  // 19ch SSD
	      cnt++;
	    }
	    out << QString( "%1" ).arg( 0, 10 );    // resets
	    cnt++;
	    for ( int j = 0; j < CHs; j++ ) {
	      out << QString( "%1" ).arg( cnt - CHs - 2, 10 );
	      // ICR  19ch SSD の番号とそろえる
	      cnt++;
	    }
	    while( ( ! MPSet.isSFluos[sfluop] )&&( sfluop < MPSet.isSFluos.count() ) )
	      sfluop++;
	    cnt--;
	  }
	  cnt++;
	}
	out << endl;
	
	// Modes Line
	sfluop = 0;
	while( ( ! MPSet.isSFluos[sfluop] )&&( sfluop < MPSet.isSFluos.count() ) )
	  sfluop++;
	out << QString( "      Mode         0         0" );
	for ( int i = 0; i < Munits; i++ ) {
	  if ( ! isASFluoUnit( mMeasUnits.at(i) ) ) {
	    out << QString( "%1" ).arg( MeasDispMode[i], 10 );
	  } else {
	    for ( int j = 0; j < SFluos[sfluop]->chs(); j++ ) {
	      out << QString( "%1" ).arg( FLUO, 10 );  // 19ch SSD
	    }
	    out << QString( "%1" ).arg( 5, 10 );    // resets
	    for ( int j = 0; j < SFluos[sfluop]->chs(); j++ ) {
	      out << QString( "%1" ).arg( 5, 10 );  // ICR
	    }
	    sfluop = 0;
	    while( ( ! MPSet.isSFluos[sfluop] )&&( sfluop < MPSet.isSFluos.count() ) )
	      sfluop++;
	  }
	}
	out << endl;
	// Offsets Line (per second)
	sfluop = 0;
	while( ( ! MPSet.isSFluos[sfluop] )&&( sfluop < MPSet.isSFluos.count() ) )
	  sfluop++;
	out << QString( "    Offset         0         0" );
	for ( int i = 0; i < Munits; i++ ) {
	  if ( ! isASFluoUnit( mMeasUnits.at(i) ) ) {
	    out << QString( "%1" ).arg( mMeasUnits.at(i)->GetDark( 1.0 ), 10, 'f', 3 );
	  } else {
	    QVector<double> darks;
	    darks = SFluos[sfluop]->getDarkCountsInROI();
	    for ( int j = 0; j < SFluos[sfluop]->chs(); j++ ) {  // 19ch SSD -- in ROI
	      out << QString( "%1" ).arg( darks[j], 10, 'f', 3 );
	    }
	    out << QString( "%1" ).arg( 0., 10, 'f', 3 ); // リセット回数 : 0 にしてる !!
	    darks = SFluos[sfluop]->getDarkICRs();
	    for ( int j = 0; j < SFluos[sfluop]->chs(); j++ ) {    // 19ch SSD -- ICR
	      out << QString( "%1" ).arg( darks[j], 10, 'f', 3 );
	    }
	    while( ( ! MPSet.isSFluos[sfluop] )&&( sfluop < MPSet.isSFluos.count() ) )
	      sfluop++;
	  }
	}
	out << endl;
      }
    }
    break;
  default:
    qDebug() << "Unknown Measuremet type";
  }
  
  file.close();
}

void MainWindow::WriteFLUOHeadSection( QTextStream &out, double I0dark )
{
  int CHs = UsingSFluoCHs();

  out << " " << QString( "CAMAC( 1)     NDCH =%1" ).arg( CHs + 1, 2 ) << endl;
  // I0 の位置を変えないといけないことが判明。なのでまた分離。
  out << "  Angle(c)  Angle(o)    time/s";
  // FLUO の時 mUnits の要素の並びは必ず I0, 19ch SSD になってるはず
  for ( int i = 0; i < MPSet.isSFluos.count(); i++ ) {
    if ( MPSet.isSFluos[i] ) {
      for ( int j = 0; j < SFluos[i]->chs(); j++ ) {
	out << QString( "%1" ).arg( j+1, 10 );  // SSD のチャンネル分
      }
    }
  }
  out << QString( "%1" ).arg( CHs + 1, 10 );    // I0
  for ( int i = 0; i < MPSet.isSFluos.count(); i++ ) {
    if ( MPSet.isSFluos[i] ) {
      for ( int j = 0; j < SFluos[i]->chs(); j++ ) {
	// ICR  19ch SSD の番号とそろえる
	out << QString( "%1" ).arg( j+1, 10 );
      }
      out << QString( "%1" ).arg( SFluos[i]->chs() + 1, 10 );    // resets
    }
  }
  out << endl;

  out << QString( "      Mode         0         0" );    // Modes Line
  for ( int j = 0; j < CHs; j++ ) {
    out << QString( "%1" ).arg( FLUO, 10 );  // Flurescence
  }
  out << QString( "%1" ).arg( 1, 10 );       // I0 : I0 は決め打ちで 1
  for ( int i = 0; i < MPSet.isSFluos.count(); i++ ) {
    if ( MPSet.isSFluos[i] ) {
      for ( int j = 0; j < SFluos[i]->chs(); j++ ) {
	out << QString( "%1" ).arg( FLUO + 100, 10 );  // ICR
      }
      out << QString( "%1" ).arg( 101, 10 );     // resets
    }
  }
  out << endl;
  
  out << QString( "    Offset         0         0" ); //Offsets Line(per socond)
  for ( int i = 0; i < MPSet.isSFluos.count(); i++ ) {
    if ( MPSet.isSFluos[i] ) {
      for ( int j = 0; j < SFluos[i]->chs(); j++ ) {            // 19ch SSD -- in ROI
	out << QString( "%1" ).arg( SFluos[i]->getDarkCountsInROI()[j],
				    10, 'f', 3 );
      }
    }
  }
  out << QString( "%1" ).arg( I0dark, 10, 'f', 3 ); // I0
  //  darks = SFluo->getDarkICRs();
  for ( int i = 0; i < MPSet.isSFluos.count(); i++ ) {
    if ( MPSet.isSFluos[i] ) {
      for ( int j = 0; j < SFluos[i]->chs(); j++ ) {            // 19ch SSD -- ICR
	out << QString( "%1" ).arg( SFluos[i]->getDarkICRs()[j], 10, 'f', 3 );
      }
      out << QString( "%1" ).arg( 0., 10, 'f', 3 );      // リセット回数 : 0 にしてる !!
    }
  }
  out << endl;
}

void MainWindow::WriteInfoFile( void )
{
  if ( ! conds->isMakeInfo() )
    return;

  QFile f( DFName0 + ".info" );
  if ( !f.open( QIODevice::WriteOnly | QIODevice::Text ) )
    return;
  QTextStream out( &f );

  int Munits = mMeasUnits.count();
  if ( MPSet.qXafsMode && ( Enc2 != NULL ) )
    Munits -= 1;

  out << "Version :" << " 1303" << endl;
  out << "Channel Names:";
  for ( int i = 0; i < Munits; i++ )
    out << QString( " \"%1\"" ).arg( mMeasUnits.at(i)->name() );
  out << endl;
  
  out << "Select Button Names:";
  for ( int i = 0; i < GSBs.count(); i++ )
    out << QString( " \"%1\"" ).arg( GSBs[i]->text() );
  out << endl;
  
  out << "Select Button State:";
  for ( int i = 0; i < GSBs.count(); i++ )
    out << QString( " \"%1\"" ).arg( GSBs[i]->isChecked() );
  out << endl;

#if 0                    // 何を記録したかったのか謎
  if ( MPSet.isUsingSFluo() ) {
    int CHs = SFluoCHs();
    out << "Sum Up Channels: " << SFluoLine << " ";
    for ( int i = 0; i < CHs; i++ ) {
      out << " " << SFluoLine + i + 1;
    }
    out << endl;
  }
#endif
  
  f.close();
}

void MainWindow::WriteInfoFile2( void )
{
  if ( ! conds->isMakeInfo() )
    return;

  QFile f( DFName0 + ".info" );
  if ( !f.open( QIODevice::Append | QIODevice::Text ) )
    return;
  QTextStream out( &f );

  if ( MPSet.isUsingSFluo() ) {
    out << "Finally Used SSD Channels:";
    for ( int i = 0; i < MPSet.isSFluos.count(); i++ ) {
      if ( MPSet.isSFluos[i] ) {
	for ( int j = 0; j < SFluos[i]->chs(); j++ ) {
	  out << " " << j;
	  if ( SSFluos[i]->selBs2()->isSelected(j) ) {
	    out << " 1";
	  } else {
	    out << " 0";
	  }
	}
      }
    }
    out << endl;
  }

  f.close();
}

void MainWindow::WriteHeader2( int Rpt )
// スキャン終了時、終了時でないと決まらない情報をヘッダに書き加える。
{
  // Generate a file name with repitation number
  SetDFName( Rpt, SelRPT->text().toInt() );
  WriteHeaderCore2();
}

void MainWindow::WriteHeaderCore2( void )
{
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
          + QString( "%1" ).arg( ( SLS == NULL ) ? 0 : SLS->value().toDouble(), 6, 'f', 1, ' ' )
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

// エンコーダの角度で記録するか選択した角度で記録するかは
//     conds->isEncAsTh()
// で決まる。
// 選択したのがどちらかは
//     EncOrPM
// でわかる。
// 何にせよ、全部の値を最後に記録するかどうかは
//     conds->isAddInfos()
// で決まる。

void MainWindow::RecordData( void )    // Data Body  // QXafs の時は使わない
// MeasVals は dark 補正済み count
// dark 補正済み cps が欲しければ MeasCPSs
// (dark 補正がかかっているのは Measurement で readValue するとき
//  dark 補正のオプションを付けているから)
{
  if ( MPSet.isUsingSFluo() ) {
    int mcaSaveP = 0;
    if ( MCACanSaveAllOnMem )
      mcaSaveP = MeasR;
    for ( int i = 0; i < MPSet.isSFluos.count(); i++ ) {
      if ( MPSet.isSFluos[i] ) {
	SaveMCADataOnMem( XafsMCAMaps[i].aPoint( MeasP, mcaSaveP ),
			  SSFluos[i] );  // MeasA は無視
	
	if ( conds->isRecordEachMCASpectrum() ) {
	  QDir newDir;
	  newDir.mkpath( DFName0 + "-MCA" );
	  newDir.cd( DFName0 + "-MCA" );
	  QString FnameExt;
	  if ( AutoModeButton->isChecked() )
	    FnameExt.sprintf( "-%03d-%02d-%04d(%1d).dat", MeasA+1, MeasR+1, MeasP, i );
	  else
	    FnameExt.sprintf( "-%02d-%04d(%1d).dat", MeasR+1, MeasP, i );
	  QFileInfo f = QFileInfo( DFName0 );
	  QFileInfo mcaf = QFileInfo( newDir.absolutePath(),
				      f.baseName() + FnameExt );
	  //      saveMCAData0( mcaf.filePath(), XafsMCAMap.aPoint( MeasP, mcaSaveP ) );
	  qDebug() << "XAFS-MCA-Maps" << XafsMCAMaps.count()
		   << MeasA << MeasR << MeasP << i;
	  XafsMCAMaps[i].aPoint( MeasP, mcaSaveP )
	    ->save( mcaf.filePath(), "measured while XAFS measurement", SFluos[i]->uid() );
	}
      }
    }
  }
  
  SetDFName( MeasR, SelRPT->text().toInt() );
  QFile file( DFName );
  double recTh;
  double encTh, PMTh;
  if ( file.open( QIODevice::Append | QIODevice::Text ) ) {
    QTextStream out( &file );
    QString buf;
    // 行の先頭は 目標角度、エンコーダ読み角度、測定時間、I0
    // Should be changed depending on the detector (ammeter or counter)
    encTh = SelectedCurPosDeg( XENC );  // エンコーダと
    PMTh = SelectedCurPosDeg( XPM );    // PM のそれぞれで見た角度
    // エンコーダの角度で記録するか、選択した角度で記録するか決定
    if ( conds->isEncAsTh() ) {
      recTh = encTh;
    } else {
      recTh = SelectedCurPosDeg( EncOrPM );
    }
    buf.sprintf( "%10.6f" " %9.6f" " %9.2f",
                 u->keV2deg( GoToKeV ), recTh, NowDwell );
    out << buf;
    
    // FLUO の時は、I0 は SSD データの後ろ
    // if ( MeasFileType != FLUO ) {
    // SSD をつかっていれば、I0 は ROI　の後ろ
    if ( ! MPSet.isUsingSFluo() ) {
      // I0 の値が整数かどうかで、記録時のフォーマットを変えようとしている
      if ( (int)(MeasVals[MC_I0]) == MeasVals[MC_I0] ) {
        buf.sprintf( " %9d",                 // 整数 : %9d
                     (int)MeasVals[ MC_I0 ] );
      } else {
        buf.sprintf( " %9.6g",               // 実数 : %9.6g
                     MeasVals[ MC_I0 ] );
      }
      out << buf;
    }
    
    // その後に測定データの並び
    if ( MPSet.isUsingSFluo() ) {
#if 0
      int CHs = SFluo->chs();
      // vals は count
      QVector<quint64> vals = SFluo->getCountsInROI();
      // darks は cps
      QVector<double> darks = SFluo->getDarkCountsInROI();
      // 19ch ROI
#endif
      for ( int i = 0; i < MPSet.isSFluos.count(); i++ ) {
	if ( MPSet.isSFluos[i] ) {
	  for ( int j = 0; j < SFluos[i]->chs(); j++ ) {
	    buf.sprintf(" %9d",
			(int)( SFluos[i]->getCountsInROI()[j]
			       - ( SFluos[i]->getDarkCountsInROI()[j]
				   * SFluos[i]->getSetTime() ) ) );
	    out << buf;
	  }
	}
      }
      // I0
      // I0 の値が整数かどうかで、記録時のフォーマットを変えようとしている
      if ( (int)(MeasVals[MC_I0]) == MeasVals[MC_I0] ) {
        // 整数 : %9d
        buf.sprintf( " %9d", (int)MeasVals[ MC_I0 ] );
      } else {
        // 実数 : %9.6g
        buf.sprintf( " %9.6g", MeasVals[ MC_I0 ] );
      }
      out << buf;
      // 19ch ICR
      //      QVector<double> icrs = SFluo->getICRs();
      // 19ch SSD  ICR ( per second )
      for ( int i = 0; i < MPSet.isSFluos.count(); i++ ) {
	if ( MPSet.isSFluos[i] ) {
	  for ( int j = 0; j < SFluos[i]->chs(); j++ ) {
	    buf.sprintf(" %9d",
			(int)( SFluos[i]->getICRs()[j] * SFluos[i]->getSetTime() ) );
	    out << buf;
	  }
	  // Reset
	  buf.sprintf(" %9d", 0 );
	  out << buf;
	}
      }
      // Others
      for ( int i = 1; i < mMeasUnits.count(); i++ ) {
        if ( ( ! isASFluoUnit( mMeasUnits.at(i) ) )&& ( mMeasUnits.at(i) != SI0 ) ) {
          double v = mMeasUnits.at(i)->value().toDouble()
              - mMeasUnits.at(i)->GetDark( mMeasUnits.at(i)->getSetTime() );
          if ( v < 1e-10 )
            v = 0.0;
          if ( (int)(v) == v ) {
            buf.sprintf(" %9d", (int)v );
          } else {
            buf.sprintf(" %9.6g", v );
          }
          out << buf;
        }
      }
    } else {
      for ( int i = 1; i < mMeasUnits.count(); i++ ) {
        if ( ! isASFluoUnit( mMeasUnits.at(i) ) ) {
          double v = mMeasUnits.at(i)->value().toDouble()
              - mMeasUnits.at(i)->GetDark( mMeasUnits.at(i)->getSetTime() );
          if ( v < 1e-10 )
            v = 0.0;
          if ( (int)(v) == v ) {
            buf.sprintf(" %9d", (int)v );
          } else {
            buf.sprintf(" %9.6g", v );
          }
          out << buf;
        } else {
          // SSD の処理
#if 0
	  int CHs = SFluo->chs();
          QVector<quint64> vals = SFluo->getCountsInROI(); // vals は count
          QVector<double> darks = SFluo->getDarkCountsInROI();   // darks は cps
#endif
	  for ( int i = 0; i < MPSet.isSFluos.count(); i++ ) {
	    if ( MPSet.isSFluos[i] ) {
	      for ( int j = 0; j < SFluos[i]->chs(); j++ ) {   // 19ch SSD  in ROI
		buf.sprintf(" %9d",
			    (int)( SFluos[i]->getCountsInROI()[j]
				   - ( SFluos[i]->getDarkCountsInROI()[j]
				       * SFluos[i]->getSetTime() ) ) );
		out << buf;
	      }
	    }
	  }
          if ( MeasFileType == FLUO ) {
            // I0 の値が整数かどうかで、記録時のフォーマットを変えようとしている
            if ( (int)(MeasVals[MC_I0]) == MeasVals[MC_I0] ) {
              buf.sprintf( " %9d",                 // 整数 : %9d
                           (int)MeasVals[ MC_I0 ] );
            } else {
              buf.sprintf( " %9.6g",               // 実数 : %9.6g
                           MeasVals[ MC_I0 ] );
            }
            out << buf;
          }
	  //          QVector<double> icrs = SFluo->getICRs();
	  for ( int i = 0; i < MPSet.isSFluos.count(); i++ ) {
	    if ( MPSet.isSFluos[i] ) {
	      for ( int j = 0; j < SFluos[i]->chs(); j++ ) { // 19ch SSD ICR (per sec)
		buf.sprintf(" %9d", (int)( SFluos[i]->getICRs()[j]
					   * SFluos[i]->getSetTime() ) );
		out << buf;
	      }
	      buf.sprintf(" %9d", 0 );           // リセット回数 : 0 にしてる
	      out << buf;
	    }
	  }
        }
      }
    }
    // 末尾に情報追加。
    // 設定エネルギー、エンコーダ読みの角度、エンコーダの読みのエネルギー換算、
    //                 PM計算角、PM計算角のエネルギー換算
    //
    // ルーチンの頭に読んでしまった値を使ってるのは、このルーチン実行中に
    // 裏で AUnit 内部の value が変わってしまうのを警戒して。(ないはずだけど)
    //
    if ( conds->isAddInfos() ) {
      buf.sprintf( " %9.5f" " %9.5f" " %9.5f" " %9.5f" " %9.5f" " %9.5f",
                   GoToKeV,
                   encTh, u->deg2keV( encTh ),
                   PMTh, u->deg2keV( PMTh ),
                   SettingMainTh );
      out << buf;
    }
    out << endl;
    file.close();
  }
}

void MainWindow::TouchDelegateFile( void )
{
  QFile file( DFName0 + ".dat" );
  if ( file.open( QIODevice::Append ) )
    file.close();
}
