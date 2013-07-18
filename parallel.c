#include <stdio.h>
#include <stdlib.h>
 
#include <CL/cl.h>

 
#define MAX_SOURCE_SIZE (0x100000)

int* h_no_sensors;
int* h_no_hits;
int* h_sensor_Zs;
int* h_sensor_hitStarts;
int* h_sensor_hitNums;
int* h_hit_IDs;
float* h_hit_Xs;
float* h_hit_Ys;
int* h_hit_Zs;

/** Read the dump file that contains the tracking information.
 *
 * @param filename route to the dump file
 * @param input char vector pointer that will hold the information of the file
 * @param size holds the size of the file
 */
void readFile(char* filename, char** input, int* size){
	
    FILE *fd;
    
    fd = fopen(filename, "rb");
    if(fd == NULL) {
        fprintf(stderr,"The file %s cannot be opened\n",filename);
        exit(-1);
    }
    
    fseek(fd, 0, SEEK_END);
    *size = ftell(fd);       // Get the size of the file
    
    rewind(fd);
    *input = (char*)malloc(*size * (sizeof(char)));
    fread(*input, sizeof(char), *size, fd);     //read the content
    fclose(fd);

	h_no_sensors = (int*) input[0];
	h_no_hits = (int*) (h_no_sensors + 1);
	h_sensor_Zs = (int*) (h_no_hits + 1);
	h_sensor_hitStarts = (int*) (h_sensor_Zs + h_no_sensors[0]);
	h_sensor_hitNums = (int*) (h_sensor_hitStarts + h_no_sensors[0]);
	h_hit_IDs = (int*) (h_sensor_hitNums + h_no_sensors[0]);
	h_hit_Xs = (float*) (h_hit_IDs + h_no_hits[0]);
	h_hit_Ys = (float*) (h_hit_Xs + h_no_hits[0]);
	h_hit_Zs = (int*) (h_hit_Ys + h_no_hits[0]);


//#ifdef DEBUG
    printf("h_no_sensors %d\n",*h_no_sensors);
    printf("no_hits %d\n",*h_no_hits);
    printf("sensor_Zs %d\n",*h_sensor_Zs);
    printf("sensor_hitStarts %d\n",*h_sensor_hitStarts);
    printf("sensor_hitNums %d\n",*h_sensor_hitNums);
    printf("hit_IDs %d\n",*h_hit_IDs);
    printf("hit_Xs %f\n",*h_hit_Xs);
    printf("hit_Ys %f\n",*h_hit_Ys);
    printf("hit_Zs %d\n",*h_hit_Zs);
//#endif

}
int gpuLoad(void) {
    // Create the two input vectors
    int i;
    const int LIST_SIZE = *h_no_hits;
    float *A = (float*)malloc(sizeof(float)*LIST_SIZE);
    float *B = (float*)malloc(sizeof(float)*LIST_SIZE);
    for(i = 0; i < LIST_SIZE; i++) {
        A[i] = h_hit_Xs[i];
        B[i] = h_hit_Ys[i];
    }
 
    // Load the kernel source code into the array source_str
    FILE *fp;
    char *source_str;
    size_t source_size;
 
    fp = fopen("kernel.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );
 
    // Get platform and device information
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;   
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1, 
            &device_id, &ret_num_devices);
 
    // Create an OpenCL context
    cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
 
    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
 
    // Create memory buffers on the device for each vector 
    cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
            LIST_SIZE * sizeof(float), NULL, &ret);
    cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
            LIST_SIZE * sizeof(float), NULL, &ret);
    cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
            LIST_SIZE * sizeof(float), NULL, &ret);
 
    // Copy the lists A and B to their respective memory buffers
    ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
            LIST_SIZE * sizeof(float), h_hit_Xs, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, 
            LIST_SIZE * sizeof(float), B, 0, NULL, NULL);
 
    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, 
            (const char **)&source_str, (const size_t *)&source_size, &ret);
 
    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
 
    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "vector_add", &ret);
 
    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);
    ret = clSetKernelArg(kernel, 3, sizeof(int), (void *)h_no_sensors);
 
    // Execute the OpenCL kernel on the list
    size_t global_item_size = LIST_SIZE; // Process the entire lists
    size_t local_item_size = 67; // Divide work items into groups of 64
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, 
            &global_item_size, &local_item_size, 0, NULL, NULL);
 
    // Read the memory buffer C on the device to the local variable C
    float *C = (float*)malloc(sizeof(float)*LIST_SIZE);
    ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, 
            LIST_SIZE * sizeof(float), C, 0, NULL, NULL);
 
    // Display the result to the screen
    for(i = 0; i < LIST_SIZE; i++) {
        printf("%f, %f, %f\n", A[i], B[i], C[i]);
    }
    
    // Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(a_mem_obj);
    ret = clReleaseMemObject(b_mem_obj);
    ret = clReleaseMemObject(c_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    free(A);
    free(B);
    free(C);
    return 0;
}

int main() {
	char* c = "pixel-sft-event-0.dump"; // Dump file name
	int size;                           // Size of the dump file
	char* input;                        // content of the dump file
	int i;
		
	readFile(c, &input, &size);
    gpuLoad();
    return 0;
}

