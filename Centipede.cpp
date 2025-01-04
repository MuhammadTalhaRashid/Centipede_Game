#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gamegrid[gameRows][gameColumns] = {0,0};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Write your functions declarations here. Some have been written for you. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


void input (float player[],sf::RenderWindow& window,float bullet[],int &py,int &px,int &by,int &bx,short &lives,int &score);
										//getting user input



void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
										//showing player sprite on screen
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite);
										//showing bullet sprite on screen
void drawlives (sf::RenderWindow& window,int lives, sf::Clock& textClock, int score);
										//showing lives sprite on screen
void bottomline (sf::RenderWindow& window);
										//showing bottom line on screen
void drawmushrooms(sf::RenderWindow& window);
										//showing mushroom sprites on screen
void draw_centepede(sf::RenderWindow& window);
										//showing centepede sprites on screen



void updatelevel(sf::RenderWindow& window, int level);
										//updating level and showing level no on screen
void updatescore(sf::RenderWindow& window, int score, short &lives);
										//updating score and showing score on screen



void movespider(sf::Clock& spiderfClock,int spider[],short &lives);		//moving spider
void moveflea(sf::Clock& fleafClock,int flea[]);				//moving flea
void moveBullet(float bullet[], sf::Clock& bulletClock,int &score,int &by,int &bx,int centepedex[][26],int centepedey[][24],short level,int spider[]);
										//moving bullet and checking its collision

void move_centepede(sf::RenderWindow& window,sf::Clock& centepedeClock,short &level,int centepedex[][26],int centepedey[][24],short &lives,short &speed);
										//moving normal centepede

void move_fast_centepede(sf::RenderWindow& window,sf::Clock& centepedefClock,short &level,int centepedex[][26],int centepedey[][24],short &lives,short &spee);
										//moving fast centepede


void initialize_centepede(sf::RenderWindow& window,int centepedex[][26],int centepedey[][24],short &level);
										//initializing centepede at the start of each level
void generateheads(sf::Clock& centepedeheadClock,int centepedex[][26],int centepedey[][24]);
										//initializing centepede at the start of each level
void assignmushrooms();
										//initializing mushrooms at the start of each level
void intitializeflea(sf::Clock& fleaClock,int flea[]);				//initializing flea
void initializespider(sf::Clock& spiderClock,int spider[]);			//initializing spider

