<?php



/****************   complex composites   **********************/



// Get all permits on a property, inclusive any group attached to it
function chdbperm_cc_get_permits($company_id, $property_id, $lot_id,
				 $filter_group_id = FALSE, $group_id = NULL,
				 $filter_member_id = FALSE, $member_id = NULL,
				 $filter_begintime_gt = FALSE, $begintime_gt = NULL,
				 $filter_begintime_lt = FALSE, $begintime_lt = NULL,
				 $filter_endtime_gt = FALSE, $endtime_gt = NULL,
				 $filter_endtime_lt = FALSE, $endtime_lt = NULL,
				 $filter_active = TRUE, $active = TRUE,
				 $filter_suspended = TRUE, $suspended = FALSE,
				 $filter_class = FALSE, $class = NULL,
				 $order_by = 'None') {

  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT permit.* FROM permit_table AS permit";
  $sql .= " INNER JOIN groupprop_table AS groupprop";
  $sql .= "  ON (permit.CompanyId=groupprop.CompanyId)";
  $sql .= "   AND ((LENGTH(permit.GroupId)>0 AND permit.GroupId=groupprop.GroupId)";
  $sql .= "     OR (LENGTH(permit.MemberId)>0 AND permit.MemberId=groupprop.GroupId))";
  $sql .= sprintf(" WHERE permit.CompanyId='%s' AND groupprop.PropertyId='%s'",
		  $company_id, $property_id);
  $sql .= sprintf(" AND (groupprop.LotId='%s' OR groupprop.LotId='ANY')",
		  $lot_id);
  if ($filter_active) {
    $sql .= sprintf(" AND permit.Active=%d", $active ? 1 : 0);
  }
  if ($filter_suspended) {
    $sql .= sprintf(" AND permit.Suspended=%d", $suspended ? 1 : 0);
  }
  if ($filter_group_id) {
    $sql .= sprintf(" AND permit.GroupId='%s'", $group_id);
  }
  if ($filter_member_id) {
    $sql .= sprintf(" AND permit.MemberId='%s'", $member_id);
  }
  if ($filter_begintime_gt) {
    $sql .= sprintf(" AND permit.BeginTime>='%s'", $begintime_gt);
  }
  if ($filter_begintime_lt) {
    $sql .= sprintf(" AND permit.BeginTime<'%s'", $begintime_lt);
  }
  if ($filter_endtime_gt) {
    $sql .= sprintf(" AND permit.ExpiryTime>'%s'", $endtime_gt);
  }
  if ($filter_endtime_lt) {
    $sql .= sprintf(" AND permit.ExpiryTime<='%s'", $endtime_lt);
  }
  if (!$filter_group_id and !$filter_member_id) {
    $sql .= " UNION ";
    $sql .= "SELECT permit.* FROM permit_table AS permit";
    $sql .= " WHERE (LENGTH(permit.GroupId)=0 OR permit.GroupId IS NULL)";
    $sql .= " AND (LENGTH(permit.MemberId)=0 OR permit.MemberId IS NULL)";
    $sql .= sprintf(" AND permit.CompanyId='%s' AND permit.PropertyId='%s'",
		    $company_id, $property_id);
    $sql .= sprintf(" AND permit.LotId='%s'", $lot_id);
    if ($filter_active) {
      $sql .= sprintf(" AND permit.Active=%d", $active ? 1 : 0);
    }
    if ($filter_suspended) {
      $sql .= sprintf(" AND permit.Suspended=%d", $suspended ? 1 : 0);
    }
    if ($filter_begintime_gt) {
      $sql .= sprintf(" AND permit.BeginTime>='%s'", $begintime_gt);
    }
    if ($filter_begintime_lt) {
      $sql .= sprintf(" AND permit.BeginTime<'%s'", $begintime_lt);
    }
    if ($filter_endtime_gt) {
      $sql .= sprintf(" AND permit.ExpiryTime>'%s'", $endtime_gt);
    }
    if ($filter_endtime_lt) {
      $sql .= sprintf(" AND permit.ExpiryTime<='%s'", $endtime_lt);
    }
  }
  if ($order_by !== 'None') {
    $sql .= sprintf(" ORDER BY %s", $order_by);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_cc_get_admin_containers($company_id,
					  $property_id = 'ANY',
					  $lot_id = 'ANY') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  if ($property_id !== 'ANY') {
    $sql = sprintf("ATTACH '%s' AS props", $db_name);
    $db->exec($sql);
  }
  //drupal_set_message($sql);
  // SQL
  $sql = "SELECT container.* FROM container_table AS container";
  if ($property_id !== 'ANY' or $lot_id === 'ANY') {
    $sql .= " INNER JOIN containerprop_table AS contprop";
    $sql .= "  ON container.CompanyId=contprop.CompanyId";
    $sql .= "   AND container.ContainerId=contprop.ContainerId";
  }
  $sql .= sprintf(" WHERE container.CompanyId='%s'", $company_id);
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" AND contprop.PropertyId='%s'", $property_id);
  }
  if ($lot_id !== 'ANY') {
    $sql .= sprintf(" AND contprop.LotId='%s'", $lot_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_cc_get_containers($uid, $company_id,
				    $property_id = 'ANY',
				    $lot_id = 'ANY') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT container.* FROM container_table AS container";
  $sql .= " INNER JOIN containerprop_table AS contprop";
  $sql .= "  ON container.CompanyId=contprop.CompanyId";
  $sql .= "   AND container.ContainerId=contprop.ContainerId";
  $sql .= " INNER JOIN props.user_table AS user";
  $sql .= "  ON (container.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (contprop.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= "    AND (contprop.LotId=user.LotId OR user.LotId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d AND container.CompanyId='%s'",
		  $uid, $company_id);
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" AND contprop.PropertyId='%s'", $property_id);
  }
  if ($lot_id !== 'ANY') {
    $sql .= sprintf(" AND contprop.LotId='%s'", $lot_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_cc_get_admin_container_types($company_id, $container_id, $property_id = 'ANY') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  if ($property_id !== 'ANY') {
    $sql = sprintf("ATTACH '%s' AS props", $db_name);
    $db->exec($sql);
  }
  // SQL
  $sql = "SELECT conttype.* FROM containertype_table AS conttype";
  $sql .= " INNER JOIN container_table AS container";
  $sql .= "  ON conttype.CompanyId=container.CompanyId";
  $sql .= "   AND conttype.ContainerId=container.ContainerId";
  if ($property_id !== 'ANY') {
    $sql .= " INNER JOIN containerprop_table AS contprop";
    $sql .= "  ON container.CompanyId=contprop.CompanyId";
    $sql .= "   AND container.ContainerId=contprop.ContainerId";
    $sql .= " INNER JOIN props.property_table AS property";
    $sql .= "  ON contprop.PropertyId=property.PropertyId";
    $sql .= " INNER JOIN props.relations_table AS relation";
    $sql .= "  ON contprop.CompanyId=relation.CompanyId";
    $sql .= "   AND property.PropertyId=relation.PropertyId";
  }
  $sql .= sprintf(" WHERE conttype.CompanyId='%s' AND conttype.ContainerId='%s'",
		  $company_id, $container_id);
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" AND property.PropertyId='%s'", $property_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_cc_get_container_types($uid, $company_id, $container_id, $property_id = 'ANY') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT conttype.* FROM containertype_table AS conttype";
  $sql .= " INNER JOIN container_table AS container";
  $sql .= "  ON conttype.CompanyId=container.CompanyId";
  $sql .= "   AND conttype.ContainerId=container.ContainerId";
  if ($property_id !== 'ANY') {
    $sql .= " INNER JOIN containerprop_table AS contprop";
    $sql .= "  ON container.CompanyId=contprop.CompanyId";
    $sql .= "   AND container.ContainerId=contprop.ContainerId";
    $sql .= " INNER JOIN props.property_table AS property";
    $sql .= "  ON contprop.PropertyId=property.PropertyId";
    $sql .= " INNER JOIN props.relations_table AS relation";
    $sql .= "  ON contprop.CompanyId=relation.CompanyId";
    $sql .= "   AND property.PropertyId=relation.PropertyId";
  }
  $sql .= " INNER JOIN props.user_table AS user";
  $sql .= "  ON (conttype.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  if ($property_id !== 'ANY') {
    $sql .= "   AND (property.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  }
  $sql .= sprintf(" WHERE user.UID=%d AND conttype.CompanyId='%s' AND conttype.ContainerId='%s'",
		  $uid, $company_id, $container_id);
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" AND property.PropertyId='%s'", $property_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_cc_get_admin_container_props($company_id, $container_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT contprop.* FROM containerprop_table AS contprop";
  $sql .= " INNER JOIN container_table AS container";
  $sql .= "  ON contprop.CompanyId=container.CompanyId";
  $sql .= "   AND contprop.ContainerId=container.ContainerId";
  $sql .= " INNER JOIN props.property_table AS property";
  $sql .= "  ON contprop.PropertyId=property.PropertyId";
  $sql .= " INNER JOIN props.relations_table AS relation";
  $sql .= "  ON property.PropertyId=relation.PropertyId";
  $sql .= "   AND contprop.CompanyId=relation.CompanyId";
  $sql .= sprintf(" WHERE contprop.CompanyId='%s' AND contprop.ContainerId='%s'",
		  $company_id, $container_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_cc_get_container_props($uid, $company_id, $container_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT contprop.* FROM containerprop_table AS contprop";
  $sql .= " INNER JOIN container_table AS container";
  $sql .= "  ON contprop.CompanyId=container.CompanyId";
  $sql .= "   AND contprop.ContainerId=container.ContainerId";
  $sql .= " INNER JOIN props.property_table AS property";
  $sql .= "  ON contprop.PropertyId=property.PropertyId";
  $sql .= " INNER JOIN props.relations_table AS relation";
  $sql .= "  ON property.PropertyId=relation.PropertyId";
  $sql .= "   AND contprop.CompanyId=relation.CompanyId";
  $sql .= " INNER JOIN props.user_table AS user";
  $sql .= "  ON (relation.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (property.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d AND contprop.CompanyId='%s' AND contprop.ContainerId='%s'",
		  $uid, $company_id, $container_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_cc_get_admin_groups($company_id,
				      $property_id = 'ANY',
				      $filter_active = TRUE, $active = TRUE,
				      $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  if ($property_id !== 'ANY') {
    $sql = sprintf("ATTACH '%s' AS props", $db_name);
    $db->exec($sql);
  }
  // SQL
  $sql = "SELECT groupdef.* FROM groupdef_table AS groupdef";
  if ($property_id !== 'ANY') {
    $sql .= " INNER JOIN groupprop_table AS groupprop";
    $sql .= "  ON groupdef.CompanyId=groupprop.CompanyId";
    $sql .= "   AND groupdef.GroupId=groupprop.GroupId";
    $sql .= " INNER JOIN props.property_table AS property";
    $sql .= "  ON groupprop.PropertyId=property.PropertyId";
    $sql .= " INNER JOIN props.relations_table AS relation";
    $sql .= "  ON groupprop.CompanyId=relation.CompanyId";
    $sql .= "   AND groupprop.PropertyId=relation.PropertyId";
  }
  $precond = TRUE;
  if ($company_id !== 'ANY') {
    $sql .= sprintf(" %s groupdef.CompanyId='%s'", $precond ? 'WHERE' : 'AND', $company_id);
    $precond = FALSE;
  }
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" %s property.PropertyId='%s'", $precond ? 'WHERE' : 'AND', $property_id);
    $precond = FALSE;
  }
  if ($filter_active) {
    $sql .= sprintf(" %s groupdef.Active=%d", $precond ? 'WHERE' : 'AND', $active ? 1 : 0);
    $precond = FALSE;
  }
  if ($filter_suspended) {
    $sql .= sprintf(" %s groupdef.Suspended=%d", $precond ? 'WHERE' : 'AND', $suspended ? 1 : 0);
    $precond = FALSE;
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_cc_get_groups($uid, $company_id,
				$property_id = 'ANY',
				$filter_active = TRUE, $active = TRUE,
				$filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT groupdef.* FROM groupdef_table AS groupdef";
  $sql .= " INNER JOIN groupprop_table AS groupprop";
  $sql .= "  ON groupdef.CompanyId=groupprop.CompanyId";
  $sql .= "   AND groupdef.GroupId=groupprop.GroupId";
  $sql .= " INNER JOIN props.property_table AS property";
  $sql .= "  ON groupprop.PropertyId=property.PropertyId";
  $sql .= " INNER JOIN props.relations_table AS relation";
  $sql .= "  ON groupprop.CompanyId=relation.CompanyId";
  $sql .= "   AND groupprop.PropertyId=relation.PropertyId";
  $sql .= " INNER JOIN props.user_table AS user";
  $sql .= "  ON (groupdef.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (groupdef.ClientId=user.ClientId OR user.ClientId='ANY')";
  $sql .= "    AND (groupprop.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= "     AND (groupprop.LotId=user.LotId OR groupprop.LotId='ANY' OR user.LotId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d", $uid);
  if ($company_id !== 'ANY') {
    $sql .= sprintf(" AND groupdef.CompanyId='%s'", $company_id);
  }
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" AND property.PropertyId='%s'", $property_id);
  }
  if ($filter_active) {
    $sql .= sprintf(" AND groupdef.Active=%d", $active ? 1 : 0);
  }
  if ($filter_suspended) {
    $sql .= sprintf(" AND groupdef.Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_cc_get_group_props($uid, $company_id, $client_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT groupprop.* FROM groupprop_table AS groupprop";
  $sql .= " INNER JOIN props.property_table AS property";
  $sql .= "  ON groupprop.PropertyId=property.PropertyId";
  $sql .= " INNER JOIN props.relations_table AS relation";
  $sql .= "  ON groupprop.CompanyId=relation.CompanyId";
  $sql .= "   AND groupprop.PropertyId=relation.PropertyId";
  $sql .= " INNER JOIN props.user_table AS user";
  $sql .= "  ON (groupprop.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
  $sql .= "   AND (groupprop.ClientId=user.ClientId OR user.ClientId='ANY')";
  $sql .= "    AND (groupprop.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
  $sql .= "     AND (groupprop.LotId=user.LotId OR groupprop.LotId='ANY' OR user.LotId='ANY')";
  $sql .= sprintf(" WHERE user.UID=%d AND groupprop.CompanyId='%s' AND groupprop.ClientId='%s'",
		  $uid, $company_id, $client_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



/****************   container_table   **********************/



function chdbperm_get_all_containers() {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM container_table";
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_get_containers($company_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM container_table WHERE CompanyId='%s'",
		 $company_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_get_container($company_id, $container_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM container_table WHERE";
  $sql .= sprintf(" CompanyId='%s' AND ContainerId='%s'",
		  $company_id, $container_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbperm_get_container_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM container_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbperm_get_container_alias($company_id, $alias) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM container_table WHERE";
  $sql .= sprintf(" CompanyId='%s' AND Alias='%s'",
		  $company_id, $alias);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 1 if exists, otherwise 0
function chdbperm_container_alias_exists($company_id, $alias) {
  $entry = chdbperm_get_container_alias($company_id, $alias);
  if (sizeof($entry) > 0) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_insert_container($company_id, $container_id, $uid, $alias, &$id) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "INSERT INTO container_table (CompanyId,ContainerId,CreatorUID,Alias)";
  $sql .= sprintf(" VALUES ('%s','%s',%d,'%s')",
		  $company_id, $container_id, $uid, $alias);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_container($id, $uid, $alias) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "UPDATE container_table SET ModifyTime=CURRENT_TIMESTAMP,";
  $sql .= sprintf("ModifierUID=%d,Alias='%s' WHERE Id=%d",
		  $uid, $alias, $id);
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
function chdbperm_delete_container_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("DELETE FROM container_table WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   groupdef_table    **********************/



function chdbperm_get_groupdefs($company_id,
				$filter_active = TRUE, $active = TRUE,
				$filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM groupdef_table";
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



function chdbperm_get_groupdef($company_id, $group_id,
			       $filter_active = TRUE, $active = TRUE,
			       $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM groupdef_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND GroupId='%s'",
		  $company_id, $group_id);
  if ($filter_active) {
    $sql .= sprintf(" AND Active=%d", $active ? 1 : 0);
  }
  if ($filter_suspended) {
    $sql .= sprintf(" AND Suspended=%d", $suspended ? 1 : 0);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbperm_get_groupdef_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM groupdef_table WHERE ";
  $sql .= sprintf("Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbperm_insert_groupdef($uid, $company_id, $client_id, $group_id,
				  $single, $alias, $lat, $lon,
				  $prename_id, $firstname, $middlename, $lastname, $sufname_id,
				  $street, $city, $postalcode, $province, $country,
				  $phone, $email,
				  $max_entries, $complementary, $shared,
				  $validation, $val_commercial, $val_residential,
				  $val_dailymax, $val_weeklymax, $val_monthlymax,
				  $val_length, $val_mission, $val_benefit,
				  $payment, $pay_interval, $pay_amount, $billing,
				  &$id) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "INSERT INTO groupdef_table (CreatorUID,CompanyId,ClientId,GroupId, ";
  $sql .= "Single,Alias,Lat,Lon, ";
  $sql .= "PrenameId,FirstName,MiddleName,LastName,SufnameId, ";
  $sql .= "Street,City,PostalCode,Province,Country, ";
  $sql .= "Phone,Email, ";
  $sql .= "MaxEntries,Complementary,Shared, ";
  $sql .= "Validation,ValCommercial,ValResidential, ";
  $sql .= "ValDailyMax,ValWeeklyMax,ValMonthlyMax, ";
  $sql .= "ValLengths,ValMissions,ValBenefits, ";
  $sql .= "Payment,PayInterval,PayAmount,Billing) VALUES (";
  $sql .= sprintf("%d,'%s','%s','%s', ".
		  "%d,'%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s', ".
		  "%d,%d,%d, ".
		  "%d,%d,%d, ".
		  "%d,%d,%d, ".
		  "'%s','%s','%s', ".
		  "'%s','%s',%0.02f,'%s')",
		  $uid, $company_id, $client_id, $group_id,
		  $single ? 1 : 0, $alias, $lat, $lon,
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id,
		  $street, $city, $postalcode, $province, $country,
		  $phone, $email,
		  $max_entries, $complementary, $shared,
		  $validation, $val_commercial, $val_residential,
		  $val_dailymax, $val_weeklymax, $val_monthlymax,
		  $val_length, $val_mission, $val_benefit,
		  $payment, $pay_interval, $pay_amount, $billing);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_groupdef($id, $uid, $alias, $lat, $lon,
				  $prename_id, $firstname, $middlename, $lastname, $sufname_id,
				  $street, $city, $postalcode, $province, $country,
				  $phone, $email,
				  $max_entries, $complementary, $shared,
				  $validation, $val_commercial, $val_residential,
				  $val_dailymax, $val_weeklymax, $val_monthlymax,
				  $val_length, $val_mission, $val_benefit,
				  $payment, $pay_interval, $pay_amount, $billing) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "UPDATE groupdef_table SET ";
  $sql .= sprintf("ModifyTime=CURRENT_TIMESTAMP,ModifierUID=%d, Alias='%s',Lat='%s',Lon='%s'",
		  $uid, $alias, $lat, $lon);
  $sql .= sprintf(", PrenameId='%s',FirstName='%s',MiddleName='%s',LastName='%s',SufnameId='%s'",
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id);
  $sql .= sprintf(", Street='%s',City='%s',PostalCode='%s',Province='%s',Country='%s',Phone='%s',Email='%s'",
		  $street, $city, $postalcode, $province, $country, $phone, $email);
  $sql .= sprintf(", Phone='%s',Email='%s'",
		  $phone, $email);
  $sql .= sprintf(", MaxEntries=%d,Complementary=%d,Shared=%d",
		  $max_entries, $complementary, $shared);
  $sql .= sprintf(", Validation=%d,ValCommercial=%d,ValResidential=%d",
		  $validation, $val_commercial, $val_residential);
  $sql .= sprintf(", ValDailyMax=%d,ValWeeklyMax=%d,ValMonthlyMax=%d",
		  $val_dailymax, $val_weeklymax, $val_monthlymax);
  $sql .= sprintf(", ValLengths='%s',ValMissions='%s',ValBenefits='%s'",
		  $val_length, $val_mission, $val_benefit);
  $sql .= sprintf(", Payment='%s',PayInterval='%s',PayAmount=%0.02f,Billing='%s'",
		  $payment, $pay_interval, $pay_amount, $billing);
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
function chdbperm_update_groupdef_status($id, $activate, $suspended) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "UPDATE groupdef_table SET ";
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
function chdbperm_retire_groupdef_id($id, $uid, $reason = '',
				     $retention_days = 365) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "UPDATE groupdef_table SET Active=0,Deactivated=1,DeactivationTime=CURRENT_TIMESTAMP";
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
function chdbperm_delete_groupdef_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "DELETE FROM groupdef_table WHERE ";
  $sql .= sprintf("Id=%d", $id);
  //drupal_set_message($sql);
  // Perform DB command
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   grouppay_table    **********************/



function chdbperm_get_grouppays($company_id, $client_id, $group_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM grouppay_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND ClientId='%s' AND GroupId='%s'",
		  $company_id, $client_id, $group_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbperm_insert_grouppay($company_id, $client_id, $group_id,
				  $amount, $reference,
				  $billing, $billing_info, $expiry_time,
				  &$id) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "INSERT INTO grouppay_table (CompanyId,ClientId,GroupId,";
  $sql .= "PayAmount,PayReference,Billing,BillingInfo,ExpiryTime) VALUES (";
  $sql .= sprintf("'%s','%s','%s','%s','%s','%s','%s','%s')",
		  $company_id, $client_id, $group_id,
		  $amount, $reference, $billing, $billing_info, $expiry_time);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_grouppay_inactivate($company_id, $client_id, $group_id) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "UPDATE grouppay_table SET Active=0";
  $sql .= sprintf(" WHERE CompanyId='%s' AND ClientId='%s' AND GroupId='%s'",
		  $company_id, $client_id, $group_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   groupadmin_table    **********************/



function chdbperm_get_groupadmins($company_id, $client_id, $group_id,
				  $filter_active = TRUE, $active = TRUE,
				  $filter_suspended = TRUE, $suspended = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM groupadmin_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND ClientId='%s' AND GroupId='%s'",
		  $company_id, $client_id, $group_id);
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
function chdbperm_insert_groupadmin($company_id, $client_id, $group_id, $admin_id, $lat, $lon,
				    $prename_id, $firstname, $middlename, $lastname, $sufname_id,
				    $street, $city, $postalcode, $province, $country,
				    $phone, $email, $emailnotification,
				    $role, $department, $title,
				    &$id) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "INSERT INTO groupadmin_table (CompanyId,ClientId,GroupId,AdminId,Lat,Lon,";
  $sql .= "PrenameId,FirstName,MiddleName,LastName,SufnameId,";
  $sql .= "Street,City,PostalCode,Province,Country, ";
  $sql .= "Phone,Email,EmailNotification, ";
  $sql .= "Role,Department,Title) VALUES (";
  $sql .= sprintf("'%s','%s','%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s',%d, ".
		  "'%s','%s','%s')",
		  $company_id, $client_id, $group_id, $admin_id, $lat, $lon,
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id,
		  $street, $city, $postalcode, $province, $country,
		  $phone, $email, $emailnotification ? 1 : 0,
		  $role, $department, $title);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_groupadmin($id,
				    $prename_id, $firstname, $middlename, $lastname, $sufname_id,
				    $street, $city, $postalcode, $province, $country,
				    $phone, $email, $emailnotification,
				    $role, $department, $title) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "UPDATE groupadmin_table SET ";
  $sql .= sprintf("PrenameId='%s',FirstName='%s',MiddleName='%s', LastName='%s',SufnameId='%s', ",
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id);
  $sql .= sprintf("Street='%s',City='%s',PostalCode='%s', Province='%s',Country='%s', ",
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
function chdbperm_update_groupadmin_latlon($id, $lat, $lon, $manlatlon = FALSE) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = 'UPDATE groupadmin_table SET';
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
function chdbperm_update_groupadmins_status($company_id, $group_id, $activate, $suspended) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "UPDATE groupadmin_table SET ";
  $sql .= sprintf("Active=%d,Suspended=%d",
		  $activate ? 1 : 0, $suspended ? 1 : 0);
  if ($activate) {
    $sql .= ",Deactivated=0,DeactivationTime=NULL,DeactivationUID=NULL,DeactivationReason=NULL";
  }
  $sql .= sprintf(" WHERE CompanyId='%s' AND GroupId='%s'",
		  $company_id, $group_id);
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
function chdbperm_retire_groupadmins($company_id, $client_id, $group_id, $uid,
				     $reason = '', $retention_days = 365) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "UPDATE groupadmin_table SET Active=0,Deactivated=1,DeactivationTime=CURRENT_TIMESTAMP";
  $sql .= sprintf(",DeactivationUID=%d,DeactivationReason='%s'",
		  $uid, $reason);
  if ($retention_days > 0) {
    $sql .= sprintf(",RetentionTime=DATETIME(CURRENT_TIMESTAMP,'%d DAYS')",
		    $retention_days);
  }
  $sql .= sprintf(" WHERE CompanyId='%s' AND ClientId='%s' AND  GroupId='%s'",
		  $company_id, $client_id, $group_id);
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
function chdbperm_delete_groupadmin($company_id, $group_id, $admin_id) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "DELETE FROM groupadmin_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND GroupId='%s' AND AdminId='%s'",
		  $company_id, $group_id, $admin_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   groupprop_table    **********************/



function chdbperm_get_all_groupprops($company_id, $client_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM groupprop_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND ClientId='%s'",
		  $company_id, $client_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_get_groupprops($company_id, $client_id, $group_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM groupprop_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND GroupId='%s'",
		  $company_id, $group_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_get_groupprops_property($company_id, $property_id,
					  $filter_lot = FALSE, $lot_id = '',
					  $filter_group = FALSE, $group_id = '') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM groupprop_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s'",
		  $company_id, $property_id);
  if ($filter_lot) {
    $sql .= sprintf(" AND LotId='%s'", $lot_id);
  }
  if ($filter_group) {
    $sql .= sprintf(" AND GroupId='%s'", $group_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbperm_insert_groupprop($company_id, $client_id, $group_id,
				   $property_id, $lot_id,
				   &$id) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "INSERT INTO groupprop_table (CompanyId,ClientId,GroupId,PropertyId,LotId) VALUES (";
  $sql .= sprintf("'%s','%s','%s','%s','%s')",
		  $company_id, $client_id, $group_id, $property_id, $lot_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1
function chdbperm_delete_groupprop($company_id, $client_id, $group_id,
				   $property_id, $lot_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "DELETE FROM groupprop_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND ClientId='%s' AND GroupId='%s'",
		  $company_id, $client_id, $group_id);
  $sql .= sprintf(" AND PropertyId='%s' AND LotId='%s'",
		  $property_id, $lot_id);
  //drupal_set_message($sql);
  // Perform DB command
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   groupnote_table   **********************/



function chdbperm_get_groupnotes($company_id, $client_id, $group_id,
				 $active = TRUE, $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM groupnote_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND ClientId='%s' AND GroupId='%s'",
		  $company_id, $client_id, $group_id);
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



function chdbperm_get_groupnote($company_id, $client_id, $group_id, $note_id,
				$active = TRUE, $fragment = FALSE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM groupnote_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND ClientId='%s' AND GroupId='%s' AND NoteId='%s'",
		  $company_id, $client_id, $group_id, $note_id);
  $sql .= sprintf(" AND Active=%d AND Fragment=%d",
		  $active ? 1 : 0, $fragment ? 1 : 0);
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



function chdbperm_get_groupnote_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM groupnote_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbperm_insert_groupnote($uid, $company_id, $client_id, $group_id, $note_id, $note, &$id) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "INSERT INTO groupnote_table (CreatorUID,CompanyId,ClientId,GroupId,NoteId,Note)";
  $sql .= sprintf(" VALUES (%d,'%s','%s','%s','%s','%s')",
		  $uid, $company_id, $client_id, $group_id, $note_id, $note);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_groupnote($id, $uid, $note) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM groupnote_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $master = $entry;
  }
  if (empty($master)) {
    drupal_set_message("Not able to read from DB", 'error');
    return 1;
  }
  // Update master non-fragment
  $sql = "UPDATE groupnote_table SET ModifyTime=CURRENT_TIMESTAMP,";
  $sql .= sprintf("ModifiedByUID=%d WHERE Id=%d", $uid, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write-1 to DB", 'error');
    return 1;
  }
  // Insert new fragment
  $sql = "INSERT INTO groupnote_table ";
  $sql .= "(CreatorUID,CompanyId,ClientId,GroupId,NoteId,Note,Fragment) VALUES ";
  $sql .= sprintf("(%d,'%s','%s','%s','%s','%s',1)",
		  $uid,
		  $master['CompanyId'],
		  $master['ClientId'],
		  $master['GroupId'],
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
function chdbperm_groupnote_solved($id, $uid, $resolution) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM groupnote_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $master = $entry;
  }
  if (empty($master)) {
    drupal_set_message("Not able to read from DB", 'error');
    return 1;
  }
  // Query
  $sql = "UPDATE groupnote_table SET SolvedTime=CURRENT_TIMESTAMP,Active=0,Solved=1,";
  $sql .= sprintf("SolvedByUID=%d,Resolution='%s' WHERE Active=1 AND ", $uid, $resolution);
  $sql .= sprintf("CompanyId='%s' AND ClientId='%s' AND GroupId='%s' AND NoteId='%s'",
		  $master['CompanyId'],
		  $master['ClientId'],
		  $master['GroupId'],
		  $master['NoteId']);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_delete_groupnote_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM groupnote_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $master = $entry;
  }
  if (empty($master)) {
    drupal_set_message("Not able to read from DB", 'error');
    return 1;
  }
  // Perform DB command
  $sql = "DELETE FROM groupnote_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND ClientId='%s' AND GroupId='%s' AND NoteId='%s'",
		  $master['CompanyId'],
		  $master['ClientId'],
		  $master['GroupId'],
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



/****************   groupnoteid_table   **********************/



function chdbperm_get_groupnoteid($company_id, $client_id, $group_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM groupnoteid_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND ClientId='%s' AND GroupId='%s'",
		  $company_id, $client_id, $group_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbperm_insert_groupnoteid($company_id, $client_id, $group_id, &$id) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "INSERT INTO groupnoteid_table (CompanyId,ClientId,GroupId) VALUES";
  $sql .= sprintf(" ('%s','%s','%s')", $company_id, $client_id, $group_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_groupnoteid_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "UPDATE groupnoteid_table SET Counter=Counter+1";
  $sql .= sprintf(" WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_delete_groupnoteid($company_id, $client_id, $group_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "DELETE FROM groupnoteid_table";
  $sql .= sprintf(" WHERE CompanyId='%s' AND ClientId='%s' AND GroupId='%s'",
		  $company_id, $client_id, $group_id);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   containerprop_table    **********************/



function chdbperm_get_all_container_props($company_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM containerprop_table";
  $sql .= sprintf(" WHERE CompanyId='%s'", $company_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_get_contprops($company_id, $container_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM containerprop_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND ContainerId='%s'",
		 $company_id, $container_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



/* function chdbperm_get_contprop_containers($company_id, $property_id) { */
/*   $entries = array(); */
/*   // Find location of SQLite database */
/*   $db_name = getenv('APPS_LPMS_PERMIT_DB'); */
/*   if ($db_name == FALSE) { */
/*     drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error'); */
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
/*   $sql = "SELECT * FROM containerprop_table WHERE "; */
/*   $sql .= sprintf("CompanyId='%s' AND PropertyId='%s'", */
/* 		 $company_id, $property_id); */
/*   //drupal_set_message($sql); */
/*   foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) { */
/*     $entries[] = $entry; */
/*   } */
/*   return $entries; */
/* } */



// Return 0 on success, otherwise 1 
function chdbperm_insert_contprop($company_id, $container_id, $property_id, $lot_id, &$id) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "INSERT INTO containerprop_table (CompanyId,ContainerId,PropertyId,LotId)";
  $sql .= sprintf(" VALUES ('%s','%s','%s','%s')",
		  $company_id, $container_id, $property_id, $lot_id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  return 0;
}



// Return 0 on success, otherwise 1
function chdbperm_delete_contprops($company_id, $container_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "DELETE FROM containerprop_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND ContainerId='%s'",
		  $company_id, $container_id);
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
function chdbperm_delete_contprop($company_id, $container_id, $property_id, $lot_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "DELETE FROM containerprop_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND ContainerId='%s'",
		  $company_id, $container_id);
  $sql .= sprintf("AND PropertyId='%s' AND LotId='%s'",
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
function chdbperm_delete_contprop_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("DELETE FROM containerprop_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   containertype_table   **********************/



function chdbperm_get_all_container_types($company_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM containertype_table";
  $sql .= sprintf(" WHERE CompanyId='%s'", $company_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_get_conttypes($company_id, $container_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM containertype_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND ContainerId='%s'",
		 $company_id, $container_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_get_conttype($company_id, $container_id, $type_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM containertype_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND ContainerId='%s' AND TypeId='%s'",
		 $company_id, $container_id, $type_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbperm_get_conttype_alias($company_id, $container_id, $alias) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM containertype_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND ContainerId='%s' AND Alias='%s'",
		 $company_id, $container_id, $alias);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbperm_get_conttype_id($id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM containertype_table WHERE Id=%d", $id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 1 if exists, otherwise 0
function chdbperm_conttype_exists($company_id, $container_id, $alias) {
  $entry = chdbperm_get_conttype_alias($company_id, $container_id, $alias);
  if (sizeof($entry) > 0) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_insert_conttype($company_id, $container_id, $type_id, $uid,
				  $long_term, $short_term, $valet,
				  $transients, $transient_type,
				  $groups, $group_type,
				  $members, $member_type,
				  $alias, $rate_type, $value_unit, $unit_value,
				  $max_cost, $fixed_period, $comment,
				  &$id) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "INSERT INTO containertype_table (CompanyId,ContainerId,TypeId,CreatorUID";
  $sql .= ",Long,Short,Valet,Transients,Groups,Members ";
  $sql .= ",Alias,RateType,ValueUnit,UnitValue,MaxCost,FixedPeriod,Comment";
  $sql .= sprintf(") VALUES (".
		  "'%s','%s','%s',%d, ".
		  "%d,%d,%d, ".
		  "%d, ".
		  "%d, ".
		  "%d, ".
		  "'%s','%s','%s',%0.02f, ".
		  "%0.02f,%d,'%s')",
		  $company_id, $container_id, $type_id, $uid,
		  $long_term ? 1 : 0, $short_term ? 1 : 0, $valet ? 1 : 0,
		  $transients ? 1 : 0,
		  $groups ? 1 : 0,
		  $members ? 1 : 0,
		  $alias, $rate_type, $value_unit, $unit_value,
		  $max_cost, $fixed_period, $comment);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  // SQL
  $sql = "UPDATE containertype_table SET ";
  if (NULL === $transient_type) {
    $sql .= "TransientType=NULL";
  } else {
    $sql .= sprintf("TransientType='%s'", $transient_type);
  }
  if (NULL === $group_type) {
    $sql .= ",GroupType=NULL";
  } else {
    $sql .= sprintf(",GroupType='%s'", $group_type);
  }
  if (NULL === $member_type) {
    $sql .= ",MemberType=NULL";
  } else {
    $sql .= sprintf(",MemberType='%s'", $group_type);
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
function chdbperm_update_conttype($id, $uid,
				  $long_term, $short_term, $valet,
				  $transients, $transient_type,
				  $groups, $group_type,
				  $members, $member_type,
				  $alias, $rate_type, $value_unit, $unit_value,
				  $max_cost, $fixed_period, $comment) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("UPDATE containertype_table SET Long=%d,Short=%d,Valet=%d",
		 $long_term ? 1 : 0, $short_term ? 1 : 0, $valet ? 1 : 0);
  $sql .= sprintf(", ModifierUID=%d,ModifyTime=CURRENT_TIMESTAMP", $uid);
  $sql .= sprintf(", Transients=%d", $transients ? 1 : 0);
  $sql .= sprintf(",Groups=%d", $groups ? 1 : 0);
  $sql .= sprintf(",Members=%d", $members ? 1 : 0);
  $sql .= sprintf(", Alias='%s',RateType='%s',ValueUnit='%s'",
		  $alias, $rate_type, $value_unit);
  $sql .= sprintf(", UnitValue=%0.02f,MaxCost=%0.02f,FixedPeriod=%d,Comment='%s'",
		  $unit_value, $max_cost, $fixed_period, $comment);
  if (NULL === $transient_type) {
    $sql .= ", TransientType=NULL";
  } else {
    $sql .= sprintf(",TransientType='%s'", $transient_type);
  }
  if (NULL === $group_type) {
    $sql .= ",GroupType=NULL";
  } else {
    $sql .= sprintf(",GroupType='%s'", $group_type);
  }
  if (NULL === $member_type) {
    $sql .= ",MemberType=NULL";
  } else {
    $sql .= sprintf(",MemberType='%s'", $group_type);
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
function chdbperm_delete_conttype($company_id, $container_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("DELETE FROM containertype_table WHERE CompanyId='%s' AND ContainerId='%s'",
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



// Return 0 on success, otherwise 1
function chdbperm_delete_conttype_id($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("DELETE FROM containertype_table WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



/****************   permit_table   **********************/



function chdbperm_get_permit_id($id, $active = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM permit_table WHERE Id=%d AND Active=%d",
		 $id, $active ? 1 : 0);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbperm_get_permits_lpn($company_id, $property_id, $lpn, $active = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM permit_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s' AND LPN='%s' AND Active=%d",
		  $company_id, $property_id, $lpn, $active ? 1 : 0);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_get_group_permit_lpn($company_id, $group_id, $lpn, $active = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM permit_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND GroupId='%s'", $company_id, $group_id);
  $sql .= sprintf(" AND LPN='%s' AND Active=%d", $lpn, $active ? 1 : 0);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbperm_get_permits_suite($company_id, $property_id, $lot_id,
				    $suite, $active = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM permit_table WHERE ";
  $sql .= sprintf("PermitTag='%s' AND Active=%d",
		  $suite, $active ? 1 : 0);
  $sql .= sprintf(" AND CompanyId='%s' AND PropertyId='%s' AND LotId='%s'",
		  $company_id, $property_id, $lot_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_get_permits_shared($company_id, $property_id, $lot_id,
				     $shared_id, $active = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM permit_table WHERE ";
  $sql .= sprintf("SharedId='%s' AND Active=%d",
		  $shared_id, $active ? 1 : 0);
  $sql .= sprintf(" AND CompanyId='%s' AND PropertyId='%s' AND LotId='%s'",
		  $company_id, $property_id, $lot_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_get_group_permits_shared($company_id, $group_id, $shared_id, $active = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM permit_table WHERE ";
  $sql .= sprintf("SharedId='%s' AND Active=%d",
		  $shared_id, $active ? 1 : 0);
  $sql .= sprintf(" AND CompanyId='%s' AND GroupId='%s'",
		  $company_id, $group_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_get_group_permits($company_id, $group_id, $member_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM permit_table WHERE CompanyId='%s'", $company_id);
  if (NULL !== $group_id) {
    $sql .= sprintf(" AND GroupId='%s'", $group_id);
  }
  if (NULL !== $member_id) {
    $sql .= sprintf(" AND MemberId='%s'", $member_id);
  }
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    if ($single) {
      $entries = $entry;
    } else {
      $entries[] = $entry;
    }
  }
  return $entries;
}



// Return 1 if exists, otherwise 0
function chdbperm_group_permit_exists($company_id, $group_id, $lpn, $active = TRUE) {
  $entry = chdbperm_get_group_permit_lpn($company_id, $group_id, $lpn, $active);
  if (sizeof($entry) > 0) {
    return 1;
  }
  return 0;
}



///////////////// TBD: try to eliminate all _get_ functions above for permit_table



function chdbperm_get_permits($company_id,
			      $filter_group = FALSE, $group_id = '',
			      $filter_property = FALSE, $property_id = '',
			      $filter_lot = FALSE, $lot_id = '',
			      $filter_lpn = FALSE, $lpn = '',
			      $filter_active = TRUE, $active = TRUE,
			      $filter_suspended = TRUE, $suspended = FALSE,
			      $filter_long = FALSE, $long = TRUE,
			      $filter_short = FALSE, $short = TRUE,
			      $filter_valet = FALSE, $valet = TRUE,
			      $order_by = 'None') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("SELECT * FROM permit_table WHERE CompanyId='%s'", $company_id);
  if ($filter_group) {
    $sql .= sprintf(" AND GroupId='%s'", $group_id);
  } else {
    $sql .= " AND LENGTH(GroupId) ISNULL";
  }
  if ($filter_property) {
    $sql .= sprintf(" AND PropertyId='%s'", $property_id);
  }
  if ($filter_lot) {
    $sql .= sprintf(" AND LotId='%s'", $lot_id);
  }
  if ($filter_lpn) {
    $sql .= sprintf(" AND LPN='%s'", $lpn);
  }
  if ($filter_active) {
    $sql .= sprintf(" AND Active=%d", $active ? 1 : 0);
  }
  if ($filter_suspended) {
    $sql .= sprintf(" AND Suspended=%d", $suspended ? 1 : 0);
  }
  if ($filter_long) {
    $sql .= sprintf(" AND Long=%d", $long ? 1 : 0);
  }
  if ($filter_short) {
    $sql .= sprintf(" AND Short=%d", $short ? 1 : 0);
  }
  if ($filter_valet) {
    $sql .= sprintf(" AND Valet=%d", $valet ? 1 : 0);
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



function chdbperm_get_permit_stalls($company_id, $property_id, $lot_id,
				    $level_id, $active = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM permit_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s' AND LotId='%s'",
		  $company_id, $property_id, $lot_id);
  $sql .= sprintf(" AND LevelId='%s' AND Active=%d",
		  $level_id, $active ? 1 : 0);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_get_permit_lpn($company_id, $property_id, $lot_id,
				 $lpn, $active = TRUE) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM permit_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s' AND LotId='%s'",
		  $company_id, $property_id, $lot_id);
  $sql .= sprintf(" AND LPN='%s' AND Active=%d",
		  $lpn, $active ? 1 : 0);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 1 if exists, otherwise 0
function chdbperm_permit_exists($company_id, $property_id, $lot_id, $lpn, $active = TRUE) {
  $entry = chdbperm_get_permit_lpn($company_id, $property_id, $lot_id, $lpn, $active);
  if (sizeof($entry) > 0) {
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1
// NULL values:
//  $group_id
//  $member_id
//  $role
//  $level_id
//  $floor_id
//  $container_id
//  $type_id
function chdbperm_insert_permit($company_id, $property_id, $lot_id, $group_id, $member_id,
				$uid, $lat, $lon,
				$long_term, $short_term, $valet,
				$prename_id, $firstname, $middlename, $lastname, $sufname_id,
				$street, $city, $postalcode, $province, $country,
				$phone, $email, $emailnotification,
				$role,
				$lpn, $make, $model, $color,
				$permit_tag, $floor_id, $suite_id, $level_id, $stall_id,
				$complementary, $cost,
				$permit_class, $container_id, $type_id,
				&$id) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "INSERT INTO permit_table (CompanyId,PropertyId,LotId, ";
  $sql .= "CreatorUID,Lat,Lon, ";
  $sql .= "Long,Short,Valet, ";
  $sql .= "PrenameId,FirstName,MiddleName,LastName,SufnameId, ";
  $sql .= "Street,City,PostalCode,Province,Country, ";
  $sql .= "Phone,Email,EmailNotification, ";
  $sql .= "Role,";
  $sql .= "LPN,Make,Model,Color, ";
  $sql .= "PermitTag,SuiteId,StallId, ";
  $sql .= "Complementary,Cost,";
  $sql .= "PermitClass)";
  $sql .= sprintf(" VALUES (".
		  "'%s','%s','%s', ".
		  "%d,'%s','%s', ".
		  "%d,%d,%d, ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s','%s','%s','%s', ".
		  "'%s','%s',%d, ".
		  "'%s',".
		  "'%s','%s','%s','%s', ".
		  "'%s','%s','%s', ".
		  "%d,'%0.02f', ".
		  "'%s')",
		  $company_id, $property_id, $lot_id,
		  $uid, $lat, $lon,
		  $long_term ? 1 : 0, $short_term ? 1 : 0, $valet ? 1 : 0,
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id,
		  $street, $city, $postalcode, $province, $country,
		  $phone, $email, $emailnotification ? 1 : 0,
		  $role,
		  $lpn, $make, $model, $color,
		  $permit_tag, $suite_id, $stall_id,
		  $complementary ? 1 : 0, $cost,
		  $permit_class);
  // PY : May 17.14
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $id = $db->lastInsertId();
  // SQL
  $sql = "UPDATE permit_table SET ";
  if ($group_id === NULL) {
    $sql .= "GroupId=NULL";
  } else {
    $sql .= sprintf("GroupId='%s'", $group_id);
  }
  if ($member_id === NULL) {
    $sql .= ",MemberId=NULL";
  } else {
    $sql .= sprintf(",MemberId='%s'", $member_id);
  }
  if ($floor_id === NULL) {
    $sql .= ",FloorId=NULL";
  } else {
    $sql .= sprintf(",FloorId='%s'", $floor_id);
  }
  if ($level_id === NULL) {
    $sql .= ",LevelId=NULL";
  } else {
    $sql .= sprintf(",LevelId='%s'", $level_id);
  }
  if ($container_id === NULL) {
    $sql .= ",ContainerId=NULL";
  } else {
    $sql .= sprintf(",ContainerId='%s'", $container_id);
  }
  if ($type_id === NULL) {
    $sql .= ",TypeId=NULL";
  } else {
    $sql .= sprintf(",TypeId='%s'", $type_id);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write-2 to DB", 'error');
    return 1;
  }
  // PY : Apr 29.14 - if PermitClass=Validation then update occu_table with AllCompliant=1
  $db_name = getenv('APPS_LPMS_OCCU_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for OCCU database!!!", 'error');
    return $entries;
  }
  if ($permit_class === 'Validation') {
      // connect to SQLite database
      try {
        $db = new PDO("sqlite:".$db_name);
      } catch(PDOException $e) {
        drupal_set_message($e->getMessage(), 'error');
        return 1;
      }
      // SQL
      $sql = "UPDATE occu_table SET AllCompliant=1, AllValidation='Validation' ";
      // PY : Apr 30.14 - added ValidationTime
      $sql .= sprintf(", ValidationTime=DATETIME(CURRENT_TIMESTAMP,'%d HOURS')", 1);
      $sql .= sprintf("WHERE LPN='%s' AND History=0 AND Active=1", $lpn);
      //drupal_set_message($sql);
      if (!$db->exec($sql)) {
        drupal_set_message("Not able to write-2 to DB", 'error');
        return 1;
      }
  }

  return 0;
}



// Return 0 on success, otherwise 1
function chdbperm_update_permit_id($id, $uid, $lat, $lon, $group_id, $member_id,
				   $prename_id, $firstname, $middlename, $lastname, $sufname_id,
				   $street, $city, $postalcode, $province, $country,
				   $phone, $email, $emailnotification,
				   $role,
				   $lpn, $make, $model, $color,
				   $floor_id, $suite_id, $level_id, $stall_id,
				   $complementary, $cost,
				   $permit_class, $container_id, $type_id) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("UPDATE permit_table SET ModifierUID=%d,Lat='%s',Lon='%s', ",
		 $uid, $lat, $lon);
  if ($group_id === NULL) {
    $sql .= "GroupId=NULL,";
  } else {
    $sql .= sprintf("GroupId='%s', ", $group_id);
  }
  if ($member_id === NULL) {
    $sql .= "MemberId=NULL,";
  } else {
    $sql .= sprintf("MemberId='%s', ", $member_id);
  }
  $sql .= sprintf("PrenameId='%s',FirstName='%s',MiddleName='%s',LastName='%s',SufnameId='%s', ",
		  $prename_id, $firstname, $middlename, $lastname, $sufname_id);
  $sql .= sprintf("Street='%s',City='%s',PostalCode='%s',Province='%s',Country='%s', ",
		  $street, $city, $postalcode, $province, $country);
  $sql .= sprintf("Phone='%s',Email='%s',EmailNotification=%d, ",
		  $phone, $email, $emailnotification ? 1 : 0);
  $sql .= sprintf("Role='%s', ",
		  $role);
  $sql .= sprintf("LPN='%s',Make='%s',Model='%s',Color='%s', ",
		  $lpn, $make, $model, $color);
  $sql .= sprintf("SuiteId='%s',StallId='%s', ",
		  $suite_id, $stall_id);
  if ($floor_id === NULL) {
    $sql .= "FloorId=NULL,";
  } else {
    $sql .= sprintf("FloorId='%s',", $floor_id);
  }
  if ($level_id === NULL) {
    $sql .= "LevelId=NULL,";
  } else {
    $sql .= sprintf("LevelId='%s',", $level_id);
  }
  $sql .= sprintf("Complementary=%d,Cost=%0.02f, ",
		  $complementary ? 1 : 0, $cost);
  $sql .= sprintf("PermitClass='%s',",
		  $permit_class);
  if ($container_id === NULL) {
    $sql .= "ContainerId=NULL,";
  } else {
    $sql .= sprintf("ContainerId='%s',", $container_id);
  }
  if ($type_id === NULL) {
    $sql .= "TypeId=NULL";
  } else {
    $sql .= sprintf("TypeId='%s'", $type_id);
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
function chdbperm_update_permit_timing_id($id,
					  $filter_begintime, $begin_time,
					  $filter_expirytime, $expiry_time) {
  if (!$filter_begintime and !$filter_expirytime) {
    return 0;
  }
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "UPDATE permit_table SET ";
  if ($filter_begintime) {
    $sql .= sprintf("BeginTime='%s'", $begin_time);
  }
  if ($filter_expirytime) {
    if ($filter_begintime) {
      $sql .= ',';
    }
    $sql .= sprintf("ExpiryTime='%s'", $expiry_time);
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
function chdbperm_update_permit_base($id,
				     $company_id, $property_id, $lot_id,
				     $long_term, $short_term, $valet,
				     $lpn,
				     $permit_tag, $permit_user,
				     $permit_class,
				     $container_id, $type_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "UPDATE permit_table SET ";
  $sql .= sprintf("CompanyId='%s',PropertyId='%s',LotId='%s'",
		  $company_id, $property_id, $lot_id);
  $sql .= sprintf(", Long=%d,Short=%d,Valet=%d",
		  $long_term ? 1 : 0, $short_term ? 1 : 0, $valet ? 1 : 0);
  $sql .= sprintf(",LPN='%s',PermitTag='%s',PermitUser='%s',PermitClass='%s'",
		  $lpn, $permit_tag, $permit_user, $permit_class);
  $sql .= sprintf(",ContainerId='%s',TypeId='%s'",
		  $container_id, $type_id);
  $sql .= sprintf(" WHERE Id=%d", $id);
  drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_permit_vehicle_info($id, $make, $model, $color) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("UPDATE permit_table SET Make='%s',Model='%s',Color='%s' WHERE Id=%d",
		 $make, $model, $color, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_permit_expiry_offset($id, $day_offset) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("UPDATE permit_table SET ExpiryTime=DATETIME(ExpiryTime,'%d DAYS') WHERE Id=%d",
		 $day_offset, $id);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $db = NULL;
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_permit_expiry_offset_from_create($id,
							  $month_offset,
							  $week_offset,
							  $day_offset,
							  $hour_offset) {
  if ($month_offset == 0 and $week_offset == 0 and $day_offset == 0 and $hour_offset == 0) {
    return 0;
  }
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = 'UPDATE permit_table SET ExpiryTime=DATETIME(CreateTime';
  if ($month_offset > 0) {
    $sql .= sprintf(",'%d MONTHS'", $month_offset);
  }
  if ($week_offset > 0) {
    $sql .= sprintf(",'%d DAYS'", $week_offset * 7);
  }
  if ($day_offset > 0) {
    $sql .= sprintf(",'%d DAYS'", $day_offset);
  }
  if ($hour_offset > 0) {
    $sql .= sprintf(",'%d HOURS'", $hour_offset);
  }
  $sql .= sprintf(") WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $db = NULL;
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_permit_expiry_time($id, $time) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("UPDATE permit_table SET ExpiryTime='%s' WHERE Id=%d",
		 $time, $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $db = NULL;
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_permit_retention($id, $retention_days) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("UPDATE permit_table SET RetentionTime=DATETIME(ExpiryTime,'%d DAYS') WHERE Id=%d",
		 $retention_days, $id);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $db = NULL;
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_permit_retention_at_expiry($id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("UPDATE permit_table SET RetentionTime=ExpiryTime WHERE Id=%d", $id);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $db = NULL;
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_permit_renew_days_used($id, $days, $offset = 0) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  if ($days > 0 and $offset == 0) {
    $sql = sprintf("UPDATE permit_table SET RenewUsed=%d WHERE Id=%d", $days, $id);
  } elseif ($days == 0 and $offset > 0) {
    $sql = sprintf("UPDATE permit_table SET RenewUsed=(RenewUsed + %d) WHERE Id=%d", $offset, $id);
  } else {
    $sql = sprintf("UPDATE permit_table SET RenewUsed=%d WHERE Id=%d", $days + $offset, $id);
  }
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $db = NULL;
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_permit_valet($id, $valet) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("UPDATE permit_table SET Valet=%d WHERE Id=%d",
		 $valet ? 1 : 0, $id);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_permit_overnight($id, $overnight) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("UPDATE permit_table SET Overnight=%d WHERE Id=%d",
		 $overnight ? 1 : 0, $id);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $db = NULL;
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_permit_shared($company_id, $property_id, $lot_id, $lpn, $shared_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("UPDATE permit_table SET SharedId='%s'", $shared_id);
  $sql .= sprintf(" WHERE CompanyId='%s' AND PropertyId='%s' AND LotId='%s' AND LPN='%s'",
		 $company_id, $property_id, $lot_id, $lpn);
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
function chdbperm_update_group_permit_shared($company_id, $group_id, $lpn, $shared_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("UPDATE permit_table SET SharedId='%s'", $shared_id);
  $sql .= sprintf(" WHERE CompanyId='%s' AND GroupId='%s' AND LPN='%s'",
		  $company_id, $group_id, $lpn);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_permit_shared_id($id, $shared_id) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "UPDATE permit_table SET SharedId=";
  if (NULL === $shared_id) {
    $sql .= "NULL";
  } else {
    $sql .= sprintf("'%s'", $shared_id);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_permit_cost($id, $cost) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("UPDATE permit_table SET Cost=%f WHERE Id=%d",
		 $cost, $id);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  $db = NULL;
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_permits_status($company_id, $client_id, $group_id, $activate, $suspended) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "UPDATE permit_table SET ";
  $sql .= sprintf("Active=%d,Suspended=%d",
		  $activate ? 1 : 0, $suspended ? 1 : 0);
  if ($activate) {
    $sql .= ",Deactivated=0,DeactivationTime=NULL,DeactivationUID=NULL,DeactivationReason=NULL";
  }
  $sql .= sprintf(" WHERE CompanyId='%s' AND ClientId='%s' AND GroupId='%s'",
		  $company_id, $client_id, $group_id);
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
function chdbperm_update_group_permits_expiry($company_id, $group_id, $expiry_time) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("UPDATE permit_table SET ExpiryTime='%s'", $expiry_time);
  $sql .= " WHERE Active=1 AND Suspended=0 AND Complementary=0";
  $sql .= sprintf(" AND CompanyId='%s' AND GroupId='%s'",
		  $company_id, $group_id);
  //drupal_set_message($sql);
  try {
    $r = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



function chdbperm_get_activation_deactivation_report($start_time, $stop_time,
						     $company_id, $property_id, $lot_id,
						     $lot_type, $lot_class,
						     $permit_class) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
      $lot_type !== 'ANY' or $lot_class !== 'ANY' or $permit_class !== 'ANY') {
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
  $sql = "SELECT permit.* FROM permit_table AS permit";
  if ($company_id === 'ANY' or $property_id === 'ANY' or $lot_id === 'ANY') {
    $sql .= " INNER JOIN props.user_table AS user";
    $sql .= "  ON (permit.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
    $sql .= "   AND (permit.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
    $sql .= "    AND (permit.LotId=user.LotId OR user.LotId='ANY')";
  }
  if ($lot_type !== 'ANY' or $lot_class !== 'ANY' or $permit_class !== 'ANY') {
    $sql .= " INNER JOIN props.policy_table AS policy";
    $sql .= "  ON (permit.PropertyId=policy.PropertyId)";
    $sql .= "   AND (permit.LotId=policy.LotId)";
    $sql .= " INNER JOIN props.profile_table AS profile";
    $sql .= "  ON (policy.ProfileId=profile.ProfileId)";
  }
  $precond = TRUE;
  if ($company_id !== 'ANY') {
    $sql .= sprintf(" %s permit.CompanyId='%s'", $precond ? 'WHERE' : 'AND', $company_id);
    $precond = FALSE;
  }
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" %s permit.PropertyId='%s'", $precond ? 'WHERE' : 'AND', $property_id);
    $precond = FALSE;
  }
  if ($lot_id !== 'ANY') {
    $sql .= sprintf(" %s permit.LotId='%s'", $precond ? 'WHERE' : 'AND', $lot_id);
    $precond = FALSE;
  }
  if ($lot_type !== 'ANY') {
    $sql .= sprintf(" %s ((permit.Long AND profile.LongTypeId='%s') OR (permit.Short AND profile.ShortTypeId='%s')",
    		    $precond ? 'WHERE' : 'AND', $lot_type, $lot_type);
    $precond = FALSE;
  }
  if ($lot_class !== 'ANY') {
    $sql .= sprintf(" %s ((permit.Long AND profile.LongClassId='%s') OR (permit.Short AND profile.ShortClassId='%s'))",
    		    $precond ? 'WHERE' : 'AND', $lot_class, $lot_class);
    $precond = FALSE;
  }
  if ($permit_class !== 'ANY') {
    $sql .= sprintf(" %s permit.PermitClass='%s'", $precond ? 'WHERE' : 'AND', $permit_class);
    $precond = FALSE;
  }
  $sql .= sprintf(" %s permit.ExpiryTime >= '%s' AND permit.BeginTime <= '%s'",
		  $precond ? 'WHERE' : 'AND', $start_time, $stop_time);
  //drupal_set_message($sql);
  //var_export($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_get_container_usage_report($start_time, $stop_time,
					     $company_id, $property_id, $lot_id,
					     $lot_type, $lot_class,
					     $permit_class) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
      $lot_type !== 'ANY' or $lot_class !== 'ANY' or $permit_class !== 'ANY') {
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
  $sql = "SELECT permit.* FROM permit_table AS permit";
  if ($company_id === 'ANY' or $property_id === 'ANY' or $lot_id === 'ANY') {
    $sql .= " INNER JOIN props.user_table AS user";
    $sql .= "  ON (permit.CompanyId=user.CompanyId OR user.CompanyId='ANY')";
    $sql .= "   AND (permit.PropertyId=user.PropertyId OR user.PropertyId='ANY')";
    $sql .= "    AND (permit.LotId=user.LotId OR user.LotId='ANY')";
  }
  if ($lot_type !== 'ANY' or $lot_class !== 'ANY' or $permit_class !== 'ANY') {
    $sql .= " INNER JOIN props.policy_table AS policy";
    $sql .= "  ON (permit.PropertyId=policy.PropertyId)";
    $sql .= "   AND (permit.LotId=policy.LotId)";
    $sql .= " INNER JOIN props.profile_table AS profile";
    $sql .= "  ON (policy.ProfileId=profile.ProfileId)";
  }
  $precond = TRUE;
  if ($company_id !== 'ANY') {
    $sql .= sprintf(" %s permit.CompanyId='%s'", $precond ? 'WHERE' : 'AND', $company_id);
    $precond = FALSE;
  }
  if ($property_id !== 'ANY') {
    $sql .= sprintf(" %s permit.PropertyId='%s'", $precond ? 'WHERE' : 'AND', $property_id);
    $precond = FALSE;
  }
  if ($lot_id !== 'ANY') {
    $sql .= sprintf(" %s permit.LotId='%s'", $precond ? 'WHERE' : 'AND', $lot_id);
    $precond = FALSE;
  }
  if ($lot_type !== 'ANY') {
    $sql .= sprintf(" %s ((permit.Long AND profile.LongTypeId='%s') OR (permit.Short AND profile.ShortTypeId='%s')",
    		    $precond ? 'WHERE' : 'AND', $lot_type, $lot_type);
    $precond = FALSE;
  }
  if ($lot_class !== 'ANY') {
    $sql .= sprintf(" %s ((permit.Long AND profile.LongClassId='%s') OR (permit.Short AND profile.ShortClassId='%s'))",
    		    $precond ? 'WHERE' : 'AND', $lot_class, $lot_class);
    $precond = FALSE;
  }
  if ($permit_class !== 'ANY') {
    $sql .= sprintf(" %s permit.PermitClass='%s'", $precond ? 'WHERE' : 'AND', $permit_class);
    $precond = FALSE;
  }
  $sql .= sprintf(" %s permit.ExpiryTime >= '%s' AND permit.BeginTime <= '%s'",
		  $precond ? 'WHERE' : 'AND', $start_time, $stop_time);
  $sql .= " AND LENGTH(ContainerId) > 0 AND LENGTH(TypeId) > 0";
  $sql .= " ORDER BY GroupId,MemberId";
  //drupal_set_message($sql);
  //print($sql.'<p>');
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_get_permits_report($start_time, $stop_time,
				     $company_id, $property_id,
				     $filter_lot = FALSE, $lot_id = '',
				     $filter_active = FALSE, $active = TRUE,
				     $filter_class = FALSE, $class = 'Regular',
				     $filter_container = FALSE, $container_id = '',
				     $filter_type = FALSE, $type_id = '') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  // Enforcer TBD
  $sql = "SELECT * FROM permit_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s'",
		  $company_id, $property_id);
  if ($filter_lot) {
    $sql .= sprintf(" AND LotId='%s'", $lot_id);
  }
  if ($filter_active) {
    $sql .= sprintf(" AND Active=%d", $active ? 1 : 0);
  }
  if ($filter_class) {
    $sql .= sprintf(" AND PermitClass='%s'", $class);
  }
  if ($filter_container) {
    $sql .= sprintf(" AND ContainerId='%s'", $container_id);
  }
  if ($filter_type) {
    $sql .= sprintf(" AND TypeId='%s'", $type_id);
  }
  $sql .= sprintf(" AND CreateTime > '%s' AND CreateTime < '%s'",
		  $start_time, $stop_time);
  //drupal_set_message($sql);
  //print($sql.'<p>');
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



function chdbperm_get_master_history($max_entries = 0) {
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
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry['DB'];
  }
  return $entries;
}



function chdbperm_get_history_permits($start_time, $stop_time,
				      $company_id, $property_id,
				      $filter_lot = FALSE, $lot_id = '',
				      $filter_class = FALSE, $class = 'Regular',
				      $filter_container = FALSE, $container_id = '',
				      $filter_type = FALSE, $type_id = '') {
  $entries = array();
  $files = chdbperm_get_master_history();
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
    // Enforcer TBD
    $sql = "SELECT * FROM permit_table WHERE ";
    $sql .= sprintf("CompanyId='%s' AND PropertyId='%s'",
		    $company_id, $property_id);
    if ($filter_lot) {
      $sql .= sprintf(" AND LotId='%s'", $lot_id);
    }
    if ($filter_class) {
      $sql .= sprintf(" AND PermitClass='%s'", $class);
    }
    if ($filter_container) {
      $sql .= sprintf(" AND ContainerId='%s'", $container_id);
    }
    if ($filter_type) {
      $sql .= sprintf(" AND TypeId='%s'", $type_id);
    }
    $sql .= sprintf(" AND CreateTime > '%s' AND CreateTime < '%s'",
		    $start_time, $stop_time);
    //drupal_set_message($sql);
    foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
      $entries[] = $entry;
    }
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbperm_retire_permit_id($id, $retention_days = 0) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "UPDATE permit_table SET";
  $sql .= " Active=0";
  $sql .= ",Deactivated=1";
  $sql .= ",DeactivationTime=CURRENT_TIMESTAMP";
  if ($retention_days > 0) {
    $sql .= sprintf(",RetentionTime=DATETIME(CURRENT_TIMESTAMP,'%d DAYS')",
		    $retention_days);
  }
  $sql .= sprintf(" WHERE Id=%d", $id);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}


// Return 0 on success, otherwise 1 
function chdbperm_retire_permits($uid, $company_id, $group_id, $member_id,
				 $reason, $retention_days = 365) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "UPDATE permit_table SET Active=0,Deactivated=1,DeactivationTime=CURRENT_TIMESTAMP";
  $sql .= sprintf(",DeactivationUID=%d,DeactivationReason='%s'",
		  $uid, $reason);
  if ($retention_days > 0) {
    $sql .= sprintf(",RetentionTime=DATETIME(CURRENT_TIMESTAMP,'%d DAYS')",
		    $retention_days);
  }
  $sql .= sprintf(" WHERE Active=1 AND CompanyId='%s'", $company_id);
  if (NULL !== $group_id) {
    $sql .= sprintf(" AND GroupId='%s'", $group_id);
  }
  if (NULL !== $member_id) {
    $sql .= sprintf(" AND MemberId='%s'", $member_id);
  }
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    return 1;
  }
  return 0;
}



// Used by CRON
// Return 0 on success, otherwise 1 
function chdbperm_cron_retire_permits($time) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
    return 1;
  }
  // connect to SQLite database
  try {
    $db = new PDO("sqlite:".$db_name);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }

  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_STATS_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for STATS database!!!", 'error');
    return 1;
  }
  // SQL
  $sql = sprintf("ATTACH '%s' AS stats", $db_name);
  //drupal_set_message($sql);
  $db->exec($sql);

  // SQL
  $sql = "INSERT INTO stats.permitstats_table (CompanyId,PropertyId,LotId,GroupId,MemberId,Remove,LPN)";
  $sql .= " SELECT CompanyId,PropertyId,LotId,GroupId,MemberId,1,LPN FROM permit_table";
  $sql .= sprintf(" WHERE Active=1 AND ExpiryTime < '%s'", $time);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }

  // SQL
  $sql = sprintf("UPDATE permit_table SET Active=0 WHERE Active=1 AND ExpiryTime < '%s'", $time);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



// Used by CRON
// Return 0 on success, otherwise 1 
function chdbperm_cron_purge_history($time) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_HISTORY_DB');
  if ($db_name == FALSE) {
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
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
    return 1;
  }
  // SQL
  $sql = sprintf("ATTACH '%s' AS permits", $db_name);
  //drupal_set_message($sql);
  $db->exec($sql);

  // SQL
  $sql = "INSERT INTO permit_table";
  $sql .= " SELECT * FROM permits.permit_table";
  $sql .= sprintf(" WHERE Active=0 AND RetentionTime < '%s'", $time);
  // PY : May 17.14
  drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



// Used by CRON
// Return 0 on success, otherwise 1
function chdbperm_cron_delete_permits($time) {
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("DELETE FROM permit_table WHERE Active=0 AND RetentionTime < '%s'", $time);
  //drupal_set_message($sql);
  try {
    $res = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   permitstats_table   **********************/



// Return 0 on success, otherwise 1 
function chdbperm_stats_new($company_id, $property_id, $lot_id,
			    $group_id, $member_id, $lpn) {
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
  $sql = "INSERT INTO permitstats_table ";
  $sql .= "(CompanyId,PropertyId,LotId,GroupId,MemberId,New,LPN) VALUES (";
  $sql .= sprintf("'%s','%s','%s',", $company_id, $property_id, $lot_id);
  if (NULL === $group_id) {
    $sql .= "NULL,";
  } else {
    $sql .= sprintf("'%s',", $group_id);
  }
  if (NULL === $member_id) {
    $sql .= "NULL,";
  } else {
    $sql .= sprintf("'%s',", $member_id);
  }
  $sql .= sprintf("1,'%s')", $lpn);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_stats_modify($company_id, $property_id, $lot_id,
			       $group_id, $member_id, $lpn) {
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
  $sql = "INSERT INTO permitstats_table ";
  $sql .= "(CompanyId,PropertyId,LotId,GroupId,MemberId,Modify,LPN) VALUES (";
  $sql .= sprintf("'%s','%s','%s',", $company_id, $property_id, $lot_id);
  if (NULL === $group_id) {
    $sql .= "NULL,";
  } else {
    $sql .= sprintf("'%s',", $group_id);
  }
  if (NULL === $member_id) {
    $sql .= "NULL,";
  } else {
    $sql .= sprintf("'%s',", $member_id);
  }
  $sql .= sprintf("1,'%s')", $lpn);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_stats_delete($company_id, $property_id, $lot_id,
			       $group_id, $member_id, $lpn) {
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
  // Query
  $sql = "INSERT INTO permitstats_table ";
  $sql .= "(CompanyId,PropertyId,LotId,GroupId,MemberId,Remove,LPN) VALUES (";
  $sql .= sprintf("'%s','%s','%s',", $company_id, $property_id, $lot_id);
  if (NULL === $group_id) {
    $sql .= "NULL,";
  } else {
    $sql .= sprintf("'%s',", $group_id);
  }
  if (NULL === $member_id) {
    $sql .= "NULL,";
  } else {
    $sql .= sprintf("'%s',", $member_id);
  }
  $sql .= sprintf("1,'%s')", $lpn);
  //drupal_set_message($sql);
  if (!$db->exec($sql)) {
    drupal_set_message("Not able to write to DB", 'error');
    return 1;
  }
  return 0;
}



/****************   permittag_table Table   **********************/



function chdbperm_get_permittag_property($company_id, $property_id, $lot_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM permittag_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s' AND LotId='%s'",
		  $company_id, $property_id, $lot_id);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



function chdbperm_get_permittag_group($company_id, $group_id) {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "SELECT * FROM permittag_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND GroupId='%s'",
		  $company_id, $group_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbperm_insert_permittag_property($company_id, $property_id, $lot_id,
					    $permit_tag) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "INSERT INTO permittag_table (CompanyId,PropertyId,LotId,PermitTag) ";
  $sql .= sprintf("VALUES ('%s','%s','%s','%s')",
		  $company_id, $property_id, $lot_id, $permit_tag);
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
function chdbperm_insert_permittag_group($company_id, $group_id,
					 $permit_tag) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = "INSERT INTO permittag_table (CompanyId,GroupId,PermitTag) ";
  $sql .= sprintf("VALUES ('%s','%s','%s')",
		  $company_id, $group_id, $permit_tag);
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
function chdbperm_update_permittag_property($company_id, $property_id, $lot_id,
					    $permit_tag) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("UPDATE permittag_table SET PermitTag='%s' WHERE ",
		 $permit_tag);
  $sql .= sprintf("CompanyId='%s' AND PropertyId='%s' AND LotId='%s'",
		  $company_id, $property_id, $lot_id);
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
function chdbperm_update_permittag_group($company_id, $group_id,
					 $permit_tag) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  $sql = sprintf("UPDATE permittag_table SET PermitTag='%s' WHERE ",
		 $permit_tag);
  $sql .= sprintf("CompanyId='%s' AND GroupId='%s'",
		  $company_id, $group_id);
  //drupal_set_message($sql);
  try {
    $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



/****************   prediction_table Table   **********************/



function chdbperm_get_predictions($company_id, $property_id,
				  $filter_lot = FALSE, $lot_id = '') {
  $entries = array();
  // Find location of SQLite database
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
function chdbperm_insert_prediction($company_id, $property_id, $lot_id) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
  try {
    $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  return 0;
}



// Return 0 on success, otherwise 1 
function chdbperm_update_prediction_daily($company_id, $property_id,
					  $lot_id, $today, $tomorrow) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
function chdbperm_update_prediction_weekly($company_id, $property_id,
					   $lot_id, $next_week) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB'); //print_r($db_name);
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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
function chdbperm_update_prediction_monthly($company_id, $property_id,
					    $lot_id, $next_month) {
  $db_name = getenv('APPS_LPMS_PERMIT_DB');
  if ($db_name == FALSE) {
    drupal_set_message("Missing HTTP environment for PERMIT database!!!", 'error');
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



/****************   validationcounts_table Table   **********************/



// $datetimes is an array
function chdbperm_get_validationcounts($company_id, $client_id, $group_id,
				       $datetimes) {
  $entries = array();
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
    return $entries;
  }
  // Perform DB command
  $sql = "SELECT * FROM validationcounts_table WHERE ";
  $sql .= sprintf("CompanyId='%s' AND ClientId='%s' AND GroupId='%s'",
		  $company_id, $client_id, $group_id);
  $sql .= " AND (";
  foreach ($datetimes as $datetime) {
    $sql .= sprintf("DateTime='%s' OR ", $datetime);
  }
  $sql = rtrim($sql, " OR ");
  $sql .= ")";
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



// Return 0 on success, otherwise 1 
function chdbperm_validationcounts_new($uid, $timezone, $company_id, $client_id, $group_id) {
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
  $sql = "UPDATE validationcounts_table SET Count=Count+1 WHERE ";
  $sql .= sprintf("CompanyId='%s' AND ClientId='%s' AND GroupId='%s'",
		  $company_id, $client_id, $group_id);
  $sql .= " AND DateTime=STRFTIME('%Y-%m','NOW',";
  $sql .= sprintf("'%d SECONDS')", $timezone);
  //drupal_set_message($sql);
  $changes = 0;
  try {
    $changes = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  if (0 == $changes) {
    $sql = "INSERT INTO validationcounts_table (UID,CompanyId,ClientId,GroupId, ";
    $sql .= "DateTime,Count) ";
    $sql .= sprintf("VALUES (%d,'%s','%s','%s',",
		    $uid, $company_id, $client_id, $group_id);
    $sql .= "STRFTIME('%Y-%m','NOW',";
    $sql .= sprintf("'%d SECONDS'),1)", $timezone);
    //drupal_set_message($sql);
    try {
      $db->exec($sql);
    } catch(PDOException $e) {
      drupal_set_message($e->getMessage(), 'error');
      return 1;
    }
  }
  // SQL
  $sql = "UPDATE validationcounts_table SET Count=Count+1 WHERE ";
  $sql .= sprintf("CompanyId='%s' AND ClientId='%s' AND GroupId='%s'",
		  $company_id, $client_id, $group_id);
  $sql .= " AND DateTime=STRFTIME('%Y-%m-%d','NOW',";
  $sql .= sprintf("'%d SECONDS')", $timezone);
  //drupal_set_message($sql);
  $changes = 0;
  try {
    $changes = $db->exec($sql);
  } catch(PDOException $e) {
    drupal_set_message($e->getMessage(), 'error');
    return 1;
  }
  if (0 == $changes) {
    $sql = "INSERT INTO validationcounts_table (UID,CompanyId,ClientId,GroupId, ";
    $sql .= "DateTime,Count) ";
    $sql .= sprintf("VALUES (%d,'%s','%s','%s',",
		    $uid, $company_id, $client_id, $group_id);
    $sql .= "STRFTIME('%Y-%m-%d','NOW',";
    $sql .= sprintf("'%d SECONDS'),1)", $timezone);
    //drupal_set_message($sql);
    try {
      $db->exec($sql);
    } catch(PDOException $e) {
      drupal_set_message($e->getMessage(), 'error');
      return 1;
    }
  }
  return 0;
}



