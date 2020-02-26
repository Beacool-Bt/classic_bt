@dat = @init = ();

sub setarg {
my ($j, $k, $n);
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
}

sub setsect {
my($i,$j,$k,$n);

if($ARGV[1] eq "c51") {
	push @init, sprintf "ifdef C51\ndownload_pdata:\n\tforce 9,loopcnt\ndownload_pdata_loop:\n\thstore 1,core_docd_pdata\n";
	push @init, sprintf "\trshift8 pdata,pdata\n\tloop download_pdata_loop\n\trtn\n";
	push @init, sprintf "download_c51:\n\thjam 5,core_docd_ctrl\n\tforce 0,pdata\n\thstore 2,core_docd_paddr\n";
}
for($i = $j = $k = $n = 0;$i < @dat + 2;$i++) {
	if($i == @dat && $n == 0) {
		push @init, sprintf "\tsetarg 0x%x\n", $k;
		if($ARGV[1] eq "c51") {
                	push @init, sprintf "\tcall download_pdata\n";
		} else {
			push @init, sprintf "\tistore %d,contw\n", ($j + 7)/8;
		}
		last;
	}
	if($i == @dat) {
		$i = $i;
	}
	if($j > 18 || $i == @dat + 1) {
		push @init, sprintf "\tsetsect %d,0x%x\n", $n,$k & 0x3ffff;
		if($n == 3 || $i == @dat + 1) {
			if($ARGV[1] eq "c51") {
                		push @init, sprintf "\tcall download_pdata\n";
			} else {
                		push @init, sprintf "\tistore %d,contw\n", ($n*18 + ($j > 18 ? 18 : $j))/8;
			}
			$n = -1;
		}
		$k >>= 18;
		$j -= 18;
		$n++;
	}
	if($i < @dat) {
		if($dat[$i] =~ /^mem_/) {
		       push @init, sprintf "\targ %s,contw\n", $dat[$i];
		} else {
			$k |= $dat[$i] << $j;
			$j += 8;
		}
	}
}
if($ARGV[1] eq "c51") {
	push @init, sprintf "\thjam 3,core_docd_ctrl\n\trtn\nendif\n";
}
}

if(@ARGV == 0) {
	printf "missing file name\n";
	exit;
}
if(open file, $ARGV[0]) {
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

setsect();

print @init;

