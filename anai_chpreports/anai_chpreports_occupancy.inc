<?php



function chpreport_occu_group($destination,
			      $company_id, $property_id, $lot_id, $group_id,
			      $start_time, $stop_time,
			      $lot_type, $lot_class,
			      $validation, $compliancy) {
  $start = chpuser_datetime_usertimezone_to_utc($start_time);
  $stop = chpuser_datetime_usertimezone_to_utc($stop_time);

  global $user;
  $account = user_load(array('uid' => $user->uid));
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    $user_is_admin = TRUE;
  } else {
    $user_is_admin = FALSE;
  }

  if ($validation === 'Group') {
    $loc_group_id = $group_id;
    $loc_member_id = 'ANY';
  } elseif ($validation === 'Member') {
    $loc_group_id = 'ANY';
    $loc_member_id = $group_id;
  } else {
    $loc_group_id = 'ERROR';
    $loc_member_id = 'ERROR';
  }

  $entries = array();
  $validations = array();
  if ($validation !== 'ANY') {
    $validations[] = $validation;
  }

  // History comes first

  // Current listings
  $entries =
    array_merge($entries,
  		chdboccu_get_occupancy_report
		($user_is_admin,
		 $start->format("Y-m-d H:i:s"),
		 $stop->format("Y-m-d H:i:s"),
		 $company_id, $property_id, $lot_id, $loc_group_id, $loc_member_id,
		 $lot_type, $lot_class,
		 $validations, $compliancy));
  //print('<p><p>');var_export($entries);

  $expanded = FALSE;

  $list = array();
  foreach ($entries as $entry) {
    if ($expanded) {
      $list[$entry['LPN']][$entry['EventId']][] = $entry;
    } elseif (!isset($list[$entry['LPN']][$entry['EventId']])) {
      $list[$entry['LPN']][$entry['EventId']][0] = $entry;
    } elseif ($entry['History'] < $list[$entry['LPN']][$entry['EventId']][0]['History']) {
      foreach ($entry as $key => $value) {
	if ($key === 'LPN' or $key === 'EventId') {
	  continue;
	}
	if (0 < strlen($value)) {
	  $list[$entry['LPN']][$entry['EventId']][0][$key] = $value;
	}
      }
    }
  }

  if ($destination === 'Browser') {
    $style = "border-bottom:1px solid #b0b0b0;";
    $html .= '<table><tr bgcolor="#e2e2e2" height="35">';
    if ($group_id === 'ANY') {
      $html .= '<td align="center" style="'.$style.'"><b>'.t('Account').'</b></td>';
    }
    $html .= '<td align="center" style="'.$style.'"><b>'.t('LPN').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Source').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Entry time').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Exit time').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Length of stay').'</b></td>';
    $html .= '</tr>';

    foreach ($list as $lpn => $events) {
      foreach ($events as $event_id => $entries) {
	foreach ($entries as $entry) {
	  $html .= '<tr bgcolor="#FFFFFF">';
	  if ($group_id === 'ANY' and $validation === 'Group') {
	    if (!isset($cache[$entry['AllGroupId']])) {
	      $cache[$entry['AllGroupId']] = chdbperm_get_groupdef($company_id, $entry['AllGroupId']);
	    }
	    $html .= '<td align="center">'.decode_entities($cache[$entry['AllGroupId']]['Alias']).'</td>';
	  } elseif ($group_id === 'ANY' and $validation === 'Member') {
	    if (!isset($cache[$entry['AllMemberId']])) {
	      $cache[$entry['AllMemberId']] = chdbperm_get_groupdef($company_id, $entry['AllMemberId']);
	    }
	    $html .= '<td align="center">'.decode_entities($cache[$entry['AllMemberId']]['Alias']).'</td>';
	  }
	  $html .= '<td align="center">'.$entry['LPN'].'</td>';
	  $html .= '<td align="center">'.$entry['Source'].'</td>';
	  $time1 = chpuser_datetime_utc_to_usertimezone($entry['InTime']);
	  $html .= '<td align="center">'.$time1->format("M j,y H:i:s").'</td>';
	  if (!empty($entry['AllOutTime'])) {
	    $time2 = chpuser_datetime_utc_to_usertimezone($entry['AllOutTime']);
	    $html .= '<td align="center">'.$time2->format("M j,y H:i:s").'</td>';
	  } else {
	    $html .= '<td align="center"></td>';
	  }
	  if (!empty($entry['AllOutTime'])) {
	    $interval = $time2->diff($time1);
	    $html .= '<td align="center">'.t('DD days, HH hours, MM minutes',
					     array('DD' => $interval->days,
						   'HH' => $interval->h,
						   'MM' => $interval->i)).'</td>';
	  } else {
	    $html .= '<td align="center"></td>';
	  }
	  $html .= '</tr>';
	}
      }
    }
    $html .= '</table>';
    return $html;
  } else if ($destination === 'CSV') {
    drupal_set_header('Content-Type:text/csv;charset=utf-8');
    drupal_set_header('Content-Disposition:attachment;filename=report.csv');
    $output = fopen('php://output', 'w');
    $headers = array();
    if ($group_id === 'ANY') {
      $headers[] = t('Account');
    }
    $headers[] = t('LPN');
    $headers[] = t('Source');
    $headers[] = t('Entry time');
    $headers[] = t('Exit time');
    $headers[] = t('Length of stay');
    fputcsv($output, $headers);
    foreach ($list as $lpn => $events) {
      foreach ($events as $event_id => $entries) {
	foreach ($entries as $entry) {
	  $row = array();
	  if ($group_id === 'ANY' and $validation === 'Group') {
	    if (!isset($cache[$entry['AllGroupId']])) {
	      $cache[$entry['AllGroupId']] = chdbperm_get_groupdef($company_id, $entry['AllGroupId']);
	    }
	    $row[] = decode_entities($cache[$entry['AllGroupId']]['Alias']);
	  } elseif ($group_id === 'ANY' and $validation === 'Member') {
	    if (!isset($cache[$entry['AllMemberId']])) {
	      $cache[$entry['AllMemberId']] = chdbperm_get_groupdef($company_id, $entry['AllMemberId']);
	    }
	    $row[] = decode_entities($cache[$entry['AllMemberId']]['Alias']);
	  }
	  $row[] = $entry['LPN'];
	  $row[] = $entry['Source'];
	  $time1 = chpuser_datetime_utc_to_usertimezone($entry['InTime']);
	  $row[] = $time1->format("M j,y H:i:s");
	  if (!empty($entry['AllOutTime'])) {
	    $time2 = chpuser_datetime_utc_to_usertimezone($entry['AllOutTime']);
	    $row[] = $time2->format("M j,y H:i:s");
	  } else {
	    $row[] = '';
	  }
	  if (!empty($entry['AllOutTime'])) {
	    $interval = $time2->diff($time1);
	    $row[] = t('DD days, HH hours, MM minutes',
		       array('DD' => $interval->days,
			     'HH' => $interval->h,
			     'MM' => $interval->i));
	  } else {
	    $row[] = '';
	  }
	  fputcsv($output, $row);
	}
      }
    }
  }
  return;
}



