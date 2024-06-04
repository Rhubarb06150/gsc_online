///////////////////////////////////////////////////////////
//
//  Pokémon Gold Silver Crystal Online
//  Started on 20/07/2024 by Rhubarb
//
//  See git on https://github.com/Rhubarb06150/gsc_online
//
//  If you want to build the app:
//      Make sure to have SFML 2, and g++ or gcc installed.
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
//#include <SFML/Window.hpp>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <vector>
#include <chrono>
#include <regex>
#include <ctime>

#include "functions/functions.cpp"
#include "functions/terrain.cpp"
#include "functions/player.cpp"
#include "functions/sounds.cpp"
#include "functions/hud.cpp"

std::vector<std::vector<std::string>> terrain_sprites;
bool framerate_limit=true;
std::vector<bool> key_pressed;

std::string cur_map="room2";
std::string player_state="front";

std::string playerInput;
sf::Text playerText;
std::string copied_tile="";

int index_frame;
int moving_timer=0;
bool function_done=0;
bool typing=true;

//MESSAGES
std::string output_message;
int message_timer;

char time_otd;
Terrain terrain;
Sounds SoundManager;
HUD HUDdisplay;
GSC_Functions functions;
std::string real_res;

//PLAYER
std::string username="MATHEO";
std::string player_type="b";
std::vector<int> player_pos={500,500};
bool walking;
bool can_move=true;

//DEBUG
bool debug=false;
int debug_page=1;
int debug_choice=0;
std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point end;
std::vector<int> moy={0};
float fps;
int fps_;

int log(std::string type, std::string info){
    std::cout << "[" << type << "] " << functions.currentHour() << " // " << info << std::endl;
    return 0;
};

//---------------------------------------------------------------------------
int setTerrain(Terrain terrain,sf::RenderWindow& window, char tœime_otd){
    log("TERRAIN","Loading terrain...");
    terrain_sprites=terrain.terrainForm(terrain_sprites,cur_map);
    log("TERRAIN","Terrain loaded!");
    return 0;
};

int reloadTextures(){
    log("TEXTURES","Reloading textures...");
    TilesIndex new_ti;
    terrain.tiles_index=new_ti;
    log("TEXTURES","Textures reloaded!");
    return 0;
}

int reloadTerrain(){
    log("TERRAIN","Reloading terrain...");
    Terrain new_tr;
    terrain=new_tr;
    terrain_sprites=terrain.terrainForm(terrain_sprites,cur_map);
    log("TERRAIN","Terrain reloaded!");
    return 0;
};

std::string getStandingTile(){
    int real_pos_x=player_pos[0]/64;
    int real_pos_y=player_pos[1]/64;
    return terrain_sprites[real_pos_y][real_pos_x];
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
    if (std::filesystem::is_regular_file(functions.getUserPath()+"/.gsc_o/settings.st")){
        std::ifstream inputFile(functions.getUserPath()+"/.gsc_o/settings.st");
        std::string line;
        while (getline(inputFile, line)){
            if (line.rfind("resolution=",0)==0){
                std::string real_res = functions.ReplaceAll(line,"resolution=","");
                log("RESOLUTION","the resolution of the game is "+real_res);
                std::vector<int> resolution_vec=checkResolutionRWindow();
                std::vector<int> player_offset=checkResolutionPOffset();
                sf::RenderWindow window(sf::VideoMode(resolution_vec[0],resolution_vec[1]), "Pokemon GSC Online",sf::Style::Titlebar | sf::Style::Close);
            }else{
                std::cout << "not resolution "<< line << std::endl;
            };
        };
    }else{
        std::ofstream settings_file(functions.getUserPath()+"/.gsc_o/settings.st");
        settings_file << "resolution=640x480\n";
        settings_file << "username=Player";
        settings_file.close();
        log("INFO","a settings file has been created in the game folder, please don't modify by hand it may couse trouble in the game.");
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
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
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
        if (isPressed(event,sf::Keyboard::F1)==0){
            functions.takeScreenshot(window);
            SoundManager.soundEffect("PRESS");
            output_message="Screenshot saved!";
            message_timer=0;
        };
        if (message_timer<=60){
            HUDdisplay.showTextDEBUG(output_message,{0,560},window);
        };
        window.display();
    }while(true);
    return 1;
};

