#include <stdio.h>
#include <stdlib.h>

/**
 * Create a straight line collision
 */
int main() {

    int sensors = 48;
    int hits = sensors;
    
    int z_position[] =  {-288,-276,-263,-251,-238,-226,-213,-201,-169,-157,-125,
                         -113,-100,-88,-75,-63,-50,-38,-25,-13,0,11,24,36,49,61,
                         74,86,99,111,124,136,149,161,174,186,199,211,224,236,
                         236,249,261,426,438,604,616,629,641};
    
    int hitStarts[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                       17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 
                       31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 
                       45, 46, 47};
    int nums[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

    int pos[] = {1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,
                 1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,
                 1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1,
                 1.1,1.1,1.1};
    
    char *collision_file = "collision_straight.dump";
    FILE *fp;

    fp=fopen(collision_file, "w");

    fwrite(&sensors, sizeof(int), 1, fp);           // Num sensors
    fwrite(&hits, sizeof(int), 1, fp);              // Num hits
    fwrite(z_position, sizeof(int), sensors, fp);   // Sensors Z positions
    fwrite(hitStarts, sizeof(int), sensors, fp);    // Hit starts index
    fwrite(nums, sizeof(int), sensors, fp);         // hit nums
    fwrite(hitStarts, sizeof(int), sensors, fp);    // hit ids
    fwrite(pos, sizeof(int), hits, fp);             // X
    fwrite(pos, sizeof(int), hits, fp);             // Y
    fwrite(z_position, sizeof(int), hits, fp);      // Z
    fclose(fp);

}
