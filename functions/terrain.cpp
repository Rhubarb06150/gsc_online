///////////////////////////////////////////////////////////
//
//  Now one of the hardest part, the terrain one.
//      
//  The terrainForm function set a given vector to a vector of tiles of a given map
//  The showTerrain functions, shows the terrain, yep.
//  
//  The showTerrain functions was maybe the hardest to write because I wanted to
//  optimize at the maximum so it displays only tiles that are arouns the player.
//  
//  Before and after optimization:
//
//   Here is the config wich I tested optimization with:
//       
//    - I7 12650H
//    - RTX 3050 Ti
//    - 16 Go of DDR5 4800Mhz RAM 
//
//   On a map of ~1500 tiles:
//      
//      Before optimization: average of 30 FPS
//      After optimization: average of 2600 FPS
//
//  So yes, it improves performances a lot, and the showPlayer function has also been optimized
//  which gave almost twice better performances.  
//
///////////////////////////////////////////////////////////

#include <SFML/Graphics.hpp>    
#include <vector>
#include <fstream>
#include <iostream>

#include "tiles.cpp"


class Terrain{

    public:

    TilesIndex tiles_index;

    std::vector<std::vector<std::string>> terrainForm(std::vector<std::vector<std::string>> terrain_vector, std::string map_path){
        
        terrain_vector={};
        std::ifstream inputFile(map_path);
        std::string line;
        int y = 0;

        while (getline(inputFile, line)){
            terrain_vector.push_back({});

            for (int i = 0; i< line.length()/4; i++){

                terrain_vector[y].push_back(line.substr(i*4, 4));
            };
            y++;
        };
        return terrain_vector;
    };

    int showTerrain(std::vector<std::vector<std::string>> terrain_vector, std::vector<int> player_pos, char time_otd, sf::RenderWindow& window, std::vector<int> player_camera, bool light_stand){

        int shown_sprites = 0;
        int ppos0 = player_pos[0];
        int ppos1 = player_pos[1];

        int player_camera0=player_camera[0];
        int player_camera1=player_camera[1];

        int real_player_pos_x = player_pos[0]/64;
        int real_player_pos_y = player_pos[1]/64;
        
        if (real_player_pos_x<6){
            real_player_pos_x=0;
        }else{
            real_player_pos_x-=6;
        };

        if (real_player_pos_y<5){
            real_player_pos_y=0;
        }else{
            real_player_pos_y-=5;
        };

        int max_x = real_player_pos_x+11;
        int max_y = real_player_pos_y+9;

        int time_index;
        if (time_otd=='m'){time_index=0;}
        else if (time_otd=='d'){time_index=1;}
        else time_index=2;

        for (int i = real_player_pos_y; i < terrain_vector.size();i++){
            for (int j = real_player_pos_x; j<terrain_vector[i].size();j++){
                int pos_vec0 = j*64;
                int pos_vec1 = i*64;

                sf::Sprite sprite;
                sprite.setTexture(tiles_index.textures[time_index][tiles_index.getIntIndex(terrain_vector[i][j])]);
                sprite.setPosition(pos_vec0-ppos0+player_camera0,pos_vec1-ppos1+player_camera1);
                sprite.scale(4.f,4.f);
                if (light_stand&&ppos0/64==j&&ppos1/64==i){
                    sprite.setColor(sf::Color(150,150,150));
                }
                window.draw(sprite);
                shown_sprites++;

                if(j>max_x+1)break;
            };
        if (i>max_y)return 2;
        if (shown_sprites>200){
            return 1;};
        };
    return 0;
    };
};