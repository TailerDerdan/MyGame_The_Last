#pragma once
#include "includes.h"

class EndGame
{
public:
	EndGame();
	void DrawDialouge(sf::RenderWindow& window);
	bool GetStateDialogue();
	void ChangeStateDialogue();

private:
	void DrawOneLine(std::string message, sf::RenderWindow& window);

private:
	std::map<int, std::string> dialogueLines;
	sf::Font font;
	sf::Clock timer;

	sf::Texture teamTexture;
	sf::Sprite team;

	sf::Music replicaTeam;

	int countOfReplica = 0;

	int index = 0;
	bool isDialogueStart = false;
	bool isFirstMessage = true;
};