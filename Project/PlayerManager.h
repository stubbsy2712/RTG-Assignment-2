#pragma once
class PlayerManager
{
public:
	void checkObjectiveCollision();
	void checkEnemyCollision();
	int getHealth();
private:
	int m_health;
	int m_score;
};

