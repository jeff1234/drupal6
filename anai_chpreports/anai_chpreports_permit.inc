<?php



function chpreport_activation_deactivation($destination,
					   $company_id, $property_id, $lot_id,
					   $start_time, $stop_time,
					   $lot_type, $lot_class,
					   $permit_class) {
  $start = chpuser_datetime_usertimezone_to_utc($start_time);
  $stop = chpuser_datetime_usertimezone_to_utc($stop_time);

  // Retrieve the repert entries
  $entries = array();

  // History comes first

  // Current listings
  $entries =
    array_merge($entries,
  		chdbperm_get_activation_deactivation_report
		($start->format("Y-m-d H:i:s"),
		 $stop->format("Y-m-d H:i:s"),
		 $company_id, $property_id, $lot_id,
		 $lot_type, $lot_class,
		 $permit_class));

  if ($destination === 'Browser') {
    $style = "border-bottom:1px solid #b0b0b0;";
    $html .= '<table><tr bgcolor="#e2e2e2" height="35">';
    if ($permit_class === 'ANY') {
      $html .= '<td align="center" style="'.$style.'"><b>'.t('Class').'</b></td>';
    }
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Permit tag').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Name').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Stall').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('LPN').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Activation').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Deactivation').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Booked').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Comment').'</b></td>';
    $html .= '</tr>';
    foreach ($entries as $entry) {
      $html .= '<tr bgcolor="#FFFFFF">';
      if ($permit_class === 'ANY') {
	$html .= '<td align="center">'.chpperm_registration_name($entry['PermitClass'], FALSE).'</td>';
      }
      $html .= '<td align="center">'.$entry['PermitTag'].'</td>';
      $html .=
	'<td align="center">'.
	decode_entities($entry['FirstName']).' '.
	decode_entities($entry['LastName']).
	'</td>';
      $dbstr = t('Random');
      if (!empty($entry['LevelId']) and !empty($entry['StallId'])) {
	$stall = chdbprop_get_stall($entry['PropertyId'], $entry['LotId'], $entry['LevelId'], $entry['StallId']);
	if (!empty($stall)) {
	  $dbstr = decode_entities($stall['Alias']);
	}
      }
      $html .= '<td align="center">'.$dbstr.'</td>';
      $html .= '<td align="center">'.decode_entities($entry['LPN']).'</td>';
      $dbstr = '--';
      if (!empty($entry['BeginTime'])) {
	$dbtime = chpuser_datetime_utc_to_usertimezone($entry['BeginTime']);
	$dbstr = $dbtime->format("M j,y H:i");
      }
      $html .= '<td align="center">'.$dbstr.'</td>';
      $dbstr = '--';
      if (!empty($entry['ExpiryTime'])) {
	$dbtime = chpuser_datetime_utc_to_usertimezone($entry['ExpiryTime']);
	$dbstr = $dbtime->format("M j,y H:i");
      }
      $html .= '<td align="center">'.$dbstr.'</td>';
      $dbstr = '--';
      if (!empty($entry['CreateTime'])) {
	$dbtime = chpuser_datetime_utc_to_usertimezone($entry['CreateTime']);
	$dbstr = $dbtime->format("M j,y H:i");
      }
      $html .= '<td align="center">'.$dbstr.'</td>';
      if ($entry['Deactivated']) {
	$html .= '<td align="center">'.decode_entities($entry['DeactivationReason']).'</td>';
      } else {
	$html .= '<td align="center"></td>';
      }
      $html .= '</tr>';
    }
    $html .= '</table>';
    return $html;
  } else if ($destination === 'CSV') {
    drupal_set_header('Content-Type:text/csv;charset=utf-8');
    drupal_set_header('Content-Disposition:attachment;filename=report.csv');
    $output = fopen('php://output', 'w');
    $headers = array();
    if ($permit_class === 'ANY') {
      $headers[] = t('Class');
    }
    $headers[] = t('Permit tag');
    $headers[] = t('Name');
    $headers[] = t('Stall');
    $headers[] = t('LPN');
    $headers[] = t('Activation');
    $headers[] = t('Deactivation');
    $headers[] = t('Booked');
    $headers[] = t('Comment');
    fputcsv($output, $headers);
    foreach ($entries as $entry) {
      $row = array();
      if ($permit_class === 'ANY') {
	$row[] = chpperm_registration_name($entry['PermitClass'], FALSE);
      }
      $row[] = $entry['PermitTag'];
      $row[] = decode_entities($entry['FirstName']).' '.decode_entities($entry['LastName']);
      $dbstr = t('Random');
      if (!empty($entry['LevelId']) and !empty($entry['StallId'])) {
	$stall = chdbprop_get_stall($entry['PropertyId'], $entry['LotId'],
				    $entry['LevelId'], $entry['StallId']);
	if (!empty($stall)) {
	  $dbstr = decode_entities($stall['Alias']);
	}
      }
      $row[] = $dbstr;
      $row[] = decode_entities($entry['LPN']);
      $dbstr = '--';
      if (!empty($entry['BeginTime'])) {
	$dbtime = chpuser_datetime_utc_to_usertimezone($entry['BeginTime']);
	$dbstr = $dbtime->format("M j,y H:i");
      }
      $row[] = $dbstr;
      $dbstr = '--';
      if (!empty($entry['ExpiryTime'])) {
	$dbtime = chpuser_datetime_utc_to_usertimezone($entry['ExpiryTime']);
	$dbstr = $dbtime->format("M j,y H:i");
      }
      $row[] = $dbstr;
      $dbstr = '--';
      if (!empty($entry['CreateTime'])) {
	$dbtime = chpuser_datetime_utc_to_usertimezone($entry['CreateTime']);
	$dbstr = $dbtime->format("M j,y H:i");
      }
      $row[] = $dbstr;
      if ($entry['Deactivated']) {
	$row[] = decode_entities($entry['DeactivationReason']);
      } else {
	$row[] = '';
      }
      fputcsv($output, $row);
    }
  }
  return;
}



