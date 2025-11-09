#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "CelestialBody.h"

using namespace std;

class SolarSystem {
    public: 
        // constructor
        SolarSystem();
        // accessors
        CelestialBody GetBody(int index) const;
        CelestialBody GetBody(const string& name) const;
        vector<CelestialBody> GetAllBodies() const;
        float GetTimeStep() const;
        // methods
        void AddBody(const CelestialBody& body);
        void RemoveBody(const string& name);
        void SetTimeStep(float timeStep);
        
    private: 
        vector<CelestialBody> bodies;
        float timeStep; 
        string name;

};

#endif // SOLARSYSTEM_H