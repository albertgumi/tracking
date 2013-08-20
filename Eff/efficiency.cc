/**
 * This algorithm aims to load the original tracks of the montecarlo simulation
 * and the calculated ones and extract: efficiency, ghost rate
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std; 

class Track {
public:
    float dxdz;
    float dydz;
    float mx;
    float my;

};

/**
 * 
 */
void loadTrack(vector<Track> &track, string filename) {

    ifstream trackFile;
    
    trackFile.open(filename.c_str());
    

    if (trackFile.is_open()) {
        while (!trackFile.eof()) {
            Track newTrack;
            trackFile >> newTrack.dxdz >> newTrack.dydz >> newTrack.mx >> newTrack.my;
            track.push_back(newTrack);
        }
    }
    trackFile.close();
}

/**
 *
 */
void calculateRates(vector<Track> &original, vector<Track> reconstructed) {

    // TODO calculate ghost rate
    // ghost rate: tracks that were not reconstructed
    
    
    // TODO calculate efficiency
}

int main() {
    vector<Track> original;
    vector<Track> reconstructed;
    
    loadTrack(original, "original.track");
    
    loadTrack(reconstructed, "reconstructed.track");
    
}
