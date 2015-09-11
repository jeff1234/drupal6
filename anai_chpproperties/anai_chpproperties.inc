<?php
// $Id$
require_once("anai_chpproperties_db.inc");

define('TENANT_VALIDITY_UNLIMITED', 0);
define('TENANT_VALIDITY_ANNUAL', 1);
define('TENANT_VALIDITY_MONTH', 2);
define('TENANT_VALIDITY_DAYS', 3);

define('VISITOR_START_TIME', 0);
define('VISITOR_STOP_TIME', 1);
define('VISITOR_OVERNIGHT_START_TIME', 2);
define('VISITOR_OVERNIGHT_STOP_TIME', 3);

define('VISITOR_EXPIRY_NONE', 0);
define('VISITOR_EXPIRY_DAYS', 1);
define('VISITOR_EXPIRY_SELECT', 2);

define('VISITOR_STALL_UNMANAGED', 0);
define('VISITOR_STALL_MANAGED_TOTAL', 1);
define('VISITOR_STALL_MANAGED_STALL', 2);

define('VISITOR_REGULAR', 0);
define('VISITOR_REGULAR_OVERNIGHT', 1);
define('VISITOR_REGULAR_CONTAINER', 2);
define('VISITOR_REGULAR_OVERNIGHT_CONTAINER', 3);
define('VISITOR_CONTAINER', 4);
define('VISITOR_HOTEL', 5);
define('VISITOR_LOADING', 6);

define('TENANT_STALL_UNMANAGED', 0);
define('TENANT_STALL_MANAGED_TOTAL', 1);
define('TENANT_STALL_MANAGED_STALL', 2);

define('TENANT_REGULAR', 0);
define('TENANT_REGULAR_CONTAINER', 1);
define('TENANT_CONTAINER', 2);

define('MOBILES_ALLOWED_NONE', 0);
define('MOBILES_ALLOWED_REGULAR', 1);
define('MOBILES_RETENTION_DAYS', 30);
define('MOBILES_DEMO_ALLOWED', 5);



/***
 * Functions used for country selection menu
 */

function chpprop_retrieve_countries() {
  $countries['US'] = 'United States';
  $countries['CA'] = 'Canada';
  return $countries;
}

function chpprop_retrieve_country($iso) {
  $countries = chpprop_retrieve_countries();
  return $countries[$iso];
}

function chpprop_retrieve_iso($country) {
  $countries = chpprop_retrieve_countries();
  foreach ($countries as $iso => $value) {
    if (!strcmp($country, $value)) {
      return $iso;
    }
  }
  return 'Unknown';
}

function chpprop_default_country() {
  return 'CA';
}



/***
 * Functions used for province/state selection menu
 */

function chpprop_retrieve_states($iso) {
  //United States
  $states['US']['AL'] = 'Alabama';
  $states['US']['AK'] = 'Alaska';
  $states['US']['AZ'] = 'Arizona';
  $states['US']['AR'] = 'Arkansas';
  $states['US']['CA'] = 'California';
  $states['US']['CO'] = 'Colorado';
  $states['US']['CT'] = 'Connecticut';
  $states['US']['DE'] = 'Delaware';
  $states['US']['DC'] = 'District of Columbia';
  $states['US']['FL'] = 'Florida';
  $states['US']['GA'] = 'Georgia';
  $states['US']['HI'] = 'Hawaii';
  $states['US']['ID'] = 'Idaho';
  $states['US']['IL'] = 'Illinois';
  $states['US']['IN'] = 'Indiana';
  $states['US']['IA'] = 'Iowa';
  $states['US']['KS'] = 'Kansas';
  $states['US']['KY'] = 'Kentucky';
  $states['US']['LA'] = 'Louisiana';
  $states['US']['ME'] = 'Maine';
  $states['US']['MD'] = 'Maryland';
  $states['US']['MA'] = 'Massachusetts';
  $states['US']['MI'] = 'Michigan';
  $states['US']['MN'] = 'Minnesota';
  $states['US']['MS'] = 'Mississippi';
  $states['US']['MO'] = 'Missouri';
  $states['US']['MT'] = 'Montana';
  $states['US']['NE'] = 'Nebraska';
  $states['US']['NV'] = 'Nevada';
  $states['US']['NH'] = 'New Hampshire';
  $states['US']['NJ'] = 'New Jersey';
  $states['US']['NM'] = 'New Mexico';
  $states['US']['NY'] = 'New York';
  $states['US']['NC'] = 'North Carolina';
  $states['US']['ND'] = 'North Dakota';
  $states['US']['OH'] = 'Ohio';
  $states['US']['OK'] = 'Oklahoma';
  $states['US']['OR'] = 'Oregon';
  $states['US']['PA'] = 'Pennsylvania';
  $states['US']['RI'] = 'Rhode Island';
  $states['US']['SC'] = 'South Carolina';
  $states['US']['SD'] = 'South Dakota';
  $states['US']['TN'] = 'Tennessee';
  $states['US']['TX'] = 'Texas';
  $states['US']['UT'] = 'Utah';
  $states['US']['VT'] = 'Vermont';
  $states['US']['VI'] = 'Virgin Islands';
  $states['US']['VA'] = 'Virginia';
  $states['US']['WA'] = 'Washington';
  $states['US']['WV'] = 'West Virginia';
  $states['US']['WI'] = 'Wisconsin';
  $states['US']['WY'] = 'Wyoming';
  //Canada
  $states['CA']['AB'] = 'Alberta';
  $states['CA']['BC'] = 'British Columbia';
  $states['CA']['MB'] = 'Manitoba';
  $states['CA']['NB'] = 'New Brunswick';
  $states['CA']['NF'] = 'Newfoundland and Labrador';
  $states['CA']['NT'] = 'Northwest Territories';
  $states['CA']['NS'] = 'Nova Scotia';
  $states['CA']['NU'] = 'Nunavut';
  $states['CA']['ON'] = 'Ontario';
  $states['CA']['PE'] = 'Prince Edward Island';
  $states['CA']['QC'] = 'Quebec';
  $states['CA']['SK'] = 'Saskatchewan';
  $states['CA']['YT'] = 'Yukon Territory';
  return $states[$iso];
}

function chpprop_retrieve_state($iso, $abbrev) {
  $states = chpprop_retrieve_states($iso);
  return $states[$abbrev];
}

function chpprop_retrieve_state_abbrev($iso, $state) {
  $states = chpprop_retrieve_states($iso);
  foreach ($states as $abbrev => $value) {
    if (!strcmp($state, $value)) {
      return $abbrev;
    }
  }
  return 'Unknown';
}

function chpprop_default_province() {
  return 'ON';
}


/***
 * Functions used for random id's
 */
function chpprop_genRandomString16() {
  $length = 16;
  $characters = "0123456789abcdefghijklmnopqrstuvwxyz";
  $string = "";
  for ($p = 0; $p < $length; $p++) {
    $string .= $characters[mt_rand(0, strlen($characters) - 1)];
  }
  return $string;
}

function chpprop_genRandomString20() {
  $length = 20;
  $characters = "0123456789abcdefghijklmnopqrstuvwxyz";
  $string = "";
  for ($p = 0; $p < $length; $p++) {
    $string .= $characters[mt_rand(0, strlen($characters) - 1)];
  }
  return $string;
}

function chpprop_genRandomString32() {
  $length = 32;
  $characters = "0123456789abcdefghijklmnopqrstuvwxyz";
  $string = "";
  for ($p = 0; $p < $length; $p++) {
    $string .= $characters[mt_rand(0, strlen($characters) - 1)];
  }
  return $string;
}

function chpprop_genRandomString40() {
  $length = 40;
  $characters = "0123456789abcdefghijklmnopqrstuvwxyz";
  $string = "";
  for ($p = 0; $p < $length; $p++) {
    $string .= $characters[mt_rand(0, strlen($characters) - 1)];
  }
  return $string;
}

function chpprop_genRandomString64() {
  $length = 64;
  $characters = "0123456789abcdefghijklmnopqrstuvwxyz";
  $string = "";
  for ($p = 0; $p < $length; $p++) {
    $string .= $characters[mt_rand(0, strlen($characters) - 1)];
  }
  return $string;
}



/***
 * Functions used for uniform layout
 */
function chpprop_name_view_table($admins, $company_id = 'ANY') {
  $header = array();
  $row = array();
  $nameopt = array();
  if ($company_id !== 'ANY') {
    $nameopt = chdbadmin_get_nameopt($company_id);
  }
  if (empty($nameopt)) {
    $nameopt = chdbadmin_get_nameopt();
  } //anai_dbg($nameopt, '$nameopt');
  $prenames = array();
  if ($company_id !== 'ANY') {
    foreach (chdbadmin_get_prenameopt($company_id) as $opt) {
      $prenames[$opt['PrenameId']] = decode_entities($opt['Alias']);
    }
  }
  if (empty($prenames)) {
    foreach (chdbadmin_get_prenameopt() as $opt) {
      $prenames[$opt['PrenameId']] = decode_entities($opt['Alias']);
    }
  }
  $sufnames = array();
  if ($company_id !== 'ANY') {
    foreach (chdbadmin_get_sufnameopt($company_id) as $opt) {
      $sufnames[$opt['SufnameId']] = decode_entities($opt['Alias']);
    }
  }
  if (empty($sufnames)) {
    foreach (chdbadmin_get_sufnameopt() as $opt) {
      $sufnames[$opt['SufnameId']] = decode_entities($opt['Alias']);
    }
  }
  foreach ($admins as $admin) {
    if (!empty($nameopt['Prename']) and $nameopt['Prename']) {
      array_push($header, t('Prefix'));
    }
    if (!empty($nameopt['FirstName']) and $nameopt['FirstName']) {
      array_push($header, t('First name'));
    }
    if (!empty($nameopt['MiddleName']) and $nameopt['MiddleName']) {
      array_push($header, t('Middle name'));
    }
    if (!empty($nameopt['LastName']) and $nameopt['LastName']) {
      array_push($header, t('Last name'));
    }
    if (!empty($nameopt['Sufname']) and $nameopt['Sufname']) {
      array_push($header, t('Suffix'));
    }
    array_push($header, t('Street'));
    array_push($header, t('City, Province'));
    array_push($header, t('Postal'));
    array_push($header, t('Country'));
    array_push($header, t('Phone'));
    break;
  }
  $row = array();
  foreach ($admins as $admin) {
    $r = array();
    if (!empty($nameopt['Prename']) and $nameopt['Prename']) {
      array_push($r, $prenames[$admin['PrenameId']]);
    }
    if (!empty($nameopt['FirstName']) and $nameopt['FirstName']) {
      array_push($r, decode_entities($admin['FirstName']));
    }
    if (!empty($nameopt['MiddleName']) and $nameopt['MiddleName']) {
      array_push($r, decode_entities($admin['MiddleName']));
    }
    if (!empty($nameopt['LastName']) and $nameopt['LastName']) {
      array_push($r, decode_entities($admin['LastName']));
    }
    if (!empty($nameopt['Sufname']) and $nameopt['Sufname']) {
      array_push($r, $sufnames[$admin['SufnameId']]);
    }
    array_push($r, decode_entities($admin['Street']));
    array_push($r, decode_entities($admin['City']).', '.decode_entities($admin['Province']));
    array_push($r, decode_entities($admin['PostalCode']));
    array_push($r, decode_entities($admin['Country']));
    array_push($r, decode_entities($admin['Phone']));
    $row[] = $r;
  }

  if (empty($row)) {
    return t('Empty.');
  } else {
    return theme('table', $header, $row);
  }
}



/***
 * Functions used for property selection without AHAH
 */

function chpprop_select_property_form($form_state) {
  //$form['chp'] = array('#type' => 'fieldset');

  global $user;
  $properties = chpprop_retrieve_properties($user->uid);

  if (sizeof($properties) == 0) {
    $form['empty'] = array('#value' => '<p>'.t('Empty. Missing properties.').'<p>');
    $form['back'] =
      array('#type' => 'submit',
	    '#value' => t('Back'),
	    '#validate' => array('chpprop_select_property_form_validate_back'),
	    '#submit' => array('chpprop_select_property_form_submit'));
    return $form;
  }

  $options = array();
  foreach ($properties as $property_id => $property) {
    $options[$property_id] = $property['data']['Alias'];
  }

  $form['PropertyId'] =
    array('#type' => 'select',
	  '#title' => t('Select property'),
	  '#options' => $options);

  $form['continue'] =
    array('#type' => 'submit',
	  '#value' => t('Continue'),
	  '#validate' => array('chpprop_select_property_form_validate_continue'),
	  '#submit' => array('chpprop_select_property_form_submit'));
  $form['back'] =
    array('#type' => 'submit',
	  '#value' => t('Back'),
	  '#validate' => array('chpprop_select_property_form_validate_back'),
	  '#submit' => array('chpprop_select_property_form_submit'));
  return $form;
}

function chpprop_select_property_form_validate_back($form, &$form_state) {
  $form_state['storage']['property_back'] = TRUE;
}

function chpprop_select_property_form_validate_continue($form, &$form_state) {
  $form_state['storage']['property_save'] = TRUE;
}

function chpprop_select_property_form_submit($form, &$form_state) {
  if ($form_state['storage']['property_back']) {
    unset($form_state['storage']);
    $form_state['redirect'] = 'front_page';
    return;
  }
  if ($form_state['storage']['property_save']) {
    $form_state['storage']['PropertyId'] = $form_state['values']['PropertyId'];
    unset($form_state['storage']['property_save']);
    return;
  }
}



/***
 * Functions used for lot selection without AHAH
 */

function chpprop_select_lot_form($form_state) {
  //$form['chp'] = array('#type' => 'fieldset');

  if (!isset($form_state['storage']['PropertyId'])) {
    $form['empty'] =
      array('#value' => '<p>'.t('Property must be selected first.').'<p>');
    return $form;
  }

  $property_id = $form_state['storage']['PropertyId'];
  $policies = chdbprop_get_all_policies($property_id);

  $options = array();
  foreach ($policies as $policy) {
    $options[$policy['LotId']] = $policy['Alias'];
  }

  $form['LotId'] =
    array('#type' => 'select',
	  '#title' => t('Select lot'),
	  '#options' => $options);

  $form['continue'] =
    array('#type' => 'submit',
	  '#value' => t('Continue'),
	  '#validate' => array('chpprop_select_lot_form_validate_continue'),
	  '#submit' => array('chpprop_select_lot_form_submit'));
  $form['back'] =
    array('#type' => 'submit',
	  '#value' => t('Back'),
	  '#validate' => array('chpprop_select_lot_form_validate_back'),
	  '#submit' => array('chpprop_select_lot_form_submit'));
  return $form;
}

function chpprop_select_lot_form_validate_back($form, &$form_state) {
  $form_state['storage']['lot_back'] = TRUE;
}

function chpprop_select_lot_form_validate_continue($form, &$form_state) {
  $form_state['storage']['lot_save'] = TRUE;
}

function chpprop_select_lot_form_submit($form, &$form_state) {
  if ($form_state['storage']['lot_back']) {
    unset($form_state['storage']);
    $form_state['redirect'] = 'front_page';
    return;
  }
  if ($form_state['storage']['lot_save']) {
    $form_state['storage']['LotId'] = $form_state['values']['LotId'];
    unset($form_state['storage']['lot_save']);
    return;
  }
}



/***
 * Functions used to provide standard elements of a page
 */

/**
 * $settings = array();
 * $settings['tag'] = 'property';
 * $settings['frame'] = TRUE;
 * $settings['show_companies'] = TRUE;
 * $settings['show_clients'] = TRUE;
 * $settings['show_regions'] = TRUE;
 * $settings['show_properties'] = TRUE;
 * $settings['show_lots'] = TRUE;
 * $settings['show_all_properties'] = FALSE; // Default value
 * $settings['show_all_lots'] = FALSE; // Default value
 * $settings['display_single_company'] = TRUE;
 * $settings['display_single_property'] = TRUE;
 * $settings['display_single_lot'] = TRUE;
 * $settings['include_company_any'] = TRUE;
 * $settings['include_region_any'] = TRUE;
 * $settings['include_client_any'] = TRUE;
 * $settings['include_property_none'] = TRUE;
 * $settings['include_sticky_management'] = TRUE;
 * $settings['policy_filter'] = array('ValetPolicy');
 * $settings['option']['skippropertynotes'] = TRUE;
 * $settings['buttons']['add']['title'] = 'Add new item';
 * $settings['buttons']['add']['prefix'] = '<p>';
 * $settings['buttons']['add']['suffix'] = '<p>';
 * $settings['buttons']['pad']['image'] = 'pad.png';
 * $settings['buttons']['add']['attributes'] = array('width' => '15%');
 * $settings['buttons']['add']['validate'] = 'NAME OF CALLBACK FUNCTION';
 * $settings['buttons']['add']['submit'] = 'NAME OF CALLBACK FUNCTION';
 * $settings['buttons']['back'] = ...
 * $settings['buttons']['home'] = ...
 * $settings['default']['CompanyId'] = $company_id;
 * $settings['default']['PropertyId'] = $property_id;
 * $settings['default']['LotId'] = $lot_id;
 * $settings['disabled']['CompanyId'] = TRUE;
 * $settings['disabled']['PropertyId'] = TRUE;
 * $settings['disabled']['LotId'] = TRUE;
 *
 * Returns:
 *   1 = Error, the caller should just return the form immediately
 *   0 = No errors
 *
 * $form modifications:
 *   According to given input
 *
 * $form_state modifications:
 *   $form_state['storage'][$tag]['Companies']
 *   $form_state['storage'][$tag]['Regions']
 *   $form_state['storage'][$tag]['Clients']
 *   $form_state['storage'][$tag]['Properties']
 *   $form_state['storage'][$tag]['Lots']
 *   $form_state['storage'][$tag]['CompanyChanged']
 *   $form_state['storage'][$tag]['RegionChanged']
 *   $form_state['storage'][$tag]['ClientChanged']
 *   $form_state['storage'][$tag]['PropertyChanged']
 *   $form_state['storage'][$tag]['LotChanged']
 */
