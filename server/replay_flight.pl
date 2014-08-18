#load modules
use DBI;
use Math::Round;
use Math::Trig;
use Time::HiRes;
use warnings;

# ------ GLOBAL VARS ------ #

$num_args = $#ARGV;
$replay_speed = 100;

if ($num_args >= 0) {
	$oldTableName = $ARGV[0];
} else {
	print "USAGE: perl replay_flight \"flight_name\"\n";
	exit;
}

#connect to the flight archive
$flight_archive = DBI->connect("DBI:Pg:dbname=flight_archive;host=localhost","operator1","",{'RaiseError'=>1}) or die "Failed to connect to flight_archive database";

#obtain the new table name
$count = 1;
$i = 1;
while ($count != 0)
{
	$newTableName = "replay_".$oldTableName."_$i";
	$count = $flight_archive->selectrow_array("select count(*) from pg_class where relname='".$newTableName."'", undef, @params);
	$i++;
}

#get all the columns from the old table
$queryNames = $flight_archive->prepare("SELECT * FROM $oldTableName WHERE 1=0");
$queryNames->execute();


$statement = "CREATE TABLE ".$newTableName."(".(join " REAL, ", @{$queryNames->{NAME}})." REAL)";
print "creating table: \n$statement\n\n";

$query = $flight_archive->prepare($statement);
$query->execute();

#find out how many rows we need to copy over
$existingRows = $flight_archive->selectrow_array("select count(*) from $oldTableName", undef, @params);
print "$existingRows Existing Rows To Copy\n\n";

$currentTime = $flight_archive->selectrow_array("select MIN(time) from $oldTableName", undef, @params);
$currentRow = 0;
$allColumns = join ", ", @{$queryNames->{NAME}};
while ($currentRow < $existingRows)
{
	$result = `psql --username=operator1 flight_archive -c "copy (SELECT $allColumns FROM $oldTableName WHERE time=$currentTime) TO STDOUT" | psql --username=operator1 flight_archive -c "copy $newTableName ($allColumns) FROM STDIN"`;
	print ("new row: $currentRow, new time: $currentTime\n");
	$currentRow++;
	$currentTime++;
	Time::HiRes::usleep(1000000/$replay_speed);
}
=cut

my $sth = $dbh->prepare("SELECT * FROM $tablename WHERE 1=0");
   $sth->execute();
   my $fields = $sth->{NAME};

#continue to periodically regenerate the kml file
while (1)
{
	# ------ Connect to flight_archive db ------ #
	$flight_archive = DBI->connect("DBI:Pg:dbname=flight_archive;host=localhost","operator1","",{'RaiseError'=>1}) or die "Failed to connect to flight_archive database";


				$query = $flight_archive->prepare("SELECT $barbDataString FROM $tableName WHERE (time::int%$barbInterval)=0 ORDER BY time");
				$query->execute();
			#end fetching

				while(my $row = $query->fetchrow_hashref()) {
						#debug to console
					    print "$barbData[0] = $row->{$barbData[0]}   $barbData[1] = $row->{$barbData[1]}   $barbData[2] = $row->{$barbData[2]}   $barbData[3] = $row->{$barbData[3]}   $barbData[4] = $row->{$barbData[4]}   $barbData[5] = $row->{$barbData[5]}   $barbData[6] = $row->{$barbData[6]}   $barbData[7] = $row->{$barbData[7]}   $barbData[8] = $row->{$barbData[8] }\n";
						$barbIndex++;
					}
=cut
