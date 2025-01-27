#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Door.generated.h"

UCLASS()
class WALKING_API ADoor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ADoor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    void OnInteract();

    class APlayerCharacter* Character;

private:

    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    class UStaticMeshComponent* DoorFrame;

    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    class UStaticMeshComponent* Door;

protected:

    FTimeline Timeline;
    UPROPERTY(EditAnywhere)
    UCurveFloat* CurveFloat;

    bool bIsDoorClosed = true;

    UPROPERTY(EditAnywhere)
    float DoorRotateAngle = 90.f;

    UFUNCTION()
    void OpenDoor(float Value); // Bind function

    bool bDoorOnSameSide;
    void SetDoorOnSameSide();
};