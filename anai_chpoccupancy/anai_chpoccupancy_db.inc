<?php



/****************   occu_table   **********************/



function chdboccu_get_all_occupancy($property_id, $lot_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_OCCU_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for OCCU database!!!", 'error');
    return $entries;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $entries;
  }
  // PY : Feb 10.14 - add second db to gather permits
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
    return $entries;
  }
  // SQL
  $sql = sprintf("ATTACH '%s' AS permit", $db_name);
  //drupal_set_message($sql);
  $db->exec($sql);
  // SQL
  // PY : Feb 10.14 - added an extra field for initial sort
  // $sql = "SELECT * FROM occu_table WHERE Active=1";
  // $sql .= sprintf(" AND PropertyId='%s' AND LotId='%s'", $property_id, $lot_id);
  $sql = "SELECT *, 1 as sortfld FROM occu_table as o WHERE o.Active=1";
  $sql .= sprintf(" AND o.PropertyId='%s' AND o.LotId='%s'", $property_id, $lot_id);
  $sql .= sprintf(" UNION");
  $sql .= sprintf(" SELECT p.Id, p.CreateTime, p.PropertyId, p.LotId, 1 as Active, 0 as History, p.LPN,");
  $sql .= sprintf(" p.BeginTime as InTime, null as OutTime, p.ExpiryTime as DeactivationTime,");
  $sql .= sprintf(" p.ExpiryTime as ValidationTime, p.PermitClass as Validation, 'White' as Color,");
  $sql .= sprintf(" 'Permit' as Source, 'atuioawcj3ra5906sgxe' as EventId, 1 as Compliant, 0 as Pending, p.LotId as AllLotId,");
  $sql .= sprintf(" p.BeginTime as AllInTime, null as AllOutTime, p.PermitClass as AllValidation, ");
  $sql .= sprintf(" p.ExpiryTime as AllValidationTime, 1 as AllCompliant, 0 as AllPending, p.GroupId as AllGroupId, ");
  $sql .= sprintf(" p.MemberId as AllMemberId, 'permit' as DBName, null as DBId, '' as ScanId, 0 as PendingDelete");
  $sql .= ', 2 as sortfld';
  $sql .= sprintf(" FROM permit.permit_table as p");
  $sql .= sprintf(" WHERE p.PropertyId='%s' AND p.LotId='%s'", $property_id, $lot_id);
  $sql .= sprintf(" AND p.BeginTime<=CURRENT_TIMESTAMP AND p.ExpiryTime>=CURRENT_TIMESTAMP");
  $sql .= sprintf(" AND p.PermitClass='Validation' ");
  $sql .= sprintf(" AND p.LPN NOT IN ");
  $sql .= '(';
  $sql .= sprintf("SELECT LPN FROM occu_table WHERE Active=1 AND AllCompliant=1");
  $sql .= ')';
  $sql .= sprintf(" AND LPN<>'033JLW'");
  $sql .= sprintf(" ORDER BY sortfld, LPN, AllInTime");
//hesam jun 3/6/2014
//$sql = "Select * from occu_table limit 10";
// PY : Jun 1.14
//drupal_set_message("chdboccu_get_all_occupancy:".$sql, 'error');
//return;
  $stmt = $db->query($sql);
  $result = $stmt->fetch(PDO::FETCH_ASSOC);
  foreach($result as $key=>$val)
  {
	 $entries[] = $val;
  }
  //foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
  //  $entries[] = $entry;
  //  $string = var_export($entries, true);
  //  drupal_set_message("chdboccu_get_all_occupancy:".$string, 'error');
  //  return $entries;
  //}
//drupal_set_message("chdboccu_get_all_occupancy:".$entries, 'error');
//return;
  return $entries;
}



