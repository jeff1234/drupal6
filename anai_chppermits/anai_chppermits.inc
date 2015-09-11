<?php
include_once drupal_get_path('module', 'anai_chppermits') .'/anai_chppermits_db.inc';


function chpperm_registration_name($tag, $full = TRUE) {
  switch ($tag) {
  case 'Transient':
    if ($full) {
      return t('Transient permit');
    } else {
      return t('Transient');
    }
  case 'Group':
    if ($full) {
      return t('Group permit');
    } else {
      return t('Group');
    }
  case 'Member':
    if ($full) {
      return t('Individual permit');
    } else {
      return t('Individual');
    }
  case 'Hotel':
    if ($full) {
      return t('Hotel permit');
    } else {
      return t('Hotel');
    }
  case 'Validation':
    if ($full) {
      return t('Validation permit');
    } else {
      return t('Validation');
    }
  case 'Dock':
    if ($full) {
      return t('Loading dock permit');
    } else {
      return t('Dock');
    }
  }
  return t('Unknown');
}

function chpperm_structure_name($tag) {
  switch ($tag) {
  case 'Linear':
    return t('Linear rate');
  case 'Flat':
    return t('Flat rate');
  case 'Fixed':
    return t('Fixed period');
  }
  return t('Unknown');
}

function chpperm_payment_arrangement_name($tag) {
  switch ($tag) {
  case 'Landlord':
    return t('Paid to landlord');
  case 'GroupPay':
    return t('Bulk payment');
  case 'IndividualPay':
    return t('Individual payments');
  }
  return t('Unknown');
}

function chpperm_billing_method_name($tag) {
  switch ($tag) {
  case 'Invoice':
    return t('Invoice');
  case 'CreditCard':
    return t('Credit Card');
  case 'PAC':
    return t('Pre-authorized cheque');
  }
  return t('Unknown');
}

function chpperm_interval_name($tag) {
  switch ($tag) {
  case 'Hourly':
    return t('Hourly');
  case 'Daily':
    return t('Daily');
  case 'Weekly':
    return t('Weekly');
  case 'Monthly':
    return t('Monthly');
  case 'Quarterly':
    return t('Quarterly');
  case 'SemiAnnually':
    return t('Semi-Annually');
  case 'Annually':
    return t('Annually');
  }
  return t('Unknown');
}

function chpperm_validation_lengths() {
  $entries = array();
  $entries['Hour1'] = array('modifiers' => array('+1 hour'),
			    'name' => t('One hour'));
  $entries['Hour2'] = array('modifiers' => array('+2 hours'),
			    'name' => t('Two hours'));
  $entries['Hour3'] = array('modifiers' => array('+3 hours'),
			    'name' => t('Three hours'));
  $entries['Hour4'] = array('modifiers' => array('+4 hours'),
			    'name' => t('Four hours'));
  $entries['Hour5'] = array('modifiers' => array('+5 hours'),
			    'name' => t('Five hours'));
  $entries['Hour6'] = array('modifiers' => array('+6 hours'),
			    'name' => t('Six hours'));
  $entries['Day1'] = array('modifiers' => array('+1 day'),
			   'name' => t('One day'));
  $entries['Day2'] = array('modifiers' => array('+2 days'),
			   'name' => t('Two days'));
  $entries['Day3'] = array('modifiers' => array('+3 days'),
			   'name' => t('Three days'));
  $entries['Day4'] = array('modifiers' => array('+4 days'),
			   'name' => t('Four days'));
  return $entries;
}

function chpperm_validation_length_name($tag, &$found) {
  $lengths = chpperm_validation_lengths();
  if (isset($lengths[$tag])) {
    $found = TRUE;
    return $lengths[$tag]['name'];
  } else {
    $found = FALSE;
    return t('Unknown');
  }
}

function chpperm_validation_benefits() {
  $entries = array();
  $entries['Free'] = array('name' => t('Free'));
  $entries['Off5'] = array('name' => t('Discount 5%'));
  $entries['Off10'] = array('name' => t('Discount 10%'));
  $entries['Off15'] = array('name' => t('Discount 15%'));
  $entries['Off20'] = array('name' => t('Discount 20%'));
  $entries['Off25'] = array('name' => t('Discount 25%'));
  $entries['Off50'] = array('name' => t('Discount 50%'));
  $entries['Off75'] = array('name' => t('Discount 75%'));
  return $entries;
}

function chpperm_validation_benefit_name($tag, &$found) {
  $benefits = chpperm_validation_benefits();
  if (isset($benefits[$tag])) {
    $found = TRUE;
    return $benefits[$tag]['name'];
  } else {
    $found = FALSE;
    return t('Unknown');
  }
}

