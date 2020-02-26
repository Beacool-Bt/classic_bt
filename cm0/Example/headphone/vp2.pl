use strict;
my @vp_array;
$vp_array[0] = "voice_prompt/British_Eng_Powering_On.pcm";
$vp_array[1] = "voice_prompt/British_Eng_powering_off.pcm";
$vp_array[2] = "voice_prompt/British_Eng_paired.pcm";
$vp_array[3] = "voice_prompt/British_Eng_disconnect.pcm";
$vp_array[4] = "voice_prompt/British_Eng_battery_is_near_empty.pcm";
$vp_array[5] = "voice_prompt/British_Eng_max_vol.pcm";
$vp_array[6] = "voice_prompt/British_Eng_tws_connected.pcm";
$vp_array[7] = "voice_prompt/zero.pcm";
$vp_array[8] = "voice_prompt/one.pcm";
$vp_array[9] = "voice_prompt/two.pcm";
$vp_array[10] = "voice_prompt/three.pcm";
$vp_array[11] = "voice_prompt/four.pcm";
$vp_array[12] = "voice_prompt/five.pcm";
$vp_array[13] = "voice_prompt/six.pcm";
$vp_array[14] = "voice_prompt/seven.pcm";
$vp_array[15] = "voice_prompt/eight.pcm";
$vp_array[16] = "voice_prompt/nine.pcm";
$vp_array[17] = "voice_prompt/ring.pcm";

my @vp_startaddr;
my @vp_size;
my $vp_array_len=@vp_array;

my $memmap = "output/memmap.format";
my $outputfile = "output/flash.dat";
my $outputfile2 = "output/flash_code_L.dat";
my $allpcmfile = "output/all_pcm_data.dat";
my @regStr = ("aa\n","55\n","03\n","00\n","00\n","00\n");
my @args;
my $hexL;
my $hexM;
my $hexH;

my $count;
my $countIn;
my $countInin;
my $length=0;
my $buff;
my $searchRes = 0;
my $temptrans;
my $temptransHex;


open(FLASHOUT, "$memmap") or die "can't open $memmap: $!";
my @lines = <FLASHOUT>;
my $length = @lines;
my $i;
for($i=0;$i<$length;$i+=1){
	my @string = split(' ', $lines[$i]);
	my $length_s = @string ;
	if( $string[1] eq "mem_vp_poweron_addr"){
#		print "$string[1] $string[0]\n";
#		print @regStr;	
		$regStr[5] = substr($string[0],2,2)."\n";
		$regStr[4] = substr($string[0],4,2)."\n";
#		print $regStr[4];
#		print $regStr[5];
#		print @regStr;
	}
}
close(FLASHOUT);

# __END__


#replace file
open(FLASHOUT, "$outputfile") or die "can't open $outputfile: $!";
my @flashArray = <FLASHOUT>;
my $flashLen = @flashArray; 
#$vp_startaddr[0] = @flashArray;
$vp_startaddr[0] = 0x30000;
close(FLASHOUT);

=pod
=cut

for($count = 0; $count < $vp_array_len;$count++)
{
	@args = stat ($vp_array[$count]);
	$vp_size[$count] = $args[7];
	$vp_startaddr[$count+1] = $vp_startaddr[$count]+$vp_size[$count]+3;
}



my $regLen = @regStr;
open(FLASHOUT, ">$outputfile2") or die "can't open $outputfile2: $!";

#print FLASHOUT "$flashLen\n";
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
			for($countInin = 0; $countInin < $vp_array_len; $countInin++)
			{
				$temptrans = ($vp_startaddr[$countInin] & 0xff);
				$temptransHex = sprintf "%02x",$temptrans;
				$flashArray[$index++] = "$temptransHex\n";
				$temptrans = ($vp_startaddr[$countInin] & 0xff00)>>8;
				$temptransHex = sprintf "%02x",$temptrans;
				$flashArray[$index++] = "$temptransHex\n";
				$temptrans = ($vp_startaddr[$countInin] & 0xff0000)>>16;
				$temptransHex = sprintf "%02x",$temptrans;
				$flashArray[$index++] = "$temptransHex\n";
			}
		}
	}
	print FLASHOUT $flashArray[$count];
}

close(FLASHOUT);



=pod
=cut

#open(FLASHOUT, ">>$outputfile") or die "can't open $outputfile: $!";

open(FLASHOUT, ">$allpcmfile") or die "can't open $allpcmfile: $!";

for($count=0; $count < $vp_array_len;$count++)
{
	open(VPPWO, $vp_array[$count]) or die "can't open $vp_array[$count]: $!";
	binmode(VPPWO);
	#print file size
	$hexL = $vp_size[$count]&0xff;
	$hexM = ($vp_size[$count]&0xff00) >>8;
	$hexH = ($vp_size[$count]&0xff0000) >>16;
	
	my $sizestr = sprintf "%02x",$hexL;
	print FLASHOUT "$sizestr\n";
	$sizestr = sprintf "%02x",$hexM;
	print FLASHOUT "$sizestr\n";
	$sizestr = sprintf "%02x",$hexH;
	print FLASHOUT "$sizestr\n";
	
	#print file content
	while(read(VPPWO, $buff, 1))
	{
	   $hexL=unpack("H*",$buff);
	   print FLASHOUT "$hexL\n";
	}
	close(VPPWO);
}

#close file handle.
close(FLASHOUT);


open(FLASHOUT, "$outputfile2") or die "can't open $outputfile2: $!";
my @lines = <FLASHOUT>;
my $length = @lines;
if($length > 0x17000 ){	
#	printf "MAX LENGTH:94208 (0x17000) ";
	printf "%s  length:%d (0x%x)\n",$outputfile2,$length,$length;	
	printf "\n--------- %s TOO LARGE ----------\n\n",$outputfile2;	
	printf "\n--------- %s TOO LARGE ----------\n\n",$outputfile2;	
}else{
#	printf "MAX LENGTH:94208 (0x17000) ";
	printf "%s length:%d (0x%x)\n",$outputfile2,$length,$length;
}

close(FLASHOUT);

open(FLASHOUT, "$allpcmfile") or die "can't open $allpcmfile: $!";
my @lines = <FLASHOUT>;
my $length = @lines;
if($length > 0x4F000 ){	
#	printf "MAX LENGTH:323584 (0x4F000) ";
	printf "%s  length:%d (0x%x)\n",$allpcmfile,$length,$length;	
	printf "\n--------- %s TOO LARGE ----------\n\n",$allpcmfile;	
	printf "\n--------- %s TOO LARGE ----------\n\n",$allpcmfile;	
}else{
#	printf "MAX LENGTH:323584 (0x4F000) ";
	printf "%s  length:%d (0x%x)\n",$allpcmfile,$length,$length;
}
close(FLASHOUT);
