
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

	for (int i = 0;i < 10;i++) {
		Projectile* pProjectile = GetProjectile();
		if (!pProjectile) return;
		pProjectile->SetSpeed(10.0f+ Math::GetRandomFloat() * 60.0f - 30.0f);
		pProjectile->SetAcceleration(30.0f+ Math::GetRandomFloat() * 20.0f - 10.0f);
		pProjectile->SetMaxRotationSpeed(0.05f);
		pProjectile->SetDirection(Vector2(0, -1));
		pProjectile->Activate(GetPosition()+Vector2(Math::GetRandomFloat()*60.0f -30.0f, Math::GetRandomFloat() * 20.0f - 10.0f), IsAttachedToPlayer());
	}

	

	AudioSample* pAudio = GetFireSound();
	if (pAudio) pAudio->Play();


	m_cooldown = m_cooldownSeconds;
}