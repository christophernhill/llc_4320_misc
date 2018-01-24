# Python script to convert " tile001.mitgrid" LLC files to Bron v1.08 layout
# 

#
#   ----x--->
#   ||
#   ||
#   y
#   ||
#   \/
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

# tile001 - holds f1,f2,f3    data in WE, NS order
# tile002 - holds f4,f5,f6    data in WE, NS order
# tile003 - holds f7          data in WE, NS order
# tile004 - holds f8,f9,f10   data in SN, WE order
# tile005 - holds f11,f12,f13 data in SN, WE order

# Region of interest (in Bron v1.08 coords)
i0,j0=0,0
il,jl=17280,12960
xc=np.zeros((il,jl))

noff=1
offset=4321*12961*8*noff

nxt=4320;
nyt=12960;
fn='tile001.mitgrid'
with open(fn, 'rb') as f:
    nx, ny = nxt+1, nyt+1  # parse; advance file-pointer to data segment
    f.seek(offset,0)
    data = np.fromfile(f, dtype='>f8', count=nx*ny*1)
    array = np.reshape(data, [nx, ny,  1], order='F')
    array = array[0:nxt,0:nyt,0]
print array.min()
print array.max()
print array.shape
xc[0*nxt:0*nxt+nxt,0:nyt]=array[:,:]

nxt=4320;
nyt=12960;
fn='tile002.mitgrid'
with open(fn, 'rb') as f:
    nx, ny = nxt+1, nyt+1  # parse; advance file-pointer to data segment
    f.seek(offset,0)
    data = np.fromfile(f, dtype='>f8', count=nx*ny*1)
    array = np.reshape(data, [nx, ny,  1], order='F')
    array = array[0:nxt,0:nyt,0]
print array.min()
print array.max()
print array.shape
xc[1*nxt:1*nxt+nxt,0:nyt]=array[:,:]

nxt=4320;
nyt=12960;
fn='tile004.mitgrid'
with open(fn, 'rb') as f:
    nx, ny = nxt+1, nyt+1  # parse; advance file-pointer to data segment
    f.seek(offset,0)
    data = np.fromfile(f, dtype='>f8', count=nx*ny*1)
    array = np.reshape(data, [ny, nx,  1], order='F')
    array = array[0:nyt,0:nxt]
    array = array[::-1,:]
    array = np.transpose(array[0:nyt,0:nxt])
print array.min()
print array.max()
print array.shape
xc[2*nxt:2*nxt+nxt,0:nyt]=array[:,:]

nxt=4320;
nyt=12960;
fn='tile005.mitgrid'
with open(fn, 'rb') as f:
    nx, ny = nxt+1, nyt+1  # parse; advance file-pointer to data segment
    f.seek(offset,0)
    data = np.fromfile(f, dtype='>f8', count=nx*ny*1)
    array = np.reshape(data, [ny, nx,  1], order='F')
    array = array[0:nyt,0:nxt]
    array = array[::-1,:]
    array = np.transpose(array[0:nyt,0:nxt])
print array.min()
print array.max()
print array.shape
xc[3*nxt:3*nxt+nxt,0:nyt]=array[:,:]

plt.imshow(xc)
# plt.plot(xc[nxt/2,:])
plt.show()
