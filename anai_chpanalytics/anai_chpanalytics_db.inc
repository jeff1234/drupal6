<?php
// $Id$



/****************   permitstats_table   **********************/



function chdban_get_permit_stats($company_id, $property_id,
				 $filter_lot = FALSE, $lot_id = NULL,
				 $filter_group = FALSE, $group_id = NULL,
				 $filter_member = FALSE, $member_id = NULL,
				 $newer_than = 'Any',
				 $older_than = 'Any',
				 $filter_new = FALSE, $new = TRUE,
				 $filter_modify = FALSE, $modify = TRUE,
				 $filter_remove = FALSE, $remove = TRUE,
				 $filter_lpn = FALSE, $lpn = NULL) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_STATS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for STATS database!!!", 'error');
    return $entries;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $entries;
  }
  // Perform DB command
  $sql = "SELECT * FROM permitstats_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s'",
		  $company_id, $property_id);

  if ($filter_lot) {
    $sql .= sprintf(" AND LotId='%s'", $lot_id);
  }

  if (strcmp($newer_than, 'Any')) {
    $sql .= sprintf(" AND CreateTime > '%s'", $newer_than);
  }

  if (strcmp($older_than, 'Any')) {
    $sql .= sprintf(" AND CreateTime < '%s'", $older_than);
  }

  if ($filter_new) {
    $sql .= sprintf(" AND New=%d", $new ? 1 : 0);
  }

  if ($filter_modify) {
    $sql .= sprintf(" AND Modify=%d", $modify ? 1 : 0);
  }

  if ($filter_remove) {
    $sql .= sprintf(" AND Remove=%d", $remove ? 1 : 0);
  }

  if ($filter_lpn) {
    $sql .= sprintf(" AND LPN='%s'", $lpn);
  }

  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



/****************   loopscount_table   **********************/



function chdban_get_loopscount_stats($property_id,
				     $lots, $lanes,
				     $stop_time, $interval, $steps) {
  $entries = array();
  if ($interval !== 'Daily' and
      $interval !== 'Weekly' and
      $interval !== 'Monthly') {
    return $entries;
  }
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_STATS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for STATS database!!!", 'error');
    return $entries;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $entries;
  }
  // SQL
  $sql = "SELECT * FROM loopscount_table WHERE ";
  $sql .= sprintf("PropertyId='%s'", $property_id);

  $stop_time = chpuser_datetime_utc_to_utc($stop_time);
  for ($i = 0; $i < $steps; $i++) {
    $start_time = chpuser_datetime_utc_to_utc($stop_time->format("Y-m-d H:i:s"));
    if ($interval === 'Daily') {
      $start_time->modify("-1 day");
    } elseif ($interval === 'Weekly') {
      $start_time->modify("-1 week");
    } elseif ($interval === 'Monthly') {
      $start_time->modify("-1 month");
    }
    $total = 0;

    foreach ($lots as $lot_id) {
      $sql_lot = $sql.sprintf(" AND LotId='%s'", $lot_id);
      foreach ($lanes as $lane_id) {
  	$sql_lane = $sql_lot.sprintf(" AND LaneId='%s'", $lane_id);
  	// Start value
	$start = 0;
  	$sql_v = $sql_lane;
  	$sql_v .= sprintf(" AND CreateTime>'%s'", $start_time->format("Y-m-d H:i:s"));
  	$sql_v .= " ORDER BY Id LIMIT 1";
  	//drupal_set_message($sql_v);
  	foreach ($db->query($sql_v, PDO::FETCH_ASSOC) as $entry) {
  	  $start += $entry['Count'];
  	}
  	// Stop value
	$stop = 0;
  	$sql_v = $sql_lane;
  	$sql_v .= sprintf(" AND CreateTime<'%s'", $stop_time->format("Y-m-d H:i:s"));
  	$sql_v .= " ORDER BY Id DESC LIMIT 1";
  	//drupal_set_message($sql_v);
  	foreach ($db->query($sql_v, PDO::FETCH_ASSOC) as $entry) {
  	  $stop += $entry['Count'];
  	}
	// Result
	$count = $stop - $start;
	if (0 > $count) {
	  $count = 0;
	}
	$total += $count;
	//drupal_set_message($stop.'-'.$start.'='.$total);
      }
    }

    
    $entries[] = $total;
    // next step
    $stop_time = chpuser_datetime_utc_to_utc($start_time->format("Y-m-d H:i:s"));
  }

  return $entries;
}



/****************   stats_ticket_table   **********************/



