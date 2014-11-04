.PHONY: run

run: addVector
	time ./addVector

joclinfo: addVector
	./addVector

ocl_info: OCLInfo.c
	gcc -l OpenCL -I /opt/intel/opencl/include/ OCLInfo.c -o ocl_info 

addVector: FIP2AddVectorTP1.c
	gcc -l OpenCL -I /opt/intel/opencl/include/ FIP2AddVectorTP1.c -o addVector

