<?php



/****************   rules_table   **********************/



function chphotlist_get_rules($company_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_HOTLIST_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for HOTLIST database!!!", 'error');
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
  $sql = "SELECT * FROM rules_table WHERE ";
  $sql .= sprintf("CompanyId='%s'", $company_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chphotlist_get_rule_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_HOTLIST_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for HOTLIST database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM rules_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chphotlist_insert_rule($uid, $company_id,
				$lpn, $make, $model, $color,
				$start_time, $forever, $stop_time,
				&$id) {
  $db_name = getenv('APPS_LPMS_HOTLIST_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for HOTLIST database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // SQL
  $sql = "INSERT INTO rules_table (CreatorUID,CompanyId,";
  $sql .= "LPN,Make,Model,Color,StartTime) VALUES ";
  $sql .= sprintf("(%d,'%s','%s','%s','%s','%s','%s')",
		  $uid, $company_id,
		  $lpn, $make, $model, $color, $start_time);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  if (!$forever) {
    // SQL
    $sql = sprintf("UPDATE rules_table SET StopTime='%s' WHERE Id=%d",
		   $stop_time, $id);
    if (!$db->exec($sql)) {
      drupal_set_message("Not able to write to DB", 'error');
      return 1;
    }
  }
  return 0;
}



// Return 0 on success, otherwise 1
function chphotlist_update_rule($id, $lpn, $make, $model, $color, $start_time, $forever, $stop_time) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_HOTLIST_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for HOTLIST database!!!", 'error');
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
  $sql = 'UPDATE rules_table SET ';
  $sql .= sprintf("LPN='%s',Make='%s',Model='%s',Color='%s',StartTime='%s',",
		  $lpn, $make, $model, $color, $start_time);
  if ($forever) {
    $sql .= "StopTime=NULL";
  } else {
    $sql .= sprintf("StopTime='%s'", $stop_time);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1
function chphotlist_delete_rule_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_HOTLIST_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for HOTLIST database!!!", 'error');
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
  $sql = sprintf("DELETE FROM rules_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   enforcement_table   **********************/



function chphotlist_get_enforcement($company_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_HOTLIST_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for HOTLIST database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM enforcement_table WHERE CompanyId='%s'",
		 $company_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chphotlist_get_enforcement_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_HOTLIST_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for HOTLIST database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM enforcement_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chphotlist_insert_enforcement($company_id, $tickets, &$id) {
  $db_name = getenv('APPS_LPMS_HOTLIST_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for HOTLIST database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // SQL
  $sql = "INSERT INTO enforcement_table (CompanyId,Tickets) VALUES ";
  $sql .= sprintf("('%s',%d)",
		  $company_id, $tickets ? 1 : 0);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1
function chphotlist_update_enforcement($company_id, $tickets) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_HOTLIST_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for HOTLIST database!!!", 'error');
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
  $sql = sprintf("UPDATE enforcement_table SET Tickets=%d", $tickets ? 1 : 0);
  $sql .= sprintf(" WHERE CompanyId='%s'", $company_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   lpr_table   **********************/



function chphotlist_get_lpr($company_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_HOTLIST_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for HOTLIST database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM lpr_table WHERE CompanyId='%s'",
		 $company_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chphotlist_get_lpr_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_HOTLIST_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for HOTLIST database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM lpr_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chphotlist_insert_lpr($company_id, $noncomp, &$id) {
  $db_name = getenv('APPS_LPMS_HOTLIST_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for HOTLIST database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // SQL
  $sql = "INSERT INTO lpr_table (CompanyId,NonCompliant) VALUES ";
  $sql .= sprintf("('%s',%d)", $company_id, $noncomp ? 1 : 0);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1
function chphotlist_update_lpr($company_id, $noncomp) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_HOTLIST_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for HOTLIST database!!!", 'error');
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
  $sql = sprintf("UPDATE lpr_table SET NonCompliant=%d", $noncomp ? 1 : 0);
  $sql .= sprintf(" WHERE CompanyId='%s'", $company_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