function chpperm_validation_missions() {
  $entries = array();
  $entries['Meeting'] = array('name' => t('Meeting'));
  $entries['Conference'] = array('name' => t('Conference'));
  $entries['SiteVisit'] = array('name' => t('Site visit'));
  $entries['Appointment'] = array('name' => t('Appointment'));
  $entries['Restaurant'] = array('name' => t('Restaurant'));
  return $entries;
}

function chpperm_validation_mission_name($tag, &$found) {
  $missions = chpperm_validation_missions();
  if (isset($missions[$tag])) {
    $found = TRUE;
    return $missions[$tag]['name'];
  } else {
    $found = FALSE;
    return t('Unknown');
  }
}

function chpperm_calculate_cost($company_id,
				$expiry, $offset, $profile, $overnight,
				$container_used = FALSE,
				$container_id = NULL, $type_id = NULL) {
  $num_years = 0;
  $num_months = 0;
  $num_days = 1;
  $num_hours = 0;
  $num_mins = 0;
  $total_days = $num_days;

  // Determine lenght of permit
  if (!empty($expiry)) {
    $datetime1 = chpuser_datetime_usertimezone_to_utc($expiry);
    if (0 < $offset) {
      $datetime1->modify(sprintf('+%d days', $offset));
    }
    $datetime2 = chpuser_datetime_usertimezone_to_utc('now');
    $interval = $datetime2->diff($datetime1);
    if (!$interval->invert) {
      $num_years = $interval->y;
      $num_months = $interval->m;
      $num_days = $interval->d;
      $num_hours = $interval->h;
      $num_mins = $interval->i;
      $total_days = $interval->days;
    }
  } elseif ($offset > 0) {
    $num_days = $offset;
    $total_days = $num_days;
  }

  // Determine cost of permit
  $cost = 0;

  // Without a container, it must be a regular permit
  if (!$container_used) {
    $cost += $total_days * $profile['RegularCost'];
    if ($overnight) {
      $cost += $total_days * $profile['OvernightCost'];
    }
  }

  // Handle container rate structures
  if ($container_used) {
    $conttype = chdbperm_get_conttype($company_id, $container_id, $type_id);
    //drupal_set_message('<pre>'.var_export($conttype, TRUE).'</pre>');

    if ($conttype['RateType'] === 'Fixed') {
      $cost = $conttype['UnitValue'];
      // An override of the set period since this is fixed period
      $num_years = 0;
      $num_months = 0;
      $num_days = 0;
      $num_hours = 0;
      $num_mins = 0;
      $total_days = $num_days;

      if ($conttype['ValueUnit'] === 'Minute') {
	$num_mins = $conttype['FixedPeriod'];
      } elseif ($conttype['ValueUnit'] === 'Hour') {
	$num_hours = $conttype['FixedPeriod'];
      } elseif ($conttype['ValueUnit'] === 'Day') {
	$num_days = $conttype['FixedPeriod'];
      } elseif ($conttype['ValueUnit'] === 'Week') {
	$num_days = 7 * $conttype['FixedPeriod'];
      } elseif ($conttype['ValueUnit'] === 'Month') {
	$num_months = $conttype['FixedPeriod'];
      }
    }

    if ($conttype['RateType'] === 'Flat') {
      $cost = $conttype['UnitValue'];
    }

    if ($conttype['RateType'] === 'Linear') {
      if ($conttype['ValueUnit'] === 'Minute') {
	// Cost for main unit
	$cost = $num_mins * $conttype['UnitValue'];
	// Cost for super-unit
	if ($cost < $conttype['MaxCost']) {
	  $tmp = $num_hours * 60;
	  $cost += $tmp * $conttype['UnitValue'];
	}
	if ($cost < $conttype['MaxCost']) {
	  $tmp = $num_days * 24 * 60;
	  $cost += $tmp * $conttype['UnitValue'];
	}
	if ($cost < $conttype['MaxCost']) {
	  $tmp = $num_months * 24 * 30 * 60;
	  $cost += $tmp * $conttype['UnitValue'];
	}
	if ($cost < $conttype['MaxCost']) {
	  $tmp = $num_years * 24 * 30 * 365 * 60;
	  $cost += $tmp * $conttype['UnitValue'];
	}
      } elseif ($conttype['ValueUnit'] === 'Hour') {
	// Cost for main unit
	$cost = $num_hours * $conttype['UnitValue'];
	// Cost for started sub-unit
	if ($num_mins > 0) {
	  $cost += $conttype['UnitValue'];
	}
	// Cost for super-unit
	if ($cost < $conttype['MaxCost']) {
	  $tmp = $num_days * 24;
	  $cost += $tmp * $conttype['UnitValue'];
	}
	if ($cost < $conttype['MaxCost']) {
	  $tmp = $num_months * 24 * 30;
	  $cost += $tmp * $conttype['UnitValue'];
	}
	if ($cost < $conttype['MaxCost']) {
	  $tmp = $num_years * 24 * 30 * 365;
	  $cost += $tmp * $conttype['UnitValue'];
	}
      } elseif ($conttype['ValueUnit'] === 'Day') {
	// Cost for main unit
	$cost = $num_days * $conttype['UnitValue'];
	// Cost for started sub-unit
	if ($num_hours > 0 or $num_mins > 0) {
	  $cost += $conttype['UnitValue'];
	}
	// Cost for super-unit
	if ($cost < $conttype['MaxCost']) {
	  $tmp = $num_months * 30;
	  $cost += $tmp * $conttype['UnitValue'];
	}
	if ($cost < $conttype['MaxCost']) {
	  $tmp = $num_years * 30 * 365;
	  $cost += $tmp * $conttype['UnitValue'];
	}
      } elseif ($conttype['ValueUnit'] === 'Week') {
	// Cost for main unit
	$cost = intval($num_days / 7) * $conttype['UnitValue'];
	// Cost for started sub-unit
	if ($num_days % 7 > 0 or
	    ($num_days % 7 == 0 and ($num_hours > 0 or $num_mins > 0))) {
	  $cost += $conttype['UnitValue'];
	}
	// Cost for super-unit
	if ($cost < $conttype['MaxCost']) {
	  $tmp = $num_months * 4.2;
	  $cost += $tmp * $conttype['UnitValue'];
	}
	if ($cost < $conttype['MaxCost']) {
	  $tmp = $num_years * 52;
	  $cost += $tmp * $conttype['UnitValue'];
	}
      } elseif ($conttype['ValueUnit'] === 'Month') {
	// Cost for main unit
	$cost = $num_months * $conttype['UnitValue'];
	// Cost for started sub-unit
	if ($num_days > 0 or $num_hours > 0 or $num_mins > 0) {
	  $cost += $conttype['UnitValue'];
	}
	// Cost for super-unit
	if ($cost < $conttype['MaxCost']) {
	  $tmp = $num_years * 12;
	  $cost += $tmp * $conttype['UnitValue'];
	}
      }
      if ($cost > $conttype['MaxCost']) {
	$cost = $conttype['MaxCost'];
      }
    }
  }

  $delimiter = '';
  $info = '';
  if ($num_years > 1) {
    $info .= $delimiter;
    $info .= t('NUM years', array('NUM' => $num_years));
    $delimiter = ', ';
  } elseif ($num_years > 0) {
    $info .= $delimiter;
    $info .= t('NUM year', array('NUM' => $num_years));
    $delimiter = ', ';
  }
  if ($num_months > 1) {
    $info .= $delimiter;
    $info .= t('NUM months', array('NUM' => $num_months));
    $delimiter = ', ';
  } elseif ($num_months > 0) {
    $info .= $delimiter;
    $info .= t('NUM month', array('NUM' => $num_months));
    $delimiter = ', ';
  }
  if ($num_days > 1) {
    $info .= $delimiter;
    $info .= t('NUM days', array('NUM' => $num_days));
    $delimiter = ', ';
  } elseif ($num_days > 0) {
    $info .= $delimiter;
    $info .= t('NUM day', array('NUM' => $num_days));
    $delimiter = ', ';
  }
  if ($num_hours > 1) {
    $info .= $delimiter;
    $info .= t('NUM hours', array('NUM' => $num_hours));
    $delimiter = ', ';
  } elseif ($num_hours > 0) {
    $info .= $delimiter;
    $info .= t('NUM hour', array('NUM' => $num_hours));
    $delimiter = ', ';
  }
  if ($num_mins > 1) {
    $info .= $delimiter;
    $info .= t('NUM minutes', array('NUM' => $num_mins));
    $delimiter = ', ';
  } elseif ($num_mins > 0) {
    $info .= $delimiter;
    $info .= t('NUM minute', array('NUM' => $num_mins));
    $delimiter = ', ';
  }

  return serialize(array('Cost' => sprintf('%.02f', $cost), 'Info' => $info));
}



