<?php
require_once("anai_chpproperties.inc");



/**********
 ********** Profile add
 ********** Address: chppropprofileadd
 ********** Access:  'anai chp add property profiles'
 *********/



function chpprop_profile_add_form($form_state) {
  drupal_set_title(t('Add profile'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['option']['skipprofileselect'] = TRUE;
  $settings['option']['skiplotname'] = TRUE;
  chpprop_produce_policy($form, $form_state, $settings);

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpprop_profile_add_form_validate_save'),
	  '#submit' => array('chpprop_profile_add_form_submit_save'));
  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_profile_add_form_submit_back'));
  return $form;
}

function chpprop_profile_add_form_validate_save($form, &$form_state) {
  if (isset($form_state['values']['ajax']['policy']['ProfileName']) and
      empty($form_state['values']['ajax']['policy']['ProfileName'])) {
    form_set_error('ajax][policy][ProfileName', 'Profile name field is required.');
    return;
  }
  // Long term sanity check
  if ($form_state['values']['ajax']['policy']['rCheckbox'] and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['policy']['rValidityDays'])) {
    form_set_error('ajax][policy][rValidityDays', 'Must be a number');
    return;
  }
  if ($form_state['values']['ajax']['policy']['rCheckbox'] and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['policy']['rRemovalDays'])) {
    form_set_error('ajax][policy][rRemovalDays', 'Must be a number');
    return;
  }
  if ($form_state['values']['ajax']['policy']['rCheckbox'] and
      empty($form_state['values']['ajax']['policy']['rStallLevels'])) {
    form_set_error('ajax][policy][rStallLevels', 'Parking levels field is required');
    return;
  }
  if ($form_state['values']['ajax']['policy']['rCheckbox'] and
      empty($form_state['values']['ajax']['policy']['rStallCount'])) {
    form_set_error('ajax][policy][rStallCount', 'Parking stalls per level field is required');
    return;
  }
  if ($form_state['values']['ajax']['policy']['rCheckbox']) {
    $levels = explode(',', $form_state['values']['ajax']['policy']['rStallLevels']);
    foreach ($levels as $val) {
      if (!preg_match('/^\d+$/', $val)) {
	form_set_error('ajax][policy][rStallLevels', 'Must be a number, or list of numbers');
	return;
      }
    }
    $counts = explode(',', $form_state['values']['ajax']['policy']['rStallCount']);
    foreach ($counts as $val) {
      if (!preg_match('/^\d+$/', $val)) {
	form_set_error('ajax][policy][rStallCount', 'Must be a number, or list of numbers');
	return;
      }
    }
    if (count($levels) !== count($counts)) {
      form_set_error('ajax][policy][rStallCount', 'Must have one entry per level');
      return;
    }
  }
  // Short term sanity check
  if ($form_state['values']['ajax']['policy']['vCheckbox'] and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['policy']['vExpiryDays'])) {
    form_set_error('ajax][policy][vExpiryDays', 'Must be a number');
    return;
  }
  if ($form_state['values']['ajax']['policy']['vCheckbox'] and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['policy']['vRenewRetentionDays'])) {
    form_set_error('ajax][policy][vRenewRetentionDays', 'Must be a number');
    return;
  }
  if ($form_state['values']['ajax']['policy']['vCheckbox'] and
      !preg_match('/^\d+\.\d{2}$/', $form_state['values']['ajax']['policy']['vRegularCost'])) {
    form_set_error('ajax][policy][vRegularCost', 'Amount must be given in format X.XX');
    return;
  }
  if ($form_state['values']['ajax']['policy']['vCheckbox'] and
      !preg_match('/^\d+\.\d{2}$/', $form_state['values']['ajax']['policy']['vOvernightCost'])) {
    form_set_error('ajax][policy][vOvernightCost', 'Amount must be given in format X.XX');
    return;
  }
  if ($form_state['values']['ajax']['policy']['vCheckbox'] and
      empty($form_state['values']['ajax']['policy']['rPolicyAlias'])) {
    form_set_error('ajax][policy][rPolicyAlias', 'Alias field is required');
    return;
  }
  if ($form_state['values']['ajax']['policy']['vCheckbox'] and
      empty($form_state['values']['ajax']['policy']['vPolicyAlias'])) {
    form_set_error('ajax][policy][vPolicyAlias', 'Alias field is required');
    return;
  }
  if ($form_state['values']['ajax']['policy']['vCheckbox'] and
      empty($form_state['values']['ajax']['policy']['vStallLevels'])) {
    form_set_error('ajax][policy][rStallLevels', 'Parking levels field is required');
    return;
  }
  if ($form_state['values']['ajax']['policy']['vCheckbox'] and
      empty($form_state['values']['ajax']['policy']['vStallCount'])) {
    form_set_error('ajax][policy][rStallCount', 'Parking stalls per level field is required');
    return;
  }
  if ($form_state['values']['ajax']['policy']['vCheckbox']) {
    $levels = explode(',', $form_state['values']['ajax']['policy']['vStallLevels']);
    foreach ($levels as $val) {
      if (!preg_match('/^\d+$/', $val)) {
	form_set_error('ajax][policy][rStallLevels', 'Must be a number, or list of numbers');
	return;
      }
    }
    $counts = explode(',', $form_state['values']['ajax']['policy']['vStallCount']);
    foreach ($counts as $val) {
      if (!preg_match('/^\d+$/', $val)) {
	form_set_error('ajax][policy][rStallCount', 'Must be a number, or list of numbers');
	return;
      }
    }
    if (count($levels) !== count($counts)) {
      form_set_error('ajax][policy][rStallCount', 'Must have one entry per level');
      return;
    }
  }
  // Valet sanity check
  if ($form_state['values']['ajax']['policy']['valCheckbox'] and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['policy']['valNumunits'])) {
    form_set_error('ajax][policy][valNumunits', 'Must be a number');
    return;
  }
  /* if (!preg_match('/^\d+$/', $form_state['values']['ajax']['policy']['enfNumunits'])) { */
  /*   form_set_error('ajax][policy][enfNumunits', 'Must be a number'); */
  /*   return; */
  /* } */
}

