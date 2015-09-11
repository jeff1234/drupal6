<?php
// $Id$
require_once("anai_chpvalet.inc");



function chpvalet_prepared_form($form_state) {
  drupal_set_title(t('Prepared for pickup'));

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
  $settings['buttons']['back']['validate'] = 'chpvalet_prepared_form_validate_back';
  $settings['buttons']['back']['submit'] = 'chpvalet_prepared_form_submit';
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $property_id = $form_state['storage']['ajax']['PropertyId'];
  $lot_id = $form_state['storage']['ajax']['LotId'];

  $valets = chdbvalet_get_valets($company_id, $property_id, $lot_id, TRUE,
				 TRUE, 'Prepare');

  if (sizeof($valets) == 0) {
    $form['ajax']['empty'] =
      array('#value' => '<p><p>'.t('Empty. No prepared valets registered.').'<p>');
    return $form;
  }

  $form['ajax']['list'] = chpvalet_prepared_table_form($valets);

  return $form;
}

function chpvalet_prepared_form_validate_back($form, &$form_state) {
  $form_state['storage']['back'] = TRUE;
}

function chpvalet_prepared_table_form($entries) {
  global $user;
  $form['db'] =
    array('#type' => 'markup', '#value' => '',
	  '#theme' => 'chpvalet_prepared_table');
  $looper = 0;
  foreach ($entries as $entry) {
    $checkin = chpuser_datetime_utc_to_usertimezone($entry['CheckinTime']);
    $form['db']['checkin_'.$looper] = array('#value' => $checkin->format("M j,y H:i"));

    $prepared = chpuser_datetime_utc_to_usertimezone($entry['PrepareTime']);
    $form['db']['prepared_'.$looper] = array('#value' => $prepared->format("M j,y H:i"));

    $form['db']['lpn_'.$looper] = array('#value' => $entry['LPN']);
    $form['db']['make_'.$looper] = array('#value' => $entry['Make']);
    $form['db']['model_'.$looper] = array('#value' => $entry['Model']);
    $form['db']['color_'.$looper] = array('#value' => $entry['Color']);

    $looper++;
  }
  return $form;
}

function theme_chpvalet_prepared_table($form) {
  // Table Header
  $header = array('Checked in', 'Prepared', 'LPN', 'Make', 'Model', 'Color');
  // Table Rows
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'checkin_')) {
      continue;
    }
    $looper = substr($key, strlen('checkin_'));
    $rows[] = array(drupal_render($form['checkin_'.$looper]),
		    drupal_render($form['prepared_'.$looper]),
		    drupal_render($form['lpn_'.$looper]),
		    drupal_render($form['make_'.$looper]),
		    drupal_render($form['model_'.$looper]),
		    drupal_render($form['color_'.$looper]),
		    );
  }
  // Render Table
  return theme_table($header, $rows);
}

function chpvalet_prepared_form_submit($form, &$form_state) {
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
}



function chpvalet_pickup_form($form_state) {
  drupal_set_title(t('Temporary picked up vehicles'));

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
  $settings['buttons']['back']['validate'] = 'chpvalet_pickup_form_validate_back';
  $settings['buttons']['back']['submit'] = 'chpvalet_pickup_form_submit';
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $property_id = $form_state['storage']['ajax']['PropertyId'];
  $lot_id = $form_state['storage']['ajax']['LotId'];

  $valets = chdbvalet_get_valets($company_id, $property_id, $lot_id, TRUE,
				 TRUE, 'PickUp');

  if (sizeof($valets) == 0) {
    $form['ajax']['empty'] =
      array('#value' => '<p><p>'.t('Empty. No temporary picked up vehicles registered.').'<p>');
    return $form;
  }

  $form['ajax']['list'] = chpvalet_pickup_table_form($valets);

  return $form;
}

function chpvalet_pickup_form_validate_back($form, &$form_state) {
  $form_state['storage']['back'] = TRUE;
}

function chpvalet_pickup_table_form($entries) {
  global $user;
  $form['db'] =
    array('#type' => 'markup', '#value' => '',
	  '#theme' => 'chpvalet_pickup_table');
  $looper = 0;
  foreach ($entries as $entry) {
    $checkin = chpuser_datetime_utc_to_usertimezone($entry['CheckinTime']);
    $form['db']['checkin_'.$looper] = array('#value' => $checkin->format("M j,y H:i"));

    $pickup = chpuser_datetime_utc_to_usertimezone($entry['PickupTime']);
    $form['db']['pickup_'.$looper] = array('#value' => $pickup->format("M j,y H:i"));

    $form['db']['lpn_'.$looper] = array('#value' => $entry['LPN']);
    $form['db']['make_'.$looper] = array('#value' => $entry['Make']);
    $form['db']['model_'.$looper] = array('#value' => $entry['Model']);
    $form['db']['color_'.$looper] = array('#value' => $entry['Color']);

    $looper++;
  }
  return $form;
}

function theme_chpvalet_pickup_table($form) {
  // Table Header
  $header = array('Checked in', 'Picked up', 'LPN', 'Make', 'Model', 'Color');
  // Table Rows
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'checkin_')) {
      continue;
    }
    $looper = substr($key, strlen('checkin_'));
    $rows[] = array(drupal_render($form['checkin_'.$looper]),
		    drupal_render($form['pickup_'.$looper]),
		    drupal_render($form['lpn_'.$looper]),
		    drupal_render($form['make_'.$looper]),
		    drupal_render($form['model_'.$looper]),
		    drupal_render($form['color_'.$looper]),
		    );
  }
  // Render Table
  return theme_table($header, $rows);
}

function chpvalet_pickup_form_submit($form, &$form_state) {
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
}