function chpreport_occu_random($destination,
			       $company_id, $property_id, $lot_id,
			       $start_time, $stop_time,
			       $lot_type, $lot_class,
			       $validations, $compliancy) {
  $start = chpuser_datetime_usertimezone_to_utc($start_time);
  $stop = chpuser_datetime_usertimezone_to_utc($stop_time);

  global $user;
  $account = user_load(array('uid' => $user->uid));
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    $user_is_admin = TRUE;
  } else {
    $user_is_admin = FALSE;
  }

  $entries = array();

  // History comes first

  // Current listings
  $entries =
    array_merge($entries,
  		chdboccu_get_occupancy_report
		($user_is_admin,
		 $start->format("Y-m-d H:i:s"),
		 $stop->format("Y-m-d H:i:s"),
		 $company_id, $property_id, $lot_id, 'ANY', 'ANY',
		 $lot_type, $lot_class,
		 $validations, $compliancy));
  //print('<p><p>');var_export($entries);

  $expanded = FALSE;

  $list = array();
  foreach ($entries as $entry) {
    if ($expanded) {
      $list[$entry['LPN']][$entry['EventId']][] = $entry;
    } elseif (!isset($list[$entry['LPN']][$entry['EventId']])) {
      $list[$entry['LPN']][$entry['EventId']][0] = $entry;
    } elseif ($entry['History'] < $list[$entry['LPN']][$entry['EventId']][0]['History']) {
      foreach ($entry as $key => $value) {
	if ($key === 'LPN' or $key === 'EventId') {
	  continue;
	}
	if (0 < strlen($value)) {
	  $list[$entry['LPN']][$entry['EventId']][0][$key] = $value;
	}
      }
    }
  }

  if ($destination === 'Browser') {
    $style = "border-bottom:1px solid #b0b0b0;";
    $html .= '<table><tr bgcolor="#e2e2e2" height="35">';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Type').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('LPN').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Source').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Entry time').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Exit time').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Length of stay').'</b></td>';
    $html .= '</tr>';

    foreach ($list as $lpn => $events) {
      foreach ($events as $event_id => $entries) {
	foreach ($entries as $entry) {
	  $html .= '<tr bgcolor="#FFFFFF">';
	  $html .= '<td align="center">'.$entry['AllValidation'].'</td>';
	  $html .= '<td align="center">'.$entry['LPN'].'</td>';
	  $html .= '<td align="center">'.$entry['Source'].'</td>';
	  $time1 = chpuser_datetime_utc_to_usertimezone($entry['InTime']);
	  $html .= '<td align="center">'.$time1->format("M j,y H:i:s").'</td>';
	  if (!empty($entry['AllOutTime'])) {
	    $time2 = chpuser_datetime_utc_to_usertimezone($entry['AllOutTime']);
	    $html .= '<td align="center">'.$time2->format("M j,y H:i:s").'</td>';
	  } else {
	    $html .= '<td align="center"></td>';
	  }
	  if (!empty($entry['AllOutTime'])) {
	    $interval = $time2->diff($time1);
	    $html .= '<td align="center">'.t('DD days, HH hours, MM minutes',
					     array('DD' => $interval->days,
						   'HH' => $interval->h,
						   'MM' => $interval->i)).'</td>';
	  } else {
	    $html .= '<td align="center"></td>';
	  }
	  $html .= '</tr>';
	}
      }
    }
    $html .= '</table>';
    return $html;
  } else if ($destination === 'CSV') {
    drupal_set_header('Content-Type:text/csv;charset=utf-8');
    drupal_set_header('Content-Disposition:attachment;filename=report.csv');
    $output = fopen('php://output', 'w');
    $headers = array();
    if ($group_id === 'ANY') {
      $headers[] = t('Account');
    }
    $headers[] = t('LPN');
    $headers[] = t('Source');
    $headers[] = t('Entry time');
    $headers[] = t('Exit time');
    $headers[] = t('Length of stay');
    fputcsv($output, $headers);
    foreach ($list as $lpn => $events) {
      foreach ($events as $event_id => $entries) {
	foreach ($entries as $entry) {
	  $row = array();
	  $row[] = $entry['AllValidation'];
	  $row[] = $entry['LPN'];
	  $row[] = $entry['Source'];
	  $time1 = chpuser_datetime_utc_to_usertimezone($entry['InTime']);
	  $row[] = $time1->format("M j,y H:i:s");
	  if (!empty($entry['AllOutTime'])) {
	    $time2 = chpuser_datetime_utc_to_usertimezone($entry['AllOutTime']);
	    $row[] = $time2->format("M j,y H:i:s");
	  } else {
	    $row[] = '';
	  }
	  if (!empty($entry['AllOutTime'])) {
	    $interval = $time2->diff($time1);
	    $row[] = t('DD days, HH hours, MM minutes',
		       array('DD' => $interval->days,
			     'HH' => $interval->h,
			     'MM' => $interval->i));
	  } else {
	    $row[] = '';
	  }
	  fputcsv($output, $row);
	}
      }
    }
  }
  return;
}



