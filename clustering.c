#include <stdio.h>
#include <limits.h>
#include <math.h>
#include "tools.h"


extern int* h_no_sensors;
extern int* h_no_hits;
extern int* h_sensor_Zs;
extern int* h_sensor_hitStarts;
extern int* h_sensor_hitNums;
extern int* h_hit_IDs;
extern float* h_hit_Xs;
extern float* h_hit_Ys;
extern int* h_hit_Zs;


int neigh[1943][2];     // temporaly hardcoded
    
/*
int cluster() {

    int i, j;
    int hit = 0;
    for(i=0; i < *h_no_sensors; i++) {
        //printf("%d,",   h_sensor_hitNums[i]);
        for(j=0; j < h_sensor_hitNums[i]; j++) {
            //printf("Sensor %d hit %d: (%f,%f,%d)\n",i,hit,h_hit_Xs[hit],h_hit_Ys[hit],h_hit_Zs[hit]);
            //printf("%d,",h_hit_Zs[hit]);
            hit++;
        }
    }
}
*/

int neighbours_find() {

    int i, j, k, l;
    int hit = 0;
    int up, down;
    
    int up_candidates[40];
    int down_candidates[40];
    int total = 0;
    
    // TODO before the internal loops select the subset of elemnts that have slope 1
    
    int i_down, i_up;
    i_down = 0;
    i_up = h_sensor_hitNums[0] + h_sensor_hitNums[1];
    float x_slope, y_slope;
    x_slope = y_slope = 1000.0;
    int up_selected, down_selected;
    
    // We will access the sensors N-1, N, N+1
    for(i=1; i < *h_no_sensors - 1; i++) {
    
        // Iterate over the sensors of the hit
        for(j=0; j < h_sensor_hitNums[i]; j++, hit++) {

            // Out max considered slope is 1.
            for(k=0; k < h_sensor_hitNums[i-1]; k++) {
            
                if((fabs(h_hit_Xs[i_down + k]-h_hit_Xs[hit])*2 < abs(h_hit_Zs[hit] - h_hit_Zs[i_down-1])) && 
                    ((fabs(h_hit_Ys[i_down + k] - h_hit_Ys[hit])*2 < abs(h_hit_Zs[hit] - h_hit_Zs[i_down-1]))) ) {
                    //total++;
                    //printf("Hit: %d k:%d X:%f Y:%f Z:%d\n",hit,k,fabs(h_hit_Xs[i_down + k]-h_hit_Xs[hit]),fabs(h_hit_Ys[i_down + k] - h_hit_Ys[hit]) ,abs(h_hit_Zs[hit] - h_hit_Zs[i_down-1]));
                    for(l=0; l < h_sensor_hitNums[i+1]; l++) {
                    
                        if((fabs(h_hit_Xs[i_up + l]-h_hit_Xs[hit])*2 < abs(h_hit_Zs[hit] - h_hit_Zs[i_up])) && 
                            ((fabs(h_hit_Ys[i_up + l] - h_hit_Ys[hit])*2 < abs(h_hit_Zs[hit] - h_hit_Zs[i_up]))) ) {
                            total++;
                            
                            printf("Hit: %d k:%d l: %d X:%f Y:%f Z:%d\n",hit,k,l,fabs(h_hit_Xs[i_up + l]-h_hit_Xs[hit]),fabs(h_hit_Ys[i_up + l] - h_hit_Ys[hit]) ,h_hit_Zs[i_up] - h_hit_Zs[hit]);     
                            //printf("Hit: %d k:%d X:%f Y:%f Z:%d\n",hit,k,fabs(h_hit_Xs[i_down + k]-h_hit_Xs[hit]),fabs(h_hit_Ys[i_down + k] - h_hit_Ys[hit]) ,abs(h_hit_Zs[hit] - h_hit_Zs[i_down-1]));     
                            if() {
                                
                            }
                            down_selected = i_down + k;
                            up_selected = i_up + l;
                            
                        }
                    }
                }
            }
            x_slope = y_slope = 1000.0;
        }
        i_down += h_sensor_hitNums[i-1];
        i_up += h_sensor_hitNums[i+1];
    }
    
    printf("Total %d over %d\n",total,*h_no_hits);
    
    return 0;
}


// TODO is necessary before save the hits that are potientially involved in one
// track
int neighbours_clean() {

    int i, j;
    
    // The position 0 is downwards link
    // the position 1 is upwards link

    for(i = 1; i < *h_no_hits - 1; i++) {
        if(neigh[neigh[i][0]][1] != i) {
            neigh[neigh[i][0]][1] = NULL;   // They don't share the link
            neigh[i][0] = NULL;             // so we clean the link
        }
        if(neigh[neigh[i][1]][0] != i) {
            neigh[neigh[i][1]][0] = NULL;   // They don't share the link
            neigh[i][1] = NULL;             // so we clean the link
        }
    }
    return 0;
}

int main() {

	char* c = "pixel-sft-event-0.dump"; // Dump file name
	int size;                           // Size of the dump file
	char* input;                        // content of the dump file
	int i;
		
	readFile(c, &input, &size);

    printf("Size %d\n",size);
    
    printf("h_no_sensors %d\n",*h_no_sensors);
    printf("no_hits %d\n",*h_no_hits);
    printf("sensor_Zs %d\n",*h_sensor_Zs);
    printf("sensor_hitStarts %d\n",*h_sensor_hitStarts);
    printf("sensor_hitNums %d\n",*h_sensor_hitNums);
    printf("hit_IDs %d\n",*h_hit_IDs);
    printf("hit_Xs %f\n",*h_hit_Xs);
    printf("hit_Ys %f\n",*h_hit_Ys);
    printf("hit_Zs %d\n",*h_hit_Zs);
    
    neighbours_find();
}
