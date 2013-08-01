    // Load grid
    // Load hits ordered
    // Load list for downward link and upward link
    // Load list of number of hits per device
    // Load list of the Z positions


/**
 * Give a identifier of a grid position, but don't parallelize by the grid
 * parallelize using the number of hits.
 */
__kernel void vector_add(__global const cluster *grid, __global const hit_str *hits, __global const int *down_up, int hits) {
 
    // Get the index of the current element to be processed
    int id = get_global_id(0);

    int i;




    // Do the operation
    C[i] = A[i] + B[i] + N;
    
}
