<?php
// $Id$


/****************   Boot tables   **********************/



function chdbboot_get_boots($company_id, $enforcement_id,
			    $filter_property = FALSE, $property_id = '',
			    $filter_lot = FALSE, $lot_id = '',
			    $filter_active = FALSE, $active = TRUE,
			    $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_BOOT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for BOOT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM boot_table WHERE CompanyId='%s'", $company_id);
  if ($filter_active) {
    $sql .= sprintf(" AND Active=%d", $active ? 1 : 0);
  }
  if ($order_by !== 'None') {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbboot_get_boot($company_id, $boot_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_BOOT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for BOOT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM boot_table WHERE CompanyId='%s' AND BootId='%s'",
		 $company_id, $boot_id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbboot_get_boot_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_BOOT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for BOOT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM boot_table WHERE Id=%d", $id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbboot_get_boots_report($start_time, $stop_time,
				   $company_id,
				   $filter_property = FALSE, $property_id = '',
				   $filter_lot = FALSE, $lot_id = '') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_BOOT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for BOOT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM boot_table WHERE CompanyId='%s'", $company_id);
  if ($filter_property) {
    $sql .= sprintf(" AND PropertyId='%s'", $property_id);
  }
  if ($filter_lot) {
    $sql .= sprintf(" AND LotId='%s'", $lot_id);
  }
  $sql .= sprintf(" AND Time > '%s' AND Time < '%s'", $start_time, $stop_time);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbboot_get_master_history($max_entries = 0) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_MASTER_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for MASTER database!!!", 'error');
    return $entries;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $entries;
  }
  $sql = 'SELECT * FROM historyfile_table';
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry['DB'];
  }
  return $entries;
}



function chdbboot_get_history_boots($start_time, $stop_time,
				    $company_id,
				    $filter_property, $property_id,
				    $filter_lot = FALSE, $lot_id = '') {
  $entries = array();
  $files = chdbboot_get_master_history();
  if (count($files) == 0) {
    // Find location of SQLite database
    $db_history_name = getenv('APPS_LPMS_HISTORY_DB');
    if ($db_history_name == FALSE) {
      drupal_set_message("Missing HTTP environment for HISTORY database!!!", 'error');
      return 1;
    }
    $files[] = $db_history_name;
  }
  foreach ($files as $file) {
    // connect to SQLite database
    try {
      $db = new PDO("sqlite:".$file);
    } catch(PDOException $e) {
      drupal_set_message($e->getMessage(), 'error');
      return $entries;
    }
    // Perform DB command
    $sql = sprintf("SELECT * FROM boot_table WHERE CompanyId='%s'", $company_id);
    if ($filter_property) {
      $sql .= sprintf(" AND PropertyId='%s'", $property_id);
    }
    if ($filter_lot) {
      $sql .= sprintf(" AND LotId='%s'", $lot_id);
    }
    $sql .= sprintf(" AND Time > '%s' AND Time < '%s'",
		    $start_time, $stop_time);
    foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
      $entries[] = $entry;
    }
  }
  return $entries;
}



// Return 1 if exists, otherwise 0
function chdbboot_boot_exists($company_id, $boot_id) {
  $entry = chdbboot_get_boot($company_id, $boot_id);
  if (sizeof($entry) > 0) {
    return 1;
  }
  return 0;
}