// Retrieve containers based on relations
function chpperm_retrieve_containers($uid, $company_id,
				     $property_id = 'ANY',
				     $lot_id = 'ANY') {
  $account = user_load(array('uid' => $uid));
  $containers = array();

  // As Admin, get all properties in a relation
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbperm_cc_get_admin_containers($company_id, $property_id, $lot_id) as $container) {
      $containers[$container['ContainerId']] =
	array('data' => $container,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $containers;
  }

  // Via relations
  foreach (chdbperm_cc_get_containers($uid, $company_id, $property_id, $lot_id) as $container) {
    $containers[$container['ContainerId']] =
      array('data' => $container,
	    'readonly' => TRUE,
	    'source' => 'Relation');
  }

  return $containers;
}



// Retrieve container types based on relations
function chpperm_retrieve_container_types($uid, $company_id, $container_id, $property_id = 'ANY') {
  $account = user_load(array('uid' => $uid));
  $containers = array();

  // As Admin, get all properties in a relation
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbperm_cc_get_admin_container_types($company_id, $container_id, $property_id) as $container) {
      $containers[$container['TypeId']] =
	array('data' => $container,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $containers;
  }

  // Via relations
  foreach (chdbperm_cc_get_container_types($uid, $company_id, $container_id, $property_id) as $container) {
    $containers[$container['TypeId']] =
      array('data' => $container,
	    'readonly' => TRUE,
	    'source' => 'Relation');
  }

  return $containers;
}



