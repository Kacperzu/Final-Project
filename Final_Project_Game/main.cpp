#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <cstdlib>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int speed_x=0;
int speed_y=0;
int speed_x_fb=0;
int speed_y_fb=0;
int zombie_amount=1;

class CustomSprite : public sf::Sprite
{
public:
    CustomSprite(const std::string& path) {

        if (!texture_.loadFromFile(path)) {
            std::cerr << "Could not load texture" << path << std::endl;
        }
        setTexture(texture_);
        setTextureRect(sf::IntRect(0, 0, 0, 0));
        setScale(2,2);
        setOrigin(sf::Vector2f(15,15));
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

    void animate(const sf::Time &elapsed)
    {
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

    void add_animation_frame(const sf::IntRect& frame)
    {
        // TODO:  add the rectnagle frame to the vector of rectangle frames
        rectVector.push_back(frame);
    }

    void animate_stand(const sf::Time &elapsed)
    {
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

    void add_animation_frame_stand(const sf::IntRect& frame)
    {
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
            animate(elapsed);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
            && rectangle_bounds.height + rectangle_bounds.top <= bound_bottom && top != true)
        {
            move(m_speed_x*0, m_speed_y*dt);
            animate(elapsed);
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

        if (!texture_.loadFromFile(path)){
            std::cerr << "Could not load texture" << path << std::endl;
        }
        setTexture(texture_);
        setScale(0.15,-0.15);
    }
    void MoveIt(const sf::Time &elapsed)
    {
        float dt = elapsed.asSeconds();
        move(speed_x*dt, speed_y*dt);
    }
private:
    sf::Texture texture_;
};

class AxeSprite : public sf::Sprite
{
public:
    AxeSprite(const std::string& path){

        if (!texture_.loadFromFile(path)){
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
    FireballSprite(const std::string& path){

        if (!texture_.loadFromFile(path)){
            std::cerr << "Could not load texture" << path << std::endl;
        }
        setTexture(texture_);
        setTextureRect(sf::IntRect(0, 0, 0, 0));
        setScale(0, 0);
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

class MonsterSprite : public sf::Sprite
{
public:
    MonsterSprite(const std::string& path){

        if (!texture_.loadFromFile(path)) {
            std::cerr << "Could not load texture" << path << std::endl;
        }
        setTexture(texture_);
        setTextureRect(sf::IntRect(0, 0, 0, 0));
        setScale(3,3);
        setOrigin(10,10);
    }
    void animate(const sf::Time &elapsed)
    {
        float dt = elapsed.asSeconds();
        t_=t_+dt;

        if(t_>0.1){
            fragments_index++;
            t_=0;
        }

        if(fragments_index>=rectVector.size()){
            fragments_index=0;
        }

        setTextureRect(rectVector[fragments_index]);
    }

    void add_animation_frame(const sf::IntRect& frame)
    {
        rectVector.push_back(frame);
    }

    void setPlayerPosition(const sf::Vector2f& position)
    {
        playerPosition_ = position;
    }

    void followPlayer(float deltaTime)
    {
        sf::Vector2f currentPosition = getPosition();
        sf::Vector2f direction = playerPosition_ - currentPosition - sf::Vector2f(33, 42);
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0) {
            direction /= length;
        }
        if(direction.x<0){
            setScale(-3,3);
        }
        if(direction.x>0){
            setScale(3,3);
        }

        sf::Vector2f newPosition = currentPosition + direction * speed * deltaTime;
        setPosition(newPosition);
    }

    void die_monster(const sf::Vector2f& playerPosition, float distance)
    {
        float angle = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 2 * M_PI;
        float offsetX = std::cos(angle) * distance;
        float offsetY = std::sin(angle) * distance;
        sf::Vector2f newPosition = playerPosition + sf::Vector2f(offsetX, offsetY);
        setPosition(newPosition);
    }

private:
    float speed = 50 + static_cast<float>(rand())/( static_cast <float> (RAND_MAX/(100-15)));
    float t_=0.0;
    unsigned int fragments_index = 0;
    sf::Vector2f playerPosition_;
    sf::Texture texture_;
    std::vector<sf::IntRect> rectVector;
};

class serce : public sf::Sprite
{

public:
    serce(const std::string& path)
    {

        if (!texture_.loadFromFile(path)) {
            std::cerr << "Could not load texture" << path << std::endl;
        }
        setTexture(texture_);
    }
private:
    sf::Texture texture_;
};

void displayBoundingBox(const sf::Sprite& sprite, sf::RenderWindow& window)
{
    sf::FloatRect bounds = sprite.getGlobalBounds();

    sf::RectangleShape boundingBox;
    boundingBox.setSize(sf::Vector2f(bounds.width, bounds.height));
    boundingBox.setOutlineThickness(2.f);
    boundingBox.setOutlineColor(sf::Color::Red);
    boundingBox.setFillColor(sf::Color::Transparent);
    boundingBox.setPosition(bounds.left, bounds.top);

    window.draw(boundingBox);
}

int main()
{
    int Score=0;
    bool intersectionOccurred = false;

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    sf::Clock clock;

    sf::Texture texture_wall;
    if(!texture_wall.loadFromFile("TILE_1G.png")) { return 1; }
    texture_wall.setRepeated(true);

    CustomSprite guy("character.png");

    guy.add_animation_frame_stand(sf::IntRect(15, 6, 22, 30)); // hero standing frame 1
    guy.add_animation_frame_stand(sf::IntRect(65, 6, 22, 30)); // hero standing frame 2
    guy.add_animation_frame_stand(sf::IntRect(115, 6, 22, 30)); // hero standing frame 3
    guy.add_animation_frame(sf::IntRect(165, 6, 22, 30)); // hero running frame 1
    guy.add_animation_frame(sf::IntRect(215, 6, 22, 30)); // hero running frame 1
    guy.add_animation_frame(sf::IntRect(265, 6, 22, 30)); // hero running frame 1
    guy.add_animation_frame(sf::IntRect(315, 6, 22, 30)); // hero running frame 1
    guy.add_animation_frame(sf::IntRect(365, 6, 22, 30)); // hero running frame 1
    guy.add_animation_frame(sf::IntRect(415, 6, 22, 30)); // hero running frame 1

    sf::Sprite grass;
    grass.setTexture(texture_wall);

    grass.setTexture(texture_wall);
    grass.setScale(1.5,1.5);
    grass.setTextureRect(sf::IntRect(0, 0, 1500, 1500));

    WeaponSprite knife("knife.png");

    AxeSprite axe("axe.png");

    FireballSprite fireb("fireball.png");
    fireb.add_animation_frame(sf::IntRect(40, 68, 400, 400));
    fireb.add_animation_frame(sf::IntRect(540, 68, 400, 400));
    fireb.add_animation_frame(sf::IntRect(1040, 68, 400, 400));
    fireb.add_animation_frame(sf::IntRect(1540, 68, 400, 400));
    fireb.add_animation_frame(sf::IntRect(2040, 68, 400, 400));
    fireb.add_animation_frame(sf::IntRect(2540, 68, 400, 400));

    MonsterSprite zombie("monster.png");
    zombie.add_animation_frame(sf::IntRect(5, 70, 26, 29));
    zombie.add_animation_frame(sf::IntRect(35, 70, 26, 29));
    zombie.add_animation_frame(sf::IntRect(66, 70, 26, 29));
    zombie.add_animation_frame(sf::IntRect(100, 70, 26, 29));
    zombie.add_animation_frame(sf::IntRect(133, 70, 26, 29));
    zombie.add_animation_frame(sf::IntRect(166, 70, 26, 29));
    zombie.add_animation_frame(sf::IntRect(196, 70, 26, 29));

    std::vector<MonsterSprite> sprites;

//    for(int i=0; i<zombie_amount; i++){
//        sprites.push_back(zombie);
//    }

    std::vector<sf::Sprite> walls;

    serce serce1("heart.png");
    serce serce2("heart.png");
    serce serce3("heart.png");
    serce serce4("heart.png");
    serce serce5("heart.png");

    serce1.setPosition(600, 10);
    serce2.setPosition(630, 10);
    serce3.setPosition(660, 10);
    serce4.setPosition(690, 10);
    serce5.setPosition(720, 10);
    std::vector<serce> serca;

    serca.push_back(serce1);
    serca.push_back(serce2);
    serca.push_back(serce3);
    serca.push_back(serce4);
    serca.push_back(serce5);

    bool IsKnife=false;
    bool IsAxe=true;
    bool IsFireb=false;
    int lives=0;

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Error handling - Failed to load the font
        // Make sure the font file name and extension are correct
    }
    sf::Text scoreText;

    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(340,10);

    sf::Text ExitText;

    ExitText.setFont(font);
    ExitText.setCharacterSize(15);
    ExitText.setFillColor(sf::Color::White);
    ExitText.setPosition(20,10);

    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();

        float dt=elapsed.asSeconds();
        float counter_fireb=counter_fireb+dt;
        float counter_knife=counter_knife+dt;
        float counter=counter+dt;

        if(counter>1.0){
            sprites.push_back(zombie);
            zombie.die_monster(guy.getPosition(),600);
            counter=0;
        }

        if(counter_fireb>5.0 && !IsFireb){
            fireb.setScale(0.25, 0.25);
            float angle=static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 2 * M_PI;
            IsFireb=true;
            counter_fireb=0;
            fireb.setPosition(guy.getPosition());
            speed_x_fb=250*cos(angle);
            speed_y_fb=250*sin(angle);
            float rotation = std::atan2(sin(angle), cos(angle)) * 180.f / static_cast<float>(M_PI);
            fireb.setRotation(rotation);
        }
        if(counter_knife>0.1 && !IsKnife){
            float angle=static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 2 * M_PI;
            IsKnife=true;
            counter_knife=0;
            knife.setPosition(guy.getPosition());
            speed_x=1000*cos(angle);
            speed_y=1000*sin(angle);
            float rotation = std::atan2(sin(angle), cos(angle)) * 180.f / static_cast<float>(M_PI);
            knife.setRotation(rotation);
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
                std::cout<<"Flag state:"<<IsKnife<<std::endl;
                window.close();
            }
        }

        sf::FloatRect guy_bounds = guy.getGlobalBounds();
        bool currentIntersection = false;
        float counter_lives=counter_lives+dt;

        sf::Vector2f position;
        sf::FloatRect knife_bounds = knife.getGlobalBounds();
        sf::FloatRect axe_bounds = axe.getGlobalBounds();
        sf::FloatRect fireball_bounds = fireb.getGlobalBounds();


        sf::Vector2f knife_pos = knife.getPosition();
        sf::Vector2u window_pos = window.getSize();
        if(knife_pos.x < 0.f || knife_pos.y < 0.f ||
            knife_pos.x > window_pos.x || knife_pos.y > window_pos.y){
            IsKnife=false;
        }
        sf::Vector2f fireb_pos = fireb.getPosition();
        if(fireb_pos.x < 0.f || fireb_pos.y < 0.f ||
            fireb_pos.x > window_pos.x || fireb_pos.y > window_pos.y){
            IsFireb=false;
        }

        guy.setBounds(0, window.getSize().x, 0, window.getSize().y);
        guy.moveInDirection(elapsed, walls);
        knife.MoveIt(elapsed);
        fireb.MoveIt(elapsed);
        fireb.animate(elapsed);

        window.clear(sf::Color::Black);
        window.draw(grass);
        window.draw(guy);

        for (auto &serce: serca)
        {
            window.draw(serce);
        }

        if(IsKnife)
        {
            window.draw(knife);
            displayBoundingBox(knife, window);
        }
        if(IsAxe)
        {
            axe.setPosition(guy.getPosition());
            window.draw(axe);
            axe.rotate(0.01);
        }

        if(IsFireb)
        {
            window.draw(fireb);
        }

        displayBoundingBox(guy, window);
        displayBoundingBox(fireb, window);
        displayBoundingBox(axe, window);

        for(auto& sprite : sprites){
            sf::FloatRect monster_bounds = sprite.getGlobalBounds();
            if (monster_bounds.intersects(guy_bounds) && counter_lives>1.0)
            {
                currentIntersection = true;
                counter_lives=0;
            }

            if (currentIntersection && !intersectionOccurred)
            {
                intersectionOccurred = true;

                if (lives < 5)
                {
                    serca[lives].setScale(0, 0);
                    lives++;
                }

                if (lives == 5)
                {
                    std::cout << "Your score is: " << Score << " ";
                    // Print the score or perform any other desired actions
                    window.close();
                }
            }
            else if (!currentIntersection)
            {
                intersectionOccurred = false;
            }
            if (monster_bounds.intersects(knife_bounds) && IsKnife==true)
            {
                Score += 20;
                sprite.die_monster(guy.getPosition(),400);
                IsKnife = false;
            }

            if (monster_bounds.intersects(fireball_bounds))
            {
                Score += 20;
                sprite.die_monster(guy.getPosition(),400);
            }

            if (monster_bounds.intersects(axe_bounds))
            {
                sprite.die_monster(guy.getPosition(),400);
                Score += 50;
            }
            sprite.setPlayerPosition(guy.getPosition());
            sprite.followPlayer(dt);
            sprite.animate(elapsed);
            displayBoundingBox(sprite, window); //hitboxes of zombies
            window.draw(sprite);
        }

        std::string scoreString = "Score: " + std::to_string(Score);
        std::string exitString = "Press Esc to exit";

        scoreText.setString(scoreString);
        window.draw(scoreText);

        ExitText.setString(exitString);
        window.draw(ExitText);
        window.display();
    }

    return 0;
}
