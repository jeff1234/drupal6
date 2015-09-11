<?php
// $Id$
require_once("anai_chpanalytics_db.inc");



function chpan_prepare_permit_predictions($company_id, $property_id, $policies) {
  // Prepare aggregate (LotId='All') permit predictions if needed
  $predictions =
    chdbperm_get_predictions
    ($company_id, $property_id, TRUE, 'All');
  if (sizeof($predictions) == 0) {
    if (chdbperm_insert_prediction($company_id, $property_id, 'All')) {
      drupal_set_message(t('Failed to insert permit predictions'), 'error');
    }
  }

  // Prepare property permit predictions if needed
  foreach ($policies as $policy) {
    $predictions =
      chdbperm_get_predictions
      ($company_id, $property_id, TRUE, $policy['data']['LotId']);
    if (sizeof($predictions) == 0) {
      if (chdbperm_insert_prediction
	  ($company_id, $property_id, $policy['data']['LotId'])) {
	drupal_set_message(t('Failed to insert permit predictions'), 'error');
      }
    }
  }
}



function chpan_prepare_lanecount_predictions($company_id, $property_id, $policies) {
  // Prepare aggregate (LotId='All') predictions if needed
  $predictions = chdbprop_get_lanepredictions($property_id, TRUE, 'All');
  if (empty($predictions)) {
    if (chdbprop_insert_laneprediction($property_id, 'All')) {
      drupal_set_message(t('Failed to insert lane predictions'), 'error');
    }
  }

  // Prepare property predictions if needed
  foreach ($policies as $policy) {
    $predictions =
      chdbprop_get_lanepredictions
      ($property_id, TRUE, $policy['data']['LotId']);
    if (empty($predictions)) {
      if (chdbprop_insert_laneprediction
	  ($property_id, $policy['data']['LotId'])) {
	drupal_set_message(t('Failed to insert lane predictions'), 'error');
      }
    }
  }
}



function chpan_prepare_ticket_predictions($company_id, $property_id, $policies) {
  // Prepare aggregate (LotId='All') ticket predictions if needed
  $predictions =
    chdbtick_get_predictions
    ($company_id, $property_id, TRUE, 'All');
  if (sizeof($predictions) == 0) {
    if (chdbtick_insert_prediction($company_id, $property_id, 'All')) {
      drupal_set_message(t('Failed to insert ticket predictions'), 'error');
    }
  }

  // Prepare property ticket predictions if needed
  foreach ($policies as $policy) {
    $predictions =
      chdbtick_get_predictions
      ($company_id, $property_id, TRUE, $policy['data']['LotId']);
    if (sizeof($predictions) == 0) {
      if (chdbtick_insert_prediction
	  ($company_id, $property_id, $policy['data']['LotId'])) {
	drupal_set_message(t('Failed to insert ticket predictions'), 'error');
      }
    }
  }
}



function chpan_prepare_boot_predictions($company_id, $property_id, $policies) {
  // Prepare aggregate (LotId='All') boot predictions if needed
  $predictions =
    chdbboot_get_predictions
    ($company_id, $property_id, TRUE, 'All');
  if (sizeof($predictions) == 0) {
    if (chdbboot_insert_prediction($company_id, $property_id, 'All')) {
      drupal_set_message(t('Failed to insert boot predictions'), 'error');
    }
  }

  // Prepare property boot predictions if needed
  foreach ($policies as $policy) {
    $predictions =
      chdbboot_get_predictions
      ($company_id, $property_id, TRUE, $policy['data']['LotId']);
    if (sizeof($predictions) == 0) {
      if (chdbboot_insert_prediction
	  ($company_id, $property_id, $policy['data']['LotId'])) {
	drupal_set_message(t('Failed to insert boot predictions'), 'error');
      }
    }
  }
}



function chpan_prepare_valet_predictions($company_id, $property_id, $policies) {
  // Prepare aggregate (LotId='All') valet predictions if needed
  $predictions =
    chdbvalet_get_predictions
    ($company_id, $property_id, TRUE, 'All');
  if (sizeof($predictions) == 0) {
    if (chdbvalet_insert_prediction($company_id, $property_id, 'All')) {
      drupal_set_message(t('Failed to insert valet predictions'), 'error');
    }
  }

  // Prepare property valet predictions if needed
  foreach ($policies as $policy) {
    $predictions =
      chdbvalet_get_predictions
      ($company_id, $property_id, TRUE, $policy['data']['LotId']);
    if (sizeof($predictions) == 0) {
      if (chdbvalet_insert_prediction
	  ($company_id, $property_id, $policy['data']['LotId'])) {
	drupal_set_message(t('Failed to insert valet predictions'), 'error');
      }
    }
  }
}