function chpprop_profile_add_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropprofiles';
}

function chpprop_profile_id_create() {
  for ($i = 0; $i < 10; $i++) {
    $profile_id = chpprop_genRandomString20();
    if (sizeof(chdbprop_get_profile($profile_id)) == 0) {
      break;
    }
  }
  return $profile_id;
}

function chpprop_profile_add_form_submit_save($form, &$form_state) {
  global $user;

  // Decide policy in use
  $long_policy = 0;
  $short_policy = 0;
  $longshort_policy = 0;
  $valet_policy = 0;
  $enforcement_policy = 0;
  if ($form_state['values']['ajax']['policy']['rCheckbox'] and
      $form_state['values']['ajax']['policy']['vCheckbox']) {
    $longshort_policy = 1;
  } elseif ($form_state['values']['ajax']['policy']['rCheckbox']) {
    $long_policy = 1;
  } elseif ($form_state['values']['ajax']['policy']['vCheckbox']) {
    $short_policy = 1;
  }
  if ($form_state['values']['ajax']['policy']['valCheckbox']) {
    $valet_policy = 1;
  }
  /* if ($form_state['values']['ajax']['policy']['enfCheckbox']) { */
  /*   $enforcement_policy = 1; */
  /* } */

  // Insert the profile
  if (chdbprop_insert_new_profile
      ($user->uid,
       check_plain($form_state['values']['ajax']['policy']['ProfileName']),
       chpprop_profile_id_create(),
       $long_policy,
       $short_policy,
       $longshort_policy,
       check_plain($form_state['values']['ajax']['policy']['rPolicyAlias']),
       check_plain($form_state['values']['ajax']['policy']['vPolicyAlias']),
       $form_state['values']['ajax']['policy']['rPolicyType'],
       $form_state['values']['ajax']['policy']['vPolicyType'],
       $form_state['values']['ajax']['policy']['rPolicyClass'],
       $form_state['values']['ajax']['policy']['vPolicyClass'],
       $valet_policy,
       // Short Policy
       $form_state['values']['ajax']['policy']['vWeekdays']['Mon'] ? 1 : 0,
       $form_state['values']['ajax']['policy']['vWeekdays']['Tue'] ? 1 : 0,
       $form_state['values']['ajax']['policy']['vWeekdays']['Wed'] ? 1 : 0,
       $form_state['values']['ajax']['policy']['vWeekdays']['Thu'] ? 1 : 0,
       $form_state['values']['ajax']['policy']['vWeekdays']['Fri'] ? 1 : 0,
       $form_state['values']['ajax']['policy']['vWeekdays']['Sat'] ? 1 : 0,
       $form_state['values']['ajax']['policy']['vWeekdays']['Sun'] ? 1 : 0,
       sprintf("%02d:%02d",
	       $form_state['values']['ajax']['policy']['vStartHH'],
	       $form_state['values']['ajax']['policy']['vStartMM']),
       sprintf("%02d:%02d",
	       $form_state['values']['ajax']['policy']['vStopHH'],
	       $form_state['values']['ajax']['policy']['vStopMM']),
       sprintf("%02d:%02d",
	       $form_state['values']['ajax']['policy']['vOvernightStartHH'],
	       $form_state['values']['ajax']['policy']['vOvernightStartMM']),
       sprintf("%02d:%02d",
	       $form_state['values']['ajax']['policy']['vOvernightStopHH'],
	       $form_state['values']['ajax']['policy']['vOvernightStopMM']),
       $form_state['values']['ajax']['policy']['vExpiryChoice'],
       $form_state['values']['ajax']['policy']['vExpiryDuration'],
       $form_state['values']['ajax']['policy']['vExpiryDays'],
       $form_state['values']['ajax']['policy']['vRenewRetention'],
       $form_state['values']['ajax']['policy']['vRenewRetentionDays'],
       $form_state['values']['ajax']['policy']['vStallChoice'],
       $form_state['values']['ajax']['policy']['vStallLevels'],
       $form_state['values']['ajax']['policy']['vStallCount'],
       $form_state['values']['ajax']['policy']['vExtensionChoice'],
       $form_state['values']['ajax']['policy']['vExtensionServices']['Shared'] ? 1 : 0,
       check_plain($form_state['values']['ajax']['policy']['vRegularComment']),
       $form_state['values']['ajax']['policy']['vRegularCost'],
       $form_state['values']['ajax']['policy']['vOvernightCost'],
       // Long Policy
       $form_state['values']['ajax']['policy']['rValidityChoice'],
       $form_state['values']['ajax']['policy']['rValidityDays'],
       $form_state['values']['ajax']['policy']['rRemovalChoice'],
       $form_state['values']['ajax']['policy']['rRemovalRetention'],
       $form_state['values']['ajax']['policy']['rRemovalDays'],
       $form_state['values']['ajax']['policy']['rStallChoice'],
       $form_state['values']['ajax']['policy']['rStallLevels'],
       $form_state['values']['ajax']['policy']['rStallCount'],
       $form_state['values']['ajax']['policy']['rPermitChoice'],
       $form_state['values']['ajax']['policy']['rPermitServices']['Shared'] ? 1 : 0,
       // Enforcement
       /* $form_state['values']['ajax']['policy']['enfServices']['Ticketing'] ? 1 : 0, */
       /* $form_state['values']['ajax']['policy']['enfServices']['Booting'] ? 1 : 0, */
       /* $form_state['values']['ajax']['policy']['enfServices']['Towing'] ? 1 : 0, */
       /* $form_state['values']['ajax']['policy']['enfServices']['Mobile'] ? 1 : 0, */
       /* $form_state['values']['ajax']['policy']['enfNumunits'], */
       // Valet
       $form_state['values']['ajax']['policy']['valServices']['Messenger'] ? 1 : 0,
       $form_state['values']['ajax']['policy']['valServices']['Mobile'] ? 1 : 0,
       $form_state['values']['ajax']['policy']['valNumunits'],
       implode(',',array_keys(array_filter($form_state['values']['ajax']['policy']['valModifiers']))),
       $profile_index)) {
    drupal_set_message('Could not insert new profile', 'error');
    return;
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropprofiles';
}



/**********
 ********** Profile list
 ********** Address: chppropprofiles
 ********** Access:  'anai chp list property profiles'
 *********/



function chpprop_profile_list_form($form_state) {
  if ($form_state['storage']['detailed_view']) {
    return chpprop_profile_view_form($form_state);
  }
  if ($form_state['storage']['detailed_edit']) {
    return chpprop_profile_edit_form($form_state);
  }

  drupal_set_title(t('Profiles'));

  $form['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/back.png',
	  '#submit' => array('chpprop_profile_list_form_submit_back'));

  if (user_access('anai chp add property profiles')) {
    $form['add'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/add.png',
	    '#submit' => array('chpprop_profile_list_form_submit_add'));
  }

  global $user;
  $entries = chpprop_retrieve_profiles($user->uid);

  if (empty($entries)) {
    $form['empty'] = array('#value' => '<p>'.t('Empty.'));
  } else {
    $form['list'] = chpprop_profile_list_table_form($entries);
  }

  return $form;
}

function chpprop_profile_list_form_submit_add($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropprofileadd';
}

function chpprop_profile_list_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'front_page';
}

