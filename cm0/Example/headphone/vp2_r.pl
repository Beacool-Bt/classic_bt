use strict;

my $memmap = "output/memmap.format";
my $inputfile = "output/flash_code_L.dat";
my $outputfile1 = "output/flash_code_R.dat";
my @regStr = ("aa\n","55\n","01\n","00\n","00\n","00\n");

open(FLASHOUT, "$memmap") or die "can't open $memmap: $!";
my @lines = <FLASHOUT>;
my $length = @lines;
my $i;
for($i=0;$i<$length;$i+=1){
	my @string = split(' ', $lines[$i]);
	my $length_s = @string ;
	if( $string[1] eq "mem_hsp_role"){
		$regStr[5] = substr($string[0],2,2)."\n";
		$regStr[4] = substr($string[0],4,2)."\n";
	}
}
close(FLASHOUT);


open(FLASHOUT, "$inputfile") or die "can't open $inputfile: $!";
my @flashArray = <FLASHOUT>;
my $flashLen = @flashArray; 
close(FLASHOUT);

=pod
=cut

my $count;
my $countIn;
my $countInin;
my $searchRes = 0;

my $regLen = @regStr;
open(FLASHOUT, ">$outputfile1") or die "can't open $outputfile1: $!";

for($count=0;$count<$flashLen;$count++){
	if ($searchRes == 0)
	{
		for($countIn=0; $countIn<$regLen; $countIn++){
			if ($countIn == 2){
				next;
			}
			if ($flashArray[$count+$countIn] ne $regStr[$countIn])
			{
				last;
			}
		}
		if ($countIn == $regLen)
		{
			$searchRes = 1;	
			my $index = $count+$regLen;
			$flashArray[$index++] = "02\n";
		}
	}
	print FLASHOUT $flashArray[$count];
}

close(FLASHOUT);

open(FLASHOUT, "$outputfile1") or die "can't open $outputfile1: $!";
my @lines = <FLASHOUT>;
my $length = @lines;
if($length > 0x17000 ){	
#	printf "MAX LENGTH:94208 (0x17000) ";
	printf "%s  length:%d (0x%x)\n",$outputfile1,$length,$length;	
	printf "\n--------- %s TOO LARGE ----------\n\n",$outputfile1;	
	printf "\n--------- %s TOO LARGE ----------\n\n",$outputfile1;	
}else{
#	printf "MAX LENGTH:94208 (0x17000) ";
	printf "%s length:%d (0x%x)\n",$outputfile1,$length,$length;
}

close(FLASHOUT);