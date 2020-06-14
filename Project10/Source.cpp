#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include<iostream>
#include<string>
#include<fstream>
using namespace std;
////GLOBAL VARIABLES
float health = 100 ;
int load = 0;
float timer = 0;
int score = 0;
bool start = true;
bool game = false;
////PROTOTYPE FUNCTION
void createenemybullet(sf::Vector2f G);
////CREATING THE SCREEN
sf::RenderWindow window(sf::VideoMode(800, 600), "Final game");
////FUNCTIONS TO MANAGE SCORE
////FOR HIGHSCORES TO WORK , THERE MUST BE TXT FILE ALREADY IN DIRECTORY WITH 3 0s IN BEGINING
vector <int> scorestore;
void savescore(int highscore) {
			fstream file("score.txt", ios::app);
			file << highscore;
			file << endl;
			file.close();
		}
void calcscore() {
		int highscore;
		fstream file("score.txt");
		while (file >> highscore) {
			scorestore.push_back(highscore);
		}
		for (int i = 0; i < scorestore.size(); i++) {
			for (int j = 0; j < scorestore.size()-1; j++) {
				int temp;
				if (scorestore[j + 1] > scorestore[j]) {
					temp = scorestore[j];
					scorestore[j] = scorestore[j + 1];
					scorestore[j + 1] = temp;
				}
			}
		}
		file.close();
		}
////PLAYER CLASS
class player {
	public :
		sf::Texture imgtext;
		sf::Sprite spr;
		player() {
			imgtext.loadFromFile("player.png");
			spr.setTexture(imgtext);
		}
		void draw() {
			window.draw(spr);
		}
};
////PLAYER GLOBAL INSTANCE
player p;
////MENU CLASS
class Menu {
private :	
	sf::Font f1;
	sf::Text menuitems[3];
	int select = 1;
public :
		
