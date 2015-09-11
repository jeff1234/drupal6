<?php
require_once("anai_chpproperties.inc");



/**********
 ********** Enforcer add
 ********** Address: chppropenforceradd
 ********** Access:  'anai chp add enforcers'
 *********/



function chpprop_enforcer_add_form($form_state) {
  drupal_set_title(t('Add enforcer'));

  global $user;
  $account = user_load(array('uid' => $user->uid));
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['show_companies'] = TRUE;
  if ((!in_array('Admin', array_values($account->roles)) and $account->status) and
      $account->uid != 1) {
    $settings['show_regions'] = TRUE;
    $settings['show_clients'] = TRUE;
  }
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];

  // Create and validate property options
  $property_options = array();
  if (!empty($form_state['storage']['ajax']['Properties'])) {
    foreach ($form_state['storage']['ajax']['Properties'] as $property_id => $property) {
      $property_options[$property_id] =
	decode_entities($property['data']['Alias']).', '.
	decode_entities($property['data']['Street']).', '.
	decode_entities($property['data']['City']);
    }
  }
  if (empty($property_options)) {
    $form['ajax']['error'] = array('#value' => '<p>'.t('Property missing.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpprop_enforcer_add_form_submit_cancel'));
    return $form;
  }

  // Create and validate policies options, used later for lot selection
  $policies = chpprop_retrieve_policies($user->uid, $company_id);
  if (empty($policies)) {
    $form['ajax']['error'] = array('#value' => '<p>'.t('Lot missing.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpprop_enforcer_add_form_submit_cancel'));
    return $form;
  }

  // Layout enforcer alias
  if (!isset($form_state['storage']['ajax']['Alias'])) {
    $form_state['storage']['ajax']['Alias'] = '';
  }
  $form['ajax']['Alias'] =
    array('#type' => 'textfield',
	  '#title' => t('Enforcer'),
	  '#default_value' => $form_state['storage']['ajax']['Alias'],
	  '#maxlength' => 80);

  // Property/Lot selector
  $form['ajax']['property'] = array('#type' => 'fieldset', '#title' => t('Assignment'));

  // Layout property options
  if (isset($form_state['storage']['ajax']['property']['PropertyId']) and
      !in_array($form_state['storage']['ajax']['property']['PropertyId'],
		array_keys($property_options))) {
    unset($form_state['storage']['ajax']['property']['PropertyId']);
  }
  if (!isset($form_state['storage']['ajax']['property']['PropertyId'])) {
    $form_state['storage']['ajax']['property']['PropertyId'] = key($property_options);
  }
  if (1 < count($property_options)) {
    $form['ajax']['property']['PropertyId'] =
      array('#type' => 'select',
	    '#title' => t('Property'),
	    '#options' => $property_options,
	    '#default_value' => $form_state['storage']['ajax']['property']['PropertyId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
  }

  $property_id = $form_state['storage']['ajax']['property']['PropertyId'];

  // Create and validate lot options based on selected property
  $lot_options = array();
  if (isset($policies[$property_id])) {
    foreach ($policies[$property_id] as $lot_id => $policy) {
      $lot_options[$lot_id] = decode_entities($policy['data']['Alias']);
    }
  }
  if (empty($lot_options)) {
    $form['ajax']['error'] = array('#value' => '<p>'.t('Lot missing.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpprop_enforcer_add_form_submit_cancel'));

    return $form;
  }

  // Layout lot options
  if (isset($form_state['storage']['ajax']['property']['LotId']) and
      !in_array($form_state['storage']['ajax']['property']['LotId'],
		array_keys($lot_options))) {
    unset($form_state['storage']['ajax']['property']['LotId']);
  }
  if (!isset($form_state['storage']['ajax']['property']['LotId'])) {
    $form_state['storage']['ajax']['property']['LotId'] = key($lot_options);
  }
  if (1 < count($lot_options)) {
    $form['ajax']['property']['LotId'] =
      array('#type' => 'select',
	    '#title' => t('Lot'),
	    '#options' => $lot_options,
	    '#default_value' => $form_state['storage']['ajax']['property']['LotId']);
  }

  $lot_id = $form_state['storage']['ajax']['property']['LotId'];

  if (1 < count($property_options) or 1 < count($lot_options)) {
    $form['ajax']['property']['addbtn'] = array('#type' => 'button', '#value' => t('Add'));
    $form['ajax']['property']['clearbtn'] = array('#type' => 'button', '#value' => t('Clear'));
    $form['ajax']['property']['allbtn'] = array('#type' => 'button', '#value' => t('All'));
    if (!isset($form_state['storage']['ajax']['property']['List'])) {
      $list = array();
      $form_state['storage']['ajax']['property']['List'] = $list;
    } else {
      $list = unserialize($form_state['storage']['ajax']['property']['List']);
    }

    if (strpos($form_state['clicked_button']['#id'], 'addbtn') !== FALSE) {
      $list[$property_id][$lot_id] = TRUE;
    }
    if (strpos($form_state['clicked_button']['#id'], 'clearbtn') !== FALSE) {
      $list = array();
    }
    if (strpos($form_state['clicked_button']['#id'], 'allbtn') !== FALSE) {
      foreach ($property_options as $propkey => $propvalue) {
	foreach ($policies[$propkey] as $lotkey => $lotvalue) {
	  $list[$propkey][$lotkey] = TRUE;
	}
      }
    }
  } else {
    $list[$property_id][$lot_id] = TRUE;
  }

  $form['ajax']['property']['List'] = array('#type' => 'value', '#value' => serialize($list));

  $header = array(t('Property'), t('Lot'));
  $rows = array();
  foreach ($list as $property_id => $values) {
    foreach ($values as $lot_id => $value) {
      $rows[] = array($property_options[$property_id], $lot_options[$lot_id]);
    }
  }
  $form['ajax']['property']['ListView'] = array('#value' => theme_table($header, $rows));

  // Settings
  $form['ajax']['settings'] = array('#type' => 'fieldset', '#title' => t('Settings'));

  // Create session options
  $options = array();
  $options['60'] = t('One hour');
  $options['120'] = t('Two hours');
  $options['180'] = t('Three hours');
  $options['240'] = t('Four hours');
  $options['300'] = t('Five hours');
  $options['360'] = t('Six hours');
  $options['420'] = t('Seven hours');
  $options['480'] = t('Eight hours');
  $options['540'] = t('Nine hours');
  $options['600'] = t('Ten hours');

  // Layout session
  if (!isset($form_state['storage']['ajax']['settings']['Session'])) {
    $form_state['storage']['ajax']['settings']['Session'] = '480';
  }
  $form['ajax']['settings']['Session'] =
    array('#type' => 'select',
	  '#title' => t('Mobile unit session timeout'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['settings']['Session']);

  // Create idle options
  $options = array();
  $options['15'] = t('15 minutes');
  $options['30'] = t('30 minutes');
  $options['45'] = t('45 minutes');
  $options['60'] = t('One hour');
  $options['90'] = t('One hour and 30 minutes');
  $options['120'] = t('Two hours');
  $options['150'] = t('Two hours and 30 minutes');
  $options['180'] = t('Three hours');

  // Layout idle
  if (!isset($form_state['storage']['ajax']['settings']['Idle'])) {
    $form_state['storage']['ajax']['settings']['Idle'] = '120';
  }
  $form['ajax']['settings']['Idle'] =
    array('#type' => 'select',
	  '#title' => t('Mobile unit idle timeout'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['settings']['Idle']);

  // Layout enforcer address
  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['subtag'] = 'config';
  $settings['subframe'] = TRUE;
  $settings['subframetitle'] = t('Enforcer address');
  $settings['option']['skipnotrequired'] = TRUE;
  $settings['option']['skipprename'] = TRUE;
  $settings['option']['skipfirstname'] = TRUE;
  $settings['option']['skipmiddlename'] = TRUE;
  $settings['option']['skiplastname'] = TRUE;
  $settings['option']['skipsufname'] = TRUE;
  $settings['option']['skipphone'] = TRUE;
  $settings['option']['skipemail'] = TRUE;
  $settings['option']['skipemailnotification'] = TRUE;
  $settings['option']['skiprole'] = TRUE;
  $settings['option']['skipdepartment'] = TRUE;
  $settings['option']['skiptitle'] = TRUE;
  $settings['option']['skiplogin'] = TRUE;
  if (chpprop_produce_person($form, $form_state, $settings)) {
    return $form;
  }

  // Layout admins
  if (!isset($form_state['storage']['ajax']['addadmin'])) {
    $form_state['storage']['ajax']['addadmin'] = FALSE;
  }
  $form['ajax']['addadmin'] =
    array('#type' => 'checkbox',
	  '#title' => t('Add admin'),
	  '#default_value' => $form_state['storage']['ajax']['addadmin'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));
  if ($form_state['storage']['ajax']['addadmin']) {
    $settings = array();
    $settings['tag'] = 'ajax';
    $settings['subtag'] = 'newadmin';
    $settings['subframe'] = TRUE;
    $settings['subframetitle'] = t('Administrator');
    $settings['option']['skipnotrequired'] = TRUE;
    chpprop_produce_person($form, $form_state, $settings);
  }

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpprop_enforcer_add_form_validate_save'),
	  '#submit' => array('chpprop_enforcer_add_form_submit_save'));
  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_enforcer_add_form_submit_back'));
  return $form;
}

function chpprop_enforcer_add_form_validate_save($form, &$form_state) {
  if (isset($form_state['values']['ajax']['Alias']) and
      empty($form_state['values']['ajax']['Alias'])) {
    form_set_error('ajax][Alias', t('Enforcer field is required.'));
    return;
  }
  if (isset($form_state['values']['Alias']) and
      empty($form_state['values']['Alias'])) {
    form_set_error('Alias', t('Enforcer field is required.'));
    return;
  }
  if (empty($form_state['values']['ajax']['config']['Street'])) {
    form_set_error('ajax][config][Street', t('Street field is required.'));
    return;
  }
  if (empty($form_state['values']['ajax']['config']['City'])) {
    form_set_error('ajax][config][City', t('City field is required.'));
    return;
  }
  if (empty($form_state['values']['ajax']['config']['PostalCode'])) {
    form_set_error('ajax][config][PostalCode', t('State/Province field is required.'));
    return;
  }

  // Check GEO Coding
  include_once drupal_get_path('module', 'location') .'/location.inc';
  $location = array();
  $location['street'] = $form_state['values']['ajax']['config']['Street'];
  $location['city'] = $form_state['values']['ajax']['config']['City'];
  $location['postal_code'] = $form_state['values']['ajax']['config']['PostalCode'];
  $location['province'] = $form_state['values']['ajax']['config']['Province'];
  $location['country'] = $form_state['values']['ajax']['config']['Country'];
  $pos = location_latlon_exact($location);
  if ($pos) {
    $form_state['storage']['Lat'] = $pos['lat'];
    $form_state['storage']['Lon'] = $pos['lon'];
  }

  // New administrator
  if (isset($form_state['values']['ajax']['newadmin'])) {
    if (isset($form_state['values']['ajax']['newadmin']['FirstName']) and
	empty($form_state['values']['ajax']['newadmin']['FirstName'])) {
      form_set_error('ajax][newadmin][FirstName', 'First name field is required.');
      return;
    }
    if (isset($form_state['values']['ajax']['newadmin']['LastName']) and
	empty($form_state['values']['ajax']['newadmin']['LastName'])) {
      form_set_error('ajax][newadmin][LastName', 'Last name field is required.');
      return;
    }
    if (isset($form_state['values']['ajax']['newadmin']['Street']) and
	empty($form_state['values']['ajax']['newadmin']['Street'])) {
      form_set_error('ajax][newadmin][Street', 'Street field for is required.');
      return;
    }
    if (isset($form_state['values']['ajax']['newadmin']['City']) and
	empty($form_state['values']['ajax']['newadmin']['City'])) {
      form_set_error('ajax][newadmin][City', 'City field is required.');
      return;
    }
    if (isset($form_state['values']['ajax']['newadmin']['PostalCode']) and
	empty($form_state['values']['ajax']['newadmin']['PostalCode'])) {
      form_set_error('ajax][newadmin][PostalCode', 'Postal code field is required.');
      return;
    }
    if (isset($form_state['values']['ajax']['newadmin']['Phone']) and
	empty($form_state['values']['ajax']['newadmin']['Phone'])) {
      form_set_error('ajax][newadmin][Phone', 'Phone field is required.');
      return;
    }
    if (isset($form_state['values']['ajax']['newadmin']['Email']) and
	empty($form_state['values']['ajax']['newadmin']['Email'])) {
      form_set_error('ajax][newadmin][Email', 'Email field is required.');
      return;
    }
    if (isset($form_state['values']['ajax']['newadmin']['OtherRole']) and
	empty($form_state['values']['ajax']['newadmin']['OtherRole'])) {
      form_set_error('ajax][newadmin][OtherRole', 'Other role field is required.');
      return;
    }
    if (isset($form_state['values']['ajax']['newadmin']['Department']) and
	empty($form_state['values']['ajax']['newadmin']['Department'])) {
      form_set_error('ajax][newadmin][Department', 'Department field is required.');
      return;
    }
    if (isset($form_state['values']['ajax']['newadmin']['Title']) and
	empty($form_state['values']['ajax']['newadmin']['Title'])) {
      form_set_error('ajax][newadmin][Title', 'Title field is required.');
      return;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Username']) and
	!empty($form_state['values']['ajax']['newadmin']['Username']) and
	empty($form_state['values']['ajax']['newadmin']['Password'])) {
      form_set_error('ajax][newadmin][Password', 'Password field is required.');
      return;
    }

   // Check GEO Coding
    $location = array();
    $location['street'] = $form_state['values']['ajax']['newadmin']['Street'];
    $location['city'] = $form_state['values']['ajax']['newadmin']['City'];
    $location['postal_code'] = $form_state['values']['ajax']['newadmin']['PostalCode'];
    $location['province'] = $form_state['values']['ajax']['newadmin']['Province'];
    $location['country'] = $form_state['values']['ajax']['newadmin']['Country'];
    $pos = location_latlon_exact($location);
    if ($pos) {
      $form_state['storage']['ajax']['newadmin']['Lat'] = $pos['lat'];
      $form_state['storage']['ajax']['newadmin']['Lon'] = $pos['lon'];
    }
  }
}

function chpprop_enforcer_add_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropenfs';
}

function chpprop_enforcer_add_form_submit_save($form, &$form_state) {
  global $user;
  $company_id = $form_state['values']['ajax']['CompanyId'];
  $enforcer_id = chpprop_genRandomString20();
  $alias = check_plain($form_state['values']['ajax']['Alias']);

  if (isset($form_state['values']['ajax']['config']['Street'])) {
    $street = check_plain($form_state['values']['ajax']['config']['Street']);
  } else {
    $street = NULL;
  }

  if (isset($form_state['values']['ajax']['config']['City'])) {
    $city = check_plain($form_state['values']['ajax']['config']['City']);
  } else {
    $city = NULL;
  }

  if (isset($form_state['values']['ajax']['config']['PostalCode'])) {
    $postalcode = check_plain($form_state['values']['ajax']['config']['PostalCode']);
  } else {
    $postalcode = NULL;
  }

  if (isset($form_state['values']['ajax']['config']['Province'])) {
    $province = $form_state['values']['ajax']['config']['Province'];
  } else {
    $province = NULL;
  }

  if (isset($form_state['values']['ajax']['config']['Country'])) {
    $country = chpprop_retrieve_country($form_state['values']['ajax']['config']['Country']);
  } else {
    $country = NULL;
  }

  if (isset($form_state['storage']['Lat'])) {
    $lat = $form_state['storage']['Lat'];
  } else {
    $lat = NULL;
  }

  if (isset($form_state['storage']['Lon'])) {
    $lon = $form_state['storage']['Lon'];
  } else {
    $lon = NULL;
  }

  $session = $form_state['values']['ajax']['settings']['Session'];
  $idle = $form_state['values']['ajax']['settings']['Idle'];

  if (chdbprop_insert_enforcer
      ($user->uid, $company_id, $enforcer_id,
       $alias, $session, $idle,
       $street, $city, $postalcode, $province, $country,
       $id)) {
    drupal_set_message('Couldn\'t insert enforcer', 'error');
    return;
  }

  if (isset($form_state['storage']['Lat'])) {
    if (chdbprop_update_enforcer_latlon
	($id, $lat, $lon)) {
      drupal_set_message('Couldn\'t update enforcer lat/lon', 'error');
      return;
    }
  }

  if (isset($form_state['values']['ajax']['newadmin'])) {
    if (isset($form_state['values']['ajax']['newadmin']['PrenameId']) and
	$form_state['values']['ajax']['newadmin']['PrenameId'] !== 'BLANK') {
      $prename_id = $form_state['values']['ajax']['newadmin']['PrenameId'];
    } else {
      $prename_id = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['FirstName'])) {
      $firstname = check_plain($form_state['values']['ajax']['newadmin']['FirstName']);
    } else {
      $firstname = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['MiddleName'])) {
      $middlename = check_plain($form_state['values']['ajax']['newadmin']['MiddleName']);
    } else {
      $middlename = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['LastName'])) {
      $lastname = check_plain($form_state['values']['ajax']['newadmin']['LastName']);
    } else {
      $lastname = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['SufnameId']) and
	$form_state['values']['ajax']['newadmin']['SufnameId'] !== 'BLANK') {
      $sufname_id = $form_state['values']['ajax']['newadmin']['SufnameId'];
    } else {
      $sufname_id = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Street'])) {
      $street = check_plain($form_state['values']['ajax']['newadmin']['Street']);
    } else {
      $street = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['City'])) {
      $city = check_plain($form_state['values']['ajax']['newadmin']['City']);
    } else {
      $city = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['PostalCode'])) {
      $postalcode = check_plain($form_state['values']['ajax']['newadmin']['PostalCode']);
    } else {
      $postalcode = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Province'])) {
      $province = $form_state['values']['ajax']['newadmin']['Province'];
    } else {
      $province = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Country'])) {
      $country = chpprop_retrieve_country($form_state['values']['ajax']['newadmin']['Country']);
    } else {
      $country = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Phone'])) {
      $phone = check_plain($form_state['values']['ajax']['newadmin']['Phone']);
    } else {
      $phone = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Email'])) {
      $email = check_plain($form_state['values']['ajax']['newadmin']['Email']);
    } else {
      $email = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['EmailNotification'])) {
      $emailnotification = $form_state['values']['ajax']['newadmin']['EmailNotification'];
    } else {
      $emailnotification = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Role']) and
	$form_state['values']['ajax']['newadmin']['Role'] !== 'Other') {
      $role = check_plain($form_state['values']['ajax']['newadmin']['Role']);
    } elseif (isset($form_state['values']['ajax']['newadmin']['Role'])) {
      $role = check_plain($form_state['values']['ajax']['newadmin']['OtherRole']);
    } else {
      $role = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Department'])) {
      $department = check_plain($form_state['values']['ajax']['newadmin']['Department']);
    } else {
      $department = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Title'])) {
      $title = check_plain($form_state['values']['ajax']['newadmin']['Title']);
    } else {
      $title = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Username'])) {
      $username = check_plain($form_state['values']['ajax']['newadmin']['Username']);
    } else {
      $username = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Password'])) {
      $password = check_plain($form_state['values']['ajax']['newadmin']['Password']);
    } else {
      $password = NULL;
    }

    if (isset($form_state['storage']['ajax']['newadmin']['Lat'])) {
      $lat = $form_state['storage']['ajax']['newadmin']['Lat'];
    } else {
      $lat = NULL;
    }

    if (isset($form_state['storage']['ajax']['newadmin']['Lon'])) {
      $lon = $form_state['storage']['ajax']['newadmin']['Lon'];
    } else {
      $lon = NULL;
    }

    $admin_id = chpprop_genRandomString20();

    if (chdbprop_insert_enforceradmin
	($company_id, $enforcer_id, $admin_id, $lat, $lon,
	 $prename_id, $firstname, $middlename, $lastname, $sufname_id,
	 $street, $city, $postalcode, $province, $country,
	 $phone, $email, $emailnotification,
	 $role, $department, $title, $username, $password,
	 $id)) {
      drupal_set_message('Could not insert enforcer admin', 'error');
      return;
    }
  }

  $list = unserialize($form_state['values']['ajax']['property']['List']);
  if (!empty($list)) {
    foreach ($list as $propkey => $values) {
      foreach ($values as $lotkey => $value) {
	if ($value) {
	  if (chdbprop_insert_enforcerprop
	      ($company_id, $enforcer_id,
	       $propkey, $lotkey,
	       $id)) {
	    drupal_set_message('Could not attach to property/lot', 'error');
	    return;
	  }
	}
      }
    }
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropenfs';
}



