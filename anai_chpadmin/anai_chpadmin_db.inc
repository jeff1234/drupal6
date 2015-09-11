<?php
// $Id$



/****************   server_table   **********************/



function chdbadmin_get_server() {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "SELECT * FROM server_table";
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbadmin_insert_server($domain, $port) {
  $db_name = getenv('APPS_LPMS_ADMIN_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "INSERT INTO server_table (Domain,Port) VALUES ";
  $sql .= sprintf("('%s',%d)",
		  $domain, $port);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbadmin_delete_server() {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "DELETE FROM server_table";
  try {
    $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   make_table   **********************/



function chdbadmin_get_all_vehicles($order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "SELECT * FROM make_table";
  if ($order_by !== 'None') {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbadmin_get_all_makes($order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "SELECT DISTINCT Make FROM make_table";
  if ($order_by !== 'None') {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbadmin_get_models($make, $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "SELECT Model FROM make_table";
  $sql .= sprintf(" WHERE Make='%s'", $make);
  if ($order_by !== 'None') {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbadmin_get_make($make) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM make_table WHERE Make='%s'", $make);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 1 if exists, otherwise 0
function chdbadmin_make_exists($make) {
  $entry = chdbadmin_get_make($make);
  if (sizeof($entry) > 0) {
    return 1;
  }
  return 0;
}



function chdbadmin_insert_default_vehicles() {
  $db_name = getenv('APPS_LPMS_ADMIN_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  $makes = array();
  $makes[] = 'ACURA';
  $makes[] = 'AUDI';
  $makes[] = 'BMW';
  $makes[] = 'BUICK';
  $makes[] = 'CADILLAC';
  $makes[] = 'CHEVROLET';
  $makes[] = 'CHRYSLER';
  $makes[] = 'DAEWOO';
  $makes[] = 'DAIHATSU';
  $makes[] = 'DATSUN';
  $makes[] = 'DODGE';
  $makes[] = 'FERRARI';
  $makes[] = 'FIAT';
  $makes[] = 'FORD';
  $makes[] = 'FREIGHTLINER';
  $makes[] = 'GEO';
  $makes[] = 'GMC';
  $makes[] = 'HARLEY DAVIDSON';
  $makes[] = 'HINO';
  $makes[] = 'HONDA';
  $makes[] = 'HUMMER';
  $makes[] = 'HYUNDAI';
  $makes[] = 'INFINITI';
  $makes[] = 'INTERNATIONAL';
  $makes[] = 'ISUZU';
  $makes[] = 'JAGUAR';
  $makes[] = 'JEEP';
  $makes[] = 'KAWASAKI';
  $makes[] = 'KENWORTH';
  $makes[] = 'KIA';
  $makes[] = 'LADA';
  $makes[] = 'LAND ROVER';
  $makes[] = 'LEXUS';
  $makes[] = 'LINCOLN';
  $makes[] = 'MACK';
  $makes[] = 'SAAB';
  $makes[] = 'SATURN';
  $makes[] = 'MAZDA';
  $makes[] = 'MERCEDES BENZ';
  $makes[] = 'MERCURY';
  $makes[] = 'MITSUBISHI';
  $makes[] = 'SUBARU';
  $makes[] = 'MOTORCYCLE';
  $makes[] = 'NISSAN';
  $makes[] = 'SUZUKI';
  $makes[] = 'OLDSMOBILE';
  $makes[] = 'TOYOTA';
  $makes[] = 'PETERBILT';
  $makes[] = 'PLYMOUTH';
  $makes[] = 'PONTIAC';
  $makes[] = 'VOLKSWAGEN';
  $makes[] = 'PORSCHE';
  $makes[] = 'VOLVO';
  $makes[] = 'YAMAHA';
  // Query
  foreach ($makes as $make) {
    $sql = sprintf("INSERT INTO make_table (Make) VALUES ('%s')", $make);
    if (!$db->exec($sql)) {
      drupal_set_message("Not able to write to DB", 'error');
      return 1;
    }
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbadmin_insert_vehicle($make, $model) {
  $db_name = getenv('APPS_LPMS_ADMIN_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = sprintf("INSERT INTO make_table (Make,Model) VALUES ('%s','%s')",
		 $make, $model);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbadmin_delete_vehicle($make) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = sprintf("DELETE FROM make_table WHERE Make='%s'", $make);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbadmin_delete_vehicle_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = sprintf("DELETE FROM make_table WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   nameopt_table   **********************/



function chdbadmin_get_all_nameopts() {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "SELECT * FROM nameopt_table";
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbadmin_get_nameopt($company_id = 'ANY') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "SELECT * FROM nameopt_table";
  if ($company_id === 'ANY') {
    $sql .= sprintf(" WHERE LENGTH(CompanyId) ISNULL");
  } else {
    $sql .= sprintf(" WHERE CompanyId='%s'", $company_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbadmin_insert_nameopt($fields,
				  $prename, $firstname, $middlename, $lastname, $sufname,
				  $phone, $email, $emailnotification,
				  $role, $department, $title,
				  $company_id = 'ANY') {
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "INSERT INTO nameopt_table (";
  if ($company_id !== 'ANY') {
    $sql .= "CompanyId,";
  }
  $sql .= "Fields,Prename,FirstName,MiddleName,LastName,Sufname,";
  $sql .= "Phone,Email,EmailNotification,Role,Department,Title) VALUES (";
  if ($company_id !== 'ANY') {
    $sql .= sprintf("'%s',", $company_id);
  }
  $sql .= sprintf("'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
		  $fields,
		  $prename ? 1 : 0,
		  $firstname ? 1 : 0,
		  $middlename ? 1 : 0,
		  $lastname ? 1 : 0,
		  $sufname ? 1 : 0,
		  $phone ? 1 : 0,
		  $email ? 1 : 0,
		  $emailnotification ? 1 : 0,
		  $role ? 1 : 0,
		  $department ? 1 : 0,
		  $title ? 1 : 0);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbadmin_update_nameopt($fields,
				  $prename, $firstname, $middlename, $lastname, $sufname,
				  $phone, $email, $emailnotification,
				  $role, $department, $title,
				  $company_id = 'ANY') {
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "UPDATE nameopt_table SET ";
  $sql .= sprintf("Fields='%s',Prename=%d,FirstName=%d,MiddleName=%d,LastName=%d,Sufname=%d,",
		  $fields,
		  $prename ? 1 : 0,
		  $firstname ? 1 : 0,
		  $middlename ? 1 : 0,
		  $lastname ? 1 : 0,
		  $sufname ? 1 : 0);
  $sql .= sprintf("Phone=%d,Email=%d,EmailNotification=%d,Role=%d,Department=%d,Title=%d",
		  $phone ? 1 : 0,
		  $email ? 1 : 0,
		  $emailnotification ? 1 : 0,
		  $role ? 1 : 0,
		  $department ? 1 : 0,
		  $title ? 1 : 0);
  if ($company_id === 'ANY') {
    $sql .= sprintf(" WHERE LENGTH(CompanyId) ISNULL");
  } else {
    $sql .= sprintf(" WHERE CompanyId='%s'", $company_id);
  }
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
function chdbadmin_delete_nameopt($company_id) {
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = sprintf("DELETE FROM nameopt_table WHERE CompanyId='%s'", $company_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   prenameopt_table   **********************/



function chdbadmin_get_all_prenameopts() {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "SELECT * FROM prenameopt_table";
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbadmin_get_prenameopt($company_id = 'Any') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "SELECT * FROM prenameopt_table";
  if ($company_id === 'Any') {
    $sql .= sprintf(" WHERE LENGTH(CompanyId) ISNULL");
  } else {
    $sql .= sprintf(" WHERE CompanyId='%s'", $company_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbadmin_insert_prenameopt($prename_id, $alias, $company_id = 'ANY') {
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "INSERT INTO prenameopt_table (";
  if ($company_id !== 'ANY') {
    $sql .= "CompanyId,";
  }
  $sql .= "PrenameId,Alias) VALUES (";
  if ($company_id !== 'ANY') {
    $sql .= sprintf("'%s',", $company_id);
  }
  $sql .= sprintf("'%s','%s')", $prename_id, $alias);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbadmin_update_prenameopt($prename_id, $alias, $company_id = 'ANY') {
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = sprintf("UPDATE prenameopt_table SET Alias='%s'", $alias);
  if ($company_id === 'ANY') {
    $sql .= sprintf(" WHERE LENGTH(CompanyId) ISNULL");
  } else {
    $sql .= sprintf(" WHERE CompanyId='%s'", $company_id);
  }
  $sql .= sprintf(" AND PrenameId='%s'", $prename_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbadmin_delete_prenameopt($company_id) {
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = sprintf("DELETE FROM prenameopt_table WHERE CompanyId='%s'", $company_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   sufnameopt_table   **********************/



function chdbadmin_get_all_sufnameopts() {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "SELECT * FROM sufnameopt_table";
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbadmin_get_sufnameopt($company_id = 'Any') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "SELECT * FROM sufnameopt_table";
  if ($company_id === 'Any') {
    $sql .= sprintf(" WHERE LENGTH(CompanyId) ISNULL");
  } else {
    $sql .= sprintf(" WHERE CompanyId='%s'", $company_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbadmin_insert_sufnameopt($sufname_id, $alias, $company_id = 'ANY') {
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "INSERT INTO sufnameopt_table (";
  if ($company_id !== 'ANY') {
    $sql .= "CompanyId,";
  }
  $sql .= "SufnameId,Alias) VALUES (";
  if ($company_id !== 'ANY') {
    $sql .= sprintf("'%s',", $company_id);
  }
  $sql .= sprintf("'%s','%s')", $sufname_id, $alias);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbadmin_update_sufnameopt($sufname_id, $alias, $company_id = 'ANY') {
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = sprintf("UPDATE sufnameopt_table SET Alias='%s'", $alias);
  if ($company_id === 'ANY') {
    $sql .= sprintf(" WHERE LENGTH(CompanyId) ISNULL");
  } else {
    $sql .= sprintf(" WHERE CompanyId='%s'", $company_id);
  }
  $sql .= sprintf(" AND SufnameId='%s'", $sufname_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbadmin_delete_sufnameopt($company_id) {
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = sprintf("DELETE FROM sufnameopt_table WHERE CompanyId='%s'", $company_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   lotclassopt_table   **********************/



function chdbadmin_get_lotclassopt() {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "SELECT * FROM lotclassopt_table";
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbadmin_insert_lotclassopt($lotclass_id, $alias) {
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "INSERT INTO lotclassopt_table (LotclassId,Alias) VALUES ";
  $sql .= sprintf("('%s','%s')", $lotclass_id, $alias);
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
function chdbadmin_update_lotclassopt($lotclass_id, $alias) {
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = sprintf("UPDATE lotclassopt_table SET Alias='%s'", $alias);
  $sql .= sprintf(" WHERE LotclassId='%s'", $lotclass_id);
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
function chdbadmin_delete_lotclassopt($lotclass_id) {
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "DELETE FROM lotclassopt_table";
  $sql .= sprintf(" WHERE LotclassId='%s'", $lotclass_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   lottypeopt_table   **********************/



function chdbadmin_get_lottypeopt() {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "SELECT * FROM lottypeopt_table";
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbadmin_insert_lottypeopt($lottype_id, $alias) {
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "INSERT INTO lottypeopt_table (LottypeId,Alias) VALUES ";
  $sql .= sprintf("('%s','%s')", $lottype_id, $alias);
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
function chdbadmin_update_lottypeopt($lottype_id, $alias) {
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = sprintf("UPDATE lottypeopt_table SET Alias='%s'", $alias);
  $sql .= sprintf(" WHERE LottypeId='%s'", $lottype_id);
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
function chdbadmin_delete_lottypeopt($lottype_id) {
  $db_name = getenv('APPS_LPMS_ADMIN_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for ADMIN database!!!", 'error');
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
  $sql = "DELETE FROM lottypeopt_table";
  $sql .= sprintf(" WHERE LottypeId='%s'", $lottype_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



