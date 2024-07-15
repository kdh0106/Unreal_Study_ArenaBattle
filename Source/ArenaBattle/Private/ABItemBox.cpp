// Fill out your copyright notice in the Description page of Project Settings.


#include "ABItemBox.h"
#include "ABWeapon.h"
#include "ABCharacter.h"

// Sets default values
AABItemBox::AABItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_BOX(TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>P_CHESTOPEN(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (P_CHESTOPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHESTOPEN.Object); //Effect�� ����� ��ƼŬ �ý��� ���ø� ����
		Effect->bAutoActivate = false;  //�� ������Ʈ�� �����Ǿ��� ��, �ڵ����� Ȱ��ȭ���� ���� -> ���������� Ȱ��ȭ�� ������ ��Ȱ�� ���·� ����
	}
	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	WeaponItemClass = AABWeapon::StaticClass();
}

// Called when the game starts or when spawned
void AABItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AABItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents(); //�ڽ� Ŭ�������� �߰��� �۾� ���� �θ� Ŭ�������� ���ǵ� �ʱ�ȭ �۾�.
	//override �߱� ������ ���ִ� �ʱ�ȭ
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnCharacterOverlap);
	//�ڽ� Ŭ������ �߰� �ʱ�ȭ �۾�
}

void AABItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABLOG_S(Warning);

	auto ABCharacter = Cast<AABCharacter>(OtherActor); //OtherActor�� AABCharacterŸ������ ��ȯ
	if (nullptr != ABCharacter && nullptr != WeaponItemClass)
	{
		if (ABCharacter->CanSetWeapon())
		{
			auto NewWeapon = GetWorld()->SpawnActor<AABWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			ABCharacter->SetWeapon(NewWeapon);
			Effect->Activate(true); //��ƼŬ ȿ�� �߻���Ű��
			Box->SetHiddenInGame(true, true);  //Box ������Ʈ�� ���ӿ��� ������, �ι�° �μ��� ���� ������Ʈ�� �Բ� ������
			SetActorEnableCollision(false);
			Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
		}
		else
		{
			ABLOG(Warning, TEXT("%s can't equip weapon currently."), *ABCharacter->GetName());
		}
	}
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}

// Called every frame
void AABItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
