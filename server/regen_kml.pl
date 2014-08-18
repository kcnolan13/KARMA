
#=================================================================
# APPLICATION NAME: regen_kml.pl
#
#
# DESCRIPTION: Periodically rebuilds KML files for the KARMA software package
#
# ENVIRONMENT PARAMS:
#
# INPUT PARAMETERS: 
#
# OUTPUT PARAMETERS:
#
# AUTHORS: Kyle Nolan
#
# DATE: 2013.07.13
#
# CONTRIBUTING AUTHORS:
#
# REVISIONS: 
# 
#
#  
#================================================================



#load modules
use DBI;
use Math::Round;
use Math::Trig;
use warnings;
use Time::HiRes;

# ------ GLOBAL VARS ------ #

$MSEC2KNOTS = 1.94384;

#wind barb generation interval (sec)
$barbInterval = 60;
$coordinateInterval = 6;

$run_continuously = 1;

#(update intervals in seconds)
$trackUpdateInterval = 2;
$waypointUpdateInterval = 4;
$updateNumber = 0;

$num_args = $#ARGV;

$fontSize = 2;
$fontFamily = "Arial";

if ($num_args >= 0) {
    $run_continuously = 0;
    $barbInterval = $ARGV[0];
}

$fName = "/home/kyle/KARMA/server/kml-track.kml";
$fNameTemp = "/home/kyle/KARMA/server/kml-track_temp.kml";


#Use the most recent table name

    # ------ Connect to flight_archive db ------ #
    $flight_archive = DBI->connect("DBI:Pg:dbname=flight_archive;host=localhost","operator1","",{'RaiseError'=>1}) or die "Failed to connect to flight_archive database";

    $query = $flight_archive->prepare("select relname from pg_class where relname LIKE '%fl_%' order by ctid desc limit 1;");
    $query->execute();
   
    while(my $row = $query->fetchrow_hashref())
    {
        $tableName = $row->{'relname'};
    }

    #disconnect from the database
    $flight_archive->disconnect();
#Done picking table name

$waypointName = "/home/kyle/KARMA/server/kml-waypoints.kml";
$waypointNameTemp = "/home/kyle/KARMA/server/kml-waypoints_temp.kml";
$waypointTrackName = "/home/kyle/KARMA/server/kml-waypoints-track.kml";
$waypointTrackNameTemp = "/home/kyle/KARMA/server/kml-waypoints-track_temp.kml";
$waypointConfigFile = "/home/kyle/KARMA/server/drops.txt";

$landmarkName = "/home/kyle/KARMA/server/kml-landmarks.kml";
$landmarkNameTemp = "/home/kyle/KARMA/server/kml-landmarks_temp.kml";
$landmarkConfigFile = "/home/kyle/KARMA/server/landmarks.txt";

    #deduce flightName from tableName --> need data from Leonard
    if (index($tableName, "i") != -1) {
        $flightName = "N43RF";
    } elsif (index($tableName, "h") != -1) {
        $flightName = "N42RF";
    } elsif (index($tableName, "n") != -1) {
        $flightName = "N49RF";
    } else {
        $flightName = "Unknown";
    }

$trackLineWidth = 2;
$waypointLineWidth = 2;
$waypointLineColor = "50003C14";
$barbScale = 3.5;
$planeScale = 1.5;
$vectorScale = 4;
$waypointScale = 0.5;
$landmarkScale = 0.5;
$barbTabs = "\t\t\t\t\t\t\t";
$barbIconPath = './icons/barbs/';
$planeIcon = './icons/plane.png';
$waypointIcon = './icons/marker.png';
$landmarkIcon = './icons/marker_blue.png';
$vectorIcon = './icons/planeVector.png';

#round wind barb direction to nearest (deg)
$barbDegNearest = 5;
@barbData = ("time", "mdshour_1", "mdsminute_1", "mdssecond_1", "latref", "lonref", "altref", "psmref", "psurf_d", "asfmrws_1", "asfmrrainrate_1", "ws_d", "wd_d", "ta_d", "td_c", "sfmrws_r", "sfmrrainrate_r", "uwz_d", "gs_d", "tas_d", "ht_d", "altpa_d", "da_d", "trk_d", "trkdesired_d", "coursecorr_d");
@barbNickNames = ("time", "mdshour", "mdsminute", "mdssecond", "LAT", "LON", "ALT", "STATIC_PRESS", "EXTRAP_SLP", "aSFMR_WS", "aSFMR_RR", "WS", "WD", "ATMP", "DEWPT", "pSFMR_WS", "pSFMR_RR", "VERT_WIND", "GROUND_SPD", "TRUE_AIR_SPD", "STAND_HT", "PRESS_ALT", "DRIFT_ANGLE", "TRACK", "CTR", "CC");
$planeTime = 0;

