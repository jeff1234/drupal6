<?php
// $Id$
require_once("anai_chpvalet.inc");



function chpvalet_announced_form($form_state) {
  drupal_set_title(t('Announced arrivals via Messenger'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['buttons']['back']['title'] = t('Back');
  $settings['buttons']['back']['validate'] = 'chpvalet_announced_form_validate_back';
  $settings['buttons']['back']['submit'] = 'chpvalet_announced_form_submit';
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['property']['CompanyId'];
  $property_id = $form_state['storage']['property']['PropertyId'];

  $valets = chdbvalet_get_valets($company_id, $property_id, $lot_id, FALSE,
				 TRUE, 'Announce');

  if (sizeof($valets) == 0) {
    $form['property']['empty'] =
      array('#value' => '<p><p>'.t('Empty. No announced valets registered.').'<p>');
    return $form;
  }

  $form['property']['list'] = chpvalet_announced_table_form($valets);

  return $form;
}

function chpvalet_announced_form_validate_back($form, &$form_state) {
  $form_state['storage']['back'] = TRUE;
}

function chpvalet_announced_table_form($entries) {
  global $user;
  $form['db'] =
    array('#type' => 'markup', '#value' => '',
	  '#theme' => 'chpvalet_announced_table');
  $looper = 0;
  foreach ($entries as $entry) {
    $checkin = chpuser_datetime_utc_to_usertimezone($entry['CheckinTime']);
    $form['db']['checkin_'.$looper] = array('#value' => $checkin->format("M j,y H:i"));

    $announced = chpuser_datetime_utc_to_usertimezone($entry['AnnounceTime']);
    $form['db']['announced_'.$looper] = array('#value' => $announced->format("M j,y H:i"));

    $form['db']['patroller_'.$looper] = array('#value' => $entry['PatrollerId']);

    $form['db']['lpn_'.$looper] = array('#value' => $entry['LPN']);
    $form['db']['make_'.$looper] = array('#value' => $entry['Make']);
    $form['db']['model_'.$looper] = array('#value' => $entry['Model']);
    $form['db']['color_'.$looper] = array('#value' => $entry['Color']);

    $looper++;
  }
  return $form;
}

function theme_chpvalet_announced_table($form) {
  // Table Header
  $header = array('Checked in', 'Announced', 'P\'ler', 'LPN', 'Make', 'Model', 'Color');
  // Table Rows
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'checkin_')) {
      continue;
    }
    $looper = substr($key, strlen('checkin_'));
    $rows[] = array(drupal_render($form['checkin_'.$looper]),
		    drupal_render($form['announced_'.$looper]),
		    drupal_render($form['patroller_'.$looper]),
		    drupal_render($form['lpn_'.$looper]),
		    drupal_render($form['make_'.$looper]),
		    drupal_render($form['model_'.$looper]),
		    drupal_render($form['color_'.$looper]),
		    );
  }
  // Render Table
  return theme_table($header, $rows);
}

function chpvalet_announced_form_submit($form, &$form_state) {
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



