$path="output/bt_format.meta";
open file, $path;
@f = <file>;
close file;
open file, ">$path";
$path =~ s/\/.*//;
open file1, ">output/memmap.format";
$xaddr = 0x4000;
$addr = 0;
if($#ARGV == 1) {
	$addr = hex($ARGV[0]);
	$xaddr = hex($ARGV[1]);
}
for($i = 0;$i < @f;$i++) { 
	$_ = $f[$i];
	$start = 1 if(/^\s*memalloc\s*\(\s*$/);
	$start = 2 if(/^\s*xmemalloc\s*\(\s*$/);
	$start = 3 if(/^\s*omemalloc\s*\(\s*$/);
	if($start) {
		if(/^\s*([0-9]+)\s+([a-zA-Z0-9_]+)\s*(.*)$/) {
			if($start == 1) {
				$str = sprintf "0x%04x %s%s\n", $addr, $2, $3;
				$addr += $1;
				$bstr .= $str;
				$sstr .= $str;
			} elsif($start == 2) {
				$str = sprintf "0x%04x %s%s\n", $xaddr, $2, $3;
				$xaddr += $1;
				$xstr .= $str;
			} else {
				push @ostr, $_;
			}
		}
		if(/^\s*\)\s*$/) {
			$start = 0;
			push @omalloc,[@ostr] if(@ostr > 0);
			@ostr = ();
		}
	} else {
		$sstr .= $_;
	}
}
$xend = $xaddr;
for($i = 0;$i <= $#omalloc;$i++) {
	for($j = 0, $oaddr = $xaddr;$j <= $#{$omalloc[$i]};$j++) {
		if($omalloc[$i][$j] =~ /^\s*([0-9]+)\s+([a-zA-Z0-9_]+)\s*(.*)$/) {
			$str = sprintf "0x%04x %s%s\n", $oaddr, $2, $3;
			$oaddr += $1;
			$xstr .= $str;
		}
	}
}
$xend = $oaddr if($oaddr > $xend);
print file $xstr, $sstr;
print file1 $bstr, $xstr;
close file;
close file1;
printf "\nLast allocated address is %04x\n", $addr;
printf "\nLast allocated xmem address is %04x\n", $xend;
