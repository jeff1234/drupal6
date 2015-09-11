<?php
require_once("anai_chpproperties.inc");



/**********
 ********** Property add
 ********** Address: chppropadd
 ********** Access:  'anai chp add properties'
 *********/



function chpprop_property_add_form($form_state) {
  drupal_set_title(t('Add property'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  /**
   * Wizard initiate
   */

  if (strpos($form_state['clicked_button']['#id'], 'nextfs1') !== FALSE) {
    if (chpprop_property_add_form_validate_save_address_helper($form_state)) {
      $form_state['storage']['ajax']['Wizard'] = 2;
    }
  } elseif(strpos($form_state['clicked_button']['#id'], 'nextfs2') !== FALSE) {
    if (chpprop_property_add_form_validate_save_person_helper($form_state)) {
      $form_state['storage']['ajax']['Wizard'] = 3;
    }
  } elseif(strpos($form_state['clicked_button']['#id'], 'nextfs3') !== FALSE) {
    if (chpprop_property_add_form_validate_save_admin_helper($form_state)) {
      $form_state['storage']['ajax']['Wizard'] = 4;
    }
  } elseif(strpos($form_state['clicked_button']['#id'], 'nextfs4') !== FALSE) {
    if (chpprop_property_add_form_validate_save_policy_helper($form_state)) {
      $form_state['storage']['ajax']['Wizard'] = 5;
    }
  } elseif(strpos($form_state['clicked_button']['#id'], 'nextfs5') !== FALSE) {
    $form_state['storage']['ajax']['Wizard'] = 6;
  }
  if (!isset($form_state['storage']['ajax']['Wizard'])) {
    $form_state['storage']['ajax']['Wizard'] = 1;
  }
  $form['ajax']['Wizard'] =
    array('#type' => 'hidden',
	  '#value' => $form_state['storage']['ajax']['Wizard']);

  /**
   * Wizard field property address
   */

  $form['ajax']['config'] =
    array('#type' => 'fieldset',
	  '#prefix' => '<div id="config-ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  if (!isset($form_state['storage']['ajax']['config']['Alias'])) {
    $form_state['storage']['ajax']['config']['Alias'] = '';
  }
  $form['ajax']['config']['Alias'] =
    array('#type' => 'textfield',
	  '#title' => t('Property name'),
	  '#default_value' => $form_state['storage']['ajax']['config']['Alias'],
	  '#maxlength' => 80);

  // Layout property
  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['subtag'] = 'config';
  $settings['subframe'] = TRUE;
  $settings['subframetitle'] = t('Property name and address');
  $settings['subframecollapsible'] = TRUE;
  $settings['option']['wizard'] = 1;
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

  $form['ajax']['nextfs1'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/next.png',
	  '#ahah' => array('event' => 'click',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  /**
   * Wizard field contact person
   */

  if (2 > $form_state['storage']['ajax']['Wizard']) {
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpprop_property_add_form_submit_back'));
    return $form;
  } else {
    unset($form['ajax']['nextfs1']);
  }

  // Layout primary person
  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['subtag'] = 'person';
  $settings['subframe'] = TRUE;
  $settings['subframetitle'] = t('Primary contact person');
  $settings['subframecollapsible'] = TRUE;
  $settings['option']['wizard'] = 2;
  $settings['option']['skipnotrequired'] = TRUE;
  $settings['option']['skiplogin'] = TRUE;
  chpprop_produce_person($form, $form_state, $settings);

  $form['ajax']['nextfs2'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/next.png',
	  '#ahah' => array('event' => 'click',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  /**
   * Wizard field admin person
   */

  if (3 > $form_state['storage']['ajax']['Wizard']) {
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpprop_property_add_form_submit_back'));
    return $form;
  } else {
    unset($form['ajax']['nextfs2']);
  }

  // Layout admins
  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['subtag'] = 'newadmin';
  $settings['subframe'] = TRUE;
  $settings['subframetitle'] = t('Administrator');
  $settings['subframecollapsible'] = TRUE;
  $settings['option']['wizard'] = 3;
  $settings['option']['skiplogin'] = TRUE;
  $settings['default']['NotRequired'] = FALSE;
  chpprop_produce_person($form, $form_state, $settings);

  $form['ajax']['nextfs3'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/next.png',
	  '#ahah' => array('event' => 'click',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  /**
   * Wizard field details
   */

  if (4 > $form_state['storage']['ajax']['Wizard']) {
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpprop_property_add_form_submit_back'));
    return $form;
  } else {
    unset($form['ajax']['nextfs3']);
  }

  $settings = array();
  $settings['option']['skipprofilename'] = TRUE;
  chpprop_produce_policy($form, $form_state, $settings);

  $form['ajax']['nextfs4'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/next.png',
	  '#ahah' => array('event' => 'click',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  /**
   * Wizard completed
   */

  if ($form_state['storage']['ajax']['Wizard'] === 5) {
    unset($form['ajax']['nextfs4']);

    $form['ajax']['msg'] =
      array('#value' => '<p>'.t('You are now ready to save the configurations.'));

    $options = array();
    $options['Done'] = t('I\'m done');
    $options['Expand'] = t('Add more lots, enforcement, equipment, etc');

    if (!isset($form_state['storage']['ajax']['NextStep'])) {
      $form_state['storage']['ajax']['NextStep'] = key($options);
    }
    $form['ajax']['NextStep'] =
      array('#type' => 'select',
	    '#title' => t('Next step'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['NextStep']);

    $form['ajax']['save'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/save.png',
	    '#validate' => array('chpprop_property_add_form_validate_save'),
	    '#submit' => array('chpprop_property_add_form_submit_save'));
  }

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_property_add_form_submit_back'));
  return $form;
}

function chpprop_property_add_form_validate_save_address_helper(&$form_state) {
  if (empty($form_state['values']['ajax']['config']['Alias'])) {
    form_set_error('ajax][config][Alias', t('Property name field is required.'));
    return FALSE;
  }
  if (empty($form_state['values']['ajax']['config']['Street'])) {
    form_set_error('ajax][config][Street', t('Street field is required.'));
    return FALSE;
  }
  if (empty($form_state['values']['ajax']['config']['City'])) {
    form_set_error('ajax][config][City', t('City field is required.'));
    return FALSE;
  }
  if (empty($form_state['values']['ajax']['config']['PostalCode'])) {
    form_set_error('ajax][config][PostalCode', t('State/Province field is required.'));
    return FALSE;
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
    $form_state['storage']['ajax']['config']['Lat'] = $pos['lat'];
    $form_state['storage']['ajax']['config']['Lon'] = $pos['lon'];
  }
  return TRUE;
}

function chpprop_property_add_form_validate_save_person_helper(&$form_state) {
  if (isset($form_state['values']['ajax']['person']['FirstName']) and
      empty($form_state['values']['ajax']['person']['FirstName'])) {
    form_set_error('ajax][person][FirstName', 'First name field is required.');
    return FALSE;
  }
  if (isset($form_state['values']['ajax']['person']['LastName']) and
      empty($form_state['values']['ajax']['person']['LastName'])) {
    form_set_error('ajax][person][LastName', 'Last name field is required.');
    return FALSE;
  }
  if (isset($form_state['values']['ajax']['person']['Street']) and
      empty($form_state['values']['ajax']['person']['Street'])) {
    form_set_error('ajax][person][Street', 'Street field is required.');
    return FALSE;
  }
  if (isset($form_state['values']['ajax']['person']['City']) and
      empty($form_state['values']['ajax']['person']['City'])) {
    form_set_error('ajax][person][City', 'City field is required.');
    return FALSE;
  }
  if (isset($form_state['values']['ajax']['person']['PostalCode']) and
      empty($form_state['values']['ajax']['person']['PostalCode'])) {
    form_set_error('ajax][person][PostalCode', 'Postal code field is required.');
    return FALSE;
  }
  if (isset($form_state['values']['ajax']['person']['Phone']) and
      empty($form_state['values']['ajax']['person']['Phone'])) {
    form_set_error('ajax][person][Phone', 'Phone field is required.');
    return FALSE;
  }
  if (isset($form_state['values']['ajax']['person']['Email']) and
      empty($form_state['values']['ajax']['person']['Email'])) {
    form_set_error('ajax][person][Email', 'Email field is required.');
    return FALSE;
  }
  if (isset($form_state['values']['ajax']['person']['OtherRole']) and
      empty($form_state['values']['ajax']['person']['OtherRole'])) {
    form_set_error('ajax][person][OtherRole', 'Other role field is required.');
    return;
  }
  if (isset($form_state['values']['ajax']['person']['Department']) and
      empty($form_state['values']['ajax']['person']['Department'])) {
    form_set_error('ajax][person][Department', 'Department field is required.');
    return;
  }
  if (isset($form_state['values']['ajax']['person']['Title']) and
      empty($form_state['values']['ajax']['person']['Title'])) {
    form_set_error('ajax][person][Title', 'Title field is required.');
    return;
  }

  // Check GEO Coding
  include_once drupal_get_path('module', 'location') .'/location.inc';
  $location = array();
  $location['street'] = $form_state['values']['ajax']['person']['Street'];
  $location['city'] = $form_state['values']['ajax']['person']['City'];
  $location['postal_code'] = $form_state['values']['ajax']['person']['PostalCode'];
  $location['province'] = $form_state['values']['ajax']['person']['Province'];
  $location['country'] = $form_state['values']['ajax']['person']['Country'];
  $pos = location_latlon_exact($location);
  if ($pos) {
    $form_state['storage']['ajax']['person']['Lat'] = $pos['lat'];
    $form_state['storage']['ajax']['person']['Lon'] = $pos['lon'];
  }
  return TRUE;
}

function chpprop_property_add_form_validate_save_admin_helper(&$form_state) {
  if (!isset($form_state['values']['ajax']['newadmin']['NotRequired']) or
      (isset($form_state['values']['ajax']['newadmin']['NotRequired']) and
       !$form_state['values']['ajax']['newadmin']['NotRequired'])) {
    if (isset($form_state['values']['ajax']['newadmin']['FirstName']) and
	empty($form_state['values']['ajax']['newadmin']['FirstName'])) {
      form_set_error('ajax][newadmin][FirstName', 'First name field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax']['newadmin']['LastName']) and
	empty($form_state['values']['ajax']['newadmin']['LastName'])) {
      form_set_error('ajax][newadmin][LastName', 'Last name field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax']['newadmin']['Street']) and
	empty($form_state['values']['ajax']['newadmin']['Street'])) {
      form_set_error('ajax][newadmin][Street', 'Street field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax']['newadmin']['City']) and
	empty($form_state['values']['ajax']['newadmin']['City'])) {
      form_set_error('ajax][newadmin][City', 'City field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax']['newadmin']['PostalCode']) and
	empty($form_state['values']['ajax']['newadmin']['PostalCode'])) {
      form_set_error('ajax][newadmin][PostalCode', 'Postal code field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax']['newadmin']['Phone']) and
	empty($form_state['values']['ajax']['newadmin']['Phone'])) {
      form_set_error('ajax][newadmin][Phone', 'Phone field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax']['newadmin']['Email']) and
	empty($form_state['values']['ajax']['newadmin']['Email'])) {
      form_set_error('ajax][newadmin][Email', 'Email field is required.');
      return FALSE;
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

    // Check GEO Coding
    include_once drupal_get_path('module', 'location') .'/location.inc';
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
  return TRUE;
}

function chpprop_property_add_form_validate_save_policy_helper(&$form_state) {
  // Profile policy
  if (isset($form_state['values']['ajax']['policy']['LotName']) and
      empty($form_state['values']['ajax']['policy']['LotName'])) {
    form_set_error('ajax][policy][LotName', 'Lot name field is required.');
    return;
  }
  if (isset($form_state['values']['ajax']['policy']['ProfileId']) and
      $form_state['values']['ajax']['policy']['ProfileId'] === 'Custom') {

    if (!$form_state['values']['ajax']['policy']['rCheckbox'] and
	!$form_state['values']['ajax']['policy']['vCheckbox'] and
	!$form_state['values']['ajax']['policy']['valCheckbox']) {
      form_set_error('ajax][policy][rCheckbox', 'At least one policy is required');
      form_set_error('ajax][policy][vCheckbox', 'At least one policy is required');
      form_set_error('ajax][policy][valCheckbox', 'At least one policy is required');
      return;
    }
  }
  // Long term sanity check
  if (isset($form_state['values']['ajax']['policy']['rPolicyAlias']) and
      empty($form_state['values']['ajax']['policy']['rPolicyAlias'])) {
    form_set_error('ajax][policy][rPolicyAlias', 'Alias field is required');
    return;
  }
  if (isset($form_state['values']['ajax']['policy']['rValidityDays']) and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['policy']['rValidityDays'])) {
    form_set_error('ajax][policy][rValidityDays', 'Must be a number');
    return;
  }
  if (isset($form_state['values']['ajax']['policy']['rRemovalDays']) and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['policy']['rRemovalDays'])) {
    form_set_error('ajax][policy][rRemovalDays', 'Must be a number');
    return;
  }
  // Short term sanity check
  if (isset($form_state['values']['ajax']['policy']['vPolicyAlias']) and
      empty($form_state['values']['ajax']['policy']['vPolicyAlias'])) {
    form_set_error('ajax][policy][vPolicyAlias', 'Alias field is required');
    return;
  }
  if (isset($form_state['values']['ajax']['policy']['vExpiryDuration']) and
      VISITOR_EXPIRY_DAYS == $form_state['values']['ajax']['policy']['vExpiryDuration']) {
    if (!preg_match('/^\d+$/', $form_state['values']['ajax']['policy']['vExpiryDays'])) {
      form_set_error('ajax][policy][vExpiryDays', 'Maximum days field must be a number');
      return;
    }
  }
  if (isset($form_state['values']['ajax']['policy']['vRenewRetentionDays']) and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['policy']['vRenewRetentionDays'])) {
    form_set_error('ajax][policy][vRenewRetentionDays', 'Retention days field must be a number');
    return;
  }
  if (isset($form_state['values']['ajax']['policy']['vRegularCost']) and
      !preg_match('/^\d+\.\d{2}$/', $form_state['values']['ajax']['policy']['vRegularCost'])) {
    form_set_error('ajax][policy][vRegularCost', 'Amount must be given in format X.XX');
    return;
  }
  if (isset($form_state['values']['ajax']['policy']['vOvernightCost']) and
      !preg_match('/^\d+\.\d{2}$/', $form_state['values']['ajax']['policy']['vOvernightCost'])) {
    form_set_error('ajax][policy][vOvernightCost', 'Amount must be given in format X.XX');
    return;
  }
  // Valet sanity check
  if (isset($form_state['values']['ajax']['policy']['valCheckbox']) and
      $form_state['values']['ajax']['policy']['valCheckbox'] and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['policy']['valNumunits'])) {
    form_set_error('ajax][policy][valNumunits', 'Maximum mobile units field must be a number');
    return;
  }

  // Enforcement policy
  if (isset($form_state['values']['ajax']['enforcement']['mobileCheckbox']) and
      $form_state['values']['ajax']['enforcement']['mobileCheckbox'] and
      isset($form_state['values']['ajax']['enforcement']['mobileUnits']) and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['enforcement']['mobileUnits'])) {
    form_set_error('ajax][enforcement][mobileUnits', 'Must be a number');
    return;
  }

  return TRUE;
}

function chpprop_property_add_form_validate_save($form, &$form_state) {
  if (!chpprop_property_add_form_validate_save_address_helper($form_state)) {
    return;
  }
  if (!chpprop_property_add_form_validate_save_person_helper($form_state)) {
    return;
  }
  if (!chpprop_property_add_form_validate_save_admin_helper($form_state)) {
    return;
  }
  if (!chpprop_property_add_form_validate_save_policy_helper($form_state)) {
    return;
  }
}

function chpprop_property_add_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpproplist';
}

function chpprop_property_add_form_submit_save($form, &$form_state) {
  global $user;
  $property_id = chpprop_genRandomString20();
  $alias = check_plain($form_state['values']['ajax']['config']['Alias']);

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

  if (isset($form_state['storage']['ajax']['config']['Lat'])) {
    $lat = $form_state['storage']['ajax']['config']['Lat'];
  } else {
    $lat = NULL;
  }

  if (isset($form_state['storage']['ajax']['config']['Lon'])) {
    $lon = $form_state['storage']['ajax']['config']['Lon'];
  } else {
    $lon = NULL;
  }

  if (chdbprop_insert_property
      ($user->uid, $property_id, $alias,
       $street, $city, $postalcode, $province, $country,
       $id)) {
    drupal_set_message('Couldn\'t insert property', 'error');
    return;
  }

  if (NULL !== $lat and NULL !== $lon) {
    if (chdbprop_update_property_latlon
	($id, $lat, $lon)) {
      drupal_set_message('Couldn\'t update property lat/lon', 'error');
      return;
    }
  }

  // Contact person
  if (isset($form_state['values']['ajax']['person']['PrenameId']) and
      $form_state['values']['ajax']['person']['PrenameId'] !== 'BLANK') {
    $prename_id = $form_state['values']['ajax']['person']['PrenameId'];
  } else {
    $prename_id = NULL;
  }

  if (isset($form_state['values']['ajax']['person']['FirstName'])) {
    $firstname = check_plain($form_state['values']['ajax']['person']['FirstName']);
  } else {
    $firstname = NULL;
  }

  if (isset($form_state['values']['ajax']['person']['MiddleName'])) {
    $middlename = check_plain($form_state['values']['ajax']['person']['MiddleName']);
  } else {
    $middlename = NULL;
  }

  if (isset($form_state['values']['ajax']['person']['LastName'])) {
    $lastname = check_plain($form_state['values']['ajax']['person']['LastName']);
  } else {
    $lastname = NULL;
  }

  if (isset($form_state['values']['ajax']['person']['SufnameId']) and
      $form_state['values']['ajax']['person']['SufnameId'] !== 'BLANK') {
    $sufname_id = $form_state['values']['ajax']['person']['SufnameId'];
  } else {
    $sufname_id = NULL;
  }

  if (isset($form_state['values']['ajax']['person']['Street'])) {
    $street = check_plain($form_state['values']['ajax']['person']['Street']);
  } else {
    $street = NULL;
  }

  if (isset($form_state['values']['ajax']['person']['City'])) {
    $city = check_plain($form_state['values']['ajax']['person']['City']);
  } else {
    $city = NULL;
  }

  if (isset($form_state['values']['ajax']['person']['PostalCode'])) {
    $postalcode = check_plain($form_state['values']['ajax']['person']['PostalCode']);
  } else {
    $postalcode = NULL;
  }

  if (isset($form_state['values']['ajax']['person']['Province'])) {
    $province = $form_state['values']['ajax']['person']['Province'];
  } else {
    $province = NULL;
  }

  if (isset($form_state['values']['ajax']['person']['Country'])) {
    $country = chpprop_retrieve_country($form_state['values']['ajax']['person']['Country']);
  } else {
    $country = NULL;
  }

  if (isset($form_state['values']['ajax']['person']['Phone'])) {
    $phone = check_plain($form_state['values']['ajax']['person']['Phone']);
  } else {
    $phone = NULL;
  }

  if (isset($form_state['values']['ajax']['person']['Email'])) {
    $email = check_plain($form_state['values']['ajax']['person']['Email']);
  } else {
    $email = NULL;
  }

  if (isset($form_state['values']['ajax']['person']['EmailNotification'])) {
    $emailnotification = $form_state['values']['ajax']['person']['EmailNotification'];
  } else {
    $emailnotification = NULL;
  }

  if (isset($form_state['values']['ajax']['person']['Role']) and
      $form_state['values']['ajax']['person']['Role'] !== 'Other') {
    $role = check_plain($form_state['values']['ajax']['person']['Role']);
  } elseif (isset($form_state['values']['ajax']['person']['Role'])) {
    $role = check_plain($form_state['values']['ajax']['person']['OtherRole']);
  } else {
    $role = NULL;
  }

  if (isset($form_state['values']['ajax']['person']['Department'])) {
    $department = check_plain($form_state['values']['ajax']['person']['Department']);
  } else {
    $department = NULL;
  }

  if (isset($form_state['values']['ajax']['person']['Title'])) {
    $title = check_plain($form_state['values']['ajax']['person']['Title']);
  } else {
    $title = NULL;
  }

  if (isset($form_state['storage']['ajax']['person']['Lat'])) {
    $lat = $form_state['storage']['ajax']['person']['Lat'];
  } else {
    $lat = NULL;
  }

  if (isset($form_state['storage']['ajax']['person']['Lon'])) {
    $lon = $form_state['storage']['ajax']['person']['Lon'];
  } else {
    $lon = NULL;
  }

  $admin_id = chpprop_genRandomString20();

  if (chdbprop_insert_propertyadmin
      ($property_id, $admin_id, $lat, $lon,
       $prename_id, $firstname, $middlename, $lastname, $sufname_id,
       $street, $city, $postalcode, $province, $country,
       $phone, $email, $emailnotification,
       $role, $department, $title,
       $not_used)) {
    drupal_set_message('Could not insert property contact', 'error');
    return;
  }

  // Admin
  if (!isset($form_state['values']['ajax']['newadmin']['NotRequired']) or
      (isset($form_state['values']['ajax']['newadmin']['NotRequired']) and
       !$form_state['values']['ajax']['newadmin']['NotRequired'])) {
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

    if (chdbprop_insert_propertyadmin
	($property_id, $admin_id, $lat, $lon,
	 $prename_id, $firstname, $middlename, $lastname, $sufname_id,
	 $street, $city, $postalcode, $province, $country,
	 $phone, $email, $emailnotification,
	 $role, $department, $title,
	 $not_used)) {
      drupal_set_message('Could not insert property admin', 'error');
      return;
    }
  }

  // Policy
  $long_policy = 0;
  $short_policy = 0;
  $longshort_policy = 0;
  $valet_policy = 0;
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

  $lot_id = chpprop_genRandomString20();

  // Property is created with 'Custom' profile.
  // A default profile will then be created.
  if ($form_state['values']['ajax']['policy']['ProfileId'] === 'Custom') {

    $profile_id = chpprop_genRandomString20();

    if (chdbprop_insert_default_profile
	($user->uid,
	 $property_id, // PropertyId used as Alias for default profile
	 $profile_id,
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
	 implode(',',array_keys(array_filter($form_state['values']['ajax']['policy']['valModifiers']))),
	 $profile_index)) {
      drupal_set_message('Could not insert new default profile', 'error');
      return;
    }

    if (chdbprop_insert_policy
	($property_id,
	 $lot_id,
	 $profile_id,
	 check_plain($form_state['values']['ajax']['policy']['LotName']))) {
      drupal_set_message('Could not insert new policy', 'error');
      return;
    }
  } else { // Use profile
    $profile_id = $form_state['values']['ajax']['policy']['ProfileId'];

    if (chdbprop_insert_policy
	($property_id,
	 $lot_id,
	 $profile_id,
	 check_plain($form_state['values']['ajax']['policy']['LotName']))) {
      drupal_set_message('Could not insert new policy', 'error');
      return;
    }
  }

  // Handheld Poll
  if (chdbprop_insert_agentpoll_property($property_id)) {
    drupal_set_message('Could not insert property poll', 'error');
    return;
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chpproplist';
  if ($form_state['values']['ajax']['NextStep'] === 'Expand') {
    $form_state['redirect'] = 'chppropexpand/'.$id;
  }
}



/**********
 ********** Property list
 ********** Address: chpproplist
 ********** Access:  'anai chp list properties'
 *********/



function chpprop_property_list_form($form_state) {
  if ($form_state['storage']['confirm_delete_property']) {
    return confirm_form($form, 'Are you sure?', 'chpproplist',
			'The property and all relations will be deleted from the system.');
  }
  if ($form_state['storage']['confirm_delete_lot']) {
    return confirm_form($form, 'Are you sure?', 'chpproplist',
			'The lot will be deleted from the system.');
  }
  if ($form_state['storage']['detailed_notes']) {
    return chpprop_property_notes_form($form_state);
  }
  if ($form_state['storage']['detailed_view']) {
    return chpprop_property_view_form($form_state);
  }
  if ($form_state['storage']['detailed_levels']) {
    return chpprop_property_levels_form($form_state);
  }
  if ($form_state['storage']['detailed_edit']) {
    return chpprop_property_edit_form($form_state);
  }

  drupal_set_title(t('Properties'));

  global $user;
  $account = user_load(array('uid' => $user->uid));
  $properties = chpprop_retrieve_properties($user->uid);
  $policies = chpprop_retrieve_policies($user->uid);

  //anai_dbg($properties, '$properties');
  //anai_dbg($policies, '$policies');

  $form['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/back.png',
	  '#submit' => array('chpprop_property_list_form_submit_back'));

  if (user_access('anai chp add properties')) {
    $form['add'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/add.png',
	    '#submit' => array('chpprop_property_list_form_submit_add'));
  }

  if (empty($properties)) {
    $form['empty'] = array('#value' => '<p>'.t('Empty.'));
  } else {
    $form['list'][] = chpprop_property_list_table_form($properties, $policies);
  }

  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    $options = array();
    foreach (chdbprop_get_all_properties() as $property) {
      if (!isset($properties[$property['PropertyId']])) {
	$options[$property['PropertyId']] = array('data' => $property);
      }
    }
    foreach (chdbprop_get_all_policies() as $policy) {
      if (!isset($properties[$policy['PropertyId']])) {
	$policies[$policy['PropertyId']][$policy['LotId']] = array('data' => $policy);
      }
    }
    if (!empty($options)) {
      $form['needrelations'] =
	array('#type' => 'fieldset',
	      '#title' => t('Properties missing relations'));
      $form['needrelations'][] = chpprop_property_list_table_form($options, $policies);
    }
  }

  return $form;
}

function chpprop_property_list_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'front_page';
}

