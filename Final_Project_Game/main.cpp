#include <iostream>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


class CustomSprite : public sf::Sprite
 {
  public:

    void setBounds(int left, int right, int top, int bottom)
     {
      bound_top = top;
      bound_bottom = bottom;
      bound_right = right;
      bound_left = left;
     }


    bool Collision_T(sf::FloatRect wall_bounds, sf::FloatRect guy_bounds)
     {
      if ((guy_bounds.top+guy_bounds.height >= wall_bounds.top-3) && (guy_bounds.top < wall_bounds.top)
      && (guy_bounds.left+guy_bounds.width > wall_bounds.left+3) && (guy_bounds.left < wall_bounds.left+wall_bounds.width-3))
       {
        return 1;
       }
      else{return 0;}

     }

    bool Collision_L(sf::FloatRect wall_bounds, sf::FloatRect guy_bounds)
    {
      if ((guy_bounds.left <= wall_bounds.left+wall_bounds.width) && (guy_bounds.left+guy_bounds.width > wall_bounds.left+wall_bounds.width)
          && (guy_bounds.top+guy_bounds.height > wall_bounds.top+3) && (guy_bounds.top < wall_bounds.top+wall_bounds.height-3))
      {
        return true;
      }
      else
      {
        return false;
      }
    }

    bool Collision_B(sf::FloatRect wall_bounds, sf::FloatRect guy_bounds)
    {
      if ((guy_bounds.top <= wall_bounds.top+wall_bounds.height) && (guy_bounds.top+guy_bounds.height > wall_bounds.top+wall_bounds.height)
          && (guy_bounds.left+guy_bounds.width > wall_bounds.left+3) && (guy_bounds.left < wall_bounds.left+wall_bounds.width-3))
      {
        return true;
      }
      else
      {
        return false;
      }
    }

    bool Collision_R(sf::FloatRect wall_bounds, sf::FloatRect guy_bounds)
    {
      if ((guy_bounds.left <= wall_bounds.left) && (guy_bounds.left+guy_bounds.width > wall_bounds.left)
          && (guy_bounds.top+guy_bounds.height > wall_bounds.top+3) && (guy_bounds.top < wall_bounds.top+wall_bounds.height-3))
      {
        return true;
      }
      else
      {
        return false;
      }
    }


    void moveInDirection(const sf::Time &elapsed, const std::vector<sf::Sprite> &obstacles)
     {


      bool top=0, left=0, bottom=0, right=0;
      for(auto &obstacle : obstacles)
      {
      sf::FloatRect guy_bounds = getGlobalBounds();
      sf::FloatRect wall_bounds = obstacle.getGlobalBounds();
      if(Collision_T(wall_bounds, guy_bounds) == 1){top = 1;}
      if(Collision_L(wall_bounds, guy_bounds) == 1){left = 1;}
      if(Collision_B(wall_bounds, guy_bounds) == 1){bottom = 1;}
      if(Collision_R(wall_bounds, guy_bounds) == 1){right = 1;}
      }


      if (top==1)
      {
          move(0,-10);
      }
      if (bottom==1)
      {
          move(0,10);
      }
      if (left==1)
      {
          move(10,0);
      }
      if (right==1)
      {
          move(-10,0);
      }





     }












    int m_speed_x = 200;
    int m_speed_y = 200;
    int bound_top = 0;
    int bound_bottom = 0;
    int bound_left = 0;
    int bound_right = 0;
 };
void dontleave(CustomSprite& gosc, const sf::RenderWindow& window)
{
    sf::FloatRect guy_bounds = gosc.getGlobalBounds();

                  if(guy_bounds.top <= 0)
                  {
                      gosc.setPosition(100,200);
                  }

                  if(guy_bounds.top + guy_bounds.height >= window.getSize().y)
                  {
                      gosc.setPosition(100,200);
                  }

                  if(guy_bounds.left <= 0 )
                  {
                     gosc.setPosition(100,200);
                  }

                  if(guy_bounds.left + guy_bounds.width >= window.getSize().x)
                  {
                      gosc.setPosition(100,200);
                  }
}



int main()
{

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    sf::Clock clock;





    sf::Texture guy_tex;

    if (!guy_tex.loadFromFile("guy.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }
    CustomSprite guy;

    guy.setTexture(guy_tex);




    sf::Texture grass_tex;
    if (!grass_tex.loadFromFile("grass.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }

    sf::Sprite grass;
    grass.setTexture(grass_tex);
    grass.setScale(window.getSize().x / grass.getLocalBounds().width,
                   window.getSize().y / grass.getLocalBounds().height);



    sf::Texture wall_tex;

    if (!wall_tex.loadFromFile("wall.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }


    std::vector<sf::Sprite> walls;


    for (int i = 0; i < 6; i++)
    {

        sf::Sprite wall;
        wall.setTexture(wall_tex);
        walls.push_back(wall);
     }
    walls[0].setPosition(200,300);
    walls[1].setPosition(200,10);
    walls[2].setPosition(400,150);
    walls[3].setPosition(500,500);
    walls[4].setPosition(600,10);
    walls[5].setPosition(-100,400);

    int guy_speedx = 10000;
    int guy_speedy = 10000;




    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();

        float dt = elapsed.asSeconds();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }


            if (event.key.code == sf::Keyboard::Left)
            {
                guy.move(-1 * (guy_speedx) * dt, 0);
            }
            else if (event.key.code == sf::Keyboard::Right)
            {
                guy.move(guy_speedx * dt, 0);
            }
            else if (event.key.code == sf::Keyboard::Up)
            {
                guy.move(0, -1 * guy_speedy * dt);
            }
            else if (event.key.code == sf::Keyboard::Down)
            {
                guy.move(0, (guy_speedy) * dt);
            }
        }





        dontleave(guy,window);
        guy.setBounds(0, window.getSize().x, 0, window.getSize().y);
        guy.moveInDirection(elapsed, walls);

        window.clear(sf::Color::Black);
        window.draw(grass);
        window.draw(guy);

        for (auto& wall : walls)
        {
            window.draw(wall);
        }

        window.display();
    }



    return 0;
}