function chpan_permit_graph_predictions($company_id, $property_id, $policies) {
  //drupal_set_message('<pre>'.var_export($policies, TRUE).'</pre>');

  global $base_path;
  $final_data = array();

  // Permit graphs and predictions
  $lyaponov_iterations = 3;
  $policy = current($policies);
  $num_policies = count($policies);

  // Looping num_lots +1
  // '+1' is for the aggregate

  for ($lot_index = 0; $lot_index < $num_policies + 1; $lot_index++) {
    $loop_lot_id = $policy['data']['LotId']; //drupal_set_message('LotId:'.$loop_lot_id);

    if ($lot_index == 0) { // First round is the aggregate
      $predictions =
	chdbperm_get_predictions
	($company_id, $property_id, TRUE, 'All');
    } else {
      $predictions =
	chdbperm_get_predictions
	($company_id, $property_id, TRUE, $loop_lot_id);
    }

    // Collect data
    $list_new = array();
    $labels = array();
    $start = chpuser_datetime_utc_to_usertimezone('now');
    $stop = chpuser_datetime_utc_to_usertimezone('now');
    $today = $start->format("D");
    $start->modify('+1 day');
    $future = $start->format("D");
    $start->modify('-2 days');
    if ($lot_index == 0) { // First round is the aggregate
      $basename = "dayperm_".$company_id."_".$property_id;
    } else {
      $basename = "dayperm_".$company_id."_".$property_id."_".$loop_lot_id;
    }
    $basename .= "_".$stop->format("Y_m_d");
    $filename = $basename.".png";
    $image_daily = $base_path."tmpimage/".$filename;

    if (file_exists(ltrim($image_daily, '/'))) {
      $current_day = $predictions['Today'];
      $next_day = $predictions['Tomorrow'];
    } else {
      // Run last 14 days as part of daily activity
      for ($i = 0; $i < 14; $i++) {
	if ($lot_index == 0) { // First round is the aggregate
	  $entries = chdban_get_permit_stats
	    ($company_id, $property_id,
	     FALSE, 'LOT',
	     FALSE, 'GROUP',
	     FALSE, 'MEMBER',
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	} else {
	  $entries = chdban_get_permit_stats
	    ($company_id, $property_id,
	     TRUE, $loop_lot_id,
	     FALSE, 'GROUP',
	     FALSE, 'MEMBER',
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	} //print_r($entries);print('<p>');
	$list_new[] = count($entries);
	$labels[] = $start->format("D d");
	$start->modify('-1 day');
	$stop->modify('-1 day');
      } //print_r($list_new);

      // Find average of all samples
      $next_day_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if ($punctures[$index] > 0.0) {
	  $n++;
	}
	$next_day_average += $value * $punctures[$index];
      }
      if ($n > 0) {
	$next_day_average /= $n;
      }
      $today_average = $next_day_average;

      // Find average of particular today among all samples
      $today_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if (strpos($labels[$index], $today) !== FALSE) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $today_particular_average += $value * $punctures[$index];
	}
      }
      if ($n > 0) {
	$today_particular_average /= $n;
      }

      // Find average of next particular day among all samples
      $next_day_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if (strpos($labels[$index], $future) !== FALSE) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_day_particular_average += $value * $punctures[$index];
	}
      }
      if ($n > 0) {
	$next_day_particular_average /= $n;
      }

      // Find Lyaponov based filtered avarage
      $next_day_lyaponov_average = $next_day_average;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      for ($i = 0; $i < $lyaponov_iterations; $i++) {
	$deviations = array();
	foreach ($list_new as $index => $value) {
	  $deviations[$index] = $value - $next_day_lyaponov_average;
	}
	foreach ($deviations as $index => $value) {
	  if ($value == max($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	  if ($value == min($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	}
	$next_day_lyaponov_average = 0;
	$n = 0;
	foreach ($list_new as $index => $value) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_day_lyaponov_average += $value * $punctures[$index];
	}
	if ($n > 0) {
	  $next_day_lyaponov_average /= $n;
	}
      }
      $today_lyaponov_average = $next_day_lyaponov_average;

      $current_day = ($today_average +
		      $today_particular_average +
		      $today_lyaponov_average) / 3;
      $current_day = round($current_day);

      $next_day = ($next_day_average +
		   $next_day_particular_average +
		   $next_day_lyaponov_average) / 3;
      $next_day = round($next_day);

      $list_new = array_reverse($list_new);
      $labels = array_reverse($labels);

      $max_scale = max($list_new);
      //drupal_set_message($max_scale);
      if ($max_scale == 0) {
	$max_scale = 1;
      }

      if ($lot_index == 0) { // First round is the aggregate
	$chart_title = t('Daily permit registrations at property');
	$chart_id = 'daily_permit_chart_aggregated';
      } else {
	$chart_title = t('Daily permit registrations at ').$policy['data']['Alias'];
	$chart_id = 'daily_permit_chart_'.$loop_lot_id;
      }

      // Daily activity graph
      $chart = array
	(// Mandatory
	 '#chart_id' => $chart_id,
	 '#type' => CHART_TYPE_BAR_V,
	 '#data' => array(0 => $list_new),
	 // Optional
	 '#title' => chart_title($chart_title),
	 '#size' => chart_size(800, 200),
	 '#adjust_resolution' => array('#adjust' => TRUE, '#max' => $max_scale),
	 '#grid_lines' => chart_grid_lines(100, 10),
	 '#data_colors' => array(0 => chart_unique_color('new')),
	 '#mixed_axis_labels' =>
	 array(CHART_AXIS_Y_LEFT =>
	       array(0 => array(chart_mixed_axis_range_label(0, $max_scale))),
	       CHART_AXIS_X_BOTTOM =>
	       array(0 => array_map(chart_mixed_axis_label, $labels),
		     1 => array(chart_mixed_axis_label(t('Day of month'), 50)))),
	 '#bar_size' => chart_bar_size(20, 30),
	 ); //print_r($chart);print('<p>');

      chart_copy($chart, $basename);
      $source = file_create_path('charts').'/'.$filename;
      //drupal_set_message('SOURCE:'.$source);
      $dest = ltrim($base_path, '/').'tmpimage/'.$filename;
      //drupal_set_message('DEST:'.$dest);
      copy($source, $dest);
      unlink($source);

      if ($lot_index == 0) { // First round is the aggregate
	if (chdbperm_update_prediction_daily
	    ($company_id, $property_id,
	     'All',
	     $current_day,
	     $next_day)) {
	  drupal_set_message(t('Failed to update daily permit predictions'), 'error');
	}
      } else {
	if (chdbperm_update_prediction_daily
	    ($company_id, $property_id,
	     $loop_lot_id,
	     $current_day,
	     $next_day)) {
	  drupal_set_message(t('Failed to update daily permit predictions'), 'error');
	}
      }
    }

    $list_new = array();
    $labels = array();
    $start = chpuser_datetime_utc_to_usertimezone('now');
    $stop = chpuser_datetime_utc_to_usertimezone('now');
    $start->modify(sprintf("-%d days", $start->format("N") - 1));
    $stop->modify(sprintf("-%d days", $stop->format("N") - 1));
    $start->modify('-4 weeks');
    $future = array();
    $future[] = $start->format("N");
    $start->modify('-4 weeks');
    $future[] = $start->format("N");
    $start->modify('-4 weeks');
    $future[] = $start->format("N");
    $start->modify('-4 weeks');
    $future[] = $start->format("N");
    $start->modify('+15 weeks');
    if ($lot_index == 0) { // First round is the aggregate
      $basename = "weekperm_".$company_id."_".$property_id;
    } else {
      $basename = "weekperm_".$company_id."_".$property_id."_".$loop_lot_id;
    }
    $basename .= "_".$stop->format("Y_W");
    $filename = $basename.".png";
    $image_weekly = $base_path."tmpimage/".$filename;

    if (file_exists(ltrim($image_weekly, '/'))) {
      $next_week = $predictions['NextWeek'];
    } else {
      // Run last 16 weeks as part of weekly activity
      for ($i = 0; $i < 16; $i++) {
	if ($lot_index == 0) { // First round is the aggregate
	  $entries = chdban_get_permit_stats
	    ($company_id, $property_id,
	     FALSE, 'LOT',
	     FALSE, 'GROUP',
	     FALSE, 'MEMBER',
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	} else {
	  $entries = chdban_get_permit_stats
	    ($company_id, $property_id,
	     TRUE, $loop_lot_id,
	     FALSE, 'GROUP',
	     FALSE, 'MEMBER',
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	}
	$list_new[] = count($entries);
	$labels[] = $start->format("W");
	$start->modify('-1 week');
	$stop->modify('-1 week');
      } //print_r($list_new);

      // Find average of all samples
      $next_week_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if ($punctures[$index] > 0.0) {
	  $n++;
	}
	$next_week_average += $value * $punctures[$index];
      }
      if ($n > 0) {
	$next_week_average /= $n;
      }

      // Find average of next particular week among all samples
      $next_week_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	foreach ($future as $tmp) {
	  if (strpos($labels[$index], $tmp) !== FALSE) {
	    if ($punctures[$index] > 0.0) {
	      $n++;
	    }
	    $next_week_particular_average += $value * $punctures[$index];
	  }
	}
      }
      if ($n > 0) {
	$next_week_particular_average /= $n;
      }

      // Find Lyaponov based filtered avarage
      $next_week_lyaponov_average = $next_week_average;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      for ($i = 0; $i < $lyaponov_iterations; $i++) {
	$deviations = array();
	foreach ($list_new as $index => $value) {
	  $deviations[$index] = $value - $next_week_lyaponov_average;
	}
	foreach ($deviations as $index => $value) {
	  if ($value == max($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	  if ($value == min($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	}
	$next_week_lyaponov_average = 0;
	$n = 0;
	foreach ($list_new as $index => $value) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_week_lyaponov_average += $value * $punctures[$index];
	}
	if ($n > 0) {
	  $next_week_lyaponov_average /= $n;
	}
      }

      $next_week = ($next_week_average +
		    $next_week_particular_average +
		    $next_week_lyaponov_average) / 3;
      $next_week = round($next_week);

      $list_new = array_reverse($list_new);
      $labels = array_reverse($labels);

      $max_scale = max($list_new);
      //drupal_set_message($max_scale);
      if ($max_scale == 0) {
	$max_scale = 1;
      }

      if ($lot_index == 0) { // First round is the aggregate
	$chart_title = t('Weekly permit registrations at property');
	$chart_id = 'weekly_permit_chart_aggregated';
      } else {
	$chart_title = t('Weekly permit registrations at ').$policy['data']['Alias'];
	$chart_id = 'weekly_permit_chart_'.$loop_lot_id;
      }

      // Weekly activity graph
      $chart = array
	(// Mandatory
	 '#chart_id' => $chart_id,
	 '#type' => CHART_TYPE_BAR_V,
	 '#data' => array(0 => $list_new),
	 // Optional
	 '#title' => chart_title($chart_title),
	 '#size' => chart_size(800, 200),
	 '#adjust_resolution' => array('#adjust' => TRUE, '#max' => $max_scale),
	 '#grid_lines' => chart_grid_lines(100, 10),
	 '#data_colors' => array(0 => chart_unique_color('new')),
	 '#mixed_axis_labels' =>
	 array(CHART_AXIS_Y_LEFT =>
	       array(0 => array(chart_mixed_axis_range_label(0, $max_scale))),
	       CHART_AXIS_X_BOTTOM =>
	       array(0 => array_map(chart_mixed_axis_label, $labels),
		     1 => array(chart_mixed_axis_label(t('ISO Week number'), 50)))),
	 '#bar_size' => chart_bar_size(30, 12),
	 ); //print_r($chart);print('<p>');

      chart_copy($chart, $basename);
      $source = file_create_path('charts').'/'.$filename;
      //drupal_set_message('SOURCE:'.$source);
      $dest = ltrim($base_path, '/').'tmpimage/'.$filename;
      //drupal_set_message('DEST:'.$dest);
      copy($source, $dest);
      unlink($source);

      if ($lot_index == 0) { // First round is the aggregate
	if (chdbperm_update_prediction_weekly
	    ($company_id, $property_id,
	     'All',
	     $next_week)) {
	  drupal_set_message(t('Failed to update weekly permit predictions'), 'error');
	}
      } else {
	if (chdbperm_update_prediction_weekly
	    ($company_id, $property_id,
	     $loop_lot_id,
	     $next_week)) {
	  drupal_set_message(t('Failed to update weekly permit predictions'), 'error');
	}
      }
    }

    $list_new = array();
    $labels = array();
    $start = chpuser_datetime_utc_to_usertimezone('now');
    $stop = chpuser_datetime_utc_to_usertimezone('now');
    $start->modify(sprintf("-%d days", $start->format("j") - 1));
    $stop->modify(sprintf("-%d days", $stop->format("j") - 1));
    $start->modify('+1 month');
    $future = $start->format("F");
    $start->modify('-2 months');
    if ($lot_index == 0) { // First round is the aggregate
      $basename = "monperm_".$company_id."_".$property_id;
    } else {
      $basename = "monperm_".$company_id."_".$property_id."_".$loop_lot_id;
    }
    $basename .= "_".$stop->format("Y_m");
    $filename = $basename.".png";
    $image_monthly = $base_path."tmpimage/".$filename;

    if (file_exists(ltrim($image_monthly, '/'))) {
      $next_month = $predictions['NextMonth'];
    } else {
      // Run last 12 months as part of monthly activity
      for ($i = 0; $i < 12; $i++) {
	if ($lot_index == 0) { // First round is the aggregate
	  $entries = chdban_get_permit_stats
	    ($company_id, $property_id,
	     FALSE, 'LOT',
	     FALSE, 'GROUP',
	     FALSE, 'MEMBER',
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	} else {
	  $entries = chdban_get_permit_stats
	    ($company_id, $property_id,
	     TRUE, $loop_lot_id,
	     FALSE, 'GROUP',
	     FALSE, 'MEMBER',
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	}
	$list_new[] = count($entries);
	$labels[] = $start->format("F");
	$start->modify('-1 month');
	$stop->modify('-1 month');
      } //print_r($list_new);

      // Find average of all samples
      $next_month_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if ($punctures[$index] > 0.0) {
	  $n++;
	}
	$next_month_average += $value * $punctures[$index];
      }
      if ($n > 0) {
	$next_month_average /= $n;
      }

      // Find average of next particular month among all samples
      $next_month_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if (strpos($labels[$index], $future) !== FALSE) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_month_particular_average += $value * $punctures[$index];
	}
      }
      if ($n > 0) {
	$next_month_particular_average /= $n;
      }

      // Find Lyaponov based filtered avarage
      $next_month_lyaponov_average = $next_month_average;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      for ($i = 0; $i < $lyaponov_iterations; $i++) {
	$deviations = array();
	foreach ($list_new as $index => $value) {
	  $deviations[$index] = $value - $next_month_lyaponov_average;
	}
	foreach ($deviations as $index => $value) {
	  if ($value == max($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	  if ($value == min($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	}
	$next_month_lyaponov_average = 0;
	$n = 0;
	foreach ($list_new as $index => $value) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_month_lyaponov_average += $value * $punctures[$index];
	}
	if ($n > 0) {
	  $next_month_lyaponov_average /= $n;
	}
      }

      if ($next_month_particular_average > 0) {
	$next_month = ($next_month_average +
		       $next_month_particular_average +
		       $next_month_lyaponov_average) / 3;
      } else {
	$next_month = ($next_month_average +
		       $next_month_lyaponov_average) / 2;
      }
      $next_month = round($next_month);

      $list_new = array_reverse($list_new);
      $labels = array_reverse($labels);

      $max_scale = max($list_new);
      //drupal_set_message($max_scale);
      if ($max_scale == 0) {
	$max_scale = 1;
      }

      if ($lot_index == 0) { // First round is the aggregate
	$chart_title = t('Monthly permit registrations at property');
	$chart_id = 'monthly_permit_chart_aggregated';
      } else {
	$chart_title = t('Monthly permit registrations at ').$policy['data']['Alias'];
	$chart_id = 'monthly_permit_chart_'.$loop_lot_id;
      }

      // Monthly activity graph
      $chart = array
	(// Mandatory
	 '#chart_id' => $chart_id,
	 '#type' => CHART_TYPE_BAR_V,
	 '#data' => array(0 => $list_new),
	 // Optional
	 '#title' => chart_title($chart_title),
	 '#size' => chart_size(800, 200),
	 '#adjust_resolution' => array('#adjust' => TRUE, '#max' => $max_scale),
	 '#grid_lines' => chart_grid_lines(100, 10),
	 '#data_colors' => array(0 => chart_unique_color('new')),
	 '#mixed_axis_labels' =>
	 array(CHART_AXIS_Y_LEFT =>
	       array(0 => array(chart_mixed_axis_range_label(0, $max_scale))),
	       CHART_AXIS_X_BOTTOM =>
	       array(0 => array_map(chart_mixed_axis_label, $labels),
		     1 => array(chart_mixed_axis_label(t('Month'), 50)))),
	 '#bar_size' => chart_bar_size(42, 15),
	 ); //print_r($chart);print('<p>');

      chart_copy($chart, $basename);
      $source = file_create_path('charts').'/'.$filename;
      //drupal_set_message('SOURCE:'.$source);
      $dest = ltrim($base_path, '/').'tmpimage/'.$filename;
      //drupal_set_message('DEST:'.$dest);
      copy($source, $dest);
      unlink($source);

      if ($lot_index == 0) { // First round is the aggregate
	if (chdbperm_update_prediction_monthly
	    ($company_id, $property_id,
	     'All',
	     $next_month)) {
	  drupal_set_message(t('Failed to update monthly permit predictions'), 'error');
	}
      } else {
	if (chdbperm_update_prediction_monthly
	    ($company_id, $property_id,
	     $loop_lot_id,
	     $next_month)) {
	  drupal_set_message(t('Failed to update monthly permit predictions'), 'error');
	}
      }
    }

    if (0) { // Debug
      $rows[] = array('<b>Predictions based on flat average</b>',
		      round($today_average),
		      round($next_day_average),
		      round($next_week_average),
		      round($next_month_average));

      if ($next_month_particular_average > 0) {
	$next_month_particular_average = round($next_month_particular_average);
      } else {
	$next_month_particular_average = t('Missing data');
      }
      $rows[] = array('<b>Predictions based on particular avarage</b>',
		      round($today_particular_average),
		      round($next_day_particular_average),
		      round($next_week_particular_average),
		      $next_month_particular_average);

      $rows[] = array('<b>Predictions based on Lyaponov filterd average</b>',
		      round($today_lyaponov_average),
		      round($next_day_lyaponov_average),
		      round($next_week_lyaponov_average),
		      round($next_month_lyaponov_average));

      $rows[] = array('<b>Final predictions</b>',
		      round($current_day),
		      round($next_day),
		      round($next_week),
		      round($next_month));

      $header = array('', 'Today', 'Tomorrow', 'Next week', 'Next month');
      $form['property']['Debug'] = array('#value' => theme_table($header, $rows),
					 '#suffix' => '<p><p>');
    }

    if ($lot_index == 0) { // First round is the aggregate
      $final_data['Aggregate']['current_day'] = $current_day;
      $final_data['Aggregate']['next_day'] = $next_day;
      $final_data['Aggregate']['next_week'] = $next_week;
      $final_data['Aggregate']['next_month'] = $next_month;
      $final_data['Aggregate']['image_daily'] = $image_daily;
      $final_data['Aggregate']['image_weekly'] = $image_weekly;
      $final_data['Aggregate']['image_monthly'] = $image_monthly;
    } else {
      $final_data[$loop_lot_id]['current_day'] = $current_day;
      $final_data[$loop_lot_id]['next_day'] = $next_day;
      $final_data[$loop_lot_id]['next_week'] = $next_week;
      $final_data[$loop_lot_id]['next_month'] = $next_month;
      $final_data[$loop_lot_id]['alias'] = $policy['data']['Alias'];
      $final_data[$loop_lot_id]['image_daily'] = $image_daily;
      $final_data[$loop_lot_id]['image_weekly'] = $image_weekly;
      $final_data[$loop_lot_id]['image_monthly'] = $image_monthly;
    }

    if ($lot_index > 0) { // First round is the aggregate
      $policy = next($policies);
    }
  } // Permit graphs and predictions

  return $final_data;
}



function chpan_lanecount_graph_predictions($company_id, $property_id, $policies,
					   $lots, $lanes) {
  //drupal_set_message('<pre>'.var_export($policies, TRUE).'</pre>');

  global $base_path;
  $final_data = array();

  // Permit graphs and predictions
  $lyaponov_iterations = 3;
  $policy = current($policies);
  $num_policies = count($policies);

  for ($lot_index = 0; $lot_index < $num_policies + 1; $lot_index++) {
    $loop_lot_id = $policy['data']['LotId']; //drupal_set_message('LotId:'.$loop_lot_id);

    if ($lot_index == 0) { // First round is the aggregate
      $predictions =
	chdbprop_get_lanepredictions
	($property_id, TRUE, 'All');
    } else {
      $predictions =
	chdbprop_get_lanepredictions
	($property_id, TRUE, $loop_lot_id);
    }

    $local = chpuser_datetime_utc_to_usertimezone('now');
    $today = $local->format("D");
    $local->modify('+1 day');
    $future = $local->format("D");
    $local->modify('-1 day');

    $time = chpuser_datetime_usertimezone_to_utc($local->format("Y-m-d 00:00:00"));

    // Set image file name
    if ($lot_index == 0) { // First round is the aggregate
      $basename = "daylanecount_".$property_id;
    } else {
      $basename = "daylanecount_".$property_id."_".$loop_lot_id;
    }
    $basename .= "_".$local->format("Y_m_d");
    $filename = $basename.".png";
    $image_daily = $base_path."tmpimage/".$filename;

    // Collect data
    $list_new = array();
    $labels = array();

    if (file_exists(ltrim($image_daily, '/'))) {
      $current_day = $predictions['CountToday'];
      $next_day = $predictions['CountTomorrow'];
    } else {
      // Run last 14 days as part of daily activity
      $steps = 14;
      if ($lot_index == 0) { // First round is the aggregate
	$list_new = chdban_get_loopscount_stats
	  ($property_id, $lots, $lanes,
	   $time->format("Y-m-d H:i:s"),
	   'Daily',
	   $steps);
      } else {
	$list_new = chdban_get_loopscount_stats
	  ($property_id, array($loop_lot_id), $lanes,
	   $time->format("Y-m-d H:i:s"),
	   'Daily',
	   $steps);
      }

      for ($i = 0; $i < $steps; $i++) {
	$time->modify('-1 day');
	$labels[] = $time->format("D d");
      }

      // Find average of all samples
      $next_day_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if ($punctures[$index] > 0.0) {
	  $n++;
	}
	$next_day_average += $value * $punctures[$index];
      }
      if ($n > 0) {
	$next_day_average /= $n;
      }
      $today_average = $next_day_average;

      // Find average of particular today among all samples
      $today_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if (strpos($labels[$index], $today) !== FALSE) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $today_particular_average += $value * $punctures[$index];
	}
      }
      if ($n > 0) {
	$today_particular_average /= $n;
      }

      // Find average of next particular day among all samples
      $next_day_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if (strpos($labels[$index], $future) !== FALSE) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_day_particular_average += $value * $punctures[$index];
	}
      }
      if ($n > 0) {
	$next_day_particular_average /= $n;
      }

      // Find Lyaponov based filtered avarage
      $next_day_lyaponov_average = $next_day_average;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      for ($i = 0; $i < $lyaponov_iterations; $i++) {
	$deviations = array();
	foreach ($list_new as $index => $value) {
	  $deviations[$index] = $value - $next_day_lyaponov_average;
	}
	foreach ($deviations as $index => $value) {
	  if ($value == max($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	  if ($value == min($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	}
	$next_day_lyaponov_average = 0;
	$n = 0;
	foreach ($list_new as $index => $value) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_day_lyaponov_average += $value * $punctures[$index];
	}
	if ($n > 0) {
	  $next_day_lyaponov_average /= $n;
	}
      }
      $today_lyaponov_average = $next_day_lyaponov_average;

      $current_day = ($today_average +
		      $today_particular_average +
		      $today_lyaponov_average) / 3;
      $current_day = round($current_day);

      $next_day = ($next_day_average +
		   $next_day_particular_average +
		   $next_day_lyaponov_average) / 3;
      $next_day = round($next_day);

      $list_new = array_reverse($list_new);
      $labels = array_reverse($labels);

      $max_scale = max($list_new);
      //drupal_set_message($max_scale);
      if ($max_scale == 0) {
	$max_scale = 1;
      }

      if ($lot_index == 0) { // First round is the aggregate
	$chart_title = t('Daily lane counts at property');
	$chart_id = 'daily_lanecount_chart_aggregated';
      } else {
	$chart_title = t('Daily lane counts at ').$policy['data']['Alias'];
	$chart_id = 'daily_lanecount_chart_'.$loop_lot_id;
      }

      // Daily activity graph
      $chart = array
	(// Mandatory
	 '#chart_id' => $chart_id,
	 '#type' => CHART_TYPE_BAR_V,
	 '#data' => array(0 => $list_new),
	 // Optional
	 '#title' => chart_title($chart_title),
	 '#size' => chart_size(800, 200),
	 '#adjust_resolution' => array('#adjust' => TRUE, '#max' => $max_scale),
	 '#grid_lines' => chart_grid_lines(100, 10),
	 '#data_colors' => array(0 => chart_unique_color('new')),
	 '#mixed_axis_labels' =>
	 array(CHART_AXIS_Y_LEFT =>
	       array(0 => array(chart_mixed_axis_range_label(0, $max_scale))),
	       CHART_AXIS_X_BOTTOM =>
	       array(0 => array_map(chart_mixed_axis_label, $labels),
		     1 => array(chart_mixed_axis_label(t('Day of month'), 50)))),
	 '#bar_size' => chart_bar_size(20, 30),
	 ); //print_r($chart);print('<p>');

      chart_copy($chart, $basename);
      $source = file_create_path('charts').'/'.$filename;
      //drupal_set_message('SOURCE:'.$source);
      $dest = ltrim($base_path, '/').'tmpimage/'.$filename;
      //drupal_set_message('DEST:'.$dest);
      copy($source, $dest);
      unlink($source);

      if ($lot_index == 0) { // First round is the aggregate
	if (chdbprop_update_laneprediction_dailycount
	    ($property_id,
	     'All',
	     $current_day,
	     $next_day)) {
	  drupal_set_message(t('Failed to update daily lanecount predictions'), 'error');
	}
      } else {
	if (chdbprop_update_laneprediction_dailycount
	    ($property_id,
	     $loop_lot_id,
	     $current_day,
	     $next_day)) {
	  drupal_set_message(t('Failed to update daily lanecount predictions'), 'error');
	}
      }
    }

    // Weekly

    $local = chpuser_datetime_utc_to_usertimezone('now');
    $local->modify(sprintf("-%d days", $local->format("N") - 1));
    $time = chpuser_datetime_usertimezone_to_utc($local->format("Y-m-d 00:00:00"));

    $future = array();
    $local->modify('-4 weeks');
    $future[] = $local->format("N");
    $local->modify('-4 weeks');
    $future[] = $local->format("N");
    $local->modify('-4 weeks');
    $future[] = $local->format("N");
    $local->modify('-4 weeks');
    $future[] = $local->format("N");
    $local->modify('+16 weeks');

    // Set image file name
    if ($lot_index == 0) { // First round is the aggregate
      $basename = "weeklanecount_".$property_id;
    } else {
      $basename = "weeklanecount_".$property_id."_".$loop_lot_id;
    }
    $basename .= "_".$local->format("Y_W");
    $filename = $basename.".png";
    $image_weekly = $base_path."tmpimage/".$filename;

    // Collect data
    $list_new = array();
    $labels = array();

    if (file_exists(ltrim($image_weekly, '/'))) {
      $next_week = $predictions['CountNextWeek'];
    } else {
      // Run last 16 weeks as part of weekly activity
      $steps = 16;
      if ($lot_index == 0) { // First round is the aggregate
	$list_new = chdban_get_loopscount_stats
	  ($property_id, $lots, $lanes,
	   $time->format("Y-m-d H:i:s"),
	   'Weekly',
	   $steps);
      } else {
	$list_new = chdban_get_loopscount_stats
	  ($property_id, array($loop_lot_id), $lanes,
	   $time->format("Y-m-d H:i:s"),
	   'Weekly',
	   $steps);
      }

      for ($i = 0; $i < $steps; $i++) {
	$time->modify('-1 week');
	$labels[] = $time->format("W");
      }

      // Find average of all samples
      $next_week_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if ($punctures[$index] > 0.0) {
	  $n++;
	}
	$next_week_average += $value * $punctures[$index];
      }
      if ($n > 0) {
	$next_week_average /= $n;
      }

      // Find average of next particular week among all samples
      $next_week_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	foreach ($future as $tmp) {
	  if (strpos($labels[$index], $tmp) !== FALSE) {
	    if ($punctures[$index] > 0.0) {
	      $n++;
	    }
	    $next_week_particular_average += $value * $punctures[$index];
	  }
	}
      }
      if ($n > 0) {
	$next_week_particular_average /= $n;
      }

      // Find Lyaponov based filtered avarage
      $next_week_lyaponov_average = $next_week_average;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      for ($i = 0; $i < $lyaponov_iterations; $i++) {
	$deviations = array();
	foreach ($list_new as $index => $value) {
	  $deviations[$index] = $value - $next_week_lyaponov_average;
	}
	foreach ($deviations as $index => $value) {
	  if ($value == max($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	  if ($value == min($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	}
	$next_week_lyaponov_average = 0;
	$n = 0;
	foreach ($list_new as $index => $value) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_week_lyaponov_average += $value * $punctures[$index];
	}
	if ($n > 0) {
	  $next_week_lyaponov_average /= $n;
	}
      }

      $next_week = ($next_week_average +
		    $next_week_particular_average +
		    $next_week_lyaponov_average) / 3;
      $next_week = round($next_week);

      $list_new = array_reverse($list_new);
      $labels = array_reverse($labels);

      $max_scale = max($list_new);
      //drupal_set_message($max_scale);
      if ($max_scale == 0) {
	$max_scale = 1;
      }

      if ($lot_index == 0) { // First round is the aggregate
	$chart_title = t('Weekly lane counts at property');
	$chart_id = 'weekly_lanecount_chart_aggregated';
      } else {
	$chart_title = t('Weekly lane counts at ').$policy['data']['Alias'];
	$chart_id = 'weekly_lanecount_chart_'.$loop_lot_id;
      }

      // Weekly activity graph
      $chart = array
	(// Mandatory
	 '#chart_id' => $chart_id,
	 '#type' => CHART_TYPE_BAR_V,
	 '#data' => array(0 => $list_new),
	 // Optional
	 '#title' => chart_title($chart_title),
	 '#size' => chart_size(800, 200),
	 '#adjust_resolution' => array('#adjust' => TRUE, '#max' => $max_scale),
	 '#grid_lines' => chart_grid_lines(100, 10),
	 '#data_colors' => array(0 => chart_unique_color('new')),
	 '#mixed_axis_labels' =>
	 array(CHART_AXIS_Y_LEFT =>
	       array(0 => array(chart_mixed_axis_range_label(0, $max_scale))),
	       CHART_AXIS_X_BOTTOM =>
	       array(0 => array_map(chart_mixed_axis_label, $labels),
		     1 => array(chart_mixed_axis_label(t('ISO Week number'), 50)))),
	 '#bar_size' => chart_bar_size(30, 12),
	 ); //print_r($chart);print('<p>');

      chart_copy($chart, $basename);
      $source = file_create_path('charts').'/'.$filename;
      //drupal_set_message('SOURCE:'.$source);
      $dest = ltrim($base_path, '/').'tmpimage/'.$filename;
      //drupal_set_message('DEST:'.$dest);
      copy($source, $dest);
      unlink($source);

      if ($lot_index == 0) { // First round is the aggregate
	if (chdbprop_update_laneprediction_weeklycount
	    ($property_id,
	     'All',
	     $next_week)) {
	  drupal_set_message(t('Failed to update weekly lanecount predictions'), 'error');
	}
      } else {
	if (chdbprop_update_laneprediction_weeklycount
	    ($property_id,
	     $loop_lot_id,
	     $next_week)) {
	  drupal_set_message(t('Failed to update weekly lanecount predictions'), 'error');
	}
      }
    }

    // Monthly

    $local = chpuser_datetime_utc_to_usertimezone('now');
    $local->modify(sprintf("-%d days", $local->format("j") - 1));
    $time = chpuser_datetime_usertimezone_to_utc($local->format("Y-m-d 00:00:00"));

    $local->modify('+1 month');
    $future = $local->format("F");
    $local->modify('-1 months');

    // Set image file name
    if ($lot_index == 0) { // First round is the aggregate
      $basename = "monlanecount_".$property_id;
    } else {
      $basename = "monlanecount_".$property_id."_".$loop_lot_id;
    }
    $basename .= "_".$local->format("Y_m");
    $filename = $basename.".png";
    $image_monthly = $base_path."tmpimage/".$filename;

    // Collect data
    $list_new = array();
    $labels = array();

    if (file_exists(ltrim($image_monthly, '/'))) {
      $next_month = $predictions['CountNextMonth'];
    } else {
      // Run last 12 months as part of monthly activity
      $steps = 12;
      if ($lot_index == 0) { // First round is the aggregate
	$list_new = chdban_get_loopscount_stats
	  ($property_id, $lots, $lanes,
	   $time->format("Y-m-d H:i:s"),
	   'Monthly',
	   $steps);
      } else {
	$list_new = chdban_get_loopscount_stats
	  ($property_id, array($loop_lot_id), $lanes,
	   $time->format("Y-m-d H:i:s"),
	   'Monthly',
	   $steps);
      }

      for ($i = 0; $i < $steps; $i++) {
	$time->modify('-1 month');
	$labels[] = $time->format("F");
      }

      // Find average of all samples
      $next_month_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if ($punctures[$index] > 0.0) {
	  $n++;
	}
	$next_month_average += $value * $punctures[$index];
      }
      if ($n > 0) {
	$next_month_average /= $n;
      }

      // Find average of next particular month among all samples
      $next_month_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if (strpos($labels[$index], $future) !== FALSE) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_month_particular_average += $value * $punctures[$index];
	}
      }
      if ($n > 0) {
	$next_month_particular_average /= $n;
      }

      // Find Lyaponov based filtered avarage
      $next_month_lyaponov_average = $next_month_average;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      for ($i = 0; $i < $lyaponov_iterations; $i++) {
	$deviations = array();
	foreach ($list_new as $index => $value) {
	  $deviations[$index] = $value - $next_month_lyaponov_average;
	}
	foreach ($deviations as $index => $value) {
	  if ($value == max($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	  if ($value == min($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	}
	$next_month_lyaponov_average = 0;
	$n = 0;
	foreach ($list_new as $index => $value) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_month_lyaponov_average += $value * $punctures[$index];
	}
	if ($n > 0) {
	  $next_month_lyaponov_average /= $n;
	}
      }

      if ($next_month_particular_average > 0) {
	$next_month = ($next_month_average +
		       $next_month_particular_average +
		       $next_month_lyaponov_average) / 3;
      } else {
	$next_month = ($next_month_average +
		       $next_month_lyaponov_average) / 2;
      }
      $next_month = round($next_month);

      $list_new = array_reverse($list_new);
      $labels = array_reverse($labels);

      $max_scale = max($list_new);
      //drupal_set_message($max_scale);
      if ($max_scale == 0) {
	$max_scale = 1;
      }

      if ($lot_index == 0) { // First round is the aggregate
	$chart_title = t('Monthly lane counts at property');
	$chart_id = 'monthly_lanecount_chart_aggregated';
      } else {
	$chart_title = t('Monthly lane counts at ').$policy['data']['Alias'];
	$chart_id = 'monthly_lanecount_chart_'.$loop_lot_id;
      }

      // Monthly activity graph
      $chart = array
	(// Mandatory
	 '#chart_id' => $chart_id,
	 '#type' => CHART_TYPE_BAR_V,
	 '#data' => array(0 => $list_new),
	 // Optional
	 '#title' => chart_title($chart_title),
	 '#size' => chart_size(800, 200),
	 '#adjust_resolution' => array('#adjust' => TRUE, '#max' => $max_scale),
	 '#grid_lines' => chart_grid_lines(100, 10),
	 '#data_colors' => array(0 => chart_unique_color('new')),
	 '#mixed_axis_labels' =>
	 array(CHART_AXIS_Y_LEFT =>
	       array(0 => array(chart_mixed_axis_range_label(0, $max_scale))),
	       CHART_AXIS_X_BOTTOM =>
	       array(0 => array_map(chart_mixed_axis_label, $labels),
		     1 => array(chart_mixed_axis_label(t('Month'), 50)))),
	 '#bar_size' => chart_bar_size(42, 15),
	 ); //print_r($chart);print('<p>');

      chart_copy($chart, $basename);
      $source = file_create_path('charts').'/'.$filename;
      //drupal_set_message('SOURCE:'.$source);
      $dest = ltrim($base_path, '/').'tmpimage/'.$filename;
      //drupal_set_message('DEST:'.$dest);
      copy($source, $dest);
      unlink($source);

      if ($lot_index == 0) { // First round is the aggregate
	if (chdbprop_update_laneprediction_monthlycount
	    ($property_id,
	     'All',
	     $next_month)) {
	  drupal_set_message(t('Failed to update monthly lanecount predictions'), 'error');
	}
      } else {
	if (chdbprop_update_laneprediction_monthlycount
	    ($property_id,
	     $loop_lot_id,
	     $next_month)) {
	  drupal_set_message(t('Failed to update monthly lanecount predictions'), 'error');
	}
      }
    }

    if (0) { // Debug
      $rows[] = array('<b>Predictions based on flat average</b>',
		      round($today_average),
		      round($next_day_average),
		      round($next_week_average),
		      round($next_month_average));

      if ($next_month_particular_average > 0) {
	$next_month_particular_average = round($next_month_particular_average);
      } else {
	$next_month_particular_average = t('Missing data');
      }
      $rows[] = array('<b>Predictions based on particular avarage</b>',
		      round($today_particular_average),
		      round($next_day_particular_average),
		      round($next_week_particular_average),
		      $next_month_particular_average);

      $rows[] = array('<b>Predictions based on Lyaponov filterd average</b>',
		      round($today_lyaponov_average),
		      round($next_day_lyaponov_average),
		      round($next_week_lyaponov_average),
		      round($next_month_lyaponov_average));

      $rows[] = array('<b>Final predictions</b>',
		      round($current_day),
		      round($next_day),
		      round($next_week),
		      round($next_month));

      $header = array('', 'Today', 'Tomorrow', 'Next week', 'Next month');
      $form['property']['Debug'] = array('#value' => theme_table($header, $rows),
					 '#suffix' => '<p><p>');
    }

    if ($lot_index == 0) { // First round is the aggregate
      $final_data['Aggregate']['current_day'] = $current_day;
      $final_data['Aggregate']['next_day'] = $next_day;
      $final_data['Aggregate']['next_week'] = $next_week;
      $final_data['Aggregate']['next_month'] = $next_month;
      $final_data['Aggregate']['image_daily'] = $image_daily;
      $final_data['Aggregate']['image_weekly'] = $image_weekly;
      $final_data['Aggregate']['image_monthly'] = $image_monthly;
    } else {
      $final_data[$loop_lot_id]['current_day'] = $current_day;
      $final_data[$loop_lot_id]['next_day'] = $next_day;
      $final_data[$loop_lot_id]['next_week'] = $next_week;
      $final_data[$loop_lot_id]['next_month'] = $next_month;
      $final_data[$loop_lot_id]['alias'] = $policy['data']['Alias'];
      $final_data[$loop_lot_id]['image_daily'] = $image_daily;
      $final_data[$loop_lot_id]['image_weekly'] = $image_weekly;
      $final_data[$loop_lot_id]['image_monthly'] = $image_monthly;
    }

    if ($lot_index > 0) { // First round is the aggregate
      $policy = next($policies);
    }
  } // Permit graphs and predictions

  return $final_data;
}



function chpan_ticket_graph_predictions($company_id, $property_id, $policies) {
  global $base_path;

  $final_data = array();

  // Permit graphs and predictions
  $lyaponov_iterations = 3;
  $policy = current($policies);
  $num_policies = count($policies);

  for ($lot_index = 0; $lot_index < $num_policies + 1; $lot_index++) {
    $loop_lot_id = $policy['data']['LotId'];

    if ($lot_index == 0) { // First round is the aggregate
      $predictions =
	chdbtick_get_predictions
	($company_id, $property_id, TRUE, 'All');
    } else {
      $predictions =
	chdbtick_get_predictions
	($company_id, $property_id, TRUE, $loop_lot_id);
    }

    // Collect data
    $list_new = array();
    $labels = array();
    $start = chpuser_datetime_utc_to_usertimezone('now');
    $stop = chpuser_datetime_utc_to_usertimezone('now');
    $today = $start->format("D");
    $start->modify('+1 day');
    $future = $start->format("D");
    $start->modify('-2 days');
    if ($lot_index == 0) { // First round is the aggregate
      $basename = "dayticket_".$company_id."_".$property_id;
    } else {
      $basename = "dayticket_".$company_id."_".$property_id."_".$loop_lot_id;
    }
    $basename .= "_".$stop->format("Y_m_d");
    $filename = $basename.".png";
    $image_daily = $base_path."tmpimage/".$filename;

    if (file_exists(ltrim($image_daily, '/'))) {
      $current_day = $predictions['Today'];
      $next_day = $predictions['Tomorrow'];
    } else {
      // Run last 21 days as part of daily activity
      for ($i = 0; $i < 14; $i++) {
	if ($lot_index == 0) { // First round is the aggregate
	  $entries = chdban_get_ticket_stats
	    ($company_id, $property_id,
	     FALSE, 'LOT',
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	} else {
	  $entries = chdban_get_ticket_stats
	    ($company_id, $property_id,
	     TRUE, $loop_lot_id,
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	} //print_r($entries);print('<p>');
	$list_new[] = count($entries);
	$labels[] = $start->format("D d");
	$start->modify('-1 day');
	$stop->modify('-1 day');
      } //print_r($list_new);

      // Find average of all samples
      $next_day_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if ($punctures[$index] > 0.0) {
	  $n++;
	}
	$next_day_average += $value * $punctures[$index];
      }
      if ($n > 0) {
	$next_day_average /= $n;
      }
      $today_average = $next_day_average;

      // Find average of particular today among all samples
      $today_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if (strpos($labels[$index], $today) !== FALSE) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $today_particular_average += $value * $punctures[$index];
	}
      }
      if ($n > 0) {
	$today_particular_average /= $n;
      }

      // Find average of next particular day among all samples
      $next_day_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if (strpos($labels[$index], $future) !== FALSE) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_day_particular_average += $value * $punctures[$index];
	}
      }
      if ($n > 0) {
	$next_day_particular_average /= $n;
      }

      // Find Lyaponov based filtered avarage
      $next_day_lyaponov_average = $next_day_average;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      for ($i = 0; $i < $lyaponov_iterations; $i++) {
	$deviations = array();
	foreach ($list_new as $index => $value) {
	  $deviations[$index] = $value - $next_day_lyaponov_average;
	}
	foreach ($deviations as $index => $value) {
	  if ($value == max($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	  if ($value == min($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	}
	$next_day_lyaponov_average = 0;
	$n = 0;
	foreach ($list_new as $index => $value) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_day_lyaponov_average += $value * $punctures[$index];
	}
	if ($n > 0) {
	  $next_day_lyaponov_average /= $n;
	}
      }
      $today_lyaponov_average = $next_day_lyaponov_average;

      $current_day = ($today_average +
		      $today_particular_average +
		      $today_lyaponov_average) / 3;
      $current_day = round($current_day);

      $next_day = ($next_day_average +
		   $next_day_particular_average +
		   $next_day_lyaponov_average) / 3;
      $next_day = round($next_day);

      $list_new = array_reverse($list_new);
      $labels = array_reverse($labels);

      $max_scale = max($list_new);
      //drupal_set_message($max_scale);
      if ($max_scale == 0) {
	$max_scale = 1;
      }

      if ($lot_index == 0) { // First round is the aggregate
	$chart_title = t('Daily ticket registrations at property');
	$chart_id = 'daily_ticket_chart_aggregated';
      } else {
	$chart_title = t('Daily ticket registrations at ').$policy['data']['Alias'];
	$chart_id = 'daily_ticket_chart_'.$loop_lot_id;
      }

      // Daily activity graph
      $chart = array
	(// Mandatory
	 '#chart_id' => $chart_id,
	 '#type' => CHART_TYPE_BAR_V,
	 '#data' => array(0 => $list_new),
	 // Optional
	 '#title' => chart_title($chart_title),
	 '#size' => chart_size(800, 200),
	 '#adjust_resolution' => array('#adjust' => TRUE, '#max' => $max_scale),
	 '#grid_lines' => chart_grid_lines(100, 10),
	 '#data_colors' => array(0 => chart_unique_color('new')),
	 '#mixed_axis_labels' =>
	 array(CHART_AXIS_Y_LEFT =>
	       array(0 => array(chart_mixed_axis_range_label(0, $max_scale))),
	       CHART_AXIS_X_BOTTOM =>
	       array(0 => array_map(chart_mixed_axis_label, $labels),
		     1 => array(chart_mixed_axis_label(t('Day of month'), 50)))),
	 '#bar_size' => chart_bar_size(20, 30),
	 ); //print_r($chart);print('<p>');

      chart_copy($chart, $basename);
      $source = file_create_path('charts').'/'.$filename;
      //drupal_set_message('SOURCE:'.$source);
      $dest = ltrim($base_path, '/').'tmpimage/'.$filename;
      //drupal_set_message('DEST:'.$dest);
      copy($source, $dest);
      unlink($source);

      if ($lot_index == 0) { // First round is the aggregate
	if (chdbtick_update_prediction_daily
	    ($company_id, $property_id,
	     'All',
	     $current_day,
	     $next_day)) {
	  drupal_set_message(t('Failed to update daily predictions'), 'error');
	}
      } else {
	if (chdbtick_update_prediction_daily
	    ($company_id, $property_id,
	     $loop_lot_id,
	     $current_day,
	     $next_day)) {
	  drupal_set_message(t('Failed to update daily predictions'), 'error');
	}
      }
    }

    $list_new = array();
    $labels = array();
    $start = chpuser_datetime_utc_to_usertimezone('now');
    $stop = chpuser_datetime_utc_to_usertimezone('now');
    $start->modify(sprintf("-%d days", $start->format("N") - 1));
    $stop->modify(sprintf("-%d days", $stop->format("N") - 1));
    $start->modify('-4 weeks');
    $future = array();
    $future[] = $start->format("N");
    $start->modify('-4 weeks');
    $future[] = $start->format("N");
    $start->modify('-4 weeks');
    $future[] = $start->format("N");
    $start->modify('-4 weeks');
    $future[] = $start->format("N");
    $start->modify('+15 weeks');
    if ($lot_index == 0) { // First round is the aggregate
      $basename = "weekticket_".$company_id."_".$property_id;
    } else {
      $basename = "weekticket_".$company_id."_".$property_id."_".$loop_lot_id;
    }
    $basename .= "_".$stop->format("Y_W");
    $filename = $basename.".png";
    $image_weekly = $base_path."tmpimage/".$filename;

    if (file_exists(ltrim($image_weekly, '/'))) {
      $next_week = $predictions['NextWeek'];
    } else {
      // Run last 16 weeks as part of weekly activity
      for ($i = 0; $i < 16; $i++) {
	if ($lot_index == 0) { // First round is the aggregate
	  $entries = chdban_get_ticket_stats
	    ($company_id, $property_id,
	     FALSE, 'LOT',
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	} else {
	  $entries = chdban_get_ticket_stats
	    ($company_id, $property_id,
	     TRUE, $loop_lot_id,
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	}
	$list_new[] = count($entries);
	$labels[] = $start->format("W");
	$start->modify('-1 week');
	$stop->modify('-1 week');
      } //print_r($list_new);

      // Find average of all samples
      $next_week_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if ($punctures[$index] > 0.0) {
	  $n++;
	}
	$next_week_average += $value * $punctures[$index];
      }
      if ($n > 0) {
	$next_week_average /= $n;
      }

      // Find average of next particular week among all samples
      $next_week_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	foreach ($future as $tmp) {
	  if (strpos($labels[$index], $tmp) !== FALSE) {
	    if ($punctures[$index] > 0.0) {
	      $n++;
	    }
	    $next_week_particular_average += $value * $punctures[$index];
	  }
	}
      }
      if ($n > 0) {
	$next_week_particular_average /= $n;
      }

      // Find Lyaponov based filtered avarage
      $next_week_lyaponov_average = $next_week_average;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      for ($i = 0; $i < $lyaponov_iterations; $i++) {
	$deviations = array();
	foreach ($list_new as $index => $value) {
	  $deviations[$index] = $value - $next_week_lyaponov_average;
	}
	foreach ($deviations as $index => $value) {
	  if ($value == max($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	  if ($value == min($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	}
	$next_week_lyaponov_average = 0;
	$n = 0;
	foreach ($list_new as $index => $value) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_week_lyaponov_average += $value * $punctures[$index];
	}
	if ($n > 0) {
	  $next_week_lyaponov_average /= $n;
	}
      }

      $next_week = ($next_week_average +
		    $next_week_particular_average +
		    $next_week_lyaponov_average) / 3;
      $next_week = round($next_week);

      $list_new = array_reverse($list_new);
      $labels = array_reverse($labels);

      $max_scale = max($list_new);
      //drupal_set_message($max_scale);
      if ($max_scale == 0) {
	$max_scale = 1;
      }

      if ($lot_index == 0) { // First round is the aggregate
	$chart_title = t('Weekly ticket registrations at property');
	$chart_id = 'weekly_ticket_chart_aggregated';
      } else {
	$chart_title = t('Weekly ticket registrations at ').$policy['data']['Alias'];
	$chart_id = 'weekly_ticket_chart_'.$loop_lot_id;
      }

      // Weekly activity graph
      $chart = array
	(// Mandatory
	 '#chart_id' => $chart_id,
	 '#type' => CHART_TYPE_BAR_V,
	 '#data' => array(0 => $list_new),
	 // Optional
	 '#title' => chart_title($chart_title),
	 '#size' => chart_size(800, 200),
	 '#adjust_resolution' => array('#adjust' => TRUE, '#max' => $max_scale),
	 '#grid_lines' => chart_grid_lines(100, 10),
	 '#data_colors' => array(0 => chart_unique_color('new')),
	 '#mixed_axis_labels' =>
	 array(CHART_AXIS_Y_LEFT =>
	       array(0 => array(chart_mixed_axis_range_label(0, $max_scale))),
	       CHART_AXIS_X_BOTTOM =>
	       array(0 => array_map(chart_mixed_axis_label, $labels),
		     1 => array(chart_mixed_axis_label(t('ISO Week number'), 50)))),
	 '#bar_size' => chart_bar_size(30, 12),
	 ); //print_r($chart);print('<p>');

      chart_copy($chart, $basename);
      $source = file_create_path('charts').'/'.$filename;
      //drupal_set_message('SOURCE:'.$source);
      $dest = ltrim($base_path, '/').'tmpimage/'.$filename;
      //drupal_set_message('DEST:'.$dest);
      copy($source, $dest);
      unlink($source);

      if ($lot_index == 0) { // First round is the aggregate
	if (chdbtick_update_prediction_weekly
	    ($company_id, $property_id,
	     'All',
	     $next_week)) {
	  drupal_set_message(t('Failed to update weekly predictions'), 'error');
	}
      } else {
	if (chdbtick_update_prediction_weekly
	    ($company_id, $property_id,
	     $loop_lot_id,
	     $next_week)) {
	  drupal_set_message(t('Failed to update weekly predictions'), 'error');
	}
      }
    }

    $list_new = array();
    $labels = array();
    $start = chpuser_datetime_utc_to_usertimezone('now');
    $stop = chpuser_datetime_utc_to_usertimezone('now');
    $start->modify(sprintf("-%d days", $start->format("j") - 1));
    $stop->modify(sprintf("-%d days", $stop->format("j") - 1));
    $start->modify('+1 month');
    $future = $start->format("F");
    $start->modify('-2 months');
    if ($lot_index == 0) { // First round is the aggregate
      $basename = "monticket_".$company_id."_".$property_id;
    } else {
      $basename = "monticket_".$company_id."_".$property_id."_".$loop_lot_id;
    }
    $basename .= "_".$stop->format("Y_m");
    $filename = $basename.".png";
    $image_monthly = $base_path."tmpimage/".$filename;

    if (file_exists(ltrim($image_monthly, '/'))) {
      $next_month = $predictions['NextMonth'];
    } else {
      // Run last 12 months as part of monthly activity
      for ($i = 0; $i < 12; $i++) {
	if ($lot_index == 0) { // First round is the aggregate
	  $entries = chdban_get_ticket_stats
	    ($company_id, $property_id,
	     FALSE, 'LOT',
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	} else {
	  $entries = chdban_get_ticket_stats
	    ($company_id, $property_id,
	     TRUE, $loop_lot_id,
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	}
	$list_new[] = count($entries);
	$labels[] = $start->format("F");
	$start->modify('-1 month');
	$stop->modify('-1 month');
      } //print_r($list_new);

      // Find average of all samples
      $next_month_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if ($punctures[$index] > 0.0) {
	  $n++;
	}
	$next_month_average += $value * $punctures[$index];
      }
      if ($n > 0) {
	$next_month_average /= $n;
      }

      // Find average of next particular month among all samples
      $next_month_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if (strpos($labels[$index], $future) !== FALSE) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_month_particular_average += $value * $punctures[$index];
	}
      }
      if ($n > 0) {
	$next_month_particular_average /= $n;
      }

      // Find Lyaponov based filtered avarage
      $next_month_lyaponov_average = $next_month_average;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      for ($i = 0; $i < $lyaponov_iterations; $i++) {
	$deviations = array();
	foreach ($list_new as $index => $value) {
	  $deviations[$index] = $value - $next_month_lyaponov_average;
	}
	foreach ($deviations as $index => $value) {
	  if ($value == max($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	  if ($value == min($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	}
	$next_month_lyaponov_average = 0;
	$n = 0;
	foreach ($list_new as $index => $value) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_month_lyaponov_average += $value * $punctures[$index];
	}
	if ($n > 0) {
	  $next_month_lyaponov_average /= $n;
	}
      }

      if ($next_month_particular_average > 0) {
	$next_month = ($next_month_average +
		       $next_month_particular_average +
		       $next_month_lyaponov_average) / 3;
      } else {
	$next_month = ($next_month_average +
		       $next_month_lyaponov_average) / 2;
      }
      $next_month = round($next_month);

      $list_new = array_reverse($list_new);
      $labels = array_reverse($labels);

      $max_scale = max($list_new);
      //drupal_set_message($max_scale);
      if ($max_scale == 0) {
	$max_scale = 1;
      }

      if ($lot_index == 0) { // First round is the aggregate
	$chart_title = t('Monthly ticket registrations at property');
	$chart_id = 'monthly_ticket_chart_aggregated';
      } else {
	$chart_title = t('Monthly ticket registrations at ').$policy['data']['Alias'];
	$chart_id = 'monthly_ticket_chart_'.$loop_lot_id;
      }

      // Monthly activity graph
      $chart = array
	(// Mandatory
	 '#chart_id' => $chart_id,
	 '#type' => CHART_TYPE_BAR_V,
	 '#data' => array(0 => $list_new),
	 // Optional
	 '#title' => chart_title($chart_title),
	 '#size' => chart_size(800, 200),
	 '#adjust_resolution' => array('#adjust' => TRUE, '#max' => $max_scale),
	 '#grid_lines' => chart_grid_lines(100, 10),
	 '#data_colors' => array(0 => chart_unique_color('new')),
	 '#mixed_axis_labels' =>
	 array(CHART_AXIS_Y_LEFT =>
	       array(0 => array(chart_mixed_axis_range_label(0, $max_scale))),
	       CHART_AXIS_X_BOTTOM =>
	       array(0 => array_map(chart_mixed_axis_label, $labels),
		     1 => array(chart_mixed_axis_label(t('Month'), 50)))),
	 '#bar_size' => chart_bar_size(42, 15),
	 ); //print_r($chart);print('<p>');

      chart_copy($chart, $basename);
      $source = file_create_path('charts').'/'.$filename;
      //drupal_set_message('SOURCE:'.$source);
      $dest = ltrim($base_path, '/').'tmpimage/'.$filename;
      //drupal_set_message('DEST:'.$dest);
      copy($source, $dest);
      unlink($source);

      if ($lot_index == 0) { // First round is the aggregate
	if (chdbtick_update_prediction_monthly
	    ($company_id, $property_id,
	     'All',
	     $next_month)) {
	  drupal_set_message(t('Failed to update monthly predictions'), 'error');
	}
      } else {
	if (chdbtick_update_prediction_monthly
	    ($company_id, $property_id,
	     $loop_lot_id,
	     $next_month)) {
	  drupal_set_message(t('Failed to update monthly predictions'), 'error');
	}
      }
    }

    if (0) { // Debug
      $rows[] = array('<b>Predictions based on flat average</b>',
		      round($today_average),
		      round($next_day_average),
		      round($next_week_average),
		      round($next_month_average));

      if ($next_month_particular_average > 0) {
	$next_month_particular_average = round($next_month_particular_average);
      } else {
	$next_month_particular_average = t('Missing data');
      }
      $rows[] = array('<b>Predictions based on particular avarage</b>',
		      round($today_particular_average),
		      round($next_day_particular_average),
		      round($next_week_particular_average),
		      $next_month_particular_average);

      $rows[] = array('<b>Predictions based on Lyaponov filterd average</b>',
		      round($today_lyaponov_average),
		      round($next_day_lyaponov_average),
		      round($next_week_lyaponov_average),
		      round($next_month_lyaponov_average));

      $rows[] = array('<b>Final predictions</b>',
		      round($current_day),
		      round($next_day),
		      round($next_week),
		      round($next_month));

      $header = array('', 'Today', 'Tomorrow', 'Next week', 'Next month');
      $form['property']['Debug'] = array('#value' => theme_table($header, $rows),
					 '#suffix' => '<p><p>');
    }

    if ($lot_index == 0) { // First round is the aggregate
      $final_data['Aggregate']['current_day'] = $current_day;
      $final_data['Aggregate']['next_day'] = $next_day;
      $final_data['Aggregate']['next_week'] = $next_week;
      $final_data['Aggregate']['next_month'] = $next_month;
      $final_data['Aggregate']['image_daily'] = $image_daily;
      $final_data['Aggregate']['image_weekly'] = $image_weekly;
      $final_data['Aggregate']['image_monthly'] = $image_monthly;
    } else {
      $final_data[$loop_lot_id]['current_day'] = $current_day;
      $final_data[$loop_lot_id]['next_day'] = $next_day;
      $final_data[$loop_lot_id]['next_week'] = $next_week;
      $final_data[$loop_lot_id]['next_month'] = $next_month;
      $final_data[$loop_lot_id]['alias'] = $policy['data']['Alias'];
      $final_data[$loop_lot_id]['image_daily'] = $image_daily;
      $final_data[$loop_lot_id]['image_weekly'] = $image_weekly;
      $final_data[$loop_lot_id]['image_monthly'] = $image_monthly;
    }

    if ($lot_index > 0) { // First round is the aggregate
      $policy = next($policies);
    }
  } // Permit graphs and predictions

  return $final_data;
}



function chpan_boot_graph_predictions($company_id, $property_id, $policies) {
  global $base_path;

  $final_data = array();

  // Permit graphs and predictions
  $lyaponov_iterations = 3;
  $policy = current($policies);
  $num_policies = count($policies);

  for ($lot_index = 0; $lot_index < $num_policies + 1; $lot_index++) {
    $loop_lot_id = $policy['data']['LotId'];

    if ($lot_index == 0) { // First round is the aggregate
      $predictions =
	chdbboot_get_predictions
	($company_id, $property_id, TRUE, 'All');
    } else {
      $predictions =
	chdbboot_get_predictions
	($company_id, $property_id, TRUE, $loop_lot_id);
    }

    // Collect data
    $list_new = array();
    $labels = array();
    $start = chpuser_datetime_utc_to_usertimezone('now');
    $stop = chpuser_datetime_utc_to_usertimezone('now');
    $today = $start->format("D");
    $start->modify('+1 day');
    $future = $start->format("D");
    $start->modify('-2 days');
    if ($lot_index == 0) { // First round is the aggregate
      $basename = "dayboot_".$company_id."_".$property_id;
    } else {
      $basename = "dayboot_".$company_id."_".$property_id."_".$loop_lot_id;
    }
    $basename .= "_".$stop->format("Y_m_d");
    $filename = $basename.".png";
    $image_daily = $base_path."tmpimage/".$filename;

    if (file_exists(ltrim($image_daily, '/'))) {
      $current_day = $predictions['Today'];
      $next_day = $predictions['Tomorrow'];
    } else {
      // Run last 21 days as part of daily activity
      for ($i = 0; $i < 14; $i++) {
	if ($lot_index == 0) { // First round is the aggregate
	  $entries = chdban_get_boot_stats
	    ($company_id, $property_id,
	     FALSE, 'LOT',
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	} else {
	  $entries = chdban_get_boot_stats
	    ($company_id, $property_id,
	     TRUE, $loop_lot_id,
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	} //print_r($entries);print('<p>');
	$list_new[] = count($entries);
	$labels[] = $start->format("D d");
	$start->modify('-1 day');
	$stop->modify('-1 day');
      } //print_r($list_new);

      // Find average of all samples
      $next_day_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if ($punctures[$index] > 0.0) {
	  $n++;
	}
	$next_day_average += $value * $punctures[$index];
      }
      if ($n > 0) {
	$next_day_average /= $n;
      }
      $today_average = $next_day_average;

      // Find average of particular today among all samples
      $today_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if (strpos($labels[$index], $today) !== FALSE) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $today_particular_average += $value * $punctures[$index];
	}
      }
      if ($n > 0) {
	$today_particular_average /= $n;
      }

      // Find average of next particular day among all samples
      $next_day_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if (strpos($labels[$index], $future) !== FALSE) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_day_particular_average += $value * $punctures[$index];
	}
      }
      if ($n > 0) {
	$next_day_particular_average /= $n;
      }

      // Find Lyaponov based filtered avarage
      $next_day_lyaponov_average = $next_day_average;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      for ($i = 0; $i < $lyaponov_iterations; $i++) {
	$deviations = array();
	foreach ($list_new as $index => $value) {
	  $deviations[$index] = $value - $next_day_lyaponov_average;
	}
	foreach ($deviations as $index => $value) {
	  if ($value == max($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	  if ($value == min($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	}
	$next_day_lyaponov_average = 0;
	$n = 0;
	foreach ($list_new as $index => $value) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_day_lyaponov_average += $value * $punctures[$index];
	}
	if ($n > 0) {
	  $next_day_lyaponov_average /= $n;
	}
      }
      $today_lyaponov_average = $next_day_lyaponov_average;

      $current_day = ($today_average +
		      $today_particular_average +
		      $today_lyaponov_average) / 3;
      $current_day = round($current_day);

      $next_day = ($next_day_average +
		   $next_day_particular_average +
		   $next_day_lyaponov_average) / 3;
      $next_day = round($next_day);

      $list_new = array_reverse($list_new);
      $labels = array_reverse($labels);

      $max_scale = max($list_new);
      //drupal_set_message($max_scale);
      if ($max_scale == 0) {
	$max_scale = 1;
      }

      if ($lot_index == 0) { // First round is the aggregate
	$chart_title = t('Daily boot registrations at property');
	$chart_id = 'daily_boot_chart_aggregated';
      } else {
	$chart_title = t('Daily boot registrations at ').$policy['data']['Alias'];
	$chart_id = 'daily_boot_chart_'.$loop_lot_id;
      }

      // Daily activity graph
      $chart = array
	(// Mandatory
	 '#chart_id' => $chart_id,
	 '#type' => CHART_TYPE_BAR_V,
	 '#data' => array(0 => $list_new),
	 // Optional
	 '#title' => chart_title($chart_title),
	 '#size' => chart_size(800, 200),
	 '#adjust_resolution' => array('#adjust' => TRUE, '#max' => $max_scale),
	 '#grid_lines' => chart_grid_lines(100, 10),
	 '#data_colors' => array(0 => chart_unique_color('new')),
	 '#mixed_axis_labels' =>
	 array(CHART_AXIS_Y_LEFT =>
	       array(0 => array(chart_mixed_axis_range_label(0, $max_scale))),
	       CHART_AXIS_X_BOTTOM =>
	       array(0 => array_map(chart_mixed_axis_label, $labels),
		     1 => array(chart_mixed_axis_label(t('Day of month'), 50)))),
	 '#bar_size' => chart_bar_size(20, 30),
	 ); //print_r($chart);print('<p>');

      chart_copy($chart, $basename);
      $source = file_create_path('charts').'/'.$filename;
      //drupal_set_message('SOURCE:'.$source);
      $dest = ltrim($base_path, '/').'tmpimage/'.$filename;
      //drupal_set_message('DEST:'.$dest);
      copy($source, $dest);
      unlink($source);

      if ($lot_index == 0) { // First round is the aggregate
	if (chdbboot_update_prediction_daily
	    ($company_id, $property_id,
	     'All',
	     $current_day,
	     $next_day)) {
	  drupal_set_message(t('Failed to update daily predictions'), 'error');
	}
      } else {
	if (chdbboot_update_prediction_daily
	    ($company_id, $property_id,
	     $loop_lot_id,
	     $current_day,
	     $next_day)) {
	  drupal_set_message(t('Failed to update daily predictions'), 'error');
	}
      }
    }

    $list_new = array();
    $labels = array();
    $start = chpuser_datetime_utc_to_usertimezone('now');
    $stop = chpuser_datetime_utc_to_usertimezone('now');
    $start->modify(sprintf("-%d days", $start->format("N") - 1));
    $stop->modify(sprintf("-%d days", $stop->format("N") - 1));
    $start->modify('-4 weeks');
    $future = array();
    $future[] = $start->format("N");
    $start->modify('-4 weeks');
    $future[] = $start->format("N");
    $start->modify('-4 weeks');
    $future[] = $start->format("N");
    $start->modify('-4 weeks');
    $future[] = $start->format("N");
    $start->modify('+15 weeks');
    if ($lot_index == 0) { // First round is the aggregate
      $basename = "weekboot_".$company_id."_".$property_id;
    } else {
      $basename = "weekboot_".$company_id."_".$property_id."_".$loop_lot_id;
    }
    $basename .= "_".$stop->format("Y_W");
    $filename = $basename.".png";
    $image_weekly = $base_path."tmpimage/".$filename;

    if (file_exists(ltrim($image_weekly, '/'))) {
      $next_week = $predictions['NextWeek'];
    } else {
      // Run last 16 weeks as part of weekly activity
      for ($i = 0; $i < 16; $i++) {
	if ($lot_index == 0) { // First round is the aggregate
	  $entries = chdban_get_boot_stats
	    ($company_id, $property_id,
	     FALSE, 'LOT',
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	} else {
	  $entries = chdban_get_boot_stats
	    ($company_id, $property_id,
	     TRUE, $loop_lot_id,
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	}
	$list_new[] = count($entries);
	$labels[] = $start->format("W");
	$start->modify('-1 week');
	$stop->modify('-1 week');
      } //print_r($list_new);

      // Find average of all samples
      $next_week_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if ($punctures[$index] > 0.0) {
	  $n++;
	}
	$next_week_average += $value * $punctures[$index];
      }
      if ($n > 0) {
	$next_week_average /= $n;
      }

      // Find average of next particular week among all samples
      $next_week_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	foreach ($future as $tmp) {
	  if (strpos($labels[$index], $tmp) !== FALSE) {
	    if ($punctures[$index] > 0.0) {
	      $n++;
	    }
	    $next_week_particular_average += $value * $punctures[$index];
	  }
	}
      }
      if ($n > 0) {
	$next_week_particular_average /= $n;
      }

      // Find Lyaponov based filtered avarage
      $next_week_lyaponov_average = $next_week_average;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      for ($i = 0; $i < $lyaponov_iterations; $i++) {
	$deviations = array();
	foreach ($list_new as $index => $value) {
	  $deviations[$index] = $value - $next_week_lyaponov_average;
	}
	foreach ($deviations as $index => $value) {
	  if ($value == max($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	  if ($value == min($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	}
	$next_week_lyaponov_average = 0;
	$n = 0;
	foreach ($list_new as $index => $value) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_week_lyaponov_average += $value * $punctures[$index];
	}
	if ($n > 0) {
	  $next_week_lyaponov_average /= $n;
	}
      }

      $next_week = ($next_week_average +
		    $next_week_particular_average +
		    $next_week_lyaponov_average) / 3;
      $next_week = round($next_week);

      $list_new = array_reverse($list_new);
      $labels = array_reverse($labels);

      $max_scale = max($list_new);
      //drupal_set_message($max_scale);
      if ($max_scale == 0) {
	$max_scale = 1;
      }

      if ($lot_index == 0) { // First round is the aggregate
	$chart_title = t('Weekly boot registrations at property');
	$chart_id = 'weekly_boot_chart_aggregated';
      } else {
	$chart_title = t('Weekly boot registrations at ').$policy['data']['Alias'];
	$chart_id = 'weekly_boot_chart_'.$loop_lot_id;
      }

      // Weekly activity graph
      $chart = array
	(// Mandatory
	 '#chart_id' => $chart_id,
	 '#type' => CHART_TYPE_BAR_V,
	 '#data' => array(0 => $list_new),
	 // Optional
	 '#title' => chart_title($chart_title),
	 '#size' => chart_size(800, 200),
	 '#adjust_resolution' => array('#adjust' => TRUE, '#max' => $max_scale),
	 '#grid_lines' => chart_grid_lines(100, 10),
	 '#data_colors' => array(0 => chart_unique_color('new')),
	 '#mixed_axis_labels' =>
	 array(CHART_AXIS_Y_LEFT =>
	       array(0 => array(chart_mixed_axis_range_label(0, $max_scale))),
	       CHART_AXIS_X_BOTTOM =>
	       array(0 => array_map(chart_mixed_axis_label, $labels),
		     1 => array(chart_mixed_axis_label(t('ISO Week number'), 50)))),
	 '#bar_size' => chart_bar_size(30, 12),
	 ); //print_r($chart);print('<p>');

      chart_copy($chart, $basename);
      $source = file_create_path('charts').'/'.$filename;
      //drupal_set_message('SOURCE:'.$source);
      $dest = ltrim($base_path, '/').'tmpimage/'.$filename;
      //drupal_set_message('DEST:'.$dest);
      copy($source, $dest);
      unlink($source);

      if ($lot_index == 0) { // First round is the aggregate
	if (chdbboot_update_prediction_weekly
	    ($company_id, $property_id,
	     'All',
	     $next_week)) {
	  drupal_set_message(t('Failed to update weekly predictions'), 'error');
	}
      } else {
	if (chdbboot_update_prediction_weekly
	    ($company_id, $property_id,
	     $loop_lot_id,
	     $next_week)) {
	  drupal_set_message(t('Failed to update weekly predictions'), 'error');
	}
      }
    }

    $list_new = array();
    $labels = array();
    $start = chpuser_datetime_utc_to_usertimezone('now');
    $stop = chpuser_datetime_utc_to_usertimezone('now');
    $start->modify(sprintf("-%d days", $start->format("j") - 1));
    $stop->modify(sprintf("-%d days", $stop->format("j") - 1));
    $start->modify('+1 month');
    $future = $start->format("F");
    $start->modify('-2 months');
    if ($lot_index == 0) { // First round is the aggregate
      $basename = "monboot_".$company_id."_".$property_id;
    } else {
      $basename = "monboot_".$company_id."_".$property_id."_".$loop_lot_id;
    }
    $basename .= "_".$stop->format("Y_m");
    $filename = $basename.".png";
    $image_monthly = $base_path."tmpimage/".$filename;

    if (file_exists(ltrim($image_monthly, '/'))) {
      $next_month = $predictions['NextMonth'];
    } else {
      // Run last 12 months as part of monthly activity
      for ($i = 0; $i < 12; $i++) {
	if ($lot_index == 0) { // First round is the aggregate
	  $entries = chdban_get_boot_stats
	    ($company_id, $property_id,
	     FALSE, 'LOT',
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	} else {
	  $entries = chdban_get_boot_stats
	    ($company_id, $property_id,
	     TRUE, $loop_lot_id,
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	}
	$list_new[] = count($entries);
	$labels[] = $start->format("F");
	$start->modify('-1 month');
	$stop->modify('-1 month');
      } //print_r($list_new);

      // Find average of all samples
      $next_month_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if ($punctures[$index] > 0.0) {
	  $n++;
	}
	$next_month_average += $value * $punctures[$index];
      }
      if ($n > 0) {
	$next_month_average /= $n;
      }

      // Find average of next particular month among all samples
      $next_month_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if (strpos($labels[$index], $future) !== FALSE) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_month_particular_average += $value * $punctures[$index];
	}
      }
      if ($n > 0) {
	$next_month_particular_average /= $n;
      }

      // Find Lyaponov based filtered avarage
      $next_month_lyaponov_average = $next_month_average;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      for ($i = 0; $i < $lyaponov_iterations; $i++) {
	$deviations = array();
	foreach ($list_new as $index => $value) {
	  $deviations[$index] = $value - $next_month_lyaponov_average;
	}
	foreach ($deviations as $index => $value) {
	  if ($value == max($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	  if ($value == min($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	}
	$next_month_lyaponov_average = 0;
	$n = 0;
	foreach ($list_new as $index => $value) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_month_lyaponov_average += $value * $punctures[$index];
	}
	if ($n > 0) {
	  $next_month_lyaponov_average /= $n;
	}
      }

      if ($next_month_particular_average > 0) {
	$next_month = ($next_month_average +
		       $next_month_particular_average +
		       $next_month_lyaponov_average) / 3;
      } else {
	$next_month = ($next_month_average +
		       $next_month_lyaponov_average) / 2;
      }
      $next_month = round($next_month);

      $list_new = array_reverse($list_new);
      $labels = array_reverse($labels);

      $max_scale = max($list_new);
      //drupal_set_message($max_scale);
      if ($max_scale == 0) {
	$max_scale = 1;
      }

      if ($lot_index == 0) { // First round is the aggregate
	$chart_title = t('Monthly boot registrations at property');
	$chart_id = 'monthly_boot_chart_aggregated';
      } else {
	$chart_title = t('Monthly boot registrations at ').$policy['data']['Alias'];
	$chart_id = 'monthly_boot_chart_'.$loop_lot_id;
      }

      // Monthly activity graph
      $chart = array
	(// Mandatory
	 '#chart_id' => $chart_id,
	 '#type' => CHART_TYPE_BAR_V,
	 '#data' => array(0 => $list_new),
	 // Optional
	 '#title' => chart_title($chart_title),
	 '#size' => chart_size(800, 200),
	 '#adjust_resolution' => array('#adjust' => TRUE, '#max' => $max_scale),
	 '#grid_lines' => chart_grid_lines(100, 10),
	 '#data_colors' => array(0 => chart_unique_color('new')),
	 '#mixed_axis_labels' =>
	 array(CHART_AXIS_Y_LEFT =>
	       array(0 => array(chart_mixed_axis_range_label(0, $max_scale))),
	       CHART_AXIS_X_BOTTOM =>
	       array(0 => array_map(chart_mixed_axis_label, $labels),
		     1 => array(chart_mixed_axis_label(t('Month'), 50)))),
	 '#bar_size' => chart_bar_size(42, 15),
	 ); //print_r($chart);print('<p>');

      chart_copy($chart, $basename);
      $source = file_create_path('charts').'/'.$filename;
      //drupal_set_message('SOURCE:'.$source);
      $dest = ltrim($base_path, '/').'tmpimage/'.$filename;
      //drupal_set_message('DEST:'.$dest);
      copy($source, $dest);
      unlink($source);

      if ($lot_index == 0) { // First round is the aggregate
	if (chdbboot_update_prediction_monthly
	    ($company_id, $property_id,
	     'All',
	     $next_month)) {
	  drupal_set_message(t('Failed to update monthly predictions'), 'error');
	}
      } else {
	if (chdbboot_update_prediction_monthly
	    ($company_id, $property_id,
	     $loop_lot_id,
	     $next_month)) {
	  drupal_set_message(t('Failed to update monthly predictions'), 'error');
	}
      }
    }

    if (0) { // Debug
      $rows[] = array('<b>Predictions based on flat average</b>',
		      round($today_average),
		      round($next_day_average),
		      round($next_week_average),
		      round($next_month_average));

      if ($next_month_particular_average > 0) {
	$next_month_particular_average = round($next_month_particular_average);
      } else {
	$next_month_particular_average = t('Missing data');
      }
      $rows[] = array('<b>Predictions based on particular avarage</b>',
		      round($today_particular_average),
		      round($next_day_particular_average),
		      round($next_week_particular_average),
		      $next_month_particular_average);

      $rows[] = array('<b>Predictions based on Lyaponov filterd average</b>',
		      round($today_lyaponov_average),
		      round($next_day_lyaponov_average),
		      round($next_week_lyaponov_average),
		      round($next_month_lyaponov_average));

      $rows[] = array('<b>Final predictions</b>',
		      round($current_day),
		      round($next_day),
		      round($next_week),
		      round($next_month));

      $header = array('', 'Today', 'Tomorrow', 'Next week', 'Next month');
      $form['property']['Debug'] = array('#value' => theme_table($header, $rows),
					 '#suffix' => '<p><p>');
    }

    if ($lot_index == 0) { // First round is the aggregate
      $final_data['Aggregate']['current_day'] = $current_day;
      $final_data['Aggregate']['next_day'] = $next_day;
      $final_data['Aggregate']['next_week'] = $next_week;
      $final_data['Aggregate']['next_month'] = $next_month;
      $final_data['Aggregate']['image_daily'] = $image_daily;
      $final_data['Aggregate']['image_weekly'] = $image_weekly;
      $final_data['Aggregate']['image_monthly'] = $image_monthly;
    } else {
      $final_data[$loop_lot_id]['current_day'] = $current_day;
      $final_data[$loop_lot_id]['next_day'] = $next_day;
      $final_data[$loop_lot_id]['next_week'] = $next_week;
      $final_data[$loop_lot_id]['next_month'] = $next_month;
      $final_data[$loop_lot_id]['alias'] = $policy['data']['Alias'];
      $final_data[$loop_lot_id]['image_daily'] = $image_daily;
      $final_data[$loop_lot_id]['image_weekly'] = $image_weekly;
      $final_data[$loop_lot_id]['image_monthly'] = $image_monthly;
    }

    if ($lot_index > 0) { // First round is the aggregate
      $policy = next($policies);
    }
  } // Permit graphs and predictions
  return $final_data;
}



function chpan_valet_graph_predictions($company_id, $property_id, $policies) {
  global $base_path;

  $final_data = array();

  // Permit graphs and predictions
  $lyaponov_iterations = 3;
  $policy = current($policies);
  $num_policies = count($policies);

  for ($lot_index = 0; $lot_index < $num_policies + 1; $lot_index++) {
    $loop_lot_id = $policy['data']['LotId'];

    if ($lot_index == 0) { // First round is the aggregate
      $predictions =
	chdbvalet_get_predictions
	($company_id, $property_id, TRUE, 'All');
    } else {
      $predictions =
	chdbvalet_get_predictions
	($company_id, $property_id, TRUE, $loop_lot_id);
    }

    // Collect data
    $list_new = array();
    $labels = array();
    $start = chpuser_datetime_utc_to_usertimezone('now');
    $stop = chpuser_datetime_utc_to_usertimezone('now');
    $today = $start->format("D");
    $start->modify('+1 day');
    $future = $start->format("D");
    $start->modify('-2 days');
    if ($lot_index == 0) { // First round is the aggregate
      $basename = "dayvalet_".$company_id."_".$property_id;
    } else {
      $basename = "dayvalet_".$company_id."_".$property_id."_".$loop_lot_id;
    }
    $basename .= "_".$stop->format("Y_m_d");
    $filename = $basename.".png";
    $image_daily = $base_path."tmpimage/".$filename;

    if (file_exists(ltrim($image_daily, '/'))) {
      $current_day = $predictions['Today'];
      $next_day = $predictions['Tomorrow'];
    } else {
      // Run last 21 days as part of daily activity
      for ($i = 0; $i < 14; $i++) {
	if ($lot_index == 0) { // First round is the aggregate
	  $entries = chdban_get_valet_stats
	    ($company_id, $property_id,
	     FALSE, 'LOT',
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	} else {
	  $entries = chdban_get_valet_stats
	    ($company_id, $property_id,
	     TRUE, $loop_lot_id,
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	} //print_r($entries);print('<p>');
	$list_new[] = count($entries);
	$labels[] = $start->format("D d");
	$start->modify('-1 day');
	$stop->modify('-1 day');
      } //print_r($list_new);

      // Find average of all samples
      $next_day_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if ($punctures[$index] > 0.0) {
	  $n++;
	}
	$next_day_average += $value * $punctures[$index];
      }
      if ($n > 0) {
	$next_day_average /= $n;
      }
      $today_average = $next_day_average;

      // Find average of particular today among all samples
      $today_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if (strpos($labels[$index], $today) !== FALSE) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $today_particular_average += $value * $punctures[$index];
	}
      }
      if ($n > 0) {
	$today_particular_average /= $n;
      }

      // Find average of next particular day among all samples
      $next_day_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if (strpos($labels[$index], $future) !== FALSE) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_day_particular_average += $value * $punctures[$index];
	}
      }
      if ($n > 0) {
	$next_day_particular_average /= $n;
      }

      // Find Lyaponov based filtered avarage
      $next_day_lyaponov_average = $next_day_average;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      for ($i = 0; $i < $lyaponov_iterations; $i++) {
	$deviations = array();
	foreach ($list_new as $index => $value) {
	  $deviations[$index] = $value - $next_day_lyaponov_average;
	}
	foreach ($deviations as $index => $value) {
	  if ($value == max($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	  if ($value == min($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	}
	$next_day_lyaponov_average = 0;
	$n = 0;
	foreach ($list_new as $index => $value) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_day_lyaponov_average += $value * $punctures[$index];
	}
	if ($n > 0) {
	  $next_day_lyaponov_average /= $n;
	}
      }
      $today_lyaponov_average = $next_day_lyaponov_average;

      $current_day = ($today_average +
		      $today_particular_average +
		      $today_lyaponov_average) / 3;
      $current_day = round($current_day);

      $next_day = ($next_day_average +
		   $next_day_particular_average +
		   $next_day_lyaponov_average) / 3;
      $next_day = round($next_day);

      $list_new = array_reverse($list_new);
      $labels = array_reverse($labels);

      $max_scale = max($list_new);
      //drupal_set_message($max_scale);
      if ($max_scale == 0) {
	$max_scale = 1;
      }

      if ($lot_index == 0) { // First round is the aggregate
	$chart_title = t('Daily valet registrations at property');
	$chart_id = 'daily_valet_chart_aggregated';
      } else {
	$chart_title = t('Daily valet registrations at ').$policy['data']['Alias'];
	$chart_id = 'daily_valet_chart_'.$loop_lot_id;
      }

      // Daily activity graph
      $chart = array
	(// Mandatory
	 '#chart_id' => $chart_id,
	 '#type' => CHART_TYPE_BAR_V,
	 '#data' => array(0 => $list_new),
	 // Optional
	 '#title' => chart_title($chart_title),
	 '#size' => chart_size(800, 200),
	 '#adjust_resolution' => array('#adjust' => TRUE, '#max' => $max_scale),
	 '#grid_lines' => chart_grid_lines(100, 10),
	 '#data_colors' => array(0 => chart_unique_color('new')),
	 '#mixed_axis_labels' =>
	 array(CHART_AXIS_Y_LEFT =>
	       array(0 => array(chart_mixed_axis_range_label(0, $max_scale))),
	       CHART_AXIS_X_BOTTOM =>
	       array(0 => array_map(chart_mixed_axis_label, $labels),
		     1 => array(chart_mixed_axis_label(t('Day of month'), 50)))),
	 '#bar_size' => chart_bar_size(20, 30),
	 ); //print_r($chart);print('<p>');

      chart_copy($chart, $basename);
      $source = file_create_path('charts').'/'.$filename;
      //drupal_set_message('SOURCE:'.$source);
      $dest = ltrim($base_path, '/').'tmpimage/'.$filename;
      //drupal_set_message('DEST:'.$dest);
      copy($source, $dest);
      unlink($source);

      if ($lot_index == 0) { // First round is the aggregate
	if (chdbvalet_update_prediction_daily
	    ($company_id, $property_id,
	     'All',
	     $current_day,
	     $next_day)) {
	  drupal_set_message(t('Failed to update daily predictions'), 'error');
	}
      } else {
	if (chdbvalet_update_prediction_daily
	    ($company_id, $property_id,
	     $loop_lot_id,
	     $current_day,
	     $next_day)) {
	  drupal_set_message(t('Failed to update daily predictions'), 'error');
	}
      }
    }

    $list_new = array();
    $labels = array();
    $start = chpuser_datetime_utc_to_usertimezone('now');
    $stop = chpuser_datetime_utc_to_usertimezone('now');
    $start->modify(sprintf("-%d days", $start->format("N") - 1));
    $stop->modify(sprintf("-%d days", $stop->format("N") - 1));
    $start->modify('-4 weeks');
    $future = array();
    $future[] = $start->format("N");
    $start->modify('-4 weeks');
    $future[] = $start->format("N");
    $start->modify('-4 weeks');
    $future[] = $start->format("N");
    $start->modify('-4 weeks');
    $future[] = $start->format("N");
    $start->modify('+15 weeks');
    if ($lot_index == 0) { // First round is the aggregate
      $basename = "weekvalet_".$company_id."_".$property_id;
    } else {
      $basename = "weekvalet_".$company_id."_".$property_id."_".$loop_lot_id;
    }
    $basename .= "_".$stop->format("Y_W");
    $filename = $basename.".png";
    $image_weekly = $base_path."tmpimage/".$filename;

    if (file_exists(ltrim($image_weekly, '/'))) {
      $next_week = $predictions['NextWeek'];
    } else {
      // Run last 16 weeks as part of weekly activity
      for ($i = 0; $i < 16; $i++) {
	if ($lot_index == 0) { // First round is the aggregate
	  $entries = chdban_get_valet_stats
	    ($company_id, $property_id,
	     FALSE, 'LOT',
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	} else {
	  $entries = chdban_get_valet_stats
	    ($company_id, $property_id,
	     TRUE, $loop_lot_id,
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	}
	$list_new[] = count($entries);
	$labels[] = $start->format("W");
	$start->modify('-1 week');
	$stop->modify('-1 week');
      } //print_r($list_new);

      // Find average of all samples
      $next_week_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if ($punctures[$index] > 0.0) {
	  $n++;
	}
	$next_week_average += $value * $punctures[$index];
      }
      if ($n > 0) {
	$next_week_average /= $n;
      }

      // Find average of next particular week among all samples
      $next_week_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	foreach ($future as $tmp) {
	  if (strpos($labels[$index], $tmp) !== FALSE) {
	    if ($punctures[$index] > 0.0) {
	      $n++;
	    }
	    $next_week_particular_average += $value * $punctures[$index];
	  }
	}
      }
      if ($n > 0) {
	$next_week_particular_average /= $n;
      }

      // Find Lyaponov based filtered avarage
      $next_week_lyaponov_average = $next_week_average;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      for ($i = 0; $i < $lyaponov_iterations; $i++) {
	$deviations = array();
	foreach ($list_new as $index => $value) {
	  $deviations[$index] = $value - $next_week_lyaponov_average;
	}
	foreach ($deviations as $index => $value) {
	  if ($value == max($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	  if ($value == min($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	}
	$next_week_lyaponov_average = 0;
	$n = 0;
	foreach ($list_new as $index => $value) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_week_lyaponov_average += $value * $punctures[$index];
	}
	if ($n > 0) {
	  $next_week_lyaponov_average /= $n;
	}
      }

      $next_week = ($next_week_average +
		    $next_week_particular_average +
		    $next_week_lyaponov_average) / 3;
      $next_week = round($next_week);

      $list_new = array_reverse($list_new);
      $labels = array_reverse($labels);

      $max_scale = max($list_new);
      //drupal_set_message($max_scale);
      if ($max_scale == 0) {
	$max_scale = 1;
      }

      if ($lot_index == 0) { // First round is the aggregate
	$chart_title = t('Weekly valet registrations at property');
	$chart_id = 'weekly_valet_chart_aggregated';
      } else {
	$chart_title = t('Weekly valet registrations at ').$policy['data']['Alias'];
	$chart_id = 'weekly_valet_chart_'.$loop_lot_id;
      }

      // Weekly activity graph
      $chart = array
	(// Mandatory
	 '#chart_id' => $chart_id,
	 '#type' => CHART_TYPE_BAR_V,
	 '#data' => array(0 => $list_new),
	 // Optional
	 '#title' => chart_title($chart_title),
	 '#size' => chart_size(800, 200),
	 '#adjust_resolution' => array('#adjust' => TRUE, '#max' => $max_scale),
	 '#grid_lines' => chart_grid_lines(100, 10),
	 '#data_colors' => array(0 => chart_unique_color('new')),
	 '#mixed_axis_labels' =>
	 array(CHART_AXIS_Y_LEFT =>
	       array(0 => array(chart_mixed_axis_range_label(0, $max_scale))),
	       CHART_AXIS_X_BOTTOM =>
	       array(0 => array_map(chart_mixed_axis_label, $labels),
		     1 => array(chart_mixed_axis_label(t('ISO Week number'), 50)))),
	 '#bar_size' => chart_bar_size(30, 12),
	 ); //print_r($chart);print('<p>');

      chart_copy($chart, $basename);
      $source = file_create_path('charts').'/'.$filename;
      //drupal_set_message('SOURCE:'.$source);
      $dest = ltrim($base_path, '/').'tmpimage/'.$filename;
      //drupal_set_message('DEST:'.$dest);
      copy($source, $dest);
      unlink($source);

      if ($lot_index == 0) { // First round is the aggregate
	if (chdbvalet_update_prediction_weekly
	    ($company_id, $property_id,
	     'All',
	     $next_week)) {
	  drupal_set_message(t('Failed to update weekly predictions'), 'error');
	}
      } else {
	if (chdbvalet_update_prediction_weekly
	    ($company_id, $property_id,
	     $loop_lot_id,
	     $next_week)) {
	  drupal_set_message(t('Failed to update weekly predictions'), 'error');
	}
      }
    }

    $list_new = array();
    $labels = array();
    $start = chpuser_datetime_utc_to_usertimezone('now');
    $stop = chpuser_datetime_utc_to_usertimezone('now');
    $start->modify(sprintf("-%d days", $start->format("j") - 1));
    $stop->modify(sprintf("-%d days", $stop->format("j") - 1));
    $start->modify('+1 month');
    $future = $start->format("F");
    $start->modify('-2 months');
    if ($lot_index == 0) { // First round is the aggregate
      $basename = "monvalet_".$company_id."_".$property_id;
    } else {
      $basename = "monvalet_".$company_id."_".$property_id."_".$loop_lot_id;
    }
    $basename .= "_".$stop->format("Y_m");
    $filename = $basename.".png";
    $image_monthly = $base_path."tmpimage/".$filename;

    if (file_exists(ltrim($image_monthly, '/'))) {
      $next_month = $predictions['NextMonth'];
    } else {
      // Run last 12 months as part of monthly activity
      for ($i = 0; $i < 12; $i++) {
	if ($lot_index == 0) { // First round is the aggregate
	  $entries = chdban_get_valet_stats
	    ($company_id, $property_id,
	     FALSE, 'LOT',
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	} else {
	  $entries = chdban_get_valet_stats
	    ($company_id, $property_id,
	     TRUE, $loop_lot_id,
	     $start->format("Y-m-d 00:00:00"),
	     $stop->format("Y-m-d 00:00:00"),
	     TRUE, TRUE);
	}
	$list_new[] = count($entries);
	$labels[] = $start->format("F");
	$start->modify('-1 month');
	$stop->modify('-1 month');
      } //print_r($list_new);

      // Find average of all samples
      $next_month_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if ($punctures[$index] > 0.0) {
	  $n++;
	}
	$next_month_average += $value * $punctures[$index];
      }
      if ($n > 0) {
	$next_month_average /= $n;
      }

      // Find average of next particular month among all samples
      $next_month_particular_average = 0;
      $n = 0;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      foreach ($list_new as $index => $value) {
	if (strpos($labels[$index], $future) !== FALSE) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_month_particular_average += $value * $punctures[$index];
	}
      }
      if ($n > 0) {
	$next_month_particular_average /= $n;
      }

      // Find Lyaponov based filtered avarage
      $next_month_lyaponov_average = $next_month_average;
      $punctures = array();
      foreach ($list_new as $index => $value) {
	$punctures[$index] = 1.0;
      }
      // Filter out zero-entries (not in service)
      foreach ($list_new as $index => $value) {
	if ($value == 0) {
	  $punctures[$index] = 0.0;
	}
      }
      // Iterate
      for ($i = 0; $i < $lyaponov_iterations; $i++) {
	$deviations = array();
	foreach ($list_new as $index => $value) {
	  $deviations[$index] = $value - $next_month_lyaponov_average;
	}
	foreach ($deviations as $index => $value) {
	  if ($value == max($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	  if ($value == min($deviations)) {
	    $punctures[$index] = 0.0;
	  }
	}
	$next_month_lyaponov_average = 0;
	$n = 0;
	foreach ($list_new as $index => $value) {
	  if ($punctures[$index] > 0.0) {
	    $n++;
	  }
	  $next_month_lyaponov_average += $value * $punctures[$index];
	}
	if ($n > 0) {
	  $next_month_lyaponov_average /= $n;
	}
      }

      if ($next_month_particular_average > 0) {
	$next_month = ($next_month_average +
		       $next_month_particular_average +
		       $next_month_lyaponov_average) / 3;
      } else {
	$next_month = ($next_month_average +
		       $next_month_lyaponov_average) / 2;
      }
      $next_month = round($next_month);

      $list_new = array_reverse($list_new);
      $labels = array_reverse($labels);

      $max_scale = max($list_new);
      //drupal_set_message($max_scale);
      if ($max_scale == 0) {
	$max_scale = 1;
      }

      if ($lot_index == 0) { // First round is the aggregate
	$chart_title = t('Monthly valet registrations at property');
	$chart_id = 'monthly_valet_chart_aggregated';
      } else {
	$chart_title = t('Monthly valet registrations at ').$policy['data']['Alias'];
	$chart_id = 'monthly_valet_chart_'.$loop_lot_id;
      }

      // Monthly activity graph
      $chart = array
	(// Mandatory
	 '#chart_id' => $chart_id,
	 '#type' => CHART_TYPE_BAR_V,
	 '#data' => array(0 => $list_new),
	 // Optional
	 '#title' => chart_title($chart_title),
	 '#size' => chart_size(800, 200),
	 '#adjust_resolution' => array('#adjust' => TRUE, '#max' => $max_scale),
	 '#grid_lines' => chart_grid_lines(100, 10),
	 '#data_colors' => array(0 => chart_unique_color('new')),
	 '#mixed_axis_labels' =>
	 array(CHART_AXIS_Y_LEFT =>
	       array(0 => array(chart_mixed_axis_range_label(0, $max_scale))),
	       CHART_AXIS_X_BOTTOM =>
	       array(0 => array_map(chart_mixed_axis_label, $labels),
		     1 => array(chart_mixed_axis_label(t('Month'), 50)))),
	 '#bar_size' => chart_bar_size(42, 15),
	 ); //print_r($chart);print('<p>');

      chart_copy($chart, $basename);
      $source = file_create_path('charts').'/'.$filename;
      //drupal_set_message('SOURCE:'.$source);
      $dest = ltrim($base_path, '/').'tmpimage/'.$filename;
      //drupal_set_message('DEST:'.$dest);
      copy($source, $dest);
      unlink($source);

      if ($lot_index == 0) { // First round is the aggregate
	if (chdbvalet_update_prediction_monthly
	    ($company_id, $property_id,
	     'All',
	     $next_month)) {
	  drupal_set_message(t('Failed to update monthly predictions'), 'error');
	}
      } else {
	if (chdbvalet_update_prediction_monthly
	    ($company_id, $property_id,
	     $loop_lot_id,
	     $next_month)) {
	  drupal_set_message(t('Failed to update monthly predictions'), 'error');
	}
      }
    }

    if (0) { // Debug
      $rows[] = array('<b>Predictions based on flat average</b>',
		      round($today_average),
		      round($next_day_average),
		      round($next_week_average),
		      round($next_month_average));

      if ($next_month_particular_average > 0) {
	$next_month_particular_average = round($next_month_particular_average);
      } else {
	$next_month_particular_average = t('Missing data');
      }
      $rows[] = array('<b>Predictions based on particular avarage</b>',
		      round($today_particular_average),
		      round($next_day_particular_average),
		      round($next_week_particular_average),
		      $next_month_particular_average);

      $rows[] = array('<b>Predictions based on Lyaponov filterd average</b>',
		      round($today_lyaponov_average),
		      round($next_day_lyaponov_average),
		      round($next_week_lyaponov_average),
		      round($next_month_lyaponov_average));

      $rows[] = array('<b>Final predictions</b>',
		      round($current_day),
		      round($next_day),
		      round($next_week),
		      round($next_month));

      $header = array('', 'Today', 'Tomorrow', 'Next week', 'Next month');
      $form['property']['Debug'] = array('#value' => theme_table($header, $rows),
					 '#suffix' => '<p><p>');
    }

    if ($lot_index == 0) { // First round is the aggregate
      $final_data['Aggregate']['current_day'] = $current_day;
      $final_data['Aggregate']['next_day'] = $next_day;
      $final_data['Aggregate']['next_week'] = $next_week;
      $final_data['Aggregate']['next_month'] = $next_month;
      $final_data['Aggregate']['image_daily'] = $image_daily;
      $final_data['Aggregate']['image_weekly'] = $image_weekly;
      $final_data['Aggregate']['image_monthly'] = $image_monthly;
    } else {
      $final_data[$loop_lot_id]['current_day'] = $current_day;
      $final_data[$loop_lot_id]['next_day'] = $next_day;
      $final_data[$loop_lot_id]['next_week'] = $next_week;
      $final_data[$loop_lot_id]['next_month'] = $next_month;
      $final_data[$loop_lot_id]['alias'] = $policy['data']['Alias'];
      $final_data[$loop_lot_id]['image_daily'] = $image_daily;
      $final_data[$loop_lot_id]['image_weekly'] = $image_weekly;
      $final_data[$loop_lot_id]['image_monthly'] = $image_monthly;
    }

    if ($lot_index > 0) { // First round is the aggregate
      $policy = next($policies);
    }
  } // Permit graphs and predictions
  return $final_data;
}