function chpprop_property_list_form_submit_add($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppropadd';
}

function chpprop_property_list_table_form($properties, $policies) {
  $form['db'] = array('#type' => 'markup', '#value' => '',
		      '#theme' => 'chpprop_property_table');

  $looper = 0;
  foreach ($properties as $property_id => $entry) {

    // Properties
    $form['db']['alias_'.$looper] =
      array('#value' => decode_entities($entry['data']['Alias']));

    $account = user_load(array('uid' => $entry['data']['CreatorUID']));
    $form['db']['user_'.$looper] = array('#value' => decode_entities($account->name));

    $form['db']['notes_'.$looper] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/notesmall.png',
	    '#name' => 'op_notes'.$entry['data']['Id']);

    if ($entry['readonly']) {
      $form['db']['addlot_'.$looper] = array('#value' => '');
    } else {
      $form['db']['addlot_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/addsmall.png',
	      '#name' => 'op_expand'.$entry['data']['Id']);
    }

    // Lots
    $form['db']['numfrags_'.$looper.'_'.count($policies[$property_id])] =
      array('#type' => 'hidden', '#value' => '');

    $lot_num = 0;
    foreach ($policies[$property_id] as $lot_id => $policy) {
      $profile = chdbprop_get_profile($policy['data']['ProfileId']);

      $form['db']['lot_'.$looper.'_'.$lot_num] =
	array('#value' => $policy['data']['Alias']);

      if ($profile['Type'] == 0) {
	$form['db']['profile_'.$looper.'_'.$lot_num] =
	  array('#value' => t('Custom'));
      } else {
	$form['db']['profile_'.$looper.'_'.$lot_num] =
	  array('#value' => decode_entities($profile['Alias']));
      }

      $typestr = '';
      if (!empty($policy['data']['EnforcementId'])) {
	$typestr .= 'Enforced ';
      }
      if ($profile['LongShortPolicy']) {
	$typestr .= $profile['LongPolicyAlias'];
	if ($profile['ValetPolicy']) {
	  $typestr .= ', ';
	} else {
	  $typestr .= t(' and ');
	}
	$typestr .= $profile['ShortPolicyAlias'];
      } elseif ($profile['LongPolicy']) {
	$typestr .= $profile['LongPolicyAlias'];
      } elseif ($profile['ShortPolicy']) {
	$typestr .= $profile['ShortPolicyAlias'];
      }
      if ($profile['ValetPolicy']) {
	if ($profile['LongShortPolicy'] or
	    $profile['LongPolicy'] or
	    $profile['ShortPolicy']) {
	  $typestr .= t(' and Valet');
	} elseif (strlen($typestr) > 0) {
	  $typestr .= t(' Valet');
	} else {
	  $typestr .= t('Valet');
	}
      }
      if (strlen($typestr) > 0) {
	$typestr .= t(' parking');
      } else {
	$typestr = t('Unmanaged');
      }
      if (!empty($policy['data']['EquipmentId'])) {
	$typestr .= t(' with equipment control');
      }
      $form['db']['type_'.$looper.'_'.$lot_num] = array('#value' => $typestr);

      $form['db']['view_'.$looper.'_'.$lot_num] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/viewsmall.png',
	      '#name' => 'op_view'.$entry['data']['Id'].'_'.$policy['data']['Id'],
	      '#submit' => array('chpprop_property_list_form_submit'));

      $form['db']['levels_'.$looper.'_'.$lot_num] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/listsmall.png',
	      '#name' => 'op_levels'.$entry['data']['Id'].'_'.$policy['data']['Id'],
	      '#submit' => array('chpprop_property_list_form_submit'));

      if (!$entry['readonly'] and user_access('anai chp edit properties')) {
	$form['db']['details_'.$looper.'_'.$lot_num] =
	  array('#type' => 'image_button',
		'#src' => drupal_get_path('module', 'anai').'/editsmall.png',
		'#name' => 'op_details'.$entry['data']['Id'].'_'.$policy['data']['Id'],
		'#submit' => array('chpprop_property_list_form_submit'));
      } else {
	$form['db']['details_'.$looper.'_'.$lot_num] = array('#value' => '');
      }
      $lot_num++;
    }
    $looper++;
  }
  return $form;
}

function theme_chpprop_property_table($form) {
  global $user;
  // Table Header
  $subheader = '</tr><tr><th>'.t('Name').'</th><th>'.t('Created by').'</th>';
  $subheader .= '<th></th><th></th><th>'.t('Name').'</th><th>'.t('Type').'</th><th></th><th></th><th></th>';
  $header = array(array('data' => t('Property'),
			'colspan' => 4),
		  array('data' => t('Lot').$subheader,
			'colspan' => 5),
		  );
  // Table Rows
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'numfrags_')) {
      continue;
    }
    $looper = substr($key, strlen('numfrags_'));
    $looper = substr($looper, 0, strpos($looper, '_'));
    $numfrags = substr($key, strlen('numfrags_'.$looper.'_'));

    $rows[] = array(array('data' => drupal_render($form['alias_'.$looper]),
			  'rowspan' => $numfrags),
		    array('data' => drupal_render($form['user_'.$looper]),
			  'rowspan' => $numfrags),
		    array('data' => drupal_render($form['notes_'.$looper]),
			  'rowspan' => $numfrags,
			  'width' => 1, 'height' => 1),
		    array('data' => drupal_render($form['addlot_'.$looper]),
			  'rowspan' => $numfrags,
			  'width' => 1, 'height' => 1),
		    drupal_render($form['lot_'.$looper.'_0']),
		    drupal_render($form['type_'.$looper.'_0']),
		    array('data' => drupal_render($form['view_'.$looper.'_0']),
			  'width' => 1, 'height' => 1),
		    array('data' => drupal_render($form['levels_'.$looper.'_0']),
			  'width' => 1, 'height' => 1),
		    array('data' => drupal_render($form['details_'.$looper.'_0']),
			  'width' => 1, 'height' => 1));
    for ($i = 1; $i < $numfrags; $i++) {
      $rows[] = array(drupal_render($form['lot_'.$looper.'_'.$i]),
		      drupal_render($form['type_'.$looper.'_'.$i]),
		      drupal_render($form['view_'.$looper.'_'.$i]),
		      drupal_render($form['levels_'.$looper.'_'.$i]),
		      drupal_render($form['details_'.$looper.'_'.$i]));
    }
  }
  // Render Table
  return theme_table($header, $rows);
}

/****************************************************************************/

function chpprop_property_notes_add_form($form_state) {
  $form = array();
  drupal_set_title(t('Add note'));

  $form['Note'] =
    array('#type' => 'textarea',
	  '#title' => t('Description'),
	  '#maxlength' => 2048);

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpprop_property_notes_add_form_validate_save'),
	  '#submit' => array('chpprop_property_notes_add_form_submit_save'));

  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_property_notes_add_form_submit_back'));
  return $form;
}

function chpprop_property_notes_add_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['Note'])) {
    form_set_error('Note', t('Description field is required.'));
    return;
  }
}

function chpprop_property_notes_add_form_submit_back($form, &$form_state) {
  unset($form_state['storage']['notes_add']);
}

