<?php
// $Id$
require_once("anai_chptickets_db.inc");

define('FPDF_FONTPATH',
       $_SERVER['DOCUMENT_ROOT'].base_path().drupal_get_path('module', 'anai_chptickets').'/fpdf16/font/');
include_once drupal_get_path('module', 'anai_chptickets').'/fpdf16/fpdf.php';

/***
 * Retrieve tickets
 *   Retrieves any ticket either self-created or inherited.
 * Input:
 *   $username
 * Output:
 *   An associative array of format:
 *     array[alias] = array(data => ticket, readonly => TRUE|FALSE)
 * Comment:
 *   If inherited aliases are identical with self-created aliases then
 *   the self-created ones take precedence.
 *   The readonly flag is TRUE if not self-created.
 */
/* function chptick_retrieve_tickets($company_id, */
/* 				  $order_by = 'None', */
/* 				  $active = TRUE, $confirmed = TRUE) { */
/*   $entries = array(); */

/*   // Self-owned */
/*   foreach (chdbtick_get_all_tickets($company_id, */
/* 				    $order_by, */
/* 				    $active, $confirmed) as $entry) { */
/*     $entries[$entry['TicketId']] = */
/*       array('data' => $entry, */
/* 	    'readonly' => FALSE); */
/*     //drupal_set_message('Retrieved: Enforcer'); */
/*   } */
/*   // Patrollers gets full access */
/*   if (!in_array('Enforcer', array_values($account->roles)) and $account->status) { */
/*     $parents = chpuser_retrieve_parents($account->uid); */
/*     foreach ($parents as $parent) { */
/*       foreach ($parent['roles'] as $role) { */
/* 	if (!strcmp($role, 'Enforcer')) { */
/* 	  $owner = $parent['name']; */
/* 	  foreach (chdbtick_get_all_tickets($owner, */
/* 					    $order_by, $active, $confirmed) as $entry) { */
/* 	    $entries[$entry['TicketId']] = */
/* 	      array('data' => $entry, */
/* 		    'readonly' => FALSE); */
/* 	    //drupal_set_message('Retrieved: Patroller'); */
/* 	  } */
/* 	  break; */
/* 	} */
/*       } */
/*     } */
/*   } */
/*   // Property owners gets readonly access */
/*   if (!in_array('Enforcer', array_values($account->roles)) and $account->status) { */
/*     foreach (chpprop_retrieve_user_properties($account->uid) as $property_id) { */
/*       $property = chdbprop_get_property_id($property_id); */
/*       $owner = user_load(array('name' => $property['PropertyOwner'])); */
/*       if (sizeof($owner) == 0 or !$owner->status) { */
/* 	continue; */
/*       } */
/*       foreach (chdbtick_get_all_tickets($owner->name, */
/* 					$order_by, $active, $confirmed) as $entry) { */
/* 	$entries[$entry['TicketId']] = */
/* 	  array('data' => $entry, */
/* 		'readonly' => TRUE); */
/* 	//drupal_set_message('Retrieved: Property owner'); */
/*       } */
/*     } */
/*   } */
/*   return $entries; */
/* } */



class PDF extends FPDF
{
  function InformationHeader($ticket)
  {
    if (sizeof($ticket['Header']) > 0) {
      $ticket_header = decode_entities($ticket['Header']);
    } else {
      $ticket_header = decode_entities($ticket['TicketLayout']['Header']);
    }

    $this->SetFont('Arial', 'B', 20);
    $this->Cell(0, 15, $ticket_header, 0, 0, 'C');
    $this->Ln();

    $this->SetFont('Arial', '', 10);

    global $user;
    $datetime1 = new DateTime($ticket['CreateTime'], new DateTimeZone('UTC'));
    if ($user->timezone > 0) {
      $datetime1->add(new DateInterval('PT'.$user->timezone.'S'));
    } else {
      $datetime1->sub(new DateInterval('PT'.abs($user->timezone).'S'));
    }

    $this->Cell(0, 5, 'Date of Issue: '.$datetime1->format("l M j, Y g:i a"), 0, 0, "L");
    $this->Ln(6);
    $patroller_id = $ticket['PatrollerId'];
    $this->Cell(0, 5, 'Patroller Number: '.$patroller_id, 0, 0, "L");
    $this->Ln(6);
  }

