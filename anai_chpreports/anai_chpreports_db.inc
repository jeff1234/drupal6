<?php


function chdbstats_get_last_report($uid) {
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
  // SQL
  $sql = sprintf("SELECT * FROM reportstats_table WHERE Active=1 AND UID=%d", $uid);
  //drupal_set_message($sql);
  //print_r($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Tore 131225: Added MerchantCode
// Return 0 on success, otherwise 1
function chdbstats_insert_report($uid,
				 $company_id, $region_id, $client_id,
				 $property_id, $lot_id, $lot_type, $lot_class,
				 $category, $report_type,
				 $permit_class, $compliancy, $group_id, $agent_id,
				 $lane_id, $visitype, $length, $benefit,
				 $merchant_code,
				 $start_time, $stop_time) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_STATS_DB');
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
  // SQL
  $sql = "INSERT INTO reportstats_table (UID,CompanyId,RegionId,ClientId, ";
  $sql .= "PropertyId,LotId,LotType,LotClass, ";
  $sql .= "Category,ReportType,PermitClass,Compliancy,GroupId, ";
  $sql .= "AgentId,LaneId,VisitType,Length,Benefit,MerchantCode) ";
  $sql .= sprintf(" VALUES (%d,'%s','%s','%s', ", $uid, $company_id, $region_id, $client_id);
  $sql .= sprintf("'%s','%s','%s','%s', ", $property_id, $lot_id, $lot_type, $lot_class);
  $sql .= sprintf("'%s','%s', ", $category, $report_type);
  $sql .= sprintf("'%s','%s','%s', ", $permit_class, $compliancy, $group_id);
  $sql .= sprintf("'%s','%s', ", $agent_id, $lane_id);
  $sql .= sprintf("'%s','%s','%s', ", $visitype, $length, $benefit);
  $sql .= sprintf("'%s')", $merchant_code);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  // SQL
  $sql = "UPDATE reportstats_table SET ";
  if (NULL === $start_time) {
    $sql .= "StartTime=NULL";
  } else {
    $sql .= sprintf("StartTime='%s'", $start_time, $stop_time);
  }
  $sql .= ",";
  if (NULL === $stop_time) {
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
function chdbstats_update_report_inactive($uid) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_STATS_DB');
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
  // SQL
  $sql = sprintf("UPDATE reportstats_table SET Active=0 WHERE UID=%d", $uid);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



function chdbstats_cc_get_loopscount_report($start_time, $stop_time,
					    $lane_id_array = array()) {
  $entries = array();
  if (empty($lane_id_array)) {
    return $entries;
  }
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_STATS_DB');
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

  // Ending values
  foreach ($lane_id_array as $lane_id) {
    // SQL 
    $sql = sprintf("SELECT * FROM loopscount_table WHERE LaneId='%s'", $lane_id);
    $sql .= sprintf(" AND CreateTime<'%s'", $stop_time);
    $sql .= " ORDER BY Id DESC LIMIT 1";
    //drupal_set_message($sql);
    //print($sql.'<br>');
    foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
      $entries[$lane_id]['Stop'] = $entry['Count'];
    }
  }

  // Starting values
  foreach ($lane_id_array as $lane_id) {
    // SQL 
    $sql = sprintf("SELECT * FROM loopscount_table WHERE LaneId='%s'", $lane_id);
    $sql .= sprintf(" AND CreateTime>'%s'", $start_time);
    $sql .= " ORDER BY Id LIMIT 1";
    //drupal_set_message($sql);
    //print($sql.'<br>');
    foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
      $entries[$lane_id]['Start'] = $entry['Count'];
    }
  }

  return $entries;
}



