#get sched ram content
$add = 0;
if(open file, "sched.rom") {
	while(<file>) {
		next if(/^#/);
		s/#.*//g;
		if(/^([a-z0-9_]+):(.*)/) {
			push @dat, $1;
			$_ = $2;
		}
		if(/^\'/) {
			s/[\'\r\n]//g;
			for($i = 0;$i < length($_);$i++) {
				push @dat, ord(substr($_, $i, 1));
			}
		} else {
			s/\s//g;
			for($i = 0;$i < length($_);$i+=2) {
				push @dat, hex(substr($_, $i, 2));
			}
		}
	}
	close file;
}
for($j = $k = $n = 0;$j < @dat;$j++) {
	if($dat[$j] =~ /^mem_/ && $j > 0 || $n == 3) {
		push @init, sprintf "\tsetarg 0x%x\n", $k;
		push @init, sprintf "\tistore %d,contw\n", $n;
		$n = $k = 0;
	}
	if($dat[$j] =~ /^mem_/) {
		push @init, sprintf "\targ %s,contw\n", $dat[$j];
	} else {
		$k |= $dat[$j] << $n++*8;
	}
}
push @init, sprintf "\tsetarg 0x%x\n", $k;
push @init, sprintf "\tistore %d,contw\n", $n;

open file,"eeprom.dat";
for($i = 0;$i < 4;$i++) {
	$_ = <file>;
}
close file;
$len = int(((hex($_) + @init*4 + 12) & 0xff)/4);
if($len > 0x3b) {
	for($i = 0;$i < 0x40 - $len;$i++) { push @init, "\trtn\n"; }
	$len = 0;
}
printf "len=%x\n", $len;

open file, "bt_program23.list";
while(<file>){
	if(/^0x([0-9a-f]+)\s+([0-9]+)/)  {
		$line = 0 + $2;
		last if(hex($1) == $len);
	}
}
close file;
open file, "bt_program23.meta";
@f = <file>;
close file;


open file, ">bt_program23.meta";
for($i = 0;$i < @f;$i++) { 
	$_ = $f[$i];
	if($i == $line) {
		printf file "\tbranch skip_a1bugfix_romdat\n";
		printf file "\tromdat 0x00,0x00,0x55,0xaa\n";
		printf file "\tromdat 0x00,0x08,0x90,0x01\n";
		printf file "\tromdat 0xa0,0x74,0x01,0xf0\n";
		printf file "\tromdat 0x80,0xfe,0x00,0x00\n";
		printf file "skip_a1bugfix_romdat:\n";
	}
	print file $_;
	if(/^init_sched:/) {
		print file @init;
	}
}
close file;
