#pragma once
#include "includes.h"

const int COUNT_OF_DATA_OF_PLAYER = 3;
const float TIME_FOR_ONE_REPLICA_FIND_TEAM = 5.0f;
const float TIME_FOR_ONE_REPLICA_PLAYER_DIED = 10.0f;

class EndGame
{
public:
	EndGame();
	void DrawDialouge(sf::RenderWindow& window, TypeOfEnd stateGame);
	bool GetStateDialogue();
	void ChangeStateDialogue();
	void ChangeStaticOfPlayer(int excavatedCoal, std::vector<float> timeInLevel, int countOfFoundFlowers);

private:
	void DrawOneLine(std::string message, sf::RenderWindow& window, TypeOfEnd stateGame);

private:
	std::map<int, std::string> dialogueLinesHappyEnd;
	std::map<int, std::string> dialogueLinesBadEnd;
	std::map<int, std::string> dialogueLinesPlayerDied;

	sf::Font font;
	sf::Clock timer;

	sf::Texture teamTexture;
	sf::Sprite team;

	sf::Texture noTeamTexture;
	sf::Sprite noTeam;

	sf::Music replicaTeam;

	int countOfReplicasHappyEnd = 0;
	int countOfReplicasBadEnd = 0;
	int countOfReplicasPlayerDied = 0;

	int indexOfHappyEnd = 0;
	int indexOfBadEnd = 0;
	int indexOfPlayerDied = 0;

	bool isDialogueStart = false;
	bool isFirstMessage = true;

	int m_excavatedCoal = 0;
	std::vector<float> m_timeInLevel;
	int m_countOfFoundFlowers = 0;

	float timeForOneReplica = 0;
};