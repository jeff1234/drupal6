<?php
// $Id$
require_once("anai_chpvalet.inc");



function chpvalet_codes_form($form_state) {
  if ($form_state['storage']['detailed_edit']) {
    return chpvalet_codes_edit_form($form_state);
  }

  drupal_set_title(t('Valet codes'));

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
  $settings['show_properties'] = TRUE;
  $settings['show_lots'] = TRUE;
  $settings['buttons']['back']['title'] = t('Back');
  $settings['buttons']['back']['submit'] = 'chpvalet_codes_form_submit_back';
  if (user_access('anai chp add valet codes')) {
    $settings['buttons']['add']['title'] = t('Add new code');
    $settings['buttons']['add']['submit'] = 'chpvalet_codes_form_submit_add';
  }
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $property_id = $form_state['storage']['ajax']['PropertyId'];
  $lot_id = $form_state['storage']['property']['LotId'];

  $codes = array();

  // Retrieve codes dedicated for the property
  foreach (chdbvalet_get_codes($company_id,
			       FALSE, // Template
			       $property_id) as $code) {
    if (!isset($codes[$code['Code']])) {
      $codes[$code['Code']] = $code;
    }
  }
  // Add up any template
  foreach (chdbvalet_get_codes($company_id,
			       TRUE, // Template
			       FALSE) as $code) {
    if (!isset($codes[$code['Code']])) {
      $codes[$code['Code']] = $code;
    }
  }

  if (sizeof($codes) == 0) {
    $form['ajax']['empty'] =
      array('#value' => '<p><p>'.t('Empty. No valet codes registered.').'<p>');
    return $form;
  }

  $form['ajax']['list'] = chpvalet_codes_table_form($codes);

  return $form;
}

function chpvalet_codes_form_submit_add($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpvaletcodeadd';
}

function chpvalet_codes_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpvaletlist';
}

function chpvalet_codes_table_form($entries) {
  global $user;
  $form['db'] =
    array('#type' => 'markup', '#value' => '',
	  '#theme' => 'chpvalet_codes_table');
  if (user_access('anai chp edit valet codes')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }
  $looper = 0;
  foreach ($entries as $entry) {
    $form['db']['code_'.$looper] = array('#value' => $entry['Code']);
    $form['db']['description_'.$looper] = array('#value' => $entry['Description']);
    $form['db']['amount_'.$looper] = array('#value' => $entry['Amount']);

    if ($readonly) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_edit'.$entry['Id'],
	      '#submit' => array('chpvalet_codes_form_submit'));
    }
    $looper++;
  }
  return $form;
}

function theme_chpvalet_codes_table($form) {
  // Table Header
  $header = array('Code', 'Description', 'Amount', '');
  // Table Rows
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'code_')) {
      continue;
    }
    $looper = substr($key, strlen('code_'));
    $rows[] = array(drupal_render($form['code_'.$looper]),
		    drupal_render($form['description_'.$looper]),
		    drupal_render($form['amount_'.$looper]),
		    array('data' => drupal_render($form['edit_'.$looper]),
			  'width' => 1, 'height' => 1),
		    );
  }
  // Render Table
  return theme_table($header, $rows);
}

