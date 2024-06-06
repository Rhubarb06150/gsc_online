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
    std::vector<std::vector<std::string>> animated_tiles;
    std::vector<std::vector<sf::Texture>> textures;
    std::vector<std::vector<std::vector<sf::Texture>>> animated_textures;
    std::vector<std::vector<std::string>> solids;
    
    std::vector<sf::Texture> morning_textures;
    std::vector<sf::Texture> day_textures;
    std::vector<sf::Texture> night_textures;
    std::vector<std::vector<sf::Texture>> animated_morning_textures;
    std::vector<std::vector<sf::Texture>> animated_day_textures;
    std::vector<std::vector<sf::Texture>> animated_night_textures;

    TilesIndex(){

    //ANIMATED TILES
        animated_tiles.push_back({"water","7","0011"});

    //ENVIRONNEMENT
        tiles.push_back({"grass","0011","\\x00","0"});
        tiles.push_back({"cleanGrass","3011","\\x01","0"});
        tiles.push_back({"cleanTallGrass","1011","\\x02","0"});
        tiles.push_back({"tallGrass","3211","\\x0a","0"});
        tiles.push_back({"flower","0111","\\x03","0"});
        tiles.push_back({"bush","2011","\\x04","0"});
        tiles.push_back({"bigTree","1122","\\x05","0"});
        tiles.push_back({"smallTree","3111","\\x06","0"});
        tiles.push_back({"grandTree","3312","\\x16","0"});
        tiles.push_back({"grassBottom","2311","\\x0b","0"});
        tiles.push_back({"water","3511","\\xa0","1"});
    //ROCKS
        tiles.push_back({"rockLeftGrass","0511","\\x20","0"});
        tiles.push_back({"rockLeft","0511","\\x10","0"});
        tiles.push_back({"rockCornerTopLeft","0411","\\x11","0"});
        tiles.push_back({"rockCornerTopLeftGrass","1411","\\x22","0"});
        tiles.push_back({"rockTop","1611","\\x12","0"});
        tiles.push_back({"rockTopRight","2411","\\x13","0"});
        tiles.push_back({"rockRight","2511","\\x14","0"});
        tiles.push_back({"rock","1511","\\x15","0"});
        tiles.push_back({"rockCornerTopRight","2611","\\x17","0"});
        tiles.push_back({"rockCornerTopRightGrass","2711","\\x18","0"});
        tiles.push_back({"rockBottomRight","1711","\\x19","0"});
        tiles.push_back({"rockBottomRightGrass","0711","\\x21","0"});
        tiles.push_back({"rockTopLeftGrass","0811","\\x23","0"});
        tiles.push_back({"rockTopGrass","1811","\\x24","0"});
        tiles.push_back({"rockTopRightGrass","2811","\\x25","0"});
    //PATHS
        tiles.push_back({"path","0211","\\x07","0"});
        tiles.push_back({"altPath","1311","\\x08","0"});
        tiles.push_back({"altPath2","0311","\\x09","0"});
    //ROOM
        tiles.push_back({"roomFloor","f011","\\x40","0"});
        tiles.push_back({"roomWall","e011","\\x41","0"});
        tiles.push_back({"roomCarpet","f211","\\x43","0"});
        tiles.push_back({"bookshelf","b112","\\x44","0"});
        tiles.push_back({"tableL","c211","\\x45","0"});
        tiles.push_back({"tableM","d211","\\x46","0"});
        tiles.push_back({"tableR","e211","\\x47","0"});
        tiles.push_back({"roomComputer","d012","\\x48","0"});
        tiles.push_back({"roomTV","b011","\\x49","0"});
        tiles.push_back({"roomTVonDesk","a012","\\x4a","0"});
        tiles.push_back({"n64OnFloor","c111","\\x4b","0"});
        tiles.push_back({"snesOnFloor","a211","\\x4c","0"});
        tiles.push_back({"snesOnDesk","9211","\\x4d","0"});
        tiles.push_back({"stool","c011","\\x4e","0"});
        tiles.push_back({"plant1","8012","\\x4f","0"});
        tiles.push_back({"plant2","9012","\\x50","0"});
        tiles.push_back({"plant3","8212","\\x51","0"});
        tiles.push_back({"couch","9312","\\x52","0"});
        tiles.push_back({"roomDoor","b311","\\x53","0"});
        tiles.push_back({"roomDesk","d311","\\x54","0"});
        tiles.push_back({"roomDeskSheet","c311","\\x55","0"});
    //OTHER
        tiles.push_back({"none","fe11","\\xff","0"});
        tiles.push_back({"unknown","ff11","XXXX","0"});

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

    for (int i = 0;i<animated_tiles.size();i++){

        std::string fish = animated_tiles[i][2];

        x = funs.hexToInt(fish[0]);
        y = funs.hexToInt(fish[1]);
        xsize = funs.hexToInt(fish[2]);
        ysize = funs.hexToInt(fish[3]);

        //le xsize et le ysize foncitonnent correctement;

        sf::Texture texture_morning;
        sf::Texture texture_day;
        sf::Texture texture_night;

        animated_morning_textures.push_back({});
        animated_day_textures.push_back({});
        animated_night_textures.push_back({});

        for (int j=0;j<j;i++){
            texture_morning.loadFromFile("assets/tiles/a_morning.png",sf::IntRect(x*16,j+y*16,xsize*16,ysize*16));
            animated_morning_textures[i].push_back(texture_morning);
        };

        for (int j=0;j<j;i++){
            texture_day.loadFromFile("assets/tiles/a_day.png",sf::IntRect(x*16,j+y*16,xsize*16,ysize*16));
            animated_day_textures[i].push_back(texture_day);
        };

        for (int j=0;j<j;i++){
            texture_night.loadFromFile("assets/tiles/a_night.png",    sf::IntRect(x*16,j+y*16,xsize*16,ysize*16));
            animated_night_textures[i].push_back(texture_night);
        };
        };

        textures.push_back(morning_textures);
        textures.push_back(day_textures);
        textures.push_back(night_textures);

        animated_textures.push_back(animated_morning_textures);
        animated_textures.push_back(animated_day_textures);
        animated_textures.push_back(animated_night_textures);

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

    int showTile(sf::RenderWindow& window,int index,std::vector<int> pos, char time_otd){
        int time_index;
        if (time_otd=='m')time_index=0;
        if (time_otd=='d')time_index=1;
        if (time_otd=='n')time_index=2;
        sf::Sprite sprite;
        sprite.setPosition(pos[0],pos[1]);
        sprite.setTexture(textures[time_index][index]);
        sprite.scale(2.f,2.f);
        window.draw(sprite);
        return 0;  
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

    int getAnimIndex(std::string index){
        int res;
        std::string fish;
        for (int i = 0;i<animated_tiles.size();i++){
            if (animated_tiles[i][0]==getTileName(index)){
                std::cout << "Index trouvé >" << i << std::endl;
                return i;
            };
        };
        return animated_tiles.size()-1;
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

    std::string getTileName(std::string index){
        for (int i = 0;i<tiles.size();i++){
            if (index==tiles[i][2]){
                return tiles[i][0];
            };
        };
        return "unknown";
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