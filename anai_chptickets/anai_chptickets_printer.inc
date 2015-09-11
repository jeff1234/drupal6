<?php
// $Id$
require_once("anai_chptickets.inc");



function chptick_printer_form($form_state) {
  drupal_set_title(t('Printer configuration'));

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

  if ($form_state['storage']['ajax']['CompanyChanged'] or
      $form_state['storage']['ajax']['PropertyChanged'] or
      $form_state['storage']['ajax']['EnforcerId'] !== $form_state['storage']['ajax']['EnforcerIdOld']) {
    unset($form_state['storage']['ajax']['Header']);
    unset($form_state['storage']['ajax']['Company']);
    unset($form_state['storage']['ajax']['Addr1']);
    unset($form_state['storage']['ajax']['Addr2']);
    unset($form_state['storage']['ajax']['Addr3']);
    unset($form_state['storage']['ajax']['Addr4']);
    unset($form_state['storage']['ajax']['Addr5']);
    unset($form_state['storage']['ajax']['Disclaimer']);
  }
  $form['ajax']['EnforcerIdOld'] =
    array('#type' => 'hidden', '#value' => $form_state['storage']['ajax']['EnforcerId']);

  if ($form_state['storage']['ajax']['Type'] === 'Lot') {
    $printer = chdbprop_get_ticketprinter
      ($company_id, $form_state['storage']['ajax']['EnforcerId'],
       FALSE, // Template
       $property_id, $lot_id);
  } else {
    $printer = chdbprop_get_ticketprinter
      ($company_id, $form_state['storage']['ajax']['EnforcerId'],
       TRUE, // Template
       NULL, NULL);
  }

  if (!isset($form_state['storage']['ajax']['Header'])) {
    if (!empty($printer)) {
      $form_state['storage']['ajax']['Header'] =
	decode_entities($printer['Header']);
    } else {
      $form_state['storage']['ajax']['Header'] = '';
    }
  }
  $form['ajax']['Header'] =
    array('#type' => 'textfield',
	  '#title' => t('Header as printed on paper'),
	  '#maxlength' => 50,
	  '#default_value' => $form_state['storage']['ajax']['Header']);

  if (!isset($form_state['storage']['ajax']['Company'])) {
    if (!empty($printer)) {
      $form_state['storage']['ajax']['Company'] =
	decode_entities($printer['Company']);
    } else {
      $form_state['storage']['ajax']['Company'] = '';
    }
  }
  $form['ajax']['Company'] =
    array('#type' => 'textfield',
	  '#title' => t('Company name as printed on paper'),
	  '#maxlength' => 50,
	  '#default_value' => $form_state['storage']['ajax']['Company']);

  if (!isset($form_state['storage']['ajax']['Addr1'])) {
    if (!empty($printer)) {
      $form_state['storage']['ajax']['Addr1'] =
	decode_entities($printer['Addr1']);
    } else {
      $form_state['storage']['ajax']['Addr1'] = '';
    }
  }
  $form['ajax']['Addr1'] =
    array('#type' => 'textfield',
	  '#title' => t('1st address line as printed on paper'),
	  '#maxlength' => 50,
	  '#default_value' => $form_state['storage']['ajax']['Addr1']);

  if (!isset($form_state['storage']['ajax']['Addr2'])) {
    if (!empty($printer)) {
      $form_state['storage']['ajax']['Addr2'] =
	decode_entities($printer['Addr2']);
    } else {
      $form_state['storage']['ajax']['Addr2'] = '';
    }
  }
  $form['ajax']['Addr2'] =
    array('#type' => 'textfield',
	  '#title' => t('2nd address line as printed on paper'),
	  '#maxlength' => 50,
	  '#default_value' => $form_state['storage']['ajax']['Addr2']);

  if (!isset($form_state['storage']['ajax']['Addr3'])) {
    if (!empty($printer)) {
      $form_state['storage']['ajax']['Addr3'] =
	decode_entities($printer['Addr3']);
    } else {
      $form_state['storage']['ajax']['Addr3'] = '';
    }
  }
  $form['ajax']['Addr3'] =
    array('#type' => 'textfield',
	  '#title' => t('3rd address line as printed on paper'),
	  '#maxlength' => 50,
	  '#default_value' => $form_state['storage']['ajax']['Addr3']);

  if (!isset($form_state['storage']['ajax']['Addr4'])) {
    if (!empty($printer)) {
      $form_state['storage']['ajax']['Addr4'] =
	decode_entities($printer['Addr4']);
    } else {
      $form_state['storage']['ajax']['Addr4'] = '';
    }
  }
  $form['ajax']['Addr4'] =
    array('#type' => 'textfield',
	  '#title' => t('4th address line as printed on paper'),
	  '#maxlength' => 50,
	  '#default_value' => $form_state['storage']['ajax']['Addr4']);

  if (!isset($form_state['storage']['ajax']['Addr5'])) {
    if (!empty($printer)) {
      $form_state['storage']['ajax']['Addr5'] =
	decode_entities($printer['Addr5']);
    } else {
      $form_state['storage']['ajax']['Addr5'] = '';
    }
  }
  $form['ajax']['Addr5'] =
    array('#type' => 'textfield',
	  '#title' => t('5th address line as printed on paper'),
	  '#maxlength' => 50,
	  '#default_value' => $form_state['storage']['ajax']['Addr5']);

  if (!isset($form_state['storage']['ajax']['Disclaimer'])) {
    if (!empty($printer)) {
      $form_state['storage']['ajax']['Disclaimer'] =
	decode_entities($printer['Disclaimer']);
    } else {
      $form_state['storage']['ajax']['Disclaimer'] = '';
    }
  }
  $form['ajax']['Disclaimer'] =
    array('#type' => 'textarea',
	  '#title' => t('Disclaimer as printed on paper'),
	  '#maxlength' => 1024,
	  '#default_value' => $form_state['storage']['ajax']['Disclaimer']);

  $form['ajax']['submit'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chptick_printer_form_validate_save'),
	  '#submit' => array('chptick_printer_form_submit_save'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chptick_printer_form_submit_cancel'));
  return $form;
}

function chptick_printer_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpticklist';
}

function chptick_printer_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['ajax']['Header'])) {
    form_set_error('property][Header', t('Header field is required.'));
    return;
  }
  if (strpos($form_state['values']['ajax']['Header'], '<') !== FALSE or
      strpos($form_state['values']['ajax']['Header'], ';') !== FALSE or
      strpos($form_state['values']['ajax']['Header'], '>') !== FALSE) {
    form_set_error('property][Password',
		   t('Sorry, Header can\'t include < or > or ; characters.'));
    return;
  }
  if (empty($form_state['values']['ajax']['Company'])) {
    form_set_error('property][Company', t('Company field is required.'));
    return;
  }
  if (strpos($form_state['values']['ajax']['Company'], '<') !== FALSE or
      strpos($form_state['values']['ajax']['Company'], ';') !== FALSE or
      strpos($form_state['values']['ajax']['Company'], '>') !== FALSE) {
    form_set_error('property][Company', t('Sorry, Company can\'t include < or > or ; characters.'));
    return;
  }
  if (empty($form_state['values']['ajax']['Addr1'])) {
    form_set_error('property][Addr1', t('1st address line field is required.'));
    return;
  }
  if (strpos($form_state['values']['ajax']['Addr1'], '<') !== FALSE or
      strpos($form_state['values']['ajax']['Addr1'], ';') !== FALSE or
      strpos($form_state['values']['ajax']['Addr1'], '>') !== FALSE) {
    form_set_error('property][Addr1',
		   t('Sorry, 1st address line can\'t include < or > or ; characters.'));
    return;
  }
  if (!empty($form_state['values']['ajax']['Addr2'])) {
    if (strpos($form_state['values']['ajax']['Addr2'], '<') !== FALSE or
	strpos($form_state['values']['ajax']['Addr2'], ';') !== FALSE or
	strpos($form_state['values']['ajax']['Addr2'], '>') !== FALSE) {
      form_set_error('property][Addr2',
		     t('Sorry, 2nd address line can\'t include < or > or ; characters.'));
      return;
    }
  }
  if (!empty($form_state['values']['ajax']['Addr3'])) {
    if (strpos($form_state['values']['ajax']['Addr3'], '<') !== FALSE or
	strpos($form_state['values']['ajax']['Addr3'], ';') !== FALSE or
	strpos($form_state['values']['ajax']['Addr3'], '>') !== FALSE) {
      form_set_error('property][Addr3',
		     t('Sorry, 3rd address line can\'t include < or > or ; characters.'));
      return;
    }
  }
  if (!empty($form_state['values']['ajax']['Addr4'])) {
    if (strpos($form_state['values']['ajax']['Addr4'], '<') !== FALSE or
	strpos($form_state['values']['ajax']['Addr4'], ';') !== FALSE or
	strpos($form_state['values']['ajax']['Addr4'], '>') !== FALSE) {
      form_set_error('property][Addr4',
		     t('Sorry, 4th address line can\'t include < or > or ; characters.'));
      return;
    }
  }
  if (!empty($form_state['values']['ajax']['Addr5'])) {
    if (strpos($form_state['values']['ajax']['Addr5'], '<') !== FALSE or
	strpos($form_state['values']['ajax']['Addr5'], ';') !== FALSE or
	strpos($form_state['values']['ajax']['Addr5'], '>') !== FALSE) {
      form_set_error('property][Addr5',
		     t('Sorry, 5th address line can\'t include < or > or ; characters.'));
      return;
    }
  }
  if (empty($form_state['values']['ajax']['Disclaimer'])) {
    form_set_error('property][Disclaimer', t('Disclaimer field is required.'));
    return;
  }
  if (strpos($form_state['values']['ajax']['Disclaimer'], '<') !== FALSE or
      strpos($form_state['values']['ajax']['Disclaimer'], ';') !== FALSE or
      strpos($form_state['values']['ajax']['Disclaimer'], '>') !== FALSE) {
    form_set_error('property][Disclaimer',
		   t('Sorry, Disclaimer can\'t include < or > or ; characters.'));
    return;
  }
}

