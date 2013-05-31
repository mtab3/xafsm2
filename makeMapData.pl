#!/usr/local/bin/perl

# Make map data from many XAFS-mode-measured sigle point data files

$maxX = 10;
$maxY = 10;
$Xstep = 0.5;
$Ystep = 0.5;
$x = 0;
$y = 0;
while( $fname = shift ) {
    open( IN, $fname );
    for ( $i = 0; $i < 12; $i++ ) {
	$dummy = <IN>;
    }
    $modeLine = <IN>;
    @Modes = split( /\s+/, $modeLine );
    $dummy = <IN>;
    $valueLine = <IN>;
    @Values = split( /\s+/, $valueLine );
    $sum = 0;
    $mode3cnt = 0;
    for ( $i = 0; $i < 30; $i++ ) {
	if ( $Modes[$i] == 3 ) {
	    if (( $mode3cnt != 3 )&&( $mode3cnt != 11 )) {   # disabled Ch of SSD
		$sum += $Values[$i];
	    }
	    $mode3cnt++;
	}
	if ( $Modes[$i] == 1 ) {
	    $I0 = $Values[$i];
	}
    }
    print( $x * 1.41421356, " ", $y, " ", $sum/$I0, "\n" );
    $x += $Xstep;
    if ( $x > $maxX ) {
	print( "\n" );
	$x = 0;
	$y -= $Ystep;
    }

    close( IN );
}
