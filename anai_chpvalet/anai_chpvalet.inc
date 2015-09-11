<?php
// $Id$
require_once("anai_chpvalet_db.inc");


function chpvalet_genRandomReference() {
  $length = 20;
  $characters = "0123456789";
  $string = '';
  for ($p = 0; $p < $length; $p++) {
    $string .= $characters[mt_rand(0, strlen($characters) - 1)];
  }
  return $string;
}