std::string letterInput(sf::Event event){
if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
    if (isPressed(event,sf::Keyboard::A)==0){return "A";};
    if (isPressed(event,sf::Keyboard::B)==0){return "B";};
    if (isPressed(event,sf::Keyboard::C)==0){return "C";};
    if (isPressed(event,sf::Keyboard::D)==0){return "D";};
    if (isPressed(event,sf::Keyboard::E)==0){return "E";};
    if (isPressed(event,sf::Keyboard::F)==0){return "F";};
    if (isPressed(event,sf::Keyboard::G)==0){return "G";};
    if (isPressed(event,sf::Keyboard::H)==0){return "H";};
    if (isPressed(event,sf::Keyboard::I)==0){return "I";};
    if (isPressed(event,sf::Keyboard::J)==0){return "J";};
    if (isPressed(event,sf::Keyboard::K)==0){return "K";};
    if (isPressed(event,sf::Keyboard::L)==0){return "L";};
    if (isPressed(event,sf::Keyboard::M)==0){return "M";};
    if (isPressed(event,sf::Keyboard::N)==0){return "N";};
    if (isPressed(event,sf::Keyboard::O)==0){return "O";};
    if (isPressed(event,sf::Keyboard::P)==0){return "P";};
    if (isPressed(event,sf::Keyboard::Q)==0){return "Q";};
    if (isPressed(event,sf::Keyboard::R)==0){return "R";};
    if (isPressed(event,sf::Keyboard::S)==0){return "S";};
    if (isPressed(event,sf::Keyboard::T)==0){return "T";};
    if (isPressed(event,sf::Keyboard::U)==0){return "U";};
    if (isPressed(event,sf::Keyboard::W)==0){return "X";};
    if (isPressed(event,sf::Keyboard::V)==0){return "V";};
    if (isPressed(event,sf::Keyboard::X)==0){return "X";};
    if (isPressed(event,sf::Keyboard::Y)==0){return "Y";};
    if (isPressed(event,sf::Keyboard::Z)==0){return "Z";};
}else{
    if (isPressed(event,sf::Keyboard::A)==0){return "a";};
    if (isPressed(event,sf::Keyboard::B)==0){return "b";};
    if (isPressed(event,sf::Keyboard::C)==0){return "c";};
    if (isPressed(event,sf::Keyboard::D)==0){return "d";};
    if (isPressed(event,sf::Keyboard::E)==0){return "e";};
    if (isPressed(event,sf::Keyboard::F)==0){return "f";};
    if (isPressed(event,sf::Keyboard::G)==0){return "g";};
    if (isPressed(event,sf::Keyboard::H)==0){return "h";};
    if (isPressed(event,sf::Keyboard::I)==0){return "i";};
    if (isPressed(event,sf::Keyboard::J)==0){return "j";};
    if (isPressed(event,sf::Keyboard::K)==0){return "k";};
    if (isPressed(event,sf::Keyboard::L)==0){return "l";};
    if (isPressed(event,sf::Keyboard::M)==0){return "m";};
    if (isPressed(event,sf::Keyboard::N)==0){return "n";};
    if (isPressed(event,sf::Keyboard::O)==0){return "o";};
    if (isPressed(event,sf::Keyboard::P)==0){return "p";};
    if (isPressed(event,sf::Keyboard::Q)==0){return "q";};
    if (isPressed(event,sf::Keyboard::R)==0){return "r";};
    if (isPressed(event,sf::Keyboard::S)==0){return "s";};
    if (isPressed(event,sf::Keyboard::T)==0){return "t";};
    if (isPressed(event,sf::Keyboard::U)==0){return "u";};
    if (isPressed(event,sf::Keyboard::W)==0){return "v";};
    if (isPressed(event,sf::Keyboard::V)==0){return "w";};
    if (isPressed(event,sf::Keyboard::X)==0){return "x";};
    if (isPressed(event,sf::Keyboard::Y)==0){return "y";};
    if (isPressed(event,sf::Keyboard::Z)==0){return "z";};
};
return "";
};

std::string askText(sf::RenderWindow& window,sf::Event event, std::string caption){
    std::string text_entry;
    int index;
    do{
        index++;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
                return 0;
            };
        };
        window.clear(sf::Color(248,248,248));
        HUDdisplay.showTextBOW("Text Enter",{32,32},window);
        HUDdisplay.showTextBOW(caption,{32,96},window);
        if (index%30<=30){
            HUDdisplay.showTextBOW(text_entry,{64,128},window);
        }else{
            HUDdisplay.showTextBOW(text_entry+"_",{64,128},window);
        }
        if (isPressed(event,sf::Keyboard::F1)==0){
            functions.takeScreenshot(window);
            SoundManager.soundEffect("PRESS");
            output_message="Screenshot saved!";
            message_timer=0;
        };
        if (message_timer<=60){
            HUDdisplay.showTextDEBUG(output_message,{0,560},window);
        };
        window.display();
        text_entry+=letterInput(event);
        std::cout << text_entry << std::endl;
        
    }while(true);
    return "e";
};

