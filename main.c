#include "definitions.h"
#include "tools.h"
#include "kalman.h"
#include "extrapolation.h"

// TODO: debug purposes only
extern float* h_hit_Xs;
extern float* h_hit_Ys;
extern int*   h_hit_Zs;

extern int*   h_prevs;
extern int*   h_nexts;


int main()
{
	char* c = "pixel-sft-event-0.dump"; // Dump file name
	int size;                           // Size of the dump file
	char* input;                        // content of the dump file
	int i;
		
	readFile(c, &input, &size);

    printf("Size %d\n",size);

	// Pre-processing, quick sort over X
	//quickSortInput(input);
	
    return 0;
}
