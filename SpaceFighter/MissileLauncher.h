
#pragma once

#include "Weapon.h"

/** @brief Represents a missilelauncher weapon that can be fired by a game object. */
class MissileLauncher : public Weapon
{

public:

	/** @brief Instantiate a missilelauncher object.
		@param isActive A flag to determine if the weapon is active. */
	MissileLauncher(const std::string& key) : Weapon(key) {}
	virtual ~MissileLauncher() {}

	/** @brief Update the missilelauncher.
		@param gameTime A reference to the game time object. */
	virtual void Update(const GameTime& gameTime);

	/** @brief Check if the missilelauncher is ready to fire.
		@return True if the missilelauncher is ready to fire. */
	virtual bool CanFire() const { return m_cooldown <= 0 && m_ammo>0; }

	/** @brief Reset the cooldown of the missilelauncher. */
	virtual void ResetCooldown() { m_cooldown = 0; }

	/** @brief Get the number of seconds the missilelauncher must wait between shots.
		@return The number of seconds the missilelauncher must wait between shots. */
	virtual float GetCooldownSeconds() { return m_cooldownSeconds; }

	/** @brief Set the number of seconds the missilelauncher must wait between shots.
		@param seconds The number of seconds the missilelauncher must wait between shots. */
	virtual void SetCooldownSeconds(const float seconds) { m_cooldownSeconds = seconds; }

	/** @brief Fire the missilelauncher.
		@param triggerType The type of trigger that fired the weapon.
		@remark If the triggerType doesn't match that of the MissileLauncher, the command will be ignored. */
	virtual void Fire(TriggerType triggerType);


private:

	float m_cooldown = 0;
	float m_cooldownSeconds = 0.50;

};