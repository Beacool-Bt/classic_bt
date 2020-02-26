e pu
make
@cd output
@geneep -f -n -z
@cd ..
perl .\vp2.pl
perl .\vp2_r.pl
perl .\vp3.pl
perl .\vp3_r.pl
@ set a=%time%
e fe 0
e fw 0 031000
@if [%1]==[r] (@echo "download flash_code_R.dat") else (@echo "download flash_code_L.dat")
@if [%1]==[r] (e fp output\flash_code_R.dat 1000) else (e fp output\flash_code_L.dat 1000)
e fp output\all_pcm_data.dat 30000
@ set b=%time%
@echo off

set /a h1=%a:~0,2%
set /a m1=1%a:~3,2%-100
set /a s1=1%a:~6,2%-100
set /a h2=%b:~0,2%
set /a m2=1%b:~3,2%-100
set /a s2=1%b:~6,2%-100
if %h2% LSS %h1% set /a h2=%h2%+24
set /a ts1=%h1%*3600+%m1%*60+%s1%
set /a ts2=%h2%*3600+%m2%*60+%s2%
set /a ts=%ts2%-%ts1%
set /a h=%ts%/3600
set /a m=(%ts%-%h%*3600)/60
set /a s=%ts%%%60
echo 耗时:%h%小时%m%分%s%秒

e k