#include "HordeMode/Public/HMGameMode.h"
#include "Components/SHealthComponent.h"
#include <TimerManager.h>
#include <GameFramework/Actor.h>
#include <Engine/World.h>
#include "HordeMode/Public/HMGameState.h"
#include "HMPlayerState.h"
#include "UI/HMHUD.h"
#include <DeclarativeSyntaxSupport.h>
#include "Components/SSTitleWidget.h"


// Tick Runs 60 times per sec / 1 for each frame

AHMGameMode::AHMGameMode()
{
	GameStateClass = AHMGameState::StaticClass();
	PlayerStateClass = AHMPlayerState::StaticClass();

	TimeBetweenWaves = 2.0f;
	PrimaryActorTick.bCanEverTick = true;
	// once a second
	PrimaryActorTick.TickInterval = 1.0f;;
	isGameOver = false;
}

void AHMGameMode::SetWaveState(EWaveState NewState)
{
	// template, we can pass in our type, and it handles the cast for us
	AHMGameState * GS = GetGameState<AHMGameState>();
	if (ensureAlways(GS)) {
		GS->SetWaveState(NewState);
	}
}

// only runs on the server, grabs all playerControllers
void AHMGameMode::RestartDeadPlayers()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
		APlayerController * CurrentPlayer = It->Get();
		if (CurrentPlayer != nullptr && CurrentPlayer->GetPawn() == nullptr) {
			RestartPlayer(CurrentPlayer);
		}
	}
}

void AHMGameMode::StartPlay()
{
	Super::StartPlay();
	InitNextWave();
}

void AHMGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AHMGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (isGameOver)
	return;

	if (!IsAnyPlayerAlive()) {
		GameOver();
	}

	CheckWaveState();
}

void AHMGameMode::StartWave()
{
	WaveCount++;

	NumberOfBotsToSpawnInCurrentWave = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &AHMGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);

	SetWaveState(EWaveState::WaveStart);
	UE_LOG(LogTemp, Warning, TEXT("GameMode: WaveStart"));
}

void AHMGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
	if(!isGameOver)
	SetWaveState(EWaveState::WaveInProgress);
}

void AHMGameMode::InitNextWave()
{
	SetWaveState(EWaveState::WaitingToStart);
	RestartDeadPlayers();
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &AHMGameMode::StartWave, TimeBetweenWaves, false);
}

void AHMGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	NumberOfBotsToSpawnInCurrentWave--;
	UE_LOG(LogTemp, Warning, TEXT("Number of bots left %i"), NumberOfBotsToSpawnInCurrentWave);

	if (NumberOfBotsToSpawnInCurrentWave <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("GameMode: Ending Wave-------------->"));
		EndWave();
	}
}

void AHMGameMode::CheckWaveState()
{
	UE_LOG(LogTemp, Warning, TEXT("CheckWaveState()"));

	bool isPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);
	if (NumberOfBotsToSpawnInCurrentWave > 0 || isPreparingForWave) {
		UE_LOG(LogTemp, Warning, TEXT("Fail 1"));
		return;
	}

	bool bisAnyBotAlive = false;

	// check if any bots are alive
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn * testPawn = It->Get();

		if (testPawn == nullptr || testPawn->IsPlayerControlled()) {
			continue; // skips rest of iteration, remaining for loop
		}
		
		USHealthComponent * HealthComp = Cast<USHealthComponent>(testPawn->GetComponentByClass(USHealthComponent::StaticClass()));

		if (HealthComp != nullptr && HealthComp->GetHealth() > 0.0f) {
			bisAnyBotAlive = true;
			break;
		}
	}

	if (!bisAnyBotAlive && IsAnyPlayerAlive()) {

		SetWaveState(EWaveState::WaveComplete);;
		InitNextWave();
		UE_LOG(LogTemp, Warning, TEXT("Init Next Wave"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Init Next Wave FAILING..."));
	}
	

}

bool AHMGameMode::IsAnyPlayerAlive() const
{	
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
		APlayerController * CurrentPlayer = It->Get();
		if (CurrentPlayer != nullptr && CurrentPlayer->GetPawn()) {
			APawn * Pawn = CurrentPlayer->GetPawn();
			USHealthComponent * HealthComp = Cast<USHealthComponent>(Pawn->GetComponentByClass(USHealthComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f) {
				return true;
			}
		}
	}
	return false;
}

void AHMGameMode::GameOver()
{
	isGameOver = true;
	SetWaveState(EWaveState::GameOver);
	EndWave();
}

