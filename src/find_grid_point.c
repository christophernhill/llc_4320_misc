#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define L_ERRMSG_MAX 1024

void bSwap8(char *buf,int lBuf){
 int i;
 char b1,b2,b3,b4,b5,b6,b7,b8;
 // return;
 for (i=0;i<lBuf;i=i+8){
  b1=buf[i+0];
  b2=buf[i+1];
  b3=buf[i+2];
  b4=buf[i+3];
  b5=buf[i+4];
  b6=buf[i+5];
  b7=buf[i+6];
  b8=buf[i+7];
  buf[i+0]=b8;
  buf[i+1]=b7;
  buf[i+2]=b6;
  buf[i+3]=b5;
  buf[i+4]=b4;
  buf[i+5]=b3;
  buf[i+6]=b2;
  buf[i+7]=b1;
 }
}

int main(int argc, char *argv[]) {
 char *lonStr, *latStr;
 float lonVal, latVal;
 int   nread;
 int   nx=4320;
 double *ioBufLon;
 double *ioBufLat;
 FILE  *fptr;

 if ( argc < 3 ){
  exit(-1);
 }
 lonStr=argv[1];
 latStr=argv[2];

 nread  = sscanf(lonStr,"%f",&lonVal);
 nread  = sscanf(latStr,"%f",&latVal);
 ioBufLon=malloc(sizeof(double)*(nx+1)*(nx*3+1));
 ioBufLat=malloc(sizeof(double)*(nx+1)*(nx*3+1));

 int tn;
 int ip;
 int tnMin=0;
 int tiMin=0;
 int tjMin=0;
 int iOffMin=0;
 double dist,dLon,dLat;
 double minDist=500.;
 int it, jt, iOff;
 char fileName[30];
 for (tn=1;tn<=5;++tn) {
  sprintf(fileName,"../grid/tile%3.3d.mitgrid",tn);
  fptr=fopen(fileName,"r");
  if ( tn == 1 | tn == 2 ){
    nread=fread(ioBufLon,8,(nx+1)*(nx*3+1),fptr);
    // printf("%d\n",nread);
    nread=fread(ioBufLat,8,(nx+1)*(nx*3+1),fptr);
    // printf("%d\n",nread);
    bSwap8((char *)ioBufLon,(nx+1)*(nx*3+1)*8);
    bSwap8((char *)ioBufLat,(nx+1)*(nx*3+1)*8);
    // printf("%le\n",ioBufLon[0]);
    // printf("%le\n",ioBufLon[1]);
    // printf("%le\n",ioBufLon[0]-ioBufLon[1]);
    // exit(-1);
    for (jt=0;jt<nx*3;++jt){
     for (it=0;it<nx;++it){
      iOff=(jt-1)*(nx+1)+it;
      dLon=ioBufLon[iOff]-lonVal;
      dLat=ioBufLat[iOff]-latVal;
      dist=sqrt(dLon*dLon+dLat*dLat);
      if (dist < minDist ) {
       minDist=dist;
       tnMin=tn;
       tiMin=it;
       tjMin=jt;
       iOffMin=iOff;
      }
     }
    }
  }
  if ( tn == 4 | tn == 5){
    nread=fread(ioBufLon,8,(nx+1)*(nx*3+1),fptr);
    // printf("%d\n",nread);
    nread=fread(ioBufLat,8,(nx+1)*(nx*3+1),fptr);
    // printf("%d\n",nread);
    bSwap8((char *)ioBufLon,(nx+1)*(nx*3+1)*8);
    bSwap8((char *)ioBufLat,(nx+1)*(nx*3+1)*8);
    // printf("%le\n",ioBufLon[0]);
    // printf("%le\n",ioBufLon[1]);
    // printf("%le\n",ioBufLon[0]-ioBufLon[1]);
    // exit(-1);
    for (jt=0;jt<nx;++jt){
     for (it=0;it<nx*3;++it){
      iOff=(jt-1)*(nx*3+1)+it;
      dLon=ioBufLon[iOff]-lonVal;
      dLat=ioBufLat[iOff]-latVal;
      dist=sqrt(dLon*dLon+dLat*dLat);
      if (dist < minDist ) {
       minDist=dist;
       tnMin=tn;
       tiMin=nx*3-it+1;
       tjMin=jt;
       iOffMin=iOff;
      }
     }
    }
  }
  if ( tn == 3 ) {
  }
  if ( tn == tnMin ) {
   printf("tnMin=%d tiMin=%d tjMin=%d %f %f\n",tnMin,tiMin,tjMin,(float)ioBufLon[iOffMin],(float)ioBufLat[iOffMin]);
  }
  fclose(fptr);
  printf("%s\n",fileName);
 }
 

 printf("%f\n",lonVal);
 printf("%f\n",latVal);
}
