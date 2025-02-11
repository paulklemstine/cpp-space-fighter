
#include "Projectile.h"
#include "Level.h"
#include "EnemyShip.h"
#include "SpaceFighter.h"
#include "MathUtil.h"

Texture* Projectile::s_pTexture = nullptr;

Projectile::Projectile()
{
	SetDirection(-Vector2::UNIT_Y);
	SetCollisionRadius(9);

}


//variables to hold targeting calculations in the update loop
Vector2 position;
EnemyShip* NearestEnemy;
Vector2 translation;
Vector2 newPositionCenter;
float m_rad_direction_left;
Vector2 m_directionLeft;
Vector2 translationLeft;
Vector2 newPositionLeft;
float m_rad_direction_right;
Vector2 m_directionRight;
Vector2 translationRight;
Vector2 newPositionRight;
float distCenter;
float distLeft;
float distRight;
Vector2 closestPosition;
Vector2 newDirection;
Vector2 size;

//color cycle through rainbow colors
std::vector<Color> rainbow = {
	Color(1.0f, 0.0f, 0.0f), 
	Color(1.0f, 0.5f, 0.0f),
	Color(1.0f, 1.0f, 0.0f), 
	Color(0.0f, 1.0f, 0.0f),
	Color(0.0f, 0.0f, 1.0f),
	Color(0.3f, 0.0f, 0.5f), 
	Color(0.5f, 0.0f, 0.5f),
	Color(0.8f, 0.0f, 0.2f),
	Color(1.0f, 0.8f, 0.0f),
	Color(0.0f, 0.5f, 1.0f),

};
Color color;
void Projectile::Update(const GameTime& gameTime)
{
	if (IsActive())
	{
		 position = GetPosition();

		//find the nearest active enemy
		NearestEnemy = GetCurrentLevel()->GetClosestObject<EnemyShip>(position, 20000);

		//I'm not sure nullptr is ever returned here, ememies are always targeted
		if (NearestEnemy != nullptr) {
			//target nearest enemy
			m_target = NearestEnemy->GetPosition();
		}
		else {
			//keep missile circliung around player ship
			m_target = position;
		}

		//predict projectile's next position
		translation = m_direction * m_speed * gameTime.GetElapsedTime();
		newPositionCenter = position + translation;

		//predict projectiles next position if it veers to the left a little

		m_rad_direction_left = std::atan2(m_direction.Y, m_direction.X) - m_rotationSpeed;
		m_directionLeft = Vector2(std::cos(m_rad_direction_left), std::sin(m_rad_direction_left));
		translationLeft = m_directionLeft * m_speed * gameTime.GetElapsedTime();
		newPositionLeft = position + translationLeft;

		//predict projectile location if it veers to the right a little
		m_rad_direction_right = std::atan2(m_direction.Y, m_direction.X) + m_rotationSpeed;
		m_directionRight = Vector2(std::cos(m_rad_direction_right), std::sin(m_rad_direction_right));
		translationRight = m_directionRight * m_speed * gameTime.GetElapsedTime();
		newPositionRight = position + translationRight;

		//calculate the distance from the predicted projectile paths to the target
		distCenter = m_target.Distance(newPositionCenter, m_target);
		distLeft = m_target.Distance(newPositionLeft, m_target);
		distRight = m_target.Distance(newPositionRight, m_target);

		//determine which projected projectile path is best, use that one.
		closestPosition = newPositionCenter;
		newDirection = m_direction;

		if (distLeft < distCenter) {
			//veer to the left
			closestPosition = newPositionLeft;
			newDirection = m_directionLeft;
		}

		if (distRight < distLeft) {
			//veer to the right
			closestPosition = newPositionRight;
			newDirection = m_directionRight;
		}

		//make the changes to position and direction
		SetPosition(closestPosition);
		m_direction = newDirection;

		//speed up the missile, always acceleration
		m_speed += m_acceleration;
		
		//fast missiles can't turn fast, the value 20/speed feels about right.
		float rotTemp = 20.0f / m_speed;
		if (m_MaxRotationSpeed > rotTemp)m_rotationSpeed = rotTemp;
		else m_rotationSpeed = m_MaxRotationSpeed;

		size = s_pTexture->GetSize();

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
		//update rainbow flash color
		color = rainbow[(int)(m_speed / 300.0f) % rainbow.size()];
		spriteBatch.Draw(s_pTexture, GetPosition(), color * alpha, s_pTexture->GetCenter(), Vector2::ONE, atan2(m_direction.X, -m_direction.Y));
		//draw targeting lock on. I need to change sprite to something better than ithe same texture eventually
		spriteBatch.Draw(s_pTexture, m_target, Color::RED * alpha, s_pTexture->GetCenter());
	}
}

void Projectile::Activate(const Vector2& position, bool wasShotByPlayer)
{
	m_wasShotByPlayer = wasShotByPlayer;
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