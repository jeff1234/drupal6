<?php
// $Id$



/****************   receipt_table   **********************/



function chdbvalet_get_all_valets() {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "SELECT * FROM receipt_table WHERE Active=1";
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbvalet_get_valets($company_id, $property_id, $lot_id,
			      $active = 1,
			      $filter_status = FALSE, $status = '') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "SELECT * FROM receipt_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s' AND LotId='%s' AND Active=%d",
		  $company_id, $property_id, $lot_id, $active ? 1 : 0);
  if ($filter_status) {
    $sql .= sprintf(" AND Status='%s'", $status);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbvalet_get_valet_lpn($company_id, $property_id, $lot_id,
				 $lpn) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "SELECT * FROM receipt_table WHERE Active=1 AND ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s' AND LotId='%s'",
		  $company_id, $property_id, $lot_id);
  $sql .= sprintf(" AND LPN='%s'", $lpn);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbvalet_get_valet_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM receipt_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbvalet_insert_receipt($company_id, $property_id, $lot_id,
				  $lpn, $make, $model, $color,
				  $code, $description, $amount,
				  $reference, $status, $checkout_time,
				  &$id) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "INSERT INTO receipt_table (CompanyId,CheckinTime,PropertyId,LotId,";
  $sql .= "LPN,Make,Model,Color,Code,Description,Amount,";
  $sql .= "Reference,Status,CheckoutTime) ";
  $sql .= sprintf("VALUES ('%s',CURRENT_TIMESTAMP,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
		  $company_id, $property_id, $lot_id,
		  $lpn, $make, $model, $color,
		  $code, $description, $amount,
		  $reference, $status, $checkout_time);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



function chdbvalet_update_receipt($id,
				  $lpn, $make, $model, $color,
				  $code, $description, $amount,
				  $checkout_time) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "UPDATE receipt_table SET ";
  $sql .= sprintf("LPN='%s',Make='%s',Model='%s',Color='%s', ",
		  $lpn, $make, $model, $color);
  $sql .= sprintf("Code='%s',Description='%s',Amount='%s',CheckoutTime='%s'",
		  $code, $description, $amount, $checkout_time);
  $sql .= sprintf(" WHERE Id=%d", $id);
  drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



function chdbvalet_update_receipt_person($id, $first_name, $last_name, $phone,
					 $street, $city, $postal_code,
					 $province, $country) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "UPDATE receipt_table SET ";
  $sql .= sprintf("FirstName='%s',LastName='%s',Phone='%s',Street='%s',City='%s',",
		  $first_name, $last_name, $phone, $street, $city);
  $sql .= sprintf("PostalCode='%s',Province='%s',Country='%s'",
		  $postal_code, $province, $country);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



function chdbvalet_update_status_checkout($id) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "UPDATE receipt_table SET ActionLog=ActionLog||'0,'||CURRENT_TIMESTAMP||',CheckOut$',";
  $sql .= "Status='CheckOut',Active=0,CheckoutTime=CURRENT_TIMESTAMP";
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



function chdbvalet_update_status_checkin($id) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "UPDATE receipt_table SET ActionLog=ActionLog||'0,'||CURRENT_TIMESTAMP||',CheckIn$',";
  $sql .= "Status='CheckIn',Active=1,CheckoutTime=NULL";
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



function chdbvalet_update_status_prepare($id) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "UPDATE receipt_table SET ActionLog=ActionLog||'0,'||CURRENT_TIMESTAMP||',Prepare$',";
  $sql .= "Status='Prepare',Active=1,PrepareTime=CURRENT_TIMESTAMP";
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



