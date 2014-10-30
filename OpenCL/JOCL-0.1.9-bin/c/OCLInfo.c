#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <CL/opencl.h>

char *getPlatformString(cl_platform_id platform, int paramName);
char *getDeviceString(cl_device_id device, int paramName);
cl_long *getLongs(cl_device_id device, int paramName, int numValues);
cl_long getLong(cl_device_id device, int paramName);
cl_int *getInts(cl_device_id device, int paramName, int numValues);
cl_int getInt(cl_device_id device, int paramName);
size_t getSize(cl_device_id device, int paramName);
size_t *getSizes(cl_device_id device, int paramName, int numValues);
char *stringFor_cl_device_fp_config(cl_long n);

char *stringFor_cl_device_fp_config(cl_long n){
  char *result = malloc(256);
  if(0 == n){
    return "(none)";
  }
  if((n & CL_FP_DENORM) != 0){
    strcat(result, "CL_FP_DENORM ");
  }
  if((n & CL_FP_INF_NAN) != 0){
    strcat(result, "CL_FP_INF_NAN ");
  }
  if((n & CL_FP_ROUND_TO_NEAREST) != 0){
    strcat(result, "CL_FP_ROUND_TO_NEAREST ");
  }
  if((n & CL_FP_ROUND_TO_ZERO) != 0){
    strcat(result, "CL_FP_ROUND_TO_ZERO ");
  }
  if((n & CL_FP_ROUND_TO_INF) != 0){
    strcat(result, "CL_FP_ROUND_TO_INF ");
  }
  if((n & CL_FP_FMA) != 0){
    strcat(result, "CL_FP_FMA ");
  }
  if((n & CL_FP_SOFT_FLOAT) != 0){
    strcat(result, "CL_FP_SOFT_FLOAT ");
  }
  if((n & CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT) != 0){
    strcat(result, "CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT ");
  }
  return result;
}

size_t getSize(cl_device_id device, int paramName){
  size_t *s = getSizes(device, paramName, 1);
  size_t res = s[0];
  free(s);
  return res;
}

size_t *getSizes(cl_device_id device, int paramName, int numValues){
  cl_uchar *buffer = malloc(numValues * sizeof(size_t));
  int i = 0;
  clGetDeviceInfo(device, paramName, sizeof(size_t)*numValues, buffer, NULL);
  size_t *values = malloc(sizeof(size_t)*numValues);
  if(4 == sizeof(size_t)){
    for(i = 0; i < numValues; i++){
      values[i] = *((size_t*)(buffer+i*sizeof(size_t)));
    }
  }
  else{
    for(i = 0; i < numValues; i++){
      values[i] = *((cl_ulong*)(buffer+i*sizeof(size_t)));
    }
  }
  return values;
}

char *getPlatformString(cl_platform_id platform, int paramName){
  size_t size;
  char *buffer;
  clGetPlatformInfo(platform, paramName, 0, NULL, &size);
  buffer = malloc(size+1);
  clGetPlatformInfo(platform, paramName, size, buffer, NULL);
  return buffer;
}

char *getDeviceString(cl_device_id device, int paramName){
  size_t size;
  char *buffer;
  clGetDeviceInfo(device, paramName, 0, NULL, &size);
  buffer = malloc(size+1);
  clGetDeviceInfo(device, paramName, size, buffer, NULL);
  return buffer;
}

cl_long *getLongs(cl_device_id device, int paramName, int numValues){
  cl_long *values = malloc(sizeof(cl_long)*numValues);
  clGetDeviceInfo(device, paramName, sizeof(cl_long)*numValues, values, NULL);
  return values;
}

cl_long getLong(cl_device_id device, int paramName){
  cl_long *l = getLongs(device, paramName, 1);
  cl_long res = l[0];
  free(l);
  return res;
}

cl_int getInt(cl_device_id device, int paramName){
  cl_int *i = getInts(device, paramName, 1);
  cl_int res = i[0];
  free(i);
  return res;
}

cl_int *getInts(cl_device_id device, int paramName, int numValues){
  cl_int *values = malloc(sizeof(cl_int)*numValues);
  clGetDeviceInfo(device, paramName, sizeof(cl_int)*numValues, values, NULL);
  return values;
}