function chdboccu_get_all_compliant($property_id, $lot_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_OCCU_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for OCCU database!!!", 'error');
    return $entries;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $entries;
  }
  // PY : Feb 10.14 - add second db to gather permits
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
    return $entries;
  }
  // SQL
  $sql = sprintf("ATTACH '%s' AS permits", $db_name);
  //drupal_set_message($sql);
  $db->exec($sql);
  // SQL
  // PY : Feb 10.14 - changed query to be a union between occu_table and permit_table
  //$sql = "SELECT * FROM occu_table WHERE Active=1 AND AllCompliant=1";
  //$sql .= sprintf(" AND PropertyId='%s' AND LotId='%s'", $property_id, $lot_id);
  $sql = "SELECT *, 1 as sortfld FROM occu_table WHERE Active=1 AND AllCompliant=1";
  $sql .= sprintf(" AND PropertyId='%s' AND LotId='%s'", $property_id, $lot_id);
  $sql .= sprintf(" UNION");
//  $sql .= sprintf(" SELECT 2, Id, CreateTime, PropertyId, LotId, 1 as Active, 0 as History,LPN,");
//  $sql .= sprintf(" BeginTime as InTime, null as OutTime, ExpiryTime as DeactivationTime,");
//  $sql .= sprintf(" ExpiryTime as ValidationTime, PermitClass as Validation, 'White' as Color,");
//  $sql .= sprintf(" 'Permit' as Source, 'atuioawcj3ra5906sgxe' as EventId, 1 as Compliant, 0 as Pending, LotId as AllLotId,");
//  $sql .= sprintf(" BeginTime as AllInTime, null as AllOutTime, PermitClass as AllValidation, ");
//  $sql .= sprintf(" ExpiryTime as AllValidationTime, 1 as AllCompliant, 0 as AllPending, GroupId as AllGroupId, ");
//  $sql .= sprintf(" MemberId as AllMemberId, 'permit' as DBName, null as DBId, '' as ScanId, 0 as PendingDelete");
//  $sql .= sprintf(" FROM permit_table p");
//  $sql .= sprintf(" WHERE PropertyId='%s' AND LotId='%s'", $property_id, $lot_id);
//  $sql .= sprintf(" AND BeginTime<=CURRENT_TIMESTAMP AND ExpiryTime>=CURRENT_TIMESTAMP");
//  $sql .= sprintf(" AND o.Id='%s'", '59257');

  $sql .= sprintf(" SELECT Id, CreateTime, PropertyId, LotId, 1 as Active, 0 as History,LPN,");
  $sql .= sprintf(" BeginTime as InTime, null as OutTime, ExpiryTime as DeactivationTime,");
  $sql .= sprintf(" ExpiryTime as ValidationTime, PermitClass as Validation, 'White' as Color,");
  $sql .= sprintf(" 'Permit' as Source, 'atuioawcj3ra5906sgxe' as EventId, 1 as Compliant, 0 as Pending, LotId as AllLotId,");
  $sql .= sprintf(" BeginTime as AllInTime, null as AllOutTime, PermitClass as AllValidation, ");
  $sql .= sprintf(" ExpiryTime as AllValidationTime, 1 as AllCompliant, 0 as AllPending, GroupId as AllGroupId, ");
  $sql .= sprintf(" MemberId as AllMemberId, 'permit' as DBName, null as DBId, '' as ScanId, 0 as PendingDelete");
  $sql .= ', 2 as sortfld';
  $sql .= sprintf(" FROM permit_table p");
  $sql .= sprintf(" WHERE PropertyId='%s' AND LotId='%s'", $property_id, $lot_id);
  $sql .= sprintf(" AND BeginTime<=CURRENT_TIMESTAMP AND ExpiryTime>=CURRENT_TIMESTAMP");
  $sql .= ' AND p.LPN NOT IN ';
  $sql .= '(';
  $sql .= 'SELECT LPN FROM occu_table WHERE Active=1 AND AllCompliant=1';
  $sql .= ')';
  $sql .= sprintf(" AND LPN<>'033JLW'");

  $sql .= sprintf(" ORDER BY sortfld, LPN, AllInTime");
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdboccu_get_all_non_compliant($property_id, $lot_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_OCCU_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for OCCU database!!!", 'error');
    return $entries;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $entries;
  }
  // PY : Feb 16.14 - join permit.db to exclude permits
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
    return $entries;
  }
  // SQL
  $sql = sprintf("ATTACH '%s' AS permits", $db_name);
  //drupal_set_message($sql);
  $db->exec($sql);
  // SQL
  $sql = "SELECT * FROM occu_table WHERE Active=1 AND AllCompliant=0 AND AllPending=0";
  $sql .= sprintf(" AND PropertyId='%s' AND LotId='%s'", $property_id, $lot_id);
  // PY : Feb 16.14 - exclude permits (validation permits could have just become active)
  $sql .= sprintf(" AND LPN NOT IN ");
  $sql .= '(';
  $sql .= sprintf("SELECT LPN from permits.permit_table");
  $sql .= ')';
  // PY : Feb 9.14 - sort by plate then entry time
  $sql .= sprintf(" ORDER BY LPN, AllInTime");
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdboccu_get_all_hotlist($company_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_OCCU_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for OCCU database!!!", 'error');
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
  //drupal_set_message($sql);
  $db->exec($sql);
  // SQL
  $sql = "SELECT occu.* FROM occu_table AS occu";
  $sql .= " INNER JOIN props.relations_table AS relation";
  $sql .= "  ON occu.PropertyId=relation.PropertyId";
  $sql .= " WHERE occu.Active=1 AND occu.AllCompliant=0 AND occu.AllPending=0";
  $sql .= sprintf(" AND relation.CompanyId='%s'", $company_id);
  // PY : Feb 9.14 - sort by plate then entry time
  $sql .= sprintf(" ORDER BY LPN, AllInTime");
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdboccu_get_hotlist_lpn($company_id, $lpns) {
  $entries = array();
  if (empty($lpns)) {
    return $entries;
  }
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_OCCU_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for OCCU database!!!", 'error');
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
  //drupal_set_message($sql);
  $db->exec($sql);
  // SQL
  $sql = "SELECT occu.* FROM occu_table AS occu";
  $sql .= " INNER JOIN props.relations_table AS relation";
  $sql .= "  ON occu.PropertyId=relation.PropertyId";
  $sql .= " WHERE occu.Active=1";
  $sql .= sprintf(" AND relation.CompanyId='%s' AND (", $company_id);
  foreach ($lpns as $lpn) {
    $sql .= sprintf("occu.LPN='%s' OR ", $lpn);
  }
  $sql = rtrim($sql, ' OR ');
  $sql .= ")";
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdboccu_get_all_pending($property_id, $lot_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_OCCU_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for OCCU database!!!", 'error');
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
  $sql = "SELECT * FROM occu_table WHERE Active=1 AND AllCompliant=0 AND AllPending=1";
  $sql .= sprintf(" AND PropertyId='%s' AND LotId='%s'", $property_id, $lot_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdboccu_get_occupancy_scan_id($property_id, $scan_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_OCCU_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for OCCU database!!!", 'error');
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
  $sql = "SELECT * FROM occu_table WHERE Active=1";
  $sql .= sprintf(" AND PropertyId='%s' AND ScanId='%s'", $property_id, $scan_id);
  if ($lot_id !== 'ANY') {
    $sql .= sprintf(" AND LotId='%s'", $lot_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdboccu_get_occupancy_event_id($property_id, $event_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_OCCU_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for OCCU database!!!", 'error');
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
  $sql = "SELECT * FROM occu_table WHERE Active=1";
  $sql .= sprintf(" AND PropertyId='%s' AND EventId='%s'", $property_id, $event_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdboccu_get_occupancy_current_lpn($property_id, $lot_id, $lpn) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_OCCU_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for OCCU database!!!", 'error');
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
  $sql = "SELECT * FROM occu_table WHERE Active=1 AND History=0 AND ";
  $sql .= sprintf("PropertyId='%s' AND LPN='%s'", $property_id, $lpn);
  if ($lot_id !== 'ANY') {
    $sql .= sprintf(" AND LotId='%s'", $lot_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdboccu_get_occupancy_report($user_is_admin, $start_time, $stop_time,
				       $company_id, $property_id, $lot_id,
				       $group_id, $member_id,
				       $lot_type, $lot_class,
				       $validations, $compliancy) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_OCCU_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for OCCU database!!!", 'error');
    return $entries;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return $entries;
  }
  // CompanyId not used by occupancy
  if ($property_id === 'ANY' or $lot_id === 'ANY' or
      $lot_type !== 'ANY' or $lot_class !== 'ANY') {
    // Find location of SQLite database
    $db_name = getenv('APPS_LPMS_PROP_DB');
    if ($db_name == FALSE) {
      drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
      return $entries;
    }
    // SQL
    $sql = sprintf("ATTACH '%s' AS props", $db_name);
    //drupal_set_message($sql);
    //var_export($sql);
    $db->exec($sql);
  }
  // SQL
  // CompanyId not used by occupancy
  $sql = "SELECT occu.* FROM occu_table AS occu";
  if (!$user_is_admin and ($property_id === 'ANY' or $lot_id === 'ANY')) {
    $sql .= " INNER JOIN props.user_table AS user";
    $sql .= "  ON (occu.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
    $sql .= "   AND (occu.AllLotId=user.LotId OR user.LotId='ANY')";
  }
  if ($lot_type !== 'ANY' or $lot_class !== 'ANY') {
    $sql .= " INNER JOIN props.policy_table AS policy";
    $sql .= "  ON (occu.PropertyId=policy.PropertyId)";
    $sql .= "   AND (occu.AllLotId=policy.LotId)";
    $sql .= " INNER JOIN props.profile_table AS profile";
    $sql .= "  ON (policy.ProfileId=profile.ProfileId)";
  }
  $precond = TRUE;
  // CompanyId not used by occupancy
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" %s occu.PropertyId='%s'", $precond ? 'WHERE' : 'AND', $property_id);
    $precond = FALSE;
  }
  if ($lot_id !== 'ANY') {
    $sql .= sprintf(" %s occu.AllLotId='%s'", $precond ? 'WHERE' : 'AND', $lot_id);
    $precond = FALSE;
  }
  if ($group_id !== 'ANY') {
    $sql .= sprintf(" %s occu.AllGroupId='%s'", $precond ? 'WHERE' : 'AND', $group_id);
    $precond = FALSE;
  }
  if ($member_id !== 'ANY') {
    $sql .= sprintf(" %s occu.AllMemberId='%s'", $precond ? 'WHERE' : 'AND', $member_id);
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
  if (!empty($validations)) {
    $sql .= sprintf(" %s (", $precond ? 'WHERE' : 'AND');
    foreach ($validations as $validation) {
      $sql .= sprintf("occu.AllValidation='%s' OR ", $validation);
    }
    $sql = rtrim($sql, ' OR ');
    $sql .= ")";
    $precond = FALSE;
  }
  if ($compliancy !== 'ANY') {
    if ($compliancy === 'Compliant') {
      $sql .= sprintf(" %s (occu.AllCompliant=1 OR (occu.AllCompliant=0 AND occu.AllPending=1))",
  		      $precond ? 'WHERE' : 'AND');
      $precond = FALSE;
    } elseif ($compliancy === 'NonCompliant') {
      $sql .= sprintf(" %s (occu.AllCompliant=0 AND occu.AllPending=0)",
  		      $precond ? 'WHERE' : 'AND');
      $precond = FALSE;
    }
  }
  $sql .= sprintf(" %s ((LENGTH(occu.AllOutTime)>0 AND occu.AllOutTime>'%s') OR".
		  " LENGTH(occu.AllOutTime) IS NULL)",
		  $precond ? 'WHERE' : 'AND', $start_time);
  $sql .= sprintf(" AND AllInTime<'%s'", $stop_time);
  $precond = FALSE;
  //drupal_set_message($sql);
  //var_export($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdboccu_unshift_event_white_immediately($property_id, $lot_id, $lpn, $event_id,
						  $expiry_time,
						  &$id) {
  $db_name = getenv('APPS_LPMS_OCCU_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for OCCU database!!!", 'error');
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
  $sql = "UPDATE occu_table SET History=History+1 WHERE Active=1 AND History>0";
  $sql .= sprintf(" AND PropertyId='%s' AND EventId='%s'", $property_id, $event_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // SQL
  $sql = "UPDATE occu_table SET DeactivationTime=CURRENT_TIMESTAMP,History=1";
  $sql .= " WHERE Active=1 AND History=0";
  $sql .= sprintf(" AND PropertyId='%s' AND EventId='%s'", $property_id, $event_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // SQL
  $sql = "INSERT INTO occu_table (PropertyId,LotId,LPN,InTime,Color,Source,";
  $sql .= "EventId,Compliant,Pending,Validation,ValidationTime) VALUES ";
  $sql .= sprintf("('%s','%s','%s',CURRENT_TIMESTAMP,'White','Permit',",
		  $property_id, $lot_id, $lpn);
  $sql .= sprintf("'%s',1,0,'Permit',", $event_id);
  if ($expiry_time === NULL) {
    $sql .= "NULL)";
  } else {
    $sql .= sprintf("'%s')", $expiry_time);
  }
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write-3 to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  // SQL
  $sql = "UPDATE occu_table SET AllValidation='Permit',AllCompliant=1,AllPending=0";
  $sql .= ",AllGroupId=NULL,AllMemberId=NULL";
  $sql .= sprintf(",AllLotId='%s'", $lot_id);
  if ($expiry_time !== NULL) {
    $sql .= sprintf(",AllValidationTime='%s'", $expiry_time);
  }
  $sql .= sprintf(" WHERE Active=1 AND PropertyId='%s' AND EventId='%s'",
		  $property_id, $event_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write-4 to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdboccu_insert_scan($property_id, $lot_id, $lpr_time, $lpn, $event_id, &$id) {
  $db_name = getenv('APPS_LPMS_OCCU_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for OCCU database!!!", 'error');
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
  $sql = "INSERT INTO occu_table (PropertyId,LotId,AllLotId,LPN,InTime,AllInTime,";
  $sql .= "Color,Source,EventId,Compliant,Pending,";
  $sql .= "AllCompliant,AllPending,AllGroupId,AllMemberId) VALUES ";
  $sql .= sprintf("('%s','%s','%s','%s',CURRENT_TIMESTAMP,CURRENT_TIMESTAMP,",
		  $property_id, $lot_id, $lot_id, $lpn);
  $sql .= sprintf("'Yellow','LPR','%s',0,1,0,1,NULL,NULL)", $event_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdboccu_update_scan_id($property_id, $scan_id, $lpn, $event_id = NULL) {
  $db_name = getenv('APPS_LPMS_OCCU_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for OCCU database!!!", 'error');
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
  $sql = "UPDATE occu_table SET ";
  $sql .= sprintf("LPN='%s'", $lpn);
  if ($event_id !== NULL) {
    $sql .= sprintf(",EventId='%s'", $event_id);
  }
  $sql .= sprintf(" WHERE Active=1 AND ScanId='%s'", $scan_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   param_table   **********************/



function chdboccu_get_params($property_id, $lot_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_OCCU_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for OCCU database!!!", 'error');
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
  $sql = "SELECT * FROM param_table";
  $sql .= sprintf(" WHERE PropertyId='%s' AND LotId='%s'",
		  $property_id, $lot_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdboccu_insert_params($property_id, $lot_id,
				$grace_in, $grace_out, $flush) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_OCCU_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for OCCU database!!!", 'error');
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
  $sql = "INSERT INTO param_table (PropertyId,LotId,GraceIn,GraceOut,FlushDays)";
  $sql .= sprintf(" VALUES ('%s','%s',%d,%d,%d)",
		  $property_id, $lot_id, $grace_in, $grace_out, $flush);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdboccu_update_params($property_id, $lot_id,
				$grace_in, $grace_out, $flush) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_OCCU_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for OCCU database!!!", 'error');
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
  $sql = sprintf("UPDATE param_table SET GraceIn=%d,GraceOut=%d,FlushDays=%d",
		 $grace_in, $grace_out, $flush);
  $sql .= sprintf(" WHERE PropertyId='%s' AND LotId='%s'", $property_id, $lot_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



