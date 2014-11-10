#include<stdio.h>
#include<math.h>
int main(){
char filenameQ[20]="tdm.txt";
char s[10000];
	FILE* fpQ=fopen(filenameQ,"r");
	FILE* fd=fopen("debug.txt","w");
	int* lineNumberQ=new int[100000000];
	int nLineQ=0;
	lineNumberQ[nLineQ++]=0;
	fseek(fpQ,0,SEEK_SET);
	printf("Preparing Lines...\n");
	while(fgets(s,10000,fpQ)){
		lineNumberQ[nLineQ++]=ftell(fpQ);
	}
	printf("tdm total Lines=%d\n",nLineQ);
	
	char filename[20]="mesh.yaml";
	FILE* fp=fopen(filename,"r");
	int* lineNumber=new int[100000000];
	int nLine=0;
	lineNumber[nLine++]=0;
	fseek(fp,0,SEEK_SET);
	while(fgets(s,10000,fp)){
		lineNumber[nLine++]=ftell(fp);
	}

	printf("mesh total Lines=%d\n",nLine);
		printf("Done\n");
		
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
	
	
	FILE* fsk=fopen("sk.txt","w");
	fprintf(fsk,"%d\n",nqpoint);
	fprintf(fsk,"%d\n",natom);
	int nbranch=3*natom;
		
	int lenSingleQ=3*(nbranch+1)+1;

	int lenBranch=3+4*natom;
	int lenSingle=3+nbranch*lenBranch+1;
	int offKpoint=4;
	int offKpointQ=2;
	FILE* ffreq=fopen("freq.txt","w");
		fprintf(ffreq,"%d\n",nqpoint);
	fprintf(ffreq,"%d\n",natom);
	for(int iqp=0;iqp<nqpoint;iqp++){
			int qlineQ=offKpointQ+iqp*lenSingleQ;
			fseek(fpQ,lineNumberQ[qlineQ],SEEK_SET);
			fgets(s,10000,fpQ);
			//fprintf(fd,"%s",s);
			fprintf(fsk,"%s",s);
			fprintf(ffreq,"%s",s);
			int qline=offKpoint+iqp*lenSingle;
			double ww[nbranch];
			double eigen[nbranch][2*nbranch];
			for(int i=0;i<nbranch;i++){
				int ibranchLine=qline+3+i*lenBranch;
				fseek(fp,lineNumber[ibranchLine+1],SEEK_SET);
				fscanf(fp,"    frequency:     %lf",&ww[i]);
				fprintf(ffreq,"%f\n",ww[i]);
				
				//printf("%f\n",ww[i]);
				int ndim=3;
				for(int iatom=0;iatom<natom;iatom++){
					for(int idim=0;idim<ndim;idim++){
						fseek(fp,lineNumber[ibranchLine+3+iatom*4+1+idim],SEEK_SET);
						fscanf(fp,"  - [    %lf,%lf",&eigen[i][iatom*(ndim*2)+idim*2],&eigen[i][iatom*(ndim*2)+idim*2+1]);
						//fprintf(fd,"%f\t%f\n",eigen[i][iatom*(idim*2)+idim*2],eigen[i][iatom*(idim*2)+idim*2+1]);
				}
				}
				/*for(int j=0;j<2*nbranch;j++){
						fprintf(fd,"%f\t\n",eigen[i][j]);
				}*/
			}
			
		for(int idim=0;idim<3;idim++){
			fprintf(fsk,"dim:%d\n",idim);
			//fprintf(fd,"dim:%d\n",idim);
			double dm[nbranch][2*nbranch];
			fseek(fpQ,lineNumberQ[qlineQ+2+idim*(1+nbranch)],SEEK_SET);
			for(int i=0;i<nbranch;i++){
				for(int j=0;j<2*nbranch;j++){
					fscanf(fpQ,"%lf",&dm[i][j]);
					//fprintf(fd,"%f\t",dm[i][j]);
				}
				fscanf(fpQ,"\n");
				//fprintf(fd,"\n");
			}
			//calculate the sk
			for(int i=0;i<nbranch;i++){				
				for(int j=0;j<nbranch;j++){
					double vvr=0;
					double vvi=0;
					double factor=sqrt(ww[i]/ww[j]);

					for(int k=0;k<nbranch;k++){
						for(int k1=0;k1<nbranch;k1++){
							double r1=eigen[i][2*k];
							double i1=-eigen[i][2*k+1];
							double r2=dm[k][2*k1];
							double i2=dm[k][2*k1+1];
							double r3=r1*r2-i1*i2;
							double i3=i1*r2+i2*r1;//fprintf(fd,"%f\t%f\n",r1,i1);
							r1=eigen[j][2*k1];
							i1=eigen[j][2*k1+1];
							r2=r3*r1-i3*i1;
							i2=i3*r1+i1*r3;
							vvr+=r2*(factor+1/factor)*.5;					
							vvi+=i2*(factor+1/factor)*.5;
						}
						
					}
				fprintf(fsk,"%f\t%f\t",vvr,vvi);
				}
				fprintf(fsk,"\n");
			}
		}
		
	}
return 0;
}









