grep position mesh.yaml |sed 's/- q-position: \[//g'|sed 's/,//g'|sed 's/\]//g'>qp.tmp
grep -c "" qp.tmp>QPOINTS
cat qp.tmp>>QPOINTS
rm qp.tmp

