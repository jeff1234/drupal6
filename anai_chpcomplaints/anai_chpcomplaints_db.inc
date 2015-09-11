<?php
// $Id$


/****************   Complaint tables   **********************/



function chdbcomp_get_complaints($company_id, $property_id,
				 $filter_lot = FALSE, $lot_id = '',
				 $filter_active = TRUE, $active = TRUE,
				 $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_COMPLAINT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for COMPLAINT database!!!", 'error');
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
  $sql = "SELECT * FROM complaint_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s'",
		  $company_id, $property_id);
  if ($filter_lot) {
    $sql .= sprintf(" AND LotId='%s'", $lot_id);
  }
  if ($filter_active) {
    $sql .= sprintf(" AND Active=%d", $active ? 1 : 0);
  }
  if (strcmp($order_by, 'None')) {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbcomp_get_complaint($company_id, $property_id, $lot_id, $complaint_id,
				$active = TRUE, $fragment = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_COMPLAINT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for COMPLAINT database!!!", 'error');
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
  $sql = "SELECT * FROM complaint_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s' AND LotId='%s'",
		  $company_id, $property_id, $lot_id);
  $sql .= sprintf(" AND ComplaintId='%s' AND Active=%d AND Fragment=%d",
		  $complaint_id, $active ? 1 : 0, $fragment ? 1 : 0);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    if (!$fragment) {
      $entries = $entry;
    } else {
      $entries[] = $entry;
    }
  }
  return $entries;
}



function chdbcomp_get_complaint_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_COMPLAINT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for COMPLAINT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM complaint_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbcomp_get_complaints_report($start_time, $stop_time,
					$company_id, $property_id,
					$filter_lot = FALSE, $lot_id = '',
					$filter_fragment = FALSE, $fragment = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_COMPLAINT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for COMPLAINT database!!!", 'error');
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
  $sql = "SELECT * FROM complaint_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s'",
		  $company_id, $property_id);

  if ($filter_lot) {
    $sql .= sprintf(" AND LotId='%s'", $lot_id);
  }

  if ($filter_fragment) {
    $sql .= sprintf(" AND Fragment=%d", $fragment ? 1 : 0);
  }

  $sql .= sprintf(" AND CreateTime > '%s' AND CreateTime < '%s'",
		  $start_time, $stop_time);
  //drupal_set_message($sql);
  //print($sql.'<br>');
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 1 if exists, otherwise 0
function chdbcomp_complaint_exists($company_id,
				   $property_id, $lot_id,
				   $complaint_id, $active = TRUE) {
  $entry = chdbcomp_get_complaint($company_id,
				  $property_id, $lot_id,
				  $complaint_id, $active);
  if (sizeof($entry) > 0) {
    return 1;
  }
  return 0;
}


