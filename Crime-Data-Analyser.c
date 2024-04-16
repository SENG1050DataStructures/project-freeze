#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>



//structure for crime data
struct CrimeData {
    std::string location;
    std::string type;
    double latitude;
    double longitude;
    
};

// Function to load crime data from a text file
void loadCrimeData(const std::string& filename, CrimeData*& crimeData, int& dataSize) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    int index = 0;
    while (std::getline(file, line) && index < dataSize) {
        std::istringstream iss(line);
        CrimeData crime;
        if (iss >> crime.location >> crime.type >> crime.latitude >> crime.longitude) {
            crimeData[index++] = crime;
        }
    }
    file.close();
    dataSize = index;
}