/**********
 ********** Enforcers
 ********** Address: chppropenfs
 ********** Access:  'anai chp list enforcers'
 *********/



function chpprop_enforcer_form($form_state) {
  if ($form_state['storage']['detailed_view']) {
    return chpprop_enforcer_view_form($form_state);
  }
  if ($form_state['storage']['detailed_edit']) {
    return chpprop_enforcer_edit_form($form_state);
  }

  drupal_set_title(t('Enforcers'));

  global $user;
  $account = user_load(array('uid' => $user->uid));
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['show_companies'] = TRUE;
  $settings['buttons']['back']['submit'] = 'chpprop_enforcer_form_submit_back';
  if (user_access('anai chp add enforcers')) {
    $settings['buttons']['add']['submit'] = 'chpprop_enforcer_form_submit_add';
  }
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];

  // Active table.
  $entries = chpprop_retrieve_enforcers($user->uid, $company_id);
  $form['ajax']['available'] =
    array('#type' => 'fieldset',
	  '#title' => t('Available'));
  if (empty($entries)) {
    $form['ajax']['available']['empty'] = array('#value' => '<p>'.t('Empty.'));
  } else {
    $form['ajax']['available']['list'] = chpprop_enforcer_table_form($entries);
  }

  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    // Suspended table.
    $entries = chpprop_retrieve_enforcers($user->uid, $company_id,
					  TRUE, TRUE, // Active
					  TRUE, TRUE); // Suspended
    if (!empty($entries)) {
      $form['ajax']['suspended'] =
	array('#type' => 'fieldset',
	      '#title' => t('Suspended enforcers'));
      if (count($entries) == 0) {
	$form['ajax']['suspended']['empty'] =
	  array('#value' => '<p>'.t('Empty.'));
      } else {
	$form['ajax']['suspended']['list'] = chpprop_enforcer_table_form($entries);
      }
    }

    // Retention table.
    // Need to loop all since this is where they are managed.
    if (!isset($form_state['storage']['ajax']['Retention'])) {
      $form_state['storage']['ajax']['Retention'] = FALSE;
    }
    $form['ajax']['Retention'] =
      array('#type' => 'checkbox',
	    '#title' => t('Show deleted enforcers'),
	    '#default_value' => $form_state['storage']['ajax']['Retention'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));

    if ($form_state['storage']['ajax']['Retention']) {
      $entries = chpprop_retrieve_enforcers($user->uid, $company_id,
					    TRUE, FALSE, // Active
					    FALSE, TRUE);

      $form['ajax']['retention'] =
	array('#type' => 'fieldset',
	      '#title' => t('Deleted enforcers'));
      if (count($entries) == 0) {
	$form['ajax']['retention']['empty'] =
	  array('#value' => '<p>'.t('Empty.'));
      } else {
	$form['ajax']['retention']['list'] = chpprop_enforcer_table_form($entries);
      }
    }
  }

  return $form;
}

