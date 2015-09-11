<?php
require_once("anai_chpproperties.inc");



/**********
 ********** Company add
 ********** Address: chppropcompadd
 ********** Access:  'anai chp add companies'
 *********/



function chpprop_company_add_form($form_state) {
  drupal_set_title(t('Add company'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  if (!isset($form_state['storage']['Alias'])) {
    $form_state['storage']['Alias'] = '';
  }
  $form['Alias'] =
    array('#type' => 'textfield',
	  '#title' => t('Company name'),
	  '#default_value' => $form_state['storage']['Alias'],
	  '#maxlength' => 80);

  // Layout company address
  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['subtag'] = 'config';
  $settings['subframe'] = TRUE;
  $settings['subframetitle'] = t('Company address');
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
  chpprop_produce_person($form, $form_state, $settings);

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
    $settings['subframetitle'] = t('Contacts and administrators');
    $settings['option']['skipnotrequired'] = TRUE;
    chpprop_produce_person($form, $form_state, $settings);
  }

  $options = array();
  $options['No'] = t('No');
  $options['Yes'] = t('Yes, redirect to regions after save');
  $form['Regions'] =
    array('#type' => 'radios',
	  '#title' => t('Company contains multiple regions'),
	  '#options' => $options,
	  '#default_value' => key($options));

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpprop_company_add_form_validate_save'),
	  '#submit' => array('chpprop_company_add_form_submit_save'));
  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_company_add_form_submit_back'));
  return $form;
}

function chpprop_company_add_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['Alias'])) {
    form_set_error('Alias', t('Company name field is required.'));
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

function chpprop_company_add_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropcomp';
}

function chpprop_company_add_form_submit_save($form, &$form_state) {
  global $user;
  $company_id = chpprop_genRandomString20();
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

  if (chdbprop_insert_company
      ($user->uid, $company_id, $alias,
       $street, $city, $postalcode, $province, $country,
       $id)) {
    drupal_set_message('Couldn\'t insert company', 'error');
    return;
  }

  if (isset($form_state['storage']['Lat'])) {
    if (chdbprop_update_company_latlon
	($id, $lat, $lon)) {
      drupal_set_message('Couldn\'t update company lat/lon', 'error');
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

    if (chdbprop_insert_companyadmin
	($company_id, $admin_id, $lat, $lon,
	 $prename_id, $firstname, $middlename, $lastname, $sufname_id,
	 $street, $city, $postalcode, $province, $country,
	 $phone, $email, $emailnotification,
	 $role, $department, $title, $username, $password,
	 $id)) {
      drupal_set_message('Could not insert company admin', 'error');
      return;
    }
  }

  unset($form_state['storage']);
  if ($form_state['storage']['Regions'] === 'Yes') {
    $form_state['redirect'] = 'chppropregion';
  } else {
    $form_state['redirect'] = 'chppropcomp';
  }
}



/**********
 ********** Company list
 ********** Address: chppropcomp
 ********** Access:  'anai chp list companies'
 *********/



function chpprop_company_form($form_state) {
  if ($form_state['storage']['detailed_view']) {
    return chpprop_company_view_form($form_state);
  }
  if ($form_state['storage']['detailed_edit']) {
    return chpprop_company_edit_form($form_state);
  }

  drupal_set_title(t('Companies'));

  global $user;
  $account = user_load(array('uid' => $user->uid));
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['buttons']['back']['title'] = t('Back');
  $settings['buttons']['back']['submit'] = 'chpprop_company_form_submit_back';
  if (user_access('anai chp add companies')) {
    $settings['buttons']['add']['title'] = t('Add');
    $settings['buttons']['add']['submit'] = 'chpprop_company_form_submit_add';
  }
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }

  // Active table.
  $entries = array();
  // As Admin, get all regardless of relations
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_get_all_companies() as $company) {
      $entries[$company['CompanyId']] = $company;
    }
  } else {
    foreach (chpprop_retrieve_companies($user->uid) as $company_id => $company) {
      $entries[$company_id] = $company['data'];
    }
  }
  $form['ajax']['available'] =
    array('#type' => 'fieldset',
	  '#title' => t('Available'));
  if (empty($entries)) {
    $form['ajax']['available']['empty'] = array('#value' => '<p>'.t('Empty.'));
  } else {
    $form['ajax']['available']['list'] = chpprop_company_table_form($entries);
  }

  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    // Suspended table.
    $entries = array();
    // As Admin, get all regardless of relations
    if ((in_array('Admin', array_values($account->roles)) and $account->status) or
	$account->uid == 1) {
      foreach (chdbprop_get_all_companies(TRUE, TRUE, // Active
					  TRUE, TRUE) // Suspended
	       as $company) {
	$entries[$company['CompanyId']] = $company;
      }
    } else {
      foreach (chpprop_retrieve_companies($user->uid,
					  TRUE, TRUE, // Active
					  TRUE, TRUE) // Suspended
	       as $company_id => $company) {
	$entries[$company_id] = $company['data'];
      }
    }
    if (!empty($entries)) {
      $form['ajax']['suspended'] =
	array('#type' => 'fieldset',
	      '#title' => t('Suspended companies'));
      if (count($entries) == 0) {
	$form['ajax']['suspended']['empty'] =
	  array('#value' => '<p>'.t('Empty.'));
      } else {
	$form['ajax']['suspended']['list'] = chpprop_company_table_form($entries);
      }
    }

    // Retention table.
    // Need to loop all since this is where companies are managed.
    if (!isset($form_state['storage']['ajax']['Retention'])) {
      $form_state['storage']['ajax']['Retention'] = FALSE;
    }
    $form['ajax']['Retention'] =
      array('#type' => 'checkbox',
	    '#title' => t('Show deleted companies'),
	    '#default_value' => $form_state['storage']['ajax']['Retention'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
    if ($form_state['storage']['ajax']['Retention']) {
      $entries = array();
      // As Admin, get all regardless of relations
      if ((in_array('Admin', array_values($account->roles)) and $account->status) or
	  $account->uid == 1) {
	foreach (chdbprop_get_all_companies(TRUE, FALSE, // Active
					    FALSE, TRUE) // Suspended) as $company) {
		 as $company) {
	  $entries[$company['CompanyId']] = $company;
	}
      } else {
	foreach (chpprop_retrieve_companies($user->uid,
					    TRUE, FALSE, // Active
					    FALSE, TRUE) // Suspended) as $company) {
		 as $company) {
	  $entries[$company_id] = $company['data'];
	}
      }
      $form['ajax']['retention'] =
	array('#type' => 'fieldset',
	      '#title' => t('Deleted companies'));
      if (count($entries) == 0) {
	$form['ajax']['retention']['empty'] =
	  array('#value' => '<p>'.t('Empty.'));
      } else {
	$form['ajax']['retention']['list'] = chpprop_company_table_form($entries);
      }
    }
  }

  return $form;
}

