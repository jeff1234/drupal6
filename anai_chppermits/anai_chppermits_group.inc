<?php
// $Id$
require_once("anai_chppermits.inc");
require_once("anai_chppermits_pay.inc");



/**********
 ********** Group add
 ********** Address: chppermgrpadd
 ********** Access:  'anai chp add permit group'
 *********/



function chpperm_group_add_form($form_state) {
  drupal_set_title(t('Add permit group'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['show_companies'] = TRUE;
  $settings['show_regions'] = TRUE;
  $settings['show_clients'] = TRUE;
  //$settings['display_single_company'] = TRUE; //
  //$settings['display_single_region'] = TRUE; //
  //$settings['display_single_client'] = TRUE; //
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
	    '#submit' => array('chpperm_group_add_form_submit_back'));

    return $form;
  }

  // Create property options
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

  if (empty($property_options)) {
    $form['ajax']['error'] =
      array('#value' => '<p>'.t('Property missing.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpperm_group_add_form_submit_back'));

    return $form;
  }

  /**
   * Wizard initiate
   */

  if (strpos($form_state['clicked_button']['#id'], 'nextfs1') !== FALSE) {
    if (chpperm_group_add_form_validate_save_helper_name($form_state)) {
      $form_state['storage']['ajax']['Wizard'] = 2;
    }
  } elseif(strpos($form_state['clicked_button']['#id'], 'nextfs2') !== FALSE) {
    if (chpperm_group_add_form_validate_save_helper_person($form_state)) {
      $form_state['storage']['ajax']['Wizard'] = 3;
    }
  } elseif(strpos($form_state['clicked_button']['#id'], 'nextfs3') !== FALSE) {
    if (chpperm_group_add_form_validate_save_helper_details($form_state)) {
      $form_state['storage']['ajax']['Wizard'] = 4;
    }
  } elseif(strpos($form_state['clicked_button']['#id'], 'nextfs4') !== FALSE) {
    if (chpperm_group_add_form_validate_save_helper_pay($form_state)) {
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
  if ($form_state['storage']['ajax']['Wizard'] > 1) {
    $form['ajax']['CompanyId']['#disabled'] = TRUE;
    $form['ajax']['RegionId']['#disabled'] = TRUE;
    $form['ajax']['ClientId']['#disabled'] = TRUE;
    $form['ajax']['PropertyId']['#disabled'] = TRUE;
    $form['ajax']['LotId']['#disabled'] = TRUE;
    $form['ajax']['all_lots']['#disabled'] = TRUE;
  }

  /**
   * Wizard name and proprties
   */

  $form['ajax']['Name'] =
    array('#type' => 'fieldset',
	  '#title' => t('Name and proprties'),
	  '#collapsible' => TRUE,
	  '#collapsed' => $form_state['storage']['ajax']['Wizard'] !== 1);

  // Create permit options
  $options = array();
  $options['Group'] = t('Permit group account for multiple vehicles');
  $options['Member'] = t('Permit account for single vehicle');

  // Layout permit options
  if (!isset($form_state['storage']['ajax']['Name']['PermitCategory'])) {
    $form_state['storage']['ajax']['Name']['PermitCategory'] = key($options);
  }
  $form['ajax']['Name']['PermitCategory'] =
    array('#type' => 'radios',
	  '#title' => t('Category'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['Name']['PermitCategory'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  if ($form_state['storage']['ajax']['Name']['PermitCategory'] === 'Group') {
    $title = t('Group name');
  } else {
    $title = t('Account name');
  }

  // Layout group name
  if (!isset($form_state['storage']['ajax']['Name']['Alias'])) {
    $form_state['storage']['ajax']['Name']['Alias'] = '';
  }
  $form['ajax']['Name']['Alias'] =
    array('#type' => 'textfield',
	  '#title' => $title,
	  '#maxlength' => 80,
	  '#default_value' => $form_state['storage']['ajax']['Name']['Alias']);

  // Layout property options
  if (!isset($form_state['storage']['ajax']['Name']['PropertyId'])) {
    $form_state['storage']['ajax']['Name']['PropertyId'] = key($property_options);
  }
  if (1 < count($property_options)) {
    $form['ajax']['Name']['PropertyId'] =
      array('#type' => 'select',
	    '#title' => t('Property'),
	    '#options' => $property_options,
	    '#default_value' => $form_state['storage']['ajax']['Name']['PropertyId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax', 'Name')),
			     'wrapper' => 'ajax-name-wrapper'));
  }

  // Create lot options
  $lot_options = array();
  $policies = chpprop_retrieve_policies($user->uid, $company_id);
  if (!empty($policies)) {
    if (isset($policies[$form_state['storage']['ajax']['Name']['PropertyId']])) {
      foreach ($policies[$form_state['storage']['ajax']['Name']['PropertyId']] as $lot_id => $policy) {
	$lot_options[$lot_id] = decode_entities($policy['data']['Alias']);
      }
    }
  }

  if (empty($lot_options)) {
    $form['ajax']['error'] =
      array('#value' => '<p>'.t('Lot missing.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpperm_group_add_form_submit_back'));

    return $form;
  }

  if (!isset($form_state['storage']['ajax']['Name']['LotId'])) {
    $form_state['storage']['ajax']['Name']['LotId'] = key($lot_options);
  }
  if (1 < count($lot_options)) {
    $form['ajax']['Name']['LotId'] =
      array('#type' => 'select',
	    '#title' => t('Lot'),
	    '#options' => $lot_options,
	    '#default_value' => $form_state['storage']['ajax']['Name']['LotId']);
  }

  $property_id = $form_state['storage']['ajax']['Name']['PropertyId'];
  $lot_id = $form_state['storage']['ajax']['Name']['LotId'];

  if (1 < count($property_options) or 1 < count($lot_options)) {
    $form['ajax']['Name']['addbtn'] = array('#type' => 'button', '#value' => t('Add'));
    $form['ajax']['Name']['clearbtn'] = array('#type' => 'button', '#value' => t('Clear'));
    $form['ajax']['Name']['allbtn'] = array('#type' => 'button', '#value' => t('All'));
    if (!isset($form_state['storage']['ajax']['List'])) {
      $list = array();
      $form_state['storage']['ajax']['List'] = $list;
    } else {
      $list = unserialize($form_state['storage']['ajax']['List']);
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

  $form['ajax']['List'] =
    array('#type' => 'value',
	  '#value' => serialize($list));

  $header = array(t('Property'), t('Lot'));
  $rows = array();
  foreach ($list as $property_id => $values) {
    foreach ($values as $lot_id => $value) {
      $rows[] = array($property_options[$property_id], $lot_options[$lot_id]);
    }
  }
  $form['ajax']['Name']['ListView'] =
    array('#value' => theme_table($header, $rows));

  if (1 == $form_state['storage']['ajax']['Wizard']) {
    $form['ajax']['nextfs1'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/next.png',
	    '#ahah' => array('event' => 'click',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
  }

  /**
   * Wizard field person
   */

  if (2 > $form_state['storage']['ajax']['Wizard']) {
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpperm_group_add_form_submit_back'));
    return $form;
  }

  // Layout person
  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['subtag'] = 'person';
  $settings['subframe'] = TRUE;
  $settings['subframetitle'] = t('Account owner');
  $settings['subframecollapsible'] = TRUE;
  $settings['option']['companynameopt'] = $company_id;
  $settings['option']['skipnotrequired'] = TRUE;
  $settings['option']['skipemailnotification'] = TRUE;
  $settings['option']['skiprole'] = TRUE;
  $settings['option']['skipdepartment'] = TRUE;
  $settings['option']['skiptitle'] = TRUE;
  $settings['option']['skiplogin'] = TRUE;
  $settings['option']['wizard'] = 2;
  chpprop_produce_person($form, $form_state, $settings);

  if ($form_state['storage']['ajax']['Name']['PermitCategory'] === 'Group') {
    // Layout admins
    if (!isset($form_state['storage']['ajax']['person']['addadmin'])) {
      $form_state['storage']['ajax']['person']['addadmin'] = FALSE;
    }
    $form['ajax']['person']['addadmin'] =
      array('#type' => 'checkbox',
	    '#title' => t('Add admin'),
	    '#default_value' => $form_state['storage']['ajax']['person']['addadmin'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));

    if ($form_state['storage']['ajax']['person']['addadmin']) {
      $settings = array();
      $settings['tag'] = 'ajax';
      $settings['subtag'] = 'newadmin';
      $settings['subframe'] = TRUE;
      $settings['subframetitle'] = t('Administrator');
      $settings['subframecollapsible'] = TRUE;
      $settings['option']['companynameopt'] = $company_id;
      $settings['option']['skipnotrequired'] = TRUE;
      $settings['option']['skiplogin'] = TRUE;
      $settings['option']['wizard'] = 2;
      chpprop_produce_person($form, $form_state, $settings);
    }

    if (2 == $form_state['storage']['ajax']['Wizard']) {
      $form['ajax']['nextfs2'] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/next.png',
	      '#ahah' => array('event' => 'click',
			       'path' => ahah_helper_path(array('ajax')),
			       'wrapper' => 'ajax-wrapper'));
    }
  } else {
    if (2 == $form_state['storage']['ajax']['Wizard']) {
      $form['ajax']['nextfs3'] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/next.png',
	      '#ahah' => array('event' => 'click',
			       'path' => ahah_helper_path(array('ajax')),
			       'wrapper' => 'ajax-wrapper'));
    }
  }

  /**
   * Wizard group details
   */

  if (3 > $form_state['storage']['ajax']['Wizard']) {
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpperm_group_add_form_submit_back'));
    return $form;
  }

  if ($form_state['storage']['ajax']['Name']['PermitCategory'] === 'Group') {
    $form['ajax']['details'] =
      array('#type' => 'fieldset',
	    '#title' => t('Group details'),
	    '#collapsible' => TRUE,
	    '#collapsed' => $form_state['storage']['ajax']['Wizard'] !== 3);

    // Create permit options
    $options = array();
    $options['Unlimited'] = t('Allow unlimited number of permits');
    $options['Complementary'] = t('Allow complementary permits');
    $options['Shared'] = t('Allow shared permit for multiple vehicles');
    $options['Validation'] = t('Allow parking validations');

    // Layout permit options
    if (!isset($form_state['storage']['ajax']['details']['Options'])) {
      $form_state['storage']['ajax']['details']['Options'] = array();
    }
    $form['ajax']['details']['Options'] =
      array('#type' => 'checkboxes',
	    '#title' => t('Group options'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['details']['Options'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));

    // Conditional: Unlimited
    if (!$form_state['storage']['ajax']['details']['Options']['Unlimited']) {
      if (!isset($form_state['storage']['ajax']['details']['MaxPermits'])) {
	$form_state['storage']['ajax']['details']['MaxPermits'] = 10;
      }
      $form['ajax']['details']['MaxPermits'] =
	array('#type' => 'textfield',
	      '#title' => t('Maximum number of group permits'),
	      '#maxlength' => 20,
	      '#default_value' => $form_state['storage']['ajax']['details']['MaxPermits']);
    }

    // Conditional: Complementary
    if ($form_state['storage']['ajax']['details']['Options']['Complementary']) {
      if (!isset($form_state['storage']['ajax']['details']['MaxComplementary'])) {
	$form_state['storage']['ajax']['details']['MaxComplementary'] = 1;
      }
      $form['ajax']['details']['MaxComplementary'] =
	array('#type' => 'textfield',
	      '#title' => t('Maximum number of complementary permits'),
	      '#maxlength' => 20,
	      '#default_value' => $form_state['storage']['ajax']['details']['MaxComplementary']);
    }

    // Conditional: Shared
    if ($form_state['storage']['ajax']['details']['Options']['Shared']) {
      if (!isset($form_state['storage']['ajax']['details']['MaxShared'])) {
	$form_state['storage']['ajax']['details']['MaxShared'] = 2;
      }
      $form['ajax']['details']['MaxShared'] =
	array('#type' => 'textfield',
	      '#title' => t('Maximum number of vehicles sharing same permit'),
	      '#maxlength' => 20,
	      '#default_value' => $form_state['storage']['ajax']['details']['MaxShared']);
    }

    // Conditional: Validation
    if ($form_state['storage']['ajax']['details']['Options']['Validation']) {
      if (!isset($form_state['storage']['ajax']['details']['MaxValidation'])) {
	$form_state['storage']['ajax']['details']['MaxValidation'] = 1;
      }
      $form['ajax']['details']['MaxValidation'] =
	array('#type' => 'textfield',
	      '#title' => t('Maximum number of validations'),
	      '#maxlength' => 20,
	      '#default_value' => $form_state['storage']['ajax']['details']['MaxValidation']);
    }

    if (3 == $form_state['storage']['ajax']['Wizard']) {
      $form['ajax']['nextfs3'] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/next.png',
	      '#ahah' => array('event' => 'click',
			       'path' => ahah_helper_path(array('ajax')),
			       'wrapper' => 'ajax-wrapper'));
    }
  }

  /**
   * Wizard pay details
   */

  if (4 > $form_state['storage']['ajax']['Wizard']) {
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpperm_group_add_form_submit_back'));
    return $form;
  }

  $form['ajax']['pay'] =
    array('#type' => 'fieldset',
	  '#title' => t('Payment'),
	  '#collapsible' => TRUE,
	  '#collapsed' => $form_state['storage']['ajax']['Wizard'] !== 4);

  // Create payment options
  $options = array();
  if ($form_state['storage']['ajax']['Name']['PermitCategory'] === 'Group') {
    $options['Landlord'] = chpperm_payment_arrangement_name('Landlord');
    $options['GroupPay'] = chpperm_payment_arrangement_name('GroupPay');
  }
  $options['IndividualPay'] = chpperm_payment_arrangement_name('IndividualPay');

  // Layout payment options
  if (!isset($form_state['storage']['ajax']['pay']['Payment'])) {
    $form_state['storage']['ajax']['pay']['Payment'] = key($options);
  }
  if (1 < count($options)) {
    $form['ajax']['pay']['Payment'] =
      array('#type' => 'radios',
	    '#title' => t('Payment arrangement'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['pay']['Payment']);
  } else {
    $form['ajax']['pay']['Payment'] =
      array('#type' => 'hidden',
	    '#value' => $form_state['storage']['ajax']['pay']['Payment']);
  }

  // Create payment interval options
  $options = array();
  $options['Daily'] = chpperm_interval_name('Daily');
  $options['Weekly'] = chpperm_interval_name('Weekly');
  $options['Monthly'] = chpperm_interval_name('Monthly');
  $options['Quarterly'] = chpperm_interval_name('Quarterly');
  $options['SemiAnnually'] = chpperm_interval_name('SemiAnnually');
  $options['Annually'] = chpperm_interval_name('Annually');

  if ($form_state['storage']['ajax']['Name']['PermitCategory'] === 'Group') {
    $title = t('Group payment interval');
  } else {
    $title = t('Account payment interval');
  }

  // Layout payment interval options
  if (!isset($form_state['storage']['ajax']['pay']['PayInterval'])) {
    $form_state['storage']['ajax']['pay']['PayInterval'] = 'Monthly';
  }
  $form['ajax']['pay']['PayInterval'] =
    array('#type' => 'select',
	  '#title' => $title,
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['pay']['PayInterval']);

  if (!isset($form_state['storage']['ajax']['pay']['PayAmount'])) {
    $form_state['storage']['ajax']['pay']['PayAmount'] = '';
  }
  $form['ajax']['pay']['PayAmount'] =
    array('#type' => 'textfield',
	  '#title' => t('Amount'),
	  '#maxlength' => 20,
	  '#default_value' => $form_state['storage']['ajax']['pay']['PayAmount'],
	  '#description' => t('Format: X.YY'));

  // Create billing options
  $options = array();
  $options['Invoice'] = chpperm_billing_method_name('Invoice');
  $options['CreditCard'] = chpperm_billing_method_name('CreditCard');
  $options['PAC'] = chpperm_billing_method_name('PAC');

  // Layout billing options
  if (!isset($form_state['storage']['ajax']['pay']['Billing'])) {
    $form_state['storage']['ajax']['pay']['Billing'] = key($options);
  }
  $form['ajax']['pay']['Billing'] =
    array('#type' => 'radios',
	  '#title' => t('Billing method'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['pay']['Billing']);

  if (4 == $form_state['storage']['ajax']['Wizard']) {
    $form['ajax']['nextfs4'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/next.png',
	    '#ahah' => array('event' => 'click',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
  }

  /**
   * Wizard completed
   */

  if (5 == $form_state['storage']['ajax']['Wizard']) {
    $form['ajax']['Done'] =
      array('#value' => t('You are now ready to save your new settings.').'<p>');

    $form['ajax']['save'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/save.png',
	    '#validate' => array('chpperm_group_add_form_validate_save'),
	    '#submit' => array('chpperm_group_add_form_submit_save'));
  }

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpperm_group_add_form_submit_back'));

  return $form;
}

function chpperm_group_add_form_validate_save_helper_name($form_state) {
  if ($form_state['values']['ajax']['Name']['PermitCategory'] === 'Group') {
    if (empty($form_state['values']['ajax']['Name']['Alias'])) {
      form_set_error('ajax][Name][Alias', 'Group name field is required.');
      return FALSE;
    }
  } else {
    if (empty($form_state['values']['ajax']['Name']['Alias'])) {
      form_set_error('ajax][Name][Alias', 'Account name field is required.');
      return FALSE;
    }
  }
  return TRUE;
}

function chpperm_group_add_form_validate_save_helper_person(&$form_state) {
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

  // New administrator
  if (isset($form_state['values']['ajax']['newadmin'])) {
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
    if (isset($form_state['values']['ajax']['newadmin']['Department']) and
	empty($form_state['values']['ajax']['newadmin']['Department'])) {
      form_set_error('ajax][newadmin][Department', 'Department field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax']['newadmin']['Title']) and
	empty($form_state['values']['ajax']['newadmin']['Title'])) {
      form_set_error('ajax][newadmin][Title', 'Title field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax']['newadmin']['Role']) and
	empty($form_state['values']['ajax']['newadmin']['Role'])) {
      form_set_error('ajax][newadmin][Role', 'Role field is required.');
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

  // Existing administrators
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
  return TRUE;
}

function chpperm_group_add_form_validate_save_helper_details($form_state) {
  if (!$form_state['values']['ajax']['details']['Options']['Unlimited']) {
    if (isset($form_state['values']['ajax']['details']['MaxPermits']) and
	empty($form_state['values']['ajax']['details']['MaxPermits'])) {
      form_set_error
	('ajax][details][MaxPermits',
	 'Maximum number of group permits field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax']['details']['MaxPermits']) and
	!preg_match('/^\d+$/', $form_state['values']['ajax']['details']['MaxPermits'])) {
      form_set_error
	('ajax][details][MaxPermits',
	 t('Maximum number of group permits must be a positive number'));
      return FALSE;
    }
    if (isset($form_state['values']['ajax']['details']['MaxPermits']) and
	2 > $form_state['values']['ajax']['details']['MaxPermits']) {
      form_set_error
	('ajax][details][MaxPermits',
	 t('Maximum number of group permits must be at least two to form a group'));
      return FALSE;
    }
  }

  if ($form_state['values']['ajax']['details']['Options']['Complementary']) {
    if (isset($form_state['values']['ajax']['details']['MaxComplementary']) and
	empty($form_state['values']['ajax']['details']['MaxComplementary'])) {
      form_set_error
	('ajax][details][MaxComplementary',
	 'Maximum number of complementary permits field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax']['details']['MaxComplementary']) and
	!preg_match('/^\d+$/', $form_state['values']['ajax']['details']['MaxComplementary'])) {
      form_set_error
	('ajax][details][MaxComplementary',
	 t('Maximum number of complementary permits must be a number'));
      return FALSE;
    }
    if (isset($form_state['values']['ajax']['details']['MaxComplementary']) and
	1 > $form_state['values']['ajax']['details']['MaxComplementary']) {
      form_set_error
	('ajax][details][MaxComplementary',
	 t('Maximum number of complementary permits must be at least one'));
      return FALSE;
    }
  }

  if ($form_state['values']['ajax']['details']['Options']['Shared']) {
    if (isset($form_state['values']['ajax']['details']['MaxShared']) and
	empty($form_state['values']['ajax']['details']['MaxShared'])) {
      form_set_error
	('ajax][details][MaxShared',
	 'Maximum number of vehicles sharing same permit field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax']['details']['MaxShared']) and
	!preg_match('/^\d+$/', $form_state['values']['ajax']['details']['MaxShared'])) {
      form_set_error
	('ajax][details][MaxShared',
	 t('Maximum number of vehicles sharing same permit must be a number'));
      return FALSE;
    }
    if (isset($form_state['values']['ajax']['details']['MaxShared']) and
	2 > $form_state['values']['ajax']['details']['MaxShared']) {
      form_set_error
	('ajax][details][MaxShared',
	 t('Maximum number of vehicles sharing same permit must be at least two'));
      return FALSE;
    }
  }

  if ($form_state['values']['ajax']['details']['Options']['Validation']) {
    if (isset($form_state['values']['ajax']['details']['MaxValidation']) and
	empty($form_state['values']['ajax']['details']['MaxValidation'])) {
      form_set_error
	('ajax][details][MaxValidation',
	 'Maximum number of validations field is required.');
      return FALSE;
    }
    if (isset($form_state['values']['ajax']['details']['MaxValidation']) and
	!preg_match('/^\d+$/', $form_state['values']['ajax']['details']['MaxValidation'])) {
      form_set_error
	('ajax][details][MaxValidation',
	 t('Maximum number of validations must be a number'));
      return FALSE;
    }
    if (isset($form_state['values']['ajax']['details']['MaxValidation']) and
	1 > $form_state['values']['ajax']['details']['MaxValidation']) {
      form_set_error
	('ajax][details][MaxValidation',
	 t('Maximum number of validations must be at least one'));
      return FALSE;
    }
  }

  return TRUE;
}

function chpperm_group_add_form_validate_save_helper_pay($form_state) {
  if (isset($form_state['values']['ajax']['pay']['PayAmount']) and
      empty($form_state['values']['ajax']['pay']['PayAmount'])) {
    form_set_error('ajax][pay][PayAmount', t('Amount field is required.'));
    return FALSE;
  }

  if (isset($form_state['values']['ajax']['pay']['PayAmount']) and
      !preg_match('/^\d+\.\d{2}$/', $form_state['values']['ajax']['pay']['PayAmount'])) {
    form_set_error
      ('ajax][pay][PayAmount',
       t('Amount field must be in format X.YY (two decimals).'));
    return FALSE;
  }

  return TRUE;
}

function chpperm_group_add_form_validate_save($form, &$form_state) {
  if (!chpperm_group_add_form_validate_save_helper_name($form_state)) {
    return;
  }
  if (!chpperm_group_add_form_validate_save_helper_person($form_state)) {
    return;
  }
  if (!chpperm_group_add_form_validate_save_helper_details($form_state)) {
    return;
  }
  if (!chpperm_group_add_form_validate_save_helper_pay($form_state)) {
    return;
  }
}

function chpperm_group_add_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermgrp';
}

function chpperm_group_add_form_submit_save($form, &$form_state) {
  $company_id = $form_state['values']['ajax']['CompanyId'];
  $client_id = $form_state['values']['ajax']['ClientId'];
  $property_id = $form_state['storage']['ajax']['PropertyId'];
  $lot_id = $form_state['storage']['ajax']['LotId'];
  $all_lots = $form_state['storage']['ajax']['all_lots'];

  $alias = check_plain($form_state['values']['ajax']['Name']['Alias']);

  if (isset($form_state['values']['ajax']['person']['PrenameId'])) {
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

  if (isset($form_state['values']['ajax']['person']['SufnameId'])) {
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

  $payment = $form_state['values']['ajax']['pay']['Payment'];
  $pay_interval = $form_state['values']['ajax']['pay']['PayInterval'];
  $pay_amount = $form_state['values']['ajax']['pay']['PayAmount'];
  $billing = $form_state['values']['ajax']['pay']['Billing'];

  if ($form_state['values']['ajax']['Name']['PermitCategory'] === 'Group') {
    if ($form_state['values']['ajax']['details']['Options']['Unlimited']) {
      $max_entries = -1;
    } else {
      $max_entries = $form_state['values']['ajax']['details']['MaxPermits'];
    }
  } else {
    $max_entries = 0;
  }
  
  if ($form_state['values']['ajax']['Name']['PermitCategory'] === 'Group' and
      $form_state['values']['ajax']['details']['Options']['Complementary']) {
    $complementary = $form_state['values']['ajax']['details']['MaxComplementary'];
  } else {
    $complementary = 0;
  }

  if ($form_state['values']['ajax']['Name']['PermitCategory'] === 'Group') {
    if ($form_state['values']['ajax']['details']['Options']['Shared']) {
      $shared = $form_state['values']['ajax']['details']['MaxShared'];
    } else {
      $shared = 1;
    }
  } else {
    $shared = 0;
  }

  if ($form_state['values']['ajax']['Name']['PermitCategory'] === 'Group' and
      $form_state['values']['ajax']['details']['Options']['Validation']) {
    $validation = $form_state['values']['ajax']['details']['MaxValidation'];
  } else {
    $validation = 0;
  }

  if ($form_state['values']['ajax']['Name']['PermitCategory'] === 'Group') {
    $single = FALSE;
  } else {
    $single = TRUE;
  }

  $group_id = chpprop_genRandomString20();

  global $user;
  if (chdbperm_insert_groupdef
      ($user->uid, $company_id, $client_id, $group_id,
       $single, $alias, $lat, $lon,
       $prename_id, $firstname, $middlename, $lastname, $sufname_id,
       $street, $city, $postalcode, $province, $country,
       $phone, $email,
       $max_entries, $complementary, $shared, $validation,
       $payment, $pay_interval, $pay_amount, $billing,
       $id)) {
    drupal_set_message('Could not insert new group', 'error');
    return;
  }

  if ($form_state['values']['ajax']['Name']['PermitCategory'] === 'Group') {
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

      if (isset($form_state['values']['ajax']['newadmin']['Role'])) {
	$role = check_plain($form_state['values']['ajax']['newadmin']['Role']);
      } else {
	$role = NULL;
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

      if (chdbperm_insert_groupadmin
	  ($company_id, $client_id, $group_id, $admin_id, $lat, $lon,
	   $prename_id, $firstname, $middlename, $lastname, $sufname_id,
	   $street, $city, $postalcode, $province, $country,
	   $phone, $email, $emailnotification,
	   $role, $department, $title,
	   $id)) {
	drupal_set_message('Could not insert group admin', 'error');
	return;
      }
    }
  }

  $list = unserialize($form_state['values']['ajax']['List']);
  if (!empty($list)) {
    foreach ($list as $propkey => $values) {
      foreach ($values as $lotkey => $value) {
	if ($value) {
	  if (chdbperm_insert_groupprop
	      ($company_id, $client_id, $group_id,
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
  $form_state['redirect'] = 'chppermgrp';
}



/**********
 ********** Group list
 ********** Address: chppermgrp
 ********** Access:  'anai chp list permit group'
 *********/



function chpperm_group_form($form_state) {
  if ($form_state['storage']['detailed_view']) {
    return chpperm_group_view_form($form_state);
  }
  if ($form_state['storage']['detailed_notes']) {
    return chpperm_group_notes_form($form_state);
  }
  if ($form_state['storage']['detailed_edit']) {
    return chpperm_group_edit_form($form_state);
  }
  if ($form_state['storage']['detailed_money']) {
    return chpperm_group_money_form($form_state);
  }

  drupal_set_title(t('Permit groups'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['show_companies'] = TRUE;
  $settings['show_regions'] = TRUE; // !!
  $settings['show_clients'] = TRUE; // !!
  //$settings['display_single_company'] = TRUE; // !!
  //$settings['display_single_region'] = TRUE; // !!
  //$settings['display_single_client'] = TRUE; // !!
  $settings['option']['skippropertynotes'] = TRUE;
  $settings['buttons']['back']['title'] = t('Back');
  $settings['buttons']['back']['submit'] = 'chpperm_group_form_submit_back';
  if (user_access('anai chp add permit group')) {
    $settings['buttons']['add']['title'] = t('Add');
    $settings['buttons']['add']['submit'] = 'chpperm_group_form_submit_add';
  }
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $region_id = $form_state['storage']['ajax']['RegionId'];
  $client_id = $form_state['storage']['ajax']['ClientId'];

  /*
   * Note 1: Region selector is mainly just there to narrow down client selector.
   * Note 2: Property options are built from groupprop_table instead of regular method.
   */

  $props = chpperm_retrieve_group_props($user->uid, $company_id, $client_id);
  //anai_dbg($props, '$props');

  $assigned_properties = array();
  if (!empty($props)) {
    foreach ($props as $property_id => $values) {
      $property = $form_state['storage']['ajax']['Properties'][$property_id]['data'];
      $assigned_properties[$property_id] =
	decode_entities($property['Alias']).', '.
	decode_entities($property['Street']).', '.
	decode_entities($property['City']);
    }
  }

  if (!empty($assigned_properties)) {
    if (!isset($form_state['storage']['ajax']['PropertyId'])) {
      $form_state['storage']['ajax']['PropertyId'] = key($assigned_properties);
    }
    $form['ajax']['PropertyId'] =
      array('#type' => 'select',
	    '#options' => $assigned_properties,
	    '#title' => t('Property'),
	    '#default_value' => $form_state['storage']['ajax']['PropertyId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
    $property_id = $form_state['storage']['ajax']['PropertyId'];
  }

  $assigned_lots = array();
  if (!empty($assigned_properties)) {
    foreach ($props[$property_id] as $lot_id => $values) {
      if ($lot_id === 'ANY') {
	$assigned_lots['ANY'] = t('Any lot');
      } else {
	$lot = $form_state['storage']['ajax']['Lots'][$property_id][$lot_id];
	$assigned_lots[$lot_id] = decode_entities($lot['data']['Alias']);
      }
    }
    if (!isset($form_state['storage']['ajax']['LotId'])) {
      $form_state['storage']['ajax']['LotId'] = key($assigned_lots);
    }
    $form['ajax']['LotId'] =
      array('#type' => 'select',
	    '#options' => $assigned_lots,
	    '#title' => t('Lot'),
	    '#default_value' => $form_state['storage']['ajax']['LotId']);
    $lot_id = $form_state['storage']['ajax']['LotId'];
  }

  // Operational groups
  $groups = array();
  if (!empty($assigned_properties)) {
    $groups = chpperm_retrieve_groups($user->uid, $company_id, $property_id);
    //anai_dbg($groups, '$groups');
    if (isset($groups[$client_id])) {
      $groups = $groups[$client_id];
    } else {
      $groups = array();
    }

  }
  $form['ajax']['filter'] =
    array('#type' => 'fieldset',
	  '#title' => t('Operational groups'));
  if (empty($groups)) {
    $form['ajax']['filter']['empty'] = array('#value' => '<p>'.t('Empty.'));
  } else {
    $form['ajax']['filter']['list'] = chpperm_group_table_form($groups);
  }


  // List those not assigned to property
  $allgroups = chpperm_retrieve_groups($user->uid, $company_id);
  //anai_dbg($allgroups, '$allgroups');

  if (isset($allgroups[$client_id])) {
    $allgroups = $allgroups[$client_id];
  } else {
    $allgroups = array();
  }
  if (!empty($allgroups)) {
    $leftovers = array();
    foreach ($allgroups as $group_id => $group) {
      if (isset($groups[$group_id])) {
	continue;
      }
      $leftovers[$group_id] = $group;
    }
  }
  if (!empty($leftovers)) {
    $form['ajax']['all'] =
      array('#type' => 'fieldset',
	    '#title' => t('Available groups not assigned to property'));
    $form['ajax']['all']['list'] = chpperm_group_table_form($leftovers);
  }

  // Suspended table
  $groups = chpperm_retrieve_groups($user->uid, $company_id, 'ANY',
				    TRUE, TRUE, // Active
				    TRUE, TRUE); // Suspended
  if (isset($groups[$client_id])) {
    $groups = $groups[$client_id];
  } else {
    $groups = array();
  }
  if (!empty($groups)) {
    $form['ajax']['suspended'] =
      array('#type' => 'fieldset',
	    '#title' => t('Suspended groups'));
    $form['ajax']['suspended']['list'] = chpperm_group_table_form($groups);
  }

  // Retention table
  if (!isset($form_state['storage']['ajax']['Retention'])) {
    $form_state['storage']['ajax']['Retention'] = FALSE;
  }
  $form['ajax']['Retention'] =
    array('#type' => 'checkbox',
	  '#title' => t('Show deleted groups'),
	  '#default_value' => $form_state['storage']['ajax']['Retention'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));
  if ($form_state['storage']['ajax']['Retention']) {
    $groups = chpperm_retrieve_groups($user->uid, $company_id, 'ANY',
				      TRUE, FALSE, // Active
				      FALSE, FALSE); // Suspended
    if (isset($groups[$client_id])) {
      $groups = $groups[$client_id];
    } else {
      $groups = array();
    }
    $form['ajax']['retention'] =
      array('#type' => 'fieldset',
	    '#title' => t('Deleted groups'));
    if (empty($groups)) {
      $form['ajax']['retention']['empty'] = array('#value' => '<p>'.t('Empty.'));
    } else {
      $form['ajax']['retention']['list'] = chpperm_group_table_form($groups);
    }
  }

  return $form;
}

function chpperm_group_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'front_page';
}

function chpperm_group_form_submit_add($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermgrpadd';
}

function chpperm_group_table_form($entries) {
  $form['db'] =
    array('#type' => 'markup', '#value' => '',
	  '#theme' => 'chpperm_group_table');

  if (user_access('anai chp edit permit group')) {
    $readonly_edit = FALSE;
  } else {
    $readonly_edit = TRUE;
  }

  if (user_access('anai chp list permit payments')) {
    $readonly_payment = FALSE;
  } else {
    $readonly_payment = TRUE;
  }

  $first = current($entries);
  $prenames = array();
  foreach (chdbadmin_get_prenameopt($first['data']['CompanyId']) as $opt) {
    $prenames[$opt['PrenameId']] = decode_entities($opt['Alias']);
  }
  if (empty($prenames)) {
    foreach (chdbadmin_get_prenameopt() as $opt) {
      $prenames[$opt['PrenameId']] = decode_entities($opt['Alias']);
    }
  }
  $sufnames = array();
  foreach (chdbadmin_get_sufnameopt($first['data']['CompanyId']) as $opt) {
    $sufnames[$opt['SufnameId']] = decode_entities($opt['Alias']);
  }
  if (empty($sufnames)) {
    foreach (chdbadmin_get_sufnameopt() as $opt) {
      $sufnames[$opt['SufnameId']] = decode_entities($opt['Alias']);
    }
  }

  $looper = 0;
  foreach ($entries as $group_id => $entry) {
    $form['db']['alias_'.$looper] =
      array('#value' => decode_entities($entry['data']['Alias']));

    $name = isset($prenames[$entry['data']['PrenameId']]) ? $prenames[$entry['data']['PrenameId']].' ' : '';
    $name .= !empty($entry['data']['FirstName']) ? decode_entities($entry['data']['FirstName']).' ' : '';
    $name .= !empty($entry['data']['MiddleName']) ? decode_entities($entry['data']['MiddleName']).' ' : '';
    $name .= !empty($entry['data']['LastName']) ? decode_entities($entry['data']['LastName']).' ' : '';
    $name .= isset($sufnames[$entry['data']['SufnameId']]) ? $sufnames[$entry['data']['SufnameId']].' ' : '';

    $form['db']['name_'.$looper] = array('#value' => $name);
    $form['db']['category_'.$looper] =
      array('#value' => $entry['data']['Single'] ? t('Single') : t('Group'));

    $form['db']['phone_'.$looper] =
      array('#value' => decode_entities($entry['data']['Phone']));

    $form['db']['suspended_'.$looper] =
      array('#value' => $entry['data']['Suspended'] ? t('Yes') : t('No'));

    $form['db']['notes_'.$looper] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/notesmall.png',
	    '#name' => 'op_notes'.$entry['data']['Id'],
	    '#submit' => array('chpperm_group_form_submit'));

    $form['db']['view_'.$looper] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/viewsmall.png',
	    '#name' => 'op_view'.$entry['data']['Id'],
	    '#submit' => array('chpperm_group_form_submit'));

    if ($readonly_payment) {
      $form['db']['list_'.$looper] = array('#value' => '');
    } else {
      $form['db']['list_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/moneysmall.png',
	      '#name' => 'op_money'.$entry['data']['Id'],
	      '#submit' => array('chpperm_group_form_submit'));
    }

    if ($readonly_edit) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_edit'.$entry['data']['Id'],
	      '#submit' => array('chpperm_group_form_submit'));
    }

    $looper++;
  }
  return $form;
}

function theme_chpperm_group_table($form) {
  $header = array(t('Name'), t('Category'), t('Account owner'), t('Phone'),
		  t('Suspended'), '', '', '', '');
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'alias_')) {
      continue;
    }
    $looper = substr($key, strlen('alias_'));
    $rows[] = array(drupal_render($form['alias_'.$looper]),
		    drupal_render($form['category_'.$looper]),
		    drupal_render($form['name_'.$looper]),
		    drupal_render($form['phone_'.$looper]),
		    array('data' => drupal_render($form['suspended_'.$looper]),
			  'width' => 1, 'height' => 1),
		    array('data' => drupal_render($form['notes_'.$looper]),
		    	  'width' => 1, 'height' => 1),
		    array('data' => drupal_render($form['view_'.$looper]),
		    	  'width' => 1, 'height' => 1),
		    array('data' => drupal_render($form['list_'.$looper]),
		    	  'width' => 1, 'height' => 1),
		    array('data' => drupal_render($form['edit_'.$looper]),
			  'width' => 1, 'height' => 1),
                    );
  }
  return theme_table($header, $rows);
}

/****************************************************************************/

function chpperm_group_view_form($form_state) {
  global $user;
  $id = $form_state['storage']['id'];
  $group = chdbperm_get_groupdef_id($id);
  drupal_set_title(t('Permit group - GRP',
		     array('GRP' => decode_entities($group['Alias']))));

  $form['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/back.png',
	  '#submit' => array('chpperm_group_view_form_submit_back'));

  $prenames = array();
  foreach (chdbadmin_get_prenameopt($group['CompanyId']) as $opt) {
    $prenames[$opt['PrenameId']] = decode_entities($opt['Alias']);
  }
  if (empty($prenames)) {
    foreach (chdbadmin_get_prenameopt() as $opt) {
      $prenames[$opt['PrenameId']] = decode_entities($opt['Alias']);
    }
  }
  $sufnames = array();
  foreach (chdbadmin_get_sufnameopt($group['CompanyId']) as $opt) {
    $sufnames[$opt['SufnameId']] = decode_entities($opt['Alias']);
  }
  if (empty($sufnames)) {
    foreach (chdbadmin_get_sufnameopt() as $opt) {
      $sufnames[$opt['SufnameId']] = decode_entities($opt['Alias']);
    }
  }

  $header = array();
  $row = array();
  if (isset($prenames[$group['PrenameId']])) {
    $row[] = array(t('Prefix:'), $prenames[$group['PrenameId']]);
  }
  if (!empty($group['FirstName'])) {
    $row[] = array(t('First name:'), decode_entities($group['FirstName']));
  }
  if (!empty($group['LastName'])) {
    $row[] = array(t('Last name:'), decode_entities($group['LastName']));
  }
  if (!empty($group['MiddleName'])) {
    $row[] = array(t('Middle name:'), decode_entities($group['MiddleName']));
  }
  if (isset($sufnames[$group['SufnameId']])) {
    $row[] = array(t('Suffix:'), $sufnames[$group['SufnameId']]);
  }
  if (!empty($group['Phone'])) {
    $row[] = array(t('Phone:'), decode_entities($group['Phone']));
  }
  if (!empty($group['Email'])) {
    $row[] = array(t('Email:'), decode_entities($group['Email']));
  }

  if (!$group['Single']) {
    $row[] = array(t('Permit limit:'),
		   (0 > $group['MaxEntries'] ? t('Unlimited') : $group['MaxEntries']));
    $row[] = array(t('Complementary limit:'), $group['Complementary']);
    $row[] = array(t('Shared permit limit:'), $group['Shared']);
    $row[] = array(t('Validation limit:'), $group['Validation']);
    $row[] = array(t('Payment arrangement:'), chpperm_payment_arrangement_name($group['Payment']));
  }

  $row[] = array(t('Payment interval:'), chpperm_interval_name($group['PayInterval']));
  $row[] = array(t('Payment amount:'), sprintf("%0.02f", $group['PayAmount']));
  $row[] = array(t('Billing method:'), chpperm_billing_method_name($group['Billing']));

  $properties = array();
  $props = chpperm_retrieve_group_props($user->uid, $group['CompanyId'], $group['ClientId']);

  $row[] = array(t('Properties:'), count($props));

  // Retrieve permits belonging to groups
  if ($group['Single']) {
    $permit = chdbperm_get_group_permits($group['CompanyId'], NULL, $group['GroupId']);
  } else {
    $permits = chdbperm_get_group_permits($group['CompanyId'], $group['GroupId'], NULL);
  }

  // Prepare and count permits in group
  if (!$group['Single']) {
    // Sort shared permits
    $shared = array();
    foreach ($permits as $permit) {
      if (empty($permit['SharedId'])) {
	continue;
      }
      $shared[$permit['SharedId']][$permit['LPN']] = $permit;
    }

    // Sort list with shared permits listed together
    $list = array();
    foreach ($permits as $permit) {
      if (empty($permit['SharedId'])) {
	$list[$permit['LPN']] = $permit;
      } else {
	foreach ($shared[$permit['SharedId']] as $lpn => $share) {
	  $list[$lpn] = $share;
	}
      }
    }

    // Create shared mapping
    $shared = array();
    $included = 0;
    foreach ($permits as $permit) {
      if (empty($permit['SharedId'])) {
	continue;
      }
      if (!isset($shared[$permit['SharedId']])) {
	$shared[$permit['SharedId']] = 1;
      } else {
	$shared[$permit['SharedId']]++;
      }
      $included++;
    }

    $row[] = array(t('Permits:'), count($list) - $included + count($shared));
  }

  $form['t1'] =
    array('#prefix' => '<table border="0"><tr valign="top"><td align="left" width="60%">',
	  '#value' => theme('table', $header, $row),
	  '#suffix' => '</td>');

  $form['t2'] =
    array('#prefix' => '<td>',
	  '#value' => chpperm_group_view_map($group, $properties),
	  '#suffix' => '</td></tr></table>');

  // View admins for group
  if (!$group['Single']) {
    $admins = chdbperm_get_groupadmins($group['CompanyId'], $group['ClientId'], $group['GroupId']);
    $form['t3'] = array('#type' => 'fieldset', '#title' => t('Administrators'));
    $form['t3']['list'] = array('#value' => chpprop_name_view_table($admins, $group['CompanyId']));
  }

  // Notes
  $header = array(t('Created'), t('By'), t('Id'), t('Note'));
  $row = array();
  $notes = array();
  foreach (chdbperm_get_groupnotes($group['CompanyId'],
				   $group['ClientId'],
				   $group['GroupId']) as $note) {
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

  $form['t4'] =
    array('#type' => 'fieldset',
	  '#title' => t('Notes'));
  if (empty($notes)) {
    $form['t4']['empty'] = array('#value' => t('Empty.'));
  } else {
    $form['t4']['list'] = array('#value' => theme('table', $header, $row));
  }

  // Permits
  $header = array('',
		  t('LPN'), t('Make'), t('Model'), t('Color'),
		  t('Person'), t('Phone'),
		  t('Class'), t('Active'));
  $row = array();

  if ($group['Single']) {
    $span = 1;
    $icon = theme('image', drupal_get_path('module', 'anai').'/permitsmall.png');
    $pre = array('data' => $icon, 'width' => 1, 'height' => 1, 'rowspan' => $span);
    $line = array(decode_entities($permit['LPN']),
		  decode_entities($permit['Make']),
		  decode_entities($permit['Model']),
		  decode_entities($permit['Color']),
		  decode_entities($permit['FirstName']).' '.decode_entities($permit['LastName']),
		  decode_entities($permit['Phone']),
		  $permit['PermitClass'],
		  ($permit['Active'] and !$permit['Suspended']) ? t('Yes') : t('No'));
    array_unshift($line, $pre);
    $form['t5'] = array('#type' => 'fieldset', '#title' => t('Permit'));
  } else {
    $shared_id = '';
    foreach ($list as $permit) {
      if (empty($permit['SharedId'])) {
	$span = 1;
	$icon = theme('image', drupal_get_path('module', 'anai').'/permitsmall.png');
	$pre = array('data' => $icon, 'width' => 1, 'height' => 1, 'rowspan' => $span);
      } else {
	$span = $shared[$permit['SharedId']];
	$icon = 1 < $span ?
	  theme('image', drupal_get_path('module', 'anai').'/permitssmall.png') :
	  theme('image', drupal_get_path('module', 'anai').'/permitsmall.png');
	$pre = array('data' => $icon, 'width' => 1, 'height' => 1, 'rowspan' => $span);
      }

      $line = array(decode_entities($permit['LPN']),
		    decode_entities($permit['Make']),
		    decode_entities($permit['Model']),
		    decode_entities($permit['Color']),
		    decode_entities($permit['FirstName']).' '.decode_entities($permit['LastName']),
		    decode_entities($permit['Phone']),
		    $permit['PermitClass'],
		    ($permit['Active'] and !$permit['Suspended']) ? t('Yes') : t('No'));

      if (empty($permit['SharedId'])) {
	array_unshift($line, $pre);
	$shared_id = '';
      } elseif ($permit['SharedId'] !== $shared_id) {
	array_unshift($line, $pre);
	$shared_id = $permit['SharedId'];
      }
      $row[] = $line;
    }
    $form['t5'] = array('#type' => 'fieldset', '#title' => t('Permits'));
  }
  if (empty($row)) {
    $form['t5']['empty'] = array('#value' => t('Empty.'));
  } else {
    $form['t5']['list'] = array('#value' => theme('table', $header, $row));
  }

  return $form;
}

function chpperm_group_view_map($group, $properties) {
  $map_array['id'] = 'LocationMap';
  $map_array['width'] = '360px';
  $map_array['height'] = '300px';
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
  $address = decode_entities($group['Street']);
  $address .= '<br>'.decode_entities($group['City']);
  $address .= ', '.$group['Province'];
  $address .= '<br>'.decode_entities($group['PostalCode']);
  $address .= ', '.$group['Country'];
  $markers[] = array
    ('text' => '<b>'.t('Permit group address:').'</b><br>'.$address,
     'latitude' => sprintf("%f", $group['Lat']),
     'longitude' => sprintf("%f", $group['Lon']),
     'markername' => 'blue');
  foreach ($properties as $property) {
    $address = decode_entities($property['Street']);
    $address .= '<br>'.decode_entities($property['City']);
    $address .= ', '.$property['Province'];
    $address .= '<br>'.decode_entities($property['PostalCode']);
    $address .= ', '.$property['Country'];
    $markers[] = array
      ('text' => '<b>'.decode_entities($property['Alias']).'</b><br>'.$address,
       'latitude' => sprintf("%f", $property['Lat']),
       'longitude' => sprintf("%f", $property['Lon']),
       'markername' => 'green');
  }
  $map_array['markers'] = $markers;
  return theme('gmap', array('#settings' => $map_array));
}

function chpperm_group_view_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermgrp';
}

/****************************************************************************/

function chpperm_group_notes_form($form_state) {
  if (isset($form_state['storage']['detailed_notes_add'])) {
    return chpperm_group_notes_add_form($form_state);
  }
  if (isset($form_state['storage']['detailed_notes_edit'])) {
    return chpperm_group_notes_edit_form($form_state);
  }
  if (isset($form_state['storage']['detailed_notes_solve'])) {
    return chpperm_group_notes_solve_form($form_state);
  }

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $id = $form_state['storage']['id'];
  $groupdef = chdbperm_get_groupdef_id($id);
  drupal_set_title(t('Group notes - NAME', array('NAME' => decode_entities($groupdef['Alias']))));

  $form['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/back.png',
	  '#submit' => array('chpperm_group_notes_form_submit_back'));

  if (user_access('anai chp add property notes')) {
    $form['add'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/add.png',
	    '#submit' => array('chpperm_group_notes_form_validate_add'));
  }

  $notes = array();
  foreach (chdbperm_get_groupnotes($groupdef['CompanyId'],
				   $groupdef['ClientId'],
				   $groupdef['GroupId']) as $note) {
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
    $form['list'] = chpperm_group_notes_table_form($notes);
  }

  return $form;
}

function chpperm_group_notes_form_validate_add($form, &$form_state) {
  $form_state['storage']['detailed_notes_add'] = TRUE;
}

function chpperm_group_notes_form_submit_back($form, &$form_state) {
  unset($form_state['storage']['detailed_notes']);
}

function chpperm_group_notes_table_form($entries) {
  global $user;
  $form['db'] =
    array('#type' => 'markup', '#theme' => 'chpperm_group_notes_table');
  if (user_access('anai chp edit permit notes')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }
  if (user_access('anai chp solve permit notes')) {
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
	      '#submit' => array('chpperm_group_form_submit'));
    }
    if ($solveable) {
      $form['db']['solve_'.$looper] = array('#value' => '');
    } else {
      $form['db']['solve_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/finalsmall.png',
	      '#name' => 'op_solvenote'.$entry['master']['Id'],
	      '#submit' => array('chpperm_group_form_submit'));
    }
    $looper++;
  }
  return $form;
}

function theme_chpperm_group_notes_table($form) {
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

function chpperm_group_notes_add_form($form_state) {
  $form = array();
  drupal_set_title(t('Add note'));

  $form['Note'] =
    array('#type' => 'textarea',
	  '#title' => t('Description'),
	  '#maxlength' => 2048);

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpperm_group_notes_add_form_validate_save'),
	  '#submit' => array('chpperm_group_notes_add_form_submit_save'));

  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpperm_group_notes_add_form_submit_back'));
  return $form;
}

function chpperm_group_notes_add_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['Note'])) {
    form_set_error('Note', t('Description field is required.'));
    return;
  }
}

function chpperm_group_notes_add_form_submit_back($form, &$form_state) {
  unset($form_state['storage']['detailed_notes_add']);
}

function chpperm_group_notes_add_form_submit_save($form, &$form_state) {
  global $user;
  $id = $form_state['storage']['id'];
  $groupdef = chdbperm_get_groupdef_id($id);

  $idd = chdbperm_get_groupnoteid($groupdef['CompanyId'],
				  $groupdef['ClientId'],
				  $groupdef['GroupId']);
  if (empty($idd)) {
    if (chdbperm_insert_groupnoteid($groupdef['CompanyId'],
				    $groupdef['ClientId'],
				    $groupdef['GroupId'], $tmp)) {
      drupal_set_message('Could not insert noteid', 'error');
      return;
    }
    $idd = chdbperm_get_groupnoteid($groupdef['CompanyId'],
				    $groupdef['ClientId'],
				    $groupdef['GroupId']);
  }
  $note_id = $idd['Counter'];

  if (chdbperm_insert_groupnote
      ($user->uid,
       $groupdef['CompanyId'],
       $groupdef['ClientId'],
       $groupdef['GroupId'],
       $note_id,
       check_plain($form_state['values']['Note']),
       $id)) {
    drupal_set_message('Could not insert note', 'error');
    return;
  }

  if (chdbperm_update_groupnoteid_id($idd['Id'])) {
    drupal_set_message('Could not update propertynoteid', 'error');
    return;
  }

  unset($form_state['storage']['detailed_notes_add']);
}

function chpperm_group_notes_edit_form($form_state) {
  global $user;
  $form = array();

  $note_id = $form_state['storage']['note_id'];
  $note = chdbperm_get_groupnote_id($note_id);
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

  $fragments = chdbperm_get_groupnote($note['CompanyId'],
				      $note['ClientId'],
				      $note['GroupId'],
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
	  '#validate' => array('chpperm_group_notes_add_form_validate_save'),
	  '#submit' => array('chpperm_group_notes_edit_form_submit_save'));

  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpperm_group_notes_edit_form_submit_back'));
  return $form;
}

function chpperm_group_notes_edit_form_submit_back($form, &$form_state) {
  unset($form_state['storage']['detailed_notes_edit']);
}

function chpperm_group_notes_edit_form_submit_save($form, &$form_state) {
  global $user;
  $note_id = $form_state['storage']['note_id'];

  if (chdbperm_update_groupnote
      ($note_id,
       $user->uid,
       check_plain($form_state['values']['Note']))) {
    drupal_set_message('Could not update note', 'error');
    return;
  }

  unset($form_state['storage']['detailed_notes_edit']);
}

function chpperm_group_notes_solve_form($form_state) {
  global $user;
  $form = array();

  $note_id = $form_state['storage']['note_id'];
  $note = chdbperm_get_groupnote_id($note_id);
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

  $fragments = chdbperm_get_groupnote($note['CompanyId'],
				      $note['ClientId'],
				      $note['GroupId'],
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
	  '#validate' => array('chpperm_group_notes_add_form_validate_save'),
	  '#submit' => array('chpperm_group_notes_solve_form_submit_save'));

  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpperm_group_notes_solve_form_submit_back'));
  return $form;
}

function chpperm_group_notes_solve_form_submit_back($form, &$form_state) {
  unset($form_state['storage']['detailed_notes_solve']);
}

function chpperm_group_notes_solve_form_submit_save($form, &$form_state) {
  global $user;
  $note_id = $form_state['storage']['note_id'];

  if (chdbperm_groupnote_solved
      ($note_id,
       $user->uid,
       check_plain($form_state['values']['Note']))) {
    drupal_set_message('Could not solve note', 'error');
    return;
  }

  unset($form_state['storage']['detailed_notes_solve']);
}

/****************************************************************************/

function chpperm_group_edit_form($form_state) {
  if ($form_state['storage']['confirm_delete_group']) {
    return confirm_form($form, 'Are you sure?', 'chppermgrp',
			'The group will be deleted from the system.');
  }

  global $user;
  $form = array();
  drupal_set_title(t('Edit permit group'));
  $id = $form_state['storage']['id'];
  $group = chdbperm_get_groupdef_id($id);
  $company_id = $group['CompanyId'];
  $client_id = $group['ClientId'];
  $group_id = $group['GroupId'];

  // Create property options
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

  if (empty($property_options)) {
    $form['ajax']['error'] =
      array('#value' => '<p>'.t('Property missing.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpperm_group_edit_form_submit_back'));

    return $form;
  }

  ahah_helper_register($form, $form_state);

  if (!$group['Active']) {
    $form['Activate'] =
      array('#type' => 'checkbox',
	    '#title' => t('Activate'),
	    '#default_value' => $group['Active']);
  }

  $form['Suspended'] =
    array('#type' => 'checkbox',
	  '#title' => t('Suspended'),
	  '#default_value' => $group['Suspended']);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  $form['ajax']['Name'] =
    array('#type' => 'fieldset',
	  '#prefix' => '<div id="ajax-name-wrapper">',
	  '#suffix' => '</div>',
	  '#title' => t('Name and assigned proprties'),
	  '#collapsible' => TRUE);

  if (!isset($form_state['storage']['ajax']['Name']['framecollapse'])) {
    $form_state['storage']['ajax']['Name']['framecollapse'] = TRUE;
  }
  $form['ajax']['Name']['framecollapse'] =
    array('#type' => 'checkbox',
	  '#title' => t('Collapsed frame'),
	  '#default_value' => $form_state['storage']['ajax']['Name']['framecollapse'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));
  $form['ajax']['Name']['#collapsed'] = $form_state['storage']['ajax']['Name']['framecollapse'];

  // Create permit options
  $options = array();
  $options['Group'] = t('Permit group account for multiple vehicles');
  $options['Member'] = t('Permit account for single vehicle');

  // Layout permit options
  if (!isset($form_state['storage']['ajax']['Name']['PermitCategory'])) {
    if ($group['Single']) {
      $form_state['storage']['ajax']['Name']['PermitCategory'] = 'Member';
    } else {
      $form_state['storage']['ajax']['Name']['PermitCategory'] = 'Group';
    }
  }
  $form['ajax']['Name']['PermitCategory'] =
    array('#type' => 'radios',
	  '#title' => t('Category'),
	  '#options' => $options,
	  '#disabled' => TRUE,
	  '#default_value' => $form_state['storage']['ajax']['Name']['PermitCategory'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  if ($form_state['storage']['ajax']['Name']['PermitCategory'] === 'Group') {
    $title = t('Group name');
  } else {
    $title = t('Account name');
  }

  if (!isset($form_state['storage']['ajax']['Name']['Alias'])) {
    $form_state['storage']['ajax']['Name']['Alias'] = decode_entities($group['Alias']);
  }
  $form['ajax']['Name']['Alias'] =
    array('#type' => 'textfield',
	  '#title' => $title,
	  '#maxlength' => 80,
	  '#default_value' => $form_state['storage']['ajax']['Name']['Alias']);

  if (!isset($form_state['storage']['ajax']['Name']['PropertyId'])) {
    $form_state['storage']['ajax']['Name']['PropertyId'] = key($property_options);
  }
  if (1 < count($property_options)) {
    $form['ajax']['Name']['PropertyId'] =
      array('#type' => 'select',
	    '#title' => t('Property'),
	    '#options' => $property_options,
	    '#default_value' => $form_state['storage']['ajax']['Name']['PropertyId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax', 'Name')),
			     'wrapper' => 'ajax-name-wrapper'));
  }

  // Create lot options
  $lot_options = array();
  $policies = chpprop_retrieve_policies($user->uid, $company_id);
  if (!empty($policies)) {
    if (isset($policies[$form_state['storage']['ajax']['Name']['PropertyId']])) {
      foreach ($policies[$form_state['storage']['ajax']['Name']['PropertyId']] as $lot_id => $policy) {
	$lot_options[$lot_id] = decode_entities($policy['data']['Alias']);
      }
    }
  }

  if (empty($lot_options)) {
    $form['ajax']['error'] =
      array('#value' => '<p>'.t('Lot missing.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpperm_group_edit_form_submit_back'));

    return $form;
  }

  if (!isset($form_state['storage']['ajax']['Name']['LotId'])) {
    $form_state['storage']['ajax']['Name']['LotId'] = key($lot_options);
  }
  if (1 < count($lot_options)) {
    $form['ajax']['Name']['LotId'] =
      array('#type' => 'select',
	    '#title' => t('Lot'),
	    '#options' => $lot_options,
	    '#default_value' => $form_state['storage']['ajax']['Name']['LotId']);
  }

  if (!empty($property_options) and !empty($lot_options)) {
    $property_id = $form_state['storage']['ajax']['Name']['PropertyId'];
    $lot_id = $form_state['storage']['ajax']['Name']['LotId'];

    $form['ajax']['Name']['addbtn'] = array('#type' => 'button', '#value' => t('Add'));
    $form['ajax']['Name']['clearbtn'] = array('#type' => 'button', '#value' => t('Clear'));
    $form['ajax']['Name']['allbtn'] = array('#type' => 'button', '#value' => t('All'));
    if (!isset($form_state['storage']['ajax']['List'])) {
      $list = array();
      $props = chpperm_retrieve_group_props($user->uid, $company_id, $client_id);
      foreach ($props as $propkey => $values) {
	foreach ($values as $lotkey => $value) {
	  if (!isset($value[$group_id])) {
	    continue;
	  }
	  if (!in_array($propkey, array_keys($property_options))) {
	    continue;
	  }
	  if (!in_array($lotkey, array_keys($lot_options))) {
	    continue;
	  }
	  $list[$propkey][$lotkey] = TRUE;
	}
      }
      $form_state['storage']['ajax']['List'] = $list;
    } else {
      $list = unserialize($form_state['storage']['ajax']['List']);
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

    $form['ajax']['List'] =
      array('#type' => 'value',
	    '#value' => serialize($list));

    $header = array(t('Property'), t('Lot'));
    $rows = array();
    foreach ($list as $property_id => $values) {
      foreach ($values as $lot_id => $value) {
	$rows[] = array($property_options[$property_id], $lot_options[$lot_id]);
      }
    }
    $form['ajax']['Name']['ListView'] =
      array('#value' => theme_table($header, $rows));

    if (empty($list)) {
      $form['ajax']['Name']['#collapsed'] = FALSE;
    }
  }

  // Create prefix and suffix name options
  $prenames = array();
  foreach (chdbadmin_get_prenameopt($company_id) as $opt) {
    $prenames[$opt['PrenameId']] = decode_entities($opt['Alias']);
  }
  if (empty($prenames)) {
    foreach (chdbadmin_get_prenameopt() as $opt) {
      $prenames[$opt['PrenameId']] = decode_entities($opt['Alias']);
    }
  }
  $sufnames = array();
  foreach (chdbadmin_get_sufnameopt($company_id) as $opt) {
    $sufnames[$opt['SufnameId']] = decode_entities($opt['Alias']);
  }
  if (empty($sufnames)) {
    foreach (chdbadmin_get_sufnameopt() as $opt) {
      $sufnames[$opt['SufnameId']] = decode_entities($opt['Alias']);
    }
  }

  // Layout person
  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['subtag'] = 'person';
  $settings['subframe'] = TRUE;
  $settings['subframetitle'] = t('Account owner');
  $settings['subframecollapsebtn'] = TRUE;
  $settings['option']['companynameopt'] = $company_id;
  $settings['option']['skipnotrequired'] = TRUE;
  $settings['option']['skipemailnotification'] = TRUE;
  $settings['option']['skiprole'] = TRUE;
  $settings['option']['skipdepartment'] = TRUE;
  $settings['option']['skiptitle'] = TRUE;
  $settings['option']['skiplogin'] = TRUE;
  $settings['default']['PrenameId'] = $group['PrenameId'];
  $settings['default']['FirstName'] = decode_entities($group['FirstName']);
  $settings['default']['MiddleName'] = decode_entities($group['MiddleName']);
  $settings['default']['LastName'] = decode_entities($group['LastName']);
  $settings['default']['SufnameId'] = $group['SufnameId'];
  $settings['default']['Street'] = decode_entities($group['Street']);
  $settings['default']['City'] = decode_entities($group['City']);
  $settings['default']['PostalCode'] = decode_entities($group['PostalCode']);
  $settings['default']['Province'] = $group['Province'];
  $settings['default']['Country'] = $group['Country'];
  $settings['default']['Phone'] = decode_entities($group['Phone']);
  $settings['default']['Email'] = decode_entities($group['Email']);
  chpprop_produce_person($form, $form_state, $settings);

  if ($form_state['storage']['ajax']['Name']['PermitCategory'] === 'Group') {
    // Layout admins
    $admins = chdbperm_get_groupadmins($company_id, $client_id, $group_id);
    foreach ($admins as $key => $admin) {
      $name = '';
      $name .= isset($prenames[$admin['PrenameId']]) ? $prenames[$admin['PrenameId']].' ' : '';
      $name .= isset($admin['FirstName']) ? decode_entities($admin['FirstName']).' ' : '';
      $name .= isset($admin['MiddleName']) ? decode_entities($admin['MiddleName']).' ' : '';
      $name .= isset($admin['LastName']) ? decode_entities($admin['LastName']).' ' : '';
      $name .= isset($sufnames[$admin['SufnameId']]) ? $sufnames[$admin['SufnameId']].' ' : '';
      $subtag = 'admin'.$admin['Id'].'_'.$admin['AdminId'];

      $settings = array();
      $settings['tag'] = 'ajax';
      $settings['subtag'] = $subtag;
      $settings['subframe'] = TRUE;
      $settings['subframetitle'] = t('Admin person: ').$name;
      $settings['subframecollapsebtn'] = TRUE;
      $settings['option']['skipnotrequired'] = TRUE;
      $settings['option']['skiplogin'] = TRUE;
      $settings['default']['PrenameId'] = $admin['PrenameId'];
      $settings['default']['FirstName'] = decode_entities($admin['FirstName']);
      $settings['default']['MiddleName'] = decode_entities($admin['MiddleName']);
      $settings['default']['LastName'] = decode_entities($admin['LastName']);
      $settings['default']['SufnameId'] = $admin['SufnameId'];
      $settings['default']['Department'] = decode_entities($admin['Department']);
      $settings['default']['Title'] = decode_entities($admin['Title']);
      $settings['default']['Role'] = decode_entities($admin['Role']);
      $settings['default']['Street'] = decode_entities($admin['Street']);
      $settings['default']['City'] = decode_entities($admin['City']);
      $settings['default']['PostalCode'] = decode_entities($admin['PostalCode']);
      $settings['default']['Province'] = $admin['Province'];
      $settings['default']['Country'] = $admin['Country'];
      $settings['default']['Phone'] = decode_entities($admin['Phone']);
      $settings['default']['Email'] = decode_entities($admin['Email']);
      $settings['default']['EmailNotification'] = $admin['EmailNotification'];
      chpprop_produce_person($form, $form_state, $settings);
      if (!isset($form_state['storage']['ajax'][$subtag]['deleteadmin'])) {
	$form_state['storage']['ajax'][$subtag]['deleteadmin'] = FALSE;
      }
      $form['ajax'][$subtag]['deleteadmin'] =
	array('#type' => 'checkbox',
	      '#title' => t('Delete this administrator'),
	      '#default_value' => $form_state['storage']['ajax'][$subtag]['deleteadmin']);
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
      $settings['subframecollapsible'] = TRUE;
      $settings['option']['companynameopt'] = $company_id;
      $settings['option']['skipnotrequired'] = TRUE;
      $settings['option']['skiplogin'] = TRUE;
      chpprop_produce_person($form, $form_state, $settings);
    }
  }

  if ($form_state['storage']['ajax']['Name']['PermitCategory'] === 'Group') {
    $form['ajax']['details'] =
      array('#type' => 'fieldset',
	    '#title' => t('Group details'),
	    '#collapsible' => TRUE);

    // Create permit options
    $options = array();
    $options['Unlimited'] = t('Allow unlimited number of permits');
    $options['Complementary'] = t('Allow complementary permits');
    $options['Shared'] = t('Allow shared permit for multiple vehicles');
    $options['Validation'] = t('Allow parking validations');

    // Layout permit options
    if (!isset($form_state['storage']['ajax']['details']['Options'])) {
      $form_state['storage']['ajax']['details']['Options'] = array();
      if (0 > $group['MaxEntries']) {
	$form_state['storage']['ajax']['details']['Options']['Unlimited'] = 'Unlimited';
      }
      if (0 < $group['Complementary']) {
	$form_state['storage']['ajax']['details']['Options']['Complementary'] = 'Complementary';
      }
      if (1 < $group['Shared']) {
	$form_state['storage']['ajax']['details']['Options']['Shared'] = 'Shared';
      }
      if (0 < $group['Validation']) {
	$form_state['storage']['ajax']['details']['Options']['Validation'] = 'Validation';
      }
    }
    $form['ajax']['details']['Options'] =
      array('#type' => 'checkboxes',
	    '#title' => t('Group options'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['details']['Options'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));

    // Conditional: Unlimited
    if (!$form_state['storage']['ajax']['details']['Options']['Unlimited']) {
      if (!isset($form_state['storage']['ajax']['details']['MaxPermits'])) {
	$form_state['storage']['ajax']['details']['MaxPermits'] =
	  0 < $group['MaxEntries'] ? $group['MaxEntries'] : 10;
      }
      $form['ajax']['details']['MaxPermits'] =
	array('#type' => 'textfield',
	      '#title' => t('Maximum number of group permits'),
	      '#maxlength' => 20,
	      '#default_value' => $form_state['storage']['ajax']['details']['MaxPermits']);
    }

    // Conditional: Complementary
    if ($form_state['storage']['ajax']['details']['Options']['Complementary']) {
      if (!isset($form_state['storage']['ajax']['details']['MaxComplementary'])) {
	$form_state['storage']['ajax']['details']['MaxComplementary'] = $group['Complementary'];
      }
      $form['ajax']['details']['MaxComplementary'] =
	array('#type' => 'textfield',
	      '#title' => t('Maximum number of complementary permits'),
	      '#maxlength' => 20,
	      '#default_value' => $form_state['storage']['ajax']['details']['MaxComplementary']);
    }

    // Conditional: Shared
    if ($form_state['storage']['ajax']['details']['Options']['Shared']) {
      if (!isset($form_state['storage']['ajax']['details']['MaxShared'])) {
	$form_state['storage']['ajax']['details']['MaxShared'] = $group['Shared'];
      }
      $form['ajax']['details']['MaxShared'] =
	array('#type' => 'textfield',
	      '#title' => t('Maximum number of vehicles sharing same permit'),
	      '#maxlength' => 20,
	      '#default_value' => $form_state['storage']['ajax']['details']['MaxShared']);
    }

    // Conditional: Validation
    if ($form_state['storage']['ajax']['details']['Options']['Validation']) {
      if (!isset($form_state['storage']['ajax']['details']['MaxValidation'])) {
	$form_state['storage']['ajax']['details']['MaxValidation'] = $group['Validation'];
      }
      $form['ajax']['details']['MaxValidation'] =
	array('#type' => 'textfield',
	      '#title' => t('Maximum number of validations'),
	      '#maxlength' => 20,
	      '#default_value' => $form_state['storage']['ajax']['details']['MaxValidation']);
    }
  }

  $form['ajax']['pay'] =
    array('#type' => 'fieldset',
	  '#title' => t('Payment'),
	  '#collapsible' => TRUE);

  // Create payment options
  $options = array();
  if ($form_state['storage']['ajax']['Name']['PermitCategory'] === 'Group') {
    $options['Landlord'] = chpperm_payment_arrangement_name('Landlord');
    $options['GroupPay'] = chpperm_payment_arrangement_name('GroupPay');
  }
  $options['IndividualPay'] = chpperm_payment_arrangement_name('IndividualPay');

  // Layout payment options
  if (!isset($form_state['storage']['ajax']['pay']['Payment'])) {
    $form_state['storage']['ajax']['pay']['Payment'] = $group['Payment'];
  }
  if (1 < count($options)) {
    $form['ajax']['pay']['Payment'] =
      array('#type' => 'radios',
	    '#title' => t('Payment arrangement'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['pay']['Payment']);
  } else {
    $form['ajax']['pay']['Payment'] =
      array('#type' => 'hidden',
	    '#value' => $form_state['storage']['ajax']['pay']['Payment']);
  }

  // Create payment interval options
  $options = array();
  $options['Daily'] = chpperm_interval_name('Daily');
  $options['Weekly'] = chpperm_interval_name('Weekly');
  $options['Monthly'] = chpperm_interval_name('Monthly');
  $options['Quarterly'] = chpperm_interval_name('Quarterly');
  $options['SemiAnnually'] = chpperm_interval_name('SemiAnnually');
  $options['Annually'] = chpperm_interval_name('Annually');


  // Layout payment interval options
  if (!isset($form_state['storage']['ajax']['pay']['PayInterval'])) {
    $form_state['storage']['ajax']['pay']['PayInterval'] = $group['PayInterval'];
  }
  $form['ajax']['pay']['PayInterval'] =
    array('#type' => 'select',
	  '#title' => t('Group payment interval'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['pay']['PayInterval']);

  if (!isset($form_state['storage']['ajax']['pay']['PayAmount'])) {
    $form_state['storage']['ajax']['pay']['PayAmount'] = sprintf("%0.02f", $group['PayAmount']);
  }
  $form['ajax']['pay']['PayAmount'] =
    array('#type' => 'textfield',
	  '#title' => t('Amount'),
	  '#maxlength' => 20,
	  '#default_value' => $form_state['storage']['ajax']['pay']['PayAmount'],
	  '#description' => t('Format: X.YY'));

  // Create billing options
  $options = array();
  $options['Invoice'] = chpperm_billing_method_name('Invoice');
  $options['CreditCard'] = chpperm_billing_method_name('CreditCard');
  $options['PAC'] = chpperm_billing_method_name('PAC');

  // Layout billing options
  if (!isset($form_state['storage']['ajax']['pay']['Billing'])) {
    $form_state['storage']['ajax']['pay']['Billing'] = $group['Billing'];
  }
  $form['ajax']['pay']['Billing'] =
    array('#type' => 'radios',
	  '#title' => t('Billing method'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['pay']['Billing']);

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpperm_group_add_form_validate_save'),
	  '#submit' => array('chpperm_group_edit_form_submit_save'));

  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpperm_group_edit_form_submit_back'));

  if (user_access('anai chp delete permit group') and
      $group['Active']) {
    $form['delete'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/delete.png',
	    '#validate' => array('chpperm_group_edit_form_validate_delete'),
	    '#submit' => array('chpperm_group_form_submit'));
  }
  return $form;
}

function chpperm_group_edit_form_validate_delete($form, &$form_state) {
  $form_state['storage']['delete_group'] = TRUE;
}

function chpperm_group_edit_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermgrp';
}

function chpperm_group_edit_form_submit_save($form, &$form_state) {
  global $user;
  $id = $form_state['storage']['id'];
  $group = chdbperm_get_groupdef_id($id);

  $alias = check_plain($form_state['values']['ajax']['Name']['Alias']);

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

  $payment = $form_state['values']['ajax']['pay']['Payment'];
  $pay_interval = $form_state['values']['ajax']['pay']['PayInterval'];
  $pay_amount = $form_state['values']['ajax']['pay']['PayAmount'];
  $billing = $form_state['values']['ajax']['pay']['Billing'];

  if ($form_state['values']['ajax']['Name']['PermitCategory'] === 'Group') {
    if ($form_state['values']['ajax']['details']['Options']['Unlimited']) {
      $max_entries = -1;
    } else {
      $max_entries = $form_state['values']['ajax']['details']['MaxPermits'];
    }
  } else {
    $max_entries = 0;
  }
  
  if ($form_state['values']['ajax']['Name']['PermitCategory'] === 'Group' and
      $form_state['values']['ajax']['details']['Options']['Complementary']) {
    $complementary = $form_state['values']['ajax']['details']['MaxComplementary'];
  } else {
    $complementary = 0;
  }

  if ($form_state['values']['ajax']['Name']['PermitCategory'] === 'Group') {
    if ($form_state['values']['ajax']['details']['Options']['Shared']) {
      $shared = $form_state['values']['ajax']['details']['MaxShared'];
    } else {
      $shared = 1;
    }
  } else {
    $shared = 0;
  }

  if ($form_state['values']['ajax']['Name']['PermitCategory'] === 'Group' and
      $form_state['values']['ajax']['details']['Options']['Validation']) {
    $validation = $form_state['values']['ajax']['details']['MaxValidation'];
  } else {
    $validation = 0;
  }

  if (chdbperm_update_groupdef
      ($id, $user->uid, $alias, $lat, $lon,
       $prename_id, $firstname, $middlename, $lastname, $sufname_id,
       $street, $city, $postalcode, $province, $country,
       $phone, $email,
       $max_entries, $complementary, $shared, $validation,
       $payment, $pay_interval, $pay_amount, $billing)) {
    drupal_set_message('Could not update group', 'error');
    return;
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

  // Manage Active and Suspended state
  if (($activate and !$group['Active']) or
      ($suspended and !$group['Suspended']) or
      (!$suspended and $group['Suspended'])) {
    if (chdbperm_update_groupdef_status
	($id,
	 $activate,
	 $suspended)) {
      drupal_set_message('Could not update group status', 'error');
      return;
    }

    if (chdbperm_update_groupadmins_status
	($group['CompanyId'], $group['GroupId'],
	 $activate,
	 $suspended)) {
      drupal_set_message('Could not update group admin status', 'error');
      return;
    }

    if (chdbperm_update_permits_status
	($group['CompanyId'], $group['ClientId'], $group['GroupId'],
	 $activate,
	 $suspended)) {
      drupal_set_message('Could not update group permit status', 'error');
      return;
    }
  }

  if ($form_state['values']['ajax']['Name']['PermitCategory'] === 'Group') {
    // Update administrators
    foreach ($form_state['values']['ajax'] as $key => $value) {
      if (substr($key, 0, strlen('admin')) !== 'admin') {
	continue;
      } //anai_dbg($key);anai_dbg($value);
      $id = substr($key, strlen('admin'), strpos($key, '_') - strlen('admin'));
      $admin_id = substr($key, 1 + strpos($key, '_'));
      $subtag = 'admin'.$id.'_'.$admin_id;

      if ($form_state['values']['ajax'][$subtag]['deleteadmin']) {
	if (chdbperm_delete_groupadmin
	    ($group['CompanyId'], $group['GroupId'], $admin_id)) {
	  drupal_set_message('Could not delete group admin', 'error');
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

      if (isset($form_state['values']['ajax'][$subtag]['Role'])) {
	$role = check_plain($form_state['values']['ajax'][$subtag]['Role']);
      } else {
	$role = NULL;
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

      if (chdbperm_update_groupadmin
	  ($id,
	   $prename_id, $firstname, $middlename, $lastname, $sufname_id,
	   $street, $city, $postalcode, $province, $country,
	   $phone, $email, $emailnotification,
	   $role, $department, $title)) {
	drupal_set_message('Could not update group admin', 'error');
	return;
      }

      if (chdbperm_update_groupadmin_latlon
	  ($id, $lat, $lon)) {
	drupal_set_message('Could not update admin lat/lon', 'error');
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

      if (isset($form_state['values']['ajax']['newadmin']['Role'])) {
	$role = check_plain($form_state['values']['ajax']['newadmin']['Role']);
      } else {
	$role = NULL;
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

      if (chdbperm_insert_groupadmin
	  ($group['CompanyId'], $group['ClientId'], $group['GroupId'], $admin_id, $lat, $lon,
	   $prename_id, $firstname, $middlename, $lastname, $sufname_id,
	   $street, $city, $postalcode, $province, $country,
	   $phone, $email, $emailnotification,
	   $role, $department, $title,
	   $id)) {
	drupal_set_message('Could not insert group admin', 'error');
	return;
      }
    }
  }

  $list = unserialize($form_state['values']['ajax']['List']);

  // Delete property assignments for those not in new list
  $props = chpperm_retrieve_group_props($user->uid, $group['CompanyId'], $group['ClientId']);
  foreach ($props as $propkey => $values) {
    foreach ($values as $lotkey => $value) {
      if (!isset($value[$group['GroupId']])) {
	continue;
      }
      if (isset($list[$propkey][$lotkey])) {
	// Mark as not needing insertion
	$list[$propkey][$lotkey] = FALSE;
      } else {
	if (chdbperm_delete_groupprop
	    ($group['CompanyId'], $group['ClientId'], $group['GroupId'],
	     $propkey, $lotkey)) {
	  drupal_set_message('Could not delete group from property/lot', 'error');
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
	  if (chdbperm_insert_groupprop
	      ($group['CompanyId'], $group['ClientId'], $group['GroupId'],
	       $propkey, $lotkey,
	       $id)) {
	    drupal_set_message('Could not attach group to property/lot', 'error');
	    return;
	  }
	}
      }
    }
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermgrp';
}

/****************************************************************************/

function chpperm_group_money_form($form_state) {

  $id = $form_state['storage']['id'];
  $group = chdbperm_get_groupdef_id($id);
  drupal_set_title(t('Group payments - GRP',
		     array('GRP' => decode_entities($group['Alias']))));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['buttons']['back']['submit'] = 'chpperm_group_money_form_submit_back';
  if (user_access('anai chp add permit payments')) {
    $settings['buttons']['add']['submit'] = 'chpperm_group_money_form_submit_add';
  }
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }

  $header = array();
  $row[] = array(t('Payment arrangement'), chpperm_payment_arrangement_name($group['Payment']));
  $row[] = array(t('Payment interval'), chpperm_interval_name($group['PayInterval']));
  $row[] = array(t('Payment amount'), sprintf("%0.02f", $group['PayAmount']));
  $row[] = array(t('Billing method'), chpperm_billing_method_name($group['Billing']));
  $form['ajax']['settings'] = array('#value' => theme('table', $header, $row));

  $form['ajax']['TBD'] =
    array('#value' => '<center><h2>Payment integration will be inlcuded in next releases.</h2></center>');

  $company_id = $group['CompanyId'];
  $client_id= $group['ClientId'];
  $group_id= $group['GroupId'];

  $grouppays = chdbperm_get_grouppays($company_id, $client_id, $group_id);

  $form['ajax']['history'] =
    array('#type' => 'fieldset', '#title' => t('Payment history'));
  if (empty($grouppays)) {
    $form['ajax']['history']['list'] = array('#value' => t('Empty.'));
  } else {
    $form['ajax']['history']['list'] = chpperm_group_money_table_form($grouppays);
  }

  return $form;
}

function chpperm_group_money_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermgrp';
}

function chpperm_group_money_form_submit_add($form, &$form_state) {
  $id = $form_state['storage']['id'];
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermgrppayadd/'.$id;
}

function chpperm_group_money_table_form($entries) {
  global $user;
  $form['db'] = array('#type' => 'markup', '#theme' => 'chpperm_group_money_table');

  $looper = 0;
  foreach ($entries as $entry) {
    $time = chpuser_datetime_utc_to_usertimezone($entry['PayTime']);
    $form['db']['time_'.$looper] = array('#value' => $time->format("M j,y H:i"));
    $form['db']['billing_'.$looper] = array('#value' => decode_entities($entry['Billing']));
    $form['db']['billinginfo_'.$looper] = array('#value' => decode_entities($entry['BillingInfo']));
    $form['db']['amount_'.$looper] = array('#value' => decode_entities($entry['PayAmount']));
    $form['db']['reference_'.$looper] = array('#value' => decode_entities($entry['PayReference']));
    $time = chpuser_datetime_utc_to_usertimezone($entry['ExpiryTime']);
    $form['db']['expiry_'.$looper] = array('#value' => $time->format("M j,y H:i"));

    $looper++;
  }
  return $form;
}

function theme_chpperm_group_money_table($form) {
  $header = array(t('Time'), t('Billing'), t('Billing info'), t('Amount'), t('Reference'), t('Expiry'));
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'time_')) {
      continue;
    }
    $looper = substr($key, strlen('time_'));
    $rows[] = array(drupal_render($form['time_'.$looper]),
                    drupal_render($form['billing_'.$looper]),
                    drupal_render($form['billinginfo_'.$looper]),
                    drupal_render($form['amount_'.$looper]),
                    drupal_render($form['reference_'.$looper]),
                    drupal_render($form['expiry_'.$looper]),
                    );
  }
  // Render Table
  return theme_table($header, $rows);
}

function chpperm_group_pay_map($entry) {
  $map_array['id'] = 'LocationMap';
  $map_array['width'] = '360px';
  $map_array['height'] = '300px';
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
  $address = decode_entities($entry['Street']);
  $address .= '<br>'.decode_entities($entry['City']);
  $address .= ', '.$entry['Province'];
  $address .= '<br>'.decode_entities($entry['PostalCode']);
  $address .= ', '.$entry['Country'];
  $markers[] = array
    ('text' => '<b>'.t('Address:').'</b><br>'.$address,
     'latitude' => sprintf("%f", $entry['Lat']),
     'longitude' => sprintf("%f", $entry['Lon']),
     'markername' => 'blue');
  $map_array['markers'] = $markers;
  return theme('gmap', array('#settings' => $map_array));
}

function chpperm_group_list_pay_form($form_state) {
  drupal_set_title(t('Permit payment'));

  $id = $form_state['storage']['id'];
  $group = chdbperm_get_groupdef_id($id);
  $list_id = $form_state['storage']['list_id'];
  $entry = chdbperm_get_grouplist_id($list_id);

  $header = array();
  $row = array();
  if (!empty($entry['FirstName'])) {
    $row[] = array(t('First name:'), decode_entities($entry['FirstName']));
  }
  if (!empty($entry['LastName'])) {
    $row[] = array(t('Last name:'), decode_entities($entry['LastName']));
  }
  if (!empty($entry['Phone'])) {
    $row[] = array(t('Phone:'), decode_entities($entry['Phone']));
  }
  if (!empty($entry['Email'])) {
    $row[] = array(t('Email:'), decode_entities($entry['Email']));
  }
  $row[] = array(t('Permit type:'), $entry['Complementary'] ? t('Complementary') : t('Subscribed'));
  $row[] = array(t('Group:'), decode_entities($group['Alias']));
  $row[] = array(t('Alias:'), decode_entities($group['Alias']));

  if (!empty($entry['Lat']) and !empty($entry['Lon'])) {
    $form['t1'] =
      array('#prefix' => '<table border="0"><tr valign="top"><td align="left" width="60%">',
	    '#value' => theme('table', $header, $row),
	    '#suffix' => '</td>');

    $form['t2'] =
      array('#prefix' => '<td>',
	    '#value' => chpperm_group_pay_map($entry),
	    '#suffix' => '</td></tr></table>');
  } else {
    $form['t1'] =
      array('#value' => theme('table', $header, $row));
  }

  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }

  $options = array();
  $options['Cheque'] = t('Cheque');
  $options['CreditCard'] = t('Credit Card');
  $options['Invoice'] = t('Invoice');
  $options['PreAuthCredit'] = t('Pre-Authorized');

  if (!isset($form_state['storage']['ajax']['pay']['Payment'])) {
    $form_state['storage']['ajax']['pay']['Payment'] = $entry['Payment'];
  }
  $form['ajax']['pay']['Payment'] =
    array('#type' => 'select',
	  '#title' => t('Payment method'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['pay']['Payment'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  if ($form_state['storage']['ajax']['pay']['Payment'] === 'Cheque') {

    if (!isset($form_state['storage']['ajax']['pay']['Cheque'])) {
      $form_state['storage']['ajax']['pay']['Cheque'] = '';
    }
    $form['ajax']['pay']['Cheque'] =
      array('#type' => 'textfield',
	    '#title' => t('Cheque receipt reference'),
	    '#maxlength' => 80,
	    '#default_value' => $form_state['storage']['ajax']['pay']['Cheque']);

  } elseif ($form_state['storage']['ajax']['pay']['Payment'] === 'CreditCard') {

    $form['ajax']['pay']['CreditCard'] =
      array('#value' => '<p>'.PAYMENT_CREDIT_CARD.'<p><p>');

  } elseif ($form_state['storage']['ajax']['pay']['Payment'] === 'Invoice') {

    if (!isset($form_state['storage']['ajax']['pay']['Invoice'])) {
      $form_state['storage']['ajax']['pay']['Invoice'] = '';
    }
    $form['ajax']['pay']['Invoice'] =
      array('#type' => 'textfield',
	    '#title' => t('Invoice receipt reference'),
	    '#maxlength' => 80,
	    '#default_value' => $form_state['storage']['ajax']['pay']['Invoice']);

  } elseif ($form_state['storage']['ajax']['pay']['Payment'] === 'PreAuthCredit') {

    $form['ajax']['pay']['PreAuthCredit'] =
      array('#value' => '<p>'.PAYMENT_CREDIT_CARD.'<p><p>');

  }

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#submit' => array('chpperm_group_list_pay_form_submit_save'));

  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpperm_group_list_pay_form_submit_back'));

  return $form;
}

function chpperm_group_list_pay_form_submit_back($form, &$form_state) {
  unset($form_state['storage']['detailed_money_pay']);
}

function chpperm_group_list_pay_form_submit_save($form, &$form_state) {
  unset($form_state['storage']['detailed_money_pay']);
}

function chpperm_group_form_submit($form, &$form_state) {
  global $user;
  if ($form_state['storage']['confirm_delete_group']) {
    $id = $form_state['storage']['id'];
    $groupdef = chdbperm_get_groupdef_id($id);
    chdbperm_retire_groupdef_id
      ($id, $user->uid, t('Group deleted'));
    chdbperm_retire_groupadmins
      ($groupdef['CompanyId'], $groupdef['ClientId'], $groupdef['GroupId'], $user->uid, t('Group deleted'));
    chdbperm_delete_groupprop
      ($groupdef['CompanyId'], $groupdef['ClientId'], $groupdef['GroupId']);
    chdbperm_retire_permits
      ($user->uid, $groupdef['CompanyId'], $groupdef['GroupId'], NULL, t('Group deleted'));
    chdbperm_retire_permits
      ($user->uid, $groupdef['CompanyId'], NULL, $groupdef['GroupId'], t('Group deleted'));
    unset($form_state['storage']);
    $form_state['redirect'] = 'chppermgrp';
    return;
  }
  if ($form_state['storage']['delete_group']) {
    $form_state['storage']['confirm_delete_group'] = TRUE;
    return;
  }
  if ($form_state['storage']['confirm_delete_list']) {
    $list_id = $form_state['storage']['list_id'];
    chdbperm_delete_grouplist_id($list_id);
    unset($form_state['storage']['confirm_delete_list']);
    unset($form_state['storage']['detailed_money_edit']);
    return;
  }
  if ($form_state['storage']['delete_list']) {
    $form_state['storage']['confirm_delete_list'] = TRUE;
    return;
  }

  // Notes buttons
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
    $form_state['storage']['detailed_notes_edit'] = TRUE;
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
    $form_state['storage']['detailed_notes_solve'] = TRUE;
    return;
  }

  // View buttons
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
    if (strstr($key, 'op_edit')) {
      $id = substr($key, strlen('op_edit'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['id'] = $id;
    $form_state['storage']['detailed_edit'] = TRUE;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_money')) {
      $id = substr($key, strlen('op_money'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['id'] = $id;
    $form_state['storage']['detailed_money'] = TRUE;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_moneyedit')) {
      $id = substr($key, strlen('op_moneyedit'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['list_id'] = $id;
    $form_state['storage']['detailed_money_edit'] = TRUE;
    return;
  }
}



/**********
 ********** Permit group payment add
 ********** Address: chppermgrppayadd
 ********** Access:  'anai chp add permit payments'
 *********/



function chpperm_group_payadd_form($form_state, $id) {
  drupal_set_title(t('Add payment'));

  if (!user_access('anai chp add permit payments')) {
    drupal_access_denied();
    exit();
  }

  if (!is_numeric($id)) {
    drupal_access_denied();
    exit();
  }

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $group = chdbperm_get_groupdef_id($id);

  if (empty($group)) {
    $form['Empty'] = array('#value' => t('Group not found.'));
    return $form;
  }

  // Save id for validation and submit retrieval
  $form['id'] = array('#type' => 'hidden', '#value' => $id);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  // payment arrangement is read only from group settings
  $form['ajax']['Payment'] =
    array('#type' => 'item',
	  '#title' => t('Payment arrangement'),
	  '#value' => chpperm_payment_arrangement_name($group['Payment']));

  // Amount is read only from group settings
  $form['ajax']['Payment amount'] =
    array('#type' => 'item',
	  '#title' => t('Amount'),
	  '#value' => sprintf("%0.02f", $group['PayAmount']));

  // Billing is read only from group settings
  $form['ajax']['Billing'] =
    array('#type' => 'item',
	  '#title' => t('Billing method'),
	  '#value' => chpperm_billing_method_name($group['Billing']));

  // Javascript date pickers
  drupal_add_js
    ('(function ($) {
          Drupal.behaviors.ANAIchppermitsGroupPayExpiry=function(context){
      	    $("#edit-ajax-ExpiryTime:not(.ExpiryTime-processed)",
              context).addClass("ExpiryTime-processed")
              .AnyTime_picker({format:"%Y-%m-%d",hideInput:true,
                              placement:"inline",askSecond:false,
                              labelTitle:"Expiry Date"});
          };
       }(jQuery));', 'inline');

  if (!isset($form_state['storage']['ajax']['ExpiryTime'])) {
    $now = chpuser_datetime_utc_to_usertimezone('now');
    $form_state['storage']['ajax']['ExpiryTime'] = $now->format("Y-m-d 23:59:59");
  }
  $form['ajax']['ExpiryTime'] =
    array('#type' => 'textfield',
	  '#default_value' => $form_state['storage']['ajax']['ExpiryTime']);

  if (!isset($form_state['storage']['ajax']['BillingInfo'])) {
    $form_state['storage']['ajax']['BillingInfo'] = '';
  }
  $form['ajax']['BillingInfo'] =
    array('#type' => 'textfield',
	  '#title' => t('Billing info'),
	  '#maxlength' => 128,
	  '#default_value' => $form_state['storage']['ajax']['BillingInfo'],
	  '#description' => t('Invoince number, etc.'));

  if (!isset($form_state['storage']['ajax']['PayReference'])) {
    $form_state['storage']['ajax']['PayReference'] = '';
  }
  $form['ajax']['PayReference'] =
    array('#type' => 'textfield',
	  '#title' => t('Payment reference'),
	  '#maxlength' => 64,
	  '#default_value' => $form_state['storage']['ajax']['PayReference'],
	  '#description' => t('From payment institution, confirmation number, etc.'));

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpperm_group_payadd_form_validate_save'),
	  '#submit' => array('chpperm_group_payadd_form_submit_save'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpperm_group_payadd_form_submit_cancel'));

  return $form;
}

function chpperm_group_payadd_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermgrp';
}

function chpperm_group_payadd_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['ajax']['BillingInfo'])) {
    form_set_error('ajax][BillingInfo', 'Billing info field is required.');
    return;
  }
  if (empty($form_state['values']['ajax']['PayReference'])) {
    form_set_error('ajax][PayReference', 'Payment reference field is required.');
    return;
  }
}

function chpperm_group_payadd_form_submit_save($form, &$form_state) {
  $id = $form_state['values']['id'];
  $group = chdbperm_get_groupdef_id($id);

  $company_id = $group['CompanyId'];
  $client_id = $group['ClientId'];
  $group_id = $group['GroupId'];

  chdbperm_update_grouppay_inactivate($company_id, $client_id, $group_id);

  $amount = $group['PayAmount'];
  $reference = check_plain($form_state['values']['ajax']['PayReference']);
  $billing = $group['Billing'];
  $billing_info = check_plain($form_state['values']['ajax']['BillingInfo']);
  $expiry = chpuser_datetime_usertimezone_to_utc($form_state['values']['ajax']['ExpiryTime']);

  if (chdbperm_insert_grouppay
      ($company_id, $client_id, $group_id,
       $amount, $reference, $billing, $billing_info, $expiry->format("Y-m-d H:i:s"),
       $id)) {
    drupal_set_message('Could not insert payment', 'error');
    return;
  }

  if (chdbperm_update_group_permits_expiry
      ($company_id, $group_id,
       $expiry->format("Y-m-d H:i:s"))) {
    drupal_set_message('Could not update permits expiry', 'error');
    return;
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermgrp';
}



