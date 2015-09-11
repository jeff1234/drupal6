<?php



/**********
 ********** PAD add
 ********** Address: chpdevicepadadd
 ********** Access:  'anai chp add pad'
 *********/



function chpdevice_pad_add_form($form_state) {
  drupal_set_title(t('Add Pay and Display meter'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['show_companies'] = TRUE;
  $settings['show_properties'] = TRUE;
  $settings['show_lots'] = TRUE;
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $property_id = $form_state['storage']['ajax']['PropertyId'];
  $lot_id = $form_state['storage']['ajax']['LotId'];

  // Layout AgentId
  if (!isset($form_state['storage']['ajax']['AgentId'])) {
    $form_state['storage']['ajax']['AgentId'] = '';
  }
  $form['ajax']['AgentId'] =
    array('#type' => 'textfield',
	  '#title' => t('Serial Number'),
	  '#maxlength' => 20,
	  '#default_value' => $form_state['storage']['ajax']['AgentId'],
	  '#description' =>
	  t('Format: TAG-<8-digit-number>').'<br>'.
	  t('Example: COMPANY-20131201'));

  // Layout Alias
  if (!isset($form_state['storage']['ajax']['Alias'])) {
    $form_state['storage']['ajax']['Alias'] = '';
  }
  $form['ajax']['Alias'] =
    array('#type' => 'textfield',
	  '#title' => t('Meter name'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['Alias']);

  // Layout UTCOffset
  if (!isset($form_state['storage']['ajax']['UTCOffset'])) {
    $form_state['storage']['ajax']['UTCOffset'] = '';
  }
  $form['ajax']['UTCOffset'] =
    array('#type' => 'textfield',
	  '#title' => t('UTC Offset'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['UTCOffset'],
	  '#description' => t('Numbers of hours difference from UTC timezone'));

  // Buzzer
  $form['ajax']['buzzer'] =
    array('#type' => 'fieldset',
	  '#title' => t('Door alarm buzzer'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  if (!isset($form_state['storage']['ajax']['buzzer']['Active'])) {
    $form_state['storage']['ajax']['buzzer']['Active'] = TRUE;
  }
  $form['ajax']['buzzer']['Active'] =
    array('#type' => 'checkbox',
	  '#title' => t('Enabled'),
	  '#default_value' => $form_state['storage']['ajax']['buzzer']['Active']);

  if (!isset($form_state['storage']['ajax']['buzzer']['DoorTimer'])) {
    $form_state['storage']['ajax']['buzzer']['DoorTimer'] = 20;
  }
  $form['ajax']['buzzer']['DoorTimer'] =
    array('#type' => 'textfield',
	  '#title' => t('Seconds until alarm activates'),
	  '#maxlength' => 3,
	  '#default_value' => $form_state['storage']['ajax']['buzzer']['DoorTimer']);

  if (!isset($form_state['storage']['ajax']['buzzer']['PIN'])) {
    $form_state['storage']['ajax']['buzzer']['PIN'] = '12341234';
  }
  $form['ajax']['buzzer']['PIN'] =
    array('#type' => 'textfield',
	  '#title' => t('Deactivation PIN'),
	  '#maxlength' => 8,
	  '#default_value' => $form_state['storage']['ajax']['buzzer']['PIN']);

  // TicketPrinter
  $form['ajax']['regularprinter'] =
    array('#type' => 'fieldset',
	  '#title' => t('Regular ticket layout'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  if (!isset($form_state['storage']['ajax']['regularprinter']['HeaderLine1'])) {
    $form_state['storage']['ajax']['regularprinter']['HeaderLine1'] = t('Parking receipt');
  }
  $form['ajax']['regularprinter']['HeaderLine1'] =
    array('#type' => 'textfield',
	  '#title' => t('Header line 1'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['regularprinter']['HeaderLine1']);

  if (!isset($form_state['storage']['ajax']['regularprinter']['HeaderLine2'])) {
    $form_state['storage']['ajax']['regularprinter']['HeaderLine2'] = t('Pay and Display');
  }
  $form['ajax']['regularprinter']['HeaderLine2'] =
    array('#type' => 'textfield',
	  '#title' => t('Header line 2'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['regularprinter']['HeaderLine2']);

  if (!isset($form_state['storage']['ajax']['regularprinter']['Text1'])) {
    $form_state['storage']['ajax']['regularprinter']['Text1'] = t('Thank you for parking with us');
  }
  $form['ajax']['regularprinter']['Text1'] =
    array('#type' => 'textfield',
	  '#title' => t('Footer line 1'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['regularprinter']['Text1']);

  if (!isset($form_state['storage']['ajax']['regularprinter']['Text2'])) {
    $form_state['storage']['ajax']['regularprinter']['Text2'] = '';
  }
  $form['ajax']['regularprinter']['Text2'] =
    array('#type' => 'textfield',
	  '#title' => t('Footer line 2'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['regularprinter']['Text2']);

  if (!isset($form_state['storage']['ajax']['regularprinter']['Text3'])) {
    $form_state['storage']['ajax']['regularprinter']['Text3'] = '';
  }
  $form['ajax']['regularprinter']['Text3'] =
    array('#type' => 'textfield',
	  '#title' => t('Footer line 3'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['regularprinter']['Text3']);

  $form['ajax']['passprinter'] =
    array('#type' => 'fieldset',
	  '#title' => t('Pass ticket layout'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  if (!isset($form_state['storage']['ajax']['passprinter']['HeaderLine1'])) {
    $form_state['storage']['ajax']['passprinter']['HeaderLine1'] = t('Parking receipt');
  }
  $form['ajax']['passprinter']['HeaderLine1'] =
    array('#type' => 'textfield',
	  '#title' => t('Header line 1'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['passprinter']['HeaderLine1']);

  if (!isset($form_state['storage']['ajax']['passprinter']['HeaderLine2'])) {
    $form_state['storage']['ajax']['passprinter']['HeaderLine2'] = t('Pay and Display');
  }
  $form['ajax']['passprinter']['HeaderLine2'] =
    array('#type' => 'textfield',
	  '#title' => t('Header line 2'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['passprinter']['HeaderLine2']);

  if (!isset($form_state['storage']['ajax']['passprinter']['Text1'])) {
    $form_state['storage']['ajax']['passprinter']['Text1'] = t('Thank you for parking with us');
  }
  $form['ajax']['passprinter']['Text1'] =
    array('#type' => 'textfield',
	  '#title' => t('Footer line 1'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['passprinter']['Text1']);

  if (!isset($form_state['storage']['ajax']['passprinter']['Text2'])) {
    $form_state['storage']['ajax']['passprinter']['Text2'] = '';
  }
  $form['ajax']['passprinter']['Text2'] =
    array('#type' => 'textfield',
	  '#title' => t('Footer line 2'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['passprinter']['Text2']);

  if (!isset($form_state['storage']['ajax']['passprinter']['Text3'])) {
    $form_state['storage']['ajax']['passprinter']['Text3'] = '';
  }
  $form['ajax']['passprinter']['Text3'] =
    array('#type' => 'textfield',
	  '#title' => t('Footer line 3'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['passprinter']['Text3']);

  // Audits
  $form['ajax']['audit'] =
    array('#type' => 'fieldset',
	  '#title' => t('Audit'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  if (!isset($form_state['storage']['ajax']['audit']['Active'])) {
    $form_state['storage']['ajax']['audit']['Active'] = TRUE;
  }
  $form['ajax']['audit']['Active'] =
    array('#type' => 'checkbox',
	  '#title' => t('Enabled'),
	  '#default_value' => $form_state['storage']['ajax']['audit']['Active']);

  if (!isset($form_state['storage']['ajax']['audit']['UpdateTimestamp'])) {
    $form_state['storage']['ajax']['audit']['UpdateTimestamp'] = TRUE;
  }
  $form['ajax']['audit']['UpdateTimestamp'] =
    array('#type' => 'checkbox',
	  '#title' => t('Update audit counter in meter'),
	  '#default_value' => $form_state['storage']['ajax']['audit']['UpdateTimestamp']);

  if (!isset($form_state['storage']['ajax']['audit']['Interval'])) {
    $form_state['storage']['ajax']['audit']['Interval'] = 15;
  }
  $form['ajax']['audit']['Interval'] =
    array('#type' => 'textfield',
	  '#title' => t('Minutes between audits'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['audit']['Interval']);

  // Communications
  $form['ajax']['commct'] =
    array('#type' => 'fieldset',
	  '#title' => t('Credit Card processing'),
	  '#collapsible' => TRUE,
	  '#collapsed' => FALSE);

  if (!isset($form_state['storage']['ajax']['commct']['CTServerAddr'])) {
    $form_state['storage']['ajax']['commct']['CTServerAddr'] = '';
  }
  $form['ajax']['commct']['CTServerAddr'] =
    array('#type' => 'textfield',
	  '#title' => t('Credit Card processing address'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['commct']['CTServerAddr']);

  if (!isset($form_state['storage']['ajax']['commct']['CTServerPort'])) {
    $form_state['storage']['ajax']['commct']['CTServerPort'] = '';
  }
  $form['ajax']['commct']['CTServerPort'] =
    array('#type' => 'textfield',
	  '#title' => t('Credit Card processing port'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commct']['CTServerPort']);

  if (!isset($form_state['storage']['ajax']['commct']['TransactionTMO'])) {
    $form_state['storage']['ajax']['commct']['TransactionTMO'] = 60;
  }
  $form['ajax']['commct']['TransactionTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Credit Card processing timeout limit'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commct']['TransactionTMO']);

  if (!isset($form_state['storage']['ajax']['commct']['TerminalId'])) {
    $form_state['storage']['ajax']['commct']['TerminalId'] = '';
  }
  $form['ajax']['commct']['TerminalId'] =
    array('#type' => 'textfield',
	  '#title' => t('Credit Card precessor terminal id'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commct']['TerminalId']);

  $form['ajax']['commalarms'] =
    array('#type' => 'fieldset',
	  '#title' => t('Alarm updates'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  if (!isset($form_state['storage']['ajax']['commalarms']['AlarmCriticalTMO'])) {
    $form_state['storage']['ajax']['commalarms']['AlarmCriticalTMO'] = 0;
  }
  $form['ajax']['commalarms']['AlarmCriticalTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Critical alarm update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commalarms']['AlarmCriticalTMO']);

  if (!isset($form_state['storage']['ajax']['commalarms']['AlarmMajorTMO'])) {
    $form_state['storage']['ajax']['commalarms']['AlarmMajorTMO'] = 300;
  }
  $form['ajax']['commalarms']['AlarmMajorTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Major alarm update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commalarms']['AlarmMajorTMO']);

  if (!isset($form_state['storage']['ajax']['commalarms']['AlarmWarningTMO'])) {
    $form_state['storage']['ajax']['commalarms']['AlarmWarningTMO'] = 1800;
  }
  $form['ajax']['commalarms']['AlarmWarningTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Warning alarm update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commalarms']['AlarmWarningTMO']);

  $form['ajax']['commstats'] =
    array('#type' => 'fieldset',
	  '#title' => t('Statistics updates'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  if (!isset($form_state['storage']['ajax']['commstats']['CashTotalTMO'])) {
    $form_state['storage']['ajax']['commstats']['CashTotalTMO'] = 3600;
  }
  $form['ajax']['commstats']['CashTotalTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Cash odometer update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commstats']['CashTotalTMO']);

  if (!isset($form_state['storage']['ajax']['commstats']['StatsCreditTransTMO'])) {
    $form_state['storage']['ajax']['commstats']['StatsCreditTransTMO'] =
      empty($communication) ? 3600 : $communication['StatsCreditTransTMO'];
  }
  $form['ajax']['commstats']['StatsCreditTransTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Credit Card statistics update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commstats']['StatsCreditTransTMO']);

  if (!isset($form_state['storage']['ajax']['commstats']['StatsCoinTMO'])) {
    $form_state['storage']['ajax']['commstats']['StatsCoinTMO'] = 3600;
  }
  $form['ajax']['commstats']['StatsCoinTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Coin statistics update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commstats']['StatsCoinTMO']);

  if (!isset($form_state['storage']['ajax']['commstats']['StatsBillTMO'])) {
    $form_state['storage']['ajax']['commstats']['StatsBillTMO'] = 3600;
  }
  $form['ajax']['commstats']['StatsBillTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Bill statistics update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commstats']['StatsBillTMO']);

  if (!isset($form_state['storage']['ajax']['commstats']['StatsOAMVolumeTMO'])) {
    $form_state['storage']['ajax']['commstats']['StatsOAMVolumeTMO'] = 3600;
  }
  $form['ajax']['commstats']['StatsOAMVolumeTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Cloud link volume statistics update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commstats']['StatsOAMVolumeTMO']);

  if (!isset($form_state['storage']['ajax']['commstats']['StatsCTVolumeTMO'])) {
    $form_state['storage']['ajax']['commstats']['StatsCTVolumeTMO'] = 3600;
  }
  $form['ajax']['commstats']['StatsCTVolumeTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Credit Card precessing link volume statistics update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commstats']['StatsCTVolumeTMO']);

  if (!isset($form_state['storage']['ajax']['commstats']['StatsPowerManTMO'])) {
    $form_state['storage']['ajax']['commstats']['StatsPowerManTMO'] = 3600;
  }
  $form['ajax']['commstats']['StatsPowerManTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Power consumption update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commstats']['StatsPowerManTMO']);

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpdevice_pad_add_form_validate_save'),
	  '#submit' => array('chpdevice_pad_add_form_submit_save'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpdevice_pad_add_form_submit_cancel'));
  return $form;
}

function chpdevice_pad_add_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepad';
}

function chpdevice_pad_add_form_validate_save($form, &$form_state) {
  if (isset($form_state['values']['ajax']['AgentId']) and
      empty($form_state['values']['ajax']['AgentId'])) {
    form_set_error('ajax][AgentId', 'Serial number field is required.');
    return;
  }
  if (empty($form_state['values']['ajax']['Alias'])) {
    form_set_error('ajax][Alias', 'Meter name field is required.');
    return;
  }
  if (empty($form_state['values']['ajax']['UTCOffset'])) {
    form_set_error('ajax][UTCOffset', 'UTC Offset field is required.');
    return;
  }
  if (!preg_match('/^-{0,1}\d+$/', $form_state['values']['ajax']['UTCOffset'])) {
    form_set_error('ajax][UTCOffset', 'UTC Offset must be a number.');
    return;
  }

  $company_id = $form_state['values']['ajax']['CompanyId'];
  $property_id = $form_state['values']['ajax']['PropertyId'];
  $lot_id = $form_state['values']['ajax']['LotId'];

  if (0 < count(chdbccpark_get_cloud_agentid($form_state['values']['ajax']['AgentId']))) {
    form_set_error('ajax][AgentId', 'Serial number already taken.');
    return;
  }

  // Buzzer
  if (empty($form_state['values']['ajax']['buzzer']['DoorTimer'])) {
    form_set_error('ajax][buzzer][DoorTimer', 'Seconds until alarm activates field is required.');
    return;
  }
  if (!preg_match('/^\d+$/', $form_state['values']['ajax']['buzzer']['DoorTimer'])) {
    form_set_error('ajax][buzzer][DoorTimer', 'Seconds until alarm activates must be a number.');
    return;
  }
  if (empty($form_state['values']['ajax']['buzzer']['PIN'])) {
    form_set_error('ajax][buzzer][PIN', 'Deactivation PIN field is required.');
    return;
  }
  if (8 != strlen($form_state['values']['ajax']['buzzer']['PIN'])) {
    form_set_error('ajax][buzzer][PIN', 'Deactivation PIN must be of length 8.');
    return;
  }

  // Audits
  if (empty($form_state['values']['ajax']['audit']['Interval'])) {
    form_set_error('ajax][audit][Interval',
		   'Minutes between audits field is required.');
    return;
  }
  if (!preg_match('/^\d+$/', $form_state['values']['ajax']['audit']['Interval'])) {
    form_set_error('ajax][audit][Interval',
		   'Minutes between audits must be a positive number.');
    return;
  }
  if (1 > $form_state['values']['ajax']['audit']['Interval']) {
    form_set_error('ajax][audit][Interval',
		   'Minutes between audits can be less than 1 minute.');
    return;
  }

  // Communications
  if (empty($form_state['values']['ajax']['commct']['CTServerAddr'])) {
    form_set_error('ajax][commct][CTServerAddr',
		   'Credit Card processing IP address field is required.');
    return;
  }
  if (empty($form_state['values']['ajax']['commct']['CTServerPort'])) {
    form_set_error('ajax][commct][CTServerPort',
		   'Credit Card processing port field is required.');
    return;
  }
  if (!preg_match('/^\d+$/', $form_state['values']['ajax']['commct']['CTServerPort'])) {
    form_set_error('ajax][commct][CTServerPort',
		   'Credit Card processing port must be a positive number.');
    return;
  }
  if (empty($form_state['values']['ajax']['commct']['TransactionTMO'])) {
    form_set_error('ajax][commct][TransactionTMO',
		   'Credit Card processing timeout limit field is required.');
    return;
  }
  if (!preg_match('/^\d+$/', $form_state['values']['ajax']['commct']['TransactionTMO'])) {
    form_set_error('ajax][commct][TransactionTMO',
		   'Credit Card processing timeout limit must be a positive number.');
    return;
  }
  if (empty($form_state['values']['ajax']['commct']['TerminalId'])) {
    form_set_error('ajax][commct][TerminalId',
		   'Credit Card precessor terminal id field is required.');
    return;
  }

  if (0 == strlen($form_state['values']['ajax']['commalarms']['AlarmCriticalTMO'])) {
    form_set_error('ajax][commalarms][AlarmCriticalTMO',
		   'Critical alarm update interval field is required.');
    return;
  }
  if (!preg_match('/^\d+$/', $form_state['values']['ajax']['commalarms']['AlarmCriticalTMO'])) {
    form_set_error('ajax][commalarms][AlarmCriticalTMO',
		   'Critical alarm update interval must be a positive number.');
    return;
  }
  if (0 == strlen($form_state['values']['ajax']['commalarms']['AlarmMajorTMO'])) {
    form_set_error('ajax][commalarms][AlarmMajorTMO',
		   'Major alarm update interval field is required.');
    return;
  }
  if (!preg_match('/^\d+$/', $form_state['values']['ajax']['commalarms']['AlarmMajorTMO'])) {
    form_set_error('ajax][commalarms][AlarmMajorTMO',
		   'Major alarm update interval must be a positive number.');
    return;
  }
  if (0 == strlen($form_state['values']['ajax']['commalarms']['AlarmWarningTMO'])) {
    form_set_error('ajax][commalarms][AlarmWarningTMO',
		   'Warning alarm update interval field is required.');
    return;
  }
  if (!preg_match('/^\d+$/', $form_state['values']['ajax']['commalarms']['AlarmWarningTMO'])) {
    form_set_error('ajax][commalarms][AlarmWarningTMO',
		   'Warning alarm update interval must be a positive number.');
    return;
  }

  if (empty($form_state['values']['ajax']['commstats']['CashTotalTMO'])) {
    form_set_error('ajax][commstats][CashTotalTMO',
		   'Cash odometer update interval field is required.');
    return;
  }
  if (!preg_match('/^\d+$/', $form_state['values']['ajax']['commstats']['CashTotalTMO'])) {
    form_set_error('ajax][commstats][CashTotalTMO',
		   'Cash odometer update interval must be a positive number.');
    return;
  }
  if (empty($form_state['values']['ajax']['commstats']['StatsCreditTransTMO'])) {
    form_set_error('ajax][commstats][StatsCreditTransTMO',
		   'Credit Card statistics update interval field is required.');
    return;
  }
  if (!preg_match('/^\d+$/', $form_state['values']['ajax']['commstats']['StatsCreditTransTMO'])) {
    form_set_error('ajax][commstats][StatsCreditTransTMO',
		   'Credit Card statistics update interval must be a positive number.');
    return;
  }
  if (empty($form_state['values']['ajax']['commstats']['StatsCoinTMO'])) {
    form_set_error('ajax][commstats][StatsCoinTMO',
		   'Coin statistics update interval field is required.');
    return;
  }
  if (!preg_match('/^\d+$/', $form_state['values']['ajax']['commstats']['StatsCoinTMO'])) {
    form_set_error('ajax][commstats][StatsCoinTMO',
		   'Coin statistics update interval must be a positive number.');
    return;
  }
  if (empty($form_state['values']['ajax']['commstats']['StatsBillTMO'])) {
    form_set_error('ajax][commstats][StatsBillTMO',
		   'Bill statistics update interval field is required.');
    return;
  }
  if (!preg_match('/^\d+$/', $form_state['values']['ajax']['commstats']['StatsBillTMO'])) {
    form_set_error('ajax][commstats][StatsBillTMO',
		   'Bill statistics update interval must be a positive number.');
    return;
  }
  if (empty($form_state['values']['ajax']['commstats']['StatsOAMVolumeTMO'])) {
    form_set_error('ajax][commstats][StatsOAMVolumeTMO',
		   'Cloud link volume statistics update interval field is required.');
    return;
  }
  if (!preg_match('/^\d+$/', $form_state['values']['ajax']['commstats']['StatsOAMVolumeTMO'])) {
    form_set_error('ajax][commstats][StatsOAMVolumeTMO',
		   'Cloud link volume statistics update interval must be a positive number.');
    return;
  }
  if (empty($form_state['values']['ajax']['commstats']['StatsCTVolumeTMO'])) {
    form_set_error('ajax][commstats][StatsCTVolumeTMO',
		   'Credit Card precessing link volume statistics update interval field is required.');
    return;
  }
  if (!preg_match('/^\d+$/', $form_state['values']['ajax']['commstats']['StatsCTVolumeTMO'])) {
    form_set_error('ajax][commstats][StatsCTVolumeTMO',
		   'Credit Card precessing link volume statistics update interval must be a positive number.');
    return;
  }
  if (empty($form_state['values']['ajax']['commstats']['StatsPowerManTMO'])) {
    form_set_error('ajax][commstats][StatsPowerManTMO',
		   'Power consumption update interval field is required.');
    return;
  }
  if (!preg_match('/^\d+$/', $form_state['values']['ajax']['commstats']['StatsPowerManTMO'])) {
    form_set_error('ajax][commstats][StatsPowerManTMO',
		   'Power consumption update interval must be a positive number.');
    return;
  }

  // Position
  if (isset($form_state['values']['ajax']['pos']['Lat']) and
      !empty($form_state['values']['ajax']['pos']['Lat']) and
      !preg_match('/^-{0,1}\d+\.\d+$/', $form_state['values']['ajax']['pos']['Lat'])) {
    form_set_error('ajax][pos][Lat',
		   'Latitude must be a decimal number.');
    return;
  }
  if (isset($form_state['values']['ajax']['pos']['Lon']) and
      !empty($form_state['values']['ajax']['pos']['Lon']) and
      !preg_match('/^-{0,1}\d+\.\d+$/', $form_state['values']['ajax']['pos']['Lon'])) {
    form_set_error('ajax][pos][Lon',
		   'Longitude must be a decimal number.');
    return;
  }
}

function chpdevice_pad_add_form_submit_save($form, &$form_state) {
  global $user;
  $company_id = $form_state['values']['ajax']['CompanyId'];
  $property_id = $form_state['values']['ajax']['PropertyId'];
  $lot_id = $form_state['values']['ajax']['LotId'];

  $agent_id = check_plain($form_state['values']['ajax']['AgentId']);
  $alias = check_plain($form_state['values']['ajax']['Alias']);
  $utc_offset = $form_state['values']['ajax']['UTCOffset'];

  $systemid = chdbccpark_get_systemid($company_id);
  if (empty($systemid)) {
    chdbccpark_insert_systemid($company_id);
    $server_id = 'MOJO-00001';
  } else {
    chdbccpark_update_systemid($company_id);
    $server_id = sprintf("%s-%05d", $systemid['Tag'], $systemid['Counter']);
  }

  if (chdbccpark_insert_cloud
      ($user->uid,
       $company_id, $property_id, $lot_id, $alias,
       $agent_id, $server_id, $utc_offset,
       $id)) {
    drupal_set_message('Could not insert meter', 'error');
    return;
  }

  if (chdbccpark_insert_buzzer
      ($company_id, $agent_id, $server_id,
       $form_state['values']['ajax']['buzzer']['Active'],
       $form_state['values']['ajax']['buzzer']['DoorTimer'],
       $form_state['values']['ajax']['buzzer']['PIN'])) {
    drupal_set_message('Could not insert buzzer', 'error');
    return;
  }

  if (chdbccpark_insert_ticketprinter
      ($company_id, $agent_id, $server_id, 'Regular',
       check_plain($form_state['values']['ajax']['regularprinter']['HeaderLine1']),
       check_plain($form_state['values']['ajax']['regularprinter']['HeaderLine2']),
       check_plain($form_state['values']['ajax']['regularprinter']['Text1']),
       check_plain($form_state['values']['ajax']['regularprinter']['Text2']),
       check_plain($form_state['values']['ajax']['regularprinter']['Text3']))) {
    drupal_set_message('Could not insert regular printer', 'error');
    return;
  }

  if (chdbccpark_insert_ticketprinter
      ($company_id, $agent_id, $server_id, 'Pass',
       check_plain($form_state['values']['ajax']['passprinter']['HeaderLine1']),
       check_plain($form_state['values']['ajax']['passprinter']['HeaderLine2']),
       check_plain($form_state['values']['ajax']['passprinter']['Text1']),
       check_plain($form_state['values']['ajax']['passprinter']['Text2']),
       check_plain($form_state['values']['ajax']['passprinter']['Text3']))) {
    drupal_set_message('Could not insert pass printer', 'error');
    return;
  }

  if (chdbccpark_insert_auditcmd
      ($company_id, $agent_id, $server_id,
       $form_state['values']['ajax']['audit']['Active'],
       $form_state['values']['ajax']['audit']['Interval'],
       $form_state['values']['ajax']['audit']['UpdateTimestamp'])) {
    drupal_set_message('Could not insert audit cmd', 'error');
    return;
  }

  if (chdbccpark_insert_comm
      ($company_id, $agent_id, $server_id,
       $form_state['values']['ajax']['commstats']['CashTotalTMO'],
       $form_state['values']['ajax']['commalarms']['AlarmCriticalTMO'],
       $form_state['values']['ajax']['commalarms']['AlarmMajorTMO'],
       $form_state['values']['ajax']['commalarms']['AlarmWarningTMO'],
       $form_state['values']['ajax']['commstats']['StatsCreditTransTMO'],
       $form_state['values']['ajax']['commstats']['StatsCoinTMO'],
       $form_state['values']['ajax']['commstats']['StatsBillTMO'],
       $form_state['values']['ajax']['commstats']['StatsOAMVolumeTMO'],
       $form_state['values']['ajax']['commstats']['StatsCTVolumeTMO'],
       $form_state['values']['ajax']['commstats']['StatsPowerManTMO'],
       $form_state['values']['ajax']['commct']['CTServerAddr'],
       $form_state['values']['ajax']['commct']['CTServerPort'],
       $form_state['values']['ajax']['commct']['TransactionTMO'],
       $form_state['values']['ajax']['commct']['TerminalId'])) {
    drupal_set_message('Could not insert communication', 'error');
    return;
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepad';
}



/**********
 ********** PAD
 ********** Address: chpdevicepad
 ********** Access:  'anai chp list pad'
 *********/



function chpdevice_pad_form($form_state) {
  if ($form_state['storage']['detailed_view']) {
    return chpdevice_pad_view_form($form_state);
  }
  if ($form_state['storage']['detailed_edit']) {
    return chpdevice_pad_edit_form($form_state);
  }
  if ($form_state['storage']['detailed_new_edit']) {
    return chpdevice_padnew_edit_form($form_state);
  }

  drupal_set_title(t('Pay and Display'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['show_companies'] = TRUE;
  $settings['show_properties'] = TRUE;
  $settings['show_lots'] = TRUE;
  $settings['buttons']['back']['submit'] = 'chpdevice_pad_form_submit_back';
  if (user_access('anai chp add pad')) {
    $settings['buttons']['add']['submit'] = 'chpdevice_pad_form_submit_add';
  }
  $settings['buttons']['options']['submit'] = 'chpdevice_pad_form_submit_rates';
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $property_id = $form_state['storage']['ajax']['PropertyId'];
  $lot_id = $form_state['storage']['ajax']['LotId'];

  $meters = array();
  foreach (chdbccpark_get_systems($company_id, $property_id, $lot_id) as $entry) {
    $meters[$entry['AgentId']] = $entry;
  }

  $form['ajax']['connected'] =
    array('#type' => 'fieldset', '#title' => t('Connected meters'));
  if (empty($meters)) {
    $form['ajax']['connected']['list'] = array('#value' => t('Empty.'));
  } else {
    $form['ajax']['connected']['list'] = chpdevice_pad_table_form($meters);
  }

  $clouds = array();
  foreach (chdbccpark_get_clouds($company_id, $property_id, $lot_id) as $value) {
    if (isset($meters[$value['AgentId']])) {
      continue;
    }
    $clouds[$value['AgentId']] = $value;
  }

  if (!empty($clouds)) {
    $form['ajax']['pending'] = array('#type' => 'fieldset', '#title' => t('Pending meters'));
    $form['ajax']['pending']['list'] = chpdevice_padnew_table_form($clouds);
  }

  return $form;
}

function chpdevice_pad_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevice';
}

function chpdevice_pad_form_submit_add($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepadadd';
}

function chpdevice_pad_form_submit_rates($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepadrate';
}

function chpdevice_pad_table_form($entries) {
  global $user;
  $form['db'] = array('#type' => 'markup', '#theme' => 'chpdevice_pad_table');

  if (user_access('anai chp edit pad')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }

  $looper = 0;
  foreach ($entries as $entry) {
    $form['db']['name_'.$looper] = array('#value' => $entry['Alias']);

    if (empty($entry['LastTime'])) {
      $str = t('Never');
    } else {
      $time = chpuser_datetime_utc_to_usertimezone($entry['LastTime']);
      $str = $time->format("M j,y H:i:s");
    }
    $form['db']['time_'.$looper] = array('#value' => $str);

    if ($entry['CashResetPending']) {
      $str = '<del>'.sprintf("%0.02f", $entry['CashTotal']).'</del>';
    } else {
      $str = sprintf("%0.02f", $entry['CashTotal']);
    }
    $form['db']['odoabs_'.$looper] = array('#value' => $str);

    if (0 < $entry['NumPurchases']) {
      $tmp = $entry['CashTotal'] / $entry['NumPurchases'];
    } else {
      $tmp = 0.0;
    }
    if ($entry['CashResetPending']) {
      $str = '<del>'.sprintf("%0.02f/%s", $tmp, t('Pay')).'</del>';
    } else {
      $str = sprintf("%0.02f/%s", $tmp, t('Pay'));
    }
    $form['db']['odorel_'.$looper] = array('#value' => $str);

    $form['db']['odores_'.$looper] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/updatesmall.png',
	    '#name' => 'op_odores'.$entry['Id'],
	    '#submit' => array('chpdevice_pad_form_submit'));

    if ($entry['Status'] === 'Green') {
      $img = '<img src="'.drupal_get_path('module', 'anai').'/greenled.png">';
    } elseif ($entry['Status'] === 'Yellow') {
      $img = '<img src="'.drupal_get_path('module', 'anai').'/yellowled.png">';
    } elseif ($entry['Status'] === 'Orange') {
      $img = '<img src="'.drupal_get_path('module', 'anai').'/orangeled.png">';
    } elseif ($entry['Status'] === 'Red') {
      $img = '<img src="'.drupal_get_path('module', 'anai').'/redled.png">';
    } else {
      $img = '<img src="'.drupal_get_path('module', 'anai').'/neutralled.png">';
    }
    $link = l($img, 'chpdevicepadalarms/'.$entry['Id'], array('html' => TRUE));
    $form['db']['status_'.$looper] = array('#value' => $link);

    $form['db']['view_'.$looper] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/viewsmall.png',
	    '#name' => 'op_view'.$entry['Id'],
	    '#submit' => array('chpdevice_pad_form_submit'));

    if ($readonly) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_edit'.$entry['Id'],
	      '#submit' => array('chpdevice_pad_form_submit'));
    }
    $looper++;
  }
  return $form;
}

function chpdevice_pad_odores_form($form_state, $id) {
}

function theme_chpdevice_pad_table($form) {
  // Table Header
  $header = array(t('Meter'), t('Connected'),
		  array('data' => t('Odometer'),
			'colspan' => 3),
		  t('Status'), '', '');
  $rows = array();
  // Table Rows
  foreach ($form as $key => $value) {
    if (!strstr($key, 'name_')) {
      continue;
    }
    $looper = substr($key, strlen('name_'));
    $rows[] = array(drupal_render($form['name_'.$looper]),
                    drupal_render($form['time_'.$looper]),
                    array('data' => drupal_render($form['odoabs_'.$looper]),
			  'width' => 1, 'height' => 1),
                    array('data' => drupal_render($form['odorel_'.$looper]),
			  'width' => 1, 'height' => 1),
                    array('data' => drupal_render($form['odores_'.$looper]),
			  'width' => 1, 'height' => 1),
                    array('data' => drupal_render($form['status_'.$looper]),
			  'width' => 1, 'height' => 1),
                    array('data' => drupal_render($form['view_'.$looper]),
			  'width' => 1, 'height' => 1),
                    array('data' => drupal_render($form['edit_'.$looper]),
			  'width' => 1, 'height' => 1),
                    );
  }
  // Render Table
  return theme_table($header, $rows);
}

function chpdevice_padnew_table_form($entries) {
  global $user;
  $form['db'] = array('#type' => 'markup', '#theme' => 'chpdevice_padnew_table');

  if (user_access('anai chp edit pad')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }

  $looper = 0;
  foreach ($entries as $entry) {
    $form['db']['name_'.$looper] = array('#value' => decode_entities($entry['Alias']));
    $form['db']['serial_'.$looper] = array('#value' => decode_entities($entry['AgentId']));
    $form['db']['time_'.$looper] = array('#value' => t('Never'));
    $img = '<img src="'.drupal_get_path('module', 'anai').'/neutralled.png">';
    $form['db']['status_'.$looper] = array('#value' => $img);

    if ($readonly) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_editnew'.$entry['Id'],
	      '#submit' => array('chpdevice_pad_form_submit'));
    }
    $looper++;
  }
  return $form;
}

function theme_chpdevice_padnew_table($form) {
  $header = array(t('Meter'), t('Serial number'), t('Connected'), t('Status'), '');
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'name_')) {
      continue;
    }
    $looper = substr($key, strlen('name_'));
    $rows[] = array(drupal_render($form['name_'.$looper]),
                    drupal_render($form['serial_'.$looper]),
                    drupal_render($form['time_'.$looper]),
                    array('data' => drupal_render($form['status_'.$looper]),
			  'width' => 1, 'height' => 1),
                    array('data' => drupal_render($form['edit_'.$looper]),
			  'width' => 1, 'height' => 1),
                    );
  }
  return theme_table($header, $rows);
}

function chpdevice_pad_view_form($form_state) {
  global $user;
  $id = $form_state['storage']['id'];
  $pad = chdbccpark_get_system_id($id);
  $company_id = $pad['CompanyId'];
  $agent_id = $pad['AgentId'];
  $server_id = $pad['ServerId'];

  drupal_set_title(t('View  meter - NAME',
		     array('NAME' => decode_entities($pad['Alias']))));

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  $form['ajax']['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/back.png',
	  '#submit' => array('chpdevice_pad_view_form_submit_back'));

  $form['t1'] =
    array('#prefix' => '<table border="0"><tr valign="top"><td align="left" width="60%">',
	  '#value' => chpdevice_pad_view_detailed_table($pad),
	  '#suffix' => '</td>');
  $form['t2'] =
    array('#prefix' => '<td align="left">',
	  '#value' => chpdevice_pad_view_detailed_map($pad),
	  '#suffix' => '</td></tr></table>');

  $purchases = chdbccpark_cc_get_last_purchases($agent_id);
  $form['t3'] = array('#type' => 'fieldset', '#title' => t('Last purchases'));
  $form['t3']['list'] =
    array('#prefix' => '<td align="left">',
	  '#value' => chpdevice_pad_view_purchases_table($purchases),
	  '#suffix' => '</td></tr></table>');
  return $form;
}

function chpdevice_pad_view_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepad';
}

function chpdevice_pad_view_detailed_table($pad) {
  $header = array();
  $property = chdbprop_get_property($pad['PropertyId']);
  $policy = chdbprop_get_policy($pad['PropertyId'], $pad['LotId']);
  $rows[] = array(t('Property:'), decode_entities($property['Alias']));
  $rows[] = array(t('Lot:'), decode_entities($policy['Alias']));
  $rows[] = array(t('Street:'), decode_entities($property['Street']));
  $rows[] = array(t('City:'), decode_entities($property['City']));
  $rows[] = array(t('Postal code:'), decode_entities($property['PostalCode']));
  $rows[] = array(t('State/Province:'), $property['Province']);
  $rows[] = array(t('Country:'), $property['Country']);
  $time = chpuser_datetime_utc_to_usertimezone($pad['LastTime']);
  $rows[] = array(t('Connected:'), $time->format("M j,y H:i:s"));
  $auditmeta = chdbccpark_get_last_auditmeta($pad['AgentId']);
  if (empty($auditmeta)) {
    $rows[] = array(t('Last audit:'), t('Not received yet'));
  } else {
    $time = chpuser_datetime_utc_to_usertimezone($auditmeta['StopTime']);
    $rows[] = array(t('Last audit:'), $time->format("M j,y H:i:s"));
  }
  $auditcoins = chdbccpark_get_last_auditcoins($pad['AgentId']);
  if (empty($auditcoins)) {
    $rows[] = array(t('Coin vault:'), t('Not received yet'));
  } else {
    $val =
      $auditcoins['Val2'] * 2.0 +
      $auditcoins['Val1'] +
      $auditcoins['Dec25'] * 0.25 +
      $auditcoins['Dec10'] * 0.10 +
      $auditcoins['Dec05'] * 0.05 +
      $auditcoins['Dec01'] * 0.01;
    $rows[] = array(t('Coin vault:'), sprintf("%0.02f", $val));
  }
  $auditbills = chdbccpark_get_last_auditbills($pad['AgentId']);
  if (empty($auditbills)) {
    $rows[] = array(t('Bill vault:'), t('Not received yet'));
  } else {
    $val =
      $auditbills['Val20'] * 20 +
      $auditbills['Val10'] * 10 +
      $auditbills['Val5'] * 5 +
      $auditbills['Val1'];
    $rows[] = array(t('Bill vault:'), sprintf("%0.02f", $val));
  }
  if ($pad['Status'] === 'Green') {
    $img = '<img src="'.drupal_get_path('module', 'anai').'/greenled.png">';
  } elseif ($pad['Status'] === 'Yellow') {
    $img = '<img src="'.drupal_get_path('module', 'anai').'/yellowled.png">';
  } elseif ($pad['Status'] === 'Orange') {
    $img = '<img src="'.drupal_get_path('module', 'anai').'/orangeled.png">';
  } elseif ($pad['Status'] === 'Red') {
    $img = '<img src="'.drupal_get_path('module', 'anai').'/redled.png">';
  } else {
    $img = '<img src="'.drupal_get_path('module', 'anai').'/neutralled.png">';
  }
  $rows[] = array(t('Status:'), $img);
  return theme('table', $header, $rows);
}

function chpdevice_pad_view_detailed_map($pad) {
  if (empty($pad['Lat']) or empty($pad['Lon'])) {
    return t('Latitude/Longitude not valid.');
  }
  $map_array['id'] = 'LocationMap';
  $map_array['width'] = '400px';
  $map_array['height'] = '280px';
  $map_array['latitude'] = '49.6023';
  $map_array['longitude'] = '-124.9391';
  $map_array['maptype'] = 'Terrain';
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
  $text = '<h3>'.decode_entities($pad['Alias']).'</h3>';
  $markers[] = array
    ('text' => $text,
     'latitude' => sprintf("%f", $pad['Lat']),
     'longitude' => sprintf("%f", $pad['Lon']),
     'markername' => 'blue',
     );
  $map_array['markers'] = $markers;
  return theme('gmap', array('#settings' => $map_array));
}

function chpdevice_pad_view_purchases_table($entries) {
  $header = array(t('Time'), t('Ticket'), t('Reference'), t('Card type'), t('Card'), t('Amount'));
  $rows = array();
  foreach ($entries as $entry) {
    $time = chpuser_datetime_utc_to_usertimezone($entry['TransactionTime']);
    $rows[] = array($time->format("M j,y H:i:s"), $entry['TicketNo'],
		    $entry['TransactionId'], $entry['CardType'],
		    $entry['Card'], $entry['Amount']);
  }
  return theme('table', $header, $rows);
}

function chpdevice_padnew_edit_form($form_state) {
  if ($form_state['storage']['confirm_delete_padnew']) {
    return confirm_form
      ($form, 'Are you sure?', 'chpdevicepad',
       'The meter will be deleted from the system.');
  }

  global $user;
  $id = $form_state['storage']['id'];
  $pad = chdbccpark_get_cloud_id($id);
  $company_id = $pad['CompanyId'];
  $agent_id = $pad['AgentId'];
  $server_id = $pad['ServerId'];

  drupal_set_title(t('Edit meter - NAME', array('NAME' => decode_entities($pad['Alias']))));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  // Layout Alias
  if (!isset($form_state['storage']['ajax']['Alias'])) {
    $form_state['storage']['ajax']['Alias'] = decode_entities($pad['Alias']);
  }
  $form['ajax']['Alias'] =
    array('#type' => 'textfield',
	  '#title' => t('Meter name'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['Alias']);

  // Layout AgentId
  if (!isset($form_state['storage']['ajax']['AgentId'])) {
    $form_state['storage']['ajax']['AgentId'] = decode_entities($pad['AgentId']);
  }
  $form['ajax']['AgentId'] =
    array('#type' => 'textfield',
	  '#title' => t('Serial number'),
	  '#maxlength' => 64,
	  '#default_value' => $form_state['storage']['ajax']['AgentId']);

  // Layout UTCOffset
  if (!isset($form_state['storage']['ajax']['UTCOffset'])) {
    $form_state['storage']['ajax']['UTCOffset'] = $pad['UTCOffset'];
  }
  $form['ajax']['UTCOffset'] =
    array('#type' => 'textfield',
	  '#title' => t('UTC Offset'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['UTCOffset'],
	  '#description' => t('Numbers of hours difference from UTC timezone'));

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpdevice_padnew_edit_form_validate_save'),
	  '#submit' => array('chpdevice_padnew_edit_form_submit_save'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpdevice_padnew_edit_form_submit_cancel'));

  if (user_access('anai chp delete pad')) {
    $form['ajax']['delete'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/delete.png',
	    '#submit' => array('chpdevice_padnew_edit_form_submit_delete'));
  }
  return $form;
}

function chpdevice_padnew_edit_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepad';
}

function chpdevice_padnew_edit_form_submit_delete($form, &$form_state) {
  $form_state['storage']['confirm_delete_padnew'] = TRUE;
}

function chpdevice_padnew_edit_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['ajax']['Alias'])) {
    form_set_error('ajax][Alias', 'Meter name field is required.');
    return;
  }
  if (empty($form_state['values']['ajax']['AgentId'])) {
    form_set_error('ajax][AgentId', 'Serial number field is required.');
    return;
  }
  if (empty($form_state['values']['ajax']['UTCOffset'])) {
    form_set_error('ajax][UTCOffset', 'UTC Offset field is required.');
    return;
  }
  if (!preg_match('/^-{0,1}\d+$/', $form_state['values']['ajax']['UTCOffset'])) {
    form_set_error('ajax][UTCOffset', 'UTC Offset must be a number.');
    return;
  }
}

function chpdevice_padnew_edit_form_submit_save($form, &$form_state) {
  $id = $form_state['storage']['id'];

  if (chdbccpark_update_cloud_id
      ($id,
       check_plain($form_state['values']['ajax']['AgentId']),
       check_plain($form_state['values']['ajax']['Alias']),
       $form_state['values']['ajax']['UTCOffset'])) {
    drupal_set_message('Could not update meter', 'error');
    return;
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepad';
}

function chpdevice_pad_edit_form($form_state) {
  global $user;
  $id = $form_state['storage']['id'];
  $pad = chdbccpark_get_system_id($id);
  $company_id = $pad['CompanyId'];
  $agent_id = $pad['AgentId'];
  $server_id = $pad['ServerId'];

  drupal_set_title(t('Edit meter - NAME',
		     array('NAME' => decode_entities($pad['Alias']))));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  // Layout serial number as read only
  $form['ajax']['SerialNumber'] =
    array('#type' => 'item',
	  '#title' => t('Serial number'),
	  '#value' => $pad['AgentId']);

  // Layout Alias
  if (!isset($form_state['storage']['ajax']['Alias'])) {
    $form_state['storage']['ajax']['Alias'] = decode_entities($pad['Alias']);
  }
  $form['ajax']['Alias'] =
    array('#type' => 'textfield',
	  '#title' => t('Meter name'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['Alias']);

  // Layout UTCOffset
  if (!isset($form_state['storage']['ajax']['UTCOffset'])) {
    $form_state['storage']['ajax']['UTCOffset'] = $pad['UTCOffset'];
  }
  $form['ajax']['UTCOffset'] =
    array('#type' => 'textfield',
	  '#title' => t('UTC Offset'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['UTCOffset'],
	  '#description' => t('Numbers of hours difference from UTC timezone'));

  // Max charges
  $options = array();
  $options['None'] = t('None');
  foreach (chdbccpark_get_maxcharges($company_id) as $maxcharge) {
    $options[$maxcharge['ChargeId']] = decode_entities($maxcharge['Alias']);
  }

  $form['ajax']['maxcharge'] =
    array('#type' => 'fieldset',
	  '#title' => t('Maximum charge'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  if (!isset($form_state['storage']['ajax']['maxcharge']['ChargeId'])) {
    $maxchargemeter = chdbccpark_get_maxchargemeter($agent_id);
    if (!empty($maxchargemeter) and in_array($maxchargemeter['ChargeId'],
					     array_keys($options))) {
      $form_state['storage']['ajax']['maxcharge']['ChargeId'] =
	$maxchargemeter['ChargeId'];
    } else {
      $form_state['storage']['ajax']['maxcharge']['ChargeId'] = 'None';
    }
  }
  $form['ajax']['maxcharge']['ChargeId'] =
    array('#type' => 'select',
	  '#title' => t('Maximum charge'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['maxcharge']['ChargeId']);

  // Rate groups
  $options = array();
  $options['None'] = t('None');
  foreach (chdbccpark_get_rates($company_id) as $rate) {
    $options[$rate['RateId']] = decode_entities($rate['Alias']);
  }

  $ratemeters = array();
  foreach (chdbccpark_get_ratemeters($agent_id) as $ratemeter) {
    $ratemeters[$ratemeter['GroupId']] = $ratemeter;
  }

  $form['ajax']['rates'] =
    array('#type' => 'fieldset',
	  '#title' => t('Rate groups'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  // H
  if (!isset($form_state['storage']['ajax']['rates']['HRateId'])) {
    if (isset($ratemeters['H']) and in_array($ratemeters['H']['RateId'],
					     array_keys($options))) {
      $form_state['storage']['ajax']['rates']['HRateId'] =
	$ratemeters['H']['RateId'];
    } else {
      $form_state['storage']['ajax']['rates']['HRateId'] = 'None';
    }
  }
  $form['ajax']['rates']['HRateId'] =
    array('#type' => 'select',
	  '#title' => t('Group H'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['rates']['HRateId']);

  // G
  if (!isset($form_state['storage']['ajax']['rates']['GRateId'])) {
    if (isset($ratemeters['G']) and in_array($ratemeters['G']['RateId'],
					     array_keys($options))) {
      $form_state['storage']['ajax']['rates']['GRateId'] =
	$ratemeters['G']['RateId'];
    } else {
      $form_state['storage']['ajax']['rates']['GRateId'] = 'None';
    }
  }
  $form['ajax']['rates']['GRateId'] =
    array('#type' => 'select',
	  '#title' => t('Group G'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['rates']['GRateId']);

  // F
  if (!isset($form_state['storage']['ajax']['rates']['FRateId'])) {
    if (isset($ratemeters['F']) and in_array($ratemeters['F']['RateId'],
					     array_keys($options))) {
      $form_state['storage']['ajax']['rates']['FRateId'] = $ratemeters['F']['RateId'];
    } else {
      $form_state['storage']['ajax']['rates']['FRateId'] = 'None';
    }
  }
  $form['ajax']['rates']['FRateId'] =
    array('#type' => 'select',
	  '#title' => t('Group F'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['rates']['FRateId']);

  // E
  if (!isset($form_state['storage']['ajax']['rates']['ERateId'])) {
    if (isset($ratemeters['E']) and in_array($ratemeters['E']['RateId'],
					     array_keys($options))) {
      $form_state['storage']['ajax']['rates']['ERateId'] =
	$ratemeters['E']['RateId'];
    } else {
      $form_state['storage']['ajax']['rates']['ERateId'] = 'None';
    }
  }
  $form['ajax']['rates']['ERateId'] =
    array('#type' => 'select',
	  '#title' => t('Group E'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['rates']['ERateId']);

  // D
  if (!isset($form_state['storage']['ajax']['rates']['DRateId'])) {
    if (isset($ratemeters['D']) and in_array($ratemeters['D']['RateId'],
					     array_keys($options))) {
      $form_state['storage']['ajax']['rates']['DRateId'] =
	$ratemeters['D']['RateId'];
    } else {
      $form_state['storage']['ajax']['rates']['DRateId'] = 'None';
    }
  }
  $form['ajax']['rates']['DRateId'] =
    array('#type' => 'select',
	  '#title' => t('Group D'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['rates']['DRateId']);

  // C
  if (!isset($form_state['storage']['ajax']['rates']['CRateId'])) {
    if (isset($ratemeters['C']) and in_array($ratemeters['C']['RateId'],
					     array_keys($options))) {
      $form_state['storage']['ajax']['rates']['CRateId'] =
	$ratemeters['C']['RateId'];
    } else {
      $form_state['storage']['ajax']['rates']['CRateId'] = 'None';
    }
  }
  $form['ajax']['rates']['CRateId'] =
    array('#type' => 'select',
	  '#title' => t('Group C'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['rates']['CRateId']);

  // B
  if (!isset($form_state['storage']['ajax']['rates']['BRateId'])) {
    if (isset($ratemeters['B']) and in_array($ratemeters['B']['RateId'],
					     array_keys($options))) {
      $form_state['storage']['ajax']['rates']['BRateId'] =
	$ratemeters['B']['RateId'];
    } else {
      $form_state['storage']['ajax']['rates']['BRateId'] = 'None';
    }
  }
  $form['ajax']['rates']['BRateId'] =
    array('#type' => 'select',
	  '#title' => t('Group B'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['rates']['BRateId']);

  // A
  if (!isset($form_state['storage']['ajax']['rates']['ARateId'])) {
    if (isset($ratemeters['A']) and in_array($ratemeters['A']['RateId'],
					     array_keys($options))) {
      $form_state['storage']['ajax']['rates']['ARateId'] =
	$ratemeters['A']['RateId'];
    } else {
      $form_state['storage']['ajax']['rates']['ARateId'] = 'None';
    }
  }
  $form['ajax']['rates']['ARateId'] =
    array('#type' => 'select',
	  '#title' => t('Group A'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['rates']['ARateId']);

  // Parking passes
  $options = array();
  $options['None'] = t('None');
  foreach (chdbccpark_get_passes($company_id) as $pass) {
    $options[$pass['PassId']] = decode_entities($pass['Alias']);
  }

  $passmeters = array();
  foreach (chdbccpark_get_passmeters($agent_id) as $passmeter) {
    $passmeters[$passmeter['Number']] = $passmeter;
  }

  // Parking pass 1
  $form['ajax']['pass1'] =
    array('#type' => 'fieldset',
	  '#title' => t('Parking pass 1'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  if (!isset($form_state['storage']['ajax']['pass1']['PassId'])) {
    if (isset($passmeters['1']) and in_array($passmeters['1']['PassId'],
					     array_keys($options))) {
      $form_state['storage']['ajax']['pass1']['PassId'] =
	$passmeters['1']['PassId'];
    } else {
      $form_state['storage']['ajax']['pass1']['PassId'] = 'None';
    }
  }
  $form['ajax']['pass1']['PassId'] =
    array('#type' => 'select',
	  '#title' => t('Parking pass'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['pass1']['PassId']);

  // Parking pass 2
  $form['ajax']['pass2'] =
    array('#type' => 'fieldset',
	  '#title' => t('Parking pass 2'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  if (!isset($form_state['storage']['ajax']['pass2']['PassId'])) {
    if (isset($passmeters['2']) and in_array($passmeters['2']['PassId'],
					     array_keys($options))) {
      $form_state['storage']['ajax']['pass2']['PassId'] =
	$passmeters['2']['PassId'];
    } else {
      $form_state['storage']['ajax']['pass2']['PassId'] = 'None';
    }
  }
  $form['ajax']['pass2']['PassId'] =
    array('#type' => 'select',
	  '#title' => t('Parking pass'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['pass2']['PassId']);

  // Parking pass 3
  $form['ajax']['pass3'] =
    array('#type' => 'fieldset',
	  '#title' => t('Parking pass 3'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  if (!isset($form_state['storage']['ajax']['pass3']['PassId'])) {
    if (isset($passmeters['3']) and in_array($passmeters['3']['PassId'],
					     array_keys($options))) {
      $form_state['storage']['ajax']['pass3']['PassId'] =
	$passmeters['3']['PassId'];
    } else {
      $form_state['storage']['ajax']['pass3']['PassId'] = 'None';
    }
  }
  $form['ajax']['pass3']['PassId'] =
    array('#type' => 'select',
	  '#title' => t('Parking pass'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['pass3']['PassId']);

  // Buzzer
  $form['ajax']['buzzer'] =
    array('#type' => 'fieldset',
	  '#title' => t('Door alarm buzzer'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  $buzzer = chdbccpark_get_buzzer($agent_id);

  if (!isset($form_state['storage']['ajax']['buzzer']['Active'])) {
    $form_state['storage']['ajax']['buzzer']['Active'] =
      empty($buzzer) ? TRUE : $buzzer['Active'];
  }
  $form['ajax']['buzzer']['Active'] =
    array('#type' => 'checkbox',
	  '#title' => t('Enabled'),
	  '#default_value' => $form_state['storage']['ajax']['buzzer']['Active']);

  if (!isset($form_state['storage']['ajax']['buzzer']['DoorTimer'])) {
    $form_state['storage']['ajax']['buzzer']['DoorTimer'] =
      empty($buzzer) ? 20 : $buzzer['DoorTimer'];
  }
  $form['ajax']['buzzer']['DoorTimer'] =
    array('#type' => 'textfield',
	  '#title' => t('Seconds until alarm activates'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['buzzer']['DoorTimer']);

  if (!isset($form_state['storage']['ajax']['buzzer']['PIN'])) {
    $form_state['storage']['ajax']['buzzer']['PIN'] =
      empty($buzzer) ? '' : $buzzer['PIN'];
  }
  $form['ajax']['buzzer']['PIN'] =
    array('#type' => 'textfield',
	  '#title' => t('Deactivation PIN'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['buzzer']['PIN']);

  // TicketPrinter
  $form['ajax']['regularprinter'] =
    array('#type' => 'fieldset',
	  '#title' => t('Regular ticket layout'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  $ticketprinter = chdbccpark_get_ticketprinter($agent_id, 'Regular');

  if (!isset($form_state['storage']['ajax']['regularprinter']['HeaderLine1'])) {
    $form_state['storage']['ajax']['regularprinter']['HeaderLine1'] =
      empty($ticketprinter) ? '' : decode_entities($ticketprinter['HeaderLine1']);
  }
  $form['ajax']['regularprinter']['HeaderLine1'] =
    array('#type' => 'textfield',
	  '#title' => t('Header line 1'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['regularprinter']['HeaderLine1']);

  if (!isset($form_state['storage']['ajax']['regularprinter']['HeaderLine2'])) {
    $form_state['storage']['ajax']['regularprinter']['HeaderLine2'] =
      empty($ticketprinter) ? '' : decode_entities($ticketprinter['HeaderLine2']);
  }
  $form['ajax']['regularprinter']['HeaderLine2'] =
    array('#type' => 'textfield',
	  '#title' => t('Header line 2'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['regularprinter']['HeaderLine2']);

  if (!isset($form_state['storage']['ajax']['regularprinter']['Text1'])) {
    $form_state['storage']['ajax']['regularprinter']['Text1'] =
      empty($ticketprinter) ? '' : decode_entities($ticketprinter['Text1']);
  }
  $form['ajax']['regularprinter']['Text1'] =
    array('#type' => 'textfield',
	  '#title' => t('Footer line 1'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['regularprinter']['Text1']);

  if (!isset($form_state['storage']['ajax']['regularprinter']['Text2'])) {
    $form_state['storage']['ajax']['regularprinter']['Text2'] =
      empty($ticketprinter) ? '' : decode_entities($ticketprinter['Text2']);
  }
  $form['ajax']['regularprinter']['Text2'] =
    array('#type' => 'textfield',
	  '#title' => t('Footer line 2'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['regularprinter']['Text2']);

  if (!isset($form_state['storage']['ajax']['regularprinter']['Text3'])) {
    $form_state['storage']['ajax']['regularprinter']['Text3'] =
      empty($ticketprinter) ? '' : decode_entities($ticketprinter['Text3']);
  }
  $form['ajax']['regularprinter']['Text3'] =
    array('#type' => 'textfield',
	  '#title' => t('Footer line 3'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['regularprinter']['Text3']);

  $form['ajax']['passprinter'] =
    array('#type' => 'fieldset',
	  '#title' => t('Pass ticket layout'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  $ticketprinter = chdbccpark_get_ticketprinter($agent_id, 'Pass');

  if (!isset($form_state['storage']['ajax']['passprinter']['HeaderLine1'])) {
    $form_state['storage']['ajax']['passprinter']['HeaderLine1'] =
      empty($ticketprinter) ? '' : decode_entities($ticketprinter['HeaderLine1']);
  }
  $form['ajax']['passprinter']['HeaderLine1'] =
    array('#type' => 'textfield',
	  '#title' => t('Header line 1'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['passprinter']['HeaderLine1']);

  if (!isset($form_state['storage']['ajax']['passprinter']['HeaderLine2'])) {
    $form_state['storage']['ajax']['passprinter']['HeaderLine2'] =
      empty($ticketprinter) ? '' : decode_entities($ticketprinter['HeaderLine2']);
  }
  $form['ajax']['passprinter']['HeaderLine2'] =
    array('#type' => 'textfield',
	  '#title' => t('Header line 2'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['passprinter']['HeaderLine2']);

  if (!isset($form_state['storage']['ajax']['passprinter']['Text1'])) {
    $form_state['storage']['ajax']['passprinter']['Text1'] =
      empty($ticketprinter) ? '' : decode_entities($ticketprinter['Text1']);
  }
  $form['ajax']['passprinter']['Text1'] =
    array('#type' => 'textfield',
	  '#title' => t('Footer line 1'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['passprinter']['Text1']);

  if (!isset($form_state['storage']['ajax']['passprinter']['Text2'])) {
    $form_state['storage']['ajax']['passprinter']['Text2'] =
      empty($ticketprinter) ? '' : decode_entities($ticketprinter['Text2']);
  }
  $form['ajax']['passprinter']['Text2'] =
    array('#type' => 'textfield',
	  '#title' => t('Footer line 2'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['passprinter']['Text2']);

  if (!isset($form_state['storage']['ajax']['passprinter']['Text3'])) {
    $form_state['storage']['ajax']['passprinter']['Text3'] =
      empty($ticketprinter) ? '' : decode_entities($ticketprinter['Text3']);
  }
  $form['ajax']['passprinter']['Text3'] =
    array('#type' => 'textfield',
	  '#title' => t('Footer line 3'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['passprinter']['Text3']);

  // Audits
  $form['ajax']['audit'] =
    array('#type' => 'fieldset',
	  '#title' => t('Audit'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  $auditcmd = chdbccpark_get_auditcmd($agent_id);

  if (!isset($form_state['storage']['ajax']['audit']['Active'])) {
    $form_state['storage']['ajax']['audit']['Active'] =
      empty($auditcmd) ? TRUE : $auditcmd['Active'];
  }
  $form['ajax']['audit']['Active'] =
    array('#type' => 'checkbox',
	  '#title' => t('Enabled'),
	  '#default_value' => $form_state['storage']['ajax']['audit']['Active']);

  if (!isset($form_state['storage']['ajax']['audit']['UpdateTimestamp'])) {
    $form_state['storage']['ajax']['audit']['UpdateTimestamp'] =
      empty($auditcmd) ? TRUE : $auditcmd['UpdateTimestamp'];
  }
  $form['ajax']['audit']['UpdateTimestamp'] =
    array('#type' => 'checkbox',
	  '#title' => t('Update audit counter in meter'),
	  '#default_value' => $form_state['storage']['ajax']['audit']['UpdateTimestamp']);

  if (!isset($form_state['storage']['ajax']['audit']['Interval'])) {
    $form_state['storage']['ajax']['audit']['Interval'] =
      empty($auditcmd) ? 30 : $auditcmd['RepeatMinutes'];
  }
  $form['ajax']['audit']['Interval'] =
    array('#type' => 'textfield',
	  '#title' => t('Minutes between audits'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['audit']['Interval']);

  // Communications
  $communication = chdbccpark_get_communication($agent_id);

  $form['ajax']['commct'] =
    array('#type' => 'fieldset',
	  '#title' => t('Credit Card processing'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  if (!isset($form_state['storage']['ajax']['commct']['CTServerAddr'])) {
    $form_state['storage']['ajax']['commct']['CTServerAddr'] =
      empty($communication) ? '' : $communication['CTServerAddr'];
  }
  $form['ajax']['commct']['CTServerAddr'] =
    array('#type' => 'textfield',
	  '#title' => t('Credit Card processing IP address'),
	  '#maxlength' => 40,
	  '#default_value' => $form_state['storage']['ajax']['commct']['CTServerAddr']);

  if (!isset($form_state['storage']['ajax']['commct']['CTServerPort'])) {
    $form_state['storage']['ajax']['commct']['CTServerPort'] =
      empty($communication) ? '' : $communication['CTServerPort'];
  }
  $form['ajax']['commct']['CTServerPort'] =
    array('#type' => 'textfield',
	  '#title' => t('Credit Card processing port'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commct']['CTServerPort']);

  if (!isset($form_state['storage']['ajax']['commct']['TransactionTMO'])) {
    $form_state['storage']['ajax']['commct']['TransactionTMO'] =
      empty($communication) ? 60 : $communication['TransactionTMO'];
  }
  $form['ajax']['commct']['TransactionTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Credit Card processing timeout limit'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commct']['TransactionTMO']);

  if (!isset($form_state['storage']['ajax']['commct']['TerminalId'])) {
    $form_state['storage']['ajax']['commct']['TerminalId'] =
      empty($communication) ? '' : $communication['TerminalId'];
  }
  $form['ajax']['commct']['TerminalId'] =
    array('#type' => 'textfield',
	  '#title' => t('Credit Card precessor terminal id'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commct']['TerminalId']);

  $form['ajax']['commalarms'] =
    array('#type' => 'fieldset',
	  '#title' => t('Alarm updates'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  if (!isset($form_state['storage']['ajax']['commalarms']['AlarmCriticalTMO'])) {
    $form_state['storage']['ajax']['commalarms']['AlarmCriticalTMO'] =
      empty($communication) ? 0 : $communication['AlarmCriticalTMO'];
  }
  $form['ajax']['commalarms']['AlarmCriticalTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Critical alarm update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commalarms']['AlarmCriticalTMO']);

  if (!isset($form_state['storage']['ajax']['commalarms']['AlarmMajorTMO'])) {
    $form_state['storage']['ajax']['commalarms']['AlarmMajorTMO'] =
      empty($communication) ? 300 : $communication['AlarmMajorTMO'];
  }
  $form['ajax']['commalarms']['AlarmMajorTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Major alarm update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commalarms']['AlarmMajorTMO']);

  if (!isset($form_state['storage']['ajax']['commalarms']['AlarmWarningTMO'])) {
    $form_state['storage']['ajax']['commalarms']['AlarmWarningTMO'] =
      empty($communication) ? 1800 : $communication['AlarmWarningTMO'];
  }
  $form['ajax']['commalarms']['AlarmWarningTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Warning alarm update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commalarms']['AlarmWarningTMO']);

  $form['ajax']['commstats'] =
    array('#type' => 'fieldset',
	  '#title' => t('Statistics updates'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  if (!isset($form_state['storage']['ajax']['commstats']['CashTotalTMO'])) {
    $form_state['storage']['ajax']['commstats']['CashTotalTMO'] =
      empty($communication) ? 3600 : $communication['CashTotalTMO'];
  }
  $form['ajax']['commstats']['CashTotalTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Cash odometer update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commstats']['CashTotalTMO']);

  if (!isset($form_state['storage']['ajax']['commstats']['StatsCreditTransTMO'])) {
    $form_state['storage']['ajax']['commstats']['StatsCreditTransTMO'] =
      empty($communication) ? 3600 : $communication['StatsCreditTransTMO'];
  }
  $form['ajax']['commstats']['StatsCreditTransTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Credit Card statistics update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commstats']['StatsCreditTransTMO']);

  if (!isset($form_state['storage']['ajax']['commstats']['StatsCoinTMO'])) {
    $form_state['storage']['ajax']['commstats']['StatsCoinTMO'] =
      empty($communication) ? 3600 : $communication['StatsCoinTMO'];
  }
  $form['ajax']['commstats']['StatsCoinTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Coin statistics update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commstats']['StatsCoinTMO']);

  if (!isset($form_state['storage']['ajax']['commstats']['StatsBillTMO'])) {
    $form_state['storage']['ajax']['commstats']['StatsBillTMO'] =
      empty($communication) ? 3600 : $communication['StatsBillTMO'];
  }
  $form['ajax']['commstats']['StatsBillTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Bill statistics update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commstats']['StatsBillTMO']);

  if (!isset($form_state['storage']['ajax']['commstats']['StatsOAMVolumeTMO'])) {
    $form_state['storage']['ajax']['commstats']['StatsOAMVolumeTMO'] =
      empty($communication) ? 3600 : $communication['StatsOAMVolumeTMO'];
  }
  $form['ajax']['commstats']['StatsOAMVolumeTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Cloud link volume statistics update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commstats']['StatsOAMVolumeTMO']);

  if (!isset($form_state['storage']['ajax']['commstats']['StatsCTVolumeTMO'])) {
    $form_state['storage']['ajax']['commstats']['StatsCTVolumeTMO'] =
      empty($communication) ? 3600 : $communication['StatsCTVolumeTMO'];
  }
  $form['ajax']['commstats']['StatsCTVolumeTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Credit Card precessing link volume statistics update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commstats']['StatsCTVolumeTMO']);

  if (!isset($form_state['storage']['ajax']['commstats']['StatsPowerManTMO'])) {
    $form_state['storage']['ajax']['commstats']['StatsPowerManTMO'] =
      empty($communication) ? 3600 : $communication['StatsPowerManTMO'];
  }
  $form['ajax']['commstats']['StatsPowerManTMO'] =
    array('#type' => 'textfield',
	  '#title' => t('Power consumption update interval'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['commstats']['StatsPowerManTMO']);

  // Position
  $form['ajax']['pos'] =
    array('#type' => 'fieldset',
	  '#title' => t('Position'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);

  if (!isset($form_state['storage']['ajax']['pos']['Lat'])) {
    $form_state['storage']['ajax']['pos']['Lat'] = $pad['Lat'];
  }
  $form['ajax']['pos']['Lat'] =
    array('#type' => 'textfield',
	  '#title' => t('Latitude'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['pos']['Lat']);

  if (!isset($form_state['storage']['ajax']['pos']['Lon'])) {
    $form_state['storage']['ajax']['pos']['Lon'] = $pad['Lon'];
  }
  $form['ajax']['pos']['Lon'] =
    array('#type' => 'textfield',
	  '#title' => t('Longitude'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['pos']['Lon']);

  // Diagnostics
  $form['ajax']['diagnostics'] =
    array('#type' => 'fieldset',
	  '#title' => t('Diagnostics'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE);
  $options = array();
  $options['None'] = t('None');
  $options['Unregister'] = t('Re-register meter');
  $options['BIT'] = t('Meter to perform Built-in-Test');
  $options['Reboot'] = t('Reboot meter');
  if (!isset($form_state['storage']['ajax']['diagnostics']['Diagnostics'])) {
    $form_state['storage']['ajax']['diagnostics']['Diagnostics'] = key($options);
  }
  $form['ajax']['diagnostics']['Diagnostics'] =
    array('#type' => 'radios',
	  '#title' => t('Diagnostics'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['diagnostics']['Diagnostics']);

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpdevice_pad_add_form_validate_save'),
	  '#submit' => array('chpdevice_pad_edit_form_submit_save'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpdevice_pad_edit_form_submit_cancel'));
  return $form;
}

function chpdevice_pad_edit_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepad';
}

function chpdevice_pad_edit_form_submit_save($form, &$form_state) {
  global $user;
  $id = $form_state['storage']['id'];
  $pad = chdbccpark_get_system_id($id);
  $company_id = $pad['CompanyId'];
  $agent_id = $pad['AgentId'];
  $server_id = $pad['ServerId'];

  // Max charge
  $maxchargemeter = chdbccpark_get_maxchargemeter($agent_id);
  if (empty($maxchargemeter) and
      $form_state['values']['ajax']['maxcharge']['ChargeId'] !== 'None') {
    if (chdbccpark_insert_maxchargemeter
	($company_id, $agent_id, $server_id,
	 $form_state['values']['ajax']['maxcharge']['ChargeId'])) {
      drupal_set_message('Could not insert maxchargemeter', 'error');
      return;
    }
  } elseif (!empty($maxchargemeter) and
	    $form_state['values']['ajax']['maxcharge']['ChargeId'] === 'None') {
    if (chdbccpark_remove_maxchargemeter
	($company_id, $agent_id, $server_id)) {
      drupal_set_message('Could not remove maxchargemeter', 'error');
      return;
    }
  } elseif (!empty($maxchargemeter) and
	    $maxchargemeter['ChargeId'] !== $form_state['values']['ajax']['maxcharge']['ChargeId']) {
    if (chdbccpark_update_maxchargemeter
	($company_id, $agent_id, $server_id,
	 $form_state['values']['ajax']['maxcharge']['ChargeId'])) {
      drupal_set_message('Could not update maxchargemeter', 'error');
      return;
    }
  }

  // Rate groups
  $ratemeters = array();
  foreach (chdbccpark_get_ratemeters($agent_id) as $ratemeter) {
    $ratemeters[$ratemeter['GroupId']] = $ratemeter;
  }
  if (!isset($ratemeters['G']) and
      $form_state['values']['ajax']['rates']['GRateId'] !== 'None') {
    if (chdbccpark_insert_ratemeter
	($company_id, $agent_id, $server_id, 'G',
	 $form_state['values']['ajax']['rates']['GRateId'])) {
      drupal_set_message('Could not insert ratemeter G', 'error');
      return;
    }
  } elseif (isset($ratemeters['G']) and
	    $form_state['values']['ajax']['rates']['GRateId'] === 'None') {
    if (chdbccpark_remove_ratemeter
	($company_id, $agent_id, $server_id, 'G')) {
      drupal_set_message('Could not remove ratemeter G', 'error');
      return;
    }
  } elseif (isset($ratemeters['G']) and
	    $ratemeters['G']['RateId'] !== $form_state['values']['ajax']['rates']['GRateId']) {
    if (chdbccpark_update_ratemeter
	($company_id, $agent_id, $server_id, 'G',
	 $form_state['values']['ajax']['rates']['GRateId'])) {
      drupal_set_message('Could not update ratemeter G', 'error');
      return;
    }
  }
  if (!isset($ratemeters['F']) and
      $form_state['values']['ajax']['rates']['FRateId'] !== 'None') {
    if (chdbccpark_insert_ratemeter
	($company_id, $agent_id, $server_id, 'F',
	 $form_state['values']['ajax']['rates']['FRateId'])) {
      drupal_set_message('Could not insert ratemeter F', 'error');
      return;
    }
  } elseif (isset($ratemeters['F']) and
	    $form_state['values']['ajax']['rates']['FRateId'] === 'None') {
    if (chdbccpark_remove_ratemeter
	($company_id, $agent_id, $server_id, 'F')) {
      drupal_set_message('Could not remove ratemeter F', 'error');
      return;
    }
  } elseif (isset($ratemeters['F']) and
	    $ratemeters['F']['RateId'] !== $form_state['values']['ajax']['rates']['FRateId']) {
    if (chdbccpark_update_ratemeter
	($company_id, $agent_id, $server_id, 'F',
	 $form_state['values']['ajax']['rates']['FRateId'])) {
      drupal_set_message('Could not update ratemeter F', 'error');
      return;
    }
  }
  if (!isset($ratemeters['E']) and
      $form_state['values']['ajax']['rates']['ERateId'] !== 'None') {
    if (chdbccpark_insert_ratemeter
	($company_id, $agent_id, $server_id, 'E',
	 $form_state['values']['ajax']['rates']['ERateId'])) {
      drupal_set_message('Could not insert ratemeter E', 'error');
      return;
    }
  } elseif (isset($ratemeters['E']) and
	    $form_state['values']['ajax']['rates']['ERateId'] === 'None') {
    if (chdbccpark_remove_ratemeter
	($company_id, $agent_id, $server_id, 'E')) {
      drupal_set_message('Could not remove ratemeter E', 'error');
      return;
    }
  } elseif (isset($ratemeters['E']) and
	    $ratemeters['E']['RateId'] !== $form_state['values']['ajax']['rates']['ERateId']) {
    if (chdbccpark_update_ratemeter
	($company_id, $agent_id, $server_id, 'E',
	 $form_state['values']['ajax']['rates']['ERateId'])) {
      drupal_set_message('Could not update ratemeter E', 'error');
      return;
    }
  }
  if (!isset($ratemeters['D']) and
      $form_state['values']['ajax']['rates']['DRateId'] !== 'None') {
    if (chdbccpark_insert_ratemeter
	($company_id, $agent_id, $server_id, 'D',
	 $form_state['values']['ajax']['rates']['DRateId'])) {
      drupal_set_message('Could not insert ratemeter D', 'error');
      return;
    }
  } elseif (isset($ratemeters['D']) and
	    $form_state['values']['ajax']['rates']['DRateId'] === 'None') {
    if (chdbccpark_remove_ratemeter
	($company_id, $agent_id, $server_id, 'D')) {
      drupal_set_message('Could not remove ratemeter D', 'error');
      return;
    }
  } elseif (isset($ratemeters['D']) and
	    $ratemeters['D']['RateId'] !== $form_state['values']['ajax']['rates']['DRateId']) {
    if (chdbccpark_update_ratemeter
	($company_id, $agent_id, $server_id, 'D',
	 $form_state['values']['ajax']['rates']['DRateId'])) {
      drupal_set_message('Could not update ratemeter D', 'error');
      return;
    }
  }
  if (!isset($ratemeters['C']) and
      $form_state['values']['ajax']['rates']['CRateId'] !== 'None') {
    if (chdbccpark_insert_ratemeter
	($company_id, $agent_id, $server_id, 'C',
	 $form_state['values']['ajax']['rates']['CRateId'])) {
      drupal_set_message('Could not insert ratemeter C', 'error');
      return;
    }
  } elseif (isset($ratemeters['C']) and
	    $form_state['values']['ajax']['rates']['CRateId'] === 'None') {
    if (chdbccpark_remove_ratemeter
	($company_id, $agent_id, $server_id, 'C')) {
      drupal_set_message('Could not remove ratemeter C', 'error');
      return;
    }
  } elseif (isset($ratemeters['C']) and
	    $ratemeters['C']['RateId'] !== $form_state['values']['ajax']['rates']['CRateId']) {
    if (chdbccpark_update_ratemeter
	($company_id, $agent_id, $server_id, 'C',
	 $form_state['values']['ajax']['rates']['CRateId'])) {
      drupal_set_message('Could not update ratemeter C', 'error');
      return;
    }
  }
  if (!isset($ratemeters['B']) and
      $form_state['values']['ajax']['rates']['BRateId'] !== 'None') {
    if (chdbccpark_insert_ratemeter
	($company_id, $agent_id, $server_id, 'B',
	 $form_state['values']['ajax']['rates']['BRateId'])) {
      drupal_set_message('Could not insert ratemeter B', 'error');
      return;
    }
  } elseif (isset($ratemeters['B']) and
	    $form_state['values']['ajax']['rates']['BRateId'] === 'None') {
    if (chdbccpark_remove_ratemeter
	($company_id, $agent_id, $server_id, 'B')) {
      drupal_set_message('Could not remove ratemeter B', 'error');
      return;
    }
  } elseif (isset($ratemeters['B']) and
	    $ratemeters['B']['RateId'] !== $form_state['values']['ajax']['rates']['BRateId']) {
    if (chdbccpark_update_ratemeter
	($company_id, $agent_id, $server_id, 'B',
	 $form_state['values']['ajax']['rates']['BRateId'])) {
      drupal_set_message('Could not update ratemeter B', 'error');
      return;
    }
  }
  if (!isset($ratemeters['A']) and
      $form_state['values']['ajax']['rates']['ARateId'] !== 'None') {
    if (chdbccpark_insert_ratemeter
	($company_id, $agent_id, $server_id, 'A',
	 $form_state['values']['ajax']['rates']['ARateId'])) {
      drupal_set_message('Could not insert ratemeter A', 'error');
      return;
    }
  } elseif (isset($ratemeters['A']) and
	    $form_state['values']['ajax']['rates']['ARateId'] === 'None') {
    if (chdbccpark_remove_ratemeter
	($company_id, $agent_id, $server_id, 'A')) {
      drupal_set_message('Could not remove ratemeter A', 'error');
      return;
    }
  } elseif (isset($ratemeters['A']) and
	    $ratemeters['A']['RateId'] !== $form_state['values']['ajax']['rates']['ARateId']) {
    if (chdbccpark_update_ratemeter
	($company_id, $agent_id, $server_id, 'A',
	 $form_state['values']['ajax']['rates']['ARateId'])) {
      drupal_set_message('Could not update ratemeter A', 'error');
      return;
    }
  }

  // Parking pass
  $passmeters = array();
  foreach (chdbccpark_get_passmeters($agent_id) as $passmeter) {
    $passmeters[$passmeter['Number']] = $passmeter;
  }
  if (!isset($passmeters[1]) and
      $form_state['values']['ajax']['pass1']['PassId'] !== 'None') {
    if (chdbccpark_insert_passmeter
	($company_id, $agent_id, $server_id, 1,
	 $form_state['values']['ajax']['pass1']['PassId'])) {
      drupal_set_message('Could not insert passmeter 1', 'error');
      return;
    }
  } elseif (isset($passmeters[1]) and
	    $form_state['values']['ajax']['pass1']['PassId'] === 'None') {
    if (chdbccpark_remove_passmeter
	($company_id, $agent_id, $server_id, 1)) {
      drupal_set_message('Could not remove passmeter 1', 'error');
      return;
    }
  } elseif (isset($passmeters[1]) and
	    $passmeters[1]['PassId'] !== $form_state['values']['ajax']['pass1']['PassId']) {
    if (chdbccpark_update_passmeter
	($company_id, $agent_id, $server_id, 1,
	 $form_state['values']['ajax']['pass1']['PassId'])) {
      drupal_set_message('Could not update passmeter 1', 'error');
      return;
    }
  }
  if (!isset($passmeters[2]) and
      $form_state['values']['ajax']['pass2']['PassId'] !== 'None') {
    if (chdbccpark_insert_passmeter
	($company_id, $agent_id, $server_id, 2,
	 $form_state['values']['ajax']['pass2']['PassId'])) {
      drupal_set_message('Could not insert passmeter 2', 'error');
      return;
    }
  } elseif (isset($passmeters[2]) and
	    $form_state['values']['ajax']['pass2']['PassId'] === 'None') {
    if (chdbccpark_remove_passmeter
	($company_id, $agent_id, $server_id, 2)) {
      drupal_set_message('Could not remove passmeter 2', 'error');
      return;
    }
  } elseif (isset($passmeters[2]) and
	    $passmeters[2]['PassId'] !== $form_state['values']['ajax']['pass2']['PassId']) {
    if (chdbccpark_update_passmeter
	($company_id, $agent_id, $server_id, 2,
	 $form_state['values']['ajax']['pass2']['PassId'])) {
      drupal_set_message('Could not update passmeter 2', 'error');
      return;
    }
  }
  if (!isset($passmeters[3]) and
      $form_state['values']['ajax']['pass3']['PassId'] !== 'None') {
    if (chdbccpark_insert_passmeter
	($company_id, $agent_id, $server_id, 3,
	 $form_state['values']['ajax']['pass3']['PassId'])) {
      drupal_set_message('Could not insert passmeter 3', 'error');
      return;
    }
  } elseif (isset($passmeters[3]) and
	    $form_state['values']['ajax']['pass3']['PassId'] === 'None') {
    if (chdbccpark_remove_passmeter
	($company_id, $agent_id, $server_id, 3)) {
      drupal_set_message('Could not remove passmeter 3', 'error');
      return;
    }
  } elseif (isset($passmeters[3]) and
	    $passmeters[3]['PassId'] !== $form_state['values']['ajax']['pass3']['PassId']) {
    if (chdbccpark_update_passmeter
	($company_id, $agent_id, $server_id, 3,
	 $form_state['values']['ajax']['pass3']['PassId'])) {
      drupal_set_message('Could not update passmeter 3', 'error');
      return;
    }
  }

  // Position
  if (chdbccpark_update_system_position
      ($agent_id,
       $form_state['values']['ajax']['pos']['Lat'],
       $form_state['values']['ajax']['pos']['Lon'])) {
    drupal_set_message('Could not update position', 'error');
    return;
  }

  // Buzzer
  $buzzer = chdbccpark_get_buzzer($agent_id);
  if (empty($buzzer)) {
    if (chdbccpark_insert_buzzer
	($company_id, $agent_id, $server_id,
	 $form_state['values']['ajax']['buzzer']['Active'],
	 $form_state['values']['ajax']['buzzer']['DoorTimer'],
	 $form_state['values']['ajax']['buzzer']['PIN'])) {
      drupal_set_message('Could not insert buzzer', 'error');
      return;
    }
  } else {
    if (chdbccpark_update_buzzer
	($agent_id,
	 $form_state['values']['ajax']['buzzer']['Active'],
	 $form_state['values']['ajax']['buzzer']['DoorTimer'],
	 $form_state['values']['ajax']['buzzer']['PIN'])) {
      drupal_set_message('Could not update buzzer', 'error');
      return;
    }
  }

  // TicketPrinter
  $ticketprinter = chdbccpark_get_ticketprinter($agent_id, 'Regular');
  if (empty($ticketprinter)) {
    if (chdbccpark_insert_ticketprinter
	($company_id, $agent_id, $server_id, 'Regular',
	 $form_state['values']['ajax']['regularprinter']['HeaderLine1'],
	 $form_state['values']['ajax']['regularprinter']['HeaderLine2'],
	 $form_state['values']['ajax']['regularprinter']['Text1'],
	 $form_state['values']['ajax']['regularprinter']['Text2'],
	 $form_state['values']['ajax']['regularprinter']['Text3'])) {
      drupal_set_message('Could not insert regular printer', 'error');
      return;
    }
  } else {
    if (chdbccpark_update_ticketprinter
	($agent_id, 'Regular',
	 $form_state['values']['ajax']['regularprinter']['HeaderLine1'],
	 $form_state['values']['ajax']['regularprinter']['HeaderLine2'],
	 $form_state['values']['ajax']['regularprinter']['Text1'],
	 $form_state['values']['ajax']['regularprinter']['Text2'],
	 $form_state['values']['ajax']['regularprinter']['Text3'])) {
      drupal_set_message('Could not update regular printer', 'error');
      return;
    }
  }

  $ticketprinter = chdbccpark_get_ticketprinter($agent_id, 'Pass');
  if (empty($ticketprinter)) {
    if (chdbccpark_insert_ticketprinter
	($company_id, $agent_id, $server_id, 'Pass',
	 $form_state['values']['ajax']['passprinter']['HeaderLine1'],
	 $form_state['values']['ajax']['passprinter']['HeaderLine2'],
	 $form_state['values']['ajax']['passprinter']['Text1'],
	 $form_state['values']['ajax']['passprinter']['Text2'],
	 $form_state['values']['ajax']['passprinter']['Text3'])) {
      drupal_set_message('Could not insert pass printer', 'error');
      return;
    }
  } else {
    if (chdbccpark_update_ticketprinter
	($agent_id, 'Pass',
	 $form_state['values']['ajax']['passprinter']['HeaderLine1'],
	 $form_state['values']['ajax']['passprinter']['HeaderLine2'],
	 $form_state['values']['ajax']['passprinter']['Text1'],
	 $form_state['values']['ajax']['passprinter']['Text2'],
	 $form_state['values']['ajax']['passprinter']['Text3'])) {
      drupal_set_message('Could not update pass printer', 'error');
      return;
    }
  }

  // Audits
  $auditcmd = chdbccpark_get_auditcmd($agent_id);
  if (empty($auditcmd)) {
    if (chdbccpark_insert_auditcmd
	($company_id, $agent_id, $server_id,
	 $form_state['values']['ajax']['audit']['Active'],
	 $form_state['values']['ajax']['audit']['Interval'],
	 $form_state['values']['ajax']['audit']['UpdateTimestamp'])) {
      drupal_set_message('Could not insert audit cmd', 'error');
      return;
    }
  } else {
    if (chdbccpark_update_auditcmd
	($agent_id,
	 $form_state['values']['ajax']['audit']['Active'],
	 $form_state['values']['ajax']['audit']['Interval'],
	 $form_state['values']['ajax']['audit']['UpdateTimestamp'])) {
      drupal_set_message('Could not update audit cmd', 'error');
      return;
    }
  }

  // Communications
  $communication = chdbccpark_get_communication($agent_id);
  if (empty($communication)) {
    if (chdbccpark_insert_comm
	($company_id, $agent_id, $server_id,
	 $form_state['values']['ajax']['commstats']['CashTotalTMO'],
	 $form_state['values']['ajax']['commalarms']['AlarmCriticalTMO'],
	 $form_state['values']['ajax']['commalarms']['AlarmMajorTMO'],
	 $form_state['values']['ajax']['commalarms']['AlarmWarningTMO'],
	 $form_state['values']['ajax']['commstats']['StatsCreditTransTMO'],
	 $form_state['values']['ajax']['commstats']['StatsCoinTMO'],
	 $form_state['values']['ajax']['commstats']['StatsBillTMO'],
	 $form_state['values']['ajax']['commstats']['StatsOAMVolumeTMO'],
	 $form_state['values']['ajax']['commstats']['StatsCTVolumeTMO'],
	 $form_state['values']['ajax']['commstats']['StatsPowerManTMO'],
	 $form_state['values']['ajax']['commct']['CTServerAddr'],
	 $form_state['values']['ajax']['commct']['CTServerPort'],
	 $form_state['values']['ajax']['commct']['TransactionTMO'],
	 $form_state['values']['ajax']['commct']['TerminalId'])) {
      drupal_set_message('Could not insert communication', 'error');
      return;
    }
  } else {
    $pending_cash = $pending_stats = $pending_alarms = $pending_ct_tmo = $pending_ct = FALSE;

    if ($communication['CashTotalTMO'] != $form_state['values']['ajax']['commstats']['CashTotalTMO']) {
      $pending_cash = TRUE;
    }

    if ($communication['StatsCreditTransTMO'] !=
	$form_state['values']['ajax']['commstats']['StatsCreditTransTMO'] or
	$communication['StatsCoinTMO'] !=
	$form_state['values']['ajax']['commstats']['StatsCoinTMO'] or
	$communication['StatsBillTMO'] !=
	$form_state['values']['ajax']['commstats']['StatsBillTMO'] or
	$communication['StatsOAMVolumeTMO'] !=
	$form_state['values']['ajax']['commstats']['StatsOAMVolumeTMO'] or
	$communication['StatsCTVolumeTMO'] !=
	$form_state['values']['ajax']['commstats']['StatsCTVolumeTMO'] or
	$communication['StatsPowerManTMO'] !=
	$form_state['values']['ajax']['commstats']['StatsPowerManTMO']) {
      $pending_stats = TRUE;
    }

    if ($communication['AlarmCriticalTMO'] !=
	$form_state['values']['ajax']['commalarms']['AlarmCriticalTMO'] or
	$communication['AlarmMajorTMO'] !=
	$form_state['values']['ajax']['commalarms']['AlarmMajorTMO'] or
	$communication['AlarmWarningTMO'] !=
	$form_state['values']['ajax']['commalarms']['AlarmWarningTMO']) {
      $pending_alarms = TRUE;
    }

    if ($communication['CreditTMOPending'] !=
	$form_state['values']['ajax']['commct']['TransactionTMO']) {
      $pending_ct_tmo = TRUE;
    }

    if ($communication['CTServerAddr'] != $form_state['values']['ajax']['commct']['CTServerAddr'] or
	$communication['CTServerPort'] != $form_state['values']['ajax']['commct']['CTServerPort'] or
	$communication['TerminalId'] != $form_state['values']['ajax']['commct']['TerminalId']) {
      $pending_ct = TRUE;
    }

    if (chdbccpark_update_comm
	($agent_id,
	 $pending_cash, $pending_stats, $pending_alarms, $pending_ct_tmo, $pending_ct,
	 $form_state['values']['ajax']['commstats']['CashTotalTMO'],
	 $form_state['values']['ajax']['commalarms']['AlarmCriticalTMO'],
	 $form_state['values']['ajax']['commalarms']['AlarmMajorTMO'],
	 $form_state['values']['ajax']['commalarms']['AlarmWarningTMO'],
	 $form_state['values']['ajax']['commstats']['StatsCreditTransTMO'],
	 $form_state['values']['ajax']['commstats']['StatsCoinTMO'],
	 $form_state['values']['ajax']['commstats']['StatsBillTMO'],
	 $form_state['values']['ajax']['commstats']['StatsOAMVolumeTMO'],
	 $form_state['values']['ajax']['commstats']['StatsCTVolumeTMO'],
	 $form_state['values']['ajax']['commstats']['StatsPowerManTMO'],
	 $form_state['values']['ajax']['commct']['CTServerAddr'],
	 $form_state['values']['ajax']['commct']['CTServerPort'],
	 $form_state['values']['ajax']['commct']['TransactionTMO'],
	 $form_state['values']['ajax']['commct']['TerminalId'])) {
      drupal_set_message('Could not update communication', 'error');
      return;
    }
  }

  // Diagnostics
  $unregister = $bit = $reboot = FALSE;
  if ($form_state['values']['ajax']['diagnostics']['Diagnostics'] === 'Unregister') {
    $unregister = TRUE;
  } elseif ($form_state['values']['ajax']['diagnostics']['Diagnostics'] === 'BIT') {
    $bit = TRUE;
  } elseif ($form_state['values']['ajax']['diagnostics']['Diagnostics'] === 'Reboot') {
    $reboot = TRUE;
  }

  if ($unregister or $bit or $reboot) {
    if (chdbccpark_insert_diagnostics
	($company_id, $agent_id, $server_id,
	 $unregister, $bit, $reboot)) {
      drupal_set_message('Could not insert diagnostics', 'error');
      return;
    }
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepad';
}

function chpdevice_pad_form_submit($form, &$form_state) {
  if ($form_state['storage']['confirm_delete_padnew']) {
    $id = $form_state['storage']['id'];
    $pad = chdbccpark_get_cloud_id($id);
    $company_id = $pad['CompanyId'];
    $agent_id = $pad['AgentId'];
    $server_id = $pad['ServerId'];

    if (chdbccpark_delete_cloud_id($id)) {
      drupal_set_message('Could not delete meter', 'error');
      return;
    }

    if (0 == count(chdbccpark_get_all_systems($company_id))) {
      chdbccpark_delete_all_systemid($company_id);
    }

    chdbccpark_delete_diagnostics($agent_id);
    chdbccpark_delete_auditcmd($agent_id);
    chdbccpark_delete_comm($agent_id);
    chdbccpark_delete_buzzer($agent_id);
    chdbccpark_delete_ticketprinter($agent_id);
    chdbccpark_delete_alarms($agent_id);

    unset($form_state['storage']);
    $form_state['redirect'] = 'chpdevicepad';
    return;
  }

  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_editnew')) {
      $id = substr($key, strlen('op_editnew'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['id'] = $id;
    $form_state['storage']['detailed_new_edit'] = TRUE;
    return;
  }

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
    if (strstr($key, 'op_odores')) {
      $id = substr($key, strlen('op_odores'));
      break;
    }
  }
  if ($id > 0) {
    chdbccpark_update_system_cashreset_id($id);
    return;
  }
}



/**********
 ********** PAD alarms
 ********** Address: chpdevicepadalarms
 ********** Access:  'anai chp pad list alarms'
 *********/



function chpdevice_pad_alarms_form($form_state, $id) {
  if ($form_state['storage']['detailed_edit_alarm']) {
    return chpdevice_pad_alarms_edit_form($form_state, $id);
  }

  global $user;
  $form = array();

  if (!is_numeric($id)) {
    drupal_access_denied();
    exit();
  }

  $pad = chdbccpark_get_system_id($id);

  if (empty($pad)) {
    $form['Empty'] = array('#value' => t('Not found.'));
    return $form;
  }

  $company_id = $pad['CompanyId'];
  $agent_id = $pad['AgentId'];
  $server_id = $pad['ServerId'];

  drupal_set_title(t('Pay and Display alarms - NAME',
		     array('NAME' => decode_entities($pad['Alias']))));

  ahah_helper_register($form, $form_state);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  $form['ajax']['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/back.png',
	  '#submit' => array('chpdevice_pad_alarms_form_submit_back'));

  $options = array();
  $options['Current'] = t('Unacknowledged');
  $options['Retention'] = t('Acknowledged');
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
  
  if ($form_state['storage']['ajax']['Type'] === 'Current') {
    $alarms = chdbccpark_get_alarms($agent_id, FALSE);
    if (empty($alarms)) {
      $form['ajax']['list'] = array('#value' => t('Empty.'));
    } else {
      $form['ajax']['list'] = chpdevice_pad_alarms_table_form($alarms);
    }
  }

  if ($form_state['storage']['ajax']['Type'] === 'Retention') {
    $alarms = chdbccpark_get_alarms($agent_id, TRUE);
    if (empty($alarms)) {
      $form['ajax']['list'] = array('#value' => t('Empty.'));
    } else {
      $form['ajax']['list'] = chpdevice_pad_alarms_table_form($alarms);
    }
  }

  return $form;
}

function chpdevice_pad_alarms_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepad';
}

function chpdevice_pad_alarms_table_form($entries) {
  global $user;
  $form['db'] = array('#type' => 'markup', '#theme' => 'chpdevice_pad_alarms_table');

  if (user_access('anai chp edit pad alarms')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }

  $looper = 0;
  foreach ($entries as $entry) {
    $time = chpuser_datetime_utc_to_usertimezone($entry['CreateTime']);
    $str = $time->format("M j,y H:i:s");
    $form['db']['time_'.$looper] = array('#value' => $str);

    $form['db']['system_'.$looper] = array('#value' => $entry['System']);

    $form['db']['code_'.$looper] = array('#value' => $entry['Code']);

    if (empty($entry['CeaseTime'])) {
      $str = '';
    } else {
      $time = chpuser_datetime_utc_to_usertimezone($entry['CeaseTime']);
      $str = $time->format("M j,y H:i:s");
    }
    $form['db']['cease_'.$looper] = array('#value' => $str);

    $account = user_load(array('uid' => $entry['AcknowledgedByUID']));
    if (empty($account)) {
      $str = t('Unknown');
    } else {
      $str = $account->name;
    }
    $form['db']['ackby_'.$looper] = array('#value' => $str);

    if (empty($entry['AcknowledgedTime'])) {
      $str = '';
    } else {
      $time = chpuser_datetime_utc_to_usertimezone($entry['AcknowledgedTime']);
      $str = $time->format("M j,y H:i:s");
    }
    $form['db']['acktime_'.$looper] = array('#value' => $str);

    $form['db']['resolution_'.$looper] = array('#value' => decode_entities($entry['Resolution']));

    if ($entry['Severity'] == 0) {
      $img = '<img src="'.url(drupal_get_path('module', 'anai').'/greenled.png').'">';
    } elseif ($entry['Severity'] == 1) {
      $img = '<img src="'.url(drupal_get_path('module', 'anai').'/yellowled.png').'">';
    } elseif ($entry['Severity'] == 2) {
      $img = '<img src="'.url(drupal_get_path('module', 'anai').'/orangeled.png').'">';
    } elseif ($entry['Severity'] == 3) {
      $img = '<img src="'.url(drupal_get_path('module', 'anai').'/redled.png').'">';
    } else {
      $img = '<img src="'.url(drupal_get_path('module', 'anai').'/neutralled.png').'">';
    }
    $form['db']['status_'.$looper] = array('#value' => $img);

    if ($readonly or $entry['Acknowledged']) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
    	array('#type' => 'image_button',
    	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
    	      '#name' => 'op_editack'.$entry['Id'],
    	      '#submit' => array('chpdevice_pad_alarms_form_submit'));
    }
    $looper++;
  }
  return $form;
}

function theme_chpdevice_pad_alarms_table($form) {
  // Table Header
  $header = array('', t('Detect time'), t('Type'), t('Code'), t('Cease time'),
		  t('Acknowledged by'), t('Acknowledged time'),
		  t('Resolution'), '');
  $rows = array();
  // Table Rows
  foreach ($form as $key => $value) {
    if (!strstr($key, 'system_')) {
      continue;
    }
    $looper = substr($key, strlen('system_'));
    $rows[] = array(array('data' => drupal_render($form['status_'.$looper]),
			  'width' => 1, 'height' => 1),
                    drupal_render($form['time_'.$looper]),
                    drupal_render($form['system_'.$looper]),
		    drupal_render($form['code_'.$looper]),
                    drupal_render($form['cease_'.$looper]),
                    drupal_render($form['ackby_'.$looper]),
                    drupal_render($form['acktime_'.$looper]),
                    drupal_render($form['resolution_'.$looper]),
                    array('data' => drupal_render($form['edit_'.$looper]),
			  'width' => 1, 'height' => 1),
                    );
  }
  // Render Table
  return theme_table($header, $rows);
}

function chpdevice_pad_alarms_edit_form($form_state) {
  global $user;
  $id = $form_state['storage']['id'];
  $alarm = chdbccpark_get_alarm_id($id);

  drupal_set_title(t('Acknowledge alarm'));

  $header = array('', t('Detect time'), t('Type'), t('Code'), t('Cease time'));
  $time = chpuser_datetime_utc_to_usertimezone($alarm['CreateTime']);
  $str_b = $time->format("M j,y H:i:s");
  if ($alarm['Severity'] == 0) {
    $img = '<img src="'.url(drupal_get_path('module', 'anai').'/greenled.png').'">';
  } elseif ($alarm['Severity'] == 1) {
    $img = '<img src="'.url(drupal_get_path('module', 'anai').'/yellowled.png').'">';
  } elseif ($alarm['Severity'] == 2) {
    $img = '<img src="'.url(drupal_get_path('module', 'anai').'/orangeled.png').'">';
  } elseif ($alarm['Severity'] == 3) {
    $img = '<img src="'.url(drupal_get_path('module', 'anai').'/redled.png').'">';
  } else {
    $img = '<img src="'.url(drupal_get_path('module', 'anai').'/neutralled.png').'">';
  }
  if (empty($alarm['CeaseTime'])) {
    $str_e = '';
  } else {
    $time = chpuser_datetime_utc_to_usertimezone($alarm['CeaseTime']);
    $str_e = $time->format("M j,y H:i:s");
  }
  $row = array(array(array('data' => $img, 'width' => 1),
		     $str_b, $alarm['System'], $alarm['Code'], $str_e));
  $form['alarm'] = array('#value' => theme_table($header, $row));

  $form['Resolution'] =
    array('#type' => 'textarea',
	  '#title' => t('Resolution'),
	  '#maxlength' => 512);

  $form['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpdevice_pad_alarms_edit_form_validate_save'),
	  '#submit' => array('chpdevice_pad_alarms_edit_form_submit_save'));

  $form['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpdevice_pad_alarms_edit_form_submit_cancel'));
  return $form;
}

function chpdevice_pad_alarms_edit_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']['detailed_edit_alarm']);
}

function chpdevice_pad_alarms_edit_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['Resolution'])) {
    form_set_error('Resolution', t('Resolution field is required.'));
    return;
  }
}

function chpdevice_pad_alarms_edit_form_submit_save($form, &$form_state) {
  global $user;
  $id = $form_state['storage']['id'];
  $alarm = chdbccpark_get_alarm_id($id);
  $agent_id = $alarm['AgentId'];

  if (chdbccpark_acknowledge_alarm
      ($id,
       $user->uid,
       check_plain($form_state['values']['Resolution']))) {
    drupal_set_message('Could not acknowledge alarm', 'error');
    return;
  }

  $system_alarm = chdbccpark_get_alarm_severity($agent_id);
  $status = current($system_alarm);

  if (0 == $status) {
    $severity = 'Green';
  } elseif (1 == $status) {
    $severity = 'Yellow';
  } elseif (2 == $status) {
    $severity = 'Orange';
  } elseif (3 == $status) {
    $severity = 'Red';
  } else {
    $severity = 'Unknown';
  }

  if (chdbccpark_update_system_status
      ($agent_id,
       $severity)) {
    drupal_set_message('Could not update system status', 'error');
    return;
  }

  unset($form_state['storage']['detailed_edit_alarm']);
}

function chpdevice_pad_alarms_form_submit($form, &$form_state) {
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_editack')) {
      $id = substr($key, strlen('op_editack'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['id'] = $id;
    $form_state['storage']['detailed_edit_alarm'] = TRUE;
    return;
  }
}



/**********
 ********** PAD Rates
 ********** Address: chpdevicepadrate
 ********** Access:  'anai chp list pad'
 *********/



function chpdevice_pad_rates_add_form($form_state) {
  drupal_set_title(t('Add rate'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['show_companies'] = TRUE;
  $settings['show_properties'] = TRUE;
  $settings['show_lots'] = TRUE;
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $property_id = $form_state['storage']['ajax']['PropertyId'];
  $lot_id = $form_state['storage']['ajax']['LotId'];

  $options = array();
  $options['Max'] = t('Max charge');
  $options['Rate'] = t('Rate group');
  $options['Pass'] = t('Parking pass');
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

  if (!isset($form_state['storage']['ajax']['Alias'])) {
    $form_state['storage']['ajax']['Alias'] = '';
  }
  $form['ajax']['Alias'] =
    array('#type' => 'textfield',
	  '#title' => t('Name'),
	  '#maxlength' => 80,
	  '#default_value' => $form_state['storage']['ajax']['Alias']);

  if ($form_state['storage']['ajax']['Type'] === 'Max') {

    if (!isset($form_state['storage']['ajax']['Charge'])) {
      $form_state['storage']['ajax']['Charge'] = '0.00';
    }
    $form['ajax']['Charge'] =
      array('#type' => 'textfield',
	    '#title' => t('Charge'),
	    '#maxlength' => 10,
	    '#default_value' => $form_state['storage']['ajax']['Charge'],
	    '#description' => t('Format: X.YY'));

    $options = array();
    $options['Relative'] = t('Expires after entry time pluss duration time');
    $options['Fixed'] = t('Expires at next given time on day');
    if (!isset($form_state['storage']['ajax']['Behavior'])) {
      $form_state['storage']['ajax']['Behavior'] = key($options);
    }
    $form['ajax']['Behavior'] =
      array('#type' => 'radios',
	    '#title' => t('Behavior when maximum charge is reached'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['Behavior'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));

    if ($form_state['storage']['ajax']['Behavior'] === 'Relative') {
      $label = t('Duration time');
    } elseif ($form_state['storage']['ajax']['Behavior'] === 'Fixed') {
      $label = t('Time on day');
    }

    // Javascript date pickers
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdevice=function(context){
      	    $("#edit-ajax-HHMM:not(.HHMM-processed)",
              context).addClass("HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:true,
                              placement:"inline",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['HHMM'])) {
      $form_state['storage']['ajax']['HHMM'] = '';
    }
    $form['ajax']['HHMM'] =
      array('#type' => 'textfield',
	    '#default_value' => $form_state['storage']['ajax']['HHMM']);

  } elseif ($form_state['storage']['ajax']['Type'] === 'Rate') {

    // Javascript date pickers
    $label = t('Begin time');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceBegin=function(context){
      	    $("#edit-ajax-BeginHHMM:not(.BeginHHMM-processed)",
              context).addClass("BeginHHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['BeginHHMM'])) {
      $form_state['storage']['ajax']['BeginHHMM'] = '';
    }
    $form['ajax']['BeginHHMM'] =
      array('#type' => 'textfield',
	    '#title' => t('Begin time'),
	    '#default_value' => $form_state['storage']['ajax']['BeginHHMM']);

    // Javascript date pickers
    $label = t('End time');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceEnd=function(context){
      	    $("#edit-ajax-EndHHMM:not(.EndHHMM-processed)",
              context).addClass("EndHHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['EndHHMM'])) {
      $form_state['storage']['ajax']['EndHHMM'] = '';
    }
    $form['ajax']['EndHHMM'] =
      array('#type' => 'textfield',
	    '#title' => t('End time'),
	    '#default_value' => $form_state['storage']['ajax']['EndHHMM']);

    // Javascript date pickers
    $label = t('Minimum duration');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceMin=function(context){
      	    $("#edit-ajax-MinHHMM:not(.MinHHMM-processed)",
              context).addClass("MinHHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['MinHHMM'])) {
      $form_state['storage']['ajax']['MinHHMM'] = '';
    }
    $form['ajax']['MinHHMM'] =
      array('#type' => 'textfield',
	    '#title' => t('Minimum duration'),
	    '#default_value' => $form_state['storage']['ajax']['MinHHMM']);

    if (!isset($form_state['storage']['ajax']['MinCharge'])) {
      $form_state['storage']['ajax']['MinCharge'] = '0.00';
    }
    $form['ajax']['MinCharge'] =
      array('#type' => 'textfield',
	    '#title' => t('Minimum charge'),
	    '#maxlength' => 10,
	    '#default_value' => $form_state['storage']['ajax']['MinCharge'],
	    '#description' => t('Format: X.YY'));

    $options = array();
    $options['Mon'] = t('Monday');
    $options['Tue'] = t('Tuesday');
    $options['Wed'] = t('Wednesday');
    $options['Thu'] = t('Thursday');
    $options['Fri'] = t('Friday');
    $options['Sat'] = t('Saturday');
    $options['Sun'] = t('Sunday');
    if (!isset($form_state['storage']['ajax']['Week'])) {
      $form_state['storage']['ajax']['Week'] = array_keys($options);
    }
    $form['ajax']['Week'] =
      array('#type' => 'checkboxes',
	    '#title' => t('Weekdays'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['Week']);

    /**
     * Rate periods
     */

    // Period 1
    $form['ajax']['Period1'] =
      array('#type' => 'item',
	    '#title' => t('Period 1'),
	    '#prefix' => '<table><th></th><th>'.t('Length').'</th><th>'.t('Time step').'</th><th>'.t('Charge step').'</th><tr><td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Length');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceL1=function(context){
      	    $("#edit-ajax-Length1HHMM:not(.Length1HHMM-processed)",
              context).addClass("Length1HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['Length1HHMM'])) {
      $form_state['storage']['ajax']['Length1HHMM'] = '';
    }
    $form['ajax']['Length1HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['Length1HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Time step');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceTS1=function(context){
      	    $("#edit-ajax-TimeStep1HHMM:not(.TimeStep1HHMM-processed)",
              context).addClass("TimeStep1HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['TimeStep1HHMM'])) {
      $form_state['storage']['ajax']['TimeStep1HHMM'] = '';
    }
    $form['ajax']['TimeStep1HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['TimeStep1HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');


    if (!isset($form_state['storage']['ajax']['ChargeStep1'])) {
      $form_state['storage']['ajax']['ChargeStep1'] = '0.00';
    }
    $form['ajax']['ChargeStep1'] =
      array('#type' => 'textfield',
	    '#maxlength' => 10,
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['ChargeStep1'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td></tr>');

    // Period 2
    $form['ajax']['Period2'] =
      array('#type' => 'item',
	    '#title' => t('Period 2'),
	    '#prefix' => '<tr><td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Length');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceL2=function(context){
      	    $("#edit-ajax-Length2HHMM:not(.Length2HHMM-processed)",
              context).addClass("Length2HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['Length2HHMM'])) {
      $form_state['storage']['ajax']['Length2HHMM'] = '';
    }
    $form['ajax']['Length2HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['Length2HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Time step');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceTS2=function(context){
      	    $("#edit-ajax-TimeStep2HHMM:not(.TimeStep2HHMM-processed)",
              context).addClass("TimeStep2HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['TimeStep2HHMM'])) {
      $form_state['storage']['ajax']['TimeStep2HHMM'] = '';
    }
    $form['ajax']['TimeStep2HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['TimeStep2HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');


    if (!isset($form_state['storage']['ajax']['ChargeStep2'])) {
      $form_state['storage']['ajax']['ChargeStep2'] = '0.00';
    }
    $form['ajax']['ChargeStep2'] =
      array('#type' => 'textfield',
	    '#maxlength' => 10,
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['ChargeStep2'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td></tr>');

    // Period 3
    $form['ajax']['Period3'] =
      array('#type' => 'item',
	    '#title' => t('Period 3'),
	    '#prefix' => '<tr><td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Length');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceL3=function(context){
      	    $("#edit-ajax-Length3HHMM:not(.Length3HHMM-processed)",
              context).addClass("Length3HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['Length3HHMM'])) {
      $form_state['storage']['ajax']['Length3HHMM'] = '';
    }
    $form['ajax']['Length3HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['Length3HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Time step');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceTS3=function(context){
      	    $("#edit-ajax-TimeStep3HHMM:not(.TimeStep3HHMM-processed)",
              context).addClass("TimeStep3HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['TimeStep3HHMM'])) {
      $form_state['storage']['ajax']['TimeStep3HHMM'] = '';
    }
    $form['ajax']['TimeStep3HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['TimeStep3HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');


    if (!isset($form_state['storage']['ajax']['ChargeStep3'])) {
      $form_state['storage']['ajax']['ChargeStep3'] = '0.00';
    }
    $form['ajax']['ChargeStep3'] =
      array('#type' => 'textfield',
	    '#maxlength' => 10,
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['ChargeStep3'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td></tr>');

    // Period 4
    $form['ajax']['Period4'] =
      array('#type' => 'item',
	    '#title' => t('Period 4'),
	    '#prefix' => '<tr><td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Length');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceL4=function(context){
      	    $("#edit-ajax-Length4HHMM:not(.Length4HHMM-processed)",
              context).addClass("Length4HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['Length4HHMM'])) {
      $form_state['storage']['ajax']['Length4HHMM'] = '';
    }
    $form['ajax']['Length4HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['Length4HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Time step');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceTS4=function(context){
      	    $("#edit-ajax-TimeStep4HHMM:not(.TimeStep4HHMM-processed)",
              context).addClass("TimeStep4HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['TimeStep4HHMM'])) {
      $form_state['storage']['ajax']['TimeStep4HHMM'] = '';
    }
    $form['ajax']['TimeStep4HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['TimeStep4HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');


    if (!isset($form_state['storage']['ajax']['ChargeStep4'])) {
      $form_state['storage']['ajax']['ChargeStep4'] = '0.00';
    }
    $form['ajax']['ChargeStep4'] =
      array('#type' => 'textfield',
	    '#maxlength' => 10,
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['ChargeStep4'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td></tr>');

    // Period 5
    $form['ajax']['Period5'] =
      array('#type' => 'item',
	    '#title' => t('Period 5'),
	    '#prefix' => '<tr><td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Length');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceL5=function(context){
      	    $("#edit-ajax-Length5HHMM:not(.Length5HHMM-processed)",
              context).addClass("Length5HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['Length5HHMM'])) {
      $form_state['storage']['ajax']['Length5HHMM'] = '';
    }
    $form['ajax']['Length5HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['Length5HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Time step');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceTS5=function(context){
      	    $("#edit-ajax-TimeStep5HHMM:not(.TimeStep5HHMM-processed)",
              context).addClass("TimeStep5HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['TimeStep5HHMM'])) {
      $form_state['storage']['ajax']['TimeStep5HHMM'] = '';
    }
    $form['ajax']['TimeStep5HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['TimeStep5HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');


    if (!isset($form_state['storage']['ajax']['ChargeStep5'])) {
      $form_state['storage']['ajax']['ChargeStep5'] = '0.00';
    }
    $form['ajax']['ChargeStep5'] =
      array('#type' => 'textfield',
	    '#maxlength' => 10,
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['ChargeStep5'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td></tr></table>');

  } elseif ($form_state['storage']['ajax']['Type'] === 'Pass') {

    if (!isset($form_state['storage']['ajax']['Menu'])) {
      $form_state['storage']['ajax']['Menu'] = '';
    }
    $form['ajax']['Menu'] =
      array('#type' => 'textfield',
	    '#title' => t('Meter display menu name'),
	    '#maxlength' => 20,
	    '#default_value' => $form_state['storage']['ajax']['Menu']);

    if (!isset($form_state['storage']['ajax']['Charge'])) {
      $form_state['storage']['ajax']['Charge'] = '0.00';
    }
    $form['ajax']['Charge'] =
      array('#type' => 'textfield',
	    '#title' => t('Charge'),
	    '#maxlength' => 10,
	    '#default_value' => $form_state['storage']['ajax']['Charge'],
	    '#description' => t('Format: X.YY'));

    if (!isset($form_state['storage']['ajax']['Days'])) {
      $form_state['storage']['ajax']['Days'] = '1';
    }
    $form['ajax']['Days'] =
      array('#type' => 'textfield',
	    '#title' => t('Days'),
	    '#maxlength' => 10,
	    '#default_value' => $form_state['storage']['ajax']['Days']);

    if (!isset($form_state['storage']['ajax']['Hours'])) {
      $form_state['storage']['ajax']['Hours'] = '0';
    }
    $form['ajax']['Hours'] =
      array('#type' => 'textfield',
	    '#title' => t('Hours'),
	    '#maxlength' => 10,
	    '#default_value' => $form_state['storage']['ajax']['Hours']);


    if (!isset($form_state['storage']['ajax']['Minutes'])) {
      $form_state['storage']['ajax']['Minutes'] = '0';
    }
    $form['ajax']['Minutes'] =
      array('#type' => 'textfield',
	    '#title' => t('Minutes'),
	    '#maxlength' => 10,
	    '#default_value' => $form_state['storage']['ajax']['Minutes']);

  }

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpdevice_pad_rates_add_form_validate_save'),
	  '#submit' => array('chpdevice_pad_rates_add_form_submit_save'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpdevice_pad_rates_add_form_submit_cancel'));
  return $form;
}

function chpdevice_pad_rates_add_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepadrate';
}

function chpdevice_pad_rates_add_form_validate_save($form, &$form_state) {
  if (isset($form_state['values']['ajax']['Alias']) and
      empty($form_state['values']['ajax']['Alias'])) {
    form_set_error('ajax][Alias', 'Name field is required.');
    return;
  }
  if (isset($form_state['values']['ajax']['Menu']) and
      empty($form_state['values']['ajax']['Menu'])) {
    form_set_error('ajax][Menu', 'Menu name field is required.');
    return;
  }
  if (isset($form_state['values']['ajax']['Days']) and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['Days'])) {
    form_set_error('ajax][Days', 'Days field is required.');
    return;
  }
  if (isset($form_state['values']['ajax']['Hours']) and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['Hours'])) {
    form_set_error('ajax][Hours', 'Hours field is required.');
    return;
  }
  if (isset($form_state['values']['ajax']['Minutes']) and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['Minutes'])) {
    form_set_error('ajax][Minutes', 'Minutes field is required.');
    return;
  }
  if (isset($form_state['values']['ajax']['Charge']) and
      empty($form_state['values']['ajax']['Charge'])) {
    form_set_error('ajax][Charge', 'Charge field is required.');
    return;
  }
  if (isset($form_state['values']['ajax']['Charge']) and
      !preg_match('/^\d+\.\d{2}$/', $form_state['values']['ajax']['Charge'])) {
    form_set_error('ajax][Charge', 'Charge must be correct format.');
    return;
  }
}

function chpdevice_pad_rates_add_form_submit_save($form, &$form_state) {
  $company_id = $form_state['values']['ajax']['CompanyId'];
  $property_id = $form_state['values']['ajax']['PropertyId'];
  $lot_id = $form_state['values']['ajax']['LotId'];
  $alias = check_plain($form_state['values']['ajax']['Alias']);

  if ($form_state['values']['ajax']['Type'] === 'Max') {

    $charge = $form_state['values']['ajax']['Charge'];
    $charge_id = chpprop_genRandomString20();
    $behavior = $form_state['values']['ajax']['Behavior'];
    $hhmm = $form_state['values']['ajax']['HHMM'];

    if (chdbccpark_insert_maxcharge
	($company_id, $charge_id,
	 $alias, $behavior, $hhmm, $charge,
	 $id)) {
      drupal_set_message('Could not insert max charge', 'error');
      return;
    }

  } elseif ($form_state['values']['ajax']['Type'] === 'Rate') {

    $rate_id = chpprop_genRandomString20();
    $min_time = $form_state['values']['ajax']['MinHHMM'];
    $min_charge = $form_state['values']['ajax']['MinCharge'];
    $begin = $form_state['values']['ajax']['BeginHHMM'];
    $end = $form_state['values']['ajax']['EndHHMM'];
    $days = '';
    if ($form_state['values']['ajax']['Week']['Mon']) {
      $days .= '1';
    }
    if ($form_state['values']['ajax']['Week']['Tue']) {
      $days .= '2';
    }
    if ($form_state['values']['ajax']['Week']['Wed']) {
      $days .= '3';
    }
    if ($form_state['values']['ajax']['Week']['Thu']) {
      $days .= '4';
    }
    if ($form_state['values']['ajax']['Week']['Fri']) {
      $days .= '5';
    }
    if ($form_state['values']['ajax']['Week']['Sat']) {
      $days .= '6';
    }
    if ($form_state['values']['ajax']['Week']['Sun']) {
      $days .= '7';
    }
    if (empty($days)) {
      $days = '0';
    }
    $len1 = $form_state['values']['ajax']['Length1HHMM'];
    $time1 = $form_state['values']['ajax']['TimeStep1HHMM'];
    $charge1 = $form_state['values']['ajax']['ChargeStep1'];
    $len2 = $form_state['values']['ajax']['Length2HHMM'];
    $time2 = $form_state['values']['ajax']['TimeStep2HHMM'];
    $charge2 = $form_state['values']['ajax']['ChargeStep2'];
    $len3 = $form_state['values']['ajax']['Length3HHMM'];
    $time3 = $form_state['values']['ajax']['TimeStep3HHMM'];
    $charge3 = $form_state['values']['ajax']['ChargeStep3'];
    $len4 = $form_state['values']['ajax']['Length4HHMM'];
    $time4 = $form_state['values']['ajax']['TimeStep4HHMM'];
    $charge4 = $form_state['values']['ajax']['ChargeStep4'];
    $len5 = $form_state['values']['ajax']['Length5HHMM'];
    $time5 = $form_state['values']['ajax']['TimeStep5HHMM'];
    $charge5 = $form_state['values']['ajax']['ChargeStep5'];

    if (chdbccpark_insert_rate
	($company_id, $rate_id, $alias,
	 $min_time, $min_charge, $begin, $end, $days,
	 $len1, $time1, $charge1,
	 $len2, $time2, $charge2,
	 $len3, $time3, $charge3,
	 $len4, $time4, $charge4,
	 $len5, $time5, $charge5,
	 $id)) {
      drupal_set_message('Could not insert rate', 'error');
      return;
    }

  } elseif ($form_state['values']['ajax']['Type'] === 'Pass') {

    $pass_id = chpprop_genRandomString20();
    $menu = check_plain($form_state['values']['ajax']['Menu']);

    $dd = $form_state['values']['ajax']['Days'];
    $hh = $form_state['values']['ajax']['Hours'];
    $mm = $form_state['values']['ajax']['Minutes'];
    $duration = sprintf("%02d", $dd).':'.sprintf("%02d", $hh).':'.sprintf("%02d", $mm);

    $charge = $form_state['values']['ajax']['Charge'];

    if (chdbccpark_insert_pass
	($company_id, $pass_id,
	 $alias, $duration, $charge, $menu,
	 $id)) {
      drupal_set_message('Could not insert parking pass', 'error');
      return;
    }
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepadrate';
}

function chpdevice_pad_rates_form($form_state) {
  if (isset($form_state['storage']['rate_add'])) {
    return chpdevice_pad_rates_add_form($form_state);
  }
  if (isset($form_state['storage']['detailed_edit_max'])) {
    return chpdevice_pad_rates_edit_maxcharge_form($form_state);
  }
  if (isset($form_state['storage']['detailed_edit_rate'])) {
    return chpdevice_pad_rates_edit_rate_form($form_state);
  }
  if (isset($form_state['storage']['detailed_edit_pass'])) {
    return chpdevice_pad_rates_edit_pass_form($form_state);
  }

  drupal_set_title(t('Rates'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['show_companies'] = TRUE;
  $settings['buttons']['back']['submit'] = 'chpdevice_pad_rates_form_submit_back';
  if (user_access('anai chp add pad rates')) {
    $settings['buttons']['add']['submit'] = 'chpdevice_pad_rates_form_submit_add';
  }
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];

  $entries = chdbccpark_get_maxcharges($company_id);
  $form['ajax']['maxcharge'] = array('#type' => 'fieldset', '#title' => t('Maximum charges'));
  if (empty($entries)) {
    $form['ajax']['maxcharge']['list'] = array('#value' => t('Empty'));
  } else {
    $form['ajax']['maxcharge']['list'] = chpdevice_pad_rates_maxcharge_form($entries);
  }

  $entries = chdbccpark_get_rates($company_id);
  $form['ajax']['rates'] = array('#type' => 'fieldset', '#title' => t('Rates'));
  if (empty($entries)) {
    $form['ajax']['rates']['list'] = array('#value' => t('Empty'));
  } else {
    $form['ajax']['rates']['list'] = chpdevice_pad_rates_groups_form($entries);
  }

  $entries = chdbccpark_get_passes($company_id);
  $form['ajax']['passes'] = array('#type' => 'fieldset', '#title' => t('Parking passes'));
  if (empty($entries)) {
    $form['ajax']['passes']['list'] = array('#value' => t('Empty'));
  } else {
    $form['ajax']['passes']['list'] = chpdevice_pad_rates_passes_form($entries);
  }

  return $form;
}

function chpdevice_pad_rates_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepad';
}

function chpdevice_pad_rates_form_submit_add($form, &$form_state) {
  $form_state['storage']['rate_add'] = TRUE;
}

function chpdevice_pad_rates_maxcharge_form($entries) {
  global $user;
  $form['db'] = array('#type' => 'markup', '#theme' => 'chpdevice_pad_rates_maxcharge_table');

  if (user_access('anai chp edit pad rates')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }

  $looper = 0;
  foreach ($entries as $entry) {
    $form['db']['name_'.$looper] = array('#value' => decode_entities($entry['Alias']));
    $form['db']['behavior_'.$looper] = array('#value' => $entry['Behavior']);
    $form['db']['time_'.$looper] = array('#value' => $entry['Time']);
    $form['db']['charge_'.$looper] = array('#value' => $entry['Charge']);

    if ($readonly) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_editmax'.$entry['Id'],
	      '#submit' => array('chpdevice_pad_rates_form_submit'));
    }
    $looper++;
  }
  return $form;
}

function theme_chpdevice_pad_rates_maxcharge_table($form) {
  $header = array(t('Name'), t('Behavior'), t('Time'), t('Charge'), '');
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'name_')) {
      continue;
    }
    $looper = substr($key, strlen('name_'));
    $rows[] = array(drupal_render($form['name_'.$looper]),
                    drupal_render($form['behavior_'.$looper]),
                    drupal_render($form['time_'.$looper]),
                    drupal_render($form['charge_'.$looper]),
                    array('data' => drupal_render($form['edit_'.$looper]),
			  'width' => 1, 'height' => 1),
                    );
  }
  // Render Table
  return theme_table($header, $rows);
}

function chpdevice_pad_rates_groups_form($entries) {
  global $user;
  $form['db'] = array('#type' => 'markup', '#theme' => 'chpdevice_pad_rates_groups_table');

  if (user_access('anai chp edit pad rates')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }

  $looper = 0;
  foreach ($entries as $entry) {
    $form['db']['name_'.$looper] = array('#value' => decode_entities($entry['Alias']));
    $form['db']['begin_'.$looper] = array('#value' => $entry['Begin']);
    $form['db']['end_'.$looper] = array('#value' => $entry['End']);
    $form['db']['mintime_'.$looper] = array('#value' => $entry['MinTime']);
    $form['db']['mincharge_'.$looper] = array('#value' => $entry['MinCharge']);

    $form['db']['l1_'.$looper] = array('#value' => $entry['Length1']);
    $form['db']['l2_'.$looper] = array('#value' => $entry['Length2']);
    $form['db']['l3_'.$looper] = array('#value' => $entry['Length3']);
    $form['db']['l4_'.$looper] = array('#value' => $entry['Length4']);
    $form['db']['l5_'.$looper] = array('#value' => $entry['Length5']);

    if ($readonly) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_editrate'.$entry['Id'],
	      '#submit' => array('chpdevice_pad_rates_form_submit'));
    }
    $looper++;
  }
  return $form;
}

function theme_chpdevice_pad_rates_groups_table($form) {
  $header = array(t('Name'), t('Begin'), t('End'), t('Min time'), t('Min charge'),
		  t('P1'), t('P2'), t('P3'), t('P4'), t('P5'), '');
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'name_')) {
      continue;
    }
    $looper = substr($key, strlen('name_'));
    $rows[] = array(drupal_render($form['name_'.$looper]),
                    drupal_render($form['begin_'.$looper]),
                    drupal_render($form['end_'.$looper]),
                    drupal_render($form['mintime_'.$looper]),
                    drupal_render($form['mincharge_'.$looper]),
		    drupal_render($form['l1_'.$looper]),
		    drupal_render($form['l2_'.$looper]),
		    drupal_render($form['l3_'.$looper]),
		    drupal_render($form['l4_'.$looper]),
		    drupal_render($form['l5_'.$looper]),
                    array('data' => drupal_render($form['edit_'.$looper]),
			  'width' => 1, 'height' => 1),
                    );
  }
  // Render Table
  return theme_table($header, $rows);
}

function chpdevice_pad_rates_passes_form($entries) {
  global $user;
  $form['db'] = array('#type' => 'markup', '#theme' => 'chpdevice_pad_rates_passes_table');

  if (user_access('anai chp edit pad rates')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }

  $looper = 0;
  foreach ($entries as $entry) {
    $form['db']['name_'.$looper] = array('#value' => decode_entities($entry['Alias']));
    $form['db']['menu_'.$looper] = array('#value' => decode_entities($entry['MenuName']));
    $form['db']['duration_'.$looper] = array('#value' => $entry['Duration']);
    $form['db']['charge_'.$looper] = array('#value' => $entry['Charge']);

    if ($readonly) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_editpass'.$entry['Id'],
	      '#submit' => array('chpdevice_pad_rates_form_submit'));
    }
    $looper++;
  }
  return $form;
}

function theme_chpdevice_pad_rates_passes_table($form) {
  $header = array(t('Name'), t('Display'), t('Duration'), t('Charge'), '');
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'name_')) {
      continue;
    }
    $looper = substr($key, strlen('name_'));
    $rows[] = array(drupal_render($form['name_'.$looper]),
                    drupal_render($form['menu_'.$looper]),
                    drupal_render($form['duration_'.$looper]),
                    drupal_render($form['charge_'.$looper]),
                    array('data' => drupal_render($form['edit_'.$looper]),
			  'width' => 1, 'height' => 1),
                    );
  }
  // Render Table
  return theme_table($header, $rows);
}

function chpdevice_pad_rates_form_submit($form, &$form_state) {
  if ($form_state['storage']['confirm_delete']) {
    $id = $form_state['storage']['id'];
    $device = chdbccpark_get_device_id($id);

    unset($form_state['storage']);
    $form_state['redirect'] = 'chpdevicepadrate';
    return;
  }

  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_editmax')) {
      $id = substr($key, strlen('op_editmax'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['id'] = $id;
    $form_state['storage']['detailed_edit_max'] = TRUE;
    return;
  }

  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_editrate')) {
      $id = substr($key, strlen('op_editrate'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['id'] = $id;
    $form_state['storage']['detailed_edit_rate'] = TRUE;
    return;
  }

  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_editpass')) {
      $id = substr($key, strlen('op_editpass'));
      break;
    }
  }
  if ($id > 0) {
    $form_state['storage']['id'] = $id;
    $form_state['storage']['detailed_edit_pass'] = TRUE;
    return;
  }
}

function chpdevice_pad_rates_edit_maxcharge_form($form_state) {
  drupal_set_title(t('Edit maximum charge'));
  $id = $form_state['storage']['id'];
  $maxcharge = chdbccpark_get_maxcharge_id($id);

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  if (!isset($form_state['storage']['ajax']['Alias'])) {
    $form_state['storage']['ajax']['Alias'] = decode_entities($maxcharge['Alias']);
  }
  $form['ajax']['Alias'] =
    array('#type' => 'textfield',
	  '#title' => t('Name'),
	  '#maxlength' => 80,
	  '#default_value' => $form_state['storage']['ajax']['Alias']);

  if (!isset($form_state['storage']['ajax']['Charge'])) {
    $form_state['storage']['ajax']['Charge'] = $maxcharge['Charge'];
  }
  $form['ajax']['Charge'] =
    array('#type' => 'textfield',
	  '#title' => t('Charge'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['Charge'],
	  '#description' => t('Format: X.YY'));

  $options = array();
  $options['Relative'] = t('Expires after entry time pluss duration time');
  $options['Fixed'] = t('Expires at next given time on day');
  if (!isset($form_state['storage']['ajax']['Behavior'])) {
    if (in_array($maxcharge['Behavior'], array_keys($options))) {
      $form_state['storage']['ajax']['Behavior'] = $maxcharge['Behavior'];
    } else {
      $form_state['storage']['ajax']['Behavior'] = key($options);
    }
  }
  $form['ajax']['Behavior'] =
    array('#type' => 'radios',
	  '#title' => t('Behavior when maximum charge is reached'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['Behavior'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  if ($form_state['storage']['ajax']['Behavior'] === 'Relative') {
    $label = t('Duration time');
  } elseif ($form_state['storage']['ajax']['Behavior'] === 'Fixed') {
    $label = t('Time on day');
  }

  // Javascript date pickers
  drupal_add_js
    ('(function ($) {
          Drupal.behaviors.ANAIchpdevice=function(context){
      	    $("#edit-ajax-HHMM:not(.HHMM-processed)",
              context).addClass("HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:true,
                              placement:"inline",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

  if (!isset($form_state['storage']['ajax']['HHMM'])) {
    $form_state['storage']['ajax']['HHMM'] = $maxcharge['Time'];
  }
  $form['ajax']['HHMM'] =
    array('#type' => 'textfield',
	  '#default_value' => $form_state['storage']['ajax']['HHMM']);

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpdevice_pad_rates_add_form_validate_save'),
	  '#submit' => array('chpdevice_pad_rates_edit_maxcharge_form_submit_save'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpdevice_pad_rates_edit_maxcharge_form_submit_cancel'));
  return $form;
}

function chpdevice_pad_rates_edit_maxcharge_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepadrate';
}

function chpdevice_pad_rates_edit_maxcharge_form_submit_save($form, &$form_state) {
  $id = $form_state['storage']['id'];
  $maxcharge = chdbccpark_get_maxcharge_id($id);
  $company_id = $maxcharge['CompanyId'];
  $charge_id = $maxcharge['ChargeId'];

  $alias = check_plain($form_state['values']['ajax']['Alias']);
  $charge = $form_state['values']['ajax']['Charge'];
  $behavior = $form_state['values']['ajax']['Behavior'];
  $hhmm = $form_state['values']['ajax']['HHMM'];

  if (chdbccpark_update_maxcharge
      ($id, $alias, $behavior, $hhmm, $charge)) {
    drupal_set_message('Could not update max charge', 'error');
    return;
  }

  if (chdbccpark_update_maxchargemeter_altered
      ($company_id, $charge_id)) {
    drupal_set_message('Could not update max charge for altered meters', 'error');
    return;
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepadrate';
}

function chpdevice_pad_rates_edit_rate_form($form_state) {
  drupal_set_title(t('Edit rate group'));
  $id = $form_state['storage']['id'];
  $rate = chdbccpark_get_rate_id($id);

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  if (!isset($form_state['storage']['ajax']['Alias'])) {
    $form_state['storage']['ajax']['Alias'] = decode_entities($rate['Alias']);
  }
  $form['ajax']['Alias'] =
    array('#type' => 'textfield',
	  '#title' => t('Name'),
	  '#maxlength' => 80,
	  '#default_value' => $form_state['storage']['ajax']['Alias']);

  // Javascript date pickers
  $label = t('Begin time');
  drupal_add_js
    ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceBegin=function(context){
      	    $("#edit-ajax-BeginHHMM:not(.BeginHHMM-processed)",
              context).addClass("BeginHHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

  if (!isset($form_state['storage']['ajax']['BeginHHMM'])) {
    $form_state['storage']['ajax']['BeginHHMM'] = $rate['Begin'];
  }
  $form['ajax']['BeginHHMM'] =
    array('#type' => 'textfield',
	  '#title' => t('Begin time'),
	  '#default_value' => $form_state['storage']['ajax']['BeginHHMM']);

  // Javascript date pickers
  $label = t('End time');
  drupal_add_js
    ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceEnd=function(context){
      	    $("#edit-ajax-EndHHMM:not(.EndHHMM-processed)",
              context).addClass("EndHHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

  if (!isset($form_state['storage']['ajax']['EndHHMM'])) {
    $form_state['storage']['ajax']['EndHHMM'] = $rate['End'];
  }
  $form['ajax']['EndHHMM'] =
    array('#type' => 'textfield',
	  '#title' => t('End time'),
	  '#default_value' => $form_state['storage']['ajax']['EndHHMM']);

  // Javascript date pickers
  $label = t('Minimum duration');
  drupal_add_js
    ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceMin=function(context){
      	    $("#edit-ajax-MinHHMM:not(.MinHHMM-processed)",
              context).addClass("MinHHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

  if (!isset($form_state['storage']['ajax']['MinHHMM'])) {
    $form_state['storage']['ajax']['MinHHMM'] = $rate['MinTime'];
  }
  $form['ajax']['MinHHMM'] =
    array('#type' => 'textfield',
	  '#title' => t('Minimum duration'),
	  '#default_value' => $form_state['storage']['ajax']['MinHHMM']);

  if (!isset($form_state['storage']['ajax']['MinCharge'])) {
    $form_state['storage']['ajax']['MinCharge'] = $rate['MinCharge'];
  }
  $form['ajax']['MinCharge'] =
    array('#type' => 'textfield',
	  '#title' => t('Minimum charge'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['MinCharge'],
	  '#description' => t('Format: X.YY'));

  $options = array();
  $options['Mon'] = t('Monday');
  $options['Tue'] = t('Tuesday');
  $options['Wed'] = t('Wednesday');
  $options['Thu'] = t('Thursday');
  $options['Fri'] = t('Friday');
  $options['Sat'] = t('Saturday');
  $options['Sun'] = t('Sunday');
  if (!isset($form_state['storage']['ajax']['Week'])) {
    $form_state['storage']['ajax']['Week'] = array();
    for ($i = 0; $i < strlen($rate['Days']); $i++) {
      if ($rate['Days'][$i] == 1) {
	$form_state['storage']['ajax']['Week']['Mon'] = 'Mon';
      }
      if ($rate['Days'][$i] == 2) {
	$form_state['storage']['ajax']['Week']['Tue'] = 'Tue';
      }
      if ($rate['Days'][$i] == 3) {
	$form_state['storage']['ajax']['Week']['Wed'] = 'Wed';
      }
      if ($rate['Days'][$i] == 4) {
	$form_state['storage']['ajax']['Week']['Thu'] = 'Thu';
      }
      if ($rate['Days'][$i] == 5) {
	$form_state['storage']['ajax']['Week']['Fri'] = 'Fri';
      }
      if ($rate['Days'][$i] == 6) {
	$form_state['storage']['ajax']['Week']['Sat'] = 'Sat';
      }
      if ($rate['Days'][$i] == 7) {
	$form_state['storage']['ajax']['Week']['Sun'] = 'Sun';
      }
    }
  }
  $form['ajax']['Week'] =
    array('#type' => 'checkboxes',
	  '#title' => t('Weekdays'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['Week']);

  /**
   * Rate periods
   */

  // Period 1
  $form['ajax']['Period1'] =
    array('#type' => 'item',
	  '#title' => t('Period 1'),
	  '#prefix' => '<table><th></th><th>'.t('Length').'</th><th>'.t('Time step').'</th><th>'.t('Charge step').'</th><tr><td>',
	  '#suffix' => '</td>');

  // Javascript date pickers
  $label = t('Length');
  drupal_add_js
    ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceL1=function(context){
      	    $("#edit-ajax-Length1HHMM:not(.Length1HHMM-processed)",
              context).addClass("Length1HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

  if (!isset($form_state['storage']['ajax']['Length1HHMM'])) {
    $form_state['storage']['ajax']['Length1HHMM'] = $rate['Length1'];
  }
  $form['ajax']['Length1HHMM'] =
    array('#type' => 'textfield',
	  '#size' => 20,
	  '#default_value' => $form_state['storage']['ajax']['Length1HHMM'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');

  // Javascript date pickers
    $label = t('Time step');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceTS1=function(context){
      	    $("#edit-ajax-TimeStep1HHMM:not(.TimeStep1HHMM-processed)",
              context).addClass("TimeStep1HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['TimeStep1HHMM'])) {
      $form_state['storage']['ajax']['TimeStep1HHMM'] = $rate['TimeStep1'];
    }
    $form['ajax']['TimeStep1HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['TimeStep1HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');


    if (!isset($form_state['storage']['ajax']['ChargeStep1'])) {
      $form_state['storage']['ajax']['ChargeStep1'] = $rate['ChargeStep1'];
    }
    $form['ajax']['ChargeStep1'] =
      array('#type' => 'textfield',
	    '#maxlength' => 10,
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['ChargeStep1'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td></tr>');

    // Period 2
    $form['ajax']['Period2'] =
      array('#type' => 'item',
	    '#title' => t('Period 2'),
	    '#prefix' => '<tr><td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Length');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceL2=function(context){
      	    $("#edit-ajax-Length2HHMM:not(.Length2HHMM-processed)",
              context).addClass("Length2HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['Length2HHMM'])) {
      $form_state['storage']['ajax']['Length2HHMM'] = $rate['Length2'];
    }
    $form['ajax']['Length2HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['Length2HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Time step');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceTS2=function(context){
      	    $("#edit-ajax-TimeStep2HHMM:not(.TimeStep2HHMM-processed)",
              context).addClass("TimeStep2HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['TimeStep2HHMM'])) {
      $form_state['storage']['ajax']['TimeStep2HHMM'] = $rate['TimeStep2'];
    }
    $form['ajax']['TimeStep2HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['TimeStep2HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');


    if (!isset($form_state['storage']['ajax']['ChargeStep2'])) {
      $form_state['storage']['ajax']['ChargeStep2'] = $rate['ChargeStep2'];
    }
    $form['ajax']['ChargeStep2'] =
      array('#type' => 'textfield',
	    '#maxlength' => 10,
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['ChargeStep2'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td></tr>');

    // Period 3
    $form['ajax']['Period3'] =
      array('#type' => 'item',
	    '#title' => t('Period 3'),
	    '#prefix' => '<tr><td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Length');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceL3=function(context){
      	    $("#edit-ajax-Length3HHMM:not(.Length3HHMM-processed)",
              context).addClass("Length3HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['Length3HHMM'])) {
      $form_state['storage']['ajax']['Length3HHMM'] = $rate['Length3'];
    }
    $form['ajax']['Length3HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['Length3HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Time step');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceTS3=function(context){
      	    $("#edit-ajax-TimeStep3HHMM:not(.TimeStep3HHMM-processed)",
              context).addClass("TimeStep3HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['TimeStep3HHMM'])) {
      $form_state['storage']['ajax']['TimeStep3HHMM'] = $rate['TimeStep3'];
    }
    $form['ajax']['TimeStep3HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['TimeStep3HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');


    if (!isset($form_state['storage']['ajax']['ChargeStep3'])) {
      $form_state['storage']['ajax']['ChargeStep3'] = $rate['ChargeStep3'];
    }
    $form['ajax']['ChargeStep3'] =
      array('#type' => 'textfield',
	    '#maxlength' => 10,
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['ChargeStep3'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td></tr>');

    // Period 4
    $form['ajax']['Period4'] =
      array('#type' => 'item',
	    '#title' => t('Period 4'),
	    '#prefix' => '<tr><td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Length');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceL4=function(context){
      	    $("#edit-ajax-Length4HHMM:not(.Length4HHMM-processed)",
              context).addClass("Length4HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['Length4HHMM'])) {
      $form_state['storage']['ajax']['Length4HHMM'] = $rate['Length4'];
    }
    $form['ajax']['Length4HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['Length4HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Time step');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceTS4=function(context){
      	    $("#edit-ajax-TimeStep4HHMM:not(.TimeStep4HHMM-processed)",
              context).addClass("TimeStep4HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['TimeStep4HHMM'])) {
      $form_state['storage']['ajax']['TimeStep4HHMM'] = $rate['TimeStep4'];
    }
    $form['ajax']['TimeStep4HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['TimeStep4HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');


    if (!isset($form_state['storage']['ajax']['ChargeStep4'])) {
      $form_state['storage']['ajax']['ChargeStep4'] = $rate['ChargeStep4'];
    }
    $form['ajax']['ChargeStep4'] =
      array('#type' => 'textfield',
	    '#maxlength' => 10,
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['ChargeStep4'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td></tr>');

    // Period 5
    $form['ajax']['Period5'] =
      array('#type' => 'item',
	    '#title' => t('Period 5'),
	    '#prefix' => '<tr><td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Length');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceL5=function(context){
      	    $("#edit-ajax-Length5HHMM:not(.Length5HHMM-processed)",
              context).addClass("Length5HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['Length5HHMM'])) {
      $form_state['storage']['ajax']['Length5HHMM'] = $rate['Length5'];
    }
    $form['ajax']['Length5HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['Length5HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');

    // Javascript date pickers
    $label = t('Time step');
    drupal_add_js
      ('(function ($) {
          Drupal.behaviors.ANAIchpdeviceTS5=function(context){
      	    $("#edit-ajax-TimeStep5HHMM:not(.TimeStep5HHMM-processed)",
              context).addClass("TimeStep5HHMM-processed")
              .AnyTime_picker({format:"%H:%i",hideInput:false,
                              placement:"popup",askSecond:false,
                              labelTitle:"'.$label.'"});
          };
       }(jQuery));', 'inline');

    if (!isset($form_state['storage']['ajax']['TimeStep5HHMM'])) {
      $form_state['storage']['ajax']['TimeStep5HHMM'] = $rate['TimeStep5'];
    }
    $form['ajax']['TimeStep5HHMM'] =
      array('#type' => 'textfield',
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['TimeStep5HHMM'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>');


    if (!isset($form_state['storage']['ajax']['ChargeStep5'])) {
      $form_state['storage']['ajax']['ChargeStep5'] = $rate['ChargeStep5'];
    }
    $form['ajax']['ChargeStep5'] =
      array('#type' => 'textfield',
	    '#maxlength' => 10,
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['ChargeStep5'],
	    '#prefix' => '<td>',
	    '#suffix' => '</td></tr></table>');

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpdevice_pad_rates_add_form_validate_save'),
	  '#submit' => array('chpdevice_pad_rates_edit_rate_form_submit_save'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpdevice_pad_rates_edit_rate_form_submit_cancel'));
  return $form;
}

function chpdevice_pad_rates_edit_rate_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepadrate';
}

function chpdevice_pad_rates_edit_rate_form_submit_save($form, &$form_state) {
  $id = $form_state['storage']['id'];
  $rate = chdbccpark_get_rate_id($id);
  $company_id = $rate['CompanyId'];
  $rate_id = $rate['RateId'];

  $alias = check_plain($form_state['values']['ajax']['Alias']);
  $min_time = $form_state['values']['ajax']['MinHHMM'];
  $min_charge = $form_state['values']['ajax']['MinCharge'];
  $begin = $form_state['values']['ajax']['BeginHHMM'];
  $end = $form_state['values']['ajax']['EndHHMM'];
  $days = '';
  if ($form_state['values']['ajax']['Week']['Mon']) {
    $days .= '1';
  }
  if ($form_state['values']['ajax']['Week']['Tue']) {
    $days .= '2';
  }
  if ($form_state['values']['ajax']['Week']['Wed']) {
    $days .= '3';
  }
  if ($form_state['values']['ajax']['Week']['Thu']) {
    $days .= '4';
  }
  if ($form_state['values']['ajax']['Week']['Fri']) {
    $days .= '5';
  }
  if ($form_state['values']['ajax']['Week']['Sat']) {
    $days .= '6';
  }
  if ($form_state['values']['ajax']['Week']['Sun']) {
    $days .= '7';
  }
  if (empty($days)) {
    $days = '0';
  }
  $len1 = $form_state['values']['ajax']['Length1HHMM'];
  $time1 = $form_state['values']['ajax']['TimeStep1HHMM'];
  $charge1 = $form_state['values']['ajax']['ChargeStep1'];
  $len2 = $form_state['values']['ajax']['Length2HHMM'];
  $time2 = $form_state['values']['ajax']['TimeStep2HHMM'];
  $charge2 = $form_state['values']['ajax']['ChargeStep2'];
  $len3 = $form_state['values']['ajax']['Length3HHMM'];
  $time3 = $form_state['values']['ajax']['TimeStep3HHMM'];
  $charge3 = $form_state['values']['ajax']['ChargeStep3'];
  $len4 = $form_state['values']['ajax']['Length4HHMM'];
  $time4 = $form_state['values']['ajax']['TimeStep4HHMM'];
  $charge4 = $form_state['values']['ajax']['ChargeStep4'];
  $len5 = $form_state['values']['ajax']['Length5HHMM'];
  $time5 = $form_state['values']['ajax']['TimeStep5HHMM'];
  $charge5 = $form_state['values']['ajax']['ChargeStep5'];

  if (chdbccpark_update_rate
      ($id, $alias,
       $min_time, $min_charge, $begin, $end, $days,
       $len1, $time1, $charge1,
       $len2, $time2, $charge2,
       $len3, $time3, $charge3,
       $len4, $time4, $charge4,
       $len5, $time5, $charge5)) {
    drupal_set_message('Could not update rate', 'error');
    return;
  }

  if (chdbccpark_update_ratemeter_altered
      ($company_id, $rate_id)) {
    drupal_set_message('Could not update rate for altered meters', 'error');
    return;
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepadrate';
}

function chpdevice_pad_rates_edit_pass_form($form_state) {
  drupal_set_title(t('Edit parking pass'));
  $id = $form_state['storage']['id'];
  $pass = chdbccpark_get_pass_id($id);

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  if (!isset($form_state['storage']['ajax']['Alias'])) {
    $form_state['storage']['ajax']['Alias'] = decode_entities($pass['Alias']);
  }
  $form['ajax']['Alias'] =
    array('#type' => 'textfield',
	  '#title' => t('Name'),
	  '#maxlength' => 80,
	  '#default_value' => $form_state['storage']['ajax']['Alias']);

  if (!isset($form_state['storage']['ajax']['Menu'])) {
    $form_state['storage']['ajax']['Menu'] = decode_entities($pass['MenuName']);
  }
  $form['ajax']['Menu'] =
    array('#type' => 'textfield',
	  '#title' => t('Meter display menu name'),
	  '#maxlength' => 20,
	  '#default_value' => $form_state['storage']['ajax']['Menu']);

  if (!isset($form_state['storage']['ajax']['Charge'])) {
    $form_state['storage']['ajax']['Charge'] = $pass['Charge'];
  }
  $form['ajax']['Charge'] =
    array('#type' => 'textfield',
	  '#title' => t('Charge'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['Charge'],
	  '#description' => t('Format: X.YY'));

  if (!isset($form_state['storage']['ajax']['Days'])) {
    $form_state['storage']['ajax']['Days'] = substr($pass['Duration'], 0, 2);
  }
  $form['ajax']['Days'] =
    array('#type' => 'textfield',
	  '#title' => t('Days'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['Days']);

  if (!isset($form_state['storage']['ajax']['Hours'])) {
    $form_state['storage']['ajax']['Hours'] = substr($pass['Duration'], 3, 2);
  }
  $form['ajax']['Hours'] =
    array('#type' => 'textfield',
	  '#title' => t('Hours'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['Hours']);


  if (!isset($form_state['storage']['ajax']['Minutes'])) {
    $form_state['storage']['ajax']['Minutes'] = substr($pass['Duration'], 6, 2);
  }
  $form['ajax']['Minutes'] =
    array('#type' => 'textfield',
	  '#title' => t('Minutes'),
	  '#maxlength' => 10,
	  '#default_value' => $form_state['storage']['ajax']['Minutes']);

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpdevice_pad_rates_add_form_validate_save'),
	  '#submit' => array('chpdevice_pad_rates_edit_pass_form_submit_save'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpdevice_pad_rates_edit_pass_form_submit_cancel'));
  return $form;
}

function chpdevice_pad_rates_edit_pass_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepadrate';
}

function chpdevice_pad_rates_edit_pass_form_submit_save($form, &$form_state) {
  $id = $form_state['storage']['id'];
  $pass = chdbccpark_get_pass_id($id);
  $company_id = $pass['CompanyId'];
  $pass_id = $pass['PassId'];

  $alias = check_plain($form_state['values']['ajax']['Alias']);
  $menu = check_plain($form_state['values']['ajax']['Menu']);

  $dd = $form_state['values']['ajax']['Days'];
  $hh = $form_state['values']['ajax']['Hours'];
  $mm = $form_state['values']['ajax']['Minutes'];
  $duration = sprintf("%02d", $dd).':'.sprintf("%02d", $hh).':'.sprintf("%02d", $mm);

  $charge = $form_state['values']['ajax']['Charge'];

  if (chdbccpark_update_pass
      ($id, $alias, $duration, $charge, $menu)) {
    drupal_set_message('Could not update parking pass', 'error');
    return;
  }

  if (chdbccpark_update_passmeter_altered
      ($company_id, $pass_id)) {
    drupal_set_message('Could not update pass for altered meters', 'error');
    return;
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicepadrate';
}


