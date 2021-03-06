<?php
#$th3="python /home/xggong/software/thirdorder/python/thirdorder_vasp.py";
$th3="python /home/zhouy/ShengBTE/thirdorder/python/thirdorder_vasp.py";
$shengBTE="/home/zhouy/ShengBTE/ShengBTE";
$vaspLMP=dirname(__FILE__);
$dim="4 4 1";
$grid="20 20 1";
$NN="N";
$Narg=" -N ";
for($i=0;$i<$argc;$i++){
if($argv[$i]=="-C"){$NN="C";$Narg=" ";}
}
if(!is_file("POSCAR"))die("no POSCAR!\n");
if(is_dir("secondorder"))shell_exec("rm -r secondorder");
if(is_dir("thirddorder"))shell_exec("rm -r thirddorder");
if(is_dir("SHENG"))shell_exec("rm -r SHENG");
passthru("
mkdir secondorder thirddorder SHENG;
cp POSCAR secondorder/INPOSCAR;
cd secondorder;
php $vaspLMP/minimize.php $Narg
cp POSCAR ../CONTCAR
phonopy -d --dim=\"$dim\";
php $vaspLMP/force.php $Narg
phonopy --writefc --dim=\"$dim\";
cp FORCE_CONSTANTS ../SHENG/FORCE_CONSTANTS_2ND
cd ..
cp CONTCAR thirddorder/POSCAR
cd thirddorder
$th3 sow $dim -2 
php $vaspLMP/force3.php $Narg
sed \"s/4 4 1 -2/$dim -2/g\" post.sh >post1.sh
sh post1.sh
cp FORCE_CONSTANTS_3RD ../SHENG/
");
$fp=fopen("SHENG/CONTROL","w");
$fcontcar=fopen("CONTCAR","r");
fgets($fcontcar);
list($lfactor)=fscanf($fcontcar,"%f");
$a1=fgets($fcontcar);
$a2=fgets($fcontcar);
$a3=fgets($fcontcar);
list($nc,$nn)=fscanf($fcontcar,"%d%d");
$ntotal=$nc+$nn;
fprintf($fp,"
&allocations
        nelements=2,
        natoms=$ntotal,
        ngrid(:)=$grid
&end
&crystal
        lfactor=$lfactor,
        lattvec(:,1)=$a1 lattvec(:,2)=$a2 lattvec(:,3)=$a3
        elements=\"C\" \"$NN\"
		types=");
for($i=0;$i<$nc;$i++)fprintf($fp,"1 ");
for($i=0;$i<$nn;$i++)fprintf($fp,"2 ");
fprintf($fp,"\n");
$n=0;
fgets($fcontcar);
while($pos=fgets($fcontcar)){
if(!trim($pos))break;
$n++;
fprintf($fp,"positions(:,$n)=$pos");
}
fprintf($fp,"        scell(:)=$dim
&end
&parameters
        T=300.
        scalebroad=1.0

&end
&flags
        nonanalytic=.TRUE.
        nanowires=.FALSE.
&end     
");
echo "START SHENGBTE...";
passthru("cd SHENG ;/usr/bin/mpirun -n 4 $shengBTE");
$fkappaLine=shell_exec("tail -1 SHENG/BTE.kappa_scalar");
list($id,$fkappa)=sscanf($fkappaLine,"%d%f");
shell_exec("echo $fkappa >kappa");












?>
