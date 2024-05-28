#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <filesystem>
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>

#include "functions/functions.cpp"
#include "functions/terrain.cpp"
#include "functions/player.cpp"
#include "functions/sounds.cpp"
#include "functions/hud.cpp"

std::vector<std::vector<std::string>> terrain_sprites;
bool framerate_limit=true;

std::string cur_map = "route1";
std::string player_state = "front";
std::string resolution = "720x576";

std::string playerInput;
sf::Text playerText;

int index_frame;
int moving_timer = 0;
bool function_done = 0;
bool typing=true;

//MESSAGES
std::string output_message;
int message_timer;

char time_otd;
Terrain terrain;
Sounds SoundManager;
HUD HUDdisplay;

//PLAYER
std::string username = "MATHEO";
std::string player_type = "b";
std::vector<int> player_pos = {500,500};
bool walking;
bool can_move=true;

//DEBUG
bool debug=false;
int debug_page=1;
int debug_choice=0;
std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point end;
std::vector<int> moy = {0};
float fps;
int fps_;

//---------------------------------------------------------------------------
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

std::string getStandingTile(){
    int real_pos_x=player_pos[0]/64;
    int real_pos_y=player_pos[1]/64;
    return terrain_sprites[real_pos_y][real_pos_x];
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

int isPressed(sf::Event event,sf::Keyboard::Key key){
    if(event.type == (sf::Event::KeyPressed)){
        if (event.key.code == key&&function_done==0){
            function_done=1;
            return 0;
        };};
        if(event.type == (sf::Event::KeyReleased)){
            if (event.key.code == key&&function_done==1)function_done=0;
    };
    return 1;
}

int main()
{

    //INIT EVERYTHIN
    
    std::vector<int> resolution_vec = checkResolutionRWindow();
    std::vector<int> player_offset = checkResolutionPOffset();
    Player player(player_offset);
    sf::RenderWindow window(sf::VideoMode(resolution_vec[0],resolution_vec[1]), "Pokemon GSC Online",sf::Style::Titlebar | sf::Style::Close);
    sf::Clock clock;
    //window.setFramerateLimit(60);
    time_otd = 'd';
    std::string time_otd_str;
    time_otd_str=time_otd;
    srand(time(NULL));
    
    GSC_Functions functions;
    std::cout << functions.currentDateTime() << std::endl;

    setTerrain(terrain,window,time_otd);
    
    while (window.isOpen())
    {   
        start = std::chrono::high_resolution_clock::now();
        index_frame++;
        message_timer++;

        sf::Event event;

        //TAKE SCREENSHOT -------------------------------------------|
        if (isPressed(event,sf::Keyboard::F1)==0){
            functions.takeScreenshot(window,"/home/rhubarb/.gsc_o/screenshots/screenshot_"+functions.currentDateTime()+".png");
            SoundManager.soundEffect("PRESS");
            output_message = "Screenshot saved!";
            message_timer = 0;
        };
        // ----------------------------------------------------------|

        //ENABLE DEBUG ----------------------------------------------|
        if (isPressed(event,sf::Keyboard::F2)==0){
            debug=!debug;
            SoundManager.soundEffect("PRESS");
            if (debug){
                output_message="Showing debug menu";
            }else{
                output_message="Hiding debug menu";
            }
            message_timer = 0;
        };
        // ----------------------------------------------------------|
        

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)&&debug){debug_page=1;};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)&&debug){debug_page=2;};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)&&debug){debug_page=3;};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)&&debug){debug_page=9;debug_choice=0;};

        //PLAYER MOVE
        if (can_move){
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
        }else{
            walking=false;moving_timer=20;
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
        
        terrain.showTerrain(terrain_sprites,player_pos,time_otd,window,player_offset,(debug&&debug_page==9&&debug_choice==2));
        player_state=player.animPlayer(player_state,moving_timer,walking);
        player.showPlayer(player_type,player_state,window);
        
        //HUDdisplay.showPauseMenu(window,username,resolution_vec[0]);

        if (debug){
            if (debug_page==1){
                can_move=true;
                HUDdisplay.showTextDEBUG("Debug page 1 (Terrain)",{0,0},window);
                HUDdisplay.showTextDEBUG("                      ",{0,16},window);
                HUDdisplay.showTextDEBUG(("Current map: "+cur_map),{0,32},window);
                HUDdisplay.showTextDEBUG((std::to_string(terrain_sprites.size()*terrain_sprites[0].size())+" tiles"),{0,48},window);
                HUDdisplay.showTextDEBUG(("Current time: "+time_otd_str),{0,64},window);
                HUDdisplay.showTextDEBUG("Press [F5] to reload terrain",{0,80},window);
                if (isPressed(event,sf::Keyboard::F5)==0){
                    reloadTerrain();
                    SoundManager.soundEffect("PRESS");
                    output_message = "Terrain reloaded!" ;
                    message_timer = 0;
                };
            }else if (debug_page==2){
                can_move=true;
                HUDdisplay.showTextDEBUG("Debug page 2 (Player)",{0,0},window);
                HUDdisplay.showTextDEBUG("                     ",{0,16},window);
                HUDdisplay.showTextDEBUG(("X: "+std::to_string(player_pos[0]/64)+" r("+std::to_string(player_pos[0])+")"),{0,32},window);
                HUDdisplay.showTextDEBUG(("Y: "+std::to_string(player_pos[1]/64)+" r("+std::to_string(player_pos[1])+")"),{0,48},window);
                HUDdisplay.showTextDEBUG("Player state: "+player_state,{0,64},window);
                HUDdisplay.showTextDEBUG("Can move: "+std::to_string(can_move),{0,80},window);
                HUDdisplay.showTextDEBUG("Username: "+username,{0,96},window);
            }else if (debug_page==3){
                can_move=true;
                HUDdisplay.showTextDEBUG("Debug page 3 (Display)",{0,0},window);
                HUDdisplay.showTextDEBUG("                      ",{0,16},window);
                HUDdisplay.showTextDEBUG(("FPS: "+std::to_string(fps_)),{0,32},window);        
                HUDdisplay.showTextDEBUG(("Frames elapsed: "+std::to_string(index_frame)),{0,48},window);
                HUDdisplay.showTextDEBUG("Resolution: "+resolution,{0,64},window);
                HUDdisplay.showTextDEBUG("Press [F5] to reload textures",{0,80},window);
                if (isPressed(event,sf::Keyboard::F4)==0){
                    reloadTextures();
                    SoundManager.soundEffect("PRESS");
                    output_message = "Textures reloaded!" ;
                    message_timer = 0;
                };
            }else if (debug_page==4){
                can_move=true;
                HUDdisplay.showTextDEBUG("Debug page 4 (Game)",{0,0},window);
                HUDdisplay.showTextDEBUG("                   ",{0,16},window);
                HUDdisplay.showTextDEBUG("Game folder /home/rhubarb/.gsc_o/",{0,32},window);
                HUDdisplay.showTextDEBUG("Press [F5] to open game folder",{0,48},window);
                HUDdisplay.showTextDEBUG("Pokemon GSC Online b0.0",{0,64},window);
            }else if (debug_page==9){
                //DEBUG MENU HERE //////////////////////////////////////////////////////////////
                can_move=false;
                std::string time_otd_str;
                time_otd_str = time_otd;
                HUDdisplay.showTextDEBUG("Debug menu",{0,0},window);
                HUDdisplay.showTextDEBUG("          ",{0,16},window);
                    //PAGE DOWN
                    if (isPressed(event,sf::Keyboard::Down)==0){
                        if (debug_choice < 4){
                            debug_choice++;
                        }else{
                            debug_choice=0;
                    };
                    };
                    //PAGE UP
                    if (isPressed(event,sf::Keyboard::Up)==0){
                        if (debug_choice > 0){
                            debug_choice--;
                        }else{
                            debug_choice=4;
                        }
                    };

                    if (debug_choice==0){
                        if (isPressed(event,sf::Keyboard::Right)==0){
                            if (time_otd == 'd'){
                                time_otd = 'n'; 
                            }else if(time_otd == 'n'){
                                time_otd = 'm';
                            }else if (time_otd = 'm'){
                                time_otd = 'd';
                            };
                        };

                        if (isPressed(event,sf::Keyboard::Left)==0){
                            if (time_otd == 'm'){
                                time_otd = 'n'; 
                            }else if(time_otd == 'n'){
                                time_otd = 'd';
                            }else if (time_otd = 'd'){
                                time_otd = 'm';
                            };
                        }
                        
                        HUDdisplay.showTextSELDEBUG("Set time: "+time_otd_str,{0,32},window);
                    }else{
                        HUDdisplay.showTextDEBUG("Set time: "+time_otd_str,{0,32},window);
                    };
                    if (debug_choice==1){
                        HUDdisplay.showTextSELDEBUG("Set player: "+player_type,{0,48},window);
                        }else{
                            HUDdisplay.showTextDEBUG("Set player: "+player_type,{0,48},window);
                        };
                    if (debug_choice==2){
                        int tile_index;
                        if (isPressed(event,sf::Keyboard::Right)==0){
                            if (terrain.tiles_index.getIntIndex(getStandingTile())==terrain.tiles_index.tiles.size()-1){
                                tile_index=0;
                            }else{
                                tile_index=terrain.tiles_index.getIntIndex(getStandingTile())+1;
                            };
                            int real_player_pos_x=player_pos[0]/64;
                            int real_player_pos_y=player_pos[1]/64;
                            terrain_sprites[real_player_pos_y][real_player_pos_x] = terrain.tiles_index.tiles[tile_index][2];
                        };
                        if (isPressed(event,sf::Keyboard::Left)==0){
                            int real_player_pos_x=player_pos[0]/64;
                            int real_player_pos_y=player_pos[1]/64;
                            if (terrain.tiles_index.getIntIndex(getStandingTile())==0){
                                tile_index=terrain.tiles_index.tiles.size()-1;
                            }else{
                                tile_index=terrain.tiles_index.getIntIndex(getStandingTile())-1;
                            };
                            terrain_sprites[real_player_pos_y][real_player_pos_x] = terrain.tiles_index.tiles[tile_index][2];
                        };
                        HUDdisplay.showTextSELDEBUG("Set standing tile: "+terrain.tiles_index.getTileName(getStandingTile())+" ("+getStandingTile()+")",{0,64},window);
                    }else{
                        HUDdisplay.showTextDEBUG("Set standing tile: "+terrain.tiles_index.getTileName(getStandingTile())+" ("+getStandingTile()+")",{0,64},window);
                    };
                    if (debug_choice==3){
                        HUDdisplay.showTextSELDEBUG("Save current terrain",{0,80},window);
                        if (isPressed(event,sf::Keyboard::F5)==0){
                            functions.saveCurrentMap(terrain_sprites,cur_map);
                            SoundManager.soundEffect("PRESS");
                            output_message=".lv file of current terrain saved!";
                            message_timer=0;
                        };
                    }else{
                        HUDdisplay.showTextDEBUG("Save current terrain",{0,80},window);
                    };
                    if (debug_choice==4){
                        HUDdisplay.showTextSELDEBUG("60 FPS limit: "+std::to_string(framerate_limit),{0,96},window);
                        if (isPressed(event,sf::Keyboard::Right)==0){
                            framerate_limit=!framerate_limit;
                            if (framerate_limit){
                                window.setFramerateLimit(60);
                                SoundManager.soundEffect("PRESS");
                                output_message="Framerate limit is now set to 60 FPS";
                                message_timer=0;
                            }else{
                                window.setFramerateLimit(8192);
                                SoundManager.soundEffect("PRESS");
                                output_message="Framerate limit is now removed";
                                message_timer=0;
                            }
                        }
                    }else{
                        HUDdisplay.showTextDEBUG("60 FPS limit: "+std::to_string(framerate_limit),{0,96},window);
                    };
            };
            //HUDdisplay.showTextDEBUG("standing tile: "+terrain.tiles_index.getTileName(getStandingTile())+" ("+getStandingTile()+")",{0,144},window);
        }else{
            can_move=true;
        };

        if (message_timer<=60){
            HUDdisplay.showTextDEBUG(output_message,{0,560},window);
        }
        
        window.display();

        end = std::chrono::high_resolution_clock::now();
        fps = (float)1e9/(float)std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
        fps_=fps;
        moy.push_back(fps_);

    };

    return 0;
};