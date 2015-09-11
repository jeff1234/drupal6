<?php
require_once("anai_chppermits.inc");



/**********
 ********** Validation add
 ********** Address: chppermvaladd
 ********** Access:  'anai chp add permit validation'
 *********/



function chpperm_validation_add_form($form_state) {
  drupal_set_title(t('Add permit validation'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['show_companies'] = TRUE;
  $settings['show_regions'] = TRUE;
  $settings['show_clients'] = TRUE;
  $settings['option']['skippropertynotes'] = TRUE;
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $region_id = $form_state['storage']['ajax']['RegionId'];
  $client_id = $form_state['storage']['ajax']['ClientId'];

  if ($client_id === 'ANY') {
    $form['ajax']['error'] =
      array('#value' => '<p>'.t('Client missing.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpperm_validation_add_form_submit_back'));
    return $form;
  }

  $groups = chpperm_retrieve_groups($user->uid, $company_id);
  $groups = $groups[$client_id];

  if (empty($groups)) {
    $form['ajax']['error'] =
      array('#value' => '<p>'.t('Account missing.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpperm_validation_add_form_submit_back'));
    return $form;
  }

  $group_options = array();
  foreach ($groups as $key => $value) {
    $group_options[$key] = decode_entities($value['data']['Alias']);
  }

  if (!isset($form_state['storage']['ajax']['GroupId'])) {
    $form_state['storage']['ajax']['GroupId'] = key($group_options);
  }
  $form['ajax']['GroupId'] =
    array('#type' => 'select',
	  '#title' => t('Account'),
	  '#options' => $group_options,
	  '#default_value' => $form_state['storage']['ajax']['GroupId'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  $group_id = $form_state['storage']['ajax']['GroupId'];
  $group = $groups[$group_id]['data'];

  $now = chpuser_datetime_utc_to_utc('now');

  $local = chpuser_datetime_utc_to_usertimezone($now->format("Y-m-d H:i:s"));

  if (0 < $group['ValMonthlyMax']) {
    $stats = chdbperm_get_validationcounts($company_id, $client_id, $group_id,
					   array($local->format("Y-m")));
    if (!empty($stats) and $stats[0]['Count'] >= $group['ValMonthlyMax']) {
      $form['ajax']['error'] =
	array('#value' => '<p>'.t('You have reached your monthly maximum.').'<p>');
      $form['ajax']['cancel'] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	      '#submit' => array('chpperm_validation_add_form_submit_back'));
      return $form;
    }
  }

  if (0 < $group['ValWeeklyMax']) {
    $time = chpuser_datetime_utc_to_usertimezone($now->format("Y-m-d H:i:s"));
    $time->modify(sprintf("-%d days", (int)$time->format("N") - 1));
    $datetimes = array();
    for ($i = 0; $i < 7; $i++) {
      $datetimes[] = $time->format("Y-m-d");
      $time->modify("+1 day");
    }
    $value = 0;
    foreach (chdbperm_get_validationcounts($company_id, $client_id, $group_id,
					   $datetimes) as $stats) {
      $value += $stats['Count'];
    }
    if ($value >= $group['ValWeeklyMax']) {
      $form['ajax']['error'] =
	array('#value' => '<p>'.t('You have reached your weekly maximum.').'<p>');
      $form['ajax']['cancel'] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	      '#submit' => array('chpperm_validation_add_form_submit_back'));
      return $form;
    }
  }

  if (0 < $group['ValDailyMax']) {
    $stats = chdbperm_get_validationcounts($company_id, $client_id, $group_id,
					   array($local->format("Y-m-d")));
    if (!empty($stats) and $stats[0]['Count'] >= $group['ValDailyMax']) {
      $form['ajax']['error'] =
	array('#value' => '<p>'.t('You have reached your daily maximum.').'<p>');
      $form['ajax']['cancel'] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	      '#submit' => array('chpperm_validation_add_form_submit_back'));
      return $form;
    }
  }

  $groupprops = chpperm_retrieve_group_props($user->uid, $company_id, $client_id);

  $property_options = array();
  foreach ($groupprops as $key => $entries) {
    $property_options[$key] = $key;
  }

  if (empty($property_options)) {
    $form['ajax']['error'] =
      array('#value' => '<p>'.t('Property assignment missing.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpperm_validation_add_form_submit_back'));
    return $form;
  }

  if (!isset($form_state['storage']['ajax']['PropertyId'])) {
    $form_state['storage']['ajax']['PropertyId'] = key($property_options);
  }
  if (1 < count($property_options)) {
    $form['ajax']['PropertyId'] =
      array('#type' => 'select',
	    '#title' => t('Property'),
	    '#options' => $property_options,
	    '#default_value' => $form_state['storage']['ajax']['PropertyId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
  } else {
    $form['ajax']['PropertyId'] =
      array('#type' => 'hidden',
	    '#value' => key($property_options));
  }

  $groupprops = $groupprops[$form_state['storage']['ajax']['PropertyId']];

  $lot_options = array();
  foreach ($groupprops as $key => $entries) {
    $lot_options[$key] = $key;
  }

  if (empty($lot_options)) {
    $form['ajax']['error'] =
      array('#value' => '<p>'.t('Lot assignment missing.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpperm_validation_add_form_submit_back'));
    return $form;
  }

  if (!isset($form_state['storage']['ajax']['LotId'])) {
    $form_state['storage']['ajax']['LotId'] = key($lot_options);
  }
  if (1 < count($lot_options)) {
    $form['ajax']['LotId'] =
      array('#type' => 'select',
	    '#title' => t('Lot'),
	    '#options' => $lot_options,
	    '#default_value' => $form_state['storage']['ajax']['LotId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
  } else {
    $form['ajax']['LotId'] =
      array('#type' => 'hidden',
	    '#value' => key($lot_options));
  }

  $groupprops = $groupprops[$form_state['storage']['ajax']['LotId']];
  $groupprops = $groupprops[$form_state['storage']['ajax']['GroupId']];

  // Layout validation reason
  $options = array();
  if ($group['ValResidential']) {
    $options['Residential'] = t('Residential');
  }
  if ($group['ValCommercial']) {
    $options['Commercial'] = t('Commercial');
  }
  if (empty($options)) {
    $form['ajax']['error'] =
      array('#value' => '<p>'.t('Account missing validation reason.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpperm_validation_add_form_submit_back'));
  }
  if (!isset($form_state['storage']['ajax']['Reason'])) {
    $form_state['storage']['ajax']['Reason'] = key($options);
  }
  if (1 < count($options)) {
    $form['ajax']['Reason'] =
      array('#type' => 'radios',
	    '#title' => t('Reason for visit'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['Reason'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
  }

  // Layout number of validations
  $options = array();
  for ($i = 1; $i <= 10; $i++) {
    $options[$i] = $i;
  }
  if (!isset($form_state['storage']['ajax']['NumValidations'])) {
    $form_state['storage']['ajax']['NumValidations'] = key($options);
  }
  $form['ajax']['NumValidations'] =
    array('#type' => 'select',
	  '#title' => t('Number of validations'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['NumValidations'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  // Layout validation entries
  $form['ajax']['Validations'] =
    array('#prefix' => '<table>',
	  '#suffix' => '</table>');

  for ($i = 0; $i < $form_state['storage']['ajax']['NumValidations']; $i++) {

    // Layout LPN
    if (!isset($form_state['storage']['ajax']['Validations']['LPN'.$i])) {
      $form_state['storage']['ajax']['Validations']['LPN'.$i] = '';
    }
    $form['ajax']['Validations']['LPN'.$i] =
      array('#type' => 'textfield',
	    '#title' => t('License Plate Number'),
	    '#maxlength' => 10,
	    '#size' => 15,
	    '#default_value' => $form_state['storage']['ajax']['Validations']['LPN'.$i],
	    '#description' => '(No spaces)',
	    '#attributes' => array('onBlur'=>'this.value=this.value.toUpperCase()'),
	    '#prefix' => '<tr><td>',
	    '#suffix' => '</td>');

    // Layout Length selector
    $options = array();
    foreach (explode(",", $group['ValLengths']) as $key) {
      $name = chpperm_validation_length_name($key, $found);
      if ($found) {
	$options[$key] = $name;
      }
    }
    if (empty($options)) {
      $options['ERROR'] = t('Unknown');
    }
    if (isset($form_state['storage']['ajax']['Validations']['Length'.$i]) and
	!in_array($form_state['storage']['ajax']['Validations']['Length'.$i],
		  array_keys($options))) {
      unset($form_state['storage']['ajax']['Validations']['Length'.$i]);
    }
    if (!isset($form_state['storage']['ajax']['Validations']['Length'.$i])) {
      $form_state['storage']['ajax']['Validations']['Length'.$i] = key($options);
    }
    $form['ajax']['Validations']['Length'.$i] =
      array('#type' => 'select',
	    '#title' => t('Length'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['Validations']['Length'.$i],
	    '#prefix' => '<td>');

    // Layout Benefit selector
    $options = array();
    foreach (explode(",", $group['ValBenefits']) as $key) {
      $name = chpperm_validation_benefit_name($key, $found);
      if ($found) {
	$options[$key] = $name;
      }
    }
    if (empty($options)) {
      $options['ERROR'] = t('Unknown');
    }
    if (isset($form_state['storage']['ajax']['Validations']['Benefit'.$i]) and
	!in_array($form_state['storage']['ajax']['Validations']['Benefit'.$i],
		  array_keys($options))) {
      unset($form_state['storage']['ajax']['Validations']['Benefit'.$i]);
    }
    if (!isset($form_state['storage']['ajax']['Validations']['Benefit'.$i])) {
      $form_state['storage']['ajax']['Validations']['Benefit'.$i] = '';
    }
    $form['ajax']['Validations']['Benefit'.$i] =
      array('#type' => 'select',
	    '#title' => t('Benefit'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['Validations']['Benefit'.$i],
	    '#suffix' => '</td>');

    // Layout Contact info for Residential
    if ($form_state['storage']['ajax']['Reason'] === 'Residential') {
      if (!isset($form_state['storage']['ajax']['Validations']['FirstName'.$i])) {
	$form_state['storage']['ajax']['Validations']['FirstName'.$i] = '';
      }
      $form['ajax']['Validations']['FirstName'.$i] =
	array('#type' => 'textfield',
	      '#title' => t('Visit first name'),
	      '#default_value' => $form_state['storage']['ajax']['Validations']['FirstName'.$i],
	      '#prefix' => '<td>',
	      '#suffix' => '</td>');
      if (!isset($form_state['storage']['ajax']['Validations']['LastName'.$i])) {
	$form_state['storage']['ajax']['Validations']['LastName'.$i] = '';
      }
      $form['ajax']['Validations']['LastName'.$i] =
	array('#type' => 'textfield',
	      '#title' => t('Visit last name'),
	      '#default_value' => $form_state['storage']['ajax']['Validations']['LastName'.$i],
	      '#prefix' => '<td>',
	      '#suffix' => '</td>');
      if (!isset($form_state['storage']['ajax']['Validations']['SuiteId'.$i])) {
	$form_state['storage']['ajax']['Validations']['SuiteId'.$i] = '';
      }
      $form['ajax']['Validations']['SuiteId'.$i] =
	array('#type' => 'textfield',
	      '#title' => t('Suite'),
	      '#maxlength' => 10,
	      '#size' => 15,
	      '#default_value' => $form_state['storage']['ajax']['Validations']['SuiteId'.$i],
	      '#prefix' => '<td>',
	      '#suffix' => '</td></tr>');

      // Layout Commercial selectors
    } elseif ($form_state['storage']['ajax']['Reason'] === 'Commercial') {
      // Layout Mission selector
      $options = array();
      foreach (explode(",", $group['ValMissions']) as $key) {
	$name = chpperm_validation_mission_name($key, $found);
	if ($found) {
	  $options[$key] = $name;
	}
      }
      if (empty($options)) {
	$options['ERROR'] = t('Unknown');
      }
      if (isset($form_state['storage']['ajax']['Validations']['Mission'.$i]) and
	  !in_array($form_state['storage']['ajax']['Validations']['Mission'.$i],
		    array_keys($options))) {
	unset($form_state['storage']['ajax']['Validations']['Mission'.$i]);
      }
      if (!isset($form_state['storage']['ajax']['Validations']['Mission'.$i])) {
	$form_state['storage']['ajax']['Validations']['Mission'.$i] = '';
      }
      $form['ajax']['Validations']['Mission'.$i] =
	array('#type' => 'select',
	      '#title' => t('Mission'),
	      '#options' => $options,
	      '#default_value' => $form_state['storage']['ajax']['Validations']['Mission'.$i],
	      '#prefix' => '<td>',
	      '#suffix' => '</td></tr>');
    }
  }

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpperm_validation_add_form_validate_save'),
	  '#submit' => array('chpperm_validation_add_form_submit_save'));
  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpperm_validation_add_form_submit_back'));

  return $form;
}

function chpperm_validation_add_form_validate_save($form, &$form_state) {
  for ($i = 0; $i < $form_state['values']['ajax']['NumValidations']; $i++) {
    if (empty($form_state['values']['ajax']['Validations']['LPN'.$i])) {
      form_set_error('ajax][Validations][LPN'.$i,
		     'LPN field is required.');
      return;
    }
    if ($form_state['values']['ajax']['Reason'] === 'Residential') {
      if (empty($form_state['values']['ajax']['Validations']['FirstName'.$i])) {
	form_set_error('ajax][Validations][FirstName'.$i,
		       'Visit first name field is required.');
	return;
      }
      if (empty($form_state['values']['ajax']['Validations']['LastName'.$i])) {
	form_set_error('ajax][Validations][LastName'.$i,
		       'Visit last name field is required.');
	return;
      }
      if (empty($form_state['values']['ajax']['Validations']['SuiteId'.$i])) {
	form_set_error('ajax][Validations][SuiteId'.$i,
		       'Suite field is required.');
	return;
      }
    }
  }
}

function chpperm_validation_add_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermval';
}

function chpperm_validation_add_form_submit_save($form, &$form_state) {
  global $user;

  $company_id = $form_state['values']['ajax']['CompanyId'];
  $region_id = $form_state['values']['ajax']['RegionId'];
  $client_id = $form_state['values']['ajax']['ClientId'];
  $group_id = $form_state['values']['ajax']['GroupId'];
  $member_id = NULL;
  $property_id = $form_state['values']['ajax']['PropertyId'];
  $lot_id = $form_state['values']['ajax']['LotId'];
  $policy = chdbprop_get_policy($property_id, $lot_id);
  $profile_id = $policy['ProfileId'];
  $profile = chdbprop_get_profile($profile_id);

  $lat = $lon = NULL;

  $long_term = FALSE;
  $short_term = FALSE;
  $valet = FALSE;
  if ($profile['LongPolicy'] or $profile['LongShortPolicy']) {
    $long_term = TRUE;
  }
  if ($profile['ShortPolicy'] or $profile['LongShortPolicy']) {
    $short_term = TRUE;
  }
  if (chpprop_property_allows_valet($profile)) {
    $valet = TRUE;
  }

  $prename_id = $middlename = $sufname_id = NULL;
  $street = $city = $postalcode = $province = $country = NULL;
  $phone = $email = $emailnotification = NULL;
  $role = NULL;
  $make = $model = $color = NULL;
  $floor_id = NULL;
  $level_id = $stall_id = NULL;
  $complementary = FALSE;
  $cost = '0.00';
  $permit_class = 'Validation';
  $container_id = $type_id = NULL;

  $lengths = chpperm_validation_lengths();

  for ($i = 0; $i < $form_state['values']['ajax']['NumValidations']; $i++) {
    if (isset($form_state['values']['ajax']['Validations']['FirstName'.$i])) {
      $firstname = check_plain($form_state['values']['ajax']['Validations']['FirstName'.$i]);
    } else {
      $firstname = NULL;
    }
    if (isset($form_state['values']['ajax']['Validations']['LastName'.$i])) {
      $lastname = check_plain($form_state['values']['ajax']['Validations']['LastName'.$i]);
    } else {
      $lastname = NULL;
    }

    $lpn = check_plain($form_state['values']['ajax']['Validations']['LPN'.$i]);

    $now = chpuser_datetime_utc_to_utc('now');
    $db = chdbperm_get_permittag_property($company_id, $property_id, $lot_id);
    if (empty($db)) {
      $permit_tag = $now->format("ymd")."0001";
      if (chdbperm_insert_permittag_property
	  ($company_id,
	   $property_id,
	   $lot_id,
	   $permit_tag)) {
	drupal_set_message('Could not insert permit_tag', 'error');
	return;
      }
    } else {
      $permit_tag = $db['PermitTag'];
      $yy = substr($permit_tag, 0, 2);
      $mm = substr($permit_tag, 2, 2);
      $dd = substr($permit_tag, 4, 2);
      if ($now->format("y") != $yy or
	  $now->format("m") != $mm or
	  $now->format("d") != $dd) {
	$permit_tag = $now->format("ymd")."0001";
      } else {
	$permit_tag++;
      }
      if (chdbperm_update_permittag_property
	  ($company_id,
	   $property_id,
	   $lot_id,
	   $permit_tag)) {
	drupal_set_message('Could not update permit_tag', 'error');
	return;
      }
    }

    if (isset($form_state['values']['ajax']['Validations']['SuiteId'.$i])) {
      $suite_id = $form_state['values']['ajax']['Validations']['SuiteId'.$i];
    } else {
      $suite_id = NULL;
    }

    // Insert common data
    if (chdbperm_insert_permit
	($company_id, $property_id, $lot_id, $group_id, $member_id,
	 $user->uid, $lat, $lon,
	 $long_term, $short_term, $valet,
	 $prename_id, $firstname, $middlename, $lastname, $sufname_id,
	 $street, $city, $postalcode, $province, $country,
	 $phone, $email, $emailnotification,
	 $role,
	 $lpn, $make, $model, $color,
	 $permit_tag, $floor_id, $suite_id, $level_id, $stall_id,
	 $complementary, $cost,
	 $permit_class, $container_id, $type_id,
	 $id)) {
      drupal_set_message('Could not insert new permit', 'error');
      return;
    }

    // Update timing
    $begin = chpuser_datetime_utc_to_utc('now');
    $expiry = chpuser_datetime_utc_to_utc('now');
    $key = $form_state['storage']['ajax']['Validations']['Length'.$i];
    foreach ($lengths[$key]['modifiers'] as $modifier) {
      $expiry->modify($modifier);
    }
    if (chdbperm_update_permit_timing_id
	($id,
	 TRUE, $begin->format("Y-m-d H:i:s"),
	 TRUE, $expiry->format("Y-m-d H:i:s"))) {
      drupal_set_message('Could not update permit timing', 'error');
      return;
    }
    if (chdbperm_update_permit_retention_at_expiry
	($id)) {
      drupal_set_message('Could not update retention', 'error');
      return;
    }

    // Insert new-stats
    if (chdbperm_stats_new
	($company_id, $property_id, $lot_id,
	 $group_id, $member_id, $lpn)) {
      drupal_set_message('Could not update stats 1', 'error');
    }

    if (chdbperm_validationcounts_new
	($user->uid, $user->timezone,
	 $company_id, $client_id, $group_id)) {
      drupal_set_message('Could not update stats 2', 'error');
    }
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermval';
}



/**********
 ********** Validation list
 ********** Address: chppermval
 ********** Access:  'anai chp list permit validation'
 *********/



function chpperm_validation_list_form($form_state) {
  drupal_set_title(t('Permit validations'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['show_companies'] = TRUE;
  $settings['show_regions'] = TRUE;
  $settings['show_clients'] = TRUE;
  $settings['show_properties'] = TRUE;
  $settings['show_lots'] = TRUE;
  $settings['buttons']['back']['title'] = t('Back');
  if (user_access('anai chp add permit validation')) {
    $settings['buttons']['add']['submit'] = 'chpperm_validation_list_form_submit_add';
  }
  $settings['buttons']['back']['submit'] = 'chpperm_validation_list_form_submit_back';
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $region_id = $form_state['storage']['ajax']['RegionId'];
  $client_id = $form_state['storage']['ajax']['ClientId'];
  $property_id = $form_state['storage']['ajax']['PropertyId'];
  $lot_id = $form_state['storage']['ajax']['LotId'];

  if ($client_id === 'ANY') {
    $form['ajax']['error'] =
      array('#value' => '<p>'.t('Client missing.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpperm_validation_list_form_submit_back'));
    return $form;
  }

  $groups = chpperm_retrieve_groups($user->uid, $company_id, $property_id);
  $groups = $groups[$client_id];

  if (empty($groups)) {
    $form['ajax']['error'] =
      array('#value' => '<p>'.t('Account missing.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpperm_validation_list_form_submit_back'));
    return $form;
  }

  $group_options = array();
  foreach ($groups as $key => $value) {
    $group_options[$key] = decode_entities($value['data']['Alias']);
  }

  if (!isset($form_state['storage']['ajax']['GroupId'])) {
    $form_state['storage']['ajax']['GroupId'] = key($group_options);
  }
  $form['ajax']['GroupId'] =
    array('#type' => 'select',
	  '#title' => t('Account'),
	  '#options' => $group_options,
	  '#default_value' => $form_state['storage']['ajax']['GroupId'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  $group_id = $form_state['storage']['ajax']['GroupId'];
  $group = $groups[$group_id]['data'];

  $now = chpuser_datetime_utc_to_utc('now');

  $permits = chdbperm_cc_get_permits($company_id, $property_id, $lot_id,
				     TRUE, $group_id,
				     FALSE, NULL, // $member_id
				     FALSE, NULL,
				     TRUE, $now->format("Y-m-d H:i:s"),
				     TRUE, $now->format("Y-m-d H:i:s"),
				     FALSE, NULL,
				     TRUE, TRUE,
				     TRUE, FALSE,
				     TRUE, 'Validation');

  $local = chpuser_datetime_utc_to_usertimezone($now->format("Y-m-d H:i:s"));

  $form['ajax']['time'] =
    array('#value' => '<p>'.$local->format("D, M j,Y H:i:s"));

  if (empty($permits)) {
    $form['ajax']['empty'] =
      array('#value' => '<p>'.t('No active validations.'));
    return $form;
  } else {
    $form['ajax']['entries'] =
      array('#value' => '<p>'.t('Active validations: NUM',
				array('NUM' => count($permits))).'<p>');
  }

  if (0 < $group['ValDailyMax']) {
    $stats = chdbperm_get_validationcounts($company_id, $client_id, $group_id,
					   array($local->format("Y-m-d")));
    $form['ajax']['daily'] =
      array('#value' => t('Daily usage: USED out of LIMIT',
			  array('USED' => empty($stats) ? 0 : $stats[0]['Count'],
				'LIMIT' => $group['ValDailyMax'])).'<br>');
  }

  if (0 < $group['ValWeeklyMax']) {
    $time = chpuser_datetime_utc_to_usertimezone($now->format("Y-m-d H:i:s"));
    $time->modify(sprintf("-%d days", (int)$time->format("N") - 1));
    $datetimes = array();
    for ($i = 0; $i < 7; $i++) {
      $datetimes[] = $time->format("Y-m-d");
      $time->modify("+1 day");
    }
    $value = 0;
    foreach (chdbperm_get_validationcounts($company_id, $client_id, $group_id,
					   $datetimes) as $stats) {
      $value += $stats['Count'];
    }
    $form['ajax']['weekly'] =
      array('#value' => t('Weekly usage: USED out of LIMIT',
			  array('USED' => $value,
				'LIMIT' => $group['ValWeeklyMax'])).'<br>');
  }

  if (0 < $group['ValMonthlyMax']) {
    $stats = chdbperm_get_validationcounts($company_id, $client_id, $group_id,
					   array($local->format("Y-m")));
    $form['ajax']['monthly'] =
      array('#value' => t('Monthly usage: USED out of LIMIT',
			  array('USED' => empty($stats) ? 0 : $stats[0]['Count'],
				'LIMIT' => $group['ValMonthlyMax'])).'<br>');
  }

  if (!empty($permits)) {
    $form['ajax']['list'] = chpperm_validation_list_table_form($permits);
  }

  return $form;
}

function chpperm_validation_list_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'front_page';
}

function chpperm_validation_list_form_submit_add($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermvaladd';
}

function chpperm_validation_list_table_form($entries) {
  $form['db'] =
    array('#type' => 'markup', '#value' => '',
  	  '#theme' => 'chpperm_validation_list_table');

  if (user_access('anai chp edit permit validation')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }

  $looper = 0;
  foreach ($entries as $entry) {
    $form['db']['lpn_'.$looper] = array('#value' => decode_entities($entry['LPN']));
    $time = chpuser_datetime_utc_to_usertimezone($entry['BeginTime']);
    $form['db']['begin_'.$looper] = array('#value' => $time->format("D, M j, Y H:i:s"));
    $time = chpuser_datetime_utc_to_usertimezone($entry['ExpiryTime']);
    $form['db']['expiry_'.$looper] = array('#value' => $time->format("D, M j, Y H:i:s"));

    $looper++;
  }
  return $form;
}

function theme_chpperm_validation_list_table($form) {
  $header = array(t('Created'), t('LPN'), t('Expires'));
  $rows = array();

  foreach ($form as $key => $value) {
    if (!strstr($key, 'lpn_')) {
      continue;
    }
    $looper = substr($key, strlen('lpn_'));
    $rows[] = array(drupal_render($form['lpn_'.$looper]),
                    drupal_render($form['begin_'.$looper]),
                    drupal_render($form['expiry_'.$looper]),
                    );
  }
  return theme_table($header, $rows);
}


