<?php
$sic=0;
$usingN=0;
for($i=1;$i<$argc;$i++){
if($argv[$i]=="-sic")$sic=1;
if($argv[$i]=="-N")$usingN=1;
}
 getMini("INPOSCAR");
 function getMini($POSCAR){
	$HOME=dirname(__file__);
	if(!$HOME)die("home error!\n");
	$dir="mini";
	if(is_dir($dir))shell_exec("rm -r $dir");
	if(is_dir("dirs"))shell_exec("rm -r dirs");
	if(is_file("FORCE_SETS"))shell_exec("rm FORCE_SETS");
	//if(is_file("INPOSCAR"))shell_exec("rm INPOSCAR");
	shell_exec("mkdir -p $dir;cp  $POSCAR $dir/POSCAR");
	get_latgen($dir);
	getpos($dir);
			global $sic;
	global $usingN;
	if($sic){
		$potential="$HOME/lmp/SiC.tersoff";
		$sedSic="|sed \"s/BNC/SiC/g\"";
	}else{
	$potential="$HOME/lmp/BNC.tersoff";
		$sedSic='';
	}

	if($usingN){
		$sedN="|sed \"s/ C/ C N/g\"|sed \"s/mass 1 12.01/mass 1 12.01\\nmass 2 14/g\"";
	}else{
	$sedN='';
	}		//echo "cat $HOME/lmp/in.graphene $sedSic  $sedN|~/lmp_ubuntu \n";
	shell_exec("cd $dir;cp  $potential .;cat $HOME/lmp/in.minimize $sedSic $sedN|~/lmp_ubuntu ");
	get_POSCAR($dir);
}

function getEnergy(){
	$energy=shell_exec("cd $home;~/lmp_ubuntu<in.energy>log.out;tail -1 dump.energy;");
	return $energy+0.0;
}
 function getpos($dir){
// input file of latgen
	$fp=fopen("$dir/lat.in","w");
	fprintf($fp,"7\ny\nlat.pos\ny\n1 1 1 \n1\n3\nC\n0\npos.xyz\ndata.pos\nmap.in\n");
	shell_exec("cd $dir;~/latgen <lat.in");
}
 function get_latgen($dir){
	$fp=fopen("$dir/POSCAR","r");
	$fout=fopen("$dir/lat.pos","w");
	$i=0;
	while($content=fgets($fp)){
		$i++;
		if($i==1 || $i==7)continue;
		fprintf($fout,$content);
	}
		fclose($fp);
	fclose($fout);
}
function get_POSCAR($dir){
	if(!is_file("$dir/dump.force"))die("no dump.force found when minimizing!\n");
$fp=fopen("$dir/dump.force","r");

	$fout=fopen("POSCAR","w");
	for($i=0;$i<5;$i++)fgets($fp);
	list($xlo,$xhi,$xy)=fscanf($fp,"%f%f%f");
	if(!$xy)$xy=0;
	list($ylo,$yhi)=fscanf($fp,"%f%f");
	list($zlo,$zhi)=fscanf($fp,"%f%f");
	$xl=$xhi-$xlo-abs($xy);$yl=$yhi-$ylo;$zl=$zhi-$zlo;
	echo $xl;
	//fclose($fp);
	$i=0;
	$fp1=fopen("$dir/POSCAR","r");
		while($content=fgets($fp1)){
		$i++;
		//if($i>7)break;
		if($i==3){
		fprintf($fout,"$xl 0 0 \n");
		continue;
		}
		if($i==4){
				fprintf($fout,"$xy $yl 0 \n");
		continue;
		}
				if($i==5){
				fprintf($fout,"0 0 $zl \n");
		continue;
		}
		fprintf($fout,$content);
	}
	fgets($fp);
	//while($content=fgets($fp)){fprintf($fout,$content);}
	fclose($fout);
}
