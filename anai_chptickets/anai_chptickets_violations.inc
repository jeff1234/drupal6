<?php
// $Id$
require_once("anai_chptickets.inc");



function chptick_violations_form($form_state) {
  if ($form_state['storage']['detailed_edit']) {
    return chptick_violations_edit_form($form_state);
  }

  drupal_set_title(t('Violations'));

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
  $settings['buttons']['back'] =
    array('title' => t('Back'),
	  'submit' => 'chptick_violations_form_submit_back');
  if (user_access('anai chp add ticket violations')) {
    $settings['buttons']['add'] =
      array('title' => t('Add violation'),
	    'submit' => 'chptick_violations_form_submit_add');
  }
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $property_id = $form_state['storage']['ajax']['PropertyId'];
  $lot_id = $form_state['storage']['ajax']['LotId'];

  $policy = $form_state['storage']['ajax']['Lots'][$property_id][$lot_id];
  $enforcement_id = $policy['data']['EnforcementId'];

  if (empty($enforcement_id)) {
    $form['ajax']['error'] = array('#value' => '<p>'.t('The lot is not enforced.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chptick_violations_form_submit_cancel'));
    return $form;
  }

  // Create enforcer options
  $options = array();
  foreach (chpprop_retrieve_enforcers($user->uid, $company_id, $property_id, $lot_id)
	   as $enforcer_id => $enforcer) {
    $options[$enforcer_id] =
      decode_entities($enforcer['data']['Alias']).', '.
      decode_entities($enforcer['data']['Street']).', '.
      decode_entities($enforcer['data']['City']);
  }

  if (empty($options)) {
    $form['ajax']['error'] =
      array('#value' => t('Enforcers yet to be configured on lot, impossible to continue.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chptick_add_form_submit_cancel'));
    return $form;
  }

  // Layout enforcer options
  if (!isset($form_state['storage']['ajax']['EnforcerId'])) {
    $form_state['storage']['ajax']['EnforcerId'] = key($options);
  }
  $form['ajax']['EnforcerId'] =
    array('#type' => 'select',
	  '#title' => t('Enforcer'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['EnforcerId'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  $ticketcodes = array();
  foreach (chdbprop_get_ticketcodes
	   ($company_id, $form_state['storage']['ajax']['EnforcerId'],
	    FALSE, // Template
	    $property_id, $lot_id) as $ticketcode) {
    if (!isset($ticketcode[$ticketcode['Code']])) {
      $ticketcodes[$ticketcode['Code']] = $ticketcode;
    }
  }
  foreach (chdbprop_get_ticketcodes
	   ($company_id, $form_state['storage']['ajax']['EnforcerId'],
	    TRUE, // Template
	    NULL, NULL) as $ticketcode) {
    if (!isset($ticketcode[$ticketcode['Code']])) {
      $ticketcodes[$ticketcode['Code']] = $ticketcode;
    }
  }

  if (empty($ticketcodes)) {
    $form['ajax']['empty'] = array('#value' => '<p>'.t('Empty.').'<p>');
  } else {
    ksort($ticketcodes);
    $form['ajax']['list'] = chptick_violations_table_form($ticketcodes);
  }
  return $form;
}

function chptick_violations_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpticklist';
}

function chptick_violations_form_submit_add($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chptickvioadd';
}

function chptick_violations_table_form($entries) {
  global $user;
  $form['db'] =
    array('#type' => 'markup', '#value' => '',
	  '#theme' => 'chptick_violations_table');
  if (user_access('anai chp edit ticket violations')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }
  $looper = 0;
  foreach ($entries as $entry) {
    $form['db']['code_'.$looper] = array('#value' => decode_entities($entry['Code']));
    $form['db']['description_'.$looper] = array('#value' => decode_entities($entry['Description']));
    $form['db']['amount_'.$looper] = array('#value' => $entry['Amount']);

    if ($readonly) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_edit'.$entry['Id'],
	      '#submit' => array('chptick_violations_form_submit'));
    }
    $looper++;
  }
  return $form;
}

