<?php
require_once("anai_chpproperties.inc");



/**********
 ********** Client add
 ********** Address: chppropclientadd
 ********** Access:  'anai chp add clients'
 *********/



function chpprop_client_add_form($form_state) {
  drupal_set_title(t('Add client'));

  global $user;
  $account = user_load(array('uid' => $user->uid));
  $form = array();
  ahah_helper_register($form, $form_state);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  $options = array();
  // As Admin, get all regardless of relations
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_get_all_companies() as $company) {
      $options[$company['CompanyId']] =
	decode_entities($company['Alias']).', '.
	decode_entities($company['Street']).', '.
	decode_entities($company['City']);
    }
  } else {
    foreach (chpprop_retrieve_companies($user->uid) as $company_id => $company) {
      $options[$company_id] =
	decode_entities($company['data']['Alias']).', '.
	decode_entities($company['data']['Street']).', '.
	decode_entities($company['data']['City']);
    }
  }

  if (empty($options)) {
    $form['ajax']['error'] =
      array('#value' => '<p>'.t('Companies missing.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpprop_client_add_form_submit_back'));
    return $form;
  }

  if (!isset($form_state['storage']['ajax']['CompanyId'])) {
    if (isset($_SESSION['anai']['TemporaryCompanyId']) and
	in_array($_SESSION['anai']['TemporaryCompanyId'], array_keys($options))) {
      $form_state['storage']['ajax']['CompanyId'] = $_SESSION['anai']['TemporaryCompanyId'];
    } else {
      $form_state['storage']['ajax']['CompanyId'] = key($options);
    }
  }

  if (1 == count($options)) {
    $form['ajax']['CompanyId'] =
      array('#type' => 'hidden',
	    '#value' => $form_state['storage']['ajax']['CompanyId']);
  } else {
    $form['ajax']['CompanyId'] =
      array('#type' => 'select',
	    '#title' => t('Company'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['CompanyId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
  }

  $options = array();
  $options['ANY'] = t('Any region');
  // As Admin, get all regardless of relations
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_get_regions($form_state['storage']['ajax']['CompanyId']) as $region) {
      $options[$region['RegionId']] =
	decode_entities($region['Alias']).', '.
	decode_entities($region['Street']).', '.
	decode_entities($region['City']);
    }
  } else {
    $regions = chpprop_retrieve_regions($user->uid);
    if (isset($regions[$form_state['storage']['ajax']['CompanyId']])) {
      foreach ($regions[$form_state['storage']['ajax']['CompanyId']] as $region_id => $region) {
	$options[$region_id] =
	  decode_entities($region['data']['Alias']).', '.
	  decode_entities($region['data']['Street']).', '.
	  decode_entities($region['data']['City']);
      }
    }
  }

  if (!isset($form_state['storage']['ajax']['RegionId'])) {
    if (isset($_SESSION['anai']['TemporaryRegionId']) and
	in_array($_SESSION['anai']['TemporaryRegionId'], array_keys($options))) {
      $form_state['storage']['ajax']['RegionId'] = $_SESSION['anai']['TemporaryRegionId'];
    } else {
      $form_state['storage']['ajax']['RegionId'] = key($options);
    }
  }

  if (1 == count($options)) {
    $form['ajax']['RegionId'] =
      array('#type' => 'hidden',
	    '#value' => $form_state['storage']['ajax']['RegionId']);
  } else {
    $form['ajax']['RegionId'] =
      array('#type' => 'select',
	    '#title' => t('Region'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['RegionId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
  }

  if (!isset($form_state['storage']['ajax']['Alias'])) {
    $form_state['storage']['ajax']['Alias'] = '';
  }
  $form['ajax']['Alias'] =
    array('#type' => 'textfield',
	  '#title' => t('Client name'),
	  '#default_value' => $form_state['storage']['ajax']['Alias'],
	  '#maxlength' => 80);

  // Layout client address
  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['subtag'] = 'config';
  $settings['subframe'] = TRUE;
  $settings['subframetitle'] = t('Client address');
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
	  '#validate' => array('chpprop_client_add_form_validate_save'),
	  '#submit' => array('chpprop_client_add_form_submit_save'));
  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_client_add_form_submit_back'));
  return $form;
}

function chpprop_client_add_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['ajax']['Alias'])) {
    form_set_error('ajax][Alias', t('Client name field is required.'));
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
      form_set_error('ajax][newadmin][Street', 'Street field is required.');
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

function chpprop_client_add_form_submit_back($form, &$form_state) {
  $_SESSION['anai']['TemporaryCompanyId'] = $form_state['values']['ajax']['CompanyId'];
  $_SESSION['anai']['TemporaryRegionId'] = $form_state['values']['ajax']['RegionId'];
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropclient';
}

function chpprop_client_add_form_submit_save($form, &$form_state) {
  $_SESSION['anai']['TemporaryCompanyId'] = $form_state['values']['ajax']['CompanyId'];
  $_SESSION['anai']['TemporaryRegionId'] = $form_state['values']['ajax']['RegionId'];
  global $user;
  $company_id = $form_state['values']['ajax']['CompanyId'];
  $region_id = $form_state['values']['ajax']['RegionId'];
  $client_id = chpprop_genRandomString20();
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

  if (chdbprop_insert_client
      ($user->uid, $company_id, $region_id, $client_id, $alias,
       $street, $city, $postalcode, $province, $country,
       $id)) {
    drupal_set_message('Couldn\'t insert client', 'error');
    return;
  }

  if (isset($form_state['storage']['Lat'])) {
    if (chdbprop_update_client_latlon
	($id, $lat, $lon)) {
      drupal_set_message('Couldn\'t update client lat/lon', 'error');
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

    if (chdbprop_insert_clientadmin
	($company_id, $client_id, $admin_id, $lat, $lon,
	 $prename_id, $firstname, $middlename, $lastname, $sufname_id,
	 $street, $city, $postalcode, $province, $country,
	 $phone, $email, $emailnotification,
	 $role, $department, $title, $username, $password,
	 $id)) {
      drupal_set_message('Could not insert client admin', 'error');
      return;
    }
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropclient';
}



/**********
 ********** Client list
 ********** Address: chppropclient
 ********** Access:  'anai chp list clients'
 *********/



function chpprop_client_form($form_state) {
  if ($form_state['storage']['detailed_view']) {
    return chpprop_client_view_form($form_state);
  }
  if ($form_state['storage']['detailed_edit']) {
    return chpprop_client_edit_form($form_state);
  }

  drupal_set_title(t('Clients'));

  global $user;
  $account = user_load(array('uid' => $user->uid));
  $form = array();
  ahah_helper_register($form, $form_state);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  $form['ajax']['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/back.png',
	  '#submit' => array('chpprop_client_form_submit_back'));

  $company_options = array();
  // As Admin, get all regardless of relations
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_get_all_companies() as $company) {
      $company_options[$company['CompanyId']] =
	decode_entities($company['Alias']).', '.
	decode_entities($company['Street']).', '.
	decode_entities($company['City']);
    }
  } else {
    foreach (chpprop_retrieve_companies($user->uid) as $company_id => $company) {
      $company_options[$company_id] =
	decode_entities($company['data']['Alias']).', '.
	decode_entities($company['data']['Street']).', '.
	decode_entities($company['data']['City']);
    }
  }

  if (empty($company_options)) {
    $form['ajax']['error'] =
      array('#value' => '<p>'.t('Companies missing.').'<p>');
    return $form;
  } 

  if (!isset($form_state['storage']['ajax']['CompanyId'])) {
    if (isset($_SESSION['anai']['TemporaryCompanyId']) and
	in_array($_SESSION['anai']['TemporaryCompanyId'], array_keys($company_options))) {
      $form_state['storage']['ajax']['CompanyId'] = $_SESSION['anai']['TemporaryCompanyId'];
    } else {
      $form_state['storage']['ajax']['CompanyId'] = key($company_options);
    }
  }

  $region_options = array();
  $region_options['ANY'] = t('Any region');
  // As Admin, get all regardless of relations
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_get_regions($form_state['storage']['ajax']['CompanyId']) as $region) {
      $region_options[$region['RegionId']] =
	decode_entities($region['Alias']).', '.
	decode_entities($region['Street']).', '.
	decode_entities($region['City']);
    }
  } else {
    $regions = chpprop_retrieve_regions($user->uid);
    if (isset($regions[$form_state['storage']['ajax']['CompanyId']])) {
      foreach ($regions[$form_state['storage']['ajax']['CompanyId']] as $region_id => $region) {
	$region_options[$region_id] =
	  decode_entities($region['data']['Alias']).', '.
	  decode_entities($region['data']['Street']).', '.
	  decode_entities($region['data']['City']);
      }
    }
  }

  if (user_access('anai chp add clients')) {
    $form['ajax']['add'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/add.png',
	    '#submit' => array('chpprop_client_form_submit_add'));
  }

  if (isset($form_state['storage']['ajax']['RegionId']) and
      !in_array($form_state['storage']['ajax']['RegionId'], array_keys($region_options))) {
    unset($form_state['storage']['ajax']['RegionId']);
  }
  if (!isset($form_state['storage']['ajax']['RegionId'])) {
    if (isset($_SESSION['anai']['TemporaryRegionId']) and
	in_array($_SESSION['anai']['TemporaryRegionId'], array_keys($region_options))) {
      $form_state['storage']['ajax']['RegionId'] = $_SESSION['anai']['TemporaryRegionId'];
    } else {
      $form_state['storage']['ajax']['RegionId'] = key($region_options);
    }
  }

  if (1 == count($company_options)) {
    $form['ajax']['CompanyId'] =
      array('#type' => 'hidden',
	    '#value' => $form_state['storage']['ajax']['CompanyId']);
  } else {
    $form['ajax']['CompanyId'] =
      array('#type' => 'select',
	    '#title' => t('Company'),
	    '#options' => $company_options,
	    '#default_value' => $form_state['storage']['ajax']['CompanyId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
  }

  if (1 == count($region_options)) {
    $form['ajax']['RegionId'] =
      array('#type' => 'hidden',
	    '#value' => $form_state['storage']['ajax']['RegionId']);
  } else {
    $form['ajax']['RegionId'] =
      array('#type' => 'select',
	    '#title' => t('Region'),
	    '#options' => $region_options,
	    '#default_value' => $form_state['storage']['ajax']['RegionId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
  }

  $form['ajax']['available'] =
    array('#type' => 'fieldset',
	  '#title' => t('Available'));
  // Active table
  // As Admin, get all regardless of relations
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    $entries = array();
    foreach (chdbprop_get_clients($form_state['storage']['ajax']['CompanyId'],
				  $form_state['storage']['ajax']['RegionId'])
	     as $client) {
      $entries[$client['ClientId']]['data'] = $client;
    }
  } else {
    $entries = chpprop_retrieve_clients($user->uid); //anai_dbg($entries);
    // Filter out selected company
    if (isset($entries[$form_state['storage']['ajax']['CompanyId']])) {
      $entries = $entries[$form_state['storage']['ajax']['CompanyId']];
    } else {
      $entries = array();
    }
    // Filter out selected region if other than 'ANY'
    if ($form_state['storage']['ajax']['RegionId'] === 'ANY') {
      // 'ANY' means just remove whatever RegionId
      $tmp = array();
      foreach ($entries as $dontcarekey => $values) {
	foreach ($values as $key => $value) {
	  $tmp[$key] = $value;
	}
      }
      $entries = $tmp;
      $tmp = NULL;
    } else { // Filter on selected RegionId
      if (isset($entries[$form_state['storage']['ajax']['RegionId']]) and isset($entries['ANY'])) {
	$entries = array_merge($entries[$form_state['storage']['ajax']['RegionId']],
			       $entries['ANY']);
      } elseif (isset($entries[$form_state['storage']['ajax']['RegionId']])) {
	$entries = $entries[$form_state['storage']['ajax']['RegionId']];
      } else {
	$entries = array();
      }
    }
  }
  if (empty($entries)) {
    $form['ajax']['available']['empty'] = array('#value' => '<p>'.t('Empty.'));
  } else {
    $form['ajax']['available']['list'] = chpprop_client_table_form($entries);
  }

  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    // Suspended table
    $entries = array();
    foreach (chdbprop_get_clients($form_state['storage']['ajax']['CompanyId'],
				  $form_state['storage']['ajax']['RegionId'],
				  TRUE, TRUE, // Active
				  TRUE, TRUE) // Suspended
	     as $client) {
      $entries[$client['ClientId']]['data'] = $client;
    }

    if (!empty($entries)) {
      $form['ajax']['suspended'] =
	array('#type' => 'fieldset',
	      '#title' => t('Suspended clients'));
      $form['ajax']['suspended']['list'] = chpprop_client_table_form($entries);
    }

    // Retention table
    if (!isset($form_state['storage']['ajax']['Retention'])) {
      $form_state['storage']['ajax']['Retention'] = FALSE;
    }
    $form['ajax']['Retention'] =
      array('#type' => 'checkbox',
	    '#title' => t('Show deleted clients'),
	    '#default_value' => $form_state['storage']['ajax']['Retention'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
    if ($form_state['storage']['ajax']['Retention']) {
      $entries = array();
      foreach (chdbprop_get_clients($form_state['storage']['ajax']['CompanyId'],
				    $form_state['storage']['ajax']['RegionId'],
				    TRUE, FALSE, // Active
				    FALSE, TRUE) // Suspended
	       as $client) {
	$entries[$client['ClientId']]['data'] = $client;
      }
      $form['ajax']['retention'] =
	array('#type' => 'fieldset',
	      '#title' => t('Deleted clients'));
      if (empty($entries)) {
	$form['ajax']['retention']['empty'] = array('#value' => '<p>'.t('Empty.'));
      } else {
	$form['ajax']['retention']['list'] = chpprop_client_table_form($entries);
      }
    }
  }

  return $form;
}

function chpprop_client_form_submit_add($form, &$form_state) {
  $_SESSION['anai']['TemporaryCompanyId'] = $form_state['values']['ajax']['CompanyId'];
  $_SESSION['anai']['TemporaryRegionId'] = $form_state['values']['ajax']['RegionId'];
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropclientadd';
}

function chpprop_client_form_submit_back($form, &$form_state) {
  $_SESSION['anai']['TemporaryCompanyId'] = $form_state['values']['ajax']['CompanyId'];
  $_SESSION['anai']['TemporaryRegionId'] = $form_state['values']['ajax']['RegionId'];
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpproprel';
}

function chpprop_client_table_form($entries) {
  $form['db'] =
    array('#type' => 'markup', '#value' => '',
	  '#theme' => 'chpprop_client_table');

  if (user_access('anai chp edit clients')) {
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
	    '#submit' => array('chpprop_client_form_submit'));

    if ($readonly or $entry['readonly']) {
      $form['db']['details_'.$looper] = array('#value' => '');
    } else {
      $form['db']['details_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_details'.$entry['data']['Id'],
	      '#submit' => array('chpprop_client_form_submit'));
    }
    $looper++;
  }
  return $form;
}

function theme_chpprop_client_table($form) {
  $header = array(t('Client name'), t('Street'), t('City'), t('Province'), t('Country'), '', '');
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

function chpprop_client_view_form($form_state) {
  $id = $form_state['storage']['id'];
  $client = chdbprop_get_client_id($id);
  drupal_set_title(t('Client - COMP', array('COMP' => decode_entities($client['Alias']))));

  $form['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/back.png',
	  '#submit' => array('chpprop_client_view_form_submit_back'));

  $form['t1'] =
    array('#prefix' => '<table border="0"><tr valign="top"><td align="left" width="60%">',
	  '#value' => chpprop_client_view_table($client),
	  '#suffix' => '</td>');
  $form['t2'] =
    array('#prefix' => '<td align="left">',
	  '#value' => chpprop_client_view_map($client),
	  '#suffix' => '</td></tr></table>');

  // Contacts and admins
  $admins = chdbprop_get_clientadmins($client['CompanyId'], $client['ClientId'],
				      TRUE, $client['Active'],
				      TRUE, $client['Suspended']);
  $form['t3'] =
    array('#type' => 'fieldset',
	  '#title' => t('Contacts and administrators'));
  $form['t3']['list'] = array('#value' => chpprop_name_view_table($admins, $client['CompanyId']));
  return $form;
}

function chpprop_client_view_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropclient';
}

function chpprop_client_view_table($entry) {
  $header = array();
  $row[] = array(t('Client name:'), decode_entities($entry['Alias']));
  $row[] = array(t('Street:'), decode_entities($entry['Street']));
  $row[] = array(t('City:'), decode_entities($entry['City']));
  $row[] = array(t('Postal code:'), decode_entities($entry['PostalCode']));
  $row[] = array(t('Province:'), $entry['Province']);
  $row[] = array(t('Country:'), $entry['Country']);

  $company = chdbprop_get_company($entry['CompanyId']);
  $row[] = array(t('Parent company:'), decode_entities($company['Alias']));

  if ($entry['RegionId'] === 'ANY') {
    $row[] = array(t('Parent region:'), t('Any region'));
  } else {
    $region = chdbprop_get_region($entry['CompanyId'], $entry['RegionId']);
    $row[] = array(t('Parent region:'), decode_entities($region['Alias']));
  }

  global $user;
  $properties = chpprop_retrieve_properties($user->uid, $entry['CompanyId']);
  $row[] = array(t('Number of properties:'), count($properties));

  $i = 1;
  foreach ($properties as $property_id => $property) {
    $row[] = array(t('Property #NUM:', array('NUM' => $i++)), decode_entities($property['data']['Alias']));
  }

  return theme('table', $header, $row);
}

function chpprop_client_view_map($entry) {
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

function chpprop_client_edit_form($form_state) {
  if ($form_state['storage']['confirm_delete_client']) {
    return confirm_form($form, 'Are you sure?', 'chppropclient',
			'The client will be deleted from the system.');
  }

  drupal_set_title(t('Edit client'));
  $id = $form_state['storage']['id'];
  $client = chdbprop_get_client_id($id);

  global $user;
  $account = user_load(array('uid' => $user->uid));
  $form = array();
  ahah_helper_register($form, $form_state);

  if (!$client['Active']) {
    $form['Activate'] =
      array('#type' => 'checkbox',
	    '#title' => t('Activate'),
	    '#default_value' => $client['Active']);
  }

  $form['Suspended'] =
    array('#type' => 'checkbox',
	  '#title' => t('Suspended'),
	  '#default_value' => $client['Suspended']);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  $region_options = array();
  $region_options['ANY'] = t('Any region');
  // As Admin, get all regardless of relations
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_get_regions($form_state['storage']['ajax']['CompanyId']) as $region) {
      $region_options[$region['RegionId']] =
	decode_entities($region['Alias']).', '.
	decode_entities($region['Street']).', '.
	decode_entities($region['City']);
    }
  } else {
    $regions = chpprop_retrieve_regions($user->uid);
    if (isset($regions[$form_state['storage']['ajax']['CompanyId']])) {
      foreach ($regions[$form_state['storage']['ajax']['CompanyId']] as $region_id => $region) {
	$region_options[$region_id] =
	  decode_entities($region['data']['Alias']).', '.
	  decode_entities($region['data']['Street']).', '.
	  decode_entities($region['data']['City']);
      }
    }
  }

  if (!isset($form_state['storage']['ajax']['RegionId'])) {
    $form_state['storage']['ajax']['RegionId'] = $client['RegionId'];
  }
  if (1 == count($region_options)) {
    $form['ajax']['RegionId'] =
      array('#type' => 'hidden',
	    '#value' => $form_state['storage']['ajax']['RegionId']);
  } else {
    $form['ajax']['RegionId'] =
      array('#type' => 'select',
	    '#title' => t('Region'),
	    '#options' => $region_options,
	    '#default_value' => $form_state['storage']['ajax']['RegionId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
  }

  if (!isset($form_state['storage']['ajax']['Alias'])) {
    $form_state['storage']['ajax']['Alias'] = decode_entities($client['Alias']);
  }
  $form['ajax']['Alias'] =
    array('#type' => 'textfield',
	  '#title' => t('Client name'),
	  '#default_value' => $form_state['storage']['ajax']['Alias'],
	  '#maxlength' => 80);

  // Layout client address
  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['subtag'] = 'config';
  $settings['subframe'] = TRUE;
  $settings['subframetitle'] = t('Client address');
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
  $settings['default']['Street'] = decode_entities($client['Street']);
  $settings['default']['City'] = decode_entities($client['City']);
  $settings['default']['PostalCode'] = decode_entities($client['PostalCode']);
  $settings['default']['Province'] = $client['Province'];
  $settings['default']['Country'] = $client['Country'];
  chpprop_produce_person($form, $form_state, $settings);
  $prenameoption = $form_state['storage']['ajax']['config']['PreNameOptions'];
  $sufnameoption = $form_state['storage']['ajax']['config']['SufNameOptions'];

  // Layout admins
  $admins = chdbprop_get_clientadmins($client['CompanyId'], $client['ClientId']);
  foreach ($admins as $key => $admin) {
    $subtag = 'admin'.$admin['Id'].'_'.$admin['AdminId'];

    $name = '';
    $name .= isset($prenameoption[$admin['PrenameId']]) ? $prenameoption[$admin['PrenameId']].' ' : '';
    $name .= isset($admin['FirstName']) ? decode_entities($admin['FirstName']).' ' : '';
    $name .= isset($admin['MiddleName']) ? decode_entities($admin['MiddleName']).' ' : '';
    $name .= isset($admin['LastName']) ? decode_entities($admin['LastName']).' ' : '';
    $name .= isset($sufnameoption[$admin['SufnameId']]) ? $sufnameoption[$admin['SufnameId']].' ' : '';

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
    $settings = array();
    $settings['tag'] = 'ajax';
    $settings['subtag'] = 'newadmin';
    $settings['subframe'] = TRUE;
    $settings['subframetitle'] = t('New administrator');
    $settings['option']['skipnotrequired'] = TRUE;
    chpprop_produce_person($form, $form_state, $settings);
  }

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpprop_client_add_form_validate_save'),
	  '#submit' => array('chpprop_client_edit_form_submit_save'));

  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_client_edit_form_submit_back'));
  if (user_access('anai chp delete clients') and
      $client['Active']) {
    $form['delete'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/delete.png',
	    '#validate' => array('chpprop_client_edit_form_validate_delete'),
	    '#submit' => array('chpprop_client_form_submit'));
  }
  return $form;
}

function chpprop_client_edit_form_validate_delete($form, &$form_state) {
  $form_state['storage']['delete_client'] = TRUE;
}

function chpprop_client_edit_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropclient';
}

function chpprop_client_edit_form_submit_save($form, &$form_state) {
  $id = $form_state['storage']['id'];
  $client = chdbprop_get_client_id($id);
  $company_id = $client['CompanyId'];
  $region_id = $form_state['values']['ajax']['RegionId'];
  $client_id = $client['ClientId'];

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

  if (chdbprop_update_client
      ($client['Id'], $alias, $region_id,
       $street, $city, $postalcode, $province, $country)) {
    drupal_set_message('Couldn\'t update client', 'error');
    return;
  }

  if (!$client['ManLatLon']) {
    if (chdbprop_update_client_latlon
	($id, $lat, $lon)) {
      drupal_set_message('Couldn\'t update client', 'error');
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

  if (chdbprop_update_client_status
      ($id,
       $activate,
       $suspended)) {
    drupal_set_message('Could not update client status', 'error');
    return;
  }

  if (chdbprop_update_clientadmins_status
      ($company_id, $client_id,
       $activate,
       $suspended)) {
    drupal_set_message('Could not update client admin status', 'error');
    return;
  }

  // TBD for all other subling status

  // Update administrators
  foreach ($form_state['values']['ajax'] as $key => $value) {
    if (substr($key, 0, strlen('admin')) !== 'admin') {
      continue;
    }
    $id = substr($key, strlen('admin'), strpos($key, '_') - strlen('admin'));
    $admin_id = substr($key, 1 + strpos($key, '_'));
    $subtag = 'admin'.$id.'_'.$admin_id;

    if ($form_state['values']['ajax'][$subtag]['deleteadmin']) {
      if (chdbprop_delete_clientadmin_id($id)) {
	drupal_set_message('Could not delete client admin', 'error');
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

    if (chdbprop_update_clientadmin
	($id,
	 $prename_id, $firstname, $middlename, $lastname, $sufname_id,
	 $street, $city, $postalcode, $province, $country,
	 $phone, $email, $emailnotification,
	 $role, $department, $title, $username, $password)) {
      drupal_set_message('Could not update client admin', 'error');
      return;
    }

    if (chdbprop_update_clientadmin_latlon
	($id, $lat, $lon)) {
      drupal_set_message('Could not update client admin lat/lon', 'error');
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

    if (chdbprop_insert_clientadmin
	($company_id, $client_id, $admin_id, $lat, $lon,
	 $prename_id, $firstname, $middlename, $lastname, $sufname_id,
	 $street, $city, $postalcode, $province, $country,
	 $phone, $email, $emailnotification,
	 $role, $department, $title, $username, $password,
	 $id)) {
      drupal_set_message('Could not insert client admin', 'error');
      return;
    }
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropclient';
}

function chpprop_client_form_submit($form, &$form_state) {
  global $user;
  if ($form_state['storage']['confirm_delete_client']) {
    $id = $form_state['storage']['id'];
    $client = chdbprop_get_client_id($id);
    // Retire the family all the way down to permits
    $reason = t('Client deleted');
    chdbprop_retire_client_id
      ($id, $user->uid, $reason);
    chdbprop_retire_clientadmins
      ($client['CompanyId'], $client['ClientId'], $user->uid, $reason);
    //TBD for all groups
    //TBD for all groupadmins
    //TBD for all permits
    unset($form_state['storage']);
    $form_state['redirect'] = 'chppropclient';
    return;
  }
  if ($form_state['storage']['delete_client']) {
    $form_state['storage']['confirm_delete_client'] = TRUE;
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



