<?php
// $Id$

function anai_chpusers_format_account_profile($account) {
  $header = array();
  $rows[] = array(t('username'), $account->name);
  $rows[] = array(t('Full name'), $account->profile_name);
  $rows[] = array(t('Phone'), $account->profile_phone_number);
  $rows[] = array(t('Email'), $account->email);
  $rows[] = array(t('Street1'), $account->profile_street1);
  $rows[] = array(t('Street2'), $account->profile_street2);
  $rows[] = array(t('City'), $account->profile_city);
  $rows[] = array(t('Postal code'), $account->profile_postal_code);
  $rows[] = array(t('State'), $account->profile_state);
  $rows[] = array(t('Country'), $account->profile_country);
  $rows[] = array(t('Lat'), $account->profile_lat);
  $rows[] = array(t('Lon'), $account->profile_lon);
  $rows[] = array(t('LatLonOK'), $account->profile_latlon_ok);
  return theme_table($header, $rows);
}



function chpusers_retrieve_siblings($role) {
  $entries = array();
  switch ($role) {
  case 'Property owner':
    $entries[] = 'Enforcer';
    break;
  case 'Enforcer':
    $entries[] = 'Patroller';
    $entries[] = 'Security';
    break;
  }
  return $entries;
}



function chpusers_retrieve_global_users($role) {
  $entries = array();
  $q = db_query("SELECT uid FROM {users}");
  while ($r = db_fetch_array($q)) {
    $account = user_load(array('uid' => $r['uid'])); //print_r($account);
    if (in_array($role, array_values($account->roles)) and $account->status) {
      $affiliates = array();
      $profile_tmp = explode("\n", $account->profile_affiliates);
      foreach ($profile_tmp as $k => $v) {
	$v_trimmed = trim($v);
	if (strlen($v_trimmed) > 0) {
	  $affiliates[$v_trimmed] = $v_trimmed;
	}
      }
      $properties = array();
      $profile_tmp = explode("\n", $account->profile_properties);
      foreach ($profile_tmp as $k => $v) {
	$v_trimmed = trim($v);
	if (strlen($v_trimmed) > 0) {
	  $properties[$v_trimmed] = $v_trimmed;
	}
      }
      $entries[] = array('uid' => $account->uid,
			 'role' => $role,
			 'username' => $account->name,
			 'fullname' => $account->profile_name,
			 'city' => $account->profile_city,
			 'affiliates' => $affiliates,
			 'properties' => $properties);
    }
  }
  return $entries;
}



function chpusers_filter_global_users($master_uid, $users) {
  foreach ($users as $i => $user) {
    $users[$i]['affiliated'] = FALSE;
    foreach ($user['affiliates'] as $affiliate) {
      if (!strcmp($master_uid, $affiliate)) {
	$users[$i]['affiliated'] = TRUE;
	break;
      }
    }
  }
  return $users;
}



function chpuser_clear_affiliates($uid) {
  $account = user_load(array('uid' => $uid));
  user_save($account, array('profile_affiliates' => ''));
}



function chpuser_remove_affiliate($uid, $master_uid) {
  //drupal_set_message('Removing master:'.$master_uid.' for uid:'.$uid);
  $affiliates = array();
  $account = user_load(array('uid' => $uid));
  $profile_tmp = explode("\n", $account->profile_affiliates);
  foreach ($profile_tmp as $k => $v) {
    $v_trimmed = trim($v);
    if (strlen($v_trimmed) > 0) {
      if (strcmp($v_trimmed, $master_uid)) {
	$affiliates[$v_trimmed] = $v_trimmed;
      }
    }
  }
  $field = implode("\n", $affiliates);
  user_save($account, array('profile_affiliates' => $field));
}



function chpuser_append_affiliate($uid, $master_uid) {
  //drupal_set_message('Appending master:'.$master_uid.' for uid:'.$uid);
  $affiliates = array();
  $account = user_load(array('uid' => $uid));
  $profile_tmp = explode("\n", $account->profile_affiliates);
  foreach ($profile_tmp as $k => $v) {
    $v_trimmed = trim($v);
    if (strlen($v_trimmed) > 0) {
      $affiliates[$v_trimmed] = $v_trimmed;
    }
  }
  $affiliates[$master_uid] = $master_uid;
  $field = implode("\n", $affiliates);
  user_save($account, array('profile_affiliates' => $field));
}



function chpusers_retrieve_affiliates($master_uid) {
  $affiliates = array();
  $global_users = array();
  $account = user_load(array('uid' => $master_uid));
  foreach ($account->roles as $role) {
    $siblings = chpusers_retrieve_siblings($role);
    foreach ($siblings as $sibling) {
      $global_users = chpusers_retrieve_global_users($sibling);
      $global_users = chpusers_filter_global_users($master_uid, $global_users);
      foreach ($global_users as $global_user) {
	if ($global_user['affiliated']) {
	  $affiliates[] = $global_user;
	}
      }
    }
  }
  return $affiliates;
}



function chpuser_retrieve_parents($uid) {
  $parents = array();
  $account = user_load(array('uid' => $uid));
  $profile_tmp = explode("\n", $account->profile_affiliates);
  foreach ($profile_tmp as $k => $v) {
    $v_trimmed = trim($v);
    if (strlen($v_trimmed) > 0) {
      $parent = user_load(array('uid' => $v_trimmed));
      $parents[] = array('uid' => $v_trimmed,
			 'name' => $parent->name,
			 'roles' => $parent->roles);
    }
  }
  return $parents;
}



function chpuser_datetime_utc_to_usertimezone($timestamp, $timezone = NULL) {
  $timeobj = new DateTime($timestamp, new DateTimeZone('UTC'));
  if ($timezone == NULL) {
    global $user;
    if ($user->timezone > 0) {
      $timeobj->add(new DateInterval('PT'.$user->timezone.'S'));
    } else {
      $timeobj->sub(new DateInterval('PT'.abs($user->timezone).'S'));
    }
  } else {
    if ($timezone > 0) {
      $timeobj->add(new DateInterval('PT'.$timezone.'S'));
    } else {
      $timeobj->sub(new DateInterval('PT'.abs($timezone).'S'));
    }
  }
  //$now = new DateTime();
  //if ($now->format("I")) {
  //  $timeobj->add(new DateInterval('PT3600S'));
  //}
  return $timeobj;
}



function chpuser_datetime_usertimezone_to_utc($timestamp) {
  global $user;
  $now = new DateTime();
  $daylight = 0;
  if ($now->format("I")) {
    $daylight = 1;
  } //drupal_set_message('Daylight:'.$daylight);
  $timezonename = timezone_name_from_abbr("", $user->timezone, $daylight);
  $timeobj = new DateTime($timestamp, new DateTimeZone($timezonename));
  $timeobj->setTimezone(new DateTimeZone('UTC'));
  return $timeobj;
}



function chpuser_datetime_utc_to_utc($timestamp) {
  $timeobj = new DateTime($timestamp, new DateTimeZone('UTC'));
  return $timeobj;
}



function chpuser_datetime_machine_to_utc($timestamp) {
  $timeobj = new DateTime($timestamp);
  $timeobj->setTimezone(new DateTimeZone('UTC'));
  return $timeobj;
}



define('RESIDENTIAL_DISPLAY', 'Tenant');
define('VISITOR_DISPLAY', 'Visitor');



function chpuser_residential_alias() {
  return RESIDENTIAL_DISPLAY;
}



function chpuser_visitor_alias() {
  return VISITOR_DISPLAY;
}



