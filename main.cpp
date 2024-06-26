///////////////////////////////////////////////////////////
//
//  Pokémon Gold Silver Crystal Online
//  Started on 20/07/2024 by Rhubarb
//
//  See git on https://github.com/Rhubarb06150/gsc_online
//
//  Yup I coded this like a dick, but it wasn't intended to be public
//
//  If you want to build the app:
//      Make sure to have SFML 2, and g++ installed.
//
//  For install last SFML version on Linux:
//      type: sudo apt-get install libsfml-dev
//
//  Here is a prepared build command for Linux:
//      g++ -c main.cpp&&g++ main.o -o main -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system&&rm -f main.o&&./main
//          It will build the app and run it
//
//  If you need more informations about SMFL it's just below:
//      https://www.sfml-dev.org/
//
//
//  If you need help about anything relating to this or you just want to contact me it's below:
//
//      - Discord: rhubarb#5297 / rhubarb06150
//      - Twitter: @rhubarb06150
//      - mail: rhubarb06150@gmail.com
//
//  ----------/!\ IMPORTANT /!\----------
//
//  I'm looking for a graphic designer that could help me to reproduce
//  some 2G Pokémon designs like tiles, icons, or NPCs and other stuff.
//
//  Folders:
//
//      If you play for the first time, the game will create you a folder located in your /home/<user>/ directory
//      In this folder will be saved: screenshots, logs, maps
//
//  Basic Controls:
//
//      In-Game:
//          
//          - Arrows : move
//          - Spacebar : interract
//          - F1 : take a screenshot
//          - F2 : open debug pages (you can choose a page by pressing numbers on top of the keyboard)
//
//      In-Menus:
//
//          - Arrows : navigate
//          - F5 / X : validate an action
//          - F6 : cancel / go back
//          - Left/Right : edit values (generally in level editor or in debug menu)
//
//  Feel free to modify code and share it!
//
//  I know I could have done a readme but I don't know how to do it and I prefer this format.
//
///////////////////////////////////////////////////////////

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <string.h>
#include <variant>
#include <thread>
#include <vector>
#include <chrono>
#include <regex>
#include <ctime>

#include "functions/connection.cpp"
#include "functions/functions.cpp"
#include "functions/trainers.cpp"
#include "functions/terrain.cpp"
#include "functions/player.cpp"
#include "functions/sounds.cpp"
#include "functions/items.cpp"
#include "functions/hud.cpp"

#include "mods/header.hpp"

class Game{

    public:
        //MODULES VARS
        Terrain terrain;//terrain functions, to init, load/reload and showterrain
        Sounds SoundManager;//sound functions, for sound effects and music
        HUD HUDdisplay;//hud functions, every texts you'll see is displayed with HUDdisplay
        Player player;//players functions, used to show the player, also all players sprites are stored here
        TilesIndex Tiles;//tiles functions, tiles manager, sprites etc are all stored here
        Trainers TrainersIndex;
        Items ItemsIndex;//items functions, items infos are stored here.
        GSC_Functions functions;//other functions such as save a map, get current date and time
        Connection connection;
        double version;

        //TERRAIN VARS
        std::vector<std::vector<std::string>> terrain_vector;//where all tiles are saved by their index for example, \x00 is the "grass" tile
        std::string cur_map;//used for determinate which map is currently played, used when terrain is getting init, and in some other functions
        char time_otd;//used to display tiles why daytime variations
        std::string time_otd_str;//just the same as top but in string so I can use it in tiles module
        
        //DISPLAY VARS
        sf::RenderWindow window;//setting up the window here
        std::string real_res;//resolution of the window stored as a string
        std::vector<int> resolution_vec;//resolution stored as an int vector for passing it to the window create function
        std::vector<int> player_offset;//works with the resolution, depending on the resolution, the offset will be different so the player is at center of the screen
        bool framerate_limit;//I think I will not use this anymore
        int index_frame;//is incremented every frame
        std::string output_message;//used to display a message, when you want to show a message, use: ouput_message="your message"
        int message_timer;//used to display message, when you want to show a message, put this value to less than 60
        bool record;//used to display message, when you want to show a message, put this value to less than 60
        std::vector<std::string> record_images;
        int border_style;//Used to manage border style choosen by the user
        int full_loaded=0;
        
        //PLAYER VARS
        std::vector<int> player_pos;//used to store player position (yup)
        std::string player_state;//used to store current frame of player animation
        int moving_timer;//used to animate player by frame index
        std::string username;//that's basically your username
        std::string player_type;//that's your type (boy, girl etc...)
        bool walking;//is set to true when player is moving for animate the player
        bool can_move;//when set to true, the player can move, so when false, the player can't (for example it's set to false when some menus are opened)
        int trainer_app_id;
        
        //EDITOR VARS
        std::string copied_tile;//the copied tile id (not index but id for example "\x00")
        int selected_tile_index;//the selcted tile index in int so it navigate faster in tiles
        int copied_tile_index;//same but for the copied tile
        int default_tile_index;//finally, he's here for you, no I'm joking it's just the same as above but for the default tile
        std::vector<int> pos1;//the pos1 vector used for filling, random patterns etc
        std::vector<int> pos2;//the pos2 vector, same as pos1 but it's the pos2
        bool pos1set;//used to determine if the pos1 is set
        bool pos2set;//same but for pos 2
        bool menu_show;//if set to trues, then the menu is showed, unless it doesnt

        //DEBUG VARS
        bool debug;//when set to true, the debug menu is showesd up
        bool debug_launch;
        int debug_page;//you can choose the debug page you want to display with this function
        int debug_choice;//used in debug page 9 for navigate throught options
        std::chrono::high_resolution_clock::time_point start;//used for fps measuring
        std::chrono::high_resolution_clock::time_point end;//also used for fps measuring
        float fps;//get the exact fps value in float
        int fps_;//used to display the fps value more clearely
        std::vector<int> moy;//setting up the fps average when exiting the game

        //OTHER VARS;
        bool function_done;//was used in isPressed function but I'm gonna delete this var and the isPressed function
        sf::Event event;//I'm just intializing this here so I don't have to pass it as an argument in every function
        sf::Image icon;//that's for the icon
        sf::Clock clock;//the clock that's used for ??? I don't even remember but I let it here bc Idk if it break the program

        //TEXT VARS
        //std::vector<std::string> letters;
        sf::String text_input;
        int sel_begin;
        int sel_end;
        int pres;
        
        //MODS VARS
        bool gpp_installed;
        std::vector<std::string> mods_list;
        std::vector<bool> mods_states;
        std::vector<std::vector<std::string>> mods_descriptions;
        //MOD INIT

        std::thread screenshotThread_Thread;

    Game(){
        version=0.0;
        //letters={"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
        sel_begin=0;
        sel_end=0;
        functions.log("ENGINE","GSC Online is launcing in version "+std::to_string(version).substr(0,4));
        framerate_limit=true;//set the framerate limit (may be unused after)
        player_state="front";//default player state is front so when 
        copied_tile="\\x00";//grass tile is copied by default
        moving_timer=0;//initializing timer for player animation
        function_done=0;//I really need to delete this
        username="Player";//Default username if username in settings file isn't detected
        player_type="b";//default player type is the basic boy, the basic boiiiiiii
        player_pos={500,500};//I will change this later
        can_move=true;//It's better to move when you start
        debug=false;//don't show debug on game launch (that's better)
        debug_page=1;//defualt debug page is the page 1
        debug_choice=0;//same goes for the choice, the default choice of Debug Menu is first choice
        moy={0};//the moy vector for get the average fps
        pos1={};//empty pos1 so we dont start with pos1 already set
        pos2={};//same but for pos2
        pos1set=false;//so we dont start with already set pos1
        pos2set=false;//so we dont start with already set pos2
        real_res="640x576";
        resolution_vec=checkResolutionRWindow();//gets the resolution
        player_offset=checkResolutionPOffset();//gets the player offset
        window.create(sf::VideoMode(resolution_vec[0],resolution_vec[1]), "Pokemon GSC Online",sf::Style::Titlebar | sf::Style::Close);//create the window with the resolution we just got
        icon.loadFromFile("assets/icon.png");//loads up the icon
        window.setIcon(64, 64, icon.getPixelsPtr());//and BANG, we set the icon
        window.setFramerateLimit(60);//60 FPS limit hell yeah, 60HZ POWAAAAAAAAA
        player.initPlayer(player_offset);//we init the player here
        time_otd='d';//default daytime is the day
        time_otd_str=time_otd;//same value as time_otd but is string, not char
        record=false;
        record_images={};
        gpp_installed=false;
        mods_list={};
        mods_states={};
        mods_descriptions={};
        debug_launch=false;
        trainer_app_id=0;
        //MOD DESC
        srand(time(NULL));//reset the random values

        if (!std::filesystem::is_directory(functions.getUserPath()+"/.gsc_o/")){// here we verify if a game folder exists
            std::filesystem::create_directory(functions.getUserPath()+"/.gsc_o");//if not, we create a folder called ".gsc_o" located in the user folder
            std::filesystem::create_directory(functions.getUserPath()+"/.gsc_o/screenshots/");//we create the screenshots folder
            std::filesystem::create_directory(functions.getUserPath()+"/.gsc_o/maps/");//we create the maps folder
            functions.log("INFO","an game folder has been created at "+functions.getUserPath()+"/.gsc_o, it will be used to store your saved maps and your screenshots");//here we tell the player thta we created him a fresh folder
        };
        functions.log("ENGINE","Game launched!");//and there, we annouce to our dear player, that the game is ready to be played
    };

int crash(std::string message){
    do{
        full_loaded=0;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed){
                functions.quitGame(window);
                return 0;
            };
            if(event.type==sf::Event::KeyPressed){
                screenshotVerify();
            };
        };
        window.clear();
        HUDdisplay.showTextWOB("Game crashed",{0,0},window);
        HUDdisplay.showTextDEBUG(message,{0,32},window);
        HUDdisplay.showTextDEBUG("Press any key to quit the game",{0,560},window);
        window.display();
        full_loaded=1;
    }while(true);
    functions.quitGame(window);
    return 0;
};

