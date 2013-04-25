#!/usr/local/bin/perl

open( OUT, ">gpscript.gp" );

$fnames = shift;

print OUT "plot ";

while( $fnames ne "" ) {
    open( IN, "ls $fnames|" );

    while(<IN>) {
	chomp;
	$fname = $_;
	print OUT "\"" . "$fname" . "\"" . " every ::17 using (12.38/(2.*3.13553*sin(\$2/180*3.1415926535))):(log(\$4/\$5)) with line title '$fname', "
    }
    
    close( IN );

    $fnames = shift;
}

print OUT "\n";

close( OUT );

system( "gnuplot gpscript.gp -p" );

