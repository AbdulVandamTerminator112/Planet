// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MovementManager.generated.h"

struct FBodySettings;
class APlanet;



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FSelectedPlanet, APlanet*, Planet ); // Делегат сообщает какая планета выбрана.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FPlanetAdded, APlanet*, Planet ); // Планета добавлена в список

UCLASS( Blueprintable, BlueprintType)
class PLANETSIMULATOR_API UMovementManager : public UTickableWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UMovementManager, STATGROUP_Tickables);
	}
	
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<FVector> PredictTrajectory_Implementation(APlanet* Target);
	
	UFUNCTION(BlueprintCallable)
	void PlanetRegister(APlanet* Planet);
	
	UFUNCTION(BlueprintCallable)
	void PlanetUnRegister(APlanet* Planet);
	
	UFUNCTION(BlueprintCallable)
	void SelectPlanet(APlanet* InPlanet);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlanetSimulator", meta=(ToolTip="Гравитационная постоянная"))
	double G = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlanetSimulator", meta=(ToolTip="Ускорение времени"))
	double TimeScale = 1.0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlanetSimulator", meta=(ToolTip="Шагов физики в секунду"))
	int32 SubSteps = 20;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlanetSimulator", meta=(ToolTip="Конец траектории в конце секунд ..."))
	float PredictSeconds = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlanetSimulator", meta=(ToolTip="Список планет"))
	TArray<TObjectPtr<APlanet>> Planets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlanetSimulator", meta=(ToolTip="Выбранная планета"))
	APlanet* CurrentPlanet = nullptr;
	
	UPROPERTY(BlueprintAssignable , Category = "PlanetSimulator")
	FSelectedPlanet SelectedPlanet;
	
	UPROPERTY(BlueprintAssignable , Category = "PlanetSimulator")
	FPlanetAdded PlanetAdded;
	
private:
	UFUNCTION()
	void Step(TArray<FBodySettings>& S, double dt);
};
