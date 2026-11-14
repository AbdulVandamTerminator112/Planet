#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera.generated.h"



class APlanet;

UCLASS()
class PLANETSIMULATOR_API ACamera : public APawn
{
	GENERATED_BODY()
	
public:	
	ACamera();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void GoToPlanet(APlanet* Planet);
};
