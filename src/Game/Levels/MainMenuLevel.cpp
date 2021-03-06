#include "Game/Levels/MainMenuLevel.h"
#include <iostream>

using namespace Game;

void MainMenuLevel::update(float dt, const KeyStates &keyStates) {

	//	Reset Labels
	startLabel.setColor({255,255,255});
	startLabel.setAlpha(255);
	instructionLabel.setColor({255,255,255});
	instructionLabel.setAlpha(255);

	if(keyStates.Z_PRESSED) {

		if (selectedOption == Options::BEGIN_GAME) {
			nextLevelCode = Levels::LOADING_BATTLE;
		}
		else if (selectedOption == Options::HOW_TO_PLAY) {
			nextLevelCode = Levels::INSTRUCTION_MENU;
		}

		menuMusic->stop();
		done = true;

	}

	if (keyStates.DOWN_PRESSED) {

		if (selectedOption == Options::BEGIN_GAME) 
			selectedOption = Options::HOW_TO_PLAY;

	}

	if (keyStates.UP_PRESSED) {

		if (selectedOption == Options::HOW_TO_PLAY) 
			selectedOption = Options::BEGIN_GAME;

	}

	for (MobileEntity * particle : particles) {

		if (particle->getY() >= screenHeight + screenHeight/10) {

			float newX, newY;

			newX = rand() % (int)screenWidth/4;
			newY = -1 * (rand() % (int)screenHeight*2);
			if (rand() % 2 == 0) {
				newX += (.75f*screenWidth);
			} 

			particle->setPosition(newX, newY);
		}

		particle->setVelocityY(100.f);

	}

	//	Call Base
	Level::update(dt, keyStates);

}

void MainMenuLevel::updateEvents(Uint32 time, float dt) {

	Level::updateEvents(time,dt);

	if (!menuMusic->isPlayed() && time > 3000) {
		menuMusic->play(20);
	}
	
	//	update alpha
	if (time % 5*dt == 0) {
		if (fadeIn) {
			++alpha;
			if (alpha >= 255) fadeIn = false;
		}
		else {
			--alpha;
			if(alpha <= 0) fadeIn = true;
		}
	}

	if (selectedOption == Options::BEGIN_GAME) {
		startLabel.setAlpha(alpha);
		startLabel.setColor({255,255,0});
	}
	else if (selectedOption == Options::HOW_TO_PLAY) {
		instructionLabel.setAlpha(alpha);
		instructionLabel.setColor({255,255,0});
	}

}

void MainMenuLevel::setup() {

	Level::setup();

	//	Reset Music
	menuMusic->setPlayed(false);

	//	Reset selectedOption
	selectedOption = Options::BEGIN_GAME;

	//	Reset Particles
	for (Entity *particle : particles) {
		particle->setY(2*screenHeight);
	}

	//	Reset Alpha
	alpha = 0;

}

MainMenuLevel::MainMenuLevel() : menuMusic(nullptr), gameTitleLabel("res/fnt/MonsterFriendFore.ttf", "ShooterGame", 30), startLabel("res/fnt/DTM-Mono.ttf", "Begin Game", 20), instructionLabel("res/fnt/DTM-Mono.ttf", "How to Play", 20),selectedOption(Options::BEGIN_GAME), asgoreEntity("res/img/asgoreLookDown.png"), alpha(0), fadeIn(true) {

	std::vector<Entity *> screenObjects;

	//	menuMusic
	menuMusic = loadMusic("res/mus/menumusic.wav");

	//	gameTitleLabel
	gameTitleLabel.setCentered(true);
	gameTitleLabel.setActive(true);
	screenObjects.push_back(&gameTitleLabel);

	//	startLabel
	int sW, sH;
	TTF_SizeText(startLabel.getFont(), startLabel.getText().c_str(), &sW, &sH);
	startLabel.setPosition(screenWidth/2-sW/2, screenHeight*(5.f/8.f)-sH/2);
	startLabel.setCentered(false);
	startLabel.setActive(true);
	screenObjects.push_back(&startLabel);

	//	instructionLabel
	int iW, iH;
	TTF_SizeText(instructionLabel.getFont(), instructionLabel.getText().c_str(), &iW, &iH);
	instructionLabel.setPosition(screenWidth/2-iW/2, screenHeight*(5.f/8.f)-iH/2+2*sH);
	instructionLabel.setCentered(false);
	instructionLabel.setActive(true);
	screenObjects.push_back(&instructionLabel);

	//	asgore
	asgoreEntity.setActive(true);
	asgoreEntity.setCentered(true);
	asgoreEntity.setScale(1.8f);
	asgoreEntity.setPosition(screenWidth/2, screenHeight/4);
	screenObjects.push_back(&asgoreEntity);	

	//	particles

	int particleCount = 50;
	for (int i = 0; i<particleCount; ++i) {
		MobileEntity *particle = new MobileEntity("res/img/asgoreParticle2.png");
		particle->setY(2*screenHeight);
		particles.push_back(particle);
		screenObjects.push_back((Entity *)particle);
	}

	//	Positioning
	gameTitleLabel.setPosition(screenWidth/2, screenHeight/2);
	//instructionLabel->setPosition(screenWidth*(3.f/4.f), screenHeight*(3.f/4.f));
	
	Game::Stage *mainStage = loadStage(screenObjects);
	addStage(mainStage);

}

MainMenuLevel::~MainMenuLevel() {

	SDL_Log("MainMenuLevel Destroyed ...");

	delete menuMusic;
	for (MobileEntity *particle : particles) {
		if (particle) {
			delete particle;
			particle = nullptr;
		}
	}
	particles.clear();

	menuMusic = nullptr;


}