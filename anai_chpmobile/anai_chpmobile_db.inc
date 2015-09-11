<?php
// $Id$


/***************  agent_table  ***************/



function chdbmob_get_mobile_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_AGENT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AGENT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM agent_table WHERE Id=%d", $id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbmob_get_mobile_agent($agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_AGENT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AGENT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM agent_table WHERE AgentId='%s'", $agent_id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbmob_get_mobiles($company_id, $active = TRUE, $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_AGENT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AGENT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM agent_table WHERE CompanyId='%s' AND Active=%d",
		 $company_id, $active ? 1 : 0);
  if ($order_by !== 'None') {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Insert a new entry
// Return 0 on success, otherwise 1 
function chdbmob_insert_new($agent_id, $agent_variant, $alias,
			    $company_id, $username, $random, $app_id, &$id,
			    $use_expiry = FALSE, $expiry = '') {
  $db_name = getenv('APPS_LPMS_AGENT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AGENT database!!!", 'error');
    return 1;
  }
  $sql = "INSERT INTO agent_table (Id,AgentId,AgentType,AgentVariant";
  $sql .= ",Alias,CompanyId,User,Random,AppId";
  if ($use_expiry) {
    $sql .= ",ExpiryTime";
  }
  $sql .= sprintf(") VALUES (NULL,'%s','Condominium',%d,'%s','%s','%s','%s','%s'",
		  $agent_id, $agent_variant, $alias,
		  $company_id, $username, $random, $app_id);
  if ($use_expiry) {
    $sql .= sprintf(",'%s'", $expiry);
  }
  $sql .= ")";
  //drupal_set_message($sql);
  $db = new PDO("sqlite:".$db_name);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbmob_update_mobile($id, $alias, $app_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_AGENT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AGENT database!!!", 'error');
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
  $sql = "UPDATE agent_table SET ";
  $sql .= sprintf("Alias='%s',AppId='%s' WHERE Id=%d",
		  $alias, $app_id, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbmob_set_suspended($id, $suspended) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_AGENT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AGENT database!!!", 'error');
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
  $sql = sprintf("UPDATE agent_table SET Suspended=%d WHERE Id=%d",
		 $suspended ? 1 : 0, $id);
  drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbmob_set_inactive_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_AGENT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AGENT database!!!", 'error');
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
  $sql = "UPDATE agent_table SET DeactivationTime=CURRENT_TIMESTAMP,Active=0";
  $sql .= sprintf(" WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbmob_delete_mobile_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_AGENT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AGENT database!!!", 'error');
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
  $sql = sprintf("DELETE FROM agent_table WHERE Id=%d",
		 $id);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/***************  agent_property_table  ***************/



function chdbmob_get_mobile_properties($company_id, $agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_AGENT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AGENT database!!!", 'error');
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
  $sql = "SELECT * FROM agent_property_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND AgentId='%s'",
		  $company_id, $agent_id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbmob_get_mobile_agents($company_id, $property_id,
				   $filter_lot = FALSE, $lot_id = '') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_AGENT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AGENT database!!!", 'error');
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
  $sql = "SELECT * FROM agent_property_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND PropertyId='%s'",
		  $company_id, $property_id);
  if ($filter_lot) {
    $sql .= sprintf(" AND LotId='%s'", $lot_id);
  }
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Insert a new entry
// Return 0 on success, otherwise 1 
function chdbmob_insert_mobile_property($agent_id, $company_id,
					$property_id, $lot_id, &$id) {
  $db_name = getenv('APPS_LPMS_AGENT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AGENT database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  $sql = "INSERT INTO agent_property_table (AgentId,CompanyId,PropertyId,LotId) ";
  $sql .= sprintf("VALUES ('%s','%s','%s','%s')",
		  $agent_id, $company_id, $property_id, $lot_id);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbmob_delete_mobile_property($company_id, $agent_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_AGENT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AGENT database!!!", 'error');
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
  $sql = "DELETE FROM agent_property_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND AgentId='%s'",
		  $company_id, $agent_id);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   CRON functions   **********************/



// Return 0 on success, otherwise 1
function chdbmob_delete_mobiles_older_than($time) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_AGENT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AGENT database!!!", 'error');
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
  $sql = "DELETE FROM agent_table WHERE Active=0 AND ";
  $sql .= sprintf("DeactivationTime < '%s'", $time);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



