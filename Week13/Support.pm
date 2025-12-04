# Support: general utility functions..

use strict;
use warnings;
use 5.010;
use Data::Dumper;
use Function::Parameters;

my $infh;      # fd of current C+Int file we're translating
my $lineno;    # current line no inside $infh
my $currline;  # current line from file (set by nextline(), used in fatal())

#
# my $ok = openfile( $inputfilename );
#   Open $inputfilename, as the source of lines via nextline().
#   Return 1 if it opens ok, 0 otherwise.
#
fun openfile( $inputfilename )
{
    $lineno = 0;
    open( $infh, '<', $inputfilename ) || return 0;
    return 1;
}

#
# my $line = nextline();
#   Read the next line from $infh, incrementing $lineno afterwards,
#   and return it.
#
fun nextline()
{
    my $line = <$infh>;
    $currline = $line;
    $lineno++;
    return $line;
}

#
# fatal( $whatsleft, $msg );
#   Given $whatsleft (a suffix of $currline) and a message $msg, print
#   a standard-formatted fatal error and die.
#
fun fatal( $whatsleft, $msg )
{
    $currline  =~ s/^\t/        /;
    $whatsleft =~ s/^\t/        /;
    my $err = $currline // '';
    my $pos = length($currline // '') - length($whatsleft // '') - 1;
    $pos = 0 if $pos < 0;
    my $indent = ' ' x $pos;
    $err .= "$indent^ Error at line $lineno: $msg\n";
    die "\n$err\n";
}

#
# my( $ok, @params ) = parse_params( $str );
#   Parse 'void' or a non-empty comma-separated list of "simpletype id"
#   parameters from $str, which should not contain any suffix after the
#   parameters, eg. ) etc which must have always been removed..
#   A simpletype is an identifier followed by zero-or-more '*'s,
#
#   if parsing is successful, build an array of "simpletype id" strings,
#   and return ( 1, array ),
#   else return ( 0, errmsg )
#
fun parse_params( $str )
{
    # print "debug: parsing params from $str\n";
    if( $str =~ /^void\s*$/ )
    {
        return ( 1 );        # empty array
    }
    my @result;

    # a param is something like int p, int *p, int ****p etc.
    while( $str =~ s/^(\w+)\s*([*]*)\s*(\w+)\s*,?\s*// )
    {
        push @result, "$1 $2$3";
    }
    if( $str )
    {
        my $n = @result;
        return (0, "params: junk <$str> after parsing $n params");
    }
    return (1, @result);
}

#
# my( $ok, $info ) = parse_func_line( $line, $checker );
#   Given a line $line, that should be a C-like function defn in which
#   the return type, and each of the parameter types, is a "Duncan's
#   Simple Type" (ie. an identifier followed by zero or more '*'s),
#   attempt to parse the whole line.
#
#   If the line parses successfully as a function definition, then the
#   $checker coderef is called with the function name (to check that the
#   function has not already been defined) - the checker coderef should
#   return an error message if the function is already defined, or undef
#   otherwise.
#
#   A successfully parsed, previously undefined, function
#   causes us to return ( 1, function info hashref ),
#   otherwise we return ( 0, errmsg )
#
#   The fields in a function info hashref are:
#       FUNCNAME     => $funcname,
#       RETURNTYPE   => $returntype,
#       PARAMS       => \@params,
#       PARAMSTR     => $paramstr,
#       ORIGLINE     => $origline,
#
fun parse_func_line( $line, $checker )
{
    chomp $line;
    my $origline = $line;

    $line =~ s/^\s+//;
    my $returntype;
    if( $line =~ /^void\s+\w/ )
    {
        $returntype = "void";
        $line =~ s/^void\s+//;
    }
    elsif( $line =~ s/^(\w+)\s*([*]*)\s*// )
    {
        my( $t, $stars ) = ($1,$2);
        $returntype = $stars ? "$t $stars" : $t;
    }
    else
    {
        return( 0, "<void | simpletype> expected at <$line>" );
    }

    $line =~ s/^(\w+)\s*//;
    my $funcname = $1;

    # check whether $funcname is already defined?
    my $error = $checker->( $funcname );
    return( 0, $error ) if defined $error;

    $line =~ s/^\(\s*//;
    $line =~ s/\s*\)\s*;?$//;

    # line now contains only the parameters.
    my $params = $line;
    my( $ok, @params ) = parse_params( $params );
    return( 0, @params ) unless $ok;

    my $paramstr = @params ? join( ', ', @params ) : 'void';
    my $info = {
        FUNCNAME   => $funcname,
        RETURNTYPE => $returntype,
        PARAMS     => \@params,
        PARAMSTR   => $paramstr,
        ORIGLINE   => $origline,
    };

    return (1, $info);
}

1;  # required by Perl modules