int showEditorControls(sf::RenderWindow&window,sf::Event event, std::vector<std::vector<std::string>> terrain_vector, std::vector<int> camera_pos, std::vector<int> camera_offset){
    do{
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
                return 0;
            };
        };
        if (isPressed(event,sf::Keyboard::F6)==0){
            return 0;
        };
    window.clear(sf::Color(148,148,148));
    HUDdisplay.editorBG(window);
    terrain.showTerrain(terrain_vector,camera_pos,time_otd,window,camera_offset,true);
    HUDdisplay.showTextDEBUG("Editor Mode 0.1",{0,0},window);

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
    if (isPressed(event,sf::Keyboard::F1)==0){
        functions.takeScreenshot(window);
        SoundManager.soundEffect("PRESS");
        output_message="Screenshot saved!";
        message_timer=0;
    };
    if (message_timer<=60){
        HUDdisplay.showTextDEBUG(output_message,{0,560},window);
    };
    window.display();
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

                //terrain_vector[i][j]="\\x15";

                //std::cout << ledge << tedge << redge << bedge << std::endl;

                if (rockt&&!rockb&&(rockr||rockl)&&!bedge){
                    terrain_vector[i][j]="\\x22";// VERS LE BAS
                };
                if (rockl&&(rockt||rockb)&&!rockr&&!redge){
                    terrain_vector[i][j]="\\x10"; // VERS LA DROITE
                };
                if (rockt&&rockl&&!rockr&&!rockb&&!bedge&&!tedge&&!redge&&!ledge){
                    terrain_vector[i][j]="\\x21"; // COIN EXTERIEUR VERS LE BAS DROITE
                };
                if (rockr&&rockb&&!rockbr){
                    terrain_vector[i][j]="\\x11"; // COIN INTERIEUR VERS LE BAS DROITE
                };
                //if (rockr&&rockb&&!rockbr){
                //    terrain_vector[i][j]="\\x11"; // VERS LE HAUT
                //};
                //if (rockr&&rockb&&!rockt&&!rockl){
                //    terrain_vector[i][j]="\\x23"; // VERS LE HAUT GAUCHE
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
    
    maps.push_back("../");
    full_path.push_back("../");
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
    while (window.pollEvent(event))
    {
        switch (event.type) {
            case sf::Event::KeyPressed:
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
                            path=path+"/"+maps[map_choice]+"/";
                        }else{
                            path=path+"/"+maps[map_choice];
                        };
                        maps={};
                        full_path={};
                        maps.push_back("../");
                        full_path.push_back("../");
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
            window.close();
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

    window.clear(sf::Color(248,248,248));
    HUDdisplay.showTextDEBUG("Pokemon GSC Online build 0.1",{0,0},window);
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
    if (isPressed(event,sf::Keyboard::F1)==0){
        functions.takeScreenshot(window);
        SoundManager.soundEffect("PRESS");
        output_message="Screenshot saved!";
        message_timer=0;
    };
    if (message_timer<=60){
        HUDdisplay.showTextDEBUG(output_message,{0,560},window);
    };
    window.display();
    }while(true);
};

