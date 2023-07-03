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
        setScale(0.1,0.1);
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

        setScale(0.1,0.1);
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

class monster :public sf::Sprite
{
private:
    sf::Texture texture_;
    sf::Vector2f playerPosition_;
    std::vector<sf::Texture> frames_;
    float frameTime_;
    float totalTime_;
    int currentFrame_;



public:
    float speed;
    monster(const std::vector<std::string>& framePaths, float frameTime)
            : frameTime_(frameTime), totalTime_(0), currentFrame_(0)
    {
                speed = 15 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(100-15)));
                if (!loadFramesFromTextures(framePaths)) {
                std::cerr << "Failed to load frames from textures" << std::endl;
            }
            setTexture(frames_[0]);
            setTextureRect(sf::IntRect(0, 0, frames_[0].getSize().x, frames_[0].getSize().y));
            setScale(0.3,0.3);



    }
    void setPlayerPosition(const sf::Vector2f& position)
        {
            playerPosition_ = position;
        }


    void followPlayer(float deltaTime, float speed)
    {
        sf::Vector2f currentPosition = getPosition();

        sf::Vector2f direction = playerPosition_ - currentPosition;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0) {
            direction /= length;
        }

        sf::Vector2f newPosition = currentPosition + direction * speed * deltaTime;
        setPosition(newPosition);
    }

    bool loadFramesFromTextures(const std::vector<std::string>& paths) {
            for (const std::string& path : paths) {
                sf::Texture frame;
                if (!frame.loadFromFile(path)) {
                    return false;
                }
                frames_.push_back(frame);
            }
            return true;
        }

        void animate(float deltaTime) {
            totalTime_ += deltaTime;
            if (totalTime_ >= frameTime_) {
                totalTime_ -= frameTime_;
                currentFrame_ = (currentFrame_ + 1) % frames_.size();
                setTexture(frames_[currentFrame_]);
            }
        }
        bool loadFramesFromTextures2(const std::vector<std::string>& paths) {
                for (const std::string& path : paths) {
                    sf::Texture frame;
                    if (!frame.loadFromFile(path)) {
                        return false;
                    }
                    frames_.push_back(frame);
                }
                return true;
            }

            void animate2(float deltaTime) {
                totalTime_ += deltaTime;
                if (totalTime_ >= frameTime_) {
                    totalTime_ -= frameTime_;
                    currentFrame_ = (currentFrame_ + 1) % frames_.size();
                    setTexture(frames_[currentFrame_]);
                }
            }


            void die_monster(const sf::Vector2f& playerPosition, float distance)
            {

                    const float PI = 3.14159265358979323846;
                    float angle = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 2 * PI;


                    float offsetX = std::cos(angle) * distance;
                    float offsetY = std::sin(angle) * distance;
                    sf::Vector2f newPosition = playerPosition + sf::Vector2f(offsetX, offsetY);

                    setPosition(newPosition);
            }



};

class serce:public sf::Sprite
{
private:
    sf::Texture texture_;
public:
    serce(const std::string& path)
    {

        if (!texture_.loadFromFile(path)) {
            std::cerr << "Could not load texture" << path << std::endl;
        }
        setTexture(texture_);

    }

};

