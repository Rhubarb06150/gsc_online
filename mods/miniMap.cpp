#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "../functions/hud.cpp"
#include "../functions/tiles.cpp"

class MOD_MiniMAP{
    private:
    HUD display;
    TilesIndex tiles;
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
        for (int height=0;height<cur_terrain_vector.size();height++){
            for (int width=0;width<cur_terrain_vector[0].size();width++){
                sf::Sprite sprite;
                sprite.setTexture(tiles.textures[0][0]);
                sprite.setScale(0.125f,0.125f);
                sprite.setPosition(width,height);
                window.draw(sprite);
            };
        };
        return 0;
    };
};