function chpprop_produce_header(&$form, &$form_state, $settings = array()) {
  global $user;
  $tag = 'property';
  if (isset($settings['tag'])) {
    $tag = $settings['tag'];
  }

  // Top-container
  if (!isset($form[$tag])) {
    $form[$tag] =
      array('#prefix' => '<div id="'.$tag.'-wrapper">',
	    '#suffix' => '</div>',
	    '#tree' => TRUE);
    if ($settings['frame']) {
      $form[$tag]['#type'] = 'fieldset';
    }
  }

  // Top line button Layout
  if (isset($settings['buttons'])) {
    foreach ($settings['buttons'] as $button => $param) {
      if ($button === 'add') {
	$form[$tag][$button] =
	  array('#type' => 'image_button',
		'#src' => drupal_get_path('module', 'anai').'/add.png');
      } elseif ($button === 'back') {
	$form[$tag][$button] =
	  array('#type' => 'image_button',
		'#src' => drupal_get_path('module', 'anai').'/back.png');
      } elseif ($button === 'printer') {
	$form[$tag][$button] =
	  array('#type' => 'image_button',
		'#src' => drupal_get_path('module', 'anai').'/printer.png');
      } elseif ($button === 'options') {
	$form[$tag][$button] =
	  array('#type' => 'image_button',
		'#src' => drupal_get_path('module', 'anai').'/options.png');
      } elseif ($button === 'search') {
	$form[$tag][$button] =
	  array('#type' => 'image_button',
		'#src' => drupal_get_path('module', 'anai').'/search.png');
      } elseif (isset($param['image'])) {
	$form[$tag][$button] =
	  array('#type' => 'image_button',
		'#src' => $param['image']);
      } else {
	$form[$tag][$button] =
	  array('#type' => 'submit',
		'#value' => $param['title']);
      }
      if (isset($param['prefix'])) {
	$form[$tag][$button]['#prefix'] = $param['prefix'];
      }
      if (isset($param['suffix'])) {
	$form[$tag][$button]['#suffix'] = $param['suffix'];
      }
      if (isset($param['attributes'])) {
	$form[$tag][$button]['#attributes'] = $param['attributes'];
      }
      if (isset($param['validate'])) {
	$form[$tag][$button]['#validate'][] = $param['validate'];
      }
      if (isset($param['submit'])) {
	$form[$tag][$button]['#submit'][] = $param['submit'];
      }
    }
  }

  // Prepare company info based on conditionally retrieved properties.
  // All properties in a relation must have a company.
  if (!isset($form_state['storage'][$tag]['Companies'])) {
    $companies = chpprop_retrieve_companies($user->uid);
    $form_state['storage'][$tag]['Companies'] = $companies;
  } else {
    $companies = $form_state['storage'][$tag]['Companies'];
  }

  /***
   * A prerequisite to continue is to have at least one company.
   */
  if (empty($companies)) {
    //$form[$tag]['error'] = array('#value' => '<p>'.t('Company missing.').'<p>');
    return 0;
  }

  // Prepare regions.
  if (!isset($form_state['storage'][$tag]['Regions'])) {
    $regions = chpprop_retrieve_regions($user->uid);
    $form_state['storage'][$tag]['Regions'] = $regions;
  } else {
    $regions = $form_state['storage'][$tag]['Regions'];
  }

  // Prepare clients.
  if (!isset($form_state['storage'][$tag]['Clients'])) {
    $clients = chpprop_retrieve_clients($user->uid);
    $form_state['storage'][$tag]['Clients'] = $clients;
  } else {
    $clients = $form_state['storage'][$tag]['Clients'];
  }

  // Build company options
  $company_options = array();
  foreach ($companies as $company_id => $company) {
    $company_options[$company_id] =
      decode_entities($company['data']['Alias']).', '.
      decode_entities($company['data']['Street']).', '.
      decode_entities($company['data']['City']);
  }

  // The 'ANY' option
  if (isset($settings['include_company_any']) and $settings['include_company_any']) {
    $company_options['ANY'] = t('All companies');
  }

  // Set CompanyId
  if (!isset($form_state['storage'][$tag]['CompanyId'])) {
    if (isset($_SESSION['anai']['TemporaryCompanyId']) and
	in_array($_SESSION['anai']['TemporaryCompanyId'], array_keys($company_options))) {
      $company_id = $_SESSION['anai']['TemporaryCompanyId'];
    } elseif (isset($_SESSION['anai']['StickyCompanyId']) and
	      in_array($_SESSION['anai']['StickyCompanyId'], array_keys($company_options))) {
      $company_id = $_SESSION['anai']['StickyCompanyId'];
    } else {
      $company_id = key($company_options);
    }
    unset($_SESSION['anai']['TemporaryCompanyId']);
    if (isset($settings['default']['CompanyId']) and
	in_array($settings['default']['CompanyId'], array_keys($company_options))) {
      $company_id = $settings['default']['CompanyId'];
    }
    $form_state['storage'][$tag]['OldCompanyId'] = $company_id;
    $form_state['storage'][$tag]['CompanyId'] = $company_id;
  } else {
    $company_id = $form_state['storage'][$tag]['CompanyId'];
  }

  // Handle CompanyId change
  if ($company_id !== $form_state['storage'][$tag]['OldCompanyId']) {
    unset($form_state['storage'][$tag]['RegionId']);
    unset($form_state['storage'][$tag]['ClientId']);
    unset($form_state['storage'][$tag]['PropertyId']);
    unset($form_state['storage'][$tag]['all_properties']);
    unset($form_state['storage'][$tag]['LotId']);
    unset($form_state['storage'][$tag]['all_lots']);
    if ($settings['include_sticky_management']) {
      unset($_SESSION['anai']['StickyPropertyId']);
      unset($_SESSION['anai']['StickyLotId']);
    }
    $form_state['storage'][$tag]['CompanyChanged'] = TRUE;
  } else {
    $form_state['storage'][$tag]['CompanyChanged'] = FALSE;
  }
  $form_state['storage'][$tag]['OldCompanyId'] = $company_id;

  // Handle sticky CompanyId
  if ($settings['include_sticky_management'] and $company_id !== 'ANY') {
    $_SESSION['anai']['StickyCompanyId'] = $company_id;
  }
  $bypass_sticky_property = FALSE;
  $bypass_sticky_lot = FALSE;
  if (isset($_SESSION['anai']['StickyCompanyId']) and
      $company_id !== $_SESSION['anai']['StickyCompanyId'] and
      $company_id !== 'ANY') {
    $bypass_sticky_property = TRUE;
    $bypass_sticky_lot = TRUE;
  }

  // Build region options
  $region_options = array();
  if (isset($regions[$company_id])) {
    foreach ($regions[$company_id] as $region_id => $region) {
      $region_options[$region_id] =
	decode_entities($region['data']['Alias']).', '.
	decode_entities($region['data']['Street']).', '.
	decode_entities($region['data']['City']);
    }
  }

  // The 'ANY' option
  if (isset($settings['include_region_any']) and $settings['include_region_any']) {
    $region_options['ANY'] = t('All regions');
  }

  // Set RegionId
  if (!isset($form_state['storage'][$tag]['RegionId'])) {
    if (isset($_SESSION['anai']['TemporaryRegionId']) and
	in_array($_SESSION['anai']['TemporaryRegionId'], array_keys($region_options))) {
      $region_id = $_SESSION['anai']['TemporaryRegionId'];
    } elseif (isset($_SESSION['anai']['StickyRegionId']) and
	      in_array($_SESSION['anai']['StickyRegionId'], array_keys($region_options))) {
      $region_id = $_SESSION['anai']['StickyRegionId'];
    } elseif (empty($region_options) or $company_id === 'ANY') {
      $region_id = 'ANY';
    } else {
      $region_id = key($region_options);
    }
    unset($_SESSION['anai']['TemporaryRegionId']);
    if (isset($settings['default']['RegionId']) and
	in_array($settings['default']['RegionId'], array_keys($region_options))) {
      $region_id = $settings['default']['RegionId'];
    }
    $form_state['storage'][$tag]['OldRegionId'] = $region_id;
    $form_state['storage'][$tag]['RegionId'] = $region_id;
  } else {
    $region_id = $form_state['storage'][$tag]['RegionId'];
  }

  // Handle RegionId change
  if ($region_id !== $form_state['storage'][$tag]['OldRegionId']) {
    unset($form_state['storage'][$tag]['ClientId']);
    unset($form_state['storage'][$tag]['PropertyId']);
    unset($form_state['storage'][$tag]['all_properties']);
    unset($form_state['storage'][$tag]['LotId']);
    unset($form_state['storage'][$tag]['all_lots']);
    if ($settings['include_sticky_management']) {
      unset($_SESSION['anai']['StickyPropertyId']);
      unset($_SESSION['anai']['StickyLotId']);
    }
    $form_state['storage'][$tag]['RegionChanged'] = TRUE;
  } else {
    $form_state['storage'][$tag]['RegionChanged'] = FALSE;
  }
  $form_state['storage'][$tag]['OldRegionId'] = $region_id;

  // Handle sticky RegionId
  if ($settings['include_sticky_management'] and $region_id !== 'ANY') {
    $_SESSION['anai']['StickyRegionId'] = $region_id;
  }
  $bypass_sticky_property = FALSE;
  $bypass_sticky_lot = FALSE;
  if (isset($_SESSION['anai']['StickyRegionId']) and
      $region_id !== $_SESSION['anai']['StickyRegionId'] and
      $region_id !== 'ANY') {
    $bypass_sticky_property = TRUE;
    $bypass_sticky_lot = TRUE;
  }

  // Build client options
  $client_options = array();
  if (isset($clients[$company_id]) and $region_id === 'ANY') {
    foreach ($clients[$company_id] as $anyregion => $regionlist) {
      foreach ($regionlist as $client_id => $client) {
	$client_options[$client_id] =
	  decode_entities($client['data']['Alias']).', '.
	  decode_entities($client['data']['Street']).', '.
	  decode_entities($client['data']['City']);
      }
    }
  } else if (isset($clients[$company_id][$region_id])) {
    foreach ($clients[$company_id][$region_id] as $client_id => $client) {
      $client_options[$client_id] =
	decode_entities($client['data']['Alias']).', '.
	decode_entities($client['data']['Street']).', '.
	decode_entities($client['data']['City']);
    }
  }

  // The 'ANY' option
  if (isset($settings['include_client_any']) and $settings['include_client_any']) {
    $client_options['ANY'] = t('All clients');
  }

  // Set ClientId
  if (!isset($form_state['storage'][$tag]['ClientId'])) {
    if (isset($_SESSION['anai']['TemporaryClientId']) and
	in_array($_SESSION['anai']['TemporaryClientId'], array_keys($client_options))) {
      $client_id = $_SESSION['anai']['TemporaryClientId'];
    } elseif (isset($_SESSION['anai']['StickyClientId']) and
	      in_array($_SESSION['anai']['StickyClientId'], array_keys($client_options))) {
      $client_id = $_SESSION['anai']['StickyClientId'];
    } elseif (empty($client_options) or ($company_id === 'ANY' and $region_id === 'ANY')) {
      $client_id = 'ANY';
    } else {
      $client_id = key($client_options);
    }
    unset($_SESSION['anai']['TemporaryClientId']);
    if (isset($settings['default']['ClientId']) and count($client_options) > 0 and
	in_array($settings['default']['ClientId'], array_keys($client_options))) {
      $client_id = $settings['default']['ClientId'];
    }
    $form_state['storage'][$tag]['OldClientId'] = $client_id;
    $form_state['storage'][$tag]['ClientId'] = $client_id;
  } else {
    $client_id = $form_state['storage'][$tag]['ClientId'];
  }

  // Handle ClientId change
  if ($client_id !== $form_state['storage'][$tag]['OldClientId']) {
    unset($form_state['storage'][$tag]['PropertyId']);
    unset($form_state['storage'][$tag]['all_properties']);
    unset($form_state['storage'][$tag]['LotId']);
    unset($form_state['storage'][$tag]['all_lots']);
    if ($settings['include_sticky_management']) {
      unset($_SESSION['anai']['StickyPropertyId']);
      unset($_SESSION['anai']['StickyLotId']);
    }
    $form_state['storage'][$tag]['ClientChanged'] = TRUE;
  } else {
    $form_state['storage'][$tag]['ClientChanged'] = FALSE;
  }
  $form_state['storage'][$tag]['OldClientId'] = $client_id;

  // Handle sticky ClientId
  if ($settings['include_sticky_management'] and $company_id !== 'ANY' and $region_id !== 'ANY') {
    $_SESSION['anai']['StickyClientId'] = $client_id;
  }
  $bypass_sticky_property = FALSE;
  $bypass_sticky_lot = FALSE;
  if (isset($_SESSION['anai']['StickyClientId']) and
      $client_id !== $_SESSION['anai']['StickyClientId'] and
      $company_id !== 'ANY' and $region_id !== 'ANY') {
    $bypass_sticky_property = TRUE;
    $bypass_sticky_lot = TRUE;
  }

  // Prepare property info.
  $policy_filters = array();
  $equipment_filters = array();
  $enforcement_filters = array();
  if ($settings['policy_filter']) {
    $policy_filters = $settings['policy_filter'];
  }
  if ($settings['equipment_filter']) {
    $equipment_filters = $settings['equipment_filter'];
  }
  if ($settings['enforcement_filter']) {
    $enforcement_filters = $settings['enforcement_filter'];
  }
  $properties = chpprop_retrieve_properties($user->uid, $company_id, $policy_filters);
  $form_state['storage'][$tag]['Properties'] = $properties;

  /***
   * A prerequisite to continue is to have at least one property.
   */
  if (empty($properties)) {
    //$form[$tag]['error'] = array('#value' => '<p>'.t('Property missing.').'<p>');
    return 0;
  }

  // Build property options
  $property_options = array();
  if ($settings['include_property_none']) {
    $property_options['None'] = t('No preselected property');
  }
  foreach ($properties as $property_id => $property) {
    $property_options[$property_id] =
      decode_entities($property['data']['Alias']).', '.
      decode_entities($property['data']['Street']).', '.
      decode_entities($property['data']['City']);
  }

  // The 'ANY' option
  if (isset($settings['include_property_any']) and $settings['include_property_any']) {
    $property_options['ANY'] = t('All properties');
  }

  // Set PropertyId
  if (!isset($form_state['storage'][$tag]['PropertyId'])) {
    if (isset($_SESSION['anai']['TemporaryPropertyId']) and
	in_array($_SESSION['anai']['TemporaryPropertyId'], array_keys($property_options))) {
      $property_id = $_SESSION['anai']['TemporaryPropertyId'];
    } elseif (isset($_SESSION['anai']['StickyPropertyId']) and !$bypass_sticky_property and
	      in_array($_SESSION['anai']['StickyPropertyId'], array_keys($property_options))) {
      $property_id = $_SESSION['anai']['StickyPropertyId'];
    } else {
      $property_id = key($property_options);
    }
    unset($_SESSION['anai']['TemporaryPropertyId']);
    if (isset($settings['default']['PropertyId']) and
	in_array($settings['default']['PropertyId'], array_keys($property_options))) {
      $property_id = $settings['default']['PropertyId'];
    }
    $form_state['storage'][$tag]['OldPropertyId'] = $property_id;
    $form_state['storage'][$tag]['PropertyId'] = $property_id;
  } else {
    $property_id = $form_state['storage'][$tag]['PropertyId'];
  }
  if ($property_id !== $form_state['storage'][$tag]['OldPropertyId']) {
    unset($form_state['storage'][$tag]['LotId']);
    unset($form_state['storage'][$tag]['all_lots']);
    if ($settings['include_sticky_management']) {
      unset($_SESSION['anai']['StickyLotId']);
    }
    $form_state['storage'][$tag]['PropertyChanged'] = TRUE;
  } else {
    $form_state['storage'][$tag]['PropertyChanged'] = FALSE;
  }
  $form_state['storage'][$tag]['OldPropertyId'] = $property_id;
  if (!isset($form_state['storage'][$tag]['all_properties'])) {
    $form_state['storage'][$tag]['all_properties'] =
      isset($settings['show_all_properties']) ?
      $settings['show_all_properties'] : FALSE;
  }
  if ($settings['include_sticky_management']) {
    if ($property_id === 'None') {
      unset($_SESSION['anai']['StickyPropertyId']);
    } else {
      $_SESSION['anai']['StickyPropertyId'] = $property_id;
    }
  }
  if (isset($_SESSION['anai']['StickyPropertyId']) and
      $property_id !== $_SESSION['anai']['StickyPropertyId']) {
    $bypass_sticky_lot = TRUE;
  }

  // Prepare lot info.
  $policy_filters = array();
  $equipment_filters = array();
  $enforcement_filters = array();
  if ($settings['policy_filter']) {
    $policy_filters = $settings['policy_filter'];
  }
  if ($settings['equipment_filter']) {
    $equipment_filters = $settings['equipment_filter'];
  }
  if ($settings['enforcement_filter']) {
    $enforcement_filters = $settings['enforcement_filter'];
  }
  $lots = chpprop_retrieve_policies($user->uid, $company_id, $policy_filters, $enforcement_filters);
  $form_state['storage'][$tag]['Lots'] = $lots;

  /***
   * A prerequisite to continue is to have at least one lot.
   */
  if (empty($lots)) {
    //$form[$tag]['error'] = array('#value' => '<p>'.t('Lot missing.').'<p>');
    return 0;
  }

  // Building lot options
  $lot_options = array();
  if (isset($lots[$property_id])) {
    foreach ($lots[$property_id] as $policy) {
      $lot_options[$policy['data']['LotId']] = decode_entities($policy['data']['Alias']);
    }
  }

  // The 'ANY' option
  if (isset($settings['include_lot_any']) and $settings['include_lot_any']) {
    $lot_options['ANY'] = t('All lots');
  }

  // Set LotId
  if (!isset($form_state['storage'][$tag]['LotId'])) {
    if (isset($_SESSION['anai']['TemporaryLotId']) and
	in_array($_SESSION['anai']['TemporaryLotId'], array_keys($lot_options))) {
      $lot_id = $_SESSION['anai']['TemporaryLotId'];
    } elseif (isset($_SESSION['anai']['StickyLotId']) and !$bypass_sticky_lot and
	      in_array($_SESSION['anai']['StickyLotId'], array_keys($lot_options))) {
      $lot_id = $_SESSION['anai']['StickyLotId'];
    } else {
      $lot_id = key($lot_options);
    }
    unset($_SESSION['anai']['TemporaryLotId']);
    if (isset($settings['default']['LotId']) and
	in_array($settings['default']['LotId'], array_keys($lot_options))) {
      $lot_id = $settings['default']['LotId'];
    }
    $form_state['storage'][$tag]['LotId'] = $lot_id;
    $form_state['storage'][$tag]['OldLotId'] = $lot_id;
  } else {
    $lot_id = $form_state['storage'][$tag]['LotId'];
  }
  if ($lot_id !== $form_state['storage'][$tag]['OldLotId']) {
    $form_state['storage'][$tag]['LotChanged'] = TRUE;
  } else {
    $form_state['storage'][$tag]['LotChanged'] = FALSE;
  }
  $form_state['storage'][$tag]['OldLotId'] = $lot_id;

  if (!isset($form_state['storage'][$tag]['all_lots'])) {
    $form_state['storage'][$tag]['all_lots'] =
      isset($settings['show_all_lots']) ? $settings['show_all_lots'] : FALSE;
  }
  if ($settings['include_sticky_management']) {
    $_SESSION['anai']['StickyLotId'] = $lot_id;
  }

  // Layout companies
  $form[$tag]['OldCompanyId'] =
    array('#type' => 'hidden',
	  '#value' => $form_state['storage'][$tag]['OldCompanyId']);
  if ((isset($settings['show_companies']) and $settings['show_companies']) and
      ((isset($settings['display_single_company'])
	and $settings['display_single_company'] and
	1 == count($company_options)) or
       count($company_options) > 1)) {
    $form[$tag]['CompanyId'] =
      array('#type' => 'select',
	    '#title' => t('Company'),
	    '#options' => $company_options,
	    '#disabled' => $settings['disabled']['CompanyId'],
	    '#default_value' => $company_id,
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array($tag)),
			     'wrapper' => $tag.'-wrapper'));
  } else {
    $form[$tag]['CompanyId'] =
      array('#type' => 'hidden',
	    '#value' => $company_id);
  }

  // Layout regions
  $form[$tag]['OldRegionId'] =
    array('#type' => 'hidden',
	  '#value' => $form_state['storage'][$tag]['OldRegionId']);
  if ((isset($settings['show_regions']) and $settings['show_regions']) and
      ((isset($settings['display_single_region']) and
	$settings['display_single_region'] and
	1 == count($region_options)) or
       count($region_options) > 1)) {
    $form[$tag]['RegionId'] =
      array('#type' => 'select',
	    '#title' => t('Region'),
	    '#options' => $region_options,
	    '#disabled' => $settings['disabled']['RegionId'],
	    '#default_value' => $region_id,
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array($tag)),
			     'wrapper' => $tag.'-wrapper'));
  } else {
    $form[$tag]['RegionId'] =
      array('#type' => 'hidden',
	    '#value' => $region_id);
  }

  // Layout clients
  $form[$tag]['OldClientId'] =
    array('#type' => 'hidden',
	  '#value' => $form_state['storage'][$tag]['OldClientId']);
  if ((isset($settings['show_clients']) and $settings['show_clients']) and
      ((isset($settings['display_single_client']) and 
	$settings['display_single_client'] and
	1 == count($client_options)) or
       count($client_options) > 1)) {
    $form[$tag]['ClientId'] =
      array('#type' => 'select',
	    '#title' => t('Client'),
	    '#options' => $client_options,
	    '#disabled' => $settings['disabled']['ClientId'],
	    '#default_value' => $client_id,
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array($tag)),
			     'wrapper' => $tag.'-wrapper'));
  } else {
    $form[$tag]['ClientId'] =
      array('#type' => 'hidden',
	    '#value' => $client_id);
  }

  // Layout properties
  $form[$tag]['OldPropertyId'] =
    array('#type' => 'hidden',
	  '#value' => $form_state['storage'][$tag]['OldPropertyId']);
  if ((isset($settings['show_properties']) and $settings['show_properties']) and
      ((isset($settings['display_single_property']) and
	$settings['display_single_property'] and
	1 == count($property_options)) or
       count($property_options) > 1)) {
    $form[$tag]['PropertyId'] =
      array('#type' => 'select',
	    '#options' => $property_options,
	    '#title' => t('Property'),
	    '#disabled' => $settings['disabled']['PropertyId'],
	    '#default_value' => $property_id,
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array($tag)),
			     'wrapper' => $tag.'-wrapper'));

    if (isset($settings['show_all_properties'])) {
      $form[$tag]['all_properties'] =
	array('#type' => 'checkbox',
	      '#title' => t('Include all properties'),
	      '#disabled' => $settings['disabled']['PropertyId'],
	      '#default_value' => $form_state['storage'][$tag]['all_properties'],
	      '#ahah' => array('event' => 'change',
			       'path' => ahah_helper_path(array($tag)),
			       'wrapper' => $tag.'-wrapper'));
    }
  } else {
    $form[$tag]['PropertyId'] =
      array('#type' => 'hidden',
	    '#value' => $property_id);
  }

  $form[$tag]['OldLotId'] =
    array('#type' => 'hidden',
	  '#value' => $form_state['storage'][$tag]['OldLotId']);
  //if ((isset($settings['show_properties']) and $settings['show_properties'] and $property_id !== 'None') and
  if ($property_id !== 'None' and
      (isset($settings['show_lots']) and $settings['show_lots']) and
      ((isset($settings['display_single_lot']) and $settings['display_single_lot'] and
	1 == count($lot_options)) or
       count($lot_options) > 1)) {
    $form[$tag]['LotId'] =
      array('#type' => 'select',
	    '#options' => $lot_options,
	    '#title' => t('Lot'),
	    '#disabled' => $settings['disabled']['LotId'],
	    '#default_value' => $lot_id,
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array($tag)),
			     'wrapper' => $tag.'-wrapper'));

    if (isset($settings['show_all_lots']) and count($lot_options) > 1) {
      $form[$tag]['all_lots'] =
	array('#type' => 'checkbox',
	      '#title' => t('Include all lots on property'),
	      '#disabled' => $settings['disabled']['LotId'],
	      '#default_value' => $form_state['storage'][$tag]['all_lots'],
	      '#ahah' => array('event' => 'change',
			       'path' => ahah_helper_path(array($tag)),
			       'wrapper' => $tag.'-wrapper'));
    }
  } else {
    $form[$tag]['LotId'] =
      array('#type' => 'hidden',
	    '#value' => $lot_id);
  }

  // Layout property notes
  if (isset($settings['show_properties']) and $settings['show_properties'] and
      (!isset($settings['option']['skippropertynotes']) or
       (isset($settings['option']['skippropertynotes']) and !$settings['option']['skippropertynotes']))) {
    $header = array(t('Created'), t('By'), t('Id'), t('Note'));
    $row = array();
    $notes = array();
    foreach (chdbprop_get_propertynotes($property_id) as $note) {
      if (!$note['Fragment']) {
	$notes[$note['NoteId']]['master'] = $note;
	$notes[$note['NoteId']]['fragments'] = array();
      } else {
	$notes[$note['NoteId']]['fragments'][] = $note;
      }
    }
    if (!empty($notes)) {
      foreach ($notes as $note_id => $entry) {
	$created = chpuser_datetime_utc_to_usertimezone($entry['master']['CreateTime']);
	$account = user_load(array('uid' => $entry['master']['CreatorUID']));
	$numfrags = count($entry['fragments']);
	$row[] = array(array('data' => $created->format("M j,y H:i"),
			     'rowspan' => 1 + $numfrags),
		       array('data' => $account->name,
			     'rowspan' => 1 + $numfrags),
		       array('data' => $entry['master']['NoteId'],
			     'rowspan' => 1 + $numfrags),
		       nl2br(decode_entities($entry['master']['Note'])));
	foreach ($entry['fragments'] as $fragment) {
	  $created = chpuser_datetime_utc_to_usertimezone($fragment['CreateTime']);
	  $account = user_load(array('uid' => $fragment['CreatorUID']));
	  $row[] = array('<b>Added '.$created->format("M j,y H:i").' by '.$account->name.':</b><br>'.
			 nl2br(decode_entities($fragment['Note'])));
	}
      }
    }

    if (!empty($notes)) {
      $form[$tag]['PropNotes'] = array('#type' => 'fieldset', '#title' => t('Property notes'));
      $form[$tag]['PropNotes']['list'] = array('#value' => theme('table', $header, $row));
    }
  }

  $form[$tag]['set'] =
    array('#type' => 'submit',
	  '#value' => t('Set'),
	  '#submit' => array('ahah_helper_generic_submit'),
	  '#attributes' => array('class' => 'no-js'));

  $_SESSION['anai']['TemporaryCompanyId'] = $company_id;
  $_SESSION['anai']['TemporaryRegionId'] = $region_id;
  $_SESSION['anai']['TemporaryClientId'] = $client_id;
  $_SESSION['anai']['TemporaryPropertyId'] = $property_id;
  $_SESSION['anai']['TemporaryLotId'] = $lot_id;
  return 0;
}



/*
 * $settings = array();
 * $settings['tag'] = 'property';
 * $settings['subtag'] = 'vehicle';
 * $settings['frame'] = TRUE;
 * $settings['frametitle'] = t('The title of the frame');
 * $settings['subframe'] = TRUE;
 * $settings['subframetitle'] = t('The title of the subframe');
 * $settings['subframecollapsible'] = TRUE;
 * $settings['subframecollapsed'] = TRUE;
 * $settings['option']['wizard'] = 3;
 * $settings['default']['LPN'] = $lpn;
 * $settings['default']['Make'] = $make;
 * $settings['default']['Model'] = $model;
 * $settings['default']['Color'] = $color;
 * $settings['disabled']['LPN'] = TRUE;
 * $settings['disabled']['Make'] = TRUE;
 * $settings['disabled']['Model'] = TRUE;
 * $settings['disabled']['Color'] = TRUE;
 * $settings['ajaxevent']['lpn'] = TRUE;
 * $settings['ajaxevent']['make'] = TRUE;
 * $settings['ajaxevent']['model'] = TRUE;
 * $settings['ajaxevent']['color'] = TRUE;
 *
 * Returns:
 *   1 = Error, the caller should just return the form immediately
 *   0 = No errors
 *   $form_state['storage'][$tag]['LPNChanged']
 *   $form_state['storage'][$tag]['MakeChanged']
 *   $form_state['storage'][$tag]['ModelChanged']
 *   $form_state['storage'][$tag]['ColorChanged']
 */
