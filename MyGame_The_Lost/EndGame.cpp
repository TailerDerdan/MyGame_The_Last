#include "EndGame.h"

EndGame::EndGame()
{
	int iter = 0;
	dialogueLinesHappyEnd.insert({ iter++, "Hey, hey buddy!" });
	dialogueLinesHappyEnd.insert({ iter++, "You managed to save us, \nthank you very much" });
	dialogueLinesHappyEnd.insert({ iter++, "If it wasn't for you, \nwe would probably \nhave died." });

	countOfReplicasHappyEnd = iter;
	iter = 0;

	dialogueLinesBadEnd.insert({ iter++, "Where are you friends?" });
	dialogueLinesBadEnd.insert({ iter++, "I didn't have time to save you..." });

	countOfReplicasBadEnd = iter;
	iter = 0;

	dialogueLinesPlayerDied.insert({ iter++, "You died" });

	countOfReplicasPlayerDied = iter;

	font.loadFromFile("../assets/Chava-Regular.otf");
	teamTexture.loadFromFile("../assets/team.png");

	sf::IntRect rectForSprite = { 0, 0, 1280, 720 };
	team.setTexture(teamTexture);
	team.setTextureRect(rectForSprite);
	team.setPosition({ 300, 0 });

	noTeamTexture.loadFromFile("../assets/noTeam.png");

	sf::IntRect rectForSpriteNoTeam = { 0, 0, 1280, 720 };
	noTeam.setTexture(noTeamTexture);
	noTeam.setTextureRect(rectForSpriteNoTeam);
	noTeam.setPosition({ 300, 0 });

	replicaTeam.openFromFile("../assets/teamReplicas/team_voice.wav");

	m_timeInLevel.push_back(0);
	m_timeInLevel.push_back(0);
	m_timeInLevel.push_back(0);

	timer.restart();
}

void EndGame::DrawOneLine(std::string message, sf::RenderWindow& window, TypeOfEnd stateGame)
{
	if (indexOfBadEnd > countOfReplicasBadEnd - 1) return;
	if (indexOfHappyEnd > countOfReplicasHappyEnd - 1) return;
	if (indexOfPlayerDied > countOfReplicasPlayerDied - 1) return;

	if (stateGame == TypeOfEnd::HappyEnd)
	{
		window.draw(team);
	}
	else if (stateGame == TypeOfEnd::BadEnd)
	{
		window.draw(noTeam);
	}

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

void EndGame::DrawDialouge(sf::RenderWindow& window, TypeOfEnd stateGame)
{
	switch (stateGame)
	{
	case HappyEnd:
		timeForOneReplica = TIME_FOR_ONE_REPLICA_FIND_TEAM;
		break;
	case BadEnd:
		timeForOneReplica = TIME_FOR_ONE_REPLICA_FIND_TEAM;
		break;
	case PlayerDied:
		if (indexOfPlayerDied == 0)
		{
			timeForOneReplica = TIME_FOR_ONE_REPLICA_FIND_TEAM;
		}
		else
		{
			timeForOneReplica = TIME_FOR_ONE_REPLICA_PLAYER_DIED;
		}
		break;
	default:
		break;
	}

	window.clear();
	if (timer.getElapsedTime().asSeconds() < timeForOneReplica && !isFirstMessage) return;

	switch (stateGame)
	{
	case HappyEnd:
		DrawOneLine(dialogueLinesHappyEnd[indexOfHappyEnd], window, stateGame);
		indexOfHappyEnd++;
		break;
	case BadEnd:
		DrawOneLine(dialogueLinesBadEnd[indexOfBadEnd], window, stateGame);
		indexOfBadEnd++;
		break;
	case PlayerDied:
		DrawOneLine(dialogueLinesPlayerDied[indexOfPlayerDied], window, stateGame);
		indexOfPlayerDied++;
		break;
	default:
		break;
	}
	
	isFirstMessage = false;
	isDialogueStart = true;

	if (indexOfHappyEnd == countOfReplicasHappyEnd + 1 || 
		indexOfBadEnd == countOfReplicasBadEnd + 1 ||
		indexOfPlayerDied == countOfReplicasPlayerDied + 1)
	{
		window.close();
	}
	replicaTeam.play();

	timer.restart();
}

void EndGame::ChangeStaticOfPlayer(int excavatedCoal, std::vector<float> timeInLevel, int countOfFoundFlowers)
{
	m_excavatedCoal = excavatedCoal;
	m_countOfFoundFlowers = countOfFoundFlowers;
	for (int iter = 0; iter < COUNT_OF_DATA_OF_PLAYER; iter++)
	{
		m_timeInLevel[iter] = timeInLevel[iter];
	}

	std::string timeForLevel1 = std::to_string(int(std::floor(m_timeInLevel[0] / 60))) + ":" +
		std::to_string(int(m_timeInLevel[0]) % 60);
	std::string timeForLevel2 = std::to_string(int(std::floor(m_timeInLevel[1] / 60))) + ":" +
		std::to_string(int(m_timeInLevel[1]) % 60);
	std::string timeForLevel3 = std::to_string(int(std::floor(m_timeInLevel[2] / 60))) + ":" +
		std::to_string(int(m_timeInLevel[2]) % 60);

	std::string staticPlayer = "Here are your stats: \nCoal: " + std::to_string(m_excavatedCoal) + "\nCount found flower: " +
		std::to_string(m_countOfFoundFlowers) + "\nTime to complete the levels: 1: " + timeForLevel1 +
		" 2: " + timeForLevel2 + " 3: " + timeForLevel3;

	int iter = countOfReplicasHappyEnd;
	dialogueLinesHappyEnd.insert({ iter++, staticPlayer });
	countOfReplicasHappyEnd = iter + 1;

	iter = countOfReplicasBadEnd;
	dialogueLinesBadEnd.insert({ iter++, staticPlayer });
	countOfReplicasBadEnd = iter + 1;

	iter = countOfReplicasPlayerDied;
	dialogueLinesPlayerDied.insert({ iter++, staticPlayer });
	countOfReplicasPlayerDied = iter;
}