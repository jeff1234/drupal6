<?php



/****************   cloud_table   **********************/



function chdbccpark_get_all_clouds($company_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM cloud_table WHERE CompanyId='%s'", $company_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbccpark_get_clouds($company_id, $property_id, $lot_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "SELECT * FROM cloud_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s' AND LotId='%s'",
		 $company_id, $property_id, $lot_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// TBD: remove when normal AgentId creation is in place
function chdbccpark_get_cloud_agentid($agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "SELECT * FROM cloud_table WHERE ";
  $sql .= sprintf("AgentId='%s'", $agent_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbccpark_get_cloud_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM cloud_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbccpark_insert_cloud($uid, $company_id, $property_id, $lot_id, $alias,
				 $agent_id, $server_id, $utc_offset,
				 &$id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "INSERT INTO cloud_table (CreatorUID,CompanyId,PropertyId,LotId,Alias,";
  $sql .= "AgentId,ServerId,UTCOffset) VALUES ";
  $sql .= sprintf("(%d,'%s','%s','%s','%s','%s','%s',%d)",
		  $uid, $company_id, $property_id, $lot_id, $alias,
		  $agent_id, $server_id, $utc_offset);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_cloud_id($id, $agent_id, $alias, $utc_offset) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("UPDATE cloud_table SET AgentId='%s',Alias='%s',UTCOffset=%d",
		 $agent_id, $alias, $utc_offset);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_delete_cloud_id($id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("DELETE FROM cloud_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   system_table   **********************/



function chdbccpark_get_all_systems($company_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "SELECT * FROM system_table WHERE ";
  $sql .= sprintf("CompanyId='%s'", $company_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbccpark_get_systems($company_id, $property_id, $lot_id,
				$lot_type = 'ANY', $lot_class = 'ANY') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
    return $entries;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $entries;
  }
  if ($company_id === 'ANY' or $property_id === 'ANY' or $lot_id === 'ANY' or
      $lot_type !== 'ANY' or $lot_class !== 'ANY') {
    // Find location of SQLite database
    $db_name = getenv('APPS_LPMS_PROP_DB');
    if ($db_name == FALSE) {
      drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
      return $entries;
    }
    // SQL
    //drupal_set_message($sql);
    //var_export($sql);
    $sql = sprintf("ATTACH '%s' AS props", $db_name);
    $db->exec($sql);
  }
  // SQL
  $sql = "SELECT system.* FROM system_table AS system";
  if ($company_id === 'ANY' or $property_id === 'ANY' or $lot_id === 'ANY') {
    $sql .= " INNER JOIN props.user_table AS user";
    $sql .= "  ON (system.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
    $sql .= "   AND (system.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
    $sql .= "    AND (system.LotId=user.LotId OR user.LotId='ANY')";
  }
  if ($lot_type !== 'ANY' or $lot_class !== 'ANY') {
    $sql .= " INNER JOIN props.policy_table AS policy";
    $sql .= "  ON (system.PropertyId=policy.PropertyId)";
    $sql .= "   AND (system.LotId=policy.LotId)";
    $sql .= " INNER JOIN props.profile_table AS profile";
    $sql .= "  ON (policy.ProfileId=profile.ProfileId)";
  }
  $precond = TRUE;
  if ($company_id !== 'ANY') {
    $sql .= sprintf(" %s system.CompanyId='%s'", $precond ? 'WHERE' : 'AND', $company_id);
    $precond = FALSE;
  }
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" %s system.PropertyId='%s'", $precond ? 'WHERE' : 'AND', $property_id);
    $precond = FALSE;
  }
  if ($lot_id !== 'ANY') {
    $sql .= sprintf(" %s system.LotId='%s'", $precond ? 'WHERE' : 'AND', $lot_id);
    $precond = FALSE;
  }
  if ($lot_type !== 'ANY') {
    $sql .= sprintf(" %s (((profile.LongPolicy OR profile.LongShortPolicy) AND profile.LongTypeId='%s') OR ((profile.ShortPolicy OR profile.LongShortPolicy) AND profile.ShortTypeId='%s'))",
    		    $precond ? 'WHERE' : 'AND', $lot_type, $lot_type);
    $precond = FALSE;
  }
  if ($lot_class !== 'ANY') {
    $sql .= sprintf(" %s (((profile.LongPolicy OR profile.LongShortPolicy) AND profile.LongClassId='%s') OR ((profile.ShortPolicy OR profile.LongShortPolicy) AND profile.ShortClassId='%s'))",
    		    $precond ? 'WHERE' : 'AND', $lot_class, $lot_class);
    $precond = FALSE;
  }
  //drupal_set_message($sql);
  //var_export($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbccpark_get_system($agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM system_table WHERE AgentId='%s'", $agent_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbccpark_get_system_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM system_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_system_status($agent_id, $severity) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("UPDATE system_table SET Status='%s' WHERE AgentId='%s'",
		 $severity, $agent_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_system_position($agent_id, $lat, $lon) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE system_table SET PosPending=1,";
  $sql .= sprintf("Lat='%s',Lon='%s' WHERE AgentId='%s'",
		  $lat, $lon, $agent_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_system_cashreset_id($id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE system_table SET CashResetPending=1";
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Tore 131201: Added
// Return 0 on success, otherwise 1 
function chdbccpark_update_system_id($id, $agent_id, $alias, $utc_offset) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("UPDATE system_table SET AgentId='%s',Alias='%s',UTCOffset=%d",
		 $agent_id, $alias, $utc_offset);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   systemid_table   **********************/



function chdbccpark_get_systemid($company_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM systemid_table WHERE CompanyId='%s'",
		 $company_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbccpark_insert_systemid($company_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "INSERT INTO systemid_table (CompanyId) VALUES ";
  $sql .= sprintf("('%s')", $company_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_systemid($company_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE systemid_table SET Counter=Counter+1";
  $sql .= sprintf(" WHERE CompanyId='%s'", $company_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_delete_all_systemid($company_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("DELETE FROM systemid_table WHERE CompanyId='%s'", $company_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   diagnostics_table   **********************/



// Return 0 on success, otherwise 1 
function chdbccpark_insert_diagnostics($company_id, $agent_id, $server_id,
				       $unregister, $bit, $reboot) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "INSERT INTO diagnostics_table ";
  $sql .= "(CompanyId,AgentId,ServerId,Unregister,BIT,Reboot) VALUES ";
  $sql .= sprintf("('%s','%s','%s',%d,%d,%d)",
		  $company_id, $agent_id, $server_id,
		  $unregister ? 1 : 0,
		  $bit ? 1 : 0,
		  $reboot ? 1 : 0);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_delete_diagnostics($agent_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("DELETE FROM diagnostics_table WHERE AgentId='%s'", $agent_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   auditcmd_table   **********************/



function chdbccpark_get_auditcmd($agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM auditcmd_table WHERE AgentId='%s'",
		 $agent_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbccpark_insert_auditcmd($company_id, $agent_id, $server_id,
				    $active, $interval, $update_timestamp) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "INSERT INTO auditcmd_table ";
  $sql .= "(CompanyId,AgentId,ServerId,Active,RepeatMinutes,UpdateTimestamp) VALUES ";
  $sql .= sprintf("('%s','%s','%s',%d,%d,%d)",
		  $company_id, $agent_id, $server_id,
		  $active ? 1 : 0,
		  $interval,
		  $update_timestamp ? 1 : 0);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_auditcmd($agent_id,
				    $active, $interval, $update_timestamp) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE auditcmd_table SET NextTime=CURRENT_TIMESTAMP,";
  $sql .= sprintf("Active=%d,RepeatMinutes=%d,UpdateTimestamp=%d",
		  $active ? 1 : 0, $interval, $update_timestamp ? 1 : 0);
  $sql .= sprintf(" WHERE AgentId='%s'", $agent_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_delete_auditcmd($agent_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("DELETE FROM auditcmd_table WHERE AgentId='%s'", $agent_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   opermode_table   **********************/



function chdbccpark_get_opermode($agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM opermode_table WHERE AgentId='%s'",
		 $agent_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbccpark_insert_opermode($company_id, $agent_id, $server_id,
				    $pad, $pbs, $pbl, $pof, $poe) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "INSERT INTO opermode_table (CompanyId,AgentId,ServerId,";
  $sql .= "PAD,PBS,PBL,POF,POE) VALUES ";
  $sql .= sprintf("('%s','%s','%s',", $company_id, $agent_id, $server_id);
  $sql .= sprintf("%d,%d,%d,%d,%d)", $pad, $pbs, $pbl, $pof, $poe);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_opermode($agent_id, $pad, $pbs, $pbl, $pof, $poe) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE opermode_table SET ";
  $sql .= sprintf("PAD=%d,PBS=%d,PBL=%d,POF=%d,POE=%d",
		  $pad, $pbs, $pbl, $pof, $poe);
  $sql .= sprintf(" WHERE AgentId='%s'", $agent_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   communication_table   **********************/



function chdbccpark_get_communication($agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM communication_table WHERE AgentId='%s'",
		 $agent_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbccpark_insert_comm($company_id, $agent_id, $server_id,
				$cash_total, $alarm_critical, $alarm_major, $alarm_warning,
				$stats_ct, $stats_coin, $stats_bill, $stats_oamvolume,
				$stats_ctvolume, $stats_power, $ip_addr, $port,
				$transaction, $terminal) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "INSERT INTO communication_table (CompanyId,AgentId,ServerId,";
  $sql .= "CashTotalTMO,AlarmCriticalTMO,AlarmMajorTMO,AlarmWarningTMO,";
  $sql .= "StatsCreditTransTMO,StatsCoinTMO,StatsBillTMO,StatsOAMVolumeTMO,";
  $sql .= "StatsCTVolumeTMO,StatsPowerManTMO,CTServerAddr,CTServerPort,";
  $sql .= "TransactionTMO,TerminalId) VALUES ";
  $sql .= sprintf("('%s','%s','%s',", $company_id, $agent_id, $server_id);
  $sql .= sprintf("%d,%d,%d,%d,",
		  $cash_total, $alarm_critical, $alarm_major, $alarm_warning);
  $sql .= sprintf("%d,%d,%d,%d,",
		  $stats_ct, $stats_coin, $stats_bill, $stats_oamvolume);
  $sql .= sprintf("%d,%d,'%s',%d,",
		  $stats_ctvolume, $stats_power, $ip_addr, $port);
  $sql .= sprintf("%d,'%s')", $transaction, $terminal);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_comm($agent_id,
				$pending_cash, $pending_stats, $pending_alarms,
				$pending_ct_tmo, $pending_ct,
				$cash_total, $alarm_critical, $alarm_major, $alarm_warning,
				$stats_ct, $stats_coin, $stats_bill, $stats_oamvolume,
				$stats_ctvolume, $stats_power, $addr, $port,
				$transaction, $terminal) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE communication_table SET ";
  $sql .= sprintf("CashTotalTMO=%d,AlarmCriticalTMO=%d,AlarmMajorTMO=%d,AlarmWarningTMO=%d, ",
		  $cash_total, $alarm_critical, $alarm_major, $alarm_warning);
  $sql .= sprintf("StatsCreditTransTMO=%d,StatsCoinTMO=%d,StatsBillTMO=%d,StatsOAMVolumeTMO=%d, ",
		  $stats_ct, $stats_coin, $stats_bill, $stats_oamvolume);
  $sql .= sprintf("StatsCTVolumeTMO=%d,StatsPowerManTMO=%d,CTServerAddr='%s',CTServerPort=%d, ",
		  $stats_ctvolume, $stats_power, $addr, $port);
  $sql .= sprintf("TransactionTMO=%d,TerminalId='%s', ",
		  $transaction, $terminal);
  $sql .= sprintf("CashTMOPending=%d,AlarmTMOPending=%d,StatsTMOPending=%d, ",
		  $pending_cash ? 1 : 0,
		  $pending_alarms ? 1 : 0,
		  $pending_stats ? 1 : 0);
  $sql .= sprintf("CreditTMOPending=%d,CreditSyncPending=%d",
		  $pending_ct_tmo ? 1 : 0,
		  $pending_ct ? 1 : 0);
  $sql .= sprintf(" WHERE AgentId='%s'", $agent_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_delete_comm($agent_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("DELETE FROM communication_table WHERE AgentId='%s'", $agent_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   buzzer_table   **********************/



function chdbccpark_get_buzzer($agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM buzzer_table WHERE AgentId='%s'",
		 $agent_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbccpark_insert_buzzer($company_id, $agent_id, $server_id,
				  $active, $door_timer, $pin) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "INSERT INTO buzzer_table ";
  $sql .= "(CompanyId,AgentId,ServerId,Active,DoorTimer,PIN) VALUES ";
  $sql .= sprintf("('%s','%s','%s',%d,%d,'%s')",
		  $company_id, $agent_id, $server_id,
		  $active ? 1 : 0, $door_timer, $pin);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_buzzer($agent_id,
				  $active, $door_timer, $pin) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE buzzer_table SET UpdatePending=1,";
  $sql .= sprintf("Active=%d,DoorTimer=%d,PIN='%s'",
		  $active ? 1 : 0, $door_timer, $pin);
  $sql .= sprintf(" WHERE AgentId='%s'", $agent_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_delete_buzzer($agent_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("DELETE FROM buzzer_table WHERE AgentId='%s'", $agent_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   ticketprinter_table   **********************/



function chdbccpark_get_ticketprinter($agent_id, $type) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM ticketprinter_table WHERE AgentId='%s' AND TicketType='%s'",
		 $agent_id, $type);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbccpark_insert_ticketprinter($company_id, $agent_id, $server_id, $type,
					 $header1, $header2, $text1, $text2, $text3) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "INSERT INTO ticketprinter_table ";
  $sql .= "(CompanyId,AgentId,ServerId,TicketType,HeaderLine1,HeaderLine2,";
  $sql .= "Text1,Text2,Text3) VALUES (";
  $sql .= sprintf("'%s','%s','%s','%s','%s','%s',",
		  $company_id, $agent_id, $server_id,
		  $type, $header1, $header2);
  $sql .= sprintf("'%s','%s','%s')", $text1, $text2, $text3);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_ticketprinter($agent_id, $type,
					 $header1, $header2, $text1, $text2, $text3) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE ticketprinter_table SET UpdatePending=1,";
  $sql .= sprintf("HeaderLine1='%s',HeaderLine2='%s',Text1='%s',Text2='%s',Text3='%s'",
		  $header1, $header2, $text1, $text2, $text3);
  $sql .= sprintf(" WHERE AgentId='%s' AND TicketType='%s'", $agent_id, $type);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_delete_ticketprinter($agent_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("DELETE FROM ticketprinter_table WHERE AgentId='%s'", $agent_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   alarm_table   **********************/



function chdbccpark_get_alarms($agent_id, $acknowledged) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_ALARMS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALARMS database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM alarm_table WHERE AgentId='%s' AND Acknowledged=%d",
		 $agent_id, $acknowledged ? 1 : 0);
  if ($acknowledged) {
    $sql .= " AND DATETIME(AcknowledgedTime, '+1 MONTH') > CURRENT_TIMESTAMP";
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbccpark_get_alarm_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_ALARMS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALARMS database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM alarm_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbccpark_get_alarm_severity($agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_ALARMS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALARMS database!!!", 'error');
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
  $sql = "SELECT MAX(Severity) FROM alarm_table WHERE Acknowledged=0";
  $sql .= sprintf(" AND AgentId='%s'", $agent_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbccpark_acknowledge_alarm($id, $uid, $resolution) {
  $db_name = getenv('APPS_CCPARK_ALARMS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALARMS database!!!", 'error');
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
  $sql = "UPDATE alarm_table SET AcknowledgedTime=CURRENT_TIMESTAMP,Acknowledged=1,";
  $sql .= sprintf("AcknowledgedByUID=%d,Resolution='%s'",$uid, $resolution);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_delete_alarms($agent_id) {
  $db_name = getenv('APPS_CCPARK_ALARMS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ALARMS database!!!", 'error');
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
  $sql = sprintf("DELETE FROM alarm_table WHERE AgentId='%s'", $agent_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   maxcharge_table   **********************/



function chdbccpark_get_maxcharges($company_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM maxcharge_table WHERE CompanyId='%s'",
		 $company_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbccpark_get_maxcharge_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM maxcharge_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbccpark_cc_get_admin_container_maxcharge($company_id, $container_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "SELECT * FROM maxcharge_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND ContainerId='%s'",
		  $company_id, $container_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbccpark_cc_get_container_maxcharge($uid, $company_id, $container_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
    return $entries;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $entries;
  }
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
    return $entries;
  }
  // SQL
  $sql = sprintf("ATTACH '%s' AS props", $db_name);
  $db->exec($sql);
  // SQL
  $sql = "SELECT maxcharge.* FROM maxcharge_table AS maxcharge";
  $sql .= " INNER JOIN props.user_table AS user";
  $sql .= "  ON (maxcharge.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d".
		  " AND maxcharge.CompanyId='%s'".
		  " AND maxcharge.ContainerId='%s'",
		  $uid, $company_id, $container_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Retrieve container types based on relations
function chdbccpark_retrieve_container_maxcharge($uid, $company_id, $container_id) {
  $account = user_load(array('uid' => $uid));
  $containers = array();

  // As Admin, get all properties in a relation
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbccpark_cc_get_admin_container_maxcharge($company_id, $container_id) as $container) {
      $containers[$container['RateId']] =
	array('data' => $container,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $containers;
  }

  // Via relations
  foreach (chdbccpark_cc_get_container_maxcharge($uid, $company_id, $container_id) as $container) {
    $containers[$container['RateId']] =
      array('data' => $container,
	    'readonly' => TRUE,
	    'source' => 'Relation');
  }

  return $containers;
}



// Return 0 on success, otherwise 1 
function chdbccpark_insert_maxcharge($company_id, $container_id, $charge_id,
				     $alias, $behavior, $time, $charge,
				     &$id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "INSERT INTO maxcharge_table ";
  $sql .= "(CompanyId,ContainerId,ChargeId,Alias,Behavior,Time,Charge) VALUES ";
  $sql .= sprintf("('%s','%s','%s','%s','%s','%s','%s')",
		  $company_id, $container_id, $charge_id,
		  $alias, $behavior, $time, $charge);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_maxcharge($id, $alias, $behavior, $time, $charge) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE maxcharge_table SET ";
  $sql .= sprintf("Alias='%s',Behavior='%s',Time='%s',Charge='%s'",
		  $alias, $behavior, $time, $charge);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   maxchargemeter_table   **********************/



function chdbccpark_get_maxchargemeter($agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM maxchargemeter_table WHERE AgentId='%s'",
		 $agent_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbccpark_insert_maxchargemeter($company_id, $agent_id, $server_id,
					  $charge_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "INSERT INTO maxchargemeter_table ";
  $sql .= "(CompanyId,AgentId,ServerId,ChargeId,UpdatePending) VALUES ";
  $sql .= sprintf("('%s','%s','%s','%s',1)",
		  $company_id, $agent_id, $server_id, $charge_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_maxchargemeter($company_id, $agent_id, $server_id,
					  $charge_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE maxchargemeter_table SET UpdatePending=1,";
  $sql .= sprintf(" ChargeId='%s'", $charge_id);
  $sql .= sprintf(" WHERE CompanyId='%s' AND AgentId='%s' AND ServerId='%s'",
		  $company_id, $agent_id, $server_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_maxchargemeter_altered($company_id, $charge_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE maxchargemeter_table SET UpdatePending=1";
  $sql .= sprintf(" WHERE CompanyId='%s' AND ChargeId='%s'",
		  $company_id, $charge_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_remove_maxchargemeter($company_id, $agent_id, $server_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE maxchargemeter_table SET DeletePending=1,ChargeId='None'";
  $sql .= sprintf(" WHERE CompanyId='%s' AND AgentId='%s' AND ServerId='%s'",
		  $company_id, $agent_id, $server_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   rate_table   **********************/



function chdbccpark_get_rates($company_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM rate_table WHERE CompanyId='%s'",
		 $company_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbccpark_get_rate_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM rate_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbccpark_cc_get_admin_container_rates($company_id, $container_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "SELECT * FROM rate_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND ContainerId='%s'",
		  $company_id, $container_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbccpark_cc_get_container_rates($uid, $company_id, $container_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
    return $entries;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $entries;
  }
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
    return $entries;
  }
  // SQL
  $sql = sprintf("ATTACH '%s' AS props", $db_name);
  $db->exec($sql);
  // SQL
  $sql = "SELECT rate.* FROM rate_table AS rate";
  $sql .= " INNER JOIN props.user_table AS user";
  $sql .= "  ON (rate.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d AND rate.CompanyId='%s' AND rate.ContainerId='%s'",
		  $uid, $company_id, $container_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Retrieve container types based on relations
function chdbccpark_retrieve_container_rates($uid, $company_id, $container_id) {
  $account = user_load(array('uid' => $uid));
  $containers = array();

  // As Admin, get all properties in a relation
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbccpark_cc_get_admin_container_rates($company_id, $container_id) as $container) {
      $containers[$container['RateId']] =
	array('data' => $container,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $containers;
  }

  // Via relations
  foreach (chdbccpark_cc_get_container_rates($uid, $company_id, $container_id) as $container) {
    $containers[$container['RateId']] =
      array('data' => $container,
	    'readonly' => TRUE,
	    'source' => 'Relation');
  }

  return $containers;
}



// Return 0 on success, otherwise 1 
function chdbccpark_insert_rate($company_id, $container_id, $rate_id, $alias,
				$min_time, $min_charge, $begin, $end, $days,
				$len1, $time1, $charge1, $cutoff1, $maxcharge1, $maxtime1,
				$len2, $time2, $charge2, $cutoff2, $maxcharge2, $maxtime2,
				$len3, $time3, $charge3, $cutoff3, $maxcharge3, $maxtime3,
				$len4, $time4, $charge4, $cutoff4, $maxcharge4, $maxtime4,
				$len5, $time5, $charge5, $cutoff5, $maxcharge5, $maxtime5,
				$len6, $time6, $charge6, $cutoff6, $maxcharge6, $maxtime6,
				$len7, $time7, $charge7, $cutoff7, $maxcharge7, $maxtime7,
				$len8, $time8, $charge8, $cutoff8, $maxcharge8, $maxtime8,
				&$id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "INSERT INTO rate_table ";
  $sql .= "(CompanyId,ContainerId,RateId,Alias,MinTime,MinCharge,Begin,End,Days, ";
  $sql .= "Length1,TimeStep1,ChargeStep1,CutOffTime1,MaxCharge1,MaxTime1, ";
  $sql .= "Length2,TimeStep2,ChargeStep2,CutOffTime2,MaxCharge2,MaxTime2, ";
  $sql .= "Length3,TimeStep3,ChargeStep3,CutOffTime3,MaxCharge3,MaxTime3, ";
  $sql .= "Length4,TimeStep4,ChargeStep4,CutOffTime4,MaxCharge4,MaxTime4, ";
  $sql .= "Length5,TimeStep5,ChargeStep5,CutOffTime5,MaxCharge5,MaxTime5, ";
  $sql .= "Length6,TimeStep6,ChargeStep6,CutOffTime6,MaxCharge6,MaxTime6, ";
  $sql .= "Length7,TimeStep7,ChargeStep7,CutOffTime7,MaxCharge7,MaxTime7, ";
  $sql .= "Length8,TimeStep8,ChargeStep8,CutOffTime8,MaxCharge8,MaxTime8)";
  $sql .= " VALUES ";
  $sql .= sprintf("('%s','%s','%s','%s','%s','%s','%s','%s','%s', ",
		  $company_id, $container_id, $rate_id, $alias,
		  $min_time, $min_charge, $begin, $end, $days);
  $sql .= sprintf("'%s','%s','%s','%s','%s','%s', ",
		  $len1, $time1, $charge1, $cutoff1, $maxcharge1, $maxtime1);
  $sql .= sprintf("'%s','%s','%s','%s','%s','%s', ",
		  $len2, $time2, $charge2, $cutoff2, $maxcharge2, $maxtime2);
  $sql .= sprintf("'%s','%s','%s','%s','%s','%s', ",
		  $len3, $time3, $charge3, $cutoff3, $maxcharge3, $maxtime3);
  $sql .= sprintf("'%s','%s','%s','%s','%s','%s', ",
		  $len4, $time4, $charge4, $cutoff4, $maxcharge4, $maxtime4);
  $sql .= sprintf("'%s','%s','%s','%s','%s','%s', ",
		  $len5, $time5, $charge5, $cutoff5, $maxcharge5, $maxtime5);
  $sql .= sprintf("'%s','%s','%s','%s','%s','%s', ",
		  $len6, $time6, $charge6, $cutoff6, $maxcharge6, $maxtime6);
  $sql .= sprintf("'%s','%s','%s','%s','%s','%s', ",
		  $len7, $time7, $charge7, $cutoff7, $maxcharge7, $maxtime7);
  $sql .= sprintf("'%s','%s','%s','%s','%s','%s')",
		  $len8, $time8, $charge8, $cutoff8, $maxcharge8, $maxtime8);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_rate($id, $alias,
				$min_time, $min_charge, $begin, $end, $days,
				$len1, $time1, $charge1, $cutoff1, $maxcharge1, $maxtime1,
				$len2, $time2, $charge2, $cutoff2, $maxcharge2, $maxtime2,
				$len3, $time3, $charge3, $cutoff3, $maxcharge3, $maxtime3,
				$len4, $time4, $charge4, $cutoff4, $maxcharge4, $maxtime4,
				$len5, $time5, $charge5, $cutoff5, $maxcharge5, $maxtime5,
				$len6, $time6, $charge6, $cutoff6, $maxcharge6, $maxtime6,
				$len7, $time7, $charge7, $cutoff7, $maxcharge7, $maxtime7,
				$len8, $time8, $charge8, $cutoff8, $maxcharge8, $maxtime8) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE rate_table SET ";
  $sql .= sprintf("Alias='%s',MinTime='%s',MinCharge='%s',Begin='%s',End='%s',Days='%s',",
		  $alias, $min_time, $min_charge, $begin, $end, $days);
  $sql .= sprintf("Length1='%s',TimeStep1='%s',ChargeStep1='%s',".
		  "CutOffTime1='%s',MaxCharge1='%s',MaxTime1='%s', ",
		  $len1, $time1, $charge1, $cutoff1, $maxcharge1, $maxtime1);
  $sql .= sprintf("Length2='%s',TimeStep2='%s',ChargeStep2='%s',".
		  "CutOffTime2='%s',MaxCharge2='%s',MaxTime2='%s', ",
		  $len2, $time2, $charge2, $cutoff2, $maxcharge2, $maxtime2);
  $sql .= sprintf("Length3='%s',TimeStep3='%s',ChargeStep3='%s',".
		  "CutOffTime3='%s',MaxCharge3='%s',MaxTime3='%s', ",
		  $len3, $time3, $charge3, $cutoff3, $maxcharge3, $maxtime3);
  $sql .= sprintf("Length4='%s',TimeStep4='%s',ChargeStep4='%s',".
		  "CutOffTime4='%s',MaxCharge4='%s',MaxTime4='%s', ",
		  $len4, $time4, $charge4, $cutoff4, $maxcharge4, $maxtime4);
  $sql .= sprintf("Length5='%s',TimeStep5='%s',ChargeStep5='%s',".
		  "CutOffTime5='%s',MaxCharge5='%s',MaxTime5='%s', ",
		  $len5, $time5, $charge5, $cutoff5, $maxcharge5, $maxtime5);
  $sql .= sprintf("Length6='%s',TimeStep6='%s',ChargeStep6='%s',".
		  "CutOffTime6='%s',MaxCharge6='%s',MaxTime6='%s', ",
		  $len6, $time6, $charge6, $cutoff6, $maxcharge6, $maxtime6);
  $sql .= sprintf("Length7='%s',TimeStep7='%s',ChargeStep7='%s',".
		  "CutOffTime7='%s',MaxCharge7='%s',MaxTime7='%s', ",
		  $len7, $time7, $charge7, $cutoff7, $maxcharge7, $maxtime7);
  $sql .= sprintf("Length8='%s',TimeStep8='%s',ChargeStep8='%s',".
		  "CutOffTime8='%s',MaxCharge8='%s',MaxTime8='%s'",
		  $len8, $time8, $charge8, $cutoff8, $maxcharge8, $maxtime8);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_delete_rate($company_id, $container_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "DELETE FROM rate_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND ContainerId='%s'",
		  $company_id, $container_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   ratemeter_table   **********************/



function chdbccpark_get_ratemeters($agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM ratemeter_table WHERE AgentId='%s'",
		 $agent_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbccpark_insert_ratemeter($company_id, $agent_id, $server_id, $group,
				     $rate_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "INSERT INTO ratemeter_table ";
  $sql .= "(CompanyId,AgentId,ServerId,RateId,GroupId,UpdatePending) VALUES ";
  $sql .= sprintf("('%s','%s','%s','%s','%s',1)",
		  $company_id, $agent_id, $server_id, $rate_id, $group);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_ratemeter($company_id, $agent_id, $server_id, $group,
				     $rate_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE ratemeter_table SET DeletePending=0,UpdatePending=1,";
  $sql .= sprintf(" RateId='%s'", $rate_id);
  $sql .= sprintf(" WHERE CompanyId='%s' AND AgentId='%s' AND ServerId='%s' AND GroupId='%s'",
		  $company_id, $agent_id, $server_id, $group);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_ratemeter_altered($company_id, $rate_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE ratemeter_table SET UpdatePending=1";
  $sql .= sprintf(" WHERE CompanyId='%s' AND RateId='%s'",
		  $company_id, $rate_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_remove_ratemeter($company_id, $agent_id, $server_id, $group) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE ratemeter_table SET UpdatePending=0,DeletePending=1,RateId='None'";
  $sql .= sprintf(" WHERE CompanyId='%s' AND AgentId='%s' AND ServerId='%s' AND GroupId='%s'",
		  $company_id, $agent_id, $server_id, $group);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   pass_table   **********************/



function chdbccpark_get_passes($company_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM pass_table WHERE CompanyId='%s'",
		 $company_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbccpark_get_pass_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM pass_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbccpark_cc_get_admin_container_passes($company_id, $container_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "SELECT * FROM pass_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND ContainerId='%s'",
		  $company_id, $container_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbccpark_cc_get_container_passes($uid, $company_id, $container_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
    return $entries;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $entries;
  }
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
    return $entries;
  }
  // SQL
  $sql = sprintf("ATTACH '%s' AS props", $db_name);
  $db->exec($sql);
  // SQL
  $sql = "SELECT pass.* FROM pass_table AS pass";
  $sql .= " INNER JOIN props.user_table AS user";
  $sql .= "  ON (pass.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d".
		  " AND pass.CompanyId='%s'".
		  " AND pass.ContainerId='%s'",
		  $uid, $company_id, $container_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Retrieve container types based on relations
function chdbccpark_retrieve_container_passes($uid, $company_id, $container_id) {
  $account = user_load(array('uid' => $uid));
  $containers = array();

  // As Admin, get all properties in a relation
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbccpark_cc_get_admin_container_passes($company_id, $container_id) as $container) {
      $containers[$container['PassId']] =
	array('data' => $container,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $containers;
  }

  // Via relations
  foreach (chdbccpark_cc_get_container_passes($uid, $company_id, $container_id) as $container) {
    $containers[$container['PassId']] =
      array('data' => $container,
	    'readonly' => TRUE,
	    'source' => 'Relation');
  }

  return $containers;
}



// Return 0 on success, otherwise 1 
function chdbccpark_insert_pass($company_id, $container_id, $pass_id,
				$alias, $duration, $charge, $menu,
				&$id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "INSERT INTO pass_table ";
  $sql .= "(CompanyId,ContainerId,PassId,Alias,Duration,Charge,MenuName) VALUES ";
  $sql .= sprintf("('%s','%s','%s','%s','%s','%s','%s')",
		  $company_id, $container_id, $pass_id,
		  $alias, $duration, $charge, $menu);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_pass($id, $alias, $duration, $charge, $menu) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE pass_table SET ";
  $sql .= sprintf("Alias='%s',Duration='%s',Charge='%s',MenuName='%s'",
		  $alias, $duration, $charge, $menu);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   passmeter_table   **********************/



function chdbccpark_get_passmeters($agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM passmeter_table WHERE AgentId='%s'",
		 $agent_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbccpark_insert_passmeter($company_id, $agent_id, $server_id, $number,
				     $pass_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "INSERT INTO passmeter_table ";
  $sql .= "(CompanyId,AgentId,ServerId,PassId,Number,UpdatePending) VALUES ";
  $sql .= sprintf("('%s','%s','%s','%s',%d,1)",
		  $company_id, $agent_id, $server_id, $pass_id, $number);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_passmeter($company_id, $agent_id, $server_id, $number,
				     $pass_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE passmeter_table SET DeletePending=0,UpdatePending=1,";
  $sql .= sprintf(" PassId='%s'", $pass_id);
  $sql .= sprintf(" WHERE CompanyId='%s' AND AgentId='%s' AND ServerId='%s' AND Number=%d",
		  $company_id, $agent_id, $server_id, $number);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_passmeter_altered($company_id, $pass_id) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE passmeter_table SET UpdatePending=1";
  $sql .= sprintf(" WHERE CompanyId='%s' AND PassId='%s'",
		  $company_id, $pass_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_remove_passmeter($company_id, $agent_id, $server_id, $number) {
  $db_name = getenv('APPS_CCPARK_CONTROL_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for CONTROL database!!!", 'error');
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
  $sql = "UPDATE passmeter_table SET UpdatePending=0,DeletePending=1,PassId='None'";
  $sql .= sprintf(" WHERE CompanyId='%s' AND AgentId='%s' AND ServerId='%s' AND Number=%d",
		  $company_id, $agent_id, $server_id, $number);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   merchantcode_table   **********************/



function chdbccpark_get_merchantcodes($company_id, $container_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_SPACE_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for SPACE database!!!", 'error');
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
  $sql = "SELECT * FROM merchantcode_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND ContainerId='%s'",
		 $company_id, $container_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbccpark_get_merchantcode_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_SPACE_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for SPACE database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM merchantcode_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbccpark_cc_get_admin_container_merchantcodes($company_id, $container_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_SPACE_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for SPACE database!!!", 'error');
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
  $sql = "SELECT * FROM merchantcode_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND ContainerId='%s'",
		  $company_id, $container_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbccpark_cc_get_container_merchantcodes($uid, $company_id, $container_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_SPACE_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for SPACE database!!!", 'error');
    return $entries;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $entries;
  }
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
    return $entries;
  }
  // SQL
  $sql = sprintf("ATTACH '%s' AS props", $db_name);
  $db->exec($sql);
  // SQL
  $sql = "SELECT merchantcode.* FROM merchantcode_table AS merchantcode";
  $sql .= " INNER JOIN props.user_table AS user";
  $sql .= "  ON (merchantcode.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d AND merchantcode.CompanyId='%s' AND merchantcode.ContainerId='%s'",
		  $uid, $company_id, $container_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Retrieve container types based on relations
function chdbccpark_retrieve_container_merchantcodes($uid, $company_id, $container_id) {
  $account = user_load(array('uid' => $uid));
  $containers = array();

  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbccpark_cc_get_admin_container_merchantcodes($company_id, $container_id) as $container) {
      $containers[$container['Code']] =
	array('data' => $container,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $containers;
  }

  // Via relations
  foreach (chdbccpark_cc_get_container_merchantcodes($uid, $company_id, $container_id) as $container) {
    $containers[$container['Code']] =
      array('data' => $container,
	    'readonly' => TRUE,
	    'source' => 'Relation');
  }

  return $containers;
}



// Return 0 on success, otherwise 1 
function chdbccpark_insert_merchantcode($company_id, $container_id, $type_id,
					$code, $mode, $amount, $percentage,
					&$id) {
  $db_name = getenv('APPS_CCPARK_SPACE_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for SPACE database!!!", 'error');
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
  $sql = "INSERT INTO merchantcode_table ";
  $sql .= "(CompanyId,ContainerId,TypeId,Code,Mode,Amount,Percentage) VALUES ";
  $sql .= sprintf("('%s','%s','%s','%s','%s','%s',%d)",
		  $company_id, $container_id, $type_id,
		  $code, $mode, $amount, $percentage);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_update_merchantcode_id($id, $code, $mode, $amount, $percentage) {
  $db_name = getenv('APPS_CCPARK_SPACE_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for SPACE database!!!", 'error');
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
  $sql = "UPDATE merchantcode_table SET ";
  $sql .= sprintf("Code='%s',Mode='%s',Amount='%s',Percentage=%d",
		  $code, $mode, $amount, $percentage);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbccpark_delete_merchantcode_id($id) {
  $db_name = getenv('APPS_CCPARK_SPACE_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for SPACE database!!!", 'error');
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
  $sql = "DELETE FROM merchantcode_table";
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   merchantcodemeter_table   **********************/



// Tore 131116: Added
function chdbccpark_get_merchantcodemeter($agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_SPACE_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for SPACE database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM merchantcodemeter_table WHERE AgentId='%s'",
		 $agent_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Tore 131116: Added
// Return 0 on success, otherwise 1 
function chdbccpark_insert_merchantcodemeter($company_id, $agent_id, $server_id, $type_id) {
  $db_name = getenv('APPS_CCPARK_SPACE_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for SPACE database!!!", 'error');
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
  $sql = "INSERT INTO merchantcodemeter_table ";
  $sql .= "(CompanyId,AgentId,ServerId,TypeId,UpdatePending) VALUES ";
  $sql .= sprintf("('%s','%s','%s','%s',1)",
		  $company_id, $agent_id, $server_id, $type_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Tore 131116: Added
// Return 0 on success, otherwise 1 
function chdbccpark_update_merchantcodemeter($company_id, $agent_id, $server_id, $type_id) {
  $db_name = getenv('APPS_CCPARK_SPACE_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for SPACE database!!!", 'error');
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
  $sql = "UPDATE merchantcodemeter_table SET UpdatePending=1,";
  $sql .= sprintf(" TypeId='%s'", $type_id);
  $sql .= sprintf(" WHERE CompanyId='%s' AND AgentId='%s' AND ServerId='%s'",
		  $company_id, $agent_id, $server_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Tore 131201: Added
// Return 0 on success, otherwise 1 
function chdbccpark_update_merchantcodemeter_alt($company_id, $type_id) {
  $db_name = getenv('APPS_CCPARK_SPACE_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for SPACE database!!!", 'error');
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
  // PY : Feb 18.14 - fixed syntax
  //$sql = "UPDATE merchantcodemeter_table SET UpdatePending=1,";
  $sql = "UPDATE merchantcodemeter_table SET UpdatePending=1";
  $sql .= sprintf(" WHERE CompanyId='%s' AND TypeId='%s'",
		  $company_id, $type_id);
  // PY : Feb 18.14 - this is commented except when wanting to see message
  //drupal_set_message($sql);
  try { // Tore 140113: Swapped to use exception handler
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



// Tore 131116: Added
// Return 0 on success, otherwise 1 
function chdbccpark_remove_merchantcodemeter($company_id, $agent_id, $server_id) {
  $db_name = getenv('APPS_CCPARK_SPACE_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for SPACE database!!!", 'error');
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
  $sql = "UPDATE merchantcodemeter_table SET DeletePending=1,TypeId='None'";
  $sql .= sprintf(" WHERE CompanyId='%s' AND AgentId='%s' AND ServerId='%s'",
		  $company_id, $agent_id, $server_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   auditmeta_table   **********************/



function chdbccpark_get_last_auditmeta($agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_AUDITS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AUDITS database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM auditmeta_table WHERE AgentId='%s' ORDER BY Id DESC LIMIT 1",
		 $agent_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



/****************   auditcoins_table   **********************/



function chdbccpark_get_last_auditcoins($agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_AUDITS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AUDITS database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM auditcoins_table WHERE AgentId='%s' ORDER BY Id DESC LIMIT 1",
		 $agent_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



/****************   auditbills_table   **********************/



function chdbccpark_get_last_auditbills($agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_AUDITS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AUDITS database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM auditbills_table WHERE AgentId='%s' ORDER BY Id DESC LIMIT 1",
		 $agent_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



/****************   composite tables   **********************/



function chdbccpark_cc_get_last_purchases($agent_id, $limit = 10) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_AUDITS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AUDITS database!!!", 'error');
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
  $sql = "SELECT TransactionTime,TicketNo,TransactionId,CardType,Card,Amount FROM auditct_table";
  $sql .= sprintf(" WHERE AgentId='%s'", $agent_id);
  $sql .= " UNION ";
  $sql .= "SELECT TransactionTime,TicketNo,NULL,NULL,NULL,Amount FROM auditcash_table";
  $sql .= sprintf(" WHERE AgentId='%s'", $agent_id);
  $sql .= sprintf(" ORDER BY TransactionTime DESC LIMIT %d", $limit);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbccpark_cc_get_creditcard_report($start_time, $stop_time,
					     $agent_id_array = array()) {
  $entries = array();
  if (empty($agent_id_array)) {
    return $entries;
  }
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_AUDITS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AUDITS database!!!", 'error');
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
  $sql = "SELECT * FROM auditct_table WHERE (";
  foreach ($agent_id_array as $agent_id) {
    $sql .= sprintf("AgentId='%s' OR ", $agent_id);
  }
  $sql = rtrim($sql, ' OR ');
  $sql .= ")";
  $sql .= sprintf(" AND TransactionTime>'%s'", $start_time);
  $sql .= sprintf(" AND TransactionTime<'%s'", $stop_time);
  $sql .= " ORDER BY TransactionTime";
  //drupal_set_message($sql);
  //var_export($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbccpark_cc_get_cash_report($start_time, $stop_time,
				       $agent_id_array = array()) {
  $entries = array();
  if (empty($agent_id_array)) {
    return $entries;
  }
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_AUDITS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AUDITS database!!!", 'error');
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
  $sql = "SELECT * FROM auditcash_table WHERE (";
  foreach ($agent_id_array as $agent_id) {
    $sql .= sprintf("AgentId='%s' OR ", $agent_id);
  }
  $sql = rtrim($sql, ' OR ');
  $sql .= ")";
  $sql .= sprintf(" AND TransactionTime>'%s'", $start_time);
  $sql .= sprintf(" AND TransactionTime<'%s'", $stop_time);
  $sql .= " ORDER BY TransactionTime";
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbccpark_cc_get_purchase_report($start_time, $stop_time,
					   $agent_id_array = array()) {
  $entries = array();
  if (empty($agent_id_array)) {
    return $entries;
  }
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_AUDITS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AUDITS database!!!", 'error');
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
  $sql = "SELECT AgentId,TransactionTime,TicketNo,TransactionId,CardType,Card,Amount,";
  $sql .= "LPN,Stall,MerchantCode,Duration,Completed FROM auditct_table WHERE (";
  foreach ($agent_id_array as $agent_id) {
    $sql .= sprintf("AgentId='%s' OR ", $agent_id);
  }
  $sql = rtrim($sql, ' OR ');
  $sql .= ")";
  $sql .= sprintf(" AND TransactionTime>'%s'", $start_time);
  $sql .= sprintf(" AND TransactionTime<'%s'", $stop_time);
  $sql .= " UNION ";
  $sql .= "SELECT AgentId,TransactionTime,TicketNo,NULL,'Cash',NULL,Amount,";
  $sql .= "LPN,Stall,MerchantCode,Duration,NULL FROM auditcash_table WHERE (";
  foreach ($agent_id_array as $agent_id) {
    $sql .= sprintf("AgentId='%s' OR ", $agent_id);
  }
  $sql = rtrim($sql, ' OR ');
  $sql .= ")";
  $sql .= sprintf(" AND TransactionTime>'%s'", $start_time);
  $sql .= sprintf(" AND TransactionTime<'%s'", $stop_time);
  $sql .= " ORDER BY TransactionTime";
  //drupal_set_message($sql);
  //var_export($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Tore 131225: Added
function chdbccpark_cc_get_merchantcode_report($start_time, $stop_time,
					       $merchant_code,
					       $agent_id_array = array()) {
  $entries = array();
  if (empty($agent_id_array)) {
    return $entries;
  }
  // Find location of SQLite database
  $db_name = getenv('APPS_CCPARK_AUDITS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for AUDITS database!!!", 'error');
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
  $sql = "SELECT AgentId,TransactionTime,TicketNo,TransactionId,CardType,Card,Amount,";
  $sql .= "LPN,Stall,MerchantCode,Duration,Completed FROM auditct_table WHERE (";
  foreach ($agent_id_array as $agent_id) {
    $sql .= sprintf("AgentId='%s' OR ", $agent_id);
  }
  $sql = rtrim($sql, ' OR ');
  $sql .= ")";
  if ($merchant_code === 'ANY') {
    $sql .= " AND MerchantCode NOT NULL";
  } else {
    $sql .= sprintf(" AND MerchantCode='%s'", $merchant_code);
  }
  $sql .= sprintf(" AND TransactionTime>'%s'", $start_time);
  $sql .= sprintf(" AND TransactionTime<'%s'", $stop_time);
  $sql .= " UNION ";
  $sql .= "SELECT AgentId,TransactionTime,TicketNo,NULL,'Cash',NULL,Amount,";
  $sql .= "LPN,Stall,MerchantCode,Duration,NULL FROM auditcash_table WHERE (";
  foreach ($agent_id_array as $agent_id) {
    $sql .= sprintf("AgentId='%s' OR ", $agent_id);
  }
  $sql = rtrim($sql, ' OR ');
  $sql .= ")";
  if ($merchant_code === 'ANY') {
    $sql .= " AND MerchantCode NOT NULL";
  } else {
    $sql .= sprintf(" AND MerchantCode='%s'", $merchant_code);
  }
  $sql .= sprintf(" AND TransactionTime>'%s'", $start_time);
  $sql .= sprintf(" AND TransactionTime<'%s'", $stop_time);
  $sql .= " ORDER BY TransactionTime";
  //drupal_set_message($sql);
  //var_export($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



