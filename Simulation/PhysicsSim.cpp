// PhysicsSim.cpp

#include "PhysicsSim.h"
#include "SolarSystem.h"
#include "CelestialBody.h"

// If you have your own log category, include it here
//#include "YourProjectLog.h"

PhysicsSim::PhysicsSim()
    : SolarSystemPtr(nullptr)
    , FixedTimeStep(1.0f)
    , TimeAccumulator(0.0f)
    , bRunning(false)
    , bInitialized(false)
    , GravitationalConstant(1.0f)  // You can scale this to get nice orbits
    , MinDistance(100.0f)          // Softening distance to avoid crazy forces
{
}

void PhysicsSim::Initialize(SolarSystem* InSolarSystem, float InFixedTimeStep)
{
    SolarSystemPtr = InSolarSystem;
    FixedTimeStep  = InFixedTimeStep;
    TimeAccumulator = 0.0f;

    Bodies.clear();
    PendingAccelerations.clear();

    // TODO: adjust this call to match your SolarSystem API.
    // For example, if you use TArray, expose a std::vector or add a helper.
    // Bodies = SolarSystemPtr->GetAllBodies();
    if (SolarSystemPtr)
    {
        // Example pattern if SolarSystem exposes std::vector<CelestialBody*>
        Bodies = SolarSystemPtr->GetAllBodies();
    }

    PendingAccelerations.resize(Bodies.size(), FVector::ZeroVector);

    bInitialized = (SolarSystemPtr != nullptr && !Bodies.empty());
}

void PhysicsSim::SetBodies(const std::vector<CelestialBody*>& InBodies)
{
    Bodies = InBodies;
    PendingAccelerations.resize(Bodies.size(), FVector::ZeroVector);
}

void PhysicsSim::SetTimeStep(float InFixedTimeStep)
{
    FixedTimeStep = FMath::Max( KINDA_SMALL_NUMBER, InFixedTimeStep );
}

void PhysicsSim::Start()
{
    if (!bInitialized)
    {
        return;
    }

    bRunning = true;
    TimeAccumulator = 0.0f;
}

void PhysicsSim::Pause()
{
    bRunning = false;
}

void PhysicsSim::Stop()
{
    bRunning = false;
    TimeAccumulator = 0.0f;
}

void PhysicsSim::Tick(float DeltaTime)
{
    if (!bRunning || !bInitialized)
    {
        return;
    }

    // Frame-independent stepping
    TimeAccumulator += DeltaTime;

    while (TimeAccumulator >= FixedTimeStep)
    {
        StepSimulation();
        TimeAccumulator -= FixedTimeStep;
    }
}

void PhysicsSim::StepSimulation()
{
    if (Bodies.empty())
    {
        return;
    }

    ComputeForces();
    Integrate();
}

void PhysicsSim::ComputeForces()
{
    const int32 N = static_cast<int32>(Bodies.size());
    if (N == 0)
    {
        return;
    }

    // Reset accelerations
    for (int32 i = 0; i < N; ++i)
    {
        PendingAccelerations[i] = FVector::ZeroVector;
    }

    // Simple N-body O(N^2) gravity
    for (int32 i = 0; i < N; ++i)
    {
        CelestialBody* BodyA = Bodies[i];
        if (!BodyA) continue;

        // TODO: adjust to match your CelestialBody API:
        // float GetMass() const;
        // FVector GetPosition() const;
        const float MassA = BodyA->GetMass();
        const FVector PosA = BodyA->GetPosition();

        for (int32 j = i + 1; j < N; ++j)
        {
            CelestialBody* BodyB = Bodies[j];
            if (!BodyB) continue;

            const float MassB = BodyB->GetMass();
            const FVector PosB = BodyB->GetPosition();

            FVector Delta = PosB - PosA;
            float DistSq = Delta.SizeSquared();

            // Softening to avoid division by zero / ridiculous forces
            if (DistSq < MinDistance * MinDistance)
            {
                DistSq = MinDistance * MinDistance;
            }

            const float Dist = FMath::Sqrt(DistSq);
            const FVector Direction = Delta / Dist;

            // Newtonian gravity: F = G * m1 * m2 / r^2
            const float ForceMag = GravitationalConstant * MassA * MassB / DistSq;

            // a = F / m
            const FVector AccelA =  ForceMag / MassA * Direction;
            const FVector AccelB = -ForceMag / MassB * Direction;

            PendingAccelerations[i] += AccelA;
            PendingAccelerations[j] += AccelB;
        }
    }
}

void PhysicsSim::Integrate()
{
    const int32 N = static_cast<int32>(Bodies.size());
    if (N == 0)
    {
        return;
    }

    const float dt = FixedTimeStep;

    for (int32 i = 0; i < N; ++i)
    {
        CelestialBody* Body = Bodies[i];
        if (!Body) continue;

        // TODO: adjust to match your CelestialBody API:
        // FVector GetVelocity() const;
        // void   SetVelocity(const FVector&);
        // void   SetPosition(const FVector&);
        FVector Vel = Body->GetVelocity();
        FVector Pos = Body->GetPosition();

        // Simple (semi-implicit) Euler integration
        Vel += PendingAccelerations[i] * dt;
        Pos += Vel * dt;

        Body->SetVelocity(Vel);
        Body->SetPosition(Pos);
    }

    // Optional: call a collision resolver here if you implement one later
    // ResolveCollisions();
}
