#include<stdio.h>
int main(){
char filename[20]="mesh.yaml";
	char s[100];
	FILE* fp=fopen(filename,"r");
	int* lineNumber=new int[100000000];
	int nLine=0;
	lineNumber[nLine++]=0;
	fseek(fp,0,SEEK_SET);
	printf("Preparing Lines...\n");
	while(fgets(s,100,fp)){
		lineNumber[nLine++]=ftell(fp);
	}
	printf("Done\n");
	printf("Total Lines=%d\n",nLine);
	FILE* fw=fopen("total_dos.dat","r");
	fgets(s,100,fw);
	int ndos=0;
	double dos[10000];
	double gw[10000];
	while(fscanf(fw,"%lf%lf\n",&dos[ndos],&gw[ndos])>1){
	//printf("%d\t%f\t%f\n",ndos,dos[ndos],gw[ndos]);
		ndos++;	
		if(ndos>=10000){printf("ndos is too large!\n");return 1;}
	}
	double ddos=dos[1]-dos[0];
	fseek(fp,lineNumber[0],SEEK_SET);
	int mesh[3];
	fscanf(fp,"mesh: [%d,%d,%d ]\n",&mesh[0],&mesh[1],&mesh[2]);
	printf("mesh=[%d,%d,%d]\n",mesh[0],mesh[1],mesh[2]);
	int nqpoint;
	fscanf(fp,"nqpoint:%d\n",&nqpoint);
	printf("nqpoint=%d\n",nqpoint);
	int natom;

	fscanf(fp,"natom: %d\n",&natom);
	printf("natom=%d\n",natom);
		int nbranch=3*natom;
					int iatom=49;
	double ldos[ndos];
	int npr=nqpoint*nbranch;
	double pr[npr];
	double fr[npr];
	FILE* fldos=fopen("ldos.txt","w");
	fprintf(fldos,"%d\n",natom);
	fprintf(fldos,"%d\n",ndos);
	for(int ipr=0;ipr<npr;ipr++){
				pr[ipr]=0.0;
			}
	for(int iatom=0;iatom<natom;iatom++){
			for(int idos=0;idos<ndos;idos++){
				ldos[idos]=0.0;
			}
			for(int iqp=0;iqp<nqpoint;iqp++){
				int qline=4+iqp*(3+nbranch*(3+4*natom)+1);
				for(int ibranch=0;ibranch<nbranch;ibranch++){
					int branchline=qline+3+ibranch*(3+4*natom);
					fseek(fp,lineNumber[branchline+1],SEEK_SET);
					double freq;
					fscanf(fp,"    frequency:%lf\n",&freq);
					int ifreq=(freq-dos[0])/ddos;
				

					//printf("%f\n",freq);
				
					int atomline=branchline+3+iatom*4;
					fseek(fp,lineNumber[atomline+1],SEEK_SET);
					double e2=0;
					for(int i=0;i<3;i++){
						double a,b;
						fscanf(fp,"      - [ %lf,%lf ]\n",&a,&b);
						e2+=a*a+b*b;//printf("%d\t%f\n",ifreq,ldos[ifreq]);					
					}
					ldos[ifreq]+=e2;
					fr[ibranch+nbranch*iqp]=freq;
					pr[ibranch+nbranch*iqp]+=e2*e2;
					//fgets(s,100,fp);
					//printf("%s",s);
				}
			}
				
			
			fprintf(fldos,"%d\n",iatom);
			printf("atom:%d\n",iatom);
			for(int idos=0;idos<ndos;idos++){
				fprintf(fldos,"%f\t%f\n",dos[idos],ldos[idos]);
			}
	}
	FILE* fpr=fopen("pr.txt","w");
		for(int ipr=0;ipr<npr;ipr++){
				pr[ipr]*=natom;
				pr[ipr]=1/pr[ipr];
				fprintf(fpr,"%f\t%f\n",fr[ipr],pr[ipr]);
			}
	//double a,b;
	//fscanf(fp,"      - [ %lf,%lf]\n",&a,&b);
	//printf("%f\t%f\n",a,b);
	return 0;
}