  function InformationFooter($ticket)
  {
    if (sizeof($ticket['Company']) > 0) {
      $ticket_company = decode_entities($ticket['Company']);
      $ticket_addr1 = decode_entities($ticket['Addr1']);
      $ticket_addr2 = decode_entities($ticket['Addr2']);
      $ticket_addr3 = decode_entities($ticket['Addr3']);
      $ticket_addr4 = decode_entities($ticket['Addr4']);
      $ticket_addr5 = decode_entities($ticket['Addr5']);
      $ticket_disclaimer = decode_entities($ticket['Disclaimer']);
    } else {
      $ticket_company = decode_entities($ticket['TicketLayout']['Company']);
      $ticket_addr1 = decode_entities($ticket['TicketLayout']['Addr1']);
      $ticket_addr2 = decode_entities($ticket['TicketLayout']['Addr2']);
      $ticket_addr3 = decode_entities($ticket['TicketLayout']['Addr3']);
      $ticket_addr4 = decode_entities($ticket['TicketLayout']['Addr4']);
      $ticket_addr5 = decode_entities($ticket['TicketLayout']['Addr5']);
      $ticket_disclaimer = decode_entities($ticket['TicketLayout']['Disclaimer']);
    }

    $addr = $ticket_company;
    if (strlen($ticket_addr1) > 0) {
      $addr .= '
'.$ticket_addr1;
    }
    if (strlen($ticket_addr2) > 0) {
      $addr .= '
'.$ticket_addr2;
    }
    if (strlen($ticket_addr3) > 0) {
      $addr .= '
'.$ticket_addr3;
    }
    if (strlen($ticket_addr4) > 0) {
      $addr .= '
'.$ticket_addr4;
    }
    if (strlen($ticket_addr5) > 0) {
      $addr .= '
'.$ticket_addr5;
    }

    $this->SetFont('Arial', 'B', 15);
    $this->Cell(0, 10, 'Review Procedure', 0, 0, 'C');

    $this->Ln(10);

    $this->SetFont('Arial', '', 10);
    $this->MultiCell(0, 4, $addr, 1, 'C');
    $this->SetFont('Arial', '', 8);
    $this->MultiCell(0, 4, $ticket_disclaimer, 1, 'C');
  }

  function ProcessVehicleInformation($ticket)
  {
    $this->SetFont('Arial', '', 9);
    $this->Cell(40, 7, 'License Plate:', 0, 0, 'L');
    $this->Cell(8, 7, '');
    $this->Cell(40, 7, 'Vehicle Type:', 0, 0, 'L');
    $this->Ln(6);
    $this->SetFont('Arial', 'I', 10);
    $this->Cell(40, 6, ''.$ticket['LPN'], 1, 0, 'C');
    $this->Cell(8, 6, '');
    $this->Cell(40, 6, ''.$ticket['Make'], 1, 0, 'C');
    $this->Ln();
    $this->SetFont('Arial', '', 9);
    $this->Cell(40, 7, 'Vehicle Make:', 0, 0, 'L');
    $this->Cell(8, 7, '');
    $this->Cell(40, 7, 'Vehicle Color:', 0, 0, 'L');
    $this->Ln(6);
    $this->SetFont('Arial', 'I', 10);
    $this->Cell(40, 6, ''.decode_entities($ticket['Model']), 1, 0, 'C');
    $this->Cell(8, 6, '');
    $this->Cell(40, 6, ''.decode_entities($ticket['Color']), 1, 0, 'C');
    $this->Ln(2);
  }

  function ProcessViolation($ticket)
  {
    $this->SetFont('Arial', '', 9);
    $this->Cell(40, 7, 'Violation Number:', 0, 0, 'L');
    $this->Cell(8, 7, '');
    $this->Cell(40, 7, 'Amount:', 0, 0, 'L');
    $this->Ln(6);
    $this->SetFont('Arial', 'I', 10);
    $this->Cell(40, 6, $ticket["TicketId"], 1, 0, 'C');
    $this->Cell(8, 6, '');
    $this->Cell(40, 6, '$'.decode_entities($ticket['Amount']), 1, 0, 'C');
    $this->Ln(5);
    $this->SetFont('Arial', '', 9);
    $this->Cell(40, 7, 'Violation:', 0, 0, 'L');
    $this->Ln(6);
    $this->SetFont('Arial', 'I', 10);
    $this->Cell(13, 6, decode_entities($ticket["Code"]), 1, 0, 'C');
    $this->MultiCell(75, 6, decode_entities($ticket["Description"]), 1, 'L');
    $this->Ln(1);
    $this->SetFont('Arial', '', 9);
    $this->Cell(40, 7, 'Property:', 0, 0, 'L');
    $this->Ln(6);
    $this->MultiCell(0, 6, $ticket["TicketProperty"], 1, 'L');
    $this->Ln(1);
    $this->SetFont('Arial', '', 9);
    $this->Cell(40, 7, 'Comments:', 0, 0, 'L');
    $this->Ln(6);
    $this->Cell(0, 12, '', 1);
    $this->Write('2', decode_entities($ticket["Comment"]));
  }

  function Body($ticket)
  {
    $this->AddPage();
    $this->InformationHeader($ticket);
    $this->SetFont('Arial', 'B', 15);
    $this->Cell(0, 10, 'Vehicle Information', 0, 0, 'C');
    $this->Ln();
    $this->ProcessVehicleInformation($ticket);
    $this->SetFont('Arial', 'B', 15);
    $this->Ln(5);
    $this->Cell(0, 10, 'Violation Information', 0, 0, 'C');
    $this->Ln();
    $this->ProcessViolation($ticket);
    $this->Ln(15);
    $this->InformationFooter($ticket);
  }
}