function chpprop_produce_vehicle(&$form, &$form_state, $settings = array()) {
  global $user;
  $tag = 'property';
  if (isset($settings['tag'])) {
    $tag = $settings['tag'];
  }
  $subtag = 'vehicle';
  if (isset($settings['subtag'])) {
    $subtag = $settings['subtag'];
  }

  // Layout
  if (!isset($form[$tag])) {
    $form[$tag] =
      array('#prefix' => '<div id="'.$tag.'-wrapper">',
	    '#suffix' => '</div>',
	    '#tree' => TRUE);
    if ($settings['frame']) {
      $form[$tag]['#type'] = 'fieldset';
      if (isset($settings['frametitle']) and $settings['frametitle']) {
	$form[$tag]['#title'] = $settings['frametitle'];
      }
    }
  }

  if (isset($settings['subframe']) and $settings['subframe']) {
    $form[$tag][$subtag] = array('#type' => 'fieldset');
    if (isset($settings['subframetitle'])) {
      $form[$tag][$subtag]['#title'] = $settings['subframetitle'];
    }
    if (isset($settings['subframecollapsible']) and $settings['subframecollapsible']) {
      $form[$tag][$subtag]['#collapsible'] = $settings['subframecollapsible'];
    }
    if (isset($settings['subframecollapsed']) and $settings['subframecollapsed']) {
      $form[$tag][$subtag]['#collapsed'] = $settings['subframecollapsed'];
    } elseif (isset($settings['option']['wizard'])) {
      $form[$tag][$subtag]['#collapsed'] = $form_state['storage'][$tag]['Wizard'] !==
	$settings['option']['wizard'];
    }
  }

  // Build makes select
  if (!isset($form_state['storage'][$tag]['Makes'])) {
    $makes = array();
    foreach (chdbadmin_get_all_makes('Make') as $make) {
      $makes[$make['Make']] = $make['Make'];
    }
    $form_state['storage'][$tag]['Makes'] = $makes;
    if (sizeof($makes) == 0) {
      if (isset($settings['subtag'])) {
	$form[$tag][$subtag]['emptymakes'] =
	  array('#value' => t('System is missing Make definitions.'));
      } else {
	$form[$tag]['emptymakes'] =
	  array('#value' => t('System is missing Make definitions.'));
      }
      return 1;
    }
  } else {
    $makes = $form_state['storage'][$tag]['Makes'];
  }

  // Layout LPN
  if (isset($settings['subtag'])) {
    if (!isset($form_state['storage'][$tag][$subtag]['LPN'])) {
      if (isset($settings['default']['LPN'])) {
	$form_state['storage'][$tag][$subtag]['LPN'] = $settings['default']['LPN'];
      } else {
	$form_state['storage'][$tag][$subtag]['LPN'] = '';
      }
      $form_state['storage'][$tag][$subtag]['OldLPN'] = $form_state['storage'][$tag][$subtag]['LPN'];
    } elseif (0 < strlen($form_state['storage'][$tag][$subtag]['LPN'])) {
      $form_state['storage'][$tag][$subtag]['LPN'] =
	strtoupper($form_state['storage'][$tag][$subtag]['LPN']);
    }
    if ($form_state['storage'][$tag][$subtag]['LPN'] !== $form_state['storage'][$tag][$subtag]['OldLPN']) {
      $form_state['storage'][$tag]['LPNChanged'] = TRUE;
    }
    $form[$tag][$subtag]['OldLPN'] =
      array('#type' => 'hidden',
	    '#value' => $form_state['storage'][$tag][$subtag]['LPN']);
  } else {
    if (!isset($form_state['storage'][$tag]['LPN'])) {
      if (isset($settings['default']['LPN'])) {
	$form_state['storage'][$tag]['LPN'] = $settings['default']['LPN'];
      } else {
	$form_state['storage'][$tag]['LPN'] = '';
      }
      $form_state['storage'][$tag]['OldLPN'] = $form_state['storage'][$tag]['LPN'];
    } elseif (0 < strlen($form_state['storage'][$tag]['LPN'])) {
      $form_state['storage'][$tag]['LPN'] =
	strtoupper($form_state['storage'][$tag]['LPN']);
    }
    if ($form_state['storage'][$tag]['LPN'] !== $form_state['storage'][$tag]['OldLPN']) {
      $form_state['storage'][$tag]['LPNChanged'] = TRUE;
    }
    $form[$tag]['OldLPN'] =
      array('#type' => 'hidden',
	    '#value' => $form_state['storage'][$tag]['LPN']);
  }
  if ($settings['disabled']['LPN']) {
    if (isset($settings['subtag'])) {
      $form[$tag][$subtag]['LPN'] =
	array('#type' => 'hidden',
	      '#value' => $form_state['storage'][$tag][$subtag]['LPN']);
      $form[$tag][$subtag]['StaticLPN'] =
	array('#type' => 'textfield',
	      '#title' => t('License Plate Number'),
	      '#disabled' => TRUE,
	      '#default_value' => $form_state['storage'][$tag][$subtag]['LPN']);
    } else {
      $form[$tag]['LPN'] =
	array('#type' => 'hidden',
	      '#value' => $form_state['storage'][$tag]['LPN']);
      $form[$tag]['StaticLPN'] =
	array('#type' => 'textfield',
	      '#title' => t('License Plate Number'),
	      '#disabled' => TRUE,
	      '#default_value' => $form_state['storage'][$tag]['LPN']);
    }
  } else {
    if (isset($settings['subtag'])) {
      $form[$tag][$subtag]['LPN'] =
	array('#type' => 'textfield',
	      '#title' => t('License Plate Number'),
	      '#maxlength' => 10,
	      '#default_value' => $form_state['storage'][$tag][$subtag]['LPN'],
	      '#description' => '(No spaces)',
	      '#attributes' => array('onBlur'=>'this.value=this.value.toUpperCase()'));
      if (isset($settings['ajaxevent']['lpn']) and $settings['ajaxevent']['lpn']) {
	$form[$tag][$subtag]['LPN']['#ahah'] =
	  array('event' => 'change', //'blur',
		'path' => ahah_helper_path(array($tag)),
		'wrapper' => $tag.'-wrapper');
      }
    } else {
      $form[$tag]['LPN'] =
	array('#type' => 'textfield',
	      '#title' => t('License Plate Number'),
	      '#maxlength' => 10,
	      '#default_value' => $form_state['storage'][$tag]['LPN'],
	      '#description' => '(No spaces)',
	      '#attributes' => array('onBlur'=>'this.value=this.value.toUpperCase()'));
      if (isset($settings['ajaxevent']['lpn']) and $settings['ajaxevent']['lpn']) {
	$form[$tag]['LPN']['#ahah'] =
	  array('event' => 'change', //'blur',
		'path' => ahah_helper_path(array($tag)),
		'wrapper' => $tag.'-wrapper');
      }
    }
  }

  if (!isset($settings['disabled'])) {
    if (isset($settings['subtag'])) {
      $form[$tag][$subtag]['updatelpnbtn'] =
	array('#type' => 'button',
	      '#value' => t('Search vehicle database'));
    } else {
      $form[$tag]['updatelpnbtn'] =
	array('#type' => 'button',
	      '#value' => t('Search vehicle database'));
    }
  }

  $form_state['storage'][$tag]['freq_user_found'] = FALSE;

  if (strpos($form_state['clicked_button']['#id'], 'updatelpnbtn') !== FALSE) {
    if (isset($settings['subtag'])) {
      if (!empty($form_state['storage'][$tag][$subtag]['LPN'])) {
	$freq_user = chdbprop_get_freq_user($form_state['storage'][$tag][$subtag]['LPN']);
	if (sizeof($freq_user) > 0) {
	  $form_state['storage'][$tag]['freq_user_found'] = TRUE;
	  $form_state['storage'][$tag][$subtag]['Make'] = $freq_user['Make'];
	  $form_state['storage'][$tag][$subtag]['Model'] = $freq_user['Model'];
	  $form_state['storage'][$tag][$subtag]['Color'] = $freq_user['Color'];
	} else {
	  $form[$tag][$subtag]['LPN']['#field_prefix'] =
	    t('Not found in vehicle database, fill in details.');
	}
      }
    } else {
      if (!empty($form_state['storage'][$tag]['LPN'])) {
	$freq_user = chdbprop_get_freq_user($form_state['storage'][$tag]['LPN']);
	if (sizeof($freq_user) > 0) {
	  $form_state['storage'][$tag]['freq_user_found'] = TRUE;
	  $form_state['storage'][$tag]['Make'] = $freq_user['Make'];
	  $form_state['storage'][$tag]['Model'] = $freq_user['Model'];
	  $form_state['storage'][$tag]['Color'] = $freq_user['Color'];
	} else {
	  $form[$tag]['LPN']['#field_prefix'] = t('Not found in vehicle database, fill in details.');
	}
      }
    }
  }

  if (isset($settings['subtag'])) {
    if (isset($form_state['storage'][$tag][$subtag]['LPN']) and 
	!empty($form_state['storage'][$tag][$subtag]['LPN'])) {
      $form[$tag][$subtag]['LPN']['#field_suffix'] =
	theme('image',
	      $form_state['storage'][$tag]['freq_user_found'] ? 'misc/watchdog-ok.png' : '');
    }
  } else {
    if (isset($form_state['storage'][$tag]['LPN']) and 
	!empty($form_state['storage'][$tag]['LPN'])) {
      $form[$tag]['LPN']['#field_suffix'] =
	theme('image',
	      $form_state['storage'][$tag]['freq_user_found'] ? 'misc/watchdog-ok.png' : '');
    }
  }

  // Layout make
  if (isset($settings['subtag'])) {
    if (!isset($form_state['storage'][$tag][$subtag]['Make'])) {
      if (isset($settings['default']['Make'])) {
	$form_state['storage'][$tag][$subtag]['Make'] = $settings['default']['Make'];
      } else {
	$form_state['storage'][$tag][$subtag]['Make'] = key($makes);
      }
      $form_state['storage'][$tag][$subtag]['OldMake'] = $form_state['storage'][$tag][$subtag]['Make'];
    }
  } else {
    if (!isset($form_state['storage'][$tag]['Make'])) {
      if (isset($settings['default']['Make'])) {
	$form_state['storage'][$tag]['Make'] = $settings['default']['Make'];
      } else {
	$form_state['storage'][$tag]['Make'] = key($makes);
      }
      $form_state['storage'][$tag]['OldMake'] = $form_state['storage'][$tag]['Make'];
    }
  }
  if (isset($settings['subtag'])) {
    if ($form_state['storage'][$tag][$subtag]['Make'] !== $form_state['storage'][$tag][$subtag]['OldMake']) {
      $form_state['storage'][$tag]['ModelChanged'] = TRUE;
    }
    $form[$tag][$subtag]['OldMake'] =
      array('#type' => 'hidden',
	    '#value' => $form_state['storage'][$tag][$subtag]['Make']);
  } else {
    if ($form_state['storage'][$tag]['Make'] !== $form_state['storage'][$tag]['OldMake']) {
      $form_state['storage'][$tag]['ModelChanged'] = TRUE;
    }
    $form[$tag]['OldMake'] =
      array('#type' => 'hidden',
	    '#value' => $form_state['storage'][$tag]['Make']);
  }
  if ($settings['disabled']['Make']) {
    if (isset($settings['subtag'])) {
      $form[$tag][$subtag]['Make'] =
	array('#type' => 'hidden',
	      '#value' => $form_state['storage'][$tag][$subtag]['Make']);
      $form[$tag][$subtag]['StaticMake'] =
	array('#type' => 'select',
	      '#title' => t('Make'),
	      '#options' => $makes,
	      '#disabled' => TRUE,
	      '#default_value' => $form_state['storage'][$tag][$subtag]['Make']);
    } else {
      $form[$tag]['Make'] =
	array('#type' => 'hidden',
	      '#value' => $form_state['storage'][$tag]['Make']);
      $form[$tag]['StaticMake'] =
	array('#type' => 'select',
	      '#title' => t('Make'),
	      '#options' => $makes,
	      '#disabled' => TRUE,
	      '#default_value' => $form_state['storage'][$tag]['Make']);
    }
  } else {
    if (isset($settings['subtag'])) {
      $form[$tag][$subtag]['Make'] =
	array('#type' => 'select',
	      '#title' => t('Make'),
	      '#options' => $makes,
	      '#default_value' => $form_state['storage'][$tag][$subtag]['Make']);
      if (isset($settings['ajaxevent']['make']) and $settings['ajaxevent']['make']) {
	$form[$tag][$subtag]['Make']['#ahah'] =
	  array('event' => 'change',
		'path' => ahah_helper_path(array($tag)),
		'wrapper' => $tag.'-wrapper');
      }
    } else {
      $form[$tag]['Make'] =
	array('#type' => 'select',
	      '#title' => t('Make'),
	      '#options' => $makes,
	      '#default_value' => $form_state['storage'][$tag]['Make']);
      if (isset($settings['ajaxevent']['make']) and $settings['ajaxevent']['make']) {
	$form[$tag]['Make']['#ahah'] =
	  array('event' => 'change',
		'path' => ahah_helper_path(array($tag)),
		'wrapper' => $tag.'-wrapper');
      }
    }
  }

  // Layout model
  if (isset($settings['subtag'])) {
    if (!isset($form_state['storage'][$tag][$subtag]['Model'])) {
      if (isset($settings['default']['Model'])) {
	$form_state['storage'][$tag][$subtag]['Model'] = $settings['default']['Model'];
      } else {
	$form_state['storage'][$tag][$subtag]['Model'] = '';
      }
      $form_state['storage'][$tag][$subtag]['OldModel'] = $form_state['storage'][$tag][$subtag]['Model'];
    }
  } else {
    if (!isset($form_state['storage'][$tag]['Model'])) {
      if (isset($settings['default']['Model'])) {
	$form_state['storage'][$tag]['Model'] = $settings['default']['Model'];
      } else {
	$form_state['storage'][$tag]['Model'] = '';
      }
      $form_state['storage'][$tag]['OldModel'] = $form_state['storage'][$tag]['Model'];
    }
  }
  if (isset($settings['subtag'])) {
    if ($form_state['storage'][$tag][$subtag]['Model'] !== $form_state['storage'][$tag][$subtag]['OldModel']) {
      $form_state['storage'][$tag]['ModelChanged'] = TRUE;
    }
    $form[$tag][$subtag]['OldModel'] =
      array('#type' => 'hidden',
	    '#value' => $form_state['storage'][$tag][$subtag]['Model']);
  } else {
    if ($form_state['storage'][$tag]['Model'] !== $form_state['storage'][$tag]['OldModel']) {
      $form_state['storage'][$tag]['ModelChanged'] = TRUE;
    }
    $form[$tag]['OldModel'] =
      array('#type' => 'hidden',
	    '#value' => $form_state['storage'][$tag]['Model']);
  }
  if ($settings['disabled']['Model']) {
    if (isset($settings['subtag'])) {
      $form[$tag][$subtag]['Model'] =
	array('#type' => 'hidden',
	      '#value' => $form_state['storage'][$tag][$subtag]['Model']);
      $form[$tag][$subtag]['StaticModel'] =
	array('#type' => 'textfield',
	      '#title' => t('Model'),
	      '#disabled' => TRUE,
	      '#default_value' => $form_state['storage'][$tag][$subtag]['Model']);
    } else {
      $form[$tag]['Model'] =
	array('#type' => 'hidden',
	      '#value' => $form_state['storage'][$tag]['Model']);
      $form[$tag]['StaticModel'] =
	array('#type' => 'textfield',
	      '#title' => t('Model'),
	      '#disabled' => TRUE,
	      '#default_value' => $form_state['storage'][$tag]['Model']);
    }
  } else {
    if (isset($settings['subtag'])) {
      $form[$tag][$subtag]['Model'] =
	array('#type' => 'textfield',
	      '#title' => t('Model'),
	      '#maxlength' => 40,
	      '#default_value' => $form_state['storage'][$tag][$subtag]['Model']);
      if (isset($settings['ajaxevent']['model']) and $settings['ajaxevent']['model']) {
	$form[$tag][$subtag]['Model']['#ahah'] =
	  array('event' => 'change',//'blur',
		'path' => ahah_helper_path(array($tag, 'Model')),
		'wrapper' => 'edit-'.$tag.'-Model-wrapper',
		'effect' => 'none',
		'method' => 'replace');
      }
    } else {
      $form[$tag]['Model'] =
	array('#type' => 'textfield',
	      '#title' => t('Model'),
	      '#maxlength' => 40,
	      '#default_value' => $form_state['storage'][$tag]['Model']);
      if (isset($settings['ajaxevent']['model']) and $settings['ajaxevent']['model']) {
	$form[$tag]['Model']['#ahah'] =
	  array('event' => 'change',//'blur',
		'path' => ahah_helper_path(array($tag, 'Model')),
		'wrapper' => 'edit-'.$tag.'-Model-wrapper',
		'effect' => 'none',
		'method' => 'replace');
      }
    }
  }

  if (isset($settings['subtag'])) {
    if (isset($form_state['storage'][$tag][$subtag]['Model']) and 
	!empty($form_state['storage'][$tag][$subtag]['Model'])) {
      $form[$tag][$subtag]['Model']['#field_suffix'] =
	theme('image',
	      $form_state['storage'][$tag]['freq_user_found'] ? 'misc/watchdog-ok.png' : '');
    }
  } else {
    if (isset($form_state['storage'][$tag]['Model']) and 
	!empty($form_state['storage'][$tag]['Model'])) {
      $form[$tag]['Model']['#field_suffix'] =
	theme('image',
	      $form_state['storage'][$tag]['freq_user_found'] ? 'misc/watchdog-ok.png' : '');
    }
  }

  // Layout color
  if (isset($settings['subtag'])) {
    if (!isset($form_state['storage'][$tag][$subtag]['Color'])) {
      if (isset($settings['default']['Color'])) {
	$form_state['storage'][$tag][$subtag]['Color'] = $settings['default']['Color'];
      } else {
	$form_state['storage'][$tag][$subtag]['Color'] = '';
      }
      $form_state['storage'][$tag][$subtag]['OldColor'] = $form_state['storage'][$tag][$subtag]['Color'];
    }
  } else {
    if (!isset($form_state['storage'][$tag]['Color'])) {
      if (isset($settings['default']['Color'])) {
	$form_state['storage'][$tag]['Color'] = $settings['default']['Color'];
      } else {
	$form_state['storage'][$tag]['Color'] = '';
      }
      $form_state['storage'][$tag]['OldColor'] = $form_state['storage'][$tag]['Color'];
    }
  }
  if (isset($settings['subtag'])) {
    if ($form_state['storage'][$tag][$subtag]['Color'] !== $form_state['storage'][$tag][$subtag]['OldColor']) {
      $form_state['storage'][$tag]['ColorChanged'] = TRUE;
    }
    $form[$tag][$subtag]['OldColor'] =
      array('#type' => 'hidden',
	    '#value' => $form_state['storage'][$tag][$subtag]['Color']);
  } else {
    if ($form_state['storage'][$tag]['Color'] !== $form_state['storage'][$tag]['OldColor']) {
      $form_state['storage'][$tag]['ColorChanged'] = TRUE;
    }
    $form[$tag]['OldColor'] =
      array('#type' => 'hidden',
	    '#value' => $form_state['storage'][$tag]['Color']);
  }
  if ($settings['disabled']['Color']) {
    if (isset($settings['subtag'])) {
      $form[$tag][$subtag]['Color'] =
	array('#type' => 'hidden',
	      '#value' => $form_state['storage'][$tag][$subtag]['Color']);
      $form[$tag][$subtag]['StaticColor'] =
	array('#type' => 'textfield',
	      '#title' => t('Color'),
	      '#disabled' => TRUE,
	      '#default_value' => $form_state['storage'][$tag][$subtag]['Color']);
    } else {
      $form[$tag]['Color'] =
	array('#type' => 'hidden',
	      '#value' => $form_state['storage'][$tag]['Color']);
      $form[$tag]['StaticColor'] =
	array('#type' => 'textfield',
	      '#title' => t('Color'),
	      '#disabled' => TRUE,
	      '#default_value' => $form_state['storage'][$tag]['Color']);
    }
  } else {
    if (isset($settings['subtag'])) {
      $form[$tag][$subtag]['Color'] =
	array('#type' => 'textfield',
	      '#title' => t('Color'),
	      '#maxlength' => 30,
	      '#default_value' => $form_state['storage'][$tag][$subtag]['Color']);
      if (isset($settings['ajaxevent']['color']) and $settings['ajaxevent']['color']) {
	$form[$tag][$subtag]['Color']['#ahah'] =
	  array('event' => 'change',//'blur',
		'path' => ahah_helper_path(array($tag, 'Color')),
		'wrapper' => 'edit-'.$tag.'-Color-wrapper',
		'effect' => 'none',
		'method' => 'replace');
      }
    } else {
      $form[$tag]['Color'] =
	array('#type' => 'textfield',
	      '#title' => t('Color'),
	      '#maxlength' => 30,
	      '#default_value' => $form_state['storage'][$tag]['Color']);
      if (isset($settings['ajaxevent']['color']) and $settings['ajaxevent']['color']) {
	$form[$tag]['Color']['#ahah'] =
	  array('event' => 'change',//'blur',
		'path' => ahah_helper_path(array($tag, 'Color')),
		'wrapper' => 'edit-'.$tag.'-Color-wrapper',
		'effect' => 'none',
		'method' => 'replace');
      }
    }
  }

  if (isset($settings['subtag'])) {
    if (isset($form_state['storage'][$tag][$subtag]['Color']) and 
	!empty($form_state['storage'][$tag][$subtag]['Color'])) {
      $form[$tag][$subtag]['Color']['#field_suffix'] =
	theme('image',
	      $form_state['storage'][$tag]['freq_user_found'] ? 'misc/watchdog-ok.png' : '');
    }
  } else {
    if (isset($form_state['storage'][$tag]['Color']) and 
	!empty($form_state['storage'][$tag]['Color'])) {
      $form[$tag]['Color']['#field_suffix'] =
	theme('image',
	      $form_state['storage'][$tag]['freq_user_found'] ? 'misc/watchdog-ok.png' : '');
    }
  }

  return 0;
}



/*
 * $settings = array();
 * $settings['tag'] = 'property';
 * $settings['subtag'] = 'person';
 * $settings['frame'] = TRUE;
 * $settings['frametitle'] = t('The title of the frame');
 * $settings['subframe'] = TRUE;
 * $settings['subframetitle'] = t('The title of the subframe');
 * $settings['default']['Country'] = 'Canada';
 * $settings['default']['Province'] = 'ON';
 * $settings['disabled']['Country'] = TRUE;
 * $settings['disabled']['Province'] = TRUE;
 *
 * Returns:
 *   1 = Error, the caller should just return the form immediately
 *   0 = No errors
 */
function chpprop_produce_location(&$form, &$form_state, $settings = array()) {
  global $user;
  $tag = 'property';
  if (isset($settings['tag'])) {
    $tag = $settings['tag'];
  }
  $subtag = 'person';
  if (isset($settings['subtag'])) {
    $subtag = $settings['subtag'];
  }

  // Layout
  if (!isset($form[$tag])) {
    $form[$tag] =
      array('#prefix' => '<div id="'.$tag.'-wrapper">',
	    '#suffix' => '</div>',
	    '#tree' => TRUE);
    if (isset($settings['frame']) and $settings['frame']) {
      $form[$tag]['#type'] = 'fieldset';
      if (isset($settings['frametitle']) and $settings['frametitle']) {
	$form[$tag]['#title'] = $settings['frametitle'];
      }
    }
  }

  // Build country select
  $countries = chpprop_retrieve_countries();

  // Layout Country
  if (!isset($settings['subtag'])) {
    if (!isset($form_state['storage'][$tag]['Country'])) {
      if (isset($settings['default']['Country'])) {
	$form_state['storage'][$tag]['Country'] =
	  chpprop_retrieve_iso($settings['default']['Country']);
      } else {
	$form_state['storage'][$tag]['Country'] =
	  chpprop_default_country();
      }
    }
    $form[$tag]['Country'] =
      array('#type' => 'select',
	    '#title' => t('Country'),
	    '#options' => $countries,
	    '#disabled' => $settings['disabled']['Country'],
	    '#default_value' => $form_state['storage'][$tag]['Country'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array($tag)),
			     'wrapper' => $tag.'-wrapper'));
  } else {
    if (!isset($form_state['storage'][$tag][$subtag]['Country'])) {
      if (isset($settings['default']['Country'])) {
	$form_state['storage'][$tag][$subtag]['Country'] =
	  chpprop_retrieve_iso($settings['default']['Country']);
      } else {
	$form_state['storage'][$tag][$subtag]['Country'] =
	  chpprop_default_country();
      }
    }
    $form[$tag][$subtag]['Country'] =
      array('#type' => 'select',
	    '#title' => t('Country'),
	    '#options' => $countries,
	    '#disabled' => $settings['disabled']['Country'],
	    '#default_value' => $form_state['storage'][$tag][$subtag]['Country'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array($tag)),
			     'wrapper' => $tag.'-wrapper'));
  }

  // Layout Province
  if (!isset($settings['subtag'])) {
    if (!isset($form_state['storage'][$tag]['Province'])) {
      if (isset($settings['default']['Province'])) {
	$form_state['storage'][$tag]['Province'] = $settings['default']['Province'];
      } else {
	$form_state['storage'][$tag]['Province'] = chpprop_default_province();
      }
    }
    $form[$tag]['Province'] =
      array('#type' => 'select',
	    '#title' => t('Province/State'),
	    '#options' => chpprop_retrieve_states($form_state['storage'][$tag]['Country']),
	    '#disabled' => $settings['disabled']['Province'],
	    '#default_value' => $form_state['storage'][$tag]['Province']);
  } else {
    if (!isset($form_state['storage'][$tag][$subtag]['Province'])) {
      if (isset($settings['default']['Province'])) {
	$form_state['storage'][$tag][$subtag]['Province'] = $settings['default']['Province'];
      } else {
	$form_state['storage'][$tag][$subtag]['Province'] = chpprop_default_province();
      }
    }
    $form[$tag][$subtag]['Province'] =
      array('#type' => 'select',
	    '#title' => t('Province/State'),
	    '#options' => chpprop_retrieve_states($form_state['storage'][$tag][$subtag]['Country']),
	    '#disabled' => $settings['disabled']['Province'],
	    '#default_value' => $form_state['storage'][$tag][$subtag]['Province']);
  }
  return 0;
}



/*
 * $settings = array();
 * $settings['tag'] = 'property';
 * $settings['frame'] = TRUE;
 * $settings['frametitle'] = t('The title of the frame');
 * $settings['subtag'] = 'person';
 * $settings['subframe'] = TRUE;
 * $settings['subframetitle'] = t('The title of the subframe');
 * $settings['subframecollapsible'] = TRUE;
 * $settings['subframecollapsed'] = TRUE;
 * $settings['subframecollapsebtn'] = FALSE; // Default value
 * $settings['option']['wizard'] = 3;
 * $settings['option']['companynameopt'] = $company_id;
 * $settings['option']['showalias'] = 'Alias default';
 * $settings['option']['skipnotrequired'] = TRUE;
 * $settings['option']['skipprename'] = TRUE;
 * $settings['option']['skipfirstname'] = TRUE;
 * $settings['option']['skipmiddlename'] = TRUE;
 * $settings['option']['skiplastname'] = TRUE;
 * $settings['option']['skipsufname'] = TRUE;
 * $settings['option']['skipphone'] = TRUE;
 * $settings['option']['skipemail'] = TRUE;
 * $settings['option']['skipemailnotification'] = TRUE;
 * $settings['option']['skiprole'] = TRUE;
 * $settings['option']['skipdepartment'] = TRUE;
 * $settings['option']['skiptitle'] = TRUE;
 * $settings['option']['skiplogin'] = TRUE;
 * $settings['options']['LinkAdmins'] = array($admin_id => $name);
 * $settings['default']['NotRequired'] = FALSE;
 * $settings['default']['PrenameId'] = $prename_id;
 * $settings['default']['FirstName'] = 'Bill';
 * $settings['default']['MiddleName'] = 'Arnold';
 * $settings['default']['LastName'] = 'Gates';
 * $settings['default']['SufnameId'] = $sufname_id;
 * $settings['default']['Street'] = '1835 73rd Avenue NE';
 * $settings['default']['City'] = 'Medina'
 * $settings['default']['PostalCode'] = '98039'
 * $settings['default']['Province'] = 'WA';
 * $settings['default']['Country'] = 'United States';
 * $settings['default']['Phone'] = '123-123-1234';
 * $settings['default']['Email'] = 'email@example.com';
 * $settings['default']['EmailNotification'] = FALSE;
 * $settings['default']['Role'] = $var;
 * $settings['default']['Department'] = $var;
 * $settings['default']['Title'] = $var;
 * $settings['default']['LinkAdminId'] = $var;
 * $settings['default']['Username'] = $var;
 * $settings['default']['Password'] = $var;
 *
 * Returns:
 *   1 = Error, the caller should just return the form immediately
 *   0 = No errors
 */