function chpprop_property_notes_add_form_submit_save($form, &$form_state) {
  global $user;
  $id = $form_state['storage']['id'];
  $property = chdbprop_get_property_id($id);
  $property_id = $property['PropertyId'];

  $propertynoteid = chdbprop_get_propertynoteid($property_id);
  if (empty($propertynoteid)) {
    if (chdbprop_insert_propertynoteid($property_id, $tmp)) {
      drupal_set_message('Could not insert propertynoteid', 'error');
      return;
    }
    $propertynoteid = chdbprop_get_propertynoteid($property_id);
  }
  $note_id = $propertynoteid['Counter'];

  if (chdbprop_insert_propertynote
      ($user->uid, $property_id, $note_id,
       check_plain($form_state['values']['Note']),
       $id)) {
    drupal_set_message('Could not insert note', 'error');
    return;
  }

  if (chdbprop_update_propertynoteid_id($propertynoteid['Id'])) {
    drupal_set_message('Could not update propertynoteid', 'error');
    return;
  }

  unset($form_state['storage']['notes_add']);
}

/****************************************************************************/

function chpprop_property_notes_edit_form($form_state) {
  global $user;
  $form = array();

  $note_id = $form_state['storage']['note_id'];
  $note = chdbprop_get_propertynote_id($note_id);
  drupal_set_title(t('Update note'));

  $created = chpuser_datetime_utc_to_usertimezone($note['CreateTime']);
  $account = user_load(array('uid' => $note['CreatorUID']));
  $form['Master'] =
    array('#type' => 'textarea',
  	  '#title' => t('Note created DATE by USER',
			array('DATE' => $created->format("M j,y H:i"),
			      'USER' => $account->name)),
  	  '#disabled' => TRUE,
  	  '#default_value' => decode_entities($note['Note']));

  $fragments = chdbprop_get_propertynote($note['PropertyId'], $note['NoteId'], TRUE, TRUE);

  foreach ($fragments as $fragment) {
    $created = chpuser_datetime_utc_to_usertimezone($fragment['CreateTime']);
    $account = user_load(array('uid' => $note['CreatorUID']));
    $form['Frag'][] =
      array('#type' => 'textarea',
	    '#title' => t('Addition DATE by USER',
			  array('DATE' => $created->format("M j,y H:i"),
				'USER' => $account->name)),
	    '#disabled' => TRUE,
	    '#default_value' => decode_entities($fragment['Note']));
  }

  $form['Note'] =
    array('#type' => 'textarea',
	  '#title' => t('Description'),
	  '#maxlength' => 2048);

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpprop_property_notes_edit_form_validate_save'),
	  '#submit' => array('chpprop_property_notes_edit_form_submit_save'));

  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_property_notes_edit_form_submit_back'));
  return $form;
}

function chpprop_property_notes_edit_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['Note'])) {
    form_set_error('Note', t('Description field is required.'));
    return;
  }
}

function chpprop_property_notes_edit_form_submit_back($form, &$form_state) {
  unset($form_state['storage']['notes_edit']);
}

function chpprop_property_notes_edit_form_submit_save($form, &$form_state) {
  global $user;
  $note_id = $form_state['storage']['note_id'];
  $note = chdbprop_get_propertynote_id($note_id);

  if (chdbprop_update_propertynote
      ($note_id, $user->uid,
       check_plain($form_state['values']['Note']))) {
    drupal_set_message('Could not update note', 'error');
    return;
  }

  unset($form_state['storage']['notes_edit']);
}

/****************************************************************************/

function chpprop_property_notes_solve_form($form_state) {
  global $user;
  $form = array();

  $note_id = $form_state['storage']['note_id'];
  $note = chdbprop_get_propertynote_id($note_id);
  drupal_set_title(t('Resolve note'));

  $created = chpuser_datetime_utc_to_usertimezone($note['CreateTime']);
  $account = user_load(array('uid' => $note['CreatorUID']));
  $form['Master'] =
    array('#type' => 'textarea',
  	  '#title' => t('Note created DATE by USER',
			array('DATE' => $created->format("M j,y H:i"),
			      'USER' => $account->name)),
  	  '#disabled' => TRUE,
  	  '#default_value' => decode_entities($note['Note']));

  $fragments = chdbprop_get_propertynote($note['PropertyId'], $note['NoteId'], TRUE, TRUE);

  foreach ($fragments as $fragment) {
    $created = chpuser_datetime_utc_to_usertimezone($fragment['CreateTime']);
    $account = user_load(array('uid' => $note['CreatorUID']));
    $form['Frag'][] =
      array('#type' => 'textarea',
	    '#title' => t('Addition DATE by USER',
			  array('DATE' => $created->format("M j,y H:i"),
				'USER' => $account->name)),
	    '#disabled' => TRUE,
	    '#default_value' => decode_entities($fragment['Note']));
  }

  $form['Note'] =
    array('#type' => 'textarea',
	  '#title' => t('Resolution'),
	  '#maxlength' => 512);

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpprop_property_notes_solve_form_validate_save'),
	  '#submit' => array('chpprop_property_notes_solve_form_submit_save'));

  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_property_notes_solve_form_submit_back'));
  return $form;
}

function chpprop_property_notes_solve_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['Note'])) {
    form_set_error('Note', t('Description field is required.'));
    return;
  }
}

function chpprop_property_notes_solve_form_submit_back($form, &$form_state) {
  unset($form_state['storage']['notes_solve']);
}

function chpprop_property_notes_solve_form_submit_save($form, &$form_state) {
  global $user;
  $note_id = $form_state['storage']['note_id'];
  $note = chdbprop_get_propertynote_id($note_id);

  if (chdbprop_propertynote_solved
      ($note_id, $user->uid,
       check_plain($form_state['values']['Note']))) {
    drupal_set_message('Could not solve note', 'error');
    return;
  }

  unset($form_state['storage']['notes_solve']);
}

/****************************************************************************/

function chpprop_property_notes_form($form_state) {
  if (isset($form_state['storage']['notes_add'])) {
    return chpprop_property_notes_add_form($form_state);
  }
  if (isset($form_state['storage']['notes_edit'])) {
    return chpprop_property_notes_edit_form($form_state);
  }
  if (isset($form_state['storage']['notes_solve'])) {
    return chpprop_property_notes_solve_form($form_state);
  }

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $id = $form_state['storage']['id'];
  $property = chdbprop_get_property_id($id);
  $property_id = $property['PropertyId'];
  drupal_set_title(t('Property notes - NAME', array('NAME' => decode_entities($property['Alias']))));

  $form['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/back.png',
	  '#submit' => array('chpprop_property_notes_form_submit_back'));

  if (user_access('anai chp add property notes')) {
    $form['add'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/add.png',
	    '#submit' => array('chpprop_property_notes_form_validate_add'));
  }

  $notes = array();
  foreach (chdbprop_get_propertynotes($property_id) as $note) {
    if (!$note['Fragment']) {
      $notes[$note['NoteId']]['master'] = $note;
      $notes[$note['NoteId']]['fragments'] = array();
    } else {
      $notes[$note['NoteId']]['fragments'][] = $note;
    }
  }

  if (empty($notes)) {
    $form['empty'] =
      array('#value' => '<p>'.t('Empty.'));
  } else {
    $form['list'] = chpprop_property_notes_table_form($notes);
  }

  return $form;
}

function chpprop_property_notes_form_validate_add($form, &$form_state) {
  $form_state['storage']['notes_add'] = TRUE;
}

function chpprop_property_notes_form_submit_back($form, &$form_state) {
  unset($form_state['storage']['detailed_notes']);
}

function chpprop_property_notes_table_form($entries) {
  global $user;
  $form['db'] =
    array('#type' => 'markup', '#theme' => 'chpprop_property_notes_table');
  if (user_access('anai chp edit property notes')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }
  if (user_access('anai chp solve property notes')) {
    $solveable = FALSE;
  } else {
    $solveable = TRUE;
  }
  $looper = 0;
  foreach ($entries as $note_id => $entry) {
    $form['db']['idd_'.$looper] =
      array('#type' => 'hidden', '#value' => $entry['master']['Id']);

    $created = chpuser_datetime_utc_to_usertimezone($entry['master']['CreateTime']);
    $form['db']['time_'.$looper] = array('#value' => $created->format("M j,y H:i"));

    $account = user_load(array('uid' => $entry['master']['CreatorUID']));
    $form['db']['user_'.$looper] = array('#value' => $account->name);

    $form['db']['noteid_'.$looper] =
      array('#value' => $entry['master']['NoteId']);

    $form['db']['note_'.$looper] =
      array('#value' => nl2br(decode_entities($entry['master']['Note'])));

    $form['db']['numfrags_'.$looper.'_'.count($entry['fragments'])] =
      array('#type' => 'hidden', '#value' => '');

    $frag = 0;
    foreach ($entry['fragments'] as $fragment) {
      $account = user_load(array('uid' => $fragment['CreatorUID']));
      $updated = chpuser_datetime_utc_to_usertimezone($fragment['CreateTime']);
      $form['db']['frag_'.$looper.'_'.$frag] =
    	array('#value' =>
	      '<b>Added '.$updated->format("M j,y H:i").' by '.$account->name.':</b><br>'.
	      nl2br(decode_entities($fragment['Note'])));
      $frag++;
    }

    if ($readonly) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/updatesmall.png',
	      '#name' => 'op_editnote'.$entry['master']['Id'],
	      '#submit' => array('chpprop_property_list_form_submit'));
    }
    if ($solveable) {
      $form['db']['solve_'.$looper] = array('#value' => '');
    } else {
      $form['db']['solve_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/finalsmall.png',
	      '#name' => 'op_solvenote'.$entry['master']['Id'],
	      '#submit' => array('chpprop_property_list_form_submit'));
    }
    $looper++;
  }
  return $form;
}

function theme_chpprop_property_notes_table($form) {
  // Table Header
  $header = array(t('Created'), t('By'), t('Id'), t('Note'), '', '');
  // Table Rows
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'numfrags_')) {
      continue;
    }
    $looper = substr($key, strlen('numfrags_'));
    $looper = substr($looper, 0, strpos($looper, '_'));
    $numfrags = substr($key, strlen('numfrags_'.$looper.'_'));
    $rows[] = array(array('data' => drupal_render($form['time_'.$looper]),
			  'rowspan' => 1 + $numfrags),
		    array('data' => drupal_render($form['user_'.$looper]),
			  'rowspan' => 1 + $numfrags),
		    array('data' => drupal_render($form['noteid_'.$looper]),
			  'rowspan' => 1 + $numfrags),
		    drupal_render($form['note_'.$looper]),
		    array('data' => drupal_render($form['edit_'.$looper]),
			  'rowspan' => 1 + $numfrags,
			  'width' => 1, 'height' => 1),
		    array('data' => drupal_render($form['solve_'.$looper]),
			  'rowspan' => 1 + $numfrags,
			  'width' => 1, 'height' => 1));
    for ($i = 0; $i < $numfrags; $i++) {
      $rows[] = array(drupal_render($form['frag_'.$looper.'_'.$i]));
    }
  }
  // Render Table
  return theme_table($header, $rows);
}

/****************************************************************************/

function chpprop_property_view_form($form_state) {
  global $user;
  // Property
  $id = $form_state['storage']['id'];
  $property = chdbprop_get_property_id($id);
  $property_id = $property['PropertyId'];
  // Lot
  $policy = chdbprop_get_policy_id($form_state['storage']['lot_num']);
  $lot_id = $policy['LotId'];
  $profile_id = $policy['ProfileId'];
  $enforcement_id = $policy['EnforcementId'];
  $equipment_id = $policy['EquipmentId'];
  // Profile
  $profile = chdbprop_get_profile($profile_id);
  if ($profile['Type'] == 0) {
    $profile_id = t('Custom');
  }
  // Enforcement
  if (!empty($enforcement_id)) {
    $enforcement = chdbprop_get_enforcement($enforcement_id);
    if ($enforcement['Type'] == 0) {
      $enforcement_id = t('Custom');
    }
  } else {
    $enforcement = array();
  }
  // Equipment
  if (!empty($equipment_id)) {
    $equipment = chdbprop_get_equipment($equipment_id);
    if ($equipment['Type'] == 0) {
      $equipment_id = t('Custom');
    }
  } else {
    $equipment = NULL;
  }
  // Relations
  $companies = array();
  foreach (chdbprop_get_property_relations($property_id) as $relation) {
    if (!isset($companies[$relation['CompanyId']])) {
      $companies[$relation['CompanyId']] = chdbprop_get_company($relation['CompanyId']);
    }
  }

  drupal_set_title(t('Property - PROP', array('PROP' => decode_entities($property['Alias']))));

  $form['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/back.png',
	  '#submit' => array('chpprop_property_view_form_submit_back'));

  $form['t1'] =
    array('#prefix' => '<table border="0"><tr valign="top"><td align="left" width="60%">',
	  '#value' => chpprop_property_view_detailed_table($property, $companies),
	  '#suffix' => '</td>');
  $form['t2'] =
    array('#prefix' => '<td align="left">',
	  '#value' => chpprop_property_view_detailed_map($property),
	  '#suffix' => '</td></tr></table>');

  // Contacts and admins
  $admins = chdbprop_get_propertyadmins($property_id);
  $form['t3'] =
    array('#type' => 'fieldset',
	  '#title' => t('Contacts and administrators'),
	  '#collapsible' => TRUE);
  $form['t3']['list'] = array('#value' => chpprop_name_view_table($admins));

  // Notes
  $header = array(t('Created'), t('By'), t('Id'), t('Note'));
  $row = array();
  $notes = array();
  foreach (chdbprop_get_propertynotes($property_id) as $note) {
    if (!$note['Fragment']) {
      $notes[$note['NoteId']]['master'] = $note;
      $notes[$note['NoteId']]['fragments'] = array();
    } else {
      $notes[$note['NoteId']]['fragments'][] = $note;
    }
  }
  foreach ($notes as $note_id => $entry) {
    $created = chpuser_datetime_utc_to_usertimezone($entry['master']['CreateTime']);
    $account = user_load(array('uid' => $entry['master']['CreatorUID']));
    $numfrags = count($entry['fragments']);
    $row[] = array(array('data' => $created->format("M j,y H:i"),
			 'rowspan' => 1 + $numfrags),
		   array('data' => $account->name,
			 'rowspan' => 1 + $numfrags),
		   array('data' => $entry['master']['NoteId'],
			 'rowspan' => 1 + $numfrags),
		   nl2br(decode_entities($entry['master']['Note'])));
    foreach ($entry['fragments'] as $fragment) {
      $created = chpuser_datetime_utc_to_usertimezone($fragment['CreateTime']);
      $account = user_load(array('uid' => $fragment['CreatorUID']));
      $row[] = array('<b>Added '.$created->format("M j,y H:i").' by '.$account->name.':</b><br>'.
		     nl2br(decode_entities($fragment['Note'])));
    }
  }

  $form['t4'] = array('#type' => 'fieldset', '#title' => t('Notes'), '#collapsible' => TRUE);
  if (empty($notes)) {
    $form['t4']['empty'] = array('#value' => t('Empty.'));
  } else {
    $form['t4']['list'] = array('#value' => theme('table', $header, $row));
  }

  // Equipment
  if (!empty($equipment)) {
    $settings = array();
    $settings['subframe'] = TRUE;
    $settings['subframetitle'] = t('Equipment settings');
    $settings['subframecollapsible'] = TRUE;
    $settings['subframecollapsed'] = TRUE;
    $settings['option']['readonly'] = TRUE;
    $settings['option']['skipprofilename'] = TRUE;
    $settings['default']['EquipmentId'] = $equipment_id;
    $settings['default']['lprCheckbox'] = $equipment['LPR'];
    $settings['default']['infoCheckbox'] = $equipment['InfoBoard'];
    $settings['default']['padCheckbox'] = $equipment['PAD'];
    chpprop_produce_equipment($form, $form_state, $settings);
  }

  // Enforcement
  if (!empty($enforcement)) {
    $settings = array();
    $settings['subframe'] = TRUE;
    $settings['subframetitle'] = t('Enforcement settings');
    $settings['subframecollapsible'] = TRUE;
    $settings['subframecollapsed'] = TRUE;
    $settings['option']['readonly'] = TRUE;
    $settings['option']['skipprofilename'] = TRUE;
    $settings['default']['EnforcementId'] = $enforcement_id;
    $settings['default']['ticketCheckbox'] = $enforcement['Ticketing'];
    $settings['default']['towCheckbox'] = $enforcement['Towing'];
    $settings['default']['bootCheckbox'] = $enforcement['Booting'];
    $settings['default']['mobileCheckbox'] = $enforcement['MobilePolicy'];
    $settings['default']['mobileUnits'] = $enforcement['MobileUnits'];
    chpprop_produce_enforcement($form, $form_state, $settings);
  }

  // Lot
  $settings = array();
  $settings['subframe'] = TRUE;
  $settings['subframetitle'] = t('Policy settings');
  $settings['subframecollapsible'] = TRUE;
  $settings['subframecollapsed'] = TRUE;
  $settings['option']['readonly'] = TRUE;
  $settings['option']['skipprofilename'] = TRUE;
  $settings['default']['LotName'] = decode_entities($policy['Alias']);
  $settings['default']['ProfileId'] = $profile_id;
  $settings['default']['rCheckbox'] = ($profile['LongShortPolicy'] or $profile['LongPolicy']) ? 1 : 0;
  $settings['default']['vCheckbox'] = ($profile['LongShortPolicy'] or $profile['ShortPolicy']) ? 1 : 0;
  $settings['default']['valCheckbox'] = $profile['ValetPolicy'] ? 1 : 0;
  $settings['default']['rPolicyAlias'] = decode_entities($profile['LongPolicyAlias']);
  $settings['default']['vPolicyAlias'] = decode_entities($profile['ShortPolicyAlias']);
  $settings['default']['rPolicyType'] = $profile['LongTypeId'];
  $settings['default']['vPolicyType'] = $profile['ShortTypeId'];
  $settings['default']['rPolicyClass'] = $profile['LongClassId'];
  $settings['default']['vPolicyClass'] = $profile['ShortClassId'];
  // Short Policy
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
  // Long Policy
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
  // Valet
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

function chpprop_property_view_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpproplist';
}