function chpprop_profile_list_table_form($entries) {
  $form['db'] =
    array('#type' => 'markup', '#value' => '',
	  '#theme' => 'chpprop_property_profile_table');

  if (user_access('anai chp edit property profiles')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }

  $looper = 0;
  foreach ($entries as $entry) {

    $form['db']['alias_'.$looper] =
      array('#value' => decode_entities($entry['data']['Alias']));

    $account = user_load(array('uid' => $entry['data']['CreatorUID']));
    $form['db']['user_'.$looper] =
      array('#value' => $account->name);

    $typestr = '';
    if ($entry['data']['LongShortPolicy']) {
      $typestr =
	$entry['data']['LongPolicyAlias'].
	t(' and ').
	$entry['data']['ShortPolicyAlias'].
	t(' parking');
    } elseif ($entry['data']['LongPolicy']) {
      $typestr =
	$entry['data']['LongPolicyAlias'].
	t(' parking');
    } elseif ($entry['data']['ShortPolicy']) {
      $typestr =
	$entry['data']['ShortPolicyAlias'].
	t(' parking');
    }
    if ($entry['data']['ValetPolicy']) {
      if (strlen($typestr) > 0) {
	$typestr .= '<br>';
      }
      $typestr .= t('Valet');
    }
    if ($entry['data']['EnforcementPolicy']) {
      if (strlen($typestr) > 0) {
	$typestr .= '<br>';
      }
      $typestr .= t('Enforcement');
    }

    $form['db']['type_'.$looper] = array('#value' => $typestr);

    $form['db']['view_'.$looper] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/viewsmall.png',
	    '#name' => 'op_view'.$entry['data']['Id'],
	    '#submit' => array('chpprop_profile_list_form_submit'));

    if ($readonly or $entry['readonly']) {
      $form['db']['details_'.$looper] = array('#value' => '');
    } else {
      $form['db']['details_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_details'.$entry['data']['Id'],
	      '#submit' => array('chpprop_profile_list_form_submit'));
    }
    $looper++;
  }
  return $form;
}

