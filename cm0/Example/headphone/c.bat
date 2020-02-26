make clean
make
@cd output
@geneep -f -n
@cd ..
perl vp.pl
