#include "MovementManager.h"

#include "Components/SplineComponent.h"
#include "Planet/Planet.h"



void UMovementManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (Planets.Num() < 2) return;

	TArray<FBodySettings> S;
	for (APlanet* P : Planets) S.Add(P->PlanetCurrentState);

	const double dt = GetWorld()->GetDeltaSeconds() * TimeScale / SubSteps;
	for (int32 k = 0; k < SubSteps; ++k)
	{
		Step(S, dt);
	}

	for (int32 i = 0; i < Planets.Num(); ++i)
	{
		Planets[i]->PlanetCurrentState = S[i];
		Planets[i]->SetActorLocation(S[i].Pos);
	}
}

void UMovementManager::PlanetRegister(APlanet* Planet)
{
	PlanetAdded.Broadcast(Planet);
	Planets.AddUnique(Planet);
}

void UMovementManager::PlanetUnRegister(APlanet* Planet)
{
	Planets.Remove(Planet);
}

void UMovementManager::SelectPlanet(APlanet* InPlanet)
{
	if (CurrentPlanet == InPlanet)
	{
		return;
	}
	
	CurrentPlanet = InPlanet;
	SelectedPlanet.Broadcast(CurrentPlanet);
}

TArray<FVector> UMovementManager::PredictTrajectory_Implementation_Implementation(APlanet* Target)
{
	TArray<FVector> Path;
	
	if (!Target || Planets.Num() < 2) return Path;
	
	// Копировать все в темп что бы не трогать планеты
	TArray<FBodySettings> S;
	S.Reserve(Planets.Num()); // Разметить массив
	int32 TargetIdx = INDEX_NONE;
	for (int32 i = 0; i < Planets.Num(); ++i)
	{
		S.Add(Planets[i]->PlanetCurrentState);
		if (Planets[i] == Target) TargetIdx = i;
	}
	if (TargetIdx == INDEX_NONE) return Path;
	
	const float PredictDt      = 0.05f;   // шаг прогноза
	const int32 TotalSteps     = FMath::CeilToInt(PredictSeconds / PredictDt); // Колличество шагов траектории
	const int32 SampleStride   = 20;
	
	Path.Reserve(TotalSteps / SampleStride + 1);
	Path.Add(S[TargetIdx].Pos);

	for (int32 s = 1; s <= TotalSteps; ++s)
	{
		Step(S, PredictDt);
		if (s % SampleStride == 0) Path.Add(S[TargetIdx].Pos); //Заполнить массив точек для траектории
	}
	
	return Path;
}

void UMovementManager::Step(TArray<FBodySettings>& S, double dt)
{
	TArray<FVector> Accelerations;
	
	// Заполнить массив нулями
	Accelerations.SetNumZeroed(S.Num());
	
	for (int32 i = 0; i < S.Num(); i++)
	{
		for (int32 j = i + 1; j < S.Num(); j++)
		{
			double Distance = FVector::DistSquared(S[i].Pos, S[j].Pos) + 1.f;
			
			float v1 = FMath::Square(G * S[i].Mass / Distance);
			float v2 = FMath::Square(G * S[j].Mass / Distance);
			// Направление 
			FVector Direction = (S[j].Pos - S[i].Pos).GetSafeNormal();
			
			// Величина ускорения первой планеты
			double AccelMagnitude = (G * S[j].Mass) / Distance;
			
			// Величина ускорения зависимой планеты
			double AccelMagnitudeSecondPlanet = (G * S[i].Mass) / Distance;
			
			//Вектор ускорения
			FVector AccelVector = Direction * AccelMagnitude;
			
			//Вектор ускорения второй планеты
			FVector AccelVectorSecondPlanet = Direction * AccelMagnitudeSecondPlanet;
			
			Accelerations[i] += AccelVector;
			Accelerations[j] -= AccelVectorSecondPlanet;
		}
	}
	
	for (int32 i = 0; i < S.Num(); ++i)
	{
		S[i].Vel += Accelerations[i] * dt;
		S[i].Pos += S[i].Vel * dt;
	}
}
