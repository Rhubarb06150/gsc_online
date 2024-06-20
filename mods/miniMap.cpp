#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "../functions/hud.cpp"

class MOD_MiniMAP{
    private:
    HUD display;
    public:
    std::string name;
    std::string author_name;
    std::string description;
    std::vector<int> cur_player_pos;
    std::vector<std::vector<std::string>> cur_terrain_vector;
    
    bool active;
    MOD_MiniMAP(){
        name="Mini Map";
        author_name="Rhubarb";
        description="A simple mini map test\nI hope you'll enjoy :)";
        active=true;
    };
    int init(){
        return 0;
    };
    int passVars(std::vector<int> player_pos, std::vector<std::vector<std::string>> terrain_vector){
        cur_player_pos=player_pos;
        cur_terrain_vector=terrain_vector;
        return 0;
    };
    int act(){
        return 0;
    };
    int show(sf::RenderWindow& window){
        sf::RectangleShape rectangle(sf::Vector2f(128,128));
        rectangle.setFillColor(sf::Color(0,0,0));
        rectangle.setPosition({0,0});
        window.draw(rectangle);
        return 0;
    };
};