function theme_chpprop_property_profile_table($form) {
  global $user;
  // Table Header
  if ($user->uid == 1 or in_array('Admin', $user->roles)) {
    $header = array('Profile name', 'Created by', 'Type', '', '');
  } else {
    $header = array('Profile name', 'Type', '', '');
  }

  // Table Rows
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'alias_')) {
      continue;
    }
    $looper = substr($key, strlen('alias_'));
    if ($user->uid == 1 or in_array('Admin', $user->roles)) {
      $rows[] = array(drupal_render($form['alias_'.$looper]),
		      drupal_render($form['user_'.$looper]),
		      drupal_render($form['type_'.$looper]),
		      array('data' => drupal_render($form['view_'.$looper]),
			    'width' => 1, 'height' => 1),
		      array('data' => drupal_render($form['details_'.$looper]),
			    'width' => 1, 'height' => 1),
		      );
    } else {
      $rows[] = array(drupal_render($form['alias_'.$looper]),
		      drupal_render($form['type_'.$looper]),
		      array('data' => drupal_render($form['view_'.$looper]),
			    'width' => 1, 'height' => 1),
		      array('data' => drupal_render($form['details_'.$looper]),
			    'width' => 1, 'height' => 1),
		      );
    }
  }
  return theme_table($header, $rows);
}