function chpprop_property_view_detailed_table($property, $companies) {
  $header = array();
  $row[] = array(t('Name:'), decode_entities($property['Alias']));
  $row[] = array(t('Street:'), decode_entities($property['Street']));
  $row[] = array(t('City:'), decode_entities($property['City']));
  $row[] = array(t('Postal code:'), decode_entities($property['PostalCode']));
  $row[] = array(t('State/Province:'), $property['Province']);
  $row[] = array(t('Country:'), $property['Country']);
  $row[] = array(t('Company relations:'), count($companies));
  return theme('table', $header, $row);
}

function chpprop_property_view_detailed_map($property) {
  if (empty($property['Lat']) or empty($property['Lon'])) {
    return t('Latitude/Longitude not valid.');
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
  $text = '<h3>'.decode_entities($property['Alias']).'</h3>';
  $text .= decode_entities($property['Street']).'<br>';
  $text .= decode_entities($property['City']).', ';
  $text .= $property['Province'].'<br>';
  $text .= decode_entities($property['PostalCode']).', ';
  $text .= $property['Country'];
  $markers[] = array
    ('text' => $text,
     'latitude' => sprintf("%f", $property['Lat']),
     'longitude' => sprintf("%f", $property['Lon']),
     'markername' => 'blue',
     );
  $map_array['markers'] = $markers;
  return theme('gmap', array('#settings' => $map_array));
}

/****************************************************************************/

function chpprop_property_levels_form($form_state) {
  if (isset($form_state['storage']['levels_add'])) {
    return chpprop_property_levels_add_form($form_state);
  }
  if (isset($form_state['storage']['levels_edit'])) {
    return chpprop_property_levels_edit_level_form($form_state);
  }
  if (isset($form_state['storage']['levels_stall_edit'])) {
    return chpprop_property_levels_edit_stall_form($form_state);
  }
  if (isset($form_state['storage']['levels_stall_note'])) {
    return chpprop_property_levels_notes_form($form_state);
  }

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  // Property
  $property = chdbprop_get_property_id($form_state['storage']['id']);
  $property_id = $property['PropertyId'];
  // Lot
  $policy = chdbprop_get_policy_id($form_state['storage']['lot_num']);
  $lot_id = $policy['LotId'];
  $profile_id = $policy['ProfileId'];
  // Profile
  $profile = chdbprop_get_profile($profile_id);

  drupal_set_title(t('Property levels'));

  // Build policy selection
  $policy_options = array();
  if (chpprop_short_managed_stalls($profile)) {
    $policy_options['ShortPolicy'] = $profile['ShortPolicyAlias'];
  }
  if (chpprop_long_managed_stalls($profile)) {
    $policy_options['LongPolicy'] = $profile['LongPolicyAlias'];
  }
  if (chpprop_valet_managed_stalls($profile)) {
    $policy_options['ValetPolicy'] = t('Valet');
  }

  $form['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/back.png',
	  '#submit' => array('chpprop_property_levels_form_submit_back'));

  if (empty($policy_options)) {
    $form['empty'] =
      array('#value' => '<p>'.t('Lot is not configured for any stalls.').'<p>');
    return $form;
  }

  if (user_access('anai chp add property levels')) {
    $form['add'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/add.png',
	    '#submit' => array('chpprop_property_levels_form_submit_add'));
  }

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  // Layout policy selection
  if (!isset($form_state['storage']['ajax']['Policy'])) {
    $form_state['storage']['ajax']['Policy'] = key($policy_options);
  }
  if (count($policy_options) > 1) {
    $form['ajax']['Policy'] =
      array('#type' => 'select',
	    '#title' => t('Policy'),
	    '#options' => $policy_options,
	    '#default_value' => $form_state['storage']['ajax']['Policy'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
  } else {
    $form['ajax']['Policy'] =
      array('#type' => 'value',
	    '#value' => $form_state['storage']['ajax']['Policy']);
  }

  if ($form_state['storage']['ajax']['Policy'] === 'LongPolicy') {
    $entries = array();
    $profile_stalls = array();
    $levels = explode(',', $profile['LongStallLevels']);
    $counts = explode(',', $profile['LongStallCount']);
    foreach ($levels as $index => $level) {
      $profile_stalls[$level] = $counts[$index];
    }

    $stallevels = chdbprop_get_stallevels($property_id, $lot_id, TRUE, FALSE, FALSE);
    $existing_stalls = array();
    foreach ($stallevels as $entry) {
      $existing_stalls[$entry['Level']] = array('Id' => $entry['Id'],
						'PropertyId' => $entry['PropertyId'],
						'LotId' => $entry['LotId'],
						'LevelId' => $entry['LevelId'],
						'Alias' => decode_entities($entry['Alias']));
    }

    foreach ($profile_stalls as $level => $count) {
      if (!isset($existing_stalls[$level])) {
	continue;
      }
      $entries[$level] = $existing_stalls[$level];
      $entries[$level]['data'] = array();
      foreach (chdbprop_get_stalls($entries[$level]['PropertyId'],
				   $entries[$level]['LotId'],
				   $entries[$level]['LevelId']) as $stall) {
	$entries[$level]['data'][$stall['StallId']] =
	  array('Id' => $stall['Id'],
		'Active' => $stall['Active'],
		'Alias' => decode_entities($stall['Alias']));
      }
    }

    $form['ajax']['longstalls'] =
      array('#type' => 'fieldset',
	    '#title' => t('TYPE stalls', array('TYPE' => decode_entities($profile['LongPolicyAlias']))));
    if (empty($entries)) {
      $form['ajax']['longstalls']['empty'] = array('#value' => '<p>'.t('Empty.'));
    } else {
      $form['ajax']['longstalls']['list'] = chpprop_property_levels_table_form($entries);
    }
  }

  if ($form_state['storage']['ajax']['Policy'] === 'ShortPolicy') {
    $entries = array();
    $profile_stalls = array();
    $levels = explode(',', $profile['ShortStallLevels']);
    $counts = explode(',', $profile['ShortStallCount']);
    foreach ($levels as $index => $level) {
      $profile_stalls[$level] = $counts[$index];
    }

    $stallevels = chdbprop_get_stallevels($property_id, $lot_id, FALSE, TRUE, FALSE);
    $existing_stalls = array();
    foreach ($stallevels as $entry) {
      $existing_stalls[$entry['Level']] = array('Id' => $entry['Id'],
						'PropertyId' => $entry['PropertyId'],
						'LotId' => $entry['LotId'],
						'LevelId' => $entry['LevelId'],
						'Alias' => decode_entities($entry['Alias']));
    }

    foreach ($profile_stalls as $level => $count) {
      if (!isset($existing_stalls[$level])) {
	continue;
      }
      $entries[$level] = $existing_stalls[$level];
      $entries[$level]['data'] = array();
      foreach (chdbprop_get_stalls($entries[$level]['PropertyId'],
				   $entries[$level]['LotId'],
				   $entries[$level]['LevelId']) as $stall) {
	$entries[$level]['data'][$stall['StallId']] =
	  array('Id' => $stall['Id'],
		'Active' => $stall['Active'],
		'Alias' => decode_entities($stall['Alias']));
      }
    }

    $form['ajax']['shortstalls'] =
      array('#type' => 'fieldset',
	    '#title' => t('TYPE stalls', array('TYPE' => decode_entities($profile['ShortPolicyAlias']))));
    if (empty($entries)) {
      $form['ajax']['shortstalls']['empty'] = array('#value' => '<p>'.t('Empty.'));
    } else {
      $form['ajax']['shortstalls']['list'] = chpprop_property_levels_table_form($entries);
    }
  }

  if ($form_state['storage']['ajax']['Policy'] === 'ValetPolicy') {
    $entries = array();
    $profile_stalls = array();
    $levels = explode(',', $profile['ValetStallLevels']);
    $counts = explode(',', $profile['ValetStallCount']);
    foreach ($levels as $index => $level) {
      $profile_stalls[$level] = $counts[$index];
    }

    $stallevels = chdbprop_get_stallevels($property_id, $lot_id, FALSE, FALSE, TRUE);
    $existing_stalls = array();
    foreach ($stallevels as $entry) {
      $existing_stalls[$entry['Level']] = array('Id' => $entry['Id'],
						'PropertyId' => $entry['PropertyId'],
						'LotId' => $entry['LotId'],
						'LevelId' => $entry['LevelId'],
						'Alias' => decode_entities($entry['Alias']));
    }

    foreach ($profile_stalls as $level => $count) {
      if (!isset($existing_stalls[$level])) {
	continue;
      }
      $entries[$level] = $existing_stalls[$level];
      $entries[$level]['data'] = array();
      foreach (chdbprop_get_stalls($entries[$level]['PropertyId'],
				   $entries[$level]['LotId'],
				   $entries[$level]['LevelId']) as $stall) {
	$entries[$level]['data'][$stall['StallId']] =
	  array('Id' => $stall['Id'],
		'Active' => $stall['Active'],
		'Alias' => decode_entities($stall['Alias']));
      }
    }

    $form['ajax']['valetstalls'] =
      array('#type' => 'fieldset',
	    '#title' => t('TYPE stalls', array('TYPE' => t('Valet'))));
    if (empty($entries)) {
      $form['ajax']['valetstalls']['empty'] = array('#value' => '<p>'.t('Empty.'));
    } else {
      $form['ajax']['valetstalls']['list'] = chpprop_property_levels_table_form($entries);
    }
  }

  return $form;
}

function chpprop_property_levels_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpproplist';
}

function chpprop_property_levels_form_submit_add($form, &$form_state) {
  $form_state['storage']['levels_add'] = TRUE;
}

function chpprop_property_levels_table_form($entries) {
  $form['db'] =
    array('#type' => 'markup', '#theme' => 'chpprop_property_levels_table');

  if (user_access('anai chp edit property levels')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }

  $looper = 0;
  foreach ($entries as $level => $entry) {
    $form['db']['level_'.$looper] = array('#value' => $level);

    if (!empty($entry)) {
      $form['db']['levelalias_'.$looper] = array('#value' => $entry['Alias']);
    } else {
      $form['db']['levelalias_'.$looper] = array('#value' => '');
    }

    if ($readonly or !isset($entry['Id'])) {
      $form['db']['leveledit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['leveledit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_leveledit'.$entry['Id'],
	      '#submit' => array('chpprop_property_list_form_submit'));
    }

    $form['db']['numfrags_'.$looper.'_'.count($entry['data'])] =
      array('#type' => 'hidden', '#value' => '');

    $stall_num = 0;
    foreach ($entry['data'] as $stall_id => $stall) {
      $form['db']['stall_'.$looper.'_'.$stall_num] =
	array('#value' => decode_entities($stall['Alias']));

      $form['db']['active_'.$looper.'_'.$stall_num] =
	array('#value' => $stall['Active'] ? t('Yes') : t('No'));

      $form['db']['stallnote_'.$looper.'_'.$stall_num] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/notesmall.png',
	      '#name' => 'op_stallnote'.$stall['Id'],
	      '#submit' => array('chpprop_property_list_form_submit'));

      if ($readonly) {
	$form['db']['stalledit_'.$looper.'_'.$stall_num] = array('#value' => '');
      } else {
	$form['db']['stalledit_'.$looper.'_'.$stall_num] =
	  array('#type' => 'image_button',
		'#src' => drupal_get_path('module', 'anai').'/editsmall.png',
		'#name' => 'op_stalledit'.$stall['Id'],
		'#submit' => array('chpprop_property_list_form_submit'));
      }
      $stall_num++;
    }
    $looper++;
  }
  return $form;
}

function theme_chpprop_property_levels_table($form) {
  $subheader = '</tr><tr><th>'.t('Level').'</th><th>'.t('Name').'</th>';
  $subheader .= '<th></th><th>'.t('Name').'</th><th>'.t('Operational').'</th><th></th><th></th>';
  $header = array(array('data' => t('Parking level'), 'colspan' => 3),
		  array('data' => t('Stall').$subheader, 'colspan' => 4));
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'numfrags_')) {
      continue;
    }
    $looper = substr($key, strlen('numfrags_'));
    $looper = substr($looper, 0, strpos($looper, '_'));
    $numfrags = substr($key, strlen('numfrags_'.$looper.'_'));
    $rows[] = array(array('data' => drupal_render($form['level_'.$looper]),
			  'rowspan' => $numfrags),
		    array('data' => drupal_render($form['levelalias_'.$looper]),
			  'rowspan' => $numfrags),
		    array('data' => drupal_render($form['leveledit_'.$looper]),
			  'rowspan' => $numfrags,
			  'width' => 1, 'height' => 1),
		    drupal_render($form['stall_'.$looper.'_0']),
		    drupal_render($form['active_'.$looper.'_0']),
		    array('data' => drupal_render($form['stallnote_'.$looper.'_0']),
			  'width' => 1, 'height' => 1),
		    array('data' => drupal_render($form['stalledit_'.$looper.'_0']),
			  'width' => 1, 'height' => 1));
    for ($i = 1; $i < $numfrags; $i++) {
      $rows[] = array(drupal_render($form['stall_'.$looper.'_'.$i]),
		      drupal_render($form['active_'.$looper.'_'.$i]),
		      array('data' => drupal_render($form['stallnote_'.$looper.'_'.$i]),
			    'width' => 1, 'height' => 1),
		      array('data' => drupal_render($form['stalledit_'.$looper.'_'.$i]),
			    'width' => 1, 'height' => 1));
    }
  }
  return theme_table($header, $rows);
}

/****************************************************************************/

