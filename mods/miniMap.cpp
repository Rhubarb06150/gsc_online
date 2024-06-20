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
    float zoom;
    std::vector<std::vector<std::string>> cur_terrain_vector;
    bool active;
    MOD_MiniMAP(){
        name="Mini Map";
        author_name="Rhubarb";
        description="A simple mini map test\nI hope you'll enjoy :)";
        active=true;
        zoom=1.0f;
    };
    int init(){
        return 0;
    };
    int passVars(std::vector<int> player_pos, std::vector<std::vector<std::string>> terrain_vector,sf::Event event){
        cur_player_pos=player_pos;
        cur_terrain_vector=terrain_vector;
        return 0;
    };
    int act(sf::RenderWindow& window,sf::Event event){
        while (window.pollEvent(event))
        {
            if(event.type==sf::Event::KeyPressed){
                if (event.key.code==sf::Keyboard::U){
                    if (zoom==0.5f){
                        zoom=1.0f;
                    }else if(zoom==0.25f){
                        zoom=0.5f;
                    }else if (zoom==0.125f){
                        zoom=0.25f;
                    };
                };
                if (event.key.code==sf::Keyboard::D){
                    if (zoom==1.0f){
                        zoom=0.5f;
                    }else if(zoom==0.5f){
                        zoom=0.25f;
                    }else if (zoom==0.25f){
                        zoom=0.125f;
                    };
                };
            };
        };
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
                sprite.setTexture(tiles.textures[0][tiles.getIntIndex(cur_terrain_vector[height][width])]);
                sprite.setScale(zoom,zoom);
                int zoom_index=16*zoom;
                int zoom_index_bis=16/zoom;
                sprite.setPosition((width*zoom_index)-(cur_player_pos[0]*zoom),height*zoom_index-(cur_player_pos[1]*zoom));
                window.draw(sprite);
            };
        };
        return 0;
    };
};