#continue to periodically regenerate the kml file
while (1)
{
    # ------ Connect to flight_archive db ------ #
    $flight_archive = DBI->connect("DBI:Pg:dbname=flight_archive;host=localhost","operator1","",{'RaiseError'=>1}) or die "Failed to connect to flight_archive database";

    #open a temp file that will eventually replace the real .kml file
    open($fOut, '>', $fNameTemp) or die "failed to open (create) $fNameTemp for writing\n"; 


    #Show data in database
=cut
    $query = $flight_archive->prepare("SELECT time, latref, lonref, altref FROM $tableName ORDER BY time");
    $query->execute();
    while(my $row = $query->fetchrow_hashref()) {
        print "time = $row->{'time'}\t\tlatref = $row->{'latref'}\t\tlonref = $row->{'lonref'}\t\taltref = $row->{'altref'}\n";
    }
=cut

    # ------ GENERATE THE KML FILE ------ #

        #generate xml and kml headings
        print $fOut '<?xml version="1.0" encoding="UTF-8"?>'."\n".'<kml xmlns="http://www.opengis.net/kml/2.2">'."\n";
        #create and name the document
        print $fOut "\t".'<Document>'."\n\t\t".'<name>KML Flight Track</name>'."\n";

        # ------ STYLES ------ #
            #aircraft track style
            print $fOut "\t\t\t".'<Style id="trackStyle">'."\n\t\t\t\t".'<LineStyle>'."\n\t\t\t\t\t".'<color>641400FF</color>'."\n\t\t\t\t\t".'<width>'."$trackLineWidth".'</width>'."\n\t\t\t\t".'</LineStyle>'."\n\t\t\t".'</Style>'."\n";

            #all the wind barb styles
            for ($windSpeed = 0; $windSpeed <= 250; $windSpeed += 5)
            {
                print $fOut $barbTabs.'<Style id="'.$windSpeed.'knots">'."\n".$barbTabs."\t".'<IconStyle>'."\n".$barbTabs."\t\t".'<scale>'.$barbScale.'</scale>'."\n";
                print $fOut $barbTabs."\t\t".'<Icon>'."\n".$barbTabs."\t\t\t".'<href>'.$barbIconPath.$windSpeed.'knots.png</href>'."\n";
                print $fOut $barbTabs."\t\t".'</Icon>'."\n".$barbTabs."\t".'</IconStyle>'."\n".$barbTabs.'</Style>'."\n\n";
            }

            #plane icon style
            print $fOut $barbTabs.'<Style id="plane">'."\n".$barbTabs."\t".'<IconStyle>'."\n".$barbTabs."\t\t".'<scale>'.$planeScale.'</scale>'."\n";
            print $fOut $barbTabs."\t\t".'<Icon>'."\n".$barbTabs."\t\t\t".'<href>'.$planeIcon.'</href>'."\n";
            print $fOut $barbTabs."\t\t".'</Icon>'."\n".$barbTabs."\t".'</IconStyle>'."\n".$barbTabs.'</Style>'."\n\n";

            #vector style
            print $fOut $barbTabs.'<Style id="vector">'."\n".$barbTabs."\t".'<IconStyle>'."\n".$barbTabs."\t\t".'<scale>'.$vectorScale.'</scale>'."\n";
            print $fOut $barbTabs."\t\t".'<Icon>'."\n".$barbTabs."\t\t\t".'<href>'.$vectorIcon.'</href>'."\n";
            print $fOut $barbTabs."\t\t".'</Icon>'."\n".$barbTabs."\t".'</IconStyle>'."\n".$barbTabs.'</Style>'."\n\n";

        # ------ AIRCRAFT TRACK ------ #
            print $fOut "\t\t".'<Folder id="acTrack">'."\n";

                #generate aircraft track
                print $fOut "\t\t\t".'<Placemark>'."\n\t\t\t\t".'<styleUrl>#trackStyle</styleUrl>'."\n";
                print $fOut "\t\t\t\t".'<name>ACTrack</name>'."\n";
                print $fOut "\t\t\t\t".'<index>-100</index>'."\n";
                print $fOut "\t\t\t\t".'<LineString>'."\n\t\t\t\t\t".'<altitudeMode>absolute</altitudeMode>'."\n\t\t\t\t\t".'<coordinates>'."\n";
                    #fetch the track coordinates from the database
                    $query = $flight_archive->prepare("SELECT time, latref, lonref, altref FROM $tableName WHERE (time::int%$coordinateInterval)=0 OR time = (SELECT time from $tableName WHERE time>=$planeTime ORDER BY time DESC limit 1) ORDER BY time");
                    $query->execute();
                    while(my $row = $query->fetchrow_hashref())
                    {
                        if (($row->{'lonref'})&&($row->{'latref'})&&($row->{'altref'}))
                        {
                            print $fOut "\t\t\t\t\t\t$row->{'lonref'}, $row->{'latref'}, $row->{'altref'}\n";

                            #save planeTime for later
                            $planeTime = $row->{'time'};
                            #print "$row->{'lonref'}, $row->{'latref'}, $row->{'altref'}\n";
                        }
                    }
                    #end coordinate fetching
                print $fOut "\t\t\t\t\t".'</coordinates>'."\n\t\t\t\t".'</LineString>'."\n\t\t\t".'</Placemark>'."\n";

            print $fOut "\t\t".'</Folder>'."\n";

        # ------ WIND BARBS ------ #
        print $fOut "\t\t".'<Folder id="windBarbs">'."\n";
            #fetch the wind data from the database
                $barbDataString = "";
                for ($i=0; $i<@barbData; $i++)
                {
                    $barbDataString = $barbDataString.$barbData[$i];
                    if ($i<@barbData-1) {
                        $barbDataString = $barbDataString.", ";
                    }
                }
                $query = $flight_archive->prepare("SELECT $barbDataString FROM $tableName WHERE (time::int%$barbInterval)=0 ORDER BY time");
                $query->execute();
            #end fetching

            #construct the wind barbs in KML
            $barbIndex = 0;
                while(my $row = $query->fetchrow_hashref()) {
                        #debug to console
                        print "$barbData[0] = $row->{$barbData[0]}   $barbData[1] = $row->{$barbData[1]}   $barbData[2] = $row->{$barbData[2]}   $barbData[3] = $row->{$barbData[3]}   $barbData[4] = $row->{$barbData[4]} \n";
                        print $fOut "\t\t\t".'<Placemark>'."\n\t\t\t\t".'<name>'."&lt;font face=$fontFamily size=$fontSize &gt; ".sprintf("%02d",$row->{'mdshour_1'}).":".sprintf("%02d",$row->{'mdsminute_1'}).":".sprintf("%02d", nearest(1,$row->{'mdssecond_1'}))." Z - "."$flightName".'</name>'."\n";
                        print $fOut "\t\t\t\t".'<description>'."&lt;font face=$fontFamily size=$fontSize &gt; "."\n";

                        #description data
                        for ($i=4; $i<@barbData; $i++)
                        {
                            if (($barbData[$i] ne "latref")&&($barbData[$i] ne "lonref")) {
                                print $fOut "\t\t\t\t".$barbNickNames[$i].":  ".nearest(.1,$row->{$barbData[$i]}).'&lt;br&gt;'."\n";
                            } else {
                                print $fOut "\t\t\t\t".$barbNickNames[$i].":  ".nearest(.0000001,$row->{$barbData[$i]}).'&lt;br&gt;'."\n";
                            }

                        }

                        print $fOut "\t\t\t\t".'</description>'."\n";

                        #heading --> how to rotate the icon --> round for better aesthetic
                        $wind_direction = nearest($barbDegNearest, $row->{'wd_d'});
                        print $fOut "\t\t\t\t".'<heading>'."$wind_direction".'</heading>'."\n";
                        print $fOut "\t\t\t\t".'<index>'."$barbIndex".'</index>'."\n";
                        #get the right icon for this velocity
                        $wind_speed = nearest(5, $row->{'ws_d'}*$MSEC2KNOTS);
                        $styleUrl = '#'."$wind_speed"."knots";
                        print $fOut "\t\t\t\t".'<styleUrl>'."$styleUrl".'</styleUrl>'."\n";
                        print $fOut "\t\t\t\t".'<Point>'."\n";
                        print $fOut "\t\t\t\t\t".'<coordinates>'."$row->{'lonref'}, $row->{'latref'}, $row->{'altref'}".'</coordinates>'."\n";
                        print $fOut "\t\t\t\t".'</Point>'."\n\t\t\t".'</Placemark>'."\n\n";
                        $barbIndex++;
                    }


        #end the wind barbs section of KML
        print $fOut "\t\t".'</Folder>'."\n\n";

        # ------ MAKE THE PLANE ICON AND THE VECTOR / LATEST BARB ------ #

            $query = $flight_archive->prepare("SELECT $barbDataString FROM $tableName WHERE time=(SELECT time from $tableName WHERE time>=$planeTime ORDER BY time DESC limit 1) ORDER BY time");
            $query->execute();

            $row = $query->fetchrow_hashref();

            #now add the KML
            print $fOut "\t\t\t".'<Placemark>'."\n\t\t\t\t".'<name>'."&lt;font face=$fontFamily size=$fontSize &gt; "."Aircraft: $flightName ".'</name>'."\n";
           
            print $fOut "\t\t\t\t".'<description>'."&lt;font face=$fontFamily size=$fontSize &gt; "."\n";

            #description data
            print $fOut "\t\t\t\tTime: ".sprintf("%02d",$row->{'mdshour_1'}).":".sprintf("%02d",$row->{'mdsminute_1'}).":".sprintf("%02d", nearest(1,$row->{'mdssecond_1'}))." Z".'&lt;br&gt;'."\n";
           
            for ($i=4; $i<@barbData; $i++)
            {
                if (($barbData[$i] ne "latref")&&($barbData[$i] ne "lonref")) {
                    print $fOut "\t\t\t\t".$barbNickNames[$i].":  ".nearest(.1,$row->{$barbData[$i]}).'&lt;br&gt;'."\n";
                } else {
                    print $fOut "\t\t\t\t".$barbNickNames[$i].":  ".nearest(.00001,$row->{$barbData[$i]}).'&lt;br&gt;'."\n";
                }

            }
            print $fOut "\t\t\t\t".'</description>'."\n";
            print $fOut "\t\t\t\t".'<heading>'."$row->{'trk_d'}".'</heading>'."\n";
            print $fOut "\t\t\t\t".'<index>'."-99".'</index>'."\n";
            print $fOut "\t\t\t\t".'<styleUrl>'.'#plane'.'</styleUrl>'."\n";
            print $fOut "\t\t\t\t".'<Point>'."\n";
            print $fOut "\t\t\t\t\t".'<coordinates>'."$row->{'lonref'}, $row->{'latref'}, $row->{'altref'}".'</coordinates>'."\n";
            print $fOut "\t\t\t\t".'</Point>'."\n\t\t\t".'</Placemark>'."\n\n";

            #plane vector icon
            print $fOut "\t\t\t".'<Placemark>'."\n\t\t\t\t".'<name>'."Vector".'</name>'."\n";
            print $fOut "\t\t\t\t".'<description>undefined</description>'."\n";
            print $fOut "\t\t\t\t".'<heading>'."$row->{'trk_d'}".'</heading>'."\n";
            print $fOut "\t\t\t\t".'<index>'."-101".'</index>'."\n";
            print $fOut "\t\t\t\t".'<styleUrl>'.'#vector'.'</styleUrl>'."\n";
            print $fOut "\t\t\t\t".'<Point>'."\n";
            print $fOut "\t\t\t\t\t".'<coordinates>'."$row->{'lonref'}, $row->{'latref'}, $row->{'altref'}".'</coordinates>'."\n";
            print $fOut "\t\t\t\t".'</Point>'."\n\t\t\t".'</Placemark>'."\n\n";

            #most recent wind barb
            print $fOut "\t\t\t".'<Placemark>'."\n\t\t\t\t".'<name>'."&lt;font face=$fontFamily size=$fontSize &gt; ".sprintf("%02d",$row->{'mdshour_1'}).":".sprintf("%02d",$row->{'mdsminute_1'}).":".sprintf("%02d", nearest(1,$row->{'mdssecond_1'}))." Z - "."$flightName".'</name>'."\n";
            print $fOut "\t\t\t\t".'<description>'."&lt;font face=$fontFamily size=$fontSize &gt; "."\n";
            print $fOut "undefined\n";
            print $fOut "\t\t\t\t".'</description>'."\n";

            #heading --> how to rotate the icon --> round for better aesthetic
            $wind_direction = nearest($barbDegNearest, $row->{'wd_d'});
            print $fOut "\t\t\t\t".'<heading>'."$wind_direction".'</heading>'."\n";
            print $fOut "\t\t\t\t".'<index>-101</index>'."\n";
            #get the right icon for this velocity
            $wind_speed = nearest(5, $row->{'ws_d'}*$MSEC2KNOTS);
            $styleUrl = '#'."$wind_speed"."knots";
            print $fOut "\t\t\t\t".'<styleUrl>'."$styleUrl".'</styleUrl>'."\n";
            print $fOut "\t\t\t\t".'<Point>'."\n";
            print $fOut "\t\t\t\t\t".'<coordinates>'."$row->{'lonref'}, $row->{'latref'}, $row->{'altref'}".'</coordinates>'."\n";
            print $fOut "\t\t\t\t".'</Point>'."\n\t\t\t".'</Placemark>'."\n\n";


            # ------ END PLANE ICONS ------ #
            $query->finish();




        #finish document and close
        print $fOut "\t".'</Document>'."\n".'</kml>';
        close($fOut);

    # ------ END GENERATE KML FILE ------ #



    # ------ REPLACE EXISTING KML FILE ------ #
    $result = `mv $fNameTemp $fName`;

    #disconnect from the database
    $flight_archive->disconnect();

    #only update drops and landmarks every so often
    if ($updateNumber % ($waypointUpdateInterval/$trackUpdateInterval) == 0)
    {
            # ------ SCP THE DROP AND LANDMARK CONFIG FILES FROM THE MDS ------ #
            $result = `sshpass -p "noaa2306" scp operator\@10.10.1.10:/home/data/landmarks.txt /var/www/html/KARMA/landmarks.txt`;
            $result = `sshpass -p "noaa2306" scp operator\@10.10.1.10:/home/data/drops.txt /var/www/html/KARMA/drops.txt`;

            print "$result\n";

            # ------ GENERATE THE WAYPOINT KML FILE ------#
            open(IN, $waypointConfigFile);

            open($fOut, '>', $waypointNameTemp) or die "failed to open (create) $waypointNameTemp for writing\n"; 
            open($fOut2, '>', $waypointTrackNameTemp) or die "failed to open (create) $waypointTrackNameTemp for writing\n";

            if (IN)
            {
                print "\n\n generating waypoint KML file\n\n";
                print ("and simultaneously generating waypoint track KML file\n\n");

                #generate xml and kml headings
                print $fOut '<?xml version="1.0" encoding="UTF-8"?>'."\n".'<kml xmlns="http://www.opengis.net/kml/2.2">'."\n";
                print $fOut2 '<?xml version="1.0" encoding="UTF-8"?>'."\n".'<kml xmlns="http://www.opengis.net/kml/2.2">'."\n";
                #create and name the document
                print $fOut "\t".'<Document>'."\n\t\t".'<name>KML Waypoints</name>'."\n";
                print $fOut2 "\t".'<Document>'."\n\t\t".'<name>KML Waypoints Track</name>'."\n";
                # ------ STYLES ------ #
                    #waypoint track style
                    print $fOut2 "\t\t\t".'<Style id="trackStyle">'."\n\t\t\t\t".'<LineStyle>'."\n\t\t\t\t\t".'<color>'.$waypointLineColor.'</color>'."\n\t\t\t\t\t".'<width>'."$waypointLineWidth".'</width>'."\n\t\t\t\t".'</LineStyle>'."\n\t\t\t".'</Style>'."\n";

                    #waypoint style
                    print $fOut $barbTabs.'<Style id="waypoint">'."\n".$barbTabs."\t".'<IconStyle>'."\n".$barbTabs."\t\t".'<scale>'.$waypointScale.'</scale>'."\n";
                    print $fOut $barbTabs."\t\t".'<Icon>'."\n".$barbTabs."\t\t\t".'<href>'.$waypointIcon.'</href>'."\n";
                    print $fOut $barbTabs."\t\t".'</Icon>'."\n".$barbTabs."\t".'</IconStyle>'."\n".$barbTabs.'</Style>'."\n\n";

                # ------ WAYPOINT TRACK ------ #
                    print $fOut2 "\t\t".'<Folder id="waypointTrack">'."\n";

                        #generate track
                        print $fOut2 "\t\t\t".'<Placemark>'."\n\t\t\t\t".'<styleUrl>#trackStyle</styleUrl>'."\n";
                        print $fOut2 "\t\t\t\t".'<name>WaypointTrack</name>'."\n";
                        print $fOut2 "\t\t\t\t".'<index>-100</index>'."\n";
                        print $fOut2 "\t\t\t\t".'<LineString>'."\n\t\t\t\t\t".'<altitudeMode>absolute</altitudeMode>'."\n\t\t\t\t\t".'<coordinates>'."\n";
                           
                            # ------ PARSE THE CONFIG FILE
                            @names = ();
                            @lats = ();
                            @lons = ();
                            $counter = 0;
                            $dashCounter = 0;

                            while (my $line = <IN>) {

                                #trim left side whitespace
                                $line =~ s/^\s+//;

                                #look for the line of -'s
                                if ($line =~ m/\-{10,}/)
                                {
                                    print "------------ detected\n";
                                    $dashCounter++;
                                    if ($dashCounter==1) {
                                        next;
                                    }
                                }

                                if ($dashCounter>1) {
                                    next;
                                } elsif ($dashCounter>0) {
                                    ($name, $lat1, $lat2, $lon1, $lon2)  = split /\s+/, $line;
                                    $names[$counter] = $name;
                                    if ($lat1 < 0) {
                                        $lat2 *= -1;
                                    }
                                    if ($lon1 < 0) {
                                        $lon2 *= -1;
                                    }
                                    if (!($lat1)) {
                                        print "\nline break detected. finishing up\n";
                                        next;
                                    }
                                    $lats[$counter] = $lat1 + $lat2/60;
                                    $lons[$counter] = $lon1 + $lon2/60;

                                    $counter++;
                                }
                            }

                            foreach (@names) {print; print "\n"}
                            print "\n\n";

                            for ($i=0; $i<$counter; $i++)
                            {
                                    print $fOut2 "\t\t\t\t\t\t$lons[$i], $lats[$i], 0\n";
                            }
                            #end coordinate fetching

                        print $fOut2 "\t\t\t\t\t".'</coordinates>'."\n\t\t\t\t".'</LineString>'."\n\t\t\t".'</Placemark>'."\n";

                    print $fOut2 "\t\t".'</Folder>'."\n\n";

                    print "waypoint track generated\n";

                #now make the placemarks
                print $fOut "\t\t".'<Folder>'."\n";

                for ($i=0; $i<$counter; $i++)
                {
                    print $fOut "\t\t\t".'<Placemark>'."\n\t\t\t\t".'<name>&lt;font color="white"&gt;'.$names[$i].'&lt;/font&gt; </name>'."\n";
                    print $fOut "\t\t\t\t".'<index>'."-99".'</index>'."\n";
                    print $fOut "\t\t\t\t".'<styleUrl>'.'#waypoint'.'</styleUrl>'."\n";
                    print $fOut "\t\t\t\t".'<Point>'."\n";
                    print $fOut "\t\t\t\t\t".'<coordinates>'."$lons[$i], $lats[$i], 0".'</coordinates>'."\n";
                    print $fOut "\t\t\t\t".'</Point>'."\n\t\t\t".'</Placemark>'."\n";
                }

                print $fOut "\t\t".'</Folder>'."\n\n";

                #finish document and close
                print $fOut "\t".'</Document>'."\n".'</kml>';
                print $fOut2 "\t".'</Document>'."\n".'</kml>';
                close($fOut);
                close($fOut2);

                # ------ END GENERATE WAYPOINT KML FILE ------ #

                # ------ REPLACE EXISTING WAYPOINT KML FILE ------ #
                $result = `mv $waypointNameTemp $waypointName`;
                $result = `mv $waypointTrackNameTemp $waypointTrackName`;

                print "both KML waypoint files finished\n";
            }


            # ------ GENERATE THE LANDMARK KML FILE ------#
            open(IN, $landmarkConfigFile);

            open($fOut, '>', $landmarkNameTemp) or die "failed to open (create) $landmarkNameTemp for writing\n"; 

            if (IN)
            {
                print "\n\n generating landmark KML file\n\n";

                #generate xml and kml headings
                print $fOut '<?xml version="1.0" encoding="UTF-8"?>'."\n".'<kml xmlns="http://www.opengis.net/kml/2.2">'."\n";
                #create and name the document
                print $fOut "\t".'<Document>'."\n\t\t".'<name>KML Landmarks</name>'."\n";

                # ------ STYLES ------ #
                   
                    #landmark style
                    print $fOut $barbTabs.'<Style id="waypoint">'."\n".$barbTabs."\t".'<IconStyle>'."\n".$barbTabs."\t\t".'<scale>'.$landmarkScale.'</scale>'."\n";
                    print $fOut $barbTabs."\t\t".'<Icon>'."\n".$barbTabs."\t\t\t".'<href>'.$landmarkIcon.'</href>'."\n";
                    print $fOut $barbTabs."\t\t".'</Icon>'."\n".$barbTabs."\t".'</IconStyle>'."\n".$barbTabs.'</Style>'."\n\n";

                # ------ PARSE THE CONFIG FILE
                            @names = ();
                            @lats = ();
                            @lons = ();
                            $counter = 0;
                            $dashCounter = 0;

                            while (my $line = <IN>) {

                                #trim left side whitespace
                                $line =~ s/^\s+//;

                                #look for the line of -'s
                                if ($line =~ m/\-{10,}/)
                                {
                                    print "------------ detected\n";
                                    $dashCounter++;
                                    if ($dashCounter==1) {
                                        next;
                                    }
                                }

                                if ($dashCounter>1) {
                                    next;
                                } elsif ($dashCounter>0) {
                                    ($name, $lat1, $lat2, $lon1, $lon2)  = split /\s+/, $line;
                                    $names[$counter] = $name;
                                    if ($lat1 < 0) {
                                        $lat2 *= -1;
                                    }
                                    if ($lon1 < 0) {
                                        $lon2 *= -1;
                                    }
                                    if (!($lat1)) {
                                        print "\nline break detected. finishing up\n";
                                        next;
                                    }
                                    $lats[$counter] = $lat1 + $lat2/60;
                                    $lons[$counter] = $lon1 + $lon2/60;

                                    $counter++;
                                }
                            }

                #now make the placemarks
                print $fOut "\t\t".'<Folder>'."\n";

                foreach (@names) {print; print "\n"}
                            print "\n\n";

                for ($i=0; $i<$counter; $i++)
                {
                    print $fOut "\t\t\t".'<Placemark>'."\n\t\t\t\t".'<name>&lt;font color="white"&gt;'.$names[$i].'&lt;/font&gt; </name>'."\n";
                    print $fOut "\t\t\t\t".'<index>'."-99".'</index>'."\n";
                    print $fOut "\t\t\t\t".'<styleUrl>'.'#waypoint'.'</styleUrl>'."\n";
                    print $fOut "\t\t\t\t".'<Point>'."\n";
                    print $fOut "\t\t\t\t\t".'<coordinates>'."$lons[$i], $lats[$i], 0".'</coordinates>'."\n";
                    print $fOut "\t\t\t\t".'</Point>'."\n\t\t\t".'</Placemark>'."\n";
                }

                print $fOut "\t\t".'</Folder>'."\n\n";

                #finish document and close
                print $fOut "\t".'</Document>'."\n".'</kml>';
                close($fOut);

                # ------ END GENERATE WAYPOINT KML FILE ------ #

                # ------ REPLACE EXISTING WAYPOINT KML FILE ------ #
                $result = `mv $landmarkNameTemp $landmarkName`;

                print "landmark KML file finished\n";
            }
    }

    if ($run_continuously == 1){
        print "\nKML update $updateNumber completed\n\n";
        $updateNumber++;
        #sleep less because the scp is known to take some time
        if ($updateNumber % ($waypointUpdateInterval/$trackUpdateInterval) == 0)
        {
            Time::HiRes::usleep(($trackUpdateInterval-1.25)*1000000);
        } else {
                Time::HiRes::usleep(($trackUpdateInterval-0.5)*1000000);
            }
    }
    else {
        exit;
    }
}