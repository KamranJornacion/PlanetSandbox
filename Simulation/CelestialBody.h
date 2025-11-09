#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;


class CelestialBody {
    public:
        CelestialBody(const string& name, float mass, float radius);
    private:
        string name;
        float mass; // in kilograms?
        float radius; // in meters?

};

#endif // CELESTIALBODY_H