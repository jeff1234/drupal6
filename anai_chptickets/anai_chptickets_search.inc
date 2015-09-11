<?php
// $Id$
require_once("anai_chptickets.inc");



function chptick_search_form($form_state) {
  drupal_set_title(t('Search tickets'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['buttons']['back'] =
    array('title' => t('Back'),
	  'validate' => 'chptick_search_form_validate_back',
	  'submit' => 'chptick_search_form_submit');
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $_SESSION['TemporaryCompanyId'] = $form_state['storage']['property']['CompanyId'];
  $_SESSION['TemporaryPropertyId'] = $form_state['storage']['property']['PropertyId'];
  $_SESSION['TemporaryLotId'] = $form_state['storage']['property']['LotId'];
  $company_id = $form_state['storage']['property']['CompanyId'];
  $property_id = $form_state['storage']['property']['PropertyId'];
  $lot_id = $form_state['storage']['property']['LotId'];

  if (!isset($form_state['storage']['property']['LPN'])) {
    $form_state['storage']['property']['LPN'] = '';
  }
  $form['property']['LPN'] =
    array('#type' => 'textfield',
	  '#title' => t('License plate number'),
	  '#maxlength' => 10,
	  '#description' => t('(No spaces)'),
	  '#default_value' => $form_state['storage']['property']['LPN'],
	  '#attributes' => array('onBlur'=>'this.value=this.value.toUpperCase()'));

  $form['property']['search'] =
    array('#type' => 'submit',
	  '#value' => t('Search'),
	  '#validate' => array('chptick_search_form_validate_search'),
	  '#submit' => array('chptick_search_form_submit'));

  $entries = chdbtick_get_tickets_lpn
    ($company_id,
     $form_state['storage']['property']['LPN'],
     'Any',
     FALSE, TRUE,
     TRUE, TRUE);

  if (sizeof($entries) > 0) {
    $form['property']['list'] = chptick_search_table_form($entries);
  } elseif (strlen($form_state['storage']['property']['LPN']) > 0) {
    $form['property']['empty'] = array('#value' => '<p><p>'.t('Not found.'));
  }

  return $form;
}

function chptick_search_form_validate_back($form, &$form_state) {
  $form_state['storage']['back'] = TRUE;
}

function chptick_search_form_validate_search($form, &$form_state) {
  $form_state['storage']['search'] = TRUE;
}

function chptick_search_table_form($entries) {
  global $user;
  $form['db'] =
    array('#type' => 'markup', '#value' => '',
	  '#theme' => 'chptick_search_table');
  $properties = array();
  $looper = 0;
  foreach ($entries as $entry) {
    $form['db']['lpn_'.$looper] = array('#value' => $entry['LPN']);
    $form['db']['ticket_'.$looper] = array('#value' => $entry['TicketId']);
    $time = chpuser_datetime_utc_to_usertimezone($entry['CreateTime']);
    $form['db']['created_'.$looper] = array('#value' => $time->format("M j,y H:i"));
    if (!$properties[$entry['PropertyId']]) {
      $properties[$entry['PropertyId']] = chdbprop_get_property($entry['PropertyId']);
    }
    $property = $properties[$entry['PropertyId']];
    $str =
      $property['Alias'].',<br>'.
      $property['Street1'].',<br>'.
      $property['City'];
    $form['db']['prop_'.$looper] = array('#value' => $str);
    $str = $entry['Code'].'<br>'.decode_entities($entry['Description']);
    $form['db']['violation_'.$looper] = array('#value' => $str);
    $str = $entry['Amount'];
    $form['db']['amount_'.$looper] = array('#value' => $str);
    if (strlen($entry['PaidTime']) > 0) {
      $time = chpuser_datetime_utc_to_usertimezone($entry['PaidTime']);
      $form['db']['paid_'.$looper] = array('#value' => $time->format("M j,y H:i"));
    } else {
      $form['db']['paid_'.$looper] = array('#value' => t('Not yet'));
    }
    $form['db']['resolution_'.$looper] =
      array('#value' => decode_entities($entry['Resolution']));
    $looper++;
  }
  return $form;
}

function theme_chptick_search_table($form) {
  // Table Header
  $header = array('LPN', 'Ticket', 'Created', 'Property', 'Violation', 'Amount', 'Paid', 'Note');
  // Table Rows
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'lpn_')) {
      continue;
    }
    $looper = substr($key, strlen('lpn_'));
    $rows[] = array(drupal_render($form['lpn_'.$looper]),
		    drupal_render($form['ticket_'.$looper]),
		    drupal_render($form['created_'.$looper]),
		    drupal_render($form['prop_'.$looper]),
		    drupal_render($form['violation_'.$looper]),
		    drupal_render($form['amount_'.$looper]),
		    drupal_render($form['paid_'.$looper]),
		    drupal_render($form['resolution_'.$looper]));
  }
  // Render Table
  return theme_table($header, $rows);
}

function chptick_search_form_submit($form, &$form_state) {
  if ($form_state['storage']['back']) {
    unset($form_state['storage']);
    $form_state['redirect'] = 'chpticklist';
    return;
  }
}