function chpprop_property_levels_add_form($form_state) {
  drupal_set_title(t('Add levels'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  // Property
  $property = chdbprop_get_property_id($form_state['storage']['id']);
  $property_id = $property['PropertyId'];
  // Lot
  $policy = chdbprop_get_policy_id($form_state['storage']['lot_num']);
  $lot_id = $policy['LotId'];
  $profile_id = $policy['ProfileId'];
  // Profile
  $profile = chdbprop_get_profile($profile_id);

  // Build policy selection
  $policy_options = array();
  if (chpprop_short_managed_stalls($profile)) {
    $policy_options['ShortPolicy'] = $profile['ShortPolicyAlias'];
  }
  if (chpprop_long_managed_stalls($profile)) {
    $policy_options['LongPolicy'] = $profile['LongPolicyAlias'];
  }
  if (chpprop_valet_managed_stalls($profile)) {
    $policy_options['ValetPolicy'] = t('Valet');
  }

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  if (empty($policy_options)) {
    $form['ajax']['empty'] =
      array('#value' => '<p>'.t('Lot is not configured for any stalls.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpprop_property_levels_add_form_submit_cancel'));
    return $form;
  }

  // Layout policy selection
  if (!isset($form_state['storage']['ajax']['Policy'])) {
    $form_state['storage']['ajax']['Policy'] = key($policy_options);
  }
  if (count($policy_options) > 1) {
    $form['ajax']['Policy'] =
      array('#type' => 'select',
	    '#title' => t('Policy'),
	    '#options' => $policy_options,
	    '#default_value' => $form_state['storage']['ajax']['Policy'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
  } else {
    $form['ajax']['Policy'] =
      array('#type' => 'value',
	    '#value' => $form_state['storage']['ajax']['Policy']);
  }

  // Determine existing versus what's available
  $profile_stalls = array();
  $long = $short = $valet = FALSE;
  if ($form_state['storage']['ajax']['Policy'] === 'LongPolicy') {
    $long = TRUE;
    $levels = explode(',', $profile['LongStallLevels']);
    $counts = explode(',', $profile['LongStallCount']);
  }
  if ($form_state['storage']['ajax']['Policy'] === 'ShortPolicy') {
    $short = TRUE;
    $levels = explode(',', $profile['ShortStallLevels']);
    $counts = explode(',', $profile['ShortStallCount']);
  }
  if ($form_state['storage']['ajax']['Policy'] === 'ValetPolicy') {
    $valet = TRUE;
    $levels = explode(',', $profile['ValetStallLevels']);
    $counts = explode(',', $profile['ValetStallCount']);
  }
  foreach ($levels as $index => $level) {
    $profile_stalls[$level] = $counts[$index];
  }
  $stallevels = chdbprop_get_stallevels($property_id, $lot_id, $long, $short, $valet);

  $existing_levels = array();
  $existing_options = array();
  $level_mapping = array();
  foreach ($stallevels as $level) {
    $existing_levels[$level['Level']] = count(chdbprop_get_stalls($property_id, $lot_id, $level['LevelId']));
    $existing_options[$level['LevelId']] = decode_entities($level['Alias']);
    $level_mapping[$level['Level']] = $level['LevelId'];
  }

  $missing_levels = array();
  foreach ($profile_stalls as $level => $count) {
    if (isset($existing_levels[$level])) {
      continue;
    }
    $missing_levels[$level] = $level;
  }

  $available_stalls = array();
  foreach ($existing_levels as $level => $count) {
    if ($count >= $profile_stalls[$level]) {
      continue;
    }
    $available_stalls[$level_mapping[$level]] = $profile_stalls[$level] - $count;
  }

  // Layout add type options
  $options = array();
  if (!empty($missing_levels)) {
    $options['Level'] = t('Add a parking level entry');
  }
  if (!empty($available_stalls)) {
    $options['SingleStall'] = t('Add single stall to a parking level');
    $options['MultipleStalls'] = t('Add range of stalls to a parking level');
  }
  if (empty($options)) {
    $form['ajax']['empty'] =
      array('#value' => '<p>'.t('Nothing to add.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpprop_property_levels_add_form_submit_cancel'));
    return $form;
  }
  if (isset($form_state['storage']['ajax']['Type']) and
      !in_array($form_state['storage']['ajax']['Type'], array_keys($options))) {
    unset($form_state['storage']['ajax']['Type']);
  }
  if (!isset($form_state['storage']['ajax']['Type'])) {
    $form_state['storage']['ajax']['Type'] = key($options);
  }
  $form['ajax']['Type'] =
    array('#type' => 'radios',
	  '#title' => t('Type'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['Type'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  // Layout conditional
  if ($form_state['storage']['ajax']['Type'] === 'Level') {
    if (!isset($form_state['storage']['ajax']['Level'])) {
      $form_state['storage']['ajax']['Level'] = key($missing_levels);
    }
    $form['ajax']['Level'] =
      array('#type' => 'select',
	    '#title' => t('Level'),
	    '#options' => $missing_levels,
	    '#default_value' => $form_state['storage']['ajax']['Level']);
    if (!isset($form_state['storage']['ajax']['Alias'])) {
      $form_state['storage']['ajax']['Alias'] = '';
    }
    $form['ajax']['Alias'] =
      array('#type' => 'textfield',
	    '#title' => t('Parking level name'),
	    '#maxlength' => 80,
	    '#default_value' => $form_state['storage']['ajax']['Alias'],
	    '#description' => t('Example: Level 1'));
  }

  // Layout conditional
  if ($form_state['storage']['ajax']['Type'] === 'SingleStall') {
    if (isset($form_state['storage']['ajax']['LevelId']) and
	!in_array($form_state['storage']['ajax']['LevelId'], array_keys($existing_options))) {
      unset($form_state['storage']['ajax']['LevelId']);
    }
    if (!isset($form_state['storage']['ajax']['LevelId'])) {
      $form_state['storage']['ajax']['LevelId'] = key($existing_options);
    }
    $form['ajax']['LevelId'] =
      array('#type' => 'select',
	    '#title' => t('Parking level'),
	    '#options' => $existing_options,
	    '#default_value' => $form_state['storage']['ajax']['LevelId']);
    if (!isset($form_state['storage']['ajax']['Alias'])) {
      $form_state['storage']['ajax']['Alias'] = '';
    }
    $form['ajax']['Alias'] =
      array('#type' => 'textfield',
	    '#title' => t('Stall name'),
	    '#maxlength' => 80,
	    '#default_value' => $form_state['storage']['ajax']['Alias'],
	    '#description' => t('Examples: 15, F, Reserved 1, etc.'));
  }

  // Layout conditional
  if ($form_state['storage']['ajax']['Type'] === 'MultipleStalls') {
    if (isset($form_state['storage']['ajax']['LevelId']) and
	!in_array($form_state['storage']['ajax']['LevelId'], array_keys($existing_options))) {
      unset($form_state['storage']['ajax']['LevelId']);
    }
    if (!isset($form_state['storage']['ajax']['LevelId'])) {
      $form_state['storage']['ajax']['LevelId'] = key($existing_options);
    }
    $form['ajax']['LevelId'] =
      array('#type' => 'select',
	    '#title' => t('Parking level'),
	    '#options' => $existing_options,
	    '#default_value' => $form_state['storage']['ajax']['LevelId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));

    if (1 >= $available_stalls[$form_state['storage']['ajax']['LevelId']]) {
      $form['ajax']['empty'] =
	array('#value' => '<p>'.t('No room for adding a range on selected parking level.').'<p>');
      $form['ajax']['cancel'] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	      '#submit' => array('chpprop_property_levels_add_form_submit_cancel'));
      return $form;
    }

    $form['ajax']['RangeMax'] =
      array('#type' => 'value',
	    '#value' => $available_stalls[$form_state['storage']['ajax']['LevelId']]);
    $form['ajax']['rangeinfo'] =
      array('#value' => '<p>'.
	    t('Room for adding NUM more stalls.',
	      array('NUM' => $available_stalls[$form_state['storage']['ajax']['LevelId']])));

    if (!isset($form_state['storage']['ajax']['Prefix'])) {
      $form_state['storage']['ajax']['Prefix'] = '';
    }
    $form['ajax']['Prefix'] =
      array('#type' => 'textfield',
	    '#title' => t('Fixed prefix to the range (optional)'),
	    '#maxlength' => 10,
	    '#default_value' => $form_state['storage']['ajax']['Prefix'],
	    '#description' => t('Will be leading to all stalls<br>'.
				'Example: [A1,A2,A3,...An]'));

    if (!isset($form_state['storage']['ajax']['From'])) {
      $form_state['storage']['ajax']['From'] = '';
    }
    $form['ajax']['From'] =
      array('#type' => 'textfield',
	    '#title' => t('Start value'),
	    '#maxlength' => 60,
	    '#default_value' => $form_state['storage']['ajax']['From']);
    if (!isset($form_state['storage']['ajax']['To'])) {
      $form_state['storage']['ajax']['To'] = '';
    }
    $form['ajax']['To'] =
      array('#type' => 'textfield',
	    '#title' => t('Stop value'),
	    '#maxlength' => 60,
	    '#default_value' => $form_state['storage']['ajax']['To']);
    if (!isset($form_state['storage']['ajax']['Suffix'])) {
      $form_state['storage']['ajax']['Suffix'] = '';
    }
    $form['ajax']['Suffix'] =
      array('#type' => 'textfield',
	    '#title' => t('Fixed suffix to the range (optional)'),
	    '#maxlength' => 10,
	    '#default_value' => $form_state['storage']['ajax']['Suffix'],
	    '#description' => t('Will be trailing to all stalls<br>'.
				'Example: [1R,2R,3R,...nR]'));

    chpprop_property_levels_add_form_validate_save_range_helper($form_state);
  }

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpprop_property_levels_add_form_validate_save'),
	  '#submit' => array('chpprop_property_levels_add_form_submit_save'));
  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_property_levels_add_form_submit_cancel'));
  return $form;
}

function chpprop_property_levels_add_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']['levels_add']);
}

function chpprop_property_levels_add_form_validate_save_range_helper($form_state) {
  // Validates only when To/From fields are filled
  if (isset($form_state['values']['ajax']['From']) and
      !empty($form_state['values']['ajax']['From']) and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['From'])) {
    form_set_error('ajax][From', 'From field must be a non-negative number.');
    return FALSE;
  }
  if (isset($form_state['values']['ajax']['To']) and
      !empty($form_state['values']['ajax']['To']) and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['To'])) {
    form_set_error('ajax][To', 'To field must be a non-negative number.');
    return FALSE;
  }
  if (isset($form_state['values']['ajax']['To']) and
      isset($form_state['values']['ajax']['From']) and
      !empty($form_state['values']['ajax']['From']) and
      !empty($form_state['values']['ajax']['To']) and
      $form_state['values']['ajax']['To'] <= $form_state['values']['ajax']['From']) {
    form_set_error('ajax][To', 'To field must larger than From field.');
    return FALSE;
  }
  if (isset($form_state['values']['ajax']['To']) and
      isset($form_state['values']['ajax']['From']) and
      !empty($form_state['values']['ajax']['From']) and
      !empty($form_state['values']['ajax']['To'])) {
    $range = $form_state['values']['ajax']['To'] - $form_state['values']['ajax']['From'] + 1;
    if ($range > $form_state['values']['ajax']['RangeMax']) {
      form_set_error('ajax][To', 'To field is set higher than available stalls.');
      return FALSE;
    }
  }

  return TRUE;
}

function chpprop_property_levels_add_form_validate_save($form, &$form_state) {
  if (isset($form_state['values']['ajax']['Alias']) and
      empty($form_state['values']['ajax']['Alias'])) {
    form_set_error('ajax][Alias', 'Name field is required.');
    return;
  }
  if (isset($form_state['values']['ajax']['From']) and
      empty($form_state['values']['ajax']['From'])) {
    form_set_error('ajax][From', 'From field is required.');
    return;
  }
  if (isset($form_state['values']['ajax']['To']) and
      empty($form_state['values']['ajax']['To'])) {
    form_set_error('ajax][To', 'To field is required.');
    return;
  }
  if (isset($form_state['values']['ajax']['From']) and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['From'])) {
    form_set_error('ajax][From', 'From field must be a non-negative number.');
    return;
  }
  if (isset($form_state['values']['ajax']['To']) and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['To'])) {
    form_set_error('ajax][To', 'To field must be a non-negative number.');
    return;
  }
  if (isset($form_state['values']['ajax']['To']) and
      isset($form_state['values']['ajax']['From']) and
      $form_state['values']['ajax']['To'] <= $form_state['values']['ajax']['From']) {
    form_set_error('ajax][To', 'To field must larger than From field.');
    return;
  }
  if (isset($form_state['values']['ajax']['To']) and
      isset($form_state['values']['ajax']['From'])) {
    $range = $form_state['values']['ajax']['To'] - $form_state['values']['ajax']['From'] + 1;
    if ($range > $form_state['values']['ajax']['RangeMax']) {
      form_set_error('ajax][To', 'To field is set higher than available stalls.');
      return;
    }
  }
}

function chpprop_property_levels_add_form_submit_save($form, &$form_state) {
  // Property
  $property = chdbprop_get_property_id($form_state['storage']['id']);
  $property_id = $property['PropertyId'];
  // Lot
  $policy = chdbprop_get_policy_id($form_state['storage']['lot_num']);
  $lot_id = $policy['LotId'];

  $long = $short = $valet = FALSE;
  if ($form_state['values']['ajax']['Policy'] === 'LongPolicy') {
    $long = TRUE;
  }
  if ($form_state['values']['ajax']['Policy'] === 'ShortPolicy') {
    $short = TRUE;
  }
  if ($form_state['values']['ajax']['Policy'] === 'ValetPolicy') {
    $valet = TRUE;
  }

  if ($form_state['values']['ajax']['Type'] === 'Level') {
    if (chdbprop_insert_stallevel
	($property_id,
	 $lot_id,
	 $long,
	 $short,
	 $valet,
	 chpprop_genRandomString20(),
	 check_plain($form_state['values']['ajax']['Alias']),
	 $form_state['values']['ajax']['Level'])) {
      drupal_set_message('Could not insert stallevel', 'error');
      return;
    }
  }

  if ($form_state['values']['ajax']['Type'] === 'SingleStall') {
    if (chdbprop_insert_stall
	($property_id,
	 $lot_id,
	 $form_state['values']['ajax']['LevelId'],
	 chpprop_genRandomString20(),
	 check_plain($form_state['values']['ajax']['Alias']))) {
      drupal_set_message('Could not insert stall', 'error');
      return;
    }
  }

  if ($form_state['values']['ajax']['Type'] === 'MultipleStalls') {
    for ($i = $form_state['values']['ajax']['From']; $i <= $form_state['values']['ajax']['To']; $i++) {
      $alias =
	check_plain($form_state['values']['ajax']['Prefix']).
	$i.
	check_plain($form_state['values']['ajax']['Suffix']);
      if (chdbprop_insert_stall
	  ($property_id,
	   $lot_id,
	   $form_state['values']['ajax']['LevelId'],
	   chpprop_genRandomString20(),
	   $alias)) {
	drupal_set_message('Could not insert stall', 'error');
	return;
      }
    }
  }

  unset($form_state['storage']['levels_add']);
}

/****************************************************************************/

function chpprop_property_levels_edit_level_form($form_state) {
  drupal_set_title(t('Edit levels'));
  $id = $form_state['storage']['level_id'];
  $level = chdbprop_get_stallevel_id($id);

  $form['Alias'] =
    array('#type' => 'textfield',
	  '#title' => t('Parking level name'),
	  '#maxlength' => 80,
	  '#default_value' => decode_entities($level['Alias']),
	  '#description' => t('Example: Level 1'));

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpprop_property_levels_add_form_validate_save'),
	  '#submit' => array('chpprop_property_levels_edit_level_form_submit_save'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_property_levels_edit_level_form_submit_cancel'));
  return $form;
}

function chpprop_property_levels_edit_level_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']['levels_edit']);
}

function chpprop_property_levels_edit_level_form_submit_save($form, &$form_state) {
  $id = $form_state['storage']['level_id'];

  if (chdbprop_update_stallevel_id
      ($id,
       check_plain($form_state['values']['Alias']))) {
    drupal_set_message('Could not update stallevel', 'error');
    return;
  }

  unset($form_state['storage']['levels_edit']);
}

function chpprop_property_levels_edit_stall_form($form_state) {
  if ($form_state['storage']['confirm_delete_stall']) {
    return confirm_form($form, 'Are you sure?', 'chpproplist',
			'The stall will be deleted from the system.');
  }

  drupal_set_title(t('Edit stall'));
  $id = $form_state['storage']['stall_id'];
  $stall = chdbprop_get_stall_id($id);

  $form['Active'] =
    array('#type' => 'checkbox',
	  '#title' => t('Stall operational'),
	  '#default_value' => $stall['Active']);

  $form['Alias'] =
    array('#type' => 'textfield',
	  '#title' => t('Stall name'),
	  '#maxlength' => 80,
	  '#default_value' => decode_entities($stall['Alias']),
	  '#description' => t('Examples: 15, F, Reserved 1, etc.'));

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpprop_property_levels_add_form_validate_save'),
	  '#submit' => array('chpprop_property_levels_edit_stall_form_submit_save'));
  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_property_levels_edit_stall_form_submit_cancel'));
  if (user_access('anai chp delete property levels')) {
    $form['delete'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/delete.png',
	    '#submit' => array('chpprop_property_levels_edit_stall_form_submit_delete'));
  }
  return $form;
}

function chpprop_property_levels_edit_stall_form_submit_delete($form, &$form_state) {
  $form_state['storage']['confirm_delete_stall'] = TRUE;
}

function chpprop_property_levels_edit_stall_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']['levels_stall_edit']);
}

