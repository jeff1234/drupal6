<?php
// $Id$
require_once("anai_chppermits.inc");

define('PAYMENT_CREDIT_CARD',
'
<form action="https://www.paypal.com/cgi-bin/webscr" method="post">
<input type="hidden" name="cmd" value="_xclick">
<input type="hidden" name="business" value="tore.stenbock@gmail.com">
<input type="hidden" name="lc" value="CA">
<input type="hidden" name="item_name" value="Subscribed">
<input type="hidden" name="amount" value="20.00">
<input type="hidden" name="currency_code" value="CAD">
<input type="hidden" name="button_subtype" value="services">
<input type="hidden" name="no_note" value="0">
<input type="hidden" name="bn" value="PP-BuyNowBF:btn_buynowCC_LG.gif:NonHostedGuest">
<input type="image" src="https://www.paypalobjects.com/en_US/i/btn/btn_buynowCC_LG.gif" border="0" name="submit" alt="PayPal - The safer, easier way to pay online!">
<img alt="" border="0" src="https://www.paypalobjects.com/en_US/i/scr/pixel.gif" width="1" height="1">
</form>
');