function chpprop_produce_person(&$form, &$form_state, $settings = array()) {
  global $user;
  $tag = 'property';
  if (isset($settings['tag'])) {
    $tag = $settings['tag'];
  }
  $subtag = 'person';
  if (isset($settings['subtag'])) {
    $subtag = $settings['subtag'];
  }

  // Layout main frame
  if (!isset($form[$tag])) {
    $form[$tag] =
      array('#prefix' => '<div id="'.$tag.'-wrapper">',
	    '#suffix' => '</div>',
	    '#tree' => TRUE);
    if (isset($settings['frame']) and $settings['frame']) {
      $form[$tag]['#type'] = 'fieldset';
      if (isset($settings['frametitle']) and $settings['frametitle']) {
	$form[$tag]['#title'] = $settings['frametitle'];
      }
    }
  }

  // Layout sub frame
  if (isset($settings['subframe']) and $settings['subframe']) {
    if (!isset($form[$tag][$subtag])) {
      $form[$tag][$subtag] =
	array('#prefix' => '<div id="'.$tag.'-'.$subtag.'-wrapper">',
	      '#suffix' => '</div>',
	      '#tree' => TRUE);
    }
    if (isset($settings['subframe']) and $settings['subframe']) {
      $form[$tag][$subtag]['#type'] = 'fieldset';
      if (isset($settings['subframetitle'])) {
	$form[$tag][$subtag]['#title'] = $settings['subframetitle'];
      }
    }
    if (isset($settings['subframecollapsible']) and $settings['subframecollapsible']) {
      $form[$tag][$subtag]['#collapsible'] = $settings['subframecollapsible'];
    }
    if (isset($settings['subframecollapsed']) and $settings['subframecollapsed']) {
      $form[$tag][$subtag]['#collapsed'] = $settings['subframecollapsed'];
    } elseif (isset($settings['option']['wizard'])) {
      $form[$tag][$subtag]['#collapsed'] =
	$form_state['storage'][$tag]['Wizard'] !== $settings['option']['wizard'];
    }
    if (isset($settings['subframecollapsebtn'])) {
      $form[$tag][$subtag]['#collapsible'] = TRUE;
      if (!isset($form_state['storage'][$tag][$subtag]['framecollapse'])) {
    	$form_state['storage'][$tag][$subtag]['framecollapse'] =
	  $settings['subframecollapsebtn'];
      }
      $form[$tag][$subtag]['framecollapse'] =
    	array('#type' => 'checkbox',
    	      '#title' => t('Collapsed frame'),
	      '#default_value' => $form_state['storage'][$tag][$subtag]['framecollapse'],
    	      '#ahah' => array('event' => 'change',
    			       'path' => ahah_helper_path(array($tag)),
    			       'wrapper' => $tag.'-wrapper'));
      $form[$tag][$subtag]['#collapsed'] =
	$form_state['storage'][$tag][$subtag]['framecollapse'];
    }
  }

  // Include Alias in sub-frame
  if (isset($settings['option']['showalias'])) {
    if (!isset($form_state['storage'][$tag][$subtag]['Alias'])) {
      if (isset($settings['default']['showalias'])) {
	$form_state['storage'][$tag][$subtag]['Alias'] =
	  $settings['default']['showalias'];
      } else {
	$form_state['storage'][$tag][$subtag]['Alias'] = '';
      }
    }
    $form[$tag][$subtag]['Alias'] =
      array('#type' => 'textfield',
	    '#title' => $settings['option']['showalias'],
	    '#default_value' => $form_state['storage'][$tag][$subtag]['Alias'],
	    '#maxlength' => 80);
  }

  // Retrieve name options
  if (isset($settings['option']['companynameopt']) and
      !empty($settings['option']['companynameopt'])) {
    $nameopt = chdbadmin_get_nameopt($settings['option']['companynameopt']);
    if (empty($nameopt)) {
      $nameopt = chdbadmin_get_nameopt();
    }
  } else {
    $nameopt = chdbadmin_get_nameopt();
  } //anai_dbg($nameopt);
  $form_state['storage'][$tag][$subtag]['NameOptions'] = $nameopt;

  if (!empty($nameopt)) {
    if ($nameopt['Fields'] !== 'Select' and
	!isset($settings['option']['skipnotrequired'])) {
      $settings['option']['skipnotrequired'] = TRUE;
    }
    if (!$nameopt['Prename'] and !isset($settings['option']['skipprename'])) {
      $settings['option']['skipprename'] = TRUE;
    }
    if (!$nameopt['FirstName'] and !isset($settings['option']['skipfirstname'])) {
      $settings['option']['skipfirstname'] = TRUE;
    }
    if (!$nameopt['MiddleName'] and !isset($settings['option']['skipmiddlename'])) {
      $settings['option']['skipmiddlename'] = TRUE;
    }
    if (!$nameopt['LastName'] and !isset($settings['option']['skiplastname'])) {
      $settings['option']['skiplastname'] = TRUE;
    }
    if (!$nameopt['Sufname'] and !isset($settings['option']['skipsufname'])) {
      $settings['option']['skipsufname'] = TRUE;
    }
    if (!$nameopt['Phone'] and !isset($settings['option']['skipphone'])) {
      $settings['option']['skipphone'] = TRUE;
    }
    if (!$nameopt['Email'] and !isset($settings['option']['skipemail'])) {
      $settings['option']['skipemail'] = TRUE;
    }
    if (!$nameopt['EmailNotification'] and
	!isset($settings['option']['skipemailnotification'])) {
      $settings['option']['skipemailnotification'] = TRUE;
    }
    if (!$nameopt['Role'] and !isset($settings['option']['skiprole'])) {
      $settings['option']['skiprole'] = TRUE;
    }
    if (!$nameopt['Department'] and !isset($settings['option']['skipdepartment'])) {
      $settings['option']['skipdepartment'] = TRUE;
    }
    if (!$nameopt['Title'] and !isset($settings['option']['skiptitle'])) {
      $settings['option']['skiptitle'] = TRUE;
    }
  }

  // Layout notrequired option
  if (!isset($settings['option']['skipnotrequired']) or
      (isset($settings['option']['skipnotrequired']) and
       !$settings['option']['skipnotrequired'])) {
    if (isset($settings['subframe']) and $settings['subframe']) {
      if (!isset($form_state['storage'][$tag][$subtag]['NotRequired'])) {
	if (isset($settings['default']['NotRequired']) and
	    $settings['default']['NotRequired']) {
	  $form_state['storage'][$tag][$subtag]['NotRequired'] =
	    $settings['default']['NotRequired'];
	} else {
	  $form_state['storage'][$tag][$subtag]['NotRequired'] = FALSE;
	}
      }
      $form[$tag][$subtag]['NotRequired'] =
	array('#type' => 'checkbox',
	      '#title' => t('Not required'),
	      '#default_value' => $form_state['storage'][$tag][$subtag]['NotRequired'],
	      '#ahah' => array('event' => 'change',
			       'path' => ahah_helper_path(array($tag)),
			       'wrapper' => $tag.'-wrapper'));
    } else {
      if (!isset($form_state['storage'][$tag]['NotRequired'])) {
	if (isset($settings['default']['NotRequired']) and
	    $settings['default']['NotRequired']) {
	  $form_state['storage'][$tag]['NotRequired'] =
	    $settings['default']['NotRequired'];
	} else {
	  $form_state['storage'][$tag]['NotRequired'] = FALSE;
	}
      }
      $form[$tag]['NotRequired'] =
	array('#type' => 'checkbox',
	      '#title' => t('Not required'),
	      '#default_value' => $form_state['storage'][$tag]['NotRequired'],
	      '#ahah' => array('event' => 'change',
			       'path' => ahah_helper_path(array($tag)),
			       'wrapper' => $tag.'-wrapper'));
    }
  }

  if ((isset($form_state['storage'][$tag][$subtag]['NotRequired']) and
       $form_state['storage'][$tag][$subtag]['NotRequired']) or
      (isset($form_state['storage'][$tag]['NotRequired']) and
       $form_state['storage'][$tag]['NotRequired'])) {
    return 0;
  }

  // Layout link options
  if (isset($settings['options']['LinkAdmins']) and
      !empty($settings['options']['LinkAdmins'])) {
    if (isset($settings['subframe']) and $settings['subframe']) {
      if (!isset($form_state['storage'][$tag][$subtag]['LinkToAdmin'])) {
	if (isset($settings['default']['LinkAdminId']) and
	    !empty($settings['default']['LinkAdminId'])) {
	  $form_state['storage'][$tag][$subtag]['LinkToAdmin'] = TRUE;
	} else {
	  $form_state['storage'][$tag][$subtag]['LinkToAdmin'] = FALSE;
	}
      }
      $form[$tag][$subtag]['LinkToAdmin'] =
      	array('#type' => 'checkbox',
      	      '#title' => t('Linked to associate'),
      	      '#default_value' => $form_state['storage'][$tag][$subtag]['LinkToAdmin'],
      	      '#ahah' => array('event' => 'change',
      			       'path' => ahah_helper_path(array($tag, $subtag)),
			       'wrapper' => $tag.'-'.$subtag.'-wrapper'));
    } else {
      if (!isset($form_state['storage'][$tag]['LinkToAdmin'])) {
	if (isset($settings['default']['LinkAdminId']) and
	    !empty($settings['default']['LinkAdminId'])) {
	  $form_state['storage'][$tag]['LinkToAdmin'] = TRUE;
	} else {
	  $form_state['storage'][$tag]['LinkToAdmin'] = FALSE;
	}
      }
      
      $form[$tag]['LinkToAdmin'] =
	array('#type' => 'checkbox',
	      '#title' => t('Linked to associate'),
	      '#default_value' => $form_state['storage'][$tag]['LinkToAdmin'],
	      '#ahah' => array('event' => 'change',
			       'method' => 'append',
			       'path' => ahah_helper_path(array($tag)),
			       'wrapper' => $tag.'-wrapper'));
    }

    if (isset($settings['subframe']) and $settings['subframe'] and
    	$form_state['storage'][$tag][$subtag]['LinkToAdmin']) {
      if (!isset($form_state['storage'][$tag][$subtag]['LinkId'])) {
    	if (isset($settings['default']['LinkAdminId'])) {
    	  $form_state['storage'][$tag][$subtag]['LinkId'] =
    	    $settings['default']['LinkAdminId'];
    	} else {
    	  $form_state['storage'][$tag][$subtag]['LinkId'] =
    	    key($settings['options']['LinkAdmins']);
    	}
      }
      $form[$tag][$subtag]['LinkId'] =
    	array('#type' => 'select',
    	      '#title' => t('Associate'),
    	      '#options' => $settings['options']['LinkAdmins'],
    	      '#default_value' => $form_state['storage'][$tag][$subtag]['LinkId']);
    } elseif ($form_state['storage'][$tag]['LinkToAdmin']) {
      if (!isset($form_state['storage'][$tag]['LinkId'])) {
    	if (isset($settings['default']['LinkAdminId'])) {
    	  $form_state['storage'][$tag]['LinkId'] =
    	    $settings['default']['LinkAdminId'];
    	} else {
    	  $form_state['storage'][$tag]['LinkId'] =
    	    key($settings['options']['LinkAdmins']);
    	}
      }
      $form[$tag]['LinkId'] =
    	array('#type' => 'select',
    	      '#title' => t('Associate'),
    	      '#options' => $settings['options']['LinkAdmins'],
    	      '#default_value' => $form_state['storage'][$tag]['LinkId']);
    }
  }

  // Layout Prename
  if (!isset($settings['option']['skipprename']) or
      (isset($settings['option']['skipprename']) and !$settings['option']['skipprename'])) {
    $options = array();
    if (isset($settings['option']['companynameopt']) and
	!empty($settings['option']['companynameopt'])) {
      foreach (chdbadmin_get_prenameopt($settings['option']['companynameopt']) as $opt) {
	$options[$opt['PrenameId']] = decode_entities($opt['Alias']);
      }
      if (empty($options)) {
	foreach (chdbadmin_get_prenameopt() as $opt) {
	  $options[$opt['PrenameId']] = decode_entities($opt['Alias']);
	}
      }
    } else {
      foreach (chdbadmin_get_prenameopt() as $opt) {
	$options[$opt['PrenameId']] = decode_entities($opt['Alias']);
      }
    }
    $form_state['storage'][$tag][$subtag]['PreNameOptions'] = $options;
    if (0 < count($options)) {
      $options = array_merge(array('BLANK' => t('<Blank>')), $options);
      if (isset($settings['subtag'])) {
	if (!isset($form_state['storage'][$tag][$subtag]['PrenameId'])) {
	  if (isset($settings['default']['PrenameId'])) {
	    if (in_array($settings['default']['PrenameId'], array_keys($options))) {
	      $form_state['storage'][$tag][$subtag]['PrenameId'] = $settings['default']['PrenameId'];
	    } else {
	      $form_state['storage'][$tag][$subtag]['PrenameId'] = key($options);
	    }
	  } else {
	    $form_state['storage'][$tag][$subtag]['PrenameId'] = key($options);
	  }
	}
	$form[$tag][$subtag]['PrenameId'] =
	  array('#type' => 'select',
		'#title' => t('Name prefix'),
		'#options' => $options,
		'#default_value' => $form_state['storage'][$tag][$subtag]['PrenameId']);
      } else {
	if (!isset($form_state['storage'][$tag]['PrenameId'])) {
	  if (isset($settings['default']['PrenameId'])) {
	    if (in_array($settings['default']['PrenameId'], array_keys($options))) {
	      $form_state['storage'][$tag]['PrenameId'] = $settings['default']['PrenameId'];
	    } else {
	      $form_state['storage'][$tag]['PrenameId'] = key($options);
	    }
	  } else {
	    $form_state['storage'][$tag]['PrenameId'] = key($options);
	  }
	}
	$form[$tag]['PrenameId'] =
	  array('#type' => 'select',
		'#title' => t('Name prefix'),
		'#options' => $options,
		'#default_value' => $form_state['storage'][$tag]['PrenameId']);
      }
    }
  }

  // Layout FirstName
  if (!isset($settings['option']['skipfirstname']) or
      (isset($settings['option']['skipfirstname']) and !$settings['option']['skipfirstname'])) {
    if (isset($settings['subtag'])) {
      if (!isset($form_state['storage'][$tag][$subtag]['FirstName'])) {
	if (isset($settings['default']['FirstName'])) {
	  $form_state['storage'][$tag][$subtag]['FirstName'] =
	    decode_entities($settings['default']['FirstName']);
	} else {
	  $form_state['storage'][$tag][$subtag]['FirstName'] = '';
	}
      }
      $form[$tag][$subtag]['FirstName'] =
	array('#type' => 'textfield',
	      '#title' => t('First name'),
	      '#maxlength' => 80,
	      '#default_value' => $form_state['storage'][$tag][$subtag]['FirstName']);
    } else {
      if (!isset($form_state['storage'][$tag]['FirstName'])) {
	if (isset($settings['default']['FirstName'])) {
	  $form_state['storage'][$tag]['FirstName'] =
	    decode_entities($settings['default']['FirstName']);
	} else {
	  $form_state['storage'][$tag]['FirstName'] = '';
	}
      }
      $form[$tag]['FirstName'] =
	array('#type' => 'textfield',
	      '#title' => t('First name'),
	      '#maxlength' => 80,
	      '#default_value' => $form_state['storage'][$tag]['FirstName']);
    }
  }

  // Layout MiddleName
  if (!isset($settings['option']['skipmiddlename']) or
      (isset($settings['option']['skipmiddlename']) and !$settings['option']['skipmiddlename'])) {
    if (isset($settings['subtag'])) {
      if (!isset($form_state['storage'][$tag][$subtag]['MiddleName'])) {
	if (isset($settings['default']['MiddleName'])) {
	  $form_state['storage'][$tag][$subtag]['MiddleName'] =
	    decode_entities($settings['default']['MiddleName']);
	} else {
	  $form_state['storage'][$tag][$subtag]['MiddleName'] = '';
	}
      }
      $form[$tag][$subtag]['MiddleName'] =
	array('#type' => 'textfield',
	      '#title' => t('Middle name'),
	      '#maxlength' => 80,
	      '#default_value' => $form_state['storage'][$tag][$subtag]['MiddleName']);
    } else {
      if (!isset($form_state['storage'][$tag]['MiddleName'])) {
	if (isset($settings['default']['MiddleName'])) {
	  $form_state['storage'][$tag]['MiddleName'] =
	    decode_entities($settings['default']['MiddleName']);
	} else {
	  $form_state['storage'][$tag]['MiddleName'] = '';
	}
      }
      $form[$tag]['MiddleName'] =
	array('#type' => 'textfield',
	      '#title' => t('Middle name'),
	      '#maxlength' => 80,
	      '#default_value' => $form_state['storage'][$tag]['MiddleName']);
    }
  }

  // Layout LastName
  if (!isset($settings['option']['skiplastname']) or
      (isset($settings['option']['skiplastname']) and !$settings['option']['skiplastname'])) {
    if (isset($settings['subtag'])) {
      if (!isset($form_state['storage'][$tag][$subtag]['LastName'])) {
	if (isset($settings['default']['LastName'])) {
	  $form_state['storage'][$tag][$subtag]['LastName'] =
	    decode_entities($settings['default']['LastName']);
	} else {
	  $form_state['storage'][$tag][$subtag]['LastName'] = '';
	}
      }
      $form[$tag][$subtag]['LastName'] =
	array('#type' => 'textfield',
	      '#title' => t('Last name'),
	      '#maxlength' => 80,
	      '#default_value' => $form_state['storage'][$tag][$subtag]['LastName']);
    } else {
      if (!isset($form_state['storage'][$tag]['LastName'])) {
	if (isset($settings['default']['LastName'])) {
	  $form_state['storage'][$tag]['LastName'] =
	    decode_entities($settings['default']['LastName']);
	} else {
	  $form_state['storage'][$tag]['LastName'] = '';
	}
      }
      $form[$tag]['LastName'] =
	array('#type' => 'textfield',
	      '#title' => t('Last name'),
	      '#maxlength' => 80,
	      '#default_value' => $form_state['storage'][$tag]['LastName']);
    }
  }

  // Layout Sufname
  if (!isset($settings['option']['skipsufname']) or
      (isset($settings['option']['skipsufname']) and !$settings['option']['skipsufname'])) {
    $options = array();
    if (isset($settings['option']['companynameopt']) and
	!empty($settings['option']['companynameopt'])) {
      foreach (chdbadmin_get_sufnameopt($settings['option']['companynameopt']) as $opt) {
	$options[$opt['SufnameId']] = decode_entities($opt['Alias']);
      }
      if (empty($options)) {
	foreach (chdbadmin_get_sufnameopt() as $opt) {
	  $options[$opt['SufnameId']] = decode_entities($opt['Alias']);
	}
      }
    } else {
      foreach (chdbadmin_get_sufnameopt() as $opt) {
	$options[$opt['SufnameId']] = decode_entities($opt['Alias']);
      }
    }
    $form_state['storage'][$tag][$subtag]['SufNameOptions'] = $options;
    if (0 < count($options)) {
      $options = array_merge(array('BLANK' => t('<Blank>')), $options);
      if (isset($settings['subtag'])) {
	if (!isset($form_state['storage'][$tag][$subtag]['SufnameId'])) {
	  if (isset($settings['default']['SufnameId'])) {
	    if (in_array($settings['default']['SufnameId'], array_keys($options))) {
	      $form_state['storage'][$tag][$subtag]['SufnameId'] = $settings['default']['SufnameId'];
	    } else {
	      $form_state['storage'][$tag][$subtag]['SufnameId'] = key($options);
	    }
	  } else {
	    $form_state['storage'][$tag][$subtag]['SufnameId'] = key($options);
	  }
	}
	$form[$tag][$subtag]['SufnameId'] =
	  array('#type' => 'select',
		'#title' => t('Name suffix'),
		'#options' => $options,
		'#default_value' => $form_state['storage'][$tag][$subtag]['SufnameId']);
      } else {
	if (!isset($form_state['storage'][$tag]['SufnameId'])) {
	  if (isset($settings['default']['SufnameId'])) {
	    if (in_array($settings['default']['SufnameId'], array_keys($options))) {
	      $form_state['storage'][$tag]['SufnameId'] = $settings['default']['SufnameId'];
	    } else {
	      $form_state['storage'][$tag]['SufnameId'] = key($options);
	    }
	  } else {
	    $form_state['storage'][$tag]['SufnameId'] = key($options);
	  }
	}
	$form[$tag]['SufnameId'] =
	  array('#type' => 'select',
		'#title' => t('Name suffix'),
		'#options' => $options,
		'#default_value' => $form_state['storage'][$tag]['SufnameId']);
      }
    }
  }

  // Layout role
  if (!isset($settings['option']['skiprole']) or
      (isset($settings['option']['skiprole']) and !$settings['option']['skiprole'])) {
    if (isset($settings['subtag'])) {
      if (isset($form_state['storage'][$tag][$subtag]['LinkToAdmin']) and
	  $form_state['storage'][$tag][$subtag]['LinkToAdmin']) {
	$options = array();
	$options[t('Wife')] = t('Wife');
	$options[t('Husband')] = t('Husband');
	$options[t('Spouse')] = t('Spouse');
	$options['Other'] = t('Other');
      } else {
	$options = array();
	$options[t('Primary contact')] = t('Primary contact');
	$options[t('Secondary contact')] = t('Secondary contact');
	$options['Other'] = t('Other');
      }
      if (!isset($form_state['storage'][$tag][$subtag]['Role'])) {
	if (isset($settings['default']['Role']) and
	    in_array($settings['default']['Role'], array_keys($options))) {
	  $form_state['storage'][$tag][$subtag]['Role'] =
	    $settings['default']['Role'];
	} elseif (isset($settings['default']['Role'])) {
	  $form_state['storage'][$tag][$subtag]['Role'] = 'Other';
	} else {
	  $form_state['storage'][$tag][$subtag]['Role'] = key($options);
	}
      }
      $form[$tag][$subtag]['Role'] =
	array('#type' => 'select',
	      '#title' => t('Role'),
	      '#options' => $options,
	      '#disabled' => $settings['disabled']['Role'],
	      '#default_value' => $form_state['storage'][$tag][$subtag]['Role'],
	      '#ahah' => array('event' => 'change',
			       'path' => ahah_helper_path(array($tag, $subtag)),
			       'wrapper' => $tag.'-'.$subtag.'-wrapper'));

      if ($form_state['storage'][$tag][$subtag]['Role'] === 'Other') {
	if (!isset($form_state['storage'][$tag][$subtag]['OtherRole'])) {
	  $form_state['storage'][$tag][$subtag]['OtherRole'] =
	    $settings['default']['Role'];
	}
	$form[$tag][$subtag]['OtherRole'] =
	  array('#type' => 'textfield',
		'#title' => t('Other role'),
		'#maxlength' => 80,
		'#default_value' => $form_state['storage'][$tag][$subtag]['OtherRole']);
      }
    } else {
      if (isset($form_state['storage'][$tag]['LinkToAdmin']) and
	  !$form_state['storage'][$tag]['LinkToAdmin']) {
	$options = array();
	$options[t('Wife')] = t('Wife');
	$options[t('Husband')] = t('Husband');
	$options[t('Spouse')] = t('Spouse');
	$options['Other'] = t('Other');
      } else {
	$options = array();
	$options[t('Primary contact')] = t('Primary contact');
	$options[t('Secondary contact')] = t('Secondary contact');
	$options['Other'] = t('Other');
      }
      if (!isset($form_state['storage'][$tag]['Role'])) {
	if (isset($settings['default']['Role']) and
	    in_array($settings['default']['Role'], array_keys($options))) {
	  $form_state['storage'][$tag]['Role'] =
	    $settings['default']['Role'];
	} elseif (isset($settings['default']['Role'])) {
	  $form_state['storage'][$tag]['Role'] = 'Other';
	} else {
	  $form_state['storage'][$tag]['Role'] = key($options);
	}
      }
      $form[$tag]['Role'] =
	array('#type' => 'select',
	      '#title' => t('Role'),
	      '#options' => $options,
	      '#disabled' => $settings['disabled']['Role'],
	      '#default_value' => $form_state['storage'][$tag]['Role'],
	      '#ahah' => array('event' => 'change',
			       'path' => ahah_helper_path(array($tag, $subtag)),
			       'wrapper' => $tag.'-'.$subtag.'-wrapper'));

      if ($form_state['storage'][$tag]['Role'] === 'Other') {
	if (!isset($form_state['storage'][$tag]['OtherRole'])) {
	  $form_state['storage'][$tag]['OtherRole'] =
	    $settings['default']['Role'];
	}
	$form[$tag]['OtherRole'] =
	  array('#type' => 'textfield',
		'#title' => t('Other role'),
		'#maxlength' => 80,
		'#default_value' => $form_state['storage'][$tag]['OtherRole']);
      }
    }
  }

  // Layout department
  if (!isset($settings['option']['skipdepartment']) or
      (isset($settings['option']['skipdepartment']) and !$settings['option']['skipdepartment'])) {
    if (isset($settings['subtag'])) {
      if (!isset($form_state['storage'][$tag][$subtag]['Department'])) {
	if (isset($settings['default']['Department'])) {
	  $form_state['storage'][$tag][$subtag]['Department'] =
	    decode_entities($settings['default']['Department']);
	} else {
	  $form_state['storage'][$tag][$subtag]['Department'] = '';
	}
      }
      $form[$tag][$subtag]['Department'] =
	array('#type' => 'textfield',
	      '#title' => t('Department'),
	      '#maxlength' => 80,
	      '#default_value' => $form_state['storage'][$tag][$subtag]['Department']);
    } else {
      if (!isset($form_state['storage'][$tag]['Department'])) {
	if (isset($settings['default']['Department'])) {
	  $form_state['storage'][$tag]['Department'] =
	    decode_entities($settings['default']['Department']);
	} else {
	  $form_state['storage'][$tag]['Department'] = '';
	}
      }
      $form[$tag]['Department'] =
	array('#type' => 'textfield',
	      '#title' => t('Department'),
	      '#maxlength' => 80,
	      '#default_value' => $form_state['storage'][$tag]['Department']);
    }
  }

  // Layout title
  if (!isset($settings['option']['skiptitle']) or
      (isset($settings['option']['skiptitle']) and !$settings['option']['skiptitle'])) {
    if (isset($settings['subtag'])) {
      if (!isset($form_state['storage'][$tag][$subtag]['Title'])) {
	if (isset($settings['default']['Title'])) {
	  $form_state['storage'][$tag][$subtag]['Title'] =
	    decode_entities($settings['default']['Title']);
	} else {
	  $form_state['storage'][$tag][$subtag]['Title'] = '';
	}
      }
      $form[$tag][$subtag]['Title'] =
	array('#type' => 'textfield',
	      '#title' => t('Job title'),
	      '#maxlength' => 80,
	      '#default_value' => $form_state['storage'][$tag][$subtag]['Title']);
    } else {
      if (!isset($form_state['storage'][$tag]['Title'])) {
	if (isset($settings['default']['Title'])) {
	  $form_state['storage'][$tag]['Title'] =
	    decode_entities($settings['default']['Title']);
	} else {
	  $form_state['storage'][$tag]['Title'] = '';
	}
      }
      $form[$tag]['Title'] =
	array('#type' => 'textfield',
	      '#title' => t('Title'),
	      '#maxlength' => 80,
	      '#default_value' => $form_state['storage'][$tag]['Title']);
    }
  }

  // Layout Street
  if (isset($settings['subtag'])) {
    if (!isset($form_state['storage'][$tag][$subtag]['Street'])) {
      if (isset($settings['default']['Street'])) {
	$form_state['storage'][$tag][$subtag]['Street'] =
	  decode_entities($settings['default']['Street']);
      } else {
	$form_state['storage'][$tag][$subtag]['Street'] = '';
      }
    }
    $form[$tag][$subtag]['Street'] =
      array('#type' => 'textfield',
	    '#title' => t('Street'),
	    '#maxlength' => 80,
	    '#default_value' => $form_state['storage'][$tag][$subtag]['Street']);
  } else {
    if (!isset($form_state['storage'][$tag]['Street'])) {
      if (isset($settings['default']['Street'])) {
	$form_state['storage'][$tag]['Street'] =
	  decode_entities($settings['default']['Street']);
      } else {
	$form_state['storage'][$tag]['Street'] = '';
      }
    }
    $form[$tag]['Street'] =
      array('#type' => 'textfield',
	    '#title' => t('Street'),
	    '#maxlength' => 80,
	    '#default_value' => $form_state['storage'][$tag]['Street']);
  }

  // Layout City
  if (isset($settings['subtag'])) {
    if (!isset($form_state['storage'][$tag][$subtag]['City'])) {
      if (isset($settings['default']['City'])) {
	$form_state['storage'][$tag][$subtag]['City'] =
	  decode_entities($settings['default']['City']);
      } else {
	$form_state['storage'][$tag][$subtag]['City'] = '';
      }
    }
    $form[$tag][$subtag]['City'] =
      array('#type' => 'textfield',
	    '#title' => t('City'),
	    '#maxlength' => 80,
	    '#default_value' => $form_state['storage'][$tag][$subtag]['City']);
  } else {
    if (!isset($form_state['storage'][$tag]['City'])) {
      if (isset($settings['default']['City'])) {
	$form_state['storage'][$tag]['City'] =
	  decode_entities($settings['default']['City']);
      } else {
	$form_state['storage'][$tag]['City'] = '';
      }
    }
    $form[$tag]['City'] =
      array('#type' => 'textfield',
	    '#title' => t('City'),
	    '#maxlength' => 80,
	    '#default_value' => $form_state['storage'][$tag]['City']);
  }

  // Layout PostalCode
  if (isset($settings['subtag'])) {
    if (!isset($form_state['storage'][$tag][$subtag]['PostalCode'])) {
      if (isset($settings['default']['PostalCode'])) {
	$form_state['storage'][$tag][$subtag]['PostalCode'] =
	  decode_entities($settings['default']['PostalCode']);
      } else {
	$form_state['storage'][$tag][$subtag]['PostalCode'] = '';
      }
    }
    $form[$tag][$subtag]['PostalCode'] =
      array('#type' => 'textfield',
	    '#title' => t('Postal code'),
	    '#maxlength' => 80,
	    '#default_value' => $form_state['storage'][$tag][$subtag]['PostalCode']);
  } else {
    if (!isset($form_state['storage'][$tag]['PostalCode'])) {
      if (isset($settings['default']['PostalCode'])) {
	$form_state['storage'][$tag]['PostalCode'] =
	  decode_entities($settings['default']['PostalCode']);
      } else {
	$form_state['storage'][$tag]['PostalCode'] = '';
      }
    }
    $form[$tag]['PostalCode'] =
      array('#type' => 'textfield',
	    '#title' => t('Postal code'),
	    '#maxlength' => 80,
	    '#default_value' => $form_state['storage'][$tag]['PostalCode']);
  }

  // Layout Province and Country
  chpprop_produce_location($form, $form_state, $settings);

  // Layout Phone
  if (!isset($settings['option']['skipphone']) or
      (isset($settings['option']['skipphone']) and !$settings['option']['skipphone'])) {
    if (isset($settings['subtag'])) {
      if (!isset($form_state['storage'][$tag][$subtag]['Phone'])) {
	if (isset($settings['default']['Phone'])) {
	  $form_state['storage'][$tag][$subtag]['Phone'] =
	    decode_entities($settings['default']['Phone']);
	} else {
	  $form_state['storage'][$tag][$subtag]['Phone'] = '';
	}
      }
      $form[$tag][$subtag]['Phone'] =
	array('#type' => 'textfield',
	      '#title' => t('Phone'),
	      '#maxlength' => 20,
	      '#default_value' => $form_state['storage'][$tag][$subtag]['Phone']);
    } else {
      if (!isset($form_state['storage'][$tag]['Phone'])) {
	if (isset($settings['default']['Phone'])) {
	  $form_state['storage'][$tag]['Phone'] =
	    decode_entities($settings['default']['Phone']);
	} else {
	  $form_state['storage'][$tag]['Phone'] = '';
	}
      }
      $form[$tag]['Phone'] =
	array('#type' => 'textfield',
	      '#title' => t('Phone'),
	      '#maxlength' => 20,
	      '#default_value' => $form_state['storage'][$tag]['Phone']);
    }
  }

  // Layout Email
  if (!isset($settings['option']['skipemail']) or
      (isset($settings['option']['skipemail']) and !$settings['option']['skipemail'])) {
    if (isset($settings['subtag'])) {
      if (!isset($form_state['storage'][$tag][$subtag]['Email'])) {
	if (isset($settings['default']['Email'])) {
	  $form_state['storage'][$tag][$subtag]['Email'] =
	    decode_entities($settings['default']['Email']);
	} else {
	  $form_state['storage'][$tag][$subtag]['Email'] = '';
	}
      }
      $form[$tag][$subtag]['Email'] =
	array('#type' => 'textfield',
	      '#title' => t('Email'),
	      '#maxlength' => 80,
	      '#default_value' => $form_state['storage'][$tag][$subtag]['Email']);
    } else {
      if (!isset($form_state['storage'][$tag]['Email'])) {
	if (isset($settings['default']['Email'])) {
	  $form_state['storage'][$tag]['Email'] =
	    decode_entities($settings['default']['Email']);
	} else {
	  $form_state['storage'][$tag]['Email'] = '';
	}
      }
      $form[$tag]['Email'] =
	array('#type' => 'textfield',
	      '#title' => t('Email'),
	      '#maxlength' => 80,
	      '#default_value' => $form_state['storage'][$tag]['Email']);
    }
  }

  // Layout EmailNotification
  if (!isset($settings['option']['skipemailnotification']) or
      (isset($settings['option']['skipemailnotification']) and !$settings['option']['skipemailnotification'])) {
    if (isset($settings['subtag'])) {
      if (!isset($form_state['storage'][$tag][$subtag]['EmailNotification'])) {
	if (isset($settings['default']['EmailNotification'])) {
	  $form_state['storage'][$tag][$subtag]['EmailNotification'] =
	    $settings['default']['EmailNotification'];
	} else {
	  $form_state['storage'][$tag][$subtag]['EmailNotification'] = FALSE;
	}
      }
      $form[$tag][$subtag]['EmailNotification'] =
	array('#type' => 'checkbox',
	      '#title' => t('Email notification'),
	      '#default_value' => $form_state['storage'][$tag][$subtag]['EmailNotification']);
    } else {
      if (!isset($form_state['storage'][$tag]['EmailNotification'])) {
	if (isset($settings['default']['EmailNotification'])) {
	  $form_state['storage'][$tag]['EmaiNotificationl'] =
	    $settings['default']['EmailNotification'];
	} else {
	  $form_state['storage'][$tag]['EmailNotification'] = FALSE;
	}
      }
      $form[$tag]['EmailNotification'] =
	array('#type' => 'checkbox',
	      '#title' => t('Email notification'),
	      '#default_value' => $form_state['storage'][$tag]['EmailNotification']);
    }
  }

  // Layout Login
  if (!isset($settings['option']['skiplogin']) or
      (isset($settings['option']['skiplogin']) and !$settings['option']['skiplogin'])) {
    if (isset($settings['subtag'])) {
      if (!isset($form_state['storage'][$tag][$subtag]['Username'])) {
	if (isset($settings['default']['Username'])) {
	  $form_state['storage'][$tag][$subtag]['Username'] =
	    decode_entities($settings['default']['Username']);
	} else {
	  $form_state['storage'][$tag][$subtag]['Username'] = '';
	}
      }
      $form[$tag][$subtag]['Username'] =
	array('#type' => 'textfield',
	      '#title' => t('Mobile unit username'),
	      '#maxlength' => 24,
	      '#default_value' => $form_state['storage'][$tag][$subtag]['Username'],
	      '#description' => t('Example: firstname.lastname'));
    } else {
      if (!isset($form_state['storage'][$tag]['Username'])) {
	if (isset($settings['default']['Username'])) {
	  $form_state['storage'][$tag]['Username'] =
	    decode_entities($settings['default']['Username']);
	} else {
	  $form_state['storage'][$tag]['Username'] = '';
	}
      }
      $form[$tag]['Username'] =
	array('#type' => 'textfield',
	      '#title' => t('Mobile unit username'),
	      '#maxlength' => 16,
	      '#default_value' => $form_state['storage'][$tag]['Username'],
	      '#description' => t('Example: firstname.lastname'));
    }

    if (isset($settings['subtag'])) {
      if (!isset($form_state['storage'][$tag][$subtag]['Password'])) {
	if (isset($settings['default']['Password'])) {
	  $form_state['storage'][$tag][$subtag]['Password'] =
	    decode_entities($settings['default']['Password']);
	} else {
	  $form_state['storage'][$tag][$subtag]['Password'] = '';
	}
      }
      $form[$tag][$subtag]['Password'] =
	array('#type' => 'textfield',
	      '#title' => t('Mobile unit password'),
	      '#maxlength' => 80,
	      '#default_value' => $form_state['storage'][$tag][$subtag]['Password']);
    } else {
      if (!isset($form_state['storage'][$tag]['Password'])) {
	if (isset($settings['default']['Password'])) {
	  $form_state['storage'][$tag]['Password'] =
	    decode_entities($settings['default']['Password']);
	} else {
	  $form_state['storage'][$tag]['Password'] = '';
	}
      }
      $form[$tag]['Password'] =
	array('#type' => 'textfield',
	      '#title' => t('Mobile unit password'),
	      '#maxlength' => 80,
	      '#default_value' => $form_state['storage'][$tag]['Password']);
    }
  }

  return 0;
}



