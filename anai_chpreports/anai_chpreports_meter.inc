<?php



function chpreport_meter_cash($destination,
			      $company_id, $property_id, $lot_id,
			      $start_time, $stop_time,
			      $lot_type, $lot_class,
			      $agent_id) {
  $start = chpuser_datetime_usertimezone_to_utc($start_time);
  $stop = chpuser_datetime_usertimezone_to_utc($stop_time);

  // Retrieve meters to include in report request
  $meters = array();

  if ($agent_id === 'ANY') {
    foreach (chdbccpark_get_systems($company_id, $property_id, $lot_id,
				    $lot_type, $lot_class) as $entry) {
      $meters[$entry['AgentId']] = $entry;
    }
  } else {
    $meters[$agent_id] = $agent_id;
  }

  // Retrieve meter settings so correct columns are included
  $pbs = FALSE;
  $pbl = TRUE;
  $merchant_codes = FALSE;

  // Retrieve the repert entries
  $entries = array();

  // History comes first

  // Current listings
  $entries =
    array_merge($entries,
  		chdbccpark_cc_get_cash_report
		($start->format("Y-m-d H:i:s"),
		 $stop->format("Y-m-d H:i:s"),
		 array_keys($meters)));

  if ($destination === 'Browser') {
    $style = "border-bottom:1px solid #b0b0b0;";
    $html .= '<table><tr bgcolor="#e2e2e2" height="35">';
    if ($agent_id === 'ANY') {
      $html .= '<td align="center" style="'.$style.'"><b>'.t('Meter').'</b></td>';
    }
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Time').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Ticket').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Amount').'</b></td>';
    if ($pbl) {
      $html .= '<td align="center" style="'.$style.'"><b>'.t('LPN').'</b></td>';
    }
    if ($pbs) {
      $html .= '<td align="center" style="'.$style.'"><b>'.t('Stall').'</b></td>';
    }
    if ($merchant_codes) {
      $html .= '<td align="center" style="'.$style.'"><b>'.t('Merchant code').'</b></td>';
    }
    $html .= '</tr>';
    foreach ($entries as $entry) {
      $html .= '<tr bgcolor="#FFFFFF">';
      if ($agent_id === 'ANY') {
	$html .= '<td align="center">'.decode_entities($meters[$entry['AgentId']]['Alias']).'</td>';
      }
      $time = chpuser_datetime_utc_to_usertimezone($entry['TransactionTime']);
      $html .= '<td align="center">'.$time->format("M j,y H:i:s").'</td>';
      $html .= '<td align="center">'.$entry['TicketNo'].'</td>';
      $html .= '<td align="center">'.$entry['Amount'].'</td>';
      if ($pbl) {
	$html .= '<td align="center">'.$entry['LPN'].'</td>';
      }
      if ($pbs) {
	$html .= '<td align="center">'.$entry['Stall'].'</td>';
      }
      if ($merchant_codes) {
	$html .= '<td align="center">'.$entry['MerchantCode'].'</td>';
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
    if ($agent_id === 'ANY') {
      $headers[] = t('Meter');
    }
    $headers[] = t('Time');
    $headers[] = t('Ticket');
    $headers[] = t('Amount');
    if ($pbl) {
      $headers[] = t('LPN');
    }
    if ($pbs) {
      $headers[] = t('Stall');
    }
    if ($merchant_codes) {
      $headers[] = t('Merchant code');
    }
    fputcsv($output, $headers);
    foreach ($entries as $entry) {
      $row = array();
      if ($agent_id === 'ANY') {
	$row[] = decode_entities($meters[$entry['AgentId']]['Alias']);
      }
      $time = chpuser_datetime_utc_to_usertimezone($entry['TransactionTime']);
      $row[] = $time->format("M j,y H:i:s");
      $row[] = $entry['TicketNo'];
      $row[] = $entry['Amount'];
      if ($pbl) {
	$row[] = $entry['LPN'];
      }
      if ($pbs) {
	$row[] = $entry['Stall'];
      }
      if ($merchant_codes) {
	$row[] = $entry['MerchantCode'];
      }
      fputcsv($output, $row);
    }
  }
  return;
}



function chpreport_meter_creditcard($destination,
				    $company_id, $property_id, $lot_id,
				    $start_time, $stop_time,
				    $lot_type, $lot_class,
				    $agent_id) {
  $start = chpuser_datetime_usertimezone_to_utc($start_time);
  $stop = chpuser_datetime_usertimezone_to_utc($stop_time);

  // Retrieve meters to include in report request
  $meters = array();

  if ($agent_id === 'ANY') {
    foreach (chdbccpark_get_systems($company_id, $property_id, $lot_id,
				    $lot_type, $lot_class) as $entry) {
      $meters[$entry['AgentId']] = $entry;
    }
  } else {
    $meters[$agent_id] = $agent_id;
  }

  // Retrieve meter settings so correct columns are included
  $pbs = FALSE;
  $pbl = TRUE;
  $merchant_codes = FALSE;

  // Retrieve the repert entries
  $entries = array();

  // History comes first

  // Current listings
  $entries =
    array_merge($entries,
  		chdbccpark_cc_get_creditcard_report
		($start->format("Y-m-d H:i:s"),
		 $stop->format("Y-m-d H:i:s"),
		 array_keys($meters)));

  if ($destination === 'Browser') {
    $style = "border-bottom:1px solid #b0b0b0;";
    $html .= '<table><tr bgcolor="#e2e2e2" height="35">';
    if ($agent_id === 'ANY') {
      $html .= '<td align="center" style="'.$style.'"><b>'.t('Meter').'</b></td>';
    }
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Time').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Ticket').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Transaction').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Card type').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Card').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Amount').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Completed').'</b></td>';
    if ($pbl) {
      $html .= '<td align="center" style="'.$style.'"><b>'.t('LPN').'</b></td>';
    }
    if ($pbs) {
      $html .= '<td align="center" style="'.$style.'"><b>'.t('Stall').'</b></td>';
    }
    if ($merchant_codes) {
      $html .= '<td align="center" style="'.$style.'"><b>'.t('Merchant code').'</b></td>';
    }
    $html .= '</tr>';
    foreach ($entries as $entry) {
      $html .= '<tr bgcolor="#FFFFFF">';
      if ($agent_id === 'ANY') {
	$html .= '<td align="center">'.decode_entities($meters[$entry['AgentId']]['Alias']).'</td>';
      }
      $time = chpuser_datetime_utc_to_usertimezone($entry['TransactionTime']);
      $html .= '<td align="center">'.$time->format("M j,y H:i:s").'</td>';
      $html .= '<td align="center">'.$entry['TicketNo'].'</td>';
      $html .= '<td align="center">'.$entry['TransactionId'].'</td>';
      $html .= '<td align="center">'.$entry['CardType'].'</td>';
      $html .= '<td align="center">'.$entry['Card'].'</td>';
      $html .= '<td align="center">'.$entry['Amount'].'</td>';
      $html .= '<td align="center">'.($entry['Completed'] ? t('Yes') : t('No')).'</td>';
      if ($pbl) {
	$html .= '<td align="center">'.$entry['LPN'].'</td>';
      }
      if ($pbs) {
	$html .= '<td align="center">'.$entry['Stall'].'</td>';
      }
      if ($merchant_codes) {
	$html .= '<td align="center">'.$entry['MerchantCode'].'</td>';
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
    if ($agent_id === 'ANY') {
      $headers[] = t('Meter');
    }
    $headers[] = t('Time');
    $headers[] = t('Ticket');
    $headers[] = t('Transaction');
    $headers[] = t('Card type');
    $headers[] = t('Card');
    $headers[] = t('Amount');
    $headers[] = t('Completed');
    if ($pbl) {
      $headers[] = t('LPN');
    }
    if ($pbs) {
      $headers[] = t('Stall');
    }
    if ($merchant_codes) {
      $headers[] = t('Merchant code');
    }
    fputcsv($output, $headers);
    foreach ($entries as $entry) {
      $row = array();
      if ($agent_id === 'ANY') {
	$row[] = decode_entities($meters[$entry['AgentId']]['Alias']);
      }
      $time = chpuser_datetime_utc_to_usertimezone($entry['TransactionTime']);
      $row[] = $time->format("M j,y H:i:s");
      $row[] = $entry['TicketNo'];
      $row[] = $entry['TransactionId'];
      $row[] = $entry['CardType'];
      $row[] = $entry['Card'];
      $row[] = $entry['Amount'];
      $row[] = $entry['Completed'] ? t('Yes') : t('No');
      if ($pbl) {
	$row[] = $entry['LPN'];
      }
      if ($pbs) {
	$row[] = $entry['Stall'];
      }
      if ($merchant_codes) {
	$row[] = $entry['MerchantCode'];
      }
      fputcsv($output, $row);
    }
  }
  return;
}



