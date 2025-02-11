
#include "BioEnemyShip.h"
#include "Level.h"


BioEnemyShip::BioEnemyShip()
{
	SetSpeed(50);
	SetMaxHitPoints(1);
	SetCollisionRadius(20);
}

void BioEnemyShip::Update(const GameTime& gameTime)
{
	if (IsActive())
	{
		float x = sin(gameTime.GetTotalTime() * Math::PI + GetIndex());
		float y = cos(gameTime.GetTotalTime() * Math::PI + GetIndex());
		x *= GetSpeed() * gameTime.GetElapsedTime() * 2.4f;
		y *= GetSpeed() * gameTime.GetElapsedTime() * 2.4f;
		//nasty homing behavior
		TranslatePosition(x - (GetPosition().X - GetCurrentLevel()->GetPlayerShip()->GetPosition().X) / 100.0f * GetSpeed() * gameTime.GetElapsedTime(), y);
		TranslatePosition(y,y - (GetPosition().Y - GetCurrentLevel()->GetPlayerShip()->GetPosition().Y) / 100.0f * GetSpeed() * gameTime.GetElapsedTime());
		if (!IsOnScreen()) Deactivate();
	}

	EnemyShip::Update(gameTime);
}


void BioEnemyShip::Draw(SpriteBatch& spriteBatch)
{
	if (IsActive())
	{
		const float alpha = GetCurrentLevel()->GetAlpha();
		spriteBatch.Draw(m_pTexture, GetPosition(), Color::WHITE * alpha, m_pTexture->GetCenter(), Vector2::ONE, Math::PI, 1);
	}
}
