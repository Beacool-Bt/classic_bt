#!/usr/bin/perl
# 2005 - David Grant.
# Take an ihex input from STDIN, and write a .mif file to STDOUT
# This script could probably be implemented with something like:
#    $#!@_%^$@%$@%$_!^$@#^@%$#@_%$@^&!%$_!%!%&$*(#^#@%^) 
# But I perfer the somewhat readable version.

# Flow from within the Nios2 SDK Shell:
# nios2-elf-as file.asm -o file.o
# nios2-elf-objcopy file.o --target ihex file.hex
# cat file.hex | perl hex2mif.pl > file.mif

my @rom = ();
my @ram = ();

sub conv {
	my ($in) = @_;
	$out = substr($in,6,2).substr($in,4,2).substr($in,2,2).substr($in,0,2);
	return hex $out;
}

sub writeromfile {
	my (@buf) = @_;
	my($x);

	for($x=0; $x<@buf; $x++) {
		printf ("%02x\n", $buf[$x]);
	}
}

sub writebinfile {
	my (@buf) = @_;
	my($x);

	for($x=0; $x<@buf; $x++) {
		for($y=0;$y<8;$y++) {
			printf ("%d", $buf[$x] & 1<<(7-$y) ? 1 : 0);
		}
		printf "\n" if(($x & 3) == 3);
	}
}

sub writemiffile {
	my (@buf) = @_;
	my($x);
	print  ("WIDTH=8;\n");
	print ("DEPTH=".@buf.";\n");
	print ("CONTENT BEGIN\n");
	for($x=0; $x<@buf; $x++) {
		printf ("\t%04x : %02x;\n", $x, $buf[$x]);
	}
	print ("END;\n");
}

if($ARGV[0] eq "ram" || $ARGV[0] eq "rammif")
{
	$org = 0x8000;
	$limit = 0xffff;
} elsif($ARGV[0] eq "rom" || $ARGV[0] eq "bin" || $ARGV[0] eq "mif" || $ARGV[0] eq "rommif" ) {
	$org = 0;
	$limit = 0x7fff;
} else {
	$org = 0;
	$limit = 0xffffff;
}


$org = 0;
$limit = 0x7fff;

while (<STDIN>) {
	$l = $_;
	next if(!/\w/);
	$count = (hex substr($l, 1, 2));
	$addr = (hex substr($l, 3, 4));
	$type = (hex substr($l, 7, 2));
	last if $type eq 1;
	if($type == 4) {
		$org = -(hex substr($l, 9, 4))*0x10000;
	}
	if($addr >= $org && $addr <= $limit) 
	{
		$addr*=4 if($ARGV[0] eq "bin");
		$addr -= $org;
		for($x=0; $x<$count; $x++) {
			$rom[$addr + $x] = (hex substr($l, 9+2*$x, 2)) ; 
		}
	}
}

if($ARGV[0] eq "rom" || $ARGV[0] eq "ram" || $ARGV[0] eq "mem")
{
	writeromfile(@rom);
}
elsif($ARGV[0] eq "bin")
{
	writebinfile(@rom);
}
else
{
	writemiffile(@rom);
}

