<?php
// $Id$
require_once("anai_chpvalet.inc");



function chpvalet_vas_form($form_state) {
  if ($form_state['storage']['detailed_edit']) {
    return chpvalet_vas_edit_form($form_state);
  }

  drupal_set_title(t('Valet services'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['show_companies'] = TRUE;
  $settings['show_properties'] = TRUE;
  $settings['policy_filter'] = array('ValetPolicy');
  $settings['buttons']['back']['title'] = t('Back');
  $settings['buttons']['back']['validate'] = 'chpvalet_vas_form_validate_back';
  $settings['buttons']['back']['submit'] = 'chpvalet_vas_form_submit';
  if (user_access('anai chp add valet vas')) {
    $settings['buttons']['add']['title'] = t('Add new service');
    $settings['buttons']['add']['validate'] = 'chpvalet_vas_form_validate_add';
    $settings['buttons']['add']['submit'] = 'chpvalet_vas_form_submit';
  }
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $property_id = $form_state['storage']['ajax']['PropertyId'];

  $entries = array();

  // Retrieve dedicated services for the property
  foreach (chdbvalet_get_vases($company_id,
			       FALSE, // Template
			       $property_id) as $vas) {
    $entries[$vas['Id']] = $vas;
  }
  // Add up any template
  foreach (chdbvalet_get_vases($company_id,
			       TRUE, // Template
			       FALSE) as $vas) {
    $entries[$vas['Id']] = $vas;
  }


  if (sizeof($entries) == 0) {
    $form['ajax']['empty'] =
      array('#value' => '<p><p>'.t('Empty. No services registered.').'<p>');
    return $form;
  }

  $form['ajax']['list'] = chpvalet_vas_table_form($entries);

  return $form;
}

function chpvalet_vas_form_validate_add($form, &$form_state) {
  $form_state['storage']['add'] = TRUE;
}

function chpvalet_vas_form_validate_back($form, &$form_state) {
  $form_state['storage']['back'] = TRUE;
}

function chpvalet_vas_table_form($entries) {
  global $user;
  $form['db'] =
    array('#type' => 'markup', '#value' => '',
	  '#theme' => 'chpvalet_vas_table');

  if (user_access('anai chp edit valet vas')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }

  $looper = 0;
  foreach ($entries as $entry) {
    $form['db']['offer_'.$looper] = array('#value' => $entry['Offer']);
    $form['db']['choice_'.$looper] = array('#value' => $entry['Choice']);
    $form['db']['amount_'.$looper] = array('#value' => $entry['Amount']);

    if ($readonly) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_edit'.$entry['Id'],
	      '#submit' => array('chpvalet_vas_form_submit'));
    }
    $looper++;
  }
  return $form;
}

function theme_chpvalet_vas_table($form) {
  // Table Header
  $header = array('Offer', 'Choice', 'Amount', '');
  // Table Rows
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'offer_')) {
      continue;
    }
    $looper = substr($key, strlen('offer_'));
    $rows[] = array(drupal_render($form['offer_'.$looper]),
		    drupal_render($form['choice_'.$looper]),
		    drupal_render($form['amount_'.$looper]),
		    array('data' => drupal_render($form['edit_'.$looper]),
			  'width' => 1, 'height' => 1),
		    );
  }
  // Render Table
  return theme_table($header, $rows);
}

