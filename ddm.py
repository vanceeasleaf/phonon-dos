#!/usr/bin/env python
import yaml
import numpy as np
data = yaml.load(open("ddm/qpoints.yaml"))
print "ddm/qpoints.yaml loaded"
natom=data['natom']
nqpoint=data['nqpoint']
basis0=np.array(data['real-basis'])
basis=np.zeros((3*natom,3*natom))
for i in range(0,natom):
	for j in range(0,natom):
		if(i!=j):
			continue
		for ii in range(0,3):
			for jj in range(0,3):
				basis[i*3+ii][j*3+jj]=basis0[ii][jj]
basis=np.array(basis)
print basis
qpositions=data['phonon']
ndim=3
ddm={}
ddm['natom']=natom
ddm['natom']=nqpoint
ddm['phonon']=[]
for iqpos in range(0,nqpoint/6):
	ddm['phonon'].append({'q-position':[]})
	#print "i-qpos",iqpos
	for idim in range(0,ndim):
		dm1=np.array(qpositions[iqpos*6+idim*2]['dynamical_matrix'])
		dm2=np.array(qpositions[iqpos*6+idim*2+1]['dynamical_matrix'])
		ddm0=(dm2-dm1)/.001
		ddm1=np.dot(basis.T,ddm0)
		ddm['phonon'][iqpos]['q-position'].append(ddm1.tolist())
		#ddm['q-position'][iqpos][idim]=ddm1
with open('ddm.yaml', 'w') as outfile:
    outfile.write( yaml.dump(ddm) )
