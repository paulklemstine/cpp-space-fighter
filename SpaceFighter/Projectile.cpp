
#include "Projectile.h"
#include "Level.h"
#include "EnemyShip.h"
#include "SpaceFighter.h"

Texture* Projectile::s_pTexture = nullptr;

Projectile::Projectile()
{
	SetDirection(-Vector2::UNIT_Y);
	SetCollisionRadius(9);

}

void Projectile::Update(const GameTime& gameTime)
{
	if (IsActive())
	{
		Vector2 position = GetPosition();

		EnemyShip* NearestEnemy = SpaceFighter::level->GetClosestObject<EnemyShip>(position, 20000);

		if (NearestEnemy != nullptr) {
			m_target = NearestEnemy->GetPosition();
		}
		else {
			m_target = position;
		}

		Vector2 translation = m_direction * m_speed * gameTime.GetElapsedTime();
		Vector2 newPositionCenter = position + translation;

		float m_rad_direction_left = std::atan2(m_direction.Y, m_direction.X) - m_rotationSpeed;
		Vector2 m_directionLeft = Vector2(std::cos(m_rad_direction_left), std::sin(m_rad_direction_left));
		Vector2 translationLeft = m_directionLeft * m_speed * gameTime.GetElapsedTime();
		Vector2 newPositionLeft = position + translationLeft;

		float m_rad_direction_right = std::atan2(m_direction.Y, m_direction.X) + m_rotationSpeed;
		Vector2 m_directionRight = Vector2(std::cos(m_rad_direction_right), std::sin(m_rad_direction_right));
		Vector2 translationRight = m_directionRight * m_speed * gameTime.GetElapsedTime();
		Vector2 newPositionRight = position + translationRight;

		float distCenter = m_target.Distance(newPositionCenter, m_target);
		float distLeft = m_target.Distance(newPositionLeft, m_target);
		float distRight = m_target.Distance(newPositionRight, m_target);

		Vector2 closestPosition = newPositionCenter;
		Vector2 newDirection = m_direction;

		if (distLeft < distCenter) {
			closestPosition = newPositionLeft;
			newDirection = m_directionLeft;
		}

		if (distRight < distLeft) {
			closestPosition = newPositionRight;
			newDirection = m_directionRight;
		}

		SetPosition(closestPosition);
		m_direction = newDirection;
		m_speed += 10.0f;
		if (m_rotationSpeed>0)m_rotationSpeed = 30.0f/m_speed;

		Vector2 size = s_pTexture->GetSize();

		// Is the projectile off the screen?
		if (position.Y < -size.Y) Deactivate();
		else if (position.X < -size.X) Deactivate();
		else if (position.Y > Game::GetScreenHeight() + size.Y) Deactivate();
		else if (position.X > Game::GetScreenWidth() + size.X) Deactivate();
	}

	GameObject::Update(gameTime);
}

void Projectile::Draw(SpriteBatch& spriteBatch)
{
	if (IsActive())
	{
		const float alpha = GetCurrentLevel()->GetAlpha();
		spriteBatch.Draw(s_pTexture, GetPosition(), Color::WHITE * alpha, s_pTexture->GetCenter(), Vector2::ONE, atan2(m_direction.X, -m_direction.Y));
		//spriteBatch.Draw(s_pTexture, m_target, Color::RED * alpha, s_pTexture->GetCenter());
	}
}

void Projectile::Activate(const Vector2& position, bool wasShotByPlayer)
{
	m_wasShotByPlayer = wasShotByPlayer;
	m_speed = Math::GetRandomFloat()*200.0f;
	m_rotationSpeed = Math::GetRandomFloat()/9.0f;
	SetPosition(position);

	GameObject::Activate();
}

std::string Projectile::ToString() const
{
	return std::string((WasShotByPlayer()) ? "Player" : "Enemy").append(" Projectile");
}

CollisionType Projectile::GetCollisionType() const
{
	CollisionType shipType = WasShotByPlayer() ? CollisionType::Player : CollisionType::Enemy;
	return (shipType | GetProjectileType());
}