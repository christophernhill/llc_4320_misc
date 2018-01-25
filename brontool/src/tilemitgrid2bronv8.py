# Python script to convert " tile00[1,2,4,5].mitgrid" LLC files to Bron v1.08 layout

#   /\
#   ||
#   ||
#   y
#   ||
#   ||
#   ----x--->
#           N
#   +---+---+---+---+
#   | 3 | 6 | 8 | 11|
#   +---+---+---+---+
# W | 2 | 5 | 9 | 12| E
#   +---+---+---+---+
#   | 1 | 4 | 10| 13|
#   +---+---+---+---+
#           S
import numpy as np
import matplotlib.pyplot as plt

# Ordering
# xc,yc,dxf,dyf,rac,xg,yg,dxv,dyu,rz,dxc,dyc,raw,ras,dxg,dyg
#  1  2  3   4   5   6  7  8   9  10  11  12  13  14  15  16
fc=['xc','yc','dxf','dyf','rac','xg','yg','dxv','dyu','rz','dxc','dyc','raw','ras','dxg','dyg']

# tile001 - holds f1,f2,f3    data in WE, NS order
# tile002 - holds f4,f5,f6    data in WE, NS order
# tile003 - holds f7          data in WE, NS order
# tile004 - holds f8,f9,f10   data in SN, WE order
# tile005 - holds f11,f12,f13 data in SN, WE order

def read_tiles(fCode,fS):
 fc=['xc','yc','dxf','dyf','rac','xg','yg','dxv','dyu','rz','dxc','dyc','raw','ras','dxg','dyg']
 fl=['tile001.mitgrid', 'tile002.mitgrid', 'tile004.mitgrid', 'tile005.mitgrid']

 try:
  nFld=fc.index(fCode)
 except ValueError:
  print fCode
  return []  
 
 nxt=fS; nyt=fS*3;
 arr=np.zeros((nxt*4,nyt))
 nf=0
 offval=nFld*(fS+1)*(nyt+1)*8
 for fn in fl:
  print fn, nf
  with open(fn,'rb') as f:
   nx, ny = nxt+1, nyt+1
   f.seek(offval,0)
   data = np.fromfile(f, dtype='>f8', count=nx*ny*1)
   if nf <= 1:
    array = np.reshape(data, [nx, ny,  1], order='F')
    array = array[0:nxt,0:nyt,0]
   if nf > 1:
    array = np.reshape(data, [ny, nx,  1], order='F')
    array = array[0:nyt,0:nxt]
    array = array[::-1,:]
    array = np.transpose(array[0:nyt,0:nxt])
  arr[nf*nxt:nf*nxt+nxt,0:nyt]=array[:,:]
  nf=nf+1
 return arr

with open('tilelatlon.bronv8','wb') as f:
 for fcode in fc:
  print 'Reading ',fcode
  arr=read_tiles(fcode,4320)
  adims=arr.shape
  arr1=np.reshape(arr, [adims[0]*adims[1]], order='F')
  print 'Writing ',fcode
  f.write(arr1.astype('>f8').tostring())