function chpprop_enforcer_form_submit_add($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropenfsadd';
}

function chpprop_enforcer_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpproprel';
}

function chpprop_enforcer_table_form($entries) {
  $form['db'] =
    array('#type' => 'markup', '#value' => '',
	  '#theme' => 'chpprop_enforcer_table');

  if (user_access('anai chp edit enforcers')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }

  $looper = 0;
  foreach ($entries as $entry) {
    $form['db']['alias_'.$looper] =
      array('#value' => decode_entities($entry['data']['Alias']));

    $form['db']['street_'.$looper] =
      array('#value' => decode_entities($entry['data']['Street']));

    $form['db']['city_'.$looper] =
      array('#value' => decode_entities($entry['data']['City']));

    $form['db']['province_'.$looper] =
      array('#value' => decode_entities($entry['data']['Province']));

    $form['db']['country_'.$looper] =
      array('#value' => decode_entities($entry['data']['Country']));

    $form['db']['view_'.$looper] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/viewsmall.png',
	    '#name' => 'op_view'.$entry['data']['Id'],
	    '#submit' => array('chpprop_enforcer_form_submit'));

    if ($readonly or $entry['data']['readonly']) {
      $form['db']['details_'.$looper] = array('#value' => '');
    } else {
      $form['db']['details_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_details'.$entry['data']['Id'],
	      '#submit' => array('chpprop_enforcer_form_submit'));
    }
    $looper++;
  }
  return $form;
}

