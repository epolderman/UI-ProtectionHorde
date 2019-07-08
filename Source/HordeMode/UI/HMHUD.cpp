
#include "HMHUD.h"
#include "HMGameMode.h"
#include <Engine/Engine.h>
#include <DeclarativeSyntaxSupport.h>
#include "Components/SSTitleWidget.h"

/*
	A Shared Reference acts like a Shared Pointer, in the sense that it owns the
	object it references. They differ with regard to null objects; Shared References must
	always reference a non-null object. Because Shared Pointers don't have that restriction,
	a Shared Reference can always be converted to a Shared Pointer, and that Shared Pointer is
	guaranteed to reference a valid object. Use Shared References when you want a guarantee that the
	referenced object is non-null, or if you want to indicate shared object ownership.
*/


AHMHUD::AHMHUD()
{
	UE_LOG(LogTemp, Warning, TEXT("AHMHUD()"));
}

void AHMHUD::PostInitializeComponents() {

	UE_LOG(LogTemp, Warning, TEXT("PostInitComponents"));
	Super::PostInitializeComponents();

	/*
	https://forums.unrealengine.com/development-discussion/c-gameplay-programming/10029-what-exactly-is-sassignnew-doing
	*/
}

void AHMHUD::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("HUD Begin Play()"));

	UWorld* const MyWorld = GetWorld();
	if (MyWorld == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("HUD: World is null()"));
	}
	SAssignNew(this->TitleWaveWidget, SSTitleWidget).OwnerWorld(MyWorld);
	this->TitleWaveWidget->ShowTitle("A HUD");
}
