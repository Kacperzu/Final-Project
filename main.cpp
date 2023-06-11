#include <iostream>
#include <vector>
#include <random>
#include <cmath>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int speed_x=0;
int speed_y=0;
int speed_x_fb=0;
int speed_y_fb=0;

class CustomSprite : public sf::Sprite
{
public:
    CustomSprite(const std::string& path) {

        if (!texture_.loadFromFile(path)) {
            std::cerr << "Could not load texture" << path << std::endl;
        }
        setTexture(texture_);
        setTextureRect(sf::IntRect(0, 0, 35, 37));

        setScale(2,2);
        setOrigin(sf::Vector2f(25,25));
        setPosition(390, 290);

    }
    void setBounds(int left, int right, int top, int bottom)
    {
        bound_top = top;
        bound_bottom = bottom;
        bound_right = right;
        bound_left = left;
    }

    bool Collision_T(sf::FloatRect wall_bounds, sf::FloatRect guy_bounds)
    {
        if ((guy_bounds.top+guy_bounds.height >= wall_bounds.top-3)
            && (guy_bounds.top < wall_bounds.top)
            && (guy_bounds.left+guy_bounds.width > wall_bounds.left+3)
            && (guy_bounds.left < wall_bounds.left+wall_bounds.width-3))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    bool Collision_L(sf::FloatRect wall_bounds, sf::FloatRect guy_bounds)
    {
        if ((guy_bounds.left+guy_bounds.width >= wall_bounds.left-3)
            && (guy_bounds.left < wall_bounds.left)
            && (guy_bounds.top+guy_bounds.height > wall_bounds.top+3)
            && (guy_bounds.top < wall_bounds.top+wall_bounds.height-3))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    bool Collision_B(sf::FloatRect wall_bounds, sf::FloatRect guy_bounds)
    {
        if ((guy_bounds.top >= wall_bounds.top+wall_bounds.height-3)
            && (guy_bounds.top < wall_bounds.top+wall_bounds.height)
            && (guy_bounds.left+guy_bounds.width > wall_bounds.left+3)
            && (guy_bounds.left < wall_bounds.left+wall_bounds.width-3))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    bool Collision_R(sf::FloatRect wall_bounds, sf::FloatRect guy_bounds)
    {
        if ((guy_bounds.left >= wall_bounds.left+wall_bounds.width-3)
            && (guy_bounds.left < wall_bounds.left+wall_bounds.width)
            && (guy_bounds.top+guy_bounds.height > wall_bounds.top+3)
            && (guy_bounds.top < wall_bounds.top+wall_bounds.height-3))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    void animate(const sf::Time &elapsed){
        // TODO:  t_ is equal to t_ + dt
        float dt = elapsed.asSeconds();
        t_=t_+dt;

        // TODO:  if t_ became bigger than duration between two frames, then fragments_index would be added by one
        if(t_>0.1){
            fragments_index++;
            t_=0;
        }

        // TODO:  if fragments_index reached the end of the vector, fragments_index becaomes zero
        if(fragments_index>=rectVector.size()){
            fragments_index=0;
        }

        // TODO:  now setTextureRect by rectangle of the current index of the vector of rectangles
        setTextureRect(rectVector[fragments_index]);
    }

    void add_animation_frame(const sf::IntRect& frame){

        // TODO:  add the rectnagle frame to the vector of rectangle frames
        rectVector.push_back(frame);

    }

    void animate_stand(const sf::Time &elapsed){

        float dt = elapsed.asSeconds();
        t__stand=t__stand+dt;

        if(t__stand>0.3){
            fragments_index_stand++;
            t__stand=0;
        }
        if(fragments_index_stand>=rectVector_stand.size()){
            fragments_index_stand=0;
        }

        setTextureRect(rectVector_stand[fragments_index_stand]);
    }

    void add_animation_frame_stand(const sf::IntRect& frame){
        rectVector_stand.push_back(frame);

    }

    void moveInDirection(const sf::Time &elapsed, const std::vector<sf::Sprite> &obstacles)
    {

        bool top=0, left=0, bottom=0, right=0;
        for(auto &obstacle : obstacles)
        {
            sf::FloatRect rectangle_bounds = getGlobalBounds();
            sf::FloatRect wall_bounds = obstacle.getGlobalBounds();
            if(Collision_T(wall_bounds, rectangle_bounds) == 1){
                top = 1;
            }
            if(Collision_L(wall_bounds, rectangle_bounds) == 1){
                left = 1;
            }
            if(Collision_B(wall_bounds, rectangle_bounds) == 1){
                bottom = 1;
            }
            if(Collision_R(wall_bounds, rectangle_bounds) == 1){
                right = 1;
            }
        }

        float dt = elapsed.asSeconds();

        sf::FloatRect rectangle_bounds = getGlobalBounds();
        if((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down) == false)
            && rectangle_bounds.left > bound_left && right != true){
            animate_stand(elapsed);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
            && rectangle_bounds.top > bound_top && bottom != true)
        {
            move(m_speed_x*0, m_speed_y*-dt);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
            && rectangle_bounds.height + rectangle_bounds.top <= bound_bottom && top != true)
        {
            move(m_speed_x*0, m_speed_y*dt);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
            && rectangle_bounds.left > bound_left && right != true)
        {
            move(m_speed_x*-dt, m_speed_y*0);
            animate(elapsed);
            setScale(-2,2);

        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
            && rectangle_bounds.width + rectangle_bounds.left <= bound_right && left != true)
        {
            move(m_speed_x*dt, m_speed_y*0);
            animate(elapsed);
            setScale(2,2);
        }
    }

private:
    int m_speed_x = 200;
    int m_speed_y = 200;
    int bound_top = 0;
    int bound_bottom = 0;
    int bound_left = 0;
    int bound_right = 0;
    float t_=0.0;
    float t__stand=0.0;
    unsigned int fragments_index = 0;
    unsigned int fragments_index_stand = 0;
    sf::Texture texture_;
    std::vector<sf::IntRect> rectVector;
    std::vector<sf::IntRect> rectVector_stand;


};

class WeaponSprite : public sf::Sprite
{
public:
    WeaponSprite(const std::string& path) {

        if (!texture_.loadFromFile(path)) {
            std::cerr << "Could not load texture" << path << std::endl;
        }
        setTexture(texture_);
//        setOrigin(0,500);
        setScale(0.15,-0.15);
    }
    void MoveIt(const sf::Time &elapsed){
        float dt = elapsed.asSeconds();
        move(speed_x*dt, speed_y*dt);
    }
private:
    sf::Texture texture_;
};
class AxeSprite : public sf::Sprite
{
public:
    AxeSprite(const std::string& path) {

        if (!texture_.loadFromFile(path)) {
            std::cerr << "Could not load texture" << path << std::endl;
        }
        setOrigin(-400,-400);
        setTexture(texture_);
        setScale(0.2, 0.2);
    }
private:
    sf::Texture texture_;
};

class FireballSprite : public sf::Sprite
{
public:
    FireballSprite(const std::string& path) {

        if (!texture_.loadFromFile(path)) {
            std::cerr << "Could not load texture" << path << std::endl;
        }
        setTexture(texture_);
        setTextureRect(sf::IntRect(0, 0, 0, 0));
    }
    void animate(const sf::Time &elapsed){
        float dt = elapsed.asSeconds();
        t_=t_+dt;

        if(t_>0.09){
            fragments_index++;
            t_=0;
        }
        if(fragments_index>=rectVector.size()){
            fragments_index=0;
        }
        setTextureRect(rectVector[fragments_index]);
    }

    void add_animation_frame(const sf::IntRect& frame){

        rectVector.push_back(frame);
    }

    void MoveIt(const sf::Time &elapsed){
        float dt = elapsed.asSeconds();
        move(speed_x_fb*dt, speed_y_fb*dt);
    }
private:
    float t_=0.0;
    sf::Texture texture_;
    std::vector<sf::IntRect> rectVector;
    unsigned int fragments_index = 0;
};

class ExplosionSprite : public sf::Sprite
{
public:
    ExplosionSprite(const std::string& path) {

        if (!texture_.loadFromFile(path)) {
            std::cerr << "Could not load texture" << path << std::endl;
        }
        setTexture(texture_);
        setTextureRect(sf::IntRect(0, 0, 0, 0));
    }
    void animate(const sf::Time &elapsed){
        float dt = elapsed.asSeconds();
        t_=t_+dt;

        if(t_>0.09){
            fragments_index++;
            t_=0;
        }
        if(fragments_index>=rectVector.size()){
            fragments_index=0;
        }
        setTextureRect(rectVector[fragments_index]);
    }

    void add_animation_frame(const sf::IntRect& frame){

        rectVector.push_back(frame);
    }
private:
    float t_=0.0;
    sf::Texture texture_;
    std::vector<sf::IntRect> rectVector;
    unsigned int fragments_index = 0;
};

int main()
{

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    sf::Clock clock;

    sf::Texture grass_tex;
    if (!grass_tex.loadFromFile("grass.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }

    sf::Texture wall_tex;
    if (!wall_tex.loadFromFile("wall.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }

    CustomSprite guy("character.png");

    guy.add_animation_frame_stand(sf::IntRect(0, 0, 50, 37)); // hero standing frame 1
    guy.add_animation_frame_stand(sf::IntRect(50, 0, 50, 37)); // hero standing frame 2
    guy.add_animation_frame_stand(sf::IntRect(100, 0, 50, 37)); // hero standing frame 3
    guy.add_animation_frame(sf::IntRect(150, 0, 37, 37)); // hero running frame 1
    guy.add_animation_frame(sf::IntRect(200, 0, 37, 37)); // hero running frame 1
    guy.add_animation_frame(sf::IntRect(250, 0, 37, 37)); // hero running frame 1
    guy.add_animation_frame(sf::IntRect(300, 0, 37, 37)); // hero running frame 1
    guy.add_animation_frame(sf::IntRect(350, 0, 37, 37)); // hero running frame 1
    guy.add_animation_frame(sf::IntRect(400, 0, 37, 37)); // hero running frame 1

    sf::Sprite grass;
    grass.setTexture(grass_tex);

    grass.setScale(
        (float)window.getSize().x / grass_tex.getSize().x,
        (float)window.getSize().y / grass_tex.getSize().y);

    WeaponSprite knife("knife.png");

    AxeSprite axe("axe.png");

    FireballSprite fireb("fireball.png");
    fireb.add_animation_frame(sf::IntRect(9, 275, 56, 25));
    fireb.add_animation_frame(sf::IntRect(71, 275, 57, 25));
    fireb.add_animation_frame(sf::IntRect(135, 275, 57, 25));
    fireb.add_animation_frame(sf::IntRect(194, 275, 61, 25));
    fireb.add_animation_frame(sf::IntRect(266, 275, 53, 25));
    fireb.add_animation_frame(sf::IntRect(325, 275, 58, 25));
    fireb.add_animation_frame(sf::IntRect(392, 275, 55, 25));
    fireb.add_animation_frame(sf::IntRect(454, 275, 58, 25));

    ExplosionSprite expl("explosion.png");
    expl.add_animation_frame(sf::IntRect(30, 5, 70, 90));
    expl.add_animation_frame(sf::IntRect(206, 5, 66, 90));
    expl.add_animation_frame(sf::IntRect(298, 5, 72, 90));
    expl.add_animation_frame(sf::IntRect(389, 5, 84, 90));
    expl.add_animation_frame(sf::IntRect(485, 5, 86, 90));
    expl.add_animation_frame(sf::IntRect(577, 5, 89, 90));
    expl.add_animation_frame(sf::IntRect(674, 5, 88, 90));
    expl.add_animation_frame(sf::IntRect(768, 5, 92, 90));
    expl.add_animation_frame(sf::IntRect(864, 5, 88, 90));
    expl.add_animation_frame(sf::IntRect(956, 5, 94, 90));
    expl.add_animation_frame(sf::IntRect(1055, 5, 89, 90));
//    expl.add_animation_frame(sf::IntRect(30, 5, 90, 90));

    /*remember to change it to more effective*/
    std::vector<sf::Sprite> walls;

    sf::Sprite wall1;
    wall1.setTexture(wall_tex);
    wall1.setScale(0.4, 0.4);
    wall1.setPosition(100, 100);

    sf::Sprite wall2;
    wall2.setTexture(wall_tex);
    wall2.setScale(0.4, 0.4);
    wall2.setPosition(100, 180);

    sf::Sprite wall3;
    wall3.setTexture(wall_tex);
    wall3.setScale(0.4, 0.4);
    wall3.setPosition(100, 260);

    sf::Sprite wall4;
    wall4.setTexture(wall_tex);
    wall4.setScale(0.4, 0.4);
    wall4.setPosition(180, 260);

    sf::Sprite wall5;
    wall5.setTexture(wall_tex);
    wall5.setScale(0.4, 0.4);
    wall5.setPosition(580, 260);

    sf::Sprite wall6;
    wall6.setTexture(wall_tex);
    wall6.setScale(0.4, 0.4);
    wall6.setPosition(580, 340);

    walls.push_back(wall1);
    walls.push_back(wall2);
    walls.push_back(wall3);
    walls.push_back(wall4);
    walls.push_back(wall5);
    walls.push_back(wall6);

    bool IsSpriteAdded=false;
    bool IsSpriteAdded2=false;
    bool IsSpriteAdded3=false;
    bool IsSpriteAdded4=false;

    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();

        float dt=elapsed.asSeconds();
        float counter=counter+dt;
        float counter2=counter2+dt;

        if(counter>2.0 && !IsSpriteAdded3){
            IsSpriteAdded3=true;
            counter=0;
            fireb.setPosition(guy.getPosition()+sf::Vector2f(10,-65));
            fireb.setScale(3,3);
            speed_x_fb=750;
            speed_y_fb=0;
        }
        if(counter2>3.0 && !IsSpriteAdded4){
            IsSpriteAdded4=true;
            expl.setPosition(guy.getPosition()+sf::Vector2f(-120,-200));
            expl.setScale(3,3);
        }
        if(counter2>3.99 && IsSpriteAdded4==true){
            IsSpriteAdded4=false;
            counter2=0;
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                std::cout<<"Closing Window"<<std::endl;
                window.close();
            }
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                std::cout<<"Closing Window"<<std::endl;
                std::cout<<"Flag state:"<<IsSpriteAdded<<std::endl;
                window.close();
            }
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W && !IsSpriteAdded)
            {
                IsSpriteAdded=true;
                knife.setPosition(guy.getPosition()+sf::Vector2f(-45,-50));
                knife.setScale(0.15,-0.15);
                knife.setRotation(-314);
                speed_y=-750;
                speed_x=0;
            }
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S && !IsSpriteAdded)
            {
                IsSpriteAdded=true;
                knife.setPosition(guy.getPosition()+sf::Vector2f(40,15));
                knife.setScale(0.15,-0.15);
                knife.setRotation(225);
                speed_y=750;
                speed_x=0;

            }
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A && !IsSpriteAdded)
            {
                IsSpriteAdded=true;
                knife.setPosition(guy.getPosition()+sf::Vector2f(-20,-65));
                knife.setScale(0.15, 0.15);
                knife.setRotation(40);
                speed_x=-750;
                speed_y=0;

            }
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D && !IsSpriteAdded)
            {
                IsSpriteAdded=true;
                knife.setPosition(guy.getPosition()+sf::Vector2f(10,-65));
                knife.setScale(0.15,-0.15);
                knife.setRotation(130);
                speed_x=750;
                speed_y=0;

            }
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num2 && !IsSpriteAdded2)
            {
                IsSpriteAdded2=true;
            }
        }


