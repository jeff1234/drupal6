<?php
// $Id$
require_once("anai_chpscans_db.inc");



// Return 0 on success, otherwise 1
function chpscan_image_extract_lpfile($scan, $lpimg, &$lpimagefile) {
  if (0 == preg_match('/.+\/(lpn_.+).db$/', $scan['DB'], $matches)) {
    drupal_set_message(t('DB lookup error, take a screen capture and call sypport'), 'error');
    drupal_set_message($scan['LPN']."|".$scan['LPImgId']."|".$scan['DB']."|LP.jpg", 'error');
  }
  $basename = $scan['LPN']."__".$scan['LPImgId']."__".$matches[1]."__LP.jpg";
  //drupal_set_message($basename);
  $lpimagefile = "tmpimage/".$basename;
  if (!file_exists($lpimagefile)) {
    $lpimagefile = file_create_filename($basename, $base_path."tmpimage");
    file_put_contents($lpimagefile, $lpimg['LPImg']);
  }
  return 0;
}



// Return 0 on success, otherwise 1
function chpscan_image_extract_carfile($scan, $carimg, &$carimagefile) {
  if (0 == preg_match('/.+\/(lpn_.+).db$/', $scan['DB'], $matches)) {
    drupal_set_message(t('DB lookup error, take a screen capture and call support'), 'error');
    drupal_set_message($scan['LPN']."|".$scan['CarImgId']."|".$scan['DB']."|Car.jpg", 'error');
  }
  $basename = $scan['LPN']."__".$scan['CarImgId']."__".$matches[1]."__Car.jpg";
  //drupal_set_message($basename);
  $carimagefile = "tmpimage/".$basename;
  if (!file_exists($carimagefile)) {
    $carimagefile = file_create_filename($basename, $base_path."tmpimage");
    file_put_contents($carimagefile, $carimg['CarImg']);
  }
  return 0;
}



function chpscan_lpn_map_form($entries) {
  $map_array['id'] = 'LocationMap';
  $map_array['width'] = '100%';
  $map_array['height'] = '300px';
  $map_array['latitude'] = '49.6023';
  $map_array['longitude'] = '-124.9391';
  $map_array['maptype'] = 'Terrain';
  $map_array['controltype'] = 'Large';
  $map_array['zoom'] = '12';
  $map_array['behavior'] =
    array('locpick' => FALSE,
	  'nodrag' => FALSE,
	  'nokeyboard' => TRUE,
	  'overview' => TRUE,
	  'autozoom' => TRUE,
	  'scale' => TRUE,
	  );
  
  // create markers
  $markers = array();
  foreach ($entries as $entry) {
    // Limit the number of markers
    if (count($markers) > 25) {
      array_shift($markers);
    }

    $created = chpuser_datetime_utc_to_usertimezone($entry['AgentTime']);

    $markers[] = array
      ('text' => t('Seen at this location on:<br>timestamp',
		   array(timestamp => $created->format("M j,y H:i"))),
       'latitude' => sprintf("%f", $entry['Lat']),
       'longitude' => sprintf("%f", $entry['Lon']),
       'markername' => 'small purple',
       );
  } //print_r($markers); //dvm($markers);

  if (count($markers) > 0) {
    $last_marker = array_pop($markers);
    $last_marker['markername'] = 'blue';
    $markers[] = $last_marker;
    $map_array['markers'] = $markers;
    $form['map']['db'] =
      array('#value' => theme('gmap', array('#settings' => $map_array)));
  } else {
    $form['map']['db'] =
      array('#value' => t('There are no positions to map!'));
  }
  return $form;
}



