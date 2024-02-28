#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

//Object IDS
enum IDS { PLAYER, BULLET, ENEMY };  //purpose of this it to have unique id my every object
//Our Player
struct SpaceShip
{
int ID;
int x;
int y;
int lives;
int speed;
int boundx;
int boundy;
int score;
};

struct Bullet
{
	int ID;
	int x;
	int y;
	bool live;
	int speed;
};

struct Enemy
{
int ID;
int x;
int y;
bool live;
int speed;
int boundx;
int boundy;
};

//GLOBALS======== ===========
const int WIDTH = 800;
const int HEIGHT = 400;
const int NUM_BULLETS = 10;
const int NUM_ENEMIES = 10;

enum KEYS {UP,DOWN,LEFT,RIGHT,SPACE};
bool keys[5]={false,false,false,false,false};
//prototypes	

void InitShip(SpaceShip &ship);
void DrawShip(SpaceShip &ship);
void MoveShipUp(SpaceShip &ship);
void MoveShipDown(SpaceShip &ship);
void MoveShipLeft(SpaceShip &ship);
void MoveShipRight(SpaceShip &ship);

//initialize bullet..//
void InitBullet(Bullet bullet[],int size);
void DrawBullet(Bullet bullet[],int size);
void FireBullet(Bullet bullet[],int size,SpaceShip &ship);
void UpdateBullet(Bullet bullet[],int size);
void CollideBullet (Bullet bullet[], int bSize, Enemy enemies [], int cSize, SpaceShip &ship);

void InitEnemy (Enemy enemies[], int size);
void DrawEnemy (Enemy enemies[], int size);
void StartEnemy (Enemy enemies[], int size);
void UpdateEnemy (Enemy enemies [], int size);
void CollideEnemy (Enemy enemies [], int cSize, SpaceShip &ship);

