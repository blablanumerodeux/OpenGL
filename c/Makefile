execAddVector: addVector
	./addVectorThread

addVector: addVectorThread.c
	gcc -O3 -lpthread -l OpenCL -I /opt/intel/opencl/include/ addVectorThread.c -o addVectorThread

