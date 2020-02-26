use strict;

open (file,"output/flash_code_R.dat") or die  "Can't open out file  : $!"; 
open(file_out,">output/flash_all_R.dat")		or die "Can't open out file  : $!"; 
open(file_pcm,"output/all_pcm_data.dat")		or die "Can't open out file  : $!"; 
my	@lines = <file>;
my	$length = @lines;
my $i =0;
my $j=0;

print file_out "00\n10\n03\n";
for($i=3;$i<0x1000;$i+=1){
	print file_out "ff\n";
}

for($j=0; $j<$length;$j+=1){
	print file_out $lines[$j];
	$i+=1;
}

for(; $i<0x30000;$i+=1){
	print file_out "ff\n";
}

@lines = <file_pcm>;
$length = @lines;
for($j=0; $j<$length;$j+=1){
	print file_out $lines[$j];
	$i+=1;
}
#print  $i;

close(file);
close(file_out);
close(file_pcm);