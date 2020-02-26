$memmap_path=$ARGV[0];
$memblocks_path=$ARGV[1];
$ipc_path=$ARGV[2];
$ipc_define_path=$ARGV[3];
$btreg_path=$ARGV[4];
open(btreg,">$btreg_path");
print btreg ("");#reset file
close(btreg);
open(memmap,$memmap_path);
open(memblocks,$memblocks_path);
open(memipc,$ipc_path);
open(memipcdefine,$ipc_define_path);
open(btreg,">>$btreg_path");
@text = <memmap>;
print btreg ("#ifndef _BT_REG_H_\n");
print btreg ("#define _BT_REG_H_\n");

print btreg ("\n/**\n");
print btreg ("* bt memory define\n");
print btreg ("**/\n");
foreach $text (@text){
	@word = split (/[\t ]+/,$text);
	$reg_addr = @word[0];
	if ($reg_addr =~ /0x[0-9abcdef]*/){#Is 0x0000~0xffff
		$word1 = @word[1];
		@reg_name = split(/\/[\/\*]/,$word1);#delete // or /*  */
		$reg_name = @reg_name[0];
		$reg_name =~ s/[\r\n]//;#delete Carriage return
		
		printf btreg ("#define\t%-40s\t%s\n",$reg_name,$reg_addr);
		#print btreg ("#define \t $reg_name \t\t\t $reg_addr \n");
		#print btreg ("@word[1]\n");
	}
}

print btreg ("\n/**\n");
print btreg ("* bt block memory define\n");
print btreg ("**/\n");
@text = <memblocks>;
foreach $text (@text){
	@word = split (/[\t ]+/,$text);
	$reg_addr = @word[0];
	if ($reg_addr =~ /0x[0-9abcdef]*/){
		$word1 = @word[1];
		@reg_name = split(/\/[\/\*]/,$word1);
		$reg_name = @reg_name[0];
		$reg_name =~ s/[\r\n]//;
		printf btreg ("#define\t%-40s\t%s\n",$reg_name,$reg_addr);
		#print btreg ("#define \t $reg_name \t\t\t\t $reg_addr \n");
		#print btreg ("@word[1]\n");
	}
}


print btreg ("\n/**\n");
print btreg ("* IPC addr define\n");
print btreg ("**/\n");
@text = <memipc>;
foreach $text (@text){
	@word = split (/[\t ]+/,$text);
	$reg_addr = @word[0];
	if ($reg_addr =~ /^0x[0-9abcdef]*/){
		$word1 = @word[1];
		@reg_name = split(/\/[\/\*]/,$word1);
		$reg_name = @reg_name[0];
		$reg_name =~ s/[\r\n]//;
		printf btreg ("#define\t%-40s\t%s\n",$reg_name,$reg_addr);
	}
}


print btreg ("\n/**\n");
print btreg ("* BT EVENT,CMD opcode\n");
print btreg ("**/\n");
@text = <memipcdefine>;
foreach $text (@text){
	@word = split (/[\t ]+/,$text);
	$reg_addr = @word[0];
	if ($reg_addr =~ /0[xX][0-9a-fA-F]+/){
		$word1 = @word[1];
		@reg_name = split(/\/[\/\*]/,$word1);
		$reg_name = @reg_name[0];
		$reg_name =~ s/[\r\n]//;
		printf btreg ("#define\t%-50s\t%s\n",$reg_name,$reg_addr);
	}
}

print btreg ("#endif\n");
close(memblocks);
close(memmap);
close(btreg);