/*
 * $settings = array();
 * $settings['tag'] = 'ajax';
 * $settings['frame'] = TRUE;
 * $settings['frametitle'] = t('The title of the frame');
 * $settings['subtag'] = 'policy';
 * $settings['subframe'] = TRUE;
 * $settings['subframetitle'] = t('The title of the subframe');
 * $settings['subframecollapsible'] = TRUE;
 * $settings['subframecollapsed'] = TRUE;
 * $settings['subframecollapsebtn'] = FALSE; // Default value
 * $settings['option']['readonly'] = TRUE;
 * $settings['option']['wizard'] = 3;
 * $settings['option']['skiplotname'] = TRUE;
 * $settings['option']['skipprofilename'] = TRUE;
 * $settings['option']['skipprofileselect'] = TRUE;
 * $settings['option']['custom'] = array('key' => $key, 'value' => $value);
 * $settings['default']['LotName'] = 'NAME';
 * $settings['default']['ProfileName'] = 'NAME';
 * $settings['default']['ProfileId'] = $profile_id;
 * $settings['default']['rCheckbox'] = $val;
 * $settings['default']['rPolicyAlias'] = $val;
 * $settings['default']['rPolicyType'] = $val;
 * $settings['default']['rPolicyClass'] = $val;
 * $settings['default']['rValidityChoice'] = $val;
 * $settings['default']['rValidityDays'] = $val;
 * $settings['default']['rRemovalChoice'] = $val;
 * $settings['default']['rRemovalRetention'] = $val;
 * $settings['default']['rRemovalDays'] = $val;
 * $settings['default']['rStallChoice'] = $val;
 * $settings['default']['rStallLevels'] = $val;
 * $settings['default']['rStallCount'] = $val;
 * $settings['default']['rPermitChoice'] = $val;
 * $settings['default']['rPermitServices'] = array('Shared' => TRUE);
 * $settings['default']['vCheckbox'] = $val;
 * $settings['default']['vPolicyAlias'] = $val;
 * $settings['default']['vPolicyType'] = $val;
 * $settings['default']['vPolicyClass'] = $val;
 * $settings['default']['vWeekdays'] = array('Mon' => 'Mon', 'Tue' => 'Tue', 'Wed' => 'Wed');
 * $settings['default']['vStartHH'] = $val;
 * $settings['default']['vStartMM'] = $val;
 * $settings['default']['vStopHH'] = $val;
 * $settings['default']['vStopMM'] = $val;
 * $settings['default']['vOvernightStartHH'] = $val;
 * $settings['default']['vOvernightStartMM'] = $val;
 * $settings['default']['vOvernightStopHH'] = $val;
 * $settings['default']['vOvernightStopMM'] = $val;
 * $settings['default']['vExpiryChoice'] = $val;
 * $settings['default']['vExpiryDuration'] = $val;
 * $settings['default']['vExpiryDays'] = $val;
 * $settings['default']['vRenewRetention'] = $val;
 * $settings['default']['vRenewRetentionDays'] = $val;
 * $settings['default']['vStallChoice'] = $val;
 * $settings['default']['vStallLevels'] = $val;
 * $settings['default']['vStallCount'] = $val;
 * $settings['default']['vExtensionChoice'] = $val;
 * $settings['default']['vExtensionServices'] = array('Shared' => TRUE);
 * $settings['default']['vRegularCost'] = '0.00';
 * $settings['default']['vOvernightCost'] = '0.00';
 * $settings['default']['vRegularComment'] = $val;
 * $settings['default']['valCheckbox'] = $val;
 * $settings['default']['valServices'] = array('Messenger' => TRUE, 'Mobile' => TRUE);
 * $settings['default']['valNumunits'] = $val;
 * $settings['default']['valModifiers'] =
 *    array('DisableCheckout' => TRUE, 'DisableSearch' => TRUE, 'DisableCodes' => TRUE);
 *
 * Returns:
 *   1 = Error, the caller should just return the form immediately
 *   0 = No errors
 */