function chptick_printer_form_submit_save($form, &$form_state) {
  global $user;
  $company_id = $form_state['values']['ajax']['CompanyId'];
  $property_id = $form_state['values']['ajax']['PropertyId'];
  $lot_id = $form_state['values']['ajax']['LotId'];
  $enforcer_id = $form_state['values']['ajax']['EnforcerId'];

  if ($form_state['values']['ajax']['Type'] === 'Template') {

    // Get the existing template
    $template = chdbprop_get_ticketprinter
      ($company_id, $enforcer_id,
       TRUE, // Template
       NULL, NULL);

    if (empty($template)) { // Insert when not existing
      if (chdbprop_insert_ticketprinter
	  ($company_id, $enforcer_id,
	   TRUE, // Template
	   NULL, NULL,
	   check_plain($form_state['values']['ajax']['Header']),
	   check_plain($form_state['values']['ajax']['Company']),
	   check_plain($form_state['values']['ajax']['Addr1']),
	   check_plain($form_state['values']['ajax']['Addr2']),
	   check_plain($form_state['values']['ajax']['Addr3']),
	   check_plain($form_state['values']['ajax']['Addr4']),
	   check_plain($form_state['values']['ajax']['Addr5']),
	   check_plain($form_state['values']['ajax']['Disclaimer']),
	   $id)) {
	drupal_set_message('Could not insert template config', 'error');
	return;
      }
    } else { // Update the existing template
      if (chdbprop_update_ticketprinter
	  ($template['Id'],
	   TRUE, // Template
	   NULL, NULL,
	   check_plain($form_state['values']['ajax']['Header']),
	   check_plain($form_state['values']['ajax']['Company']),
	   check_plain($form_state['values']['ajax']['Addr1']),
	   check_plain($form_state['values']['ajax']['Addr2']),
	   check_plain($form_state['values']['ajax']['Addr3']),
	   check_plain($form_state['values']['ajax']['Addr4']),
	   check_plain($form_state['values']['ajax']['Addr5']),
	   check_plain($form_state['values']['ajax']['Disclaimer']))) {
	drupal_set_message('Could not update template config', 'error');
	return;
      }
    }
    // Remove any old assigned per property
    $policies = chpprop_retrieve_policies($user->uid);
    foreach ($policies as $property_id => $values) {
      foreach ($values as $lot_id => $value) {
	$current = chdbprop_get_ticketprinter
	  ($company_id, $enforcer_id,
	   FALSE, // Template
	   $property_id, $lot_id);
	if (!empty($current)) {
	  if (chdbprop_delete_ticketprinter
	      ($company_id, $enforcer_id,
	       FALSE, // Template
	       $property_id, $lot_id)) {
	    drupal_set_message('Could not update multiple config', 'error');
	    return;
	  }
	}
      }
    }
  } else {
    $current = chdbprop_get_ticketprinter
      ($company_id, $enforcer_id,
       FALSE, // Template
       $property_id, $lot_id);

    if (sizeof($current) == 0) {
      if (chdbprop_insert_ticketprinter
	  ($company_id, $enforcer_id,
	   FALSE, // Template
	   $property_id, $lot_id,
	   check_plain($form_state['values']['ajax']['Header']),
	   check_plain($form_state['values']['ajax']['Company']),
	   check_plain($form_state['values']['ajax']['Addr1']),
	   check_plain($form_state['values']['ajax']['Addr2']),
	   check_plain($form_state['values']['ajax']['Addr3']),
	   check_plain($form_state['values']['ajax']['Addr4']),
	   check_plain($form_state['values']['ajax']['Addr5']),
	   check_plain($form_state['values']['ajax']['Disclaimer']),
	   $id)) {
	drupal_set_message('Could not insert single config', 'error');
	return;
      }
    } else {
      if (chdbprop_update_ticketprinter
	  ($current['Id'],
	   FALSE, // Template
	   $property_id, $lot_id,
	   check_plain($form_state['values']['ajax']['Header']),
	   check_plain($form_state['values']['ajax']['Company']),
	   check_plain($form_state['values']['ajax']['Addr1']),
	   check_plain($form_state['values']['ajax']['Addr2']),
	   check_plain($form_state['values']['ajax']['Addr3']),
	   check_plain($form_state['values']['ajax']['Addr4']),
	   check_plain($form_state['values']['ajax']['Addr5']),
	   check_plain($form_state['values']['ajax']['Disclaimer']))) {
	drupal_set_message('Could not update single config', 'error');
	return;
      }
    }
  }

  if (chdbprop_insert_agentpoll_ticketprinter
      ($company_id,
       $enforcer_id)) {
    drupal_set_message('Could not insert violation poll', 'error');
    return;
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chpticklist';
}

