#!/usr/bin/perl
sub clear_print_buf{
	for($i = 0; $i < 10; $i++){
		$binData[$i] = 0;
	}
}

sub print_line{
	if($byte_cnt == 0){
		#new line print header;
		$offset = $patch_cmd_len * $line_cnt;
		printf "{";
		printf "0x%x%x, ", ($offset / 16 )     % 16, ($offset)       % 16;
		printf "0x%x%x, ", ($offset / 4096)    % 16, ($offset / 256) % 16;
		printf "0x%x%x, ", ($patch_cmd_len / 16) % 16, ($patch_cmd_len)  % 16;
		$line_cnt ++;
	}
	printf "0x%s%s, 0x%s%s, 0x%s%s, 0x%s%s",$binData[0],$binData[1],$binData[2],$binData[3],$binData[4],$binData[5],$binData[6],$binData[7];
	
	$byte_cnt += 4;
	if($byte_cnt >= $patch_cmd_len){
		$byte_cnt = 0;
		printf "},\n\t";
	}else{
		printf ", ";
	}
}

################STATIC########################

$patch_cmd_len = 96;
$patch_cmd_header_len = 3;
$patch_cmd_checksum_len = 1;

################main fuction##################

printf "/******************************************************************************\n";
printf " ** File Name:      yc_patch_yc1021.h                                         *\n";
printf " **                                                                           *\n";
printf " ******************************************************************************\n";
printf "\n";
printf " ******************************************************************************\n";
printf " **                               Edit History                                *\n";
printf " ** ------------------------------------------------------------------------- *\n";
printf " **     DATE          NAME             DESCRIPTION                            *\n";
printf " **  2015.3.17       fan.kou            Create.                               *\n";
printf " ******************************************************************************/\n";
printf "/******************************************************************************/\n";
printf "#ifndef _YC_PATCH_YC1021_H_2015_03_17_16_39_59_298_\n";
printf "#define _YC_PATCH_YC1021_H_2015_03_17_16_39_59_298_\n";
printf "\n";
printf "/**---------------------------------------------------------------------------*\n";
printf " **                             Dependencies                                  *\n";
printf " **---------------------------------------------------------------------------*/\n";
printf "/**---------------------------------------------------------------------------*\n";
printf " **                            Compiler Flag                                  *\n";
printf " **---------------------------------------------------------------------------*/\n";
printf "#ifdef __cplusplus\n";
printf "extern   \"C\" \n";
printf "{\n";
printf "#endif\n";
printf "/**---------------------------------------------------------------------------*\n";
printf " **                           Macros & Enums                                  *\n";
printf " **---------------------------------------------------------------------------*/\n";

open(FILE,"ramcode.rom") or die $!;
$line_cnt = 0;
while($line = <FILE>){
	$line_cnt++;
}
$cmd_cnt = $line_cnt * 4 / $patch_cmd_len + (($line_cnt * 4 % $patch_cmd_len) > 0);
close(FILE);

printf "#define YC_PATCH_CMD_CNT %d\n",$cmd_cnt;
printf "#define YC_PATCH_CMD_LEN %d\n",$patch_cmd_len + $patch_cmd_header_len + $patch_cmd_checksum_len;
printf "#define YC_PATCH_ARRAY_LEN 0x20\n";
printf "/**---------------------------------------------------------------------------*\n";
printf " **                           Data Structures                                 *\n";
printf " **---------------------------------------------------------------------------*/\n";
printf "\n";
printf "/**---------------------------------------------------------------------------*\n";
printf " **                             Variables                                     *\n";
printf " **---------------------------------------------------------------------------*/\n";
printf "\n";

printf "const uint8_t yc_patch_array[YC_PATCH_ARRAY_LEN] =\n";
printf "{\n";
printf "\t0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \n";
printf "\t0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \n";
printf "\t0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \n";
printf "\t0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \n";
printf("\n");
printf("};\n");


printf "const uint8_t yc_patch_cmd[YC_PATCH_CMD_CNT][YC_PATCH_CMD_LEN] =\n";
printf "{\n\t";

open(BINFILE,"ramcode.rom") or die $!;
binmode (BINFILE);
$byte_cnt = 0;
$end_of_file = 0;
$checksum = 0;
$line_cnt = 0;
while(!$end_of_file)
{
	for($i = 0; $i < 10; $i++){
		if(!read(BINFILE, $binData[$i], 1)){
			$end_of_file = 1;
			if(i != 0){
				printf "Error: Error at the end of file.";
			}
		}
	}
	if(!$end_of_file){
		&print_line;
	}
}
while($byte_cnt){
	&clear_print_buf;
	&print_line;
}
printf("\n");
printf("};\n");
printf "\n";
printf "/**---------------------------------------------------------------------------*\n";
printf " **                         Function Prototypes                               *\n";
printf " **---------------------------------------------------------------------------*/\n";
printf "\n";
printf "#ifdef __cplusplus\n";
printf "}\n";
printf "#endif\n";
printf "\n";
printf "/******************************************************************************/\n";
printf "#endif //#ifndef _YC_PATCH_YC1021_H_2015_03_17_16_39_59_298_\n";