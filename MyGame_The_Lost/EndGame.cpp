#include "EndGame.h"

EndGame::EndGame()
{
	int iter = 0;
	dialogueLines.insert({ iter++, "Hey, hey buddy!" });
	dialogueLines.insert({ iter++, "You managed to save us, \nthank you very much" });
	dialogueLines.insert({ iter++, "If it wasn't for you, \nwe would probably \nhave died." });

	countOfReplica = iter + 1;

	font.loadFromFile("../assets/Chava-Regular.otf");
	teamTexture.loadFromFile("../assets/team.png");

	sf::IntRect rectForSprite = { 0, 0, 1280, 720 };
	team.setTexture(teamTexture);
	team.setTextureRect(rectForSprite);
	team.setPosition({ 300, 0 });

	replicaTeam.openFromFile("../assets/teamReplicas/team_voice.wav");

	timer.restart();
}

void EndGame::DrawOneLine(std::string message, sf::RenderWindow& window)
{
	if (index > countOfReplica - 1) return;

	window.draw(team);

	sf::Text dialogueLine;
	dialogueLine.setFont(font);
	dialogueLine.setString(message);
	dialogueLine.setCharacterSize(50);
	dialogueLine.setFillColor(sf::Color(255, 255, 255));
	dialogueLine.setStyle(sf::Text::Style::Bold);
	dialogueLine.setPosition({ 250, 150 });

	window.draw(dialogueLine);
	window.display();
}

bool EndGame::GetStateDialogue()
{
	return isDialogueStart;
}

void EndGame::ChangeStateDialogue()
{
	isDialogueStart = !isDialogueStart;
}

void EndGame::DrawDialouge(sf::RenderWindow& window)
{
	window.clear();
	if (timer.getElapsedTime().asSeconds() < 5 && !isFirstMessage) return;

	DrawOneLine(dialogueLines[index], window);
	index++;
	isFirstMessage = false;
	isDialogueStart = true;

	if (index == 4)
	{
		window.close();
	}
	replicaTeam.play();

	timer.restart();
}