<?php
// $Id$


/****************   Towing tables   **********************/



function chdbtow_get_towings($company_id, $property_id,
			     $filter_lot, $lot_id = '',
			     $active = TRUE,
			     $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TOWING_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TOWING database!!!", 'error');
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
  $sql = "SELECT * FROM towing_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s'",
		  $company_id, $property_id);
  if ($filter_lot) {
    $sql .= sprintf(" AND LotId='%s'", $lot_id);
  }
  $sql .= sprintf(" AND Active=%d", $active ? 1 : 0);
  if (strcmp($order_by, 'None')) {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbtow_get_towing($company_id, $towing_id, $active = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TOWING_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TOWING database!!!", 'error');
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
  $sql = "SELECT * FROM towing_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND TowingId='%s'",
		  $company_id, $towing_id);
  $sql .= sprintf(" AND Active=%d", $active ? 1 : 0);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbtow_get_towing_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TOWING_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TOWING database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM towing_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbtow_get_towings_report($start_time, $stop_time, $company_id, $property_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TOWING_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TOWING database!!!", 'error');
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
  $sql = "SELECT * FROM towing_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s'",
		  $company_id, $property_id);
  $sql .= sprintf(" AND CreateTime > '%s' AND CreateTime < '%s'",
		  $start_time, $stop_time);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 1 if exists, otherwise 0
function chdbtow_towing_exists($company_id, $towing_id, $active = TRUE) {
  $entry = chdbtow_get_towing($company_id, $towing_id, $active);
  if (sizeof($entry) > 0) {
    return 1;
  }
  return 0;
}



function chdbtow_get_towings_lpn($company_id, $lpn,
				 $newer_than = 'Any',
				 $active = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TOWING_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TOWING database!!!", 'error');
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
  $sql = "SELECT * FROM towing_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND LPN='%s' AND Active=%d",
		 $company_id, $lpn, $active ? 1 : 0);
  if (strcmp($newer_than, 'Any')) {
    $sql .= sprintf(" AND CreateTime > '%s'", $newer_than);
  }
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbtow_insert_new_towing($company_id, $patroller_id, $towing_id,
				   $property_id, $lot_id,
				   $code, $description, $vin,
				   $lpn, $make, $model, $color,
				   $comment, $operator,
				   $created_by, &$id,
				   $create_time = NULL) {
  $db_name = getenv('APPS_LPMS_TOWING_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TOWING database!!!", 'error');
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
  $sql = "INSERT INTO towing_table (";
  if ($create_time != NULL) {
    $sql .= "CreateTime,CopyManual,";
  }
  $sql .= "CompanyId,PatrollerId,TowingId,PropertyId,LotId,Code,Description,";
  $sql .= "VIN,LPN,Make,Model,Color,Comment,Operator,CreatedBy) VALUES (";
  if ($create_time != NULL) {
    $sql .= sprintf("'%s',1,", $create_time);
  }
  $sql .= sprintf("'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
		  $company_id, $patroller_id, $towing_id,
		  $property_id, $lot_id,
		  $code, $description, $vin, $lpn,
		  $make, $model, $color, $comment,
		  $operator, $created_by);
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
function chdbtow_update_towing($id, $active = FALSE) {
  $db_name = getenv('APPS_LPMS_TOWING_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TOWING database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Update
  $sql = "UPDATE towing_table SET ";
  $sql .= sprintf("Active=%d WHERE Id=%d",
		  $active ? 1 : 0, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbtow_delete_towing_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TOWING_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TOWING database!!!", 'error');
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
  $sql = sprintf("DELETE FROM towing_table WHERE Id=%d",
		 $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   Counter Table   **********************/



// Return 0 on success, otherwise 1 
function chdbtow_insert_new_counter($company_id, &$id) {
  $db_name = getenv('APPS_LPMS_TOWING_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TOWING database!!!", 'error');
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
  $sql = sprintf("INSERT INTO counter_table (CompanyId) VALUES ('%s')", $company_id);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  $db = NULL;
  return 0;
}



function chdbtow_get_all_counters($company_id, $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TOWING_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TOWING database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM counter_table WHERE CompanyId='%s'", $company_id);
  if (strcmp($order_by, 'None')) {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbtow_modify_counter($id, $counter_value) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TOWING_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TOWING database!!!", 'error');
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
  $sql = sprintf("UPDATE counter_table SET TowingIdCounter=%d WHERE Id=%d",
		 $counter_value, $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbtow_update_counter($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TOWING_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TOWING database!!!", 'error');
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
  $sql = "UPDATE counter_table SET TowingIdCounter=TowingIdCounter+1";
  $sql .= sprintf(" WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbtow_delete_counter($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TOWING_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TOWING database!!!", 'error');
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
  $sql = sprintf("DELETE FROM counter_table WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   History table   **********************/



function chdbtow_get_master_history($max_entries = 0) {
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



function chdbtow_get_history_towings($start_time, $stop_time,
				     $company_id, $property_id) {
  $entries = array();
  $files = chdbtow_get_master_history();
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
    $sql = "SELECT * FROM towing_table WHERE ";
    $sql .= sprintf("CompanyId='%s' AND PropertyId='%s'",
		    $company_id, $property_id);
    $sql .= sprintf(" AND CreateTime > '%s' AND CreateTime < '%s'",
		    $start_time, $stop_time);
    foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
      $entries[] = $entry;
    }
  }
  return $entries;
}



