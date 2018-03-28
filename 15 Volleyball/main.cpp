#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
using namespace sf;

const float SCALE = 30.f;
const float DEG  =  57.29577f;

b2Vec2 Gravity(0.f, 9.8f);
b2World World(Gravity);

void setWall(int x,int y,int w,int h)
{
	b2PolygonShape gr;  
	gr.SetAsBox(w/SCALE,h/SCALE);

	b2BodyDef bdef;
	bdef.position.Set(x/SCALE, y/SCALE);

	b2Body *b_ground = World.CreateBody(&bdef);
	b_ground->CreateFixture(&gr,1);
}

int main()
{
	RenderWindow window(VideoMode(800, 600), "Volleyball Game!");
    window.setFramerateLimit(60);
	window.setSize(Vector2u(800*0.8,600*0.8));
	
	Texture t1,t2,t3;
	t1.loadFromFile("images/background.png");
	t2.loadFromFile("images/ball.png");
	t3.loadFromFile("images/blobby.png");
	t1.setSmooth(true);
	t2.setSmooth(true);
	t3.setSmooth(true);

	Sprite sBackground(t1), sBall(t2), sPlayer(t3);
	sPlayer.setOrigin(75/2,90/2);
	sBall.setOrigin(32,32);


	/////////box2d///////////
	setWall(400,520,2000,10);
	setWall(400, 450,10,170);
	setWall(0,0,10,2000);
	setWall(800,0,10,2000);


	b2PolygonShape shape;
	shape.SetAsBox(30/SCALE,30/SCALE);
	b2BodyDef bdef;
	bdef.type=b2_dynamicBody;
	///players///////////////
	b2Body *pBody[2];
	for(int i=0;i<2;i++){
	bdef.position.Set(20*i,2);
	b2CircleShape circle;
	circle.m_radius=32/SCALE;
	circle.m_p.Set(0,13/SCALE);
	pBody[i] = World.CreateBody(&bdef);
	pBody[i]->CreateFixture(&circle,5);
	circle.m_radius=25/SCALE;
	circle.m_p.Set(0,-20/SCALE);
	pBody[i]->CreateFixture(&circle,5);
	pBody[i]->SetFixedRotation(true);
	}
	pBody[0]->SetUserData("player1");
	pBody[1]->SetUserData("player2");

	/// ball /////////////
	bdef.position.Set(5,1);
	b2CircleShape circle;
	circle.m_radius=32/SCALE;
	b2Body *b = World.CreateBody(&bdef);
	b2FixtureDef fdef;
	fdef.shape=&circle;
	fdef.restitution=0.95;
	fdef.density=0.2;
	b->CreateFixture(&fdef);
	b->SetUserData("ball");
	/////////////////////////
	
	bool onGround=0;

	while (window.isOpen())
    {	
		Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
        }

        for(int n=0;n<2;n++) // 2 - speed
		World.Step(1/60.f, 8, 3);

		//player1 
		b2Vec2 pos = pBody[0]->GetPosition();
		b2Vec2 vel = pBody[0]->GetLinearVelocity();

		if (Keyboard::isKeyPressed(Keyboard::Right))  vel.x=5;
		if (Keyboard::isKeyPressed(Keyboard::Left))   vel.x=-5;
		if (Keyboard::isKeyPressed(Keyboard::Up))    if (pos.y*SCALE>=463)  vel.y=-13;
		
		if (!Keyboard::isKeyPressed(Keyboard::Right))
        if (!Keyboard::isKeyPressed(Keyboard::Left)) 
			vel.x=0;

		pBody[0]->SetLinearVelocity(vel);


	    //player2 
		pos = pBody[1]->GetPosition();
		vel = pBody[1]->GetLinearVelocity();

		if (Keyboard::isKeyPressed(Keyboard::D))  vel.x=5;
		if (Keyboard::isKeyPressed(Keyboard::A))  vel.x=-5;
		if (Keyboard::isKeyPressed(Keyboard::W))    if (pos.y*SCALE>=463)  vel.y=-13;
		
		if (!Keyboard::isKeyPressed(Keyboard::D))
        if (!Keyboard::isKeyPressed(Keyboard::A)) 
			vel.x=0;

		pBody[1]->SetLinearVelocity(vel);
		
		//ball max speed
		vel = b->GetLinearVelocity();
		if (vel.Length()>15) b->SetLinearVelocity( 15/vel.Length() * vel ); 

		//////////Draw///////////////
		window.draw(sBackground);
			
		for (b2Body* it = World.GetBodyList(); it != 0; it = it->GetNext())
        {
			b2Vec2 pos = it->GetPosition();
			float angle = it->GetAngle();
			
			if (it->GetUserData()=="player1")
			{	
				sPlayer.setPosition(pos.x*SCALE,pos.y*SCALE);
				sPlayer.setRotation(angle*DEG);
				sPlayer.setColor(Color::Red);
				window.draw(sPlayer);	
			}

			if (it->GetUserData()=="player2")
			{	
				sPlayer.setPosition(pos.x*SCALE,pos.y*SCALE);
				sPlayer.setRotation(angle*DEG);
				sPlayer.setColor(Color::Green);
				window.draw(sPlayer);	
			}

			if (it->GetUserData()=="ball")
			{				
				sBall.setPosition(pos.x*SCALE,pos.y*SCALE);
				sBall.setRotation(angle*DEG);
				window.draw(sBall);	
			}	
		}
		
        window.display();
    }
	return 0;
}