#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <CL/cl.h>

#include "definitions.h"

#define MAX_SOURCE_SIZE (0x100000)
#define CLUSTER_ROWS 50 // Constant number of rows for the cluster structure
#define CLUSTER_COLS 50 // Constant number of columns for the cluster structure
#define NUM_VELO 48     // Total number of VELO detectors

#define VELO_MIN -40
#define VELO_MAX 40

typedef struct cluster {
    int position;       // First index in the hits structure where the elements start
    int num_elems;      // Number of elements inside this cluster
} cluster;

typedef struct hit_str {
    float x;
    float y;
    int z;
    //int id;   // Is the ID necessary during the calculation?
} hit_str;

struct link {
    int index;
    struct link* next;
}link;

cluster grid[NUM_VELO][CLUSTER_ROWS][CLUSTER_COLS];

hit_str hit[1943]; // TODO hardcoded load dinamically


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

    printf("no_hits %d\n",*h_no_hits);
#ifdef DEBUG
    printf("h_no_sensors %d\n",h_no_sensors);
    printf("no_hits %d\n",h_no_hits);
    printf("sensor_Zs %d\n",&h_sensor_Zs);
    printf("sensor_hitStarts %d\n",h_sensor_hitStarts);
    printf("sensor_hitNums %d\n",h_sensor_hitNums);
    printf("hit_IDs %d\n",h_hit_IDs);
    printf("hit_Xs %f\n",h_hit_Xs);
    printf("hit_Ys %f\n",h_hit_Ys);
    printf("hit_Zs %d\n",h_hit_Zs);
#endif

}

/** Sort hits according the the position inside the cluster.
 * Save the 
 * This approach cannot be used inside the Device because function malloc cannot
 * be used inside.
 */
void sortHits() {
    int max = INT_MIN;
    int i, j, k;
    int *list;  // List where will me stored the hits per panel
    int row, cols;   // Calculated row and column
    int count;  // counter of the number of elements of the hits of a panel
    
    struct elem_cluster{
        int row;    // index of the row in the final cluster structer
        int column;  // index of the column in the final cluster structer
        int hit_index;  // Save the index to acces later
        struct elem_cluster *next;
    } elem_cluster;
    
    struct elem_cluster *first, *last, *new, *current, *previous;
    
    for(i=0; i < *h_no_sensors; i++) {
        if (max < h_sensor_hitStarts[i]) {
            max = h_sensor_hitStarts[i];
        }
    }
    
    // Reservar cada vez, no solo la primera
    list=malloc(max*sizeof(int));
    
    // TODO sort the elements in a bidimensional space
    
    for(i=0; i < *h_no_sensors; i++) {
    
        list=malloc(h_sensor_hitNums[i]*sizeof(int));
        count = 1;      // We didn't use any element of the list, still
        
        // Treat the element 0
        first = (struct elem_cluster*) malloc( sizeof(struct elem_cluster));
        last = first;
        
        
        // Generate the struct
        for(j=1; j < h_sensor_hitNums[i]; j++) {
            // TODO Calculate the position inside the grid
            // TODO create a data struct where save the position of the grid
            
            // TODO every time a data structue is insterted sort it
            
            new = (struct elem_cluster*) malloc( sizeof(struct elem_cluster));
            

            // Add the sruct inside the linked list
            for(k=0; k < count; k++) {
                // TODO check if the position is less or more
                
                // TODO if it's less than the current element, then introduce it before
            
                if(current->row * CLUSTER_COLS + current->column <= row * CLUSTER_COLS + cols ) {
                    new->next = previous->next;
                    previous->next = new;
                    break;  // Go out of the loop
                }
                
                if(current == last) {
                    last->next = new;
                    last = new;
                }
            }
            count++;
        }
        // TODO pass the linked list to the array
        // TODO free memory
    }
}

/** Load the collision from a dump file.
 * This function will be modified as the read operations will be diferent in 
 * production.
 * 
 */
void loadCollision(int* size) {

    // TODO this file is hardcoded for development purposes
	char* dumpFile = "../dump/pixel-sft-event-0.dump"; // Dump file name
	//int size;                           // Size of the dump file
	char* input;
	readFile(dumpFile,&input, size);

    printf("Size %d\n",*size);
}

void checkError(int retValue, char *msg) {
    if(retValue != 0) {
        fprintf(stderr, "Error: %s\n", msg);
        exit(-1);
    }
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
 
    fp = fopen("hits_finder.cl", "r");
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
 
    // TODO
    // Load grid
    // Load hits ordered
    // Load list for downward link and upward link
    // Load list of number of hits per device
    // Load list of the Z positions
    // Load the total number of hits
    
    
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
 
    checkError(ret, "Copying from device to host");
    
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
 
    checkError(ret, "Setting the arguments of the function");
    
    /* TODO the number of threads doesn't has to be hardcoded.
       the number of threads has to be related to the geometry, the number
       of devices and the size of the grid.
    */
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
        //printf("%f, %f, %f\n", A[i], B[i], C[i]);
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
    
    checkError(ret, "Releasing variables");

    free(A);
    free(B);
    free(C);
    return 0;
}

int main() {
	int size;                           // Size of the dump file
    
    int i, j, k;
    
    // TODO consider initialization with memset
    for(i=0; i < NUM_VELO; i++) {
        for(j=0; j < CLUSTER_ROWS; j++) {
            for(k=0; k < CLUSTER_COLS; k++) {
                grid[i][j][k].position = -1;
                grid[i][j][k].num_elems = -1;
            }
        }
    }
    
	loadCollision(&size);
    printf("Size %d\n",size);
    gpuLoad();

    return 0;
}
