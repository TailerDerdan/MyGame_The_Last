#pragma once
#include "includes.h"

class StartLobby
{
public:
	StartLobby();
	void DrawDialouge(sf::RenderWindow& window);
	bool GetStateDialogue();

private:
	void DrawOneLine(std::string message, sf::RenderWindow& window);
private:
	std::map<int, std::string> dialogueLines;
	sf::Font font;
	sf::Clock timer;

	sf::Texture angryMinerTexture;
	sf::Sprite angryMiner;

	sf::Music soundFirstReplica;
	sf::Music soundSecondReplica;
	sf::Music soundAngryLaugh;

	int countOfReplica = 0;

	int index = 0;
	bool isDialogueEnd = false;
	bool isFirstMessage = true;
};