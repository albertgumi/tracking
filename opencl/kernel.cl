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

typedef struct downup_str {
    int down;
    int up;
} downup_str;


__kernel void vector_add(__global const cluster *grid, __global const hit_str *hit, __global downup_str *downup, int N) {
 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
 
    // Do the operation
    //C[i] = A[i] + B[i] + N;
    
    int f = 33*3;
    
    
    if(i < 1943) {
        downup[i].down = 5;
        downup[i].up = -5;
    }
}