function chpvalet_vas_edit_form($form_state) {
  if ($form_state['storage']['confirm_save']) {
    return confirm_form($form, 'Are you sure?', 'chpvaletvas',
			'The service will be active on all properties.');
  }
  if ($form_state['storage']['confirm_delete']) {
    return confirm_form($form, 'Are you sure?', 'chpvaletvas',
       'The service will be deleted from the system on any property.');
  }

  drupal_set_title(t('Edit service'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $id = $form_state['storage']['id']; //drupal_set_message('Id:'.$id);
  $vas = chdbvalet_get_vas_id($id);

  $settings = array();
  $settings['tag'] = 'ajax';
  //$settings['show_companies'] = TRUE;
  $settings['show_properties'] = TRUE;
  $settings['show_all_properties'] = $vas['Template'] > 0 ? TRUE : FALSE;
  $settings['policy_filter'] = array('ValetPolicy');
  $settings['default']['PropertyId'] = $vas['PropertyId'];
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $property_id = $form_state['storage']['ajax']['PropertyId'];
  $lot_id = $form_state['storage']['ajax']['LotId'];

  if (!isset($form_state['storage']['ajax']['Offer'])) {
    $form_state['storage']['ajax']['Offer'] = decode_entities($vas['Offer']);
  }
  $form['ajax']['Offer'] =
    array('#type' => 'textfield',
	  '#title' => t('Offer'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['Offer']);

  if (!isset($form_state['storage']['ajax']['Choice'])) {
    $form_state['storage']['ajax']['Choice'] = decode_entities($vas['Choice']);
  }
  $form['ajax']['Choice'] =
    array('#type' => 'textfield',
	  '#title' => t('Choice'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['Choice']);

  if (!isset($form_state['storage']['ajax']['Amount'])) {
    $form_state['storage']['ajax']['Amount'] = $vas['Amount'];
  }
  $form['ajax']['Amount'] =
    array('#type' => 'textfield',
	  '#title' => t('Amount'),
	  '#maxlength' => 10,
	  '#description' => 'Format: X.XX',
	  '#default_value' => $form_state['storage']['ajax']['Amount']);

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpvalet_vas_edit_form_validate_save'),
	  '#submit' => array('chpvalet_vas_form_submit'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#validate' => array('chpvalet_vas_edit_form_validate_cancel'),
	  '#submit' => array('chpvalet_vas_form_submit'));

  if (user_access('anai chp delete valet vas')) {
    $form['ajax']['delete'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/delete.png',
  	    '#validate' => array('chpvalet_vas_edit_form_validate_delete'),
	    '#submit' => array('chpvalet_vas_form_submit'));
  }

  return $form;
}

function chpvalet_vas_edit_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['ajax']['Offer'])) {
    form_set_error('ajax][Offer', t('Offer field is required.'));
    return;
  }
  if (empty($form_state['values']['ajax']['Choice'])) {
    form_set_error('ajax][Choice', t('Choice field is required.'));
    return;
  }
  if (empty($form_state['values']['ajax']['Amount'])) {
    form_set_error('ajax][Amount', t('Amount field is required.'));
    return;
  }
  if (!preg_match('/\d+\.\d{2}/', $form_state['values']['ajax']['Amount'])) {
    form_set_error('ajax][Amount', 'Amount must be given in format X.XX');
    return;
  }

  $id = $form_state['storage']['id'];
  $vas = chdbvalet_get_vas_id($id);

  if ($vas['Offer'] != check_plain($form_state['values']['ajax']['Offer']) or
      $vas['Choice'] != check_plain($form_state['values']['ajax']['Choice'])) {
    if (chdbvalet_vas_exists
	($form_state['values']['ajax']['CompanyId'],
	 $form_state['values']['ajax']['all_properties'], // Template
	 check_plain($form_state['values']['ajax']['Offer']),
	 check_plain($form_state['values']['ajax']['Choice']),
	 $form_state['values']['ajax']['PropertyId'])) {
      form_set_error('Offer', t('Service already in use.'));
      return;
    }
  }

  $form_state['storage']['ajax']['CompanyId'] =
    $form_state['values']['ajax']['CompanyId'];
  $form_state['storage']['ajax']['PropertyId'] =
    $form_state['values']['ajax']['PropertyId'];
  $form_state['storage']['ajax']['all_properties'] =
    $form_state['values']['ajax']['all_properties'];

  $form_state['storage']['ajax']['Offer'] =
    $form_state['values']['ajax']['Offer'];
  $form_state['storage']['ajax']['Choice'] =
    $form_state['values']['ajax']['Choice'];
  $form_state['storage']['ajax']['Amount'] =
    $form_state['values']['ajax']['Amount'];

  if ($form_state['values']['ajax']['all_properties']) {
    $form_state['storage']['confirm_cmd'] = TRUE;
  } else {
    $form_state['storage']['save'] = TRUE;
  }
}

function chpvalet_vas_edit_form_validate_cancel($form, &$form_state) {
  $form_state['storage']['cancel'] = TRUE;
}

function chpvalet_vas_edit_form_validate_delete($form, &$form_state) {
  $form_state['storage']['delete'] = TRUE;
}

function chpvalet_vas_form_submit($form, &$form_state) {
  if ($form_state['storage']['home']) {
    unset($form_state['storage']);
    $form_state['redirect'] = 'front_page';
    return;
  }
  if ($form_state['storage']['back']) {
    unset($form_state['storage']);
    $form_state['redirect'] = 'chpvaletlist';
    return;
  }
  if ($form_state['storage']['cancel']) {
    unset($form_state['storage']);
    $form_state['redirect'] = 'chpvaletvas';
    return;
  }
  if ($form_state['storage']['add']) {
    unset($form_state['storage']);
    $form_state['redirect'] = 'chpvaletvasadd';
    return;
  }
  if ($form_state['storage']['confirm_delete']) {
    $id = $form_state['storage']['id'];
    if (chdbvalet_delete_vas_id($id)) {
      drupal_set_message('Could not delete valet vas', 'error');
      return;
    }
    unset($form_state['storage']);
    $form_state['redirect'] = 'chpvaletvas';
    return;
  }
  if ($form_state['storage']['delete']) {
    unset($form_state['storage']['delete']);
    $form_state['storage']['confirm_delete'] = TRUE;
    return;
  }
  if ($form_state['storage']['confirm_cmd']) {
    unset($form_state['storage']['confirm_cmd']);
    $form_state['storage']['confirm_save'] = TRUE;
    $form_state['storage']['save'] = TRUE;
    return;
  }
  if ($form_state['storage']['save']) {
    $id = $form_state['storage']['id'];
    $vas = chdbvalet_get_vas_id($id);
    $company_id = $form_state['storage']['ajax']['CompanyId'];
    $property_id = $form_state['storage']['ajax']['PropertyId'];

    if (chdbvalet_update_vas
	($id,
	 $form_state['storage']['ajax']['all_properties'], // Template
	 $property_id,
	 check_plain($form_state['storage']['ajax']['Offer']),
	 check_plain($form_state['storage']['ajax']['Choice']),
	 check_plain($form_state['storage']['ajax']['Amount']),
	 $id)) {
      drupal_set_message('Could not update service', 'error');
      return;
    }

    unset($form_state['storage']);
    $form_state['redirect'] = 'chpvaletvas';
    return;
  }

  unset($form_state['storage']['ajax']);

  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_edit')) {
      $id = substr($key, strlen('op_edit'));
      break;
    }
  }
  if ($id > 0) {
    //drupal_set_message('Detailed edit of id:'.$id);
    $form_state['storage']['id'] = $id;
    $form_state['storage']['detailed_edit'] = TRUE;
    return;
  }
}