function chdbvalet_update_status_pickup($id) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "UPDATE receipt_table SET ActionLog=ActionLog||'0,'||CURRENT_TIMESTAMP||',PickUp$',";
  $sql .= "Status='PickUp',Active=1,PickupTime=CURRENT_TIMESTAMP";
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbvalet_delete_valet_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = sprintf("DELETE FROM receipt_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   damage_table   **********************/



function chdbvalet_get_damages($company_id, $property_id, $lot_id,
			       $lpn, $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "SELECT * FROM damage_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s' AND LotId='%s' AND LPN='%s'",
		  $company_id, $property_id, $lot_id, $lpn);
  if (strcmp($order_by, 'None')) {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbvalet_insert_damage($company_id, $property_id, $lot_id,
				 $lpn, $reference, $place, $damage,
				 &$id) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "INSERT INTO damage_table (CompanyId,PropertyId,LotId,";
  $sql .= "LPN,Reference,Place,Damage) ";
  $sql .= sprintf("VALUES ('%s','%s','%s','%s','%s','%s','%s')",
		  $company_id, $property_id, $lot_id,
		  $lpn, $reference, $place, $damage);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbvalet_delete_damages($company_id, $lpn) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "DELETE FROM damage_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND LPN='%s'",
		  $company_id, $lpn);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   valetvas_table   **********************/



function chdbvalet_get_valetvas($company_id, $property_id, $lot_id,
				$lpn, $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "SELECT * FROM valetvas_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s' AND LotId='%s' AND LPN='%s'",
		  $company_id, $property_id, $lot_id, $lpn);
  if (strcmp($order_by, 'None')) {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbvalet_insert_valetvas($company_id, $property_id, $lot_id,
				   $lpn, $reference,
				   $offer, $choice, $amount,
				   &$id) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "INSERT INTO valetvas_table (CompanyId,PropertyId,LotId,";
  $sql .= "LPN,Reference,Offer,Choice,Amount) ";
  $sql .= sprintf("VALUES ('%s','%s','%s','%s','%s','%s','%s','%s')",
		  $company_id, $property_id, $lot_id,
		  $lpn, $reference, $offer, $choice, $amount);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbvalet_delete_valetvases($company_id, $lpn) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "DELETE FROM valetvas_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND LPN='%s'",
		  $company_id, $lpn);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   valet_table   **********************/



function chdbvalet_get_codes($company_id, $template, $property_id,
			     $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM valet_table WHERE CompanyId='%s' AND Template=%d",
		 $company_id, $template ? 1 : 0);
  if (!$template) {
    $sql .= sprintf(" AND PropertyId='%s'", $property_id);
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



function chdbvalet_get_code($company_id, $template, $code,
			    $property_id, $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "SELECT * FROM valet_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND Template=%d AND Code='%s'",
		  $company_id, $template ? 1 : 0, $code);
  if (!$template) {
    $sql .= sprintf(" AND PropertyId='%s'", $property_id);
  }
  if (strcmp($order_by, 'None')) {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 1 if exists, otherwise 0
function chdbvalet_code_exists($company_id, $template, $code, $property_id) {
  $entry = chdbvalet_get_code($company_id, $template, $code, $property_id);
  if (sizeof($entry) > 0) {
    return 1;
  }
  return 0;
}



function chdbvalet_get_code_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM valet_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbvalet_insert_new_code($company_id, $template, $property_id,
				   $code, $description, $amount,
				   &$id) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "INSERT INTO valet_table (CompanyId,Template,PropertyId,";
  $sql .= "Code,Description,Amount) ";
  $sql .= sprintf("VALUES ('%s',%d,'%s','%s','%s','%s')",
		  $company_id,
		  $template ? 1 : 0,
		  $property_id ? $property_id : 'NA',
		  $code, $description, $amount);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



function chdbvalet_update_code($id, $template, $property_id, $code, $description, $amount) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "UPDATE valet_table SET ";
  $sql .= sprintf("Template=%d,PropertyId='%s',Code='%s',Description='%s',Amount='%s'",
		  $template ? 1 : 0, $property_id, $code, $description, $amount);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbvalet_delete_code($company_id, $template, $code, $property_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "DELETE FROM valet_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND Template=%d AND Code='%s'",
		  $company_id, $template ? 1 : 0, $code);
  if (!$template) {
    $sql .= sprintf(" AND PropertyId='%s'", $property_id);
  }
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbvalet_delete_code_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = sprintf("DELETE FROM valet_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   vas_table   **********************/



function chdbvalet_get_vases($company_id, $template, $property_id,
			    $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM vas_table WHERE CompanyId='%s' AND Template=%d",
		 $company_id, $template ? 1 : 0);
  if (!$template) {
    $sql .= sprintf(" AND PropertyId='%s'", $property_id);
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



function chdbvalet_get_vas($company_id, $template, $offer, $choice,
			   $property_id, $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "SELECT * FROM vas_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND Template=%d AND Offer='%s' AND Choice='%s'",
		  $company_id, $template ? 1 : 0, $offer, $choice);
  if (!$template) {
    $sql .= sprintf(" AND PropertyId='%s'", $property_id);
  }
  if (strcmp($order_by, 'None')) {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 1 if exists, otherwise 0
function chdbvalet_vas_exists($company_id, $template, $offer, $choice, $property_id) {
  $entry = chdbvalet_get_vas($company_id, $template, $offer, $choice, $property_id);
  if (sizeof($entry) > 0) {
    return 1;
  }
  return 0;
}



function chdbvalet_get_vas_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM vas_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbvalet_insert_new_vas($company_id, $template, $property_id,
				  $offer, $choice, $amount,
				  &$id) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "INSERT INTO vas_table (CompanyId,Template,PropertyId,";
  $sql .= "Offer,Choice,Amount) ";
  $sql .= sprintf("VALUES ('%s',%d,'%s','%s','%s','%s')",
		  $company_id, $template ? 1 : 0,
		  $template ? 'NA' : $property_id,
		  $offer, $choice, $amount);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



function chdbvalet_update_vas($id, $template, $property_id, $offer, $choice, $amount) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "UPDATE vas_table SET ";
  $sql .= sprintf("Template=%d,PropertyId='%s',Offer='%s',Choice='%s',Amount='%s'",
		  $template ? 1 : 0, $property_id, $offer, $choice, $amount);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbvalet_delete_vas($template, $offer, $choice, $user, $property_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "DELETE FROM vas_table WHERE ";
  $sql .= sprintf("Template=%d AND Offer='%s' AND Choice='%s'",
		  $template ? 1 : 0, $offer, $choice);
  if ($template) {
    $sql .= sprintf(" AND User='%s'", $user);
  } else {
    $sql .= sprintf(" AND PropertyId='%s'", $property_id);
  }
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbvalet_delete_vas_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = sprintf("DELETE FROM vas_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   printer_table   **********************/



function chdbvalet_get_printer_config($company_id, $template, $property_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "SELECT * FROM printer_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND Template=%d",
		  $company_id, $template ? 1 : 0);
  if (!$template) {
    $sql .= sprintf(" AND PropertyId='%s'", $property_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
    break;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbvalet_insert_printer_config($company_id, $template,
					 $property_id, $header,
					 &$id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "INSERT INTO printer_table (CompanyId,Template,PropertyId,";
  $sql .= "Header) VALUES ";
  $sql .= sprintf("('%s',%d,'%s','%s')",
		  $company_id, $template ? 1 : 0,
		  $template ? 'NA' : $property_id,
		  $header);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbvalet_update_printer_config($id, $template, $property_id,
					 $header) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "UPDATE printer_table SET ";
  $sql .= sprintf("Template=%d,PropertyId='%s',Header='%s'",
		  $template ? 1 : 0, $property_id, $header);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbvalet_delete_printer_config($company_id, $template, $property_id) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
  $sql = "DELETE FROM printer_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND Template=%d",
		  $company_id, $template ? 1 : 0);
  if (!$template) {
    $sql .= sprintf(" AND PropertyId='%s'", $property_id);
  }
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   valetstats_table   **********************/



// Return 0 on success, otherwise 1 
function chdbvalet_stats_new($company_id, $property_id, $lot_id,
			     $code, $days, $lpn) {
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
  $sql = "INSERT INTO valetstats_table ";
  $sql .= "(CompanyId,PropertyId,LotId,Code,Days,LPN,New) VALUES ";
  $sql .= sprintf("('%s','%s','%s','%s',%d,'%s',1)",
		  $company_id, $property_id, $lot_id,
		  $code, $days, $lpn);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   prediction_table Table   **********************/



function chdbvalet_get_predictions($company_id, $property_id,
				   $filter_lot = FALSE, $lot_id = '') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_VALET_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
function chdbvalet_insert_prediction($company_id, $property_id, $lot_id) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
function chdbvalet_update_prediction_daily($company_id, $property_id,
					   $lot_id, $today, $tomorrow) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
function chdbvalet_update_prediction_weekly($company_id, $property_id,
					    $lot_id, $next_week) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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
function chdbvalet_update_prediction_monthly($company_id, $property_id,
					     $lot_id, $next_month) {
  $db_name = getenv('APPS_LPMS_VALET_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for VALET database!!!", 'error');
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



