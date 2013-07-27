#!/usr/local/bin/perl

open( OUT, ">gitlog.tmp" );

print OUT<<EOF;
#ifndef GITLOG_H
#define GITLOG_H

EOF

open( IN, "git branch|" );
while( <IN> ) {
    if ( /^\*\s+(\w+)/ ) {
	$Branch = $1;
    }
}

print OUT<<EOF;

#define BRANCH_NAME ( "$Branch" )
#define GITLOGS ""\\
EOF

open( IN, "git log|" );

$cnt = 0;
$in = 0;
while ( <IN> ) {
    chomp;
    last if ( $cnt > 5 );
    if ( /^commit\s+/ ) {
	if ( $in == 1 ) {
print OUT<<EOF;
                "Author      : $Author\\n"\\
                "Date        : $Date\\n"\\
                $Comment
		"\\n"\\
EOF
	}
	$cnt++;
	$in = 0;
	$Comment = "";
    } elsif ( /^Author:\s+(.*)/ ) {
	$Author = $1;
    } elsif ( /^Date:\s+(.*)/ ) {
	$in = 1;
	$Date = $1;
    } else {
	if ( ( $in == 1 ) && ( ! /^s*$/ ) ) {
	    $Comment .= "\"$_\\n\"\\";
	}
    }
}

print OUT<<EOF;
                ""
#endif
EOF

if ( -e "gitlog.h" ) {
    if ( `diff gitlog.h gitlog.tmp` ) {
	system( "cp gitlog.tmp gitlog.h" );
    }
} else {
    system( "cp gitlog.tmp gitlog.h" );
}
system( "rm gitlog.tmp" );
