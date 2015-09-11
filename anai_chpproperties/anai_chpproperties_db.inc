<?php



/****************   composite constructions  **********************/



function chdbprop_cc_get_relations($uid) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT relation.* FROM relations_table AS relation";
  $sql .= " INNER JOIN user_table AS user";
  $sql .= "  ON (relation.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (relation.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d", $uid);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_admin_companies($filter_active = TRUE, $active = TRUE,
					 $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT company.* FROM company_table AS company";
  $sql .= " INNER JOIN relations_table AS relation";
  $sql .= "  ON (company.CompanyId=relation.CompanyId)";
  if ($filter_active and $filter_suspended) {
    $sql .= sprintf(" WHERE company.Active=%d AND company.Suspended=%d",
		    $active ? 1 : 0, $suspended ? 1 : 0);
  } elseif ($filter_active) {
    $sql .= sprintf(" WHERE company.Active=%d", $active ? 1 : 0);
  } elseif ($filter_suspended) {
    $sql .= sprintf(" WHERE company.Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_companies($uid,
				   $filter_active = TRUE, $active = TRUE,
				   $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT company.* FROM company_table AS company";
  $sql .= " INNER JOIN relations_table AS relation";
  $sql .= "  ON (company.CompanyId=relation.CompanyId)";
  $sql .= " INNER JOIN user_table AS user";
  $sql .= "  ON (relation.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d", $uid);
  if ($filter_active) {
    $sql .= sprintf(" AND company.Active=%d", $active ? 1 : 0);
  }
  if ($filter_suspended) {
    $sql .= sprintf(" AND company.Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_admin_regions($filter_active = TRUE, $active = TRUE,
				       $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT region.* FROM region_table AS region";
  $sql .= " INNER JOIN relations_table AS relation";
  $sql .= "  ON (region.CompanyId=relation.CompanyId)";
  if ($filter_active and $filter_suspended) {
    $sql .= sprintf(" WHERE region.Active=%d AND region.Suspended=%d",
		    $active ? 1 : 0, $suspended ? 1 : 0);
  } elseif ($filter_active) {
    $sql .= sprintf(" WHERE region.Active=%d", $active ? 1 : 0);
  } elseif ($filter_suspended) {
    $sql .= sprintf(" WHERE region.Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_regions($uid,
				 $filter_active = TRUE, $active = TRUE,
				 $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT region.* FROM region_table AS region";
  $sql .= " INNER JOIN relations_table AS relation";
  $sql .= "  ON (region.CompanyId=relation.CompanyId)";
  $sql .= " INNER JOIN user_table AS user";
  $sql .= "  ON (relation.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (region.RegionId=user.RegionId OR user.RegionId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d", $uid);
  if ($filter_active) {
    $sql .= sprintf(" AND region.Active=%d", $active ? 1 : 0);
  }
  if ($filter_suspended) {
    $sql .= sprintf(" AND region.Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_admin_clients($filter_active = TRUE, $active = TRUE,
				       $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT client.* FROM client_table AS client";
  $sql .= " INNER JOIN relations_table AS relation";
  $sql .= "  ON (client.CompanyId=relation.CompanyId)";
  if ($filter_active and $filter_suspended) {
    $sql .= sprintf(" WHERE client.Active=%d AND client.Suspended=%d",
		    $active ? 1 : 0, $suspended ? 1 : 0);
  } elseif ($filter_active) {
    $sql .= sprintf(" WHERE client.Active=%d", $active ? 1 : 0);
  } elseif ($filter_suspended) {
    $sql .= sprintf(" WHERE client.Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_clients($uid,
				 $filter_active = TRUE, $active = TRUE,
				 $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT client.* FROM client_table AS client";
  $sql .= " INNER JOIN relations_table AS relation";
  $sql .= "  ON (client.CompanyId=relation.CompanyId)";
  $sql .= " INNER JOIN user_table AS user";
  $sql .= "  ON (relation.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (client.RegionId=user.RegionId OR client.RegionId='ANY' OR user.RegionId='ANY')";
  $sql .= "    AND (client.ClientId=user.ClientId OR user.ClientId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d", $uid);
  if ($filter_active) {
    $sql .= sprintf(" AND client.Active=%d", $active ? 1 : 0);
  }
  if ($filter_suspended) {
    $sql .= sprintf(" AND client.Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_admin_properties($company_id = 'ANY',
					  $policy_filters = array(),
					  $equipment_filters = array(),
					  $enforcement_filters = array()) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT property.* FROM property_table AS property";
  $sql .= " INNER JOIN relations_table AS relation";
  $sql .= "  ON (property.PropertyId=relation.PropertyId)";
  if (0 < count($policy_filters)) {
    $sql .= " INNER JOIN policy_table AS policy";
    $sql .= "  ON (relation.PropertyId=policy.PropertyId)";
    $sql .= " INNER JOIN profile_table AS profile";
    $sql .= "  ON (policy.ProfileId=profile.ProfileId)";
  }
  if (!empty($policy_filters) or $company_id !== 'ANY') {
    $sql .= " WHERE ";
  }
  if (!empty($policy_filters)) {
    $sql .= '(';
    foreach ($policy_filters as $filter) {
      $sql .= sprintf("profile.%s>0 OR ", $filter);
    }
    $sql = rtrim($sql, ' OR ');
    $sql .= ')';
  }
  if ($company_id !== 'ANY') {
    if (!empty($policy_filters)) {
      $sql .= ' AND ';
    }
    $sql .= sprintf("relation.CompanyId='%s'", $company_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_properties($uid, $company_id = 'ANY',
				    $policy_filters = array(),
				    $equipment_filters = array(),
				    $enforcement_filters = array()) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT property.* FROM property_table AS property";
  $sql .= " INNER JOIN relations_table AS relation";
  $sql .= "  ON (property.PropertyId=relation.PropertyId)";
  if (0 < count($policy_filters)) {
    $sql .= " INNER JOIN policy_table AS policy";
    $sql .= "  ON (relation.PropertyId=policy.PropertyId)";
    $sql .= " INNER JOIN profile_table AS profile";
    $sql .= "  ON (policy.ProfileId=profile.ProfileId)";
  }
  $sql .= " INNER JOIN user_table AS user";
  $sql .= "  ON (relation.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (relation.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d", $uid);
  if (0 < count($policy_filters)) {
    $sql .= " AND (";
    foreach ($policy_filters as $filter) {
      $sql .= sprintf("profile.%s>0 OR ", $filter);
    }
    $sql = rtrim($sql, ' OR ');
    $sql .= ')';
  }
  if ($company_id !== 'ANY') {
    $sql .= sprintf(" AND relation.CompanyId='%s'", $company_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_admin_policies($company_id = 'ANY',
					$policy_filters = array(),
					$enforcement_filters = array()) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT policy.* FROM policy_table AS policy";
  $sql .= " INNER JOIN relations_table AS relation";
  $sql .= "  ON (policy.PropertyId=relation.PropertyId)";
  if (0 < count($policy_filters)) {
    $sql .= " INNER JOIN profile_table AS profile";
    $sql .= "  ON (policy.ProfileId=profile.ProfileId)";
  }
  if (!empty($enforcement_filters)) {
    $sql .= " INNER JOIN enforcement_table AS enforcement";
    $sql .= "  ON (policy.EnforcementId=enforcement.EnforcementId)";
  }
  if (!empty($policy_filters) or !empty($enforcement_filters) or $company_id !== 'ANY') {
    $sql .= " WHERE ";
  }
  if ($company_id !== 'ANY') {
    $sql .= sprintf("relation.CompanyId='%s' AND ", $company_id);
  }
  if (!empty($policy_filters) and !empty($enforcement_filters)) {
    $sql .= '(';
  }
  if (!empty($policy_filters)) {
    $sql .= '(';
    foreach ($policy_filters as $filter) {
      $sql .= sprintf("profile.%s>0 OR ", $filter);
    }
    $sql = rtrim($sql, ' OR ');
    $sql .= ') OR ';
  }
  if (!empty($enforcement_filters)) {
    $sql .= '(';
    foreach ($enforcement_filters as $filter) {
      $sql .= sprintf("enforcement.%s>0 OR ", $filter);
    }
    $sql = rtrim($sql, ' OR ');
    $sql .= ') OR ';
  }
  $sql = rtrim($sql, ' OR ');
  $sql = rtrim($sql, ' AND ');
  if (!empty($policy_filters) and !empty($enforcement_filters)) {
    $sql .= ')';
  }
  $sql .= " ORDER BY policy.Alias";
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_policies($uid, $company_id = 'ANY',
				  $policy_filters = array(),
				  $enforcement_filters = array()) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT policy.* FROM policy_table AS policy";
  $sql .= " INNER JOIN property_table AS property";
  $sql .= "  ON policy.PropertyId=property.PropertyId";
  $sql .= " INNER JOIN relations_table AS relation";
  $sql .= "  ON property.PropertyId=relation.PropertyId";
  $sql .= " INNER JOIN user_table AS user";
  $sql .= "  ON (relation.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (property.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= "    AND (policy.LotId=user.LotId OR user.LotId='ANY')";
  if (0 < count($policy_filters)) {
    $sql .= " INNER JOIN profile_table AS profile";
    $sql .= "  ON (policy.ProfileId=profile.ProfileId)";
  }
  if (!empty($enforcement_filters)) {
    $sql .= " INNER JOIN enforcement_table AS enforcement";
    $sql .= "  ON (policy.EnforcementId=enforcement.EnforcementId)";
  }
  $sql .= sprintf(" WHERE user.UID=%d AND ", $uid);
  if ($company_id !== 'ANY') {
    $sql .= sprintf("relation.CompanyId='%s' AND ", $company_id);
  }
  if (!empty($policy_filters) and !empty($enforcement_filters)) {
    $sql .= '(';
  }
  if (0 < count($policy_filters)) {
    $sql .= "(";
    foreach ($policy_filters as $filter) {
      $sql .= sprintf("profile.%s>0 OR ", $filter);
    }
    $sql = rtrim($sql, ' OR ');
    $sql .= ') OR ';
  }
  if (!empty($enforcement_filters)) {
    $sql .= '(';
    foreach ($enforcement_filters as $filter) {
      $sql .= sprintf("enforcement.%s>0 OR ", $filter);
    }
    $sql = rtrim($sql, ' OR ');
    $sql .= ') OR ';
  }
  $sql = rtrim($sql, ' OR ');
  if (!empty($policy_filters) and !empty($enforcement_filters)) {
    $sql .= ')';
  }
  $sql = rtrim($sql, ' AND ');
  $sql .= " ORDER BY policy.Alias";
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_profiles($uid) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT profile.* FROM profile_table AS profile";
  $sql .= " INNER JOIN policy_table AS policy";
  $sql .= "  ON profile.ProfileId=policy.ProfileId AND profile.Type=1";
  $sql .= " INNER JOIN property_table AS property";
  $sql .= "  ON property.PropertyId=policy.PropertyId";
  $sql .= " INNER JOIN relations_table AS relation";
  $sql .= "  ON property.PropertyId=relation.PropertyId";
  $sql .= " INNER JOIN user_table AS user";
  $sql .= "  ON (relation.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (property.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= "    AND (policy.LotId=user.LotId OR user.LotId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d", $uid);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_enforcement($uid) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT enforcement.* FROM enforcement_table AS enforcement";
  $sql .= " INNER JOIN policy_table AS policy";
  $sql .= "  ON enforcement.EnforcementId=policy.EnforcementId AND enforcement.Type=1";
  $sql .= " INNER JOIN relations_table AS relation";
  $sql .= "  ON policy.PropertyId=relation.PropertyId";
  $sql .= " INNER JOIN user_table AS user";
  $sql .= "  ON (relation.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (property.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= "    AND (policy.LotId=user.LotId OR user.LotId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d", $uid);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_admin_enforcers($company_id,
					 $property_id = 'ANY', $lot_id = 'ANY',
					 $filter_active = TRUE, $active = TRUE,
					 $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT enforcer.* FROM enforcer_table AS enforcer";
  if ($property_id !== 'ANY' or $lot_id !== 'ANY') {
    $sql .= " INNER JOIN enforcerprop_table AS enforcerprop";
    $sql .= "  ON (enforcer.CompanyId=enforcerprop.CompanyId)";
    $sql .= "   AND (enforcer.EnforcerId=enforcerprop.EnforcerId)";
  }
  $sql .= sprintf(" WHERE enforcer.CompanyId='%s'", $company_id);
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" AND enforcerprop.PropertyId='%s'", $property_id);
  }
  if ($lot_id !== 'ANY') {
    $sql .= sprintf(" AND enforcerprop.LotId='%s'", $lot_id);
  }
  if ($filter_active) {
    $sql .= sprintf(" AND enforcer.Active=%d", $active ? 1 : 0);
  }
  if ($filter_suspended) {
    $sql .= sprintf(" AND enforcer.Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_enforcers($uid, $company_id,
				   $property_id = 'ANY', $lot_id = 'ANY',
				   $filter_active = TRUE, $active = TRUE,
				   $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT enforcer.* FROM enforcer_table AS enforcer";
  $sql .= " INNER JOIN enforcerprop_table AS enforcerprop";
  $sql .= "  ON (enforcer.CompanyId=enforcerprop.CompanyId)";
  $sql .= "   AND (enforcer.EnforcerId=enforcerprop.EnforcerId)";
  $sql .= " INNER JOIN user_table AS user";
  $sql .= "  ON (enforcer.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND enforcer.EnforcerId=user.EnforcerId";
  $sql .= "    AND (enforcerprop.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= "     AND (enforcerprop.LotId=user.LotId OR user.LotId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d AND enforcer.CompanyId='%s'", $uid, $company_id);
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" AND enforcerprop.PropertyId='%s'", $property_id);
  }
  if ($lot_id !== 'ANY') {
    $sql .= sprintf(" AND enforcerprop.LotId='%s'", $lot_id);
  }
  if ($filter_active) {
    $sql .= sprintf(" AND enforcer.Active=%d", $active ? 1 : 0);
  }
  if ($filter_suspended) {
    $sql .= sprintf(" AND enforcer.Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_enforcerprops($uid, $company_id, $enforcer_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT enforcerprop.* FROM enforcerprop_table AS enforcerprop";
  $sql .= " INNER JOIN user_table AS user";
  $sql .= "  ON (enforcerprop.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (enforcerprop.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= "    AND (enforcerprop.LotId=user.LotId OR enforcerprop.LotId='ANY' OR user.LotId='ANY')";
  $sql .= "     AND enforcerprop.EnforcerId=user.EnforcerId";
  $sql .= sprintf(" WHERE user.UID=%d AND enforcerprop.CompanyId='%s' AND enforcerprop.EnforcerId='%s'",
		  $uid, $company_id, $enforcer_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_equipment($uid) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT equipment.* FROM equipment_table AS equipment";
  $sql .= " INNER JOIN policy_table AS policy";
  $sql .= "  ON equipment.EquipmentId=policy.EquipmentId AND equipment.Type=1";
  $sql .= " INNER JOIN relations_table AS relation";
  $sql .= "  ON policy.PropertyId=relation.PropertyId";
  $sql .= " INNER JOIN user_table AS user";
  $sql .= "  ON (relation.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (policy.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= "    AND (policy.LotId=user.LotId OR user.LotId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d", $uid);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_admin_stalls($level, $long, $short, $valet) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT stall.* FROM stall_table AS stall";
  $sql .= " INNER JOIN stallevel_table AS stallevel";
  $sql .= "  ON stall.PropertyId=stallevel.PropertyId";
  $sql .= "   AND stall.LotId=stallevel.LotId";
  $sql .= "    AND stall.LevelId=stallevel.LevelId";
  $sql .= sprintf(" WHERE stallevel.Level=%d", $level);
  $sql .= sprintf(" AND stallevel.Long=%d AND stallevel.Short=%d AND stallevel.Valet=%d",
		  $long ? 1 : 0, $short ? 1 : 0, $valet ? 1 : 0);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_stalls($uid, $level, $long, $short, $valet) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT stall.* FROM stall_table AS stall";
  $sql .= " INNER JOIN stallevel_table AS stallevel";
  $sql .= "  ON stall.PropertyId=stallevel.PropertyId";
  $sql .= "   AND stall.LotId=stallevel.LotId";
  $sql .= "    AND stall.LevelId=stallevel.LevelId";
  $sql .= " INNER JOIN property_table AS property";
  $sql .= "  ON stall.PropertyId=property.PropertyId";
  $sql .= " INNER JOIN relations_table AS relation";
  $sql .= "  ON property.PropertyId=relation.PropertyId";
  $sql .= " INNER JOIN user_table AS user";
  $sql .= "  ON (relation.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (relation.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= "    AND (stall.LotId=user.LotId OR user.LotId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d", $uid);
  $sql .= sprintf(" AND stallevel.Long=%d AND stallevel.Short=%d AND stallevel.Valet=%d",
		  $long ? 1 : 0, $short ? 1 : 0, $valet ? 1 : 0);
  $sql .= sprintf(" AND stallevel.Level=%d", $level);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_admin_agents($company_id, $app_id,
				      $property_id = 'ANY', $lot_id = 'ANY',
				      $active = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT agent.* FROM agent_table AS agent";
  $sql .= " INNER JOIN mobileapp_table AS mobileapp";
  $sql .= "  ON agent.AppId=mobileapp.AppId";
  if ($property_id !== 'ANY' or $lot_id !== 'ANY') {
    $sql .= " INNER JOIN agentprop_table AS agentprop";
    $sql .= "  ON agent.CompanyId=agentprop.CompanyId";
    $sql .= "   AND agent.AgentId=agentprop.AgentId";
  }
  $sql .= sprintf(" WHERE agent.Active=%d AND agent.CompanyId='%s'",
		  $active ? 1 : 0, $company_id);
  $sql .= sprintf(" AND mobileapp.AppId='%s'", $app_id);
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" AND (agentprop.PropertyId='%s' OR agentprop.PropertyId='ANY')",
		    $property_id);
  }
  if ($lot_id !== 'ANY') {
    $sql .= sprintf(" AND (agentprop.LotId='%s' OR agentprop.LotId='ANY')",
		    $lot_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_agents($uid, $company_id, $app_id,
				$property_id = 'ANY', $lot_id = 'ANY',
				$active = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT agent.* FROM agent_table AS agent";
  $sql .= " INNER JOIN mobileapp_table AS mobileapp";
  $sql .= "  ON agent.AppId=mobileapp.AppId";
  $sql .= " INNER JOIN agentprop_table AS agentprop";
  $sql .= "  ON agent.CompanyId=agentprop.CompanyId";
  $sql .= "   AND agent.AgentId=agentprop.AgentId";
  $sql .= " INNER JOIN user_table AS user";
  $sql .= "  ON (agentprop.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (agentprop.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= "    AND (agentprop.LotId=user.LotId OR user.LotId='ANY')";
  $sql .= sprintf(" WHERE agent.Active=%d AND user.UID=%d AND agent.CompanyId='%s'",
		  $active ? 1 : 0, $uid, $company_id);
  $sql .= sprintf(" AND mobileapp.AppId='%s'", $app_id);
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" AND (agentprop.PropertyId='%s' OR agentprop.PropertyId='ANY')",
		    $property_id);
  }
  if ($lot_id !== 'ANY') {
    $sql .= sprintf(" AND (agentprop.LotId='%s' OR agentprop.LotId='ANY')",
		    $lot_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_agentprops($uid, $company_id, $agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT agentprop.* FROM agentprop_table AS agentprop";
  $sql .= " INNER JOIN relations_table AS relation";
  $sql .= "  ON agentprop.CompanyId=relation.CompanyId";
  $sql .= "   AND agentprop.PropertyId=relation.PropertyId";
  $sql .= " INNER JOIN user_table AS user";
  $sql .= "  ON (agentprop.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (agentprop.ClientId=user.ClientId OR user.ClientId='ANY')";
  $sql .= "    AND (agentprop.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= "     AND (agentprop.LotId=user.LotId OR agentprop.LotId='ANY' OR user.LotId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d AND agentprop.CompanyId='%s' AND agentprop.AgentId='%s'",
		  $uid, $company_id, $agent_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
// This requires PropertyId to CompanyId via relations
// and insertions into multiple tables
function chdbprop_cc_insert_agent_lane($uid, $property_id, $agent_id, $app_id, $cluster_id, &$id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO agent_table (CreatorUID,CompanyId,AgentId,AppId,ClusterId)";
  $sql .= sprintf(" SELECT %d,relation.CompanyId,'%s','%s','%s'".
		  " FROM relations_table AS relation",
		  $uid, $agent_id, $app_id, $cluster_id);
  $sql .= sprintf(" WHERE relation.PropertyId='%s'", $property_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



function chdbprop_cc_get_admin_patrollers($company_id,
					  $property_id = 'ANY', $lot_id = 'ANY',
					  $enforcer_id = NULL,
					  $active = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT patroller.* FROM patroller_table AS patroller";
  if ($property_id !== 'ANY' or $lot_id !== 'ANY') {
    $sql .= " INNER JOIN patrollerprop_table AS patrollerprop";
    $sql .= "  ON patroller.CompanyId=patrollerprop.CompanyId";
    $sql .= "   AND patroller.PatrollerId=patrollerprop.PatrollerId";
  }
  if (NULL !== $enforcer_id) {
    $sql .= " INNER JOIN enforcerprop_table AS enforcerprop";
    $sql .= "  ON patroller.CompanyId=enforcerprop.CompanyId";
    $sql .= "   AND patroller.EnforcerId=enforcerprop.EnforcerId";
    if ($property_id !== 'ANY' or $lot_id !== 'ANY') {
      $sql .= "   AND patrollerprop.PropertyId=enforcerprop.PropertyId";
      $sql .= "    AND patrollerprop.LotId=enforcerprop.LotId";
    }
  }
  $sql .= sprintf(" WHERE patroller.Active=%d AND patroller.CompanyId='%s'",
		  $active ? 1 : 0, $company_id);
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" AND (patrollerprop.PropertyId='%s' OR patrollerprop.PropertyId='ANY')",
		    $property_id);
  }
  if ($lot_id !== 'ANY') {
    $sql .= sprintf(" AND (patrollerprop.LotId='%s' OR patrollerprop.LotId='ANY')",
		    $lot_id);
  }
  if (NULL !== $enforcer_id) {
    $sql .= sprintf(" AND patroller.EnforcerId='%s'", $enforcer_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_patrollers($uid, $company_id,
				    $property_id = 'ANY', $lot_id = 'ANY',
				    $enforcer_id = NULL,
				    $active = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT patroller.* FROM patroller_table AS patroller";
  $sql .= " INNER JOIN patrollerprop_table AS patrollerprop";
  $sql .= "  ON patroller.CompanyId=patrollerprop.CompanyId";
  $sql .= "   AND patroller.PatrollerId=patrollerprop.PatrollerId";
  if (NULL !== $enforcer_id) {
    $sql .= " INNER JOIN enforcerprop_table AS enforcerprop";
    $sql .= "  ON patroller.CompanyId=enforcerprop.CompanyId";
    $sql .= "   AND patroller.EnforcerId=enforcerprop.EnforcerId";
    $sql .= "    AND patrollerprop.PropertyId=enforcerprop.PropertyId";
    $sql .= "     AND patrollerprop.LotId=enforcerprop.LotId";
  }
  $sql .= " INNER JOIN user_table AS user";
  $sql .= "  ON (patrollerprop.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (patrollerprop.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= "    AND (patrollerprop.LotId=user.LotId OR user.LotId='ANY')";
  if (NULL !== $enforcer_id) {
    $sql .= "    AND enforcerprop.EnforcerId=user.EnforcerId";
  }
  $sql .= sprintf(" WHERE patroller.Active=%d AND user.UID=%d AND patroller.CompanyId='%s'",
		  $active ? 1 : 0, $uid, $company_id);
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" AND (patrollerprop.PropertyId='%s' OR patrollerprop.PropertyId='ANY')",
		    $property_id);
  }
  if ($lot_id !== 'ANY') {
    $sql .= sprintf(" AND (patrollerprop.LotId='%s' OR patrollerprop.LotId='ANY')",
		    $lot_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_patrollerprops($uid, $company_id, $patroller_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT patrollerprop.* FROM patrollerprop_table AS patrollerprop";
  $sql .= " INNER JOIN relations_table AS relation";
  $sql .= "  ON patrollerprop.CompanyId=relation.CompanyId";
  $sql .= "   AND patrollerprop.PropertyId=relation.PropertyId";
  $sql .= " INNER JOIN user_table AS user";
  $sql .= "  ON (patrollerprop.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (patrollerprop.ClientId=user.ClientId OR user.ClientId='ANY')";
  $sql .= "    AND (patrollerprop.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= "     AND (patrollerprop.LotId=user.LotId OR patrollerprop.LotId='ANY' OR user.LotId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d AND patrollerprop.CompanyId='%s' AND patrollerprop.PatrollerId='%s'",
		  $uid, $company_id, $patroller_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_laneprops($uid, $lane_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT laneprop.* FROM laneprop_table AS laneprop";
  $sql .= " INNER JOIN user_table AS user";
  $sql .= "  ON (laneprop.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= "   AND (laneprop.LotId=user.LotId OR user.LotId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d AND laneprop.LaneId='%s'",
		  $uid, $laneid);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



/****************   company_table  **********************/



function chdbprop_get_all_companies($filter_active = TRUE, $active = TRUE,
				    $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM company_table";
  if ($filter_active AND $filter_suspended) {
    $sql .= sprintf(" WHERE Active=%d AND Suspended=%d",
		    $active ? 1 : 0, $suspended ? 1 : 0);
  } elseif ($filter_active) {
    $sql .= sprintf(" WHERE Active=%d", $active ? 1 : 0);
  } elseif ($filter_suspended) {
    $sql .= sprintf(" WHERE Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_company($company_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM company_table WHERE CompanyId='%s'",
		 $company_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_company_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM company_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_company($uid, $company_id, $alias,
				 $street, $city, $postal_code, $province, $country,
				 &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO company_table (CreatorUID,CompanyId,Alias,";
  $sql .= "Street,City,PostalCode,Province,Country) VALUES ";
  $sql .= sprintf("(%d,'%s','%s','%s','%s','%s','%s','%s')",
		  $uid, $company_id, $alias,
		  $street, $city, $postal_code, $province, $country);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1
function chdbprop_update_company($id, $alias, $street,
				 $city, $postal_code, $province, $country) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = 'UPDATE company_table SET';
  $sql .= sprintf(" Alias='%s',Street='%s',City='%s'",
		  $alias, $street, $city);
  $sql .= sprintf(",PostalCode='%s',Province='%s',Country='%s'",
		  $postal_code, $province, $country);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1
function chdbprop_update_company_latlon($id, $lat, $lon, $manlatlon = FALSE) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = 'UPDATE company_table SET';
  $sql .= sprintf(" ManLatLon=%d,Lat='%s',Lon='%s'", $manlatlon ? 1 : 0, $lat, $lon);
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_update_company_status($id, $activate, $suspended) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE company_table SET ";
  $sql .= sprintf("Active=%d,Suspended=%d",
		  $activate ? 1 : 0, $suspended ? 1 : 0);
  if ($activate) {
    $sql .= ",Deactivated=0,DeactivationTime=NULL,DeactivationUID=NULL,DeactivationReason=NULL";
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_retire_company_id($id, $uid, $reason = '',
				    $retention_days = 365) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE company_table SET Active=0,Deactivated=1,DeactivationTime=CURRENT_TIMESTAMP";
  $sql .= sprintf(",DeactivationUID=%d,DeactivationReason='%s'",
		  $uid, $reason);
  if ($retention_days > 0) {
    $sql .= sprintf(",RetentionTime=DATETIME(CURRENT_TIMESTAMP,'%d DAYS')",
		    $retention_days);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_delete_company_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM company_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   companyadmin_table  **********************/



function chdbprop_get_companyadmins($company_id,
				    $filter_active = TRUE, $active = TRUE,
				    $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM companyadmin_table";
  $sql .= sprintf(" WHERE CompanyId='%s'", $company_id);
  if ($filter_active) {
    $sql .= sprintf(" AND Active=%d", $active ? 1 : 0);
  }
  if ($filter_suspended) {
    $sql .= sprintf(" AND Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_companyadmin($company_id, $admin_id, $lat, $lon,
				      $prename_id, $firstname, $middlename, $lastname, $sufname_id,
				      $street, $city, $postalcode, $province, $country,
				      $phone, $email, $emailnotification,
				      $role, $department, $title, $username, $password,
				      &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO companyadmin_table (CompanyId,AdminId,Lat,Lon,";
  $sql .= "PrenameId,FirstName,MiddleName,LastName,SufnameId,";
  $sql .= "Street,City,PostalCode,Province,Country, ";
  $sql .= "Phone,Email,EmailNotification, ";
  $sql .= "Role,Department,Title) VALUES (";
  $sql .= sprintf("'%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s',%d, ".
		  "'%s','%s','%s')",
		  $company_id, $admin_id, $lat, $lon,
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id,
		  $street, $city, $postalcode, $province, $country,
		  $phone, $email, $emailnotification ? 1 : 0,
		  $role, $department, $title);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  // SQL
  $sql = "UPDATE companyadmin_table SET ";
  if (NULL === $username) {
    $sql .= "Username=NULL";
  } else {
    $sql .= sprintf("Username='%s'", $username);
  }
  if (NULL === $password) {
    $sql .= ",Password=NULL";
  } else {
    $sql .= sprintf(",Password='%s'", $password);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_update_companyadmin($id,
				      $prename_id, $firstname, $middlename, $lastname, $sufname_id,
				      $street, $city, $postalcode, $province, $country,
				      $phone, $email, $emailnotification,
				      $role, $department, $title, $username, $password) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE companyadmin_table SET ";
  $sql .= sprintf("PrenameId='%s',FirstName='%s',MiddleName='%s',LastName='%s',SufnameId='%s', ",
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id);
  $sql .= sprintf("Street='%s',City='%s',PostalCode='%s',Province='%s',Country='%s', ",
		  $street, $city, $postalcode, $province, $country);
  $sql .= sprintf("Phone='%s',Email='%s',EmailNotification=%d, ",
		  $phone, $email, $emailnotification ? 1 : 0);
  $sql .= sprintf("Role='%s',Department='%s',Title='%s', ",
		  $role, $department, $title);
  if (NULL === $username) {
    $sql .= "Username=NULL,";
  } else {
    $sql .= sprintf("Username='%s',", $username);
  }
  if (NULL === $password) {
    $sql .= "Password=NULL";
  } else {
    $sql .= sprintf("Password='%s'", $password);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
  drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1
function chdbprop_update_companyadmin_latlon($id, $lat, $lon, $manlatlon = FALSE) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = 'UPDATE companyadmin_table SET';
  $sql .= sprintf(" ManLatLon=%d,Lat='%s',Lon='%s'", $manlatlon ? 1 : 0, $lat, $lon);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_companyadmins_status($company_id, $activate, $suspended) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE companyadmin_table SET ";
  $sql .= sprintf("Active=%d,Suspended=%d",
		  $activate ? 1 : 0, $suspended ? 1 : 0);
  if ($activate) {
    $sql .= ",Deactivated=0,DeactivationTime=NULL,DeactivationUID=NULL,DeactivationReason=NULL";
  }
  $sql .= sprintf(" WHERE CompanyId='%s'", $company_id);
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
function chdbprop_retire_companyadmins($company_id, $uid,
				       $reason = '', $retention_days = 365) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE companyadmin_table SET Active=0,Deactivated=1,DeactivationTime=CURRENT_TIMESTAMP";
  $sql .= sprintf(",DeactivationUID=%d,DeactivationReason='%s'",
		  $uid, $reason);
  if ($retention_days > 0) {
    $sql .= sprintf(",RetentionTime=DATETIME(CURRENT_TIMESTAMP,'%d DAYS')",
		    $retention_days);
  }
  $sql .= sprintf(" WHERE CompanyId='%s'", $company_id);
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
function chdbprop_delete_companyadmin($company_id, $admin_id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM companyadmin_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND AdminId='%s'",
		  $company_id, $admin_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   region_table  **********************/



function chdbprop_get_all_regions($filter_active = TRUE, $active = TRUE,
				  $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM region_table";
  if ($filter_active AND $filter_suspended) {
    $sql .= sprintf(" WHERE Active=%d AND Suspended=%d",
		    $active ? 1 : 0, $suspended ? 1 : 0);
  } elseif ($filter_active) {
    $sql .= sprintf(" WHERE Active=%d", $active ? 1 : 0);
  } elseif ($filter_suspended) {
    $sql .= sprintf(" WHERE Suspended=%d", $suspended ? 1 : 0);
  }
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_regions($company_id,
			      $filter_active = TRUE, $active = TRUE,
			      $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM region_table WHERE CompanyId='%s'", $company_id);
  if ($filter_active) {
    $sql .= sprintf(" AND Active=%d", $active ? 1 : 0);
  }
  if ($filter_suspended) {
    $sql .= sprintf(" AND Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_region($company_id, $region_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM region_table WHERE CompanyId='%s' AND RegionId='%s'",
		 $company_id, $region_id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_region_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM region_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_region($uid, $company_id, $region_id, $alias,
				 $street, $city, $postal_code, $province, $country,
				 &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO region_table (CreatorUID,CompanyId,RegionId,Alias,";
  $sql .= "Street,City,PostalCode,Province,Country) VALUES ";
  $sql .= sprintf("(%d,'%s','%s','%s','%s','%s','%s','%s','%s')",
		  $uid, $company_id, $region_id, $alias,
		  $street, $city, $postal_code, $province, $country);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1
function chdbprop_update_region($id, $alias, $street,
				$city, $postal_code, $province, $country) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = 'UPDATE region_table SET';
  $sql .= sprintf(" Alias='%s',Street='%s',City='%s'",
		  $alias, $street, $city);
  $sql .= sprintf(",PostalCode='%s',Province='%s',Country='%s'",
		  $postal_code, $province, $country);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1
function chdbprop_update_region_latlon($id, $lat, $lon, $manlatlon = FALSE) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = 'UPDATE region_table SET';
  $sql .= sprintf(" ManLatLon=%d,Lat='%s',Lon='%s'", $manlatlon ? 1 : 0, $lat, $lon);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_region_status($id, $activate, $suspended) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE region_table SET ";
  $sql .= sprintf("Active=%d,Suspended=%d",
		  $activate ? 1 : 0, $suspended ? 1 : 0);
  if ($activate) {
    $sql .= ",Deactivated=0,DeactivationTime=NULL,DeactivationUID=NULL,DeactivationReason=NULL";
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_retire_region_id($id, $uid, $reason = '',
				   $retention_days = 365) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE region_table SET Active=0,Deactivated=1,DeactivationTime=CURRENT_TIMESTAMP";
  $sql .= sprintf(",DeactivationUID=%d,DeactivationReason='%s'",
		  $uid, $reason);
  if ($retention_days > 0) {
    $sql .= sprintf(",RetentionTime=DATETIME(CURRENT_TIMESTAMP,'%d DAYS')",
		    $retention_days);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_delete_region_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM region_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   regionadmin_table  **********************/



function chdbprop_get_regionadmins($company_id, $region_id,
				   $filter_active = TRUE, $active = TRUE,
				   $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM regionadmin_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND RegionId='%s'",
		  $company_id, $region_id);
  if ($filter_active) {
    $sql .= sprintf(" AND Active=%d", $active ? 1 : 0);
  }
  if ($filter_suspended) {
    $sql .= sprintf(" AND Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_regionadmin($company_id, $region_id, $admin_id, $lat, $lon,
				     $prename_id, $firstname, $middlename, $lastname, $sufname_id,
				     $street, $city, $postalcode, $province, $country,
				     $phone, $email, $emailnotification,
				     $role, $department, $title, $username, $password,
				     &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO regionadmin_table (CompanyId,RegionId,AdminId,Lat,Lon,";
  $sql .= "PrenameId,FirstName,MiddleName,LastName,SufnameId,";
  $sql .= "Street,City,PostalCode,Province,Country, ";
  $sql .= "Phone,Email,EmailNotification, ";
  $sql .= "Role,Department,Title) VALUES (";
  $sql .= sprintf("'%s','%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s',%d, ".
		  "'%s','%s','%s')",
		  $company_id, $region_id, $admin_id, $lat, $lon,
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id,
		  $street, $city, $postalcode, $province, $country,
		  $phone, $email, $emailnotification ? 1 : 0,
		  $role, $department, $title);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  // SQL
  $sql = "UPDATE regionadmin_table SET ";
  if (NULL === $username) {
    $sql .= "Username=NULL";
  } else {
    $sql .= sprintf("Username='%s'", $username);
  }
  if (NULL === $password) {
    $sql .= ",Password=NULL";
  } else {
    $sql .= sprintf(",Password='%s'", $password);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_update_regionadmin($id,
				     $prename_id, $firstname, $middlename, $lastname, $sufname_id,
				     $street, $city, $postalcode, $province, $country,
				     $phone, $email, $emailnotification,
				     $role, $department, $title, $username, $password) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE regionadmin_table SET ";
  $sql .= sprintf("PrenameId='%s',FirstName='%s',MiddleName='%s',LastName='%s',SufnameId='%s', ",
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id);
  $sql .= sprintf("Street='%s',City='%s',PostalCode='%s',Province='%s',Country='%s', ",
		  $street, $city, $postalcode, $province, $country);
  $sql .= sprintf("Phone='%s',Email='%s',EmailNotification=%d, ",
		  $phone, $email, $emailnotification ? 1 : 0);
  $sql .= sprintf("Role='%s',Department='%s',Title='%s', ",
		  $role, $department, $title);
  if (NULL === $username) {
    $sql .= "Username=NULL,";
  } else {
    $sql .= sprintf("Username='%s',", $username);
  }
  if (NULL === $password) {
    $sql .= "Password=NULL";
  } else {
    $sql .= sprintf("Password='%s'", $password);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_update_regionadmin_latlon($id, $lat, $lon, $manlatlon = FALSE) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = 'UPDATE regionadmin_table SET';
  $sql .= sprintf(" ManLatLon=%d,Lat='%s',Lon='%s'", $manlatlon ? 1 : 0, $lat, $lon);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_regionadmins_status($company_id, $region_id, $activate, $suspended) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE regionadmin_table SET ";
  $sql .= sprintf("Active=%d,Suspended=%d",
		  $activate ? 1 : 0, $suspended ? 1 : 0);
  if ($activate) {
    $sql .= ",Deactivated=0,DeactivationTime=NULL,DeactivationUID=NULL,DeactivationReason=NULL";
  }
  $sql .= sprintf(" WHERE CompanyId='%s' AND RegionId='%s'",
		  $company_id, $region_id);
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
function chdbprop_retire_regionadmins($company_id, $region_id, $uid,
				      $reason = '', $retention_days = 365) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE regionadmin_table SET Active=0,Deactivated=1,DeactivationTime=CURRENT_TIMESTAMP";
  $sql .= sprintf(",DeactivationUID=%d,DeactivationReason='%s'",
		  $uid, $reason);
  if ($retention_days > 0) {
    $sql .= sprintf(",RetentionTime=DATETIME(CURRENT_TIMESTAMP,'%d DAYS')",
		    $retention_days);
  }
  $sql .= sprintf(" WHERE CompanyId='%s' AND RegionId='%s'",
		  $company_id, $region_id);
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
function chdbprop_delete_regionadmin_id($id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM regionadmin_table";
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   client_table  **********************/



function chdbprop_get_all_clients($filter_active = TRUE, $active = TRUE,
				  $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM client_table";
  if ($filter_active AND $filter_suspended) {
    $sql .= sprintf(" WHERE Active=%d AND Suspended=%d",
		    $active ? 1 : 0, $suspended ? 1 : 0);
  } elseif ($filter_active) {
    $sql .= sprintf(" WHERE Active=%d", $active ? 1 : 0);
  } elseif ($filter_suspended) {
    $sql .= sprintf(" WHERE Suspended=%d", $suspended ? 1 : 0);
  }
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_clients($company_id, $region_id,
			      $filter_active = TRUE, $active = TRUE,
			      $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM client_table WHERE ";
  $sql .= sprintf("CompanyId='%s'", $company_id);
  if ($region_id !== 'ANY') {
    $sql .= sprintf(" AND (RegionId='%s' OR RegionId='ANY')", $region_id);
  }
  if ($filter_active) {
    $sql .= sprintf(" AND Active=%d", $active ? 1 : 0);
  }
  if ($filter_suspended) {
    $sql .= sprintf(" AND Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_client($company_id, $client_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM client_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND ClientId='%s'",
		  $company_id, $client_id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_client_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM client_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_client($uid, $company_id, $region_id, $client_id, $alias,
				 $street, $city, $postal_code, $province, $country,
				 &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO client_table (CreatorUID,CompanyId,RegionId,ClientId,Alias,";
  $sql .= "Street,City,PostalCode,Province,Country) VALUES ";
  $sql .= sprintf("(%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s')",
		  $uid, $company_id, $region_id, $client_id, $alias,
		  $street, $city, $postal_code, $province, $country);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1
function chdbprop_update_client($id, $alias, $region_id,
				$street, $city, $postal_code, $province, $country) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = 'UPDATE client_table SET';
  $sql .= sprintf(" Alias='%s',RegionId='%s'", $alias, $region_id);
  $sql .= sprintf(",Street='%s',City='%s'", $street, $city);
  $sql .= sprintf(",PostalCode='%s',Province='%s',Country='%s'",
		  $postal_code, $province, $country);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1
function chdbprop_update_client_latlon($id, $lat, $lon, $manlatlon = FALSE) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = 'UPDATE client_table SET';
  $sql .= sprintf(" ManLatLon=%d,Lat='%s',Lon='%s'", $manlatlon ? 1 : 0, $lat, $lon);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_client_status($id, $activate, $suspended) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE client_table SET ";
  $sql .= sprintf("Active=%d,Suspended=%d",
		  $activate ? 1 : 0, $suspended ? 1 : 0);
  if ($activate) {
    $sql .= ",Deactivated=0,DeactivationTime=NULL,DeactivationUID=NULL,DeactivationReason=NULL";
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_retire_client_id($id, $uid, $reason = '',
				    $retention_days = 365) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE client_table SET Active=0,Deactivated=1,DeactivationTime=CURRENT_TIMESTAMP";
  $sql .= sprintf(",DeactivationUID=%d,DeactivationReason='%s'",
		  $uid, $reason);
  if ($retention_days > 0) {
    $sql .= sprintf(",RetentionTime=DATETIME(CURRENT_TIMESTAMP,'%d DAYS')",
		    $retention_days);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_delete_client_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM client_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   clientadmin_table  **********************/



function chdbprop_get_clientadmins($company_id, $client_id,
				   $filter_active = TRUE, $active = TRUE,
				   $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM clientadmin_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND ClientId='%s'",
		  $company_id, $client_id);
  if ($filter_active) {
    $sql .= sprintf(" AND Active=%d", $active ? 1 : 0);
  }
  if ($filter_suspended) {
    $sql .= sprintf(" AND Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_clientadmin($company_id, $client_id, $admin_id, $lat, $lon,
				     $prename_id, $firstname, $middlename, $lastname, $sufname_id,
				     $street, $city, $postalcode, $province, $country,
				     $phone, $email, $emailnotification,
				     $role, $department, $title, $username, $password,
				     &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO clientadmin_table (CompanyId,ClientId,AdminId,Lat,Lon,";
  $sql .= "PrenameId,FirstName,MiddleName,LastName,SufnameId,";
  $sql .= "Street,City,PostalCode,Province,Country, ";
  $sql .= "Phone,Email,EmailNotification, ";
  $sql .= "Role,Department,Title) VALUES (";
  $sql .= sprintf("'%s','%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s',%d, ".
		  "'%s','%s','%s')",
		  $company_id, $client_id, $admin_id, $lat, $lon,
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id,
		  $street, $city, $postalcode, $province, $country,
		  $phone, $email, $emailnotification ? 1 : 0,
		  $role, $department, $title);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  // SQL
  $sql = "UPDATE clientadmin_table SET ";
  if (NULL === $username) {
    $sql .= "Username=NULL";
  } else {
    $sql .= sprintf("Username='%s'", $username);
  }
  if (NULL === $password) {
    $sql .= ",Password=NULL";
  } else {
    $sql .= sprintf(",Password='%s'", $password);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_clientadmin($id,
				     $prename_id, $firstname, $middlename, $lastname, $sufname_id,
				     $street, $city, $postalcode, $province, $country,
				     $phone, $email, $emailnotification,
				     $role, $department, $title, $username, $password) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE clientadmin_table SET ";
  $sql .= sprintf("PrenameId='%s',FirstName='%s',MiddleName='%s',LastName='%s',SufnameId='%s', ",
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id);
  $sql .= sprintf("Street='%s',City='%s',PostalCode='%s',Province='%s',Country='%s', ",
		  $street, $city, $postalcode, $province, $country);
  $sql .= sprintf("Phone='%s',Email='%s',EmailNotification=%d, ",
		  $phone, $email, $emailnotification ? 1 : 0);
  $sql .= sprintf("Role='%s',Department='%s',Title='%s', ",
		  $role, $department, $title);
  if (NULL === $username) {
    $sql .= "Username=NULL,";
  } else {
    $sql .= sprintf("Username='%s',", $username);
  }
  if (NULL === $password) {
    $sql .= "Password=NULL";
  } else {
    $sql .= sprintf("Password='%s'", $password);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_update_clientadmin_latlon($id, $lat, $lon, $manlatlon = FALSE) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = 'UPDATE clientadmin_table SET';
  $sql .= sprintf(" ManLatLon=%d,Lat='%s',Lon='%s'", $manlatlon ? 1 : 0, $lat, $lon);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_clientadmins_status($company_id, $client_id, $activate, $suspended) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE clientadmin_table SET ";
  $sql .= sprintf("Active=%d,Suspended=%d",
		  $activate ? 1 : 0, $suspended ? 1 : 0);
  if ($activate) {
    $sql .= ",Deactivated=0,DeactivationTime=NULL,DeactivationUID=NULL,DeactivationReason=NULL";
  }
  $sql .= sprintf(" WHERE CompanyId='%s' AND ClientId='%s'",
		  $company_id, $client_id);
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
function chdbprop_retire_clientadmins($company_id, $client_id, $uid,
				      $reason = '', $retention_days = 365) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE clientadmin_table SET Active=0,Deactivated=1,DeactivationTime=CURRENT_TIMESTAMP";
  $sql .= sprintf(",DeactivationUID=%d,DeactivationReason='%s'",
		  $uid, $reason);
  if ($retention_days > 0) {
    $sql .= sprintf(",RetentionTime=DATETIME(CURRENT_TIMESTAMP,'%d DAYS')",
		    $retention_days);
  }
  $sql .= sprintf(" WHERE CompanyId='%s' AND ClientId='%s'",
		  $company_id, $client_id);
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
function chdbprop_delete_clientadmin_id($id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM regionadmin_table";
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   user_table   **********************/



function chdbprop_get_users($uid) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM user_table WHERE UID=%d", $uid);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_user_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM user_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_company_users($company_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM user_table WHERE CompanyId='%s'",
		 $company_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_company_user($company_id, $uid) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM user_table WHERE CompanyId='%s' AND UID=%d",
		 $company_id, $uid);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_enforcer_user($company_id, $enforcer_id, $uid) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM user_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND EnforcerId='%s' AND UID=%d",
		  $company_id, $enforcer_id, $uid);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_user($company_id, $region_id, $client_id,
			      $property_id, $lot_id, $enforcer_id, $uid) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO user_table (CompanyId,RegionId,ClientId,PropertyId,LotId,UID) VALUES ";
  $sql .= sprintf("('%s','%s','%s','%s','%s',%d)",
		  $company_id, $region_id, $client_id, $property_id, $lot_id, $uid);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  if (NULL === $enforcer_id) {
    // SQL
    $sql = "UPDATE user_table SET EnforcerId=NULL";
  } else {
    $sql = sprintf("UPDATE user_table SET EnforcerId='%s'", $enforcer_id);
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
function chdbprop_update_user($id, $company_id, $region_id, $client_id,
			      $property_id, $lot_id, $enforcer_id) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE user_table SET ";
  $sql .= sprintf("CompanyId='%s',RegionId='%s',ClientId='%s', PropertyId='%s',LotId='%s',",
		  $company_id, $region_id, $client_id, $property_id, $lot_id);
  if (NULL === $enforcer_id) {
    // SQL
    $sql = "EnforcerId=NULL";
  } else {
    $sql = sprintf("EnforcerId='%s'", $enforcer_id);
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
function chdbprop_delete_user($company_id, $uid) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM user_table WHERE CompanyId='%s' AND UID=%d",
		 $company_id, $uid);
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
function chdbprop_delete_user_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM user_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   relations_table   **********************/



function chdbprop_get_all_relations() {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM relations_table";
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_relation($relations_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM relations_table WHERE RelationsId='%s'",
		 $relations_id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_relation_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM relations_table WHERE Id=%d", $id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_property_relations($property_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM relations_table WHERE PropertyId='%s'",
		 $property_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_company_relations($company_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM relations_table WHERE CompanyId='%s'",
		 $company_id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_relation($relations_id, $property_id, $company_id, &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO relations_table (RelationsId,PropertyId,CompanyId)";
  $sql .= sprintf(" VALUES ('%s','%s','%s')",
		  $relations_id, $property_id, $company_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_relation($id, $property_id, $company_id) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE relations_table SET ";
  $sql .= sprintf("PropertyId='%s',CompanyId='%s'",
		  $property_id, $company_id);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_relation_id($id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM relations_table";
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   property_table   **********************/



function chdbprop_get_all_properties() {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM property_table";
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_property($property_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM property_table";
  $sql .= sprintf(" WHERE PropertyId='%s'", $property_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_property_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM property_table WHERE Id=%d", $id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 1 if exists, otherwise 0
function chdbprop_property_id_exists($property_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
    return 0;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 0;
  }
  // Perform DB command
  $sql = "SELECT Id FROM property_table WHERE ";
  $sql .= sprintf("PropertyId='%s'", $property_id);
  $entries = array();
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
    break;
  }
  if (sizeof($entries) > 0) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_property($uid, $property_id, $alias,
				  $street, $city, $postalcode, $province, $country,
				  &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO property_table (CreatorUID,PropertyId,Alias,";
  $sql .= "Street,City,PostalCode,Province,Country) VALUES ";
  $sql .= sprintf("(%d,'%s','%s','%s','%s','%s','%s','%s')",
		  $uid, $property_id, $alias,
		  $street, $city, $postalcode, $province, $country);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1
function chdbprop_update_property($id, $alias,
				  $street, $city, $postalcode, $province, $country) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("UPDATE property_table SET Alias='%s'", $alias);
  $sql .= sprintf(",Street='%s',City='%s',PostalCode='%s',Province='%s',Country='%s'",
		  $street, $city, $postalcode, $province, $country);
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_update_property_latlon($id, $lat, $lon, $manlatlon = FALSE) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = 'UPDATE property_table SET';
  $sql .= sprintf(" ManLatLon=%d,Lat='%s',Lon='%s'", $manlatlon ? 1 : 0, $lat, $lon);
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_delete_property_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM property_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   propertyadmin_table  **********************/



function chdbprop_get_propertyadmins($property_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM propertyadmin_table";
  $sql .= sprintf(" WHERE PropertyId='%s'", $property_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_propertyadmin($property_id, $admin_id, $lat, $lon,
				       $prename_id, $firstname, $middlename, $lastname, $sufname_id,
				       $street, $city, $postalcode, $province, $country,
				       $phone, $email, $emailnotification,
				       $role, $department, $title,
				       &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO propertyadmin_table (PropertyId,AdminId,Lat,Lon,";
  $sql .= "PrenameId,FirstName,MiddleName,LastName,SufnameId,";
  $sql .= "Street,City,PostalCode,Province,Country, ";
  $sql .= "Phone,Email,EmailNotification, ";
  $sql .= "Role,Department,Title) VALUES (";
  $sql .= sprintf("'%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s',%d, ".
		  "'%s','%s','%s')",
		  $property_id, $admin_id, $lat, $lon,
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id,
		  $street, $city, $postalcode, $province, $country,
		  $phone, $email, $emailnotification ? 1 : 0,
		  $role, $department, $title);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_propertyadmin($id,
				       $prename_id, $firstname, $middlename, $lastname, $sufname_id,
				       $street, $city, $postalcode, $province, $country,
				       $phone, $email, $emailnotification,
				       $role, $department, $title) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE propertyadmin_table SET ";
  $sql .= sprintf("PrenameId='%s',FirstName='%s',MiddleName='%s',LastName='%s',SufnameId='%s', ",
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id);
  $sql .= sprintf("Street='%s',City='%s',PostalCode='%s',Province='%s',Country='%s', ",
		  $street, $city, $postalcode, $province, $country);
  $sql .= sprintf("Phone='%s',Email='%s',EmailNotification=%d, ",
		  $phone, $email, $emailnotification ? 1 : 0);
  $sql .= sprintf("Role='%s',Department='%s',Title='%s'",
		  $role, $department, $title);
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_update_propertyadmin_latlon($id, $lat, $lon, $manlatlon = FALSE) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = 'UPDATE propertyadmin_table SET';
  $sql .= sprintf(" ManLatLon=%d,Lat='%s',Lon='%s'", $manlatlon ? 1 : 0, $lat, $lon);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_propertyadmin_id($id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM propertyadmin_table";
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   propertynote_table   **********************/



function chdbprop_get_propertynotes($property_id, $active = TRUE, $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM propertynote_table WHERE ";
  $sql .= sprintf("Active=%d AND PropertyId='%s'",
		  $active ? 1 : 0, $property_id);
  if ($order_by !== 'None') {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_propertynote($property_id, $note_id,
				   $active = TRUE, $fragment = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM propertynote_table WHERE ";
  $sql .= sprintf("PropertyId='%s' AND NoteId='%s' AND Active=%d AND Fragment=%d",
		  $property_id, $note_id, $active ? 1 : 0, $fragment ? 1 : 0);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    if (!$fragment) {
      $entries = $entry;
    } else {
      $entries[] = $entry;
    }
  }
  return $entries;
}



function chdbprop_get_propertynote_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM propertynote_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_propertynote($uid, $property_id, $note_id, $note, &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO propertynote_table (CreatorUID,PropertyId,NoteId,Note)";
  $sql .= sprintf(" VALUES (%d,'%s','%s','%s')",
		  $uid, $property_id, $note_id, $note);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_propertynote($id, $uid, $note) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Retrieve current
  $master = array();
  $sql = sprintf("SELECT * FROM propertynote_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $master = $entry;
  }
  if (empty($master)) {
    drupal_set_message("Not able to read from DB", 'error');
    return 1;
  }
  // Update master non-fragment
  $sql = "UPDATE propertynote_table SET ModifyTime=CURRENT_TIMESTAMP,";
  $sql .= sprintf("ModifiedByUID=%d WHERE Id=%d", $uid, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write-1 to DB", 'error');
    return 1;
  }
  // Insert new fragment
  $sql = "INSERT INTO propertynote_table ";
  $sql .= "(CreatorUID,PropertyId,NoteId,Note,Fragment) VALUES ";
  $sql .= sprintf("(%d,'%s','%s','%s',1)",
		  $uid, $master['PropertyId'], $master['NoteId'], $note);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write-2 to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_propertynote_solved($id, $uid, $resolution) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Retrieve current
  $master = array();
  $sql = sprintf("SELECT * FROM propertynote_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $master = $entry;
  }
  if (empty($master)) {
    drupal_set_message("Not able to read from DB", 'error');
    return 1;
  }
  // Query
  $sql = "UPDATE propertynote_table SET SolvedTime=CURRENT_TIMESTAMP,Active=0,Solved=1,";
  $sql .= sprintf("SolvedByUID=%d,Resolution='%s'", $uid, $resolution);
  $sql .= sprintf(" WHERE Active=1 AND PropertyId='%s' AND NoteId='%s'",
		  $master['PropertyId'], $master['NoteId']);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_propertynote_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Retrieve current
  $master = array();
  $sql = sprintf("SELECT * FROM propertynote_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $master = $entry;
  }
  if (empty($master)) {
    drupal_set_message("Not able to read from DB", 'error');
    return 1;
  }
  // Perform DB command
  $sql = sprintf("DELETE FROM propertynote_table WHERE PropertyId='%s' AND NoteId='%s'",
		 $master['PropertyId'], $master['NoteId']);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   propertynoteid_table   **********************/



function chdbprop_get_propertynoteid($property_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM propertynoteid_table WHERE ";
  $sql .= sprintf("PropertyId='%s'", $property_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_propertynoteid($property_id, &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO propertynoteid_table (PropertyId) VALUES";
  $sql .= sprintf(" ('%s')", $property_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_propertynoteid_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE propertynoteid_table SET Counter=Counter+1";
  $sql .= sprintf(" WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_propertynoteid($property_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM propertynoteid_table";
  $sql .= sprintf(" WHERE PropertyId='%s'", $property_id);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   policy_table   **********************/



function chdbprop_get_all_policies() {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM policy_table";
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_policies($property_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM policy_table WHERE";
  $sql .= sprintf(" PropertyId='%s'", $property_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_policy($property_id, $lot_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM policy_table WHERE";
  $sql .= sprintf(" PropertyId='%s'", $property_id);
  $sql .= sprintf(" AND LotId='%s'", $lot_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_policy_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM policy_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_insert_policy($property_id, $lot_id, $profile_id, $alias) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO policy_table (PropertyId,LotId,ProfileId,Alias)";
  $sql .= sprintf(" VALUES ('%s','%s','%s','%s')",
		  $property_id, $lot_id, $profile_id, $alias);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_policy($property_id, $lot_id,
				$filter_alias = FALSE, $alias = NULL,
				$filter_profile = FALSE, $profile_id = NULL,
				$filter_enforcement = FALSE, $enforcement_id = NULL,
				$filter_equipment = FALSE, $equipment_id = NULL) {
  if (!$filter_alias and !$filter_profile and !$filter_enforcement and !$filter_equipment) {
    drupal_set_message('Wrong usage', 'error');
    return 0;
  }

  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE policy_table SET ";
  $prefix = '';
  if ($filter_alias) {
    $sql .= sprintf("%sAlias='%s'", $prefix, $alias);
    $prefix = ',';
  }
  if ($filter_profile) {
    $sql .= sprintf("%sProfileId='%s'", $prefix, $profile_id);
    $prefix = ',';
  }
  if ($filter_enforcement) {
    $sql .= sprintf("%sEnforcementId='%s'", $prefix, $enforcement_id);
    $prefix = ',';
  }
  if ($filter_equipment) {
    $sql .= sprintf("%sEquipmentId='%s'", $prefix, $equipment_id);
    $prefix = ',';
  }
  $sql .= sprintf(" WHERE PropertyId='%s' AND LotId='%s'", $property_id, $lot_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_policy($property_id, $lot_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM policy_table WHERE PropertyId='%s' AND LotId='%s'",
		 $property_id, $lot_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_policy_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM policy_table WHERE Id=%d",
		 $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   profile_table   **********************/



function chdbprop_get_all_profiles($active = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM profile_table WHERE Type=1 AND Active=%d",
		 $active ? 1 : 0);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_profile($profile_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM profile_table WHERE ProfileId='%s'",
		 $profile_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_profile_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM profile_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function
chdbprop_insert_default_profile($uid, $alias, $profile_id,
				$long_policy, $short_policy, $longshort_policy,
				$long_alias, $short_alias,
				$long_type, $short_type,
				$long_class, $short_class,
				$valet_policy,
				// Short Policy
				$mon, $tue, $wed, $thu, $fri, $sat, $sun, $start, $stop,
				$overnight_start, $overnight_stop,
				$expiry_policy, $expiry_duration, $expiry_days,
				$renew_retention, $renew_retention_days,
				$short_stall_policy, $short_stall_levels, $short_stall_count,
				$extension_policy, $extension_shared,
				$regular_comment, $regular_cost, $overnight_cost,
				// Long Policy
				$validity_policy, $validity_days,
				$removal_policy, $removal_retention, $removal_days,
				$long_stall_policy, $long_stall_levels, $long_stall_count,
				$permit_policy, $permit_shared,
				// Valet
				$valet_messenger, $valunits_policy,
				$valet_units, $valet_mods,
				&$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO profile_table (CreatorUID,Alias,Type,ProfileId,";
  $sql .= "LongPolicy,ShortPolicy,LongShortPolicy, ";
  $sql .= "LongPolicyAlias,ShortPolicyAlias,";
  $sql .= "LongTypeId,ShortTypeId,";
  $sql .= "LongClassId,ShortClassId,";
  $sql .= "ValetPolicy, ";
  // Short Policy
  $sql .= "Mon,Tue,Wed,Thu,Fri,Sat,Sun,Start,Stop,OvernightStart,OvernightStop,";
  $sql .= "ExpirePolicy,ExpireDuration,ExpireDays, ";
  $sql .= "RenewRetention,RenewRetentionDays, ";
  $sql .= "ShortStallPolicy,ShortStallLevels,ShortStallCount,";
  $sql .= "ExtensionPolicy,ExtensionShared,";
  $sql .= "RegularComment,RegularCost,OvernightCost, ";
  // Long Policy
  $sql .= "ValidityPolicy,ValidityDays,";
  $sql .= "RemovalPolicy,RemovalRetention,RemovalDays, ";
  $sql .= "LongStallPolicy,LongStallLevels,LongStallCount,";
  $sql .= "PermitPolicy,PermitShared, ";
  // Valet
  $sql .= "ValetMessenger,ValUnitsPolicy,ValetUnits,ValetMods)";
  $sql .= sprintf(" VALUES (".
		  "%d,'%s',0,'%s',". //CreatorUID,Alias,Type,ProfileId,
		  "%d,%d,%d,". //LongPolicy,ShortPolicy,LongShortPolicy,
		  "'%s','%s',". //LongPolicyAlias,ShortPolicyAlias,
		  "'%s','%s',". //LongTypeId,ShortTypeId,
		  "'%s','%s',". //LongClassId,ShortClassId,
		  "%d, ". //ValetPolicy,
		  // Short Policy
		  "%d,%d,%d,%d,%d,%d,%d,'%s','%s',". //Mon,Tue,Wed,Thu,Fri,Sat,Sun,Start,Stop,
		  "'%s','%s',". //OvernightStart,OvernightStop,
		  "%d,%d,%d,". //ExpirePolicy,ExpireDuration,ExpireDays,
		  "%d,%d,". //RenewRetention,RenewRetentionDays,
		  "%d,'%s','%s',". //ShortStallPolicy,ShortStallLevels,ShortStallCount,
		  "%d,%d,". //ExtensionPolicy,ExtensionShared,
		  "'%s',%0.02f,%0.02f, ". //RegularComment,RegularCost,OvernightCost,
		  // Long Policy
		  "%d,%d,". //ValidityPolicy,ValidityDays,
		  "%d,%d,%d,". //RemovalPolicy,RemovalRetention,RemovalDays,
		  "%d,'%s','%s',". //LongStallPolicy,LongStallLevels,LongStallCount,
		  "%d,%d, ". //PermitPolicy,PermitShared,
		  // Valet
		  "%d,%d,%d,'%s'". //ValetMessenger,ValUnitsPolicy,ValetUnits,ValetMods
		  ")",
		  $uid, $alias, $profile_id,
		  $long_policy, $short_policy, $longshort_policy,
		  $long_alias, $short_alias,
		  $long_type, $short_type,
		  $long_class, $short_class,
		  $valet_policy,
		  // Short Policy
		  $mon, $tue, $wed, $thu, $fri, $sat, $sun, $start, $stop,
		  $overnight_start, $overnight_stop,
		  $expiry_policy, $expiry_duration, $expiry_days,
		  $renew_retention, $renew_retention_days,
		  $short_stall_policy, $short_stall_levels, $short_stall_count,
		  $extension_policy, $extension_shared,
		  $regular_comment, $regular_cost, $overnight_cost,
		  // Long Policy
		  $validity_policy, $validity_days,
		  $removal_policy, $removal_retention, $removal_days,
		  $long_stall_policy, $long_stall_levels, $long_stall_count,
		  $permit_policy, $permit_shared,
		  // Valet
		  $valet_messenger, $valunits_policy,
		  $valet_units, $valet_mods);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function
chdbprop_insert_new_profile($uid, $alias, $profile_id,
			    $long_policy, $short_policy, $longshort_policy,
			    $long_alias, $short_alias,
			    $long_type, $short_type,
			    $long_class, $short_class,
			    $valet_policy,
			    // Short Policy
			    $mon, $tue, $wed, $thu, $fri, $sat, $sun, $start, $stop,
			    $overnight_start, $overnight_stop,
			    $expiry_policy, $expiry_duration, $expiry_days,
			    $renew_retention, $renew_retention_days,
			    $short_stall_policy, $short_stall_levels, $short_stall_count,
			    $extension_policy, $extension_shared,
			    $regular_comment, $regular_cost, $overnight_cost,
			    // Long Policy
			    $validity_policy, $validity_days,
			    $removal_policy, $removal_retention, $removal_days,
			    $long_stall_policy, $long_stall_levels, $long_stall_count,
			    $permit_policy, $permit_shared,
			    // Valet
			    $valet_messenger, $valunits_policy,
			    $valet_units, $valet_mods,
			    &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO profile_table (CreatorUID,Alias,Type,ProfileId,";
  $sql .= "LongPolicy,ShortPolicy,LongShortPolicy, ";
  $sql .= "LongPolicyAlias,ShortPolicyAlias,";
  $sql .= "LongTypeId,ShortTypeId,";
  $sql .= "LongClassId,ShortClassId,";
  $sql .= "ValetPolicy, ";
  // Short Policy
  $sql .= "Mon,Tue,Wed,Thu,Fri,Sat,Sun,Start,Stop,OvernightStart,OvernightStop,";
  $sql .= "ExpirePolicy,ExpireDuration,ExpireDays, ";
  $sql .= "RenewRetention,RenewRetentionDays, ";
  $sql .= "ShortStallPolicy,ShortStallLevels,ShortStallCount,";
  $sql .= "ExtensionPolicy,ExtensionShared,";
  $sql .= "RegularComment,RegularCost,OvernightCost, ";
  // Long Policy
  $sql .= "ValidityPolicy,ValidityDays,";
  $sql .= "RemovalPolicy,RemovalRetention,RemovalDays, ";
  $sql .= "LongStallPolicy,LongStallLevels,LongStallCount,";
  $sql .= "PermitPolicy,PermitShared, ";
  // Valet
  $sql .= "ValetMessenger,ValUnitsPolicy,ValetUnits,ValetMods)";
  $sql .= sprintf(" VALUES (".
		  "%d,'%s',1,'%s',". //CreatorUID,Alias,Type,ProfileId,
		  "%d,%d,%d,". //LongPolicy,ShortPolicy,LongShortPolicy,
		  "'%s','%s',". //LongPolicyAlias,ShortPolicyAlias,
		  "'%s','%s',". //LongTypeId,ShortTypeId,
		  "'%s','%s',". //LongClassId,ShortClassId,
		  "%d, ". //ValetPolicy,
		  // Short Policy
		  "%d,%d,%d,%d,%d,%d,%d,'%s','%s',". //Mon,Tue,Wed,Thu,Fri,Sat,Sun,Start,Stop,
		  "'%s','%s',". //OvernightStart,OvernightStop,
		  "%d,%d,%d,". //ExpirePolicy,ExpireDuration,ExpireDays,
		  "%d,%d,". //RenewRetention,RenewRetentionDays,
		  "%d,'%s','%s',". //ShortStallPolicy,ShortStallLevels,ShortStallCount,
		  "%d,%d,". //ExtensionPolicy,ExtensionShared,
		  "'%s',%0.02f,%0.02f, ". //RegularComment,RegularCost,OvernightCost,
		  // Long Policy
		  "%d,%d,". //ValidityPolicy,ValidityDays,
		  "%d,%d,%d,". //RemovalPolicy,RemovalRetention,RemovalDays,
		  "%d,'%s','%s',". //LongStallPolicy,LongStallLevels,LongStallCount,
		  "%d,%d, ". //PermitPolicy,PermitShared,
		  // Valet
		  "%d,%d,%d,'%s'". //ValetMessenger,ValUnitsPolicy,ValetUnits,ValetMods
		  ")",
		  $uid, $alias, $profile_id,
		  $long_policy, $short_policy, $longshort_policy,
		  $long_alias, $short_alias,
		  $long_type, $short_type,
		  $long_class, $short_class,
		  $valet_policy,
		  // Short Policy
		  $mon, $tue, $wed, $thu, $fri, $sat, $sun, $start, $stop,
		  $overnight_start, $overnight_stop,
		  $expiry_policy, $expiry_duration, $expiry_days,
		  $renew_retention, $renew_retention_days,
		  $short_stall_policy, $short_stall_levels, $short_stall_count,
		  $extension_policy, $extension_shared,
		  $regular_comment, $regular_cost, $overnight_cost,
		  // Long Policy
		  $validity_policy, $validity_days,
		  $removal_policy, $removal_retention, $removal_days,
		  $long_stall_policy, $long_stall_levels, $long_stall_count,
		  $permit_policy, $permit_shared,
		  // Valet
		  $valet_messenger, $valunits_policy,
		  $valet_units, $valet_mods);
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
function
chdbprop_update_profile($id, $alias,
			$long_policy, $short_policy, $longshort_policy,
			$long_alias, $short_alias,
			$long_type, $short_type,
			$long_class, $short_class,
			$valet_policy,
			// Short Policy
			$mon, $tue, $wed, $thu, $fri, $sat, $sun, $start, $stop,
			$overnight_start, $overnight_stop,
			$expiry_policy, $expiry_duration, $expiry_days,
			$renew_retention, $renew_retention_days,
			$short_stall_policy, $short_stall_levels, $short_stall_count,
			$extension_policy, $extension_shared,
			$regular_comment, $regular_cost, $overnight_cost,
			// Long Policy
			$validity_policy, $validity_days,
			$removal_policy, $removal_retention, $removal_days,
			$long_stall_policy, $long_stall_levels, $long_stall_count,
			$permit_policy, $permit_shared,
			// Valet
			$valet_messenger, $valunits_policy,
			$valet_units, $valet_mods) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = 'UPDATE profile_table SET';
  $sql .= sprintf(" Alias='%s',LongPolicy=%d,ShortPolicy=%d,LongShortPolicy=%d",
		  $alias, $long_policy, $short_policy, $longshort_policy);
  $sql .= sprintf(", LongPolicyAlias='%s',ShortPolicyAlias='%s'",
		  $long_alias, $short_alias);
  $sql .= sprintf(",LongTypeId='%s',ShortTypeId='%s'",
		  $long_type, $short_type);
  $sql .= sprintf(",LongClassId='%s',ShortClassId='%s'",
		  $long_class, $short_class);
  $sql .= sprintf(",ValetPolicy=%d",
		  $valet_policy);
  // Short Policy
  $sql .= sprintf(", Mon=%d,Tue=%d,Wed=%d,Thu=%d,Fri=%d,Sat=%d,Sun=%d",
		  $mon, $tue, $wed, $thu, $fri, $sat, $sun);
  $sql .= sprintf(",Start='%s',Stop='%s',OvernightStart='%s',OvernightStop='%s'",
		  $start, $stop, $overnight_start, $overnight_stop);
  $sql .= sprintf(", ExpirePolicy=%d,ExpireDuration=%d,ExpireDays=%d",
		  $expiry_policy, $expiry_duration, $expiry_days);
  $sql .= sprintf(",RenewRetention=%d,RenewRetentionDays=%d",
		  $renew_retention, $renew_retention_days);
  $sql .= sprintf(", ShortStallPolicy=%d,ShortStallLevels='%s',ShortStallCount='%s'",
		  $short_stall_policy, $short_stall_levels, $short_stall_count);
  $sql .= sprintf(", ExtensionPolicy=%d,ExtensionShared=%d",
		  $extension_policy, $extension_shared);
  $sql .= sprintf(", RegularComment='%s',RegularCost=%0.02f,OvernightCost=%0.02f",
		  $regular_comment, $regular_cost, $overnight_cost);
  // Long Policy
  $sql .= sprintf(", ValidityPolicy=%d,ValidityDays=%d",
		  $validity_policy, $validity_days);
  $sql .= sprintf(",RemovalPolicy=%d,RemovalRetention=%d,RemovalDays=%d",
		  $removal_policy, $removal_retention, $removal_days);
  $sql .= sprintf(", LongStallPolicy=%d,LongStallLevels='%s',LongStallCount='%s'",
		  $long_stall_policy, $long_stall_levels, $long_stall_count);
  $sql .= sprintf(", PermitPolicy=%d,PermitShared=%d",
		  $permit_policy, $permit_shared);
  // Valet
  $sql .= sprintf(", ValetMessenger=%d,ValUnitsPolicy=%d,ValetUnits=%d,ValetMods='%s'",
		  $valet_messenger, $valunits_policy, $valet_units, $valet_mods);
  $sql .= sprintf("  WHERE Id=%d", $id);
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
function chdbprop_retire_profile_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE profile_table SET Active=0,DeactivationTime=CURRENT_TIMESTAMP";
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_profile_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM profile_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   stallevel_table   **********************/



function chdbprop_get_stallevels($property_id, $lot_id, $long, $short, $valet) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM stallevel_table WHERE PropertyId='%s' AND LotId='%s'",
		 $property_id, $lot_id);
  $sql .= sprintf(" AND Long=%d AND Short=%d and Valet=%d",
		  $long ? 1 : 0, $short ? 1 : 0, $valet ? 1 : 0);
  $sql .= " ORDER BY Level";
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_stallevel($property_id, $lot_id, $level_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM stallevel_table WHERE ";
  $sql .= sprintf("PropertyId='%s' AND LotId='%s' AND LevelId='%s'",
		  $property_id, $lot_id, $level_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_stallevel_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM stallevel_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_stallevel($property_id, $lot_id, $long, $short, $valet, $level_id, $alias, $level) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO stallevel_table (PropertyId,LotId,Long,Short,Valet,LevelId,Alias,Level)";
  $sql .= sprintf(" VALUES ('%s','%s',%d,%d,%d,'%s','%s','%s')",
		  $property_id, $lot_id,
		  $long ? 1 : 0, $short ? 1 : 0, $valet ? 1 : 0,
		  $level_id, $alias, $level);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_stallevel_id($id, $alias) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("UPDATE stallevel_table SET Alias='%s' WHERE Id=%d", $alias, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_stallevel_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM stallevel_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   stall_table   **********************/



function chdbprop_get_stalls($property_id, $lot_id, $level_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM stall_table";
  $sql .= sprintf(" WHERE PropertyId='%s' AND LotId='%s' AND LevelId='%s'",
		 $property_id, $lot_id, $level_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_stall($property_id, $lot_id, $level_id, $stall_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM stall_table WHERE ";
  $sql .= sprintf("PropertyId='%s' AND LotId='%s' AND LevelId='%s' AND StallId='%s'",
		  $property_id, $lot_id, $level_id, $stall_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_stall_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM stall_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_stall($property_id, $lot_id, $level_id, $stall_id, $alias) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO stall_table (PropertyId,LotId,LevelId,StallId,Alias)";
  $sql .= sprintf(" VALUES ('%s','%s','%s','%s','%s')",
		  $property_id, $lot_id, $level_id, $stall_id, $alias);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_stall_id($id, $active, $alias) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("UPDATE stall_table SET Active=%d,Alias='%s' WHERE Id=%d",
		 $active ? 1 : 0, $alias, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_level_stalls($property_id, $lot_id, $level_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM stall_table";
  $sql .= sprintf(" WHERE PropertyId='%s' AND LotId='%s' AND LevelId='%s'",
		  $property_id, $lot_id, $level_id);
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
function chdbprop_delete_stall_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM stall_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   stallnote_table   **********************/



function chdbprop_get_stallnotes($property_id, $lot_id, $level_id, $stall_id,
				 $active = TRUE, $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM stallnote_table WHERE ";
  $sql .= sprintf("PropertyId='%s' AND LotId='%s' AND LevelId='%s' AND StallId='%s'",
		  $property_id, $lot_id, $level_id, $stall_id);
  $sql .= sprintf(" AND Active=%d", $active ? 1 : 0);
  if ($order_by !== 'None') {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_stallnote($property_id, $lot_id, $level_id, $stall_id, $note_id,
				$active = TRUE, $fragment = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM stallnote_table WHERE ";
  $sql .= sprintf("PropertyId='%s' AND LotId='%s' AND LevelId='%s' AND StallId='%s' AND NoteId='%s'",
		  $property_id, $lot_id, $level_id, $stall_id, $note_id);
  $sql .= sprintf(" AND Active=%d AND Fragment=%d",
		  $active ? 1 : 0,
		  $fragment ? 1 : 0);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    if (!$fragment) {
      $entries = $entry;
    } else {
      $entries[] = $entry;
    }
  }
  return $entries;
}



function chdbprop_get_stallnote_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM stallnote_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_stallnote($uid, $property_id, $lot_id, $level_id, $stall_id, $note_id, $note, &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO stallnote_table (CreatorUID,PropertyId,LotId,LevelId,StallId,NoteId,Note)";
  $sql .= sprintf(" VALUES (%d,'%s','%s','%s','%s','%s','%s')",
		  $uid, $property_id, $lot_id, $level_id, $stall_id, $note_id, $note);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_stallnote($id, $uid, $note) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Retrieve current
  $master = array();
  $sql = sprintf("SELECT * FROM stallnote_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $master = $entry;
  }
  if (empty($master)) {
    drupal_set_message("Not able to read from DB", 'error');
    return 1;
  }
  // Update master non-fragment
  $sql = "UPDATE stallnote_table SET ModifyTime=CURRENT_TIMESTAMP,";
  $sql .= sprintf("ModifiedByUID=%d WHERE Id=%d", $uid, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write-1 to DB", 'error');
    return 1;
  }
  // Insert new fragment
  $sql = "INSERT INTO stallnote_table ";
  $sql .= "(CreatorUID,PropertyId,LotId,LevelId,StallId,NoteId,Note,Fragment) VALUES ";
  $sql .= sprintf("(%d,'%s','%s','%s','%s','%s','%s',1)",
		  $uid,
		  $master['PropertyId'],
		  $master['LotId'],
		  $master['LevelId'],
		  $master['StallId'],
		  $master['NoteId'],
		  $note);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write-2 to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_stallnote_solved($id, $uid, $resolution) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Retrieve current
  $master = array();
  $sql = sprintf("SELECT * FROM stallnote_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $master = $entry;
  }
  if (empty($master)) {
    drupal_set_message("Not able to read from DB", 'error');
    return 1;
  }
  // Query
  $sql = "UPDATE stallnote_table SET SolvedTime=CURRENT_TIMESTAMP,Active=0,Solved=1,";
  $sql .= sprintf("SolvedByUID=%d,Resolution='%s' WHERE Active=1 AND ", $uid, $resolution);
  $sql .= sprintf("PropertyId='%s' AND LotId='%s' AND LevelId='%s' AND StallId='%s' AND NoteId='%s'",
		  $master['PropertyId'],
		  $master['LotId'],
		  $master['LevelId'],
		  $master['StallId'],
		  $master['NoteId']);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_stallnote_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  // Retrieve current
  $master = array();
  $sql = sprintf("SELECT * FROM stallnote_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $master = $entry;
  }
  if (empty($master)) {
    drupal_set_message("Not able to read from DB", 'error');
    return 1;
  }
  // Perform DB command
  $sql = "DELETE FROM stallnote_table WHERE ";
  $sql .= sprintf("PropertyId='%s' AND LotId='%s' AND LevelId='%s' AND StallId='%s' AND NoteId='%s'",
		 $master['PropertyId'],
		 $master['LotId'],
		 $master['LevelId'],
		 $master['StallId'],
		 $master['NoteId']);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   stallnoteid_table   **********************/



function chdbprop_get_stallnoteid($property_id, $lot_id, $level_id, $stall_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM stallnoteid_table WHERE ";
  $sql .= sprintf("PropertyId='%s' AND LotId='%s' AND LevelId='%s' AND StallId='%s'",
		  $property_id, $lot_id, $level_id, $stall_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_stallnoteid($property_id, $lot_id, $level_id, $stall_id, &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO stallnoteid_table (PropertyId,LotId,LevelId,StallId) VALUES";
  $sql .= sprintf(" ('%s','%s','%s','%s')",
		  $property_id, $lot_id, $level_id, $stall_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_stallnoteid_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE stallnoteid_table SET Counter=Counter+1";
  $sql .= sprintf(" WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   enforcement_table   **********************/




function chdbprop_get_all_enforcement($active = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM enforcement_table WHERE Type=1 AND Active=%d",
		 $active ? 1 : 0);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_enforcement($enforcement_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM enforcement_table WHERE EnforcementId='%s'",
		 $enforcement_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_enforcement_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM enforcement_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}




// Return 0 on success, otherwise 1 
function chdbprop_insert_enforcement($uid, $enforcement_id, $profile = FALSE,
				     $ticketing, $booting, $towing,
				     $mobile_policy, $num_mobiles,
				     &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO enforcement_table (CreatorUID,EnforcementId,Type,";
  $sql .= "Ticketing,Booting,Towing,MobilePolicy,MobileUnits) VALUES (";
  $sql .= sprintf("%d,'%s',%d,", $uid, $enforcement_id, $profile ? 1 : 0);
  $sql .= sprintf("%d,%d,%d,", $ticketing ? 1 : 0, $booting ? 1 : 0, $towing ? 1 : 0);
  $sql .= sprintf("%d,%d)", $mobile_policy, $num_mobiles);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_enforcement($id,
				     $ticketing, $booting, $towing,
				     $mobile_policy, $num_mobiles) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE enforcement_table SET ";
  $sql .= sprintf("Ticketing=%d,Booting=%d,Towing=%d,MobilePolicy=%d,MobileUnits=%d",
		  $ticketing ? 1 : 0, $booting ? 1 : 0, $towing ? 1 : 0,
		  $mobile_policy, $num_mobiles);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_enforcement_id($id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM enforcement_table WHERE Id=%d", $id);;
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



/****************   equipment_table   **********************/



function chdbprop_get_all_equipment() {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM equipment_table";
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_equipment($equipment_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM equipment_table WHERE EquipmentId='%s'",
		 $equipment_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_equipment_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM equipment_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}




// Return 0 on success, otherwise 1 
function chdbprop_insert_equipment($uid, $equipment_id, $profile = FALSE,
				   $lpr, $info_board, $pad,
				   &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO equipment_table (CreatorUID,EquipmentId,Type,";
  $sql .= "LPR,InfoBoard,PAD) VALUES (";
  $sql .= sprintf("%d,'%s',%d,", $uid, $equipment_id, $profile ? 1 : 0);
  $sql .= sprintf("%d,%d,%d)", $lpr ? 1 : 0, $info_board ? 1 : 0, $pad ? 1 : 0);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_equipment($id, $lpr, $info_board, $pad) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE equipment_table SET ";
  $sql .= sprintf("LPR=%d,InfoBoard=%d,PAD=%d",
		  $lpr ? 1 : 0, $info_board ? 1 : 0, $pad ? 1 : 0);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_equipment_id($id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM equipment_table WHERE Id=%d", $id);;
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   clusterstatus_table   **********************/



function chdbprop_get_clusterstatus($app_id, $cluster_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM clusterstatus_table";
  $sql .= sprintf(" WHERE AppId='%s'", $app_id);
  $sql .= sprintf(" AND ClusterId='%s'", $cluster_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



/****************   lane_table   **********************/



function chdbprop_get_all_lanes($property_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM lane_table WHERE PropertyId='%s'",
		 $property_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_lane_clusters($property_id, $app_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT DISTINCT ClusterId FROM lane_table";
  $sql .= sprintf(" WHERE PropertyId='%s'", $property_id);
  $sql .= sprintf(" AND AppId='%s'", $app_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[$entry['ClusterId']] = TRUE;
  }
  return $entries;
}



function chdbprop_get_lanes($property_id, $cluster_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM lane_table WHERE PropertyId='%s' AND ClusterId='%s'",
		 $property_id, $cluster_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_lane($property_id, $lane_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM lane_table WHERE ";
  $sql .= sprintf("PropertyId='%s' AND LaneId='%s'",
		 $property_id, $lane_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_lane_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM lane_table WHERE Id=%d", $id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_lane($uid, $property_id, $cluster_id, $lane_id, $alias,
			      $entry, $exit, $entry_gate, $exit_gate, $app_id,
			      $entry_whitelist, $entry_blacklist, $entry_hotlist,
			      $entry_reslist, $entry_corrector,
			      $exit_whitelist, $exit_blacklist, $exit_hotlist,
			      $exit_reslist, $exit_pay,
			      &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO lane_table (CreatorUID,PropertyId,ClusterId,LaneId,Alias, ";
  $sql .= "Entry,Exit,EntryGate,ExitGate,AppId, ";
  $sql .= "EntryWhitelist,EntryBlacklist,EntryHotlist,EntryReslist,EntryCorrector, ";
  $sql .= "ExitWhitelist,ExitBlacklist,ExitHotlist,ExitReslist,ExitPay) VALUES (";
  $sql .= sprintf("%d,'%s','%s','%s','%s', ",
		  $uid, $property_id, $cluster_id, $lane_id, $alias);
  $sql .= sprintf("%d,%d,'%s','%s','%s', ",
		  $entry ? 1 : 0, $exit ? 1 : 0, $entry_gate, $exit_gate, $app_id);
  $sql .= sprintf("%d,%d,%d,%d,%d, ",
		  $entry_whitelist ? 1 : 0, $entry_blacklist ? 1 : 0,
		  $entry_hotlist ? 1 : 0, $entry_reslist ? 1 : 0,
		  $entry_corrector ? 1 : 0);
  $sql .= sprintf("%d,%d,%d,%d,%d)",
		  $exit_whitelist ? 1 : 0, $exit_blacklist ? 1 : 0,
		  $exit_hotlist ? 1 : 0, $exit_reslist,
		  $exit_pay  ? 1 : 0);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_lane($id, $alias,
			      $entry, $exit, $entry_gate, $exit_gate,
			      $entry_whitelist, $entry_blacklist, $entry_hotlist,
			      $entry_reslist, $entry_corrector,
			      $exit_whitelist, $exit_blacklist, $exit_hotlist,
			      $exit_reslist, $exit_pay) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("UPDATE lane_table SET ModifyTime=CURRENT_TIMESTAMP,Alias='%s', ", $alias);
  $sql .= sprintf("Entry=%d,Exit=%d,EntryGate='%s',ExitGate='%s', ",
		  $entry ? 1 : 0, $exit ? 1 : 0, $entry_gate, $exit_gate);
  $sql .= sprintf("EntryWhitelist=%d,EntryBlacklist=%d,EntryHotlist=%d,EntryReslist=%d,EntryCorrector=%d, ",
		  $entry_whitelist ? 1 : 0, $entry_blacklist ? 1 : 0,
		  $entry_hotlist ? 1 : 0, $entry_reslist ? 1 : 0,
		  $entry_corrector ? 1 : 0);
  $sql .= sprintf("ExitWhitelist=%d,ExitBlacklist=%d,ExitHotlist=%d,ExitReslist=%d,ExitPay=%d",
		  $exit_whitelist ? 1 : 0, $exit_blacklist ? 1 : 0,
		  $exit_hotlist ? 1 : 0, $exit_reslist ? 1 : 0,
		  $exit_pay ? 1 : 0);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_lane_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM lane_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   laneprop_table   **********************/



function chdbprop_get_laneprops($lane_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM laneprop_table WHERE ";
  $sql .= sprintf("LaneId='%s'", $lane_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_lanes_admin($company_id, $property_id, $lot_id,
				     $lot_type = 'ANY', $lot_class = 'ANY') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM lane_table AS lane";
  $sql .= " INNER JOIN laneprop_table AS laneprop";
  $sql .= "  ON lane.ClusterId=laneprop.ClusterId";
  $sql .= "   AND lane.LaneId=laneprop.LaneId";
  if ($lot_type !== 'ANY' or $lot_class !== 'ANY') {
    $sql .= " INNER JOIN policy_table AS policy";
    $sql .= "  ON (laneprop.PropertyId=policy.PropertyId)";
    $sql .= "   AND (laneprop.LotId=policy.LotId)";
    $sql .= " INNER JOIN profile_table AS profile";
    $sql .= "  ON (policy.ProfileId=profile.ProfileId)";
  }
  $precond = TRUE;
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" %s laneprop.PropertyId='%s'", $precond ? 'WHERE' : 'AND', $property_id);
    $precond = FALSE;
  }
  if ($lot_id !== 'ANY') {
    $sql .= sprintf(" %s laneprop.LotId='%s'", $precond ? 'WHERE' : 'AND', $lot_id);
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
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_cc_get_lanes($company_id, $property_id, $lot_id,
			       $lot_type = 'ANY', $lot_class = 'ANY') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM lane_table AS lane";
  $sql .= " INNER JOIN laneprop_table AS laneprop";
  $sql .= "  ON lane.ClusterId=laneprop.ClusterId";
  $sql .= "   AND lane.LaneId=laneprop.LaneId";
  if ($property_id === 'ANY' or $lot_id === 'ANY') {
    $sql .= " INNER JOIN user_table AS user";
    $prefix = 'ON';
    if ($property_id === 'ANY') {
      $sql .= sprintf(" %s (laneprop.PropertyId=user.PropertyId OR user.PropertyId='ANY')",
		      $prefix);
      $prefix = 'AND';
    }
    if ($lot_id === 'ANY') {
      $sql .= sprintf(" %s (laneprop.LotId=user.LotId OR user.LotId='ANY')",
		      $prefix);
      $prefix = 'AND';
    }
  }
  if ($lot_type !== 'ANY' or $lot_class !== 'ANY') {
    $sql .= " INNER JOIN policy_table AS policy";
    $sql .= "  ON (laneprop.PropertyId=policy.PropertyId)";
    $sql .= "   AND (laneprop.LotId=policy.LotId)";
    $sql .= " INNER JOIN profile_table AS profile";
    $sql .= "  ON (policy.ProfileId=profile.ProfileId)";
  }
  $precond = TRUE;
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" %s laneprop.PropertyId='%s'", $precond ? 'WHERE' : 'AND', $property_id);
    $precond = FALSE;
  }
  if ($lot_id !== 'ANY') {
    $sql .= sprintf(" %s laneprop.LotId='%s'", $precond ? 'WHERE' : 'AND', $lot_id);
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
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_laneprop($cluster_id, $lane_id, $property_id, $lot_id, &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO laneprop_table (ClusterId,LaneId,PropertyId,LotId) VALUES (";
  $sql .= sprintf("'%s','%s','%s','%s')",
		  $cluster_id, $lane_id, $property_id, $lot_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1
function chdbprop_delete_laneprops($lane_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM laneprop_table WHERE ";
  $sql .= sprintf("LaneId='%s'", $lane_id);
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
function chdbprop_delete_laneprop($lane_id, $property_id, $lot_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM laneprop_table WHERE ";
  $sql .= sprintf("LaneId='%s'", $lane_id);
  $sql .= sprintf(" AND PropertyId='%s' AND LotId='%s'", $property_id, $lot_id);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   lanecount_table   **********************/



function chdbprop_get_lanecount($property_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM lanecount_table";
  $sql .= sprintf(" WHERE PropertyId='%s'", $property_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



/****************   lanecountopt_table   **********************/



function chdbprop_get_all_lanecountopt() {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM lanecountopt_table";
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_lanecountopt($uid, $property_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM lanecountopt_table";
  $sql .= sprintf(" WHERE UID=%d AND PropertyId='%s'", $uid, $property_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1
function chdbprop_insert_lanecountopt($uid, $property_id, $scheme) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO lanecountopt_table (UID,PropertyId,Scheme) VALUES ";
  $sql .= sprintf("(%d,'%s','%s')", $uid, $property_id, $scheme);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1
function chdbprop_update_lanecountopt($uid, $property_id, $scheme) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE lanecountopt_table SET ";
  $sql .= sprintf("Scheme='%s' WHERE UID=%d AND PropertyId='%s'",
		  $scheme, $uid, $property_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   lanecountref_table   **********************/



function chdbprop_get_lanecountref($uid, $property_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM lanecountref_table";
  $sql .= sprintf(" WHERE UID=%d AND PropertyId='%s'", $uid, $property_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1
function chdbprop_insert_lanecountref($uid, $property_id, $lane_id, $value) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO lanecountref_table (UID,PropertyId,LaneId,Ref) VALUES ";
  $sql .= sprintf("(%d,'%s','%s',%d)", $uid, $property_id, $lane_id, $value);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1
function chdbprop_delete_lanecountref($uid, $property_id, $lane_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM lanecountref_table WHERE ";
  $sql .= sprintf("UID=%d AND PropertyId='%s'", $uid, $property_id);
  if ($lane_id !== 'ANY') {
    $sql .= sprintf(" AND LaneId='%s'", $lane_id);
  }
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   lanecountlast_table   **********************/



function chdbprop_get_lanecountlast($property_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM lanecountlast_table";
  $sql .= sprintf(" WHERE PropertyId='%s'", $property_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



/****************   laneprediction_table Table   **********************/



function chdbprop_get_lanepredictions($property_id,
				      $filter_lot = FALSE, $lot_id = '') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM laneprediction_table";
  $sql .= sprintf(" WHERE PropertyId='%s'", $property_id);
  if ($filter_lot) {
    $sql .= sprintf(" AND LotId='%s'", $lot_id);
  }
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_laneprediction($property_id, $lot_id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO laneprediction_table (PropertyId,LotId) ";
  $sql .= sprintf("VALUES ('%s','%s')",
		  $property_id, $lot_id);
  //drupal_set_message($sql);
  try {
    $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_laneprediction_dailycount($property_id, $lot_id,
						   $today, $tomorrow) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE laneprediction_table SET ModifyTime=CURRENT_TIMESTAMP,";
  $sql .= sprintf("CountToday=%u,CountTomorrow=%u", $today, $tomorrow);
  $sql .= sprintf(" WHERE PropertyId='%s'", $property_id);
  $sql .= sprintf(" AND LotId='%s'", $lot_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_laneprediction_weeklycount($property_id, $lot_id,
						    $next_week) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE laneprediction_table SET ModifyTime=CURRENT_TIMESTAMP,";
  $sql .= sprintf("CountNextWeek=%d", $next_week);
  $sql .= sprintf(" WHERE PropertyId='%s'", $property_id);
  $sql .= sprintf(" AND LotId='%s'", $lot_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_laneprediction_monthlycount($property_id, $lot_id,
						     $next_month) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE laneprediction_table SET ModifyTime=CURRENT_TIMESTAMP,";
  $sql .= sprintf("CountNextMonth=%d", $next_month);
  $sql .= sprintf(" WHERE PropertyId='%s'", $property_id);
  $sql .= sprintf(" AND LotId='%s'", $lot_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   camera_table   **********************/



function chdbprop_get_cameras_cluster($cluster_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM camera_table WHERE ";
  $sql .= sprintf("ClusterId='%s'", $cluster_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_camera_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM camera_table WHERE ";
  $sql .= sprintf("Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_camera($uid, $cluster_id, $lane_id, $camera_id,
				$ip_addr, $port, $stream_port,
				$entry, $exit, $overview, $front, $rear,
				&$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO camera_table (CreatorUID,ClusterId,LaneId,CameraId,";
  $sql .= "IPAddr,Port,StreamPort,Entry,Exit,Front,Rear) VALUES (";
  $sql .= sprintf("%d,'%s','%s','%s', ", $uid, $cluster_id, $lane_id, $camera_id);
  $sql .= sprintf("'%s',%d,%d,", $ip_addr, $port, $stream_port);
  $sql .= sprintf("%d,%d,", $entry ? 1 : 0, $exit ? 1 : 0);
  $sql .= sprintf("%d,%d)", $front ? 1 : 0, $rear ? 1 : 0);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  if ($overview) {
    $sql = sprintf("UPDATE camera_table SET PlateCamera=0,OverviewCamera=1 WHERE Id=%d", $id);
    //drupal_set_message($sql);
    if (!$db->exec($sql)) {
      drupal_set_message("Not able to write to DB", 'error');
      return 1;
    }
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_camera_id($id,
				   $suspended,
				   $ip_addr, $port, $stream_port,
				   $deployment_code,
				   $entry, $exit, $overview, $front, $rear) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE camera_table SET ";
  $sql .= sprintf("Suspended=%d,", $suspended ? 1 : 0);
  $sql .= sprintf("IPAddr='%s',Port=%d,StreamPort=%d,", $ip_addr, $port, $stream_port);
  $sql .= sprintf("DeploymentCode=%d,", $deployment_code);
  $sql .= sprintf("Entry=%d,Exit=%d,", $entry ? 1 : 0, $exit ? 1 : 0);
  $sql .= sprintf("Front=%d,Rear=%d", $front ? 1 : 0, $rear ? 1 : 0);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1
function chdbprop_delete_camera_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM camera_table WHERE ";
  $sql .= sprintf("Id=%d", $id);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/* /\****************   device_table   **********************\/ */



/* function chdbprop_get_devices($property_id, $equipment_id) { */
/*   $entries = array(); */
/*   // Find location of SQLite database */
/*   $db_name = getenv('APPS_LPMS_PROP_DB'); */
/*   if ($db_name == FALSE) { */
/*     drupal_set_message("Missing HTTP environment for PROP database!!!", 'error'); */
/*     return $entries; */
/*   } */
/*   // connect to SQLite database */
/*   try { */
/*     $db = new PDO("sqlite:".$db_name); */
/*   } catch(PDOException $e) { */
/*     drupal_set_message($e->getMessage(), 'error'); */
/*     return $entries; */
/*   } */
/*   // SQL */
/*   $sql = sprintf("SELECT * FROM device_table WHERE PropertyId='%s' AND EquipmentId='%s'", */
/* 		 $property_id, $equipment_id); */
/*   //drupal_set_message($sql); */
/*   foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) { */
/*     $entries[] = $entry; */
/*   } */
/*   return $entries; */
/* } */



/* function chdbprop_get_device_id($id) { */
/*   $entries = array(); */
/*   // Find location of SQLite database */
/*   $db_name = getenv('APPS_LPMS_PROP_DB'); */
/*   if ($db_name == FALSE) { */
/*     drupal_set_message("Missing HTTP environment for PROP database!!!", 'error'); */
/*     return $entries; */
/*   } */
/*   // connect to SQLite database */
/*   try { */
/*     $db = new PDO("sqlite:".$db_name); */
/*   } catch(PDOException $e) { */
/*     drupal_set_message($e->getMessage(), 'error'); */
/*     return $entries; */
/*   } */
/*   // Perform DB command */
/*   $sql = sprintf("SELECT * FROM device_table WHERE Id=%d", $id); */
/*   foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) { */
/*     $entries = $entry; */
/*   } */
/*   return $entries; */
/* } */



/* // Return 0 on success, otherwise 1  */
/* function chdbprop_insert_device($uid, $property_id, $equipment_id, $device_id, $alias, */
/* 				//TBD */
/* 				&$id) { */
/*   $db_name = getenv('APPS_LPMS_PROP_DB'); */
/*   if ($db_name == FALSE) { */
/*     drupal_set_message("Missing HTTP environment for PROP database!!!", 'error'); */
/*     return 1; */
/*   } */
/*   // connect to SQLite database */
/*   try { */
/*     $db = new PDO("sqlite:".$db_name); */
/*   } catch(PDOException $e) { */
/*     drupal_set_message($e->getMessage(), 'error'); */
/*     return 1; */
/*   } */
/*   // Query */
/*   $sql = "INSERT INTO device_table (CreatorUID,PropertyId,EquipmentId,DeviceId,Alias, "; */
/*   $sql .= "ExitWhitelist,ExitTicket) VALUES ("; */
/*   $sql .= sprintf("%d,'%s','%s','%s','%s', ", $uid, $property_id, $equipment_id, $device_id, $alias); */
/*   $sql .= sprintf("%d,%d)", $exit_whitelist ? 1 : 0, $exit_ticket ? 1 : 0); */
/*   //drupal_set_message($sql); */
/*   if (!$db->exec($sql)) { */
/*     drupal_set_message("Not able to write to DB", 'error'); */
/*     return 1; */
/*   } */
/*   $id = $db->lastInsertId(); */
/*   return 0; */
/* } */



/* // Return 0 on success, otherwise 1  */
/* function chdbprop_update_device($id, $alias, */
/* 				$exit_whitelist, $exit_ticket) { */
/*   $db_name = getenv('APPS_LPMS_PROP_DB'); */
/*   if ($db_name == FALSE) { */
/*     drupal_set_message("Missing HTTP environment for PROP database!!!", 'error'); */
/*     return 1; */
/*   } */
/*   // connect to SQLite database */
/*   try { */
/*     $db = new PDO("sqlite:".$db_name); */
/*   } catch(PDOException $e) { */
/*     drupal_set_message($e->getMessage(), 'error'); */
/*     return 1; */
/*   } */
/*   // SQL */
/*   $sql = sprintf("UPDATE device_table SET ModifyTime=CURRENT_TIMESTAMP,Alias='%s', ", $alias); */

/*   $sql .= sprintf("ExitWhitelist=%d,ExitTicket=%d", $exit_whitelist ? 1 : 0, $exit_ticket ? 1 : 0); */
/*   $sql .= sprintf(" WHERE Id=%d", $id); */
/*   //drupal_set_message($sql); */
/*   if (!$db->exec($sql)) { */
/*     drupal_set_message("Not able to write to DB", 'error'); */
/*     return 1; */
/*   } */
/*   return 0; */
/* } */



/****************   agent_table   **********************/



function chdbprop_get_agent($company_id, $agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM agent_table WHERE CompanyId='%s' AND AgentId='%s'",
		 $company_id, $agent_id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_agent_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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



// Return 0 on success, otherwise 1 
function chdbprop_insert_agent($uid, $company_id, $agent_id, $app_id, $enforcer_id,
			       $lat, $lon,
			       $prename_id, $firstname, $middlename, $lastname, $sufname_id,
			       $street, $city, $postalcode, $province, $country,
			       $phone, $email,
			       $role, $department, $title,
			       $expiry,
			       &$id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO agent_table (CreatorUID,CompanyId,AgentId,AppId,Lat,Lon, ";
  $sql .= "PrenameId,FirstName,MiddleName,LastName,SufnameId, ";
  $sql .= "Street,City,PostalCode,Province,Country, ";
  $sql .= "Phone,Email, ";
  $sql .= "Role,Department,Title) VALUES (";
  $sql .= sprintf("%d,'%s','%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s', ".
		  "'%s','%s','%s')",
		  $uid, $company_id, $agent_id, $app_id, $lat, $lon,
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id,
		  $street, $city, $postalcode, $province, $country,
		  $phone, $email,
		  $role, $department, $title);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  // SQL
  $sql = "UPDATE agent_table SET ";
  if (NULL === $expiry) {
    $sql .= "ExpiryTime=NULL";
  } else {
    $sql .= sprintf("ExpiryTime='%s'", $expiry);
  }
  if (NULL === $enforcer_id) {
    $sql .= ",EnforcerId=NULL";
  } else {
    $sql .= sprintf(",EnforcerId='%s'", $enforcer_id);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_update_agent($id, $uid, $lat, $lon,
			       $prename_id, $firstname, $middlename, $lastname, $sufname_id,
			       $street, $city, $postalcode, $province, $country,
			       $phone, $email,
			       $role, $department, $title,
			       $expiry) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE agent_table SET ";
  $sql .= sprintf("Lat='%s',Lon='%s', ", $lat, $lon);
  $sql .= sprintf("PrenameId='%s',FirstName='%s',MiddleName='%s',LastName='%s',SufnameId='%s', ",
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id);
  $sql .= sprintf("Street='%s',City='%s',PostalCode='%s',Province='%s',Country='%s', ",
		  $street, $city, $postalcode, $province, $country);
  $sql .= sprintf("Phone='%s',Email='%s', ", $phone, $email);
  $sql .= sprintf("Role='%s',Department='%s',Title='%s', ", $role, $department, $title);
  if (NULL === $expiry) {
    $sql .= "ExpiryTime=NULL";
  } else {
    $sql .= sprintf("ExpiryTime='%s'", $expiry);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_update_agent_status($id, $suspended) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("UPDATE agent_table SET Suspended=%d WHERE Id=%d",
		 $suspended, $id);
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
function chdbprop_delete_agent_from_lane($lane_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM agent_table WHERE LaneId='%s'", $lane_id);
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
function chdbprop_retire_agent_id($id, $uid, $reason, $retention_days = 60) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE agent_table SET Active=0,Deactivated=1,DeactivationTime=CURRENT_TIMESTAMP,";
  $sql .= sprintf("DeactivationUID=%d,DeactivationReason='%s'",
		  $uid, $reason);
  if ($retention_days > 0) {
    $sql .= sprintf(",RetentionTime=DATETIME(CURRENT_TIMESTAMP,'%d DAYS')",
		    $retention_days);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



// Used by CRON
// Return 0 on success, otherwise 1
function chdbprop_delete_mobiles_retention_older_than($time) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM agent_table WHERE Active=0 AND Deactivated=1 AND ";
  $sql .= sprintf("RetentionTime < '%s'", $time);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   agentcrypt_table   **********************/



function chdbprop_get_agentcrypts($order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM agentcrypt_table";
  if ($order_by !== 'None') {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_agentcrypt_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM agentcrypt_table WHERE Id=%d", $id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
// 534533EE78287A93B514A0CBF23EA1E2
function chdbprop_insert_agentcrypt($kek, $dek) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO agentcrypt_table (KEK,DEK)";
  $sql .= sprintf(" VALUES ('%s','%s')", $kek, $dek);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_agentcrypt($id, $dek) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE agentcryp_table SET ";
  $sql .= sprintf("DEK='%s' WHERE Id=%d",
		  $alias,
		  $enforcement_policy ? 1 : 0,
		  $valet_policy ? 1 : 0,
		  $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_agentcrypt_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM agentcrypt_table WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   agentprop_table   **********************/



function chdbprop_get_agentprops($company_id, $agent_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM agentprop_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND AgentId='%s'",
		  $company_id, $agent_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_agentprop($company_id, $agent_id,
				   $property_id, $lot_id,
				   &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO agentprop_table (CompanyId,AgentId,PropertyId,LotId) VALUES (";
  $sql .= sprintf("'%s','%s','%s','%s')",
		  $company_id, $agent_id, $property_id, $lot_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1
function chdbprop_delete_agentprops($company_id, $agent_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM agentprop_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND AgentId='%s'", $company_id, $agent_id);
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
function chdbprop_delete_agentprop($company_id, $agent_id, $property_id, $lot_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM agentprop_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND AgentId='%s'", $company_id, $agent_id);
  $sql .= sprintf(" AND PropertyId='%s' AND LotId='%s'", $property_id, $lot_id);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   agentstatus_table   **********************/



/****************   patroller_table   **********************/



function chdbprop_get_patroller($company_id, $patroller_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM patroller_table WHERE ";
  $sql = sprintf("CompanyId='%s' AND PatrollerId='%s'",
		 $company_id, $patroller_id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_patroller_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM patroller_table WHERE Id=%d", $id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1
function chdbprop_insert_patroller($uid, $company_id, $patroller_id, $password, $lat, $lon,
				   $prename_id, $firstname, $middlename, $lastname, $sufname_id,
				   $street, $city, $postalcode, $province, $country,
				   $phone, $email,
				   $role, $department, $title,
				   $session, $idle,
				   $tickets, $boots, $towing, $whitelist, $hotlist, $valet,
				   $enforcer_id,
				   &$id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO patroller_table (CreatorUID,CompanyId,PatrollerId,Password,Lat,Lon, ";
  $sql .= "PrenameId,FirstName,MiddleName,LastName,SufnameId, ";
  $sql .= "Street,City,PostalCode,Province,Country, ";
  $sql .= "Phone,Email, ";
  $sql .= "Role,Department,Title, ";
  $sql .= "SessionMinutes,IdleMinutes, ";
  $sql .= "Tickets,Boots,Towing,Whitelist,Hotlist,Valet) VALUES (";
  $sql .= sprintf("%d,'%s','%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s', ".
		  "'%s','%s','%s', ".
		  "%d,%d, ".
		  "%d,%d,%d,%d,%d,%d)",
		  $uid, $company_id, $patroller_id, $password, $lat, $lon,
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id,
		  $street, $city, $postalcode, $province, $country,
		  $phone, $email,
		  $role, $department, $title,
		  $session, $idle,
		  $tickets ? 1 : 0, $boots ? 1 : 0, $towing ? 1 : 0,
		  $whitelist ? 1 : 0, $hotlist ? 1 : 0, $valet ? 1 : 0);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  // SQL
  $sql = "UPDATE patroller_table SET ";
  if (NULL === $enforcer_id) {
    $sql .= "EnforcerId=NULL";
  } else {
    $sql .= sprintf("EnforcerId='%s'", $enforcer_id);
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
function chdbprop_update_patroller($id, $password, $lat, $lon,
				   $prename_id, $firstname, $middlename, $lastname, $sufname_id,
				   $street, $city, $postalcode, $province, $country,
				   $phone, $email,
				   $role, $department, $title,
				   $session, $idle,
				   $tickets, $boots, $towing, $whitelist, $hotlist, $valet,
				   $enforcer_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE patroller_table SET ";
  $sql .= sprintf("Password=%d,Lat='%s',Lon='%s', ", $password, $lat, $lon);
  $sql .= sprintf("PrenameId='%s',FirstName='%s',MiddleName='%s',LastName='%s',SufnameId='%s', ",
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id);
  $sql .= sprintf("Street='%s',City='%s',PostalCode='%s',Province='%s',Country='%s', ",
		  $street, $city, $postalcode, $province, $country);
  $sql .= sprintf("Phone='%s',Email='%s', ", $phone, $email);
  $sql .= sprintf("Role='%s',Department='%s',Title='%s', ", $role, $department, $title);
  $sql .= sprintf("SessionMinutes=%d,IdleMinutes=%d, ", $session, $idle);
  $sql .= sprintf("Tickets=%d,Boots=%d,Towing=%d,Whitelist=%d,Hotlist=%d,Valet=%d, ",
		  $tickets ? 1 : 0, $boots ? 1 : 0, $towing ? 1 : 0,
		  $whitelist ? 1 : 0, $hotlist ? 1 : 0, $valet ? 1 : 0);
  if (NULL === $enforcer_id) {
    $sql .= "EnforcerId=NULL,";
  } else {
    $sql .= sprintf("EnforcerId='%s',", $enforcer_id);
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
function chdbprop_update_patroller_status($id, $suspended) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("UPDATE patroller_table SET Suspended=%d WHERE Id=%d",
		 $suspended, $id);
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
function chdbprop_delete_patroller_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM patroller_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   patrollerprop_table   **********************/



function chdbprop_get_patrollerprops($company_id, $patroller_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM patrollerprop_table WHERE CompanyId='%s' AND PatrollerId='%s'",
		 $company_id, $patroller_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_patrollerprop($company_id, $patroller_id,
				       $property_id, $lot_id,
				       &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO patrollerprop_table (CompanyId,PatrollerId,PropertyId,LotId) VALUES (";
  $sql .= sprintf("'%s','%s','%s','%s')",
		  $company_id, $patroller_id, $property_id, $lot_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1
function chdbprop_delete_patrollerprops($company_id, $patroller_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM patrollerprop_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PatrollerId='%s'", $company_id, $patroller_id);
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
function chdbprop_delete_patrollerprop($company_id, $patroller_id, $property_id, $lot_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM patrollerprop_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PatrollerId='%s'", $company_id, $patroller_id);
  $sql .= sprintf(" AND PropertyId='%s' AND LotId='%s'", $property_id, $lot_id);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   enforcer_table   **********************/



function chdbprop_get_enforcers($company_id,
				$filter_active = TRUE, $active = TRUE,
				$filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM enforcer_table WHERE CompanyId='%s'", $company_id);
  if ($filter_active) {
    $sql .= sprintf(" AND Active=%d", $active ? 1 : 0);
  }
  if ($filter_suspended) {
    $sql .= sprintf(" AND Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_enforcer($company_id, $enforcer_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM enforcer_table WHERE CompanyId='%s' AND EnforcerId='%s'",
		 $company_id, $enforcer_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_enforcer_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM enforcer_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_enforcer($uid, $company_id, $enforcer_id,
				  $alias, $session, $idle,
				  $street, $city, $postal_code, $province, $country,
				  &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO enforcer_table (CreatorUID,CompanyId,EnforcerId,Alias, ";
  $sql .= "SessionMinutes,IdleMinutes, ";
  $sql .= "Street,City,PostalCode,Province,Country) VALUES ";
  $sql .= sprintf("(%d,'%s','%s','%s',%d,%d,'%s','%s','%s','%s','%s')",
		  $uid, $company_id, $enforcer_id, $alias, $session, $idle,
		  $street, $city, $postal_code, $province, $country);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1
function chdbprop_update_enforcer($id, $alias, $session, $idle,
				  $street, $city, $postal_code, $province, $country) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = 'UPDATE enforcer_table SET ';
  $sql .= sprintf("Alias='%s',Street='%s',City='%s',SessionMinutes=%d,IdleMinutes=%d, ",
		  $alias, $street, $city, $session, $idle);
  $sql .= sprintf("PostalCode='%s',Province='%s',Country='%s' ",
		  $postal_code, $province, $country);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1
function chdbprop_update_enforcer_latlon($id, $lat, $lon, $manlatlon = FALSE) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = 'UPDATE enforcer_table SET';
  $sql .= sprintf(" ManLatLon=%d,Lat='%s',Lon='%s'", $manlatlon ? 1 : 0, $lat, $lon);
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_update_enforcer_status($id, $activate, $suspended) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE enforcer_table SET ";
  $sql .= sprintf("Active=%d,Suspended=%d",
		  $activate ? 1 : 0, $suspended ? 1 : 0);
  if ($activate) {
    $sql .= ",Deactivated=0,DeactivationTime=NULL,DeactivationUID=NULL,DeactivationReason=NULL";
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_retire_enforcer_id($id, $uid, $reason = '',
				    $retention_days = 365) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE enforcer_table SET Active=0,Deactivated=1,DeactivationTime=CURRENT_TIMESTAMP";
  $sql .= sprintf(",DeactivationUID=%d,DeactivationReason='%s'",
		  $uid, $reason);
  if ($retention_days > 0) {
    $sql .= sprintf(",RetentionTime=DATETIME(CURRENT_TIMESTAMP,'%d DAYS')",
		    $retention_days);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_delete_enforcer_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM enforcer_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   enforcerprop_table   **********************/



function chdbprop_get_enforcerprops($company_id, $enforcer_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM enforcerprop_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND EnforcerId='%s'",
		  $company_id, $enforcer_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_enforcerprop($company_id, $enforcer_id,
				      $property_id, $lot_id,
				      &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO enforcerprop_table (CompanyId,EnforcerId,PropertyId,LotId) VALUES (";
  $sql .= sprintf("'%s','%s','%s','%s')",
		  $company_id, $enforcer_id, $property_id, $lot_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1
function chdbprop_delete_enforcerprops($company_id, $enforcer_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM enforcerprop_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND EnforcerId='%s'", $company_id, $enforcer_id);
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
function chdbprop_delete_enforcerprop($company_id, $enforcer_id, $property_id, $lot_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM enforcerprop_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND EnforcerId='%s'", $company_id, $enforcer_id);
  $sql .= sprintf(" AND PropertyId='%s' AND LotId='%s'", $property_id, $lot_id);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}




/****************   enforceradmin_table   **********************/



function chdbprop_get_enforceradmins($company_id, $enforcer_id,
				     $filter_active = TRUE, $active = TRUE,
				     $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM enforceradmin_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND EnforcerId='%s'",
		  $company_id, $enforcer_id);
  if ($filter_active) {
    $sql .= sprintf(" AND Active=%d", $active ? 1 : 0);
  }
  if ($filter_suspended) {
    $sql .= sprintf(" AND Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_enforceradmin($company_id, $enforcer_id, $admin_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM enforceradmin_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND EnforcerId='%s' AND AdminId='%s'",
		  $company_id, $enforcer_id, $admin_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_enforceradmin($company_id, $enforcer_id, $admin_id, $lat, $lon,
				       $prename_id, $firstname, $middlename, $lastname, $sufname_id,
				       $street, $city, $postalcode, $province, $country,
				       $phone, $email, $emailnotification,
				       $role, $department, $title, $username, $password,
				       &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO enforceradmin_table (CompanyId,EnforcerId,AdminId,Lat,Lon,";
  $sql .= "PrenameId,FirstName,MiddleName,LastName,SufnameId,";
  $sql .= "Street,City,PostalCode,Province,Country, ";
  $sql .= "Phone,Email,EmailNotification, ";
  $sql .= "Role,Department,Title) VALUES (";
  $sql .= sprintf("'%s','%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s',%d, ".
		  "'%s','%s','%s')",
		  $company_id, $enforcer_id, $admin_id, $lat, $lon,
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id,
		  $street, $city, $postalcode, $province, $country,
		  $phone, $email, $emailnotification ? 1 : 0,
		  $role, $department, $title);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  // SQL
  $sql = "UPDATE enforceradmin_table SET ";
  if (NULL === $username) {
    $sql .= "Username=NULL";
  } else {
    $sql .= sprintf("Username='%s'", $username);
  }
  if (NULL === $password) {
    $sql .= ",Password=NULL";
  } else {
    $sql .= sprintf(",Password='%s'", $password);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_update_enforceradmin($id,
				       $prename_id, $firstname, $middlename, $lastname, $sufname_id,
				       $street, $city, $postalcode, $province, $country,
				       $phone, $email, $emailnotification,
				       $role, $department, $title, $username, $password) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE enforceradmin_table SET ";
  $sql .= sprintf("PrenameId='%s',FirstName='%s',MiddleName='%s',LastName='%s',SufnameId='%s', ",
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id);
  $sql .= sprintf("Street='%s',City='%s',PostalCode='%s',Province='%s',Country='%s', ",
		  $street, $city, $postalcode, $province, $country);
  $sql .= sprintf("Phone='%s',Email='%s',EmailNotification=%d, ",
		  $phone, $email, $emailnotification ? 1 : 0);
  $sql .= sprintf("Role='%s',Department='%s',Title='%s', ",
		  $role, $department, $title);
  if (NULL === $username) {
    $sql .= "Username=NULL,";
  } else {
    $sql .= sprintf("Username='%s',", $username);
  }
  if (NULL === $password) {
    $sql .= "Password=NULL";
  } else {
    $sql .= sprintf("Password='%s'", $password);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
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
function chdbprop_update_enforceradmin_latlon($id, $lat, $lon, $manlatlon = FALSE) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = 'UPDATE enforceradmin_table SET';
  $sql .= sprintf(" ManLatLon=%d,Lat='%s',Lon='%s'", $manlatlon ? 1 : 0, $lat, $lon);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_enforceradmins_status($company_id, $enforcer_id, $activate, $suspended) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE enforceradmin_table SET ";
  $sql .= sprintf("Active=%d,Suspended=%d",
		  $activate ? 1 : 0, $suspended ? 1 : 0);
  if ($activate) {
    $sql .= ",Deactivated=0,DeactivationTime=NULL,DeactivationUID=NULL,DeactivationReason=NULL";
  }
  $sql .= sprintf(" WHERE CompanyId='%s' AND EnforcerId='%s'",
		  $company_id, $enforcer_id);
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
function chdbprop_retire_enforceradmins($company_id, $enforcer_id, $uid,
					$reason = '', $retention_days = 365) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE enforceradmin_table SET Active=0,Deactivated=1,DeactivationTime=CURRENT_TIMESTAMP";
  $sql .= sprintf(",DeactivationUID=%d,DeactivationReason='%s'",
		  $uid, $reason);
  if ($retention_days > 0) {
    $sql .= sprintf(",RetentionTime=DATETIME(CURRENT_TIMESTAMP,'%d DAYS')",
		    $retention_days);
  }
  $sql .= sprintf(" WHERE CompanyId='%s' AND EnforcerId='%s'",
		  $company_id, $enforcer_id);
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
function chdbprop_delete_enforceradmin($company_id, $enforcer_id, $admin_id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM enforceradmin_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND EnforcerId='%s' AND AdminId='%s'",
		  $company_id, $enforcer_id, $admin_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   freq_users_table   **********************/



function chdbprop_get_freq_user($lpn) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_FREQ_USERS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for FREQ_USERS database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM freq_users_table WHERE LPN='%s'", $lpn);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_freq_user($property_id, $lpn, $make, $model, $color) {
  $db_name = getenv('APPS_LPMS_FREQ_USERS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for FREQ_USERS database!!!", 'error');
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
  $sql = "INSERT INTO freq_users_table ";
  $sql .= "(PropertyId,LPN,Make,Model,Color) VALUES ";
  $sql .= sprintf("('%s','%s','%s','%s','%s')",
		  $property_id, $lpn, $make, $model, $color);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_freq_user($property_id, $lpn, $make, $model, $color) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_FREQ_USERS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for FREQ_USERS database!!!", 'error');
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
  $sql = "UPDATE freq_users_table SET IdleCount=0";
  $sql .= sprintf(",PropertyId='%s',Make='%s',Model='%s',Color='%s'",
		  $property_id, $make, $model, $color);
  $sql .= sprintf(" WHERE LPN='%s'", $lpn);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_freq_user_person($lpn,
					  $first_name, $last_name, $phone,
					  $street, $city, $postal_code,
					  $province, $country) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_FREQ_USERS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for FREQ_USERS database!!!", 'error');
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
  $sql = "UPDATE freq_users_table SET IdleCount=0,";
  $sql .= sprintf("FirstName='%s',LastName='%s',Phone='%s',Street='%s',",
		  $first_name, $last_name, $phone, $street);
  $sql .= sprintf("City='%s',PostalCode='%s',Province='%s',Country='%s'",
		  $city, $postal_code, $province, $country);
  $sql .= sprintf(" WHERE LPN='%s'", $lpn);
  drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   mobileapp_table   **********************/



function chdbprop_get_all_apps($order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM mobileapp_table";
  if ($order_by !== 'None') {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbprop_get_mobileapp($app_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM mobileapp_table WHERE AppId='%s'", $app_id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_app_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM mobileapp_table WHERE Id=%d", $id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_app($app_id, $alias, $enforcement, $valet, $securepark, $videopark) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO mobileapp_table ";
  $sql .= "(AppId,Alias,EnforcementPolicy,ValetPolicy,SecureparkPolicy,VideoparkPolicy)";
  $sql .= sprintf(" VALUES ('%s','%s',%d,%d,%d,%d)",
		  $app_id, $alias,
		  $enforcement ? 1 : 0,
		  $valet ? 1 : 0,
		  $securepark ? 1 : 0,
		  $videopark ? 1 : 0);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_app($id, $alias, $enforcement_policy, $valet_policy) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE mobileapp_table SET ";
  $sql .= sprintf("Alias='%s',EnforcementPolicy=%d,ValetPolicy=%d WHERE Id=%d",
		  $alias,
		  $enforcement_policy ? 1 : 0,
		  $valet_policy ? 1 : 0,
		  $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_app_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM mobileapp_table WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   ticketprinter_table   **********************/



function chdbprop_get_ticketprinter($company_id, $enforcer_id, $template,
				    $property_id, $lot_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM ticketprinter_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND EnforcerId='%s' AND Template=%d",
		  $company_id, $enforcer_id, $template ? 1 : 0);
  if (!$template) {
    $sql .= sprintf(" AND PropertyId='%s' AND LotId='%s'",
		    $property_id, $lot_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
    break;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbprop_insert_ticketprinter($company_id, $enforcer_id, $template,
				       $property_id, $lot_id,
				       $header, $company,
				       $addr1, $addr2, $addr3, $addr4, $addr5,
				       $disclaimer,
				       &$id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO ticketprinter_table (CompanyId,EnforcerId,Template, ";
  $sql .= "Header,Company,Addr1,Addr2,Addr3,Addr4,Addr5,Disclaimer) VALUES (";
  $sql .= sprintf("'%s','%s',%d,'%s','%s','%s','%s','%s','%s','%s','%s')",
		  $company_id, $enforcer_id, $template ? 1 : 0,
		  $header, $company,
		  $addr1, $addr2, $addr3, $addr4, $addr5,
		  $disclaimer);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  // SQL
  $sql = "UPDATE ticketprinter_table SET ";
  if ($template) {
    $sql .= "PropertyId=NULL,LotId=NULL";
  } else {
    $sql .= sprintf("PropertyId='%s',LotId='%s'", $property_id, $lot_id);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    //drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_update_ticketprinter($id, $template,
				       $property_id, $lot_id,
				       $header, $company,
				       $addr1, $addr2, $addr3, $addr4, $addr5,
				       $disclaimer) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE ticketprinter_table SET ";
  $sql .= sprintf("Template=%d,", $template ? 1 : 0);
  if ($template) {
    $sql .= "PropertyId=NULL,LotId=NULL, ";
  } else {
    $sql .= sprintf("PropertyId='%s',LotId='%s', ", $property_id, $lot_id);
  }
  $sql .= sprintf("Header='%s',Company='%s', ", $header, $company);
  $sql .= sprintf("Addr1='%s',Addr2='%s',Addr3='%s',Addr4='%s',Addr5='%s',Disclaimer='%s'",
		  $addr1, $addr2, $addr3, $addr4, $addr5, $disclaimer);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_ticketprinter($company_id, $enforcer_id, $template,
				       $property_id, $lot_id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM ticketprinter_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND EnforcerId='%s' AND Template=%d",
		  $company_id, $enforcer_id, $template ? 1 : 0);
  if (!$template) {
    $sql .= sprintf(" AND PropertyId='%s' AND LotId='%s'",
		    $property_id, $lot_id);
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



/****************   ticketcode_table   **********************/



function chdbprop_get_ticketcodes($company_id, $enforcer_id, $template,
				  $property_id, $lot_id,
				  $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM ticketcode_table WHERE  ";
  $sql .= sprintf("CompanyId='%s' AND EnforcerId='%s' AND Template=%d",
		  $company_id, $enforcer_id, $template ? 1 : 0);
  if (!$template) {
    $sql .= sprintf(" AND PropertyId='%s' AND LotId='%s'", $property_id, $lot_id);
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



function chdbprop_get_ticketcode($company_id, $enforcer_id, $template,
				 $code,
				 $property_id, $lot_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "SELECT * FROM ticketcode_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND EnforcerId='%s' AND Template=%d AND Code='%s'",
		  $company_id, $enforcer_id, $template ? 1 : 0, $code);
  if (!$template) {
    $sql .= sprintf(" AND PropertyId='%s' AND LotId='%s'", $property_id, $lot_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbprop_get_ticketcode_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM ticketcode_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 1 if exists, otherwise 0
function chdbprop_ticketcode_exists($company_id, $enforcer_id, $template,
				    $code,
				    $property_id, $lot_id) {
  $entry = chdbprop_get_ticketcode($company_id, $enforcer_id, $template,
				   $code,
				   $property_id, $lot_id);
  if (sizeof($entry) > 0) {
    return 1;
  }
  return 0;
}



function chdbprop_insert_ticketcode($company_id, $enforcer_id, $template,
				    $property_id, $lot_id,
				    $code, $description, $amount,
				    &$id) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO ticketcode_table (CompanyId,EnforcerId,Template,";
  $sql .= "Code,Description,Amount) ";
  $sql .= sprintf("VALUES ('%s','%s',%d,'%s','%s','%s')",
		  $company_id, $enforcer_id, $template ? 1 : 0,
		  $code, $description, $amount);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  // SQL
  $sql = "UPDATE ticketcode_table SET ";
  if ($template) {
    $sql .= "PropertyId=NULL,LotId=NULL";
  } else {
    $sql .= sprintf("PropertyId='%s',LotId='%s'", $property_id, $lot_id);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



function chdbprop_update_ticketcode($id, $template, $property_id, $lot_id,
				    $code, $description, $amount) {
  $db_name = getenv('APPS_LPMS_PROP_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "UPDATE ticketcode_table SET ";
  $sql .= sprintf("Template=%d,PropertyId='%s',LotId='%s',",
		  $template ? 1 : 0, $property_id, $lot_id);
  $sql .= sprintf("Code='%s',Description='%s',Amount='%s'",
		  $code, $description, $amount);
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_ticketcode($company_id, $enforcer_id, $template,
				    $code,
				    $property_id, $lot_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "DELETE FROM ticketcode_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND EnforcerId='%s' AND Template=%d AND Code='%s'",
		  $company_id, $enforcer_id, $template ? 1 : 0, $code);
  if (!$template) {
    $sql .= sprintf(" AND PropertyId='%s' AND LotId='%s'", $property_id, $lot_id);
  }
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbprop_delete_ticketcode_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("DELETE FROM ticketcode_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   agentpoll_table   **********************/



// Return 0 on success, otherwise 1
function chdbprop_insert_agentpoll($agent_id, $tag) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = sprintf("INSERT INTO agentpoll_table (AgentId,Tag) VALUES ('%s','%s')",
		 $agent_id, $tag);
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
function chdbprop_insert_agentpoll_property($property_id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO agentpoll_table (AgentId,Tag)";
  $sql .= " SELECT DISTINCT agent.AgentId,'Property' FROM agent_table AS agent";
  $sql .= "  INNER JOIN agentprop_table AS agentprop";
  $sql .= "   ON agent.CompanyId=agentprop.CompanyId";
  $sql .= "    AND agent.AgentId=agentprop.AgentId";
  $sql .= sprintf(" WHERE agentprop.PropertyId='%s'", $property_id);
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
function chdbprop_insert_agentpoll_lot($property_id, $lot_id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO agentpoll_table (AgentId,Tag)";
  $sql .= " SELECT DISTINCT agent.AgentId,'Lot' FROM agent_table AS agent";
  $sql .= "  INNER JOIN agentprop_table AS agentprop";
  $sql .= "   ON agent.CompanyId=agentprop.CompanyId";
  $sql .= "    AND agent.AgentId=agentprop.AgentId";
  $sql .= sprintf(" WHERE agentprop.PropertyId='%s' AND agentprop.LotId='%s'",
		  $property_id, $lot_id);
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
// To be used when policy profile changes are made
function chdbprop_insert_agentpoll_lot_policy($profile_id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO agentpoll_table (AgentId,Tag)";
  $sql .= " SELECT DISTINCT agent.AgentId,'Lot' FROM agent_table AS agent";
  $sql .= "  INNER JOIN agentprop_table AS agentprop";
  $sql .= "   ON agent.CompanyId=agentprop.CompanyId";
  $sql .= "    AND agent.AgentId=agentprop.AgentId";
  $sql .= "  INNER JOIN policy_table AS policy";
  $sql .= "   ON policy.PropertyId=policy.PropertyId";
  $sql .= "    AND policy.LotId=policy.LotId";
  $sql .= sprintf(" WHERE policy.ProfileId='%s'", $profile_id);
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
// To be used when policy profile changes are made
function chdbprop_insert_agentpoll_lane($cluster_id, $lane_id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO agentpoll_table (AgentId,Tag)";
  $sql .= " SELECT DISTINCT agent.AgentId,'Lane' FROM agent_table AS agent";
  $sql .= "  INNER JOIN laneprop_table AS laneprop";
  $sql .= "   ON agent.ClusterId=laneprop.ClusterId";
  $sql .= sprintf(" WHERE laneprop.ClusterId='%s' AND LaneId='%s'",
		  $cluster_id, $lane_id);
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
// To be used when policy profile changes are made
function chdbprop_insert_agentpoll_camera($camera_id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO agentpoll_table (AgentId,Tag)";
  $sql .= " SELECT DISTINCT agent.AgentId,'Camera' FROM agent_table AS agent";
  $sql .= "  INNER JOIN laneprop_table AS laneprop";
  $sql .= "   ON agent.ClusterId=laneprop.ClusterId";
  $sql .= "  INNER JOIN camera_table AS camera";
  $sql .= "   ON laneprop.ClusterId=camera.ClusterId";
  $sql .= "    AND laneprop.LaneId=camera.LaneId";
  $sql .= sprintf(" WHERE camera.CameraId='%s'", $camera_id);
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
function chdbprop_insert_agentpoll_whitelist($property_id, $lot_id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO agentpoll_table (AgentId,Tag)";
  $sql .= " SELECT DISTINCT agent.AgentId,'WhiteList' FROM agent_table AS agent";
  $sql .= "  INNER JOIN laneprop_table AS laneprop";
  $sql .= "   ON agent.ClusterId=laneprop.ClusterId";
  $sql .= sprintf(" WHERE laneprop.PropertyId='%s' AND laneprop.LotId='%s'",
		  $property_id, $lot_id);
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
function chdbprop_insert_agentpoll_makes() {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO agentpoll_table (AgentId,Tag)";
  $sql .= " SELECT DISTINCT agent.AgentId,'VehicleMake' FROM agent_table AS agent";
  $sql .= "  INNER JOIN agentprop_table AS agentprop";
  $sql .= "   ON agent.CompanyId=agentprop.CompanyId";
  $sql .= "    AND agent.AgentId=agentprop.AgentId";
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
// To be used when enforcment profile changes are made
function chdbprop_insert_agentpoll_lot_enforcement($enforcement_id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO agentpoll_table (AgentId,Tag)";
  $sql .= " SELECT DISTINCT agent.AgentId,'Lot' FROM agent_table AS agent";
  $sql .= "  INNER JOIN agentprop_table AS agentprop";
  $sql .= "   ON agent.CompanyId=agentprop.CompanyId";
  $sql .= "    AND agent.AgentId=agentprop.AgentId";
  $sql .= "  INNER JOIN policy_table AS policy";
  $sql .= "   ON policy.PropertyId=policy.PropertyId";
  $sql .= "    AND policy.LotId=policy.LotId";
  $sql .= sprintf(" WHERE policy.EnforcementId='%s'", $enforcement_id);
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
function chdbprop_insert_agentpoll_patroller($company_id, $patroller_id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO agentpoll_table (AgentId,Tag)";
  $sql .= " SELECT DISTINCT agent.AgentId,'Patrollers' FROM agent_table AS agent";
  $sql .= "  INNER JOIN agentprop_table AS agentprop";
  $sql .= "   ON agent.CompanyId=agentprop.CompanyId";
  $sql .= "    AND agent.AgentId=agentprop.AgentId";
  $sql .= "  INNER JOIN patroller_table AS patroller";
  $sql .= "   ON agent.CompanyId=patroller.CompanyId";
  $sql .= "  INNER JOIN patrollerprop_table AS patrollerprop";
  $sql .= "   ON patroller.CompanyId=patrollerprop.CompanyId";
  $sql .= "    AND patroller.PatrollerId=patrollerprop.PatrollerId";
  $sql .= "     AND agentprop.PropertyId=patrollerprop.PropertyId";
  $sql .= "      AND agentprop.LotId=patrollerprop.LotId";
  $sql .= sprintf(" WHERE agent.CompanyId='%s' AND patroller.PatrollerId='%s'",
		  $company_id, $patroller_id);
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
function chdbprop_insert_agentpoll_ticketcode($company_id, $enforcer_id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO agentpoll_table (AgentId,Tag)";
  $sql .= " SELECT DISTINCT agent.AgentId,'TicketCode' FROM agent_table AS agent";
  $sql .= "  INNER JOIN agentprop_table AS agentprop";
  $sql .= "   ON agent.CompanyId=agentprop.CompanyId";
  $sql .= "    AND agent.AgentId=agentprop.AgentId";
  $sql .= "  INNER JOIN enforcerprop_table AS enforcerprop";
  $sql .= "   ON agentprop.PropertyId=enforcerprop.PropertyId";
  $sql .= "    AND agentprop.LotId=enforcerprop.LotId";
  $sql .= sprintf(" WHERE agent.CompanyId='%s' AND enforcerprop.EnforcerId='%s'",
		  $company_id, $enforcer_id);
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
function chdbprop_insert_agentpoll_ticketprinter($company_id, $enforcer_id) {
  $db_name = getenv('APPS_LPMS_PROP_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PROP database!!!", 'error');
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
  $sql = "INSERT INTO agentpoll_table (AgentId,Tag)";
  $sql .= " SELECT DISTINCT agent.AgentId,'TicketPrinter' FROM agent_table AS agent";
  $sql .= "  INNER JOIN agentprop_table AS agentprop";
  $sql .= "   ON agent.CompanyId=agentprop.CompanyId";
  $sql .= "    AND agent.AgentId=agentprop.AgentId";
  $sql .= "  INNER JOIN enforcerprop_table AS enforcerprop";
  $sql .= "   ON agentprop.PropertyId=enforcerprop.PropertyId";
  $sql .= "    AND agentprop.LotId=enforcerprop.LotId";
  $sql .= sprintf(" WHERE agent.CompanyId='%s' AND enforcerprop.EnforcerId='%s'",
		  $company_id, $enforcer_id);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



