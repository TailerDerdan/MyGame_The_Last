#include "StartLobby.h"

StartLobby::StartLobby()
{
	int iter = 0;
	dialogueLines.insert({ iter++, "Hey you!" });
	dialogueLines.insert({ iter++, "Now you need to jump down \ninto the caves \nand get a lot of coal" });
	dialogueLines.insert({ iter++, "You can certainly \ndo this with your team, \nbut I don't think \nit will help you much." });

	countOfReplica = iter + 1;

	font.loadFromFile("../assets/Chava-Regular.otf");
	angryMinerTexture.loadFromFile("../assets/angryMiner.png");

	sf::IntRect rectForSprite = { 0, 0, 1024, 991 };
	angryMiner.setTexture(angryMinerTexture);
	angryMiner.setTextureRect(rectForSprite);


	soundFirstReplica.openFromFile("../assets/angryReplicas/replica_1.wav");
	soundSecondReplica.openFromFile("../assets/angryReplicas/replica_2.wav");
	soundAngryLaugh.openFromFile("../assets/angryReplicas/angryManLaugh.wav");

	timer.restart();
}

void StartLobby::DrawOneLine(std::string message, sf::RenderWindow& window)
{
	if (index > countOfReplica - 1) return;

	window.draw(angryMiner);

	sf::Text dialogueLine;
	dialogueLine.setFont(font);
	dialogueLine.setString(message);
	dialogueLine.setCharacterSize(50);
	dialogueLine.setFillColor(sf::Color(255, 255, 255));
	dialogueLine.setStyle(sf::Text::Style::Bold);
	dialogueLine.setPosition({ 900, 150 });

	window.draw(dialogueLine);
	window.display();

	if (isFirstMessage)
	{
		soundFirstReplica.play();
	}
	else
	{
		soundSecondReplica.play();
	}
}

bool StartLobby::GetStateDialogue()
{
	return isDialogueEnd;
}

void StartLobby::DrawDialouge(sf::RenderWindow& window)
{
	window.clear();
	if (timer.getElapsedTime().asSeconds() < 5 && !isFirstMessage) return;

	DrawOneLine(dialogueLines[index], window);
	index++;
	isFirstMessage = false;

	if (index == countOfReplica)
	{
		soundAngryLaugh.play();
		isDialogueEnd = true;
	}

	timer.restart();
}