#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <chrono>

#include "other/terrain.cpp"
#include "other/player.cpp"
#include "other/sounds.cpp"

std::vector<std::vector<std::string>> terrain_sprites;
std::vector<int> moy;

std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point end;

std::string cur_map = "route1";
std::string player_state = "front";

std::string resolution = "720x576";

int index_frame;
std::vector<int> player_pos = {0,0};
int moving_timer = 0;
char time_otd;
float fps;
bool walking;
Terrain terrain;
Sounds SoundManager;

int setTerrain(Terrain terrain,sf::RenderWindow& window, char tœime_otd){
    std::cout << "Loading terrain..." << std::endl;
    terrain_sprites=terrain.terrainForm(terrain_sprites,cur_map,player_pos);
    std::cout << "Terrain loaded!" << std::endl;
    return 0;
};

int reloadTextures(){
    std::cout << "Reloading textures..." << std::endl;
    TilesIndex new_ti;
    terrain.tiles_index=new_ti;
    std::cout << "Textures reloaded!" << std::endl;
    return 0;
}

int reloadTerrain(){
    std::cout << "Reloading terrain..." << std::endl;
    Terrain new_tr;
    terrain=new_tr;
    terrain_sprites=terrain.terrainForm(terrain_sprites,cur_map,player_pos);
    std::cout << "Terrain reloaded!" << std::endl;
    return 0;
};

int getStandingTile(){
    int real_pos_x=player_pos[0]/64;
    int real_pos_y=player_pos[1]/64;
    std::cout << terrain_sprites[real_pos_y][real_pos_x] << std::endl;
    return 0;
};

std::vector<int> checkResolutionRWindow(){
    std::vector<int> res;
    if (resolution=="640x576"){
        res={640,576};
    }else if (resolution=="720x576"){
        res={720,576};
    }else {
        res={640,576};
    };
    return res;
};

std::vector<int> checkResolutionPOffset(){
    std::vector<int> offset;
    if (resolution=="640x576"){
        offset={256,256};
    }else if (resolution=="720x576"){
        offset={320,256};
    }else {
        offset={256,256};
    };
    return offset;
};

int main()
{
    //INIT EVERYTHIN
    std::vector<int> resolution_vec = checkResolutionRWindow();
    std::vector<int> player_offset = checkResolutionPOffset();
    sf::RenderWindow window(sf::VideoMode(resolution_vec[0],resolution_vec[1]), "Pokémon GSC Online");
    srand(time(NULL));
    sf::Clock clock;
    window.setFramerateLimit(60);
    Player player(player_offset);
    time_otd = 'd';

    setTerrain(terrain,window,time_otd);
    
    while (window.isOpen())
    {
        start = std::chrono::high_resolution_clock::now();
        
        index_frame++;
        sf::Event event;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))getStandingTile();
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)){time_otd='m';SoundManager.soundEffect("PRESS");}
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)){time_otd='d';SoundManager.soundEffect("PRESS");}
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)){time_otd='n';SoundManager.soundEffect("PRESS");}

        //PLAYER MOVE
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            if (player_pos[0]<terrain_sprites[0].size()*64-64){
                if (terrain.tiles_index.checkCollision(player_pos,'r',terrain_sprites)==0){
                    player_pos[0]+=4;
                };
            }else{SoundManager.soundEffect("COLLISION");}
            walking=true;
            moving_timer+=1;
            player_state="right";
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){ 
            if(player_pos[0]>0){
                if (terrain.tiles_index.checkCollision(player_pos,'l',terrain_sprites)==0){
                    player_pos[0]-=4;
                };
            }else{SoundManager.soundEffect("COLLISION");}
            walking=true;
            moving_timer+=1;
            player_state="left";
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){ 
            if(player_pos[1]<terrain_sprites.size()*64-64){
                if (terrain.tiles_index.checkCollision(player_pos,'d',terrain_sprites)==0){
                    player_pos[1]+=4;
                };
            }else{SoundManager.soundEffect("COLLISION");}
            walking=true;
            moving_timer+=1;
            player_state="front";
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){   
            if(player_pos[1]>0){
                if (terrain.tiles_index.checkCollision(player_pos,'u',terrain_sprites)==0){
                    player_pos[1]-=4;
                };
            }else{SoundManager.soundEffect("COLLISION");}
            walking=true;
            moving_timer+=1;
            player_state="back";
        }
        else {walking=false;moving_timer=20;};

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5)&&sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
            reloadTextures();
        };
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F4)&&sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
            reloadTerrain();
        };

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                int sum_moy = 0;
                int biggest = moy[0];
                int lowest = moy[0];
                for (int i =0;i<moy.size();i++){
                    if (moy[i]>biggest)biggest=moy[i];
                    if (moy[i]<lowest)lowest=moy[i];
                    sum_moy+=moy[i];
                };
                std::cout << "AVG FPS: " << sum_moy/moy.size() << std::endl;
                std::cout << "Lowest FPS: " << lowest << std::endl;
                std::cout << "Highest FPS: " << biggest << std::endl;
                window.close();
                };
        };

        //SHOW ALL
        
        window.clear();
        
        terrain.showTerrain(terrain_sprites,player_pos,time_otd,window,player_offset);
        player_state=player.animPlayer(player_state,moving_timer,walking);
        player.showPlayer('b',player_state,window);
        
        window.display();

        end = std::chrono::high_resolution_clock::now();
        fps = (float)1e9/(float)std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
        int fps_;
        fps_=fps;

        moy.push_back(fps_);

        if (index_frame%30==0)window.setTitle("Pokémon GSC Online - "+std::to_string(fps_)+" fps - "+std::to_string(terrain_sprites.size()*terrain_sprites[0].size())+" tiles");

    };

    return 0;
};