void mainLoop(){
    full_loaded=0;
    start=std::chrono::high_resolution_clock::now();
    index_frame++;      //increasing this for frames mesuring
    message_timer++;    //increasing this for messages display

    sf::Event event;    //get happening events for handle them

    //if (isPressed(event,sf::Keyboard::F3)==0){
    //    show_debug_pause();
    //};

    //PLAYER MOVE
    if (can_move){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down)||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left)||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            moving_timer+=1;
        }else{
            walking=false;
            moving_timer=20;
        };
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            if (player_pos[0]<terrain_vector[0].size()*64-64){
                if (Tiles.checkCollision(player_pos,'r',terrain_vector)==0){
                    player_pos[0]+=4;
                };
            }else{SoundManager.soundEffect("COLLISION");}
            walking=true;
            player_state="right";
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){ 
            if(player_pos[0]>0){
                if (Tiles.checkCollision(player_pos,'l',terrain_vector)==0){
                    player_pos[0]-=4;
                };
            }else{SoundManager.soundEffect("COLLISION");}
            walking=true;
            player_state="left";
        };
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){ 
            if(player_pos[1]<terrain_vector.size()*64-64){
                if (Tiles.checkCollision(player_pos,'d',terrain_vector)==0){
                    player_pos[1]+=4;
                };
            }else{SoundManager.soundEffect("COLLISION");}
            walking=true;
            player_state="front";
        }   
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){   
            if(player_pos[1]>0){
            if (Tiles.checkCollision(player_pos,'u',terrain_vector)==0){
                player_pos[1]-=4;
            };
            }else{SoundManager.soundEffect("COLLISION");}
            walking=true;
            player_state="back";
        };
    }else{
        walking=false;moving_timer=20;
    };

    if (connection.connected){
        message_timer=0;
        if (connection.server){
            output_message="Server";
            connection.recvPos();
            connection.sendPos(player_pos);
        };
        if (connection.client){
            output_message="Client";
            connection.recvPos();
            connection.sendPos(player_pos);
        };
    };

    // WHILE EVENT LOOP HERE
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed){
            int sum_moy=0;
            int biggest=moy[0];
            int lowest=moy[0];
            for (int i =0;i<moy.size();i++){
                if (moy[i]>biggest)biggest=moy[i];
                if (moy[i]<lowest)lowest=moy[i];
                sum_moy+=moy[i];
            };
            functions.log("DEBUG","AVG FPS: "+std::to_string(sum_moy/moy.size()));
            functions.log("DEBUG","Lowest FPS: "+std::to_string(lowest));
            functions.log("DEBUG","Highest FPS: "+std::to_string(biggest));
            functions.quitGame(window);
        }else if (event.type==sf::Event::KeyPressed){
            screenshotVerify();
            //MOD KEYPRESS
            if (event.key.code==sf::Keyboard::S){
                connection.serverStart();
            };
            if (event.key.code==sf::Keyboard::C){
                connection.clientConnect();        
            };
            if (event.key.code==sf::Keyboard::Escape){
                pauseMenu();
            };
            if(debug_launch){
            if (event.key.code==sf::Keyboard::F2){
                debug=!debug;
                SoundManager.soundEffect("PRESS");
                if (debug){
                    output_message="Showing debug menu";
                }else{
                    output_message="Hiding debug menu";
                };
                message_timer=0;
            };
            };
            if (debug){
                if (debug_page==9&&debug_choice==0){
                if (event.key.code==sf::Keyboard::Right){
                    if (time_otd == 'd'){
                        time_otd='n'; 
                    }else if(time_otd == 'n'){
                        time_otd='m';
                    }else if (time_otd='m'){
                        time_otd='d';
                    };
                };
                if (event.key.code==sf::Keyboard::Left){
                    if (time_otd == 'm'){
                        time_otd='n'; 
                    }else if(time_otd == 'n'){
                        time_otd='d';
                    }else if (time_otd='d'){
                        time_otd='m';
                    };
                };
                };
                if (debug_page==9){
                    if (event.key.code==sf::Keyboard::Down){
                        if (debug_choice < 4){
                            debug_choice++;
                        }else{
                            debug_choice=0;
                        };
                    };
                    if (event.key.code==sf::Keyboard::Up){
                        if (debug_choice > 0){
                            debug_choice--;
                        }else{
                            debug_choice=4;
                        };
                    };
                };
            };
        };
    };
    //SHOW ALL
    //MOD PASSVARS
    //MOD ACT
    window.clear();
    terrain.showTerrain(terrain_vector,player_pos,time_otd,window,player_offset,(debug&&debug_page==9&&debug_choice==2),index_frame);
    player_state=player.animPlayer(player_state,moving_timer,walking);
    player.showPlayer(player_type,player_state,window);
    //HUDdisplay.drawSquare(window,{0,0},{352,96});
    //HUDdisplay.showTextBOW("Rhubarb",{32,32},window);
    if (debug){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)&&debug){debug_page=1;};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)&&debug){debug_page=2;};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)&&debug){debug_page=3;};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)&&debug){debug_page=9;};
        if (debug_page==1){
            can_move=true;
            HUDdisplay.showTextDEBUG("Debug page 1 (Terrain)",{0,0},window);
            HUDdisplay.showTextDEBUG("                      ",{0,16},window);
            HUDdisplay.showTextDEBUG(("Current map: "+cur_map),{0,32},window);
            HUDdisplay.showTextDEBUG((std::to_string(terrain_vector.size()*terrain_vector[0].size())+" tiles"),{0,48},window);
            HUDdisplay.showTextDEBUG(("Current time: "+time_otd_str),{0,64},window);
            HUDdisplay.showTextDEBUG("Press [F5] to reload terrain",{0,80},window);
            if (isPressed(event,sf::Keyboard::F5)==0){
                reloadTerrain();
                SoundManager.soundEffect("PRESS");
                output_message="Terrain reloaded!" ;
                message_timer=0;
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
            HUDdisplay.showTextDEBUG("Resolution: "+real_res,{0,64},window);
            HUDdisplay.showTextDEBUG("Press [F5] to reload textures",{0,80},window);
            if (isPressed(event,sf::Keyboard::F5)==0){
                reloadTextures();
                SoundManager.soundEffect("PRESS");
                output_message="Textures reloaded!" ;
                message_timer=0;
            };
        }else if (debug_page==4){
            can_move=true;
            HUDdisplay.showTextDEBUG("Debug page 4 (Game)",{0,0},window);
            HUDdisplay.showTextDEBUG("                   ",{0,16},window);
            HUDdisplay.showTextDEBUG("Game folder /home/rhubarb/.gsc_o/",{0,32},window);
            HUDdisplay.showTextDEBUG("Press [F5] to open game folder",{0,48},window);
            HUDdisplay.showTextDEBUG("Pokemon GSC Online "+std::to_string(version).substr(0,4),{0,64},window);
        }else if (debug_page==9){
            //DEBUG MENU HERE //////////////////////////////////////////////////////////////
            can_move=false;
            std::string time_otd_str;
            time_otd_str=time_otd;
            HUDdisplay.showTextDEBUG("Debug menu",{0,0},window);
            HUDdisplay.showTextDEBUG("          ",{0,16},window);

                if (debug_choice==0){
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
                    if (isPressed(event,sf::Keyboard::F5)==0){
                        int real_player_pos_x=player_pos[0]/64;
                        int real_player_pos_y=player_pos[1]/64;
                        copied_tile=terrain_vector[real_player_pos_y][real_player_pos_x];
                        output_message="Copied "+copied_tile+" ("+ Tiles.getTileName(copied_tile) +")";
                        message_timer=0;
                    }; 
                    if (isPressed(event,sf::Keyboard::F6)==0){
                        if (copied_tile!=""){
                            int real_player_pos_x=player_pos[0]/64;
                            int real_player_pos_y=player_pos[1]/64;
                            terrain_vector[real_player_pos_y][real_player_pos_x]=copied_tile;
                            output_message="Pasted "+copied_tile+" ("+ Tiles.getTileName(copied_tile) +")";
                            message_timer=0;
                        }else{
                            output_message="No element to paste";
                            message_timer=0; 
                        }
                    }; 
                    if (isPressed(event,sf::Keyboard::Right)==0){
                        if (Tiles.getIntIndex(getStandingTile())==Tiles.tiles.size()-1){
                            tile_index=0;
                        }else{
                            tile_index=Tiles.getIntIndex(getStandingTile())+1;
                        };
                        int real_player_pos_x=player_pos[0]/64;
                        int real_player_pos_y=player_pos[1]/64;
                        terrain_vector[real_player_pos_y][real_player_pos_x]=Tiles.tiles[tile_index][2];
                    };
                    if (isPressed(event,sf::Keyboard::Left)==0){
                        int real_player_pos_x=player_pos[0]/64;
                        int real_player_pos_y=player_pos[1]/64;
                        if (Tiles.getIntIndex(getStandingTile())==0){
                            tile_index=Tiles.tiles.size()-1;
                        }else{
                            tile_index=Tiles.getIntIndex(getStandingTile())-1;
                        };
                        terrain_vector[real_player_pos_y][real_player_pos_x]=Tiles.tiles[tile_index][2];
                    };
                    HUDdisplay.showTextSELDEBUG("Set standing tile: "+Tiles.getTileName(getStandingTile())+" ("+getStandingTile()+")",{0,64},window);
                }else{
                    HUDdisplay.showTextDEBUG("Set standing tile: "+Tiles.getTileName(getStandingTile())+" ("+getStandingTile()+")",{0,64},window);
                };
                if (debug_choice==3){
                    HUDdisplay.showTextSELDEBUG("Save current terrain",{0,80},window);
                    if (isPressed(event,sf::Keyboard::F5)==0){
                        functions.saveCurrentMap(terrain_vector,cur_map);
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
                        if (framerate_limit){
                            SoundManager.soundEffect("PRESS");
                            output_message="Framerate limit is now set to 60 FPS";
                            message_timer=0;
                        }else{
                            SoundManager.soundEffect("PRESS");
                            output_message="Framerate limit is now removed";
                            message_timer=0;
                        };
                    };
                }else{
                    HUDdisplay.showTextDEBUG("60 FPS limit: "+std::to_string(framerate_limit),{0,96},window);
                };
        };
    }else{
        can_move=true;
    };

    if (message_timer<=60){
        HUDdisplay.showTextDEBUG(output_message,{0,560},window);
    };
    //DO NOT DELETE THIS PLS
    //MOD DISPLAY
    if (debug_launch)
        HUDdisplay.showTextDEBUG(std::to_string(fps_),{0,560},window);
    window.display();
    full_loaded=1;

    //if (record){
    //    if(index_frame%4==0){
    //        record_images=functions.saveGifFrame(index_frame,window,record_images);
    //    };
    //};

    end=std::chrono::high_resolution_clock::now();
    fps=(float)1e9/(float)std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    fps_=fps;
    moy.push_back(fps_);
};

int initGame(){
    //INIT EVERYTHIN
    main_menu();

    return 0;
};

//---------------------------------------------------------------------------
int setTerrain(){
    functions.log("TERRAIN","Loading terrain...");
    functions.log("MAP","Loading map from ");
    terrain_vector=terrain.terrainForm(terrain_vector,cur_map);
    functions.log("TERRAIN","Terrain loaded!");
    return 0;
};

