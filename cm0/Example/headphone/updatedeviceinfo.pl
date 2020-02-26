use strict;

my $outputfile = "output/flash.dat";
#my $outputfile=$ARGV[0];
my @regStrlebdr = ("aa\n","55\n","06\n","00\n","0c\n","45\n");
my @regStrbrbdr = ("aa\n","55\n","06\n","00\n","40\n","41\n");
my @regStrname = ("aa\n","55\n","07\n","00\n","5c\n","45\n");
my @args;


my $count;
my $countIn;
my $countInin;
my $length=0;
my $buff;
my $searchRes = 0;
my $temptrans;
my $temptransHex;



#replace file
open(FLASHOUT, "$outputfile") or die "can't open $outputfile: $!";
my @flashArray = <FLASHOUT>;
my $flashLen = @flashArray; 
close(FLASHOUT);


my $regLen = @regStrlebdr;
open(FLASHOUT, ">$outputfile") or die "can't open $outputfile: $!";

#print FLASHOUT "$flashLen\n";
for($count=0;$count<$flashLen;$count++){
	if ($searchRes == 0)
	{
		for($countIn=0; $countIn<$regLen; $countIn++){
			if ($countIn == 2){
				next;
			}
			if ($flashArray[$count+$countIn] ne $regStrlebdr[$countIn])
			{
				last;
			}
		}
		if ($countIn == $regLen)
		{
			$searchRes = 1;	
			my $index = $count+$regLen;
			$temptrans = (($flashArray[$index]+1) & 0xFF);
			$flashArray[$index++] = "$temptrans\n";	
		}
	}
	#print FLASHOUT $flashArray[$count];
}

$searchRes = 0;
for($count=0;$count<$flashLen;$count++){
	if ($searchRes == 0)
	{
		for($countIn=0; $countIn<$regLen; $countIn++){
			if ($countIn == 2){
				next;
			}
			if ($flashArray[$count+$countIn] ne $regStrbrbdr[$countIn])
			{
				last;
			}
		}
		if ($countIn == $regLen)
		{
			$searchRes = 1;	
			my $index = $count+$regLen;
			$temptrans = (($flashArray[$index]+1) & 0xFF);
			$flashArray[$index++] = "$temptrans\n";	
		}
	}
	#print FLASHOUT $flashArray[$count];
}

$searchRes = 0;
for($count=0;$count<$flashLen;$count++){
	if ($searchRes == 0)
	{
		for($countIn=0; $countIn<$regLen; $countIn++){
			if ($countIn == 2){
				next;
			}
			if ($flashArray[$count+$countIn] ne $regStrname[$countIn])
			{
				last;
			}
		}
		if ($countIn == $regLen)
		{
			$searchRes = 1;	
			my $index = $count+$regLen+6;
			$temptrans = (($flashArray[$index]+1) & 0xFF);
			$flashArray[$index++] = "$temptrans\n";	
		}
	}
	print FLASHOUT $flashArray[$count];
}

close(FLASHOUT);