function chpreport_meter_revenue($destination,
				 $company_id, $property_id, $lot_id,
				 $start_time, $stop_time,
				 $lot_type, $lot_class,
				 $agent_id) {
  $start = chpuser_datetime_usertimezone_to_utc($start_time);
  $stop = chpuser_datetime_usertimezone_to_utc($stop_time);

  // Retrieve meters to include in report request
  $meters = array();

  if ($agent_id === 'ANY') {
    foreach (chdbccpark_get_systems($company_id, $property_id, $lot_id,
				    $lot_type, $lot_class) as $entry) {
      $meters[$entry['AgentId']] = $entry;
    }
  } else {
    $meters[$agent_id] = $agent_id;
  }

  // Retrieve meter settings so correct columns are included
  $pbs = FALSE;
  $pbl = TRUE;
  $merchant_codes = FALSE;

  // Retrieve the repert entries
  $entries = array();

  // History comes first

  // Current listings
  $entries =
    array_merge($entries,
  		chdbccpark_cc_get_purchase_report
		($start->format("Y-m-d H:i:s"),
		 $stop->format("Y-m-d H:i:s"),
		 array_keys($meters)));

  if ($destination === 'Browser') {
    $style = "border-bottom:1px solid #b0b0b0;";
    $html .= '<table><tr bgcolor="#e2e2e2" height="35">';
    if ($agent_id === 'ANY') {
      $html .= '<td align="center" style="'.$style.'"><b>'.t('Meter').'</b></td>';
    }
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Time').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Ticket').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Transaction').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Card type').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Card').'</b></td>';
    if ($merchant_codes) {
      $html .= '<td align="center" style="'.$style.'"><b>'.t('Merchant code').'</b></td>';
    }
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Completed').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Amount').'</b></td>';
    $html .= '</tr>';
    $grand_total = 0.0;
    $day_total = 0.0;
    $day = NULL;
    $colspan = 6 + ($agent_id === 'ANY' ? 1 : 0) + ($merchant_codes ? 1 : 0);
    foreach ($entries as $entry) {
      $time = chpuser_datetime_utc_to_usertimezone($entry['TransactionTime']);
      if ($day !== $time->format('Y-m-d')) {
	if (NULL !== $day) {
	  $html .= '<tr bgcolor="#FFFFFF">';
	  $html .= '<td align="right" colspan="'.$colspan.'" class="noborder"><b>'.t('Day total:').'</b></td>';
	  $html .= '<td align="center" class="noborder">'.sprintf("%0.02f", $day_total).'</td>';
	  $html .= '</tr>';
	}
	$day = $time->format('Y-m-d');
	$day_total = 0.0;
      }
      $grand_total += $entry['Amount'];
      $day_total += $entry['Amount'];
      $html .= '<tr bgcolor="#FFFFFF">';
      if ($agent_id === 'ANY') {
	$html .= '<td align="center">'.decode_entities($meters[$entry['AgentId']]['Alias']).'</td>';
      }
      $html .= '<td align="center">'.$time->format("M j,y H:i").'</td>';
      $html .= '<td align="center">'.$entry['TicketNo'].'</td>';
      $html .= '<td align="center">'.$entry['TransactionId'].'</td>';
      $html .= '<td align="center">'.$entry['CardType'].'</td>';
      $html .= '<td align="center">'.$entry['Card'].'</td>';
      if ($merchant_codes) {
	$html .= '<td align="center">'.$entry['MerchantCode'].'</td>';
      }
      $html .= '<td align="center">'.((!isset($entry['Completed']) or
				       (isset($entry['Completed']) and $entry['Completed'])) ? t('Yes') : t('No')).'</td>';
      $html .= '<td align="center">'.$entry['Amount'].'</td>';
      $html .= '</tr>';
    }
    if (0 < $grand_total) {
      $html .= '<tr bgcolor="#FFFFFF">';
      $html .= '<td align="right" colspan="'.$colspan.'" class="noborder"><b>'.t('Day total:').'</b></td>';
      $html .= '<td align="center" class="noborder">'.sprintf("%0.02f", $day_total).'</td>';
      $html .= '</tr>';
      $html .= '<tr bgcolor="#FFFFFF">';
      $html .= '<td align="right" colspan="'.$colspan.'" class="noborder"><b>'.t('Grand total:').'</b></td>';
      $html .= '<td align="center" class="noborder">'.sprintf("%0.02f", $grand_total).'</td>';
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
      $headers[] = t('Meter');
    }
    $headers[] = t('Time');
    $headers[] = t('Ticket');
    $headers[] = t('Transaction');
    $headers[] = t('Card type');
    $headers[] = t('Card');
    if ($merchant_codes) {
      $headers[] = t('Merchant code');
    }
    $headers[] = t('Completed');
    $headers[] = t('Amount');
    fputcsv($output, $headers);
    foreach ($entries as $entry) {
      $row = array();
      if ($agent_id === 'ANY') {
	$row[] = decode_entities($meters[$entry['AgentId']]['Alias']);
      }
      $time = chpuser_datetime_utc_to_usertimezone($entry['TransactionTime']);
      $row[] = $time->format("M j,y H:i:s");
      $row[] = $entry['TicketNo'];
      $row[] = $entry['TransactionId'];
      $row[] = $entry['CardType'];
      $row[] = $entry['Card'];
      if ($merchant_codes) {
	$row[] = $entry['MerchantCode'];
      }
      $row[] = (!isset($entry['Completed']) or
		(isset($entry['Completed']) and $entry['Completed'])) ? t('Yes') : t('No');
      $row[] = sprintf("%0.02f", $entry['Amount']);
      fputcsv($output, $row);
    }
  }
  return;
}