int main()
{
	srand(time(0));
	
	int score = 0;
	short lives = 3;
	//short checklives = 4;	
	short checklevel=0;
	short level = 1;
	int px=14,py=27;
	int bx=0,by=0;
	bool pause = 1;
	short spee = 300;
	short speed = 500;
	bool end = 0;
										// Declaring RenderWindow.
	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	//window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(100, 0));

									// Initializing Background Music.
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/field_of_hopes.ogg");
	bgMusic.play();
	bgMusic.setVolume(50);

									// Initializing Background.
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/download (3).png");
	backgroundSprite.setTexture(backgroundTexture);
									//backgroundSprite.setTextureRect(sf::IntRect(0, 0, 960, 960));
	backgroundSprite.setScale(3.25f, 6.20f);
	backgroundSprite.setColor(sf::Color(250, 250, 250, 250 * 0.25)); // Reduces Opacity to 25%
	
	
									//initializing game start message and level change message
	sf::Font sfont,chfont;
	sf::Text stext,chtext;
	
	sfont.loadFromFile("Textures/OpenSans-Italic.ttf");
    	chfont.loadFromFile("Textures/OpenSans-Italic.ttf");
    	stext.setFont(sfont);
    	chtext.setFont(chfont);
    	stext.setString("GAME START...");
    	chtext.setString("LEVEL UP...");
    	stext.setCharacterSize(124);
    	chtext.setCharacterSize(124);
    	stext.setFillColor(sf::Color::White);
    	chtext.setFillColor(sf::Color::White);
    	stext.setPosition(120, 260);
	chtext.setPosition(180, 300);
	
	sf::SoundBuffer levelp;
	sf::Sound levelup;
	
	levelp.loadFromFile("Sound Effects/1up.wav");
	levelup.setBuffer(levelp);
	levelup.setVolume(50);
	
	sf::Clock textClock;
	
        while (textClock.getElapsedTime().asSeconds() < 2.0) {
        //window.clear();
        window.draw(backgroundSprite);
        window.draw(stext);
        window.display();}
        textClock.restart();
        
	// Initializing Player and Player Sprites.
	float player[2] = {};
	player[x] = 448;//(gameColumns / 2) * boxPixelsX;	//=480
	player[y] = 864;//(gameColumns * 3 / 4) * boxPixelsY;	//=704
	gamegrid[py][px] = 11;
	sf::Texture playerTexture;
	playerTexture.loadFromFile("Textures/download (2).jpeg");
	sf::Sprite playerSprite(playerTexture);
	playerSprite.setScale(32.0f / 150.0f, 32.0f / 150.0f);
	
									// Initializing Bullet and Bullet Sprites.
	float bullet[3] = {};
	sf::Clock bulletClock;
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	bulletSprite.setColor(sf::Color(139, 0, 0));
	
									//initializing lives, levels and score below
	sf::Font font,lfont;
	sf::Text text,ltext;
	
	font.loadFromFile("Textures/OpenSans-Italic.ttf");
    	lfont.loadFromFile("Textures/OpenSans-Italic.ttf");
    	text.setFont(font);
    	ltext.setFont(lfont);
    	text.setString("SCORE: ");
    	ltext.setString("LEVEL: ");
    	text.setCharacterSize(24);
    	ltext.setCharacterSize(24);
    	text.setFillColor(sf::Color::White);
    	ltext.setFillColor(sf::Color::White);
    	text.setPosition(672+32, 928);
	ltext.setPosition(480+32, 928);
	
									//initializing centipede and its clock
	int centepedex[25][26]={};
	int centepedey[25][24]={};
	
	sf::Clock centepedeClock;
	sf::Clock centepedefClock;					//initializing centipede head generator clock
	sf::Clock centepedeheadClock;
	
									//initializing flea clock and array
	int flea[3] = {0};
	sf::Clock fleaClock;
	sf::Clock fleafClock;
	
									//initializing spider clock and array
	int spider[5] = {0};
	sf::Clock spiderClock;
	sf::Clock spiderfClock;
	
		
	while (window.isOpen()){					//main loop
		
		///////////////////////////////////////////////////////////////
		//                                                           //
		// Call Your Functions Here. Some have been written for you. //
		// Be vary of the order you call them, SFML draws in order.  //
		//                                                           //
		///////////////////////////////////////////////////////////////
		
		if (level > checklevel){			//it will execute after each level just once
		if (level > 1){
			levelup.play();
			
			textClock.restart(); 
			while (textClock.getElapsedTime().asSeconds() < 2.0) {
			window.clear();
			window.draw(backgroundSprite);
			window.draw(chtext);
			window.display();}
		}
		textClock.restart();
		
		checklevel = level;
		initialize_centepede(window,centepedex,centepedey,level);//calling fuction to initialize centepede
		assignmushrooms();}					//calling fuction to initialize mushrooms
		
		updatescore(window, score, lives);			//calling fuction to show score on screen
		updatelevel(window,level);				//calling fuction to show level on screen
		bottomline (window);					//calling fuction to show bottom line on screen
				
		
		drawmushrooms(window);					//calling fuction to show mushrooms on screen
		drawlives (window,lives,textClock,score);		//calling fuction to show lives on screen
		
		
		
		
		window.draw(text);					
		window.draw(ltext);				
		window.draw(backgroundSprite);
		drawPlayer(window, player, playerSprite);		//calling fuction to show player sprite on screen
		
		//for (int exist=0;exist<3;exist++){
		if (bullet[exists] == true) {
			moveBullet(bullet, bulletClock,score,by,bx,centepedex,centepedey,level,spider);
									//calling fuction to move bullet on screen
			drawBullet(window, bullet, bulletSprite);}	//calling fuction to show bullet on screen
		
		
		initializespider(spiderClock,spider);			//initializing spider
		intitializeflea(fleaClock,flea);			//initializing flea
		moveflea(fleafClock,flea);				//moving flea
		movespider(spiderfClock,spider,lives);		//moving spider
		move_fast_centepede(window,centepedefClock,level,centepedex,centepedey,lives,spee);
									//calling fuction to move fast centepede
		move_centepede(window,centepedeClock,level,centepedex,centepedey,lives,speed);
									//calling fuction to move ordinary centepede
		draw_centepede(window);					//calling fuction to show centepede on screen
		generateheads(centepedeheadClock,centepedex,centepedey);//calling fuction to generate new heads

		
		input (player,window,bullet,py,px,by,bx,lives,score);	//calling function to get input
		
			end =0;
			for (int a=0;a<25;a++){
				if (!(centepedex[a][24] == -1)){
					end = 1;}}
					
			if (end == 0){
				level +=1;
				end = 0;
				speed *= 0.85;
				spee *= 0.85;}
		
		window.display();
		window.clear();
		
}}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////
void input(float player[],sf::RenderWindow& window,float bullet[],int &py,int &px,int &by,int &bx,short &lives,int &score) {

			
		sf::SoundBuffer ddeath;					
		sf::Sound death;
		
		ddeath.loadFromFile("Sound Effects/death.wav");
		death.setBuffer(ddeath);
		death.setVolume(80);
		
		sf::Clock soundDeath;
			
		sf::Event event;
		while (window.pollEvent(event)) {				//checking event means input
		if (event.type == sf::Event::KeyPressed){
			
		if (event.key.code == sf::Keyboard::Up){
			if (player[y]>736){
			py--;
			
			if (gamegrid[py][px] == 4 || gamegrid[py][px] == 144 || gamegrid[py][px] == 244 || gamegrid[py][px] == 245 || gamegrid[py][px] == 145){
				
				gamegrid[py+1][px] = 0;
				death.play();
				while (soundDeath.getElapsedTime().asMilliseconds() < 400){}
				lives -= 1;}
			
			if (gamegrid[py][px] == 3){
				score -= 1000;}
					
			gamegrid[py][px] = 11;
			gamegrid[py+1][px] = 0;
			player[y] -= 32;}
			break;}
		
		else if (event.key.code == sf::Keyboard::Down){
			if (player[y]<864){
			py++;
			
			if (gamegrid[py][px] == 4 || gamegrid[py][px] == 144 || gamegrid[py][px] == 244 || gamegrid[py][px] == 245 || gamegrid[py][px] == 145){
				
				gamegrid[py-1][px] = 0;
				death.play();
				while (soundDeath.getElapsedTime().asMilliseconds() < 200){}
				lives -= 1;}
			
			if (gamegrid[py][px] == 3){
				score -= 1000;}
					
			gamegrid[py][px] = 11;
			gamegrid[py-1][px] = 0;
			player[y] += 32;}break;}
		
		else if (event.key.code == sf::Keyboard::Right){
			if (player[x]<928){
			px++;
			
			if (gamegrid[py][px] == 4 || gamegrid[py][px] == 144 || gamegrid[py][px] == 244 || gamegrid[py][px] == 245 || gamegrid[py][px] == 145){
				gamegrid[py][px-1] = 0;
				death.play();
				while (soundDeath.getElapsedTime().asMilliseconds() < 200){}
				lives -= 1;}
			
			if (gamegrid[py][px] == 3){
				score -= 1000;}
					
			gamegrid[py][px] = 11;
			gamegrid[py][px-1] = 0;
			player[x] += 32;}break;}
		
		else if (event.key.code == sf::Keyboard::Left){
			
			if (player[x]>0){
			px--;
			
			if (gamegrid[py][px] == 4 || gamegrid[py][px] == 144 || gamegrid[py][px] == 244 || gamegrid[py][px] == 245 || gamegrid[py][px] == 145){
				
				gamegrid[py][px+1] = 0;
				death.play();
				while (soundDeath.getElapsedTime().asMilliseconds() < 200){}
				lives -= 1;}
				
			if (gamegrid[py][px] == 3){
				score -= 1000;}
				
			gamegrid[py][px] = 11;
			gamegrid[py][px+1] = 0;
			player[x] -= 32;}break;}
		
		else if (event.key.code == sf::Keyboard::Space){
			
			
			if (bullet[exists] == false){				//firing bullet
				bullet[exists] = true;
				bullet[x] = player[x];
				bullet[y] = player[y]-32;
				bx = px;
				by = py-1;
				
			sf::SoundBuffer firep;
			sf::Sound fire;
			
			firep.loadFromFile("Sound Effects/fire1.wav");
			fire.setBuffer(firep);
			fire.setVolume(80);
			
			sf::Clock soundDelay;
			fire.play();
			while (soundDelay.getElapsedTime().asMilliseconds() < 200){}
			
			}
			}}
			
		else if (event.type == sf::Event::Closed) {
				window.close();}
		break;}
}		
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) 
{									//showing player sprite on screen
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}
void moveBullet(float bullet[], sf::Clock& bulletClock,int &score,int &by,int &bx,int centepedex[][26],int centepedey[][24],short level,int spider[]) 
{									//moving bullet
	//static short bull=20;
	if (bulletClock.getElapsedTime().asMilliseconds() < 20)
		return;
	
	bulletClock.restart();
	bullet[y] -=32;
		
	switch (gamegrid[by][bx]){
			
			case 3: {} 					//to check collision with half mashrooms
			case 1: {gamegrid[by][bx] = 0;
				bullet[exists] = false;
				score +=1;
				break;}
			
			case 4: {}					//to check collision with full mashrooms
			case 2: {gamegrid[by][bx] -= 1;
				bullet[exists] = false;
				break;}
			
			case 556: {
					
				gamegrid[by][bx] = 0;
				
				if (by == 26){
					score += 900;}
				else if (by == 25){
					score += 600;}
				else {
					score += 300;}
					
				bullet[exists] = false;
				spider[2] = false;
				
				break;}
				
			case 244: {}					//to check collision with centepede body
			case 144: {
				
				if (by>22){
				gamegrid[by][bx] = 4;}
				else {
				gamegrid[by][bx] = 2;}
				
				bullet[exists] = false;
						
					for (int i=0;i<25;i++){
						if (centepedex[i][24] != -1){
						for (int j=1;j<24;j++){
							if (centepedex[i][j] == bx && centepedey[i][j] == by){
								
								score += 10;
								centepedex[i][j] = -1;
								centepedey[i][j] = -1;
								centepedex[i][25] = -1;
								
							for (int o=0;o<25;o++){
								if (centepedex[o][24] == -1){
							bool go = 0;			
							for (int k=23,l=0;k>=j;k--){
							
							if (!(centepedex[i][k] == -1)){
								
								go = 1;
								centepedex[o][l] = centepedex[i][k];
								centepedex[i][k] = -1; 
								centepedey[o][l] = centepedey[i][k];
								centepedey[i][k] = -1;
								l++;}}
							
								if (go == 1){
								if (centepedex[i][24] == 1 || centepedex[i][24] == 11){
									if (centepedey[i][0] == centepedey[o][0]){
										gamegrid[centepedey[o][0]][centepedex[o][0]] = 245;
										centepedex[o][24] = 2;}
									else {
										gamegrid[centepedey[o][0]][centepedex[o][0]] = 145;
										centepedex[o][24] = 1;}}
								
								else if (centepedex[i][24] == 21 || centepedex[i][24] == 211){

									if (centepedey[i][0] == centepedey[o][0]){
										gamegrid[centepedey[o][0]][centepedex[o][0]] = 245;
										centepedex[o][24] = 22;}
									else {
										gamegrid[centepedey[o][0]][centepedex[o][0]] = 145;
										centepedex[o][24] = 21;}}
								
								else if (centepedex[i][24] == 22 || centepedex[i][24] == 212){
									if (centepedey[i][0] == centepedey[o][0]){
										gamegrid[centepedey[o][0]][centepedex[o][0]] = 145;
										centepedex[o][24] = 21;}
									else {
										gamegrid[centepedey[o][0]][centepedex[o][0]] = 245;
										centepedex[o][24] = 22;}}
										
								else if (centepedex[i][24] == 2 || centepedex[i][24] == 12){
									if (centepedey[i][0] == centepedey[o][0]){
										gamegrid[centepedey[o][0]][centepedex[o][0]] = 145;
										centepedex[o][24] = 1;}
									else {
										gamegrid[centepedey[o][0]][centepedex[o][0]] = 245;
										centepedex[o][24] = 2;}}}
								break;}
								}
					break;}}}} 
						
						
				break;}
			
			case 245: {}					//to check collision with centepede head	
			case 145: {
				
				sf::SoundBuffer kkill;
				sf::Sound kill;
				
				kkill.loadFromFile("Sound Effects/kill.wav");
				kill.setBuffer(kkill);
				kill.setVolume(80);
				
				sf::Clock soundDelay;
				kill.play();
				while (soundDelay.getElapsedTime().asMilliseconds() < 200){}
				
				if (by>22){
				gamegrid[by][bx] = 4;}
				else {
				gamegrid[by][bx] = 2;}
								
				bullet[exists] = false;
				
				if (level < 6){
					for (int i=0;i<25;i++){
						if (centepedex[i][0] == bx && centepedey[i][0] == by){
							centepedex[i][0] = -1;
							centepedey[i][0] = -1;
							centepedex[i][24] = -1;
							
							for (int j=1;j<25 && centepedex[i][j] != -1;j++){
								
								gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;
								centepedex[i][j] = -1;
								centepedey[i][j] = -1;}
							centepedex[i][24] = -1;
							centepedex[i][25] = -1;}}
						}
				else {
					for (int i=0;i<25;i++){
						if (centepedex[i][0] == bx && centepedey[i][0] == by){
							centepedex[i][0] = -1;
							centepedey[i][0] = -1;
							centepedex[i][25] = -1;
							
							for (int j=1;j<24;j++){
								if (centepedex[i][j] != -1){
								centepedex[i][j-1] = centepedex[i][j];
								centepedey[i][j-1] = centepedey[i][j];}
								else {
									centepedex[i][j-1] = -1;
									centepedey[i][j-1] = -1;
									break;}}
								break;}}}
						
						score += 20;
				break;}
				}
	by -=1;
				
	if (bullet[y] < -32){
		gamegrid[0][bx] = 0;
		bullet[exists] = false;}
}
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite) 
{								//showing bullet on screen
	bulletSprite.setPosition(bullet[x], bullet[y]);
	window.draw(bulletSprite);
}
void drawlives (sf::RenderWindow& window,int lives, sf::Clock& textClock,int score)
{								//showing live sprite and game end on screen
	sf::Texture liveTexture;
	
	liveTexture.loadFromFile("Textures/heart.png");
	sf::Sprite liveSprite(liveTexture);
	liveSprite.setScale(32.0 / 120.0f, 32.0 / 120.0f);
	
	if (lives == 0 || score == 999999){
			
		sf::Text endtext;
		sf::Font endfont;
		
		endfont.loadFromFile("Textures/OpenSans-Italic.ttf");
		endtext.setFont(endfont);
		endtext.setString("GAME END...");
		endtext.setCharacterSize(124);
		endtext.setFillColor(sf::Color::White);
		endtext.setPosition(160, 300);
			
		textClock.restart(); 
		while (textClock.getElapsedTime().asSeconds() < 2.0) {
		window.clear();
		window.draw(endtext);
		window.display();}
		
		window.close();}
		
	for (int i=32,l=0;l<lives;i+=64,l++){
	liveSprite.setPosition(i,928);
	window.draw(liveSprite);}
}
void bottomline (sf::RenderWindow& window)
{								//showing bottom line on screen	
	sf::Texture slideTexture;
	sf::Sprite slideSprite;
	slideTexture.loadFromFile("Textures/slide.png");
	slideSprite.setTexture(slideTexture);
	slideSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	slideSprite.setColor(sf::Color(255, 0, 0));
	
	for (int i=0;i<960;i+=32){
		slideSprite.setPosition(i,896);
		window.draw(slideSprite);}
}
void assignmushrooms()
{								//giving mushrooms with random values
	short a = 0, j = 0;
	static short n=40;
	
	for (int i=0;i<27;i++){
		for (int j=0;j<30;j++){
			if (gamegrid[i][j] == 2 || gamegrid[i][j] == 1){
				gamegrid[i][j] = 0;}}}
	
	for (int i=1;i<26;){
		a = rand()%n;
		j += a;
		if (j>=30){
			i++;j -=30;}
		gamegrid[i][j] = 2;}
	n -= 2;
	for (int i=0;i<30;i++){
		gamegrid[27][i] = 0;}
}
void drawmushrooms(sf::RenderWindow& window)
{								//showing mushrooms on screem
	sf::Texture mushTexture;
	sf::Sprite mushSprite,mush1Sprite,mush2Sprite,mush3Sprite;
	mushTexture.loadFromFile("Textures/mushroom.png");
	mushSprite.setTexture(mushTexture);
	mushSprite.setTextureRect(sf::IntRect(0, 32,boxPixelsX, boxPixelsY));
	mushSprite.setColor(sf::Color(250, 10, 10));
	
	mush1Sprite.setTexture(mushTexture);
	mush1Sprite.setTextureRect(sf::IntRect(64, 32,boxPixelsX, boxPixelsY));
	//mush1Sprite.setColor(sf::Color(0, 128, 128));
	
	mush2Sprite.setTexture(mushTexture);
	mush2Sprite.setTextureRect(sf::IntRect(0, 0,boxPixelsX, boxPixelsY));
	mush2Sprite.setColor(sf::Color(250, 10, 10));
	
	mush3Sprite.setTexture(mushTexture);
	mush3Sprite.setTextureRect(sf::IntRect(64, 0,boxPixelsX, boxPixelsY));
	//mush1Sprite.setColor(sf::Color(0, 128, 128));
	
	for (int i=0;i<27;i++){
		for (int j=0;j<30;j++){
			if (gamegrid[i][j] == 4 ){		//for poisonous full mushroom
				mush2Sprite.setPosition(j*32,i*32);
				window.draw(mush2Sprite);}
				
			if (gamegrid[i][j] == 3 ){		//for poisonous half mushroom
				mush3Sprite.setPosition(j*32,i*32);
				window.draw(mush3Sprite);}
			
			if (gamegrid[i][j] == 2 ){		//for normal full mushroom
				mushSprite.setPosition(j*32,i*32);
				window.draw(mushSprite);}
			
			if (gamegrid[i][j] == 1 ){		//for normal half mushroom
				mush1Sprite.setPosition(j*32,i*32);
				window.draw(mush1Sprite);}}}
}
void updatescore(sf::RenderWindow& window, int score, short &lives)
{								//updating score on screen
	static int checkscore = 10000;
	
	if (score >= checkscore){
		checkscore += 10000;
	if (lives < 6){						//giving extra lives
		lives += 1;
		
		sf::SoundBuffer bbonus;
		sf::Sound bonus;
				
		bbonus.loadFromFile("Sound Effects/kill.wav");
		bonus.setBuffer(bbonus);
		bonus.setVolume(80);
				
		sf::Clock soundb;
		bonus.play();
		while (soundb.getElapsedTime().asMilliseconds() < 200){}
	}}
		
	sf::Font sfont;
	sf::Text scor;
	
	sfont.loadFromFile("Textures/OpenSans-Italic.ttf");
    	scor.setFont(sfont);
    	
    	scor.setCharacterSize(24);
    	scor.setFillColor(sf::Color::Red);
    	scor.setPosition(768+32, 928);	
	
	char buff[10];
	sprintf(buff, "%d",score);
	scor.setString(buff);
	window.draw(scor);
}
void updatelevel(sf::RenderWindow& window, int level)
{								//showing level on screen
	sf::Font sfont;
	sf::Text scor;
	
	sfont.loadFromFile("Textures/OpenSans-Italic.ttf");
    	scor.setFont(sfont);
    	
    	scor.setCharacterSize(24);
    	scor.setFillColor(sf::Color::Green);
    	scor.setPosition(544+48, 928);	
	
	char buff[10];
	sprintf(buff, "%d",level);
	scor.setString(buff);
	window.draw(scor);
}
void initialize_centepede(sf::RenderWindow& window,int centepedex[][26],int centepedey[][24],short &level)
{								//initializing centepede
	for (int i=0;i<25;i++){
	for (int j=0;j<26;j++){
		centepedex[i][j] = -1;}}
	
	int a = rand()%30;
	
	for (int i=0;i<12-level+1;i++){
	centepedex[0][i] = a+i;
	centepedey[0][i] = 0;}
	
	centepedex[0][24] = 2;
	
	for (int i=0;centepedex[0][i+1] != -1;i++){		//initializing fast centepede heads
		if (centepedex[0][i+1]<30){
		gamegrid[0][centepedex[0][i+1]] = 144;}}
	gamegrid[0][centepedex[0][0]] = 245;
	
	for (int i=1,c=0,r=1;i<level;i++){
		
		if (c == 30){
			c = 0;
			r++;}
		
		short a=rand()%5;
		centepedex[i][0] = c+a;
		centepedey[i][0] = r; 
		centepedex[i][24] = 22;
		
		c+=5;
	}
	
}
void draw_centepede(sf::RenderWindow& window)
{								//showing centepede
	sf::Texture cenTexture,cenhTexture,cenhrTexture,fleaTexture,spiderTexture;
	sf::Sprite cenSprite,cenhSprite,cenhrSprite,cen1Sprite,fleaSprite,spiderSprite;
	cenTexture.loadFromFile("Textures/c_body_left_walk.png");
	cenSprite.setTexture(cenTexture);
	cenSprite.setTextureRect(sf::IntRect(0, 0,boxPixelsX, boxPixelsY));
	
	cen1Sprite.setTexture(cenTexture);
	cen1Sprite.setTextureRect(sf::IntRect(0, 0,boxPixelsX, boxPixelsY));
	
	cenhTexture.loadFromFile("Textures/c_head_left_walk.png");
	cenhSprite.setTexture(cenhTexture);
	cenhSprite.setTextureRect(sf::IntRect(0, 0,boxPixelsX, boxPixelsY));
	
	cenhrTexture.loadFromFile("Textures/c_head_right_walk.png");
	cenhrSprite.setTexture(cenhrTexture);
	cenhrSprite.setTextureRect(sf::IntRect(0, 0,boxPixelsX, boxPixelsY));
	
	fleaTexture.loadFromFile("Textures/flea.png");
	fleaSprite.setTexture(fleaTexture);
	fleaSprite.setTextureRect(sf::IntRect(0, 0,boxPixelsX, boxPixelsY));
	
	spiderTexture.loadFromFile("Textures/spider_and_score.png");
	spiderSprite.setTexture(spiderTexture);
	spiderSprite.setTextureRect(sf::IntRect(0, 0,64, boxPixelsY));
	spiderSprite.setScale(0.6f, 0.6f);
	
	cen1Sprite.setColor(sf::Color(240, 10, 10));
	
	for (int i=0;i<28;i++){
		for (int j=0;j<30;j++){
			switch (gamegrid[i][j]){
					
					case 244: {cen1Sprite.setPosition(j*32,i*32);	//poisonous
							window.draw(cen1Sprite);
							break;}	
					case 144: {cenSprite.setPosition(j*32,i*32);	//normal
							window.draw(cenSprite);
							break;}
					case 245: {cenhSprite.setPosition(j*32,i*32);	//right head
							window.draw(cenhSprite);
							break;}
					case 145: {cenhrSprite.setPosition(j*32,i*32);	//left head
							window.draw(cenhrSprite);
							break;}
					case 555: {fleaSprite.setPosition(j*32,i*32);	//flea
							window.draw(fleaSprite);
							break;}
					case 556: {spiderSprite.setPosition(j*32,i*32);	//flea
							window.draw(spiderSprite);
							break;}
					}}}
}
void move_centepede(sf::RenderWindow& window,sf::Clock& centepedeClock,short &level,int centepedex[][26],int centepedey[][24],short &lives,short &speed)
{								//moving normal centepede
	
	if (centepedeClock.getElapsedTime().asMilliseconds() < speed)
		return;
	centepedeClock.restart();

	short lencen = 0;
	
	for (int i=0;i<25;i++){					//main loop
	
	if (centepedex[i][24] > 0){
	lencen =0;
	for (int j=0;(centepedex[i][j] != -1) && (j<24);j++){	//measuring length of centepede
		lencen++;} 
		
	if (lencen > 0){
					
		if (centepedex[i][24] == 1 || centepedex[i][24] == 11 || centepedex[i][24] == 3 || centepedex[i][24] == 13){
								//right side condition
		if (centepedex[i][0] == 29){
			 					//corner 
			if (centepedex[i][1] == -1 && centepedey[i][0]>22){
				gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
				centepedex[i][24] = 2;
				break;}		
			
			for (int j=0;j<lencen;j++){
				gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
			
			if (centepedex[i][24] == 1 || centepedex[i][24] == 11){	
				centepedey[i][0] += 1;}
			else if (centepedex[i][24] == 3 || centepedex[i][24] == 13){
				centepedey[i][0] -= 1;}
				
			if ((centepedex[i][24] == 3 || centepedex[i][24] == 13) && centepedey[i][0] == 22){
				centepedey[i][0] += 2;
				
				gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
						if (centepedex[i][24] == 3){
							
							for (int j=0;j<lencen-1-1;j++){
								gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}
							centepedex[i][24] = 2;}
						else if (centepedex[i][24] == 13){
							
							for (int j=0;j<lencen-1-1;j++){
								gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}
							centepedex[i][24] = 12;}
					}
				
					if (gamegrid[centepedey[i][0]][centepedex[i][0]] == 2){
					centepedey[i][0] -= 1;}
			
					if (centepedey[i][0] == 28){
					
						centepedey[i][0] -= 2;
						
						gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
						if (centepedex[i][24] == 1){
							
							for (int j=0;j<lencen-1-1;j++){
								gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}
							centepedex[i][24] = 4;}
						else if (centepedex[i][24] == 11){
							
							for (int j=0;j<lencen-1-1;j++){
								gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}
							centepedex[i][24] = 14;}
					}
			
			
			
			
			for (int j=lencen-1;j>0;j--){
			centepedex[i][j] = centepedex[i][j-1];}
				
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
					if (centepedex[i][24] == 1 || centepedex[i][24] == 3){
						
						for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
							gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}
						if (centepedex[i][24] == 1){
							centepedex[i][24] = 2;}
						else {
							centepedex[i][24] = 4;}
						}
					else if (centepedex[i][24] == 11 || centepedex[i][24] == 13){
						
						for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
							gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}
						if (centepedex[i][24] == 11){
							centepedex[i][24] = 12;}
						else {
							centepedex[i][24] = 14;}
						}
			break;}
			
		switch (gamegrid[centepedey[i][0]][(centepedex[i][0])+1]){
			
			case 11: {	
									
				for (int j=0;j<lencen;j++){
					//gamegrid[centepedey[i][j]][centepedex[i][j]+1] = 0;
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;
					centepedex[i][j] = -1;
					centepedey[i][j] = -1;
					centepedex[i][24] = -1;}
					lives -= 1;
					
					sf::SoundBuffer ddeath;
					sf::Sound death;
					
					ddeath.loadFromFile("Sound Effects/death.wav");
					death.setBuffer(ddeath);
					death.setVolume(80);
					
					sf::Clock soundDeath;
					death.play();
					while (soundDeath.getElapsedTime().asMilliseconds() < 200){}	
				break;}
			
			case 244: {}	
			case 144: {}
			case 245: {}	
			case 0: { 
				 
				for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
					
				for (int j=lencen-1;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];
					centepedey[i][j] = centepedey[i][j-1];}
				
					centepedex[i][0] += 1;
				
				if (centepedex[i][24] == 1 || centepedex[i][24] == 3){
				for (int j=1;j<lencen && centepedex[0][j]<30;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 144;}}
				else if (centepedex[i][24] == 11 || centepedex[i][24] == 13){
				for (int j=1;j<lencen && centepedex[0][j]<30;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 244;}}
					
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
				
	
				break;}
			case 3: {					//poisionous half mushroom
				gamegrid[centepedey[i][0]][(centepedex[i][0])+1] = 0;
				
				for (int j=0;j<lencen-1;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}

					gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
					
					centepedex[i][24] = 11;

				break;}
			
			case 4: {					//poisionous full mushroom
				gamegrid[centepedey[i][0]][(centepedex[i][0])+1] = 0;
				
				for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
					
				for (int j=1;j<12;j++){
				centepedex[i][j] = centepedex[i][0] - j;
				centepedey[i][j] = centepedey[i][0];}
				
				for (int j=0;j<lencen-1 && centepedex[i][j+1]>-1;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}

					gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
				centepedex[i][24] = 11;
				
				break;}
							
			case 1:{					//normal half mushroom
			
				if (lencen < 24){
				
				for (int j=lencen;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];
					centepedey[i][j] = centepedey[i][j-1];}	
	
					centepedex[i][0] += 1;
					
					if (centepedex[i][24] == 1 || centepedex[i][24] == 3){
					for (int j=1;j<lencen;j++){
						gamegrid[centepedey[i][j]][centepedex[i][j]] = 144;}}
					else if (centepedex[i][24] == 11 || centepedex[i][24] == 13){
					for (int j=1;j<lencen;j++){
						gamegrid[centepedey[i][j]][centepedex[i][j]] = 244;}}
						
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
				}
				else if (lencen == 24){
					for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}

					for (int j=lencen-1;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];}	

					centepedex[i][0] += 1;
					
					if (centepedex[i][24] == 1 || centepedex[i][24] == 3){
					for (int j=1;j<lencen;j++){
						gamegrid[centepedey[i][j]][centepedex[i][j]] = 144;}}
					else if (centepedex[i][24] == 11 || centepedex[i][24] == 13){
					for (int j=1;j<lencen;j++){
						gamegrid[centepedey[i][j]][centepedex[i][j]] = 244;}}
						
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
				}
				break;}
				
			case 2:{					//normal full mushroom
				gamegrid[centepedey[i][0]][(centepedex[i][0])+1] = 0;
				centepedex[i][25] += 1;
				
				for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
					
				for (int j=lencen-1;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];
					centepedey[i][j] = centepedey[i][j-1];}
				
				
					centepedey[i][0] += 1;
				
					if ((centepedey[i][0] == 27 && centepedex[i][1] == -1) || gamegrid[centepedey[i][0]][centepedex[i][0]] == 2){
						centepedey[i][0] -= 1;}
					
					if (centepedex[i][24] == 1 || centepedex[i][24] == 3){
					for (int j=1;j<lencen;j++){
						gamegrid[centepedey[i][j]][centepedex[i][j]] = 144;}}
					else if (centepedex[i][24] == 11 || centepedex[i][24] == 13){
					for (int j=1;j<lencen;j++){
						gamegrid[centepedey[i][j]][centepedex[i][j]] = 244;}}
										
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
					centepedex[i][24] = 2;
				break;}
				}}
				
		else if (centepedex[i][24] == 2 || centepedex[i][24] == 12 || centepedex[i][24] == 4 || centepedex[i][24] == 14){
										//left condition
			if (centepedex[i][0] == 0){
										//corner
				if (centepedex[i][1] == -1 && centepedey[i][0]>22){
				gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
				centepedex[i][24] = 1;
				break;}		
				
				for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
					
				if (centepedex[i][24] == 2 || centepedex[i][24] == 12){	
					centepedey[i][0] += 1;}
				else if (centepedex[i][24] == 4 || centepedex[i][24] == 14){
					centepedey[i][0] -= 1;}
					
				if ((centepedex[i][24] == 4 || centepedex[i][24] == 14) && centepedey[i][0] == 22){
				centepedey[i][0] += 2;
				
				gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
						if (centepedex[i][24] == 4){
							
							for (int j=0;j<lencen-1-1;j++){
								gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}
							centepedex[i][24] = 1;}
						else if (centepedex[i][24] == 14){
							
							for (int j=0;j<lencen-1-1;j++){
								gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}
							centepedex[i][24] = 11;}
					}
					
					if (gamegrid[centepedey[i][0]][centepedex[i][0]] == 2){
						centepedey[i][0] -= 1;}
				
					if (centepedey[i][0] == 28){
					
						centepedey[i][0] -= 2;
						
						gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
						if (centepedex[i][24] == 2){
							
							for (int j=0;j<lencen-1-1;j++){
								gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}
							centepedex[i][24] = 3;}
						else if (centepedex[i][24] == 12){
							
							for (int j=0;j<lencen-1-1;j++){
								gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}
							centepedex[i][24] = 13;}
					}
						
				for (int j=lencen-1;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];}
				
				if (centepedex[i][24] == 2 || centepedex[i][24] == 4){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}
					if (centepedex[i][24] == 2){
						centepedex[i][24] = 1;}
					else {
						centepedex[i][24] = 3;}
					}
				else if (centepedex[i][24] == 12 || centepedex[i][24] == 14){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}
					if (centepedex[i][24] == 12){
						centepedex[i][24] = 11;}
					else {
						centepedex[i][24] = 13;}
					}
						
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
					
				break;}
			
			switch (gamegrid[centepedey[i][0]][(centepedex[i][0])-1]){
			
			case 11: {	
			
				for (int j=0;j<lencen;j++){
					
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;
					centepedex[i][j] = -1;
					centepedey[i][j] = -1;
					centepedex[i][24] = -1;}
					lives -= 1;
					
					sf::SoundBuffer ddeath;
					sf::Sound death;
					
					ddeath.loadFromFile("Sound Effects/death.wav");
					death.setBuffer(ddeath);
					death.setVolume(80);
					
					sf::Clock soundDeath;
					death.play();
					while (soundDeath.getElapsedTime().asMilliseconds() < 200){}
			
				break;}
			
			case 244: {}	
			case 144: {}
			case 145: {}	
			case 0:{ 
				 
				for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
					
				for (int j=lencen-1;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];
					centepedey[i][j] = centepedey[i][j-1];}
				

					centepedex[i][0] -= 1;
				
				if (centepedex[i][24] == 2 || centepedex[i][24] == 4){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}}
				else if (centepedex[i][24] == 12 || centepedex[i][24] == 14){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}}
					
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
					
				break;}
			
			case 3: {					//poisionous half mushroom
				gamegrid[centepedey[i][0]][(centepedex[i][0])-1] = 0;
				
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}

					gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
					
					centepedex[i][24] = 12;
				break;}
			
			case 4: {					//poisionous full mushroom
				gamegrid[centepedey[i][0]][(centepedex[i][0])-1] = 0;
				
				for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
					
				for (int j=0;j<12;j++){
				centepedex[i][j] = centepedex[i][0] + j;
				centepedey[i][j] = centepedey[i][0];}
				
				for (int j=0;j<lencen-1 && centepedex[i][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}

					gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
				centepedex[i][24] = 12;
				
				break;}
					
			case 1:{					//normal half mushroom
			
				if (lencen < 24){
				
				for (int j=lencen;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];
					centepedey[i][j] = centepedey[i][j-1];}	
	

					centepedex[i][0] -= 1;
					
				if (centepedex[i][24] == 2 || centepedex[i][24] == 4){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}}
				else if (centepedex[i][24] == 12 || centepedex[i][24] == 14){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}}
					
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
				}
				else if (lencen == 24){
					
					for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
					
					for (int j=lencen-1;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];}	
	

					centepedex[i][0] -= 1;
				
				if (centepedex[i][24] == 2 || centepedex[i][24] == 4){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}}
				else if (centepedex[i][24] == 12 || centepedex[i][24] == 14){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}}
					
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
				}
				break;}
				
			case 2:{					//normal full mushroom
				gamegrid[centepedey[i][0]][(centepedex[i][0])-1] = 0;
				centepedex[i][25] += 1;
				
				for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
					
				for (int j=lencen-1;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];
					centepedey[i][j] = centepedey[i][j-1];}
				
					centepedey[i][0] += 1;
				
					if ((centepedey[i][0] == 27 && centepedex[i][1] == -1) || gamegrid[centepedey[i][0]][centepedex[i][0]] == 2){
						centepedey[i][0] -= 1;}
				
				if (centepedex[i][24] == 2 || centepedex[i][24] == 4){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}}
				else if (centepedex[i][24] == 12 || centepedex[i][24] == 14){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}}
					
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
					centepedex[i][24] = 1;
				break;}
				}}
			
		if (centepedex[i][25] == 4){				//doubleing the length of centepede
			centepedex[i][25] = -1;
			
		if (lencen < 13){
		
		if (centepedex[i][24] == 1 || centepedex[i][24] == 3 || centepedex[i][24] == 11 || centepedex[i][24] == 13){	
			for (int g=lencen;g<lencen*2;g++){
				centepedex[i][g] = centepedex[i][g-1] -1;
				centepedey[i][g] = centepedey[i][g-1];}}
				
		else if (centepedex[i][24] == 2 || centepedex[i][24] == 4 || centepedex[i][24] == 12 || centepedex[i][24] == 14){	
			for (int g=lencen;g<lencen*2;g++){
				centepedex[i][g] = centepedex[i][g-1] +1;
				centepedey[i][g] = centepedey[i][g-1];}}}}
				
				}}}
}