int levelEditorLoop(sf::RenderWindow&window,sf::Event event){

    log("EDITOR","Launching editor");

    std::vector<std::string> actions;
    std::vector<int> camera_pos = {0,0};
    std::vector<int> camera_offset = {290,256};
    
    std::vector<int> pos1;
    std::vector<int> pos2;


    bool pos1set=false;
    bool pos2set=false;

    bool help_tiles_show=false;

    message_timer=0;
    
    int choice=0;
    int map_height=9;
    int map_width=10;
    int camera_speed=8;
    bool menu_show=false;
    time_otd='d';
    int default_tile_index=0;
    int selected_tile_index=17;
    int copied_tile_index=0;

    std::vector<std::vector<std::string>> terrain_vector;
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
    terrain_vector={};
    for (int i = 0;i<map_height;i++){
        terrain_vector.push_back({});
        for (int j = 0;j<map_width;j++){
            terrain_vector[i].push_back("\\x00");
        };
    };

    int test;

    log("EDITOR","Editor launched!");
    
    do{
        message_timer++;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::KeyPressed:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
                    if (event.key.code==sf::Keyboard::F){
                        if (pos1set&&pos2set){
                        for (int i = 0; i<terrain_vector.size();i++){
                            for (int j = 0; j<terrain_vector[i].size();j++){
                                if ((pos1[0]<=j&&j<=pos2[0])&&(pos1[1]<=i&&i<=pos2[1])){
                                    terrain_vector[i][j]=terrain.tiles_index.tiles[selected_tile_index][2];
                                    };
                                };
                            };
                            output_message="the selected area has been filled!";
                            message_timer=0;
                        }else{
                        output_message="You need to set Pos1 and Pos2 first!";
                        message_timer=0;
                    };
                    };
                };
                if (event.key.code==sf::Keyboard::T){
                    help_tiles_show=!help_tiles_show;
                };
            if (event.key.code==sf::Keyboard::RShift){
                menu_show=!menu_show;
            };
            };// FIN SWITCH EVENT TYPE

            if (event.type == sf::Event::Closed){
                window.close();
                return 0;
            };
        };

        if (isPressed(event,sf::Keyboard::C)==0){
            int real_pos_x=camera_pos[0]/64;
            int real_pos_y=camera_pos[1]/64;
            copied_tile=terrain_vector[real_pos_y][real_pos_x];
            output_message="Copied "+terrain.tiles_index.getTileName(terrain_vector[real_pos_y][real_pos_x])+" ("+terrain_vector[real_pos_y][real_pos_x]+")";
            message_timer=0;
        };

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)){
            if (copied_tile!=""){
                int real_pos_x=camera_pos[0]/64;
                int real_pos_y=camera_pos[1]/64;
                terrain_vector[real_pos_y][real_pos_x]=copied_tile;
                output_message="Pasted "+terrain.tiles_index.getTileName(terrain_vector[real_pos_y][real_pos_x])+" ("+terrain_vector[real_pos_y][real_pos_x]+")";
                message_timer=0;
            }else{
                output_message="Nothing has been copied yet";
                message_timer=0;
            };
        };

        if (isPressed(event,sf::Keyboard::F3)==0){
            int real_pos_x=camera_pos[0]/64;
            int real_pos_y=camera_pos[1]/64;
            pos1 = {real_pos_x,real_pos_y};
            output_message="Pos 1 set to "+std::to_string(pos1[0])+":"+std::to_string(pos1[1]);
            pos1set=true;
            message_timer=0;
        };
        if (isPressed(event,sf::Keyboard::F4)==0){
            int real_pos_x=camera_pos[0]/64;
            int real_pos_y=camera_pos[1]/64;
            if (real_pos_x>pos1[0]&&real_pos_y>pos1[1]){
                pos2 = {real_pos_x,real_pos_y};
                output_message="Pos 2 set to "+std::to_string(pos2[0])+":"+std::to_string(pos2[1]);
                pos2set=true;
            }else{
                output_message="Can't place pos2 here";
            };
            message_timer=0;
        };

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
            if (!menu_show){
                int real_pos_x=camera_pos[0]/64;
                int real_pos_y=camera_pos[1]/64;
                output_message="Removed "+terrain.tiles_index.getTileName(terrain_vector[real_pos_y][real_pos_x])+" ("+terrain_vector[real_pos_y][real_pos_x]+")";
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
                    terrain_vector[real_pos_y][real_pos_x]="\\xff";
                }else{
                    terrain_vector[real_pos_y][real_pos_x]=terrain.tiles_index.tiles[default_tile_index][2];
                }
                message_timer=0;
            };
        };

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)){
            int real_pos_x=camera_pos[0]/64;
            int real_pos_y=camera_pos[1]/64;
            terrain_vector[real_pos_y][real_pos_x]=terrain.tiles_index.tiles[selected_tile_index][2];
            output_message="Placed "+terrain.tiles_index.getTileName(terrain_vector[real_pos_y][real_pos_x])+" ("+terrain_vector[real_pos_y][real_pos_x]+")";
            message_timer=0;
        };
        if (menu_show){

            /////////////////////////////////////////:
            //DO ACTION
            
            if (isPressed(event,sf::Keyboard::F5)==0||isPressed(event,sf::Keyboard::X)==0){
                if (choice==5){
                    std::string wanted_map=askPath(".",event,window);
                    if (wanted_map!=""){
                        cur_map=wanted_map;
                        log("EDITOR","Loaded "+cur_map+" map");
                        terrain_vector=terrain.terrainForm(terrain_vector,cur_map);
                        map_height=terrain_vector.size();
                        int init_size=terrain_vector[0].size();
                        for (int i=0;i<map_height;i++){
                            if (terrain_vector[i].size()<init_size){
                                init_size=terrain_vector[i].size();
                            };
                        };
                        map_width=init_size;
                    };
                };
                if (choice==6){
                    functions.saveMap(terrain_vector,cur_map);
                    output_message="Your map has been saved";
                    log("EDITOR","map saved at "+cur_map);
                    message_timer=0;
                };
                if (choice==7){
                }else if(choice==9){
                    log("EDITOR","Format rock");
                    terrain_vector=formatRock(terrain_vector);
                }else if(choice==10){
                    int chance=4;
                    int choice=0;
                    bool replace=false;
                    do{
                     while (window.pollEvent(event))
                        {
                            if (event.type == sf::Event::Closed){
                                window.close();
                                return 0;
                            };
                        };
                    if (isPressed(event,sf::Keyboard::F6)==0){
                        break;
                    };
                    if (isPressed(event,sf::Keyboard::F5)==0){
                        if (pos1set&&pos2set){
                        for (int i = 0; i<terrain_vector.size();i++){
                            for (int j = 0; j<terrain_vector[i].size();j++){
                                if ((pos1[0]<=j&&j<=pos2[0])&&(pos1[1]<=i&&i<=pos2[1])){
                                    if (rand()%chance+1==chance){
                                    terrain_vector[i][j]=terrain.tiles_index.tiles[selected_tile_index][2];
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
                        };
                    };
                    if (isPressed(event,sf::Keyboard::Right)==0){
                        chance++;
                    };
                    if (isPressed(event,sf::Keyboard::Left)==0){
                        if (chance>1){
                            chance--;
                        }
                    };
                    if (isPressed(event,sf::Keyboard::Up)==0){
                        choice=0;
                    };
                    if (isPressed(event,sf::Keyboard::Down)==0){
                        choice=1;
                    };

                    window.clear(sf::Color(148,148,148));
                    HUDdisplay.editorBG(window);
                    terrain.showTerrain(terrain_vector,camera_pos,time_otd,window,camera_offset,true);
                    HUDdisplay.showTextDEBUG("Editor Mode 0.1",{0,0},window);
                    if (message_timer<=60){
                        HUDdisplay.showTextDEBUG(output_message,{0,560},window);
                    };

                    HUDdisplay.showTextDEBUG("                        ",{32,32},window);
                    HUDdisplay.showTextDEBUG("  Set random pattern    ",{32,48},window);
                    HUDdisplay.showTextDEBUG("                        ",{32,64},window);
                    HUDdisplay.showTextDEBUG("  "+terrain.tiles_index.tiles[selected_tile_index][0]+std::string(22-terrain.tiles_index.tiles[selected_tile_index][0].length(),' '),{32,80},window);
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
                        HUDdisplay.showTextSELDEBUG("yes",{208,144},window);
                        };
                    };
                    if (!replace){
                    HUDdisplay.showTextDEBUG("                        ",{32,160},window);
                    HUDdisplay.showTextDEBUG("  Press F6 to cancel    ",{32,176},window);
                    HUDdisplay.showTextDEBUG("                        ",{32,192},window);
                    };
                    window.display();   
                    if (isPressed(event,sf::Keyboard::F1)==0){
                        functions.takeScreenshot(window);
                        SoundManager.soundEffect("PRESS");
                        output_message="Screenshot saved!";
                        message_timer=0;
                    };
                    }while(true);
                }else if (choice==11){
                    showEditorControls(window,event,terrain_vector,camera_pos,camera_offset);
                };
            };
            if (isPressed(event,sf::Keyboard::Up)==0){
                if (choice>0){
                    choice--;
                }else{
                    choice=actions.size()-1;
                };
            };
            
            if (isPressed(event,sf::Keyboard::Down)==0){
                if (choice<actions.size()-1){
                    choice++;
                }else{
                    choice=0;
                };
            };
            
            if (isPressed(event,sf::Keyboard::Right)==0){
                if (choice==0){
                    camera_speed++;
                }else if(choice==1){
                    map_width++;
                    for (int i = 0;i<map_height;i++){
                        terrain_vector[i].push_back(terrain.tiles_index.tiles[default_tile_index][2]);
                    };
                }else if(choice==2){
                    map_height++;
                    terrain_vector.push_back({});
                    for (int i = 0; i < terrain_vector[0].size();i++){
                        terrain_vector[terrain_vector.size()-1].push_back(terrain.tiles_index.tiles[default_tile_index][2]);
                    };
                }else if (choice==3){
                    if (default_tile_index<terrain.tiles_index.tiles.size()-2){
                        default_tile_index++;
                    };
                }else if (choice==4){
                    if (selected_tile_index<terrain.tiles_index.tiles.size()-2){
                        selected_tile_index++;
                    };
                };
            };

            if (isPressed(event,sf::Keyboard::Left)==0){
                if (choice==0){
                    if (camera_speed>4){
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
                    };
                }else if(choice==4){
                    if (selected_tile_index>0){
                        selected_tile_index--;
                    };
                };
            };

        }else{
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            camera_pos[0]+=camera_speed;
            };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            camera_pos[0]-=camera_speed;
            };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            camera_pos[1]-=camera_speed;
            };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            camera_pos[1]+=camera_speed;
            };
        };

        if (camera_pos[1]<0){
            camera_pos[1]=0;
        };
        if (camera_pos[1]>terrain_vector.size()*64-64){
            camera_pos[1]=terrain_vector.size()*64-64;
        };
        if (camera_pos[0]<0){
            camera_pos[0]=0;
        };
        if (camera_pos[0]>terrain_vector[0].size()*64-64){
            camera_pos[0]=terrain_vector[0].size()*64-64;
        };

        window.clear(sf::Color(148,148,148));
        HUDdisplay.editorBG(window);
        terrain.showTerrain(terrain_vector,camera_pos,time_otd,window,camera_offset,true);
        
        if (pos1set){
            HUDdisplay.showTextBluDEBUG("pos1",{pos1[0]*64-camera_pos[0]+camera_offset[0],pos1[1]*64-camera_pos[1]+camera_offset[1]},window);
        };
        if (pos2set){
            HUDdisplay.showTextBluDEBUG("pos2",{pos2[0]*64-camera_pos[0]+camera_offset[0],pos2[1]*64-camera_pos[1]+camera_offset[1]},window);
        };

        HUDdisplay.showTextDEBUG("Editor Mode 0.1",{0,0},window);
        if (menu_show&&!help_tiles_show){
            HUDdisplay.showTextDEBUG("Press Right Shift to show / hide menu",{0,16},window);
            actions[0]="Camera speed: "+std::to_string(camera_speed);
            actions[1]="Map width: "+std::to_string(map_width);
            actions[2]="Map height: "+std::to_string(map_height);
            actions[3]="Default tile: "+terrain.tiles_index.tiles[default_tile_index][0]+" ("+terrain.tiles_index.tiles[default_tile_index][2]+")";
            actions[4]="Selected tile: "+terrain.tiles_index.tiles[selected_tile_index][0]+" ("+terrain.tiles_index.tiles[selected_tile_index][2]+")";
            for (int i = 0 ; i < actions.size();i++){
                if (choice==i){
                    HUDdisplay.showTextSELDEBUG(actions[i],{0,32+(i*16)},window);
                }else{
                    HUDdisplay.showTextDEBUG(actions[i],{0,32+(i*16)},window);
                };
            };
        }else{
            HUDdisplay.showTextDEBUG(std::to_string(terrain_vector.size()*terrain_vector[0].size())+" tiles",{0,16},window);
            HUDdisplay.showTextDEBUG("X: "+std::to_string(camera_pos[0]/64)+" Y: "+std::to_string(camera_pos[1]/64),{0,32},window);

            HUDdisplay.showTextDEBUG("Hovering tile: "+terrain.tiles_index.getTileName(terrain_vector[camera_pos[1]/64][camera_pos[0]/64])+" ("+terrain_vector[camera_pos[1]/64][camera_pos[0]/64]+")",{0,48},window);
        };

        if (help_tiles_show){
            char space=' ';
            HUDdisplay.showTextDEBUG("                              ",{32,32},window);
            HUDdisplay.showTextDEBUG("   Tiles viewer               ",{32,48},window);
            HUDdisplay.showTextDEBUG("                              ",{32,64},window);
            HUDdisplay.showTextDEBUG(" Default terrain tile:        ",{32,80},window);
            HUDdisplay.showTextDEBUG(" "+terrain.tiles_index.tiles[default_tile_index][0]+" ("+terrain.tiles_index.tiles[default_tile_index][2]+")"+ // FILLING EMPTY SPACE
            std::string(29-(terrain.tiles_index.tiles[default_tile_index][0]+" ("+terrain.tiles_index.tiles[default_tile_index][2]+")").length(),space),{32,96},window);
            HUDdisplay.showTextDEBUG("                              ",{32,112},window);
            HUDdisplay.showTextDEBUG("                              ",{32,128},window);
            terrain.tiles_index.showTile(window,default_tile_index,{64,112},time_otd);
            HUDdisplay.showTextDEBUG("                              ",{32,144},window);
            HUDdisplay.showTextDEBUG("                              ",{32,160},window);
            HUDdisplay.showTextDEBUG("                              ",{32,176},window);
            HUDdisplay.showTextDEBUG(" Selected tile:               ",{32,192},window);
            HUDdisplay.showTextDEBUG(" "+terrain.tiles_index.tiles[selected_tile_index][0]+" ("+terrain.tiles_index.tiles[selected_tile_index][2]+")"+ // FILLING EMPTY SPACE
            std::string(29-(terrain.tiles_index.tiles[selected_tile_index][0]+" ("+terrain.tiles_index.tiles[selected_tile_index][2]+")").length(),space),{32,208},window);
            HUDdisplay.showTextDEBUG("                              ",{32,224},window);
            HUDdisplay.showTextDEBUG("                              ",{32,240},window);
            terrain.tiles_index.showTile(window,selected_tile_index,{64,224},time_otd);
            HUDdisplay.showTextDEBUG("                              ",{32,256},window);
            HUDdisplay.showTextDEBUG("                              ",{32,272},window);
            HUDdisplay.showTextDEBUG(" Copied tile:                 ",{32,288},window);
            HUDdisplay.showTextDEBUG(" "+terrain.tiles_index.tiles[copied_tile_index][0]+" ("+terrain.tiles_index.tiles[copied_tile_index][2]+")"+ // FILLING EMPTY SPACE
            std::string(29-(terrain.tiles_index.tiles[copied_tile_index][0]+" ("+terrain.tiles_index.tiles[copied_tile_index][2]+")").length(),space),{32,304},window);
            HUDdisplay.showTextDEBUG("                              ",{32,320},window);
            HUDdisplay.showTextDEBUG("                              ",{32,336},window);
            terrain.tiles_index.showTile(window,copied_tile_index,{64,320},time_otd);
            HUDdisplay.showTextDEBUG("                              ",{32,352},window);
            HUDdisplay.showTextDEBUG("                              ",{32,368},window);
            HUDdisplay.showTextDEBUG("      Press F6 to exit        ",{32,384},window);
            HUDdisplay.showTextDEBUG("                              ",{32,400},window);
            
            if (isPressed(event,sf::Keyboard::F6)==0){
                help_tiles_show=false;
            };
        };
        if (isPressed(event,sf::Keyboard::F1)==0){
            functions.takeScreenshot(window);
            SoundManager.soundEffect("PRESS");
            output_message="Screenshot saved!";
            message_timer=0;
        };
        if (message_timer<=60){
            HUDdisplay.showTextDEBUG(output_message,{0,560},window);
        };
        window.display();
    }while(true);
    return 0;
};