int main(int argc, char **argv){
  cl_uint numPlatforms;
  cl_platform_id *platforms;
  int i = 0, j= 0;
  int globalNumberOfDevices = 0;
  cl_device_id *devices = NULL;
  clGetPlatformIDs(0, NULL, &numPlatforms);
  printf("Number of platforms: %d\n",numPlatforms);
  platforms = malloc(sizeof(cl_platform_id*)*numPlatforms);
  clGetPlatformIDs(numPlatforms, platforms, NULL);
  for(i = 0; i < numPlatforms; i++){
    cl_uint numDevices;
    cl_device_id *devicesArray;
    char *platformName = getPlatformString(platforms[i], CL_PLATFORM_NAME);
    clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
    printf("Number of devices in platform %s: %d\n",platformName,numDevices);
    free(platformName);
    devicesArray = malloc(sizeof(cl_device_id)*numDevices);
    clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, numDevices, devicesArray, NULL);
    if(NULL == devices){
      devices = malloc(sizeof(cl_device_id)*(globalNumberOfDevices+numDevices));
    }
    else{
      devices = realloc(devices, sizeof(cl_device_id)*(globalNumberOfDevices+numDevices));
    }
    for(j = 0; j < numDevices; j++){
      devices[globalNumberOfDevices+j] = devicesArray[j];
    }
    free(devicesArray);
    globalNumberOfDevices += numDevices;
  }
  free(platforms);
  for(i = 0; i < globalNumberOfDevices; i++){
    cl_device_id device = devices[i];
    char *deviceName = getDeviceString(device, CL_DEVICE_NAME);
    printf("--- Info for device %s: ---\n", deviceName);
    printf("CL_DEVICE_NAME: \t\t\t%s\n", deviceName);
    free(deviceName);
    char *deviceVendor = getDeviceString(device, CL_DEVICE_VENDOR);
    printf("CL_DEVICE_VENDOR: \t\t\t%s\n", deviceVendor);
    free(deviceVendor);
    char *driverVersion = getDeviceString(device, CL_DRIVER_VERSION);
    printf("CL_DRIVER_VERSION: \t\t\t%s\n", driverVersion);
    free(driverVersion);
    long deviceType = getLong(device, CL_DEVICE_TYPE);
    if((deviceType & CL_DEVICE_TYPE_CPU) != 0){
      printf("CL_DEVICE_TYPE:\t\t\t\t%s\n", "CL_DEVICE_TYPE_CPU");
    }
    if((deviceType & CL_DEVICE_TYPE_GPU) != 0){
      printf("CL_DEVICE_TYPE:\t\t\t\t%s\n", "CL_DEVICE_TYPE_GPU");
    }
    if((deviceType & CL_DEVICE_TYPE_ACCELERATOR) != 0){
      printf("CL_DEVICE_TYPE:\t\t\t\t%s\n", "CL_DEVICE_TYPE_ACCELERATOR");
    }
    if((deviceType & CL_DEVICE_TYPE_DEFAULT) != 0){
      printf("CL_DEVICE_TYPE:\t\t\t\t%s\n", "CL_DEVICE_TYPE_DEFAULT");
    }
    cl_int maxComputeUnits = getInt(device, CL_DEVICE_MAX_COMPUTE_UNITS);
    printf("CL_DEVICE_MAX_COMPUTE_UNITS:\t\t%d\n", maxComputeUnits);
    long maxWorkItemDimensions = getLong(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS);
    printf("CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:\t%ld\n", maxWorkItemDimensions);
    size_t *maxWorkItemSizes = getSizes(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, 3);
    printf("CL_DEVICE_MAX_WORK_ITEM_SIZES:\t\t%ld / %ld / %ld \n"
             , maxWorkItemSizes[0], maxWorkItemSizes[1], maxWorkItemSizes[2]);
    free(maxWorkItemSizes);
    long maxWorkGroupSize = getSize(device, CL_DEVICE_MAX_WORK_GROUP_SIZE);
    printf("CL_DEVICE_MAX_WORK_GROUP_SIZE:\t\t%ld\n", maxWorkGroupSize);
    long maxClockFrequency = getLong(device, CL_DEVICE_MAX_CLOCK_FREQUENCY);
    printf("CL_DEVICE_MAX_CLOCK_FREQUENCY:\t\t%ld MHz\n", maxClockFrequency);
    int addressBits = getInt(device, CL_DEVICE_ADDRESS_BITS);
    printf("CL_DEVICE_ADDRESS_BITS:\t\t\t%d\n", addressBits);
    long maxMemAllocSize = getLong(device, CL_DEVICE_MAX_MEM_ALLOC_SIZE);
    printf("CL_DEVICE_MAX_MEM_ALLOC_SIZE:\t\t%d MByte\n", (int)(maxMemAllocSize / (1024 * 1024)));
    long globalMemSize = getLong(device, CL_DEVICE_GLOBAL_MEM_SIZE);
    printf("CL_DEVICE_GLOBAL_MEM_SIZE:\t\t%d MByte\n", (int)(globalMemSize / (1024 * 1024)));
    int errorCorrectionSupport = getInt(device, CL_DEVICE_ERROR_CORRECTION_SUPPORT);
    printf("CL_DEVICE_ERROR_CORRECTION_SUPPORT:\t%s\n", errorCorrectionSupport != 0 ? "yes" : "no");
    int localMemType = getInt(device, CL_DEVICE_LOCAL_MEM_TYPE);
    printf("CL_DEVICE_LOCAL_MEM_TYPE:\t\t%s\n", localMemType == 1 ? "local" : "global");
    long localMemSize = getLong(device, CL_DEVICE_LOCAL_MEM_SIZE);
    printf("CL_DEVICE_LOCAL_MEM_SIZE:\t\t%d KByte\n", (int)(localMemSize / 1024));
    long maxConstantBufferSize = getLong(device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE);
    printf("CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE:\t%d KByte\n", (int)(maxConstantBufferSize / 1024));
    long queueProperties = getLong(device, CL_DEVICE_QUEUE_PROPERTIES);
    if(( queueProperties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE ) != 0)
        printf("CL_DEVICE_QUEUE_PROPERTIES:\t\t%s\n", "CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE");
    if(( queueProperties & CL_QUEUE_PROFILING_ENABLE ) != 0)
        printf("CL_DEVICE_QUEUE_PROPERTIES:\t\t%s\n", "CL_QUEUE_PROFILING_ENABLE");
    int imageSupport = getInt(device, CL_DEVICE_IMAGE_SUPPORT);
    printf("CL_DEVICE_IMAGE_SUPPORT:\t\t%d\n", imageSupport);
    int maxReadImageArgs = getInt(device, CL_DEVICE_MAX_READ_IMAGE_ARGS);
    printf("CL_DEVICE_MAX_READ_IMAGE_ARGS:\t\t%d\n", maxReadImageArgs);
    int maxWriteImageArgs = getInt(device, CL_DEVICE_MAX_WRITE_IMAGE_ARGS);
    printf("CL_DEVICE_MAX_WRITE_IMAGE_ARGS:\t\t%d\n", maxWriteImageArgs);
    long singleFpConfig = getLong(device, CL_DEVICE_SINGLE_FP_CONFIG);
    printf("CL_DEVICE_SINGLE_FP_CONFIG:\t\t%s\n",
        stringFor_cl_device_fp_config(singleFpConfig));
    long image2dMaxWidth = getSize(device, CL_DEVICE_IMAGE2D_MAX_WIDTH);
    printf("CL_DEVICE_2D_MAX_WIDTH\t\t\t%ld\n", image2dMaxWidth);
    long image2dMaxHeight = getSize(device, CL_DEVICE_IMAGE2D_MAX_HEIGHT);
    printf("CL_DEVICE_2D_MAX_HEIGHT\t\t\t%ld\n", image2dMaxHeight);
    long image3dMaxWidth = getSize(device, CL_DEVICE_IMAGE3D_MAX_WIDTH);
    printf("CL_DEVICE_3D_MAX_WIDTH\t\t\t%ld\n", image3dMaxWidth);
    long image3dMaxHeight = getSize(device, CL_DEVICE_IMAGE3D_MAX_HEIGHT);
    printf("CL_DEVICE_3D_MAX_HEIGHT\t\t\t%ld\n", image3dMaxHeight);
    long image3dMaxDepth = getSize(device, CL_DEVICE_IMAGE3D_MAX_DEPTH);
    printf("CL_DEVICE_3D_MAX_DEPTH\t\t\t%ld\n", image3dMaxDepth);
    printf("CL_DEVICE_PREFERRED_VECTOR_WIDTH_\t");
    int preferredVectorWidthChar = getInt(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR);
    int preferredVectorWidthShort = getInt(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT);
    int preferredVectorWidthInt = getInt(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT);
    int preferredVectorWidthLong = getInt(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG);
    int preferredVectorWidthFloat = getInt(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT);
    int preferredVectorWidthDouble = getInt(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE);
    printf("CHAR %d, SHORT %d, INT %d, LONG %d, FLOAT %d, DOUBLE %d\n\n\n",
           preferredVectorWidthChar, preferredVectorWidthShort,
           preferredVectorWidthInt, preferredVectorWidthLong,
           preferredVectorWidthFloat, preferredVectorWidthDouble);
  }
  free(devices);
  return 0;
}

          
