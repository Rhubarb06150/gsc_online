#include <SFML/Graphics.hpp>    
#include <vector>
#include <string>

class Player{

    public:

    sf::Sprite sprite;
    std::vector<sf::Texture> boy_textures;
    std::vector<sf::Texture> girl_textures;
    std::vector<sf::Texture> rival_textures;
    std::vector<sf::Texture> mother_textures;

    std::vector<std::vector<sf::Texture>> players_textures;

    std::vector<std::vector<std::string>> player_tiles_index;
    Player(){
        player_tiles_index.push_back({"front",    "1011"});
        player_tiles_index.push_back({"front1",   "0011"});
        player_tiles_index.push_back({"front2",   "2011"});

        player_tiles_index.push_back({"back",     "1111"});
        player_tiles_index.push_back({"back1",    "0111"});
        player_tiles_index.push_back({"back2",    "2111"});

        player_tiles_index.push_back({"left" ,    "0211"});
        player_tiles_index.push_back({"left1",    "1211"});

        player_tiles_index.push_back({"right" ,   "0311"});
        player_tiles_index.push_back({"right1",   "1311"});

        int x;
        int y;
        int xsize;
        int ysize;

        for (int i = 0;i<player_tiles_index.size();i++){

            std::string fish = player_tiles_index[i][1];

            x = hexToInt(fish[0]);
            y = hexToInt(fish[1]);
            xsize = hexToInt(fish[2]);
            ysize = hexToInt(fish[3]);

            //le xsize et le ysize foncitonnent correctement;

            sf::Texture boy_texture;
            sf::Texture girl_texture;
            sf::Texture rival_texture;
            sf::Texture mother_texture;

            boy_texture.loadFromFile("assets/player/boy.png",sf::IntRect(x*16,y*16,xsize*16,ysize*16));
            boy_textures.push_back(boy_texture);

            girl_texture.loadFromFile("assets/player/girl.png",sf::IntRect(x*16,y*16,xsize*16,ysize*16));
            girl_textures.push_back(girl_texture);

            rival_texture.loadFromFile("assets/player/rival.png",sf::IntRect(x*16,y*16,xsize*16,ysize*16));
            rival_textures.push_back(rival_texture);

            mother_texture.loadFromFile("assets/player/mother.png",sf::IntRect(x*16,y*16,xsize*16,ysize*16));
            mother_textures.push_back(mother_texture);
            };

            players_textures.push_back(boy_textures);
            players_textures.push_back(girl_textures);
            players_textures.push_back(rival_textures);
            players_textures.push_back(mother_textures);
      
    };
    Player(std::vector<int> player_pos){

        sprite.setPosition(player_pos[0],player_pos[1]);
        sprite.scale(4.f,4.f);
    };

    int getIndex(std::string index){
        for (int i = 0;i<player_tiles_index.size();i++){
            if (player_tiles_index[i][2]==index){
                return i;
            };
        };
    };

    std::string animPlayer(std::string cur_state, int index_frame, bool walking){

    if (walking){

        if ((cur_state=="front")&&(index_frame%60>45))return "front1";
        if ((cur_state=="front")&&(index_frame%30>15))return "front2";

        if ((cur_state=="back")&&(index_frame%60>45))return "back1";
        if ((cur_state=="back")&&(index_frame%30>15))return "back2";

        if ((cur_state=="right")&&(index_frame%30>15))return "right1";
        if ((cur_state=="left")&&(index_frame%30>15))return "left1";

    }else{
        if (cur_state=="front1"||cur_state=="front2")return "front";
        if (cur_state=="back1"||cur_state=="back2")return "back";
        if (cur_state=="left1")return "left";
        if (cur_state=="right1")return "right";
    }
    return cur_state;
    };

    int getTextureIndex(std::string index){
        for (int i = 0; i < player_tiles_index.size(); i++){
            if (index==player_tiles_index[i][0])return i;
        };
        return 0;
    };

    int showPlayer(std::string gender, std::string player_state, sf::RenderWindow& window){
        
        int gender_index;
        if (gender=="b")gender_index=0;
        if (gender=="g")gender_index=1;
        if (gender=="r")gender_index=2;
        if (gender=="m")gender_index=3;
        sprite.setTexture(players_textures[gender_index][getTextureIndex(player_state)]);
        window.draw(sprite);
        return 0;
    };

    std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        };
        return str;
    };

    int hexToInt(char hexa){

        std::string res;

        res=hexa;
        res=ReplaceAll(res, "a", "10");
        res=ReplaceAll(res, "b", "11");
        res=ReplaceAll(res, "c", "12");
        res=ReplaceAll(res, "d", "13");
        res=ReplaceAll(res, "e", "14");
        res=ReplaceAll(res, "f", "15");

        return std::stoi(res);
    };
};