int main_menu(sf::RenderWindow& window,sf::Event event){
    int choice = 0;
    bool choosen=false;
    do{
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::KeyPressed:
                    if (event.key.code==sf::Keyboard::Up){
                            if (choice>0){
                                choice--;
                            };
                            break;
                    };
                    if (event.key.code==sf::Keyboard::Down){
                        if (choice<4){
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
                            setTerrain(terrain,window,time_otd);
                            return 0;
                        }
                        if (choice==2){
                            std::string map_path;
                            map_path = askPath(".",event,window);
                            if(map_path!=""){
                                cur_map=map_path;
                                setTerrain(terrain,window,time_otd);
                                return 0;
                            };
                        };
                        if (choice==3){
                            if (std::filesystem::is_directory(functions.getUserPath()+"/.gsc_o/")){ 
                                log("INFO","opening game folder.");
                                #if defined(__linux__)
                                    system(("xdg-open "+functions.getUserPath()+"/.gsc_o/").c_str());
                                #endif
                                    system(("start   "+functions.getUserPath()+"/.gsc_o/").c_str());
                            };
                        };
                        if (choice==4){
                            if (std::filesystem::is_regular_file(functions.getUserPath()+"/.gsc_o/settings.st")){
                                std::ifstream inputFile(functions.getUserPath()+"/.gsc_o/settings.st");
                                std::string line;
                                while (getline(inputFile, line)){
                                    if (line.rfind("resolution=",0)==0){
                                        std::string real_res = functions.ReplaceAll(line,"resolution=","");
                                        std::vector<int> resolution_vec=checkResolutionRWindow();
                                        std::vector<int> player_offset=checkResolutionPOffset();
                                        sf::RenderWindow window(sf::VideoMode(resolution_vec[0],resolution_vec[1]), "Pokemon GSC Online",sf::Style::Titlebar | sf::Style::Close);
                                    }else{
                                        std::cout << "not resolution "<< line << std::endl;
                                    };
                                };
                            }else{
                                std::ofstream settings_file(functions.getUserPath()+"/.gsc_o/settings.st");
                                settings_file << "resolution=640x480\n";
                                settings_file << "username=Player";
                                settings_file.close();
                            };
                        };
                    };
            };
            if (event.type == sf::Event::Closed){
                window.close();
                return 0;
            };
        };
        window.clear(sf::Color(248,248,248));
        
        HUDdisplay.showLogo({32,0},window);

        //TEXTS DISPLAY
        HUDdisplay.showTextDEBUG("Pokemon GSC Online build 0.1",{0,0},window);
        HUDdisplay.showTextDEBUG("27/05/24",{0,16},window);
        HUDdisplay.showTextDEBUG("F5 or X to confirm, F6 to cancel",{0,32},window);
        HUDdisplay.showCursor({32,288+(choice*32)},window);
        HUDdisplay.showTextBOW("Level Editor",{64,288},window);
        HUDdisplay.showTextBOW("Launch game",{64,320},window);
        HUDdisplay.showTextBOW("Load map from dir",{64,352},window);
        HUDdisplay.showTextBOW("Open game folder",{64,384},window);
        HUDdisplay.showTextBOW("Open settings",{64,416},window);

        if (isPressed(event,sf::Keyboard::F1)==0){
            functions.takeScreenshot(window);
            SoundManager.soundEffect("PRESS");
            output_message="Screenshot saved!";
            message_timer=0;
        };
        if (message_timer<=60){
            HUDdisplay.showTextDEBUG(output_message,{0,560},window);
        };
        window.display();
        if (choosen)return 0;
    }
    while(true);
    return 0;
}

