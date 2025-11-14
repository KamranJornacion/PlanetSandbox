// PhysicsSim.h
#pragma once

#include "CoreMinimal.h"
#include <vector>

// Forward declarations
class CelestialBody;
class SolarSystem;

/**
 * PhysicsSim
 *  - Runs N-body gravity simulation
 *  - Designed to be driven from USimulationController::TickComponent()
 */
class PhysicsSim
{
public:
    PhysicsSim();

    // Set up with a solar system and fixed time step (in seconds)
    void Initialize(SolarSystem* InSolarSystem, float InFixedTimeStep);

    void Start();
    void Pause();
    void Stop();

    // Called every frame from SimulationController (frame-independent)
    void Tick(float DeltaTime);

    bool IsRunning() const      { return bRunning; }
    bool IsInitialized() const  { return bInitialized; }

    // Optional: override bodies directly instead of reading from SolarSystem
    void SetBodies(const std::vector<CelestialBody*>& InBodies);

    void SetTimeStep(float InFixedTimeStep);

private:
    void StepSimulation();   // one fixed-step update
    void ComputeForces();    // fill PendingAccelerations
    void Integrate();        // apply accelerations -> update pos/vel

    SolarSystem* SolarSystemPtr;
    std::vector<CelestialBody*> Bodies;

    // Per-body accelerations for current step
    std::vector<FVector> PendingAccelerations;

    float FixedTimeStep;     // seconds per physics step
    float TimeAccumulator;   // for frame-independent stepping

    bool bRunning;
    bool bInitialized;

    // Tuning constants
    float GravitationalConstant; // in whatever units you choose
    float MinDistance;           // to avoid singularities (softening)
};
