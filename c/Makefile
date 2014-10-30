.PHONY: run

run: ocl_info
	time ./ocl_info

joclinfo: ocl_info
	./ocl_info	

ocl_info: OCLInfo.c
	gcc -l OpenCL -I /opt/intel/opencl/include/ OCLInfo.c -o ocl_info 

