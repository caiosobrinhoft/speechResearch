#include <iostream>
#include <string>
#include "functions.h" 
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem; // Use boost::filesystem instead of std::filesystem

#define headerSize 44

int main(){

    path p("healthySounds"); // Define the path to the directory here

    for (const auto & entry : directory_iterator(p)) {
        string path = entry.path().string();
        FILE *arq = fopen(path.c_str(), "rb");

        header audioHeader;

        fread(&audioHeader, headerSize, 1, arq);
        fseek(arq, headerSize, SEEK_SET);
        short int signal[audioHeader.subchunk2_size];
        fread(signal, sizeof(short int), audioHeader.subchunk2_size, arq);

        double audioDouble[audioHeader.subchunk2_size];
        for (int i = 0; i < audioHeader.subchunk2_size; i++) {
            audioDouble[i] = static_cast<double>(signal[i]);
        }

        normalizeAudio(audioDouble, audioHeader.subchunk2_size);
        
        double *features = new double[20];
        features = a3(audioDouble, audioHeader.subchunk2_size);
        
        FILE *output = fopen("healthy_featuresa3.txt", "a");
        for (int i = 0; i < 20; i++){
            fprintf(output, "%f, ", features[i]);
        }
        fprintf(output ,"\n");
        fclose(output);
        fclose(arq);
    }

    return 0;
}