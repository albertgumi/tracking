#include <stdlib.h>
#include <stdio.h>
#include "tools.h"

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

void quickSortInput(char* input){
    int i;
	for(i=0; i<h_no_sensors[0]; i++)
        quickSort(h_hit_Xs, h_hit_Ys, h_hit_IDs, h_hit_Zs,
		    h_sensor_hitStarts[i], h_sensor_hitStarts[i] + h_sensor_hitNums[i]);
}

void quickSort(float* hit_Xs, float* hit_Ys, int* hit_IDs, int* hit_Zs, int _beginning, int _end)
{
	const int max_levels = 300;
	int beg[max_levels], end[max_levels], i=0, L, R, swap;

	double piv, d1;
	int i1, i2;

	beg[0]=_beginning; end[0]=_end;
	while (i>=0) {
		L=beg[i]; R=end[i]-1;
		if (L<R) {

			piv = hit_Xs[L];
			d1  = hit_Ys[L];
			i1  = hit_IDs[L];
			i2  = hit_Zs[L];

			while (L<R) {
				while (hit_Xs[R] >= piv && L < R) R--;
				if (L<R){
					hit_Xs[L] = hit_Xs[R];
					hit_Ys[L] = hit_Ys[R];
					hit_Zs[L] = hit_Zs[R];
					hit_IDs[L] = hit_IDs[R];
					L++;
				}

				while (hit_Xs[L] <= piv && L < R) L++;
				if (L<R){
					hit_Xs[R] = hit_Xs[L];
					hit_Ys[R] = hit_Ys[L];
					hit_Zs[R] = hit_Zs[L];
					hit_IDs[R] = hit_IDs[L];
					R--;
				}
			}
			hit_Xs[L] = piv;
			hit_Ys[L] = d1;
			hit_IDs[L] = i1;
			hit_Zs[L] = i2;

			beg[i+1]=L+1; end[i+1]=end[i]; end[i++]=L;
			if (end[i]-beg[i]>end[i-1]-beg[i-1]) {
				swap=beg[i]; beg[i]=beg[i-1]; beg[i-1]=swap;
				swap=end[i]; end[i]=end[i-1]; end[i-1]=swap;
			}
		}
		else {
			i--;
		}
	}
}