function chpvalet_codes_edit_form($form_state) {
  if ($form_state['storage']['confirm_save']) {
    return confirm_form($form, 'Are you sure?', 'chpvaletcodes',
			'The valet code will be active on all properties.');
  }
  if ($form_state['storage']['confirm_delete']) {
    return confirm_form($form, 'Are you sure?', 'chpvaletcodes',
       'The valet code will be deleted from the system on any property.');
  }

  drupal_set_title(t('Edit valet code'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $id = $form_state['storage']['id']; //drupal_set_message('Id:'.$id);
  $code = chdbvalet_get_code_id($id);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['show_companies'] = TRUE;
  $settings['show_properties'] = TRUE;
  $settings['show_all_properties'] = $code['Template'] > 0 ? TRUE : FALSE;
  $settings['default']['PropertyId'] = $code['PropertyId'];
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $property_id = $form_state['storage']['ajax']['PropertyId'];
  $lot_id = $form_state['storage']['ajax']['LotId'];

  if (!isset($form_state['storage']['ajax']['Code'])) {
    $form_state['storage']['ajax']['Code'] = $code['Code'];
  }
  $form['ajax']['Code'] =
    array('#type' => 'textfield',
	  '#title' => t('Code'),
	  '#maxlength' => 15,
	  '#default_value' => $form_state['storage']['ajax']['Code']);

  if (!isset($form_state['storage']['ajax']['Description'])) {
    $form_state['storage']['ajax']['Description'] = $code['Description'];
  }
  $form['ajax']['Description'] =
    array('#type' => 'textfield',
	  '#title' => t('Description'),
	  '#maxlength' => 80,
	  '#default_value' => $form_state['storage']['ajax']['Description']);

  if (!isset($form_state['storage']['ajax']['Amount'])) {
    $form_state['storage']['ajax']['Amount'] = $code['Amount'];
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
	  '#validate' => array('chpvalet_codes_edit_form_validate_save'),
	  '#submit' => array('chpvalet_codes_form_submit'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#validate' => array('chpvalet_codes_edit_form_validate_cancel'),
	  '#submit' => array('chpvalet_codes_form_submit'));

  if (user_access('anai chp delete valet codes')) {
    $form['ajax']['delete'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/delete.png',
  	    '#validate' => array('chpvalet_codes_edit_form_validate_delete'),
	    '#submit' => array('chpvalet_codes_form_submit'));
  }

  return $form;
}

function chpvalet_codes_edit_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['ajax']['Code'])) {
    form_set_error('ajax][Code', t('Code field is required.'));
    return;
  }
  if (empty($form_state['values']['ajax']['Description'])) {
    form_set_error('ajax][Description', t('Description field is required.'));
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

  // When updating, the code can stay the same
  // hence only checking when changed.
  $id = $form_state['storage']['id'];
  $code = chdbvalet_get_code_id($id);
  if ($code['Code'] != $form_state['values']['ajax']['Code']) {
    if (chdbvalet_code_exists
	($form_state['values']['ajax']['CompanyId'],
	 $form_state['values']['ajax']['all_properties'], // Template
	 check_plain($form_state['values']['ajax']['Code']),
	 $form_state['values']['ajax']['PropertyId'])) {
      form_set_error('ajax][Code', t('Code already in use.'));
      return;
    }
  }

  $form_state['storage']['ajax']['Code'] =
    $form_state['values']['ajax']['Code'];
  $form_state['storage']['ajax']['Description'] =
    $form_state['values']['ajax']['Description'];
  $form_state['storage']['ajax']['Amount'] =
    $form_state['values']['ajax']['Amount'];

  $form_state['storage']['ajax']['CompanyId'] =
    $form_state['values']['ajax']['CompanyId'];
  $form_state['storage']['ajax']['PropertyId'] =
    $form_state['values']['ajax']['PropertyId'];
  $form_state['storage']['ajax']['all_properties'] =
    $form_state['values']['ajax']['all_properties'];

  if ($form_state['values']['ajax']['all_properties']) {
    $form_state['storage']['confirm_cmd'] = TRUE;
  } else {
    $form_state['storage']['save'] = TRUE;
  }
}

function chpvalet_codes_edit_form_validate_cancel($form, &$form_state) {
  $form_state['storage']['cancel'] = TRUE;
}

function chpvalet_codes_edit_form_validate_delete($form, &$form_state) {
  $form_state['storage']['delete'] = TRUE;
}

function chpvalet_codes_form_submit($form, &$form_state) {
  if ($form_state['storage']['cancel']) {
    unset($form_state['storage']);
    $form_state['redirect'] = 'chpvaletcodes';
    return;
  }
  if ($form_state['storage']['confirm_delete']) {
    $id = $form_state['storage']['id'];
    if (chdbvalet_delete_code_id($id)) {
      drupal_set_message('Could not delete valet code', 'error');
      return;
    }
    unset($form_state['storage']);
    $form_state['redirect'] = 'chpvaletcodes';
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
    global $user;
    $id = $form_state['storage']['id'];
    $code = chdbvalet_get_code_id($id);
    $property_id = $form_state['storage']['ajax']['PropertyId'];

    if (chdbvalet_update_code
	($id,
	 $form_state['storage']['ajax']['all_properties'], // Template
	 $property_id,
	 check_plain($form_state['storage']['ajax']['Code']),
	 check_plain($form_state['storage']['ajax']['Description']),
	 check_plain($form_state['storage']['ajax']['Amount']))) {
      drupal_set_message('Could not update code', 'error');
      return;
    }

    unset($form_state['storage']);
    $form_state['redirect'] = 'chpvaletcodes';
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
 ********** Valet code add
 ********** Address: chpvaletcodeadd
 ********** Access:  'anai chp add valet code'
 *********/



function chpvalet_codes_add_form($form_state) {
  if ($form_state['storage']['confirm_save']) {
    return confirm_form($form, 'Are you sure?', 'chpvaletcodes',
			'The valet code will be active on all properties.');
  }

  drupal_set_title(t('Add valet code'));

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
  $settings['show_properties'] = TRUE;
  $settings['show_all_properties'] = FALSE; // Default value
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $property_id = $form_state['storage']['ajax']['PropertyId'];
  $lot_id = $form_state['storage']['ajax']['LotId'];

  if (!isset($form_state['storage']['ajax']['Code'])) {
    $form_state['storage']['ajax']['Code'] = '';
  }
  $form['ajax']['Code'] =
    array('#type' => 'textfield',
	  '#title' => t('Code'),
	  '#maxlength' => 15,
	  '#default_value' => $form_state['storage']['ajax']['Code']);

  if (!isset($form_state['storage']['ajax']['Description'])) {
    $form_state['storage']['ajax']['Description'] = '';
  }
  $form['ajax']['Description'] =
    array('#type' => 'textfield',
	  '#title' => t('Description'),
	  '#maxlength' => 80,
	  '#default_value' => $form_state['storage']['ajax']['Description']);

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
	  '#validate' => array('chpvalet_codes_add_form_validate_save'),
	  '#submit' => array('chpvalet_codes_add_form_submit'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpvalet_codes_add_form_submit_cancel'));
  return $form;
}

function chpvalet_codes_add_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['ajax']['Code'])) {
    form_set_error('ajax][Code', t('Code field is required.'));
    return;
  }
  if (empty($form_state['values']['ajax']['Description'])) {
    form_set_error('ajax][Description', t('Description field is required.'));
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

  if (chdbvalet_code_exists
      ($form_state['storage']['ajax']['CompanyId'],
       $form_state['storage']['ajax']['all_properties'], // Template
       check_plain($form_state['values']['ajax']['Code']),
       $form_state['storage']['ajax']['PropertyId'])) {
    form_set_error('ajax][Code', t('Code already in use.'));
    return;
  }

  $form_state['storage']['ajax']['Application'] =
    $form_state['values']['ajax']['Application'];
  $form_state['storage']['ajax']['Code'] =
    $form_state['values']['ajax']['Code'];
  $form_state['storage']['ajax']['Description'] =
    $form_state['values']['ajax']['Description'];
  $form_state['storage']['ajax']['Amount'] =
    $form_state['values']['ajax']['Amount'];
  if ($form_state['values']['ajax']['Application'] === 'Multi') {
    $form_state['storage']['confirm_cmd'] = TRUE;
  } else {
    $form_state['storage']['save'] = TRUE;
  }
}

function chpvalet_codes_add_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpvaletcodes';
}

function chpvalet_codes_add_form_submit($form, &$form_state) {
  if ($form_state['storage']['confirm_cmd']) {
    unset($form_state['storage']['confirm_cmd']);
    $form_state['storage']['confirm_save'] = TRUE;
    $form_state['storage']['save'] = TRUE;
    return;
  }
  if ($form_state['storage']['save']) {
    $company_id = $form_state['storage']['ajax']['CompanyId'];
    $property_id = $form_state['storage']['ajax']['PropertyId'];

    // Since validation has made sure the Code is new,
    // then just insert without any concerns.
    if (chdbvalet_insert_new_code
	($company_id,
	 $form_state['storage']['ajax']['all_properties'], // Template
	 $property_id,
	 check_plain($form_state['storage']['ajax']['Code']),
	 check_plain($form_state['storage']['ajax']['Description']),
	 check_plain($form_state['storage']['ajax']['Amount']),
	 $id)) {
      drupal_set_message('Could not insert new code', 'error');
      return;
    }

    unset($form_state['storage']);
    $form_state['redirect'] = 'chpvaletcodes';
    return;
  }
}



