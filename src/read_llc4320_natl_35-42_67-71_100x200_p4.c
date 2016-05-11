
#include <stdio.h>
#include <stdlib.h>

#define L_ERRMSG_MAX 1024

main(int argc, char *argv[]) {
 char *infname;
 char errmsg[L_ERRMSG_MAX];
 
 /*
   panel_size :: Size of "elemental panel" for this LLC grid
              :: 1/48 - 4320
              :: 1/24 - 2160
              :: 1/12 - 1080
   f32_nbytes :: Size of float32
   f64_nbytes :: Size of float64
   yp0        :: west most cell index
   yp1        :: east most cell index
   xp0        :: north most cell index
   xp1        :: south most cell index
   off_base        :: offset to start of panel
   off_pan_start   :: offset from start of panel to start of patch to extract
   nrec        :: Number of records to read
   nel_per_rec :: Number of elements per record
 */
 int panel_size = 4320;
 int f32_nbytes=4;
 int f64_nbytes=8;
 /* Select HOTS centered 150x150 cutout */
 /*
 int bpanel=7;
 int yp0=1500;
 int yp1=3500;
 int xp0=3000;
 int xp1=4320;
 yp0=yp0+1300;
 yp1=yp0+150;
 xp0 = xp0+750;
 xp1 = xp0+150;
 int npanel=3;
 */
 /* Select panel 0 surface, North Atlantic*/
 int bpanel=10;
 int npanel=1;
 // int yp0=0;
 // int yp1=4320;
 // int xp0=0;
 // int xp1=4320;
 /* Narrow to shelf break south from WHOI */
 int yp0=2320;
 int yp1=3120;
 int xp0=2600;
 int xp1=3400;
 // 3d square
 xp0=3200;
 xp1=xp0+200;
 yp0=2720+250;
 yp1=yp0+100;

 int off_base=panel_size*panel_size*bpanel;
 int off_pan_start=panel_size*3*(yp0-1)+(npanel-1)*panel_size+xp0;
 long nseek, bstride;
 
 int nrec=yp1-yp0+1;
 int nel_per_rec=xp1-xp0+1;
 int i0;
 int j;

 int koff=0;
 int kcount=1;
 kcount=90;
 int nread;
 
 int nbyte_cur=f32_nbytes;

 char *reg_buf;

 FILE *fin_hndl, *fout_hndl;

 if ( argc !=3 & argc != 4 ) {
  printf("Usage: %s FILE_NAME_TO_READ FILE_NAME_TO_WRITE [klev]\n",argv[0]);
  exit(0);
 }

 if ( argc == 4 ) {
  nread=sscanf(argv[3],"%d",&kcount);
  if ( nread != 1 ) {
   printf("Usage: %s FILE_NAME_TO_READ FILE_NAME_TO_WRITE [kcount]\n",argv[0]);
   exit(0);
  }
 }
 printf("%s\n",argv[1]);

 fin_hndl = fopen(argv[1],"r");
 if ( fin_hndl == NULL ) {
  snprintf(errmsg,L_ERRMSG_MAX-1,"fopen \"%s\"",argv[1]);
  perror(errmsg);
  exit(0);
 }
 fout_hndl = fopen(argv[2],"r");
 if ( fout_hndl != NULL ) {
  snprintf(errmsg,L_ERRMSG_MAX-1,"fopen \"%s\", file already exists",argv[2]);
  printf("%s\n",errmsg);
  exit(0);
 }
 fout_hndl = fopen(argv[2],"w");

 reg_buf=malloc(nel_per_rec*nrec*nbyte_cur);

 /* Loop over kcount levels, starting from level 1 (the surface) */
 for (koff=0;koff<kcount;++koff) {
  /* Seek to start of first record to read */
  nseek=(off_base+off_pan_start)*nbyte_cur;
  if ( koff != 0 ) {
   nseek=nseek+
    (long)koff*
    (long)panel_size*
    (long)panel_size*
    (long)13*
    (long)nbyte_cur;
  }
  fseek(fin_hndl,(long)nseek,SEEK_SET);
  i0=0;
  bstride=(panel_size*3-nel_per_rec)*nbyte_cur;
  for (j=0;j<nrec;++j){
   fread(reg_buf+i0,nbyte_cur,nel_per_rec,fin_hndl);
   fseek(fin_hndl,(long)bstride,SEEK_CUR);
   i0=i0+nel_per_rec*nbyte_cur;
  }
  fwrite(reg_buf,nbyte_cur,nel_per_rec*nrec,fout_hndl);
 }

 fclose(fout_hndl);
 fclose(fin_hndl);
 free(reg_buf);
}