void move_fast_centepede(sf::RenderWindow& window,sf::Clock& centepedefClock,short &level,int centepedex[][26],int centepedey[][24],short &lives,short &spee)
{									//moving fast centepede
	
	if (centepedefClock.getElapsedTime().asMilliseconds() < spee)
		return;
	centepedefClock.restart();

	short lencen = 0;
	
	for (int i=0;i<25;i++){						//main loop
	
	if (centepedex[i][24] > 0){
	lencen =0;
	for (int j=0;(centepedex[i][j] != -1) && (j<24);j++){
		lencen++;} 
		
	if (lencen > 0){
					
		if (centepedex[i][24] == 21 || centepedex[i][24] == 211 || centepedex[i][24] == 23 || centepedex[i][24] == 213){
									//right condition
		if (centepedex[i][0] == 29){
			 						//corner
			if (centepedex[i][1] == -1 && centepedey[i][0]>22){
				gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
				centepedex[i][24] = 22;
				break;}		
			
			for (int j=0;j<lencen;j++){
				gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
			
			if (centepedex[i][24] == 21 || centepedex[i][24] == 211){	
				centepedey[i][0] += 1;}
			else if (centepedex[i][24] == 23 || centepedex[i][24] == 213){
				centepedey[i][0] -= 1;}
				
			if ((centepedex[i][24] == 23 || centepedex[i][24] == 213) && centepedey[i][0] == 22){
				centepedey[i][0] += 2;
				
				gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
						if (centepedex[i][24] == 23){
							
							for (int j=0;j<lencen-1-1;j++){
								gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}
							centepedex[i][24] = 22;}
						else if (centepedex[i][24] == 213){
							
							for (int j=0;j<lencen-1-1;j++){
								gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}
							centepedex[i][24] = 212;}
					}
				
					if (gamegrid[centepedey[i][0]][centepedex[i][0]] == 2){
					centepedey[i][0] -= 1;}
			
					if (centepedey[i][0] == 28){
					
						centepedey[i][0] -= 2;
						
						gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
						if (centepedex[i][24] == 21){
							
							for (int j=0;j<lencen-1-1;j++){
								gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}
							centepedex[i][24] = 24;}
						else if (centepedex[i][24] == 211){
							
							for (int j=0;j<lencen-1-1;j++){
								gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}
							centepedex[i][24] = 214;}
					}
			
			
			
			
			for (int j=lencen-1;j>0;j--){
			centepedex[i][j] = centepedex[i][j-1];}
				
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
					if (centepedex[i][24] == 21 || centepedex[i][24] == 23){
						
						for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
							gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}
						if (centepedex[i][24] == 21){
							centepedex[i][24] = 22;}
						else {
							centepedex[i][24] = 24;}
						}
					else if (centepedex[i][24] == 211 || centepedex[i][24] == 213){
						
						for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
							gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}
						if (centepedex[i][24] == 211){
							centepedex[i][24] = 212;}
						else {
							centepedex[i][24] = 214;}
						}
			break;}
			
		switch (gamegrid[centepedey[i][0]][(centepedex[i][0])+1]){
			
			case 11: {	
					
				for (int j=0;j<lencen;j++){
					//gamegrid[centepedey[i][j]][centepedex[i][j]+1] = 0;
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;
					centepedex[i][j] = -1;
					centepedey[i][j] = -1;
					centepedex[i][24] = -1;}
					lives -= 1;
					
					sf::SoundBuffer ddeath;
					sf::Sound death;
					
					ddeath.loadFromFile("Sound Effects/death.wav");
					death.setBuffer(ddeath);
					death.setVolume(80);
					
					sf::Clock soundDeath;
					death.play();
					while (soundDeath.getElapsedTime().asMilliseconds() < 200){}	
				break;}
			
			case 244: {}	
			case 144: {}
			case 245: {}	
			case 0: { 
				 
				for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
					
				for (int j=lencen-1;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];
					centepedey[i][j] = centepedey[i][j-1];}
				
					centepedex[i][0] += 1;
				
				if (centepedex[i][24] == 21 || centepedex[i][24] == 23){
				for (int j=1;j<lencen && centepedex[0][j]<30;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 144;}}
				else if (centepedex[i][24] == 211 || centepedex[i][24] == 213){
				for (int j=1;j<lencen && centepedex[0][j]<30;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 244;}}
					
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
				
	
				break;}
			case 3: {					//poisionous half mushroom
				gamegrid[centepedey[i][0]][(centepedex[i][0])+1] = 0;
				
				for (int j=0;j<lencen-1;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}

					gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
					
					centepedex[i][24] = 211;

				break;}
			
			case 4: {					//poisionous full mushroom
				gamegrid[centepedey[i][0]][(centepedex[i][0])+1] = 0;
				
				for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
					
				for (int j=1;j<12;j++){
				centepedex[i][j] = centepedex[i][0] - j;
				centepedey[i][j] = centepedey[i][0];}
				
				for (int j=0;j<lencen-1 && centepedex[i][j+1]>-1;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}

					gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
				centepedex[i][24] = 211;
				
				break;}
							
			case 1:{					//normal half mushroom
			
				if (lencen < 24){
				
				for (int j=lencen;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];
					centepedey[i][j] = centepedey[i][j-1];}	
	
					centepedex[i][0] += 1;
					
					if (centepedex[i][24] == 1 || centepedex[i][24] == 3){
					for (int j=1;j<lencen;j++){
						gamegrid[centepedey[i][j]][centepedex[i][j]] = 144;}}
					else if (centepedex[i][24] == 11 || centepedex[i][24] == 13){
					for (int j=1;j<lencen;j++){
						gamegrid[centepedey[i][j]][centepedex[i][j]] = 244;}}
						
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
				}
				else if (lencen == 24){
					for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}

					for (int j=lencen-1;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];}	

					centepedex[i][0] += 1;
					
					if (centepedex[i][24] == 21 || centepedex[i][24] == 23){
					for (int j=1;j<lencen;j++){
						gamegrid[centepedey[i][j]][centepedex[i][j]] = 144;}}
					else if (centepedex[i][24] == 211 || centepedex[i][24] == 213){
					for (int j=1;j<lencen;j++){
						gamegrid[centepedey[i][j]][centepedex[i][j]] = 244;}}
						
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
				}
				break;}
				
			case 2:{					//normal full mushroom
				gamegrid[centepedey[i][0]][(centepedex[i][0])+1] = 0;
				centepedex[i][25] += 1;
				
				for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
					
				for (int j=lencen-1;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];
					centepedey[i][j] = centepedey[i][j-1];}
				
					centepedey[i][0] += 1;
				
					if ((centepedey[i][0] == 27 && centepedex[i][1] == -1) || gamegrid[centepedey[i][0]][centepedex[i][0]] == 2){
						centepedey[i][0] -= 1;}
					
					if (centepedex[i][24] == 21 || centepedex[i][24] == 23){
					for (int j=1;j<lencen;j++){
						gamegrid[centepedey[i][j]][centepedex[i][j]] = 144;}}
					else if (centepedex[i][24] == 211 || centepedex[i][24] == 213){
					for (int j=1;j<lencen;j++){
						gamegrid[centepedey[i][j]][centepedex[i][j]] = 244;}}
										
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
					centepedex[i][24] = 22;
				break;}
				}}
				
		else if (centepedex[i][24] == 22 || centepedex[i][24] == 212 || centepedex[i][24] == 24 || centepedex[i][24] == 214){
									//left condition	
			if (centepedex[i][0] == 0){
									//corner
				if (centepedex[i][1] == -1 && centepedey[i][0]>22){
				gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
				centepedex[i][24] = 21;
				break;}		
				
				for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
					
				if (centepedex[i][24] == 22 || centepedex[i][24] == 212){	
					centepedey[i][0] += 1;}
				else if (centepedex[i][24] == 24 || centepedex[i][24] == 214){
					centepedey[i][0] -= 1;}
					
				if ((centepedex[i][24] == 24 || centepedex[i][24] == 214) && centepedey[i][0] == 22){
				centepedey[i][0] += 2;
				
				gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
						if (centepedex[i][24] == 24){
							
							for (int j=0;j<lencen-1-1;j++){
								gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}
							centepedex[i][24] = 21;}
						else if (centepedex[i][24] == 214){
							
							for (int j=0;j<lencen-1-1;j++){
								gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}
							centepedex[i][24] = 211;}
					}
					
					if (gamegrid[centepedey[i][0]][centepedex[i][0]] == 2){
						centepedey[i][0] -= 1;}
				
					if (centepedey[i][0] == 28){
					
						centepedey[i][0] -= 2;
						
						gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
						if (centepedex[i][24] == 22){
							
							for (int j=0;j<lencen-1-1;j++){
								gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}
							centepedex[i][24] = 23;}
						else if (centepedex[i][24] == 212){
							
							for (int j=0;j<lencen-1-1;j++){
								gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}
							centepedex[i][24] = 213;}
					}
				
				
				
					
				for (int j=lencen-1;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];}
				
				if (centepedex[i][24] == 22 || centepedex[i][24] == 24){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}
					if (centepedex[i][24] == 22){
						centepedex[i][24] = 21;}
					else {
						centepedex[i][24] = 23;}
					}
				else if (centepedex[i][24] == 212 || centepedex[i][24] == 214){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}
					if (centepedex[i][24] == 212){
						centepedex[i][24] = 211;}
					else {
						centepedex[i][24] = 213;}
					}
						
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
					
				break;}
			
			switch (gamegrid[centepedey[i][0]][(centepedex[i][0])-1]){
			
			case 11: {	
			
					//gamegrid[centepedey[i][j]][centepedex[i][j]-1] = 0;
					
				for (int j=0;j<lencen;j++){
					//gamegrid[centepedey[i][j]][centepedex[i][j]-1] = 0;
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;
					centepedex[i][j] = -1;
					centepedey[i][j] = -1;
					centepedex[i][24] = -1;}
					lives -= 1;
					
					sf::SoundBuffer ddeath;
					sf::Sound death;
					
					ddeath.loadFromFile("Sound Effects/death.wav");
					death.setBuffer(ddeath);
					death.setVolume(80);
					
					sf::Clock soundDeath;
					death.play();
					while (soundDeath.getElapsedTime().asMilliseconds() < 200){}
			
				break;}
			
			case 244: {}	
			case 144: {}
			case 145: {}	
			case 0:{ 
				 
				for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
					
				for (int j=lencen-1;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];
					centepedey[i][j] = centepedey[i][j-1];}
				

					centepedex[i][0] -= 1;
				
				if (centepedex[i][24] == 22 || centepedex[i][24] == 24){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){

					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}}
				else if (centepedex[i][24] == 212 || centepedex[i][24] == 214){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}}
					
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
					
				break;}
			
			case 3: {					//poisionous half mushroom
				gamegrid[centepedey[i][0]][(centepedex[i][0])-1] = 0;
				
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}

					gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
					
					centepedex[i][24] = 212;
				break;}
			
			case 4: {					//poisionous full mushroom
				gamegrid[centepedey[i][0]][(centepedex[i][0])-1] = 0;
				
				for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
					
				for (int j=0;j<12;j++){
				centepedex[i][j] = centepedex[i][0] + j;
				centepedey[i][j] = centepedey[i][0];}
				
				for (int j=0;j<lencen-1 && centepedex[i][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}

					gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
				centepedex[i][24] = 212;
				
				break;}
					
			case 1:{					//normal half mushroom
			
				if (lencen < 24){
				
				for (int j=lencen;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];
					centepedey[i][j] = centepedey[i][j-1];}	
	
					centepedex[i][0] -= 1;
					
				if (centepedex[i][24] == 22 || centepedex[i][24] == 24){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}}
				else if (centepedex[i][24] == 212 || centepedex[i][24] == 214){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}}
					
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
				}
				else if (lencen == 24){
					
					for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
					
					for (int j=lencen-1;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];}	
	
					centepedex[i][0] -= 1;
				
				if (centepedex[i][24] == 22 || centepedex[i][24] == 24){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}}
				else if (centepedex[i][24] == 212 || centepedex[i][24] == 214){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}}
					
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 245;
				}
				break;}
				
			case 2:{					//normal full mushroom
				gamegrid[centepedey[i][0]][(centepedex[i][0])-1] = 0;
				centepedex[i][25] += 1;
				
				for (int j=0;j<lencen;j++){
					gamegrid[centepedey[i][j]][centepedex[i][j]] = 0;}
					
				for (int j=lencen-1;j>0;j--){
					centepedex[i][j] = centepedex[i][j-1];
					centepedey[i][j] = centepedey[i][j-1];}
				
					centepedey[i][0] += 1;
				
					if ((centepedey[i][0] == 27 && centepedex[i][1] == -1) || gamegrid[centepedey[i][0]][centepedex[i][0]] == 2){
						centepedey[i][0] -= 1;}
				
				if (centepedex[i][24] == 22 || centepedex[i][24] == 24){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 144;}}
				else if (centepedex[i][24] == 212 || centepedex[i][24] == 214){
				for (int j=0;j<lencen-1 && centepedex[0][j+1]<30;j++){
					gamegrid[centepedey[i][j+1]][centepedex[i][j+1]] = 244;}}
					
					gamegrid[centepedey[i][0]][centepedex[i][0]] = 145;
					centepedex[i][24] = 21;
				break;}
				}}
			
		if (centepedex[i][25] == 4){				//doubleing the length of centepede
			centepedex[i][25] = -1;
			
		if (lencen < 13){
		
		if (centepedex[i][24] == 21 || centepedex[i][24] == 23 || centepedex[i][24] == 211 || centepedex[i][24] == 213){	
			for (int g=lencen;g<lencen*2;g++){
				centepedex[i][g] = centepedex[i][g-1] -1;
				centepedey[i][g] = centepedey[i][g-1];}}
				
		else if (centepedex[i][24] == 22 || centepedex[i][24] == 24 || centepedex[i][24] == 212 || centepedex[i][24] == 214){	
			for (int g=lencen;g<lencen*2;g++){
				centepedex[i][g] = centepedex[i][g-1] +1;
				centepedey[i][g] = centepedey[i][g-1];}}}}
				
				}}}
}

