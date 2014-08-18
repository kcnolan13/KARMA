<?php

	//image will be rendered as a png
	header('Content-type: image/png');

	//set error reporting
	ini_set('display_errors',1); 
	ini_set('display_startup_errors',1);
	error_reporting(E_ALL);

	//GLOBAL VARS
	$image_width = 512;
	$image_height = 512;
	$origin_x = $image_width/2;
	$origin_y = $image_height/2;
	$barb_stem = $image_width/2;
	$barb_base = $image_width/32;
	$barb_branch_long = $image_width/4;
	$barb_branch_short = $image_width/8;
	$barb_flag_length = $image_width/4;
	$barb_flag_width = $image_width/16;
	$voffset_branch = $barb_branch_short*2/5;
	$voffset_flag = $barb_flag_width+$voffset_branch/4;
	$line_thickness = 6;
	$barb_stem_top = $line_thickness;

	$wind_speed = 0;

	//MAKE ALL THE WIND BARBS
	for ($wind_speed = 0; $wind_speed <= 250; $wind_speed += 5)
	{
		$temp = $wind_speed;
		for ($k=0; $k<6; $k++)
		{
			$wind_speed = $temp;
			//how far down the stem will the next branch or flag be?
			$voffset = $barb_stem_top;

			//how much speed left to account for?
			$speed_remaining = $wind_speed;

			//create a blank canvas
			$canvas = imagecreate($image_width, $image_height);

				//define custom colors to be used
				$c_white = imagecolorallocate($canvas, 255, 255, 255);

				$c_black = imagecolorallocate($canvas, 0, 0, 0);
				$c_red = imagecolorallocate($canvas,255,0,0);
				$c_blue = imagecolorallocate($canvas,0,0,255);
				$c_green = imagecolorallocate($canvas,0,140,0);
				$c_yellow = imagecolorallocate($canvas,225,95,0);
				$c_magenta = imagecolorallocate($canvas,180,0,180);

				$c_grid = imagecolorallocate($canvas, 192, 192, 192);
				$c_plot_normal = imagecolorallocate($canvas, 0, 0, 255);
				$c_yesterday = imagecolorallocate($canvas, 220, 255, 220);
				$c_today = imagecolorallocate($canvas, 180, 255, 180);

			imagealphablending($canvas,true);
			imagecolortransparent($canvas, $c_white);

			//GENERATE THE WIND BARB
			if ($k==0) {
				$barbColor = $c_black;
				$barbAppend = "";
			} else if ($k==1) {
				$barbColor = $c_blue;
				$barbAppend = "_blue";
			} else if ($k==2) {
				$barbColor = $c_green;
				$barbAppend = "_green";
			} else if ($k==3) {
				$barbColor = $c_yellow;
				$barbAppend = "_yellow";
			} else if ($k==4) {
				$barbColor = $c_red;
				$barbAppend = "_red";
			} else if ($k==5) {
				$barbColor = $c_magenta;
				$barbAppend = "_magenta";
			}

			//name this image
			$file_name = './icons/barbs/'.(string)$wind_speed.'knots'.$barbAppend.'.png';

			imagesetthickness($canvas, $line_thickness);
				//draw the base
				imagefilledellipse($canvas, $origin_x, $origin_y, $barb_base, $barb_base, $barbColor);
				//handle the case where wind speed is 0
				if ($wind_speed==0)
				{
					imageellipse($canvas, $origin_x, $origin_y, $barb_base*6, $barb_base*6, $barbColor);
				}
				else
				{
					//have a wind speed of 5 add an offset for the lone small branch
					if ($wind_speed==5)
						$voffset += $voffset_branch;

					//draw the stem
					imageline($canvas,$origin_x,$origin_y,$origin_x,$barb_stem_top,$barbColor);
					while ($speed_remaining > 0)
					{
						if ($speed_remaining>=50)
						{
							//draw a flag
							$vertices = array($origin_x, $voffset, $origin_x, $voffset+$barb_flag_width, $origin_x+$barb_flag_length, $voffset+$barb_flag_width/2);
							imagesetthickness($canvas, 1);
							imagefilledpolygon($canvas, $vertices, 3, $barbColor);
							imagesetthickness($canvas, $line_thickness);
							$speed_remaining -= 50;
							$voffset += $voffset_flag;
						} else if ($speed_remaining >= 10)
						{
							//draw a long branch
							imageline($canvas, $origin_x-$line_thickness/2, $voffset, $origin_x+$barb_branch_long, $voffset, $barbColor);
							$speed_remaining -= 10;
							$voffset += $voffset_branch;
						} else if ($speed_remaining >= 5)
						{
							//draw a long branch
							imageline($canvas, $origin_x-$line_thickness/2, $voffset, $origin_x+$barb_branch_short, $voffset, $barbColor);
							$speed_remaining -= 5;
							$voffset += $voffset_branch;
						}
					}
				}

			//render the canvas and free dynamically allocated memory
			imagepng($canvas);
			imagepng($canvas, $file_name);
			imagedestroy($canvas);
		}
	}

	$canvas = imagecreate($image_width, $image_height);

	$c_white = imagecolorallocate($canvas, 255, 255, 255);
	$c_black = imagecolorallocate($canvas, 0, 0, 0);

	imagealphablending($canvas,true);
	imagecolortransparent($canvas, $c_white);

	$file_name = './icons/planeVector.png';
	imagesetthickness($canvas, $line_thickness*3);
	imageline($canvas,$origin_x,$origin_y,$origin_x,$barb_stem_top+30,$c_black);
	$voffset = 0;
	$vertices = array($origin_x, 0, $origin_x-20, 50, $origin_x+20, 50);
	imagesetthickness($canvas, 1);
	imagefilledpolygon($canvas, $vertices, 3, $c_black);

	imagepng($canvas);
	imagepng($canvas, $file_name);
	imagedestroy($canvas);



	$image_width = 60;
	$image_height = 180;
	$padding = ($image_width)/10;
	$side_length = ($image_width-2*$padding);
	$line_thickness = 2;

	$canvas = imagecreate($image_width, $image_height);

	$c_white = imagecolorallocate($canvas, 255, 255, 255);
	$c_black = imagecolorallocate($canvas, 0, 0, 0);
	$c_red = imagecolorallocate($canvas,255,0,0);
	$c_blue = imagecolorallocate($canvas,0,0,255);
	$c_green = imagecolorallocate($canvas,0,140,0);
	$c_yellow = imagecolorallocate($canvas,225,95,0);
	$c_magenta = imagecolorallocate($canvas,180,0,180);
	$c_grid = imagecolorallocate($canvas, 40, 40, 40);

	imagealphablending($canvas,true);
	imagecolortransparent($canvas, $c_white);

	$font = "./icons/arial.ttf";
	$font_size = 8;
	$file_name = './icons/barbLegend.png';
	imagesetthickness($canvas, $line_thickness);
	imagettftext($canvas, $font_size, 0, $padding,$padding*2,$c_black,$font,"Wind Barbs");
	imagettftext($canvas, $font_size, 0, $padding*1.5,$padding*3.5, $c_black, $font, "(knots)");
	//imagerectangle($canvas,$padding,$padding,$padding+$side_length,$padding+$side_length/2,$c_grid);
	for ($i=1; $i<7; $i++)
	{
		imagerectangle($canvas,$padding,$padding+$side_length/2*$i,$padding+$side_length/2,$padding+$side_length/2*($i+1),$c_grid);
		imagerectangle($canvas,$padding+$side_length/2,$padding+$side_length/2*$i,$padding+$side_length,$padding+$side_length/2*($i+1),$c_grid);
	}

	$color_array = array($c_black, $c_blue, $c_green, $c_yellow, $c_red, $c_magenta);
	$text_array = array("0-63", "64-82", "83-95", "96-112", "113-136", "137+");
	$text_color_array = array($c_white, $c_white, $c_black, $c_black, $c_black, $c_black);
	for ($i=1; $i<7; $i++)
	{
		imagefilledrectangle($canvas,$padding,$padding+$side_length/2*$i,$padding+$side_length,$padding+$side_length/2*($i+1),$color_array[$i-1]);
		imagettftext($canvas, $font_size, 0, $padding+$padding, $padding+$side_length/2*($i+0.75), $text_color_array[$i-1], $font, $text_array[$i-1]);
	}

	//imagepng($canvas);
	imagepng($canvas, $file_name);
	imagedestroy($canvas);
?>
