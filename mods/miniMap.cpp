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
    int size;
    sf::Event events;
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
        zoom=0.5f;
        size=256;
    };
    int init(){
        return 0;
    };
    int passVars(std::vector<int> player_pos, std::vector<std::vector<std::string>> terrain_vector,sf::Event event){
        cur_player_pos=player_pos;
        cur_terrain_vector=terrain_vector;
        events=event;
        return 0;
    };
    int act(){
        return 0;
    };
    int show(sf::RenderWindow& window){
        sf::RectangleShape rectangle(sf::Vector2f(size,size));
        rectangle.setFillColor(sf::Color(50,50,50));
        rectangle.setPosition({0,0});
        sf::RectangleShape player(sf::Vector2f(8,8));
        player.setFillColor(sf::Color(255,128,0));
        player.setPosition({64,64});
        window.draw(rectangle);
        int real_player_pos_x = cur_player_pos[0]/64;
        int real_player_pos_y = cur_player_pos[1]/64;
        
        if (real_player_pos_x<11){real_player_pos_x=0;}
        else{real_player_pos_x-=11;}

        if (real_player_pos_y<11){real_player_pos_y=0;}
        else{real_player_pos_y-=11;};
        
        int rows=0;
        int shown_sprites=0;
        for (int height=real_player_pos_y;height<cur_terrain_vector.size();height++){
            int cols=0;
            for (int width=real_player_pos_x;width<cur_terrain_vector[0].size();width++){
                sf::Sprite sprite;
                sprite.setTexture(tiles.textures[2][tiles.getIntIndex(cur_terrain_vector[height][width])]);
                sprite.setScale(zoom,zoom);
                sprite.setPosition((width*8)-(cur_player_pos[0]/8)+64,height*8-(cur_player_pos[1]/8)+64);
                window.draw(sprite);    
                shown_sprites++;
                cols++;
                if (((width*8)-(cur_player_pos[0]/8)+64)>25){
                    break;
                }
                if (cols>(size/8)+1){
                    break;
                };
        };
        std::cout << shown_sprites << std::endl;
        rows++;
        if (rows>(size/8)+1){
            break;
        }
        };
        window.draw(player);

        sf::RectangleShape top_line({size,16});
        top_line.setFillColor({0,0,0});
        top_line.setPosition({0,0});
        window.draw(top_line);
        
        sf::RectangleShape left_line({16,size});
        left_line.setFillColor({0,0,0});
        left_line.setPosition({0,0});
        window.draw(left_line);

        sf::RectangleShape bottom_line({size,16});
        bottom_line.setFillColor({0,0,0});
        bottom_line.setPosition({0,size-16});
        window.draw(bottom_line);
        
        sf::RectangleShape right_line({16,size});
        right_line.setFillColor({0,0,0});
        right_line.setPosition({size-16,0});
        window.draw(right_line);

        return 0;
    };
};