// Return 0 on success, otherwise 1 
function chdbboot_insert_new_boot($company_id, $boot_id, &$id) {
  $db_name = getenv('APPS_LPMS_BOOT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for BOOT database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Query
  $sql = "INSERT INTO boot_table (CompanyId,BootId) VALUES ";
  $sql .= sprintf("('%s','%s')", $company_id, $boot_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  $db = NULL;
  return 0;
}




// Return 0 on success, otherwise 1 
function chdbboot_update_boot_id($id, $boot_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_BOOT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for BOOT database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Perform DB command
  $sql = "UPDATE boot_table SET Time=CURRENT_TIMESTAMP,";
  $sql .= sprintf("BootId='%s' WHERE Id=%d",
		  $boot_id, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbboot_update_assignment($id, $patroller_id, $property_id, $lot_id,
				    $lpn, $make, $model, $color, $comment) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_BOOT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for BOOT database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Perform DB command
  $sql = "UPDATE boot_table SET Time=CURRENT_TIMESTAMP,";
  $sql .= sprintf("PatrollerId='%s',PropertyId='%s',LotId='%s',LPN='%s',",
		  $patroller_id, $property_id, $lot_id, $lpn);
  $sql .= sprintf("Make='%s',Model='%s',Color='%s',Comment='%s' WHERE Id=%d",
		  $make, $model, $color, $comment, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbboot_update_active($id, $active) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_BOOT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for BOOT database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Perform DB command
  $sql = "UPDATE boot_table SET Time=CURRENT_TIMESTAMP,";
  $sql .= sprintf("Active=%d WHERE Id=%d",
		  $active ? 1 : 0, $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbboot_update_active_id($company_id, $boot_id, $active = FALSE) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_BOOT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for BOOT database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Perform DB command
  $sql = "UPDATE boot_table SET Time=CURRENT_TIMESTAMP,";
  $sql .= sprintf("Active=%d WHERE CompanyId='%s' AND BootId='%s'",
		  $active ? 1 : 0, $company_id, $boot_id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbboot_update_ticket_id($id, $ticket_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_BOOT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for BOOT database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Perform DB command
  $sql = "UPDATE boot_table SET Time=CURRENT_TIMESTAMP,";
  $sql .= sprintf("TicketId='%s' WHERE Id=%d",
		  $ticket_id, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbboot_delete_boot_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_BOOT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for BOOT database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Perform DB command
  $sql = sprintf("DELETE FROM boot_table WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   stats_boot_table   **********************/



// Return 0 on success, otherwise 1 
function chdbboot_stats_new($company_id, $property_id, $lot_id,
			    $boot_id, $patroller_id, $ticket_id, $lpn) {
  $db_name = getenv('APPS_LPMS_STATS_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for STATS database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Query
  $sql = "INSERT INTO stats_boot_table ";
  $sql .= "(CompanyId,PropertyId,LotId,BootId,PatrollerId,TicketId,LPN,New) VALUES ";
  $sql .= sprintf("('%s','%s','%s','%s','%s','%s','%s',1)",
		  $company_id, $property_id, $lot_id,
		  $boot_id, $patroller_id, $ticket_id, $lpn);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbboot_stats_remove($company_id, $property_id, $lot_id,
			       $boot_id, $patroller_id, $ticket_id, $lpn) {
  $db_name = getenv('APPS_LPMS_STATS_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for STATS database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Query
  $sql = "INSERT INTO stats_boot_table ";
  $sql .= "(CompanyId,PropertyId,LotId,BootId,PatrollerId,TicketId,LPN,Remove) VALUES ";
  $sql .= sprintf("('%s','%s','%s','%s','%s','%s','%s',1)",
		  $company_id, $property_id, $lot_id,
		  $boot_id, $patroller_id, $ticket_id, $lpn);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   prediction_table Table   **********************/



function chdbboot_get_predictions($company_id, $property_id,
				   $filter_lot = FALSE, $lot_id = '') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_BOOT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for BOOT database!!!", 'error');
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
  $sql = "SELECT * FROM prediction_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s'",
		  $company_id, $property_id);
  if ($filter_lot) {
    $sql .= sprintf(" AND LotId='%s'", $lot_id);
  }
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbboot_insert_prediction($company_id, $property_id, $lot_id) {
  $db_name = getenv('APPS_LPMS_BOOT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for BOOT database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Query
  $sql = "INSERT INTO prediction_table (CompanyId,PropertyId,LotId) ";
  $sql .= sprintf("VALUES ('%s','%s','%s')",
		  $company_id, $property_id, $lot_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbboot_update_prediction_daily($company_id, $property_id,
					  $lot_id, $today, $tomorrow) {
  $db_name = getenv('APPS_LPMS_BOOT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for BOOT database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Query
  $sql = "UPDATE prediction_table SET ModifyTime=CURRENT_TIMESTAMP,";
  $sql .= sprintf("Today=%d,Tomorrow=%d", $today, $tomorrow);
  $sql .= sprintf(" WHERE CompanyId='%s' AND PropertyId='%s'",
		  $company_id, $property_id);
  $sql .= sprintf(" AND LotId='%s'", $lot_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbboot_update_prediction_weekly($company_id, $property_id,
					   $lot_id, $next_week) {
  $db_name = getenv('APPS_LPMS_BOOT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for BOOT database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Query
  $sql = "UPDATE prediction_table SET ModifyTime=CURRENT_TIMESTAMP,";
  $sql .= sprintf("NextWeek=%d", $next_week);
  $sql .= sprintf(" WHERE CompanyId='%s' AND PropertyId='%s'",
		  $company_id, $property_id);
  $sql .= sprintf(" AND LotId='%s'", $lot_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbboot_update_prediction_monthly($company_id, $property_id,
					    $lot_id, $next_month) {
  $db_name = getenv('APPS_LPMS_BOOT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for BOOT database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Query
  $sql = "UPDATE prediction_table SET ModifyTime=CURRENT_TIMESTAMP,";
  $sql .= sprintf("NextMonth=%d", $next_month);
  $sql .= sprintf(" WHERE CompanyId='%s' AND PropertyId='%s'",
		  $company_id, $property_id);
  $sql .= sprintf(" AND LotId='%s'", $lot_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