/**********
 ********** Valet vas add
 ********** Address: chpvaletvasadd
 ********** Access:  'anai chp add valet vas'
 *********/



function chpvalet_vas_add_form($form_state) {
  if ($form_state['storage']['confirm_save']) {
    return confirm_form($form, 'Are you sure?', 'chpvaletvas',
			'The valet service will be active on all properties.');
  }

  drupal_set_title(t('Add service'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['show_companies'] = TRUE;
  $settings['show_properties'] = TRUE;
  $settings['show_all_properties'] = FALSE; // Default value
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $property_id = $form_state['storage']['ajax']['PropertyId'];
  $lot_id = $form_state['storage']['ajax']['LotId'];

  if (!isset($form_state['storage']['ajax']['Offer'])) {
    $form_state['storage']['ajax']['Offer'] = '';
  }
  $form['ajax']['Offer'] =
    array('#type' => 'textfield',
	  '#title' => t('Offer'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['Offer']);

  if (!isset($form_state['storage']['ajax']['Choice'])) {
    $form_state['storage']['ajax']['Choice'] = '';
  }
  $form['ajax']['Choice'] =
    array('#type' => 'textfield',
	  '#title' => t('Choice'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['Choice']);

  if (!isset($form_state['storage']['ajax']['Amount'])) {
    $form_state['storage']['ajax']['Amount'] = '0.00';
  }
  $form['ajax']['Amount'] =
    array('#type' => 'textfield',
	  '#title' => t('Amount'),
	  '#maxlength' => 10,
	  '#description' => 'Format: X.XX',
	  '#default_value' => $form_state['storage']['ajax']['Amount']);

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpvalet_vas_add_form_validate_save'),
	  '#submit' => array('chpvalet_vas_add_form_submit'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#validate' => array('chpvalet_vas_add_form_validate_cancel'),
	  '#submit' => array('chpvalet_vas_add_form_submit'));
  return $form;
}

function chpvalet_vas_add_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['ajax']['Offer'])) {
    form_set_error('ajax][Offer', t('Offer field is required.'));
    return;
  }
  if (empty($form_state['values']['ajax']['Choice'])) {
    form_set_error('ajax][Choice', t('Choice field is required.'));
    return;
  }
  if (empty($form_state['values']['ajax']['Amount'])) {
    form_set_error('ajax][Amount', t('Amount field is required.'));
    return;
  }
  if (!preg_match('/\d+\.\d{2}/', $form_state['values']['ajax']['Amount'])) {
    form_set_error('ajax][Amount', 'Amount must be given in format X.XX');
    return;
  }

  if (chdbvalet_vas_exists
      ($form_state['storage']['ajax']['CompanyId'],
       $form_state['storage']['ajax']['all_properties'], // Template
       check_plain($form_state['values']['ajax']['Offer']),
       check_plain($form_state['values']['ajax']['Choice']),
       $form_state['storage']['ajax']['PropertyId'])) {
    form_set_error('ajax][Offer', t('Offer already in use.'));
    return;
  }

  $form_state['storage']['ajax']['Offer'] =
    $form_state['values']['ajax']['Offer'];
  $form_state['storage']['ajax']['Choice'] =
    $form_state['values']['ajax']['Choice'];
  $form_state['storage']['ajax']['Amount'] =
    $form_state['values']['ajax']['Amount'];

  if ($form_state['storage']['ajax']['all_properties']) {
    $form_state['storage']['confirm_cmd'] = TRUE;
  } else {
    $form_state['storage']['save'] = TRUE;
  }
}

function chpvalet_vas_add_form_validate_cancel($form, &$form_state) {
  $form_state['storage']['back'] = TRUE;
}

function chpvalet_vas_add_form_submit($form, &$form_state) {
  if ($form_state['storage']['back']) {
    unset($form_state['storage']);
    $form_state['redirect'] = 'chpvaletvas';
    return;
  }
  if ($form_state['storage']['home']) {
    unset($form_state['storage']);
    $form_state['redirect'] = 'front_page';
    return;
  }
  if ($form_state['storage']['confirm_cmd']) {
    unset($form_state['storage']['confirm_cmd']);
    $form_state['storage']['confirm_save'] = TRUE;
    $form_state['storage']['save'] = TRUE;
    return;
  }
  if ($form_state['storage']['save']) {
    $company_id = $form_state['storage']['ajax']['CompanyId'];
    $property_id = $form_state['storage']['ajax']['PropertyId'];

    // Since validation has made sure the Offer/Choice combination is new,
    // then just insert without any concerns.
    if (chdbvalet_insert_new_vas
	($company_id,
	 $form_state['storage']['ajax']['all_properties'], // Template
	 $property_id,
	 check_plain($form_state['storage']['ajax']['Offer']),
	 check_plain($form_state['storage']['ajax']['Choice']),
	 check_plain($form_state['storage']['ajax']['Amount']),
	 $id)) {
      drupal_set_message('Could not insert new service', 'error');
      return;
    }

    unset($form_state['storage']);
    $form_state['redirect'] = 'chpvaletvas';
    return;
  }
}




