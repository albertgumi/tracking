#include <stdlib.h>
#include <stdio.h>
#include "loadCollision.h"

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
void readFile(char* filename, int* size){
	
	char** input;
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


/** Load the collision from a dump file.
 * This function will be modified as the read operations will be diferent in 
 * production.
 * 
 */
void loadCollision(int* size) {

    // TODO this file is hardcoded for development purposes
	char* dumpFile = "pixel-sft-event-0.dump"; // Dump file name
	//int size;                           // Size of the dump file
		
	readFile(dumpFile, size);

    printf("Size %d\n",*size);
}
