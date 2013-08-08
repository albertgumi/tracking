    // Load grid
    // Load hits ordered
    // Load list for downward link and upward link
    // Load list of number of hits per device
    // Load list of the Z positions


/**
 * Give a identifier of a grid position, but don't parallelize by the grid
 * parallelize using the number of hits.
 */
__kernel void vector_add(
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
