<?php
require_once("anai_chpoccupancy_db.inc");



function chpoccu_update_entry_scan($property_id, $lot_id, $scan_id, $lpr_time, $lpn) {
  $occu = chdboccu_get_occupancy_scan_id($property_id, $scan_id);

  if (empty($occu)) {
    // A discrepancy scan has been made real
    $insert_needed = TRUE;

    // Is there an entry with same LPN? ...
    $entry = chdboccu_get_occupancy_current_lpn($property_id, $lot_id, $lpn);

    if (!empty($entry)) {
      // ... yes it is, include into EventId stack
      $insert_needed = FALSE;
      $event_id = $entry['EventId'];
      if (chdboccu_update_scan_id($property_id, $scan_id, $lpn, $event_id)) {
	drupal_set_message('Could not update occupancy', 'error');
	return;
      }
    } else {
      // ... no, see if there are any close enough ...
      foreach (chdboccu_get_all_occupancy($property_id, $lot_id) as $entry) {
	if (1 < chpperm_fuzzy_search_lpn($lpn, $entry['LPN'])) {
	  continue;
	}
	// ... yes it is, include into EventId stack
	$insert_needed = FALSE;
	$event_id = $entry['EventId'];
	if (chdboccu_update_scan_id($property_id, $scan_id, $lpn, $event_id)) {
	  drupal_set_message('Could not update occupancy', 'error');
	  return;
	}
	break;
      }
    }
    if ($insert_needed) {
      $event_id = chpprop_genRandomString20();
      if (chdboccu_insert_scan($property_id, $lot_id, $lpr_time, $lpn, $event_id, $id)) {
	drupal_set_message('Could not insert occupancy', 'error');
	return;
      }
    }
  } else {
    if (chdboccu_update_scan_id($property_id, $scan_id, $lpn)) {
      drupal_set_message('Could not update occupancy', 'error');
      return;
    }
  }

  return 0;
}



function chpoccu_update_exit_scan($property_id, $scan_id, $lpn) {
  $occu = chdboccu_get_occupancy_scan_id($property_id, $scan_id);
  if (empty($occu)) {
    return 0;
  }

  if (chdboccu_update_scan_id($property_id, $scan_id, $lpn)) {
    drupal_set_message('Could not update occupancy', 'error');
    return;
  }

  return 0;
}



