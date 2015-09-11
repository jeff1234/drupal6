<?php



/**********
 ********** Securepark add
 ********** Address: chpdevicespadd
 ********** Access:  'anai chp add securepark'
 *********/



function chpdevice_securepark_add_form($form_state) {
  drupal_set_title(t('Add Securepark LPR Camera'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['show_companies'] = TRUE;
  $settings['show_properties'] = TRUE;
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $property_id = $form_state['storage']['ajax']['PropertyId'];

  $options = array();
  $entryexit = array();
  $apps = array();
  foreach (chdbprop_get_all_lanes($property_id) as $lane) {
    if (!isset($apps[$lane['AppId']])) {
      $apps[$lane['AppId']] = chdbprop_get_mobileapp($lane['AppId']);
    }
    if (!$apps[$lane['AppId']]['SecureparkPolicy']) {
      continue;
    }
    $options[$lane['LaneId']] = decode_entities($lane['Alias']);
    if ($lane['Entry'] and $lane['Exit']) {
      $entryexit[$lane['LaneId']] = TRUE;
    } else {
      $entryexit[$lane['LaneId']] = FALSE;
    }
  }

  if (empty($options)) {
    $form['ajax']['error'] =
      array('#value' => '<p>'.t('Lane is missing, impossible to continue.').'<p>');
    $form['ajax']['back'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpdevice_securepark_add_form_submit_cancel'));
    return $form;
  }

  if (!isset($form_state['storage']['ajax']['LaneId'])) {
    $form_state['storage']['ajax']['LaneId'] = key($options);
  }
  $form['ajax']['LaneId'] =
    array('#type' => 'select',
	  '#title' => t('Lane'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['LaneId'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  if ($entryexit[$form_state['storage']['ajax']['LaneId']]) {
    $options = array();
    $options['Entry'] = t('Entry');
    $options['Exit'] = t('Exit');
    if (!isset($form_state['storage']['ajax']['EntryExit'])) {
      $form_state['storage']['ajax']['EntryExit'] = key($options);
    }
    $form['ajax']['EntryExit'] =
      array('#type' => 'radios',
	    '#title' => t('Camera direction'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['EntryExit']);
  }

  if (!isset($form_state['storage']['ajax']['IPAddr'])) {
    $form_state['storage']['ajax']['IPAddr'] = '';
  }
  $form['ajax']['IPAddr'] =
    array('#type' => 'textfield',
	  '#title' => t('Camera LPR-LAN IP address'),
	  '#default_value' => $form_state['storage']['ajax']['IPAddr'],
	  '#maxlength' => 20);

  if (!isset($form_state['storage']['ajax']['Port'])) {
    $form_state['storage']['ajax']['Port'] = 80;
  }
  $form['ajax']['Port'] =
    array('#type' => 'textfield',
	  '#title' => t('Camera TCP port'),
	  '#default_value' => $form_state['storage']['ajax']['Port'],
	  '#maxlength' => 10);

  if (!isset($form_state['storage']['ajax']['StreamPort'])) {
    $form_state['storage']['ajax']['StreamPort'] = 9901;
  }
  $form['ajax']['StreamPort'] =
    array('#type' => 'textfield',
	  '#title' => t('Camera stream port'),
	  '#default_value' => $form_state['storage']['ajax']['StreamPort'],
	  '#maxlength' => 10);

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpdevice_securepark_add_form_validate_save'),
	  '#submit' => array('chpdevice_securepark_add_form_submit_save'));

  $form['ajax']['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpdevice_securepark_add_form_submit_cancel'));
  return $form;
}

function chpdevice_securepark_add_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicesp';
}

function chpdevice_securepark_add_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['ajax']['IPAddr'])) {
    form_set_error('ajax][IPAddr', t('Camera LPR-LAN IP address field is required.'));
    return;
  }
  if (empty($form_state['values']['ajax']['Port'])) {
    form_set_error('ajax][Port', t('Camera TCP port field is required.'));
    return;
  }
  if (empty($form_state['values']['ajax']['StreamPort'])) {
    form_set_error('ajax][StreamPort', t('Camera Secure port field is required.'));
    return;
  }
  if (!preg_match('/^\d+$/', $form_state['values']['ajax']['Port'])) {
    form_set_error('ajax][Port', t('Camera TCP port must be a number.'));
    return;
  }
  if (!preg_match('/^\d+$/', $form_state['values']['ajax']['StreamPort'])) {
    form_set_error('ajax][StreamPort', t('Camera secure port must be a number.'));
    return;
  }
}

function chpdevice_securepark_add_form_submit_save($form, &$form_state) {
  global $user;

  $property_id = $form_state['values']['ajax']['PropertyId'];
  $lane_id = $form_state['values']['ajax']['LaneId'];
  $lane = chdbprop_get_lane($property_id, $lane_id);

  $cluster_id = $lane['ClusterId'];
  $camera_id = chpprop_genRandomString20();
  $ip_addr = check_plain($form_state['values']['ajax']['IPAddr']);
  $port = $form_state['values']['ajax']['Port'];
  $stream_port = $form_state['values']['ajax']['StreamPort'];

  $entry = FALSE;
  $exit = FALSE;
  if ($lane['Entry'] and $lane['Exit']) {
    if ($form_state['values']['ajax']['EntryExit'] === 'Entry') {
      $entry = TRUE;
    } elseif ($form_state['values']['ajax']['EntryExit'] === 'Exit') {
      $exit = TRUE;
    }
  } elseif ($lane['Entry']) {
    $entry = TRUE;
  } elseif ($lane['Exit']) {
    $exit = TRUE;
  }

  if (chdbprop_insert_camera
      ($user->uid,
       $cluster_id, $lane_id, $camera_id,
       $ip_addr, $port, $stream_port,
       $entry, $exit, FALSE,
       $id)) {
    drupal_set_message('Could not insert camera', 'error');
    return;
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicesp';
}



/**********
 ********** Securepark
 ********** Address: chpdevicesp
 ********** Access:  'anai chp list securepark'
 *********/



function chpdevice_securepark_form($form_state) {
  if ($form_state['storage']['detailed_edit']) {
    return chpdevice_securepark_edit_form($form_state);
  }

  drupal_set_title(t('Securepark LPR'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['show_companies'] = TRUE;
  $settings['show_properties'] = TRUE;
  $settings['buttons']['back']['submit'] = 'chpdevice_securepark_form_submit_back';
  if (user_access('anai chp add securepark')) {
    $settings['buttons']['add']['submit'] = 'chpdevice_securepark_form_submit_add';
  }
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];
  $property_id = $form_state['storage']['ajax']['PropertyId'];

  $lanes = array();
  $clusters = array();
  $apps = array();
  foreach (chdbprop_get_all_lanes($property_id) as $lane) {
    if (!isset($apps[$lane['AppId']])) {
      $apps[$lane['AppId']] = chdbprop_get_mobileapp($lane['AppId']);
    }
    if (!$apps[$lane['AppId']]['SecureparkPolicy']) {
      continue;
    }
    $clusters[$lane['ClusterId']] = decode_entities($apps[$lane['AppId']]['Alias']);
    $lanes[$lane['LaneId']] = $lane;
  }

  foreach ($clusters as $cluster_id => $alias) {
    $entries = chdbprop_get_cameras_cluster($cluster_id);
    $form['ajax'][$cluster_id] = array('#type' => 'fieldset', '#title' => $alias);
    if (empty($entries)) {
      $form['ajax'][$cluster_id]['list'] = array('#value' => t('Empty.'));
    } else {
      $form['ajax'][$cluster_id]['list'] = chpdevice_securepark_table_form($entries, $lanes);
    }
  }

  return $form;
}

function chpdevice_securepark_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevice';
}

function chpdevice_securepark_form_submit_add($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicespadd';
}

function chpdevice_securepark_table_form($entries, $lanes) {
  global $user;
  $form['db'] = array('#type' => 'markup', '#theme' => 'chpdevice_securepark_table');

  if (user_access('anai chp edit securepark')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }

  $looper = 0;
  foreach ($entries as $entry) {
    $form['db']['lane_'.$looper] = array('#value' => decode_entities($lanes[$entry['LaneId']]['Alias']));
    if ($entry['PlateCamera']) {
      $type = t('LPR');
    } elseif ($entry['OverviewCamera']) {
      $type = t('Overview');
    } else {
      $type = t('Unspecified');
    }
    $form['db']['type_'.$looper] = array('#value' => $type);
    $form['db']['ipaddr_'.$looper] = array('#value' => decode_entities($entry['IPAddr']));
    $form['db']['port_'.$looper] = array('#value' => $entry['Port']);
    $form['db']['stream_'.$looper] = array('#value' => $entry['StreamPort']);
    $form['db']['entry_'.$looper] = array('#value' => $entry['Entry'] ? t('Yes') : t('No'));
    $form['db']['exit_'.$looper] = array('#value' => $entry['Exit'] ? t('Yes') : t('No'));

    if ($readonly) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_edit'.$entry['Id'],
	      '#submit' => array('chpdevice_securepark_form_submit'));
    }
    $looper++;
  }
  return $form;
}