void generateheads(sf::Clock& centepedeheadClock,int centepedex[][26],int centepedey[][24])
{									//generating heads in player area
	if (centepedeheadClock.getElapsedTime().asMilliseconds() < 10000)
		return;
	centepedeheadClock.restart();
	
	for (int j=0;j<25;j++){
		if (centepedex[j][24] == -1){
		for (int i=0;i<25;i++){
			if (centepedey[i][0] > 22){
				short a = rand()%2;
				short b = rand()%4;
			
				switch (a){
					case 0: {			//from left side
						centepedex[j][0] = 29;
						centepedey[j][0] = b+23;
						
						centepedex[j][24] = 2;
						gamegrid[centepedey[j][0]][centepedex[j][0]] = 245;
						
						break;}
					
					case 1: {			//from right side
						centepedex[j][0] = 0;
						centepedey[j][0] = b+23;
						
						centepedex[j][24] = 1;
						gamegrid[centepedey[j][0]][centepedex[j][0]] = 145;
						
						break;}}
						break;}
		}
		break;}
	}	
}

void intitializeflea(sf::Clock& fleaClock,int flea[])
{								//initializing flea
	if (fleaClock.getElapsedTime().asSeconds() < 10)
		return;
	fleaClock.restart();
	
	int checkk = 0;
	
	for (int i=23;i<27;i++){
		for (int j=0;j<30;j++){
			if (gamegrid[i][j] == 2 || gamegrid[i][j] == 4){
				checkk++;}}}
				
	if (checkk == 3){
		int a = rand()%30;
		
		gamegrid[0][a] = 555;
		flea[x] = a;
		flea[y] = 0;
		flea[2] = true;}
}	

