<?php
// $Id$
require_once("anai_chpalerts_db.inc");


define('ALERT_LINK_RETENTION_DAYS', 7);


function chpalert_alert_type($tag) {
  $list['Permit_Report'] = 'Scheduled permit registrations report';
  $list['PermitRevenue_Report'] = 'Scheduled permit revenue report';
  $list['Whitelist_Report'] = 'Scheduled whitelist report';
  $list['Tickets_Report'] = 'Scheduled tickets report';
  $list['TicketRevenue_Report'] = 'Scheduled ticket revenue report';
  $list['Complaints_Report'] = 'Scheduled '.strtolower(COMPLAINT_NAME).'s report';
  $list['Boots_Report'] = 'Scheduled boots report';
  return $list[$tag];
}