int reloadTextures(){
    functions.log("TEXTURES","Reloading textures...");
    TilesIndex new_ti;
    Tiles=new_ti;
    functions.log("TEXTURES","Textures reloaded!");
    return 0;
};

int reloadTerrain(){
    functions.log("TERRAIN","Reloading terrain...");
    Terrain new_tr;
    terrain=new_tr;
    terrain_vector=terrain.terrainForm(terrain_vector,cur_map);
    functions.log("TERRAIN","Terrain reloaded!");
    return 0;
};

std::string getStandingTile(){
    int real_pos_x=player_pos[0]/64;
    int real_pos_y=player_pos[1]/64;
    return terrain_vector[real_pos_y][real_pos_x];
};

std::vector<int> checkResolutionRWindow(){
    std::vector<int> res;
    if (real_res=="640x576"){
        res={640,576};
    }else if (real_res=="720x576"){
        res={720,576};
    }else {
        res={640,576};
    };
    return res;
};

std::vector<int> checkResolutionPOffset(){
    std::vector<int> offset;
    if (real_res=="640x576"){
        offset={256,256};
    }else if (real_res=="720x576"){
        offset={320,256};
    }else {
        offset={256,256};
    };
    return offset;
};

int loadSettings(){
    if (std::filesystem::is_regular_file(functions.getUserPath()+"/.gsc_o/settings")){
        std::ifstream inputFile(functions.getUserPath()+"/.gsc_o/settings");
        std::string line;
        while (getline(inputFile, line)){
            if (line.rfind("resolution=",0)==0){
                real_res = functions.ReplaceAll(line,"resolution=","");
                functions.log("ENGINE","The resolution of the game is "+real_res);
                std::vector<int> resolution_vec=checkResolutionRWindow();
                std::vector<int> player_offset=checkResolutionPOffset();
            }else if (line.rfind("username=",0)==0){
                username = functions.ReplaceAll(line,"username=","");
                functions.log("INFO","Game launched as "+username);
            }else if (line.rfind("border=",0)==0){
                border_style = std::stoi(functions.ReplaceAll(line,"border=",""));
            }else if (line.rfind("trainer=",0)==0){
                border_style = std::stoi(functions.ReplaceAll(line,"trainer=",""));
            };
        };
    }else{
        functions.log("WARN","Failed to load settings file");
    };
    return 0;
};

int isPressed(sf::Event event,sf::Keyboard::Key key_pressed){
    if (event.type == sf::Event::KeyPressed && event.key.code==key_pressed){
        return 0;
    }else{
        return 1;
    };
};


int yesNoQuestion(std::string question,sf::Event event,sf::RenderWindow& window){
    int choice = 1;
    do{
        full_loaded=0;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                functions.quitGame(window);
                return 0;
            };
        };
        window.clear();
        HUDdisplay.showTextDEBUG("Following action:",{32,80},window);
        HUDdisplay.showTextDEBUG(question,{32,96},window);
        HUDdisplay.showTextDEBUG("Press F5 or X to validate your choice",{32,128},window);

        if (isPressed(event,sf::Keyboard::Right)==0){
            choice=1;
        };
        if (isPressed(event,sf::Keyboard::Left)==0){
            choice=0;
        };
        if (isPressed(event,sf::Keyboard::F5)==0||isPressed(event,sf::Keyboard::X)==0){
            return choice;
        };

        if (choice==0){
            HUDdisplay.showTextSELDEBUG("YES",{32,160},window);
            HUDdisplay.showTextDEBUG("NO",{144,160},window);
        }else{
            HUDdisplay.showTextDEBUG("YES",{32,160},window);
            HUDdisplay.showTextSELDEBUG("NO",{144,160},window);
        };
        if (message_timer<=60){
            HUDdisplay.showTextDEBUG(output_message,{0,560},window);
        };
        window.display();
        full_loaded=1;
    }while(true);
    return 1;
};

std::string takeLetter(sf::String text_input){
    if (event.type==sf::Event::TextEntered){
        if (event.text.unicode<32){
            if (text_input.getSize()>0){
                text_input.erase(text_input.getSize()-1,1);
            };
        }else if (event.text.unicode<128){
            text_input+=event.text.unicode;
        };
    };
    return text_input;
};

std::string askText(std::string caption){
    text_input="";
    std::string text;
    do{
        full_loaded=0;
        index_frame++;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                functions.quitGame(window);
                return 0;
            };
            if (event.type==sf::Event::KeyPressed){
                screenshotVerify();
                if (event.key.code==sf::Keyboard::Enter||event.key.code==sf::Keyboard::Return){
                    return text_input.toAnsiString();
                };
                if (event.key.code==sf::Keyboard::F6){
                    return "";
                };
            };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)||sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)){
                if (event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::C){
                    return "";
                };
            };
            text_input=takeLetter(text_input);
        };
        window.clear(sf::Color(255,255,255));
        int win_width=window.getSize().x;
        HUDdisplay.drawSquare(window,{0,0},{win_width,576},border_style);
        HUDdisplay.showTextBOW(caption,{32,32},window);
        if (index_frame%60<=30){
            HUDdisplay.showTextAskBOW(text_input,{32,96},window);
        }else{
            HUDdisplay.showTextAskBOW(text_input+"_",{32,96},window);
        }
        if (message_timer<=60){
            HUDdisplay.showTextDEBUG(output_message,{0,560},window);
        };
        window.display();
        full_loaded=1;
    }while(true);
    return text_input.toAnsiString();
};

int randomPatternLoop(){
    int chance=4;
    int choice=0;
    bool replace=false;
    do{
        full_loaded=0;
        while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed){
                    functions.quitGame(window);
                    return 0;
                };
                if (event.type==sf::Event::KeyPressed){
                    screenshotVerify();
                    if (event.key.code==sf::Keyboard::F6){
                        break;
                    };
                    if (event.key.code==sf::Keyboard::Right){
                        if (choice==0){
                            chance++;
                        };
                    };
                    if (event.key.code==sf::Keyboard::Left){
                        if (choice==0){
                            if (chance>1){
                                chance--;
                            };
                        };
                    };
                    if (event.key.code==sf::Keyboard::Up){
                        choice=0;
                    };
                    if (event.key.code==sf::Keyboard::Down){
                        choice=1;
                    };
                };
                if (event.key.code==sf::Keyboard::F5||event.key.code==sf::Keyboard::X){
                if (pos1set&&pos2set){
                    for (int i = 0; i<terrain_vector.size();i++){
                    for (int j = 0; j<terrain_vector[i].size();j++){
                        if ((pos1[0]<=j&&j<=pos2[0])&&(pos1[1]<=i&&i<=pos2[1])){
                            if (rand()%chance+1==chance){
                            terrain_vector[i][j]=Tiles.tiles[selected_tile_index][2];
                            };
                        };
                        };
                    };
                    menu_show=false;
                    break;
                    output_message="The random pattern has been applied";
                    message_timer=0;
                }else{
                output_message="You need to set Pos1 and Pos2 first!";
                message_timer=0;
                return 0;
                };
                };
            };
        window.clear(sf::Color(148,148,148));
        HUDdisplay.editorBG(window);
        terrain.showTerrain(terrain_vector,player_pos,time_otd,window,player_offset,true,index_frame);
        HUDdisplay.showTextDEBUG("Editor Mode "+std::to_string(version).substr(0,4),{0,0},window);
        if (message_timer<=60){
            HUDdisplay.showTextDEBUG(output_message,{0,560},window);
        };

        HUDdisplay.showTextDEBUG("                        ",{32,32},window);
        HUDdisplay.showTextDEBUG("  Set random pattern    ",{32,48},window);
        HUDdisplay.showTextDEBUG("                        ",{32,64},window);
        HUDdisplay.showTextDEBUG("  "+Tiles.tiles[selected_tile_index][0]+std::string(22-Tiles.tiles[selected_tile_index][0].length(),' '),{32,80},window);
        HUDdisplay.showTextDEBUG("                        ",{32,96},window);
        HUDdisplay.showTextDEBUG("  1 chance on ",{32,112},window);
        if (choice==0){
            HUDdisplay.showTextSELDEBUG(std::to_string(chance)+std::string(10-std::to_string(chance).length(),' '),{256,112},window);
        }else{
            HUDdisplay.showTextDEBUG(std::to_string(chance)+std::string(10-std::to_string(chance).length(),' '),{256,112},window);
        }
        HUDdisplay.showTextDEBUG("                        ",{32,128},window);
        HUDdisplay.showTextDEBUG("  Replace? ",{32,144},window);
        if (replace){
            if (choice==1){
                HUDdisplay.showTextSELDEBUG("yes          ",{208,144},window);
            }else{
                HUDdisplay.showTextDEBUG("yes          ",{208,144},window);
            };

        }
        else{
            if (choice==1){
                HUDdisplay.showTextSELDEBUG("no           ",{208,144},window);
            }else{
                HUDdisplay.showTextDEBUG("no           ",{208,144},window);
            }
            HUDdisplay.showTextDEBUG("                        ",{32,160},window);
            HUDdisplay.showTextDEBUG("  Press F6 to cancel    ",{32,176},window);
            HUDdisplay.showTextDEBUG("                        ",{32,192},window);
        };
        window.display();
        full_loaded=1;
    }while(true);
    return 0;
};

int showEditorControls(){
    do{
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                functions.quitGame(window);
                return 0;
            };
        };
        if (isPressed(event,sf::Keyboard::F6)==0){
            return 0;
        };
    window.clear(sf::Color(148,148,148));
    HUDdisplay.editorBG(window);
    terrain.showTerrain(terrain_vector,player_pos,time_otd,window,player_offset,true,index_frame);
    HUDdisplay.showTextDEBUG("Editor Mode "+std::to_string(version).substr(0,4),{0,0},window);

    HUDdisplay.showTextDEBUG("                                   ",{32,32},window);
    HUDdisplay.showTextDEBUG("       Level Editor Controls       ",{32,48},window);
    HUDdisplay.showTextDEBUG("                                   ",{32,64},window);
    HUDdisplay.showTextDEBUG("  Navigate: up-down arrows         ",{32,80},window);
    HUDdisplay.showTextDEBUG("  Modify value: left-right arrows  ",{32,96},window);
    HUDdisplay.showTextDEBUG("  F5 or X: validate action         ",{32,112},window);
    HUDdisplay.showTextDEBUG("  Move around the terrain: arrows  ",{32,128},window);
    HUDdisplay.showTextDEBUG("  Toggle menu: Right Shift         ",{32,144},window);
    HUDdisplay.showTextDEBUG("  Copy tile: C                     ",{32,160},window);
    HUDdisplay.showTextDEBUG("  Paste tile: V                    ",{32,176},window);
    HUDdisplay.showTextDEBUG("  Set Pos1: F3                     ",{32,192},window);
    HUDdisplay.showTextDEBUG("  Set Pos2: F4                     ",{32,208},window);
    HUDdisplay.showTextDEBUG("  Forced fill: Left Shift + F      ",{32,224},window);
    HUDdisplay.showTextDEBUG("                                   ",{32,240},window);
    HUDdisplay.showTextDEBUG("  Press F6 to exit                 ",{32,256},window);
    HUDdisplay.showTextDEBUG("                                   ",{32,272},window);
    if (message_timer<=60){
        HUDdisplay.showTextDEBUG(output_message,{0,560},window);
    };
    window.display();
    full_loaded=1;
    }while(true);
    return 0;
};