// Retrieve container types based on relations
function chpperm_retrieve_container_props($uid, $company_id, $container_id) {
  $account = user_load(array('uid' => $uid));
  $containers = array();

  // As Admin, get all properties in a relation
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbperm_cc_get_admin_container_props($company_id, $container_id) as $container) {
      $containers[$container['PropertyId']][$container['LotId']] =
	array('data' => $container,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $containers;
  }

  // Via relations
  foreach (chdbperm_cc_get_container_props($uid, $company_id, $container_id) as $container) {
    $containers[$container['PropertyId']][$container['LotId']] =
      array('data' => $container,
	    'readonly' => TRUE,
	    'source' => 'Relation');
  }

  return $containers;
}



// Retrieve groups based on relations
function chpperm_retrieve_groups($uid, $company_id,
				 $property_id = 'ANY',
				 $filter_active = TRUE, $active = TRUE,
				 $filter_suspended = TRUE, $suspended = FALSE) {
  $account = user_load(array('uid' => $uid));
  $groups = array();

  // As Admin, get all properties in a relation
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbperm_cc_get_admin_groups($company_id,
					  $property_id,
					  $filter_active, $active,
					  $filter_suspended, $suspended) as $group) {
      $groups[$group['ClientId']][$group['GroupId']] =
	array('data' => $group,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $groups;
  }

  // Via relations
  foreach (chdbperm_cc_get_groups($uid,
				  $company_id,
				  $property_id,
				  $filter_active, $active,
				  $filter_suspended, $suspended) as $group) {
    $groups[$group['ClientId']][$group['GroupId']] =
      array('data' => $group,
	    'readonly' => TRUE,
	    'source' => 'Relation');
  }

  return $groups;
}



// Retrieve groups based on relations
function chpperm_retrieve_group_props($uid, $company_id, $client_id) {
  $account = user_load(array('uid' => $uid));
  $groups = array();

  // As Admin, get all properties in a relation
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbperm_get_all_groupprops($company_id, $client_id) as $group) {
      $groups[$group['PropertyId']][$group['LotId']][$group['GroupId']] =
	array('data' => $group,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $groups;
  }

  // Via relations
  foreach (chdbperm_cc_get_group_props($uid, $company_id, $client_id) as $group) {
    $groups[$group['PropertyId']][$group['LotId']][$group['GroupId']] =
      array('data' => $group,
	    'readonly' => TRUE,
	    'source' => 'Relation');
  }

  return $groups;
}



function chpperm_create_stall_options($company_id, $property_id, $lot_id, $level_id) {
  $stall_options = array();
  $stall_options_x = array();
  $permits = chdbperm_get_permit_stalls($company_id, $property_id, $lot_id, $level_id);
  $stalls_used = array();
  foreach ($permits as $entry) {
    $stalls_used[$entry['StallId']] = TRUE;
  }
  $stalls = chdbprop_get_stalls($property_id, $lot_id, $level_id);
  foreach ($stalls as $stall) {
    if (!$stall['Active']) {
      continue;
    }
    if (isset($stalls_used[$stall['StallId']])) {
      $stall_options_x[$stall['StallId']] = t('Busy: ').decode_entities($stall['Alias']);
    } else {
      $stall_options[$stall['StallId']] = decode_entities($stall['Alias']);
    }
  }
  $stall_options = array_merge($stall_options, $stall_options_x);
  return $stall_options;
}



function chpperm_fuzzy_search_lpn($lpn_a, $lpn_b) {
  return levenshtein($lpn_a, $lpn_b);
}



