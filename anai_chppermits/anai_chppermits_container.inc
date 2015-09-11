<?php
// $Id$
require_once("anai_chppermits.inc");



/**********
 ********** Container add
 ********** Address: chppermcontadd
 ********** Access:  'anai chp add permit container'
 *********/



function chpperm_container_add_form($form_state) {
  drupal_set_title(t('Add container'));

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['show_companies'] = TRUE;
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];

  // Create property options
  $property_options = array();
  if (!empty($form_state['storage']['ajax']['Properties'])) {
    foreach ($form_state['storage']['ajax']['Properties'] as $property_id => $property) {
      $property_options[$property_id] =
	decode_entities($property['data']['Alias']).', '.
	decode_entities($property['data']['Street']).', '.
	decode_entities($property['data']['City']);
    }
  }

  if (empty($property_options)) {
    $form['ajax']['error'] = array('#value' => '<p>'.t('Property missing.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpperm_container_add_form_submit_cancel'));

    return $form;
  }

  // Layout category selection
  $options = array();
  $options['Container'] = t('Add permit type in a new container');
  $options['ContType'] = t('Add permit type within a container');
  $options['Merchant'] = t('Add merchant codes within a container');
  if (!isset($form_state['storage']['ajax']['Category'])) {
    $form_state['storage']['ajax']['Category'] = key($options);
  }
  $form['ajax']['Category'] =
    array('#type' => 'radios',
	  '#title' => t(''),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['Category'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  if ($form_state['storage']['ajax']['Category'] === 'Container') {
    if (!isset($form_state['storage']['ajax']['ContainerAlias'])) {
      $form_state['storage']['ajax']['ContainerAlias'] = '';
    }
    $form['ajax']['ContainerAlias'] =
      array('#type' => 'textfield',
	    '#title' => t('Container name'),
	    '#maxlength' => 60,
	    '#default_value' => $form_state['storage']['ajax']['ContainerAlias']);

    // Property/Lot selector
    $form['ajax']['property'] =
      array('#type' => 'fieldset',
	    '#title' => t('Property assignment'));

    // Layout property options
    if (isset($form_state['storage']['ajax']['property']['PropertyId']) and
	!in_array($form_state['storage']['ajax']['property']['PropertyId'],
		  array_keys($property_options))) {
      unset($form_state['storage']['ajax']['property']['PropertyId']);
    }
    if (!isset($form_state['storage']['ajax']['property']['PropertyId'])) {
      $form_state['storage']['ajax']['property']['PropertyId'] = key($property_options);
    }
    if (1 < count($property_options)) {
      $form['ajax']['property']['PropertyId'] =
	array('#type' => 'select',
	      '#title' => t('Property'),
	      '#options' => $property_options,
	      '#default_value' => $form_state['storage']['ajax']['property']['PropertyId'],
	      '#ahah' => array('event' => 'change',
			       'path' => ahah_helper_path(array('ajax')),
			       'wrapper' => 'ajax-wrapper'));
    }

    $property_id = $form_state['storage']['ajax']['property']['PropertyId'];

    // Create lot options
    $lot_options = array();
    $policies = chpprop_retrieve_policies($user->uid, $company_id);
    if (!empty($policies)) {
      if (isset($policies[$property_id])) {
	foreach ($policies[$property_id] as $lot_id => $policy) {
	  $lot_options[$lot_id] = decode_entities($policy['data']['Alias']);
	}
      }
    }

    if (empty($lot_options)) {
      $form['ajax']['error'] = array('#value' => '<p>'.t('Lot missing.').'<p>');
      $form['ajax']['cancel'] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	      '#submit' => array('chpperm_container_add_form_submit_cancel'));

      return $form;
    }

    if (isset($form_state['storage']['ajax']['property']['LotId']) and
	!in_array($form_state['storage']['ajax']['property']['LotId'],
		  array_keys($lot_options))) {
      unset($form_state['storage']['ajax']['property']['LotId']);
    }
    if (!isset($form_state['storage']['ajax']['property']['LotId'])) {
      $form_state['storage']['ajax']['property']['LotId'] = key($lot_options);
    }
    if (1 < count($lot_options)) {
      $form['ajax']['property']['LotId'] =
	array('#type' => 'select',
	      '#title' => t('Lot'),
	      '#options' => $lot_options,
	      '#default_value' => $form_state['storage']['ajax']['property']['LotId']);
    }

    $lot_id = $form_state['storage']['ajax']['property']['LotId'];

    if (1 < count($property_options) or 1 < count($lot_options)) {
      $form['ajax']['property']['addbtn'] = array('#type' => 'button', '#value' => t('Add'));
      $form['ajax']['property']['clearbtn'] = array('#type' => 'button', '#value' => t('Clear'));
      $form['ajax']['property']['allbtn'] = array('#type' => 'button', '#value' => t('All'));
      if (!isset($form_state['storage']['ajax']['property']['List'])) {
	$list = array();
	$form_state['storage']['ajax']['property']['List'] = $list;
      } else {
	$list = unserialize($form_state['storage']['ajax']['property']['List']);
      }

      if (strpos($form_state['clicked_button']['#id'], 'addbtn') !== FALSE) {
	$list[$property_id][$lot_id] = TRUE;
      }
      if (strpos($form_state['clicked_button']['#id'], 'clearbtn') !== FALSE) {
	$list = array();
      }
      if (strpos($form_state['clicked_button']['#id'], 'allbtn') !== FALSE) {
	foreach ($property_options as $propkey => $propvalue) {
	  foreach ($policies[$propkey] as $lotkey => $lotvalue) {
	    $list[$propkey][$lotkey] = TRUE;
	  }
	}
      }
    } else {
      $list[$property_id][$lot_id] = TRUE;
    }

    $form['ajax']['property']['List'] = array('#type' => 'value', '#value' => serialize($list));

    $header = array(t('Property'), t('Lot'));
    $rows = array();
    foreach ($list as $property_id => $values) {
      foreach ($values as $lot_id => $value) {
	$rows[] = array($property_options[$property_id], $lot_options[$lot_id]);
      }
    }
    $form['ajax']['property']['ListView'] = array('#value' => theme_table($header, $rows));

  } elseif ($form_state['storage']['ajax']['Category'] === 'ContType' or
	    $form_state['storage']['ajax']['Category'] === 'Merchant') {

    // Create container options
    $container_options = array();
    foreach (chpperm_retrieve_containers($user->uid, $company_id) as $container_id => $container) {
      $container_options[$container_id] = decode_entities($container['data']['Alias']);
    }

    // Sanity check container options
    if (count($container_options) == 0) {
      $form['ajax']['error'] =
	array('#value' => '<p>'.t('At least one container is needed.').'<p>');
      $form['ajax']['back'] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	      '#submit' => array('chpperm_container_add_form_submit_cancel'));
      return $form;
    }

    // Layout container options
    if (!isset($form_state['storage']['ajax']['ContainerId'])) {
      $form_state['storage']['ajax']['ContainerId'] = key($container_options);
    }
    $form['ajax']['ContainerId'] =
      array('#type' => 'select',
	    '#title' => t('Container'),
	    '#options' => $container_options,
	    '#default_value' => $form_state['storage']['ajax']['ContainerId']);
  }

  if ($form_state['storage']['ajax']['Category'] === 'Container' or
      $form_state['storage']['ajax']['Category'] === 'ContType') {
    // Layout alias field
    if (!isset($form_state['storage']['ajax']['TypeAlias'])) {
      $form_state['storage']['ajax']['TypeAlias'] = '';
    }
    $form['ajax']['TypeAlias'] =
      array('#type' => 'textfield',
	    '#title' => t('Rate name'),
	    '#maxlength' => 60,
	    '#default_value' => $form_state['storage']['ajax']['TypeAlias']);

    // Create policy options
    $options = array();
    $options['LongPolicy'] = t('Restrict to long term parking lots');
    $options['ShortPolicy'] = t('Restrict to short term parking lots');
    $options['ValetPolicy'] = t('Restrict to valet parking lots');

    // Layout policy options
    if (!isset($form_state['storage']['ajax']['Policy'])) {
      $form_state['storage']['ajax']['Policy'] = array();
    }
    $form['ajax']['Policy'] =
      array('#type' => 'checkboxes',
	    '#title' => t('Container policy restrictors'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['Policy']);

    // Create permit type options
    $options = array();
    $options['Transients'] = t('Transient permit rate');
    $options['Groups'] = t('Group account permit rate');
    $options['Members'] = t('Individual account permit rate');
    $options['Meter'] = t('Meter rate');

    // Layout permit type options
    if (!isset($form_state['storage']['ajax']['PermitType'])) {
      $form_state['storage']['ajax']['PermitType'] = key($options);//array();
    }
    $form['ajax']['PermitType'] =
      array('#type' => 'radios',//'checkboxes',
	    '#title' => t('Permit type'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['PermitType'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));

    if ($form_state['storage']['ajax']['PermitType'] === 'Groups') {
      // Create group policy options
      $options = array();
      $options['Subscribed'] = t('Subscribed permit');
      $options['Complementary'] = t('Complementary permit');

      // Layout group policy options
      if (!isset($form_state['storage']['ajax']['Charging'])) {
	$form_state['storage']['ajax']['Charging'] = key($options);
      }
      $form['ajax']['Charging'] =
	array('#type' => 'radios',
	      '#title' => t('Group permit charging'),
	      '#options' => $options,
	      '#default_value' => $form_state['storage']['ajax']['Charging'],
	      '#ahah' => array('event' => 'change',
			       'path' => ahah_helper_path(array('ajax')),
			       'wrapper' => 'ajax-wrapper'));

    }

    if ($form_state['storage']['ajax']['PermitType'] === 'Transients' or
	$form_state['storage']['ajax']['PermitType'] === 'Members' or
	$form_state['storage']['ajax']['PermitType'] === 'Groups') {
      // Create permit options
      $options = array();
      $options['Linear'] = chpperm_structure_name('Linear');
      $options['Flat'] = chpperm_structure_name('Flat');
      $options['Fixed'] = chpperm_structure_name('Fixed');

      // Layout permit options
      if (!isset($form_state['storage']['ajax']['RateType'])) {
	$form_state['storage']['ajax']['RateType'] = key($options);
      }
      $form['ajax']['RateType'] =
	array('#type' => 'select',
	      '#title' => t('Rate structure'),
	      '#options' => $options,
	      '#default_value' => $form_state['storage']['ajax']['RateType'],
	      '#ahah' => array('event' => 'change',
			       'path' => ahah_helper_path(array('ajax')),
			       'wrapper' => 'ajax-wrapper'));

      // Layout unit cost value
      if (!isset($form_state['storage']['ajax']['UnitValue'])) {
	$form_state['storage']['ajax']['UnitValue'] = '0.00';
      }
      $form['ajax']['UnitValue'] =
	array('#type' => 'textfield',
	      '#title' => t('Cost'),
	      '#maxlength' => 10,
	      '#default_value' => $form_state['storage']['ajax']['UnitValue'],
	      '#description' => t('Format: x.xx'));

      if ($form_state['storage']['ajax']['RateType'] === 'Linear') {

	// Create value unit options
	$options = array();
	$options['Minute'] = t('Minutes');
	$options['Hour'] = t('Per hour');
	$options['Day'] = t('per day');
	$options['Week'] = t('per week');
	$options['Month'] = t('Per month');
 
	// Layout value unit options
	if (!in_array($form_state['storage']['ajax']['ValueUnit'], array_keys($options))) {
	  unset($form_state['storage']['ajax']['ValueUnit']);
	}
	if (!isset($form_state['storage']['ajax']['ValueUnit'])) {
	  $form_state['storage']['ajax']['ValueUnit'] = key($options);
	}
	$form['ajax']['ValueUnit'] =
	  array('#type' => 'select',
		'#options' => $options,
		'#default_value' => $form_state['storage']['ajax']['ValueUnit']);

	// Layout max cost field
	if (!isset($form_state['storage']['ajax']['MaxCost'])) {
	  $form_state['storage']['ajax']['MaxCost'] = '';
	}
	$form['ajax']['MaxCost'] =
	  array('#type' => 'textfield',
		'#title' => t('Maximum cost of permit'),
		'#maxlength' => 10,
		'#default_value' => $form_state['storage']['ajax']['MaxCost'],
		'#description' => t('Format: x.xx'));

      } elseif ($form_state['storage']['ajax']['RateType'] === 'Fixed') {

	// Layout fixed period field
	if (!isset($form_state['storage']['ajax']['FixedPeriod'])) {
	  $form_state['storage']['ajax']['FixedPeriod'] = '';
	}
	$form['ajax']['FixedPeriod'] =
	  array('#type' => 'textfield',
		'#title' => t('Duration'),
		'#maxlength' => 10,
		'#default_value' => $form_state['storage']['ajax']['FixedPeriod']);

	// Create value unit options
	$options = array();
	$options['Minute'] = t('Minutes');
	$options['Hour'] = t('Hours');
	$options['Day'] = t('Days');
	$options['Week'] = t('Weeks');
	$options['Month'] = t('Months');

	// Layout value unit options
	if (!isset($form_state['storage']['ajax']['ValueUnit'])) {
	  $form_state['storage']['ajax']['ValueUnit'] = key($options);
	}
	$form['ajax']['ValueUnit'] =
	  array('#type' => 'select',
		'#options' => $options,
		'#default_value' => $form_state['storage']['ajax']['ValueUnit']);
      }

      if (!isset($form_state['storage']['ajax']['Comment'])) {
	$form_state['storage']['ajax']['Comment'] = '';
      }
      $form['ajax']['Comment'] =
	array('#type' => 'textfield',
	      '#title' => t('Comment'),
	      '#maxlength' => 120,
	      '#default_value' => $form_state['storage']['ajax']['Comment'],
	      '#description' =>
	      t('Displayed when creating new permit').'<br>'.
	      t('Example 1: Cost of permit is $6.00 flat rate').'<br>'.
	      t('Example 2: Cost of permit is $6.00 per hour').'<br>'.
	      t('Example 3: Complementary until end of next month'));
    }

    // Add meter rate
    if ($form_state['storage']['ajax']['PermitType'] === 'Meter') {
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
          Drupal.behaviors.ANAIchpcontainer=function(context){
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
          Drupal.behaviors.ANAIchpcontainerBegin=function(context){
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
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['BeginHHMM'],
		'#prefix' => '<table><tr><td>');

	// Javascript date pickers
	$label = t('End time');
	drupal_add_js
	  ('(function ($) {
          Drupal.behaviors.ANAIchpcontainerEnd=function(context){
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
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['EndHHMM'],
		'#suffix' => '</td>');

	// Javascript date pickers
	$label = t('Minimum duration');
	drupal_add_js
	  ('(function ($) {
          Drupal.behaviors.ANAIchpcontainerMin=function(context){
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
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MinHHMM'],
		'#prefix' => '<td>');

	if (!isset($form_state['storage']['ajax']['MinCharge'])) {
	  $form_state['storage']['ajax']['MinCharge'] = '0.00';
	}
	$form['ajax']['MinCharge'] =
	  array('#type' => 'textfield',
		'#title' => t('Minimum charge'),
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MinCharge'],
		'#description' => t('Format: X.YY'),
		'#suffix' => '</td>');

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
		'#default_value' => $form_state['storage']['ajax']['Week'],
		'#prefix' => '<td>',
		'#suffix' => '</td></tr></table>');

	/**
	 * Rate periods
	 */

	// Period 1
	$form['ajax']['Period1'] =
	  array('#type' => 'item',
		'#title' => t('P1'),
		'#prefix' => '<table><th></th>'.
		'<th>'.t('Length').'</th>'.
		'<th>'.t('Time step').'</th>'.
		'<th>'.t('Charge step').'</th>'.
		'<th>'.t('Cut-Off Time').'</th>'.
		'<th>'.t('Max Charge').'</th>'.
		'<th>'.t('Max Time').'</th>'.
		'<tr><td>',
		'#suffix' => '</td>');

	if (!isset($form_state['storage']['ajax']['Length1HHMM'])) {
	  $form_state['storage']['ajax']['Length1HHMM'] = '';
	}
	$form['ajax']['Length1HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['Length1HHMM'],
		'#description' => t('Format: HH:MM<br>(Blank if not used)'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['TimeStep1HHMM'])) {
	  $form_state['storage']['ajax']['TimeStep1HHMM'] = '';
	}
	$form['ajax']['TimeStep1HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['TimeStep1HHMM'],
		'#description' => t('Format: HH:MM'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['ChargeStep1'])) {
	  $form_state['storage']['ajax']['ChargeStep1'] = '0.00';
	}
	$form['ajax']['ChargeStep1'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['ChargeStep1'],
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['CutOffTime1'])) {
	  $form_state['storage']['ajax']['CutOffTime1'] = '';
	}
	$form['ajax']['CutOffTime1'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['CutOffTime1'],
		'#description' => t('Format: HH:MM<br>(Blank if not used)'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['MaxCharge1'])) {
	  $form_state['storage']['ajax']['MaxCharge1'] = '0.00';
	}
	$form['ajax']['MaxCharge1'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MaxCharge1'],
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['MaxTime1HHMM'])) {
	  $form_state['storage']['ajax']['MaxTime1HHMM'] = '';
	}
	$form['ajax']['MaxTime1HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MaxTime1HHMM'],
		'#description' => t('Format: HH:MM'),
		'#prefix' => '<td>',
		'#suffix' => '</td></tr>');

	// Period 2
	$form['ajax']['Period2'] =
	  array('#type' => 'item',
		'#title' => t('P2'),
		'#prefix' => '<tr><td>',
		'#suffix' => '</td>');

	if (!isset($form_state['storage']['ajax']['Length2HHMM'])) {
	  $form_state['storage']['ajax']['Length2HHMM'] = '';
	}
	$form['ajax']['Length2HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['Length2HHMM'],
		'#description' => t('Format: HH:MM<br>(Blank if not used)'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['TimeStep2HHMM'])) {
	  $form_state['storage']['ajax']['TimeStep2HHMM'] = '';
	}
	$form['ajax']['TimeStep2HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['TimeStep2HHMM'],
		'#description' => t('Format: HH:MM'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['ChargeStep2'])) {
	  $form_state['storage']['ajax']['ChargeStep2'] = '0.00';
	}
	$form['ajax']['ChargeStep2'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['ChargeStep2'],
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['CutOffTime2'])) {
	  $form_state['storage']['ajax']['CutOffTime2'] = '';
	}
	$form['ajax']['CutOffTime2'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['CutOffTime2'],
		'#description' => t('Format: HH:MM<br>(Blank if not used)'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['MaxCharge2'])) {
	  $form_state['storage']['ajax']['MaxCharge2'] = '0.00';
	}
	$form['ajax']['MaxCharge2'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MaxCharge2'],
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['MaxTime2HHMM'])) {
	  $form_state['storage']['ajax']['MaxTime2HHMM'] = '';
	}
	$form['ajax']['MaxTime2HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MaxTime2HHMM'],
		'#description' => t('Format: HH:MM'),
		'#prefix' => '<td>',
		'#suffix' => '</td></tr>');

	// Period 3
	$form['ajax']['Period3'] =
	  array('#type' => 'item',
		'#title' => t('P3'),
		'#prefix' => '<tr><td>',
		'#suffix' => '</td>');

	if (!isset($form_state['storage']['ajax']['Length3HHMM'])) {
	  $form_state['storage']['ajax']['Length3HHMM'] = '';
	}
	$form['ajax']['Length3HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['Length3HHMM'],
		'#description' => t('Format: HH:MM<br>(Blank if not used)'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['TimeStep3HHMM'])) {
	  $form_state['storage']['ajax']['TimeStep3HHMM'] = '';
	}
	$form['ajax']['TimeStep3HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['TimeStep3HHMM'],
		'#description' => t('Format: HH:MM'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['ChargeStep3'])) {
	  $form_state['storage']['ajax']['ChargeStep3'] = '0.00';
	}
	$form['ajax']['ChargeStep3'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['ChargeStep3'],
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['CutOffTime3'])) {
	  $form_state['storage']['ajax']['CutOffTime3'] = '';
	}
	$form['ajax']['CutOffTime3'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['CutOffTime3'],
		'#description' => t('Format: HH:MM<br>(Blank if not used)'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['MaxCharge3'])) {
	  $form_state['storage']['ajax']['MaxCharge3'] = '0.00';
	}
	$form['ajax']['MaxCharge3'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MaxCharge3'],
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['MaxTime3HHMM'])) {
	  $form_state['storage']['ajax']['MaxTime3HHMM'] = '';
	}
	$form['ajax']['MaxTime3HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MaxTime3HHMM'],
		'#description' => t('Format: HH:MM'),
		'#prefix' => '<td>',
		'#suffix' => '</td></tr>');

	// Period 4
	$form['ajax']['Period4'] =
	  array('#type' => 'item',
		'#title' => t('P4'),
		'#prefix' => '<tr><td>',
		'#suffix' => '</td>');

	if (!isset($form_state['storage']['ajax']['Length4HHMM'])) {
	  $form_state['storage']['ajax']['Length4HHMM'] = '';
	}
	$form['ajax']['Length4HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['Length4HHMM'],
		'#description' => t('Format: HH:MM<br>(Blank if not used)'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['TimeStep4HHMM'])) {
	  $form_state['storage']['ajax']['TimeStep4HHMM'] = '';
	}
	$form['ajax']['TimeStep4HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['TimeStep4HHMM'],
		'#description' => t('Format: HH:MM'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['ChargeStep4'])) {
	  $form_state['storage']['ajax']['ChargeStep4'] = '0.00';
	}
	$form['ajax']['ChargeStep4'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['ChargeStep4'],
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['CutOffTime4'])) {
	  $form_state['storage']['ajax']['CutOffTime4'] = '';
	}
	$form['ajax']['CutOffTime4'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['CutOffTime4'],
		'#description' => t('Format: HH:MM<br>(Blank if not used)'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['MaxCharge4'])) {
	  $form_state['storage']['ajax']['MaxCharge4'] = '0.00';
	}
	$form['ajax']['MaxCharge4'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MaxCharge4'],
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['MaxTime4HHMM'])) {
	  $form_state['storage']['ajax']['MaxTime4HHMM'] = '';
	}
	$form['ajax']['MaxTime4HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MaxTime4HHMM'],
		'#description' => t('Format: HH:MM'),
		'#prefix' => '<td>',
		'#suffix' => '</td></tr>');

	// Period 5
	$form['ajax']['Period5'] =
	  array('#type' => 'item',
		'#title' => t('P5'),
		'#prefix' => '<tr><td>',
		'#suffix' => '</td>');

	if (!isset($form_state['storage']['ajax']['Length5HHMM'])) {
	  $form_state['storage']['ajax']['Length5HHMM'] = '';
	}
	$form['ajax']['Length5HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['Length5HHMM'],
		'#description' => t('Format: HH:MM<br>(Blank if not used)'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['TimeStep5HHMM'])) {
	  $form_state['storage']['ajax']['TimeStep5HHMM'] = '';
	}
	$form['ajax']['TimeStep5HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['TimeStep5HHMM'],
		'#description' => t('Format: HH:MM'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['ChargeStep5'])) {
	  $form_state['storage']['ajax']['ChargeStep5'] = '0.00';
	}
	$form['ajax']['ChargeStep5'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['ChargeStep5'],
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['CutOffTime5'])) {
	  $form_state['storage']['ajax']['CutOffTime5'] = '';
	}
	$form['ajax']['CutOffTime5'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['CutOffTime5'],
		'#description' => t('Format: HH:MM<br>(Blank if not used)'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['MaxCharge5'])) {
	  $form_state['storage']['ajax']['MaxCharge5'] = '0.00';
	}
	$form['ajax']['MaxCharge5'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MaxCharge5'],
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['MaxTime5HHMM'])) {
	  $form_state['storage']['ajax']['MaxTime5HHMM'] = '';
	}
	$form['ajax']['MaxTime5HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MaxTime5HHMM'],
		'#description' => t('Format: HH:MM'),
		'#prefix' => '<td>',
		'#suffix' => '</td></tr>');

	// Period 6
	$form['ajax']['Period6'] =
	  array('#type' => 'item',
		'#title' => t('P6'),
		'#prefix' => '<tr><td>',
		'#suffix' => '</td>');

	if (!isset($form_state['storage']['ajax']['Length6HHMM'])) {
	  $form_state['storage']['ajax']['Length6HHMM'] = '';
	}
	$form['ajax']['Length6HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['Length6HHMM'],
		'#description' => t('Format: HH:MM<br>(Blank if not used)'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['TimeStep6HHMM'])) {
	  $form_state['storage']['ajax']['TimeStep6HHMM'] = '';
	}
	$form['ajax']['TimeStep6HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['TimeStep6HHMM'],
		'#description' => t('Format: HH:MM'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['ChargeStep6'])) {
	  $form_state['storage']['ajax']['ChargeStep6'] = '0.00';
	}
	$form['ajax']['ChargeStep6'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['ChargeStep6'],
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['CutOffTime6'])) {
	  $form_state['storage']['ajax']['CutOffTime6'] = '';
	}
	$form['ajax']['CutOffTime6'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['CutOffTime6'],
		'#description' => t('Format: HH:MM<br>(Blank if not used)'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['MaxCharge6'])) {
	  $form_state['storage']['ajax']['MaxCharge6'] = '0.00';
	}
	$form['ajax']['MaxCharge6'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MaxCharge6'],
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['MaxTime6HHMM'])) {
	  $form_state['storage']['ajax']['MaxTime6HHMM'] = '';
	}
	$form['ajax']['MaxTime6HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MaxTime6HHMM'],
		'#description' => t('Format: HH:MM'),
		'#prefix' => '<td>',
		'#suffix' => '</td></tr>');

	// Period 7
	$form['ajax']['Period7'] =
	  array('#type' => 'item',
		'#title' => t('P7'),
		'#prefix' => '<tr><td>',
		'#suffix' => '</td>');

	if (!isset($form_state['storage']['ajax']['Length7HHMM'])) {
	  $form_state['storage']['ajax']['Length7HHMM'] = '';
	}
	$form['ajax']['Length7HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['Length7HHMM'],
		'#description' => t('Format: HH:MM<br>(Blank if not used)'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['TimeStep7HHMM'])) {
	  $form_state['storage']['ajax']['TimeStep7HHMM'] = '';
	}
	$form['ajax']['TimeStep7HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['TimeStep7HHMM'],
		'#description' => t('Format: HH:MM'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['ChargeStep7'])) {
	  $form_state['storage']['ajax']['ChargeStep7'] = '0.00';
	}
	$form['ajax']['ChargeStep7'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['ChargeStep7'],
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['CutOffTime7'])) {
	  $form_state['storage']['ajax']['CutOffTime7'] = '';
	}
	$form['ajax']['CutOffTime7'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['CutOffTime7'],
		'#description' => t('Format: HH:MM<br>(Blank if not used)'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['MaxCharge7'])) {
	  $form_state['storage']['ajax']['MaxCharge7'] = '0.00';
	}
	$form['ajax']['MaxCharge7'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MaxCharge7'],
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['MaxTime7HHMM'])) {
	  $form_state['storage']['ajax']['MaxTime7HHMM'] = '';
	}
	$form['ajax']['MaxTime7HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MaxTime7HHMM'],
		'#description' => t('Format: HH:MM'),
		'#prefix' => '<td>',
		'#suffix' => '</td></tr>');

	// Period 8
	$form['ajax']['Period8'] =
	  array('#type' => 'item',
		'#title' => t('P8'),
		'#prefix' => '<tr><td>',
		'#suffix' => '</td>');

	if (!isset($form_state['storage']['ajax']['Length8HHMM'])) {
	  $form_state['storage']['ajax']['Length8HHMM'] = '';
	}
	$form['ajax']['Length8HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['Length8HHMM'],
		'#description' => t('Format: HH:MM<br>(Blank if not used)'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['TimeStep8HHMM'])) {
	  $form_state['storage']['ajax']['TimeStep8HHMM'] = '';
	}
	$form['ajax']['TimeStep8HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['TimeStep8HHMM'],
		'#description' => t('Format: HH:MM'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['ChargeStep8'])) {
	  $form_state['storage']['ajax']['ChargeStep8'] = '0.00';
	}
	$form['ajax']['ChargeStep8'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['ChargeStep8'],
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['CutOffTime8'])) {
	  $form_state['storage']['ajax']['CutOffTime8'] = '';
	}
	$form['ajax']['CutOffTime8'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['CutOffTime8'],
		'#description' => t('Format: HH:MM<br>(Blank if not used)'),
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['MaxCharge8'])) {
	  $form_state['storage']['ajax']['MaxCharge8'] = '0.00';
	}
	$form['ajax']['MaxCharge8'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MaxCharge8'],
		'#prefix' => '<td>',
		'#suffix' => '</td>');
	if (!isset($form_state['storage']['ajax']['MaxTime8HHMM'])) {
	  $form_state['storage']['ajax']['MaxTime8HHMM'] = '';
	}
	$form['ajax']['MaxTime8HHMM'] =
	  array('#type' => 'textfield',
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['MaxTime8HHMM'],
		'#description' => t('Format: HH:MM'),
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
		'#title' => t('Flat rate charge'),
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
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['Days'],
		'#prefix' => '<table><tr><td>',
		'#suffix' => '</td>');

	if (!isset($form_state['storage']['ajax']['Hours'])) {
	  $form_state['storage']['ajax']['Hours'] = '0';
	}
	$form['ajax']['Hours'] =
	  array('#type' => 'textfield',
		'#title' => t('Hours'),
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['Hours'],
		'#prefix' => '<td>',
		'#suffix' => '</td>');


	if (!isset($form_state['storage']['ajax']['Minutes'])) {
	  $form_state['storage']['ajax']['Minutes'] = '0';
	}
	$form['ajax']['Minutes'] =
	  array('#type' => 'textfield',
		'#title' => t('Minutes'),
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['Minutes'],
		'#prefix' => '<td>',
		'#suffix' => '</td></tr></table>');
      }
    }
  
  } elseif ($form_state['storage']['ajax']['Category'] === 'Merchant') {
    // Layout number of merchant codes
    $options = array();
    for ($i = 1; $i <= 25; $i++) {
      $options[$i] = $i;
    }
    if (!isset($form_state['storage']['ajax']['NumMerchantCodes'])) {
      $form_state['storage']['ajax']['NumMerchantCodes'] = key($options);
    }
    $form['ajax']['NumMerchantCodes'] =
      array('#type' => 'select',
	    '#title' => t('Number of merchant codes'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['NumMerchantCodes'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));

    // Layout merchant code entries
    $form['ajax']['Merchants'] =
      array('#prefix' => '<table>',
	    '#suffix' => '</table>');

    for ($i = 0; $i < $form_state['storage']['ajax']['NumMerchantCodes']; $i++) {

      // Layout Code
      if (!isset($form_state['storage']['ajax']['Merchants']['Code'.$i])) {
	$form_state['storage']['ajax']['Merchants']['Code'.$i] = '';
      }
      $form['ajax']['Merchants']['Code'.$i] =
	array('#type' => 'textfield',
	      '#title' => t('Code'),
	      '#maxlength' => 15,
	      '#size' => 20,
	      '#default_value' => $form_state['storage']['ajax']['Merchants']['Code'.$i],
	      '#description' => '(No spaces)',
	      '#attributes' => array('onBlur'=>'this.value=this.value.toUpperCase()'),
	      '#prefix' => '<tr><td>',
	      '#suffix' => '</td>');

      // Layout discount type
      // Tore 131116: Added Flat and Capped
      $options = array();
      $options['Fixed'] = t('Discount amount');
      $options['Flat'] = t('Flat amount');
      $options['Capped'] = t('Capped amount');
      $options['Percentage'] = t('Discount percentage');
      if (!isset($form_state['storage']['ajax']['Merchants']['Discount'.$i])) {
	$form_state['storage']['ajax']['Merchants']['Discount'.$i] = key($options);
      }
      $form['ajax']['Merchants']['Discount'.$i] =
	array('#type' => 'select',
	      '#title' => t('Discount type'),
	      '#options' => $options,
	      '#default_value' => $form_state['storage']['ajax']['Merchants']['Discount'.$i],
	      '#prefix' => '<td>',
	      '#suffix' => '</td>',
	      '#ahah' => array('event' => 'change',
			       'path' => ahah_helper_path(array('ajax')),
			       'wrapper' => 'ajax-wrapper'));

      if ($form_state['storage']['ajax']['Merchants']['Discount'.$i] === 'Fixed' or
	  $form_state['storage']['ajax']['Merchants']['Discount'.$i] === 'Flat' or
	  $form_state['storage']['ajax']['Merchants']['Discount'.$i] === 'Capped') {
	// Layout amount seletor
	// Tore 131126: Changed from select widget to textfield widget
	if (!isset($form_state['storage']['ajax']['Merchants']['Amount'.$i])) {
	  $form_state['storage']['ajax']['Merchants']['Amount'.$i] = '';
	}
	$form['ajax']['Merchants']['Amount'.$i] =
	  array('#type' => 'textfield',
		'#title' => t('Amount'),
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['Merchants']['Amount'.$i],
		'#prefix' => '<td>',
		'#suffix' => '</td></tr>');
      } elseif ($form_state['storage']['ajax']['Merchants']['Discount'.$i] === 'Percentage') {
	// Layout percentage seletor
	// Tore 131201: Changed from select widget to textfield widget
	if (!isset($form_state['storage']['ajax']['Merchants']['Percentage'.$i])) {
	  $form_state['storage']['ajax']['Merchants']['Percentage'.$i] = '';
	}
	$form['ajax']['Merchants']['Percentage'.$i] =
	  array('#type' => 'textfield',
		'#title' => t('Percentage'),
		'#maxlength' => 10,
		'#size' => 15,
		'#default_value' => $form_state['storage']['ajax']['Merchants']['Percentage'.$i],
		'#prefix' => '<td>',
		'#suffix' => '</td></tr>');
      }
    }
  }

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpperm_container_add_form_validate_save'),
	  '#submit' => array('chpperm_container_add_form_submit_save'));

  $form['ajax']['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpperm_container_add_form_submit_cancel'));
  return $form;
}