std::vector<std::vector<std::string>> formatRock(std::vector<std::vector<std::string>> terrain_vector){

    std::vector<std::string> rocks = {"\\x15","\\x20","\\x10","\\x11","\\x22","\\x12","\\x13","\\x14","\\x17","\\x18","\\x19","\\x21","\\x23","\\x24","\\x25"};

    for (int i = 0; i <terrain_vector.size();i++){
        for (int j = 0; j <terrain_vector[0].size();j++){
            //DETECTS ROCK
            if(std::find(rocks.begin(), rocks.end(), terrain_vector[i][j]) != rocks.end()) {
                
                //std::cout << "rock detected at" << j << "|" << i << std::endl;
                
                bool rockbl=false;
                bool rockl=false;
                bool rocktl=false;
                bool rockt=false;
                bool rocktr=false;
                bool rockr=false;
                bool rockbr=false;
                bool rockb=false;
                bool tedge=false;
                bool bedge=false;
                bool ledge=false;
                bool redge=false;

                //  J = X
                //  I = Y
                
                if (i>0){//TOP
                    if(std::find(rocks.begin(), rocks.end(), terrain_vector[i-1][j]) != rocks.end()) {
                        rockt=true;
                    };
                }else{
                    tedge=true;
                };
                if (j>0){//LEFT
                    if(std::find(rocks.begin(), rocks.end(), terrain_vector[i][j-1]) != rocks.end()) {
                        rockl=true;
                    };
                }else{
                    ledge=true;
                };
                if (j<terrain_vector[0].size()-1){//RIGHT
                    if(std::find(rocks.begin(), rocks.end(), terrain_vector[i][j+1]) != rocks.end()) {
                        rockr=true;
                    };
                }else{
                    redge=true;
                };
                if (i<terrain_vector.size()-1){//BOTTOM
                    if(std::find(rocks.begin(), rocks.end(), terrain_vector[i+1][j]) != rocks.end()) {
                        rockb=true;
                    };
                }else{
                    bedge=true;
                };
                if (i>0&&j<terrain_vector[0].size()-1){//TOP RIGHT
                    if(std::find(rocks.begin(), rocks.end(), terrain_vector[i-1][j+1]) != rocks.end()) {
                        rocktr=true;
                    };
                };
                if (i>0&&j>0){//TOP LEFT
                    if(std::find(rocks.begin(), rocks.end(), terrain_vector[i-1][j-1]) != rocks.end()) {
                        rocktl=true;
                    };
                };
                if (i<terrain_vector.size()-1&&j<terrain_vector[0].size()-1){//BOTTOM RIGHT
                    if(std::find(rocks.begin(), rocks.end(), terrain_vector[i+1][j+1]) != rocks.end()) {
                        rockbr=true;
                    };
                };
                if (i<terrain_vector.size()-1&&j>0){//BOTTOM LEFT
                    if(std::find(rocks.begin(), rocks.end(), terrain_vector[i+1][j-1]) != rocks.end()) {
                        rockbl=true;
                    };
                };

                terrain_vector[i][j]="\\x15";

                if (rockt&&!rockb&&(rockr||rockl)&&!bedge){
                    terrain_vector[i][j]="\\x22";//rock is going down
                };
                if (rockl&&(rockt||rockb)&&!rockr&&!redge){
                    terrain_vector[i][j]="\\x10";//rock is going right
                };
                if (rockt&&rockl&&!rockr&&!rockb&&!bedge&&!tedge&&!redge&&!ledge){
                    terrain_vector[i][j]="\\x21";//rock is in interior corner top left
                };
                if (rockr&&rockb&&!rockbr){
                    terrain_vector[i][j]="\\x11";//rock is in exterioir corner bottom right
                };
                //if (rockr&&rockb&&!rockbr){
                //    terrain_vector[i][j]="\\x11"; //rock is going up
                //};
                //if (rockr&&rockb&&!rockt&&!rockl){
                //    terrain_vector[i][j]="\\x23"; //rock is going up left
                //};
            };
        };   
    };
    return terrain_vector;
};

std::string askPath(std::string path,sf::Event event,sf::RenderWindow& window){
    std::vector<std::string> maps;
    std::vector<std::string> full_path;
    std::string file;
    if (path[path.length()-1]=='.'){
        path.pop_back();
    };
    if (path!="/"){
    maps.push_back("../");
    full_path.push_back("../");
    };
    for (const auto & entry : std::filesystem::directory_iterator(path)){
        std::string fish;
        fish=entry.path();
        //KEEP ONLY .LV FILES AND DIRECTORIES
        if (fish.substr(fish.find_last_of(".") + 1) == "lv"||std::filesystem::is_directory(fish)) {
            maps.push_back(entry.path().filename());
            full_path.push_back(entry.path());
        };
        std::sort(maps.begin(),maps.end());
        std::sort(full_path.begin(),full_path.end());
    };
    int map_choice = 0;
    int map_offset = 0;
    do{
    full_loaded=0;
    while (window.pollEvent(event))
    {
        switch (event.type) {
            case sf::Event::KeyPressed:
                screenshotVerify();
                if (event.key.code==sf::Keyboard::Up){
                    if (map_choice>0){
                        map_choice--;
                    }else{
                        map_choice=maps.size()-1;
                    };
                };
                if (event.key.code==sf::Keyboard::Down){
                    if (map_choice<maps.size()-1){
                        map_choice++;
                    }else{
                        map_choice=0;
                    };
                    break;
                };
                if (event.key.code==sf::Keyboard::Left){
                    map_choice=0;
                };
                if (event.key.code==sf::Keyboard::Right){
                    map_choice=maps.size()-1;
                };
                if (event.key.code==sf::Keyboard::F5||event.key.code==sf::Keyboard::X){
                    if (std::filesystem::is_directory(full_path[map_choice])){
                        if (maps[map_choice]!="../"){
                            path=path+maps[map_choice]+"/";
                        }else{
                            if (path!="/")
                            path=functions.goParentFolder(path);
                        };
                        maps={};
                        full_path={};
                        if (path!="/"){
                        maps.push_back("../");
                        full_path.push_back("../");
                        };
                        for (const auto & entry : std::filesystem::directory_iterator(path)){
                            std::string fish;
                            fish=entry.path();
                            //KEEP ONLY .LV FILES AND DIRECTORIES
                            if(fish.substr(fish.find_last_of(".") + 1) == "lv"||std::filesystem::is_directory(fish)) {
                                maps.push_back(entry.path().filename());
                                full_path.push_back(entry.path());
                            };
                        };
                        std::sort(maps.begin(),maps.end());
                        std::sort(full_path.begin(),full_path.end());
                    map_choice=0;
                    }else{
                    file=full_path[map_choice];
                    return file;
                    };
                }
            };
        if (event.type == sf::Event::Closed){
            functions.quitGame(window);
            return 0;
        };
    };
    if (isPressed(event,sf::Keyboard::F6)==0)
    return "";

    //FOR NAVIGATE TROUGHT DIRS

    //SCROLLING IN MAPS
    if (map_choice>12){
        map_offset=map_choice-12;
    }else{
        map_offset=0;
    };

    window.clear(sf::Color(255,255,255));
    HUDdisplay.showTextDEBUG("Pokemon GSC Online build "+std::to_string(version).substr(0,4),{0,0},window);
    HUDdisplay.showTextDEBUG("27/05/24",{0,16},window);
    HUDdisplay.showTextDEBUG("F5 or X to confirm, F6 to cancel",{0,32},window);

    //SHOWING MAX 13 MAPS
    for (int i = 0; i<maps.size();i++){
        if (std::filesystem::is_directory(full_path[i+map_offset])&&full_path[i+map_offset]!="../"){
            HUDdisplay.showTextBOW(maps[i+map_offset]+"/",{96,128+(i*32)},window);
        }else{
            HUDdisplay.showTextBOW(maps[i+map_offset],{96,128+(i*32)},window);
        };
        if (i>11)break;
    };
    HUDdisplay.showCursor({64,128+(map_choice*32)-(map_offset*32)},window);
    HUDdisplay.showTextBOW("CUR DIRECTORY ("+std::to_string(maps.size()-1)+")",{64,64},window);
    if (message_timer<=60){
        HUDdisplay.showTextDEBUG(output_message,{0,560},window);
    };
    window.display();
    full_loaded=1;
    }while(true);
};

int playerMenu(){
    do{
        full_loaded=0;
        while (window.pollEvent(event)){
            if (event.type==sf::Event::KeyPressed){
                screenshotVerify();
                if (event.key.code==sf::Keyboard::F6||event.key.code==sf::Keyboard::Escape){
                    return 0;
                };
                if (event.key.code==sf::Keyboard::Left){
                    if(trainer_app_id>0){
                        trainer_app_id--;
                    };
                };
                if (event.key.code==sf::Keyboard::Right){
                    if(trainer_app_id<TrainersIndex.trainers_index.size()-1){
                        trainer_app_id++;
                    };
                };
                updateSettings(false);
            };
        };
        window.clear();
        HUDdisplay.showMenu(window,"assets/menus/playerStatus.mn");
        HUDdisplay.showTextBOW("NAME/"+username,{64,64},window);
        TrainersIndex.showTrainer(window,{416,32},trainer_app_id,1);
        //HUDdisplay.showMenuTile(window,"\xe0",{608,0});
        //HUDdisplay.showMenuTile(window,"\xe0",{608,32});
        //HUDdisplay.showMenuTile(window,"\xe0",{608,64});
        //HUDdisplay.showMenuTile(window,"\xe0",{608,96});
        //HUDdisplay.showMenuTile(window,"\xe0",{608,128});
        //HUDdisplay.showMenuTile(window,"\xe0",{608,160});
        //HUDdisplay.showMenuTile(window,"\xe0",{608,192});
        //HUDdisplay.showMenuTile(window,"\xe0",{608,224});
        //HUDdisplay.showMenuTile(window,"\xe0",{608,256});
        window.display();
        full_loaded=1;
    }while(true);
    return 0;
};