function chpprop_company_form_submit_add($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropcompadd';
}

function chpprop_company_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpproprel';
}

function chpprop_company_table_form($entries) {
  $form['db'] =
    array('#type' => 'markup', '#value' => '',
	  '#theme' => 'chpprop_company_table');

  if (user_access('anai chp edit companies')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }

  $looper = 0;
  foreach ($entries as $entry) {
    $form['db']['alias_'.$looper] =
      array('#value' => decode_entities($entry['Alias']));

    $form['db']['street_'.$looper] =
      array('#value' => decode_entities($entry['Street']));

    $form['db']['city_'.$looper] =
      array('#value' => decode_entities($entry['City']));

    $form['db']['province_'.$looper] =
      array('#value' => decode_entities($entry['Province']));

    $form['db']['country_'.$looper] =
      array('#value' => decode_entities($entry['Country']));

    $form['db']['view_'.$looper] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/viewsmall.png',
	    '#name' => 'op_view'.$entry['Id'],
	    '#submit' => array('chpprop_company_form_submit'));

    if ($readonly or $entry['readonly']) {
      $form['db']['details_'.$looper] = array('#value' => '');
    } else {
      $form['db']['details_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_details'.$entry['Id'],
	      '#submit' => array('chpprop_company_form_submit'));
    }
    $looper++;
  }
  return $form;
}

function theme_chpprop_company_table($form) {
  $header = array(t('Company name'), t('Street'), t('City'), t('Province'), t('Country'), '', '');
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

function chpprop_company_view_form($form_state) {
  $id = $form_state['storage']['id'];
  $company = chdbprop_get_company_id($id);
  drupal_set_title(t('Company - COMP', array('COMP' => decode_entities($company['Alias']))));

  $form['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/back.png',
	  '#submit' => array('chpprop_company_view_form_submit_back'));

  $form['t1'] =
    array('#prefix' => '<table border="0"><tr valign="top"><td align="left" width="60%">',
	  '#value' => chpprop_company_view_table($company),
	  '#suffix' => '</td>');
  $form['t2'] =
    array('#prefix' => '<td align="left">',
	  '#value' => chpprop_company_view_map($company),
	  '#suffix' => '</td></tr></table>');

  // Contacts and admins
  $admins = chdbprop_get_companyadmins($company['CompanyId'],
				       TRUE, $company['Active'],
				       TRUE, $company['Suspended']);
  $form['t3'] =
    array('#type' => 'fieldset',
	  '#title' => t('Contacts and administrators'));
  $form['t3']['list'] = array('#value' => chpprop_name_view_table($admins, $company['CompanyId']));
  return $form;
}

function chpprop_company_view_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropcomp';
}

