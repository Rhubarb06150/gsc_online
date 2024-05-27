#include <SFML/Graphics.hpp>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <iostream>

#include "functions.cpp"

class TilesIndex{
    private:
    GSC_Functions funs;
    public:

    std::vector<std::vector<std::string>> tiles;
    std::vector<std::vector<sf::Texture>> textures;
    std::vector<std::vector<std::string>> solids;
    std::vector<sf::Texture> morning_textures;
    std::vector<sf::Texture> day_textures;
    std::vector<sf::Texture> night_textures;

    TilesIndex(){


    //ENVIRONNEMENT
        tiles.push_back({"grass","0011","\\x00"});
        tiles.push_back({"cleanGrass","3011","\\x01"});
        tiles.push_back({"cleanTallGrass","1011","\\x02"});
        tiles.push_back({"tallGrass","3211","\\x0a"});
        tiles.push_back({"flower","0111","\\x03"});
        tiles.push_back({"bush","2011","\\x04"});
        solids.push_back({"\\x04","0044"});
        tiles.push_back({"bigTree","1122","\\x05"});
        tiles.push_back({"smallTree","3111","\\x06"});
        tiles.push_back({"grandTree","3312","\\x16"});
        tiles.push_back({"grassBottom","2311","\\x0b"});

        tiles.push_back({"rockLeftGrass","0511","\\x20"});
        tiles.push_back({"rockLeft","0511","\\x10"});
        tiles.push_back({"rockTopLeft","0411","\\x11"});
        tiles.push_back({"rockTopGrass","1411","\\x22"});
        tiles.push_back({"rockTop","1411","\\x12"});
        tiles.push_back({"rockTopRight","2411","\\x13"});
        tiles.push_back({"rockRight","2511","\\x14"});
        tiles.push_back({"rock","1511","\\x15"});
    //PATHS
        tiles.push_back({"path","0211","\\x07"});
        tiles.push_back({"altPath","1311","\\x08"});
        tiles.push_back({"altPath2","0311","\\x09"});
    //ROOM
        tiles.push_back({"roomFloor","f011","\\x40"});
        tiles.push_back({"roomWall","e011","\\x41"});
        tiles.push_back({"roomCarpet","f211","\\x43"});
        tiles.push_back({"bookshelf","b112","\\x44"});
        tiles.push_back({"tableL","c211","\\x45"});
        tiles.push_back({"tableM","d211","\\x46"});
        tiles.push_back({"tableR","e211","\\x47"});
        tiles.push_back({"roomComputer","d012","\\x48"});
        tiles.push_back({"roomTV","b011","\\x49"});
        tiles.push_back({"roomTVonDesk","a012","\\x4a"});
        tiles.push_back({"n64OnFloor","c111","\\x4b"});
        tiles.push_back({"snesOnFloor","a211","\\x4c"});
        tiles.push_back({"snesOnDesk","9211","\\x4d"});
        tiles.push_back({"stool","c011","\\x4e"});
        tiles.push_back({"plant1","8012","\\x4f"});
        tiles.push_back({"plant2","9012","\\x50"});
        tiles.push_back({"plant3","8212","\\x51"});
        tiles.push_back({"couch","9312","\\x52"});
        tiles.push_back({"roomDoor","b311","\\x53"});
    //OTHER
        tiles.push_back({"none","fe11","\\xff"});

    int x;
    int y;
    int xsize;
    int ysize;
    sf::Sprite sprite;

    for (int i = 0;i<tiles.size();i++){

        std::string fish = tiles[i][1];

        x = funs.hexToInt(fish[0]);
        y = funs.hexToInt(fish[1]);
        xsize = funs.hexToInt(fish[2]);
        ysize = funs.hexToInt(fish[3]);

        //le xsize et le ysize foncitonnent correctement;

        sf::Texture texture_morning;
        sf::Texture texture_day;
        sf::Texture texture_night;

        texture_morning.loadFromFile("assets/tiles/morning.png",sf::IntRect(x*16,y*16,xsize*16,ysize*16));
        morning_textures.push_back(texture_morning);

        texture_day.loadFromFile("assets/tiles/day.png",        sf::IntRect(x*16,y*16,xsize*16,ysize*16));
        day_textures.push_back(texture_day);

        texture_night.loadFromFile("assets/tiles/night.png",    sf::IntRect(x*16,y*16,xsize*16,ysize*16));
        night_textures.push_back(texture_night);

        };

        textures.push_back(morning_textures);
        textures.push_back(day_textures);
        textures.push_back(night_textures);

        //les textures fonctionnent bien
    };    

    std::string getIndex(std::string index){
        std::string res="";
        std::string fish;
        fish=index;
        for (int i = 0;i<tiles.size();i++){
            if (tiles[i][2]==fish){
                res=tiles[i][0];
                break;
                };
        };
        return res;
    };

    int getIntIndex(std::string index){
        int res;
        std::string fish;
        for (int i = 0;i<tiles.size();i++){
            if (tiles[i][2]==index){
                return i;
            };
        };
        return tiles.size()-1;
    };

    sf::Sprite getTile(std::string index){

        sf::Sprite sprite;

        int x;
        int y;
        int xsize;
        int ysize;
            
        for (int i = 0;i<tiles.size();i++){
            if (tiles[i][0]==index){
                std::string fish = tiles[i][1];
                x = funs.hexToInt(fish[0]);
                y = funs.hexToInt(fish[1]);
                xsize = funs.hexToInt(fish[2])*16;
                ysize = funs.hexToInt(fish[3])*16;
                break;
            };
        };
        return sprite;
    };

    sf::Texture getTileTexture(std::string index, char time){

        // index -> index hexa \x00
        sf::Texture texture;

        for (int i = 0;i<tiles.size();i++){
            if (index==tiles[i][2]){
                if (time=='m'){
                    texture=morning_textures[i];
                    break;
                }else if (time=='d'){
                    texture=day_textures[i];
                    break;
                }else if (time=='n'){
                    texture=night_textures[i];
                    break;
                };   
            };
        };
        return texture;
    };

    int checkCollision(std::vector<int> player_pos, char direction, std::vector<std::vector<std::string>> terrain_sprites){
        int real_pos_x=player_pos[0]/64;
        int real_pos_y=player_pos[1]/64;
        return 0;
    };
};