<?php
// $Id$
require_once("anai_chpvalet.inc");



function chpvalet_printer_form($form_state) {
  drupal_set_title(t('Printer configuration'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['show_companies'] = TRUE;
  $settings['show_properties'] = TRUE;
  $settings['show_all_properties'] = FALSE; // Default value
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $_SESSION['TemporaryCompanyId'] = $form_state['storage']['property']['CompanyId'];
  $_SESSION['TemporaryPropertyId'] = $form_state['storage']['property']['PropertyId'];
  $_SESSION['TemporaryLotId'] = $form_state['storage']['property']['LotId'];
  $company_id = $form_state['storage']['property']['CompanyId'];
  $property_id = $form_state['storage']['property']['PropertyId'];
  $lot_id = $form_state['storage']['property']['LotId'];

  if ($form_state['storage']['property']['CompanyChanged'] or
      $form_state['storage']['property']['PropertyChanged']) {
    unset($form_state['storage']['property']['Header']);
  }

  $config = chdbvalet_get_printer_config
    ($company_id,
     FALSE, // Template
     $property_id);
  if (sizeof($config) == 0) {
    $config = chdbvalet_get_printer_config
      ($company_id,
       TRUE, // Template
       FALSE);
  }

  if (!isset($form_state['storage']['property']['Header'])) {
    $form_state['storage']['property']['Header'] =
      decode_entities($config['Header']);
  }
  $form['property']['Header'] =
    array('#type' => 'textfield',
	  '#title' => t('Header as printed on paper'),
	  '#maxlength' => 50,
	  '#default_value' => $form_state['storage']['property']['Header']);

  $form['property']['submit'] =
    array('#type' => 'submit', '#value' => t('Save'),
	  '#validate' => array('chpvalet_printer_form_validate_save'),
	  '#submit' => array('chpvalet_printer_form_submit'));

  $form['property']['back'] =
    array('#type' => 'submit', '#value' => t('Cancel'),
	  '#validate' => array('chpvalet_printer_form_validate_back'),
	  '#submit' => array('chpvalet_printer_form_submit'));
  return $form;
}

function chpvalet_printer_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['property']['Header'])) {
    form_set_error('property][Header', t('Header field is required.'));
    return;
  }
  if (strpos($form_state['values']['property']['Header'], '<') !== FALSE or
      strpos($form_state['values']['property']['Header'], ';') !== FALSE or
      strpos($form_state['values']['property']['Header'], '>') !== FALSE) {
    form_set_error('property][Password',
		   t('Sorry, Header can\'t include < or > or ; characters.'));
    return;
  }
  $form_state['storage']['save'] = TRUE;
}

function chpvalet_printer_form_validate_back($form, &$form_state) {
  $form_state['storage']['back'] = TRUE;
}

function chpvalet_printer_form_submit($form, &$form_state) {
  if ($form_state['storage']['back']) {
    unset($form_state['storage']);
    $form_state['redirect'] = 'chpvaletlist';
    return;
  }

  if ($form_state['storage']['save']) {
    global $user;
    $company_id = $form_state['storage']['property']['CompanyId'];
    $property_id = $form_state['storage']['property']['PropertyId'];
    $lot_id = $form_state['storage']['property']['LotId'];

    if ($form_state['storage']['property']['all_properties']) {

      // Get the existing template
      $template = chdbvalet_get_printer_config
	($company_id,
	 TRUE, // Template
	 FALSE);

      if (sizeof($template) == 0) { // Insert when not existing
	if (chdbvalet_insert_printer_config
	    ($company_id,
	     TRUE, // Template
	     FALSE, // PropertyId
	     check_plain($form_state['values']['property']['Header']),
	     $id)) {
	  drupal_set_message('Could not insert template config', 'error');
	  return;
	}
      } else { // Update the existing template
	if (chdbvalet_update_printer_config
	    ($template['Id'],
	     TRUE, // Template
	     FALSE, // PropertyId
	     check_plain($form_state['values']['property']['Header']))) {
	  drupal_set_message('Could not update template config', 'error');
	  return;
	}
      }
      // Remove any old assigned per property
      $properties = $form_state['storage']['property']['Properties'];
      foreach ($properties as $property_id => $property) {
	$current = chdbvalet_get_printer_config
	  ($company_id,
	   FALSE, // Template
	   $property_id);
	if (sizeof($current) > 0) {
	  if (chdbvalet_delete_printer_config
	      ($company_id,
	       FALSE, // Template
	       $property_id)) {
	    drupal_set_message('Could not update multiple config', 'error');
	    return;
	  }
	}
      }
    } else {
      $current = chdbvalet_get_printer_config
	($company_id,
	 FALSE, // Template
	 $property_id);

      if (sizeof($current) == 0) {
	if (chdbvalet_insert_printer_config
	    ($company_id,
	     FALSE, // Template
	     $property_id,
	     check_plain($form_state['values']['property']['Header']),
	     $id)) {
	  drupal_set_message('Could not insert single printer', 'error');
	  return;
	}
      } else {
	if (chdbvalet_update_printer_config
	    ($current['Id'],
	     FALSE, // Template
	     $property_id,
	     check_plain($form_state['values']['property']['Header']))) {
	  drupal_set_message('Could not update single printer', 'error');
	  return;
	}
      }
    }

    unset($form_state['storage']);
    $form_state['redirect'] = 'chpvaletlist';
    return;
  }
}