int levelEditorLoop(sf::RenderWindow&window,sf::Event event){
    functions.log("EDITOR","Launching editor");

    std::vector<std::string> actions;//actions list for they are  displayed in the editor menu
    std::vector<int> player_pos = {0,0};//Begins at top left of the map
    std::vector<int> player_offset = {290,256};//Yup, that's a strange offset but I like it
    
    pos1;// init pos1 for filling and other stuff here
    pos2;// init pos2 for filling and other stuff here

    pos1set=false;//used for verify if pos1 is set
    pos2set=false;//used for verify if pos2 is set

    bool help_tiles_show=false;//for display or not tiles viewer
    output_message="Welcome to level editor";//a welcome message when you launch level editor
    message_timer=0;//put the message timer to 0 so it displays the message
    
    int choice=0;//set the choice to 0 so it's by default at top of editor menu
    int map_height=9;//default height is same height as a map that would fit a gameboy screen
    int map_width=10;//default width is same width as a map that would fit a gameboy screen
    int camera_speed=8;//camera speed can be increased in editor menu so you are faster or more precise
    bool menu_show=false;//simply to show or not the menu, when the menu is shown, you can't move on terrain, you just navigate in the menu
    time_otd='d';//puts the time to day
    int default_tile_index=0;//puts the default terrain tile to grass (\x00)
    int selected_tile_index=0;//puts the selected tile to grass (\x00)
    int copied_tile_index=0;//puts the copied tile to grass (\x00)

    std::vector<std::vector<std::string>> terrain_vector;//same as in-game version of it, functions are almost all used the same in level editor and in-game
    actions.push_back("Camera speed");//0
    actions.push_back("Map width");//1
    actions.push_back("Map height");//2
    actions.push_back("Default tile");//3
    actions.push_back("Select tile");//4
    actions.push_back("Load map");//5
    actions.push_back("Save map");//6
    actions.push_back("Fill square");//7
    actions.push_back("Options");//8
    actions.push_back("Smart Rock");//9
    actions.push_back("Set Random Pattern");//10
    actions.push_back("Controls");//11
    terrain_vector={};//reset the terrain vector and below, fills it with grass
    for (int i = 0;i<map_height;i++){
        terrain_vector.push_back({});
        for (int j = 0;j<map_width;j++){
            terrain_vector[i].push_back("\\x00");
        };
    };

    functions.log("EDITOR","Editor launched!");//logs in the console that the editor is launched
    
    do{
        full_loaded=0;
        message_timer++;//increase the message timer for display messages
        while (window.pollEvent(event))//level editor true loop
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
                if (event.type==sf::Event::KeyPressed){
                    if (event.key.code==sf::Keyboard::B){
                        if (HUDdisplay.editor_bg_index==HUDdisplay.editor_background.size()-1){
                            HUDdisplay.editor_bg_index=0;
                        }else{
                            HUDdisplay.editor_bg_index++;
                        };
                    }else if(event.key.code==sf::Keyboard::O){
                        std::string wanted_map=askPath(std::filesystem::absolute("."),event,window);
                        if (wanted_map!=""){
                            cur_map=wanted_map;
                            functions.log("EDITOR","Loaded "+cur_map+" map");
                            terrain_vector=terrain.terrainForm(terrain_vector,cur_map);
                            map_height=terrain_vector.size();
                            int init_size=terrain_vector[0].size();
                            for (int i=0;i<map_height;i++){
                                if (terrain_vector[i].size()<init_size){
                                init_size=terrain_vector[i].size();
                            };
                            map_width=init_size;
                            };
                        };
                    }else if (event.key.code==sf::Keyboard::S){
                        std::string save_map_path=functions.getUserPath()+"/.gsc_o/maps/"+askText("Map name?")+".lv";
                        functions.saveMap(terrain_vector,save_map_path); 
                        if (!std::filesystem::is_regular_file(save_map_path)){
                            output_message="Failed to save map!";
                            functions.log("ERROR","Failed to save map at "+save_map_path+".\nYou should verify if you have the rights to save in this path.");
                        }else{
                            output_message="Map saved at "+save_map_path;
                            functions.log("EDITOR","Map saved at "+save_map_path);
                        }
                        message_timer=0;
                    };
                };
            };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
                if (event.type==sf::Event::KeyPressed){
                    if (event.key.code==sf::Keyboard::R){
                        functions.renderMap(terrain_vector,Tiles);
                    };
                };
            };
            if (event.type==sf::Event::KeyPressed){
                screenshotVerify();
                if (event.key.code==sf::Keyboard::RShift){
                    menu_show=!menu_show;
                };
                

                if (menu_show){

                    if(event.key.code==sf::Keyboard::T){
                        help_tiles_show=!help_tiles_show;//show/hide the tile viewer
                    }else if (event.key.code==sf::Keyboard::F5||event.key.code==sf::Keyboard::X){
                        if (choice==0){
                            std::string camera_ask;
                            int camera_convert;
                            try{
                                camera_ask=askText("Camera speed?");
                                if (camera_ask==""){
                                    break;
                                }
                                camera_convert = std::stoi(camera_ask);
                                camera_speed=camera_convert;
                            }catch (std::exception &err){
                                functions.log("ERROR","Can't convert '"+camera_ask+"' to int.");
                                message_timer=0;
                                output_message="Can't convert result to int";
                            };
                        }else if (choice==1){
                            std::string width_ask;
                            int width_convert;
                            try{
                                width_ask=askText("Map width?");
                                if (width_ask==""){
                                    break;
                                }
                                width_convert = std::stoi(width_ask);
                                if (width_convert<=0){//REFUSE NULL OR NEGATIVE VALUE
                                    functions.log("ERROR","Can't enter a negative or null value as map width");
                                    message_timer=0;
                                    output_message="Can't enter a negative value, or 0.";
                                    break;
                                };
                                if (map_width<width_convert){
                                    while(map_width!=width_convert){//ADDING WIDTH
                                        map_width++;
                                        for (int i=0;i<terrain_vector.size();i++){
                                            terrain_vector[i].push_back(Tiles.tiles[default_tile_index][2]);
                                        };
                                    };
                                }else if (map_width>width_convert){
                                    while(map_width!=width_convert){//REMOVING WIDTH
                                        map_width--;
                                        for (int i = 0;i<map_height;i++){
                                        terrain_vector[i].pop_back();
                                        };
                                    };
                                };
                            }catch (std::exception &err){
                                functions.log("ERROR","Can't convert '"+width_ask+"' to int.");
                                message_timer=0;
                                output_message="Can't convert result to int";
                            };
                        }else if (choice==2){
                            std::string height_ask;
                            int height_convert;
                            try{
                                height_ask=askText("Map height?");
                                if (height_ask==""){
                                    break;
                                };
                                height_convert = std::stoi(height_ask);
                                if (height_convert<=0){//REFUSE NULL OR NEGATIVE VALUE
                                    functions.log("ERROR","Can't enter a negative or null value as map height");
                                    message_timer=0;
                                    output_message="Can't enter a negative value, or 0.";
                                    break;
                                };
                                if (map_height<height_convert){
                                    while(map_height!=height_convert){//ADDING HEIGHT
                                        map_height++;
                                        std::vector<std::string> terrain_line;
                                        for (int i=0;i<terrain_vector[0].size();i++){
                                            terrain_line.push_back(Tiles.tiles[default_tile_index][2]);
                                        };
                                        terrain_vector.push_back(terrain_line);
                                    };
                                }else if (map_height>height_convert){
                                    while(map_height!=height_convert){//REMOVING HEIGHT
                                        map_height--;
                                        terrain_vector.pop_back();
                                    };
                                };
                            }catch (std::exception &err){
                                functions.log("ERROR","Can't convert '"+height_ask+"' to int.");
                                message_timer=0;
                                output_message="Can't convert result to int";
                            };
                        }else if (choice==3){
                            std::string tile_ask;
                            tile_ask=askText("Tile? \n(name or index)");
                            for (int i=0;i<Tiles.tiles.size()-1;i++){
                                if (tile_ask==Tiles.tiles[i][2]){// grass
                                    default_tile_index=i;
                                    break;
                                }else if (tile_ask==Tiles.tiles[i][0]){// \x00
                                    default_tile_index=i;
                                    break;
                                };
                            };
                        }else if (choice==5){
                            std::string wanted_map=askPath(std::filesystem::absolute("."),event,window);
                            if (wanted_map!=""){
                                cur_map=wanted_map;
                                functions.log("EDITOR","Loaded "+cur_map+" map");
                                terrain_vector=terrain.terrainForm(terrain_vector,cur_map);
                                map_height=terrain_vector.size();
                                int init_size=terrain_vector[0].size();
                                for (int i=0;i<map_height;i++){
                                    if (terrain_vector[i].size()<init_size){
                                    init_size=terrain_vector[i].size();
                                };
                                map_width=init_size;
                                };
                            };
                        }else if (choice==6){
                            std::string save_map_path=functions.getUserPath()+"/.gsc_o/maps/"+askText("Map name?")+".lv";
                            functions.saveMap(terrain_vector,save_map_path);
                            if (!std::filesystem::is_regular_file(save_map_path)){
                                output_message="Failed to save map!";
                                functions.log("ERROR","Failed to save map at "+save_map_path+".\nYou should verify if you have the rights to save in this path.");
                            }else{
                                output_message="Map saved at "+save_map_path;
                                message_timer=0;
                                functions.log("EDITOR","Map saved at "+save_map_path);
                            };
                        }else if (choice==7){

                        }else if(choice==9){
                            functions.log("EDITOR","Format rock");
                            terrain_vector=formatRock(terrain_vector);
                        }else if(choice==10){
                            randomPatternLoop();
                        }else if (choice==11){
                            showEditorControls();
                        };
                    }else if (event.key.code==sf::Keyboard::Left){
                        if (choice==0){
                            if (camera_speed>1){
                                camera_speed--;
                            };
                        }else if(choice==1){
                            if (map_width>1){
                                map_width--;
                                for (int i = 0;i<map_height;i++){
                                terrain_vector[i].pop_back();
                                };
                            };
                        }else if(choice==2){
                            if (map_height>1){
                                map_height--;
                                terrain_vector.pop_back();
                            };
                        }else if(choice==3){
                            if (default_tile_index>0){
                                default_tile_index--;
                            }else{
                                default_tile_index=Tiles.tiles.size()-2;
                            };
                        }else if(choice==4){
                            if (selected_tile_index>0){
                                selected_tile_index--;
                            }else{
                                selected_tile_index=Tiles.tiles.size()-2;
                            }
                        };
                    }else if (event.key.code==sf::Keyboard::Right){
                        if (choice==0){
                            camera_speed++;
                        }else if(choice==1){
                            map_width++;
                            for (int i=0;i<terrain_vector.size();i++){
                                terrain_vector[i].push_back(Tiles.tiles[default_tile_index][2]);
                            };
                        }else if(choice==2){
                            map_height++;
                            std::vector<std::string> terrain_line;
                            for (int i=0;i<terrain_vector[0].size();i++){
                                terrain_line.push_back(Tiles.tiles[default_tile_index][2]);
                            };
                            terrain_vector.push_back(terrain_line);
                        }else if(choice==3){
                            if (default_tile_index==Tiles.tiles.size()-2){
                                default_tile_index=0;
                            }else{
                                default_tile_index++;
                            };
                        }else if(choice==4){
                            if (selected_tile_index==Tiles.tiles.size()-2){
                                selected_tile_index=0;
                            }else{
                                selected_tile_index++;
                            };
                        };
                    }else if (event.key.code==sf::Keyboard::Down){
                        if (choice==actions.size()-1){
                            choice=0;
                        }else{
                            choice++;
                        };
                    }else if (event.key.code==sf::Keyboard::Up){
                        if (choice==0){
                            choice=actions.size()-1;
                        }else{
                            choice--;
                        }
                    };

                }else{ // END OF MENU SHOW

                    if (event.key.code==sf::Keyboard::F3){
                        int real_pos_x=player_pos[0]/64;
                        int real_pos_y=player_pos[1]/64;
                        pos1 = {real_pos_x,real_pos_y};
                        output_message="Pos 1 set to "+std::to_string(pos1[0])+":"+std::to_string(pos1[1]);
                        pos1set=true;
                        message_timer=0;
                    }else if (event.key.code==sf::Keyboard::F4){
                        int real_pos_x=player_pos[0]/64;
                        int real_pos_y=player_pos[1]/64;
                        pos2 = {real_pos_x,real_pos_y};
                        output_message="Pos 2 set to "+std::to_string(pos1[0])+":"+std::to_string(pos1[1]);
                        pos2set=true;
                        message_timer=0;
                    }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){//for forced fill verify first if left shift is pressed
                        if (event.key.code==sf::Keyboard::F){
                            if (pos1set&&pos2set){//verify if pos1 and pos2 is set
                            for (int i = 0; i<terrain_vector.size();i++){
                                for (int j = 0; j<terrain_vector[i].size();j++){
                                    if ((pos1[0]<=j&&j<=pos2[0])&&(pos1[1]<=i&&i<=pos2[1])){// if the tile is in the pos1/pos2 zone
                                        terrain_vector[i][j]=Tiles.tiles[selected_tile_index][2];//then its replaced with the selected tile
                                        };
                                    };
                                };
                                output_message="the selected area has been filled!";//here's the output message
                                message_timer=0;//then timer reset
                            }else{
                            output_message="You need to set Pos1 and Pos2 first!";//in case pos1 and/or pos2 arent set
                            message_timer=0;//reset timer
                            };
                        };
                    };
                };//END IF !menu_show
            };//ENF OF KEYPRESSED
            if (event.type == sf::Event::Closed){
                functions.quitGame(window);
                return 0;
            };
        };//END OF POLYEVENTS

        if (event.key.code==sf::Keyboard::C&&!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
            int real_pos_x=player_pos[0]/64;
            int real_pos_y=player_pos[1]/64;
            copied_tile=terrain_vector[real_pos_y][real_pos_x];
            output_message="Copied "+Tiles.getTileName(terrain_vector[real_pos_y][real_pos_x])+" ("+terrain_vector[real_pos_y][real_pos_x]+")";
            message_timer=0;
        };

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)){
            if (copied_tile!=""){
                int real_pos_x=player_pos[0]/64;
                int real_pos_y=player_pos[1]/64;
                terrain_vector[real_pos_y][real_pos_x]=copied_tile;
                output_message="Pasted "+Tiles.getTileName(terrain_vector[real_pos_y][real_pos_x])+" ("+terrain_vector[real_pos_y][real_pos_x]+")";
                message_timer=0;
            }else{
                output_message="Nothing has been copied yet";
                message_timer=0;
            };
        };

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
            if (!menu_show){
                int real_pos_x=player_pos[0]/64;
                int real_pos_y=player_pos[1]/64;
                output_message="Removed "+Tiles.getTileName(terrain_vector[real_pos_y][real_pos_x])+" ("+terrain_vector[real_pos_y][real_pos_x]+")";
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
                    terrain_vector[real_pos_y][real_pos_x]="\\xff";
                }else{
                    terrain_vector[real_pos_y][real_pos_x]=Tiles.tiles[default_tile_index][2];
                }
                message_timer=0;
            };
        };

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)){
            int real_pos_x=player_pos[0]/64;
            int real_pos_y=player_pos[1]/64;
            terrain_vector[real_pos_y][real_pos_x]=Tiles.tiles[selected_tile_index][2];
            output_message="Placed "+Tiles.getTileName(terrain_vector[real_pos_y][real_pos_x])+" ("+terrain_vector[real_pos_y][real_pos_x]+")";
            message_timer=0;
        };

        if(!menu_show){
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            player_pos[0]+=camera_speed;
            };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            player_pos[0]-=camera_speed;
            };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            player_pos[1]-=camera_speed;
            };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            player_pos[1]+=camera_speed;
            };
        };

        if (player_pos[1]<0){
            player_pos[1]=0;
        };
        if (player_pos[1]>terrain_vector.size()*64-64){
            player_pos[1]=terrain_vector.size()*64-64;
        };
        if (player_pos[0]<0){
            player_pos[0]=0;
        };
        if (player_pos[0]>terrain_vector[0].size()*64-64){
            player_pos[0]=terrain_vector[0].size()*64-64;
        };

        window.clear(sf::Color(148,148,148));
        HUDdisplay.editorBG(window);
        terrain.showTerrain(terrain_vector,player_pos,time_otd,window,player_offset,true,index_frame);
        
        if (pos1set){
            HUDdisplay.showTextBluDEBUG("pos1",{pos1[0]*64-player_pos[0]+player_offset[0],pos1[1]*64-player_pos[1]+player_offset[1]},window);
        };
        if (pos2set){
            HUDdisplay.showTextBluDEBUG("pos2",{pos2[0]*64-player_pos[0]+player_offset[0],pos2[1]*64-player_pos[1]+player_offset[1]},window);
        };

        HUDdisplay.showTextDEBUG("Editor Mode "+std::to_string(version).substr(0,4),{0,0},window);
        if (menu_show&&!help_tiles_show){
            HUDdisplay.showTextDEBUG("Press Right Shift to show / hide menu",{0,16},window);
            actions[0]="Camera speed: "+std::to_string(camera_speed);
            actions[1]="Map width: "+std::to_string(map_width);
            actions[2]="Map height: "+std::to_string(map_height);
            actions[3]="Default tile: "+Tiles.tiles[default_tile_index][0]+" ("+Tiles.tiles[default_tile_index][2]+")";
            actions[4]="Selected tile: "+Tiles.tiles[selected_tile_index][0]+" ("+Tiles.tiles[selected_tile_index][2]+")";
            for (int i = 0 ; i < actions.size();i++){
                if (choice==i){
                    HUDdisplay.showTextSELDEBUG(actions[i],{0,32+(i*16)},window);
                }else{
                    HUDdisplay.showTextDEBUG(actions[i],{0,32+(i*16)},window);
                };
            };
        }else if(!help_tiles_show){
            HUDdisplay.showTextDEBUG(std::to_string(terrain_vector.size()*terrain_vector[0].size())+" tiles",{0,16},window);
            HUDdisplay.showTextDEBUG("X: "+std::to_string(player_pos[0]/64)+" Y: "+std::to_string(player_pos[1]/64),{0,32},window);
            if (!pos1set||!pos2set){
                HUDdisplay.showTextDEBUG("You need to set pos1 and pos2,\nto see the square size.",{0,64},window);
            }else{
                int x_size=0;
                int y_size=0;
                x_size=pos2[0]-pos1[0]+1;
                y_size=pos2[1]-pos1[1]+1;
                HUDdisplay.showTextDEBUG("Select square size: "+std::to_string(x_size*y_size)+" tiles",{0,64},window);
            };
            HUDdisplay.showTextDEBUG("Hovering tile: "+Tiles.getTileName(terrain_vector[player_pos[1]/64][player_pos[0]/64])+" ("+terrain_vector[player_pos[1]/64][player_pos[0]/64]+")",{0,48},window);
        }else{
            char space=' ';
            HUDdisplay.showTextDEBUG("                              ",{32,32},window);
            HUDdisplay.showTextDEBUG("   Tiles viewer               ",{32,48},window);
            HUDdisplay.showTextDEBUG("                              ",{32,64},window);
            HUDdisplay.showTextDEBUG(" Default terrain tile:        ",{32,80},window);
            HUDdisplay.showTextDEBUG(" "+Tiles.tiles[default_tile_index][0]+" ("+Tiles.tiles[default_tile_index][2]+")"+ // FILLING EMPTY SPACE
            std::string(29-(Tiles.tiles[default_tile_index][0]+" ("+Tiles.tiles[default_tile_index][2]+")").length(),space),{32,96},window);
            HUDdisplay.showTextDEBUG("                              ",{32,112},window);
            HUDdisplay.showTextDEBUG("                              ",{32,128},window);
            Tiles.showTile(window,default_tile_index,{64,112},time_otd);
            HUDdisplay.showTextDEBUG("                              ",{32,144},window);
            HUDdisplay.showTextDEBUG("                              ",{32,160},window);
            HUDdisplay.showTextDEBUG("                              ",{32,176},window);
            HUDdisplay.showTextDEBUG(" Selected tile:               ",{32,192},window);
            HUDdisplay.showTextDEBUG(" "+Tiles.tiles[selected_tile_index][0]+" ("+Tiles.tiles[selected_tile_index][2]+")"+ // FILLING EMPTY SPACE
            std::string(29-(Tiles.tiles[selected_tile_index][0]+" ("+Tiles.tiles[selected_tile_index][2]+")").length(),space),{32,208},window);
            HUDdisplay.showTextDEBUG("                              ",{32,224},window);
            HUDdisplay.showTextDEBUG("                              ",{32,240},window);
            Tiles.showTile(window,selected_tile_index,{64,224},time_otd);
            HUDdisplay.showTextDEBUG("                              ",{32,256},window);
            HUDdisplay.showTextDEBUG("                              ",{32,272},window);
            HUDdisplay.showTextDEBUG(" Copied tile:                 ",{32,288},window);
            HUDdisplay.showTextDEBUG(" "+Tiles.tiles[copied_tile_index][0]+" ("+Tiles.tiles[copied_tile_index][2]+")"+ // FILLING EMPTY SPACE
            std::string(29-(Tiles.tiles[copied_tile_index][0]+" ("+Tiles.tiles[copied_tile_index][2]+")").length(),space),{32,304},window);
            HUDdisplay.showTextDEBUG("                              ",{32,320},window);
            HUDdisplay.showTextDEBUG("                              ",{32,336},window);
            Tiles.showTile(window,copied_tile_index,{64,320},time_otd);
            HUDdisplay.showTextDEBUG("                              ",{32,352},window);
            HUDdisplay.showTextDEBUG("                              ",{32,368},window);
            HUDdisplay.showTextDEBUG("      Press F6 to exit        ",{32,384},window);
            HUDdisplay.showTextDEBUG("                              ",{32,400},window);
            
            if (event.key.code==sf::Keyboard::F6){
                help_tiles_show=false;
            };
        };
        if (message_timer<=60){
            HUDdisplay.showTextDEBUG(output_message,{0,560},window);
        };
        //HUDdisplay.drawSquare(window,{0,352},{640,224});
        window.display();
        full_loaded=1;
    }while(true);
    return 0;
};