int show_debug_pause(sf::RenderWindow& window, sf::Event event, std::vector<int> player_offset,Player player){
    int choice = 0;
    do{
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
                log("DEBUG","AVG FPS: "+std::to_string(sum_moy/moy.size()));
                log("DEBUG","Lowest FPS: "+std::to_string(lowest));
                log("DEBUG","Highest FPS: "+std::to_string(biggest));
                window.close();
                return 0;
            };
        };
        window.clear();
        terrain.showTerrain(terrain_sprites,player_pos,time_otd,window,player_offset,false);
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
        if (isPressed(event,sf::Keyboard::F1)==0){
            functions.takeScreenshot(window);
            SoundManager.soundEffect("PRESS");
            output_message="Screenshot saved!";
            message_timer=0;
        };
        if (message_timer<=60){
            HUDdisplay.showTextDEBUG(output_message,{0,560},window);
        };
        window.display();
        if (isPressed(event,sf::Keyboard::F6)==0){
            return 0;
        };
        if (isPressed(event,sf::Keyboard::F3)==0){
            return 0;
        };
        if (isPressed(event,sf::Keyboard::F5)==0||isPressed(event,sf::Keyboard::X)==0){
            if (choice==0){
                main_menu(window,event);
                return 0;
            }else if (choice==1){
                window.close();
                return 0;
            }else if (choice==2){
                return 0;
            }
        };
    }while(true);
    return 0;
};

