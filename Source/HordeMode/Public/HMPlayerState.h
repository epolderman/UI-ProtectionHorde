
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HMGameMode.h"
#include "HMPlayerState.generated.h"


/*
	Replicated Class Data that holds Kills, Score, Player name
*/

UCLASS()
class HORDEMODE_API AHMPlayerState : public APlayerState
{
	GENERATED_BODY()

	virtual void OnRep_Score() override;

	UFUNCTION()
	void OnRep_Kills();

	UPROPERTY(ReplicatedUsing = OnRep_Kills, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Kills")
	int32 Kills;
public:
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(float DeltaScore);

	float GetScore() const;

	UFUNCTION(BlueprintCallable, Category = "Kills")
	void AddKill(int32 KillCount);

	int32 GetKills() const;
};