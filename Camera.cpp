#include "Camera.h"
#include "PlanetSimulator/Planet/Planet.h"



ACamera::ACamera()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACamera::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACamera::GoToPlanet(APlanet* Planet)
{
}

