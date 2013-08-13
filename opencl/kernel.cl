typedef struct cluster {
    int position;       // First index in the hits structure where the elements start
    int num_elems;      // Number of elements inside this cluster
} cluster;

typedef struct hit_str {
    float x;
    float y;
    int z;
} hit_str;

typedef struct downup_str {
    int down;
    int up;
} downup_str;


#define ABS(num) { if(num < 0.0) {return -num;} return num}
/*
inline float abs(float num) {
    if(num < 0.0) {
        return -num;
    }
    return num;
}
*/

float Abs(float num) {
    if(num < 0.0) {
        return -num;
    }
    return num;
}


__kernel void vector_add(
        __global const cluster      *grid, 
        __global const hit_str      *hit, 
        __global       downup_str   *downup, 
        __global const int          *z_list, 
                       int          sensors, 
                       int          hits, 
        __global       float        *a) // TODO remove A argument
{
    
    int CLUSTER_ROWS = 50;
    int CLUSTER_COLS = 50;
    int VELO_MIN = -40.0;
    int VELO_MAX = 40.0;

    int z_index = 0;
    int i, j, k, l, m, n;
    int row, col;
    int i_down_grid, i_up_grid;
    int start_col, start_row;

    float min_slope_x = 9999999.9;  // The lowest slope difference in X
    float min_slope_y = 9999999.9;  // The lowest slope difference in Y
    float min_slope = 9999999.9;
    
    float tmp_slope_x_up, tmp_slope_x_down;
    float tmp_slope_y_up, tmp_slope_y_down;
    
    int up_id = 0;  // ID of the upward link
    int down_id = 0;    // ID of the downward link
    
    //float cluster_row_size = (-VELO_MIN + VELO_MAX) / CLUSTER_ROWS;   // This returns 1.0 instead of 1.6
    //float cluster_col_size = (-VELO_MIN + VELO_MAX) / CLUSTER_COLS;   // su madre
    float cluster_row_size = 1.6;   // TODO don't hardcode the number
    float cluster_col_size = 1.6;
    
    float sum = -2; 
    
    // Get the index of the current element to be processed
    int id = get_global_id(0);
    
    
    if(id < hits) {
        
        for(i=0; i < sensors; i++) {
            if(hit[id].z == z_list[i]) {
                z_index = i;
            }
        }
        
        row = (int) ((-VELO_MIN + hit[id].x)/cluster_row_size);
        col = (int) ((-VELO_MIN + hit[id].y)/cluster_col_size);

        // Take into account the limits of the grid
        if(row > 0) {
            start_row = row-1;
        } else {
            start_row=0;
        }

        if(col > 0) {
            start_col = col-1;
        } else {
            start_col=0;
        }
        
        
        if(z_index != 0 && z_index != (sensors-1)) {

            // Iterate over the 9 grids of the cluster
            for(i = start_row; i <= (row+1) && i < CLUSTER_ROWS; i++) {
            
                //down_id = 3;
                //up_id = 3;
                    
                for(j = start_col; j <= (col+1) && j < CLUSTER_COLS; j++) {
                
                    // We consider first the downwards
                    i_down_grid = (z_index-1)*CLUSTER_ROWS*CLUSTER_COLS + i*CLUSTER_COLS + j;
                    
                    //down_id = 2;
                    //up_id = 2;
                    
                    if(grid[i_down_grid].position != -1) {  // If there is a hit we go inside
                    
                        //down_id = 1;
                        //up_id = 1;
                        
                        for(k = grid[i_down_grid].position; k < (grid[i_down_grid].num_elems + grid[i_down_grid].position); k++) {
                            // Calculate the slope 
                            
                            // m = (x2-x1)/(z2-z1)
                            tmp_slope_x_down = Abs(hit[k].x-hit[id].x)/Abs(hit[k].z-hit[id].z);
                            // m = (y2-y1)/(z2-z1)
                            tmp_slope_y_down = Abs(hit[k].y-hit[id].y)/Abs(hit[k].z-hit[id].z);
                            
                            //down_id = -2;
                            //up_id = -2;
                            
                            // Iterate over the upward
                            for(l = start_row; l <= (row+1) && l < CLUSTER_ROWS; l++) {
                            
                                //down_id = -3;
                                //up_id = -3;
                                    
                                for(m = start_col; m <= (col+1) && m < CLUSTER_COLS; m++) {
                                    i_up_grid = (z_index+1)*CLUSTER_ROWS*CLUSTER_COLS + l*CLUSTER_COLS + m;
                                    
                                    //down_id = -4;
                                    //up_id = -4;
                                         
                                    if(grid[i_up_grid].position != -1) {  // If there is a hit we go inside
                                        //down_id = -5;
                                        //up_id = -5;
                                        for(n = grid[i_up_grid].position; n < (grid[i_up_grid].num_elems + grid[i_up_grid].position); n++) {
                                        
                                            // TODO this access can be done once and save but we cannot use Malloc...

                                            // Calculate the slope
                                            // m = (x2-x1)/(z2-z1)
                                            tmp_slope_x_up = (hit[n].x-hit[id].x)/(hit[n].z-hit[id].z);
                                            // m = (y2-y1)/(z2-z1)
                                            tmp_slope_y_up = (hit[n].y-hit[id].y)/(hit[n].z-hit[id].z);

                                            // TODO make it absolute value
                                            //if(abs(tmp_slope_x_up - tmp_slope_x_down) +  abs(tmp_slope_y_up - tmp_slope_y_down) < min_slope) {
                                            sum = (tmp_slope_x_up - tmp_slope_x_down) + (tmp_slope_y_up - tmp_slope_y_down);
                                            
                                            down_id = hit[n].z;   // 2147483647 MAX_INT
                                            up_id = (hit[n].x-hit[id].x);
                                            
                                            if((tmp_slope_x_up - tmp_slope_x_down) + (tmp_slope_y_up - tmp_slope_y_down) < min_slope) {
                                                min_slope = (tmp_slope_x_up - tmp_slope_x_down) +  (tmp_slope_y_up - tmp_slope_y_down);
                                                
                                                //min_slope = ABS(ABS(tmp_slope_x_up) - ABS(tmp_slope_x_down)) +  ABS(ABS(tmp_slope_y_up) - ABS(tmp_slope_y_down));
                                                
                                                
                                                //a[id] = min_slope;
                                                a[id] = hit[id].y;
                                                down_id = hit[n].z;
                                                up_id = hit[id].x;
                                                //down_id = k;
                                                //up_id = n;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            //down_id = grid[29*50+24].position;
            //up_id = grid[29*50+24].num_elems;
            //up_id = i_down_grid;
            
            //down_id = start_row ;
            //up_id =start_col;
        } else {
            down_id = -1;
            up_id = -1;
        }


        downup[id].down = down_id;
        downup[id].up = up_id;
    }
    
    //barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
}



__kernel void neighbour_cleaner(
        __global       downup_str   *downup, 
                       int          hits,
                       int          starthits) 
{

    int id = get_global_id(0);
    
    if(id < hits) {
        int down = downup[id].down;
        int up = downup[id].up;
        
        if(downup[down].up != id) {
            downup[down].up = -1;
            downup[id].down = -1;
        }
        
        if(downup[up].down != id) {
            downup[up].down = -1;
            downup[id].up = -1;
        }
    }
}
