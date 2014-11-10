#!/usr/bin/env python
import yaml
import numpy as np
data = yaml.load(open("mesh.yaml"))
natom=data['natom']
paticipation_ratio=[]
for qpos in data['phonon']:
	for band in qpos['band']:
		s=0.0
		for atom in band['eigenvector']:
				val=np.array(atom)
				#print  val
				s+=(np.sum(val[:,0]**2+val[:,1]**2))**2
		paticipation_ratio.append([band['frequency'],1/float(natom)/s])
np.savetxt("df_paticipation_ratio.txt",paticipation_ratio)
print "paticipation_ratio.txt dumped "
'''
print paticipation_ratio

val=np.array(data['phonon'][0]['band'][0]['eigenvector'][0])
print val[:,0]**2+val[:,1]**2#np.sum(val**2)

dynmat = []
dynmat_data = data['phonon'][1]['dynamical_matrix']
for row in dynmat_data:
	vals = np.reshape(row, (-1, 2))
	dynmat.append(vals[:, 0] + vals[:, 1] * 1j)
ynmat = np.array(dynmat)
eigvals, eigvecs, = np.linalg.eigh(dynmat)
frequencies = np.sqrt(np.abs(eigvals.real)) * np.sign(eigvals.real)
conversion_factor_to_THz = 15.633302
print frequencies * conversion_factor_to_THz
'''