function theme_chptick_violations_table($form) {
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

function chptick_violations_edit_form($form_state) {
  if (isset($form_state['storage']['confirm_delete'])) {
    return confirm_form
      ($form, 'Are you sure?', 'chptickviolist',
       'The violation will be deleted from the system.');
  }

  drupal_set_title(t('Edit violation'));

  global $user;
  $id = $form_state['storage']['id'];
  $ticketcode = chdbprop_get_ticketcode_id($id);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  if (!isset($form_state['storage']['ajax']['Code'])) {
    $form_state['storage']['ajax']['Code'] = $ticketcode['Code'];
  }
  $form['ajax']['Code'] =
    array('#type' => 'textfield',
	  '#title' => t('Code'),
	  '#maxlength' => 15,
	  '#default_value' => $form_state['storage']['ajax']['Code']);

  if (!isset($form_state['storage']['ajax']['Description'])) {
    $form_state['storage']['ajax']['Description'] = $ticketcode['Description'];
  }
  $form['ajax']['Description'] =
    array('#type' => 'textfield',
	  '#title' => t('Description'),
	  '#maxlength' => 80,
	  '#default_value' => $form_state['storage']['ajax']['Description']);

  if (!isset($form_state['storage']['ajax']['Amount'])) {
    $form_state['storage']['ajax']['Amount'] = $ticketcode['Amount'];
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
	  '#validate' => array('chptick_violations_edit_form_validate_save'),
	  '#submit' => array('chptick_violations_edit_form_submit_save'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chptick_violations_edit_form_submit_cancel'));

  if (user_access('anai chp delete ticket violations')) {
    $form['ajax']['delete'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/delete.png',
	    '#submit' => array('chptick_violations_edit_form_submit_delete'));
  }
  return $form;
}

function chptick_violations_edit_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chptickviolist';
}

function chptick_violations_edit_form_validate_save($form, &$form_state) {
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
  $ticketcode = chdbprop_get_ticketcode_id($id);
  if ($ticketcode['Code'] != $form_state['values']['ajax']['Code']) {
    if (chdbprop_ticketcode_exists
	($ticketcode['CompanyId'],
	 $ticketcode['EnforcerId'],
	 $ticketcode['Template'],
	 check_plain($form_state['values']['ajax']['Code']),
	 $ticketcode['PropertyId'],
	 $ticketcode['LotId'])) {
      form_set_error('ajax][Code', t('Code already in use.'));
      return;
    }
  }
}

function chptick_violations_edit_form_submit_delete($form, &$form_state) {
  $form_state['storage']['confirm_delete'] = TRUE;
}

function chptick_violations_edit_form_submit_save($form, &$form_state) {
  $id = $form_state['storage']['id'];
  $ticketcode = chdbprop_get_ticketcode_id($id);

  if (chdbprop_update_ticketcode
      ($id,
       $ticketcode['Template'],
       $ticketcode['PropertyId'],
       $ticketcode['LotId'],
       check_plain($form_state['values']['ajax']['Code']),
       check_plain($form_state['values']['ajax']['Description']),
       check_plain($form_state['values']['ajax']['Amount']))) {
    drupal_set_message('Could not update violation', 'error');
    return;
  }

  if (chdbprop_insert_agentpoll_ticketcode
      ($ticketcode['CompanyId'],
       $ticketcode['EnforcerId'])) {
    drupal_set_message('Could not insert violation poll', 'error');
    return;
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chptickviolist';
}

function chptick_violations_form_submit($form, &$form_state) {
  if ($form_state['storage']['confirm_delete']) {
    $id = $form_state['storage']['id'];
    if (chdbprop_delete_ticketcode_id($id)) {
      drupal_set_message('Could not delete violation', 'error');
      return;
    }
    unset($form_state['storage']);
    $form_state['redirect'] = 'chptickviolist';
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
    //drupal_set_message('Detailed edit of id:'.$id);
    $form_state['storage']['id'] = $id;
    $form_state['storage']['detailed_edit'] = TRUE;
    return;
  }
}



function chptick_violations_add_form($form_state) {
  drupal_set_title(t('Add violation'));

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
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $property_id = $form_state['storage']['ajax']['PropertyId'];
  $lot_id = $form_state['storage']['ajax']['LotId'];

  $policy = $form_state['storage']['ajax']['Lots'][$property_id][$lot_id];
  $enforcement_id = $policy['data']['EnforcementId'];

  if (empty($enforcement_id)) {
    $form['ajax']['error'] = array('#value' => '<p>'.t('The lot is not enforced.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chptick_printer_form_submit_cancel'));
    return $form;
  }

  // Create enforcer options
  $options = array();
  foreach (chpprop_retrieve_enforcers($user->uid, $company_id, $property_id, $lot_id)
	   as $enforcer_id => $enforcer) {
    $options[$enforcer_id] =
      decode_entities($enforcer['data']['Alias']).', '.
      decode_entities($enforcer['data']['Street']).', '.
      decode_entities($enforcer['data']['City']);
  }

  if (empty($options)) {
    $form['ajax']['error'] =
      array('#value' => t('Enforcers yet to be configured on lot, impossible to continue.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chptick_add_form_submit_cancel'));
    return $form;
  }

  // Layout enforcer options
  if (!isset($form_state['storage']['ajax']['EnforcerId'])) {
    $form_state['storage']['ajax']['EnforcerId'] = key($options);
  }
  $form['ajax']['EnforcerId'] =
    array('#type' => 'select',
	  '#title' => t('Enforcer'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['EnforcerId'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  $options = array();
  $options['Template'] = t('For all enforced lots on property');
  if (1 < count($form_state['storage']['ajax']['Lots'][$property_id])) {
    $options['Lot'] = t('Specific for selected lot');
  }

  if (!isset($form_state['storage']['ajax']['Type'])) {
    $form_state['storage']['ajax']['Type'] = key($options);
  }
  if (1 < count($options)) {
    $form['ajax']['Type'] =
      array('#type' => 'select',
	    '#title' => t('Type'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['Type'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
  } else {
    $form['ajax']['Type'] =
      array('#type' => 'hidden',
	    '#value' => $form_state['storage']['ajax']['Type']);
  }

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
	  '#validate' => array('chptick_violations_add_form_validate_save'),
	  '#submit' => array('chptick_violations_add_form_submit_save'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chptick_violations_add_form_submit_cancel'));

  return $form;
}

function chptick_violations_add_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chptickviolist';
}

function chptick_violations_add_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['ajax']['Code'])) {
    form_set_error('ajax][Code', t('Code field is required.'));
    return;
  }

  $property_id = $form_state['values']['ajax']['PropertyId'];
  $lot_id = $form_state['values']['ajax']['LotId'];

  if ($form_state['values']['ajax']['Type'] === 'Template') {
    if (chdbprop_ticketcode_exists
	($form_state['values']['ajax']['CompanyId'],
	 $form_state['values']['ajax']['EnforcerId'],
	 TRUE, // Template
	 check_plain($form_state['values']['ajax']['Code']),
	 NULL, NULL)) {
      form_set_error('ajax][Code', t('Code already in use.'));
      return;
    }
  } else {
    if (chdbprop_ticketcode_exists
	($form_state['values']['ajax']['CompanyId'],
	 $form_state['values']['ajax']['EnforcerId'],
	 FALSE, // Template
	 check_plain($form_state['values']['ajax']['Code']),
	 $property_id, $lot_id)) {
      form_set_error('ajax][Code', t('Code already in use.'));
      return;
    }
  }

  if (empty($form_state['values']['ajax']['Description'])) {
    form_set_error('ajax][Description', t('Description field is required.'));
    return;
  }
  if (empty($form_state['values']['ajax']['Amount'])) {
    form_set_error('ajax][Amount', t('Amount field is required.'));
    return;
  }
  if (!preg_match('/^\d+\.\d{2}$/', $form_state['values']['ajax']['Amount'])) {
    form_set_error('ajax][Amount', 'Amount must be given in format X.XX');
    return;
  }
}

function chptick_violations_add_form_submit_save($form, &$form_state) {
  global $user;
  $company_id = $form_state['values']['ajax']['CompanyId'];
  $property_id = $form_state['values']['ajax']['PropertyId'];
  $lot_id = $form_state['values']['ajax']['LotId'];
  $enforcer_id = $form_state['values']['ajax']['EnforcerId'];

  if ($form_state['values']['ajax']['Type'] === 'Template') {

    // Get the existing template
    $template = chdbprop_get_ticketcode
      ($company_id, $enforcer_id,
       TRUE,
       check_plain($form_state['values']['ajax']['Code']),
       NULL, NULL);

    if (empty($template)) {// Insert when not existing
      if (chdbprop_insert_ticketcode
	  ($company_id, $enforcer_id,
	   TRUE, // Template
	   NULL, NULL,
	   check_plain($form_state['values']['ajax']['Code']),
	   check_plain($form_state['values']['ajax']['Description']),
	   check_plain($form_state['values']['ajax']['Amount']),
	   $id)) {
	drupal_set_message('Could not insert violation', 'error');
	return;
      }
    } else {
      if (chdbprop_update_ticketcode
	  ($template['Id'],
	   TRUE, // Template
	   NULL, NULL,
	   check_plain($form_state['values']['ajax']['Code']),
	   check_plain($form_state['values']['ajax']['Description']),
	   check_plain($form_state['values']['ajax']['Amount']))) {
	drupal_set_message('Could not update violation', 'error');
	return;
      }
    }

    // Remove any old assigned per property
    $policies = chpprop_retrieve_policies($user->uid);
    foreach ($policies as $property_id => $values) {
      foreach ($values as $lot_id => $value) {
	$current = chdbprop_get_ticketcode
	  ($company_id, $enforcer_id,
	   FALSE, // Template
	   check_plain($form_state['values']['ajax']['Code']),
	   $property_id, $lot_id);
	if (!empty($current)) {
	  if (chdbprop_delete_ticketcode
	      ($company_id, $enforcer_id,
	       FALSE, // Template
	       check_plain($form_state['values']['ajax']['Code']),
	       $property_id, $lot_id)) {
	    drupal_set_message('Could not update multiple config', 'error');
	    return;
	  }
	}
      }
    }
  } else {
    $current = chdbprop_get_ticketcode
      ($company_id, $enforcer_id,
       FALSE, // Template
       check_plain($form_state['values']['ajax']['Code']),
       $property_id, $lot_id);

    if (empty($current)) {
      if (chdbprop_insert_ticketcode
	  ($company_id, $enforcer_id,
	   FALSE, // Template
	   $property_id, $lot_id,
	   check_plain($form_state['values']['ajax']['Code']),
	   check_plain($form_state['values']['ajax']['Description']),
	   check_plain($form_state['values']['ajax']['Amount']),
	   $id)) {
	drupal_set_message('Could not insert violation', 'error');
	return;
      }
    } else {
      if (chdbprop_update_ticketcode
	  ($template['Id'],
	   FALSE, // Template
	   $property_id, $lot_id,
	   check_plain($form_state['values']['ajax']['Code']),
	   check_plain($form_state['values']['ajax']['Description']),
	   check_plain($form_state['values']['ajax']['Amount']))) {
	drupal_set_message('Could not update violation', 'error');
	return;
      }
    }
  }

  if (chdbprop_insert_agentpoll_ticketcode
      ($company_id,
       $enforcer_id)) {
    drupal_set_message('Could not insert violation poll', 'error');
    return;
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chptickviolist';
}