function chpprop_profile_view_form($form_state) {
  $id = $form_state['storage']['id'];
  $profile = chdbprop_get_profile_id($id);
  drupal_set_title(t('Profile - NAME', array('NAME' => decode_entities($profile['Alias']))));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $form['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/back.png',
	  '#submit' => array('chpprop_profile_view_form_submit_back'));

  $settings = array();
  $settings['option']['readonly'] = TRUE;
  $settings['option']['skipprofileselect'] = TRUE;
  $settings['option']['skiplotname'] = TRUE;
  $settings['default']['ProfileName'] = decode_entities($profile['Alias']);
  $settings['default']['rCheckbox'] = ($profile['LongShortPolicy'] or $profile['LongPolicy']) ? 1 : 0;
  $settings['default']['rPolicyAlias'] = decode_entities($profile['LongPolicyAlias']);
  $settings['default']['rPolicyType'] = $profile['LongTypeId'];
  $settings['default']['rPolicyClass'] = $profile['LongClassId'];
  $settings['default']['rValidityChoice'] = $profile['ValidityPolicy'];
  $settings['default']['rValidityDays'] = $profile['ValidityDays'];
  $settings['default']['rRemovalChoice'] = $profile['RemovalPolicy'];
  $settings['default']['rRemovalRetention'] = $profile['RemovalRetention'];
  $settings['default']['rRemovalDays'] = $profile['RemovalDays'];
  $settings['default']['rStallChoice'] = $profile['LongStallPolicy'];
  $settings['default']['rStallLevels'] = $profile['LongStallLevels'];
  $settings['default']['rStallCount'] = $profile['LongStallCount'];
  $settings['default']['rPermitChoice'] = $profile['PermitPolicy'];
  $settings['default']['rPermitServices']['Shared'] = $profile['PermitShared'] ? 'Shared' : 0;
  $settings['default']['vCheckbox'] = ($profile['LongShortPolicy'] or $profile['ShortPolicy']) ? 1 : 0;
  $settings['default']['vPolicyAlias'] = decode_entities($profile['ShortPolicyAlias']);
  $settings['default']['vPolicyType'] = $profile['ShortTypeId'];
  $settings['default']['vPolicyClass'] = $profile['ShortClassId'];
  $settings['default']['vWeekdays'] = array('Mon' => 'Mon', 'Tue' => 'Tue', 'Wed' => 'Wed');
  $settings['default']['vWeekdays']['Mon'] = $profile['Mon'] ? 'Mon' : 0;
  $settings['default']['vWeekdays']['Tue'] = $profile['Tue'] ? 'Tue' : 0;
  $settings['default']['vWeekdays']['Wed'] = $profile['Wed'] ? 'Wed' : 0;
  $settings['default']['vWeekdays']['Thu'] = $profile['Thu'] ? 'Thu' : 0;
  $settings['default']['vWeekdays']['Fri'] = $profile['Fri'] ? 'Fri' : 0;
  $settings['default']['vWeekdays']['Sat'] = $profile['Sat'] ? 'Sat' : 0;
  $settings['default']['vWeekdays']['Sun'] = $profile['Sun'] ? 'Sun' : 0;
  list($hh, $mm) = sscanf($profile['Start'], "%02d:%02d");
  $settings['default']['vStartHH'] = $hh;
  $settings['default']['vStartMM'] = $mm;
  list($hh, $mm) = sscanf($profile['Stop'], "%02d:%02d");
  $settings['default']['vStopHH'] = $hh;
  $settings['default']['vStopMM'] = $mm;
  list($hh, $mm) = sscanf($profile['OvernightStart'], "%02d:%02d");
  $settings['default']['vOvernightStartHH'] = $hh;
  $settings['default']['vOvernightStartMM'] = $mm;
  list($hh, $mm) = sscanf($profile['OvernightStop'], "%02d:%02d");
  $settings['default']['vOvernightStopHH'] = $hh;
  $settings['default']['vOvernightStopMM'] = $mm;
  $settings['default']['vExpiryChoice'] = $profile['ExpirePolicy'];
  $settings['default']['vExpiryDuration'] = $profile['ExpireDuration'];
  $settings['default']['vExpiryDays'] = $profile['ExpireDays'];
  $settings['default']['vRenewRetention'] = $profile['RenewRetention'];
  $settings['default']['vRenewRetentionDays'] = $profile['RenewRetentionDays'];
  $settings['default']['vStallChoice'] = $profile['ShortStallPolicy'];
  $settings['default']['vStallLevels'] = $profile['ShortStallLevels'];
  $settings['default']['vStallCount'] = $profile['ShortStallCount'];
  $settings['default']['vExtensionChoice'] = $profile['ExtensionPolicy'];
  $settings['default']['vExtensionServices']['Shared'] = $profile['ExtensionShared'] ? 'Shared' : 0;
  $settings['default']['vRegularComment'] = decode_entities($profile['RegularComment']);
  $settings['default']['vRegularCost'] = sprintf("%02.2f", $profile['RegularCost']);
  $settings['default']['vOvernightCost'] = sprintf("%02.2f", $profile['OvernightCost']);
  $settings['default']['valCheckbox'] = $profile['ValetPolicy'] ? 1 : 0;
  $settings['default']['valServices']['Messenger'] =
    $profile['ValetMessenger'] ? 'Messenger' : 0;
  $settings['default']['valServices']['Mobile'] =
    $profile['ValUnitsPolicy'] ? 'Mobile' : 0;
  $settings['default']['valNumunits'] = $profile['ValetUnits'];
  $settings['default']['valModifiers']['DisableCheckout'] = 
    in_array('DisableCheckout',
	     explode(',',$profile['ValetMods'])) ? 'DisableCheckout' : 0;
  $settings['default']['valModifiers']['DisableSearch'] = 
    in_array('DisableSearch',
	     explode(',',$profile['ValetMods'])) ? 'DisableSearch' : 0;
  $settings['default']['valModifiers']['DisableCodes'] = 
    in_array('DisableCodes',
	     explode(',',$profile['ValetMods'])) ? 'DisableCodes' : 0;
  chpprop_produce_policy($form, $form_state, $settings);

  return $form;
}

