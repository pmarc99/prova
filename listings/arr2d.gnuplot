reset
set autoscale xfixmin
set autoscale xfixmax
set autoscale yfixmin
set autoscale yfixmax
set pm3d map
set palette defined
splot "arr2D.bin" bin array=16x8 format='%lf' with image
