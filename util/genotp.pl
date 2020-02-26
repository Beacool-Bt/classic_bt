#!/usr/bin/perl

open(FILE,"program.lis") or die $!;
$page_size = 64;
$j = $page_size;
while(<FILE>){
	if(/^([0-9a-f]+)\s+([0-9a-f]+)/ && hex($1) >= 0x10000) {
		#print $1,"\n",$j,"\n";
		if(hex(substr($1,length($1)-4,4)) == 1){

			#print "aaa\n";
			for(;$j < $page_size;$j++) {
					print "00\n00\n00\n00\n";
					#print $j,"\n";
			}
			$j = 0;
		}
		if($j == 0){
			print "20\n20\n00\n40\n";
			$j ++;
		}
		for($i = 0;$i < 4;$i++) {
			print substr($2, $i*2, 2),"\n"; 

		}
		$j ++;
		if($j == $page_size){
			$j = 0;
		}elsif($j > $page_size){
			print "Overflow at 0x",$1," !\n";
			close(FILE);
			die;
		}

	}
}
#print "codecnt = ",$codecnt,"\n";
close(FILE);