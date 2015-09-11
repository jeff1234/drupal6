#!/usr/bin/perl -w

use English;
use DBI;

if (@ARGV != 2) {
    print "Usage: LPNDBFile NewPath\n";
    exit();
}

my $dbfile = shift;
my $newpath = shift;

print $dbfile."\n";
print $newpath."\n";

my $dbh = DBI->connect("dbi:SQLite:".$dbfile, "", "", {RaiseError => 1}) or die $DBI::errstr;
my $sql = "SELECT Id,LPImgDB,CarImgDB FROM lpn_table WHERE LPImgRecvd=1 AND CarImgRecvd=1 ";
$sql .= sprintf("AND (LPImgDB NOT LIKE '%s%%' OR CarImgDB NOT LIKE '%s%%')", $newpath, $newpath);
my $fst = $dbh->selectall_arrayref($sql);

foreach my $row (@$fst) {
    my ($id, $lpimgdb, $carimgdb) = @$row;
    $lpimgdb =~ m/^(.+)(\/lpimg_\d{4}-\d{2}-\d{2}\.db)/i;
    my $newlpimgdb = $newpath.$2;
    $carimgdb =~ m/^(.+)(\/carimg_\d{4}-\d{2}-\d{2}\.db)/i;
    my $newcarimgdb = $newpath.$2;
    $sql = sprintf("UPDATE lpn_table SET LPImgDB='%s',CarImgDB='%s' WHERE Id=%d",
     		   $newlpimgdb, $newcarimgdb, $id);
    print $sql."\n";
    $dbh->do($sql);
}

$dbh->disconnect;
print "Done\n";
