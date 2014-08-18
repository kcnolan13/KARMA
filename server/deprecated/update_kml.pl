# ------ GLOBAL VARS ------ #
$fIn = "./kml-track.kml";
$fOut = "./kml-track_temp.kml";

#regenerate a whole new .kml file if the proper one doesn't exist
if (!(-e $filename)) {
	open(my $out, '>', $fIn) or die "failed to open (create) $fIn for writing\n";  
 	#pass the file handle off to the new kml routine
 	regenKml($out);
 }

sub regenKml{
	#get args passed
	$n = scalar(@_);
	@args = @_;
	#obtain the filehandle to print to
	$fOut = @args[0];

	#generate xml and kml headings
	print $fOut '<?xml version="1.0" encoding="UTF-8"?>'."\n".'<kml xmlns="http://www.opengis.net/kml/2.2">'."\n";
	#create and name the document
	print $fOut "\t".'<Document>'."\n\t\t".'<name>KML Flight Track</name>'."\n";

	# ------ STYLES ------ #
		$barbScale = 4;
		$barbTabs = "\t\t\t\t\t\t\t";
		$barbIconPath = './icons/barbs/';

		#aircraft track style
		print $fOut "\t\t\t".'<Style id="trackStyle">'."\n\t\t\t\t".'<LineStyle>'."\n\t\t\t\t\t".'<color>641400FF</color>'."\n\t\t\t\t\t".'<width>4</width>'."\n\t\t\t\t".'</LineStyle>'."\n\t\t\t".'</Style>'."\n";

		#all the wind barb styles
		for ($windSpeed = 0; $windSpeed <= 250; $windSpeed += 5)
		{
			print $fOut $barbTabs.'<Style id="'.$windSpeed.'knots">'."\n".$barbTabs."\t".'<IconStyle>'."\n".$barbTabs."\t\t".'<scale>'.$barbScale.'</scale>'."\n";
			print $fOut $barbTabs."\t\t".'<Icon>'."\n".$barbTabs."\t\t\t".'<href>'.$barbIconPath.$windSpeed.'knots.png</href>'."\n";
			print $fOut $barbTabs."\t\t".'</Icon>'."\n".$barbTabs."\t".'</IconStyle>'."\n".$barbTabs.'</Style>'."\n\n";
		}

	# ------ WIND BARBS ------ #
		print $fOut "\t\t".'<Folder id="windBarbs">'."\n\t\t".'</Folder>'."\n\n";

	# ------ AIRCRAFT TRACK ------ #
		print $fOut "\t\t".'<Folder id="acTrack">'."\n";

			#generate aircraft track
			print $fOut "\t\t\t".'<Placemark>'."\n\t\t\t\t".'<styleUrl>#trackStyle</styleUrl>'."\n";
			print $fOut "\t\t\t\t".'<LineString>'."\n\t\t\t\t\t".'<altitudeMode>absolute</altitudeMode>'."\n\t\t\t\t\t".'<coordinates>'."\n";
				#coordinates go here

				#end coordinates
			print $fOut "\t\t\t\t\t".'</coordinates>'."\n\t\t\t\t".'</LineString>'."\n\t\t\t".'</Placemark>'."\n";

		print $fOut "\t\t".'</Folder>'."\n";


	#finish document and close
	print $fOut "\t".'</Document>'."\n".'</kml>';
	close($fOut);
}