
#include "MissileLauncher.h"

void MissileLauncher::Update(const GameTime& gameTime)
{
	if (m_cooldown > 0) m_cooldown -= gameTime.GetElapsedTime();
}
void MissileLauncher::Fire(TriggerType triggerType)
{
	if (!IsActive()) return;
	if (!CanFire()) return;
	if (!triggerType.Contains(GetTriggerType())) return;

	Projectile* pProjectile = GetProjectile();
	pProjectile->SetSpeed(100);
	pProjectile->SetAcceleration(50);
	pProjectile->SetMaxRotationSpeed(0.05f);
	pProjectile->SetDirection(Vector2(0, -1));


	if (!pProjectile) return;

	AudioSample* pAudio = GetFireSound();
	if (pAudio) pAudio->Play();

	pProjectile->Activate(GetPosition(), IsAttachedToPlayer());
	m_cooldown = m_cooldownSeconds;
}