        sf::Vector2f knife_pos = knife.getPosition();
        sf::Vector2u window_pos = window.getSize();
        if(knife_pos.x < 0.f || knife_pos.y < 0.f ||
            knife_pos.x > window_pos.x || knife_pos.y > window_pos.y){
            IsSpriteAdded=false;
        }
        sf::Vector2f fireb_pos = fireb.getPosition();
        if(fireb_pos.x < 0.f || fireb_pos.y < 0.f ||
            fireb_pos.x > window_pos.x || fireb_pos.y > window_pos.y){
            IsSpriteAdded3=false;
        }

        guy.setBounds(0, window.getSize().x, 0, window.getSize().y);
        guy.moveInDirection(elapsed, walls);
        knife.MoveIt(elapsed);
        fireb.MoveIt(elapsed);
        fireb.animate(elapsed);

//        axe.setPosition(guy.getPosition());

        window.clear(sf::Color::Black);
        window.draw(grass);
        window.draw(guy);

        for (auto &wall : walls){
            window.draw(wall);
        }

        if(IsSpriteAdded)
        {
            window.draw(knife);
        }
        if(IsSpriteAdded2)
        {
            axe.setPosition(guy.getPosition());
            window.draw(axe);
            axe.rotate(0.01);
        }
        if(IsSpriteAdded3)
        {
            window.draw(fireb);
        }
        if(IsSpriteAdded4)
        {
            expl.animate(elapsed);
            window.draw(expl);
        }

        window.display();
    }

    return 0;
}
