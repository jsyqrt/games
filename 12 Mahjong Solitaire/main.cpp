#include <SFML/Graphics.hpp>
#include <fstream>
#include <time.h>
using namespace sf;

int field[50][50][50] = {0};

int& f(int x,int y,int z){return field[y+2][x+2][z];}
int& f(Vector3i v){return f(v.x,v.y,v.z);}


bool isOpen(int x,int y,int z)
{
  for(int i=-1;i<=1;i++)
   for(int j=-1;j<=1;j++)
    if (f(x+2,y+i,z)>0 && f(x-2,y+j,z)>0) return 0;

  for(int i=-1;i<=1;i++)
   for(int j=-1;j<=1;j++)
    if ( f(x+i,y+j,z+1)>0 ) return 0;

  return 1;
}


int main()
{
    srand(time(0));

    RenderWindow app(VideoMode(740, 570), "Mahjong Solitaire!");

    Texture t1,t2;
    t1.loadFromFile("files/tiles.png");
    t2.loadFromFile("files/background.png");
    Sprite s(t1), sBackground(t2);
	int w=48, h=66;
	int stepX=w/2-2, stepY=h/2-2;
	float offX=4.6, offY=7.1; // z offset
    Vector3i v1,v2;
    std::vector<Vector3i> moves;


    ////load from file////
    std::fstream myfile("files/map.txt");
    for(int y=0;y<18;y++)
     for(int x=0;x<30;x++)
      {
        char a;  myfile >> a;
        int n = a - '0';
        for(int z=0;z<n;z++)
          if (f(x-1,y-1,z)) f(x-1,y,z)=f(x,y-1,z)=0;
          else f(x,y,z)=1;
      }

    ////create map//////
    for(int k=1;;k++)
    {
     std::vector<Vector3i> opens;
     for(int z=0;z<10;z++)
      for(int y=0;y<18;y++)
       for(int x=0;x<30;x++)
        if (f(x,y,z)>0 && isOpen(x,y,z)) opens.push_back(Vector3i(x,y,z));

     int n=opens.size();
     if (n<2) break;
     int a=0,b=0;
     while(a==b){a=rand()%n;b=rand()%n;}
     f(opens[a])=-k;  if (k>34) k++;
     f(opens[b])=-k;
     k%=42;
    }

    for(int z=0;z<10;z++)
     for(int y=0;y<18;y++)
      for(int x=0;x<30;x++) f(x,y,z)*=-1;


    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();

			//move back
            if (e.type == Event::MouseButtonReleased)
				if (e.key.code == Mouse::Right)
                  {
                    int n = moves.size();
					if (n==0) continue;
                    f(moves[n-1])*=-1; moves.pop_back();
                    f(moves[n-2])*=-1; moves.pop_back();
                  }

   			if (e.type == Event::MouseButtonPressed)
				if (e.key.code == Mouse::Left)
                  for(int z=0;z<10;z++)
                   {
                     Vector2i pos = Mouse::getPosition(app) - Vector2i(30,0); // 30 - desk offset
                     int x = (pos.x-z*offX)/stepX;
                     int y = (pos.y+z*offY)/stepY;

                     for(int i=0;i<2;i++)
                      for(int j=0;j<2;j++)
                        if (f(x-i,y-j,z)>0 && isOpen(x-i,y-j,z))
                           v1=Vector3i(x-i,y-j,z);

                     if (v1==v2) continue;

                     int a=f(v1),b=f(v2);
                     if ( a==b || (a>34 && a<39 && b>34 && b<39) || (a>=39 && b>=39) )
                        {
                         f(v1)*=-1; moves.push_back(v1);
                         f(v2)*=-1; moves.push_back(v2);
                        }
                     v2=v1;
                   }
        }

       app.clear();
       app.draw(sBackground);
       for(int z=0;z<10;z++)
        for(int x=30;x>=0;x--)
         for(int y=0;y<18;y++)
         {
            int k = f(x,y,z)-1;
            if (k<0) continue;
            s.setTextureRect(IntRect(k*w,0,w,h));
            if (isOpen(x,y,z)) s.setTextureRect(IntRect(k*w,h,w,h));
            s.setPosition(x*stepX + z*offX, y*stepY - z*offY);
            s.move(30,0); //desk offset
            app.draw(s);
          }

        app.display();
    }

    return 0;
}
