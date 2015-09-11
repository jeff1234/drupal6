<?php
// $Id$



function chdbscan_get_lpn_master($days = 'ANY') {
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
  $sql = 'SELECT * FROM lpnfile_table ORDER BY Id DESC';
  if ($days !== 'ANY') {
    $sql .= sprintf(" LIMIT %d", $days);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry['DB'];
  }
  return $entries;
}



function chdbscan_get_all_lpn($lpn) {
  $entries = array();
  $lpn_files = chdbscan_get_lpn_master();
  $sql = sprintf("SELECT * FROM lpn_table WHERE LPN='%s'", $lpn);
  //drupal_set_message($sql);
  foreach ($lpn_files as $lpn_file) {
    // connect to SQLite database
    try {
      $db = new PDO("sqlite:".$lpn_file);
    } catch(PDOException $e) {
      drupal_set_message($e->getMessage(), 'error');
      return $entries;
    }
    foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
      $entries[] = array_merge($entry, array('DB' => $lpn_file));
    }
  }
  return $entries;
}



function chdbscan_get_lpn_window($lpn, $start_time, $stop_time) {
  $entries = array();
  $lpn_files = chdbscan_get_lpn_master();
  $sql = "SELECT * FROM lpn_table WHERE ";
  $sql .= sprintf("LPN='%s' AND AgentTime>='%s'",
		  $lpn, $start_time);
  if ($stop_time !== 'ANY') {
    $sql .= sprintf("AND AgentTime<='%s'", $stop_time);
  }
  //drupal_set_message($sql);
  foreach ($lpn_files as $lpn_file) {
    // connect to SQLite database
    try {
      $db = new PDO("sqlite:".$lpn_file);
    } catch(PDOException $e) {
      drupal_set_message($e->getMessage(), 'error');
      return $entries;
    }
    foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
      $entries[] = array_merge($entry, array('DB' => $lpn_file));
    }
  }
  return $entries;
}



// 
function chdbscan_get_scans($properties, $lots, $lanes,
			    $start_time, $stop_time,
			    $lpn = 'ANY') {
  $entries = array();
  // Empty conditions are not accepted
  if (empty($lanes) or empty($properties) or empty($lots)) {
    return $entries;
  }

  if ($start_time !== 'ANY') {
    $datetime1 = new DateTime($start_time);
    if ($stop_time !== 'ANY') {
      $datetime2 = new DateTime($stop_time);
    } else {
      $datetime2 = new DateTime('now');
      $datetime2->setTimezone(new DateTimeZone('UTC'));
    }
    $interval = $datetime1->diff($datetime2);
    $lpn_files = chdbscan_get_lpn_master($interval->d + 2);
  } else {
    $lpn_files = chdbscan_get_lpn_master();
  }

  $sql = "SELECT * FROM lpn_table WHERE (";
  foreach ($properties as $entry) {
    $sql .= sprintf("PropertyId='%s' OR ", $entry);
  }
  $sql = rtrim($sql, " OR ");
  $sql .= ") AND (";
  foreach ($lots as $entry) {
    $sql .= sprintf("LotId='%s' OR ", $entry);
  }
  $sql = rtrim($sql, " OR ");
  $sql .= ") AND (";
  foreach ($lanes as $entry) {
    $sql .= sprintf("LaneId='%s' OR ", $entry);
  }
  $sql = rtrim($sql, " OR ");
  $sql .= ")";
  if ($lpn !== 'ANY') {
    $sql .= sprintf(" AND LPN='%s'", $lpn);
  }
  if ($start_time !== 'ANY') {
    $sql .= sprintf(" AND AgentTime>='%s'", $start_time);
  }
  if ($stop_time !== 'ANY') {
    $sql .= sprintf("AND AgentTime<='%s'", $stop_time);
  }
  $sql .= " ORDER BY Id DESC";
  //drupal_set_message($sql);
  foreach ($lpn_files as $lpn_file) {
    // connect to SQLite database
    try {
      $db = new PDO("sqlite:".$lpn_file);
    } catch(PDOException $e) {
      drupal_set_message($e->getMessage(), 'error');
      return $entries;
    }
    foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
      $entries[] = array_merge($entry, array('DB' => $lpn_file));
    }
  }
  return $entries;
}



function chdbscan_get_scan_id($id, $db) {
  $entries = array();
  // Find location of SQLite database
  $db_name = $db;
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $entries;
  }

  $sql = sprintf("SELECT * FROM lpn_table WHERE Id=%d", $id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbscan_get_lpimg_id($id, $db) {
  $lpimg = array();
  $lpimg['Format'] = 'Text';
  $lpimg['Text'] = 'Not found';

  // Find location of SQLite database
  $db_name = $db;

  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $lpimg;
  }

  $sql = sprintf("SELECT Size,Img FROM lpimg_table WHERE Id=%d", $id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $lpimg['Format'] = 'LPImg';
    $lpimg['LPImg'] = $entry['Img'];
    $lpimg['LPImgSize'] = $entry['Size'];
  }
  return $lpimg;
}



function chdbscan_get_carimg_id($id, $db) {
  $lpimg = array();
  $lpimg['Format'] = 'Text';
  $lpimg['Text'] = 'Not found';

  // Find location of SQLite database
  $db_name = $db;

  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $lpimg;
  }
  // SQL
  $sql = sprintf("SELECT Size,Img FROM carimg_table WHERE Id=%d", $id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $lpimg['Format'] = 'CarImg';
    $lpimg['CarImg'] = $entry['Img'];
    $lpimg['CarImgSize'] = $entry['Size'];
  }
  return $lpimg;
}



// Return 0 on success, otherwise 1 
function chdbscan_update_scan_id($id, $db, $lpn) {
  // Find location of SQLite database
  $db_name = $db;
  //drupal_set_message($db);
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // SQL
  $sql = "UPDATE lpn_table SET ";
  $sql .= sprintf("LPN='%s'", $lpn);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbscan_delete_scan_id($id, $db) {
  // Find location of SQLite database
  $db_name = $db;
  //drupal_set_message($db);
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // SQL
  $sql = "DELETE FROM lpn_table";
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbscan_delete_lpimg_id($id, $db) {
  // Find location of SQLite database
  $db_name = $db;
  //drupal_set_message($db);
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // SQL
  $sql = "DELETE FROM lpimg_table";
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbscan_delete_carimg_id($id, $db) {
  // Find location of SQLite database
  $db_name = $db;
  //drupal_set_message($db);
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // SQL
  $sql = "DELETE FROM carimg_table";
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