int modManager(){
    int choice=0;
    int offset=0;
    while (true){
        full_loaded=0;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                functions.quitGame(window);
                return 0;
            };
            if (event.type == sf::Event::KeyPressed){
                screenshotVerify();
                if (event.key.code==sf::Keyboard::F6){
                    return 0;
                };
                if (event.key.code==sf::Keyboard::Up){
                    if (choice>0){
                        choice--;
                    };
                };
                if (event.key.code==sf::Keyboard::Down){
                    if (choice<mods_list.size()-1){
                        choice++;
                    };
                };
            };
        };
        window.clear(sf::Color(255,255,255));
        for (int i = 0; i<mods_list.size();i++){
            if (i>5)break;
            if (mods_states[i]==true){
                HUDdisplay.showTextBOW(mods_descriptions[i+offset][2],{64,96+(i*32)},window);
            }else{
                HUDdisplay.showTextWOB(mods_descriptions[i+offset][2],{64,96+(i*32)},window);
            };
        };
        if (choice>=5){
            offset=choice-5;
        }else{
            offset=0;
        };
        HUDdisplay.showTextBOW("Mods ("+std::to_string(mods_list.size())+")",{32,32},window);
        HUDdisplay.showCursor({32,96+(choice-offset)*32},window);
        sf::RectangleShape rectangle(sf::Vector2f(720, 256));
        rectangle.setFillColor(sf::Color(0,0,0));
        rectangle.setPosition({0,320});
        window.draw(rectangle);
        HUDdisplay.showTextDEBUG("Author:",{0,320},window);
        HUDdisplay.showTextBluDEBUG(mods_descriptions[choice][1],{180,320},window);
        HUDdisplay.showTextDEBUG("Description:",{0,336},window);
        HUDdisplay.showTextDEBUG(mods_descriptions[choice][0],{16,352},window);
        window.display();
        full_loaded=1;
    };
    return 0;
};