function chpprop_property_levels_edit_stall_form_submit_save($form, &$form_state) {
  $id = $form_state['storage']['stall_id'];

  if (chdbprop_update_stall_id
      ($id,
       $form_state['values']['Active'],
       check_plain($form_state['values']['Alias']))) {
    drupal_set_message('Could not update stall', 'error');
    return;
  }

  unset($form_state['storage']['levels_stall_edit']);
}

/****************************************************************************/

function chpprop_property_levels_notes_add_form($form_state) {
  drupal_set_title(t('Add stall note'));

  $form['Note'] =
    array('#type' => 'textarea',
	  '#title' => t('Description'),
	  '#maxlength' => 2048);

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpprop_property_levels_notes_add_form_validate_save'),
	  '#submit' => array('chpprop_property_levels_notes_add_form_submit_save'));

  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_property_levels_notes_add_form_submit_back'));
  return $form;
}

function chpprop_property_levels_notes_add_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['Note'])) {
    form_set_error('Note', t('Description field is required.'));
    return;
  }
}

function chpprop_property_levels_notes_add_form_submit_back($form, &$form_state) {
  unset($form_state['storage']['stall_note_add']);
}

function chpprop_property_levels_notes_add_form_submit_save($form, &$form_state) {
  global $user;
  $id = $form_state['storage']['stall_id'];
  $stall = chdbprop_get_stall_id($id);
  $property_id = $stall['PropertyId'];
  $lot_id = $stall['LotId'];
  $level_id = $stall['LevelId'];
  $stall_id = $stall['StallId'];

  $noteid = chdbprop_get_stallnoteid($property_id, $lot_id, $level_id, $stall_id);
  if (empty($noteid)) {
    if (chdbprop_insert_stallnoteid($property_id, $lot_id, $level_id, $stall_id, $tmp)) {
      drupal_set_message('Could not insert stallnoteid', 'error');
      return;
    }
    $noteid = chdbprop_get_stallnoteid($property_id, $lot_id, $level_id, $stall_id);
  }
  $note_id = $noteid['Counter'];

  if (chdbprop_insert_stallnote
      ($user->uid, $property_id, $lot_id, $level_id, $stall_id, $note_id,
       check_plain($form_state['values']['Note']),
       $id)) {
    drupal_set_message('Could not insert note', 'error');
    return;
  }

  if (chdbprop_update_stallnoteid_id($noteid['Id'])) {
    drupal_set_message('Could not update stallnoteid', 'error');
    return;
  }

  unset($form_state['storage']['stall_note_add']);
}

/****************************************************************************/

function chpprop_property_levels_notes_edit_form($form_state) {
  global $user;
  $id = $form_state['storage']['stall_id'];
  $stall = chdbprop_get_stall_id($id);
  $property_id = $stall['PropertyId'];
  $lot_id = $stall['LotId'];
  $level_id = $stall['LevelId'];
  $stall_id = $stall['StallId'];
  $stallnoteid = $form_state['storage']['stall_note_id'];
  $note = chdbprop_get_stallnote_id($stallnoteid);
  drupal_set_title(t('Update stall note'));

  $created = chpuser_datetime_utc_to_usertimezone($note['CreateTime']);
  $account = user_load(array('uid' => $note['CreatorUID']));
  $form['Master'] =
    array('#type' => 'textarea',
  	  '#title' => t('Note created DATE by USER',
			array('DATE' => $created->format("M j,y H:i"),
			      'USER' => $account->name)),
  	  '#disabled' => TRUE,
  	  '#default_value' => decode_entities($note['Note']));

  $fragments = chdbprop_get_stallnote($property_id, $lot_id, $level_id, $stall_id,
				      $note['NoteId'], TRUE, TRUE);

  foreach ($fragments as $fragment) {
    $created = chpuser_datetime_utc_to_usertimezone($fragment['CreateTime']);
    $account = user_load(array('uid' => $note['CreatorUID']));
    $form['Frag'][] =
      array('#type' => 'textarea',
	    '#title' => t('Addition DATE by USER',
			  array('DATE' => $created->format("M j,y H:i"),
				'USER' => $account->name)),
	    '#disabled' => TRUE,
	    '#default_value' => decode_entities($fragment['Note']));
  }

  $form['Note'] =
    array('#type' => 'textarea',
	  '#title' => t('Description'),
	  '#maxlength' => 2048);

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpprop_property_levels_notes_edit_form_validate_save'),
	  '#submit' => array('chpprop_property_levels_notes_edit_form_submit_save'));

  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_property_levels_notes_edit_form_submit_back'));
  return $form;
}

function chpprop_property_levels_notes_edit_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['Note'])) {
    form_set_error('Note', t('Description field is required.'));
    return;
  }
}

function chpprop_property_levels_notes_edit_form_submit_back($form, &$form_state) {
  unset($form_state['storage']['stall_note_edit']);
}

function chpprop_property_levels_notes_edit_form_submit_save($form, &$form_state) {
  global $user;
  $stallnoteid = $form_state['storage']['stall_note_id'];

  if (chdbprop_update_stallnote
      ($stallnoteid, $user->uid,
       check_plain($form_state['values']['Note']))) {
    drupal_set_message('Could not update note', 'error');
    return;
  }

  unset($form_state['storage']['stall_note_edit']);
}

/****************************************************************************/

function chpprop_property_levels_notes_solve_form($form_state) {
  global $user;
  $id = $form_state['storage']['stall_id'];
  $stall = chdbprop_get_stall_id($id);
  $property_id = $stall['PropertyId'];
  $lot_id = $stall['LotId'];
  $level_id = $stall['LevelId'];
  $stall_id = $stall['StallId'];
  $stallnoteid = $form_state['storage']['stall_note_id'];
  $note = chdbprop_get_stallnote_id($stallnoteid);
  drupal_set_title(t('Resolve stall note'));

  $created = chpuser_datetime_utc_to_usertimezone($note['CreateTime']);
  $account = user_load(array('uid' => $note['CreatorUID']));
  $form['Master'] =
    array('#type' => 'textarea',
  	  '#title' => t('Note created DATE by USER',
			array('DATE' => $created->format("M j,y H:i"),
			      'USER' => $account->name)),
  	  '#disabled' => TRUE,
  	  '#default_value' => decode_entities($note['Note']));

  $fragments = chdbprop_get_stallnote($property_id, $lot_id, $level_id, $stall_id,
				      $note['NoteId'], TRUE, TRUE);

  foreach ($fragments as $fragment) {
    $created = chpuser_datetime_utc_to_usertimezone($fragment['CreateTime']);
    $account = user_load(array('uid' => $note['CreatorUID']));
    $form['Frag'][] =
      array('#type' => 'textarea',
	    '#title' => t('Addition DATE by USER',
			  array('DATE' => $created->format("M j,y H:i"),
				'USER' => $account->name)),
	    '#disabled' => TRUE,
	    '#default_value' => decode_entities($fragment['Note']));
  }

  $form['Note'] =
    array('#type' => 'textarea',
	  '#title' => t('Resolution'),
	  '#maxlength' => 512);

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpprop_property_levels_notes_solve_form_validate_save'),
	  '#submit' => array('chpprop_property_levels_notes_solve_form_submit_save'));

  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_property_levels_notes_solve_form_submit_back'));
  return $form;
}

function chpprop_property_levels_notes_solve_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['Note'])) {
    form_set_error('Note', t('Description field is required.'));
    return;
  }
}

function chpprop_property_levels_notes_solve_form_submit_back($form, &$form_state) {
  unset($form_state['storage']['stall_note_solve']);
}

function chpprop_property_levels_notes_solve_form_submit_save($form, &$form_state) {
  global $user;
  $stallnoteid = $form_state['storage']['stall_note_id'];

  if (chdbprop_stallnote_solved
      ($stallnoteid, $user->uid,
       check_plain($form_state['values']['Note']))) {
    drupal_set_message('Could not solve note', 'error');
    return;
  }

  unset($form_state['storage']['stall_note_solve']);
}

/****************************************************************************/

function chpprop_property_levels_notes_form($form_state) {
  if (isset($form_state['storage']['stall_note_add'])) {
    return chpprop_property_levels_notes_add_form($form_state);
  }
  if (isset($form_state['storage']['stall_note_edit'])) {
    return chpprop_property_levels_notes_edit_form($form_state);
  }
  if (isset($form_state['storage']['stall_note_solve'])) {
    return chpprop_property_levels_notes_solve_form($form_state);
  }

  global $user;
  $id = $form_state['storage']['stall_id'];
  $stall = chdbprop_get_stall_id($id);
  $property_id = $stall['PropertyId'];
  $lot_id = $stall['LotId'];
  $level_id = $stall['LevelId'];
  $stall_id = $stall['StallId'];
  $stallevel = chdbprop_get_stallevel($property_id, $lot_id, $level_id);

  drupal_set_title(t('Stall notes - LEVEL - STALL',
		     array('LEVEL' => decode_entities($stallevel['Alias']),
			   'STALL' => decode_entities($stall['Alias']))));

  $form = array();
  ahah_helper_register($form, $form_state);

  $form['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/back.png',
	  '#submit' => array('chpprop_property_levels_notes_form_submit_back'));

  if (user_access('anai chp add property notes')) {
    $form['add'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/add.png',
	    '#submit' => array('chpprop_property_levels_notes_form_validate_add'));
  }

  $notes = array();
  foreach (chdbprop_get_stallnotes($property_id, $lot_id, $level_id, $stall_id) as $note) {
    if (!$note['Fragment']) {
      $notes[$note['NoteId']]['master'] = $note;
      $notes[$note['NoteId']]['fragments'] = array();
    } else {
      $notes[$note['NoteId']]['fragments'][] = $note;
    }
  }

  if (empty($notes)) {
    $form['empty'] =
      array('#value' => '<p>'.t('Empty.'));
  } else {
    $form['list'] = chpprop_property_levels_notes_table_form($notes);
  }

  return $form;
}

function chpprop_property_levels_notes_form_validate_add($form, &$form_state) {
  $form_state['storage']['stall_note_add'] = TRUE;
}

function chpprop_property_levels_notes_form_submit_back($form, &$form_state) {
  unset($form_state['storage']['levels_stall_note']);
}

function chpprop_property_levels_notes_table_form($entries) {
  global $user;
  $form['db'] =
    array('#type' => 'markup', '#theme' => 'chpprop_property_levels_notes_table');
  if (user_access('anai chp edit property notes')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }
  if (user_access('anai chp solve property notes')) {
    $solveable = FALSE;
  } else {
    $solveable = TRUE;
  }
  $looper = 0;
  foreach ($entries as $note_id => $entry) {
    $form['db']['idd_'.$looper] =
      array('#type' => 'hidden', '#value' => $entry['master']['Id']);

    $created = chpuser_datetime_utc_to_usertimezone($entry['master']['CreateTime']);
    $form['db']['time_'.$looper] = array('#value' => $created->format("M j,y H:i"));

    $account = user_load(array('uid' => $entry['master']['CreatorUID']));
    $form['db']['user_'.$looper] = array('#value' => $account->name);

    $form['db']['noteid_'.$looper] =
      array('#value' => $entry['master']['NoteId']);

    $form['db']['note_'.$looper] =
      array('#value' => nl2br(decode_entities($entry['master']['Note'])));

    $form['db']['numfrags_'.$looper.'_'.count($entry['fragments'])] =
      array('#type' => 'hidden', '#value' => '');

    $frag = 0;
    foreach ($entry['fragments'] as $fragment) {
      $account = user_load(array('uid' => $fragment['CreatorUID']));
      $updated = chpuser_datetime_utc_to_usertimezone($fragment['CreateTime']);
      $form['db']['frag_'.$looper.'_'.$frag] =
    	array('#value' =>
	      '<b>Added '.$updated->format("M j,y H:i").' by '.$account->name.':</b><br>'.
	      nl2br(decode_entities($fragment['Note'])));
      $frag++;
    }

    if ($readonly) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/updatesmall.png',
	      '#name' => 'op_stallnoteedit'.$entry['master']['Id'],
	      '#submit' => array('chpprop_property_list_form_submit'));
    }
    if ($solveable) {
      $form['db']['solve_'.$looper] = array('#value' => '');
    } else {
      $form['db']['solve_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/finalsmall.png',
	      '#name' => 'op_stallnotesolve'.$entry['master']['Id'],
	      '#submit' => array('chpprop_property_list_form_submit'));
    }
    $looper++;
  }
  return $form;
}

function theme_chpprop_property_levels_notes_table($form) {
  // Table Header
  $header = array(t('Created'), t('By'), t('Id'), t('Note'), '', '');
  // Table Rows
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'numfrags_')) {
      continue;
    }
    $looper = substr($key, strlen('numfrags_'));
    $looper = substr($looper, 0, strpos($looper, '_'));
    $numfrags = substr($key, strlen('numfrags_'.$looper.'_'));
    $rows[] = array(array('data' => drupal_render($form['time_'.$looper]),
			  'rowspan' => 1 + $numfrags),
		    array('data' => drupal_render($form['user_'.$looper]),
			  'rowspan' => 1 + $numfrags),
		    array('data' => drupal_render($form['noteid_'.$looper]),
			  'rowspan' => 1 + $numfrags),
		    drupal_render($form['note_'.$looper]),
		    array('data' => drupal_render($form['edit_'.$looper]),
			  'rowspan' => 1 + $numfrags,
			  'width' => 1, 'height' => 1),
		    array('data' => drupal_render($form['solve_'.$looper]),
			  'rowspan' => 1 + $numfrags,
			  'width' => 1, 'height' => 1));
    for ($i = 0; $i < $numfrags; $i++) {
      $rows[] = array(drupal_render($form['frag_'.$looper.'_'.$i]));
    }
  }
  // Render Table
  return theme_table($header, $rows);
}

/****************************************************************************/