function chpprop_company_view_table($entry) {
  $header = array();
  $row[] = array(t('Company name:'), decode_entities($entry['Alias']));
  $row[] = array(t('Street:'), decode_entities($entry['Street']));
  $row[] = array(t('City:'), decode_entities($entry['City']));
  $row[] = array(t('Postal code:'), decode_entities($entry['PostalCode']));
  $row[] = array(t('Province:'), $entry['Province']);
  $row[] = array(t('Country:'), $entry['Country']);

  global $user;
  $regions = chpprop_retrieve_regions($user->uid);
  if (!empty($regions)) {
    $regions = $regions[$entry['CompanyId']];
    $row[] = array(t('Number of regions:'), count($regions));

    $i = 1;
    foreach ($regions as $region) {
      $row[] = array(t('Region #NUM:', array('NUM' => $i++)), decode_entities($region['data']['Alias']));
    }
  }

  $clients = chpprop_retrieve_clients($user->uid);
  if (!empty($clients)) {
    $clients = $clients[$entry['CompanyId']];
    $row[] = array(t('Number of clients:'), count($clients));

    $i = 1;
    foreach ($clients as $region_id => $regions) {
      foreach ($regions as $client_id => $client) {
	$row[] = array(t('Client #NUM:', array('NUM' => $i++)), decode_entities($client['data']['Alias']));
      }
    }
    if (isset($clients['ANY'])) {
      foreach ($clients['ANY'] as $client_id => $client) {
	$row[] = array(t('Client #NUM:', array('NUM' => $i++)), decode_entities($client['data']['Alias']));
      }
    }
  }

  $properties = chpprop_retrieve_properties($user->uid, $entry['CompanyId']);
  $row[] = array(t('Number of properties:'), count($properties));

  $i = 1;
  foreach ($properties as $property_id => $property) {
    $row[] = array(t('Property #NUM:', array('NUM' => $i++)), decode_entities($property['data']['Alias']));
  }

  return theme('table', $header, $row);
}

function chpprop_company_view_map($entry) {
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

function chpprop_company_edit_form($form_state) {
  if ($form_state['storage']['confirm_delete_company']) {
    return confirm_form($form, 'Are you sure?', 'chppropcomp',
			'The company will be deleted from the system.');
  }

  drupal_set_title(t('Edit company'));
  $id = $form_state['storage']['id'];
  $company = chdbprop_get_company_id($id);

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  if (!$company['Active']) {
    $form['Activate'] =
      array('#type' => 'checkbox',
	    '#title' => t('Activate'),
	    '#default_value' => $company['Active']);
  }

  $form['Suspended'] =
    array('#type' => 'checkbox',
	  '#title' => t('Suspended'),
	  '#default_value' => $company['Suspended']);

  if (!isset($form_state['storage']['Alias'])) {
    $form_state['storage']['Alias'] = decode_entities($company['Alias']);
  }
  $form['Alias'] =
    array('#type' => 'textfield',
	  '#title' => t('Company name'),
	  '#default_value' => $form_state['storage']['Alias'],
	  '#maxlength' => 80);

  // Layout company address
  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['subtag'] = 'config';
  $settings['subframe'] = TRUE;
  $settings['subframetitle'] = t('Company address');
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
  $settings['default']['Street'] = decode_entities($company['Street']);
  $settings['default']['City'] = decode_entities($company['City']);
  $settings['default']['PostalCode'] = decode_entities($company['PostalCode']);
  $settings['default']['Province'] = $company['Province'];
  $settings['default']['Country'] = $company['Country'];
  chpprop_produce_person($form, $form_state, $settings);
  $prenameoption = $form_state['storage']['ajax']['config']['PreNameOptions'];
  $sufnameoption = $form_state['storage']['ajax']['config']['SufNameOptions'];

  // Layout admins
  $admins = chdbprop_get_companyadmins($company['CompanyId']);
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
      $name .= isset($prenameoption[$admin['PrenameId']]) ? $prenameoption[$admin['PrenameId']].' ' : '';
      $name .= isset($admin['FirstName']) ? decode_entities($admin['FirstName']).' ' : '';
      $name .= isset($admin['MiddleName']) ? decode_entities($admin['MiddleName']).' ' : '';
      $name .= isset($admin['LastName']) ? decode_entities($admin['LastName']).' ' : '';
      $name .= isset($sufnameoption[$admin['SufnameId']]) ? $sufnameoption[$admin['SufnameId']].' ' : '';
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
	  '#validate' => array('chpprop_company_add_form_validate_save'),
	  '#submit' => array('chpprop_company_edit_form_submit_save'));

  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_company_edit_form_submit_back'));
  if (user_access('anai chp delete companies') and
      $company['Active']) {
    $form['delete'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/delete.png',
	    '#validate' => array('chpprop_company_edit_form_validate_delete'),
	    '#submit' => array('chpprop_company_form_submit'));
  }
  return $form;
}