function chpreport_meter_merchantcode($destination,
				      $company_id, $property_id, $lot_id,
				      $start_time, $stop_time,
				      $lot_type, $lot_class,
				      $agent_id,
				      $merchant_code) {
  $start = chpuser_datetime_usertimezone_to_utc($start_time);
  $stop = chpuser_datetime_usertimezone_to_utc($stop_time);

  // Retrieve meters to include in report request
  $meters = array();

  if ($agent_id === 'ANY') {
    foreach (chdbccpark_get_systems($company_id, $property_id, $lot_id,
				    $lot_type, $lot_class) as $entry) {
      $meters[$entry['AgentId']] = $entry;
    }
  } else {
    $meters[$agent_id] = $agent_id;
  }

  // Retrieve the repert entries
  $entries = array();

  // History comes first

  // Current listings
  $entries =
    array_merge($entries,
  		chdbccpark_cc_get_merchantcode_report
		($start->format("Y-m-d H:i:s"),
		 $stop->format("Y-m-d H:i:s"),
		 $merchant_code,
		 array_keys($meters)));

  if ($destination === 'Browser') {
    $style = "border-bottom:1px solid #b0b0b0;";
    $html .= '<table><tr bgcolor="#e2e2e2" height="35">';
    if ($agent_id === 'ANY') {
      $html .= '<td align="center" style="'.$style.'"><b>'.t('Meter').'</b></td>';
    }
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Time').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Ticket').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Transaction').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Card type').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Card').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Merchant code').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Completed').'</b></td>';
    $html .= '<td align="center" style="'.$style.'"><b>'.t('Amount').'</b></td>';
    $html .= '</tr>';
    $grand_total = 0.0;
    $day_total = 0.0;
    $day = NULL;
    $colspan = 6 + ($agent_id === 'ANY' ? 1 : 0) + ($merchant_codes ? 1 : 0);
    foreach ($entries as $entry) {
      $time = chpuser_datetime_utc_to_usertimezone($entry['TransactionTime']);
      if ($day !== $time->format('Y-m-d')) {
	if (NULL !== $day) {
	  $html .= '<tr bgcolor="#FFFFFF">';
	  $html .= '<td align="right" colspan="'.$colspan.'" class="noborder"><b>'.t('Day total:').'</b></td>';
	  $html .= '<td align="center" class="noborder">'.sprintf("%0.02f", $day_total).'</td>';
	  $html .= '</tr>';
	}
	$day = $time->format('Y-m-d');
	$day_total = 0.0;
      }
      $grand_total += $entry['Amount'];
      $day_total += $entry['Amount'];
      $html .= '<tr bgcolor="#FFFFFF">';
      if ($agent_id === 'ANY') {
	$html .= '<td align="center">'.decode_entities($meters[$entry['AgentId']]['Alias']).'</td>';
      }
      $html .= '<td align="center">'.$time->format("M j,y H:i").'</td>';
      $html .= '<td align="center">'.$entry['TicketNo'].'</td>';
      $html .= '<td align="center">'.$entry['TransactionId'].'</td>';
      $html .= '<td align="center">'.$entry['CardType'].'</td>';
      $html .= '<td align="center">'.$entry['Card'].'</td>';
      $html .= '<td align="center">'.$entry['MerchantCode'].'</td>';
      $html .= '<td align="center">'.((!isset($entry['Completed']) or
				       (isset($entry['Completed']) and $entry['Completed'])) ? t('Yes') : t('No')).'</td>';
      $html .= '<td align="center">'.$entry['Amount'].'</td>';
      $html .= '</tr>';
    }
    if (0 < $grand_total) {
      $html .= '<tr bgcolor="#FFFFFF">';
      $html .= '<td align="right" colspan="'.$colspan.'" class="noborder"><b>'.t('Day total:').'</b></td>';
      $html .= '<td align="center" class="noborder">'.sprintf("%0.02f", $day_total).'</td>';
      $html .= '</tr>';
      $html .= '<tr bgcolor="#FFFFFF">';
      $html .= '<td align="right" colspan="'.$colspan.'" class="noborder"><b>'.t('Grand total:').'</b></td>';
      $html .= '<td align="center" class="noborder">'.sprintf("%0.02f", $grand_total).'</td>';
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
      $headers[] = t('Meter');
    }
    $headers[] = t('Time');
    $headers[] = t('Ticket');
    $headers[] = t('Transaction');
    $headers[] = t('Card type');
    $headers[] = t('Card');
    $headers[] = t('Merchant code');
    $headers[] = t('Completed');
    $headers[] = t('Amount');
    fputcsv($output, $headers);
    foreach ($entries as $entry) {
      $row = array();
      if ($agent_id === 'ANY') {
	$row[] = decode_entities($meters[$entry['AgentId']]['Alias']);
      }
      $time = chpuser_datetime_utc_to_usertimezone($entry['TransactionTime']);
      $row[] = $time->format("M j,y H:i:s");
      $row[] = $entry['TicketNo'];
      $row[] = $entry['TransactionId'];
      $row[] = $entry['CardType'];
      $row[] = $entry['Card'];
      $row[] = $entry['MerchantCode'];
      $row[] = (!isset($entry['Completed']) or
		(isset($entry['Completed']) and $entry['Completed'])) ? t('Yes') : t('No');
      $row[] = sprintf("%0.02f", $entry['Amount']);
      fputcsv($output, $row);
    }
  }
  return;
}



