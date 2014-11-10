DIM="6 6 1";
phonopy -d --dim="${DIM}"
php /home/zhouy/phononpyTest/lammps/phononpy/phononpyLMP/minimize.php
php /home/zhouy/phononpyTest/lammps/phononpy/phononpyLMP/force.php -fz
sed "s/4 4 1/${DIM}/g" mesh.conf>mesh1.conf
sh post.sh
phonopy -p mesh1.conf
