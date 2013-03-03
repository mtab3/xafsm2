#!/usr/local/bin/perl

# MainWindow.h に依存しているファイルを探し、どの宣言がどのファイルと関連しているかを
# 調べるためのツール。
# どうつかうかは CheckDefs-1.pl に記述

@Files = ( "AUnit", "Atoms", "ChCoord", "Fio", "Goto", "MCAView", "MainWindow", "Measurement", "Monitor", "MsgType", "MultiUnits", "MwCommon", "MwLog", "MwMeas", "MwSSDSetUp", "MwSetup", "MwView", "PeriodicT", "ReadDef", "SMsg", "Scan", "SelMC", "Stars", "StarsSV", "ViewCtrl", "XView", "XafsM" );

$lc1 = 0;
open( IN, "MainWindow.h" );
while( <IN> ) {
    chomp;
    $MW[$lc1] = $_;
    $lc1++;
}
close( IN );


foreach( @Files ) {
    $file = $_;
    if ( -s "$file.key" ) {
	open( IN, "$file.key" );
	while ( <IN> ) {
	    chomp;
	    $word = $_;
	    for ( $i = 0; $i < $lc1; $i++ ) {
		if ( $MW[$i] =~ /$word/ ) {
		    if ( $F[$i] == 0 ) {
			$MW[$i] .= "\t\t\t//";
			$F[$i] = 1;
		    }
		    $MW[$i] .= "$file, ";
		}
	    }
	}
	close( IN );
    }
}

for ( $i = 0; $i < $lc1; $i++ ) {
    print $MW[$i], "\n";
}
