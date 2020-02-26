sub swap
{
	my($b) = @_;
	my($i, $j);
	for($i = 0, $j = 0;$i < 8;$i++) {
		$j |= 1 << $i if($b & 1 << (7 - $i)); 
	}
	return $j;
}

@crc_tabccitt = ();
$initd = 0;
sub init_crcccitt_tab
{
	my($crc, $i, $c, $j);

  for ($i=0; $i<256; $i++) {
      $crc = 0;
      $c   = $i << 8;
      for ($j=0; $j<8; $j++) {
          if ( ($crc ^ $c) & 0x8000 ) {	$crc = ( $crc << 1 ) ^ 0x1021;	}
          else  { $crc =   $crc << 1; }
          $c = $c << 1;
      }
      $crc_tabccitt[$i] = $crc & 0xffff;
  }
	$initd = 1;
}

sub crc16_ccitt 

{
	my($crc, $c) = @_;
	my($tmp, $short_c);

	init_crcccitt_tab() if(!$initd);
  $short_c  = 0x00ff & $c;

  $tmp = ($crc >> 8) ^ $short_c;
  $crc = (($crc << 8) ^ $crc_tabccitt[$tmp]) & 0xffff;

  return $crc;

} 

sub crc16_ccitt2
{
	my($crc, $c) = @_;

  $crc  = ($crc >> 8) | ($crc << 8);
  $crc ^= $c;
  $crc ^= ($crc & 0xff) >> 4;
  $crc ^= $crc << 12;
  $crc ^= ($crc & 0xff) << 5;
  $crc &= 0xffff;
 return $crc;
}

init_crcccitt_tab();

1;