function theme_chpprop_enforcer_table($form) {
  $header = array(t('Enforcer'), t('Street'), t('City'), t('Province'), t('Country'), '', '');
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'alias_')) {
      continue;
    }
    $looper = substr($key, strlen('alias_'));
    $rows[] = array(drupal_render($form['alias_'.$looper]),
		    drupal_render($form['street_'.$looper]),
		    drupal_render($form['city_'.$looper]),
		    drupal_render($form['province_'.$looper]),
		    drupal_render($form['country_'.$looper]),
		    array('data' => drupal_render($form['view_'.$looper]),
			  'width' => 1, 'height' => 1),
		    array('data' => drupal_render($form['details_'.$looper]),
			  'width' => 1, 'height' => 1),
		    );
  }
  return theme_table($header, $rows);
}

function chpprop_enforcer_view_form($form_state) {
  $id = $form_state['storage']['id'];
  $enforcer = chdbprop_get_enforcer_id($id);
  drupal_set_title(t('Enforcer - COMP', array('COMP' => decode_entities($enforcer['Alias']))));

  $form['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/back.png',
	  '#submit' => array('chpprop_enforcer_view_form_submit_back'));

  $form['t1'] =
    array('#prefix' => '<table border="0"><tr valign="top"><td align="left" width="60%">',
	  '#value' => chpprop_enforcer_view_table($enforcer),
	  '#suffix' => '</td>');
  $form['t2'] =
    array('#prefix' => '<td align="left">',
	  '#value' => chpprop_enforcer_view_map($enforcer),
	  '#suffix' => '</td></tr></table>');

  // Contacts and admins
  $admins = chdbprop_get_enforceradmins($enforcer['CompanyId'], $enforcer['EnforcerId'],
					TRUE, $enforcer['Active'],
					TRUE, $enforcer['Suspended']);
  $form['t3'] =
    array('#type' => 'fieldset',
	  '#title' => t('Contacts and administrators'));
  $form['t3']['list'] = array('#value' => chpprop_name_view_table($admins, $enforcer['EnforcerId']));
  return $form;
}

function chpprop_enforcer_view_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropenfs';
}

function chpprop_enforcer_view_table($entry) {
  $header = array();
  $row[] = array(t('Enforcer name:'), decode_entities($entry['Alias']));
  $row[] = array(t('Street:'), decode_entities($entry['Street']));
  $row[] = array(t('City:'), decode_entities($entry['City']));
  $row[] = array(t('Postal code:'), decode_entities($entry['PostalCode']));
  $row[] = array(t('Province:'), $entry['Province']);
  $row[] = array(t('Country:'), $entry['Country']);

  return theme('table', $header, $row);
}

function chpprop_enforcer_view_map($entry) {
  if (empty($entry['Lat']) or empty($entry['Lon'])) {
    return t('Latitude and Longitude is missing');
  }
  $map_array['id'] = 'LocationMap';
  $map_array['width'] = '400px';
  $map_array['height'] = '260px';
  $map_array['latitude'] = '49.6023';
  $map_array['longitude'] = '-124.9391';
  $map_array['maptype'] = 'Map';
  $map_array['controltype'] = 'Small';
  $map_array['zoom'] = '7';
  $map_array['behavior'] =
    array('locpick' => FALSE,
	  'nodrag' => FALSE,
	  'nokeyboard' => TRUE,
	  'overview' => FALSE,
	  'autozoom' => TRUE,
	  'scale' => FALSE,
	  );
  $text = '<h2>'.decode_entities($entry['Alias']).'</h2>';
  $markers[] = array
    ('text' => $text,
     'latitude' => sprintf("%f", $entry['Lat']),
     'longitude' => sprintf("%f", $entry['Lon']),
     'markername' => 'blue');
  $map_array['markers'] = $markers;
  return theme('gmap', array('#settings' => $map_array));
}

