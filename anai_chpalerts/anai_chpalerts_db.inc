<?php
// $Id$


/****************   Alert tables   **********************/



function chdbalert_get_all_alerts() {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ALERT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALERT database!!!", 'error');
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
  $sql = "SELECT * FROM alert_table";
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbalert_get_alerts($company_id, $uid,
			      $filter_unread = FALSE, $unread = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ALERT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALERT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM alert_table WHERE CompanyId='%s' AND UID=%d",
		 $company_id, $uid);
  if ($filter_unread) {
    $sql .= sprintf(" AND Unread=%d", $unread ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbalert_get_alert($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ALERT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALERT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM alert_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbalert_insert_new_alert($company_id, $uid,
				    $alert_type, $trigger, $action_type,
				    &$id) {
  $db_name = getenv('APPS_LPMS_ALERT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALERT database!!!", 'error');
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
  $sql = "INSERT INTO alert_table (CompanyId,UID,AlertType,Trigger,ActionType)";
  $sql .= sprintf(" VALUES ('%s',%d,'%s','%s','%s')",
  		  $company_id, $uid, $alert_type, $trigger, $action_type);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbalert_update_alert($id, $alert_type, $trigger, $action_type) {
  $db_name = getenv('APPS_LPMS_ALERT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALERT database!!!", 'error');
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
  $sql = "UPDATE alert_table SET ";
  $sql .= sprintf("AlertType='%s',Trigger='%s',ActionType='%s' WHERE Id=%d",
		  $alert_type, $trigger, $action_type, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbalert_update_trigger_levels($id, $l1, $l2, $l3, $l4, $l5) {
  $db_name = getenv('APPS_LPMS_ALERT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALERT database!!!", 'error');
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
  $sql = "UPDATE alert_table SET ";
  $sql .= sprintf("L1='%s',L2='%s',L3='%s',L4='%s',L5='%s' WHERE Id=%d",
		  $l1, $l2, $l3, $l4, $l5, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbalert_update_action_levels($id, $a1, $a2, $a3, $a4, $a5) {
  $db_name = getenv('APPS_LPMS_ALERT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALERT database!!!", 'error');
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
  $sql = "UPDATE alert_table SET ";
  $sql .= sprintf("A1='%s',A2='%s',A3='%s',A4='%s',A5='%s' WHERE Id=%d",
		  $a1, $a2, $a3, $a4, $a5, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbalert_update_action($id) {
  $db_name = getenv('APPS_LPMS_ALERT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALERT database!!!", 'error');
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
  $sql = "UPDATE alert_table SET LastTime=CURRENT_TIMESTAMP,Unread=1,";
  $sql .= sprintf("Count=Count+1 WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbalert_update_read($company_id, $uid) {
  $db_name = getenv('APPS_LPMS_ALERT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALERT database!!!", 'error');
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
  $sql = "UPDATE alert_table SET Unread=0";
  $sql .= sprintf(" WHERE CompanyId='%s' AND UID=%d", $company_id, $uid);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbalert_delete_alert_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ALERT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALERT database!!!", 'error');
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
  $sql = sprintf("DELETE FROM alert_table WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   alertlink_table   **********************/



function chdbalert_get_links($company_id, $uid) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ALERT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALERT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM alertlink_table WHERE CompanyId='%s' AND UID=%d",
		 $company_id, $uid);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbalert_insert_new_link($company_id, $uid, $link, $alert_type) {
  $db_name = getenv('APPS_LPMS_ALERT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALERT database!!!", 'error');
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
  $sql = "INSERT INTO alertlink_table (CompanyId,UID,Link,AlertType) VALUES ";
  $sql .= sprintf("('%s',%d,'%s','%s')",
  		  $company_id, $uid, $link, $alert_type);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbalert_delete_links_retention_older_than($time, $retention_days) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ALERT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALERT database!!!", 'error');
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
  $sql = "DELETE FROM alertlink_table";
  $sql .= sprintf(" WHERE DATETIME(CreateTime,'%d DAYS') < '%s'",
		  $retention_days, $time);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}




/****************   Notify table   **********************/



// Return 0 on success, otherwise 1 
function chdbalert_insert_new_email($company_id, $uid, $email, $link, $alert_type) {
  $db_name = getenv('APPS_LPMS_NOTIFY_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for NOTIFY database!!!", 'error');
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
  $sql = "INSERT INTO new_alert_table (CompanyId,UID,Email,Link,AlertType) VALUES ";
  $sql .= sprintf("('%s',%d,'%s','%s','%s')",
  		  $company_id, $uid, $email, $link, $alert_type);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



