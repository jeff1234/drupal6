<?php
// $Id$


/****************   Ticket Table   **********************/



function chdbtick_get_all_tickets($company_id,
				  $active = TRUE, $confirmed = TRUE,
				  $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TICKET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
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
  $sql = "SELECT * FROM ticket_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND Active=%d AND Confirmed=%d",
		  $company_id, $active ? 1 : 0, $confirmed ? 1 : 0);
  if (strcmp($order_by, 'None')) {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbtick_get_tickets($company_id, $enforcer_id, $property_id, $lot_id,
			      $active = TRUE, $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TICKET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
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
  $sql = "SELECT * FROM ticket_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND EnforcerId='%s'", $company_id, $enforcer_id);
  $sql .= sprintf(" AND PropertyId='%s' AND LotId='%s' AND Active=%d",
		  $property_id, $lot_id, $active ? 1 : 0);
  if (strcmp($order_by, 'None')) {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbtick_get_ticket($company_id, $enforcer_id, $ticket_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TICKET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM ticket_table WHERE Active=1 AND ");
  $sql .= sprintf("CompanyId='%s' AND EnforcerId='%s' AND TicketId='%s'",
		  $company_id, $enforcer_id, $ticket_id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbtick_get_ticket_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TICKET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM ticket_table WHERE Id=%d", $id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbtick_get_tickets_lpn($company_id, $enforcer_id, $lpn,
				  $newer_than = 'Any',
				  $filter_active = FALSE, $active = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TICKET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
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
  $sql = "SELECT * FROM ticket_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND EnforcerId='%s' AND LPN='%s'",
		  $company_id, $enforcer_id, $lpn);
  if ($newer_than !== 'Any') {
    $sql .= sprintf(" AND CreateTime > '%s'", $newer_than);
  }
  if ($filter_active) {
    $sql .= sprintf(" AND Active=%d", $active ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbtick_insert_ticket($company_id, $enforcer_id, $property_id, $lot_id,
				$admin_id, $ticket_id, $manual_time = NULL,
				$code, $description, $amount,
				$lpn, $make, $model, $color,
				$comment, $header, $company,
				$addr1, $addr2, $addr3, $addr4, $addr5,
				$disclaimer, $width,
				&$id) {
  $db_name = getenv('APPS_LPMS_TICKET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
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
  $sql = "INSERT INTO ticket_table (TicketTime,CompanyId,EnforcerId,PropertyId,LotId, ";
  $sql .= "AdminId,TicketId, ";
  $sql .= "Code,Description,Amount, ";
  $sql .= "LPN,Make,Model,Color, ";
  $sql .= "Comment,Header,Company,Addr1,Addr2,Addr3,Addr4,Addr5,Disclaimer,";
  $sql .= "Width) VALUES (";
  $sql .= sprintf("'CURRENT_TIMESTAMP','%s','%s','%s','%s', ",
		  $company_id, $enforcer_id, $property_id, $lot_id);
  $sql .= sprintf("'%s','%s', ", $admin_id, $ticket_id);
  $sql .= sprintf("'%s','%s','%s', ", $code, $description, $amount);
  $sql .= sprintf("'%s','%s','%s','%s', ", $lpn, $make, $model, $color);
  $sql .= sprintf("'%s','%s','%s', ", $comment, $header, $company);
  $sql .= sprintf("'%s','%s','%s','%s','%s', ", $addr1, $addr2, $addr3, $addr4, $addr5);
  $sql .= sprintf("'%s','%s')", $disclaimer, $width);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  // SQL
  if ($manual_time !== NULL) {
    $sql = sprintf("UPDATE ticket_table SET ManualTime='%s' WHERE Id=%d",
		   $manual_time, $id);
    if (!$db->exec($sql)) {
      drupal_set_message("Not able to write to DB", 'error');
      return 1;
    }
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbtick_update_ticket_paid($id, $resolution, $paid = TRUE) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TICKET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
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
  $sql = "UPDATE ticket_table SET PaidTime=CURRENT_TIMESTAMP,";
  $sql .= sprintf("Paid=%d,Resolution='%s' WHERE Id=%d",
		  $paid ? 1 : 0, $resolution, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbtick_update_ticket_active($id, $resolution) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TICKET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
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
  $sql = sprintf("UPDATE ticket_table SET Active=0,Resolution='%s' WHERE Id=%d",
		 $resolution, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



function chdbtick_get_tickets_report($start_time, $stop_time,
				     $company_id, $property_id,
				     $filter_lot = FALSE, $lot_id = '',
				     $filter_paid = FALSE, $paid = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TICKET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
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
  $sql = "SELECT * FROM ticket_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND PropertyId='%s'",
		  $company_id, $property_id);

  if ($filter_lot) {
    $sql .= sprintf(" AND LotId='%s'", $lot_id);
  }

  if ($filter_paid) {
    $sql .= sprintf(" AND Paid=%d", $paid ? 1 : 0);
  }

  $sql .= sprintf(" AND CreateTime > '%s' AND CreateTime < '%s'", $start_time, $stop_time);
  //drupal_set_message($sql);
  //print_r($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 1 if exists, otherwise 0
function chdbtick_ticket_exists($company_id, $ticket_id) {
  $entry = chdbtick_get_ticket($company_id, $ticket_id);
  if (sizeof($entry) > 0) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbtick_delete_ticket_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TICKET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
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
  $sql = sprintf("DELETE FROM ticket_table WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbtick_update_tickets_to_retention_when_paid() {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TICKET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
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
  $sql = sprintf("UPDATE ticket_table SET Active=0 WHERE Active=1 AND Paid=1");
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbtick_retire_tickets_retention_older_than($time, $retention_days) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TICKET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
    return 1;
  }
  $db_history_name = getenv('APPS_LPMS_HISTORY_DB');
  if ($db_history_name == FALSE) {
    drupal_set_message("Missing HTTP environment for HISTORY database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  try {
    $db_history = new PDO("sqlite:".$db_history_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Retrieve from permit db
  $sql = "SELECT * FROM ticket_table WHERE Active=0 AND ";
  $sql .= sprintf("DATETIME(CreateTime,'%d DAYS') < '%s'",
		  $retention_days, $time);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    // Insert into history db
    $sqli = "INSERT INTO ticket_table (CreateTime,TicketTime,ManualTime,CompanyId,PatrollerId,";
    $sqli .= "TicketId,PropertyId,LotId,Code,Description,Amount,LPN,Make,Model,Color,Active,";
    $sqli .= "Comment,Confirmed,Paid,PaidTime,CopyManual,Resolution,Header,";
    $sqli .= "Company,Addr1,Addr2,Addr3,Addr4,Addr5,Disclaimer) ";
    $sqli .= sprintf(" VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%d,'%s',%d,%d,'%s',%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s')",
		     $entry['CreateTime'],
		     $entry['TicketTime'],
		     $entry['ManualTime'],
		     $entry['CompanyId'],
		     $entry['PatrollerId'],
		     $entry['TicketId'],
		     $entry['PropertyId'],
		     $entry['LotId'],
		     $entry['Code'],
		     $entry['Description'],
		     $entry['Amount'],
		     $entry['LPN'],
		     $entry['Make'],
		     $entry['Model'],
		     $entry['Color'],
		     $entry['Active'],
		     $entry['Comment'],
		     $entry['Confirmed'],
		     $entry['Paid'],
		     $entry['PaidTime'],
		     $entry['CopyManual'],
		     $entry['Resolution'],
		     $entry['Header'],
		     $entry['Company'],
		     $entry['Addr1'],
		     $entry['Addr2'],
		     $entry['Addr3'],
		     $entry['Addr4'],
		     $entry['Addr5'],
		     $entry['Disclaimer']);
    if (!$db_history->exec($sqli)) {
      return 1;
    }
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbtick_delete_tickets_retention_older_than($time, $retention_days) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TICKET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
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
  $sql = sprintf("DELETE FROM ticket_table WHERE Active=0 AND DATETIME(CreateTime,'%d DAYS') < '%s'",
		 $retention_days, $time);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   History table   **********************/



function chdbtick_get_master_history($max_entries = 0) {
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



function chdbtick_get_history_tickets($start_time, $stop_time,
				      $company_id, $property_id,
				      $filter_lot = FALSE, $lot_id = '',
				      $filter_paid = FALSE, $paid = FALSE) {
  $entries = array();
  $files = chdbtick_get_master_history();
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
    $sql = "SELECT * FROM ticket_table";
    $sql .= sprintf(" WHERE CompanyId='%s' AND PropertyId='%s'",
		    $company_id, $property_id);

    if ($filter_lot) {
      $sql .= sprintf(" AND LotId='%s'", $lot_id);
    }

    if ($filter_paid) {
      $sql .= sprintf(" AND Paid=%d", $paid ? 1 : 0);
    }

    $sql .= sprintf(" AND CreateTime > '%s' AND CreateTime < '%s'",
		    $start_time, $stop_time);

    foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
      $entries[] = $entry;
    }
  }
  return $entries;
}



/****************   ticketstats_table   **********************/



// Return 0 on success, otherwise 1 
function chdbtick_stats_new($company_id, $property_id, $lot_id, $enforcer_id,
			    $admin_id, $ticket_id, $amount, $lpn) {
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
  $sql = "INSERT INTO ticketstats_table ";
  $sql .= "(CompanyId,PropertyId,LotId,EnforcerId,AdminId,TicketId,Amount,New,LPN) VALUES ";
  $sql .= sprintf("('%s','%s','%s','%s','%s','%s','%s',1,'%s')",
		  $company_id, $property_id, $lot_id, $enforcer_id,
		  $admin_id, $ticket_id, $amount, $lpn);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   prediction_table Table   **********************/



function chdbtick_get_predictions($company_id, $property_id,
				  $filter_lot = FALSE, $lot_id = '') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_TICKET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
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
function chdbtick_insert_prediction($company_id, $property_id, $lot_id) {
  $db_name = getenv('APPS_LPMS_TICKET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
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
function chdbtick_update_prediction_daily($company_id, $property_id,
					  $lot_id, $today, $tomorrow) {
  $db_name = getenv('APPS_LPMS_TICKET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
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
function chdbtick_update_prediction_weekly($company_id, $property_id,
					   $lot_id, $next_week) {
  $db_name = getenv('APPS_LPMS_TICKET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
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
function chdbtick_update_prediction_monthly($company_id, $property_id,
					    $lot_id, $next_month) {
  $db_name = getenv('APPS_LPMS_TICKET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for TICKET database!!!", 'error');
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