function chpprop_property_edit_form($form_state) {
  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  // Property
  $property = chdbprop_get_property_id($form_state['storage']['id']);
  $property_id = $property['PropertyId'];
  // Lot
  $policy = chdbprop_get_policy_id($form_state['storage']['lot_num']);
  $lot_id = $policy['LotId'];
  $profile_id = $policy['ProfileId'];
  $enforcement_id = $policy['EnforcementId'];
  $equipment_id = $policy['EquipmentId'];
  // Profile
  $profile = chdbprop_get_profile($profile_id);
  // Enforcement
  if (!empty($enforcement_id)) {
    $enforcement = chdbprop_get_enforcement($enforcement_id);
  } else {
    $enforcement = NULL;
  }
  // Equipment
  if (!empty($equipment_id)) {
    $equipment = chdbprop_get_equipment($equipment_id);
  } else {
    $equipment = NULL;
  }

  drupal_set_title(t('Edit property'));

  // Layout property
  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['subtag'] = 'config';
  $settings['subframe'] = TRUE;
  $settings['subframetitle'] = t('Property name and address');
  $settings['subframecollapsible'] = TRUE;
  $settings['subframecollapsebtn'] = TRUE;
  $settings['option']['showalias'] = t('Property name');
  $settings['default']['showalias'] = decode_entities($property['Alias']);
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
  $settings['default']['Street'] = decode_entities($property['Street']);
  $settings['default']['City'] = decode_entities($property['City']);
  $settings['default']['PostalCode'] = decode_entities($property['PostalCode']);
  $settings['default']['Province'] = $property['Province'];
  $settings['default']['Country'] = $property['Country'];
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

  $admins = chdbprop_get_propertyadmins($property_id);
  $admin = current($admins);

  if (!empty($admin)) {
    // Layout primary person
    $name = '';
    $name .= isset($prenames[$admin['PrenameId']]) ? $prenames[$admin['PrenameId']].' ' : '';
    $name .= !empty($admin['FirstName']) ? decode_entities($admin['FirstName']).' ' : '';
    $name .= !empty($admin['MiddleName']) ? decode_entities($admin['MiddleName']).' ' : '';
    $name .= !empty($admin['LastName']) ? decode_entities($admin['LastName']).' ' : '';
    $name .= isset($sufnames[$admin['SufnameId']]) ? $sufnames[$admin['SufnameId']].' ' : '';
    $subtag = 'admin'.$admin['Id'].'_'.$admin['AdminId'];

    $settings = array();
    $settings['tag'] = 'ajax';
    $settings['subtag'] = $subtag;
    $settings['subframe'] = TRUE;
    $settings['subframetitle'] = t('Primary contact: ').$name;
    $settings['subframecollapsebtn'] = TRUE;
    $settings['option']['skipnotrequired'] = TRUE;
    $settings['option']['skiplogin'] = TRUE;
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
  }

  while (FALSE !== $admin = next($admins)) {
    $name = '';
    $name .= isset($prenames[$admin['PrenameId']]) ? $prenames[$admin['PrenameId']].' ' : '';
    $name .= !empty($admin['FirstName']) ? decode_entities($admin['FirstName']).' ' : '';
    $name .= !empty($admin['MiddleName']) ? decode_entities($admin['MiddleName']).' ' : '';
    $name .= !empty($admin['LastName']) ? decode_entities($admin['LastName']).' ' : '';
    $name .= isset($sufnames[$admin['SufnameId']]) ? $sufnames[$admin['SufnameId']].' ' : '';
    $subtag = 'admin'.$admin['Id'].'_'.$admin['AdminId'];

    $settings = array();
    $settings['tag'] = 'ajax';
    $settings['subtag'] = $subtag;
    $settings['subframe'] = TRUE;
    $settings['subframetitle'] = t('Administrator: ').$name;
    $settings['subframecollapsebtn'] = TRUE;
    $settings['option']['skipnotrequired'] = TRUE;
    $settings['option']['skiplogin'] = TRUE;
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
    if (!isset($form_state['storage']['ajax'][$subtag]['deleteadmin'])) {
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

  // Equipment
  if (!empty($equipment)) {
    $settings = array();
    $settings['subframe'] = TRUE;
    $settings['subframetitle'] = t('Equipment settings');
    $settings['subframecollapsible'] = TRUE;
    $settings['subframecollapsed'] = TRUE;
    $settings['option']['custom']['key'] = $equipment['Type'] == 0 ? $equipment_id : 'Custom';
    $settings['option']['custom']['value'] = 'Custom';
    $settings['option']['skipprofilename'] = TRUE;
    $settings['default']['EquipmentId'] = $equipment_id;
    $settings['default']['lprCheckbox'] = $equipment['LPR'];
    $settings['default']['infoCheckbox'] = $equipment['InfoBoard'];
    $settings['default']['padCheckbox'] = $equipment['PAD'];
    chpprop_produce_equipment($form, $form_state, $settings);
  }

  // Enforcement
  if (!empty($enforcement)) {
    $settings = array();
    $settings['subframe'] = TRUE;
    $settings['subframetitle'] = t('Enforcement settings');
    $settings['subframecollapsible'] = TRUE;
    $settings['subframecollapsed'] = TRUE;
    $settings['option']['custom']['key'] = $enforcement['Type'] == 0 ? $enforcement_id : 'Custom';
    $settings['option']['custom']['value'] = 'Custom';
    $settings['option']['skipprofilename'] = TRUE;
    $settings['default']['EnforcementId'] = $enforcement_id;
    $settings['default']['ticketCheckbox'] = $enforcement['Ticketing'];
    $settings['default']['towCheckbox'] = $enforcement['Towing'];
    $settings['default']['bootCheckbox'] = $enforcement['Booting'];
    $settings['default']['mobileCheckbox'] = $enforcement['MobilePolicy'];
    $settings['default']['mobileUnits'] = $enforcement['MobileUnits'];
    chpprop_produce_enforcement($form, $form_state, $settings);
  }

  // Lot
  $settings = array();
  $settings['subframe'] = TRUE;
  $settings['subframetitle'] = t('Policy settings');
  $settings['subframecollapsible'] = TRUE;
  $settings['subframecollapsebtn'] = TRUE;
  $settings['option']['skipprofilename'] = TRUE;
  $settings['option']['custom']['key'] = $profile['Type'] == 0 ? $profile_id : 'Custom';
  $settings['option']['custom']['value'] = 'Custom';
  $settings['default']['LotName'] = decode_entities($policy['Alias']);
  $settings['default']['ProfileId'] = $profile_id;
  $settings['default']['rCheckbox'] = ($profile['LongShortPolicy'] or $profile['LongPolicy']) ? 1 : 0;
  $settings['default']['vCheckbox'] = ($profile['LongShortPolicy'] or $profile['ShortPolicy']) ? 1 : 0;
  $settings['default']['valCheckbox'] = $profile['ValetPolicy'] ? 1 : 0;
  $settings['default']['rPolicyAlias'] = decode_entities($profile['LongPolicyAlias']);
  $settings['default']['vPolicyAlias'] = decode_entities($profile['ShortPolicyAlias']);
  $settings['default']['rPolicyType'] = $profile['LongTypeId'];
  $settings['default']['vPolicyType'] = $profile['ShortTypeId'];
  $settings['default']['rPolicyClass'] = $profile['LongClassId'];
  $settings['default']['vPolicyClass'] = $profile['ShortClassId'];
  // Short Policy
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
  // Long Policy
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
  // Valet
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

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpprop_property_add_form_validate_save',
			       'chpprop_property_edit_form_validate_save'),
	  '#submit' => array('chpprop_property_edit_form_submit_save'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_property_edit_form_submit_back'));
  return $form;
}

function chpprop_property_edit_form_validate_save($form, &$form_state) {
  foreach ($form_state['values']['ajax'] as $key => $value) {
    if (substr($key, 0, strlen('admin')) !== 'admin') {
      continue;
    }
    $id = substr($key, strlen('admin'), strpos($key, '_') - strlen('admin'));
    $admin_id = substr($key, 1 + strpos($key, '_'));
    $subtag = 'admin'.$id.'_'.$admin_id;

    if ($form_state['values']['ajax'][$subtag]['deleteadmin']) {
      continue;
    }

    if (isset($form_state['values']['ajax'][$subtag]['FirstName']) and
	empty($form_state['values']['ajax'][$subtag]['FirstName'])) {
      form_set_error('ajax][person][FirstName', 'First name field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax'][$subtag]['LastName']) and
	empty($form_state['values']['ajax'][$subtag]['LastName'])) {
      form_set_error('ajax][person][LastName', 'Last name field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax'][$subtag]['Street']) and
	empty($form_state['values']['ajax'][$subtag]['Street'])) {
      form_set_error('ajax][person][Street', 'Street field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax'][$subtag]['City']) and
	empty($form_state['values']['ajax'][$subtag]['City'])) {
      form_set_error('ajax][person][City', 'City field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax'][$subtag]['PostalCode']) and
	empty($form_state['values']['ajax'][$subtag]['PostalCode'])) {
      form_set_error('ajax][person][PostalCode', 'Postal code field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax'][$subtag]['Phone']) and
	empty($form_state['values']['ajax'][$subtag]['Phone'])) {
      form_set_error('ajax][person][Phone', 'Phone field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax'][$subtag]['Email']) and
	empty($form_state['values']['ajax'][$subtag]['Email'])) {
      form_set_error('ajax][person][Email', 'Email field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax'][$subtag]['OtherRole']) and
	empty($form_state['values']['ajax'][$subtag]['OtherRole'])) {
      form_set_error('ajax][person][OtherRole', 'Other role field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax'][$subtag]['Department']) and
	empty($form_state['values']['ajax'][$subtag]['Department'])) {
      form_set_error('ajax][person][Department', 'Department field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax'][$subtag]['Title']) and
	empty($form_state['values']['ajax'][$subtag]['Title'])) {
      form_set_error('ajax][person][Title', 'Title field is required.');
      return FALSE;
    }

    // Check GEO Coding
    include_once drupal_get_path('module', 'location') .'/location.inc';
    $location = array();
    $location['street'] = $form_state['values']['ajax'][$subtag]['Street'];
    $location['city'] = $form_state['values']['ajax'][$subtag]['City'];
    $location['postal_code'] = $form_state['values']['ajax'][$subtag]['PostalCode'];
    $location['province'] = $form_state['values']['ajax'][$subtag]['Province'];
    $location['country'] = $form_state['values']['ajax'][$subtag]['Country'];
    $pos = location_latlon_exact($location);
    if ($pos) {
      $form_state['storage']['ajax'][$subtag]['Lat'] = $pos['lat'];
      $form_state['storage']['ajax'][$subtag]['Lon'] = $pos['lon'];
    }
  }
}

function chpprop_property_edit_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpproplist';
}