// Return 0 on success, otherwise 1 
function chdbcomp_insert_complaint($uid, $company_id, $property_id, $lot_id, $complaint_id,
				   $complaint, &$id, $create_time = NULL) {
  $db_name = getenv('APPS_LPMS_COMPLAINT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for COMPLAINT database!!!", 'error');
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
  $sql = "INSERT INTO complaint_table (";
  if ($create_time != NULL) {
    $sql .= "CreateTime,";
  }
  $sql .= "CreatorUID,CompanyId,PropertyId,LotId,ComplaintId,Complaint) VALUES (";
  if ($create_time != NULL) {
    $sql .= sprintf("'%s',", $create_time);
  }
  $sql .= sprintf("%d,'%s','%s','%s','%s','%s')",
		  $uid, $company_id, $property_id, $lot_id, $complaint_id, $complaint);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbcomp_update_complaint($id, $uid, $complaint) {
  $db_name = getenv('APPS_LPMS_COMPLAINT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for COMPLAINT database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Retrieve current
  $master = array();
  $sql = sprintf("SELECT * FROM complaint_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $master = $entry;
  }
  if (sizeof($master) == 0) {
    drupal_set_message("Not able to read from DB", 'error');
    return 1;
  }
  // Update master non-fragment
  $sql = "UPDATE complaint_table SET ModifyTime=CURRENT_TIMESTAMP,";
  $sql .= sprintf("ModifiedByUID=%d WHERE Id=%d", $uid, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write-1 to DB", 'error');
    return 1;
  }
  // Insert new fragment
  $sql = "INSERT INTO complaint_table ";
  $sql .= "(CreatorUID,CompanyId,PropertyId,LotId,ComplaintId,Complaint,Fragment) VALUES ";
  $sql .= sprintf("(%d,'%s','%s','%s','%s','%s',1)",
		  $uid,
		  $master['CompanyId'],
		  $master['PropertyId'],
		  $master['LotId'],
		  $master['ComplaintId'],
		  $complaint);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write-2 to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbcomp_retire_complaint($id, $uid) {
  $db_name = getenv('APPS_LPMS_COMPLAINT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for COMPLAINT database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Retrieve current
  $master = array();
  $sql = sprintf("SELECT * FROM complaint_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $master = $entry;
  }
  if (sizeof($master) == 0) {
    drupal_set_message("Not able to read from DB", 'error');
    return 1;
  }
  // Query
  $sql = "UPDATE complaint_table SET ModifyTime=CURRENT_TIMESTAMP,Active=0,";
  $sql .= sprintf("ModifiedByUID='%s' WHERE Active=1 AND CompanyId='%s' AND ComplaintId='%s'",
		  $uid, $master['CompanyId'], $master['ComplaintId']);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbcomp_solve_complaint($id, $uid, $resolution) {
  $db_name = getenv('APPS_LPMS_COMPLAINT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for COMPLAINT database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Retrieve current
  $master = array();
  $sql = sprintf("SELECT * FROM complaint_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $master = $entry;
  }
  if (sizeof($master) == 0) {
    drupal_set_message("Not able to read from DB", 'error');
    return 1;
  }
  // Query
  $sql = "UPDATE complaint_table SET SolvedTime=CURRENT_TIMESTAMP,Active=0,Solved=1,";
  $sql .= sprintf("SolvedByUID=%d,Resolution='%s' WHERE ", $uid, $resolution);
  $sql .= sprintf("Active=1 AND CompanyId='%s' AND ComplaintId='%s'",
		  $master['CompanyId'], $master['ComplaintId']);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbcomp_delete_complaint_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_COMPLAINT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for COMPLAINT database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Retrieve current
  $master = array();
  $sql = sprintf("SELECT * FROM complaint_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $master = $entry;
  }
  if (sizeof($master) == 0) {
    drupal_set_message("Not able to read from DB", 'error');
    return 1;
  }
  // Perform DB command
  $sql = sprintf("DELETE FROM complaint_table WHERE CompanyId='%s' AND ComplaintId='%s'",
		 $master['CompanyId'], $master['ComplaintId']);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   counter_table   **********************/



function chpcomp_get_counter($company_id, $property_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_COMPLAINT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for COMPLAINT database!!!", 'error');
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
  $sql = "SELECT * FROM counter_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s'",
		  $company_id, $property_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chpcomp_insert_counter($company_id, $property_id, &$id) {
  $db_name = getenv('APPS_LPMS_COMPLAINT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for COMPLAINT database!!!", 'error');
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
  $sql = "INSERT INTO counter_table (CompanyId,PropertyId) VALUES";
  $sql .= sprintf(" ('%s','%s')", $company_id, $property_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chpcomp_modify_counter($id, $counter_value) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_COMPLAINT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for COMPLAINT database!!!", 'error');
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
  $sql = sprintf("UPDATE counter_table SET Counter=%d WHERE Id=%d",
		 $counter_value, $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chpcomp_update_counter_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_COMPLAINT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for COMPLAINT database!!!", 'error');
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
  $sql = "UPDATE counter_table SET Counter = Counter + 1";
  $sql .= sprintf(" WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chpcomp_delete_counter_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_COMPLAINT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for COMPLAINT database!!!", 'error');
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



function chdbcomp_get_master_history($max_entries = 0) {
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



function chdbcomp_get_history_complaints($start_time, $stop_time,
					 $company_id, $property_id,
					 $filter_lot = FALSE, $lot_id = '',
					 $filter_fragment = FALSE, $fragment = FALSE) {
  $entries = array();
  $files = chdbcomp_get_master_history();
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
    $sql = "SELECT * FROM complaint_table WHERE ";
    $sql .= sprintf("CompanyId='%s' AND PropertyId='%s'",
		    $company_id, $property_id);

    if ($filter_lot) {
      $sql .= sprintf(" AND LotId='%s'", $lot_id);
    }

    if ($filter_fragment) {
      $sql .= sprintf(" AND Fragment=%d", $fragment ? 1 : 0);
    }

    $sql .= sprintf(" AND CreateTime > '%s' AND CreateTime < '%s'",
		    $start_time, $stop_time);
    //drupal_set_message($sql);
    //print($sql.'<br>');
    foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
      $entries[] = $entry;
    }
  }
  return $entries;
}