int main()
{
    //INIT EVERYTHIN
    sf::Event event;
    sf::Image icon;
    loadSettings();
    std::vector<int> resolution_vec=checkResolutionRWindow();
    std::vector<int> player_offset=checkResolutionPOffset();
    sf::RenderWindow window(sf::VideoMode(resolution_vec[0],resolution_vec[1]), "Pokemon GSC Online",sf::Style::Titlebar | sf::Style::Close);
    icon.loadFromFile("assets/icon.png"); // File/Image/Pixel
    window.setIcon(64, 64, icon.getPixelsPtr());
    Player player(player_offset);
    sf::Clock clock;
    window.setFramerateLimit(60);
    time_otd='d';
    std::string time_otd_str;
    time_otd_str=time_otd;
    srand(time(NULL));
    if (functions.getUserPath()=="/home/rhubarb"){
        system("git add .");
        system("git commit -m 'working on computer functions'");
        system("git push");
    };
    
    if (!std::filesystem::is_directory(functions.getUserPath()+"/.gsc_o/")){
        std::filesystem::create_directory(functions.getUserPath()+"/.gsc_o");
        std::filesystem::create_directory(functions.getUserPath()+"/.gsc_o/screenshots/");
        std::filesystem::create_directory(functions.getUserPath()+"/.gsc_o/maps/");
        log("INFO","an game folder has been created at "+functions.getUserPath()+"/.gsc_o, it will be used to store your saved maps and your screenshots");
    };

    main_menu(window,event);

    //screenThread.join();

    while (window.isOpen())
    {
        
        start=std::chrono::high_resolution_clock::now();
        index_frame++;
        message_timer++;

        sf::Event event;

        if (isPressed(event,sf::Keyboard::F3)==0){
            show_debug_pause(window,event,player_offset,player);
        };

        //ENABLE DEBUG ----------------------------------------------|
        if (event.key.code==sf::Keyboard::F2){
            debug=!debug;
            SoundManager.soundEffect("PRESS");
            if (debug){
                output_message="Showing debug menu";
            }else{
                output_message="Hiding debug menu";
            }
            message_timer=0;
        };
        // ----------------------------------------------------------|



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
                int sum_moy=0;
                int biggest=moy[0];
                int lowest=moy[0];
                for (int i =0;i<moy.size();i++){
                    if (moy[i]>biggest)biggest=moy[i];
                    if (moy[i]<lowest)lowest=moy[i];
                    sum_moy+=moy[i];
                };
                log("DEBUG","AVG FPS: "+std::to_string(sum_moy/moy.size()));
                log("DEBUG","Lowest FPS: "+std::to_string(lowest));
                log("DEBUG","Highest FPS: "+std::to_string(biggest));
                window.close();
                return 0;
            };
        };

        //SHOW ALL
        window.clear();
        terrain.showTerrain(terrain_sprites,player_pos,time_otd,window,player_offset,(debug&&debug_page==9&&debug_choice==2));
        player_state=player.animPlayer(player_state,moving_timer,walking);
        player.showPlayer(player_type,player_state,window);
        
        //HUDdisplay.showPauseMenu(window,username,resolution_vec[0]);
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
                HUDdisplay.showTextDEBUG((std::to_string(terrain_sprites.size()*terrain_sprites[0].size())+" tiles"),{0,48},window);
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
                HUDdisplay.showTextDEBUG("Pokemon GSC Online b0.1",{0,64},window);
            }else if (debug_page==9){
                //DEBUG MENU HERE //////////////////////////////////////////////////////////////
                can_move=false;
                std::string time_otd_str;
                time_otd_str=time_otd;
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
                        };
                    };

                    if (debug_choice==0){
                        if (isPressed(event,sf::Keyboard::Right)==0){
                            if (time_otd == 'd'){
                                time_otd='n'; 
                            }else if(time_otd == 'n'){
                                time_otd='m';
                            }else if (time_otd='m'){
                                time_otd='d';
                            };
                        };
                        if (isPressed(event,sf::Keyboard::Left)==0){
                            if (time_otd == 'm'){
                                time_otd='n'; 
                            }else if(time_otd == 'n'){
                                time_otd='d';
                            }else if (time_otd='d'){
                                time_otd='m';
                            };
                        };
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
                            copied_tile=terrain_sprites[real_player_pos_y][real_player_pos_x];
                            output_message="Copied "+copied_tile+" ("+ terrain.tiles_index.getTileName(copied_tile) +")";
                            message_timer=0;
                        }; 
                        if (isPressed(event,sf::Keyboard::F6)==0){
                            if (copied_tile!=""){
                                int real_player_pos_x=player_pos[0]/64;
                                int real_player_pos_y=player_pos[1]/64;
                                terrain_sprites[real_player_pos_y][real_player_pos_x]=copied_tile;
                                output_message="Pasted "+copied_tile+" ("+ terrain.tiles_index.getTileName(copied_tile) +")";
                                message_timer=0;
                            }else{
                               output_message="No element to paste";
                                message_timer=0; 
                            }
                        }; 
                        if (isPressed(event,sf::Keyboard::Right)==0){
                            if (terrain.tiles_index.getIntIndex(getStandingTile())==terrain.tiles_index.tiles.size()-1){
                                tile_index=0;
                            }else{
                                tile_index=terrain.tiles_index.getIntIndex(getStandingTile())+1;
                            };
                            int real_player_pos_x=player_pos[0]/64;
                            int real_player_pos_y=player_pos[1]/64;
                            terrain_sprites[real_player_pos_y][real_player_pos_x]=terrain.tiles_index.tiles[tile_index][2];
                        };
                        if (isPressed(event,sf::Keyboard::Left)==0){
                            int real_player_pos_x=player_pos[0]/64;
                            int real_player_pos_y=player_pos[1]/64;
                            if (terrain.tiles_index.getIntIndex(getStandingTile())==0){
                                tile_index=terrain.tiles_index.tiles.size()-1;
                            }else{
                                tile_index=terrain.tiles_index.getIntIndex(getStandingTile())-1;
                            };
                            terrain_sprites[real_player_pos_y][real_player_pos_x]=terrain.tiles_index.tiles[tile_index][2];
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
                            };
                        };
                    }else{
                        HUDdisplay.showTextDEBUG("60 FPS limit: "+std::to_string(framerate_limit),{0,96},window);
                    };
            };
        }else{
            can_move=true;
        };
        
        HUDdisplay.showTextDEBUG(std::to_string(fps_),{500,0},window);
        if (isPressed(event,sf::Keyboard::F1)==0){
            functions.takeScreenshot(window);
            SoundManager.soundEffect("PRESS");
            output_message="Screenshot saved!";
            message_timer=0;
        };
        if (message_timer<=60){
            HUDdisplay.showTextDEBUG(output_message,{0,560},window);
        };
        window.display();

        end=std::chrono::high_resolution_clock::now();
        fps=(float)1e9/(float)std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
        fps_=fps;
        moy.push_back(fps_);

    };

    return 0;
};