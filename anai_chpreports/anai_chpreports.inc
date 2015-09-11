<?php
require_once("anai_chpreports_db.inc");


function chpreport_type_name($tag) {
  switch ($tag) {
  case 'PermitActDeact':
    return t('Permit Activation & Deactivation Report');
  case 'PermitContainerUsage':
    return t('Permit Container Usage Report');
  case 'ValidationUsage':
    return t('Validation Usage Report');
  case 'OccuGroup':
    return t('Occupancy Group Account Report');
  case 'OccuMember':
    return t('Occupancy Individual Account Report');
  case 'OccuPublic':
    return t('Occupancy Public Parking Report');
  case 'OccuCompliancy':
    return t('Occupancy Compliancy Report');
  case 'MeterCreditCard':
    return t('Meter Credit Card Report');
  case 'MeterCash':
    return t('Meter Cash Report');
  case 'MeterRevenue':
    return t('Meter Revenue Report');
  case 'MeterMerchantcode': // Tore 131225: Added
    return t('Meter Merchant Code Report');
  case 'LaneCount':
    return t('Lane Vehicle Count Report');
  }
  return t('Unknown');
}



function chpreport_get_permit_classes() {
  $options = array();
  $options['Transient'] = chpperm_registration_name('Transient');
  $options['Group'] = chpperm_registration_name('Group');
  $options['Member'] = chpperm_registration_name('Member');
  $options['Hotel'] = chpperm_registration_name('Hotel');
  $options['Validation'] = chpperm_registration_name('Validation');
  $options['Dock'] = chpperm_registration_name('Dock');
  return $options;
}



function chpreport_get_compliancy_name($tag) {
  switch ($tag) {
  case 'Compliant':
    return t('Compliant');
  case 'NonCompliant':
    return t('Non-compliant');
  case 'GraceExempt':
    return t('Grace exempt');
  }
  return t('Unknown');
}



