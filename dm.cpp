#include<stdio.h>
int main(){
char filenameQ[20]="qpoints.yaml";
char s[10000];
	FILE* fpQ=fopen(filenameQ,"r");
	int* lineNumberQ=new int[100000000];
	int nLineQ=0;
	lineNumberQ[nLineQ++]=0;
	fseek(fpQ,0,SEEK_SET);
	printf("Preparing Lines...\n");
	while(fgets(s,10000,fpQ)){
		lineNumberQ[nLineQ++]=ftell(fpQ);
	}
	printf("qpoints total Lines=%d\n",nLineQ);
	printf("Done\n");
	fseek(fpQ,lineNumberQ[0],SEEK_SET);
	int nqpoint;
	fscanf(fpQ,"nqpoint:%d\n",&nqpoint);
	printf("nqpoint=%d\n",nqpoint);
	int natom;

	fscanf(fpQ,"natom: %d\n",&natom);
	printf("natom=%d\n",natom);

		double basis[3][3];
		int offBasis=3+natom+1;
	fseek(fpQ,lineNumberQ[offBasis],SEEK_SET);
	printf("basis:\n");
	for(int i=0;i<3;i++){
		fscanf(fpQ,"- [    %lf,%lf,%lf ]\n",&basis[i][0],&basis[i][1],&basis[i][2]);
		printf("%f\t%f\t%f\n",basis[i][0],basis[i][1],basis[i][2]);
	}
	int offPos=offBasis+8;
	fseek(fpQ,lineNumberQ[offPos],SEEK_SET);
	double pos[natom][3];
	printf("poses:\n");
	for(int i=0;i<natom;i++){
		fscanf(fpQ,"- [    %lf,%lf,%lf ]\n",&pos[i][0],&pos[i][1],&pos[i][2]);
		double xyz[3];
		for(int j=0;j<3;j++){
			xyz[j]=0;
			for(int k=0;k<3;k++){
				xyz[j]+=basis[k][j]*pos[i][k];
			}
			pos[i][j]=xyz[j];
		}
		printf("%f\t%f\t%f\n",pos[i][0],pos[i][1],pos[i][2]);
	}
			int nbranch=3*natom;
		FILE* fout=fopen("dm.txt","w");
					//	fprintf(fout,"%d\n",nqpoint);
				//fprintf(fout,"%d\n",natom);
		int lenSingleQ=2+nbranch+1+2*nbranch+1;
	int offKpoint=3+natom+8+1+natom+1;
	for(int iqp=0;iqp<nqpoint;iqp++){
		int qline=offKpoint+iqp*lenSingleQ;
		fseek(fpQ,lineNumberQ[qline],SEEK_SET);
		fgets(s,10000,fpQ);
		printf("%s",s);
		fprintf(fout,"%s",s);
		double dm[3*natom][6*natom];
		fseek(fpQ,lineNumberQ[qline+2],SEEK_SET);
		for(int j=0;j<3*natom;j++){
			fscanf(fpQ,"  - [    %lf",&dm[j][0]);
			//printf("%f\t",a);
			//if(debug)fprintf(fout,"%f\t",dm[idim][iff][j][0]);
			for(int i=1;i<6*natom;i++){
				fscanf(fpQ,",    %lf",&dm[j][i]);
				//printf("%f\t",a);
				//if(debug)fprintf(fout,"%f\t",dm[idim][iff][j][i]);
			}
			fscanf(fpQ," ]\n");
			//printf("\n");
			//if(debug)fprintf(fout,"\n");
		}
		for(int idim=0;idim<3;idim++){
			fprintf(fout,"dim:%d\n",idim);
			for(int j=0;j<3*natom;j++){
				for(int i=0;i<3*natom;i++){
					int atom1=j/3;
					int atom2=i/3;
					double refpos=pos[atom1][idim]-pos[atom2][idim];		
					fprintf(fout,"%f\t%f\t",dm[j][2*i+1]*refpos,dm[j][2*i]*refpos);
				}
				fprintf(fout,"\n");
			}
		}	
				
	}
	fclose(fout);
	printf("getting tdm.txt...\n");
	FILE* fddm=fopen("ddm.txt","r");
	FILE* fdm=fopen("dm.txt","r");
	FILE* ftdm=fopen("tdm.txt","w");
							fprintf(ftdm,"%d\n",nqpoint);
				fprintf(ftdm,"%d\n",natom);
		for(int iqp=0;iqp<nqpoint;iqp++){
			fgets(s,100,fddm);
			fgets(s,100,fdm);
			fprintf(ftdm,"%s",s);
			//printf("%s",s);
			for(int idim=0;idim<3;idim++){
				fgets(s,100,fddm);
				fgets(s,100,fdm);
				fprintf(ftdm,"%s",s);
				for(int i=0;i<3*natom;i++){
					for(int j=0;j<6*natom;j++){
						double a,b;
						fscanf(fddm,"%lf",&a);
						fscanf(fdm,"%lf",&b);
						fprintf(ftdm,"%f\t",a+b);
					}
					fscanf(fddm,"\n");
					fscanf(fdm,"\n");
					fprintf(ftdm,"\n");
				}
			}
		}
return 0;
}
