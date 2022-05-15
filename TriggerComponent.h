#pragma once

#include "CoreMinimal.h"
#include "Mover.h"
#include "Components/BoxComponent.h"
#include "TriggerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UTriggerComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* Mover);
	
	
protected:
	virtual void BeginPlay() override;
	
private:

	UPROPERTY(EditAnywhere)
	FName UnlockTag = "Unlock1";

	UMover* Mover;

	AActor* GetAcceptableActor() const;

	bool IsUnlocked= false;
};
