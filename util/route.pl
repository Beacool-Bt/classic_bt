system("route print > 1");
open file,"1";
while(<file>) {
	if(/^\s+0\.0\.0\.0\s+0\.0\.0\.0\s+([\.0-9]+)\s+([\.0-9]+)\s+1\s+$/) {
		print "added route to " . $1;
		system("route add 192.168.2.126 " .$1);
		last;
	}
}
close file;