function chdban_get_ticket_stats($company_id, $property_id,
				 $filter_lot = FALSE, $lot_id = '',
				 $newer_than = 'Any',
				 $older_than = 'Any',
				 $filter_new = FALSE, $new = TRUE,
				 $filter_modify = FALSE, $modify = TRUE,
				 $filter_remove = FALSE, $remove = TRUE,
				 $filter_lpn = FALSE, $lpn = '') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_STATS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for STATS database!!!", 'error');
    return $entries;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $entries;
  }
  // Perform DB command
  $sql = "SELECT * FROM ticketstats_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s'",
		  $company_id, $property_id);

  if ($filter_lot) {
    $sql .= sprintf(" AND LotId='%s'", $lot_id);
  }

  if (strcmp($newer_than, 'Any')) {
    $sql .= sprintf(" AND CreateTime > '%s'", $newer_than);
  }

  if (strcmp($older_than, 'Any')) {
    $sql .= sprintf(" AND CreateTime < '%s'", $older_than);
  }

  if ($filter_new) {
    $sql .= sprintf(" AND New=%d", $new ? 1 : 0);
  }

  if ($filter_modify) {
    $sql .= sprintf(" AND Modify=%d", $modify ? 1 : 0);
  }

  if ($filter_remove) {
    $sql .= sprintf(" AND Remove=%d", $remove ? 1 : 0);
  }

  if ($filter_lpn) {
    $sql .= sprintf(" AND LPN='%s'", $lpn);
  }

  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



/****************   stats_boot_table   **********************/



function chdban_get_boot_stats($company_id, $property_id,
			       $filter_lot = FALSE, $lot_id = '',
			       $newer_than = 'Any',
			       $older_than = 'Any',
			       $filter_new = FALSE, $new = TRUE,
			       $filter_modify = FALSE, $modify = TRUE,
			       $filter_remove = FALSE, $remove = TRUE,
			       $filter_lpn = FALSE, $lpn = '') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_STATS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for STATS database!!!", 'error');
    return $entries;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $entries;
  }
  // Perform DB command
  $sql = "SELECT * FROM stats_boot_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s'",
		  $company_id, $property_id);

  if ($filter_lot) {
    $sql .= sprintf(" AND LotId='%s'", $lot_id);
  }

  if (strcmp($newer_than, 'Any')) {
    $sql .= sprintf(" AND CreateTime > '%s'", $newer_than);
  }

  if (strcmp($older_than, 'Any')) {
    $sql .= sprintf(" AND CreateTime < '%s'", $older_than);
  }

  if ($filter_new) {
    $sql .= sprintf(" AND New=%d", $new ? 1 : 0);
  }

  if ($filter_modify) {
    $sql .= sprintf(" AND Modify=%d", $modify ? 1 : 0);
  }

  if ($filter_remove) {
    $sql .= sprintf(" AND Remove=%d", $remove ? 1 : 0);
  }

  if ($filter_lpn) {
    $sql .= sprintf(" AND LPN='%s'", $lpn);
  }

  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



/****************   stats_valet_table   **********************/



function chdban_get_valet_stats($company_id, $property_id,
				$filter_lot = FALSE, $lot_id = '',
				$newer_than = 'Any',
				$older_than = 'Any',
				$filter_new = FALSE, $new = TRUE,
				$filter_modify = FALSE, $modify = TRUE,
				$filter_remove = FALSE, $remove = TRUE,
				$filter_lpn = FALSE, $lpn = '') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_STATS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for STATS database!!!", 'error');
    return $entries;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $entries;
  }
  // Perform DB command
  $sql = "SELECT * FROM valetstats_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s'",
		  $company_id, $property_id);

  if ($filter_lot) {
    $sql .= sprintf(" AND LotId='%s'", $lot_id);
  }

  if (strcmp($newer_than, 'Any')) {
    $sql .= sprintf(" AND CreateTime > '%s'", $newer_than);
  }

  if (strcmp($older_than, 'Any')) {
    $sql .= sprintf(" AND CreateTime < '%s'", $older_than);
  }

  if ($filter_new) {
    $sql .= sprintf(" AND New=%d", $new ? 1 : 0);
  }

  if ($filter_modify) {
    $sql .= sprintf(" AND Modify=%d", $modify ? 1 : 0);
  }

  if ($filter_remove) {
    $sql .= sprintf(" AND Remove=%d", $remove ? 1 : 0);
  }

  if ($filter_lpn) {
    $sql .= sprintf(" AND LPN='%s'", $lpn);
  }

  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



