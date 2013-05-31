#!/usr/local/bin/perl

# Make map data from many XFA files

#Fe
#$E0 = 6.0;
#$E1 = 6.7;
#Cu
#$E0 = 7.8;
#$E1 = 8.5;
#??
$E0 = 6.7;
$E1 = 7.3;


$maxX = 10;
$maxY = 10;
$Xstep = 0.5;
$Ystep = 0.5;
$x = 0;
$y = 0;

while( $fname = shift ) {
    $count = 0;
    open( IN, $fname );
    while ( <IN> ) {
	$aLine = $_;
	if ( !( $aLine =~ /^#/ ) ) {
	    @Vals = split( /\s+/, $aLine );
	    for ( $i = 0; $i < 19; $i++ ) {
		$E = $Vals[$i*2+1];
		if (( $E >= $E0 )&&( $E <= $E1 )) {
		    $count += $Vals[$i*2+2];
		}
	    }
	}
    }
    print( $x * 1.41421356, " ", $y, " ", $count, "\n" );

    $x += $Xstep;
    if ( $x > $maxX ) {
	print( "\n" );
	$x = 0;
	$y -= $Ystep;
    }

    close( IN );
}