function chpreport_container_usage($destination,
				   $company_id, $property_id, $lot_id,
				   $start_time, $stop_time,
				   $lot_type, $lot_class,
				   $permit_class) {
  global $user;
  $start = chpuser_datetime_usertimezone_to_utc($start_time);
  $stop = chpuser_datetime_usertimezone_to_utc($stop_time);

  $containers = array();
  foreach (chpperm_retrieve_containers
	   ($user->uid, $company_id, $property_id) as $container_id => $container) {
    $containers[$container_id]['Alias'] = decode_entities($container['data']['Alias']);
    $containers[$container_id]['data'] = array();
  }
  if (!empty($containers)) {
    foreach ($containers as $container_id => $alias) {
      foreach (chpperm_retrieve_container_types
	       ($user->uid, $company_id, $container_id, $property_id) as $type_id => $conttype) {
	$containers[$container_id]['data'][$type_id] = $conttype;
      }
    }
  } //print_r($containers);print('<p>');

  $groupdefs = array();
  foreach (chdbperm_get_groupdefs($company_id, FALSE, FALSE) as $entry) {
    $groupdefs[$entry['GroupId']] = decode_entities($entry['Alias']);
  }

  $entries = array();
  // History comes first
  // Current listings
  foreach (chdbperm_get_container_usage_report
	   ($start->format("Y-m-d H:i:s"),
	    $stop->format("Y-m-d H:i:s"),
	    $company_id, $property_id, $lot_id,
	    $lot_type, $lot_class,
	    $permit_class) as $entry) {
    $entries[$entry['ContainerId']][$entry['TypeId']][$entry['Id']] = $entry;
  }

  if ($destination === 'Browser') {
    $style = "border-bottom:1px solid #b0b0b0;";
    $html .= '<table><tr bgcolor="#e2e2e2" height="35">';
    if ($permit_class === 'ANY') {
      $html .= '<td align="center" style="'.$style.'"><b>'.t('Class').'</b></td>';
    }
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Permit tag').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Name').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Stall').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('LPN').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Activation').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Deactivation').'</b></td>';
    $html .= '</tr>';
    if (empty($entries)) {
      $html .= '</table>';
      return $html;
    }

    $colspan_h = 6 + ($permit_class === 'ANY' ? 1 : 0);
    $colspan_r = 5 + ($permit_class === 'ANY' ? 1 : 0);

    // Rows
    $grand_total = 0;
    foreach ($entries as $container_id => $list1) {
      foreach ($list1 as $type_id => $list2) {
	$type_tag = NULL;
	$type_total = -1;
	$tag =
	  $containers[$container_id]['Alias'].', '.
	  decode_entities($containers[$container_id]['data'][$type_id]['data']['Alias']);
	$html .= '<tr bgcolor="#FFFFFF">';
	$html .= '<td align="left" colspan="'.$colspan_h.'"><b>'.$tag.'</b></td>';
	$html .= '</tr>';
	foreach ($list2 as $entry) {
	  if (NULL === $type_tag) {
	    // First iteration, initiate
	    $group_id = $entry['GroupId'];
	    $member_id = $entry['MemberId'];
	    $type_tag = $group_id.$member_id;
	    $type_total = 0;
	  }
	  if ($type_tag !== $entry['GroupId'].$entry['MemberId']) {
	    // Category has changed, insert a total-row before continue
	    $html .= '<tr bgcolor="#FFFFFF">';
	    $tag = t('Total class permits:');
	    $html .= '<td align="right" colspan="'.$colspan_r.'" class="noborder"><b>'.$tag.'</b></td>';
	    $html .= '<td align="center" class="noborder">'.$type_total.'</td>';
	    $html .= '</tr>';
	    // Reinitiate
	    $group_id = $entry['GroupId'];
	    $member_id = $entry['MemberId'];
	    $type_tag = $group_id.$member_id;
	    $type_total = 0;
	  }
	  $type_total++;
	  $grand_total++;
	  $html .= '<tr bgcolor="#FFFFFF">';
	  if ($permit_class === 'ANY') {
	    $html .= '<td align="center">'.chpperm_registration_name($entry['PermitClass'], FALSE).'</td>';
	  }
	  $html .= '<td align="center">'.$entry['PermitTag'].'</td>';
	  $html .=
	    '<td align="center">'.
	    decode_entities($entry['FirstName']).' '.
	    decode_entities($entry['LastName']).
	    '</td>';
	  $dbstr = t('Random');
	  if (!empty($entry['LevelId']) and !empty($entry['StallId'])) {
	    $stall = chdbprop_get_stall($entry['PropertyId'],
					$entry['LotId'],
					$entry['LevelId'],
					$entry['StallId']);
	    if (!empty($stall)) {
	      $dbstr = decode_entities($stall['Alias']);
	    }
	  }
	  $html .= '<td align="center">'.$dbstr.'</td>';
	  $html .= '<td align="center">'.decode_entities($entry['LPN']).'</td>';
	  $dbstr = '--';
	  if (!empty($entry['BeginTime'])) {
	    $dbtime = chpuser_datetime_utc_to_usertimezone($entry['BeginTime']);
	    $dbstr = $dbtime->format("M j,y H:i");
	  }
	  $html .= '<td align="center">'.$dbstr.'</td>';
	  $dbstr = '--';
	  if (!empty($entry['ExpiryTime'])) {
	    $dbtime = chpuser_datetime_utc_to_usertimezone($entry['ExpiryTime']);
	    $dbstr = $dbtime->format("M j,y H:i");
	  }
	  $html .= '<td align="center">'.$dbstr.'</td>';

	  $html .= '</tr>';
	}
	$html .= '<tr bgcolor="#FFFFFF">';
	$tag = t('Total class permits:');
	$html .= '<td align="right" colspan="'.$colspan_r.'" class="noborder"><b>'.$tag.'</b></td>';
	$html .= '<td align="center" class="noborder">'.$type_total.'</td>';
	$html .= '</tr>';
      }
    }
    if (0 < $grand_total) {
      $html .= '<tr bgcolor="#FFFFFF">';
      $html .= '<td align="right" colspan="'.$colspan_r.'" class="noborder"><b>'.t('Total permits:').'</b></td>';
      $html .= '<td align="center" class="noborder">'.$grand_total.'</td>';
      $html .= '</tr>';
    }
    $html .= '</table>';
    return $html;
  } else if ($destination === 'CSV') {
    drupal_set_header('Content-Type:text/csv;charset=utf-8');
    drupal_set_header('Content-Disposition:attachment;filename=report.csv');
    $output = fopen('php://output', 'w');
    $headers = array();
    if ($permit_class === 'ANY') {
      $headers[] = t('Class');
    }
    $headers[] = t('Permit tag');
    $headers[] = t('Name');
    $headers[] = t('Stall');
    $headers[] = t('LPN');
    $headers[] = t('Activation');
    $headers[] = t('Deactivation');
    fputcsv($output, $headers);
    foreach ($entries as $container_id => $list1) {
      foreach ($list1 as $type_id => $list2) {
	$type_tag = NULL;
	$type_total = -1;
	$tag =
	  $containers[$container_id]['Alias'].', '.
	  decode_entities($containers[$container_id]['data'][$type_id]['data']['Alias']);
	$del = array();
	$del[] = $tag;
	fputcsv($output, $del);
	foreach ($list2 as $entry) {
	  $row = array();
	  if (NULL === $type_tag) {
	    // First iteration, initiate
	    $group_id = $entry['GroupId'];
	    $member_id = $entry['MemberId'];
	    $type_tag = $group_id.$member_id;
	    $type_total = 0;
	  }
	  if ($type_tag !== $entry['GroupId'].$entry['MemberId']) {
	    // Category has changed, insert a total-row before continue
	    $tag = t('Total class permits:');
	    $del = array();
	    $del[] = $tag;
	    $del[] = $type_total;
	    fputcsv($output, $del);
	    // Reinitiate
	    $group_id = $entry['GroupId'];
	    $member_id = $entry['MemberId'];
	    $type_tag = $group_id.$member_id;
	    $type_total = 0;
	  }
	  $type_total++;
	  $grand_total++;
	  if ($permit_class === 'ANY') {
	    $row[] = chpperm_registration_name($entry['PermitClass'], FALSE);
	  }
	  $row[] = $entry['PermitTag'];
	  $row[] = decode_entities($entry['FirstName']).' '.decode_entities($entry['LastName']);
	  $dbstr = t('Random');
	  if (!empty($entry['LevelId']) and !empty($entry['StallId'])) {
	    $stall = chdbprop_get_stall($entry['PropertyId'], $entry['LotId'],
					$entry['LevelId'], $entry['StallId']);
	    if (!empty($stall)) {
	      $dbstr = decode_entities($stall['Alias']);
	    }
	  }
	  $row[] = $dbstr;
	  $row[] = decode_entities($entry['LPN']);
	  $dbstr = '--';
	  if (!empty($entry['BeginTime'])) {
	    $dbtime = chpuser_datetime_utc_to_usertimezone($entry['BeginTime']);
	    $dbstr = $dbtime->format("M j,y H:i");
	  }
	  $row[] = $dbstr;
	  $dbstr = '--';
	  if (!empty($entry['ExpiryTime'])) {
	    $dbtime = chpuser_datetime_utc_to_usertimezone($entry['ExpiryTime']);
	    $dbstr = $dbtime->format("M j,y H:i");
	  }
	  $row[] = $dbstr;
	  fputcsv($output, $row);
	}
	$tag = t('Total class permits:');
	$del = array();
	$del[] = $tag;
	$del[] = $type_total;
	fputcsv($output, $del);
      }
      if (0 < $grand_total) {
	$tag = t('Total permits:');
	$del = array();
	$del[] = $tag;
	$del[] = $grand_total;
	fputcsv($output, $del);
      }
    }
  }
  return;
}

