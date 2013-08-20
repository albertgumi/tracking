/**
 * @author: Albert Gutierrez-Milla
 */


#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iostream>
#include <fstream>
#include <vector>

// ROOT libraries
#include "TRandom.h"

using namespace std;

namespace Const {
    const int resolution = 1;
    const int numTracks = 2;
    const int numSensors = 48;
    const int zPosition[] = {-288,-276,-263,-251,-238,-226,-213,-201,-169,-157,-125,
                         -113,-100,-88,-75,-63,-50,-38,-25,-13,0,11,24,36,49,61,
                         74,86,99,111,124,136,149,161,174,186,199,211,224,236,
                         249,261,426,438,604,616,629,641};
};


class Vertex {

public:
    float x;
    float y;
    float z;

};



class Track {

public:
    float dxdz;
    float dydz;
    float mx;
    float my;

};


class Hits {

public:
    int id;
    float x;
    float y;
    int z;
    
};

void saveText(int numHits, list<Hits>* hits, int* hitsPlane, int* hitStart) {

    int i;
    
    ofstream file;
    file.open ("dump.txt");
    
    file << Const::numSensors << endl;
    file << numHits << endl;
    
    for(i=0; i < Const::numSensors; i++) {
        file << Const::zPosition[i] << " ";
    }
    file << endl << endl;
    
    
    for(i=0; i < Const::numSensors; i++) {
        file << hitsPlane[i] << " ";
    }
    file << endl << endl;
    
    
    for(i=0; i < Const::numSensors; i++) {
        file << hitStart[i] << " ";
    }
    file << endl << endl;
    
    
    list<Hits>::const_iterator it;
    
    for(i=0; i < Const::numSensors; i++) {
        for (it = hits[i].begin(); it != hits[i].end(); it++) {
            file << it->id << " ";
        }
    }
    file << endl << endl;

    for(i=0; i < Const::numSensors; i++) {
        for (it = hits[i].begin(); it != hits[i].end(); it++) {
            file << it->x << " ";
        }
    }
    file << endl << endl;

    for(i=0; i < Const::numSensors; i++) {
        for (it = hits[i].begin(); it != hits[i].end(); it++) {
            file << it->y << " ";
        }
    }
    file << endl << endl;

    for(i=0; i < Const::numSensors; i++) {
        for (it = hits[i].begin(); it != hits[i].end(); it++) {
            file << it->z << " ";
        }
    }
    file << endl;
    
    file.close();
}

void saveDump(int numHits, list<Hits>* hits, int* hitsPlane, int* hitStart) {

    int i;
    list<Hits>::const_iterator it;
    
    // TODO don't hardcode
    ofstream outbin( "collision.dump", ios::binary );
    
    // Num sensors
    outbin.write( reinterpret_cast <const char*> (&Const::numSensors), sizeof(int));
    // Num hits
    outbin.write( reinterpret_cast <const char*> (&numHits), sizeof(int));
    // Sensors Z positions
    outbin.write( reinterpret_cast <const char*> (&Const::zPosition), sizeof(int)*Const::numSensors);
    // Hit starts index
    outbin.write( reinterpret_cast <const char*> (hitStart), sizeof(int)*Const::numSensors);
    // hit nums
    outbin.write( reinterpret_cast <const char*> (hitsPlane), sizeof(int)*Const::numSensors);

    // hit ids
    for(i=0; i < Const::numSensors; i++) {
        for (it = hits[i].begin(); it != hits[i].end(); it++) {
            outbin.write( reinterpret_cast <const char*> (&it->id), sizeof(int));
        }
    }

    // X
    for(i=0; i < Const::numSensors; i++) {
        for (it = hits[i].begin(); it != hits[i].end(); it++) {
            outbin.write( reinterpret_cast <const char*> (&it->x), sizeof(float));
        }
    }

    // Y
    for(i=0; i < Const::numSensors; i++) {
        for (it = hits[i].begin(); it != hits[i].end(); it++) {
            outbin.write( reinterpret_cast <const char*> (&it->y), sizeof(float));
        }
    }
    
    // Z
    for(i=0; i < Const::numSensors; i++) {
        for (it = hits[i].begin(); it != hits[i].end(); it++) {
            outbin.write( reinterpret_cast <const char*> (&it->z), sizeof(int));
        }
    }
    
    outbin.close();
    
}


void collision(int &numHits, int* hitsPlane, list<Hits>* hits, int* hitStart) {

    static TRandom r;
    int i, j;

    // Create the vertex around the point (0,0,0)
    Vertex vertex;
    vertex.x = r.Gaus(0.,Const::resolution); 
    vertex.y = r.Gaus(0.,Const::resolution); 
    vertex.z = r.Gaus(0.,Const::resolution);
    
    
    // Create the list of tracks
    list<Track> tracks;
    for(i=0; i < Const::numTracks; i++) {
        Track newTrack;
        
        newTrack.dxdz = r.Uniform(0,0.4);
        newTrack.dydz = r.Uniform(0,0.4);
        
        newTrack.mx = vertex.x;
        newTrack.my = vertex.y;
        
        tracks.push_back(newTrack);
    }

    list<Track>::const_iterator it;
    
    // Iterate over the tracks
    for (it = tracks.begin(); it != tracks.end(); it++) {
        // Iterate over all the XY planes
        for(j=0; j < Const::numSensors; j++) {

            Hits newHit;
            
            //TODO add a distorsion to the hit position, and efficiency
            newHit.x = Const::zPosition[j] * it->dxdz + it->mx;
            newHit.y = Const::zPosition[j] * it->dydz + it->my;
            
            //cout << "newHit.x" << newHit.x << endl;
            //cout << "newHit.y" << newHit.y << endl;
            
            newHit.z = Const::zPosition[j];
            newHit.id = numHits;
            
            // Check if is inside the plane
            if(newHit.x < 50 && newHit.x > -50 && newHit.y < 50 && newHit.y > -50) {
                hits[j].push_back(newHit);  // Add elements to the list
                numHits++;
                hitsPlane[j]++;
            }
        }
    }
}

/**
 * Create a straight line collision
 */
int main() {

    
    int i;
    int numHits = 0;
    int hitsPlane[48] = {0};
    int hitStart[48] = {0};
    
    // create the array of 48 Hits lists
    list<Hits> hitsList[48];
    
    collision(numHits, hitsPlane, hitsList, hitStart);
    hitStart[0] = 0;
    for(i=1; i < Const::numSensors; i++) {
        hitStart[i] = hitStart[i-1] + hitsPlane[i-1];
    }
    saveText(numHits, hitsList, hitsPlane,hitStart);
    saveDump(numHits, hitsList, hitsPlane,hitStart);
    
    return 0;
}