function theme_chpdevice_securepark_table($form) {
  $header = array(t('Lane'), t('Camera type'),
		  t('IP Address'), t('Port'), t('Stream'),
		  t('Entry'), t('Exit'),
		  '');
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'ipaddr_')) {
      continue;
    }
    $looper = substr($key, strlen('ipaddr_'));
    $rows[] = array(drupal_render($form['lane_'.$looper]),
                    drupal_render($form['type_'.$looper]),
                    drupal_render($form['ipaddr_'.$looper]),
                    drupal_render($form['port_'.$looper]),
		    drupal_render($form['stream_'.$looper]),
		    drupal_render($form['entry_'.$looper]),
		    drupal_render($form['exit_'.$looper]),
                    array('data' => drupal_render($form['edit_'.$looper]),
			  'width' => 1, 'height' => 1),
                    );
  }
  return theme_table($header, $rows);
}

function chpdevice_securepark_edit_form($form_state) {
  if ($form_state['storage']['confirm_delete']) {
    return confirm_form
      ($form, 'Are you sure?', 'chpdevicesp',
       'The camera will be deleted from the system.');
  }

  $id = $form_state['storage']['id'];
  $camera = chdbprop_get_camera_id($id);
  $lane = chdbprop_get_lane($camera['PropertyId'], $camera['LaneId']);

  drupal_set_title(t('Edit camera'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  if ($lane['Entry'] and $lane['Exit']) {
    $options = array();
    $options['Entry'] = t('Entry');
    $options['Exit'] = t('Exit');
    if (!isset($form_state['storage']['ajax']['EntryExit'])) {
      if ($camera['Entry']) {
	$form_state['storage']['ajax']['EntryExit'] = 'Entry';
      } elseif ($camera['Exit']) {
	$form_state['storage']['ajax']['EntryExit'] = 'Exit';
      } else {
	$form_state['storage']['ajax']['EntryExit'] = key($options);
      }
    }
    $form['ajax']['EntryExit'] =
      array('#type' => 'radios',
	    '#title' => t('Camera direction'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['EntryExit']);
  }

  if (!isset($form_state['storage']['ajax']['IPAddr'])) {
    $form_state['storage']['ajax']['IPAddr'] = decode_entities($camera['IPAddr']);
  }
  $form['ajax']['IPAddr'] =
    array('#type' => 'textfield',
	  '#title' => t('Camera LPR-LAN IP address'),
	  '#default_value' => $form_state['storage']['ajax']['IPAddr'],
	  '#maxlength' => 20);

  if (!isset($form_state['storage']['ajax']['Port'])) {
    $form_state['storage']['ajax']['Port'] = $camera['Port'];
  }
  $form['ajax']['Port'] =
    array('#type' => 'textfield',
	  '#title' => t('Camera TCP port'),
	  '#default_value' => $form_state['storage']['ajax']['Port'],
	  '#maxlength' => 10);

  if (!isset($form_state['storage']['ajax']['StreamPort'])) {
    $form_state['storage']['ajax']['StreamPort'] = $camera['StreamPort'];
  }
  $form['ajax']['StreamPort'] =
    array('#type' => 'textfield',
	  '#title' => t('Camera stream port'),
	  '#default_value' => $form_state['storage']['ajax']['StreamPort'],
	  '#maxlength' => 10);

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpdevice_securepark_add_form_validate_save'),
	  '#submit' => array('chpdevice_securepark_edit_form_submit_save'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpdevice_securepark_edit_form_submit_cancel'));

  if (user_access('anai chp delete securepark')) {
    $form['ajax']['delete'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/delete.png',
	    '#submit' => array('chpdevice_securepark_edit_form_submit_delete'));
  }
  return $form;
}

function chpdevice_securepark_edit_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicesp';
}

function chpdevice_securepark_edit_form_submit_delete($form, &$form_state) {
  $form_state['storage']['confirm_delete'] = TRUE;
}

function chpdevice_securepark_edit_form_submit_save($form, &$form_state) {
  global $user;
  $id = $form_state['storage']['id'];
  $camera = chdbprop_get_camera_id($id);

  $ip_addr = check_plain($form_state['values']['ajax']['IPAddr']);
  $port = $form_state['values']['ajax']['Port'];
  $stream_port = $form_state['values']['ajax']['StreamPort'];

  $entry = $camera['Entry'];
  $exit = $camera['Exit'];
  if (isset($form_state['values']['ajax']['EntryExit'])) {
    $entry = FALSE;
    $exit = FALSE;
    if ($form_state['values']['ajax']['EntryExit'] === 'Entry') {
      $entry = TRUE;
    } elseif ($form_state['values']['ajax']['EntryExit'] === 'Exit') {
      $exit = TRUE;
    }
  }

  $deployment_code = 1;

  if (chdbprop_update_camera_id
      ($id,
       $ip_addr, $port, $stream_port,
       $deployment_code,
       $entry, $exit, FALSE)) {
    drupal_set_message('Could not update camera', 'error');
    return;
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chpdevicesp';
}

function chpdevice_securepark_form_submit($form, &$form_state) {
  if ($form_state['storage']['confirm_delete']) {
    $id = $form_state['storage']['id'];
    chdbprop_delete_camera_id($id);

    unset($form_state['storage']);
    $form_state['redirect'] = 'chpdevicesp';
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
}

