
#include "Blaster.h"

void Blaster::Update(const GameTime & gameTime)
{
	if (m_cooldown > 0) m_cooldown -= gameTime.GetElapsedTime();
}

void Blaster::Fire(TriggerType triggerType)
{
	if (!IsActive()) return;
	if (!CanFire()) return; 
	if (!triggerType.Contains(GetTriggerType())) return;
	FireAmmo();

	Projectile* pProjectile = GetProjectile();
	pProjectile->SetSpeed(1000);
	pProjectile->SetAcceleration(0);
	pProjectile->SetMaxRotationSpeed(0.0f);
	pProjectile->SetDirection(Vector2(0,-1));

	if (!pProjectile) return;

	AudioSample* pAudio = GetFireSound();
	if (pAudio) pAudio->Play();

	pProjectile->Activate(GetPosition(), IsAttachedToPlayer());
	m_cooldown = m_cooldownSeconds;
}