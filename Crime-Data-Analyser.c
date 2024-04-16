#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#define M_PI 3.14159265 //for geospatial calculations


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

// Function to calculate distance between two coordinates (Haversine formula)

double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    constexpr double EarthRadiusKm = 6371.0;
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    double a = sin(dLat / 2) * sin(dLat / 2) +
        cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
        sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double distance = EarthRadiusKm * c;
    return distance;
}

// Function to get crimes within a specified radius of a given coordinate
CrimeData* getCrimesInRadius(const CrimeData* crimeData, int dataSize, double targetLatitude, double targetLongitude, double radius, int& resultSize) {
    // Initialize an array to store crimes within the specified radius
    const int INITIAL_CAPACITY = 100; // Initial capacity of the array
    int capacity = INITIAL_CAPACITY;
    CrimeData* crimesInRadius = new CrimeData[capacity];
    int count = 0; // Number of crimes within the radius

    // Iterate over the crime data and check if each crime falls within the radius
    for (int i = 0; i < dataSize; ++i) {
        double distance = calculateDistance(targetLatitude, targetLongitude, crimeData[i].latitude, crimeData[i].longitude);
        if (distance <= radius) {
            // Crime falls within the radius, add it to the array
            crimesInRadius[count++] = crimeData[i];

            // Check if the array is full and resize if necessary
            if (count == capacity) {
                // Double the capacity of the array
                capacity *= 2;
                CrimeData* temp = new CrimeData[capacity];
                // Copy existing data to the resized array
                for (int j = 0; j < count; ++j) {
                    temp[j] = crimesInRadius[j];
                }
                delete[] crimesInRadius;
                crimesInRadius = temp;
            }
        }
    }

    // Update the size of the result array
    resultSize = count;

    return crimesInRadius;
}

// Function to display crime data
void displayCrimeData(const CrimeData* crimes, int size) {
    std::cout << "Crimes within the specified radius:" << std::endl;
    if (size == 0) {
        std::cout << "No crimes found within the specified radius.\n";
    }
    else {
        for (int i = 0; i < size; ++i) {
            std::cout << "Location: " << crimes[i].location << "\nType:\t   " << crimes[i].type << "\n___________________________________________\n";
        }
    }
}

