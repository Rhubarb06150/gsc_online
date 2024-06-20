#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "../functions/hud.cpp"

class MOD_MiniMAP{
    public:
    std::string name;
    std::string author_name;
    std::string description;
    std::vector<int> cur_player_pos;
    HUD display;
    bool active;
    MOD_MiniMAP(){
        name="Mini Map";
        author_name="Rhubarb";
        description="A simple mini map test\nI hope you'll enjoy :)";
        active=true;
        display=HUD();
    };
    int init(){
        return 0;
    };
    int passVars(std::vector<int> player_pos){
        cur_player_pos=player_pos;
        return 0;
    };
    int act(){
        return 0;
    };
    int show(sf::RenderWindow& window){
        display.showTextDEBUG("x: "+std::to_string(cur_player_pos[0]),{0,0},window);
        display.showTextDEBUG("y: "+std::to_string(cur_player_pos[1]),{0,16},window);
        return 0;
    };
};