function chpprop_profile_view_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropprofiles';
}

function chpprop_profile_edit_form($form_state) {
  if ($form_state['storage']['confirm_delete_profile']) {
    return confirm_form($form, 'Are you sure?', 'chppropprofiles',
			'The profile will be deleted from the system.');
  }

  $id = $form_state['storage']['id'];
  $profile = chdbprop_get_profile_id($id);
  drupal_set_title(t('Edit profile'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['option']['skipprofileselect'] = TRUE;
  $settings['option']['skiplotname'] = TRUE;
  $settings['default']['ProfileName'] = decode_entities($profile['Alias']);
  $settings['default']['rCheckbox'] = ($profile['LongShortPolicy'] or $profile['LongPolicy']) ? 1 : 0;
  $settings['default']['rPolicyAlias'] = decode_entities($profile['LongPolicyAlias']);
  $settings['default']['rPolicyType'] = $profile['LongTypeId'];
  $settings['default']['rPolicyClass'] = $profile['LongClassId'];
  $settings['default']['rValidityChoice'] = $profile['ValidityPolicy'];
  $settings['default']['rValidityDays'] = $profile['ValidityDays'];
  $settings['default']['rRemovalChoice'] = $profile['RemovalPolicy'];
  $settings['default']['rRemovalRetention'] = $profile['RemovalRetention'];
  $settings['default']['rRemovalDays'] = $profile['RemovalDays'];
  $settings['default']['rStallChoice'] = $profile['LongStallPolicy'];
  $settings['default']['rStallLevels'] = $profile['LongStallLevels'];
  $settings['default']['rStallCount'] = $profile['LongStallCount'];
  $settings['default']['rPermitChoice'] = $profile['PermitPolicy'];
  $settings['default']['rPermitServices']['Shared'] = $profile['PermitShared'] ? 'Shared' : 0;
  $settings['default']['vCheckbox'] = ($profile['LongShortPolicy'] or $profile['ShortPolicy']) ? 1 : 0;
  $settings['default']['vPolicyAlias'] = decode_entities($profile['ShortPolicyAlias']);
  $settings['default']['vPolicyType'] = $profile['ShortTypeId'];
  $settings['default']['vPolicyClass'] = $profile['ShortClassId'];
  $settings['default']['vWeekdays'] = array('Mon' => 'Mon', 'Tue' => 'Tue', 'Wed' => 'Wed');
  $settings['default']['vWeekdays']['Mon'] = $profile['Mon'] ? 'Mon' : 0;
  $settings['default']['vWeekdays']['Tue'] = $profile['Tue'] ? 'Tue' : 0;
  $settings['default']['vWeekdays']['Wed'] = $profile['Wed'] ? 'Wed' : 0;
  $settings['default']['vWeekdays']['Thu'] = $profile['Thu'] ? 'Thu' : 0;
  $settings['default']['vWeekdays']['Fri'] = $profile['Fri'] ? 'Fri' : 0;
  $settings['default']['vWeekdays']['Sat'] = $profile['Sat'] ? 'Sat' : 0;
  $settings['default']['vWeekdays']['Sun'] = $profile['Sun'] ? 'Sun' : 0;
  list($hh, $mm) = sscanf($profile['Start'], "%02d:%02d");
  $settings['default']['vStartHH'] = $hh;
  $settings['default']['vStartMM'] = $mm;
  list($hh, $mm) = sscanf($profile['Stop'], "%02d:%02d");
  $settings['default']['vStopHH'] = $hh;
  $settings['default']['vStopMM'] = $mm;
  list($hh, $mm) = sscanf($profile['OvernightStart'], "%02d:%02d");
  $settings['default']['vOvernightStartHH'] = $hh;
  $settings['default']['vOvernightStartMM'] = $mm;
  list($hh, $mm) = sscanf($profile['OvernightStop'], "%02d:%02d");
  $settings['default']['vOvernightStopHH'] = $hh;
  $settings['default']['vOvernightStopMM'] = $mm;
  $settings['default']['vExpiryChoice'] = $profile['ExpirePolicy'];
  $settings['default']['vExpiryDuration'] = $profile['ExpireDuration'];
  $settings['default']['vExpiryDays'] = $profile['ExpireDays'];
  $settings['default']['vRenewRetention'] = $profile['RenewRetention'];
  $settings['default']['vRenewRetentionDays'] = $profile['RenewRetentionDays'];
  $settings['default']['vStallChoice'] = $profile['ShortStallPolicy'];
  $settings['default']['vStallLevels'] = $profile['ShortStallLevels'];
  $settings['default']['vStallCount'] = $profile['ShortStallCount'];
  $settings['default']['vExtensionChoice'] = $profile['ExtensionPolicy'];
  $settings['default']['vExtensionServices']['Shared'] = $profile['ExtensionShared'] ? 'Shared' : 0;
  $settings['default']['vRegularComment'] = decode_entities($profile['RegularComment']);
  $settings['default']['vRegularCost'] = sprintf("%02.2f", $profile['RegularCost']);
  $settings['default']['vOvernightCost'] = sprintf("%02.2f", $profile['OvernightCost']);
  $settings['default']['valCheckbox'] = $profile['ValetPolicy'] ? 1 : 0;
  $settings['default']['valServices']['Messenger'] =
    $profile['ValetMessenger'] ? 'Messenger' : 0;
  $settings['default']['valServices']['Mobile'] =
    $profile['ValUnitsPolicy'] ? 'Mobile' : 0;
  $settings['default']['valNumunits'] = $profile['ValetUnits'];
  $settings['default']['valModifiers']['DisableCheckout'] = 
    in_array('DisableCheckout',
	     explode(',',$profile['ValetMods'])) ? 'DisableCheckout' : 0;
  $settings['default']['valModifiers']['DisableSearch'] = 
    in_array('DisableSearch',
	     explode(',',$profile['ValetMods'])) ? 'DisableSearch' : 0;
  $settings['default']['valModifiers']['DisableCodes'] = 
    in_array('DisableCodes',
	     explode(',',$profile['ValetMods'])) ? 'DisableCodes' : 0;
  chpprop_produce_policy($form, $form_state, $settings);

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpprop_profile_add_form_validate_save'),
	  '#submit' => array('chpprop_profile_edit_form_submit_save'));

  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_profile_view_form_submit_back'));

  if (user_access('anai chp delete property profiles')) {
    $form['delete'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/delete.png',
	    '#validate' => array('chpprop_profile_edit_form_validate_delete'));
  }
  return $form;
}

function chpprop_profile_edit_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropprofiles';
}

