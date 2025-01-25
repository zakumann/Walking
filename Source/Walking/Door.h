#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
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

    // Timeline for smooth door animations
    UPROPERTY()
    class UTimelineComponent* DoorTimeline;

    // Curve for animation
    UPROPERTY(EditAnywhere, Category = "Door")
    class UCurveFloat* DoorCurve;

    UPROPERTY(EditAnywhere, Category = "Door")
    class UStaticMeshComponent* DoorMesh;

    FOnTimelineFloat InterpFunction;
    FOnTimelineEvent TimelineFinished;

    // States and rotations
    bool bIsOpen;
    FRotator ClosedRotation;
    FRotator OpenRotation;

    // Trigger box
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    UBoxComponent* TriggerBox;

    UFUNCTION()
    void OnTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnTriggerBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // Function to update door rotation
    UFUNCTION()
    void HandleDoorProgress(float Value);

    UFUNCTION()
    void OnTimelineFinished();

    // Reference to the player character
    AActor* OverlappingPlayer;

public:
    // Toggle door state
    UFUNCTION(BlueprintCallable, Category = "Door")
    void ToggleDoor();
};