		Menu(float width, float height) {	
			f1.loadFromFile("f1.otf");
			menuitems[0].setString("PLAY");
			menuitems[1].setString("HIGHSCORE");
			menuitems[2].setString("EXIT");
			for (int i = 0; i < 3; i++) {
				menuitems[i].setFont(f1);
				menuitems[i].setPosition((width / 2)-25, ((height / 3)*i/2)+250);
				menuitems[i].setFillColor(sf::Color::Blue);
			}
		}
		void draw() {
			for (int i = 0; i < 3; i++) {
				window.draw(menuitems[i]);
			}
		}
		int moveup() {
			if (select > 0 ) {
				select--;
				for (int i = 0; i < 3; i++) { menuitems[i].setFillColor(sf::Color::Blue); }
				menuitems[select].setFillColor(sf::Color::Red);
			}
			return select;
		}
		int movedown() {
			if (select < 2) {
				select++;
				for (int i = 0; i < 3; i++) { menuitems[i].setFillColor(sf::Color::Blue); }
				menuitems[select].setFillColor(sf::Color::Red);
			}
			return select;
		}
};
////MENU GLOBAL INSTANCE
Menu m(window.getSize().x, window.getSize().y);
////GUI CLASS
class gui {
private:
	sf::Vector2f sizeofhealth;
	sf::Vector2f loadsize;
	sf::Vector2f pos;
	sf::RectangleShape healthbar;
	sf::RectangleShape ammobar;
	sf::Font f1;
	sf::Text t1;
	sf::Text t2;
	sf::Text t3;
	sf::Text finalscore;
	sf::Text indicator;
public :
	gui(sf::Vector2f H,sf::Vector2f G ) {
		f1.loadFromFile("f1.otf");
		sizeofhealth.x = ::health * 2;
		sizeofhealth.y = 25;
		loadsize.x = ::load * 2;
		loadsize.y = 25;
		healthbar.setSize(sizeofhealth);
		ammobar.setSize(loadsize);
		healthbar.setPosition(H);
		ammobar.setPosition(G);
		t1.setFont(f1);
		t2.setFont(f1);
		t3.setFont(f1);
		t1.setString("Health :");
		t2.setString("Ammo :");
		t3.setString(to_string(::score));
		t3.setFillColor(sf::Color::White);
		t2.setFillColor(sf::Color::Red);
		t1.setFillColor(sf::Color::Green);
		t3.setPosition(400, 30);
		t2.setPosition(530, 520);
		t1.setPosition(30, 520);
		t3.setCharacterSize(30);
		t2.setCharacterSize(14);
		t1.setCharacterSize(14);
		indicator.setFont(f1);
		finalscore.setFont(f1);
		indicator.setFillColor(sf::Color::Magenta);
		finalscore.setFillColor(sf::Color::Magenta);
		indicator.setPosition(260, 30);
		finalscore.setPosition(480, 30);
		finalscore.setString(to_string(::score));
		indicator.setString("You scored : ");
	}
	friend void drawGUI();
};
////PARENT SPRITE FOR BULLET(THIS NEEDED TO BE DONE DUE TO MULTIPLE INSTANCES OF BULLETS AT SAME TIME)
sf::Sprite bsrs;
sf::Texture bullettexs;
////BULLET CLASS
class bullet {
private :
	sf::Vector2f pos;
	sf::Sprite bsr;
	sf::Texture bullettex;
	int bulletid;
public :
	bullet(sf::Vector2f G )
	{
		pos.x = G.x;
		pos.y = G.y;
		bsr.setPosition(G);
		::bsrs.setPosition(G);
	}
	void generalmove() {
		bsr.move(5, 0);
		pos = bsr.getPosition();
		bsrs.move(5, 0);
	}
	friend void handlebullets();
};
////PARENT SPRITE FOR OBSTACLES(THIS NEEDED TO BE DONE DUE TO MULTIPLE INSTANCES OF OBSTABCLE AT SAME TIME)
sf::Sprite OBSs;
sf::Texture OBSTEXs;
////OBSTACLE CLASS
class obstacles {
private :
	sf::Vector2f pos;
	sf::Sprite OBS;
	sf::Texture OBSTEX;
public :
	obstacles(sf::Vector2f H ){
		pos = H;
		OBS.setPosition(H);
		::OBSs.setPosition(H);
	}
	void generalmove() {
		OBS.move(-5, 0);
		::OBSs.move(-5, 0);
		pos = OBS.getPosition();
	}
	friend void handleobstacles();
};
////PARENT SPRITE FOR ENEMIES(THIS NEEDED TO BE DONE DUE TO MULTIPLE INSTANCES OF ENEMIES AT SAME TIME)
sf::Sprite Es;
sf::Texture Estex;
////ENEMIES CLASS
class enemies {
private :
	sf::Vector2f pos;
	sf::Sprite E;
	sf::Texture Etex;
	int timer = 0;
public:
	enemies(sf::Vector2f G) {
		pos = G;
		E.setPosition(pos);
		Es.setPosition(pos);
	}
	void generalmove() {
		timer++;
		E.move(-5, 0);
		Es.move(-5, 0);
		pos = E.getPosition();
		if (timer > 50) {
			createenemybullet(pos);
			timer = 0;
		}
	}
	friend void handleenemies();
};
////FOR THIS CLASS,PARENT SPRITE IS NOT NEEDED SINCE WE WILL DETECT ITS COLLISION ONLY WIHT PLAYER WHICH IS ONE AT ANY GIVEN TIME
////CLASS ENEMYBULLET
class enemybullet {
private :
	sf::Vector2f pos;
	sf::Sprite bsr;
	sf::Texture bullettex;
public:
	enemybullet(sf::Vector2f G)
	{
		pos.x = G.x;
		pos.y = G.y;
		bsr.setPosition(G);
	}
	void generalmove() {
		bsr.move(-10, 0);
		pos = bsr.getPosition();
	}
	friend void handleenemybullets();

};
////VECTORS TO STORE INSTANCES DYNAMICALLY
vector <bullet> bulletstore;
vector<obstacles> obstaclestore;
vector <enemies> enemystore;
vector <enemybullet> enemybulletstore;
////FUNCTION TO CREATE BULLET
void createbullet(sf::Vector2f G) {
	bullet b1(G);
	bulletstore.push_back(b1);
}
////FUNCTION TO CREATE ENEMY BULLET
void createenemybullet(sf::Vector2f G) {
	enemybullet eb1(G);
	enemybulletstore.push_back(eb1);
}
////FUNCTION TO CREATE OBSTACLE(INCLUDES ENEMY AS WELL)
void createobstacle(sf::Vector2f H ) {
	int choice = rand()%2 + 1;
	sf::Vector2i imgchoice;
	imgchoice.y = rand() % 3;
	switch (choice)
	{
	case 1: {
		obstacles o1(H );
		obstaclestore.push_back(o1);
		break;
	}
	case 2: {
		enemies e1(H);
		enemystore.push_back(e1);
		break;
	}
	default:
		break;
	}
}
////FUNCTION TO HANDLE BULLETS DISPLAYED ON SCREEN
void handlebullets() {
	for (int i = 0; i < bulletstore.size(); i++) {
		::bullettexs.loadFromFile("bullet.png");
		::bsrs.setTexture(bullettexs);
		bulletstore[i].bullettex.loadFromFile("bullet.png");
		bulletstore[i].bsr.setTexture(bullettexs);
		bulletstore[i].generalmove();
		////DELETION AFTER GETTING OUT OF SCREEN
		if (bulletstore[i].pos.x > 1000) {
			bulletstore.erase(bulletstore.begin() + i);
			break;
		}
		////COLLISION BETWEEN BULLET AND ENEMY
		if (bulletstore[i].bsr.getGlobalBounds().intersects(::Es.getGlobalBounds())) {
			bulletstore.erase(bulletstore.begin() + i);
			::Es.setPosition(1000, 1000);
			break;
		}
		////COLLISION BETWEEN BULLET AND OBSTACLE
		if (bulletstore[i].bsr.getGlobalBounds().intersects(::OBSs.getGlobalBounds())) {
			bulletstore.erase(bulletstore.begin() + i);
			::bsrs.setPosition(1000, 1000);
			break;
		}
		window.draw(bulletstore[i].bsr);
	}
}
////FUNCTION TO HANDLE ENEMYBULLETS DISPLAYED ON SCREEN
void handleenemybullets() {
	for (int i = 0; i < enemybulletstore.size(); i++) {
		enemybulletstore[i].bullettex.loadFromFile("enemybullet.png");
		enemybulletstore[i].bsr.setTexture(enemybulletstore[i].bullettex);
		enemybulletstore[i].generalmove();
		////COLLISION BETWEEN ENEMY BULLET AND PLAYER
		if (enemybulletstore[i].bsr.getGlobalBounds().intersects(p.spr.getGlobalBounds())) {
			enemybulletstore.erase(enemybulletstore.begin() + i);
			health -= 10;
			break;
		}
		////DELETION AFTER GETTING OUT OF SCREEN
		if (enemybulletstore[i].pos.x <= 0) {
			enemybulletstore.erase(enemybulletstore.begin()+i);
			break;
		}
		window.draw(enemybulletstore[i].bsr);
	}
}
////FUNCTION TO HANDLE OBSTACLES DISPLAYED ON SCREEN
void handleobstacles() {
	for (int i = 0; i < obstaclestore.size(); i++) {
		obstaclestore[i].OBSTEX.loadFromFile("astr.png");
		obstaclestore[i].OBS.setTexture(obstaclestore[i].OBSTEX);
		::OBSTEXs.loadFromFile("astr.png");
		::OBSs.setTexture(obstaclestore[i].OBSTEX);
		obstaclestore[i].generalmove();
		////DELETION AFTER GETTING OUT OF SCREEN
		if (obstaclestore[i].pos.x < -50) {
			obstaclestore.erase(obstaclestore.begin() + i);
			break;
		}
		////COLLISION BETWEEN OBSTACLE AND PLAYER
		if (obstaclestore[i].OBS.getGlobalBounds().intersects(p.spr.getGlobalBounds())) {
			obstaclestore.erase(obstaclestore.begin() + i);
			health -= 25;
			break;
		}
		window.draw(obstaclestore[i].OBS);
	}
}
////FUNCTION TO HANDLE ENEMIES DISPLAYED ON SCREEN
void handleenemies() {
	for (int i = 0; i < enemystore.size(); i++) {
		enemystore[i].Etex.loadFromFile("enemy1.png");
		enemystore[i].E.setTexture(enemystore[i].Etex);
		::Estex.loadFromFile("enemy1.png");
		::Es.setTexture(enemystore[i].Etex);
		enemystore[i].generalmove();
		////DELETION AFTER GETTING OUT OF SCREEN
		if (enemystore[i].pos.x < -50) {
			enemystore.erase(enemystore.begin()+i);
			break;
		}
		////COLLISION BETWEEN ENEMY AND BULLET
		if (enemystore[i].E.getGlobalBounds().intersects(::bsrs.getGlobalBounds())) {
			enemystore.erase(enemystore.begin() + i);
			::bsrs.setPosition(	1000, 1000);
			break;
		}
		////COLLISION BETWEEN ENEMY AND PLAYER
		if (enemystore[i].E.getGlobalBounds().intersects(p.spr.getGlobalBounds())) {
			::health -= 50;
			enemystore.erase(enemystore.begin() + i);
			break;
		}
		window.draw(enemystore[i].E);
	}
}
////FUNCTION TO HANDLE GUI
void drawGUI() {
	////SETTING UP THE POSSITIONS
	sf::Vector2f H;
	sf::Vector2f G;
	G.x = 530;
	G.y = 550;
	H.x = 30;
	H.y = 550;
	////CREATING GUI OBJECT AND MANAGING
	gui h1(H, G);
	if (health > 1 and game==true)
	{
		h1.ammobar.setFillColor(sf::Color::Red);
	if (::health > 75)
		h1.healthbar.setFillColor(sf::Color::Green);
	if (::health <= 75 and health >= 25)
		h1.healthbar.setFillColor(sf::Color::Yellow);
	if (::health <= 25)
		h1.healthbar.setFillColor(sf::Color::Red);
	window.draw(h1.healthbar);
	window.draw(h1.ammobar);
	window.draw(h1.t1);
	window.draw(h1.t2);
	window.draw(h1.t3);
	}
	////GUI AFTER PLAYER LOOSES
	if (game == false and start == false) {
		window.draw(h1.indicator);
		window.draw(h1.finalscore);
	}
}
int main() {
	////MENU VARIABLE TO DECLARE WHAT TO DISPLAY
	bool scoremenu = false;
	////MOVE VARIABLES
	bool moveup = false;
	bool movedown = false;
	////SETING THE FONT USED TO DISPLAY SCORES
	sf::Font f1;
	f1.loadFromFile("f1.otf");
	////TEXT TO DISPLAY SCORES
	sf::Text scoreshow[3];
	////CREATING BACKGROUNDS FOR 2 DIFFERENT SCREENS
	sf::Sprite back_start;
	sf::Texture backtext_start;
	sf::Sprite back;
	sf::Texture backtext;
	backtext_start.loadFromFile("back_start.png");
	backtext.loadFromFile("back.png");
	back.setTexture(backtext);
	back_start.setTexture(backtext_start);
	back_start.setPosition(0, 0);
	back.setPosition(0, 0);
	////THIS WILL ACT AS A SCROLL FOR MENU
	int select = 1;
	////SETING RANDOM SEED FOR RANDOMIZATION 
	srand(time(0));
	////SETTING CONSTANT FRAMERATE
	window.setFramerateLimit(120);
	
	sf::Vector2f loc(1000, 0);
	////Settting THE TIMER
	float time = 0;
	////GAME LOOP
	while (window.isOpen()) {
		////SETTING UP EVENTS
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			switch (Event.type)
			{
			case sf::Event::Closed: { window.close(); break; }
			case sf::Event::KeyPressed: {
				////EVENT FOR PLAYER MOVEMENT
				if (Event.key.code == sf::Keyboard::Down) { if (game == true) movedown = true; else select = m.movedown(); }
				if (Event.key.code == sf::Keyboard::Up) { if (game == true)  moveup = true; else select = m.moveup(); }

				////EVENTS FOR MENU HANDLING
				if (Event.key.code == sf::Keyboard::Enter) {
					if (game == false) {
						if (select == 0) {
							game = true; start = false;
							load = 0;
							timer = 0;
							health = 100;
							score = 0;
							cout << game;
							continue;
						}
						if (select == 1) { calcscore(); scoremenu = true; }
						if (select == 2) { window.close(); break; }
					}
				}
				if (Event.key.code == sf::Keyboard::Escape and scoremenu == true) { scoremenu = false; }
				////EVENT FOR PLAYER SHOOTING
				if (Event.key.code == sf::Keyboard::Space) {
					if (::load >= 100 and game == true)
					{
						createbullet(p.spr.getPosition());
						::load = 0;
					}
				}
				break;
			}
			case sf::Event::KeyReleased: {
				if (Event.key.code == sf::Keyboard::Down) { movedown = false;  }
				if (Event.key.code == sf::Keyboard::Up) {  moveup = false;  }
				break;
			}
			}
		}
		////PLAYER MOVEMENT
		if (moveup == true) {
			p.spr.move(0, -3);
		}
		if (movedown == true) {
			p.spr.move(0, 3);
		}
		////DETERMINING THE BACKGROUNDS
		if (start == true) {
			window.draw(back_start);
			}
		if (start == false) {
			window.draw(back);
		}
		////CHECKING BOUNDS OF SCREEN
		if (p.spr.getPosition().y < 0) { p.spr.setPosition(0, 0); }
		if (p.spr.getPosition().y > 550) { p.spr.setPosition(0, 550); }
			////INCREASING THE TIMER
			timer++;
			////AMMUNATION LOAD
			if (::load <= 100)
			{
				load++;
			}
			////CONDITION FOR PLAYER LOOSE
			if (health <= 0 and game == true) {
				savescore(score);
				game = false;
			}
			////CONDITION TO INCREASE SCORE AND ENEMY CREATION
			if (timer > 100 and game == true) {
				::score++;
				timer = 0;
				int y = rand() % 800 + 1;
				loc.y = float(y);
				createobstacle(loc);
			}
			////CONDITON FOR MENU AND PLAYER
			if (game == true)
			{
				p.draw();
			}
			if (game == false and scoremenu == false) {
				m.draw();
			}
			////HANDLING SCOREMENU
			if (scoremenu == true) {
				for (int i = 0; i < 3; i++) {
					scoreshow[i].setFillColor(sf::Color::Blue);
					scoreshow[i].setFont(f1);
					scoreshow[i].setCharacterSize(24);
					scoreshow[i].setPosition((window.getSize().x / 2) - 25, ((window.getSize().y / 3)*i / 2) + 250);
					scoreshow[i].setString(to_string(scorestore[i]));
				}
				sf::Text t1;
				sf::Text t2;
				t1.setFillColor(sf::Color::Blue);
				t2.setFillColor(sf::Color::Blue);
				t1.setFont(f1);
				t2.setFont(f1);
				t1.setPosition((window.getSize().x / 2) - 120, 200);
				t2.setPosition((window.getSize().x / 2) - 100, 500);
				t2.setCharacterSize(18);
				t2.setString("Press ESC for Back");
				t1.setString("TOP SCORES ARE");
				for (int i = 0; i < 3; i++) {
					window.draw(scoreshow[i]);
				}
				window.draw(t1);
				window.draw(t2);
			}
			////DRAWING GUI
			drawGUI();
			////HANDLING ALL INSTANCES USING RESPECTIVE FUNCTIONS
			handleenemybullets();
			handleenemies();
			handleobstacles();
			handlebullets();
			window.display();
			window.clear();
		}
	}