function chpperm_container_add_form_validate_save($form, &$form_state) {
  if (isset($form_state['values']['ajax']['ContainerAlias']) and
      empty($form_state['values']['ajax']['ContainerAlias'])) {
    form_set_error('ajax][ContainerAlias', 'Container name field is required.');
    return;
  }

  if (isset($form_state['values']['ajax']['TypeAlias']) and
      empty($form_state['values']['ajax']['TypeAlias'])) {
    form_set_error('ajax][TypeAlias', 'Permit type name field is required.');
    return;
  }

  if (isset($form_state['values']['ajax']['UnitValue']) and
      empty($form_state['values']['ajax']['UnitValue'])) {
    form_set_error('ajax][UnitValue', 'Cost field is required (Use 0.00 if not wanted).');
    return;
  }

  if (isset($form_state['values']['ajax']['UnitValue']) and
      !preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['UnitValue'])) {
    form_set_error('ajax][UnitValue', 'Must be given in format x.xx');
    return;
  }

  if (isset($form_state['values']['ajax']['MaxCost']) and
      empty($form_state['values']['ajax']['MaxCost'])) {
    form_set_error('ajax][MaxCost', 'Maximum cost of permit field is required.');
    return;
  }

  if (isset($form_state['values']['ajax']['MaxCost']) and
      !preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['MaxCost'])) {
    form_set_error('ajax][MaxCost', 'Maximum cost of permit must be given in format x.xx');
    return;
  }

  if (isset($form_state['values']['ajax']['FixedPeriod']) and
      empty($form_state['values']['ajax']['FixedPeriod'])) {
    form_set_error('ajax][FixedPeriod', 'Fixed period length field is required.');
    return;
  }

  if (isset($form_state['values']['ajax']['FixedPeriod']) and
      !preg_match('/^\d+$/', $form_state['values']['ajax']['FixedPeriod'])) {
    form_set_error('ajax][FixedPeriod', 'Must be a number');
    return;
  }

  if (isset($form_state['values']['ajax']['Charge']) and
      empty($form_state['values']['ajax']['Charge'])) {
    form_set_error('ajax][Charge', 'Charge field is required.');
    return;
  }

  if (isset($form_state['values']['ajax']['Charge']) and
      !preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['Charge'])) {
    form_set_error('ajax][Charge', 'Charge must be given in format x.xx');
    return;
  }

  if (isset($form_state['values']['ajax']['MinCharge']) and
      empty($form_state['values']['ajax']['MinCharge'])) {
    form_set_error('ajax][MinCharge', 'Minimum charge field is required.');
    return;
  }

  if (isset($form_state['values']['ajax']['MinCharge']) and
      !preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['MinCharge'])) {
    form_set_error('ajax][MinCharge', 'Minimum charge must be given in format x.xx');
    return;
  }

  if (isset($form_state['values']['ajax']['Length1HHMM']) and
      !empty($form_state['values']['ajax']['Length1HHMM'])) {
    if (isset($form_state['values']['ajax']['Length1HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['Length1HHMM'])) {
      form_set_error('ajax][Length1HHMM', 'Length must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['TimeStep1HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['TimeStep1HHMM'])) {
      form_set_error('ajax][TimeStep1HHMM', 'Time step must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['ChargeStep1']) and
	!preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['ChargeStep1'])) {
      form_set_error('ajax][ChargeStep1', 'Charge step must be given in format x.xx');
      return;
    }
    if (isset($form_state['values']['ajax']['CutOffTime1']) and
	0 < strlen($form_state['values']['ajax']['CutOffTime1']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['CutOffTime1'])) {
      form_set_error('ajax][CutOffTime1', 'Cut-off time must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['MaxCharge1']) and
	!preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['MaxCharge1'])) {
      form_set_error('ajax][MaxCharge1', 'Maximum charge must be given in format x.xx');
      return;
    }
    if (isset($form_state['values']['ajax']['MaxTime1HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['MaxTime1HHMM'])) {
      form_set_error('ajax][MaxTime1HHMM', 'Maximum time must be given in format HH:MM');
      return;
    }
  }

  if (isset($form_state['values']['ajax']['Length2HHMM']) and
      !empty($form_state['values']['ajax']['Length2HHMM'])) {
    if (isset($form_state['values']['ajax']['Length2HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['Length2HHMM'])) {
      form_set_error('ajax][Length2HHMM', 'Length must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['TimeStep2HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['TimeStep2HHMM'])) {
      form_set_error('ajax][TimeStep2HHMM', 'Time step must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['ChargeStep2']) and
	!preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['ChargeStep2'])) {
      form_set_error('ajax][ChargeStep2', 'Charge step must be given in format x.xx');
      return;
    }
    if (isset($form_state['values']['ajax']['CutOffTime2']) and
	0 < strlen($form_state['values']['ajax']['CutOffTime2']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['CutOffTime2'])) {
      form_set_error('ajax][CutOffTime2', 'Cut-off time must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['MaxCharge2']) and
	!preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['MaxCharge2'])) {
      form_set_error('ajax][MaxCharge2', 'Maximum charge must be given in format x.xx');
      return;
    }
    if (isset($form_state['values']['ajax']['MaxTime2HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['MaxTime2HHMM'])) {
      form_set_error('ajax][MaxTime2HHMM', 'Maximum time must be given in format HH:MM');
      return;
    }
  }

  if (isset($form_state['values']['ajax']['Length3HHMM']) and
      !empty($form_state['values']['ajax']['Length3HHMM'])) {
    if (isset($form_state['values']['ajax']['Length3HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['Length3HHMM'])) {
      form_set_error('ajax][Length3HHMM', 'Length must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['TimeStep3HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['TimeStep3HHMM'])) {
      form_set_error('ajax][TimeStep3HHMM', 'Time step must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['ChargeStep3']) and
	!preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['ChargeStep3'])) {
      form_set_error('ajax][ChargeStep3', 'Charge step must be given in format x.xx');
      return;
    }
    if (isset($form_state['values']['ajax']['CutOffTime3']) and
	0 < strlen($form_state['values']['ajax']['CutOffTime3']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['CutOffTime3'])) {
      form_set_error('ajax][CutOffTime3', 'Cut-off time must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['MaxCharge3']) and
	!preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['MaxCharge3'])) {
      form_set_error('ajax][MaxCharge3', 'Maximum charge must be given in format x.xx');
      return;
    }
    if (isset($form_state['values']['ajax']['MaxTime3HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['MaxTime3HHMM'])) {
      form_set_error('ajax][MaxTime3HHMM', 'Maximum time must be given in format HH:MM');
      return;
    }
  }

  if (isset($form_state['values']['ajax']['Length4HHMM']) and
      !empty($form_state['values']['ajax']['Length4HHMM'])) {
    if (isset($form_state['values']['ajax']['Length4HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['Length4HHMM'])) {
      form_set_error('ajax][Length4HHMM', 'Length must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['TimeStep4HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['TimeStep4HHMM'])) {
      form_set_error('ajax][TimeStep4HHMM', 'Time step must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['ChargeStep4']) and
	!preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['ChargeStep4'])) {
      form_set_error('ajax][ChargeStep4', 'Charge step must be given in format x.xx');
      return;
    }
    if (isset($form_state['values']['ajax']['CutOffTime4']) and
	0 < strlen($form_state['values']['ajax']['CutOffTime4']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['CutOffTime4'])) {
      form_set_error('ajax][CutOffTime4', 'Cut-off time must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['MaxCharge4']) and
	!preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['MaxCharge4'])) {
      form_set_error('ajax][MaxCharge4', 'Maximum charge must be given in format x.xx');
      return;
    }
    if (isset($form_state['values']['ajax']['MaxTime4HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['MaxTime4HHMM'])) {
      form_set_error('ajax][MaxTime4HHMM', 'Maximum time must be given in format HH:MM');
      return;
    }
  }

  if (isset($form_state['values']['ajax']['Length5HHMM']) and
      !empty($form_state['values']['ajax']['Length5HHMM'])) {
    if (isset($form_state['values']['ajax']['Length5HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['Length5HHMM'])) {
      form_set_error('ajax][Length5HHMM', 'Length must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['TimeStep5HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['TimeStep5HHMM'])) {
      form_set_error('ajax][TimeStep5HHMM', 'Time step must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['ChargeStep5']) and
	!preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['ChargeStep5'])) {
      form_set_error('ajax][ChargeStep5', 'Charge step must be given in format x.xx');
      return;
    }
    if (isset($form_state['values']['ajax']['CutOffTime5']) and
	0 < strlen($form_state['values']['ajax']['CutOffTime5']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['CutOffTime5'])) {
      form_set_error('ajax][CutOffTime5', 'Cut-off time must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['MaxCharge5']) and
	!preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['MaxCharge5'])) {
      form_set_error('ajax][MaxCharge5', 'Maximum charge must be given in format x.xx');
      return;
    }
    if (isset($form_state['values']['ajax']['MaxTime5HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['MaxTime5HHMM'])) {
      form_set_error('ajax][MaxTime5HHMM', 'Maximum time must be given in format HH:MM');
      return;
    }
  }

  if (isset($form_state['values']['ajax']['Length6HHMM']) and
      !empty($form_state['values']['ajax']['Length6HHMM'])) {
    if (isset($form_state['values']['ajax']['Length6HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['Length6HHMM'])) {
      form_set_error('ajax][Length6HHMM', 'Length must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['TimeStep6HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['TimeStep6HHMM'])) {
      form_set_error('ajax][TimeStep6HHMM', 'Time step must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['ChargeStep6']) and
	!preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['ChargeStep6'])) {
      form_set_error('ajax][ChargeStep6', 'Charge step must be given in format x.xx');
      return;
    }
    if (isset($form_state['values']['ajax']['CutOffTime6']) and
	0 < strlen($form_state['values']['ajax']['CutOffTime6']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['CutOffTime6'])) {
      form_set_error('ajax][CutOffTime6', 'Cut-off time must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['MaxCharge6']) and
	!preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['MaxCharge6'])) {
      form_set_error('ajax][MaxCharge6', 'Maximum charge must be given in format x.xx');
      return;
    }
    if (isset($form_state['values']['ajax']['MaxTime6HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['MaxTime6HHMM'])) {
      form_set_error('ajax][MaxTime6HHMM', 'Maximum time must be given in format HH:MM');
      return;
    }
  }

  if (isset($form_state['values']['ajax']['Length7HHMM']) and
      !empty($form_state['values']['ajax']['Length7HHMM'])) {
    if (isset($form_state['values']['ajax']['Length7HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['Length7HHMM'])) {
      form_set_error('ajax][Length7HHMM', 'Length must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['TimeStep7HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['TimeStep7HHMM'])) {
      form_set_error('ajax][TimeStep7HHMM', 'Time step must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['ChargeStep7']) and
	!preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['ChargeStep7'])) {
      form_set_error('ajax][ChargeStep7', 'Charge step must be given in format x.xx');
      return;
    }
    if (isset($form_state['values']['ajax']['CutOffTime7']) and
	0 < strlen($form_state['values']['ajax']['CutOffTime7']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['CutOffTime7'])) {
      form_set_error('ajax][CutOffTime7', 'Cut-off time must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['MaxCharge7']) and
	!preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['MaxCharge7'])) {
      form_set_error('ajax][MaxCharge7', 'Maximum charge must be given in format x.xx');
      return;
    }
    if (isset($form_state['values']['ajax']['MaxTime7HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['MaxTime7HHMM'])) {
      form_set_error('ajax][MaxTime7HHMM', 'Maximum time must be given in format HH:MM');
      return;
    }
  }

  if (isset($form_state['values']['ajax']['Length8HHMM']) and
      !empty($form_state['values']['ajax']['Length8HHMM'])) {
    if (isset($form_state['values']['ajax']['Length8HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['Length8HHMM'])) {
      form_set_error('ajax][Length8HHMM', 'Length must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['TimeStep8HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['TimeStep8HHMM'])) {
      form_set_error('ajax][TimeStep8HHMM', 'Time step must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['ChargeStep8']) and
	!preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['ChargeStep8'])) {
      form_set_error('ajax][ChargeStep8', 'Charge step must be given in format x.xx');
      return;
    }
    if (isset($form_state['values']['ajax']['CutOffTime8']) and
	0 < strlen($form_state['values']['ajax']['CutOffTime8']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['CutOffTime8'])) {
      form_set_error('ajax][CutOffTime8', 'Cut-off time must be given in format HH:MM');
      return;
    }
    if (isset($form_state['values']['ajax']['MaxCharge8']) and
	!preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['MaxCharge8'])) {
      form_set_error('ajax][MaxCharge8', 'Maximum charge must be given in format x.xx');
      return;
    }
    if (isset($form_state['values']['ajax']['MaxTime8HHMM']) and
	!preg_match('/^\d+:[0-9]{2}$/', $form_state['values']['ajax']['MaxTime8HHMM'])) {
      form_set_error('ajax][MaxTime8HHMM', 'Maximum time must be given in format HH:MM');
      return;
    }
  }

  for ($i = 0; $i < $form_state['values']['ajax']['NumMerchantCodes']; $i++) {
    if (isset($form_state['values']['ajax']['Merchants']['Code'.$i]) and
	empty($form_state['values']['ajax']['Merchants']['Code'.$i])) {
      form_set_error('ajax][Merchants][Code'.$i, 'Cost field is required.');
      return;
    }

    if (isset($form_state['values']['ajax']['Merchants']['Code'.$i]) and
	preg_match('/\s/', $form_state['values']['ajax']['Merchants']['Code'.$i])) {
      form_set_error('ajax][Merchants][Code'.$i, 'Code shall be given without any spaces.');
      return;
    }

    // Tore 131201: Added
    if (isset($form_state['values']['ajax']['Merchants']['Amount'.$i]) and
	empty($form_state['values']['ajax']['Merchants']['Amount'.$i])) {
      form_set_error('ajax][Merchants][Amount'.$i, 'Amount field is required.');
      return;
    }

    // Tore 131201: Added
    if (isset($form_state['values']['ajax']['Merchants']['Amount'.$i]) and
	!preg_match('/^\d+\.[0-9]{2}$/', $form_state['values']['ajax']['Merchants']['Amount'.$i])) {
      form_set_error('ajax][Merchants][Amount'.$i, 'Amount must be given in format x.yy');
      return;
    }

    // Tore 131201: Added
    if (isset($form_state['values']['ajax']['Merchants']['Amount'.$i]) and
	$form_state['values']['ajax']['Merchants']['Amount'.$i] < 0.00) {
      form_set_error('ajax][Merchants][Amount'.$i, 'Amount must be positive');
      return;
    }

    // Tore 131201: Added
    if (isset($form_state['values']['ajax']['Merchants']['Percentage'.$i]) and
	empty($form_state['values']['ajax']['Merchants']['Percentage'.$i])) {
      form_set_error('ajax][Merchants][Percentage'.$i, 'Percentage field is required.');
      return;
    }

    // Tore 131201: Added
    if (isset($form_state['values']['ajax']['Merchants']['Percentage'.$i]) and
	!preg_match('/^\d+$/', $form_state['values']['ajax']['Merchants']['Percentage'.$i])) {
      form_set_error('ajax][Merchants][Percentage'.$i, 'Percentage must be given as a non-decimal value');
      return;
    }

    // Tore 131201: Added
    if (isset($form_state['values']['ajax']['Merchants']['Percentage'.$i]) and
	$form_state['values']['ajax']['Merchants']['Percentage'.$i] < 0) {
      form_set_error('ajax][Merchants][Percentage'.$i, 'Percentage must be positive');
      return;
    }

    // Tore 131201: Added
    if (isset($form_state['values']['ajax']['Merchants']['Percentage'.$i]) and
	$form_state['values']['ajax']['Merchants']['Percentage'.$i] > 99) {
      form_set_error('ajax][Merchants][Percentage'.$i, 'Percentage can\'t be greater than 99');
      return;
    }
  }
}

function chpperm_container_add_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermtypes';
}

function chpperm_container_add_form_submit_save($form, &$form_state) {
  global $user;
  $company_id = $form_state['values']['ajax']['CompanyId'];

  if ($form_state['values']['ajax']['Category'] === 'Container') {
    $container_id = chpprop_genRandomString20();
    if (chdbperm_insert_container
	($company_id,
	 $container_id,
	 $user->uid,
	 check_plain($form_state['values']['ajax']['ContainerAlias']),
	 $id)) {
      drupal_set_message('Could not insert new container', 'error');
      return;
    }
  } else {
    $container_id = $form_state['values']['ajax']['ContainerId'];
  }

  if ($form_state['values']['ajax']['Category'] === 'Container' or
      $form_state['values']['ajax']['Category'] === 'ContType') {

    $type_id = chpprop_genRandomString20();

    // Policy options

    if ($form_state['values']['ajax']['Policy']['LongPolicy']) {
      $long_term = TRUE;
    } else {
      $long_term = FALSE;
    }

    if ($form_state['values']['ajax']['Policy']['ShortPolicy']) {
      $short_term = TRUE;
    } else {
      $short_term = FALSE;
    }

    if ($form_state['values']['ajax']['Policy']['ValetPolicy']) {
      $valet = TRUE;
    } else {
      $valet = FALSE;
    }

    // Permit type options

    if ($form_state['values']['ajax']['PermitType']['Transients']) {
      $transients = TRUE;
      $transient_type = NULL;
    } else {
      $transients = FALSE;
      $transient_type = NULL;
    }

    if ($form_state['values']['ajax']['PermitType']['Groups']) {
      $groups = TRUE;
      $group_type = $form_state['values']['ajax']['Charging'];
    } else {
      $groups = FALSE;
      $group_type = NULL;
    }

    if ($form_state['values']['ajax']['PermitType']['Members']) {
      $members = TRUE;
      $member_type = $form_state['values']['ajax']['Charging'];
    } else {
      $members = FALSE;
      $member_type = NULL;
    }

    if ($form_state['values']['ajax']['PermitType']['Meter']) {
      $meter = TRUE;
    } else {
      $meter = FALSE;
    }

    if (isset($form_state['values']['ajax']['RateType'])) {
      $rate_type = $form_state['values']['ajax']['RateType'];
    } else {
      $rate_type = NULL;
    }

    if (isset($form_state['values']['ajax']['ValueUnit'])) {
      $value_unit = $form_state['values']['ajax']['ValueUnit'];
    } else {
      $value_unit = NULL;
    }

    if (isset($form_state['values']['ajax']['UnitValue'])) {
      $unit_value = check_plain(sprintf("%0.02f", $form_state['values']['ajax']['UnitValue']));
    } else {
      $unit_value = '0.00';
    }

    if (isset($form_state['values']['ajax']['MaxCost'])) {
      $max_cost = check_plain(sprintf("%0.02f", $form_state['values']['ajax']['MaxCost']));
    } else {
      $max_cost = '0.00';
    }

    if (isset($form_state['values']['ajax']['FixedPeriod'])) {
      $fixed_period = check_plain($form_state['values']['ajax']['FixedPeriod']);
    } else {
      $fixed_period = NULL;
    }

    $alias = check_plain($form_state['values']['ajax']['TypeAlias']);

    if ($meter) {
      if ($form_state['values']['ajax']['Type'] === 'Max') {

	$charge = $form_state['values']['ajax']['Charge'];
	$charge_id = chpprop_genRandomString20();
	$behavior = $form_state['values']['ajax']['Behavior'];
	$hhmm = $form_state['values']['ajax']['HHMM'];

	if (chdbccpark_insert_maxcharge
	    ($company_id, $container_id, $charge_id,
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

	$len1 = $len2 = $len3 = $len4 = $len5 = $len6 = $len7 = $len8 = '00:00';

	if (!empty($form_state['values']['ajax']['Length1HHMM'])) {
	  $len1 = $form_state['values']['ajax']['Length1HHMM'];
	}
	$time1 = $form_state['values']['ajax']['TimeStep1HHMM'];
	$charge1 = $form_state['values']['ajax']['ChargeStep1'];
	$cutoff1 = $form_state['values']['ajax']['CutOffTime1'];
	$maxcharge1 = $form_state['values']['ajax']['MaxCharge1'];
	$maxtime1 = $form_state['values']['ajax']['MaxTime1HHMM'];

	if (!empty($form_state['values']['ajax']['Length2HHMM'])) {
	  $len2 = $form_state['values']['ajax']['Length2HHMM'];
	}
	$time2 = $form_state['values']['ajax']['TimeStep2HHMM'];
	$charge2 = $form_state['values']['ajax']['ChargeStep2'];
	$cutoff2 = $form_state['values']['ajax']['CutOffTime2'];
	$maxcharge2 = $form_state['values']['ajax']['MaxCharge2'];
	$maxtime2 = $form_state['values']['ajax']['MaxTime2HHMM'];

	if (!empty($form_state['values']['ajax']['Length3HHMM'])) {
	  $len3 = $form_state['values']['ajax']['Length3HHMM'];
	}
	$time3 = $form_state['values']['ajax']['TimeStep3HHMM'];
	$charge3 = $form_state['values']['ajax']['ChargeStep3'];
	$cutoff3 = $form_state['values']['ajax']['CutOffTime3'];
	$maxcharge3 = $form_state['values']['ajax']['MaxCharge3'];
	$maxtime3 = $form_state['values']['ajax']['MaxTime3HHMM'];

	if (!empty($form_state['values']['ajax']['Length4HHMM'])) {
	  $len4 = $form_state['values']['ajax']['Length4HHMM'];
	}
	$time4 = $form_state['values']['ajax']['TimeStep4HHMM'];
	$charge4 = $form_state['values']['ajax']['ChargeStep4'];
	$cutoff4 = $form_state['values']['ajax']['CutOffTime4'];
	$maxcharge4 = $form_state['values']['ajax']['MaxCharge4'];
	$maxtime4 = $form_state['values']['ajax']['MaxTime4HHMM'];

	if (!empty($form_state['values']['ajax']['Length5HHMM'])) {
	  $len5 = $form_state['values']['ajax']['Length5HHMM'];
	}
	$time5 = $form_state['values']['ajax']['TimeStep5HHMM'];
	$charge5 = $form_state['values']['ajax']['ChargeStep5'];
	$cutoff5 = $form_state['values']['ajax']['CutOffTime5'];
	$maxcharge5 = $form_state['values']['ajax']['MaxCharge5'];
	$maxtime5 = $form_state['values']['ajax']['MaxTime5HHMM'];

	if (!empty($form_state['values']['ajax']['Length6HHMM'])) {
	  $len6 = $form_state['values']['ajax']['Length6HHMM'];
	}
	$time6 = $form_state['values']['ajax']['TimeStep6HHMM'];
	$charge6 = $form_state['values']['ajax']['ChargeStep6'];
	$cutoff6 = $form_state['values']['ajax']['CutOffTime6'];
	$maxcharge6 = $form_state['values']['ajax']['MaxCharge6'];
	$maxtime6 = $form_state['values']['ajax']['MaxTime6HHMM'];

	if (!empty($form_state['values']['ajax']['Length7HHMM'])) {
	  $len7 = $form_state['values']['ajax']['Length7HHMM'];
	}
	$time7 = $form_state['values']['ajax']['TimeStep7HHMM'];
	$charge7 = $form_state['values']['ajax']['ChargeStep7'];
	$cutoff7 = $form_state['values']['ajax']['CutOffTime7'];
	$maxcharge7 = $form_state['values']['ajax']['MaxCharge7'];
	$maxtime7 = $form_state['values']['ajax']['MaxTime7HHMM'];

	if (!empty($form_state['values']['ajax']['Length8HHMM'])) {
	  $len8 = $form_state['values']['ajax']['Length8HHMM'];
	}
	$time8 = $form_state['values']['ajax']['TimeStep8HHMM'];
	$charge8 = $form_state['values']['ajax']['ChargeStep8'];
	$cutoff8 = $form_state['values']['ajax']['CutOffTime8'];
	$maxcharge8 = $form_state['values']['ajax']['MaxCharge8'];
	$maxtime8 = $form_state['values']['ajax']['MaxTime8HHMM'];

	if (chdbccpark_insert_rate
	    ($company_id, $container_id, $rate_id, $alias,
	     $min_time, $min_charge, $begin, $end, $days,
	     $len1, $time1, $charge1, $cutoff1, $maxcharge1, $maxtime1,
	     $len2, $time2, $charge2, $cutoff2, $maxcharge2, $maxtime2,
	     $len3, $time3, $charge3, $cutoff3, $maxcharge3, $maxtime3,
	     $len4, $time4, $charge4, $cutoff4, $maxcharge4, $maxtime4,
	     $len5, $time5, $charge5, $cutoff5, $maxcharge5, $maxtime5,
	     $len6, $time6, $charge6, $cutoff6, $maxcharge6, $maxtime6,
	     $len7, $time7, $charge7, $cutoff7, $maxcharge7, $maxtime7,
	     $len8, $time8, $charge8, $cutoff8, $maxcharge8, $maxtime8,
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
	    ($company_id, $container_id, $pass_id,
	     $alias, $duration, $charge, $menu,
	     $id)) {
	  drupal_set_message('Could not insert parking pass', 'error');
	  return;
	}
      }
    } else {
      if (chdbperm_insert_conttype
	  ($company_id, $container_id, $type_id, $user->uid,
	   $long_term, $short_term, $valet,
	   $transients, $transient_type,
	   $groups, $group_type,
	   $members, $member_type,
	   $alias,
	   $rate_type,
	   $value_unit,
	   $unit_value,
	   $max_cost,
	   $fixed_period,
	   check_plain($form_state['values']['ajax']['Comment']),
	   $id)) {
	drupal_set_message('Could not insert new conttype', 'error');
	return;
      }
    }
  }

  if ($form_state['values']['ajax']['Category'] === 'Merchant') {
    $type_id = chpprop_genRandomString20();

    for ($i = 0; $i < $form_state['values']['ajax']['NumMerchantCodes']; $i++) {
      $code = check_plain($form_state['values']['ajax']['Merchants']['Code'.$i]);
      $mode = $form_state['values']['ajax']['Merchants']['Discount'.$i];
      $amount = (isset($form_state['values']['ajax']['Merchants']['Amount'.$i]) ?
		 $form_state['values']['ajax']['Merchants']['Amount'.$i] : '0.00');
      $percentage = (isset($form_state['values']['ajax']['Merchants']['Percentage'.$i]) ?
		     $form_state['values']['ajax']['Merchants']['Percentage'.$i] : 0);

      if (chdbccpark_insert_merchantcode
	  ($company_id, $container_id, $type_id,
	   $code, $mode, $amount, $percentage,
	   $id)) {
	drupal_set_message('Could not insert merchant code', 'error');
	return;
      }
    }
  }

  if ($form_state['values']['ajax']['Category'] === 'Container') {
    $list = unserialize($form_state['values']['ajax']['property']['List']);
    if (!empty($list)) {
      foreach ($list as $propkey => $values) {
	foreach ($values as $lotkey => $value) {
	  if ($value) {
	    if (chdbperm_insert_contprop
		($company_id, $container_id,
		 $propkey, $lotkey,
		 $id)) {
	      drupal_set_message('Could not attach to property/lot', 'error');
	      return;
	    }
	  }
	}
      }
    }
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermtypes';
}

function chpperm_container_edit_form($form_state) {
  if ($form_state['storage']['confirm_delete']) {
    return confirm_form($form, 'Are you sure?', 'chppermtypes',
			'The container type will be deleted from the system.');
  }

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  drupal_set_title(t('Edit container'));

  $id = $form_state['storage']['id']; //drupal_set_message('Id:'.$id);
  $container = chdbperm_get_container_id($id);
  $company_id = $container['CompanyId'];
  $container_id = $container['ContainerId'];

  $form['ContainerAlias'] =
    array('#type' => 'textfield',
	  '#title' => t('Container name'),
	  '#maxlength' => 60,
	  '#default_value' => $container['Alias']);

  // Create property options
  $property_options = array();
  $properties = chpprop_retrieve_properties($user->uid, $company_id);
  if (!empty($properties)) {
    foreach ($properties as $property_id => $property) {
      $property_options[$property_id] =
	decode_entities($property['data']['Alias']).', '.
	decode_entities($property['data']['Street']).', '.
	decode_entities($property['data']['City']);
    }
  }
  $properties = NULL;

  if (empty($property_options)) {
    $form['error'] = array('#value' => '<p>'.t('Property missing.').'<p>');
    $form['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpperm_container_edit_form_submit_back'));

    return $form;
  }

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  // Property/Lot selector
  $form['ajax']['property'] = array('#type' => 'fieldset', '#title' => t('Assignment'));

  // Layout property options
  if (isset($form_state['storage']['ajax']['property']['PropertyId']) and
      !in_array($form_state['storage']['ajax']['property']['PropertyId'],
		array_keys($property_options))) {
    unset($form_state['storage']['ajax']['property']['PropertyId']);
  }
  if (!isset($form_state['storage']['ajax']['property']['PropertyId'])) {
    $form_state['storage']['ajax']['property']['PropertyId'] = key($property_options);
  }
  if (1 < count($property_options)) {
    $form['ajax']['property']['PropertyId'] =
      array('#type' => 'select',
	    '#title' => t('Property'),
	    '#options' => $property_options,
	    '#default_value' => $form_state['storage']['ajax']['property']['PropertyId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));
  }

  $property_id = $form_state['storage']['ajax']['property']['PropertyId'];

  // Create lot options
  $lot_options = array();
  $policies = chpprop_retrieve_policies($user->uid, $company_id);
  if (!empty($policies)) {
    if (isset($policies[$property_id])) {
      foreach ($policies[$property_id] as $lot_id => $policy) {
	$lot_options[$lot_id] = decode_entities($policy['data']['Alias']);
      }
    }
  }

  if (empty($lot_options)) {
    $form['ajax']['error'] = array('#value' => '<p>'.t('Lot missing.').'<p>');
    $form['ajax']['cancel'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	    '#submit' => array('chpperm_container_edit_form_submit_back'));
    return $form;
  }

  if (isset($form_state['storage']['ajax']['property']['LotId']) and
      !in_array($form_state['storage']['ajax']['property']['LotId'],
		array_keys($lot_options))) {
    unset($form_state['storage']['ajax']['property']['LotId']);
  }
  if (!isset($form_state['storage']['ajax']['property']['LotId'])) {
    $form_state['storage']['ajax']['property']['LotId'] = key($lot_options);
  }
  if (1 < count($lot_options)) {
    $form['ajax']['property']['LotId'] =
      array('#type' => 'select',
	    '#title' => t('Lot'),
	    '#options' => $lot_options,
	    '#default_value' => $form_state['storage']['ajax']['property']['LotId']);
  }

  $lot_id = $form_state['storage']['ajax']['property']['LotId'];

  if (1 < count($property_options) or 1 < count($lot_options)) {
    $form['ajax']['property']['addbtn'] = array('#type' => 'button', '#value' => t('Add'));
    $form['ajax']['property']['clearbtn'] = array('#type' => 'button', '#value' => t('Clear'));
    $form['ajax']['property']['allbtn'] = array('#type' => 'button', '#value' => t('All'));
    if (!isset($form_state['storage']['ajax']['property']['List'])) {
      $list = array();
      $props = chpperm_retrieve_container_props($user->uid, $company_id, $container_id);
      foreach ($props as $propkey => $values) {
	foreach ($values as $lotkey => $value) {
	  $list[$propkey][$lotkey] = TRUE;
	}
      }
      $form_state['storage']['ajax']['property']['List'] = $list;
    } else {
      $list = unserialize($form_state['storage']['ajax']['property']['List']);
    }

    if (strpos($form_state['clicked_button']['#id'], 'addbtn') !== FALSE) {
      $list[$property_id][$lot_id] = TRUE;
    }
    if (strpos($form_state['clicked_button']['#id'], 'clearbtn') !== FALSE) {
      $list = array();
    }
    if (strpos($form_state['clicked_button']['#id'], 'allbtn') !== FALSE) {
      foreach ($property_options as $propkey => $propvalue) {
	foreach ($policies[$propkey] as $lotkey => $lotvalue) {
	  $list[$propkey][$lotkey] = TRUE;
	}
      }
    }
  } else {
    $list[$property_id][$lot_id] = TRUE;
  }

  $form['ajax']['property']['List'] = array('#type' => 'value', '#value' => serialize($list));

  $header = array(t('Property'), t('Lot'));
  $rows = array();
  foreach ($list as $property_id => $values) {
    foreach ($values as $lot_id => $value) {
      $rows[] = array($property_options[$property_id], $lot_options[$lot_id]);
    }
  }
  $form['ajax']['property']['ListView'] = array('#value' => theme_table($header, $rows));

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpperm_container_edit_form_validate_save'),
	  '#submit' => array('chpperm_container_edit_form_submit_save'));

  $form['ajax']['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpperm_container_edit_form_submit_back'));

  if (user_access('anai chp delete permit container')) {
    $form['ajax']['delete'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/delete.png',
	    '#submit' => array('chpperm_container_edit_form_submit_delete'));
  }
  return $form;
}

function chpperm_container_edit_form_validate_save($form, &$form_state) {
  if (empty($form_state['values']['ContainerAlias'])) {
    form_set_error('ContainerAlias', 'Container name field is required.');
    return;
  }
}

function chpperm_container_edit_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermtypes';
}

function chpperm_container_edit_form_submit_save($form, &$form_state) {
  global $user;
  $id = $form_state['storage']['id'];
  $container = chdbperm_get_container_id($id);
  $company_id = $container['CompanyId'];
  $container_id = $container['ContainerId'];

  if (chdbperm_update_container
      ($id,
       $user->uid,
       check_plain($form_state['values']['ContainerAlias']))) {
    drupal_set_message('Could not insert new container', 'error');
    return;
  }

  $list = unserialize($form_state['values']['ajax']['property']['List']);

  // Delete property assignments for those not in new list
  $props = chpperm_retrieve_container_props($user->uid, $company_id, $container_id);

  foreach ($props as $propkey => $values) {
    foreach ($values as $lotkey => $value) {
      if (isset($list[$propkey][$lotkey])) {
	// Mark as not needing insertion
	$list[$propkey][$lotkey] = FALSE;
      } else {
	if (chdbperm_delete_contprop
	    ($company_id, $container_id,
	     $propkey, $lotkey)) {
	  drupal_set_message('Could not delete from property/lot', 'error');
	  return;
	}
      }
    }
  }

  // Insert new assigned properties
  if (!empty($list)) {
    foreach ($list as $propkey => $values) {
      foreach ($values as $lotkey => $value) {
	if ($value) {
	  if (chdbperm_insert_contprop
	      ($company_id, $container_id,
	       $propkey, $lotkey,
	       $id)) {
	    drupal_set_message('Could not attach to property/lot', 'error');
	    return;
	  }
	}
      }
    }
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermtypes';
}

function chpperm_container_edit_form_submit_delete($form, &$form_state) {
  $form_state['storage']['confirm_delete'] = TRUE;
}



/**********
 ********** Conttype edit
 ********** Address: chppermtypeedit
 ********** Access:  'anai chp edit permit container'
 *********/



function chpperm_conttype_edit_form($form_state, $id) {
  if ($form_state['storage']['confirm_delete']) {
    return confirm_form($form, 'Are you sure?', 'chppermtypes',
			'The permit type will be deleted from the system.');
  }

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $conttype = chdbperm_get_conttype_id($id);
  $companies = chpprop_retrieve_companies($user->uid);

  // Sanity check permits
  if (!in_array($conttype['CompanyId'], array_keys($companies))) {
    unset($form_state['storage']);
    drupal_access_denied();
    exit();
  }

  drupal_set_title(t('Edit permit type'));
  $company_id = $conttype['CompanyId'];
  $container_id = $conttype['ContainerId'];

  // Remember type id
  $form['id'] = array('#type' => 'hidden', '#value' => $id);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  // Layout alias field
  if (!isset($form_state['storage']['ajax']['TypeAlias'])) {
    $form_state['storage']['ajax']['TypeAlias'] = decode_entities($conttype['Alias']);
  }
  $form['ajax']['TypeAlias'] =
    array('#type' => 'textfield',
	  '#title' => t('Permit type name'),
	  '#maxlength' => 60,
	  '#default_value' => $form_state['storage']['ajax']['TypeAlias']);

  // Create policy options
  $options = array();
  $options['LongPolicy'] = t('Restrict to long term parking lots');
  $options['ShortPolicy'] = t('Restrict to short term parking lots');
  $options['ValetPolicy'] = t('Restrict to valet parking lots');

  // Layout type options
  if (!isset($form_state['storage']['ajax']['Policy'])) {
    if ($conttype['Long']) {
      $form_state['storage']['ajax']['Policy']['LongPolicy'] = 'LongPolicy';
    } else {
      $form_state['storage']['ajax']['Policy']['LongPolicy'] = FALSE;
    }
    if ($conttype['Short']) {
      $form_state['storage']['ajax']['Policy']['ShortPolicy'] = 'ShortPolicy';
    } else {
      $form_state['storage']['ajax']['Policy']['ShortPolicy'] = FALSE;
    }
    if ($conttype['Valet']) {
      $form_state['storage']['ajax']['Policy']['ValetPolicy'] = 'ValetPolicy';
    } else {
      $form_state['storage']['ajax']['Policy']['ValetPolicy'] = FALSE;
    }
  }
  $form['ajax']['Policy'] =
    array('#type' => 'checkboxes',
	  '#title' => t('Container policy restrictors'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['Policy']);

  // Create permit type options
  $options = array();
  $options['Transients'] = t('Transient permits');
  $options['Groups'] = t('Group permits');
  $options['Members'] = t('Individual permits');

  // Layout permit type options
  if (!isset($form_state['storage']['ajax']['PermitType'])) {
    if ($conttype['Transients']) {
      $form_state['storage']['ajax']['PermitType']['Transients'] = 'Transients';
    } else {
      $form_state['storage']['ajax']['PermitType']['Transients'] = FALSE;
    }
    if ($conttype['Groups']) {
      $form_state['storage']['ajax']['PermitType']['Groups'] = 'Groups';
    } else {
      $form_state['storage']['ajax']['PermitType']['Groups'] = FALSE;
    }
    if ($conttype['Members']) {
      $form_state['storage']['ajax']['PermitType']['Members'] = 'Members';
    } else {
      $form_state['storage']['ajax']['PermitType']['Members'] = FALSE;
    }
  }
  $form['ajax']['PermitType'] =
    array('#type' => 'checkboxes',
	  '#title' => t('Permit type'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['PermitType']);

  // Create group policy options
  $options = array();
  $options['Subscribed'] = t('Subscribed permit');
  $options['Complementary'] = t('Complementary permit');

  // Layout group policy options
  if (!isset($form_state['storage']['ajax']['Charging'])) {
    if ($conttype['Groups']) {
      $form_state['storage']['ajax']['Charging'] = $conttype['GroupType'];
    } elseif ($conttype['Members']) {
      $form_state['storage']['ajax']['Charging'] = $conttype['MemberType'];
    } else {
      $form_state['storage']['ajax']['Charging'] = key($options);
    }
  }
  $form['ajax']['Charging'] =
    array('#type' => 'radios',
	  '#title' => t('Charging'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['Charging'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  if ($form_state['storage']['ajax']['Charging'] !== 'Complementary') {
    // Create permit type options
    $options = array();
    $options['Linear'] = chpperm_structure_name('Linear');
    $options['Flat'] = chpperm_structure_name('Flat');
    $options['Fixed'] = chpperm_structure_name('Fixed');

    // Layout permit type options
    if (!isset($form_state['storage']['ajax']['RateType'])) {
      if (in_array($conttype['RateType'], array_keys($options))) {
	$form_state['storage']['ajax']['RateType'] = $conttype['RateType'];
      } else {
	$form_state['storage']['ajax']['RateType'] = key($options);
      }
    }
    $form['ajax']['RateType'] =
      array('#type' => 'select',
	    '#title' => t('Rate structure'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['RateType'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));

    // Layout unit cost value
    if (!isset($form_state['storage']['ajax']['UnitValue'])) {
      $form_state['storage']['ajax']['UnitValue'] =
	sprintf("%0.02f", $conttype['UnitValue']);
    }
    $form['ajax']['UnitValue'] =
      array('#type' => 'textfield',
	    '#title' => t('Cost'),
	    '#maxlength' => 10,
	    '#default_value' => $form_state['storage']['ajax']['UnitValue'],
	    '#description' => t('Format: x.xx'));

    if ($form_state['storage']['ajax']['RateType'] === 'Linear') {
      // Create value unit options
      $options = array();
      $options['Minute'] = t('Minutes');
      $options['Hour'] = t('Per hour');
      $options['Day'] = t('per day');
      $options['Week'] = t('per week');
      $options['Month'] = t('Per month');

      // Layout value unit options
      if (!in_array($form_state['storage']['ajax']['ValueUnit'], array_keys($options))) {
	unset($form_state['storage']['ajax']['ValueUnit']);
      }
      if (!isset($form_state['storage']['ajax']['ValueUnit'])) {
	$form_state['storage']['ajax']['ValueUnit'] = $conttype['ValueUnit'];
      }
      $form['ajax']['ValueUnit'] =
	array('#type' => 'select',
	      '#options' => $options,
	      '#default_value' => $form_state['storage']['ajax']['ValueUnit']);

      // Layout max cost field
      if (!isset($form_state['storage']['ajax']['MaxCost'])) {
	$form_state['storage']['ajax']['MaxCost'] =
	  sprintf("%0.02f", $conttype['MaxCost']);
      }
      $form['ajax']['MaxCost'] =
	array('#type' => 'textfield',
	      '#title' => t('Maximum cost of permit'),
	      '#maxlength' => 10,
	      '#default_value' => $form_state['storage']['ajax']['MaxCost'],
	      '#description' => t('Format: x.xx'));

    } elseif ($form_state['storage']['ajax']['RateType'] === 'Fixed') {

      // Layout fixed period field
      if (!isset($form_state['storage']['ajax']['FixedPeriod'])) {
	$form_state['storage']['ajax']['FixedPeriod'] = $conttype['FixedPeriod'];
      }
      $form['ajax']['FixedPeriod'] =
	array('#type' => 'textfield',
	      '#title' => t('Duration'),
	      '#maxlength' => 10,
	      '#default_value' => $form_state['storage']['ajax']['FixedPeriod']);

      // Create value unit options
      $options = array();
      $options['Minute'] = t('Minutes');
      $options['Hour'] = t('Hours');
      $options['Day'] = t('Days');
      $options['Week'] = t('Weeks');
      $options['Month'] = t('Months');

      // Layout value unit options
      if (!isset($form_state['storage']['ajax']['ValueUnit'])) {
	$form_state['storage']['ajax']['ValueUnit'] = $conttype['ValueUnit'];
      }
      $form['ajax']['ValueUnit'] =
	array('#type' => 'select',
	      '#options' => $options,
	      '#default_value' => $form_state['storage']['ajax']['ValueUnit']);
    }
  }

  if (!isset($form_state['storage']['ajax']['Comment'])) {
    $form_state['storage']['ajax']['Comment'] = decode_entities($conttype['Comment']);
  }
  $form['ajax']['Comment'] =
    array('#type' => 'textfield',
	  '#title' => t('Comment'),
	  '#maxlength' => 120,
	  '#default_value' => $form_state['storage']['ajax']['Comment'],
	  '#description' =>
	  t('Displayed when creating new permit').'<br>'.
	  t('Example 1: Cost of permit is $6.00 flat rate').'<br>'.
	  t('Example 2: Cost of permit is $6.00 per hour'));

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpperm_container_add_form_validate_save'),
	  '#submit' => array('chpperm_conttype_edit_form_submit_save'));

  $form['ajax']['cancel'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpperm_conttype_edit_form_submit_cancel'));

  if (user_access('anai chp delete permit container')) {
    $form['ajax']['delete'] =
      array('#type' => 'image_button',
	    '#src' => drupal_get_path('module', 'anai').'/delete.png',
	    '#validate' => array('chpperm_conttype_edit_form_validate_delete'),
	    '#submit' => array('chpperm_conttype_edit_form_submit'));
  }
  return $form;
}

function chpperm_conttype_edit_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermtypes';
}

function chpperm_conttype_edit_form_validate_delete($form, &$form_state) {
  $form_state['storage']['id'] = $form_state['values']['id'];
  $form_state['storage']['delete'] = TRUE;
}

function chpperm_conttype_edit_form_submit_save($form, &$form_state) {
  global $user;
  $id = $form_state['values']['id'];
  $conttype = chdbperm_get_conttype_id($id);
  $company_id = $conttype['CompanyId'];
  $container_id = $conttype['ContainerId'];
  $type_id = $conttype['TypeId'];

  if ($form_state['values']['ajax']['Policy']['LongPolicy']) {
    $long_term = TRUE;
  } else {
    $long_term = FALSE;
  }

  if ($form_state['values']['ajax']['Policy']['ShortPolicy']) {
    $short_term = TRUE;
  } else {
    $short_term = FALSE;
  }

  if ($form_state['values']['ajax']['Policy']['ValetPolicy']) {
    $valet = TRUE;
  } else {
    $valet = FALSE;
  }

  if ($form_state['values']['ajax']['PermitType']['Transients']) {
    $transients = TRUE;
    $transient_type = NULL;
  } else {
    $transients = FALSE;
    $transient_type = NULL;
  }

  if ($form_state['values']['ajax']['PermitType']['Groups']) {
    $groups = TRUE;
    $group_type = $form_state['values']['ajax']['Charging'];
  } else {
    $groups = FALSE;
    $group_type = NULL;
  }

  if ($form_state['values']['ajax']['PermitType']['Members']) {
    $members = TRUE;
    $member_type = $form_state['values']['ajax']['Charging'];
  } else {
    $members = FALSE;
    $member_type = NULL;
  }

  if (isset($form_state['values']['ajax']['RateType'])) {
    $rate_type = $form_state['values']['ajax']['RateType'];
  } else {
    $rate_type = NULL;
  }

  if (isset($form_state['values']['ajax']['ValueUnit'])) {
    $value_unit = $form_state['values']['ajax']['ValueUnit'];
  } else {
    $value_unit = NULL;
  }

  if (isset($form_state['values']['ajax']['UnitValue'])) {
    $unit_value = check_plain(sprintf("%0.02f", $form_state['values']['ajax']['UnitValue']));
  } else {
    $unit_value = '0.00';
  }

  if (isset($form_state['values']['ajax']['MaxCost'])) {
    $max_cost = check_plain(sprintf("%0.02f", $form_state['values']['ajax']['MaxCost']));
  } else {
    $max_cost = '0.00';
  }

  if (isset($form_state['values']['ajax']['FixedPeriod'])) {
    $fixed_period = check_plain($form_state['values']['ajax']['FixedPeriod']);
  } else {
    $fixed_period = NULL;
  }

  if (chdbperm_update_conttype
      ($id, $user->uid,
       $long_term, $short_term, $valet,
       $transients, $transient_type,
       $groups, $group_type,
       $members, $member_type,
       check_plain($form_state['values']['ajax']['TypeAlias']),
       $rate_type,
       $value_unit,
       $unit_value,
       $max_cost,
       $fixed_period,
       check_plain($form_state['values']['ajax']['Comment']))) {
    drupal_set_message('Could not update permit type', 'error');
    return;
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermtypes';
}

function chpperm_conttype_edit_form_submit($form, &$form_state) {
  if ($form_state['storage']['confirm_delete']) {
    $id = $form_state['storage']['id'];
    chdbperm_delete_conttype_id($id);
    unset($form_state['storage']);
    $form_state['redirect'] = 'chppermtypes';
    return;
  }
  if ($form_state['storage']['delete']) {
    unset($form_state['storage']['delete']);
    $form_state['storage']['confirm_delete'] = TRUE;
    return;
  }
}



/**********
 ********** Meterrate edit
 ********** Address: chppermrateedit
 ********** Access:  'anai chp edit permit container'
 *********/



function chpperm_meterrate_edit_form($form_state, $id) {
  if ($form_state['storage']['confirm_delete']) {
    return confirm_form($form, 'Are you sure?', 'chppermtypes',
			'The rate will be deleted from the system.');
  }

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $rate = chdbccpark_get_rate_id($id); //anai_dbg($rate);
  $companies = chpprop_retrieve_companies($user->uid);

  // Sanity check permits
  if (!in_array($rate['CompanyId'], array_keys($companies))) {
    unset($form_state['storage']);
    drupal_access_denied();
    exit();
  }

  drupal_set_title(t('Edit meter rate'));
  $company_id = $rate['CompanyId'];
  $container_id = $rate['ContainerId'];

  // Remember type id
  $form['id'] = array('#type' => 'hidden', '#value' => $id);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  // Layout alias field
  if (!isset($form_state['storage']['ajax']['TypeAlias'])) {
    $form_state['storage']['ajax']['TypeAlias'] = decode_entities($rate['Alias']);
  }
  $form['ajax']['TypeAlias'] =
    array('#type' => 'textfield',
	  '#title' => t('Rate name'),
	  '#maxlength' => 60,
	  '#default_value' => $form_state['storage']['ajax']['TypeAlias']);

  // Layout rate group

  // Javascript date pickers
  $label = t('Begin time');
  drupal_add_js
    ('(function ($) {
          Drupal.behaviors.ANAIchpcontainerBegin=function(context){
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
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['BeginHHMM'],
	  '#prefix' => '<table><tr><td>');

  // Javascript date pickers
  $label = t('End time');
  drupal_add_js
    ('(function ($) {
          Drupal.behaviors.ANAIchpcontainerEnd=function(context){
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
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['EndHHMM'],
	  '#suffix' => '</td>');

  // Javascript date pickers
  $label = t('Minimum duration');
  drupal_add_js
    ('(function ($) {
          Drupal.behaviors.ANAIchpcontainerMin=function(context){
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
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MinHHMM'],
	  '#prefix' => '<td>');

  if (!isset($form_state['storage']['ajax']['MinCharge'])) {
    $form_state['storage']['ajax']['MinCharge'] = $rate['MinCharge'];
  }
  $form['ajax']['MinCharge'] =
    array('#type' => 'textfield',
	  '#title' => t('Minimum charge'),
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MinCharge'],
	  '#description' => t('Format: X.YY'),
	  '#suffix' => '</td>');

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
	  '#default_value' => $form_state['storage']['ajax']['Week'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td></tr></table>');

  /**
   * Rate periods
   */

  // Period 1
  $form['ajax']['Period1'] =
    array('#type' => 'item',
	  '#title' => t('P1'),
	  '#prefix' => '<table><th></th>'.
	  '<th>'.t('Length').'</th>'.
	  '<th>'.t('Time step').'</th>'.
	  '<th>'.t('Charge step').'</th>'.
	  '<th>'.t('Cut-Off Time').'</th>'.
	  '<th>'.t('Max Charge').'</th>'.
	  '<th>'.t('Max Time').'</th>'.
	  '<tr><td>',
	  '#suffix' => '</td>');

  if (!isset($form_state['storage']['ajax']['Length1HHMM'])) {
    $form_state['storage']['ajax']['Length1HHMM'] = $rate['Length1'];
  }
  $form['ajax']['Length1HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['Length1HHMM'],
	  '#description' => t('Format: HH:MM<br>(Blank if not used)'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['TimeStep1HHMM'])) {
    $form_state['storage']['ajax']['TimeStep1HHMM'] = $rate['TimeStep1'];
  }
  $form['ajax']['TimeStep1HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['TimeStep1HHMM'],
	  '#description' => t('Format: HH:MM'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['ChargeStep1'])) {
    $form_state['storage']['ajax']['ChargeStep1'] = $rate['ChargeStep1'];
  }
  $form['ajax']['ChargeStep1'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['ChargeStep1'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['CutOffTime1'])) {
    $form_state['storage']['ajax']['CutOffTime1'] = $rate['CutOffTime1'];
  }
  $form['ajax']['CutOffTime1'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['CutOffTime1'],
	  '#description' => t('Format: HH:MM<br>(Blank if not used)'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['MaxCharge1'])) {
    $form_state['storage']['ajax']['MaxCharge1'] = $rate['MaxCharge1'];
  }
  $form['ajax']['MaxCharge1'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MaxCharge1'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['MaxTime1HHMM'])) {
    $form_state['storage']['ajax']['MaxTime1HHMM'] = $rate['MaxTime1'];
  }
  $form['ajax']['MaxTime1HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MaxTime1HHMM'],
	  '#description' => t('Format: HH:MM'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td></tr>');

  // Period 2
  $form['ajax']['Period2'] =
    array('#type' => 'item',
	  '#title' => t('P2'),
	  '#prefix' => '<tr><td>',
	  '#suffix' => '</td>');

  if (!isset($form_state['storage']['ajax']['Length2HHMM'])) {
    if ($rate['Length2'] !== '00:00') {
      $form_state['storage']['ajax']['Length2HHMM'] = $rate['Length2'];
    } else {
      $form_state['storage']['ajax']['Length2HHMM'] = '';
    }
  }
  $form['ajax']['Length2HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['Length2HHMM'],
	  '#description' => t('Format: HH:MM<br>(Blank if not used)'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['TimeStep2HHMM'])) {
    $form_state['storage']['ajax']['TimeStep2HHMM'] = $rate['TimeStep2'];
  }
  $form['ajax']['TimeStep2HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['TimeStep2HHMM'],
	  '#description' => t('Format: HH:MM'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['ChargeStep2'])) {
    $form_state['storage']['ajax']['ChargeStep2'] = $rate['ChargeStep2'];
  }
  $form['ajax']['ChargeStep2'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['ChargeStep2'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['CutOffTime2'])) {
    $form_state['storage']['ajax']['CutOffTime2'] = $rate['CutOffTime2'];
  }
  $form['ajax']['CutOffTime2'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['CutOffTime2'],
	  '#description' => t('Format: HH:MM<br>(Blank if not used)'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['MaxCharge2'])) {
    $form_state['storage']['ajax']['MaxCharge2'] = $rate['MaxCharge2'];
  }
  $form['ajax']['MaxCharge2'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MaxCharge2'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['MaxTime2HHMM'])) {
    $form_state['storage']['ajax']['MaxTime2HHMM'] = $rate['MaxTime2'];
  }
  $form['ajax']['MaxTime2HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MaxTime2HHMM'],
	  '#description' => t('Format: HH:MM'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td></tr>');

  // Period 3
  $form['ajax']['Period3'] =
    array('#type' => 'item',
	  '#title' => t('P3'),
	  '#prefix' => '<tr><td>',
	  '#suffix' => '</td>');

  if (!isset($form_state['storage']['ajax']['Length3HHMM'])) {
    if ($rate['Length3'] !== '00:00') {
      $form_state['storage']['ajax']['Length3HHMM'] = $rate['Length3'];
    } else {
      $form_state['storage']['ajax']['Length3HHMM'] = '';
    }
  }
  $form['ajax']['Length3HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['Length3HHMM'],
	  '#description' => t('Format: HH:MM<br>(Blank if not used)'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['TimeStep3HHMM'])) {
    $form_state['storage']['ajax']['TimeStep3HHMM'] = $rate['TimeStep3'];
  }
  $form['ajax']['TimeStep3HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['TimeStep3HHMM'],
	  '#description' => t('Format: HH:MM'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['ChargeStep3'])) {
    $form_state['storage']['ajax']['ChargeStep3'] = $rate['ChargeStep3'];
  }
  $form['ajax']['ChargeStep3'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['ChargeStep3'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['CutOffTime3'])) {
    $form_state['storage']['ajax']['CutOffTime3'] = $rate['CutOffTime3'];
  }
  $form['ajax']['CutOffTime3'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['CutOffTime3'],
	  '#description' => t('Format: HH:MM<br>(Blank if not used)'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['MaxCharge3'])) {
    $form_state['storage']['ajax']['MaxCharge3'] = $rate['MaxCharge3'];
  }
  $form['ajax']['MaxCharge3'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MaxCharge3'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['MaxTime3HHMM'])) {
    $form_state['storage']['ajax']['MaxTime3HHMM'] = $rate['MaxTime3'];
  }
  $form['ajax']['MaxTime3HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MaxTime3HHMM'],
	  '#description' => t('Format: HH:MM'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td></tr>');

  // Period 4
  $form['ajax']['Period4'] =
    array('#type' => 'item',
	  '#title' => t('P4'),
	  '#prefix' => '<tr><td>',
	  '#suffix' => '</td>');

  if (!isset($form_state['storage']['ajax']['Length4HHMM'])) {
    if ($rate['Length4'] !== '00:00') {
      $form_state['storage']['ajax']['Length4HHMM'] = $rate['Length4'];
    } else {
      $form_state['storage']['ajax']['Length4HHMM'] = '';
    }
  }
  $form['ajax']['Length4HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['Length4HHMM'],
	  '#description' => t('Format: HH:MM<br>(Blank if not used)'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['TimeStep4HHMM'])) {
    $form_state['storage']['ajax']['TimeStep4HHMM'] = $rate['TimeStep4'];
  }
  $form['ajax']['TimeStep4HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['TimeStep4HHMM'],
	  '#description' => t('Format: HH:MM'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['ChargeStep4'])) {
    $form_state['storage']['ajax']['ChargeStep4'] = $rate['ChargeStep4'];
  }
  $form['ajax']['ChargeStep4'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['ChargeStep4'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['CutOffTime4'])) {
    $form_state['storage']['ajax']['CutOffTime4'] = $rate['CutOffTime4'];
  }
  $form['ajax']['CutOffTime4'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['CutOffTime4'],
	  '#description' => t('Format: HH:MM<br>(Blank if not used)'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['MaxCharge4'])) {
    $form_state['storage']['ajax']['MaxCharge4'] = $rate['MaxCharge4'];
  }
  $form['ajax']['MaxCharge4'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MaxCharge4'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['MaxTime4HHMM'])) {
    $form_state['storage']['ajax']['MaxTime4HHMM'] = $rate['MaxTime4'];
  }
  $form['ajax']['MaxTime4HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MaxTime4HHMM'],
	  '#description' => t('Format: HH:MM'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td></tr>');

  // Period 5
  $form['ajax']['Period5'] =
    array('#type' => 'item',
	  '#title' => t('P5'),
	  '#prefix' => '<tr><td>',
	  '#suffix' => '</td>');

  if (!isset($form_state['storage']['ajax']['Length5HHMM'])) {
    if ($rate['Length5'] !== '00:00') {
      $form_state['storage']['ajax']['Length5HHMM'] = $rate['Length5'];
    } else {
      $form_state['storage']['ajax']['Length5HHMM'] = '';
    }
  }
  $form['ajax']['Length5HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['Length5HHMM'],
	  '#description' => t('Format: HH:MM<br>(Blank if not used)'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['TimeStep5HHMM'])) {
    $form_state['storage']['ajax']['TimeStep5HHMM'] = $rate['TimeStep5'];
  }
  $form['ajax']['TimeStep5HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['TimeStep5HHMM'],
	  '#description' => t('Format: HH:MM'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['ChargeStep5'])) {
    $form_state['storage']['ajax']['ChargeStep5'] = $rate['ChargeStep5'];
  }
  $form['ajax']['ChargeStep5'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['ChargeStep5'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['CutOffTime5'])) {
    $form_state['storage']['ajax']['CutOffTime5'] = $rate['CutOffTime5'];
  }
  $form['ajax']['CutOffTime5'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['CutOffTime5'],
	  '#description' => t('Format: HH:MM<br>(Blank if not used)'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['MaxCharge5'])) {
    $form_state['storage']['ajax']['MaxCharge5'] = $rate['MaxCharge5'];
  }
  $form['ajax']['MaxCharge5'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MaxCharge5'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['MaxTime5HHMM'])) {
    $form_state['storage']['ajax']['MaxTime5HHMM'] = $rate['MaxTime5'];
  }
  $form['ajax']['MaxTime5HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MaxTime5HHMM'],
	  '#description' => t('Format: HH:MM'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td></tr>');

  // Period 6
  $form['ajax']['Period6'] =
    array('#type' => 'item',
	  '#title' => t('P6'),
	  '#prefix' => '<tr><td>',
	  '#suffix' => '</td>');

  if (!isset($form_state['storage']['ajax']['Length6HHMM'])) {
    if ($rate['Length6'] !== '00:00') {
      $form_state['storage']['ajax']['Length6HHMM'] = $rate['Length6'];
    } else {
      $form_state['storage']['ajax']['Length6HHMM'] = '';
    }
  }
  $form['ajax']['Length6HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['Length6HHMM'],
	  '#description' => t('Format: HH:MM<br>(Blank if not used)'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['TimeStep6HHMM'])) {
    $form_state['storage']['ajax']['TimeStep6HHMM'] = $rate['TimeStep6'];
  }
  $form['ajax']['TimeStep6HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['TimeStep6HHMM'],
	  '#description' => t('Format: HH:MM'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['ChargeStep6'])) {
    $form_state['storage']['ajax']['ChargeStep6'] = $rate['ChargeStep6'];
  }
  $form['ajax']['ChargeStep6'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['ChargeStep6'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['CutOffTime6'])) {
    $form_state['storage']['ajax']['CutOffTime6'] = $rate['CutOffTime6'];
  }
  $form['ajax']['CutOffTime6'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['CutOffTime6'],
	  '#description' => t('Format: HH:MM<br>(Blank if not used)'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['MaxCharge6'])) {
    $form_state['storage']['ajax']['MaxCharge6'] = $rate['MaxCharge6'];
  }
  $form['ajax']['MaxCharge6'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MaxCharge6'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['MaxTime6HHMM'])) {
    $form_state['storage']['ajax']['MaxTime6HHMM'] = $rate['MaxTime6'];
  }
  $form['ajax']['MaxTime6HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MaxTime6HHMM'],
	  '#description' => t('Format: HH:MM'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td></tr>');

  // Period 7
  $form['ajax']['Period7'] =
    array('#type' => 'item',
	  '#title' => t('P7'),
	  '#prefix' => '<tr><td>',
	  '#suffix' => '</td>');

  if (!isset($form_state['storage']['ajax']['Length7HHMM'])) {
    if ($rate['Length7'] !== '00:00') {
      $form_state['storage']['ajax']['Length7HHMM'] = $rate['Length7'];
    } else {
      $form_state['storage']['ajax']['Length7HHMM'] = '';
    }
  }
  $form['ajax']['Length7HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['Length7HHMM'],
	  '#description' => t('Format: HH:MM<br>(Blank if not used)'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['TimeStep7HHMM'])) {
    $form_state['storage']['ajax']['TimeStep7HHMM'] = $rate['TimeStep7'];
  }
  $form['ajax']['TimeStep7HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['TimeStep7HHMM'],
	  '#description' => t('Format: HH:MM'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['ChargeStep7'])) {
    $form_state['storage']['ajax']['ChargeStep7'] = $rate['ChargeStep7'];
  }
  $form['ajax']['ChargeStep7'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['ChargeStep7'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['CutOffTime7'])) {
    $form_state['storage']['ajax']['CutOffTime7'] = $rate['CutOffTime7'];
  }
  $form['ajax']['CutOffTime7'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['CutOffTime7'],
	  '#description' => t('Format: HH:MM<br>(Blank if not used)'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['MaxCharge7'])) {
    $form_state['storage']['ajax']['MaxCharge7'] = $rate['MaxCharge7'];
  }
  $form['ajax']['MaxCharge7'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MaxCharge7'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['MaxTime7HHMM'])) {
    $form_state['storage']['ajax']['MaxTime7HHMM'] = $rate['MaxTime7'];
  }
  $form['ajax']['MaxTime7HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MaxTime7HHMM'],
	  '#description' => t('Format: HH:MM'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td></tr>');

  // Period 8
  $form['ajax']['Period8'] =
    array('#type' => 'item',
	  '#title' => t('P8'),
	  '#prefix' => '<tr><td>',
	  '#suffix' => '</td>');

  if (!isset($form_state['storage']['ajax']['Length8HHMM'])) {
    if ($rate['Length8'] !== '00:00') {
      $form_state['storage']['ajax']['Length8HHMM'] = $rate['Length8'];
    } else {
      $form_state['storage']['ajax']['Length8HHMM'] = '';
    }
  }
  $form['ajax']['Length8HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['Length8HHMM'],
	  '#description' => t('Format: HH:MM<br>(Blank if not used)'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['TimeStep8HHMM'])) {
    $form_state['storage']['ajax']['TimeStep8HHMM'] = $rate['TimeStep8'];
  }
  $form['ajax']['TimeStep8HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['TimeStep8HHMM'],
	  '#description' => t('Format: HH:MM'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['ChargeStep8'])) {
    $form_state['storage']['ajax']['ChargeStep8'] = $rate['ChargeStep8'];
  }
  $form['ajax']['ChargeStep8'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['ChargeStep8'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['CutOffTime8'])) {
    $form_state['storage']['ajax']['CutOffTime8'] = $rate['CutOffTime8'];
  }
  $form['ajax']['CutOffTime8'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['CutOffTime8'],
	  '#description' => t('Format: HH:MM<br>(Blank if not used)'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['MaxCharge8'])) {
    $form_state['storage']['ajax']['MaxCharge8'] = $rate['MaxCharge8'];
  }
  $form['ajax']['MaxCharge8'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MaxCharge8'],
	  '#prefix' => '<td>',
	  '#suffix' => '</td>');
  if (!isset($form_state['storage']['ajax']['MaxTime8HHMM'])) {
    $form_state['storage']['ajax']['MaxTime8HHMM'] = $rate['MaxTime8'];
  }
  $form['ajax']['MaxTime8HHMM'] =
    array('#type' => 'textfield',
	  '#maxlength' => 10,
	  '#size' => 15,
	  '#default_value' => $form_state['storage']['ajax']['MaxTime8HHMM'],
	  '#description' => t('Format: HH:MM'),
	  '#prefix' => '<td>',
	  '#suffix' => '</td></tr></table>');

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpperm_meterrate_add_form_validate_save'),
	  '#submit' => array('chpperm_meterrate_edit_form_submit_save'));

  $form['ajax']['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpperm_meterrate_edit_form_submit_cancel'));
  return $form;
}

function chpperm_meterrate_edit_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermtypes';
}

function chpperm_meterrate_edit_form_submit_save($form, &$form_state) {
  global $user;
  $id = $form_state['values']['id'];
  $rate = chdbccpark_get_rate_id($id); //anai_dbg($rate);
  $company_id = $rate['CompanyId'];
  $container_id = $rate['ContainerId'];
  $rate_id = $rate['RateId'];

  $alias = check_plain($form_state['values']['ajax']['TypeAlias']);

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

  $len1 = $len2 = $len3 = $len4 = $len5 = $len6 = $len7 = $len8 = '00:00';

  if (!empty($form_state['values']['ajax']['Length1HHMM'])) {
    $len1 = $form_state['values']['ajax']['Length1HHMM'];
  }
  $time1 = $form_state['values']['ajax']['TimeStep1HHMM'];
  $charge1 = $form_state['values']['ajax']['ChargeStep1'];
  $cutoff1 = $form_state['values']['ajax']['CutOffTime1'];
  $maxcharge1 = $form_state['values']['ajax']['MaxCharge1'];
  $maxtime1 = $form_state['values']['ajax']['MaxTime1HHMM'];

  if (!empty($form_state['values']['ajax']['Length2HHMM'])) {
    $len2 = $form_state['values']['ajax']['Length2HHMM'];
  }
  $time2 = $form_state['values']['ajax']['TimeStep2HHMM'];
  $charge2 = $form_state['values']['ajax']['ChargeStep2'];
  $cutoff2 = $form_state['values']['ajax']['CutOffTime2'];
  $maxcharge2 = $form_state['values']['ajax']['MaxCharge2'];
  $maxtime2 = $form_state['values']['ajax']['MaxTime2HHMM'];

  if (!empty($form_state['values']['ajax']['Length3HHMM'])) {
    $len3 = $form_state['values']['ajax']['Length3HHMM'];
  }
  $time3 = $form_state['values']['ajax']['TimeStep3HHMM'];
  $charge3 = $form_state['values']['ajax']['ChargeStep3'];
  $cutoff3 = $form_state['values']['ajax']['CutOffTime3'];
  $maxcharge3 = $form_state['values']['ajax']['MaxCharge3'];
  $maxtime3 = $form_state['values']['ajax']['MaxTime3HHMM'];

  if (!empty($form_state['values']['ajax']['Length4HHMM'])) {
    $len4 = $form_state['values']['ajax']['Length4HHMM'];
  }
  $time4 = $form_state['values']['ajax']['TimeStep4HHMM'];
  $charge4 = $form_state['values']['ajax']['ChargeStep4'];
  $cutoff4 = $form_state['values']['ajax']['CutOffTime4'];
  $maxcharge4 = $form_state['values']['ajax']['MaxCharge4'];
  $maxtime4 = $form_state['values']['ajax']['MaxTime4HHMM'];

  if (!empty($form_state['values']['ajax']['Length5HHMM'])) {
    $len5 = $form_state['values']['ajax']['Length5HHMM'];
  }
  $time5 = $form_state['values']['ajax']['TimeStep5HHMM'];
  $charge5 = $form_state['values']['ajax']['ChargeStep5'];
  $cutoff5 = $form_state['values']['ajax']['CutOffTime5'];
  $maxcharge5 = $form_state['values']['ajax']['MaxCharge5'];
  $maxtime5 = $form_state['values']['ajax']['MaxTime5HHMM'];

  if (!empty($form_state['values']['ajax']['Length6HHMM'])) {
    $len6 = $form_state['values']['ajax']['Length6HHMM'];
  }
  $time6 = $form_state['values']['ajax']['TimeStep6HHMM'];
  $charge6 = $form_state['values']['ajax']['ChargeStep6'];
  $cutoff6 = $form_state['values']['ajax']['CutOffTime6'];
  $maxcharge6 = $form_state['values']['ajax']['MaxCharge6'];
  $maxtime6 = $form_state['values']['ajax']['MaxTime6HHMM'];

  if (!empty($form_state['values']['ajax']['Length7HHMM'])) {
    $len7 = $form_state['values']['ajax']['Length7HHMM'];
  }
  $time7 = $form_state['values']['ajax']['TimeStep7HHMM'];
  $charge7 = $form_state['values']['ajax']['ChargeStep7'];
  $cutoff7 = $form_state['values']['ajax']['CutOffTime7'];
  $maxcharge7 = $form_state['values']['ajax']['MaxCharge7'];
  $maxtime7 = $form_state['values']['ajax']['MaxTime7HHMM'];

  if (!empty($form_state['values']['ajax']['Length8HHMM'])) {
    $len8 = $form_state['values']['ajax']['Length8HHMM'];
  }
  $time8 = $form_state['values']['ajax']['TimeStep8HHMM'];
  $charge8 = $form_state['values']['ajax']['ChargeStep8'];
  $cutoff8 = $form_state['values']['ajax']['CutOffTime8'];
  $maxcharge8 = $form_state['values']['ajax']['MaxCharge8'];
  $maxtime8 = $form_state['values']['ajax']['MaxTime8HHMM'];

  if (chdbccpark_update_rate
      ($id, $alias,
       $min_time, $min_charge, $begin, $end, $days,
       $len1, $time1, $charge1, $cutoff1, $maxcharge1, $maxtime1,
       $len2, $time2, $charge2, $cutoff2, $maxcharge2, $maxtime2,
       $len3, $time3, $charge3, $cutoff3, $maxcharge3, $maxtime3,
       $len4, $time4, $charge4, $cutoff4, $maxcharge4, $maxtime4,
       $len5, $time5, $charge5, $cutoff5, $maxcharge5, $maxtime5,
       $len6, $time6, $charge6, $cutoff6, $maxcharge6, $maxtime6,
       $len7, $time7, $charge7, $cutoff7, $maxcharge7, $maxtime7,
       $len8, $time8, $charge8, $cutoff8, $maxcharge8, $maxtime8,
       $id)) {
    drupal_set_message('Could not insert rate', 'error');
    return;
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermtypes';
}



/**********
 ********** Meter max edit
 ********** Address: chppermmaxedit
 ********** Access:  'anai chp edit permit container'
 *********/



function chpperm_metermax_edit_form($form_state, $id) {
  if ($form_state['storage']['confirm_delete']) {
    return confirm_form($form, 'Are you sure?', 'chppermtypes',
			'The maximum rate will be deleted from the system.');
  }

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $maxcharge = chdbccpark_get_maxcharge_id($id);
  $companies = chpprop_retrieve_companies($user->uid);

  // Sanity check permits
  if (!in_array($maxcharge['CompanyId'], array_keys($companies))) {
    unset($form_state['storage']);
    drupal_access_denied();
    exit();
  }

  drupal_set_title(t('Edit meter rate'));
  $company_id = $maxcharge['CompanyId'];
  $container_id = $maxcharge['ContainerId'];

  // Remember type id
  $form['id'] = array('#type' => 'hidden', '#value' => $id);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  // Layout alias field
  if (!isset($form_state['storage']['ajax']['TypeAlias'])) {
    $form_state['storage']['ajax']['TypeAlias'] = decode_entities($maxcharge['Alias']);
  }
  $form['ajax']['TypeAlias'] =
    array('#type' => 'textfield',
	  '#title' => t('Rate name'),
	  '#maxlength' => 60,
	  '#default_value' => $form_state['storage']['ajax']['TypeAlias']);

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
          Drupal.behaviors.ANAIchpcontainer=function(context){
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
	  '#validate' => array('chpperm_container_add_form_validate_save'),
	  '#submit' => array('chpperm_metermax_edit_form_submit_save'));

  $form['ajax']['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpperm_metermax_edit_form_submit_cancel'));
  return $form;
}

function chpperm_metermax_edit_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermtypes';
}

function chpperm_metermax_edit_form_submit_save($form, &$form_state) {
  global $user;
  $id = $form_state['values']['id'];
  $maxcharge = chdbccpark_get_maxcharge_id($id);
  $company_id = $maxcharge['CompanyId'];
  $container_id = $maxcharge['ContainerId'];
  $rate_id = $maxcharge['RateId'];

  $alias = check_plain($form_state['values']['ajax']['TypeAlias']);

  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermtypes';
}



/**********
 ********** Meter pass edit
 ********** Address: chppermpassedit
 ********** Access:  'anai chp edit permit container'
 *********/



function chpperm_meterpass_edit_form($form_state, $id) {
  if ($form_state['storage']['confirm_delete']) {
    return confirm_form($form, 'Are you sure?', 'chppermtypes',
			'The pass will be deleted from the system.');
  }

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $pass = chdbccpark_get_pass_id($id);
  $companies = chpprop_retrieve_companies($user->uid);

  // Sanity check permits
  if (!in_array($pass['CompanyId'], array_keys($companies))) {
    unset($form_state['storage']);
    drupal_access_denied();
    exit();
  }

  drupal_set_title(t('Edit meter rate'));
  $company_id = $pass['CompanyId'];
  $container_id = $pass['ContainerId'];

  // Remember type id
  $form['id'] = array('#type' => 'hidden', '#value' => $id);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  // Layout alias field
  if (!isset($form_state['storage']['ajax']['TypeAlias'])) {
    $form_state['storage']['ajax']['TypeAlias'] = decode_entities($pass['Alias']);
  }
  $form['ajax']['TypeAlias'] =
    array('#type' => 'textfield',
	  '#title' => t('Rate name'),
	  '#maxlength' => 60,
	  '#default_value' => $form_state['storage']['ajax']['TypeAlias']);



  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpperm_container_add_form_validate_save'),
	  '#submit' => array('chpperm_meterpass_edit_form_submit_save'));

  $form['ajax']['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpperm_meterpass_edit_form_submit_cancel'));
  return $form;
}

function chpperm_meterpass_edit_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermtypes';
}

function chpperm_meterpass_edit_form_submit_save($form, &$form_state) {
  global $user;
  $id = $form_state['values']['id'];
  $pass = chdbccpark_get_pass_id($id);
  $company_id = $pass['CompanyId'];
  $container_id = $pass['ContainerId'];
  $pass_id = $pass['PassId'];

  $alias = check_plain($form_state['values']['ajax']['TypeAlias']);

  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermtypes';
}



/**********
 ********** Merchant code edit
 ********** Address: chppermmcedit
 ********** Access:  'anai chp edit permit container'
 *********/



function chpperm_merchantcode_edit_form($form_state, $id) {
  if ($form_state['storage']['confirm_delete']) {
    return confirm_form($form, 'Are you sure?', 'chppermtypes',
			'The merchant code will be deleted from the system.');
  }

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $container = chdbperm_get_container_id($id);
  $companies = chpprop_retrieve_companies($user->uid);

  // Sanity check permits
  if (!in_array($container['CompanyId'], array_keys($companies))) {
    unset($form_state['storage']);
    drupal_access_denied();
    exit();
  }

  drupal_set_title(t('Edit merchant codes'));
  $company_id = $container['CompanyId'];
  $container_id = $container['ContainerId'];
  $merchantcodes = chdbccpark_get_merchantcodes($company_id, $container_id);

  // Remember type id
  $form['id'] = array('#type' => 'hidden', '#value' => $id);

  $form['ajax'] =
    array('#prefix' => '<div id="ajax-wrapper">',
	  '#suffix' => '</div>',
	  '#tree' => TRUE);

  // Layout number of merchant codes
  $options = array();
  for ($i = 0; $i <= 25; $i++) {
    $options[$i] = $i;
  }
  if (!isset($form_state['storage']['ajax']['NumMerchantCodes'])) {
    if (empty($merchantcodes)) {
      $form_state['storage']['ajax']['NumMerchantCodes'] = key($options);
    } else {
      $form_state['storage']['ajax']['NumMerchantCodes'] = count($merchantcodes);
    }
  }
  $form['ajax']['NumMerchantCodes'] =
    array('#type' => 'select',
	  '#title' => t('Number of merchant codes'),
	  '#options' => $options,
	  '#default_value' => $form_state['storage']['ajax']['NumMerchantCodes'],
	  '#ahah' => array('event' => 'change',
			   'path' => ahah_helper_path(array('ajax')),
			   'wrapper' => 'ajax-wrapper'));

  // Layout merchant code entries
  $form['ajax']['Merchants'] =
    array('#prefix' => '<table>',
	  '#suffix' => '</table>');

  for ($i = 0; $i < $form_state['storage']['ajax']['NumMerchantCodes']; $i++) {

    // Layout Code
    if (!isset($form_state['storage']['ajax']['Merchants']['Code'.$i])) {
      $form_state['storage']['ajax']['Merchants']['Code'.$i] =
	decode_entities($merchantcodes[$i]['Code']);
    }
    $form['ajax']['Merchants']['Code'.$i] =
      array('#type' => 'textfield',
	    '#title' => t('Code'),
	    '#maxlength' => 15,
	    '#size' => 20,
	    '#default_value' => $form_state['storage']['ajax']['Merchants']['Code'.$i],
	    '#description' => '(No spaces)',
	    '#attributes' => array('onBlur'=>'this.value=this.value.toUpperCase()'),
	    '#prefix' => '<tr><td>',
	    '#suffix' => '</td>');

    // Layout discount type
    // Tore 131116: Added Flat and Capped
    $options = array();
    $options['Fixed'] = t('Discount amount');
    $options['Flat'] = t('Flat amount');
    $options['Capped'] = t('Capped amount');
    $options['Percentage'] = t('Discount percentage');
    if (!isset($form_state['storage']['ajax']['Merchants']['Discount'.$i])) {
      if (in_array($merchantcodes[$i]['Mode'], array_keys($options))) {
	$form_state['storage']['ajax']['Merchants']['Discount'.$i] =
	  $merchantcodes[$i]['Mode'];
      } else {
	$form_state['storage']['ajax']['Merchants']['Discount'.$i] = key($options);
      }
    }
    $form['ajax']['Merchants']['Discount'.$i] =
      array('#type' => 'select',
	    '#title' => t('Discount type'),
	    '#options' => $options,
	    '#default_value' => $form_state['storage']['ajax']['Merchants']['Discount'.$i],
	    '#prefix' => '<td>',
	    '#suffix' => '</td>',
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));

    if ($form_state['storage']['ajax']['Merchants']['Discount'.$i] === 'Fixed' or
	$form_state['storage']['ajax']['Merchants']['Discount'.$i] === 'Flat' or
	$form_state['storage']['ajax']['Merchants']['Discount'.$i] === 'Capped') {
      // Layout amount seletor
      // Tore 131126: Changed from select widget to textfield widget
      if (!isset($form_state['storage']['ajax']['Merchants']['Amount'.$i])) {
	$form_state['storage']['ajax']['Merchants']['Amount'.$i] =
	  $merchantcodes[$i]['Amount'];
      }
      $form['ajax']['Merchants']['Amount'.$i] =
	array('#type' => 'textfield',
	      '#title' => t('Amount'),
	      '#maxlength' => 10,
	      '#size' => 15,
	      '#default_value' => $form_state['storage']['ajax']['Merchants']['Amount'.$i],
	      '#prefix' => '<td>',
	      '#suffix' => '</td></tr>');
    } elseif ($form_state['storage']['ajax']['Merchants']['Discount'.$i] === 'Percentage') {
      // Layout percentage seletor
      // Tore 131201: Changed from select widget to textfield widget
      if (!isset($form_state['storage']['ajax']['Merchants']['Percentage'.$i])) {
	$form_state['storage']['ajax']['Merchants']['Percentage'.$i] = $merchantcodes[$i]['Percentage'];
      }
      $form['ajax']['Merchants']['Percentage'.$i] =
	array('#type' => 'textfield',
	      '#title' => t('Percentage'),
	      '#maxlength' => 10,
	      '#size' => 15,
	      '#default_value' => $form_state['storage']['ajax']['Merchants']['Percentage'.$i],
	      '#prefix' => '<td>',
	      '#suffix' => '</td></tr>');
    }
  }

  $form['ajax']['save'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/save.png',
	  '#validate' => array('chpperm_container_add_form_validate_save'),
	  '#submit' => array('chpperm_merchantcode_edit_form_submit_save'));

  $form['ajax']['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/cancel.png',
	  '#submit' => array('chpperm_merchantcode_edit_form_submit_cancel'));
  return $form;
}

function chpperm_merchantcode_edit_form_submit_cancel($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermtypes';
}

function chpperm_merchantcode_edit_form_submit_save($form, &$form_state) {
  global $user;
  $id = $form_state['values']['id'];
  $container = chdbperm_get_container_id($id);
  $company_id = $container['CompanyId'];
  $container_id = $container['ContainerId'];
  $merchantcodes = chdbccpark_get_merchantcodes($company_id, $container_id);
  $type_id = $merchantcodes[0]['TypeId'];

  // Tore 131201: Added
  $meters_need_update = FALSE;

  $n = 0;
  foreach ($merchantcodes as $n => $merchantcode) {
    if ($n < $form_state['values']['ajax']['NumMerchantCodes']) {
      // Update
      $code = check_plain($form_state['values']['ajax']['Merchants']['Code'.$n]);
      $mode = $form_state['values']['ajax']['Merchants']['Discount'.$n];
      $amount = (isset($form_state['values']['ajax']['Merchants']['Amount'.$n]) ?
		 $form_state['values']['ajax']['Merchants']['Amount'.$n] : '0.00');
      $percentage = (isset($form_state['values']['ajax']['Merchants']['Percentage'.$n]) ?
		     $form_state['values']['ajax']['Merchants']['Percentage'.$n] : 0);
      if (chdbccpark_update_merchantcode_id
	  ($merchantcode['Id'],
	   $code, $mode, $amount, $percentage)) {
	drupal_set_message('Could not update merchant code', 'error');
	return;
      }
      $meters_need_update = TRUE; // Tore 131201: Added
    } else {
      // Delete
      if (chdbccpark_delete_merchantcode_id($merchantcode['Id'])) {
	drupal_set_message('Could not delete merchant code', 'error');
	return;
      }
      $meters_need_update = TRUE; // Tore 131201: Added
    }
  }

  $n = 0 < $n ? $n + 1 : 0;
  if ($n < $form_state['values']['ajax']['NumMerchantCodes']) {
    // Insert
    for ($i = $n; $i < $form_state['values']['ajax']['NumMerchantCodes']; $i++) {
      $code = check_plain($form_state['values']['ajax']['Merchants']['Code'.$i]);
      $mode = $form_state['values']['ajax']['Merchants']['Discount'.$i];
      $amount = (isset($form_state['values']['ajax']['Merchants']['Amount'.$i]) ?
		 $form_state['values']['ajax']['Merchants']['Amount'.$i] : '0.00');
      $percentage = (isset($form_state['values']['ajax']['Merchants']['Percentage'.$i]) ?
		     $form_state['values']['ajax']['Merchants']['Percentage'.$i] : 0);
      if (chdbccpark_insert_merchantcode
	  ($company_id, $container_id, $type_id,
	   $code, $mode, $amount, $percentage,
	   $tmp)) {
	drupal_set_message('Could not insert merchant code', 'error');
	return;
      }
      $meters_need_update = TRUE; // Tore 131201: Added
    }
  }

  // Tore 131201: Added
  if ($meters_need_update) {
    if (chdbccpark_update_merchantcodemeter_alt
	($company_id, $type_id)) {
      drupal_set_message('Could not push merchant code', 'error');
      return;
    }
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermtypes';
}



/**********
 ********** Container list
 ********** Address: chppermtypes
 ********** Access:  'anai chp list permit container'
 *********/



function chpperm_conttype_form($form_state) {
  drupal_set_title(t('Containers'));

  if ($form_state['storage']['detailed_view']) {
    unset($form_state['storage']['detailed_view']);
    return chpperm_conttype_view_form($form_state);
  }
  if ($form_state['storage']['container_edit']) {
    return chpperm_container_edit_form($form_state);
  }

  global $user;
  $form = array();
  ahah_helper_register($form, $form_state);

  $settings = array();
  $settings['tag'] = 'ajax';
  $settings['show_companies'] = TRUE;
  $settings['buttons']['back']['title'] = t('Back');
  $settings['buttons']['back']['submit'] = 'chpperm_conttype_form_submit_back';
  if (user_access('anai chp add permit container')) {
    $settings['buttons']['add']['title'] = t('Add container');
    $settings['buttons']['add']['submit'] = 'chpperm_conttype_form_submit_add_container';
  }
  if (chpprop_produce_header(&$form, &$form_state, $settings)) {
    return $form;
  }
  $company_id = $form_state['storage']['ajax']['CompanyId'];

  $containers = chpperm_retrieve_containers($user->uid, $company_id);

  if (count($containers) == 0) {
    $form['ajax']['empty'] = array('#value' => '<p>'.t('Empty.'));
  } else {
    $form['ajax']['list'] = chpperm_conttype_table_form($company_id, $containers);
  }

  return $form;
}

function chpperm_conttype_form_submit_add_container($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermcontadd';
}

function chpperm_conttype_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'front_page';
}

function chpperm_conttype_table_form($company_id, $containers) {
  global $user;
  $form['db'] =
    array('#type' => 'markup', '#value' => '',
	  '#theme' => 'chpperm_conttype_table');

  if (user_access('anai chp edit permit container')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }

  $looper = 0;
  foreach ($containers as $container_id => $container) {
    $form['db']['container_'.$looper] = array('#value' => decode_entities($container['data']['Alias']));

    if ($readonly) {
      $form['db']['contedit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['contedit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_contedit'.$container['data']['Id'],
	      '#submit' => array('chpperm_conttype_form_submit'));
    }

    $list = array();
    $conttypes = chpperm_retrieve_container_types($user->uid, $company_id, $container_id);
    $contrates = chdbccpark_retrieve_container_rates($user->uid, $company_id, $container_id);
    $contmaxs = chdbccpark_retrieve_container_maxcharge($user->uid, $company_id, $container_id);
    $contpasses = chdbccpark_retrieve_container_passes($user->uid, $company_id, $container_id);
    $merchantcodes = chdbccpark_retrieve_container_merchantcodes($user->uid, $company_id, $container_id);

    foreach ($conttypes as $conttype) {
      $list[] = decode_entities($conttype['data']['Alias']);
    }
    foreach ($contrates as $conttype) {
      $list[] = decode_entities($conttype['data']['Alias']);
    }
    foreach ($contmaxs as $contmax) {
      $list[] = decode_entities($contmax['data']['Alias']);
    }
    foreach ($contpasses as $contpass) {
      $list[] = decode_entities($contpass['data']['Alias']);
    }

    if (!empty($merchantcodes)) {
      $list[] = t('Merchant codes');
    }

    $form['db']['conttype_'.$looper] = array('#value' => theme_item_list($list));

    if (!empty($list)) {
      $form['db']['details_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_details'.$container['data']['Id'],
	      '#submit' => array('chpperm_conttype_form_submit'));
    } else {
      $form['db']['details_'.$looper] = array('#value' => '');
    }

    $looper++;
  }
  return $form;
}

function theme_chpperm_conttype_table($form) {
  $header = array(array('data' => t('Container'),
			'colspan' => 2),
		  array('data' => t('Permit types'),
			'colspan' => 2),
		  );
  $rows = array();
  foreach ($form as $key => $value) {
    if (!strstr($key, 'container_')) {
      continue;
    }
    $looper = substr($key, strlen('container_'));
    $rows[] = array(drupal_render($form['container_'.$looper]),
		    array('data' => drupal_render($form['contedit_'.$looper]),
			  'width' => 1, 'height' => 1),
		    drupal_render($form['conttype_'.$looper]),
		    array('data' => drupal_render($form['details_'.$looper]),
			  'width' => 1, 'height' => 1),
                    );
  }
  return theme_table($header, $rows);
}

function chpperm_conttype_view_form($form_state) {
  global $user;
  $id = $form_state['storage']['id'];
  $container = chdbperm_get_container_id($id);
  $company_id = $container['CompanyId'];
  $container_id = $container['ContainerId'];
  $conttypes = chpperm_retrieve_container_types($user->uid, $company_id, $container_id);
  $contrates = chdbccpark_retrieve_container_rates($user->uid, $company_id, $container_id);
  $contmaxcharges = chdbccpark_retrieve_container_maxcharge($user->uid, $company_id, $container_id);
  $contpasses = chdbccpark_retrieve_container_passes($user->uid, $company_id, $container_id);
  $merchantcodes = chdbccpark_retrieve_container_merchantcodes($user->uid, $company_id, $container_id);

  drupal_set_title(t('Container - NAME',
		     array('NAME' => decode_entities($container['Alias']))));

  $form['back'] =
    array('#type' => 'image_button',
	  '#src' => drupal_get_path('module', 'anai').'/back.png',
	  '#submit' => array('chpperm_conttype_view_form_submit_back'));

  $form['list'] =
    chpperm_conttype_view_table_form($id,
				     $conttypes,
				     $contrates,
				     $contmaxcharges,
				     $contpasses,
				     $merchantcodes);
  return $form;
}

function chpperm_conttype_view_form_submit_back($form, &$form_state) {
  unset($form_state['storage']);
  $form_state['redirect'] = 'chppermtypes';
}

function chpperm_conttype_view_table_form($id,
					  $conttypes,
					  $contrates,
					  $contmaxcharges,
					  $contpasses,
					  $merchantcodes) {
  $form['db'] =
    array('#type' => 'markup', '#value' => '',
	  '#theme' => 'chpperm_conttype_view_table');

  if (user_access('anai chp edit permit container')) {
    $readonly = FALSE;
  } else {
    $readonly = TRUE;
  }

  $looper = 0;

  foreach ($conttypes as $entry) {
    $n = 0;
    $form['db']['alias_'.$looper] = array('#value' => $entry['data']['Alias']);

    $form['db']['item_'.$n.'_'.$looper] = array('#value' => t('Structure'));

    $tag = '';
    if ($entry['data']['Long']) {
      $tag .= t('Long term property, ');
    }
    if ($entry['data']['Short']) {
      $tag .= t('Short term property, ');
    }
    if ($entry['data']['Groups']) {
      if ($entry['data']['Charging'] === 'Subscribed') {
	$tag .= t('Subscribed group permit, ');
      } elseif ($entry['data']['Charging'] === 'Complementary') {
	$tag .= t('Complementary group permit, ');
      }
    }
    $tag .= chpperm_structure_name($entry['data']['RateType']);
    $form['db']['value_'.$n.'_'.$looper] =
      array('#value' => $tag);
    $n++;

    if ($entry['data']['RateType'] === 'Linear') {
      $form['db']['item_'.$n.'_'.$looper] =
	array('#value' => t('Linear'));
      $form['db']['value_'.$n.'_'.$looper] =
	array('#value' =>
	      sprintf("%0.02f", $entry['data']['UnitValue']).
	      t(' per ').
	      $entry['data']['ValueUnit']);
      $n++;

      $form['db']['item_'.$n.'_'.$looper] =
	array('#value' => t('Max cost'));
      $form['db']['value_'.$n.'_'.$looper] =
	array('#value' => sprintf("%0.02f", $entry['data']['MaxCost']));
      $n++;
    } elseif ($entry['data']['RateType'] === 'Flat') {
      $form['db']['item_'.$n.'_'.$looper] =
	array('#value' => t('Cost'));
      $form['db']['value_'.$n.'_'.$looper] =
	array('#value' => sprintf("%0.02f", $entry['data']['UnitValue']));
      $n++;
    } elseif ($entry['data']['RateType'] === 'Fixed') {
      $form['db']['item_'.$n.'_'.$looper] =
	array('#value' => t('Cost'));
      $form['db']['value_'.$n.'_'.$looper] =
	array('#value' => sprintf("%0.02f", $entry['data']['UnitValue']));
      $n++;

      $form['db']['item_'.$n.'_'.$looper] =
	array('#value' => t('Duration'));
      $form['db']['value_'.$n.'_'.$looper] =
	array('#value' => $entry['data']['FixedPeriod'].' '.$entry['data']['ValueUnit']);
      $n++;
    }

    if (!empty($entry['data']['Comment'])) {
      $form['db']['item_'.$n.'_'.$looper] =
	array('#value' => t('Comment'));
      $form['db']['value_'.$n.'_'.$looper] =
	array('#value' => decode_entities($entry['data']['Comment']));
      $n++;
    }

    $form['db']['numrows_'.$looper.'_'.$n] =
      array('#type' => 'hidden', '#value' => '');

    if ($readonly) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_edit'.$entry['data']['Id'],
	      '#submit' => array('chpperm_conttype_form_submit'));
    }
    $looper++;
  }

  foreach ($contmaxcharges as $entry) {
    $n = 0;
    $form['db']['alias_'.$looper] = array('#value' => $entry['data']['Alias']);

    $form['db']['item_'.$n.'_'.$looper] =
      array('#value' => t('Type'));
    $form['db']['value_'.$n.'_'.$looper] =
      array('#value' => t('Meter maximum charge'));
    $n++;

    $form['db']['item_'.$n.'_'.$looper] =
      array('#value' => t('Maximum charge'));
    $form['db']['value_'.$n.'_'.$looper] =
      array('#value' => $entry['data']['Charge']);
    $n++;

    if ($entry['data']['Behavior'] === 'Fixed') {
      $expiry = t('Until STR', array('STR' => $entry['data']['Time']));
    } elseif ($entry['data']['Behavior'] === 'Relative') {
      $expiry = t('Duration of STR', array('STR' => $entry['data']['Time']));
    } else {
      $expiry = t('Unknown');
    }
    $form['db']['item_'.$n.'_'.$looper] =
      array('#value' => t('Expiry'));
    $form['db']['value_'.$n.'_'.$looper] =
      array('#value' => $expiry);
    $n++;

    $form['db']['numrows_'.$looper.'_'.$n] =
      array('#type' => 'hidden', '#value' => '');

    if ($readonly) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
  	array('#type' => 'image_button',
  	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
  	      '#name' => 'op_metermax'.$entry['data']['Id'],
  	      '#submit' => array('chpperm_conttype_form_submit'));
    }
    $looper++;
  }

  foreach ($contrates as $entry) {
    $n = 0;
    $form['db']['alias_'.$looper] = array('#value' => $entry['data']['Alias']);

    $form['db']['item_'.$n.'_'.$looper] =
      array('#value' => t('Type'));
    $form['db']['value_'.$n.'_'.$looper] =
      array('#value' => t('Meter rate group'));
    $n++;

    $form['db']['item_'.$n.'_'.$looper] =
      array('#value' => t('Begin time'));
    $form['db']['value_'.$n.'_'.$looper] =
      array('#value' => $entry['data']['Begin']);
    $n++;

    $form['db']['item_'.$n.'_'.$looper] =
      array('#value' => t('End time'));
    $form['db']['value_'.$n.'_'.$looper] =
      array('#value' => $entry['data']['End']);
    $n++;

    $days = '';
    for ($i = 0; $i < strlen($entry['data']['Days']); $i++) {
      if ($entry['data']['Days'][$i] == 1) {
	$days .= t('Monday, ');
      }
      if ($entry['data']['Days'][$i] == 2) {
	$days .= t('Tuesday, ');
      }
      if ($entry['data']['Days'][$i] == 3) {
	$days .= t('Wednesday, ');
      }
      if ($entry['data']['Days'][$i] == 4) {
	$days .= t('Thursday, ');
      }
      if ($entry['data']['Days'][$i] == 5) {
	$days .= t('Friday, ');
      }
      if ($entry['data']['Days'][$i] == 6) {
	$days .= t('Saturday, ');
      }
      if ($entry['data']['Days'][$i] == 7) {
	$days .= t('Sunday, ');
      }
    }
    $days = rtrim($days, ', ');
    $form['db']['item_'.$n.'_'.$looper] =
      array('#value' => t('Days'));
    $form['db']['value_'.$n.'_'.$looper] =
      array('#value' => $days);
    $n++;

    $form['db']['numrows_'.$looper.'_'.$n] =
      array('#type' => 'hidden', '#value' => '');

    if ($readonly) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_meterrate'.$entry['data']['Id'],
	      '#submit' => array('chpperm_conttype_form_submit'));
    }
    $looper++;
  }

  foreach ($contpasses as $entry) {
    $n = 0;
    $form['db']['alias_'.$looper] = array('#value' => $entry['data']['Alias']);

    $form['db']['item_'.$n.'_'.$looper] =
      array('#value' => t('Type'));
    $form['db']['value_'.$n.'_'.$looper] =
      array('#value' => t('Meter parking pass'));
    $n++;

    $form['db']['item_'.$n.'_'.$looper] =
      array('#value' => t('Cost'));
    $form['db']['value_'.$n.'_'.$looper] =
      array('#value' => $entry['data']['Charge']);
    $n++;

    $form['db']['item_'.$n.'_'.$looper] =
      array('#value' => t('Duration'));
    $form['db']['value_'.$n.'_'.$looper] =
      array('#value' => $entry['data']['Duration']);
    $n++;

    $form['db']['item_'.$n.'_'.$looper] =
      array('#value' => t('Display'));
    $form['db']['value_'.$n.'_'.$looper] =
      array('#value' => t('Appears in display as STR',
			  array('STR' => $entry['data']['MenuName'])));
    $n++;

    $form['db']['numrows_'.$looper.'_'.$n] =
      array('#type' => 'hidden', '#value' => '');

    if ($readonly) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_meterpass'.$entry['data']['Id'],
	      '#submit' => array('chpperm_conttype_form_submit'));
    }
    $looper++;
  }

  if (0 < count($merchantcodes)) {
    $n = 0;
    $form['db']['alias_'.$looper] = array('#value' => t('Merchant codes'));

    $form['db']['item_'.$n.'_'.$looper] =
      array('#value' => t('Number of codes'));
    $form['db']['value_'.$n.'_'.$looper] =
      array('#value' => count($merchantcodes));
    $n++;

    $form['db']['numrows_'.$looper.'_'.$n] =
      array('#type' => 'hidden', '#value' => '');

    if ($readonly) {
      $form['db']['edit_'.$looper] = array('#value' => '');
    } else {
      $form['db']['edit_'.$looper] =
	array('#type' => 'image_button',
	      '#src' => drupal_get_path('module', 'anai').'/editsmall.png',
	      '#name' => 'op_merchant'.$id,
	      '#submit' => array('chpperm_conttype_form_submit'));
    }
    $looper++;
  }

  return $form;
}

function theme_chpperm_conttype_view_table($form) {
  $header = array(array('data' => 'Parameter',
			'colspan' => 2),
		  'Setting');
  $rows = array();
  //drupal_set_message('<pre>'.var_export($form, TRUE).'</pre>');
  foreach ($form as $key => $value) {
    if (!strstr($key, 'numrows_')) {
      continue;
    }
    $looper = substr($key, strlen('numrows_'));
    $looper = substr($looper, 0, strpos($looper, '_'));
    $numrows = substr($key, strlen('numrows_'.$looper.'_'));
    //drupal_set_message('<pre>'.var_export($numrows, TRUE).'</pre>');
    $rows[] = array(array('data' => drupal_render($form['alias_'.$looper]),
			  'colspan' => 2),
		    drupal_render($form['edit_'.$looper]));

    for ($n = 0; $n < $numrows; $n++) {
      if ($n == 0) {
	$rows[] = array(array('data' => '',
			      'rowspan' => $numrows),
			drupal_render($form['item_'.$n.'_'.$looper]),
			drupal_render($form['value_'.$n.'_'.$looper]),
			);
      } else {
	$rows[] = array(drupal_render($form['item_'.$n.'_'.$looper]),
			drupal_render($form['value_'.$n.'_'.$looper]),
			);
      }
    }
  }
  return theme_table($header, $rows);
}

function chpperm_conttype_form_submit($form, &$form_state) {
  if (isset($form_state['storage']['confirm_delete'])) {
    $id = $form_state['storage']['id'];
    $container = chdbperm_get_container_id($id);
    chdbccpark_delete_rate($container['CompanyId'], $container['ContainerId']);
    chdbperm_delete_conttype($container['CompanyId'], $container['ContainerId']);
    chdbperm_delete_contprops($container['CompanyId'], $container['ContainerId']);
    chdbperm_delete_container_id($id);
    unset($form_state['storage']);
    $form_state['redirect'] = 'chppermtypes';
    return;
  }

  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_details')) {
      $id = substr($key, strlen('op_details'));
      break;
    }
  }
  if ($id > 0) {
    //drupal_set_message('Detailed view of id:'.$id);
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
    unset($form_state['storage']);
    $form_state['redirect'] = 'chppermtypeedit/'.$id;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_meterrate')) {
      $id = substr($key, strlen('op_meterrate'));
      break;
    }
  }
  if ($id > 0) {
    unset($form_state['storage']);
    $form_state['redirect'] = 'chppermrateedit/'.$id;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_metermax')) {
      $id = substr($key, strlen('op_metermax'));
      break;
    }
  }
  if ($id > 0) {
    unset($form_state['storage']);
    $form_state['redirect'] = 'chppermmaxedit/'.$id;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_meterpass')) {
      $id = substr($key, strlen('op_meterpass'));
      break;
    }
  }
  if ($id > 0) {
    unset($form_state['storage']);
    $form_state['redirect'] = 'chppermpassedit/'.$id;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_merchant')) {
      $id = substr($key, strlen('op_merchant'));
      break;
    }
  }
  if ($id > 0) {
    unset($form_state['storage']);
    $form_state['redirect'] = 'chppermmcedit/'.$id;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_propedit')) {
      $id = substr($key, strlen('op_propedit'));
      break;
    }
  }
  if ($id > 0) {
    unset($form_state['storage']);
    $form_state['redirect'] = 'chppermtypeprop/'.$id;
    return;
  }
  $id = -1;
  foreach ($form_state['values'] as $key => $value) {
    if (strstr($key, 'op_contedit')) {
      $id = substr($key, strlen('op_contedit'));
      break;
    }
  }
  if ($id > 0) {
    //drupal_set_message('Detailed view of id:'.$id);
    $form_state['storage']['id'] = $id;
    $form_state['storage']['container_edit'] = TRUE;
    return;
  }
}