function chpreport_create_header($report) {
  global $user;

  // Top title
  $title = chpreport_type_name($report['ReportType']);
  $html = '<center><h1>'.$title.'</h1></center><p>';

  // Sub title
  if ($report['CompanyId'] === 'ANY') {
    $location = t('All companies').'<br>';
  } else {
    $company = chdbprop_get_company($report['CompanyId']);
    $location = decode_entities($company['Alias']).'<br>';
  }

  if ($report['PropertyId'] === 'ANY') {
    $location .= t('All properties').'<br>';
  } else {
    $property = chdbprop_get_property($report['PropertyId']);
    $location .=
      decode_entities($property['Alias']).', '.
      decode_entities($property['Street']).', '.
      decode_entities($property['City']);
  }
  $html .= '<center><h2>'.$location.'</h2></center><p>';

  // Sub-sub title
  if ($report['LotId'] === 'ANY') {
    $location = t('All lots');
  } else {
    $policy = chdbprop_get_policy($report['PropertyId'], $report['LotId']);
    $location = t('Lot: LOT', array('LOT' => decode_entities($policy['Alias'])));
  }
  if ($report['LotId'] === 'ANY') {
    $location .= '<br>';
    if ($report['LotType'] !== 'ANY') {
      $lottypeopt = array();
      foreach (chdbadmin_get_lottypeopt() as $opt) {
	$lottypeopt[$opt['LottypeId']] = decode_entities($opt['Alias']);
      }
      $location .= t('Lot type TYPE', array('TYPE' => $lottypeopt[$report['LotType']]));
    } else {
      $location .= t('Any type');
    }
    $location .= ', ';
    if ($report['LotClass'] !== 'ANY') {
      $lotclassopt = array();
      foreach (chdbadmin_get_lotclassopt() as $opt) {
	$lotclassopt[$opt['LotclassId']] = decode_entities($opt['Alias']);
      }
      $location .= t('lot classification CLASS', array('CLASS' => $lotclassopt[$report['LotClass']]));
    } else {
      $location .= t('any classification');
    }
  }
  $html .= '<center><h3>'.$location.'</h3></center><p>';

  // Sub-sub title
  if ($report['Category'] === 'Permit') {
    if ($report['PermitClass'] === 'ANY') {
      $class = t('All classes');
    } else {
      $class = t('Permit class: CLASS',
		 array('CLASS' => chpperm_registration_name($report['PermitClass'])));
      $class .= 's'; // Plural :)
    }
    $html .= '<center><h4>'.$class.'</h4></center><p>';
  } elseif ($report['Category'] === 'Validation') {
    if ($report['ClientId'] === 'ANY') {
      $occu = t('All clients');
    } else {
      $client = chdbprop_get_client($report['CompanyId'], $report['ClientId']);
      $occu = t('Client: CLIENT', array('CLIENT' => decode_entities($client['Alias'])));
    }
    $occu .= '<br>';
    if ($report['GroupId'] === 'ANY') {
      $occu .= t('All group accountss');
    } else {
      $groupdef = chdbperm_get_groupdef($report['CompanyId'], $report['GroupId']);
      $occu .= t('Group: GROUP', array('GROUP' => decode_entities($groupdef['Alias'])));
    }
    $occu .= '<br>';
    if ($report['VisitType'] === 'Commercial') {
      $occu .= t('Commercial validations, ');
    } elseif ($report['VisitType'] === 'Residential') {
      $occu .= t('Residential validations, ');
    }
    if ($report['Length'] === 'ANY') {
      $occu .= t('any lengths, ');
    } else {
      $occu .= chpperm_validation_length_name($report['Length'], $found);
      $occu .= t(' lengths, ');
    }
    if ($report['Benefit'] === 'ANY') {
      $occu .= t('any benefits');
    } else {
      $occu .= chpperm_validation_benefit_name($report['Benefit'], $found);
      $occu .= t(' benefits');
    }
    $html .= '<center><h4>'.$occu.'</h4></center><p>';
  } elseif ($report['Category'] === 'Occupancy') {
    if ($report['ReportType'] === 'OccuGroup') {
      if ($report['ClientId'] === 'ANY') {
	$occu = t('All clients');
      } else {
	$client = chdbprop_get_client($report['CompanyId'], $report['ClientId']);
	$occu = t('Client: CLIENT', array('CLIENT' => decode_entities($client['Alias'])));
      }
      $occu .= '<br>';
      if ($report['GroupId'] === 'ANY') {
	$occu .= t('All group accountss');
      } else {
	$groupdef = chdbperm_get_groupdef($report['CompanyId'], $report['GroupId']);
	$occu .= t('Group: GROUP', array('GROUP' => decode_entities($groupdef['Alias'])));
      }
      $html .= '<center><h4>'.$occu.'</h4></center><p>';
    } elseif ($report['ReportType'] === 'OccuMember') {
      if ($report['ClientId'] === 'ANY') {
	$occu = t('All clients');
      } else {
	$client = chdbprop_get_client($report['CompanyId'], $report['ClientId']);
	$occu = t('Client: CLIENT', array('CLIENT' => decode_entities($client['Alias'])));
      }
      $occu .= '<br>';
      if ($report['GroupId'] === 'ANY') {
	$occu .= t('All individual accounts');
      } else {
	$groupdef = chdbperm_get_groupdef($report['CompanyId'], $report['GroupId']);
	$occu .= t('Individual: GROUP', array('GROUP' => decode_entities($groupdef['Alias'])));
      }
      $html .= '<center><h4>'.$occu.'</h4></center><p>';
    } elseif ($report['ReportType'] === 'OccuCompliancy') {
      if ($report['Compliancy'] === 'ANY') {
	$occu = t('All compliancies');
      } else {
	$occu = t('Compliancy: COMP',
		  array('COMP' => chpreport_get_compliancy_name($report['Compliancy'])));
      }
      $html .= '<center><h4>'.$occu.'</h4></center><p>';
    }
  } elseif ($report['Category'] === 'Meter') {
    if ($report['AgentId'] === 'ANY') {
      $meter = t('All meters');
    } else {
      $tmp = chdbccpark_get_system($report['AgentId']);
      $meter = t('Meter: METER', array('METER' => decode_entities($tmp['Alias'])));
    }
    // Tore 131225: Added
    if ($report['ReportType'] === 'MeterMerchantcode') {
      $meter .= '<br>';
      if ($report['MerchantCode'] === 'ANY') {
	$meter .= t('All merchant codes');
      } else {
	$meter .= t('Merchant Code: CODE',
		    array('CODE' => decode_entities($report['MerchantCode'])));
      }
    }
    $html .= '<center><h4>'.$meter.'</h4></center><p>';
  } elseif ($report['Category'] === 'Lane') {
    if ($report['LaneId'] === 'ANY') {
      $meter = t('All lanes');
    } else {
      $tmp = chdbprop_get_lane($report['PropertyId'], $report['LaneId']);
      $meter = t('Lane: STR', array('STR' => decode_entities($tmp['Alias'])));
    }
    $html .= '<center><h4>'.$meter.'</h4></center><p>';
  }

  $html .= '<table><tr>';
  $html .= '<td align="left" class="noborder">';
  $html .= 'Start date: <b>'.$report['StartTime'].'</b><br>End date: <b>'.$report['StopTime'].'</b>';
  $html .= '</td><td align="right" class="noborder">';
  $html .= 'Report created by: <b>'.$user->name.'</b><br>';
  $html .= 'on <b><script language="JavaScript">window.document.write(new Date())</script></b>';
  $html .= '</td></tr></table>';
  return $html;
}