int main(void)
{
	//primitive variable.
	bool done = false;
	bool redraw = true;
	const int FPS = 60;
	bool isGameOver = false;
	//creating object variables
	SpaceShip ship;
	Bullet bullets [NUM_BULLETS];
	Enemy enemies [NUM_ENEMIES];
   
   
   //Allegro variables
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER* timer = NULL;
	ALLEGRO_FONT* font18 = NULL;
	
	
	//Initialization Functions
	if (!al_init()) //initialize Allegro
		return -1;
	
	display = al_create_display(WIDTH, HEIGHT); //create our display object
	
	if (!display) //test display object
		return -1;
		
	al_init_primitives_addon();
	al_install_keyboard(); 			// this allows us to work with keyboard
	al_init_font_addon();
	al_init_ttf_addon();
	
	event_queue = al_create_event_queue();
	
	timer = al_create_timer(1.0/FPS);
	
	srand (time (NULL));    	//this ensures that the seed value is different each time the program runs
	InitShip(ship);
	InitBullet(bullets,NUM_BULLETS);
	InitEnemy (enemies, NUM_ENEMIES);
		
	font18 = al_load_font("arial.ttf",18,0); 	
		
	al_register_event_source(event_queue,al_get_keyboard_event_source());
	al_register_event_source (event_queue, al_get_timer_event_source (timer));
	al_register_event_source (event_queue, al_get_display_event_source (display));
	
	al_start_timer (timer);

	while (!done)
	{
		ALLEGRO_EVENT ev;    //everytime event is fired this variable will store the  detail about that event
		al_wait_for_event(event_queue,&ev);			// this wait for something as pass event queue with keyboard events register
		
		if (ev.type == ALLEGRO_EVENT_TIMER)		
			{
				redraw = true;
					if (keys [UP])
				MoveShipUp (ship);
				if (keys [DOWN])
				MoveShipDown (ship);
				if (keys [LEFT])
				MoveShipLeft (ship);
				if (keys [RIGHT])
				MoveShipRight (ship);
				
				if(!isGameOver){
				UpdateBullet(bullets, NUM_BULLETS);
				StartEnemy(enemies, NUM_ENEMIES);
				UpdateEnemy(enemies, NUM_ENEMIES);
				CollideBullet (bullets, NUM_BULLETS, enemies, NUM_ENEMIES,ship);
				CollideEnemy (enemies, NUM_ENEMIES, ship);
				if(ship.lives<=0)
				isGameOver = true;
				}
			}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
		done= true;
		}
		
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)		//if the type of event is pressed 
		{
		switch(ev.keyboard.keycode)			// this check which keys is pressed 
		{
		case ALLEGRO_KEY_ESCAPE:
		done = true;
		break;
		case ALLEGRO_KEY_UP:
		keys [UP] = true;
		break;
		case ALLEGRO_KEY_DOWN:
		keys [DOWN] = true;
		break;
		case ALLEGRO_KEY_LEFT:
		keys [LEFT] = true;
		break;
		case ALLEGRO_KEY_RIGHT:
		keys [RIGHT] = true;
		break;
		case ALLEGRO_KEY_SPACE:
		keys [SPACE] = true;
		FireBullet(bullets,NUM_BULLETS,ship);
		break;
	}
}
	else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
		switch(ev.keyboard.keycode)
		{
		case ALLEGRO_KEY_ESCAPE:
		done = true;
		break;
		case ALLEGRO_KEY_UP:
		keys [UP] = false;
		break;
		case ALLEGRO_KEY_DOWN:
		keys [DOWN] = false;
		break;
		case ALLEGRO_KEY_LEFT:
		keys [LEFT] = false;
		break;
		case ALLEGRO_KEY_RIGHT:
		keys [RIGHT] = false;
		break;
		case ALLEGRO_KEY_SPACE:
		keys [SPACE] = false;
		break;
	}
}	

	if(redraw && al_is_event_queue_empty(event_queue))
	{
	redraw  = false;
	if(!isGameOver){
		DrawShip(ship);
		DrawBullet(bullets,NUM_BULLETS);
		DrawEnemy (enemies, NUM_ENEMIES);
	
		al_draw_textf(font18, al_map_rgb(255,0,255),5, 5, 0,"You have %i lives left. Score = %i",ship.lives,ship.score);	
	}
	else
	{ 
		al_draw_textf(font18, al_map_rgb(0,255,255),WIDTH/2, HEIGHT/2,ALLEGRO_ALIGN_CENTER,"Game Over. Final score: %i",ship.score);
	}
	
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
		
}
	}
	al_destroy_display(display); //destroy our display object
	return 0;
}
void InitShip(SpaceShip& ship)
{
	ship.x = 20;
	ship.y = HEIGHT / 2;
	ship.ID = PLAYER;
	ship.lives = 5;
	ship.speed = 7;
	ship.boundx = 6;
	ship.boundy = 7;
	ship.score = 0;
}
void DrawShip(SpaceShip& ship)
{
 // Body
    al_draw_filled_rectangle(ship.x-12,ship.y-2,ship.x+15,ship.y+2,al_map_rgb(255,0,0));
    // Wings
    al_draw_filled_triangle(ship.x-12,ship.y-17,ship.x+12,ship.y,ship.x-12,ship.y+17,al_map_rgb(0,255,0));
    // Fuselage
    al_draw_filled_rectangle(ship.x,ship.y-9,ship.x+10,ship.y-7,al_map_rgb(255,0,0));
    al_draw_filled_rectangle(ship.x,ship.y+9,ship.x+10,ship.y+7,al_map_rgb(255,0,0));
    // Tail flame
    al_draw_filled_triangle(ship.x-18,ship.y-2,ship.x-12,ship.y,ship.x-18,ship.y+2,al_map_rgb(255,165,0));
}

void MoveShipUp(SpaceShip &ship)
{
	ship.y-=ship.speed;
	if(ship.y <0)
	ship.y = 0;
}
void MoveShipDown(SpaceShip &ship)
{
	ship.y+=ship.speed;
	if(ship.y >HEIGHT)
	ship.y = HEIGHT;
}