function chpreport_occu_compliancy($destination,
				   $company_id, $property_id, $lot_id,
				   $start_time, $stop_time,
				   $lot_type, $lot_class,
				   $validation, $compliancy) {
  $start = chpuser_datetime_usertimezone_to_utc($start_time);
  $stop = chpuser_datetime_usertimezone_to_utc($stop_time);

  global $user;
  $account = user_load(array('uid' => $user->uid));
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    $user_is_admin = TRUE;
  } else {
    $user_is_admin = FALSE;
  }

  $entries = array();
  $validations = array();
  if ($validation !== 'ANY') {
    $validations[] = $validation;
  }

  // History comes first

  // Current listings
  $entries =
    array_merge($entries,
  		chdboccu_get_occupancy_report
		($user_is_admin,
		 $start->format("Y-m-d H:i:s"),
		 $stop->format("Y-m-d H:i:s"),
		 $company_id, $property_id, $lot_id, 'ANY', 'ANY',
		 $lot_type, $lot_class,
		 $validations, $compliancy));
  //print('<p><p>');var_export($entries);

  $expanded = FALSE;

  $list = array();
  foreach ($entries as $entry) {
    if ($expanded) {
      $list[$entry['LPN']][$entry['EventId']][] = $entry;
    } elseif (!isset($list[$entry['LPN']][$entry['EventId']])) {
      $list[$entry['LPN']][$entry['EventId']][0] = $entry;
    } elseif ($entry['History'] < $list[$entry['LPN']][$entry['EventId']][0]['History']) {
      foreach ($entry as $key => $value) {
	if ($key === 'LPN' or $key === 'EventId') {
	  continue;
	}
	if (0 < strlen($value)) {
	  $list[$entry['LPN']][$entry['EventId']][0][$key] = $value;
	}
      }
    }
  }

  if ($destination === 'Browser') {
    $style = "border-bottom:1px solid #b0b0b0;";
    $html .= '<table><tr bgcolor="#e2e2e2" height="35">';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('LPN').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Source').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Entry time').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Exit time').'</b></td>';
    if ($compliancy === 'ANY') {
      $html .= '<td align="center" style="'.$style.'"><b>'.t('Compliancy').'</b></td>';
    }
    if ($compliancy === 'ANY' or $compliancy === 'Compliant') {
      $html .= '<td align="center" style="'.$style.'"><b>'.t('Validated by').'</b></td>';
    }
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Length of stay').'</b></td>';
    $html .= '</tr>';

    foreach ($list as $lpn => $events) {
      foreach ($events as $event_id => $entries) {
	foreach ($entries as $entry) {
	  $html .= '<tr bgcolor="#FFFFFF">';
	  $html .= '<td align="center">'.$entry['LPN'].'</td>';
	  $html .= '<td align="center">'.$entry['Source'].'</td>';
	  $time1 = chpuser_datetime_utc_to_usertimezone($entry['InTime']);
	  $html .= '<td align="center">'.$time1->format("M j,y H:i:s").'</td>';
	  if (!empty($entry['AllOutTime'])) {
	    $time2 = chpuser_datetime_utc_to_usertimezone($entry['AllOutTime']);
	    $html .= '<td align="center">'.$time2->format("M j,y H:i:s").'</td>';
	  } else {
	    $html .= '<td align="center"></td>';
	  }
	  if ($compliancy === 'ANY') {
	    if ($entry['AllCompliant']) {
	      $html .= '<td align="center">'.chpreport_get_compliancy_name('Compliant').'</td>';
	    } elseif (!$entry['AllCompliant'] and !$entry['AllPending']) {
	      $html .= '<td align="center">'.chpreport_get_compliancy_name('NonCompliant').'</td>';
	    } else {
	      $html .= '<td align="center">'.chpreport_get_compliancy_name('GraceExempt').'</td>';
	    }
	  }
	  if ($compliancy === 'ANY' or $compliancy === 'Compliant') {
	    $html .= '<td align="center">'.$entry['Validation'].'</td>';
	  }
	  if (!empty($entry['AllOutTime'])) {
	    $interval = $time2->diff($time1);
	    $html .= '<td align="center">'.t('DD days, HH hours, MM minutes',
					     array('DD' => $interval->days,
						   'HH' => $interval->h,
						   'MM' => $interval->i)).'</td>';
	  } else {
	    $html .= '<td align="center"></td>';
	  }
	  $html .= '</tr>';
	}
      }
    }
    $html .= '</table>';
    return $html;
  } else if ($destination === 'CSV') {
    drupal_set_header('Content-Type:text/csv;charset=utf-8');
    drupal_set_header('Content-Disposition:attachment;filename=report.csv');
    $output = fopen('php://output', 'w');
    $headers = array();
    $headers[] = t('LPN');
    $headers[] = t('Source');
    $headers[] = t('Entry time');
    $headers[] = t('Exit time');
    if ($compliancy === 'ANY') {
      $headers[] = t('Compliancy');
    }
    if ($compliancy === 'ANY' or $compliancy === 'Compliant') {
      $headers[] = t('Validated by');
    }
    $headers[] = t('Length of stay');
    fputcsv($output, $headers);
    foreach ($list as $lpn => $events) {
      foreach ($events as $event_id => $entries) {
	foreach ($entries as $entry) {
	  $row = array();
	  $row[] = $entry['LPN'];
	  $row[] = $entry['Source'];
	  $time1 = chpuser_datetime_utc_to_usertimezone($entry['InTime']);
	  $row[] = $time1->format("M j,y H:i:s");
	  if (!empty($entry['AllOutTime'])) {
	    $time2 = chpuser_datetime_utc_to_usertimezone($entry['AllOutTime']);
	    $row[] = $time2->format("M j,y H:i:s");
	  } else {
	    $row[] = '';
	  }
	  if ($compliancy === 'ANY') {
	    if ($entry['AllCompliant']) {
	      $row[] = chpreport_get_compliancy_name('Compliant');
	    } elseif (!$entry['AllCompliant'] and !$entry['AllPending']) {
	      $row[] = chpreport_get_compliancy_name('NonCompliant');
	    } else {
	      $row[] = chpreport_get_compliancy_name('GraceExempt');
	    }
	  }
	  if ($compliancy === 'ANY' or $compliancy === 'Compliant') {
	    $row[] = $entry['Validation'];
	  }
	  if (!empty($entry['AllOutTime'])) {
	    $interval = $time2->diff($time1);
	    $row[] = t('DD days, HH hours, MM minutes',
		       array('DD' => $interval->days,
			     'HH' => $interval->h,
			     'MM' => $interval->i));
	  } else {
	    $row[] = '';
	  }
	  fputcsv($output, $row);
	}
      }
    }
  }
  return;
}

