#include <string.h>
#pragma comment(lib, "OpenCL.lib")
#include "CL\Cl.h"
#include <iostream>

int main()
{
cl_platform_id platform_id;
cl_uint ret_num_platforms;
cl_device_id device_id;
cl_uint ret_num_devices;
cl_context context;
cl_command_queue command_queue;
cl_program program;
char* source_str="__kernel void test(__global int* message){int gid = get_global_id(0); message[gid] = gid; int i;}";
char** zzz=&source_str;
size_t source_size=strlen(source_str);
size_t* szz =&source_size;

/* получить доступные платформы */
cl_int ret;
ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
if (ret==0)     std::cout<< "clGetPlatformIDs             OK."<<          std::endl;
    else std::cout<< "clGetPlatformIDs          Error "<<ret<<"."<<std::endl;

/* получить доступные устройства */
ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
if (ret==0)     std::cout<< "clGetDeviceIDs               OK."<<          std::endl;
    else std::cout<< "clGetDeviceIDs            Error "<<ret<<"."<<std::endl;

/* создать контекст */
context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
if (ret==0)     std::cout<< "clCreateContext              OK."<<          std::endl;
    else std::cout<< "clCreateContext           Error "<<ret<<"."<<std::endl;

/* создаем команду */
command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
if (ret==0)     std::cout<< "clCreateCommandQueue         OK."<<          std::endl;
    else std::cout<< "clCreateCommandQueue      Error "<<ret<<"."<<std::endl;

/* создать бинарник из кода программы */
program = clCreateProgramWithSource(context, 1,(const char **)&source_str, (const size_t *)&source_size, &ret);
if (ret==0)     std::cout<< "clCreateProgramWithSource    OK."<<          std::endl;
    else std::cout<< "clCreateProgramWithSource Error "<<ret<<"."<<std::endl;

/* скомпилировать программу */
ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
if (ret==0)     std::cout<< "clBuildProgram               OK."<<          std::endl;
    else std::cout<< "clBuildProgram            Error "<<ret<<"."<<std::endl;

/* создать кернел */
cl_kernel kernel = clCreateKernel(program, "test", &ret);
if (ret==0)     std::cout<< "clCreateKernel               OK."<<          std::endl;
    else std::cout<< "clCreateKernel            Error "<<ret<<"."<<std::endl;
cl_mem memobj = NULL;
int memLenth = 10;
cl_int* mem = (cl_int *)malloc(sizeof(cl_int) * memLenth);

/* создать буфер */
memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, memLenth * sizeof(cl_int), NULL, &ret);
if (ret==0)     std::cout<< "clCreateBuffer               OK."<<          std::endl;
    else std::cout<< "clCreateBuffer            Error "<<ret<<"."<<std::endl;

/* записать данные в буфер */
ret = clEnqueueWriteBuffer(command_queue, memobj, CL_TRUE, 0, memLenth * sizeof(cl_int), mem, 0, NULL, NULL);
if (ret==0)     std::cout<< "clEnqueueWriteBuffer         OK."<<          std::endl;
    else std::cout<< "clEnqueueWriteBuffer      Error "<<ret<<"."<<std::endl;

/* устанавливаем параметр */
ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);
if (ret==0)     std::cout<< "clSetKernelArg               OK."<<          std::endl;
    else std::cout<< "clSetKernelArg            Error "<<ret<<"."<<std::endl;
size_t global_work_size[1] = { 10 };//  оличество потоков наверно

/* выполнить кернел */
ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, NULL);
if (ret==0)     std::cout<< "clEnqueueNDRangeKernel       OK."<<          std::endl;
    else std::cout<< "clEnqueueNDRangeKernel    Error "<<ret<<"."<<std::endl;

/* считать данные из буфера */
ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0, memLenth * sizeof(float), mem, 0, NULL, NULL);
if (ret==0)     std::cout<< "clEnqueueReadBuffer          OK."<<          std::endl;
    else std::cout<< "clEnqueueReadBuffer       Error "<<ret<<"."<<std::endl;

for (int f=0;f<10;f++) std::cout<<mem[f]<<std::endl;
system("pause");
return 0;
}