void MoveShipLeft(SpaceShip &ship)
{
	ship.x-=ship.speed;
	if(ship.x <0)
	ship.x = 0;
}
void MoveShipRight(SpaceShip &ship)
{
		ship.x+=ship.speed;
	if(ship.x >300)
	ship.x = 300;
}
void InitBullet(Bullet bullet[],int size)
{
	for(int i=0; i<size; i++)
	{
		bullet[i].ID = BULLET;
		bullet[i].speed = 15;
		bullet[i].live = false;
				
	}
}
void DrawBullet(Bullet bullet[],int size)
{
	for(int i=0; i<size; i++)
	{
		if(bullet[i].live)
		al_draw_filled_circle(bullet[i].x,bullet[i].y,3,al_map_rgb(255,255,255));	
				
	}
}
void FireBullet(Bullet bullet[],int size,SpaceShip &ship)
{
	for(int i=0; i<size; i++)
	{
		if(!bullet[i].live)
		{
			bullet[i].x	= ship.x+17;	
			bullet[i].y = ship.y;
			bullet[i].live =true;
			break; 
		}
	}
}
void UpdateBullet(Bullet bullet[],int size)
{
	for(int i=0; i<size; i++)
	{
		if(bullet[i].live)
		{
			bullet[i].x	+= bullet[i].speed;	
			if(bullet[i].x >WIDTH)
			bullet[i].live = false;
		}
	}
}
void CollideBullet (Bullet bullet[], int bSize, Enemy enemies [], int cSize, SpaceShip &ship)
{
	for(int i = 0; i < bSize; i++)
	{
		if(bullet[i].live)
		{
			for(int j = 0; j < cSize; j++)
			{
				if (enemies[j].live)
			{
	if(bullet[i].x > (enemies[j].x - enemies [j].boundx) &&
	   bullet[i].x < (enemies[j].x + enemies [j]. boundx) && 
       bullet[i].y > (enemies [j].y - enemies [j].boundy) && 
	   bullet[i].y < (enemies [j].y + enemies [j].boundy))
	{
	bullet[i].live = false;
	enemies [j].live = false;
	ship.score++;
	}
		}
			}
		}
	}
}
void InitEnemy (Enemy enemies [], int size)
{
for (int i = 0; i < size; i++)
{
enemies[i].ID = ENEMY;
enemies[i].live = false;
enemies[i].speed = 5;
enemies[i].boundx = 18;
enemies[i].boundy = 18;
enemies[i].x = 20;
enemies[i].y = HEIGHT / 2;
}
}

void DrawEnemy (Enemy enemies [], int size)
{
for (int i = 0; i < size; i++)
{
if(enemies[i].live)
{
   // Body
    al_draw_filled_rectangle(enemies[i].x - 15, enemies[i].y - 2, enemies[i].x + 12, enemies[i].y + 2, al_map_rgb(255, 0, 0));
    // Wings
	al_draw_filled_triangle(enemies[i].x + 12, enemies[i].y + 17, enemies[i].x - 12, enemies[i].y, enemies[i].x + 12, enemies[i].y - 17, al_map_rgb(255, 165, 0));
	// Fuselage
    al_draw_filled_rectangle(enemies[i].x, enemies[i].y - 7, enemies[i].x + 10, enemies[i].y - 9, al_map_rgb(255, 0, 0));
    al_draw_filled_rectangle(enemies[i].x, enemies[i].y + 7, enemies[i].x + 10, enemies[i].y + 9, al_map_rgb(255, 0, 0));
}
}
}


void StartEnemy (Enemy enemies[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if(!enemies[i].live)
			{
				if(rand() % 500 == 0)
					{							
						enemies[i].live = true;
						enemies[i].x = WIDTH;
			  		     enemies[i].y = 30 + rand() % (HEIGHT - 60);
							break;
								}
						}
				}
}

void UpdateEnemy (Enemy enemies [], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (enemies[i].live)
		{
			enemies [i].x -= enemies[i].speed;
		}
	}		
}
void CollideEnemy (Enemy enemies [], int cSize, SpaceShip &ship)
{
	for(int i = 0; i < cSize; i++)
	{
		if (enemies[i].live)
		{ 
			if (enemies[i].x - enemies[i].boundx < ship.x + ship.boundx &&
				enemies[i].x + enemies[i].boundx > ship.x - ship.boundx &&
				enemies[i].y - enemies[i].boundy < ship.y + ship.boundy &&
				enemies[i].y + enemies[i].boundy> ship.y - ship.boundy)
			{
				ship.lives--;
				enemies[i].live = false;
			}
			else if (enemies[i].x < 0)
			{
				enemies[i].live = false;
				ship.lives--;
			}
		}
	}
}