int main_menu(){
    int debug_x;
    debug_x=window.getSize().x-160;
    int choice = 0;
    bool choosen=false;
    int max_choice;
    if (mods_list.size()>0){
        max_choice=4;
    }else{
        max_choice=3;
    };
    functions.log("INFO","Opening main menu");
    do{
        full_loaded=0;
        while (window.pollEvent(event))
        {
            switch (event.type) {
            case sf::Event::KeyPressed:
                screenshotVerify();
                if (event.key.code==sf::Keyboard::Up){
                        if (choice>0){
                            choice--;
                        };
                        break;
                };
                if (event.key.code==sf::Keyboard::Down){
                    if (choice<max_choice){
                        choice++;
                    };
                    break;
                };
                if (event.key.code==sf::Keyboard::F5||event.key.code==sf::Keyboard::X){
                    if (choice==0){
                        levelEditorLoop(window,event);
                        return 0;
                    };
                    if (choice==1){
                        choosen=true;
                        std::string path = "maps/route1.lv";
                        cur_map=path;
                        setTerrain();
                        return 0;
                    }
                    if (choice==2){
                        std::string map_path;
                        map_path = askPath(std::filesystem::absolute("."),event,window);
                        if(map_path!=""){
                            cur_map=map_path;
                            setTerrain();
                            return 0;
                        };
                    };
                    if (choice==3){
                        if (std::filesystem::is_directory(functions.getUserPath()+"/.gsc_o/")){ 
                            functions.log("INFO","Opening game folder.");
                            #if __linux__
                                system(("xdg-open "+functions.getUserPath()+"/.gsc_o/").c_str());
                            #elif _WIN32
                                system(("start   "+functions.getUserPath()+"/.gsc_o/").c_str());
                            #endif
                        };
                    };
                    if (choice==4){
                        modManager();
                    };
                };
            };
            if (event.type == sf::Event::Closed){
                functions.quitGame(window);
                return 0;
            };
            
        };
        index_frame++;
        window.clear(sf::Color(255,255,255));
        
        HUDdisplay.showLogo({32,0},window);

        //TEXTS DISPLAY
        HUDdisplay.showTextDEBUG("Pokemon GSC Online build "+std::to_string(version).substr(0,4),{0,0},window);
        HUDdisplay.showTextDEBUG("27/05/24",{0,16},window);
        HUDdisplay.showTextDEBUG("F5 or X to confirm, F6 to cancel",{0,32},window);
        if (mods_list.size()==0){
            HUDdisplay.showTextDEBUG("no mods loaded",{0,48},window);
        }else if (mods_list.size()==1){
            HUDdisplay.showTextDEBUG("1 mod loaded",{0,48},window);
        }else{
            HUDdisplay.showTextDEBUG(std::to_string(mods_list.size())+" mods loaded",{0,48},window);
        }
        HUDdisplay.showCursor({32,288+(choice*32)},window);
        HUDdisplay.showTextBOW("Level Editor",{64,288},window);
        HUDdisplay.showTextBOW("Launch game",{64,320},window);
        HUDdisplay.showTextBOW("Load map from dir",{64,352},window);
        HUDdisplay.showTextBOW("Open game folder",{64,384},window);
        if (mods_list.size()>0){
        HUDdisplay.showTextBOW("Mods",{64,416},window);
        };
        if (message_timer<=60){
            HUDdisplay.showTextDEBUG(output_message,{0,560},window);
        };
        //if (debug_launch)
        //    HUDdisplay.showTextWOB("DEBUG",{debug_x,0},window);
        window.display();
        full_loaded=1;
        if (choosen)return 0;
    }
    while(true);
    return 0;
};

int updateSettings(bool advert){
    std::string init_username;
    std::string init_resolution;
    std::string init_border;
    std::string init_trainer;

    std::string final_content;
    std::ofstream ofile(functions.getUserPath()+"/.gsc_o/settings.temp");
    std::ifstream ifile(functions.getUserPath()+"/.gsc_o/settings");
    std::string line;
    while (getline(ifile, line)){
        if (line.rfind("username=",0)==0){
            init_username=functions.ReplaceAll(line,"username=","");
        }else if(line.rfind("resolution=",0)==0){
            init_resolution=functions.ReplaceAll(line,"resolution=","");
        }else if(line.rfind("border=",0)==0){
            init_border=functions.ReplaceAll(line,"border=","");
        }else if(line.rfind("trainer=",0)==0){
            init_trainer=functions.ReplaceAll(line,"trainer=","");
        };
        final_content.append(line+"\n");
    };
    final_content=functions.ReplaceAll(final_content,init_username,username);
    final_content=functions.ReplaceAll(final_content,init_resolution,real_res);
    final_content=functions.ReplaceAll(final_content,init_border,std::to_string(border_style));
    final_content=functions.ReplaceAll(final_content,init_trainer,std::to_string(trainer_app_id));
    ofile<<final_content;
    std::string instruction;
    instruction="mv "+functions.getUserPath()+"/.gsc_o/settings.temp "+functions.getUserPath()+"/.gsc_o/settings";
    system(instruction.c_str());
    instruction="rm -f "+functions.getUserPath()+"/.gsc_o/settings.temp -f";
    system(instruction.c_str());
    if (advert) 
    functions.log("INFO","Settings updated");
    return 0;
};

int optionMenu(){
    int choice=0;
    do{
        full_loaded=0;
        moving_timer=20;
        walking=false;
        while (window.pollEvent(event))
        {
            if (event.type==sf::Event::KeyPressed){
                screenshotVerify();
                if (event.key.code==sf::Keyboard::Down){
                    if (choice==7){
                        choice=0;
                    }else{
                        choice++;
                    };
                }else if (event.key.code==sf::Keyboard::Up){
                    if (choice==0){
                        choice=7;
                    }else{
                        choice--;
                    };
                }else if (event.key.code==sf::Keyboard::Left){
                    if (choice==1){
                        if (border_style>0){
                            border_style--;
                            updateSettings(false);
                        };
                    };
                }else if (event.key.code==sf::Keyboard::Right){
                    if (choice==1){
                        if (border_style<(HUDdisplay.border_textures.size()/6)-1){
                            border_style++;
                            updateSettings(false);
                        };
                    };
                }else if (event.key.code==sf::Keyboard::F6||event.key.code==sf::Keyboard::Escape){
                    return 0;
                }else if (event.key.code==sf::Keyboard::F5){//OPTION SELECT
                    if (choice==0){//USERNAME MODIFY
                        std::string username_res;
                        std::string init_username=username;
                        username_res=askText("Username? (7 max.)");
                        if (username_res!=""){
                            username=username_res;
                            updateSettings(true);
                        };
                    }else if(choice==1){

                    }else if(choice==2){

                    }else if(choice==3){

                    }else if(choice==4){
                        
                    }else if(choice==5){
                            
                    }else if(choice==6){

                    }else if(choice==7){
                        return 0;
                    };
                };
            }else if (event.Closed){
                functions.quitGame(window);
            };
        };
        index_frame++;
        window.clear();
        int screen_width=window.getSize().x;
        HUDdisplay.drawSquare(window,{0,0},{screen_width,576},border_style);
        HUDdisplay.showTextBOW("USERNAME:\n  "+username+"\nBORDER STYLE:\n\n2\n\n3\n\n4\n\n5\n\nOPEN GITHUB REPO\n\nEXIT",{64,64},window);
        if (choice==1){
            HUDdisplay.showTextWOB(std::to_string(border_style+1),{480,128},window);
        }else{
            HUDdisplay.showTextBOW(std::to_string(border_style+1),{480,128},window);
        }
        HUDdisplay.showCursor({32,64+(choice*64)},window);
        window.display();
        full_loaded=1;
    }while(true);
    return 0;
};

