<?php



/****************   loopscount_table  **********************/



function chdbstats_get_loopscount($property_id) {
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
  // Perform DB command
  $sql = "SELECT * FROM loopscount_table";
  $sql .= sprintf(" WHERE PropertyId='%s'", $property_id);
  //drupal_set_message($sql);
  foreach ($db->query($sql, PDO::FETCH_ASSOC) as $entry) {
    $entries[] = $entry;
  }
  return $entries;
}