function chpprop_profile_edit_form_validate_delete($form, &$form_state) {
  $id = $form_state['storage']['id'];
  $profile = chdbprop_get_profile_id($id);
  $in_use = array();
  global $user;
  foreach (chpprop_retrieve_properties($user->uid) as $property) {
    $property_id = $property['data']['PropertyId'];
    foreach (chdbprop_get_policies($property_id) as $policy) {
      if ($policy['ProfileId'] === $profile['ProfileId']) {
	$in_use[] =
	  decode_entities($property['data']['Alias']).', '.
	  decode_entities($property['data']['Street1']).', '.
	  decode_entities($property['data']['City']);
      }
    }
  }
  if (count($in_use) > 0) {
    drupal_set_message('Delete profile aborted! Profile in use at:', 'error');
    drupal_set_message(implode('\n', $in_use), 'error');
    return;
  }
  $form_state['storage']['delete_profile'] = TRUE;
}

function chpprop_profile_edit_form_submit_save($form, &$form_state) {
  global $user;
  $id = $form_state['storage']['id'];

  // Decide policy in use
  $long_policy = 0;
  $short_policy = 0;
  $longshort_policy = 0;
  $valet_policy = 0;
  /* $enforcement_policy = 0; */
  if ($form_state['values']['ajax']['policy']['rCheckbox'] and
      $form_state['values']['ajax']['policy']['vCheckbox']) {
    $longshort_policy = 1;
  } elseif ($form_state['values']['ajax']['policy']['rCheckbox']) {
    $long_policy = 1;
  } elseif ($form_state['values']['ajax']['policy']['vCheckbox']) {
    $short_policy = 1;
  }
  if ($form_state['values']['ajax']['policy']['valCheckbox']) {
    $valet_policy = 1;
  }
  /* if ($form_state['values']['ajax']['policy']['enfCheckbox']) { */
  /*   $enforcement_policy = 1; */
  /* } */

  // Base settings
  if (chdbprop_update_profile
      ($id,
       check_plain($form_state['values']['ajax']['policy']['ProfileName']),
       $long_policy,
       $short_policy,
       $longshort_policy,
       check_plain($form_state['values']['ajax']['policy']['rPolicyAlias']),
       check_plain($form_state['values']['ajax']['policy']['vPolicyAlias']),
       $form_state['values']['ajax']['policy']['rPolicyType'],
       $form_state['values']['ajax']['policy']['vPolicyType'],
       $form_state['values']['ajax']['policy']['rPolicyClass'],
       $form_state['values']['ajax']['policy']['vPolicyClass'],
       $valet_policy,
       // Short Policy
       $form_state['values']['ajax']['policy']['vWeekdays']['Mon'] ? 1 : 0,
       $form_state['values']['ajax']['policy']['vWeekdays']['Tue'] ? 1 : 0,
       $form_state['values']['ajax']['policy']['vWeekdays']['Wed'] ? 1 : 0,
       $form_state['values']['ajax']['policy']['vWeekdays']['Thu'] ? 1 : 0,
       $form_state['values']['ajax']['policy']['vWeekdays']['Fri'] ? 1 : 0,
       $form_state['values']['ajax']['policy']['vWeekdays']['Sat'] ? 1 : 0,
       $form_state['values']['ajax']['policy']['vWeekdays']['Sun'] ? 1 : 0,
       sprintf("%02d:%02d",
	       $form_state['values']['ajax']['policy']['vStartHH'],
	       $form_state['values']['ajax']['policy']['vStartMM']),
       sprintf("%02d:%02d",
	       $form_state['values']['ajax']['policy']['vStopHH'],
	       $form_state['values']['ajax']['policy']['vStopMM']),
       sprintf("%02d:%02d",
	       $form_state['values']['ajax']['policy']['vOvernightStartHH'],
	       $form_state['values']['ajax']['policy']['vOvernightStartMM']),
       sprintf("%02d:%02d",
	       $form_state['values']['ajax']['policy']['vOvernightStopHH'],
	       $form_state['values']['ajax']['policy']['vOvernightStopMM']),
       $form_state['values']['ajax']['policy']['vExpiryChoice'],
       $form_state['values']['ajax']['policy']['vExpiryDuration'],
       $form_state['values']['ajax']['policy']['vExpiryDays'],
       $form_state['values']['ajax']['policy']['vRenewRetention'],
       $form_state['values']['ajax']['policy']['vRenewRetentionDays'],
       $form_state['values']['ajax']['policy']['vStallChoice'],
       $form_state['values']['ajax']['policy']['vStallLevels'],
       $form_state['values']['ajax']['policy']['vStallCount'],
       $form_state['values']['ajax']['policy']['vExtensionChoice'],
       $form_state['values']['ajax']['policy']['vExtensionServices']['Shared'] ? 1 : 0,
       check_plain($form_state['values']['ajax']['policy']['vRegularComment']),
       $form_state['values']['ajax']['policy']['vRegularCost'],
       $form_state['values']['ajax']['policy']['vOvernightCost'],
       // Long Policy
       $form_state['values']['ajax']['policy']['rValidityChoice'],
       $form_state['values']['ajax']['policy']['rValidityDays'],
       $form_state['values']['ajax']['policy']['rRemovalChoice'],
       $form_state['values']['ajax']['policy']['rRemovalRetention'],
       $form_state['values']['ajax']['policy']['rRemovalDays'],
       $form_state['values']['ajax']['policy']['rStallChoice'],
       $form_state['values']['ajax']['policy']['rStallLevels'],
       $form_state['values']['ajax']['policy']['rStallCount'],
       $form_state['values']['ajax']['policy']['rPermitChoice'],
       $form_state['values']['ajax']['policy']['rPermitServices']['Shared'] ? 1 : 0,
       // Valet
       $form_state['values']['ajax']['policy']['valServices']['Messenger'] ? 1 : 0,
       $form_state['values']['ajax']['policy']['valServices']['Mobile'] ? 1 : 0,
       $form_state['values']['ajax']['policy']['valNumunits'],
       implode(',',array_keys(array_filter($form_state['values']['ajax']['policy']['valModifiers']))))) {
    drupal_set_message('Could not update profile', 'error');
    return;
  }
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropprofiles';
}



function chpprop_profile_list_form_submit($form, &$form_state) {
  if ($form_state['storage']['confirm_delete_profile']) {
    $id = $form_state['storage']['id'];
    chdbprop_retire_profile_id($id);
    unset($form_state['storage']);
    $form_state['redirect'] = 'chppropprofiles';
    return;
  }
  if ($form_state['storage']['delete_profile']) {
    $form_state['storage']['confirm_delete_profile'] = TRUE;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_view')) {
      $id = substr($key, strlen('op_view'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['id'] = $id;
    $form_state['storage']['detailed_view'] = TRUE;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_details')) {
      $id = substr($key, strlen('op_details'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['id'] = $id;
    $form_state['storage']['detailed_edit'] = TRUE;
    return;
  }
}



