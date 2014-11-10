<?php
	$fp=fopen("QPOINTS","r");
	list($natom)=fscanf($fp,"%d");
	if(!is_dir("ddm"))shell_exec("mkdir ddm");
	$fout=fopen("ddm/QPOINTS","w");
	fprintf($fout,"%d\n",6*$natom);
	for($i=0;$i<$natom;$i++){
		list($x[$i],$y[$i],$z[$i])=fscanf($fp,"%f%f%f");
		fprintf($fout,"%f\t%f\t%f\n",$x[$i]-.0005,$y[$i],$z[$i]);
		fprintf($fout,"%f\t%f\t%f\n",$x[$i]+.0005,$y[$i],$z[$i]);
		fprintf($fout,"%f\t%f\t%f\n",$x[$i],$y[$i]-.0005,$z[$i]);
		fprintf($fout,"%f\t%f\t%f\n",$x[$i],$y[$i]+.0005,$z[$i]);	
		fprintf($fout,"%f\t%f\t%f\n",$x[$i],$y[$i],$z[$i]-.0005);
		fprintf($fout,"%f\t%f\t%f\n",$x[$i],$y[$i],$z[$i]+.0005);		
	}
	printf("getting qpoints.yaml..\n");
	shell_exec("cp FORCE_SETS mesh1.conf POSCAR dm.conf ddm;cd ddm; phonopy dm.conf");
	printf("Done.\n");
	//printf("getting ddm.txt...\n");
	?>