// This function is called recursive, so additions only!!!
function chpreport_retrieve_category_type($profile, $equipment, $enforcement, &$reports) {
  $lottypeopt = array();
  foreach (chdbadmin_get_lottypeopt() as $opt) {
    $lottypeopt[$opt['LottypeId']] = decode_entities($opt['Alias']);
  }
  $lotclassopt = array();
  foreach (chdbadmin_get_lotclassopt() as $opt) {
    $lotclassopt[$opt['LotclassId']] = decode_entities($opt['Alias']);
  }

  if (!isset($reports['Reports'])) {
    $reports['Reports'] = array();
  }
  if (!isset($reports['LotType'])) {
    $reports['LotType'] = array();
  }
  if (!isset($reports['LotClass'])) {
    $reports['LotClass'] = array();
  }
  // Profile dependencies
  if (!empty($profile)) {
    if (user_access('anai chp permit activation deactivation report') and
	($profile['LongPolicy'] or
	 $profile['ShortPolicy'] or
	 $profile['LongShortPolicy'])) {
      $reports['Reports']['Permit']['PermitActDeact'] =
	chpreport_type_name('PermitActDeact');
      if (($profile['LongPolicy'] or $profile['LongShortPolicy']) and
	  !empty($profile['LongTypeId'])) {
	if (!isset($reports['LotType'][$profile['LongTypeId']]) and
	    isset($lottypeopt[$profile['LongTypeId']])) {
	  $reports['LotType'][$profile['LongTypeId']] = $lottypeopt[$profile['LongTypeId']];
	}
      } elseif (($profile['ShortPolicy'] or $profile['LongShortPolicy']) and
		!empty($profile['ShortTypeId'])) {
	if (!isset($reports['LotType'][$profile['ShortTypeId']]) and
	    isset($lottypeopt[$profile['ShortTypeId']])) {
	  $reports['LotType'][$profile['ShortTypeId']] = $lottypeopt[$profile['ShortTypeId']];
	}
      }
      if (($profile['LongPolicy'] or $profile['LongShortPolicy']) and
	  !empty($profile['LongClassId'])) {
	if (!isset($reports['LotClass'][$profile['LongClassId']]) and
	    isset($lotclassopt[$profile['LongClassId']])) {
	  $reports['LotClass'][$profile['LongClassId']] = $lotclassopt[$profile['LongClassId']];
	}
      } elseif (($profile['ShortPolicy'] or $profile['LongShortPolicy']) and
		!empty($profile['ShortClassId'])) {
	if (!isset($reports['LotClass'][$profile['ShortClassId']]) and
	    isset($lotclassopt[$profile['ShortClassId']])) {
	  $reports['LotClass'][$profile['ShortClassId']] = $lotclassopt[$profile['ShortClassId']];
	}
      }
    }
    if (user_access('anai chp permit container usage report') and
	($profile['LongPolicy'] or
	 $profile['ShortPolicy'] or
	 $profile['LongShortPolicy'])) {
      $reports['Reports']['Permit']['PermitContainerUsage'] =
	chpreport_type_name('PermitContainerUsage');
      if (($profile['LongPolicy'] or $profile['LongShortPolicy']) and
	  !empty($profile['LongTypeId'])) {
	if (!isset($reports['LotType'][$profile['LongTypeId']])) {
	  $reports['LotType'][$profile['LongTypeId']] = $lottypeopt[$profile['LongTypeId']];
	}
      } elseif (($profile['ShortPolicy'] or $profile['LongShortPolicy']) and
		!empty($profile['ShortTypeId'])) {
	if (!isset($reports['LotType'][$profile['ShortTypeId']])) {
	  $reports['LotType'][$profile['ShortTypeId']] = $lottypeopt[$profile['LongTypeId']];
	}
      }
      if (($profile['LongPolicy'] or $profile['LongShortPolicy']) and
	  !empty($profile['LongClassId'])) {
	if (!isset($reports['LotClass'][$profile['LongClassId']])) {
	  $reports['LotClass'][$profile['LongClassId']] = $lotclassopt[$profile['LongClassId']];
	}
      } elseif (($profile['ShortPolicy'] or $profile['LongShortPolicy']) and
		!empty($profile['ShortClassId'])) {
	if (!isset($reports['LotClass'][$profile['ShortClassId']])) {
	  $reports['LotClass'][$profile['ShortClassId']] = $lotclassopt[$profile['LongClassId']];
	}
      }
    }
  }

  if (user_access('anai chp validation type report')) {
    $reports['Reports']['Validation']['ValidationUsage'] =
      chpreport_type_name('ValidationUsage');
  }

  // Equipment dependencies
  if (!empty($equipment)) {
    if (user_access('anai chp occupancy group report') and
	(isset($equipment['LPR']) and $equipment['LPR'])) {
      $reports['Reports']['Occupancy']['OccuGroup'] =
	chpreport_type_name('OccuGroup');
    }
    if (user_access('anai chp occupancy member report') and
	(isset($equipment['LPR']) and $equipment['LPR'])) {
      $reports['Reports']['Occupancy']['OccuMember'] =
	chpreport_type_name('OccuMember');
    }
    if (user_access('anai chp occupancy public report') and
	((isset($equipment['PAD']) and $equipment['PAD']) or
	 (isset($equipment['LPR']) and $equipment['LPR']))) {
      $reports['Reports']['Occupancy']['OccuPublic'] =
	chpreport_type_name('OccuPublic');
    }
    if (user_access('anai chp occupancy compliancy report') and
	((isset($equipment['PAD']) and $equipment['PAD']) or
	 (isset($equipment['LPR']) and $equipment['LPR']))) {
      $reports['Reports']['Occupancy']['OccuCompliancy'] =
	chpreport_type_name('OccuCompliancy');
    }
    if (user_access('anai chp meter creditcard report') and
	(isset($equipment['PAD']) and $equipment['PAD'])) {
      $reports['Reports']['Meter']['MeterCreditCard'] =
	chpreport_type_name('MeterCreditCard');
    }
    if (user_access('anai chp meter cash report') and
	(isset($equipment['PAD']) and $equipment['PAD'])) {
      $reports['Reports']['Meter']['MeterCash'] =
	chpreport_type_name('MeterCash');
    }
    if (user_access('anai chp meter revenue report') and
	(isset($equipment['PAD']) and $equipment['PAD'])) {
      $reports['Reports']['Meter']['MeterRevenue'] =
	chpreport_type_name('MeterRevenue');
    }
    // Tore 131225: Added
    if (user_access('anai chp meter merchantcode report') and
	(isset($equipment['PAD']) and $equipment['PAD'])) {
      $reports['Reports']['Meter']['MeterMerchantcode'] =
	chpreport_type_name('MeterMerchantcode');
    }
    if (user_access('anai chp lane count report') and
	(isset($equipment['LPR']) and $equipment['LPR'])) {
      $reports['Reports']['Lane']['LaneCount'] =
	chpreport_type_name('LaneCount');
    }
  }

  // Enforcement dependencies
  if (!empty($enforcement)) {
  }
}



