#include<stdio.h>
double lorents(double x,double w){
	double const pi=3.14159265359;
	return w/4/pi/pi*1/(x*x+w*w/4);
}
int main(){
double const pi=3.14159265359;
double width=0.13*2*pi;
FILE* fsk=fopen("sk.txt","r");
FILE* ffreq=fopen("freq.txt","r");
FILE* fd=fopen("diffuse.txt","w");
FILE* fd1=fopen("diffW.txt","w");
int nqpoint;
int natom;
fscanf(fsk,"%d\n",&nqpoint);
fscanf(fsk,"%d\n",&natom);
printf("%d\n",nqpoint);
printf("%d\n",natom);
fprintf(fd,"nqpoint:%d\n",nqpoint);
fprintf(fd,"natom:%d\n",natom);
char s[100];
fgets(s,100,ffreq);
fgets(s,100,ffreq);
int nbranch=3*natom;
double freq[nqpoint][nbranch];

for(int iqp=0;iqp<nqpoint;iqp++){
		fgets(s,100,ffreq);
	for(int ibra=0;ibra<nbranch;ibra++){

		fscanf(ffreq,"%lf\n",&freq[iqp][ibra]);
		//printf("%f\n",freq[iqp][ibra]);
	}
}
printf("reading freq done.\n");
for(int iqp=0;iqp<nqpoint;iqp++){
	fgets(s,100,fsk);
	fprintf(fd,"%s",s);
	double dif[nbranch];
	for(int ibra=0;ibra<nbranch;ibra++){
		dif[ibra]=0;
	}
	for(int idim=0;idim<3;idim++){
		fgets(s,100,fsk);
		for(int ibra=0;ibra<nbranch;ibra++){	
			for(int jbra=0;jbra<nbranch;jbra++){
				double a,b;
				fscanf(fsk,"%lf%lf",&a,&b);
				if(ibra==jbra)continue;
				dif[ibra]+=(a*a+b*b)*lorents(freq[iqp][ibra]-freq[iqp][jbra],width);
			}
			fscanf(fsk,"\n");
		}
	}
	for(int ibra=0;ibra<nbranch;ibra++){
		dif[ibra]*=pi/12/freq[iqp][ibra]/freq[iqp][ibra];
		fprintf(fd,"%f\n",dif[ibra]);
		fprintf(fd1,"%f\t%f\n",freq[iqp][ibra],dif[ibra]);
	}
}
return 0;
}