function chpprop_company_edit_form_validate_delete($form, &$form_state) {
  $form_state['storage']['delete_company'] = TRUE;
}

function chpprop_company_edit_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropcomp';
}

function chpprop_company_edit_form_submit_save($form, &$form_state) {
  $id = $form_state['storage']['id'];
  $company = chdbprop_get_company_id($id);
  $company_id = $company['CompanyId'];

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

  if (chdbprop_update_company
      ($id, $alias,
       $street, $city, $postalcode, $province, $country)) {
    drupal_set_message('Couldn\'t update company', 'error');
    return;
  }

  if (!$company['ManLatLon']) {
    if (chdbprop_update_company_latlon
	($id, $lat, $lon)) {
      drupal_set_message('Couldn\'t update company', 'error');
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

  if (chdbprop_update_company_status
      ($id,
       $activate,
       $suspended)) {
    drupal_set_message('Could not update company status', 'error');
    return;
  }

  if (chdbprop_update_companyadmins_status
      ($company_id,
       $activate,
       $suspended)) {
    drupal_set_message('Could not update company admin status', 'error');
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
      if (chdbprop_delete_companyadmin
	  ($company_id, $admin_id)) {
	drupal_set_message('Could not delete company admin', 'error');
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

    if (chdbprop_update_companyadmin
	($id,
	 $prename_id, $firstname, $middlename, $lastname, $sufname_id,
	 $street, $city, $postalcode, $province, $country,
	 $phone, $email, $emailnotification,
	 $role, $department, $title, $username, $password)) {
      drupal_set_message('Could not update company admin', 'error');
      return;
    }

    if (chdbprop_update_companyadmin_latlon
	($id, $lat, $lon)) {
      drupal_set_message('Could not update company admin lat/lon', 'error');
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

    if (chdbprop_insert_companyadmin
	($company_id, $admin_id, $lat, $lon,
	 $prename_id, $firstname, $middlename, $lastname, $sufname_id,
	 $street, $city, $postalcode, $province, $country,
	 $phone, $email, $emailnotification,
	 $role, $department, $title, $username, $password,
	 $id)) {
      drupal_set_message('Could not insert company admin', 'error');
      return;
    }
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropcomp';
}

function chpprop_company_form_submit($form, &$form_state) {
  global $user;
  if ($form_state['storage']['confirm_delete_company']) {
    $id = $form_state['storage']['id'];
    $company = chdbprop_get_company_id($id);
    // Retire the family all the way down to permits
    $reason = t('Company deleted');
    chdbprop_retire_company_id
      ($id, $user->uid, t('Company deleted'));
    chdbprop_retire_companyadmins
      ($company['CompanyId'], $user->uid, $reason);
    foreach (chdbprop_get_regions
	     ($company['CompanyId'],
	      TRUE, TRUE, // Active
	      FALSE, FALSE) as $region) {
      chdbprop_retire_region_id
	($region['Id'], $user->uid, $reason);
      chdbprop_retire_regionadmins
	($region['CompanyId'], $region['RegionId'], $user->uid, $reason);
      foreach (chdbprop_get_clients
	       ($region['CompanyId'], $region['RegionId'],
		TRUE, TRUE, // Active
		FALSE, FALSE) as $client) {
	chdbprop_retire_client_id
	  ($client['Id'], $user->uid, $reason);
	chdbprop_retire_clientadmins
	  ($client['CompanyId'], $client['ClientId'], $user->uid, $reason);
      }
    }
    //TBD for all groups
    //TBD for all groupadmins
    //TBD for all permits
    unset($form_state['storage']);
    $form_state['redirect'] = 'chppropcomp';
    return;
  }
  if ($form_state['storage']['delete_company']) {
    $form_state['storage']['confirm_delete_company'] = TRUE;
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



