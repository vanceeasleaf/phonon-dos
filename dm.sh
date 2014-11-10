path="/home/zhouy/phononpyTest/lammps/phononpy/dosofCNn/CN/2atom"
sh ${path}/genQP.sh
phonopy dm.conf
php ${path}/getDdm.php
${path}/cddm
${path}/cdm
${path}/csk
${path}/mobility