int packMenu(){
    int shown_items=0;
    int pocket=0;
    int choice=0;
    int offset=0;
    std::vector<std::vector<std::string>> pocket_content;
    pocket_content={
        {"\x02",std::to_string(rand()%20+1)},
        {"\x04",std::to_string(rand()%20+1)}
    };
    do{
        full_loaded=0;
        while (window.pollEvent(event)){
            if(event.type==sf::Event::KeyPressed){
                screenshotVerify();
                if(event.key.code==sf::Keyboard::F6||event.key.code==sf::Keyboard::Escape){
                    return 0;
                };
                if(event.key.code==sf::Keyboard::Left){
                    std::cout << "left";
                    if(pocket==0){
                        pocket=3;
                    }else{
                        pocket--;
                    };
                };
                if(event.key.code==sf::Keyboard::Right){
                    if(pocket==3){
                        pocket=0;
                    }else{
                        pocket++;
                    };
                };
                if(event.key.code==sf::Keyboard::Down){
                    if(choice<pocket_content.size()-1){
                        choice++;
                        if(offset<4){offset++;};
                    };
                };
                if(event.key.code==sf::Keyboard::Up){
                    if(choice>0){
                        choice--;
                        if(offset>0){offset--;};
                    };
                };
            };
        };
        window.clear();
        HUDdisplay.showMenuTile(window,HUDdisplay.menu_tiles_indexes[15+pocket][2],{0,96});
        HUDdisplay.showMenu(window,"assets/menus/packMenu.mn");
        if(pocket==0)HUDdisplay.showMenuTile(window,"\xc1",{32,256});
        if(pocket==1)HUDdisplay.showMenuTile(window,"\xc0",{32,256});
        if(pocket==2)HUDdisplay.showMenuTile(window,"\xc2",{0,256});
        if(pocket==3)HUDdisplay.showMenuTile(window,"\xc3",{0,256});
        HUDdisplay.showRedCursor({224,64+(offset*64)},window);
        HUDdisplay.drawSquare(window,{0,384},{640,192},border_style);
        shown_items=0;
        for(int i=0;i<pocket_content.size();i++){
            int posy=96+(i*64);
            HUDdisplay.showTextBOW(ItemsIndex.items[ItemsIndex.getItemIntIndexByIndex(pocket_content[i+choice-offset][0])][2],{256,64+(i*64)},window);//SHOWING ITEM NAME
            shown_items++;
            std::string item_nb;
            item_nb=pocket_content[i+choice-offset][1];
            int xpos=640-(item_nb.length()*32);
            HUDdisplay.showTextBOW(item_nb,{xpos,posy},window);
            HUDdisplay.showMenuTile(window,"\xd6",{544,posy});
            if (shown_items>=5)break;
        };
        HUDdisplay.showTextBOW(ItemsIndex.items[ItemsIndex.getItemIntIndexByIndex(pocket_content[choice][0])][3],{32,448},window);//SHOWING ITEM DESCRIPTION
        window.display();
        full_loaded=1;
    }while(true);
};

int pauseMenu(){
    int choice=0;
    do{
        full_loaded=0;
        index_frame++;
        moving_timer=20;
        walking=false;
        while (window.pollEvent(event))
        {
            if (event.type==sf::Event::KeyPressed){
                screenshotVerify();
                if (event.key.code==sf::Keyboard::Down){
                    if (choice==7){
                        choice=0;
                    }else{
                        choice++;
                    };
                }else if (event.key.code==sf::Keyboard::Up){
                    if (choice==0){
                        choice=7;
                    }else{
                        choice--;
                    };
                }else if (event.key.code==sf::Keyboard::F6||event.key.code==sf::Keyboard::Escape){
                    return 0;
                }else if (event.key.code==sf::Keyboard::F5){//OPTION SELECT
                    if (choice==0){

                    }else if(choice==1){

                    }else if(choice==2){
                        packMenu();
                    }else if(choice==3){

                    }else if(choice==4){
                        playerMenu();
                    }else if(choice==5){

                    }else if(choice==6){
                        optionMenu();
                    }else if(choice==7){
                        return 0;
                    };
                };
            }else if (event.Closed){
                functions.quitGame(window);
            };
        };
        window.clear();
        terrain.showTerrain(terrain_vector,player_pos,time_otd,window,player_offset,(debug&&debug_page==9&&debug_choice==2),index_frame);
        player_state=player.animPlayer(player_state,moving_timer,walking);
        player.showPlayer(player_type,player_state,window);
        int screen_width=window.getSize().x;
        sf::RectangleShape rec({720,160});
        rec.setFillColor({255,255,255});
        rec.setPosition(0,416);
        window.draw(rec);
        if (choice==0){
            HUDdisplay.showTextBOW("Pokémon\n\nIndex",{0,448},window);
        }else if(choice==1){
            HUDdisplay.showTextBOW("\xde\xdf Team\n\nStatus",{0,448},window);
        }else if(choice==2){
            HUDdisplay.showTextBOW("Your\n\nbackpack",{0,448},window);
        }else if(choice==3){
            HUDdisplay.showTextBOW("Useful\n\nstuff",{0,448},window);
        }else if(choice==4){
            HUDdisplay.showTextBOW("Your\n\nstatus",{0,448},window);
        }else if(choice==5){
            HUDdisplay.showTextBOW("Save the\n\ngame",{0,448},window);
        }else if(choice==6){
            HUDdisplay.showTextBOW("Edit\n\noptions",{0,448},window);
        }else if(choice==7){
            HUDdisplay.showTextBOW("Close this\n\nmenu",{0,448},window);
        };
        HUDdisplay.drawSquare(window,{screen_width-320,0},{320,576},border_style);
        HUDdisplay.showTextBOW(username,{screen_width-256,320},window);
        HUDdisplay.showTextBOW("POKéDEX\n\nPOKéMON\n\nPACK\n\n\xee\xefGEAR\n\n\n\nSAVE\n\nOPTION\n\nEXIT",{screen_width-256,64},window);
        HUDdisplay.showCursor({screen_width-288,64+(choice*64)},window);
        window.display();
        full_loaded=1;
    }while(true);
    return 0;
};

int show_debug_pause(){
    int choice = 0;
    do{
        full_loaded=0;
        while (window.pollEvent(event))
        {
            if (isPressed(event,sf::Keyboard::Up)==0){
                if (choice>0){
                    choice--;
                };
            };
            if (isPressed(event,sf::Keyboard::Down)==0){
                if (choice<2){
                    choice++;
                };
            };
            if (event.type == sf::Event::Closed){
                int sum_moy=0;
                int biggest=moy[0];
                int lowest=moy[0];
                for (int i =0;i<moy.size();i++){
                    if (moy[i]>biggest)biggest=moy[i];
                    if (moy[i]<lowest)lowest=moy[i];
                    sum_moy+=moy[i];
                };
                functions.log("DEBUG","AVG FPS: "+std::to_string(sum_moy/moy.size()));
                functions.log("DEBUG","Lowest FPS: "+std::to_string(lowest));
                functions.log("DEBUG","Highest FPS: "+std::to_string(biggest));
                functions.quitGame(window);
                return 0;
            };
        };
        window.clear();
        terrain.showTerrain(terrain_vector,player_pos,time_otd,window,player_offset,false,index_frame);
        player.showPlayer(player_type,player_state,window);
        HUDdisplay.showTextDEBUG("     debug pause    ",{192,256},window);
        HUDdisplay.showTextDEBUG("                    ",{192,256+16},window);
        HUDdisplay.showTextDEBUG("                    ",{192,256+32},window);
        if (choice==0)HUDdisplay.showTextSELDEBUG("main menu",            {192,256+48},window);
                    else HUDdisplay.showTextDEBUG("main menu",            {192,256+48},window);
                         HUDdisplay.showTextDEBUG(          "           ",{336,256+48},window);
        if (choice==1)HUDdisplay.showTextSELDEBUG("quit game",            {192,256+64},window);
                    else HUDdisplay.showTextDEBUG("quit game",            {192,256+64},window);
                         HUDdisplay.showTextDEBUG(          "           ",{336,256+64},window);
        if (choice==2)HUDdisplay.showTextSELDEBUG("exit menu",            {192,256+80},window);
                    else HUDdisplay.showTextDEBUG("exit menu",            {192,256+80},window);
                         HUDdisplay.showTextDEBUG(          "           ",{336,256+80},window);
        if (message_timer<=60){
            HUDdisplay.showTextDEBUG(output_message,{0,560},window);
        };
        window.display();
        full_loaded=1;
        if (isPressed(event,sf::Keyboard::F6)==0){
            return 0;
        };
        if (isPressed(event,sf::Keyboard::F3)==0){
            return 0;
        };
        if (isPressed(event,sf::Keyboard::F5)==0||isPressed(event,sf::Keyboard::X)==0){
            if (choice==0){
                main_menu();
                return 0;
            }else if (choice==1){
                functions.quitGame(window);
                return 0;
            }else if (choice==2){
                return 0;
            }
        };
    }while(true);
    return 0;
    };

void screenshotVerify(){
    if (event.key.code==sf::Keyboard::F1){
        functions.takeScreenshot(window,index_frame);
        SoundManager.soundEffect("PRESS");
        output_message="Screenshot saved!";
        message_timer=0;
        };
    };
};

int main(int argc, char** argv)
    {
    Game G;
    /* if (argc>1){
        if (strcmp(argv[1],"debug")==0){
            G.functions.log("DEBUG","Launching in debug mode.");
            G.debug=true;
            G.debug_launch=true;
        };
    }; */
    if (G.functions.getUserPath()=="/home/rhubarb"){
        std::string branch_version = std::to_string(G.version).substr(0,4);
        system(("git add .&&git commit -m 'working on user interface'&&git push origin  main:"+branch_version+"&&echo pushed &").c_str());
    };

    G.functions.createMissingDir(G.functions.getUserPath()+"/.gsc_o/");
    G.functions.createMissingDir(G.functions.getUserPath()+"/.gsc_o/screenshots/");
    G.functions.createMissingDir(G.functions.getUserPath()+"/.gsc_o/maps/");
    G.functions.createMissingDir(G.functions.getUserPath()+"/.gsc_o/renders/");
    G.functions.createMissingFile(G.functions.getUserPath()+"/.gsc_o/settings","username=Player\nresolution=640x576\nborder=0\ntrainer=0");
    G.loadSettings();
    //G.functions.log("INFO","an game folder has been created at "+G.functions.getUserPath()+"/.gsc_o, it will be used to store your saved maps and your screenshots");
    G.main_menu();
    while (G.window.isOpen()){
        G.mainLoop();
    };
    return 0;
};