int main()
{

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    sf::Clock clock;



    sf::Texture texture_wall;
    if(!texture_wall.loadFromFile("TILE_1G.png")) { return 1; }
    texture_wall.setRepeated(true);

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
    grass.setTexture(texture_wall);


    grass.setTexture(texture_wall);
    grass.setScale(1.5,1.5);
    grass.setTextureRect(sf::IntRect(0, 0, 1500, 1500));

    int Score=0;
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
    expl.setScale(0.1,0.1);
//    expl.add_animation_frame(sf::IntRect(30, 5, 90, 90));

    /*remember to change it to more effective*/
    std::vector<sf::Sprite> walls;
    std::vector<std::string> framePaths = {
           "go_1.png",
           "go_2.png",
           "go_3.png",
           "go_4.png",
           "go_5.png",
           "go_6.png",
            "go_7.png",
            "go_8.png",
        "go_9.png",
        "go_10.png"
       };

    std::vector<std::string> framePaths1 = {
            "die_1.png",
            "die_2.png",
            "die_3.png",
            "die_4.png",
            "die_5.png",
            "die_6.png"
       };



    monster Monster1(framePaths, 0.2f);
    monster Monster2(framePaths, 0.2f);
    monster Monster3(framePaths, 0.2f);
    monster Monster4(framePaths, 0.2f);
    monster Monster5(framePaths, 0.2f);
    monster Monster6(framePaths, 0.2f);
    monster Monster7(framePaths, 0.2f);
    monster Monster8(framePaths, 0.2f);

    monster monsterdead(framePaths1, 0.2f);







    Monster1.setPosition(10,10);
    Monster2.setPosition(10,210);
    Monster3.setPosition(10,510);
    Monster4.setPosition(700,10);
    Monster5.setPosition(700,210);
    Monster6.setPosition(700,410);


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

    std::vector<monster> zombies;
    zombies.push_back(Monster1);
    zombies.push_back(Monster2);
    zombies.push_back(Monster3);
    zombies.push_back(Monster4);
    zombies.push_back(Monster5);
    zombies.push_back(Monster6);
    //zombies.push_back(Monster7);





    const float safeSpace = 200.0f;


    sf::Vector2f characterPosition = guy.getPosition();
    sf::FloatRect characterBounds = guy.getGlobalBounds();


    sf::Vector2u windowSize = window.getSize();






    bool IsSpriteAdded=false;
    bool IsSpriteAdded2=true;
    bool IsSpriteAdded3=false;
    bool IsSpriteAdded4=false;
    int zycia=0;
    bool intersectionOccurred = false;
    bool is_dead= false;

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Error handling - Failed to load the font
        // Make sure the font file name and extension are correct
    }
    sf::Text scoreText;

    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(300,10);

    sf::Text ExitText;

    ExitText.setFont(font);
    ExitText.setCharacterSize(15);
    ExitText.setFillColor(sf::Color::White);
    ExitText.setPosition(20,10);

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
            expl.setPosition(guy.getPosition()+sf::Vector2f(-80 ,-100));
            expl.setScale(0,0);
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
//            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num2 && !IsSpriteAdded2)
//            {
//                IsSpriteAdded2=true;
//            }
        }
        sf::FloatRect guy_bounds = guy.getGlobalBounds();

        bool currentIntersection = false;

            for (auto& monster : zombies)
            {
                sf::FloatRect monster_bounds = monster.getGlobalBounds();
                if (monster_bounds.intersects(guy_bounds))
                {
                    currentIntersection = true;
                    break;
                }
            }

            if (currentIntersection && !intersectionOccurred)
            {
                intersectionOccurred = true;

                if (zycia < 5)
                {
                    serca[zycia].setScale(0, 0);
                    zycia++;
                }

                if (zycia == 5)
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



        for (auto& monster : zombies) {
                monster.setPlayerPosition(guy.getPosition());
                monster.followPlayer(dt, monster.speed);
                monster.animate(dt);
                window.draw(monster);
            }

        for (auto &serce: serca)
        {
            window.draw(serce);


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

        /*if(IsSpriteAdded3)
        {
            window.draw(fireb);
        }*/

        if(IsSpriteAdded4)
        {
            expl.animate(elapsed);
            window.draw(expl);
        }

        sf::FloatRect expl_bounds = expl.getGlobalBounds();
        sf::Vector2f position;
        sf::FloatRect knife_bounds = knife.getGlobalBounds();
         sf::FloatRect axe_bounds = axe.getGlobalBounds();
        for (auto& monster : zombies)
        {


            sf::FloatRect monster_bounds = monster.getGlobalBounds();

            if (monster_bounds.intersects(expl_bounds))
            {

                Score += 10;

                monster.die_monster(guy.getPosition(),400);


            }

             if (monster_bounds.intersects(knife_bounds))
             {
                 Score += 20;
                 monster.die_monster(guy.getPosition(),400);

             }

             if (monster_bounds.intersects(axe_bounds))
             {

                 monster.die_monster(guy.getPosition(),400);
                 Score += 50;
             }
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