function chpprop_produce_policy(&$form, &$form_state, $settings = array()) {
  global $user;
  $tag = 'ajax';
  if (isset($settings['tag'])) {
    $tag = $settings['tag'];
  }
  $subtag = 'policy';
  if (isset($settings['subtag'])) {
    $subtag = $settings['subtag'];
  }
  if (!isset($form[$tag])) {
    $form[$tag] =
      array('#prefix' => '<div id="'.$tag.'-wrapper">',
	    '#suffix' => '</div>',
	    '#tree' => TRUE);
    if (isset($settings['frame']) and $settings['frame']) {
      $form[$tag]['#type'] = 'fieldset';
      if (isset($settings['frametitle']) and $settings['frametitle']) {
	$form[$tag]['#title'] = $settings['frametitle'];
      }
    }
  }
  if (!isset($form[$tag][$subtag])) {
    $form[$tag][$subtag] =
      array('#prefix' => '<div id="'.$tag.'-'.$subtag.'-wrapper">',
	    '#suffix' => '</div>',
	    '#tree' => TRUE);
  }
  if (isset($settings['subframe']) and $settings['subframe']) {
    $form[$tag][$subtag] =
      array('#type' => 'fieldset');
    if (isset($settings['subframetitle'])) {
      $form[$tag][$subtag]['#title'] = $settings['subframetitle'];
    }
    if (isset($settings['subframecollapsible']) and $settings['subframecollapsible']) {
      $form[$tag][$subtag]['#collapsible'] = $settings['subframecollapsible'];
    }
    if (isset($settings['subframecollapsed']) and $settings['subframecollapsed']) {
      $form[$tag][$subtag]['#collapsed'] = $settings['subframecollapsed'];
    } elseif (isset($settings['option']['wizard'])) {
      $form[$tag][$subtag]['#collapsed'] = $form_state['storage'][$tag]['Wizard'] !==
	$settings['option']['wizard'];
    }
    if (isset($settings['subframecollapsebtn'])) {
      $form[$tag][$subtag]['#collapsible'] = TRUE;
      if (!isset($form_state['storage'][$tag][$subtag]['framecollapse'])) {
    	$form_state['storage'][$tag][$subtag]['framecollapse'] = $settings['subframecollapsebtn'];
      }
      $form[$tag][$subtag]['framecollapse'] =
    	array('#type' => 'checkbox',
    	      '#title' => t('Collapsed frame'),
	      '#default_value' => $form_state['storage'][$tag][$subtag]['framecollapse'],
    	      '#ahah' => array('event' => 'change',
    			       'path' => ahah_helper_path(array($tag)),
    			       'wrapper' => $tag.'-wrapper'));
      $form[$tag][$subtag]['#collapsed'] = $form_state['storage'][$tag][$subtag]['framecollapse'];
    }
  }
  if (isset($settings['option']['readonly']) and $settings['option']['readonly']) {
    $disabled = TRUE;
  } else {
    $disabled = FALSE;
  }

  if (!isset($settings['option']['skipprofilename']) or
      (isset($settings['option']['skipprofilename']) and !$settings['option']['skipprofilename'])) {
    if (!isset($form_state['storage'][$tag][$subtag]['ProfileName'])) {
      if (isset($settings['default']['ProfileName'])) {
	$form_state['storage'][$tag][$subtag]['ProfileName'] = $settings['default']['ProfileName'];
      } else {
	$form_state['storage'][$tag][$subtag]['ProfileName'] = '';
      }
    }
    $form[$tag][$subtag]['ProfileName'] =
      array('#type' => 'textfield',
	    '#title' => t('Profile name'),
	    '#maxlength' => 80,
	    '#disabled' => $disabled,
	    '#default_value' => $form_state['storage'][$tag][$subtag]['ProfileName']);
  }

  if (!isset($settings['option']['skiplotname']) or
      (isset($settings['option']['skiplotname']) and !$settings['option']['skiplotname'])) {
    if (!isset($form_state['storage'][$tag][$subtag]['LotName'])) {
      if (isset($settings['default']['LotName'])) {
	$form_state['storage'][$tag][$subtag]['LotName'] = $settings['default']['LotName'];
      } else {
	$form_state['storage'][$tag][$subtag]['LotName'] = '';
      }
    }
    $form[$tag][$subtag]['LotName'] =
      array('#type' => 'textfield',
	    '#title' => t('Lot name'),
	    '#maxlength' => 80,
	    '#disabled' => $disabled,
	    '#default_value' => $form_state['storage'][$tag][$subtag]['LotName'],
	    '#description' => t('Examples: Main lot, North lot, etc'));
  }

  if (!isset($settings['option']['skipprofileselect']) or
      (isset($settings['option']['skipprofileselect']) and
       !$settings['option']['skipprofileselect'])) {
    $options = array();
    if (isset($settings['option']['custom']['key']) and
	isset($settings['option']['custom']['value'])) {
      $options[$settings['option']['custom']['key']] = $settings['option']['custom']['value'];
      $custom_key = $settings['option']['custom']['key'];
    } else {
      $options['Custom'] = t('Custom');
      $custom_key = 'Custom';
    }
    $profiles = chpprop_retrieve_profiles($user->uid);
    foreach ($profiles as $profile_id => $profile) {
      $options[$profile_id] = $profile['data']['Alias'];
    }
    if (!isset($form_state['storage'][$tag][$subtag]['ProfileId'])) {
      if (1 < count($options) and
	  (isset($settings['default']['ProfileId']) and
	   in_array($settings['default']['ProfileId'], array_keys($options)))) {
	$form_state['storage'][$tag][$subtag]['ProfileId'] = $settings['default']['ProfileId'];
      } else {
	$form_state['storage'][$tag][$subtag]['ProfileId'] = key($options);
      }
    }
    $form[$tag][$subtag]['ProfileId'] =
      array('#type' => 'select',
	    '#title' => t('Property profile'),
	    '#options' => $options,
	    '#disabled' => $disabled,
	    '#default_value' => $form_state['storage'][$tag][$subtag]['ProfileId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));

    if ($form_state['storage'][$tag][$subtag]['ProfileId'] !== $custom_key) {
      return 0;
    }
  }

  /***
   * Residential
   */

  if (!isset($form_state['storage'][$tag][$subtag]['rCheckbox'])) {
    if (isset($settings['default']['rCheckbox'])) {
      $form_state['storage'][$tag][$subtag]['rCheckbox'] = $settings['default']['rCheckbox'];
    } else {
      $form_state['storage'][$tag][$subtag]['rCheckbox'] = FALSE;
    }
  }
  $form[$tag][$subtag]['rCheckbox'] =
    array('#type' => 'checkbox',
	  '#title' => t('Long term permits'),
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['rCheckbox'],
  	  '#disabled' => $disabled,
  	  '#prefix' => '<table><tr><td>');

  if (!isset($form_state['storage'][$tag][$subtag]['rPolicyAlias'])) {
    if (isset($settings['default']['rPolicyAlias'])) {
      $form_state['storage'][$tag][$subtag]['rPolicyAlias'] =
	$settings['default']['rPolicyAlias'];
    } else {
      $form_state['storage'][$tag][$subtag]['rPolicyAlias'] = t('Residential');
    }
  }
  $form[$tag][$subtag]['rPolicyAlias'] =
    array('#type' => 'textfield',
	  '#title' => t('Alias'),
	  '#maxlength' => 50,
	  '#size' => 20,
  	  '#disabled' => $disabled,
	  '#default_value' => $form_state['storage'][$tag][$subtag]['rPolicyAlias']);

  $options = array();
  foreach (chdbadmin_get_lottypeopt() as $opt) {
    $options[$opt['LottypeId']] = decode_entities($opt['Alias']);
  }
  if (empty($options)) {
    $options['Unspecified'] = t('Unspecified');
  }
  if (!isset($form_state['storage'][$tag][$subtag]['rPolicyType'])) {
    if (isset($settings['default']['rPolicyType'])) {
      $form_state['storage'][$tag][$subtag]['rPolicyType'] =
	$settings['default']['rPolicyType'];
    } else {
      $form_state['storage'][$tag][$subtag]['rPolicyType'] = key($options);
    }
  }
  $form[$tag][$subtag]['rPolicyType'] =
    array('#type' => 'select',
	  '#title' => t('Lot type'),
  	  '#options' => $options,
  	  '#disabled' => $disabled,
	  '#default_value' => $form_state['storage'][$tag][$subtag]['rPolicyType']);

  $options = array();
  foreach (chdbadmin_get_lotclassopt() as $opt) {
    $options[$opt['LotclassId']] = decode_entities($opt['Alias']);
  }
  if (empty($options)) {
    $options['Unspecified'] = t('Unspecified');
  }
  if (!isset($form_state['storage'][$tag][$subtag]['rPolicyClass'])) {
    if (isset($settings['default']['rPolicyClass'])) {
      $form_state['storage'][$tag][$subtag]['rPolicyClass'] =
	$settings['default']['rPolicyClass'];
    } else {
      $form_state['storage'][$tag][$subtag]['rPolicyClass'] = key($options);
    }
  }
  $form[$tag][$subtag]['rPolicyClass'] =
    array('#type' => 'select',
	  '#title' => t('Classification'),
  	  '#options' => $options,
  	  '#disabled' => $disabled,
	  '#default_value' => $form_state['storage'][$tag][$subtag]['rPolicyClass'],
	  '#suffix' => '</td>');

  $options = array();
  $options = array(t('Never expires'),
		   t('One year'),
		   t('One month'),
		   t('Other'));
  if (!isset($form_state['storage'][$tag][$subtag]['rValidityChoice'])) {
    if (isset($settings['default']['rValidityChoice'])) {
      $form_state['storage'][$tag][$subtag]['rValidityChoice'] =
	$settings['default']['rValidityChoice'];
    } else {
      $form_state['storage'][$tag][$subtag]['rValidityChoice'] = key($options);
    }
  }
  $form[$tag][$subtag]['rValidityChoice'] =
    array('#type' => 'radios',
	  '#title' => t('Validity'),
  	  '#options' => $options,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['rValidityChoice'],
  	  '#prefix' => '<td valign="top" width="25%">');

  if (!isset($form_state['storage'][$tag][$subtag]['rValidityDays'])) {
    if (isset($settings['default']['rValidityDays'])) {
      $form_state['storage'][$tag][$subtag]['rValidityDays'] =
	$settings['default']['rValidityDays'];
    } else {
      $form_state['storage'][$tag][$subtag]['rValidityDays'] = 30;
    }
  }
  $form[$tag][$subtag]['rValidityDays'] =
    array('#type' => 'textfield',
	  '#title' => t('Number of days'),
  	  '#maxlength' => 5,
	  '#size' => 5,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['rValidityDays']);

  $options = array();
  $options = array(t('Manual'),
		   t('Automatic'));
  if (!isset($form_state['storage'][$tag][$subtag]['rRemovalChoice'])) {
    if (isset($settings['default']['rRemovalChoice'])) {
      $form_state['storage'][$tag][$subtag]['rRemovalChoice'] =
	$settings['default']['rRemovalChoice'];
    } else {
      $form_state['storage'][$tag][$subtag]['rRemovalChoice'] = key($options);
    }
  }
  $form[$tag][$subtag]['rRemovalChoice'] =
    array('#type' => 'radios',
	  '#title' => t('Permit removal'),
  	  '#options' => $options,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['rRemovalChoice']);

  if (!isset($form_state['storage'][$tag][$subtag]['rRemovalRetention'])) {
    if (isset($settings['default']['rRemovalRetention'])) {
      $form_state['storage'][$tag][$subtag]['rRemovalRetention'] =
	$settings['default']['rRemovalRetention'];
    } else {
      $form_state['storage'][$tag][$subtag]['rRemovalRetention'] = FALSE;
    }
  }
  $form[$tag][$subtag]['rRemovalRetention'] =
    array('#type' => 'checkbox',
	  '#title' => t('Retention'),
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['rRemovalRetention']);

  if (!isset($form_state['storage'][$tag][$subtag]['rRemovalDays'])) {
    if (isset($settings['default']['rRemovalDays'])) {
      $form_state['storage'][$tag][$subtag]['rRemovalDays'] =
	$settings['default']['rRemovalDays'];
    } else {
      $form_state['storage'][$tag][$subtag]['rRemovalDays'] = 14;
    }
  }
  $form[$tag][$subtag]['rRemovalDays'] =
    array('#type' => 'textfield',
	  '#title' => t('Retention days'),
  	  '#maxlength' => 5,
	  '#size' => 5,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['rRemovalDays'],
  	  '#suffix' => '</td>');

  // Stall options
  $options = array(t('Unmanaged'),
		   t('Managed total'),
		   t('Managed stalls'));
  if (!isset($form_state['storage'][$tag][$subtag]['rStallChoice'])) {
    if (isset($settings['default']['rStallChoice'])) {
      $form_state['storage'][$tag][$subtag]['rStallChoice'] =
	$settings['default']['rStallChoice'];
    } else {
      $form_state['storage'][$tag][$subtag]['rStallChoice'] = TENANT_STALL_UNMANAGED;
    }
  }
  $form[$tag][$subtag]['rStallChoice'] =
    array('#type' => 'radios',
	  '#title' => t('Stall policy'),
  	  '#options' => $options,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['rStallChoice'],
  	  '#prefix' => '<td valign="top" width="25%">');

  if (!isset($form_state['storage'][$tag][$subtag]['rStallLevels'])) {
    if (isset($settings['default']['rStallLevels'])) {
      $form_state['storage'][$tag][$subtag]['rStallLevels'] =
	$settings['default']['rStallLevels'];
    } else {
      $form_state['storage'][$tag][$subtag]['rStallLevels'] = 1;
    }
  }
  $form[$tag][$subtag]['rStallLevels'] =
    array('#type' => 'textfield',
	  '#title' => t('Parking levels'),
  	  '#maxlength' => 50,
	  '#size' => 20,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['rStallLevels'],
	  '#description' => t('Comma separated list of levels'));

  if (!isset($form_state['storage'][$tag][$subtag]['rStallCount'])) {
    if (isset($settings['default']['rStallCount'])) {
      $form_state['storage'][$tag][$subtag]['rStallCount'] =
	$settings['default']['rStallCount'];
    } else {
      $form_state['storage'][$tag][$subtag]['rStallCount'] = 20;
    }
  }
  $form[$tag][$subtag]['rStallCount'] =
    array('#type' => 'textfield',
	  '#title' => t('Parking stalls per level'),
  	  '#maxlength' => 50,
	  '#size' => 20,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['rStallCount'],
	  '#description' => t('Comma separated list of stalls per level'),
  	  '#suffix' => '</td>');

  // Permit policy options
  $options = array();
  $options = array(t('Regular transients'),
		   t('+Container'),
		   t('Container only'));
  if (!isset($form_state['storage'][$tag][$subtag]['rPermitChoice'])) {
    if (isset($settings['default']['rPermitChoice'])) {
      $form_state['storage'][$tag][$subtag]['rPermitChoice'] =
	$settings['default']['rPermitChoice'];
    } else {
      $form_state['storage'][$tag][$subtag]['rPermitChoice'] = key($options);
    }
  }
  $form[$tag][$subtag]['rPermitChoice'] =
    array('#type' => 'radios',
	  '#title' => t('Permit policy'),
  	  '#options' => $options,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['rPermitChoice'],
  	  '#prefix' => '<td valign="top">');

  // Added services options
  $options = array();
  $options['Shared'] = t('Shared permits');
  if (!isset($form_state['storage'][$tag][$subtag]['rPermitServices'])) {
    if (isset($settings['default']['rPermitServices'])) {
      $form_state['storage'][$tag][$subtag]['rPermitServices'] =
	$settings['default']['rPermitServices'];
    } else {
      $form_state['storage'][$tag][$subtag]['rPermitServices'] = array();
    }
  }
  $form[$tag][$subtag]['rPermitServices'] =
    array('#type' => 'checkboxes',
	  '#title' => t('Added services'),
  	  '#options' => $options,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['rPermitServices'],
  	  '#suffix' => '</td> </tr></table>');

  /***
   * Visitor
   */

  if (!isset($form_state['storage'][$tag][$subtag]['vCheckbox'])) {
    if (isset($settings['default']['vCheckbox'])) {
      $form_state['storage'][$tag][$subtag]['vCheckbox'] =
	$settings['default']['vCheckbox'];
    } else {
      $form_state['storage'][$tag][$subtag]['vCheckbox'] = FALSE;
    }
  }
  $form[$tag][$subtag]['vCheckbox'] =
    array('#type' => 'checkbox',
	  '#title' => t('Short term permits'),
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vCheckbox'],
  	  '#prefix' => '<table><tr><td rowspan="3">');

  if (!isset($form_state['storage'][$tag][$subtag]['vPolicyAlias'])) {
    if (isset($settings['default']['vPolicyAlias'])) {
      $form_state['storage'][$tag][$subtag]['vPolicyAlias'] =
	$settings['default']['vPolicyAlias'];
    } else {
      $form_state['storage'][$tag][$subtag]['vPolicyAlias'] = t('Visitor');
    }
  }
  $form[$tag][$subtag]['vPolicyAlias'] =
    array('#type' => 'textfield',
	  '#title' => t('Alias'),
	  '#maxlength' => 50,
	  '#size' => 20,
  	  '#disabled' => $disabled,
	  '#default_value' => $form_state['storage'][$tag][$subtag]['vPolicyAlias']);

  $options = array();
  foreach (chdbadmin_get_lottypeopt() as $opt) {
    $options[$opt['LottypeId']] = decode_entities($opt['Alias']);
  }
  if (empty($options)) {
    $options['Unspecified'] = t('Unspecified');
  }
  if (!isset($form_state['storage'][$tag][$subtag]['vPolicyType'])) {
    if (isset($settings['default']['vPolicyType'])) {
      $form_state['storage'][$tag][$subtag]['vPolicyType'] =
	$settings['default']['vPolicyType'];
    } else {
      $form_state['storage'][$tag][$subtag]['vPolicyType'] = key($options);
    }
  }
  $form[$tag][$subtag]['vPolicyType'] =
    array('#type' => 'select',
	  '#title' => t('Lot type'),
  	  '#options' => $options,
  	  '#disabled' => $disabled,
	  '#default_value' => $form_state['storage'][$tag][$subtag]['vPolicyType']);

  $options = array();
  foreach (chdbadmin_get_lotclassopt() as $opt) {
    $options[$opt['LotclassId']] = decode_entities($opt['Alias']);
  }
  if (empty($options)) {
    $options['Unspecified'] = t('Unspecified');
  }
  if (!isset($form_state['storage'][$tag][$subtag]['vPolicyClass'])) {
    if (isset($settings['default']['vPolicyClass'])) {
      $form_state['storage'][$tag][$subtag]['vPolicyClass'] =
	$settings['default']['vPolicyClass'];
    } else {
      $form_state['storage'][$tag][$subtag]['vPolicyClass'] = key($options);
    }
  }
  $form[$tag][$subtag]['vPolicyClass'] =
    array('#type' => 'select',
	  '#title' => t('Classification'),
  	  '#options' => $options,
  	  '#disabled' => $disabled,
	  '#default_value' => $form_state['storage'][$tag][$subtag]['vPolicyClass'],
	  '#suffix' => '</td>');

  $options = array();
  $options = array('Mon' => t('Monday'),
		   'Tue' => t('Tuesday'),
		   'Wed' => t('Wednesday'),
  		   'Thu' => t('Thursday'),
		   'Fri' => t('Friday'),
		   'Sat' => t('Saturday'),
  		   'Sun' => t('Sunday'));
  $defaults = array('Mon' => 'Mon',
		    'Tue' => 'Tue',
		    'Wed' => 'Wed',
  		    'Thu' => 'Thu',
		    'Fri' => 'Fri',
		    'Sat' => 'Sat',
  		    'Sun' => 'Sun');

  if (!isset($form_state['storage'][$tag][$subtag]['vWeekdays'])) {
    if (isset($settings['default']['vWeekdays'])) {
      $form_state['storage'][$tag][$subtag]['vWeekdays'] =
	$settings['default']['vWeekdays'];
    } else {
      $form_state['storage'][$tag][$subtag]['vWeekdays'] = $defaults;
    }
  }
  $form[$tag][$subtag]['vWeekdays'] =
    array('#type' => 'checkboxes',
	  '#title' => t('Days open'),
	  '#options' => $options,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vWeekdays'],
  	  '#prefix' => '<td valign="top">',
  	  '#suffix' => '</td>');

  $options = array();
  for ($i = 0; $i < 24; $i++) {
    $options[$i] = sprintf("%02d", $i);
  }
  if (!isset($form_state['storage'][$tag][$subtag]['vStartHH'])) {
    if (isset($settings['default']['vStartHH'])) {
      $form_state['storage'][$tag][$subtag]['vStartHH'] =
	$settings['default']['vStartHH'];
    } else {
      $form_state['storage'][$tag][$subtag]['vStartHH'] = 7;
    }
  }
  $form[$tag][$subtag]['vStartHH'] =
    array('#type' => 'select',
	  '#title' => t('HH'),
	  '#options' => $options,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vStartHH'],
  	  '#prefix' =>
	  '<td valign="top" width="20%" class="noborder">'.
	  '<fieldset><legend><b>Visitor start:</b></legend><table><tr><td class="noborder">',
  	  '#suffix' => '</td>');

  $options = array();
  for ($i = 0; $i < 60; $i++) {
    $options[$i] = sprintf("%02d", $i);
  }
  if (!isset($form_state['storage'][$tag][$subtag]['vStartMM'])) {
    if (isset($settings['default']['vStartMM'])) {
      $form_state['storage'][$tag][$subtag]['vStartMM'] =
	$settings['default']['vStartMM'];
    } else {
      $form_state['storage'][$tag][$subtag]['vStartMM'] = key($options);
    }
  }
  $form[$tag][$subtag]['vStartMM'] =
    array('#type' => 'select',
	  '#title' => t('MM'),
	  '#options' => $options,
   	  '#disabled' => $disabled,
 	  '#default_value' => $form_state['storage'][$tag][$subtag]['vStartMM'],
  	  '#prefix' => '<td class="noborder">',
  	  '#suffix' => '</td></tr></table></fieldset>');

  $options = array();
  for ($i = 0; $i < 24; $i++) {
    $options[$i] = sprintf("%02d", $i);
  }
  if (!isset($form_state['storage'][$tag][$subtag]['vStopHH'])) {
    if (isset($settings['default']['vStopHH'])) {
      $form_state['storage'][$tag][$subtag]['vStopHH'] =
	$settings['default']['vStopHH'];
    } else {
      $form_state['storage'][$tag][$subtag]['vStopHH'] = 23;
    }
  }
  $form[$tag][$subtag]['vStopHH'] =
    array('#type' => 'select',
	  '#title' => t('HH'),
	  '#options' => $options,
   	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vStopHH'],
  	  '#prefix' => '<fieldset><legend><b>Visitor stop:</b></legend><table><tr><td class="noborder">',
  	  '#suffix' => '</td>');

  $options = array();
  for ($i = 0; $i < 60; $i++) {
    $options[$i] = sprintf("%02d", $i);
  }
  if (!isset($form_state['storage'][$tag][$subtag]['vStopMM'])) {
    if (isset($settings['default']['vStopMM'])) {
      $form_state['storage'][$tag][$subtag]['vStopMM'] =
	$settings['default']['vStopMM'];
    } else {
      $form_state['storage'][$tag][$subtag]['vStopMM'] = 59;
    }
  }
  $form[$tag][$subtag]['vStopMM'] =
    array('#type' => 'select',
	  '#title' => t('MM'),
	  '#options' => $options,
   	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vStopMM'],
  	  '#prefix' => '<td class="noborder">',
  	  '#suffix' => '</td></tr></table></fieldset> </td>');

  $options = array();
  for ($i = 0; $i < 24; $i++) {
    $options[$i] = sprintf("%02d", $i);
  }
  if (!isset($form_state['storage'][$tag][$subtag]['vOvernightStartHH'])) {
    if (isset($settings['default']['vOvernightStartHH'])) {
      $form_state['storage'][$tag][$subtag]['vOvernightStartHH'] =
	$settings['default']['vOvernightStartHH'];
    } else {
      $form_state['storage'][$tag][$subtag]['vOvernightStartHH'] = 2;
    }
  }
  $form[$tag][$subtag]['vOvernightStartHH'] =
    array('#type' => 'select',
	  '#title' => t('HH'),
	  '#options' => $options,
   	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vOvernightStartHH'],
  	  '#prefix' =>
	  '<td valign="top" width="20%" class="noborder">'.
	  '<fieldset><legend><b>Overnight start:</b></legend><table><tr><td class="noborder">',
  	  '#suffix' => '</td>');

  $options = array();
  for ($i = 0; $i < 60; $i++) {
    $options[$i] = sprintf("%02d", $i);
  }
  if (!isset($form_state['storage'][$tag][$subtag]['vOvernightStartMM'])) {
    if (isset($settings['default']['vOvernightStartMM'])) {
      $form_state['storage'][$tag][$subtag]['vOvernightStartMM'] =
	$settings['default']['vOvernightStartMM'];
    } else {
      $form_state['storage'][$tag][$subtag]['vOvernightStartMM'] = 0;
    }
  }
  $form[$tag][$subtag]['vOvernightStartMM'] =
    array('#type' => 'select',
	  '#title' => t('MM'),
	  '#options' => $options,
   	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vOvernightStartMM'],
  	  '#prefix' => '<td class="noborder">',
  	  '#suffix' => '</td></tr></table></fieldset>');

  $options = array();
  for ($i = 0; $i < 24; $i++) {
    $options[$i] = sprintf("%02d", $i);
  }
  if (!isset($form_state['storage'][$tag][$subtag]['vOvernightStopHH'])) {
    if (isset($settings['default']['vOvernightStopHH'])) {
      $form_state['storage'][$tag][$subtag]['vOvernightStopHH'] =
	$settings['default']['vOvernightStopHH'];
    } else {
      $form_state['storage'][$tag][$subtag]['vOvernightStopHH'] = 6;
    }
  }
  $form[$tag][$subtag]['vOvernightStopHH'] =
    array('#type' => 'select',
	  '#title' => t('HH'),
	  '#options' => $options,
   	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vOvernightStopHH'],
  	  '#prefix' => '<fieldset><legend><b>Overnight stop:</b></legend><table><tr><td class="noborder">',
  	  '#suffix' => '</td>');

  $options = array();
  for ($i = 0; $i < 60; $i++) {
    $options[$i] = sprintf("%02d", $i);
  }
  if (!isset($form_state['storage'][$tag][$subtag]['vOvernightStopMM'])) {
    if (isset($settings['default']['vOvernightStopMM'])) {
      $form_state['storage'][$tag][$subtag]['vOvernightStopMM'] =
	$settings['default']['vOvernightStopMM'];
    } else {
      $form_state['storage'][$tag][$subtag]['vOvernightStopMM'] = 59;
    }
  }
  $form[$tag][$subtag]['vOvernightStopMM'] =
    array('#type' => 'select',
	  '#title' => t('MM'),
	  '#options' => $options,
   	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vOvernightStopMM'],
  	  '#prefix' => '<td class="noborder">',
  	  '#suffix' => '</td></tr></table></fieldset> </td></td>');

  $options = array();
  $options = array(t('Visitor start'),
		   t('Visitor stop'),
  		   t('Overnight start'),
		   t('Overnight stop'));
  if (!isset($form_state['storage'][$tag][$subtag]['vExpiryChoice'])) {
    if (isset($settings['default']['vExpiryChoice'])) {
      $form_state['storage'][$tag][$subtag]['vExpiryChoice'] =
 $settings['default']['vExpiryChoice'];
    } else {
      $form_state['storage'][$tag][$subtag]['vExpiryChoice'] = VISITOR_STOP_TIME;
    }
  }
  $form[$tag][$subtag]['vExpiryChoice'] =
    array('#type' => 'radios',
	  '#title' => t('Daily expiry policy'),
  	  '#options' => $options,
   	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vExpiryChoice'],
  	  '#prefix' => '<tr><td valign="top" width="25%">');

  $options = array();
  $options = array(t('Single day'),
		   t('Select days'),
		   t('Select date'));
  if (!isset($form_state['storage'][$tag][$subtag]['vExpiryDuration'])) {
    if (isset($settings['default']['vExpiryDuration'])) {
      $form_state['storage'][$tag][$subtag]['vExpiryDuration'] =
 $settings['default']['vExpiryDuration'];
    } else {
      $form_state['storage'][$tag][$subtag]['vExpiryDuration'] = key($options);
    }
  }
  $form[$tag][$subtag]['vExpiryDuration'] =
    array('#type' => 'radios',
	  '#title' => t('Permit expiry policy'),
  	  '#options' => $options,
   	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vExpiryDuration']);

  if (!isset($form_state['storage'][$tag][$subtag]['vExpiryDays'])) {
    if (isset($settings['default']['vExpiryDays'])) {
      $form_state['storage'][$tag][$subtag]['vExpiryDays'] =
	$settings['default']['vExpiryDays'];
    } else {
      $form_state['storage'][$tag][$subtag]['vExpiryDays'] = 3;
    }
  }
  $form[$tag][$subtag]['vExpiryDays'] =
    array('#type' => 'textfield',
	  '#title' => t('Maximum days'),
  	  '#maxlength' => 5,
	  '#size' => 5,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vExpiryDays']);

  if (!isset($form_state['storage'][$tag][$subtag]['vRenewRetention'])) {
    if (isset($settings['default']['vRenewRetention'])) {
      $form_state['storage'][$tag][$subtag]['vRenewRetention'] =
	$settings['default']['vRenewRetention'];
    } else {
      $form_state['storage'][$tag][$subtag]['vRenewRetention'] = FALSE;
    }
  }
  $form[$tag][$subtag]['vRenewRetention'] =
    array('#type' => 'checkbox',
	  '#title' => t('Retention'),
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vRenewRetention']);

  if (!isset($form_state['storage'][$tag][$subtag]['vRenewRetentionDays'])) {
    if (isset($settings['default']['vRenewRetentionDays'])) {
      $form_state['storage'][$tag][$subtag]['vRenewRetentionDays'] =
	$settings['default']['vRenewRetentionDays'];
    } else {
      $form_state['storage'][$tag][$subtag]['vRenewRetentionDays'] = 14;
    }
  }
  $form[$tag][$subtag]['vRenewRetentionDays'] =
    array('#type' => 'textfield',
	  '#title' => t('Retention days'),
  	  '#maxlength' => 5,
	  '#size' => 5,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vRenewRetentionDays'],
  	  '#suffix' => '</td>');

  // Stall options
  $options = array(t('Unmanaged'),
		   t('Managed total'),
		   t('Managed stalls'));
  if (!isset($form_state['storage'][$tag][$subtag]['vStallChoice'])) {
    if (isset($settings['default']['vStallChoice'])) {
      $form_state['storage'][$tag][$subtag]['vStallChoice'] =
	$settings['default']['vStallChoice'];
    } else {
      $form_state['storage'][$tag][$subtag]['vStallChoice'] = VISITOR_STALL_UNMANAGED;
    }
  }
  $form[$tag][$subtag]['vStallChoice'] =
    array('#type' => 'radios',
	  '#title' => t('Stall policy'),
  	  '#options' => $options,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vStallChoice'],
  	  '#prefix' => '<td valign="top" width="25%">');

  if (!isset($form_state['storage'][$tag][$subtag]['vStallLevels'])) {
    if (isset($settings['default']['vStallLevels'])) {
      $form_state['storage'][$tag][$subtag]['vStallLevels'] =
	$settings['default']['vStallLevels'];
    } else {
      $form_state['storage'][$tag][$subtag]['vStallLevels'] = 1;
    }
  }
  $form[$tag][$subtag]['vStallLevels'] =
    array('#type' => 'textfield',
	  '#title' => t('Parking levels'),
  	  '#maxlength' => 50,
	  '#size' => 20,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vStallLevels'],
	  '#description' => t('Comma separated list of levels'));

  if (!isset($form_state['storage'][$tag][$subtag]['vStallCount'])) {
    if (isset($settings['default']['vStallCount'])) {
      $form_state['storage'][$tag][$subtag]['vStallCount'] =
	$settings['default']['vStallCount'];
    } else {
      $form_state['storage'][$tag][$subtag]['vStallCount'] = 20;
    }
  }
  $form[$tag][$subtag]['vStallCount'] =
    array('#type' => 'textfield',
	  '#title' => t('Parking stalls per level'),
  	  '#maxlength' => 50,
	  '#size' => 20,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vStallCount'],
	  '#description' => t('Comma separated list of stalls per level'),
  	  '#suffix' => '</td>');

  // Permit policy options
  $options = array();
  $options = array(t('Regular transients'),
		   t('+Overnight'),
		   t('+Container'),
		   t('++Both'),
		   t('Container only'),
		   t('Hotel'),
		   t('Loading dock'));
  if (!isset($form_state['storage'][$tag][$subtag]['vExtensionChoice'])) {
    if (isset($settings['default']['vExtensionChoice'])) {
      $form_state['storage'][$tag][$subtag]['vExtensionChoice'] =
	$settings['default']['vExtensionChoice'];
    } else {
      $form_state['storage'][$tag][$subtag]['vExtensionChoice'] = key($options);
    }
  }
  $form[$tag][$subtag]['vExtensionChoice'] =
    array('#type' => 'radios',
	  '#title' => t('Permit policy'),
  	  '#options' => $options,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vExtensionChoice'],
  	  '#prefix' => '<td valign="top">');

  // Added services options
  $options = array();
  $options['Shared'] = t('Shared permits');
  if (!isset($form_state['storage'][$tag][$subtag]['vExtensionServices'])) {
    if (isset($settings['default']['vExtensionServices'])) {
      $form_state['storage'][$tag][$subtag]['vExtensionServices'] =
	$settings['default']['vExtensionServices'];
    } else {
      $form_state['storage'][$tag][$subtag]['vExtensionServices'] = array();
    }
  }
  $form[$tag][$subtag]['vExtensionServices'] =
    array('#type' => 'checkboxes',
	  '#title' => t('Added services'),
  	  '#options' => $options,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['vExtensionServices'],
  	  '#suffix' => '</td> </tr>');

  if (!isset($form_state['storage'][$tag][$subtag]['vRegularCost'])) {
    if (isset($settings['default']['vRegularCost'])) {
      $form_state['storage'][$tag][$subtag]['vRegularCost'] =
	$settings['default']['vRegularCost'];
    } else {
      $form_state['storage'][$tag][$subtag]['vRegularCost'] = '0.00';
    }
  }
  $form[$tag][$subtag]['vRegularCost'] =
    array('#type' => 'textfield',
	  '#title' => t('Regular permit cost'),
	  '#maxlength' => 10,
	  '#size' => 10,
	  '#description' => 'Format: x.xx',
	  '#disabled' => $disabled,
	  '#default_value' => $form_state['storage'][$tag][$subtag]['vRegularCost'],
	  '#prefix' => '<tr><td colspan="3"><table><tr><td class="noborder">',
  	  '#suffix' => '</td>');

  if (!isset($form_state['storage'][$tag][$subtag]['vOvernightCost'])) {
    if (isset($settings['default']['vOvernightCost'])) {
      $form_state['storage'][$tag][$subtag]['vOvernightCost'] =
	$settings['default']['vOvernightCost'];
    } else {
      $form_state['storage'][$tag][$subtag]['vOvernightCost'] = '0.00';
    }
  }
  $form[$tag][$subtag]['vOvernightCost'] =
    array('#type' => 'textfield',
	  '#title' => t('Overnight permit cost'),
	  '#maxlength' => 10,
	  '#size' => 10,
	  '#description' => 'Format: x.xx',
	  '#disabled' => $disabled,
	  '#default_value' => $form_state['storage'][$tag][$subtag]['vOvernightCost'],
	  '#prefix' => '<td class="noborder">',
  	  '#suffix' => '</td></tr></table>');

  if (!isset($form_state['storage'][$tag][$subtag]['vRegularComment'])) {
    if (isset($settings['default']['vRegularComment'])) {
      $form_state['storage'][$tag][$subtag]['vRegularComment'] =
	$settings['default']['vRegularComment'];
    } else {
      $form_state['storage'][$tag][$subtag]['vRegularComment'] = '';
    }
  }
  $form[$tag][$subtag]['vRegularComment'] =
    array('#type' => 'textfield',
	  '#title' => t('Regular permit comment'),
	  '#maxlength' => 120,
	  '#size' => 40,
	  '#description' =>
	  t('Displayed when creating new permit.<br>'.
	    'Example: Cost of permit is $6.00, overnight is $20.00'),
	  '#disabled' => $disabled,
	  '#default_value' => $form_state['storage'][$tag][$subtag]['vRegularComment'],
	  '#suffix' => '</td></tr> </tr></table>');

  /***
   * Valet
   */

  if (!isset($form_state['storage'][$tag][$subtag]['valCheckbox'])) {
    if (isset($settings['default']['valCheckbox'])) {
      $form_state['storage'][$tag][$subtag]['valCheckbox'] =
	$settings['default']['valCheckbox'];
    } else {
      $form_state['storage'][$tag][$subtag]['valCheckbox'] = FALSE;
    }
  }
  $form[$tag][$subtag]['valCheckbox'] =
    array('#type' => 'checkbox',
	  '#title' => t('Valet permits'),
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['valCheckbox'],
  	  '#prefix' => '<table><tr><td rowspan="1">',
  	  '#suffix' => '</td>');

  $options = array();
  $options['Messenger'] = t('ANAI Messenger');
  $options['Mobile'] = t('Mobile units');
  if (!isset($form_state['storage'][$tag][$subtag]['valServices'])) {
    if (isset($settings['default']['valServices'])) {
      $form_state['storage'][$tag][$subtag]['valServices'] =
	$settings['default']['valServices'];
    } else {
      $form_state['storage'][$tag][$subtag]['valServices'] = array();
    }
  }
  $form[$tag][$subtag]['valServices'] =
    array('#type' => 'checkboxes',
	  '#title' => t('Services'),
	  '#options' => $options,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['valServices'],
  	  '#prefix' => '<td valign="top">');

  if (!isset($form_state['storage'][$tag][$subtag]['valNumunits'])) {
    if (isset($settings['default']['valNumunits'])) {
      $form_state['storage'][$tag][$subtag]['valNumunits'] =
	$settings['default']['valNumunits'];
    } else {
      $form_state['storage'][$tag][$subtag]['valNumunits'] = 5;
    }
  }
  $form[$tag][$subtag]['valNumunits'] =
    array('#type' => 'textfield',
	  '#title' => t('Maximum mobile units'),
  	  '#maxlength' => 5,
	  '#size' => 5,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['valNumunits'],
  	  '#suffix' => '</td>');

  $options = array();
  $options['DisableCheckout'] = t('Disable checkout');
  $options['DisableSearch'] = t('Disable search');
  $options['DisableCodes'] = t('Disable codes');
  if (!isset($form_state['storage'][$tag][$subtag]['valModifiers'])) {
    if (isset($settings['default']['valModifiers'])) {
      $form_state['storage'][$tag][$subtag]['valModifiers'] =
	$settings['default']['valModifiers'];
    } else {
      $form_state['storage'][$tag][$subtag]['valModifiers'] = array();
    }
  }
  $form[$tag][$subtag]['valModifiers'] =
    array('#type' => 'checkboxes',
	  '#title' => t('Modifiers'),
	  '#options' => $options,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['valModifiers'],
  	  '#prefix' => '<td valign="top">',
  	  '#suffix' => '</td> </tr></table>');

  return $form;
}



/*
 * $settings = array();
 * $settings['tag'] = 'ajax';
 * $settings['frame'] = TRUE;
 * $settings['frametitle'] = t('The title of the frame');
 * $settings['subtag'] = 'policy';
 * $settings['subframe'] = TRUE;
 * $settings['subframetitle'] = t('The title of the subframe');
 * $settings['subframecollapsible'] = TRUE;
 * $settings['subframecollapsed'] = TRUE;
 * $settings['subframecollapsebtn'] = FALSE; // Default value
 * $settings['option']['readonly'] = TRUE;
 * $settings['option']['wizard'] = 3;
 * $settings['option']['skipprofilename'] = TRUE;
 * $settings['option']['skipprofileselect'] = TRUE;
 * $settings['option']['custom'] = array('key' => $key, 'value' => $value);
 * $settings['default']['ProfileName'] = 'NAME';
 * $settings['default']['EnforcementId'] = $enforcement_id;
 * $settings['default']['ticketCheckbox'] = $val;
 * $settings['default']['towCheckbox'] = $val;
 * $settings['default']['bootCheckbox'] = $val;
 * $settings['default']['mobileCheckbox'] = $val;
 * $settings['default']['mobileUnits'] = $val;
 * Returns:
 *   1 = Error, the caller should just return the form immediately
 *   0 = No errors
 */
function chpprop_produce_enforcement(&$form, &$form_state, $settings = array()) {
  global $user;
  $tag = 'ajax';
  if (isset($settings['tag'])) {
    $tag = $settings['tag'];
  }
  $subtag = 'enforcement';
  if (isset($settings['subtag'])) {
    $subtag = $settings['subtag'];
  }
  if (!isset($form[$tag])) {
    $form[$tag] =
      array('#prefix' => '<div id="'.$tag.'-wrapper">',
	    '#suffix' => '</div>',
	    '#tree' => TRUE);
    if (isset($settings['frame']) and $settings['frame']) {
      $form[$tag]['#type'] = 'fieldset';
      if (isset($settings['frametitle']) and $settings['frametitle']) {
	$form[$tag]['#title'] = $settings['frametitle'];
      }
    }
  }
  if (!isset($form[$tag][$subtag])) {
    $form[$tag][$subtag] =
      array('#prefix' => '<div id="'.$tag.'-'.$subtag.'-wrapper">',
	    '#suffix' => '</div>',
	    '#tree' => TRUE);
  }
  if (isset($settings['subframe']) and $settings['subframe']) {
    $form[$tag][$subtag] =
      array('#type' => 'fieldset');
    if (isset($settings['subframetitle'])) {
      $form[$tag][$subtag]['#title'] = $settings['subframetitle'];
    }
    if (isset($settings['subframecollapsible']) and $settings['subframecollapsible']) {
      $form[$tag][$subtag]['#collapsible'] = $settings['subframecollapsible'];
    }
    if (isset($settings['subframecollapsed']) and $settings['subframecollapsed']) {
      $form[$tag][$subtag]['#collapsed'] = $settings['subframecollapsed'];
    } elseif (isset($settings['option']['wizard'])) {
      $form[$tag][$subtag]['#collapsed'] = $form_state['storage'][$tag]['Wizard'] !==
	$settings['option']['wizard'];
    }
    if (isset($settings['subframecollapsebtn'])) {
      $form[$tag][$subtag]['#collapsible'] = TRUE;
      if (!isset($form_state['storage'][$tag][$subtag]['framecollapse'])) {
    	$form_state['storage'][$tag][$subtag]['framecollapse'] = $settings['subframecollapsebtn'];
      }
      $form[$tag][$subtag]['framecollapse'] =
    	array('#type' => 'checkbox',
    	      '#title' => t('Collapsed frame'),
	      '#default_value' => $form_state['storage'][$tag][$subtag]['framecollapse'],
    	      '#ahah' => array('event' => 'change',
    			       'path' => ahah_helper_path(array($tag)),
    			       'wrapper' => $tag.'-wrapper'));
      $form[$tag][$subtag]['#collapsed'] = $form_state['storage'][$tag][$subtag]['framecollapse'];
    }
  }
  if (isset($settings['option']['readonly']) and $settings['option']['readonly']) {
    $disabled = TRUE;
  } else {
    $disabled = FALSE;
  }

  // Profile name when used for creating profiles
  if (!isset($settings['option']['skipprofilename']) or
      (isset($settings['option']['skipprofilename']) and !$settings['option']['skipprofilename'])) {
    if (!isset($form_state['storage'][$tag][$subtag]['ProfileName'])) {
      if (isset($settings['default']['ProfileName'])) {
	$form_state['storage'][$tag][$subtag]['ProfileName'] = $settings['default']['ProfileName'];
      } else {
	$form_state['storage'][$tag][$subtag]['ProfileName'] = '';
      }
    }
    $form[$tag][$subtag]['ProfileName'] =
      array('#type' => 'textfield',
	    '#title' => t('Profile name'),
	    '#maxlength' => 80,
	    '#disabled' => $disabled,
	    '#default_value' => $form_state['storage'][$tag][$subtag]['ProfileName']);
  }

  // Profile selection
  if (!isset($settings['option']['skipprofileselect']) or
      (isset($settings['option']['skipprofileselect']) and !$settings['option']['skipprofileselect'])) {
    $options = array();
    if (isset($settings['option']['custom']['key']) and isset($settings['option']['custom']['value'])) {
      $options[$settings['option']['custom']['key']] = $settings['option']['custom']['value'];
      $custom_key = $settings['option']['custom']['key'];
    } else {
      $options['Custom'] = t('Custom');
      $custom_key = 'Custom';
    }
    $enforcement = chpprop_retrieve_enforcement($user->uid);
    foreach ($enforcement as $enforcement_id => $enforcement) {
      $options[$enforcement_id] = $enforcement['data']['Alias'];
    }
    if (!isset($form_state['storage'][$tag][$subtag]['EnforcementId'])) {
      if (1 < count($options) and
	  (isset($settings['default']['EnforcementId']) and
	   in_array($settings['default']['EnforcementId'], array_keys($options)))) {
	$form_state['storage'][$tag][$subtag]['EnforcementId'] = $settings['default']['EnforcementId'];
      } else {
	$form_state['storage'][$tag][$subtag]['EnforcementId'] = key($options);
      }
    }
    $form[$tag][$subtag]['EnforcementId'] =
      array('#type' => 'select',
	    '#title' => t('Profile'),
	    '#options' => $options,
	    '#disabled' => $disabled,
	    '#default_value' => $form_state['storage'][$tag][$subtag]['EnforcementId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));

    if ($form_state['storage'][$tag][$subtag]['EnforcementId'] !== $custom_key) {
      return 0;
    }
  }

  // Ticketing
  if (!isset($form_state['storage'][$tag][$subtag]['ticketCheckbox'])) {
    if (isset($settings['default']['ticketCheckbox'])) {
      $form_state['storage'][$tag][$subtag]['ticketCheckbox'] =
	$settings['default']['ticketCheckbox'];
    } else {
      $form_state['storage'][$tag][$subtag]['ticketCheckbox'] = FALSE;
    }
  }
  $form[$tag][$subtag]['ticketCheckbox'] =
    array('#type' => 'checkbox',
	  '#title' => t('Ticketing'),
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['ticketCheckbox'],
  	  '#prefix' => '<table><tr><td rowspan="1">',
  	  '#suffix' => '</td> </tr></table>');

  // Towing
  if (!isset($form_state['storage'][$tag][$subtag]['towCheckbox'])) {
    if (isset($settings['default']['towCheckbox'])) {
      $form_state['storage'][$tag][$subtag]['towCheckbox'] =
	$settings['default']['towCheckbox'];
    } else {
      $form_state['storage'][$tag][$subtag]['towCheckbox'] = FALSE;
    }
  }
  $form[$tag][$subtag]['towCheckbox'] =
    array('#type' => 'checkbox',
	  '#title' => t('Towing'),
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['towCheckbox'],
  	  '#prefix' => '<table><tr><td rowspan="1">',
  	  '#suffix' => '</td> </tr></table>');

  // Booting
  if (!isset($form_state['storage'][$tag][$subtag]['bootCheckbox'])) {
    if (isset($settings['default']['bootCheckbox'])) {
      $form_state['storage'][$tag][$subtag]['bootCheckbox'] =
	$settings['default']['bootCheckbox'];
    } else {
      $form_state['storage'][$tag][$subtag]['bootCheckbox'] = FALSE;
    }
  }
  $form[$tag][$subtag]['bootCheckbox'] =
    array('#type' => 'checkbox',
	  '#title' => t('Booting'),
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['bootCheckbox'],
  	  '#prefix' => '<table><tr><td rowspan="1">',
  	  '#suffix' => '</td> </tr></table>');

  // Mobile
  if (!isset($form_state['storage'][$tag][$subtag]['mobileCheckbox'])) {
    if (isset($settings['default']['mobileCheckbox'])) {
      $form_state['storage'][$tag][$subtag]['mobileCheckbox'] =
	$settings['default']['mobileCheckbox'];
    } else {
      $form_state['storage'][$tag][$subtag]['mobileCheckbox'] = FALSE;
    }
  }
  $form[$tag][$subtag]['mobileCheckbox'] =
    array('#type' => 'checkbox',
	  '#title' => t('Mobile units'),
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['mobileCheckbox'],
  	  '#prefix' => '<table><tr><td rowspan="1">',
  	  '#suffix' => '</td>');

  if (!isset($form_state['storage'][$tag][$subtag]['mobileUnits'])) {
    if (isset($settings['default']['mobileUnits'])) {
      $form_state['storage'][$tag][$subtag]['mobileUnits'] =
	$settings['default']['mobileUnits'];
    } else {
      $form_state['storage'][$tag][$subtag]['mobileUnits'] = 5;
    }
  }
  $form[$tag][$subtag]['mobileUnits'] =
    array('#type' => 'textfield',
	  '#title' => t('Maximum mobile units'),
  	  '#maxlength' => 5,
	  '#size' => 5,
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['mobileUnits'],
  	  '#prefix' => '</td> <td valign="top">',
  	  '#suffix' => '</td> </tr></table>');

  return $form;
}



/*
 * $settings = array();
 * $settings['tag'] = 'ajax';
 * $settings['frame'] = TRUE;
 * $settings['frametitle'] = t('The title of the frame');
 * $settings['subtag'] = 'policy';
 * $settings['subframe'] = TRUE;
 * $settings['subframetitle'] = t('The title of the subframe');
 * $settings['subframecollapsible'] = TRUE;
 * $settings['subframecollapsed'] = TRUE;
 * $settings['subframecollapsebtn'] = FALSE; // Default value
 * $settings['option']['readonly'] = TRUE;
 * $settings['option']['wizard'] = 3;
 * $settings['option']['skipprofilename'] = TRUE;
 * $settings['option']['skipprofileselect'] = TRUE;
 * $settings['option']['custom'] = array('key' => $key, 'value' => $value);
 * $settings['default']['ProfileName'] = 'NAME';
 * $settings['default']['EnforcementId'] = $enforcement_id;
 * $settings['default']['lprCheckbox'] = $val;
 * $settings['default']['infoCheckbox'] = $val;
 * $settings['default']['padCheckbox'] = $val;
 *
 * Returns:
 *   1 = Error, the caller should just return the form immediately
 *   0 = No errors
 */
function chpprop_produce_equipment(&$form, &$form_state, $settings = array()) {
  global $user;
  $tag = 'ajax';
  if (isset($settings['tag'])) {
    $tag = $settings['tag'];
  }
  $subtag = 'equipment';
  if (isset($settings['subtag'])) {
    $subtag = $settings['subtag'];
  }
  if (!isset($form[$tag])) {
    $form[$tag] =
      array('#prefix' => '<div id="'.$tag.'-wrapper">',
	    '#suffix' => '</div>',
	    '#tree' => TRUE);
    if (isset($settings['frame']) and $settings['frame']) {
      $form[$tag]['#type'] = 'fieldset';
      if (isset($settings['frametitle']) and $settings['frametitle']) {
	$form[$tag]['#title'] = $settings['frametitle'];
      }
    }
  }
  if (!isset($form[$tag][$subtag])) {
    $form[$tag][$subtag] =
      array('#prefix' => '<div id="'.$tag.'-'.$subtag.'-wrapper">',
	    '#suffix' => '</div>',
	    '#tree' => TRUE);
  }
  if (isset($settings['subframe']) and $settings['subframe']) {
    $form[$tag][$subtag] =
      array('#type' => 'fieldset');
    if (isset($settings['subframetitle'])) {
      $form[$tag][$subtag]['#title'] = $settings['subframetitle'];
    }
    if (isset($settings['subframecollapsible']) and $settings['subframecollapsible']) {
      $form[$tag][$subtag]['#collapsible'] = $settings['subframecollapsible'];
    }
    if (isset($settings['subframecollapsed']) and $settings['subframecollapsed']) {
      $form[$tag][$subtag]['#collapsed'] = $settings['subframecollapsed'];
    } elseif (isset($settings['option']['wizard'])) {
      $form[$tag][$subtag]['#collapsed'] = $form_state['storage'][$tag]['Wizard'] !==
	$settings['option']['wizard'];
    }
    if (isset($settings['subframecollapsebtn'])) {
      $form[$tag][$subtag]['#collapsible'] = TRUE;
      if (!isset($form_state['storage'][$tag][$subtag]['framecollapse'])) {
    	$form_state['storage'][$tag][$subtag]['framecollapse'] = $settings['subframecollapsebtn'];
      }
      $form[$tag][$subtag]['framecollapse'] =
    	array('#type' => 'checkbox',
    	      '#title' => t('Collapsed frame'),
	      '#default_value' => $form_state['storage'][$tag][$subtag]['framecollapse'],
    	      '#ahah' => array('event' => 'click',
    			       'path' => ahah_helper_path(array($tag)),
    			       'wrapper' => $tag.'-wrapper'));
      $form[$tag][$subtag]['#collapsed'] = $form_state['storage'][$tag][$subtag]['framecollapse'];
    }
  }
  if (isset($settings['option']['readonly']) and $settings['option']['readonly']) {
    $disabled = TRUE;
  } else {
    $disabled = FALSE;
  }

  // Profile name when used for creating profiles
  if (!isset($settings['option']['skipprofilename']) or
      (isset($settings['option']['skipprofilename']) and !$settings['option']['skipprofilename'])) {
    if (!isset($form_state['storage'][$tag][$subtag]['ProfileName'])) {
      if (isset($settings['default']['ProfileName'])) {
	$form_state['storage'][$tag][$subtag]['ProfileName'] = $settings['default']['ProfileName'];
      } else {
	$form_state['storage'][$tag][$subtag]['ProfileName'] = '';
      }
    }
    $form[$tag][$subtag]['ProfileName'] =
      array('#type' => 'textfield',
	    '#title' => t('Profile name'),
	    '#maxlength' => 80,
	    '#disabled' => $disabled,
	    '#default_value' => $form_state['storage'][$tag][$subtag]['ProfileName']);
  }

  // Profile selection
  if (!isset($settings['option']['skipprofileselect']) or
      (isset($settings['option']['skipprofileselect']) and !$settings['option']['skipprofileselect'])) {
    $options = array();
    if (isset($settings['option']['custom']['key']) and isset($settings['option']['custom']['value'])) {
      $options[$settings['option']['custom']['key']] = $settings['option']['custom']['value'];
      $custom_key = $settings['option']['custom']['key'];
    } else {
      $options['Custom'] = t('Custom');
      $custom_key = 'Custom';
    }
    $equipment = chpprop_retrieve_equipment($user->uid);
    foreach ($equipment as $equipment_id => $value) {
      $options[$equipment_id] = $value['data']['Alias'];
    }
    if (!isset($form_state['storage'][$tag][$subtag]['EquipmentId'])) {
      if (1 < count($options) and
	  (isset($settings['default']['EquipmentId']) and
	   in_array($settings['default']['EquipmentId'], array_keys($options)))) {
	$form_state['storage'][$tag][$subtag]['EquipmentId'] = $settings['default']['EquipmentId'];
      } else {
	$form_state['storage'][$tag][$subtag]['EquipmentId'] = key($options);
      }
    }
    $form[$tag][$subtag]['EquipmentId'] =
      array('#type' => 'select',
	    '#title' => t('Profile'),
	    '#options' => $options,
	    '#disabled' => $disabled,
	    '#default_value' => $form_state['storage'][$tag][$subtag]['EquipmentId'],
	    '#ahah' => array('event' => 'change',
			     'path' => ahah_helper_path(array('ajax')),
			     'wrapper' => 'ajax-wrapper'));

    if ($form_state['storage'][$tag][$subtag]['EquipmentId'] !== $custom_key) {
      return 0;
    }
  }

  // LPR
  if (!isset($form_state['storage'][$tag][$subtag]['lprCheckbox'])) {
    if (isset($settings['default']['lprCheckbox'])) {
      $form_state['storage'][$tag][$subtag]['lprCheckbox'] =
	$settings['default']['lprCheckbox'];
    } else {
      $form_state['storage'][$tag][$subtag]['lprCheckbox'] = FALSE;
    }
  }
  $form[$tag][$subtag]['lprCheckbox'] =
    array('#type' => 'checkbox',
	  '#title' => t('LPR gate control'),
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['lprCheckbox'],
  	  '#prefix' => '<table><tr><td rowspan="1">',
  	  '#suffix' => '</td> </tr></table>');

  // Info Board
  if (!isset($form_state['storage'][$tag][$subtag]['infoCheckbox'])) {
    if (isset($settings['default']['infoCheckbox'])) {
      $form_state['storage'][$tag][$subtag]['infoCheckbox'] =
	$settings['default']['infoCheckbox'];
    } else {
      $form_state['storage'][$tag][$subtag]['infoCheckbox'] = FALSE;
    }
  }
  $form[$tag][$subtag]['infoCheckbox'] =
    array('#type' => 'checkbox',
	  '#title' => t('Info board'),
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['infoCheckbox'],
  	  '#prefix' => '<table><tr><td rowspan="1">',
  	  '#suffix' => '</td> </tr></table>');

  // Pay And Display
  if (!isset($form_state['storage'][$tag][$subtag]['padCheckbox'])) {
    if (isset($settings['default']['padCheckbox'])) {
      $form_state['storage'][$tag][$subtag]['padCheckbox'] =
	$settings['default']['padCheckbox'];
    } else {
      $form_state['storage'][$tag][$subtag]['padCheckbox'] = FALSE;
    }
  }
  $form[$tag][$subtag]['padCheckbox'] =
    array('#type' => 'checkbox',
	  '#title' => t('Pay And Display pay station'),
  	  '#disabled' => $disabled,
  	  '#default_value' => $form_state['storage'][$tag][$subtag]['padCheckbox'],
  	  '#prefix' => '<table><tr><td rowspan="1">',
  	  '#suffix' => '</td> </tr></table>');

  return $form;
}



/***
 * Function used for retrieving the valid relations list
 * based on user assignments and relations.
 */
function chpprop_retrieve_relations($uid) {
  $account = user_load(array('uid' => $uid));
  $relations = array();

  // As Admin, get all regardless of owner
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_get_all_relations() as $relation) {
      $relations[$relation['RelationsId']] =
	array('data' => $relation,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $relations;
  }

  foreach (chdbprop_cc_get_relations($uid) as $relation) {
    $relations[$relation['RelationsId']] =
      array('data' => $relation,
	    'readonly' => FALSE,
	    'source' => 'Relations');
  }
  return $relations;
}



/***
 * Function used for retrieving the valid company list
 * based on user assignments and relations.
 */
function chpprop_retrieve_companies($uid,
				    $filter_active = TRUE, $active = TRUE,
				    $filter_suspended = TRUE, $suspended = FALSE) {
  $account = user_load(array('uid' => $uid));
  $companies = array();

  // As Admin, get all regardless of owner
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_cc_get_admin_companies($filter_active, $active,
					     $filter_suspended, $suspended) as $company) {
      $companies[$company['CompanyId']] =
	array('data' => $company,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $companies;
  }

  // Via relations
  foreach (chdbprop_cc_get_companies($uid,
				     $filter_active, $active,
				     $filter_suspended, $suspended) as $company) {
    $companies[$company['CompanyId']] =
      array('data' => $company,
	    'readonly' => FALSE,
	    'source' => 'Relations');
  }
  return $companies;
}



/***
 * Function used for retrieving the valid region list
 */
function chpprop_retrieve_regions($uid,
				  $filter_active = TRUE, $active = TRUE,
				  $filter_suspended = TRUE, $suspended = FALSE) {
  $account = user_load(array('uid' => $uid));
  $regions = array();

  // As Admin, get all regardless of owner
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_cc_get_admin_regions($filter_active, $active,
					   $filter_suspended, $suspended) as $region) {
      $regions[$region['CompanyId']][$region['RegionId']] =
	array('data' => $region,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $regions;
  }

  // Via relations
  foreach (chdbprop_cc_get_regions($account->uid,
				   $filter_active, $active,
				   $filter_suspended, $suspended) as $region) {
      $regions[$region['CompanyId']][$region['RegionId']] =
	array('data' => $region,
	      'readonly' => TRUE,
	      'source' => 'Relations');
  }

  return $regions;
}



/***
 * Function used for retrieving the valid client list
 */
function chpprop_retrieve_clients($uid,
				  $filter_active = TRUE, $active = TRUE,
				  $filter_suspended = TRUE, $suspended = FALSE) {
  $account = user_load(array('uid' => $uid));
  $clients = array();

  // As Admin, get all regardless of owner
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_cc_get_admin_clients($filter_active, $active,
					   $filter_suspended, $suspended) as $client) {
      $clients[$client['CompanyId']][$client['RegionId']][$client['ClientId']] =
	array('data' => $client,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $clients;
  }

  // Via relations
  foreach (chdbprop_cc_get_clients($account->uid,
				   $filter_active, $active,
				   $filter_suspended, $suspended) as $client) {
    $clients[$client['CompanyId']][$client['RegionId']][$client['ClientId']] =
      array('data' => $client,
	    'readonly' => TRUE,
	    'source' => 'Relations');
  }

  return $clients;
}



/***
 * Function used for retrieving the valid property list
 * based on user assignments and relations.
 */
function chpprop_retrieve_properties($uid, $company_id = 'ANY', $policy_filters = array()) {
  $account = user_load(array('uid' => $uid));
  $properties = array();

  // As Admin, get all properties in a relation
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_cc_get_admin_properties($company_id, $policy_filters) as $property) {
      $properties[$property['PropertyId']] =
	array('data' => $property,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $properties;
  }

  // Via relations
  foreach (chdbprop_cc_get_properties($uid, $company_id, $policy_filters) as $property) {
    $properties[$property['PropertyId']] =
      array('data' => $property,
	    'readonly' => TRUE,
	    'source' => 'Relations');
  }
  //anai_dbg($properties, 'Props');
  return $properties;
}



/***
 * Function used for retrieving the valid policy list
 * based on user assignments.
 */
function chpprop_retrieve_policies($uid, $company_id = 'ANY',
				   $policy_filters = array(),
				   $enforcement_filters = array()) {
  $account = user_load(array('uid' => $uid));
  $policies = array();

  // As Admin, get all properties in a relation
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_cc_get_admin_policies($company_id,
					    $policy_filters,
					    $enforcement_filters) as $policy) {
      $policies[$policy['PropertyId']][$policy['LotId']] =
	array('data' => $policy,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $policies;
  }

  // Via relations
  foreach (chdbprop_cc_get_policies($uid, $company_id,
				    $policy_filters,
				    $enforcement_filters) as $policy) {
    $policies[$policy['PropertyId']][$policy['LotId']] =
      array('data' => $policy,
	    'readonly' => TRUE,
	    'source' => 'Relations');
  }
  //anai_dbg($properties, 'Props');
  return $policies;
}



/***
 * Retrieve profiles
 *   Retrieves profiles according to relations
 * Input:
 *   $uid
 * Output:
 *   An associative array of format:
 *     array[alias] = array(data => profile, readonly => TRUE|FALSE)
 */
function chpprop_retrieve_profiles($uid) {
  $account = user_load(array('uid' => $uid));
  $profiles = array();

  // As Admin, get all
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_get_all_profiles() as $profile) {
      $profiles[$profile['ProfileId']] =
	array('data' => $profile,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $profiles;
  }

  // Via relations
  foreach (chdbprop_cc_get_profiles($uid) as $profile) {
    $profiles[$profile['ProfileId']] =
      array('data' => $profile,
	    'readonly' => TRUE,
	    'source' => 'Relations');
  }

  return $profiles;
}



/***
 * Retrieve enforcement profiles
 *   Retrieves enforcement profiles according to relations
 * Input:
 *   $uid
 * Output:
 *   An associative array of format:
 *     array[alias] = array(data => profile, readonly => TRUE|FALSE)
 */
function chpprop_retrieve_enforcement($uid) {
  $account = user_load(array('uid' => $uid));
  $profiles = array();

  // As Admin, get all
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_get_all_enforcement() as $profile) {
      $profiles[$profile['ProfileId']] =
	array('data' => $profile,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $profiles;
  }

  // Via relations
  foreach (chdbprop_cc_get_enforcement($uid) as $profile) {
    $profiles[$profile['ProfileId']] =
      array('data' => $profile,
	    'readonly' => TRUE,
	    'source' => 'Relations');
  }

  return $profiles;
}



/***
 * Function used for retrieving the valid enforcer list
 */
function chpprop_retrieve_enforcers($uid, $company_id,
				    $property_id = 'ANY', $lot_id = 'ANY',
				    $filter_active = TRUE, $active = TRUE,
				    $filter_suspended = TRUE, $suspended = FALSE) {
  $account = user_load(array('uid' => $uid));
  $enforcers = array();

  // As Admin, get all regardless of owner
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_cc_get_admin_enforcers($company_id,
					     $property_id, $lot_id,
					     $filter_active, $active,
					     $filter_suspended, $suspended) as $enforcer) {
      $enforcers[$enforcer['EnforcerId']] =
	array('data' => $enforcer,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $enforcers;
  }

  // Via relations
  foreach (chdbprop_cc_get_enforcers($account->uid, $company_id,
				     $property_id, $lot_id,
				     $filter_active, $active,
				     $filter_suspended, $suspended) as $enforcer) {
    $enforcers[$enforcer['EnforcerId']] =
      array('data' => $enforcer,
	    'readonly' => TRUE,
	    'source' => 'Relations');
  }

  return $enforcers;
}



function chpprop_retrieve_enforcerprops($uid, $company_id, $enforcer_id) {
  $account = user_load(array('uid' => $uid));
  $props = array();
  //anai_dbg($account);

  // As Admin, get all properties in a relation
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_get_enforcerprops($company_id, $enforcer_id) as $prop) {
      $props[$prop['PropertyId']][$prop['LotId']] =
	array('data' => $prop,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $props;
  }

  // Via relations
  foreach (chdbprop_cc_get_enforcerprops($uid, $company_id, $enforcer_id) as $prop) {
    $props[$prop['PropertyId']][$prop['LotId']] =
      array('data' => $prop,
	    'readonly' => TRUE,
	    'source' => 'Relation');
  }

  return $props;
}



/***
 * Retrieve equipment profiles according to relations
 */
function chpprop_retrieve_equipment($uid) {
  $account = user_load(array('uid' => $uid));
  $profiles = array();

  // As Admin, get all
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_get_all_equipment() as $profile) {
      $profiles[$profile['ProfileId']] =
	array('data' => $profile,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $profiles;
  }

  // Via relations
  foreach (chdbprop_cc_get_equipment($uid) as $profile) {
    $profiles[$profile['ProfileId']] =
      array('data' => $profile,
	    'readonly' => TRUE,
	    'source' => 'Relations');
  }

  return $profiles;
}



/***
 * Functions to retrieve users in the different roles from a list of companies
 */
function chpprop_retrieve_users($uid, $company_id) {
  $users = array();
  $users['Manager'] = array();
  $users['Security'] = array();
  $users['Enforcer'] = array();
  $users['Tenant'] = array();

  $account = user_load(array('uid' => $uid));

  // As Admin, get all properties in a relation
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    $dbusers = array_merge(chdbprop_get_company_users($company_id),
			   chdbprop_get_company_users('ANY'));
    foreach ($dbusers as $dbuser) {
      $dbuid = $dbuser['UID'];
      $dbaccount = user_load(array('uid' => $dbuid));
      if ($dbaccount === FALSE) {
	// Automatic cleanup of discrapencies
	chdbprop_delete_user($dbuid, $account->uid);
	continue;
      }
      if (in_array('Manager', array_values($dbaccount->roles)) and $dbaccount->status) {
	if (!isset($users['Manager'][$dbuid])) {
	  $users['Manager'][$dbuid]['Name'] = $dbaccount->name;
	}
	$users['Manager'][$dbuid]['Rules'][] =
	  array('Id' => $dbuser['Id'],
		'CompanyId' => $dbuser['CompanyId'],
		'RegionId' => $dbuser['RegionId'],
		'ClientId' => $dbuser['ClientId'],
		'PropertyId' => $dbuser['PropertyId'],
		'LotId' => $dbuser['LotId']);
      }
      if (in_array('Enforcer', array_values($dbaccount->roles)) and $dbaccount->status) {
	if (!isset($users['Enforcer'][$dbuid])) {
	  $users['Enforcer'][$dbuid]['Name'] = $dbaccount->name;
	}
	$users['Enforcer'][$dbuid]['Rules'][] =
	  array('Id' => $dbuser['Id'],
		'CompanyId' => $dbuser['CompanyId'],
		'RegionId' => $dbuser['RegionId'],
		'ClientId' => $dbuser['ClientId'],
		'PropertyId' => $dbuser['PropertyId'],
		'LotId' => $dbuser['LotId']);
      }
      if (in_array('Security', array_values($dbaccount->roles)) and $dbaccount->status) {
	if (!isset($users['Security'][$dbuid])) {
	  $users['Security'][$dbuid]['Name'] = $dbaccount->name;
	}
	$users['Security'][$dbuid]['Rules'][] =
	  array('Id' => $dbuser['Id'],
		'CompanyId' => $dbuser['CompanyId'],
		'RegionId' => $dbuser['RegionId'],
		'ClientId' => $dbuser['ClientId'],
		'PropertyId' => $dbuser['PropertyId'],
		'LotId' => $dbuser['LotId']);
      }
      if (in_array('Tenant', array_values($dbaccount->roles)) and $dbaccount->status) {
	if (!isset($users['Tenant'][$dbuid])) {
	  $users['Tenant'][$dbuid]['Name'] = $dbaccount->name;
	}
	$users['Tenant'][$dbuid]['Rules'][] =
	  array('Id' => $dbuser['Id'],
		'CompanyId' => $dbuser['CompanyId'],
		'RegionId' => $dbuser['RegionId'],
		'ClientId' => $dbuser['ClientId'],
		'PropertyId' => $dbuser['PropertyId'],
		'LotId' => $dbuser['LotId']);
      }
    }
    return $users;
  }

  // Via relations
  $dbusers = array_merge(chdbprop_get_company_user($company_id, $uid),
			 chdbprop_get_company_user('ANY', $uid));
  foreach ($dbusers as $dbuser) {
    if (in_array('Manager', array_values($account->roles)) and $account->status) {
      if (!isset($users['Manager'][$uid])) {
	$users['Manager'][$uid]['Name'] = $account->name;
      }
      $users['Manager'][$uid]['Rules'][] =
	array('Id' => $dbuser['Id'],
	      'CompanyId' => $dbuser['CompanyId'],
	      'RegionId' => $dbuser['RegionId'],
	      'ClientId' => $dbuser['ClientId'],
	      'PropertyId' => $dbuser['PropertyId'],
	      'LotId' => $dbuser['LotId']);
    }
    if (in_array('Enforcer', array_values($account->roles)) and $account->status) {
      if (!isset($users['Enforcer'][$uid])) {
	$users['Enforcer'][$uid]['Name'] = $account->name;
      }
      $users['Enforcer'][$uid]['Rules'][] =
	array('Id' => $dbuser['Id'],
	      'CompanyId' => $dbuser['CompanyId'],
	      'RegionId' => $dbuser['RegionId'],
	      'ClientId' => $dbuser['ClientId'],
	      'PropertyId' => $dbuser['PropertyId'],
	      'LotId' => $dbuser['LotId']);
    }
    if (in_array('Security', array_values($account->roles)) and $account->status) {
      if (!isset($users['Security'][$uid])) {
	$users['Security'][$uid]['Name'] = $account->name;
      }
      $users['Security'][$uid]['Rules'][] =
	array('Id' => $dbuser['Id'],
	      'CompanyId' => $dbuser['CompanyId'],
	      'RegionId' => $dbuser['RegionId'],
	      'ClientId' => $dbuser['ClientId'],
	      'PropertyId' => $dbuser['PropertyId'],
	      'LotId' => $dbuser['LotId']);
    }
    if (in_array('Tenant', array_values($account->roles)) and $account->status) {
      if (!isset($users['Tenant'][$uid])) {
	$users['Tenant'][$uid]['Name'] = $account->name;
      }
      $users['Tenant'][$uid]['Rules'][] =
	array('Id' => $dbuser['Id'],
	      'CompanyId' => $dbuser['CompanyId'],
	      'RegionId' => $dbuser['RegionId'],
	      'ClientId' => $dbuser['ClientId'],
	      'PropertyId' => $dbuser['PropertyId'],
	      'LotId' => $dbuser['LotId']);
    }
  }
  return $users;
}



/***
 * Function used for retrieving the valid agent list
 * based on user assignments.
 */
function chpprop_retrieve_agents($uid, $company_id, $app_id,
				 $property_id = 'ANY', $lot_id = 'ANY') {
  $agents = array();
  $account = user_load(array('uid' => $uid));

  // As Admin, get all properties in a relation
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {

    foreach (chdbprop_cc_get_admin_agents($company_id, $app_id, $property_id, $lot_id) as $agent) {
      $agents[$agent['AgentId']] =
	array('data' => $agent,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $agents;
  }

  foreach (chdbprop_cc_get_agents($uid, $company_id, $app_id, $property_id, $lot_id) as $agent) {
    $agents[$agent['AgentId']] =
      array('data' => $agent,
	    'readonly' => TRUE,
	    'source' => 'Relations');
  }

  return $agents;
}



function chpprop_retrieve_agentprops($uid, $company_id, $agent_id) {
  $account = user_load(array('uid' => $uid));
  $props = array();

  // As Admin, get all properties in a relation
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_get_agentprops($company_id, $agent_id) as $prop) {
      $props[$prop['PropertyId']][$prop['LotId']] =
	array('data' => $prop,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $props;
  }

  // Via relations
  foreach (chdbprop_cc_get_agentprops($uid, $company_id, $agent_id) as $prop) {
    $props[$prop['PropertyId']][$prop['LotId']] =
      array('data' => $prop,
	    'readonly' => TRUE,
	    'source' => 'Relation');
  }

  return $props;
}



/***
 * Function used for retrieving the valid patroller list
 * based on user assignments.
 */
function chpprop_retrieve_patrollers($uid, $company_id,
				     $property_id = 'ANY', $lot_id = 'ANY',
				     $enforcer_id = NULL) {
  $patrollers = array();
  $account = user_load(array('uid' => $uid));

  // As Admin, get all properties in a relation
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {

    foreach (chdbprop_cc_get_admin_patrollers($company_id, $property_id,
					      $lot_id, $enforcer_id) as $patroller) {
      $patrollers[$patroller['PatrollerId']] =
	array('data' => $patroller,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $patrollers;
  }

  foreach (chdbprop_cc_get_patrollers($uid, $company_id, $property_id,
				      $lot_id, $enforcer_id) as $patroller) {
    $patrollers[$patroller['PatrollerId']] =
      array('data' => $patroller,
	    'readonly' => TRUE,
	    'source' => 'Relations');
  }

  return $patrollers;
}



function chpprop_retrieve_patrollerprops($uid, $company_id, $patroller_id) {
  $account = user_load(array('uid' => $uid));
  $props = array();

  // As Admin, get all properties in a relation
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_get_patrollerprops($company_id, $patroller_id) as $prop) {
      $props[$prop['PropertyId']][$prop['LotId']][$prop['PatrollerId']] =
	array('data' => $prop,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $props;
  }
  return $props;

  // Via relations
  foreach (chdbprop_cc_get_patrollerprops($uid, $company_id, $patroller_id) as $prop) {
    $props[$prop['PropertyId']][$prop['LotId']][$prop['PatrollerId']] =
      array('data' => $prop,
	    'readonly' => TRUE,
	    'source' => 'Relation');
  }

  return $props;
}



/***
 * Function used for retrieving the valid agent list
 * based on user assignments.
 */
function chpprop_retrieve_lanes($uid, $company_id, $property_id, $lot_id,
				$lot_type = 'ANY', $lot_class = 'ANY') {
  $lanes = array();
  $account = user_load(array('uid' => $uid));

  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {

    foreach (chdbprop_cc_get_lanes_admin($company_id, $property_id, $lot_id,
					 $lot_type, $lot_class) as $lane) {
      $lanes[$lane['LaneId']] =
	array('data' => $lane,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $lanes;
  }

  foreach (chdbprop_cc_get_lanes($company_id, $property_id, $lot_id,
				 $lot_type, $lot_class) as $lane) {
    $lanes[$lane['LaneId']] =
      array('data' => $lane,
	    'readonly' => TRUE,
	    'source' => 'Relations');
  }

  return $lanes;
}



function chpprop_retrieve_laneprops($uid, $lane_id) {
  $account = user_load(array('uid' => $uid));
  $props = array();

  // As Admin, get all properties in a relation
  if ((in_array('Admin', array_values($account->roles)) and $account->status) or
      $account->uid == 1) {
    foreach (chdbprop_get_laneprops($lane_id) as $prop) {
      $props[$prop['PropertyId']][$prop['LotId']] =
	array('data' => $prop,
	      'readonly' => FALSE,
	      'source' => 'Admin');
    }
    return $props;
  }

  // Via relations
  foreach (chdbprop_cc_get_laneprops($uid, $lane_id) as $prop) {
    $props[$prop['PropertyId']][$prop['LotId']] =
      array('data' => $prop,
	    'readonly' => TRUE,
	    'source' => 'Relation');
  }

  return $props;
}



function chpprop_property_allows_unlimited_validity($profile) {
  if (($profile['LongPolicy'] or $profile['LongShortPolicy']) and
      TENANT_VALIDITY_UNLIMITED == $profile['ValidityPolicy']) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_property_allows_annual_validity($profile) {
  if (($profile['LongPolicy'] or $profile['LongShortPolicy']) and
      TENANT_VALIDITY_ANNUAL == $profile['ValidityPolicy']) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_property_allows_month_validity($profile) {
  if (($profile['LongPolicy'] or $profile['LongShortPolicy']) and
      TENANT_VALIDITY_MONTH == $profile['ValidityPolicy']) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_property_allows_validity_days($profile) {
  if (($profile['LongPolicy'] or $profile['LongShortPolicy']) and
      TENANT_VALIDITY_DAYS == $profile['ValidityPolicy']) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_property_validity_days($profile) {
  return $profile['ValidityDays'];
}

function chpprop_property_has_permit_expiry_at_start($profile) {
  if (($profile['ShortPolicy'] or $profile['LongShortPolicy']) and
      VISITOR_START_TIME == $profile['ExpirePolicy']) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_property_has_permit_expiry_at_stop($profile) {
  if (($profile['ShortPolicy'] or $profile['LongShortPolicy']) and
      VISITOR_STOP_TIME == $profile['ExpirePolicy']) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_property_has_permit_expiry_at_overnight_start($profile) {
  if (($profile['ShortPolicy'] or $profile['LongShortPolicy']) and
      VISITOR_OVERNIGHT_START_TIME == $profile['ExpirePolicy']) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_property_has_permit_expiry_at_overnight_stop($profile) {
  if (($profile['ShortPolicy'] or $profile['LongShortPolicy']) and
      VISITOR_OVERNIGHT_STOP_TIME == $profile['ExpirePolicy']) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_property_demands_permit_expiry_by_days($profile) {
  if (($profile['ShortPolicy'] or $profile['LongShortPolicy']) and
      VISITOR_EXPIRY_DAYS == $profile['ExpireDuration']) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_property_demands_permit_expiry_select($profile) {
  if (($profile['ShortPolicy'] or $profile['LongShortPolicy']) and
      VISITOR_EXPIRY_SELECT == $profile['ExpireDuration']) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_property_allow_regular_permit($profile) {
  if ($profile['ShortPolicy'] or $profile['LongShortPolicy']) {
    if (VISITOR_REGULAR == $profile['ExtensionPolicy'] or
	VISITOR_REGULAR_OVERNIGHT == $profile['ExtensionPolicy'] or
	VISITOR_REGULAR_CONTAINER == $profile['ExtensionPolicy'] or
	VISITOR_REGULAR_OVERNIGHT_CONTAINER == $profile['ExtensionPolicy']) {
      return TRUE;
    }
  } elseif ($profile['LongPolicy'] or $profile['LongShortPolicy']) {
    if (TENANT_REGULAR == $profile['PermitPolicy'] or
	TENANT_REGULAR_CONTAINER == $profile['PermitPolicy']) {
      return TRUE;
    }
  }
  return FALSE;
}

function chpprop_property_allow_overnight_permit($profile) {
  if ($profile['ShortPolicy'] or $profile['LongShortPolicy']) {
    if (VISITOR_REGULAR_OVERNIGHT == $profile['ExtensionPolicy'] or
	VISITOR_REGULAR_OVERNIGHT_CONTAINER == $profile['ExtensionPolicy']) {
      return TRUE;
    }
  }
  return FALSE;
}

function chpprop_property_allow_container($profile) {
  if ($profile['ShortPolicy'] or $profile['LongShortPolicy']) {
    if (VISITOR_REGULAR_CONTAINER == $profile['ExtensionPolicy'] or
	VISITOR_REGULAR_OVERNIGHT_CONTAINER == $profile['ExtensionPolicy'] or
	VISITOR_CONTAINER == $profile['ExtensionPolicy']) {
      return TRUE;
    }
  } elseif ($profile['LongPolicy'] or $profile['LongShortPolicy']) {
    if (TENANT_CONTAINER == $profile['PermitPolicy']) {
      return TRUE;
    }
  }
  return FALSE;
}

function chpprop_property_allow_hotel_permit($profile) {
  if ($profile['ShortPolicy'] or $profile['LongShortPolicy']) {
    if (VISITOR_HOTEL == $profile['ExtensionPolicy']) {
      return TRUE;
    }
  }
  return FALSE;
}

function chpprop_property_allow_loading_dock($profile) {
  if ($profile['ShortPolicy'] or $profile['LongShortPolicy']) {
    if (VISITOR_LOADING == $profile['ExtensionPolicy']) {
      return TRUE;
    }
  }
  return FALSE;
}

function chpprop_property_uses_renew_retention($profile) {
  if (($profile['ShortPolicy'] or $profile['LongShortPolicy']) and
      $profile['RenewRetention']) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_property_renew_retention_days($profile) {
  if (chpprop_property_uses_renew_retention($profile)) {
    return $profile['RenewRetentionDays'];
  }
  return 0;
}

function chpprop_property_allows_valet_messenger($profile) {
  if ($profile['ValetPolicy'] and $profile['ValetMessenger']) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_property_disables_valet_checkout($profile) {
  if ($profile['ValetPolicy'] and
      in_array('DisableCheckout', explode(',',$profile['ValetMods']))) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_property_disables_valet_codes($profile) {
  if ($profile['ValetPolicy'] and
      in_array('DisableCodes', explode(',',$profile['ValetMods']))) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_property_allows_valet($profile) {
  if ($profile['ValetPolicy']) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_property_allowed_valet_mobiles($profile) {
  if ($profile['ValetPolicy'] and
      MOBILES_ALLOWED_REGULAR == $profile['ValUnitsPolicy']) {
    return $profile['ValetUnits'];
  }
  return 0;
}

function chpprop_property_allowed_enforcement_mobiles($enforcement) {
  if (!empty($enforcement) and
      0 < $enforcement['MobilePolicy'] and
      ($enforcement['Ticketing'] or $enforcement['Booting'] or $enforcement['Towing'])) {
    return $enforcement['MobileUnits'];
  }
  return 0;
}

function chpprop_property_allowed_equipment_mobiles($equipment) {
  if (!empty($equipment) and
      ($equipment['LPR'] or $equipment['InfoBoard'] or $equipment['PAD'])) {
    return 1;
  }
  return 0;
}

function chpprop_property_allow_shared_permits($profile) {
  if ((($profile['LongPolicy'] or $profile['LongShortPolicy']) and
       $profile['PermitShared']) or
      (($profile['ShortPolicy'] or $profile['LongShortPolicy']) and 
       $profile['ExtensionShared'])) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_uses_unmanaged_stalls($profile) {
  if ((($profile['LongPolicy'] or $profile['LongShortPolicy']) and
       $profile['LongStallPolicy'] == TENANT_STALL_UNMANAGED) or
      (($profile['ShortPolicy'] or $profile['LongShortPolicy']) and 
       $profile['ShortStallPolicy'] == VISITOR_STALL_UNMANAGED)) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_long_managed_stalls($profile) {
  if ((($profile['LongPolicy'] or $profile['LongShortPolicy']) and
       $profile['LongStallPolicy'] == TENANT_STALL_MANAGED_STAL)) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_long_managed_stall_totals($profile) {
  if ((($profile['LongPolicy'] or $profile['LongShortPolicy']) and
       $profile['LongStallPolicy'] == TENANT_STALL_MANAGED_TOTAL)) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_short_managed_stalls($profile) {
  if (($profile['ShortPolicy'] or $profile['LongShortPolicy']) and 
      $profile['ShortStallPolicy'] == VISITOR_STALL_MANAGED_STALL) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_short_managed_stall_totals($profile) {
  if (($profile['ShortPolicy'] or $profile['LongShortPolicy']) and 
      $profile['ShortStallPolicy'] == VISITOR_STALL_MANAGED_TOTAL) {
    return TRUE;
  }
  return FALSE;
}

function chpprop_valet_managed_stalls($profile) {
  // TBD
  return FALSE;
}






