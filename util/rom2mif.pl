#!/usr/bin/perl
#perl hex2mif.pl <filename without ext> 


$file0 = $file1 = $ARGV[0];
$file1 =~ s/\.rom/.mif/;
$wid = 8;



print ("opening $file0\n");
open(ifile, "$file0") || die("cannot find $file0 file\n");
for($l = 0;$val = <ifile>;$l++)
{
	if($l == 0)
	{
		if(substr($val, 1, 1) !~ /[0-9a-fA-F]/) { $wid = 1; }
		elsif(substr($val, 2, 1) !~ /[0-9a-fA-F]/) { $wid = 8; }
		elsif(substr($val, 4, 1) !~ /[0-9a-fA-F]/) { $wid = 16; }
		else { $wid = 32;	}
	}
}
printf ("width is %d\n", $wid);
printf ("totally %d words\n", $l);
seek(ifile, 0, 0);
open(ofile, ">$file1");
printf ofile ("DEPTH = %d;\n", $wid == 1 ? $l/8 : $l);
printf ofile ("WIDTH = %d;\n", $wid == 1 ? 8 : $wid);
print ofile ("ADDRESS_RADIX = HEX;\n");
print ofile ("DATA_RADIX = HEX;\n");
print ofile ("CONTENT\n");
print ofile ("BEGIN\n");
for($l = 0, $val1 = 0;$val = <ifile>;$l++)
{
	$val =~ s/\n//g;
	$val0 = hex(substr($val, 0, $wid == 1 ? 1 : $wid/4));
	if($wid > 1) {
		printf ofile ("%x : %x;\n", $l, $val0);
	} elsif($l % 8 == 0 && $l > 0) {
		printf ofile ("%x : %x;\n", $l/8 - 1, $val1);
		$val1 = 0;
	}
	$val1 |= $val0 << ($l & 7);
}
if($wid == 1) { printf ofile ("%x : %x;\n", $l/8 - 1, $val1); }
print ofile ("END;\n");

close(ifile);
close(ofile);

