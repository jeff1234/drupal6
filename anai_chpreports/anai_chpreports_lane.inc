<?php



function chpreport_lane_lanecount($destination,
				  $company_id, $property_id, $lot_id,
				  $start_time, $stop_time,
				  $lot_type, $lot_class,
				  $lane_id) {
  global $user;
  $account = user_load(array('uid' => $user->uid));
  $start = chpuser_datetime_usertimezone_to_utc($start_time);
  $stop = chpuser_datetime_usertimezone_to_utc($stop_time);

  // Retrieve lanes to include in report request
  $lanes = array();
  if ($lane_id === 'ANY') {
    if ((in_array('Admin', array_values($account->roles)) and $account->status) or
	$account->uid == 1) {
      foreach (chdbprop_cc_get_lanes_admin($company_id, $property_id, $lot_id,
				     $lot_type, $lot_class) as $lane) {
	$lanes[$lane['LaneId']] = $lane;
      }
    } else {
      foreach (chdbprop_cc_get_lanes($company_id, $property_id, $lot_id,
				     $lot_type, $lot_class) as $lane) {
	$lanes[$lane['LaneId']] = $lane;
      }
    }
  } else {
    $lanes[$lane_id] = chdbprop_get_lane($property_id, $lane_id);
  } // print_r($lanes);

  // Retrieve the repert entries
  $entries = array();

  // History comes first

  // Current listings
  $entries =
    array_merge($entries,
  		chdbstats_cc_get_loopscount_report
		($start->format("Y-m-d H:i:s"),
		 $stop->format("Y-m-d H:i:s"),
		 array_keys($lanes)));
  //print_r($entries);

  $start = chpuser_datetime_utc_to_usertimezone($start->format("Y-m-d H:i:s"));
  $stop = chpuser_datetime_utc_to_usertimezone($stop->format("Y-m-d H:i:s"));

  if ($destination === 'Browser') {
    $style = "border-bottom:1px solid #b0b0b0;";
    $html .= '<table><tr bgcolor="#e2e2e2" height="35">';
    if ($lane_id === 'ANY') {
      $html .= '<td align="center" style="'.$style.'"><b>'.t('Lane').'</b></td>';
    }
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Direction').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('From').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('To').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Count').'</b></td>';
    $html .= '</tr>';
    $total = 0;
    foreach ($entries as $key => $entry) {
      $html .= '<tr bgcolor="#FFFFFF">';
      if ($lane_id === 'ANY') {
	$html .= '<td align="center">'.decode_entities($lanes[$key]['Alias']).'</td>';
      }
      if ($lanes[$key]['Entry'] and $lanes[$key]['Exit']) {
	$direction = t('Entry/Exit');
      } elseif ($lanes[$key]['Entry']) {
	$direction = t('Entry');
      } elseif ($lanes[$key]['Exit']) {
	$direction = t('Exit');
      } else {
	$direction = t('Unknown');
      }
      $html .= '<td align="center">'.$direction.'</td>';
      $html .= '<td align="center">'.$start->format("M j,y H:i:s").'</td>';
      $html .= '<td align="center">'.$stop->format("M j,y H:i:s").'</td>';
      $count = $entry['Stop'] - $entry['Start'];
      if (0 > $count) {
	$count = 0;
      }
      $total += $count;
      $html .= '<td align="center">'.$count.'</td>';
      $html .= '</tr>';
    }
    if ($lane_id === 'ANY') {
      $html .= '<tr bgcolor="#FFFFFF">';
      $html .= '<td align="right" colspan="4" class="noborder"><b>'.t('Total:').'</b></td>';
      $html .= '<td align="center" class="noborder">'.$total.'</td>';
      $html .= '</tr>';
    }
    $html .= '</table>';
    return $html;
  } else if ($destination === 'CSV') {
    drupal_set_header('Content-Type:text/csv;charset=utf-8');
    drupal_set_header('Content-Disposition:attachment;filename=report.csv');
    $output = fopen('php://output', 'w');
    $headers = array();
    if ($agent_id === 'ANY') {
      $headers[] = t('Lane');
    }
    $headers[] = t('Direction');
    $headers[] = t('From');
    $headers[] = t('To');
    $headers[] = t('Count');
    fputcsv($output, $headers);
    foreach ($entries as $key => $entry) {
      $row = array();
      if ($lane_id === 'ANY') {
	$row[] = decode_entities($lanes[$key]['Alias']);
      }
      if ($lanes[$key]['Entry'] and $lanes[$key]['Exit']) {
	$direction = t('Entry/Exit');
      } elseif ($lanes[$key]['Entry']) {
	$direction = t('Entry');
      } elseif ($lanes[$key]['Exit']) {
	$direction = t('Exit');
      } else {
	$direction = t('Unknown');
      }
      $row[] = $direction;
      $row[] = $start->format("M j,y H:i:s");
      $row[] = $stop->format("M j,y H:i:s");
      $count = $entry['Stop'] - $entry['Start'];
      if (0 > $count) {
	$count = 0;
      }
      $row[] = $count;
      fputcsv($output, $row);
    }
  }
  return;
}