void moveflea(sf::Clock& fleafClock,int flea[])
{								//moving flea
	if (flea[2] == true){	
	
	if (fleafClock.getElapsedTime().asMilliseconds() < 100)
		return;
	fleafClock.restart();
	
	int a = rand()&3;
	int b = rand()&3;
	
	if (a == b){
		gamegrid[flea[y]][flea[x]] = 2;}
	else {
		gamegrid[flea[y]][flea[x]] = 0;}
	
	flea[y] += 1;
	if (flea[y] == 28){
		flea[2] = false;}
	if (flea[2] == true){
		gamegrid[flea[y]][flea[x]] = 555;}
		
	}
}

void initializespider(sf::Clock& spiderClock,int spider[])
{
	if (spider[2] == false){
		if (spiderClock.getElapsedTime().asSeconds() < 15)
		return;
		spiderClock.restart();
	
	spider[x] = 0;
	spider[y] = 26;
	spider[3] = 1;
	spider[4] = 11;
	spider[2] = 1;
	gamegrid[spider[y]][spider[x]] = 556;}
}	

void movespider(sf::Clock& spiderfClock,int spider[],short &lives)
{
	if (spider[2] == true){
	
	if (spiderfClock.getElapsedTime().asMilliseconds() < 100)
		return;
	spiderfClock.restart();
	
	gamegrid[spider[y]][spider[x]] = 0;
	
	if (spider[x] == 29){
		spider[3] = 2;}
	else if (spider[x] == 0){
		spider[3] = 1;}
	
	if (spider[y] == 23){
		spider[4] = 12;}
	else if (spider[y] == 27){
		spider[4] = 11;}
	
	if (spider[3] == 1){
		spider[x] += 1;}
	else {
		spider[x] -= 1;}
		
	if (spider[4] == 11){
		spider[y] -= 1;}
	else {
		spider[y] += 1;}
		
	if (gamegrid[spider[y]][spider[x]] == 11){
		lives -= 1;
		spider[2] = false;}
	else {
		gamegrid[spider[y]][spider[x]] = 556;}
	
	}
}










			
