
#include "FragCannon.h"
#include "Level.h"

void FragCannon::Update(const GameTime& gameTime)
{
	if (m_cooldown > 0) m_cooldown -= gameTime.GetElapsedTime();
}

void FragCannon::Fire(TriggerType triggerType)
{
	if (!IsActive()) return;
	if (!CanFire()) return;
	if (!triggerType.Contains(GetTriggerType())) return;
	FireAmmo();
	
	//make an explosion centered around the ship
	GetProjectile()->GetCurrentLevel()->SpawnExplosion(GetProjectile()->GetCurrentLevel()->GetPlayerShip());
	
	//fire 16 shots away from the player ship in a starburst pattern 
	float shots = 16;
	for (float i = 0;i < shots;i++) {
		Projectile* pProjectile = GetProjectile();
		if (!pProjectile) return;
		pProjectile->SetSpeed(1000.0f+ Math::GetRandomFloat() * 200.0f - 100.0f);
		pProjectile->SetAcceleration(-10.0f -Math::GetRandomFloat() * 8.0f - 4.0f);
		pProjectile->SetMaxRotationSpeed(0.1f);
		float x = std::cos(2.0f * Math::PI * (i/ (shots-1)));
		float y = std::sin(2.0f * Math::PI * (i/ (shots-1)));
		pProjectile->SetDirection(Vector2(x,y));
		pProjectile->Activate(GetPosition(), IsAttachedToPlayer());
	}

	AudioSample* pAudio = GetFireSound();
	if (pAudio) pAudio->Play();


	m_cooldown = m_cooldownSeconds;
}