function chpprop_enforcer_edit_form($form_state) {
  if ($form_state['storage']['confirm_delete_enforcer']) {
    return confirm_form($form, 'Are you sure?', 'chppropenfs',
			'The enforcer will be deleted from the system.');
  }

  drupal_set_title(t('Edit enforcer'));
  $id = $form_state['storage']['id'];
  $enforcer = chdbprop_get_enforcer_id($id);
  $company_id = $enforcer['CompanyId'];
  $enforcer_id = $enforcer['EnforcerId'];

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  // Create and validate property options
  $property_options = array();
  $properties = chpprop_retrieve_properties($user->uid, $company_id);
  if (!empty($properties)) {
    foreach ($properties as $property_id => $property) {
      $property_options[$property_id] =
	decode_entities($property['data']['Alias']).', '.
	decode_entities($property['data']['Street']).', '.
	decode_entities($property['data']['City']);
    }
  }
  $properties = NULL;
  if (empty($property_options)) {
    $form['error'] = array('#value' => '<p>'.t('Property missing.').'<p>');
    $form['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpprop_enforcer_edit_form_submit_cancel'));

    return $form;
  }

  // Create and validate policies options, used later for lot selection
  $policies = chpprop_retrieve_policies($user->uid, $company_id);
  if (empty($policies)) {
    $form['error'] = array('#value' => '<p>'.t('Lot missing.').'<p>');
    $form['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpprop_enforcer_edit_form_submit_cancel'));
    return $form;
  }

  if (!$enforcer['Active']) {
    $form['Activate'] =
      array('#type' => 'checkbox',
	    '#title' => t('Activate'),
	    '#default_value' => $enforcer['Active']);
  }

  // Layout suspension option
  $form['Suspended'] =
    array('#type' => 'checkbox',
	  '#title' => t('Suspended'),
	  '#default_value' => $enforcer['Suspended']);

  // Layout enforcer name
  if (!isset($form_state['storage']['Alias'])) {
    $form_state['storage']['Alias'] = decode_entities($enforcer['Alias']);
  }
  $form['Alias'] =
    array('#type' => 'textfield',
	  '#title' => t('Enforcer name'),
	  '#default_value' => $form_state['storage']['Alias'],
	  '#maxlength' => 80);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  // Property/Lot selector
  $form['ajax']['property'] = array('#type' => 'fieldset', '#title' => t('Assignment'));

  // Layout property options
  if (isset($form_state['storage']['ajax']['property']['PropertyId']) and
      !in_array($form_state['storage']['ajax']['property']['PropertyId'],
		array_keys($property_options))) {
    unset($form_state['storage']['ajax']['property']['PropertyId']);
  }
  if (!isset($form_state['storage']['ajax']['property']['PropertyId'])) {
    $form_state['storage']['ajax']['property']['PropertyId'] = key($property_options);
  }
  if (1 < count($property_options)) {
    $form['ajax']['property']['PropertyId'] =
      array('#type' => 'select',
	    '#title' => t('Property'),
	    '#options' => $property_options,
	    '#default_value' => $form_state['storage']['ajax']['property']['PropertyId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
  }

  $property_id = $form_state['storage']['ajax']['property']['PropertyId'];

  // Create and validate lot options based on selected property
  $lot_options = array();
  if (isset($policies[$property_id])) {
    foreach ($policies[$property_id] as $lot_id => $policy) {
      $lot_options[$lot_id] = decode_entities($policy['data']['Alias']);
    }
  }

  if (empty($lot_options)) {
    $form['ajax']['error'] = array('#value' => '<p>'.t('Lot missing.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpmob_list_edit_form_submit_cancel'));
    return $form;
  }

  // Layout lot options
  if (isset($form_state['storage']['ajax']['property']['LotId']) and
      !in_array($form_state['storage']['ajax']['property']['LotId'],
		array_keys($lot_options))) {
    unset($form_state['storage']['ajax']['property']['LotId']);
  }
  if (!isset($form_state['storage']['ajax']['property']['LotId'])) {
    $form_state['storage']['ajax']['property']['LotId'] = key($lot_options);
  }
  if (1 < count($lot_options)) {
    $form['ajax']['property']['LotId'] =
      array('#type' => 'select',
	    '#title' => t('Lot'),
	    '#options' => $lot_options,
	    '#default_value' => $form_state['storage']['ajax']['property']['LotId']);
  }

  $lot_id = $form_state['storage']['ajax']['property']['LotId'];

  if (1 < count($property_options) or 1 < count($lot_options)) {
    $form['ajax']['property']['addbtn'] = array('#type' => 'button', '#value' => t('Add'));
    $form['ajax']['property']['clearbtn'] = array('#type' => 'button', '#value' => t('Clear'));
    $form['ajax']['property']['allbtn'] = array('#type' => 'button', '#value' => t('All'));
    if (!isset($form_state['storage']['ajax']['property']['List'])) {
      $list = array();
      $props = chpprop_retrieve_enforcerprops($user->uid, $company_id, $enforcer_id);
      foreach ($props as $propkey => $values) {
	foreach ($values as $lotkey => $value) {
	  $list[$propkey][$lotkey] = TRUE;
	}
      }
      $form_state['storage']['ajax']['property']['List'] = $list;
    } else {
      $list = unserialize($form_state['storage']['ajax']['property']['List']);
    }

    if (strpos($form_state['clicked_button']['#id'], 'addbtn') !== FALSE) {
      $list[$property_id][$lot_id] = TRUE;
    }
    if (strpos($form_state['clicked_button']['#id'], 'clearbtn') !== FALSE) {
      $list = array();
    }
    if (strpos($form_state['clicked_button']['#id'], 'allbtn') !== FALSE) {
      foreach ($property_options as $propkey => $propvalue) {
	foreach ($policies[$propkey] as $lotkey => $lotvalue) {
	  $list[$propkey][$lotkey] = TRUE;
	}
      }
    }
  } else {
    $list[$property_id][$lot_id] = TRUE;
  }

  $form['ajax']['property']['List'] = array('#type' => 'value', '#value' => serialize($list));

  $header = array(t('Property'), t('Lot'));
  $rows = array();
  foreach ($list as $property_id => $values) {
    foreach ($values as $lot_id => $value) {
      $rows[] = array($property_options[$property_id], $lot_options[$lot_id]);
    }
  }
  $form['ajax']['property']['ListView'] = array('#value' => theme_table($header, $rows));

  // Settings
  $form['ajax']['settings'] = array('#type' => 'fieldset', '#title' => t('Settings'));

  // Create session options
  $options = array();
  $options['60'] = t('One hour');
  $options['120'] = t('Two hours');
  $options['180'] = t('Three hours');
  $options['240'] = t('Four hours');
  $options['300'] = t('Five hours');
  $options['360'] = t('Six hours');
  $options['420'] = t('Seven hours');
  $options['480'] = t('Eight hours');
  $options['540'] = t('Nine hours');
  $options['600'] = t('Ten hours');

  // Layout session
  if (!isset($form_state['storage']['ajax']['settings']['Session'])) {
    $form_state['storage']['ajax']['settings']['Session'] = $enforcer['SessionMinutes'];
  }
  $form['ajax']['settings']['Session'] =
    array('#type' => 'select',
	  '#title' => t('Mobile unit session timeout'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['settings']['Session']);

  // Create idle options
  $options = array();
  $options['15'] = t('15 minutes');
  $options['30'] = t('30 minutes');
  $options['45'] = t('45 minutes');
  $options['60'] = t('One hour');
  $options['90'] = t('One hour and 30 minutes');
  $options['120'] = t('Two hours');
  $options['150'] = t('Two hours and 30 minutes');
  $options['180'] = t('Three hours');

  // Layout idle
  if (!isset($form_state['storage']['ajax']['settings']['Idle'])) {
    $form_state['storage']['ajax']['settings']['Idle'] = $enforcer['IdleMinutes'];
  }
  $form['ajax']['settings']['Idle'] =
    array('#type' => 'select',
	  '#title' => t('Mobile unit idle timeout'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['settings']['Idle']);

  // Layout enforcer address
  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['subtag'] = 'config';
  $settings['subframe'] = TRUE;
  $settings['subframetitle'] = t('Enforcer address');
  $settings['option']['skipnotrequired'] = TRUE;
  $settings['option']['skipprename'] = TRUE;
  $settings['option']['skipfirstname'] = TRUE;
  $settings['option']['skipmiddlename'] = TRUE;
  $settings['option']['skiplastname'] = TRUE;
  $settings['option']['skipsufname'] = TRUE;
  $settings['option']['skipphone'] = TRUE;
  $settings['option']['skipemail'] = TRUE;
  $settings['option']['skipemailnotification'] = TRUE;
  $settings['option']['skiprole'] = TRUE;
  $settings['option']['skipdepartment'] = TRUE;
  $settings['option']['skiptitle'] = TRUE;
  $settings['option']['skiplogin'] = TRUE;
  $settings['default']['Street'] = decode_entities($enforcer['Street']);
  $settings['default']['City'] = decode_entities($enforcer['City']);
  $settings['default']['PostalCode'] = decode_entities($enforcer['PostalCode']);
  $settings['default']['Province'] = $enforcer['Province'];
  $settings['default']['Country'] = $enforcer['Country'];
  chpprop_produce_person($form, $form_state, $settings);

  // Contacts and admins
  $nameopt = chdbadmin_get_nameopt();
  $prenames = array();
  foreach (chdbadmin_get_prenameopt() as $opt) {
    $prenames[$opt['PrenameId']] = decode_entities($opt['Alias']);
  }
  $sufnames = array();
  foreach (chdbadmin_get_sufnameopt() as $opt) {
    $sufnames[$opt['SufnameId']] = decode_entities($opt['Alias']);
  }

  // Layout admins
  $admins = chdbprop_get_enforceradmins($company_id, $enforcer['EnforcerId']);
  foreach ($admins as $key => $admin) {
    $subtag = 'admin'.$admin['Id'].'_'.$admin['AdminId'];

    $name = '';
    $name .= isset($prenames[$admin['PrenameId']]) ? $prenames[$admin['PrenameId']].' ' : '';
    $name .= !empty($admin['FirstName']) ? decode_entities($admin['FirstName']).' ' : '';
    $name .= !empty($admin['MiddleName']) ? decode_entities($admin['MiddleName']).' ' : '';
    $name .= !empty($admin['LastName']) ? decode_entities($admin['LastName']).' ' : '';
    $name .= isset($sufnames[$admin['SufnameId']]) ? $sufnames[$admin['SufnameId']].' ' : '';

    $settings = array();
    $settings['tag'] = 'ajax';
    $settings['subtag'] = $subtag;
    $settings['subframe'] = TRUE;
    $settings['subframetitle'] = t('Admin person: ').$name;
    $settings['subframecollapsebtn'] = TRUE;
    $settings['option']['skipnotrequired'] = TRUE;
    $settings['default']['PrenameId'] = $admin['PrenameId'];
    $settings['default']['FirstName'] = decode_entities($admin['FirstName']);
    $settings['default']['MiddleName'] = decode_entities($admin['MiddleName']);
    $settings['default']['LastName'] = decode_entities($admin['LastName']);
    $settings['default']['SufnameId'] = $admin['SufnameId'];
    $settings['default']['Street'] = decode_entities($admin['Street']);
    $settings['default']['City'] = decode_entities($admin['City']);
    $settings['default']['PostalCode'] = decode_entities($admin['PostalCode']);
    $settings['default']['Province'] = $admin['Province'];
    $settings['default']['Country'] = $admin['Country'];
    $settings['default']['Phone'] = decode_entities($admin['Phone']);
    $settings['default']['Email'] = decode_entities($admin['Email']);
    $settings['default']['EmailNotification'] = $admin['EmailNotification'];
    $settings['default']['Role'] = decode_entities($admin['Role']);
    $settings['default']['Department'] = decode_entities($admin['Department']);
    $settings['default']['Title'] = decode_entities($admin['Title']);
    $settings['default']['Username'] = decode_entities($admin['Username']);
    $settings['default']['Password'] = decode_entities($admin['Password']);
    $settings['default']['LinkAdminId'] = $admin['AdminId'];
    chpprop_produce_person($form, $form_state, $settings);

    if (!isset($form_state['storage']['ajax']['admin'.$admin['AdminId']]['deleteadmin'])) {
      $form_state['storage']['ajax'][$subtag]['deleteadmin'] = FALSE;
    }
    $form['ajax'][$subtag]['deleteadmin'] =
      array('#type' => 'checkbox',
	    '#title' => t('Delete this administrator'),
	    '#default_value' => $form_state['storage']['ajax'][$subtag]['deleteadmin']);
  }

  // Layout new admin
  if (!isset($form_state['storage']['ajax']['addadmin'])) {
    $form_state['storage']['ajax']['addadmin'] = FALSE;
  }
  $form['ajax']['addadmin'] =
    array('#type' => 'checkbox',
	  '#title' => t('Add admin'),
	  '#default_value' => $form_state['storage']['ajax']['addadmin'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));
  if ($form_state['storage']['ajax']['addadmin']) {
    $options = array();
    foreach ($admins as $key => $admin) {
      $name = '';
      $name .= isset($prenames[$admin['PrenameId']]) ? $prenames[$admin['PrenameId']].' ' : '';
      $name .= !empty($admin['FirstName']) ? decode_entities($admin['FirstName']).' ' : '';
      $name .= !empty($admin['MiddleName']) ? decode_entities($admin['MiddleName']).' ' : '';
      $name .= !empty($admin['LastName']) ? decode_entities($admin['LastName']).' ' : '';
      $name .= isset($sufnames[$admin['SufnameId']]) ? $sufnames[$admin['SufnameId']].' ' : '';
      $name = rtrim($name);
      $name .= ', '.decode_entities($admin['Title']).', ';
      $name .= decode_entities($admin['Department']);
      $options[$admin['AdminId']] = $name;
    }
    $settings = array();
    $settings['tag'] = 'ajax';
    $settings['subtag'] = 'newadmin';
    $settings['subframe'] = TRUE;
    $settings['subframetitle'] = t('New administrator');
    $settings['option']['skipnotrequired'] = TRUE;
    $settings['options']['LinkAdmins'] = $options;
    chpprop_produce_person($form, $form_state, $settings);
  }

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpprop_enforcer_add_form_validate_save'),
	  '#submit' => array('chpprop_enforcer_edit_form_submit_save'));

  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_enforcer_edit_form_submit_back'));
  if (user_access('anai chp delete enforcers') and
      $enforcer['Active']) {
    $form['delete'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/delete.png',
	    '#validate' => array('chpprop_enforcer_edit_form_validate_delete'),
	    '#submit' => array('chpprop_enforcer_form_submit'));
  }
  return $form;
}

function chpprop_enforcer_edit_form_validate_delete($form, &$form_state) {
  $form_state['storage']['delete_enforcer'] = TRUE;
}

function chpprop_enforcer_edit_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropenfs';
}

function chpprop_enforcer_edit_form_submit_save($form, &$form_state) {
  global $user;
  $id = $form_state['storage']['id'];
  $enforcer = chdbprop_get_enforcer_id($id);
  $company_id = $enforcer['CompanyId'];
  $enforcer_id = $enforcer['EnforcerId'];

  $alias = check_plain($form_state['values']['Alias']);

  if (isset($form_state['values']['ajax']['config']['Street'])) {
    $street = check_plain($form_state['values']['ajax']['config']['Street']);
  } else {
    $street = NULL;
  }

  if (isset($form_state['values']['ajax']['config']['City'])) {
    $city = check_plain($form_state['values']['ajax']['config']['City']);
  } else {
    $city = NULL;
  }

  if (isset($form_state['values']['ajax']['config']['PostalCode'])) {
    $postalcode = check_plain($form_state['values']['ajax']['config']['PostalCode']);
  } else {
    $postalcode = NULL;
  }

  if (isset($form_state['values']['ajax']['config']['Province'])) {
    $province = $form_state['values']['ajax']['config']['Province'];
  } else {
    $province = NULL;
  }

  if (isset($form_state['values']['ajax']['config']['Country'])) {
    $country = chpprop_retrieve_country($form_state['values']['ajax']['config']['Country']);
  } else {
    $country = NULL;
  }

  if (isset($form_state['storage']['Lat'])) {
    $lat = $form_state['storage']['Lat'];
  } else {
    $lat = NULL;
  }

  if (isset($form_state['storage']['Lon'])) {
    $lon = $form_state['storage']['Lon'];
  } else {
    $lon = NULL;
  }

  $session = $form_state['values']['ajax']['settings']['Session'];
  $idle = $form_state['values']['ajax']['settings']['Idle'];

  if (chdbprop_update_enforcer
      ($id, $alias, $session, $idle,
       $street, $city, $postalcode, $province, $country)) {
    drupal_set_message('Couldn\'t update enforcer', 'error');
    return;
  }

  if (!$enforcer['ManLatLon']) {
    if (chdbprop_update_enforcer_latlon
	($id, $lat, $lon)) {
      drupal_set_message('Couldn\'t update enforcer', 'error');
      return;
    }
  }

  if (isset($form_state['values']['Activate'])) {
    $activate = $form_state['values']['Activate'];
  } else {
    $activate = TRUE;
  }

  if (isset($form_state['values']['Suspended'])) {
    $suspended = $form_state['values']['Suspended'];
  } else {
    $suspended = FALSE;
  }

  if (chdbprop_update_enforcer_status
      ($id,
       $activate,
       $suspended)) {
    drupal_set_message('Could not update enforcer status', 'error');
    return;
  }

  if (chdbprop_update_enforceradmins_status
      ($company_id,
       $enforcer_id,
       $activate,
       $suspended)) {
    drupal_set_message('Could not update enforcer admin status', 'error');
    return;
  }

  // Update administrators
  foreach ($form_state['values']['ajax'] as $key => $value) {
    if (substr($key, 0, strlen('admin')) !== 'admin') {
      continue;
    }
    $id = substr($key, strlen('admin'), strpos($key, '_') - strlen('admin'));
    $admin_id = substr($key, 1 + strpos($key, '_'));
    $subtag = 'admin'.$id.'_'.$admin_id;

    if ($form_state['values']['ajax'][$subtag]['deleteadmin']) {
      if (chdbprop_delete_enforceradmin
	  ($enforcer_id, $admin_id)) {
	drupal_set_message('Could not delete enforcer admin', 'error');
	return;
      }
      continue;
    }

    if (isset($form_state['values']['ajax'][$subtag]['PrenameId']) and
	$form_state['values']['ajax'][$subtag]['PrenameId'] !== 'BLANK') {
      $prename_id = $form_state['values']['ajax'][$subtag]['PrenameId'];
    } else {
      $prename_id = NULL;
    }

    if (isset($form_state['values']['ajax'][$subtag]['FirstName'])) {
      $firstname = check_plain($form_state['values']['ajax'][$subtag]['FirstName']);
    } else {
      $firstname = NULL;
    }

    if (isset($form_state['values']['ajax'][$subtag]['MiddleName'])) {
      $middlename = check_plain($form_state['values']['ajax'][$subtag]['MiddleName']);
    } else {
      $middlename = NULL;
    }

    if (isset($form_state['values']['ajax'][$subtag]['LastName'])) {
      $lastname = check_plain($form_state['values']['ajax'][$subtag]['LastName']);
    } else {
      $lastname = NULL;
    }

    if (isset($form_state['values']['ajax'][$subtag]['SufnameId']) and
	$form_state['values']['ajax'][$subtag]['SufnameId'] !== 'BLANK') {
      $sufname_id = $form_state['values']['ajax'][$subtag]['SufnameId'];
    } else {
      $sufname_id = NULL;
    }

    if (isset($form_state['values']['ajax'][$subtag]['Street'])) {
      $street = check_plain($form_state['values']['ajax'][$subtag]['Street']);
    } else {
      $street = NULL;
    }

    if (isset($form_state['values']['ajax'][$subtag]['City'])) {
      $city = check_plain($form_state['values']['ajax'][$subtag]['City']);
    } else {
      $city = NULL;
    }

    if (isset($form_state['values']['ajax'][$subtag]['PostalCode'])) {
      $postalcode = check_plain($form_state['values']['ajax'][$subtag]['PostalCode']);
    } else {
      $postalcode = NULL;
    }

    if (isset($form_state['values']['ajax'][$subtag]['Province'])) {
      $province = $form_state['values']['ajax'][$subtag]['Province'];
    } else {
      $province = NULL;
    }

    if (isset($form_state['values']['ajax'][$subtag]['Country'])) {
      $country = chpprop_retrieve_country($form_state['values']['ajax'][$subtag]['Country']);
    } else {
      $country = NULL;
    }

    if (isset($form_state['values']['ajax'][$subtag]['Phone'])) {
      $phone = check_plain($form_state['values']['ajax'][$subtag]['Phone']);
    } else {
      $phone = NULL;
    }

    if (isset($form_state['values']['ajax'][$subtag]['Email'])) {
      $email = check_plain($form_state['values']['ajax'][$subtag]['Email']);
    } else {
      $email = NULL;
    }

    if (isset($form_state['values']['ajax'][$subtag]['EmailNotification'])) {
      $emailnotification = $form_state['values']['ajax'][$subtag]['EmailNotification'];
    } else {
      $emailnotification = NULL;
    }

    if (isset($form_state['values']['ajax'][$subtag]['Role']) and
	$form_state['values']['ajax'][$subtag]['Role'] !== 'Other') {
      $role = check_plain($form_state['values']['ajax'][$subtag]['Role']);
    } elseif (isset($form_state['values']['ajax'][$subtag]['Role'])) {
      $role = check_plain($form_state['values']['ajax'][$subtag]['OtherRole']);
    } else {
      $role = NULL;
    }

    if (isset($form_state['values']['ajax'][$subtag]['Department'])) {
      $department = check_plain($form_state['values']['ajax'][$subtag]['Department']);
    } else {
      $department = NULL;
    }

    if (isset($form_state['values']['ajax'][$subtag]['Title'])) {
      $title = check_plain($form_state['values']['ajax'][$subtag]['Title']);
    } else {
      $title = NULL;
    }

    if (isset($form_state['values']['ajax'][$subtag]['Username'])) {
      $username = check_plain($form_state['values']['ajax'][$subtag]['Username']);
    } else {
      $username = NULL;
    }

    if (isset($form_state['values']['ajax'][$subtag]['Password'])) {
      $password = check_plain($form_state['values']['ajax'][$subtag]['Password']);
    } else {
      $password = NULL;
    }

    if (isset($form_state['storage']['ajax'][$subtag]['Lat'])) {
      $lat = $form_state['storage']['ajax'][$subtag]['Lat'];
    } else {
      $lat = NULL;
    }

    if (isset($form_state['storage']['ajax'][$subtag]['Lon'])) {
      $lon = $form_state['storage']['ajax'][$subtag]['Lon'];
    } else {
      $lon = NULL;
    }

    if (chdbprop_update_enforceradmin
	($id,
	 $prename_id, $firstname, $middlename, $lastname, $sufname_id,
	 $street, $city, $postalcode, $province, $country,
	 $phone, $email, $emailnotification,
	 $role, $department, $title, $username, $password)) {
      drupal_set_message('Could not update enforcer admin', 'error');
      return;
    }

    if (chdbprop_update_enforceradmin_latlon
	($id, $lat, $lon)) {
      drupal_set_message('Could not update enforcer admin lat/lon', 'error');
      return;
    }
  }

  // New administrator
  if (isset($form_state['values']['ajax']['newadmin'])) {
    if (isset($form_state['values']['ajax']['newadmin']['PrenameId']) and
	$form_state['values']['ajax']['newadmin']['PrenameId'] !== 'BLANK') {
      $prename_id = $form_state['values']['ajax']['newadmin']['PrenameId'];
    } else {
      $prename_id = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['FirstName'])) {
      $firstname = check_plain($form_state['values']['ajax']['newadmin']['FirstName']);
    } else {
      $firstname = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['MiddleName'])) {
      $middlename = check_plain($form_state['values']['ajax']['newadmin']['MiddleName']);
    } else {
      $middlename = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['LastName'])) {
      $lastname = check_plain($form_state['values']['ajax']['newadmin']['LastName']);
    } else {
      $lastname = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['SufnameId']) and
	$form_state['values']['ajax']['newadmin']['SufnameId'] !== 'BLANK') {
      $sufname_id = $form_state['values']['ajax']['newadmin']['SufnameId'];
    } else {
      $sufname_id = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Street'])) {
      $street = check_plain($form_state['values']['ajax']['newadmin']['Street']);
    } else {
      $street = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['City'])) {
      $city = check_plain($form_state['values']['ajax']['newadmin']['City']);
    } else {
      $city = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['PostalCode'])) {
      $postalcode = check_plain($form_state['values']['ajax']['newadmin']['PostalCode']);
    } else {
      $postalcode = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Province'])) {
      $province = $form_state['values']['ajax']['newadmin']['Province'];
    } else {
      $province = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Country'])) {
      $country = chpprop_retrieve_country($form_state['values']['ajax']['newadmin']['Country']);
    } else {
      $country = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Phone'])) {
      $phone = check_plain($form_state['values']['ajax']['newadmin']['Phone']);
    } else {
      $phone = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Email'])) {
      $email = check_plain($form_state['values']['ajax']['newadmin']['Email']);
    } else {
      $email = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['EmailNotification'])) {
      $emailnotification = $form_state['values']['ajax']['newadmin']['EmailNotification'];
    } else {
      $emailnotification = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Role']) and
	$form_state['values']['ajax']['newadmin']['Role'] !== 'Other') {
      $role = check_plain($form_state['values']['ajax']['newadmin']['Role']);
    } elseif (isset($form_state['values']['ajax']['newadmin']['Role'])) {
      $role = check_plain($form_state['values']['ajax']['newadmin']['OtherRole']);
    } else {
      $role = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Department'])) {
      $department = check_plain($form_state['values']['ajax']['newadmin']['Department']);
    } else {
      $department = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Title'])) {
      $title = check_plain($form_state['values']['ajax']['newadmin']['Title']);
    } else {
      $title = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Username'])) {
      $username = check_plain($form_state['values']['ajax']['newadmin']['Username']);
    } else {
      $username = NULL;
    }

    if (isset($form_state['values']['ajax']['newadmin']['Password'])) {
      $password = check_plain($form_state['values']['ajax']['newadmin']['Password']);
    } else {
      $password = NULL;
    }

    if (isset($form_state['storage']['ajax']['newadmin']['Lat'])) {
      $lat = $form_state['storage']['ajax']['newadmin']['Lat'];
    } else {
      $lat = NULL;
    }

    if (isset($form_state['storage']['ajax']['newadmin']['Lon'])) {
      $lon = $form_state['storage']['ajax']['newadmin']['Lon'];
    } else {
      $lon = NULL;
    }

    $admin_id = chpprop_genRandomString20();

    if (chdbprop_insert_enforceradmin
	($company_id, $enforcer_id, $admin_id, $lat, $lon,
	 $prename_id, $firstname, $middlename, $lastname, $sufname_id,
	 $street, $city, $postalcode, $province, $country,
	 $phone, $email, $emailnotification,
	 $role, $department, $title, $username, $password,
	 $id)) {
      drupal_set_message('Could not insert enforcer admin', 'error');
      return;
    }
  }

  $list = unserialize($form_state['values']['ajax']['property']['List']);

  // Delete property assignments for those not in new list
  $props = chpprop_retrieve_enforcerprops($user->uid, $company_id, $enforcer_id);
  //anai_dbg($props, '$props');

  foreach ($props as $propkey => $values) {
    foreach ($values as $lotkey => $value) {
      if (isset($list[$propkey][$lotkey])) {
	// Mark as not needing insertion
	$list[$propkey][$lotkey] = FALSE;
      } else {
	if (chdbprop_delete_enforcerprop
	    ($company_id, $enforcer_id,
	     $propkey, $lotkey)) {
	  drupal_set_message('Could not delete from property/lot', 'error');
	  return;
	}
      }
    }
  }

  // Insert new assigned properties
  if (!empty($list)) {
    foreach ($list as $propkey => $values) {
      foreach ($values as $lotkey => $value) {
	if ($value) {
	  if (chdbprop_insert_enforcerprop
	      ($company_id, $enforcer_id,
	       $propkey, $lotkey,
	       $id)) {
	    drupal_set_message('Could not attach to property/lot', 'error');
	    return;
	  }
	}
      }
    }
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropenfs';
}

function chpprop_enforcer_form_submit($form, &$form_state) {
  global $user;
  if ($form_state['storage']['confirm_delete_enforcer']) {
    $id = $form_state['storage']['id'];
    $enforcer = chdbprop_get_enforcer_id($id);
    // Retire the family all the way down to permits
    $reason = t('Enforcer deleted');
    chdbprop_retire_enforcer_id
      ($id, $user->uid, t('Enforcer deleted'));
    chdbprop_retire_enforceradmins
      ($enforcer['CompanyId'], $enforcer['EnforcerId'], $user->uid, $reason);
    unset($form_state['storage']);
    $form_state['redirect'] = 'chppropenfs';
    return;
  }
  if ($form_state['storage']['delete_enforcer']) {
    $form_state['storage']['confirm_delete_enforcer'] = TRUE;
    return;
  }
  if ($form_state['storage']['save']) {
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
    $fill = TRUE;
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
    $fill = TRUE;
  }
}



