while(<stdin>) {
	next if(/^#/);
	s/#.*//g;
	if(/^\'/) {
		s/[\r\n]//g;
		$len += length($_) - 1;
	} else {
		s/\s//g;
		$len += length($_)/2;
	}
}
printf "len = %x\n", $len;
