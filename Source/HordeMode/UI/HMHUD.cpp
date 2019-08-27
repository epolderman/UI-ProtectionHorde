
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

	Each client (including server) gets a unique Player Controller, which is specified in the game mode,
	so creating local gui elements and other objects are probably best created in the player controller as
	there is no way to distinct if a character is controlled by one client or another. My understanding is that
	each instance of the game has one player controller but infinite possible characters so you can be assured
	when you are creating objects in the player controller that they will be local to that instance of the game.

	issue is wwhat owns this? server? clients? how to display only on the client?
*/


AHMHUD::AHMHUD()
{
	UE_LOG(LogTemp, Warning, TEXT("AHMHUD()"));
}
/*
	https://forums.unrealengine.com/development-discussion/c-gameplay-programming/10029-what-exactly-is-sassignnew-doing
*/
void AHMHUD::PostInitializeComponents() {
	Super::PostInitializeComponents();
}

void AHMHUD::BeginPlay()
{
	Super::BeginPlay();
}

/*
	So SAssignNew is just more convenient way of assigning widgets to variables and it makes sure
	that you don't disturb you declarative syntax code and split it into multiple blocks.
	Of course there are scenarios when you would prefer using SNew instead SAssignNew
	but thats totally up to you.
*/
void AHMHUD::ShowWaveTitle() {
	UWorld* const MyWorld = GetWorld();
	if (MyWorld == nullptr || bIsTitleVisible) {
		return;
	}
	SAssignNew(this->TitleWaveWidget, SSTitleWidget).OwnerWorld(MyWorld).OwnerHud(this);
	this->TitleWaveWidget->ShowTitle("Wave Start");
	bIsTitleVisible = true;
}
void AHMHUD::HideWaveTitle() {
	TitleWaveWidget = nullptr;
	bIsTitleVisible = false;
}
