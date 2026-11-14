#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataAsset.h"
#include "Planet.generated.h"



USTRUCT(Blueprintable, BlueprintType)
struct FBodySettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlanetSimulator", meta=(ToolTip="Скорость"))
	FVector Vel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlanetSimulator", meta=(ToolTip="Масса"))
	double Mass = 1.0;
	
	UPROPERTY(BlueprintReadWrite, Category = "PlanetSimulator", meta=(ToolTip="Позиция"))
	FVector Pos;
};

UCLASS(Blueprintable, BlueprintType)
class UPlanetDefaultData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlanetSimulator", meta=(ToolTip="Название планеты"))
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlanetSimulator", meta=(ToolTip="Описание планеты"))
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlanetSimulator", meta=(ToolTip="Настройка планеты"))
	FBodySettings BodySettings;
};

UCLASS()
class PLANETSIMULATOR_API APlanet : public AActor
{
	GENERATED_BODY()
	
public:	
	APlanet();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlanetSimulator", meta=(ToolTip="Прессет планеты"))
	UPlanetDefaultData* PlanetData;
	
	UPROPERTY(BlueprintReadWrite, Category = "PlanetSimulator", meta=(ToolTip="Текущее состояние планеты"))
	FBodySettings PlanetCurrentState;
};