function chpprop_property_edit_form_submit_save($form, &$form_state) {
  global $user;
  // Property
  $id = $form_state['storage']['id'];
  $property = chdbprop_get_property_id($id);
  $property_id = $property['PropertyId'];
  // Policy
  $policy = chdbprop_get_policy_id($form_state['storage']['lot_num']);
  $lot_id = $policy['LotId'];
  $profile_id = $policy['ProfileId'];
  $enforcement_id = $policy['EnforcementId'];
  $equipment_id = $policy['EquipmentId'];
  // Profile
  $profile = chdbprop_get_profile($profile_id);
  // Enforcement
  if (!empty($enforcement_id)) {
    $enforcement = chdbprop_get_enforcement($enforcement_id);
  } else {
    $enforcement = NULL;
  }
  // Equipment
  if (!empty($equipment_id)) {
    $equipment = chdbprop_get_equipment($equipment_id);
  } else {
    $equipment = NULL;
  }

  $alias = check_plain($form_state['values']['ajax']['config']['Alias']);

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

  if (isset($form_state['storage']['ajax']['config']['Lat'])) {
    $lat = $form_state['storage']['ajax']['config']['Lat'];
  } else {
    $lat = NULL;
  }

  if (isset($form_state['storage']['ajax']['config']['Lon'])) {
    $lon = $form_state['storage']['ajax']['config']['Lon'];
  } else {
    $lon = NULL;
  }

  if (chdbprop_update_property
      ($id, $alias,
       $street, $city, $postalcode, $province, $country)) {
    drupal_set_message('Couldn\'t update property', 'error');
    return;
  }

  if (!$property['ManLatLon']) {
    if (chdbprop_update_property_latlon
	($id, $lat, $lon)) {
      drupal_set_message('Couldn\'t update property lat/lon', 'error');
      return;
    }
  }

  // Decide policy in use
  $long_policy = 0;
  $short_policy = 0;
  $longshort_policy = 0;
  $valet_policy = 0;
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

  //anai_dbg($profile_id);
  //anai_dbg($form_state['values']['ajax']['policy']['ProfileId']);
  if ($form_state['values']['ajax']['policy']['ProfileId'] === $profile_id and
      $profile['Type'] == 0) {
    //drupal_set_message('Editing custom profile');
    if (chdbprop_update_profile
	($profile['Id'],
	 $profile['Alias'], // check_plain areade executed
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
      drupal_set_message('Could not update custom settings', 'error');
      return;
    }

    if (chdbprop_update_policy
	($property_id, $lot_id,
	 TRUE, check_plain($form_state['values']['ajax']['policy']['LotName']))) {
      drupal_set_message('Could not update policy', 'error');
      return;
    }
  }
  elseif (($form_state['values']['ajax']['policy']['ProfileId'] === 'Custom') and
	  $profile['Type'] != 0) {
    //drupal_set_message('Changing from predefined to custom profile');

    $new_profile_id = chpprop_genRandomString20();

    if (chdbprop_insert_default_profile
	($user->uid,
	 $property_id,
	 $new_profile_id,
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
	 implode(',',array_keys(array_filter($form_state['values']['ajax']['policy']['valModifiers']))),
	 $profile_index)) {
      drupal_set_message('Could not insert custom profile', 'error');
      return;
    }

    if (chdbprop_update_policy
	($property_id, $lot_id,
	 TRUE, check_plain($form_state['values']['ajax']['policy']['LotName']),
	 TRUE, $new_profile_id)) {
      drupal_set_message('Could not update policy', 'error');
      return;
    }
  }
  elseif ($form_state['values']['ajax']['policy']['ProfileId'] !== $profile_id and
	  $profile['Type'] == 0) {
    //drupal_set_message('Changing from custom to predefined profile');

    if (chdbprop_delete_profile_id($profile['Id'])) {
      drupal_set_message('Could not delete custom profile', 'error');
      return;
    }

    if (chdbprop_update_policy
	($property_id, $lot_id,
	 TRUE, check_plain($form_state['values']['ajax']['policy']['LotName']),
	 TRUE, $form_state['values']['ajax']['policy']['ProfileId'])) {
      drupal_set_message('Could not update policy', 'error');
      return;
    }
  }
  elseif ($form_state['values']['ajax']['policy']['ProfileId'] !== $profile_id and
	  $profile['Type'] != 0) {
    //drupal_set_message('Changing from one predefined profile to another');
    if (chdbprop_update_policy
	($property_id, $lot_id,
	 TRUE, check_plain($form_state['values']['ajax']['policy']['LotName']),
	 TRUE, $form_state['values']['ajax']['policy']['ProfileId'])) {
      drupal_set_message('Could not update policy', 'error');
      return;
    }
  }
  else {
    //drupal_set_message('Not changing any profile setting, just the alias');
    if (chdbprop_update_policy
	($property_id, $lot_id,
	 TRUE, check_plain($form_state['values']['ajax']['policy']['LotName']))) {
      drupal_set_message('Could not update policy', 'error');
      return;
    }
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
      if (chdbprop_delete_propertyadmin_id($id)) {
	drupal_set_message('Could not delete property admin', 'error');
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

    if (chdbprop_update_propertyadmin
	($id,
	 $prename_id, $firstname, $middlename, $lastname, $sufname_id,
	 $street, $city, $postalcode, $province, $country,
	 $phone, $email, $emailnotification,
	 $role, $department, $title)) {
      drupal_set_message('Could not update property admin', 'error');
      return;
    }

    if (chdbprop_update_propertyadmin_latlon
	($id, $lat, $lon)) {
      drupal_set_message('Could not update property admin lat/lon', 'error');
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

    if (chdbprop_insert_propertyadmin
	($property_id, $admin_id, $lat, $lon,
	 $prename_id, $firstname, $middlename, $lastname, $sufname_id,
	 $street, $city, $postalcode, $province, $country,
	 $phone, $email, $emailnotification,
	 $role, $department, $title,
	 $id)) {
      drupal_set_message('Could not insert property admin', 'error');
      return;
    }
  }

  if (!empty($enforcement)) {
    $ticketing = $form_state['values']['ajax']['enforcement']['ticketCheckbox'];
    $booting = $form_state['values']['ajax']['enforcement']['bootCheckbox'];
    $towing = $form_state['values']['ajax']['enforcement']['towCheckbox'];

    $mobile_policy = $form_state['values']['ajax']['enforcement']['mobileCheckbox'];
    $num_mobiles = $form_state['values']['ajax']['enforcement']['mobileUnits'];

    if ($form_state['values']['ajax']['enforcement']['EnforcementId'] === $enforcement_id and
	$enforcement['Type'] == 0) {
      //drupal_set_message('Editing custom enforcement');
      if (chdbprop_update_enforcement
	  ($enforcement['Id'],
	   $ticketing,
	   $booting,
	   $towing,
	   $mobile_policy,
	   $num_mobiles)) {
	drupal_set_message('Could not insert enforcement', 'error');
	return;
      }
    }
    elseif (($form_state['values']['ajax']['enforcement']['EnforcementId'] === 'Custom') and
	    $enforcement['Type'] != 0) {
      //drupal_set_message('Changing from predefined to custom enforcement');

      $new_enforcement_id = chpprop_genRandomString20();

      if (chdbprop_insert_enforcement
	  ($user->uid,
	   $new_enforcement_id,
	   FALSE,
	   $ticketing,
	   $booting,
	   $towing,
	   $mobile_policy,
	   $num_mobiles,
	   $enforcement_index)) {
	drupal_set_message('Could not insert custom enforcement', 'error');
	return;
      }

      if (chdbprop_update_policy
	  ($property_id, $lot_id,
	   FALSE, NULL, // alias
	   FALSE, NULL, // profile
	   TRUE, $new_enforcement_id)) {
	drupal_set_message('Could not update policy', 'error');
	return;
      }
    }
    elseif ($form_state['values']['ajax']['enforcement']['EnforcementId'] !== $enforcement_id and
	    $enforcement['Type'] == 0) {
      //drupal_set_message('Changing from custom to predefined enforcement');

      if (chdbprop_delete_enforcement_id($enforcement['Id'])) {
	drupal_set_message('Could not delete custom enforcement', 'error');
	return;
      }

      if (chdbprop_update_policy
	  ($property_id, $lot_id,
	   FALSE, NULL, // alias
	   FALSE, NULL, // profile
	   TRUE, $form_state['values']['ajax']['enforcement']['EnforcementId'])) {
	drupal_set_message('Could not update policy', 'error');
	return;
      }
    }
    elseif ($form_state['values']['ajax']['enforcement']['EnforcementId'] !== $enforcement_id and
	    $enforcement['Type'] != 0) {
      //drupal_set_message('Changing from one predefined enforcement to another');
      if (chdbprop_update_policy
	  ($property_id, $lot_id,
	   FALSE, NULL, // alias
	   FALSE, NULL, // profile
	   TRUE, $form_state['values']['ajax']['enforcement']['EnforcementId'])) {
	drupal_set_message('Could not update policy', 'error');
	return;
      }
    }
    else {
      drupal_set_message('Not changing any enforcement setting, just the alias');
    }
  }

  if (!empty($equipment)) {
    $lpr = $form_state['values']['ajax']['equipment']['lprCheckbox'];
    $info_board = $form_state['values']['ajax']['equipment']['infoCheckbox'];
    $pad = $form_state['values']['ajax']['equipment']['padCheckbox'];

    if ($form_state['values']['ajax']['equipment']['EquipmentId'] === $equipment_id and
	$equipment['Type'] == 0) {
      //drupal_set_message('Editing custom equipment');
      if (chdbprop_update_equipment
	  ($equipment['Id'],
	   $lpr, $info_board, $pad)) {
	drupal_set_message('Could not insert equipment', 'error');
	return;
      }
    }
    elseif (($form_state['values']['ajax']['equipment']['EquipmentId'] === 'Custom') and
	    $equipment['Type'] != 0) {
      //drupal_set_message('Changing from predefined to custom equipment');

      $new_equipment_id = chpprop_genRandomString20();

      if (chdbprop_insert_equipment
	  ($user->uid, $new_equipment_id,
	   FALSE,
	   $lpr, $info_board, $pad,
	   $equipment_index)) {
	drupal_set_message('Could not insert custom equipment', 'error');
	return;
      }

      if (chdbprop_update_policy
	  ($property_id, $lot_id,
	   FALSE, NULL, // alias
	   FALSE, NULL, // profile
	   FALSE, NULL, // enforcement
	   TRUE, $new_equipment_id)) {
	drupal_set_message('Could not update policy', 'error');
	return;
      }
    }
    elseif ($form_state['values']['ajax']['equipment']['EquipmentId'] !== $equipment_id and
	    $equipment['Type'] == 0) {
      //drupal_set_message('Changing from custom to predefined equipment');

      if (chdbprop_delete_equipment_id($equipment['Id'])) {
	drupal_set_message('Could not delete custom equipment', 'error');
	return;
      }

      if (chdbprop_update_policy
	  ($property_id, $lot_id,
	   FALSE, NULL, // alias
	   FALSE, NULL, // profile
	   FALSE, NULL, // enforcement
	   TRUE, $form_state['values']['ajax']['equipment']['EquipmentId'])) {
	drupal_set_message('Could not update policy', 'error');
	return;
      }
    }
    elseif ($form_state['values']['ajax']['equipment']['EquipmentId'] !== $equipment_id and
	    $equipment['Type'] != 0) {
      //drupal_set_message('Changing from one predefined equipment to another');
      if (chdbprop_update_policy
	  ($property_id, $lot_id,
	   FALSE, NULL, // alias
	   FALSE, NULL, // profile
	   FALSE, NULL, // enforcement
	   TRUE, $form_state['values']['ajax']['equipment']['EquipmentId'])) {
	drupal_set_message('Could not update policy', 'error');
	return;
      }
    }
    else {
      drupal_set_message('Not changing any equipment setting, just the alias');
    }
  }

  // Handheld Poll
  if (chdbprop_insert_agentpoll_lot($property_id, $lot_id)) {
    drupal_set_message('Could not insert property poll', 'error');
    return;
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chpproplist';
}

function chpprop_property_list_form_submit($form, &$form_state) {
  if (isset($form_state['storage']['confirm_delete_stall'])) {
    $id = $form_state['storage']['stall_id'];
    chdbprop_delete_stall_id($id);
    unset($form_state['storage']['confirm_delete_stall']);
    unset($form_state['storage']['levels_stall_edit']);
    return;
  }
  if ($form_state['storage']['delete_lot']) {
    $form_state['storage']['confirm_delete_lot'] = TRUE;
    return;
  }
  if ($form_state['storage']['delete_property']) {
    $form_state['storage']['confirm_delete_property'] = TRUE;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_notes')) {
      $id = substr($key, strlen('op_notes'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['id'] = $id;
    $form_state['storage']['detailed_notes'] = TRUE;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_editnote')) {
      $id = substr($key, strlen('op_editnote'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['note_id'] = $id;
    $form_state['storage']['notes_edit'] = TRUE;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_solvenote')) {
      $id = substr($key, strlen('op_solvenote'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['note_id'] = $id;
    $form_state['storage']['notes_solve'] = TRUE;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_expand')) {
      $id = substr($key, strlen('op_expand'));
      break;
    }
  }
  if ($id > 0) {
    unset($form_state['storage']);
    $form_state['redirect'] = 'chppropexpand/'.$id;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_leveledit')) {
      $id = substr($key, strlen('op_leveledit'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['level_id'] = $id;
    $form_state['storage']['levels_edit'] = TRUE;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_stalledit')) {
      $id = substr($key, strlen('op_stalledit'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['stall_id'] = $id;
    $form_state['storage']['levels_stall_edit'] = TRUE;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_stallnote')) {
      $id = substr($key, strlen('op_stallnote'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['stall_id'] = $id;
    $form_state['storage']['levels_stall_note'] = TRUE;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_stallnoteedit')) {
      $id = substr($key, strlen('op_stallnoteedit'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['stall_note_id'] = $id;
    $form_state['storage']['stall_note_edit'] = TRUE;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_stallnotesolve')) {
      $id = substr($key, strlen('op_stallnotesolve'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['stall_note_id'] = $id;
    $form_state['storage']['stall_note_solve'] = TRUE;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_view')) {
      $id = substr($key, strlen('op_view'));
      $id = substr($id, 0, strpos($id, '_'));
      $lot_num = substr($key, strlen('op_view'.$id.'_'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['id'] = $id;
    $form_state['storage']['lot_num'] = $lot_num;
    $form_state['storage']['detailed_view'] = TRUE;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_levels')) {
      $id = substr($key, strlen('op_levels'));
      $id = substr($id, 0, strpos($id, '_'));
      $lot_num = substr($key, strlen('op_levels'.$id.'_'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['id'] = $id;
    $form_state['storage']['lot_num'] = $lot_num;
    $form_state['storage']['detailed_levels'] = TRUE;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_details')) {
      $id = substr($key, strlen('op_details'));
      $id = substr($id, 0, strpos($id, '_'));
      $lot_num = substr($key, strlen('op_details'.$id.'_'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['id'] = $id;
    $form_state['storage']['lot_num'] = $lot_num;
    $form_state['storage']['detailed_edit'] = TRUE;
    return;
  }
}



/**********
 ********** Property expand
 ********** Address: chppropexpand
 ********** Access:  'anai chp edit properties'
 *********/



function chpprop_property_expand_form($form_state, $id) {
  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $form_state['storage']['id'] = $id;
  $form['id'] = array('#type' => 'hidden', '#value' => $id);

  $property = chdbprop_get_property_id($id);
  $property_id = $property['PropertyId'];

  drupal_set_title(t('Expand - NAME', array('NAME' => decode_entities($property['Alias']))));

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  // Create expand options
  $options = array();
  $options['Lot'] = t('Add lot to property');
  $options['Enforcement'] = t('Add enforcement to lot');
  $options['Equipment'] = t('Add equipment to lot');

  // Layout expand options
  if (!isset($form_state['storage']['ajax']['Type'])) {
    $form_state['storage']['ajax']['Type'] = key($options);
  }
  $form['ajax']['Type'] =
    array('#type' => 'radios',
	  '#title' => t('Type of expansion'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['Type'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  $ok_to_continue = TRUE;

  // Lot

  // Enforcement
  if ($form_state['storage']['ajax']['Type'] === 'Enforcement') {
    $policies = chdbprop_get_policies($property_id);
    $options = array();
    foreach ($policies as $value) {
      $options[$value['LotId']] = decode_entities($value['Alias']);
    }
    if (!isset($form_state['storage']['ajax']['LotId'])) {
      $form_state['storage']['ajax']['LotId'] = key($options);
    }
    $form['ajax']['LotId'] =
      array('#type' => 'select',
	    '#title' => t('Lot'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['LotId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));

    foreach ($policies as $value) {
      if ($form_state['storage']['ajax']['LotId'] === $value['LotId']) {
	if (!empty($value['EnforcementId'])) {
	  $ok_to_continue = FALSE;
	  $form['ajax']['Info'] = array('#value' => '<p>'.t('The lot already has enforcement.').'<p>');
	  break;
	}
      }
    }
  }

  // Equipment
  if ($form_state['storage']['ajax']['Type'] === 'Equipment') {
    $policies = chdbprop_get_policies($property_id);
    $options = array();
    foreach ($policies as $value) {
      $options[$value['LotId']] = decode_entities($value['Alias']);
    }
    if (!isset($form_state['storage']['ajax']['LotId'])) {
      $form_state['storage']['ajax']['LotId'] = key($options);
    }
    $form['ajax']['LotId'] =
      array('#type' => 'select',
	    '#title' => t('Lot'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['LotId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));

    foreach ($policies as $value) {
      if ($form_state['storage']['ajax']['LotId'] === $value['LotId']) {
	if (!empty($value['EquipmentId'])) {
	  $ok_to_continue = FALSE;
	  $form['ajax']['Info'] = array('#value' => '<p>'.t('The lot already has equipment.').'<p>');
	  break;
	}
      }
    }
  }

 // Create a 2-stage wizard
  if (strpos($form_state['clicked_button']['#id'], 'next') !== FALSE) {
    $form_state['storage']['ajax']['Wizard'] = 2;
  }
  if (!isset($form_state['storage']['ajax']['Wizard'])) {
    $form_state['storage']['ajax']['Wizard'] = 1;
  }
  $form['ajax']['Wizard'] =
    array('#type' => 'hidden',
	  '#value' => $form_state['storage']['ajax']['Wizard']);

  // Force a "Next" button click before showing forms
  if ($ok_to_continue) {
    $form['ajax']['next'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/next.png',
	    '#ahah' => array('event' => 'click',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
  }

  if (2 > $form_state['storage']['ajax']['Wizard']) {
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpprop_property_expand_form_submit_back'));
    return $form;
  } else {
    unset($form['ajax']['next']);
  }

  // Disable option of changing after selection made
  $form['ajax']['Type']['#disabled'] = TRUE;

  // Continue by creating different expansion forms

  // Lot
  if ($form_state['storage']['ajax']['Type'] === 'Lot') {
    $settings = array();
    $settings['option']['skipprofilename'] = TRUE;
    chpprop_produce_policy($form, $form_state, $settings);
  }

  // Enforcement
  if ($form_state['storage']['ajax']['Type'] === 'Enforcement') {
    $settings = array();
    $settings['option']['skipprofilename'] = TRUE;
    chpprop_produce_enforcement($form, $form_state, $settings);
  }

  // Equipment
  if ($form_state['storage']['ajax']['Type'] === 'Equipment') {
    $settings = array();
    $settings['option']['skipprofilename'] = TRUE;
    chpprop_produce_equipment($form, $form_state, $settings);
  }

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpprop_property_expand_form_validate_save'),
	  '#submit' => array('chpprop_property_expand_form_submit_save'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpprop_property_expand_form_submit_back'));
  return $form;
}

function chpprop_property_expand_form_validate_save($form, &$form_state) {
  if (!chpprop_property_add_form_validate_save_policy_helper($form_state)) {
    return;
  }
}

function chpprop_property_expand_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpproplist';
}

function chpprop_property_expand_form_submit_save($form, &$form_state) {
  global $user;
  $id = $form_state['values']['id'];
  $property = chdbprop_get_property_id($id);
  $property_id = $property['PropertyId'];

  // Lot
  if ($form_state['values']['ajax']['Type'] === 'Lot') {
    $lot_id = chpprop_genRandomString20();
    $profile_id = $form_state['values']['ajax']['policy']['ProfileId'];

    if ($profile_id !== 'Custom') {
      if (chdbprop_insert_policy
	  ($property_id,
	   $lot_id,
	   $profile_id,
	   check_plain($form_state['values']['ajax']['policy']['LotName']))) {
	drupal_set_message('Could not insert new policy', 'error');
	return;
      }

      unset($form_state['storage']);
      $form_state['redirect'] = 'chpproplist';
      return;
    }

    // Custom lot
    $profile_id = chpprop_genRandomString20();

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

    if (chdbprop_insert_default_profile
	($user->uid,
	 $property_id, // PropertyId used as Alias for default profile
	 $profile_id,
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
	 implode(',',array_keys(array_filter($form_state['values']['ajax']['policy']['valModifiers']))),
	 $profile_index)) {
      drupal_set_message('Could not insert new custom lot', 'error');
      return;
    }

    if (chdbprop_insert_policy
	($property_id,
	 $lot_id,
	 $profile_id,
	 check_plain($form_state['values']['ajax']['policy']['LotName']))) {
      drupal_set_message('Could not insert new policy', 'error');
      return;
    }
  }

  // Enforcement
  if ($form_state['values']['ajax']['Type'] === 'Enforcement') {
    $lot_id = $form_state['values']['ajax']['LotId'];
    $enforcement_id = $form_state['values']['ajax']['enforcement']['EnforcementId'];

    if ($enforcement_id !== 'Custom') {
      if (chdbprop_update_policy
	  ($property_id, $lot_id,
	   FALSE, NULL, // alias
	   FALSE, NULL, // profile
	   TRUE, $enforcement_id)) {
	drupal_set_message('Could not update enforcement policy', 'error');
	return;
      }
    } else {
      $ticketing = $form_state['values']['ajax']['enforcement']['ticketCheckbox'];
      $booting = $form_state['values']['ajax']['enforcement']['bootCheckbox'];
      $towing = $form_state['values']['ajax']['enforcement']['towCheckbox'];
      $mobile_policy = $form_state['values']['ajax']['enforcement']['mobileCheckbox'];
      $num_mobiles = $form_state['values']['ajax']['enforcement']['mobileUnits'];

      // Custom settings
      $enforcement_id = chpprop_genRandomString20();

      if (chdbprop_insert_enforcement
	  ($user->uid,
	   $enforcement_id,
	   FALSE, // not a profile
	   $ticketing,
	   $booting,
	   $towing,
	   $mobile_policy,
	   $num_mobiles,
	   $id)) {
	drupal_set_message('Could not insert enforcement', 'error');
	return;
      }

      if (chdbprop_update_policy
	  ($property_id, $lot_id,
	   FALSE, NULL, // alias
	   FALSE, NULL, // profile
	   TRUE, $enforcement_id)) {
	drupal_set_message('Could not update enforcement', 'error');
	return;
      }
    }
  }

  // Equipment
  if ($form_state['values']['ajax']['Type'] === 'Equipment') {
    $lot_id = $form_state['values']['ajax']['LotId'];
    $equipment_id = $form_state['values']['ajax']['equipment']['EquipmentId'];

    if ($equipment_id !== 'Custom') {
      if (chdbprop_update_policy
	  ($property_id, $lot_id,
	   FALSE, NULL, // alias
	   FALSE, NULL, // profile
	   FALSE, NULL, // enforcement
	   TRUE, $equipment_id)) {
	drupal_set_message('Could not update equipment policy', 'error');
	return;
      }
    } else {
      $lpr = $form_state['values']['ajax']['equipment']['lprCheckbox'];
      $info_board = $form_state['values']['ajax']['equipment']['infoCheckbox'];
      $pad = $form_state['values']['ajax']['equipment']['padCheckbox'];

      // Custom settings
      $equipment_id = chpprop_genRandomString20();

      if (chdbprop_insert_equipment
	  ($user->uid, $equipment_id,
	   FALSE, // not a profile
	   $lpr, $info_board, $pad,
	   $id)) {
	drupal_set_message('Could not insert equipment', 'error');
	return;
      }

      if (chdbprop_update_policy
	  ($property_id, $lot_id,
	   FALSE, NULL, // alias
	   FALSE, NULL, // profile
	   FALSE, NULL, // enforcement
	   TRUE, $equipment_id)) {
	drupal_set_message('Could not update equipment policy', 'error');
	return;
      }
    }
  }

  // Handheld Poll
  if (chdbprop_insert_agentpoll_lot($property_id, $lot_id)) {
    drupal_set_message('Could not insert property poll', 'error');
    return;
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chpproplist';
}



