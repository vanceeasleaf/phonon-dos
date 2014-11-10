#include<stdio.h>
int main(){
char filenameQ[20]="ddm/qpoints.yaml";
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
			int nbranch=3*natom;
		FILE* fout=fopen("ddm.txt","w");
		int lenSingleQ=2+nbranch+1+2*nbranch+1;
		int debug=0;
		int debug1=0;
	int offKpoint=3+natom+8+1+natom+1;
	for(int irqp=0;irqp<nqpoint/6;irqp++){
		int iqp=irqp*6;
		int qline=offKpoint+iqp*lenSingleQ;
		fseek(fpQ,lineNumberQ[qline],SEEK_SET);
		fgets(s,10000,fpQ);
		printf("%s",s);
		fprintf(fout,"%s",s);
		double dm[3][2][3*natom][6*natom];
		
		for(int idim=0;idim<3;idim++){
			if(debug||debug1)fprintf(fout,"dim:%d\n",idim);
			for(int iff=0;iff<2;iff++){//two values used to determine the diff
				int iqp=irqp*6+idim*2+iff;
				int qline=offKpoint+iqp*lenSingleQ;
				fseek(fpQ,lineNumberQ[qline+2],SEEK_SET);
				for(int j=0;j<3*natom;j++){
					fscanf(fpQ,"  - [    %lf",&dm[idim][iff][j][0]);
					//printf("%f\t",a);
					if(debug)fprintf(fout,"%f\t",dm[idim][iff][j][0]);
					for(int i=1;i<6*natom;i++){
						fscanf(fpQ,",    %lf",&dm[idim][iff][j][i]);
						//printf("%f\t",a);
						if(debug)fprintf(fout,"%f\t",dm[idim][iff][j][i]);
					}
					fscanf(fpQ," ]\n");
					//printf("\n");
					if(debug)fprintf(fout,"\n");
				}
			}
			for(int j=0;j<3*natom;j++){
				for(int i=0;i<6*natom;i++){
					dm[idim][0][j][i]=(dm[idim][1][j][i]-dm[idim][0][j][i])/.001;
					if(debug1)fprintf(fout,"%f\t",dm[idim][0][j][i]);
				}
				if(debug1)fprintf(fout,"\n");
			}
		}
		if(debug||debug1)continue;
		for(int idim=0;idim<3;idim++){		
			fprintf(fout,"dim:%d\n",idim);
			for(int j=0;j<3*natom;j++){
				for(int i=0;i<6*natom;i++){
					double s=0;
					for(int idim1=0;idim1<3;idim1++)
						s+=basis[idim1][idim]*dm[idim1][0][j][i];
					fprintf(fout,"%f\t",s);
				}
				fprintf(fout,"\n");
			}
		}		
	}
return 0;
}
