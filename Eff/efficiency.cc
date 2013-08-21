/**
 * This algorithm aims to load the original tracks of the montecarlo simulation
 * and the calculated ones and extract: efficiency, ghost rate
 *
 * @author: Albert Gutierrez-Milla
 * @date: 20 Aug 2013
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
 * Load track from file to a Track data structure.
 *
 * @param track Data structure to store the track
 * @param filename Path to the file that stores the track
 */
void loadTrack(vector<Track> &track, string filename) {

    ifstream trackFile;
    
    trackFile.open(filename.c_str());
    int i = 0;

    if (trackFile.is_open()) {
        Track newTrack;
        while (trackFile >> newTrack.dxdz >> newTrack.dydz >> newTrack.mx >> newTrack.my) {
            track.push_back(newTrack);
        }
    } else {
        cerr << "Error opening the file " << filename << endl;
    }
    trackFile.close();
}

/**
 * Calculate reconstruction efficiency, ghost fraction, clone fraction, purity
 * and collection efficiency of an orginal and reconstructed track
 * The accepted error is absolute, not proportional.
 * 
 * @param original Tracks used for the Monte Carlo simulation
 * @param reconstructed Tracks reconstructed by the tracking algorithm
 */
void calculateRates(vector<Track> &original, vector<Track> &reconstructed) {

    int i, j;
    
    float slopeErr = 0.02;    // Slope error accepted to consider a valid track
    float interErr = 0.01;    // Intercept error accepted to consider a valid track

    int recEff = 0;
    
    // Reconstruction efficiency
    for(i = 0; i != original.size(); i++) {
        for(j = 0; j != reconstructed.size(); j++) {
        
            // if they are the same inside an error
            if( (original[i].dxdz + slopeErr) > reconstructed[j].dxdz && 
                (original[i].dxdz - slopeErr) < reconstructed[j].dxdz && 
                (original[i].dydz + slopeErr) > reconstructed[j].dydz && 
                (original[i].dydz - slopeErr) < reconstructed[j].dydz &&
                (original[i].mx + interErr) > reconstructed[j].mx && 
                (original[i].mx - interErr) < reconstructed[j].mx &&
                (original[i].my + interErr) > reconstructed[j].my && 
                (original[i].my - interErr) < reconstructed[j].my) 
            {
                recEff++;
            }
        }
    }
    
    cout << "Reconstruction efficiency: " << (recEff/original.size()) * 100 << endl;
    


    // Ghost fraction
    cout << "Ghost fraction: " << 100 - (recEff/original.size()) * 100 << endl;


    int clone = 0;
    // Clone fraction
    for(i = 0; i != reconstructed.size()-1; i++) {
        for(j = i+1; j != reconstructed.size(); j++) {

            // if they are the same inside an error
            if( (reconstructed[i].dxdz + slopeErr) > reconstructed[j].dxdz && 
                (reconstructed[i].dxdz - slopeErr) < reconstructed[j].dxdz && 
                (reconstructed[i].dydz + slopeErr) > reconstructed[j].dydz && 
                (reconstructed[i].dydz - slopeErr) < reconstructed[j].dydz &&
                (reconstructed[i].mx + interErr) > reconstructed[j].mx && 
                (reconstructed[i].mx - interErr) < reconstructed[j].mx &&
                (reconstructed[i].my + interErr) > reconstructed[j].my && 
                (reconstructed[i].my - interErr) < reconstructed[j].my) 
            {
                clone++;
            }
        }
    }
    
    cout << "Clone fraction: " << (clone/original.size()) * 100 << endl;
    
    // TODO Purity
    // TODO Collection efficiency

}

int main(int argc, char* argv[]) {

    vector<Track> original;
    vector<Track> reconstructed;
    
    if(argc != 3) {
        cout << "Usage:" << endl << argv[0] << " [original.track] [reconstructed.track]" << endl;
        return -1;
    }
    
    loadTrack(original, argv[1]);   
    loadTrack(reconstructed, argv[2]);
    
    calculateRates(original,reconstructed);
    
    return 0;
}
