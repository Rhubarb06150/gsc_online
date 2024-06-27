///////////////////////////////////////////////////////////
//
//  And here is all functions related to HUD (no shit) and the text display
//      
//  Here are what showText functions do:
//
//      showTextDEBUG : show white text on black transparent background.
//      showTextSELDEBUG : show yellow text on black transparent background (usually used when you are selecting something).
//      showTextBluDEBUG : show blue text on black transparent background (used in level editor).
//      showTextBOW : show black text on white background.
//      showTextWOB : show white text on black background.
//
//          Note : the showText debug functions display text twice smaller than the regular ones.
//
//  There is also a function thats display an menu pause (which is unused for now)
//
///////////////////////////////////////////////////////////

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

#include "functions.cpp"

class HUD{
    private:
    GSC_Functions funs;
    public:
    int editor_bg_index;
    std::vector<std::vector<std::string>> letters_index;
    std::vector<sf::Texture> letters_wob_textures;
    std::vector<sf::Texture> letters_bow_textures;
    std::vector<sf::Texture> letters_debug_textures;
    std::vector<sf::Texture> letters_debug_sel_textures;
    std::vector<sf::Texture> letters_debug_blu_textures;
    std::vector<sf::Texture> menu_textures;
    std::vector<sf::Texture> editor_background;
    std::vector<sf::Texture> border_textures;
    std::vector<std::vector<std::string>> menu_tiles_indexes;
    std::vector<sf::Texture> menu_tiles;

    HUD(){
        editor_bg_index=0;
        letters_index.push_back({" ","ef"});
        //UPPERCASES
        letters_index.push_back({"0","0f"});
        letters_index.push_back({"1","1f"});
        letters_index.push_back({"2","2f"});
        letters_index.push_back({"3","3f"});
        letters_index.push_back({"4","4f"});
        letters_index.push_back({"5","5f"});
        letters_index.push_back({"6","6f"});
        letters_index.push_back({"7","7f"});
        letters_index.push_back({"8","8f"});
        letters_index.push_back({"9","9f"});

        letters_index.push_back({"A","00"});
        letters_index.push_back({"B","10"});
        letters_index.push_back({"C","20"});
        letters_index.push_back({"D","30"});
        letters_index.push_back({"E","40"});
        letters_index.push_back({"F","50"});
        letters_index.push_back({"G","60"});
        letters_index.push_back({"H","70"});
        letters_index.push_back({"I","80"});
        letters_index.push_back({"J","90"});
        letters_index.push_back({"K","a0"});
        letters_index.push_back({"L","b0"});
        letters_index.push_back({"M","c0"});
        letters_index.push_back({"N","d0"});
        letters_index.push_back({"O","e0"});
        letters_index.push_back({"P","f0"});
        letters_index.push_back({"Q","01"});
        letters_index.push_back({"R","11"});
        letters_index.push_back({"S","21"});
        letters_index.push_back({"T","31"});
        letters_index.push_back({"U","41"});
        letters_index.push_back({"V","51"});
        letters_index.push_back({"W","61"});
        letters_index.push_back({"X","71"});
        letters_index.push_back({"Y","81"});
        letters_index.push_back({"Z","91"});

        letters_index.push_back({"a","02"});
        letters_index.push_back({"b","12"});
        letters_index.push_back({"c","22"});
        letters_index.push_back({"d","32"});
        letters_index.push_back({"e","42"});
        letters_index.push_back({"\xed","a3"});//é
        letters_index.push_back({"f","52"});
        letters_index.push_back({"g","62"});
        letters_index.push_back({"h","72"});
        letters_index.push_back({"i","82"});
        letters_index.push_back({"j","92"});
        letters_index.push_back({"k","a2"});
        letters_index.push_back({"l","b2"});
        letters_index.push_back({"m","c2"});
        letters_index.push_back({"n","d2"});
        letters_index.push_back({"o","e2"});
        letters_index.push_back({"p","f2"});
        letters_index.push_back({"q","03"});
        letters_index.push_back({"r","13"});
        letters_index.push_back({"s","23"});
        letters_index.push_back({"t","33"});
        letters_index.push_back({"u","43"});
        letters_index.push_back({"v","53"});
        letters_index.push_back({"w","63"});
        letters_index.push_back({"x","73"});
        letters_index.push_back({"y","83"});
        letters_index.push_back({"z","93"});

        //NONE
        letters_index.push_back({":","0e"});
        letters_index.push_back({"/","1e"});
        letters_index.push_back({"\\","2e"});
        letters_index.push_back({",","3e"});
        letters_index.push_back({".","4e"});
        letters_index.push_back({"(","5e"});
        letters_index.push_back({")","6e"});
        letters_index.push_back({";","7e"});
        letters_index.push_back({"[","8e"});
        letters_index.push_back({"]","9e"});
        letters_index.push_back({"'","ae"});
        letters_index.push_back({"-","be"});
        letters_index.push_back({"_","bf"});
        letters_index.push_back({"+","ce"});
        letters_index.push_back({"?","de"});
        letters_index.push_back({"!","ee"});
        letters_index.push_back({"\xee","cf"});//PO
        letters_index.push_back({"\xef","df"});//Ké
        letters_index.push_back({"\xde","dd"});//PK
        letters_index.push_back({"\xdf","ed"});//MN

        letters_index.push_back({"","de"});

        // MENU TEXTURES
        sf::Texture texture;
        texture.loadFromFile("assets/hud/logo.png");
        menu_textures.push_back(texture);

        // MENU TILES INDEX
        menu_tiles_indexes.push_back({"empty","ff11"," "});
        menu_tiles_indexes.push_back({"cursor","7111","\x00"});
        menu_tiles_indexes.push_back({"white","3211","\x01"});
        menu_tiles_indexes.push_back({"blueSquare","0011","\xe0"});
        menu_tiles_indexes.push_back({"blueSlopeBL","1011","\xe1"});
        menu_tiles_indexes.push_back({"blueSlopeTR","2011","\xe2"});
        menu_tiles_indexes.push_back({"blueSlopeTL","4311","\xe3"});
        menu_tiles_indexes.push_back({"blueSlopeBR","5311","\xe4"});
        menu_tiles_indexes.push_back({"status","2151","\xa0"});
        menu_tiles_indexes.push_back({"wideLine","0111","\xd0"});
        menu_tiles_indexes.push_back({"wideLineRight","1111","\xd1"});
        menu_tiles_indexes.push_back({"wideLineLeft","1311","\xd2"});
        menu_tiles_indexes.push_back({"carpet","3311","\xb0"});
        menu_tiles_indexes.push_back({"itemsLabel","0461","\xb1"});
        menu_tiles_indexes.push_back({"pocketLabel","0561","\xb2"});
        menu_tiles_indexes.push_back({"itemsLabel_Pack","b053","\xb3"});
        menu_tiles_indexes.push_back({"ballsLabel","b353","\xb4"});
        menu_tiles_indexes.push_back({"keysItemsLabel","b653","\xb5"});
        menu_tiles_indexes.push_back({"TMTHLabel","b953","\xb6"});
        menu_tiles_indexes.push_back({"ballsLabel2","0831","\xc0"});
        menu_tiles_indexes.push_back({"itemsLabel2","0931","\xc1"});
        menu_tiles_indexes.push_back({"keysItemsLabel2","0a51","\xc2"});
        menu_tiles_indexes.push_back({"TMTHLabel2","0b51","\xc3"});
        menu_tiles_indexes.push_back({"bagBoxTL","0611","\xb7"});
        menu_tiles_indexes.push_back({"bagBoxTR","1611","\xb8"});
        menu_tiles_indexes.push_back({"bagBoxBL","0711","\xb9"});
        menu_tiles_indexes.push_back({"bagBoxBR","1711","\xba"});
        menu_tiles_indexes.push_back({"bagBoxT","2611","\xbb"});
        menu_tiles_indexes.push_back({"bagBoxB","2711","\xbc"});
        menu_tiles_indexes.push_back({"bagBoxL","3611","\xbd"});
        menu_tiles_indexes.push_back({"bagBoxR","3711","\xbe"});
        menu_tiles_indexes.push_back({"redCursor","7211","\x00"});
        menu_tiles_indexes.push_back({"idN","6421","\xd5"});

        //EDITOR TEXTURES
        texture.loadFromFile("assets/hud/editor_bg.png");
        editor_background.push_back(texture);
        texture.loadFromFile("assets/hud/editor_bg2.png");
        editor_background.push_back(texture);
        texture.loadFromFile("assets/hud/editor_bg3.png");
        editor_background.push_back(texture);
        texture.loadFromFile("assets/hud/editor_bg4.png");
        editor_background.push_back(texture);

        int x;
        int y;
        int xsize;
        int ysize;

        for(int i=0;i<menu_tiles_indexes.size();i++){
            sf::Texture texture;
            std::string fish = menu_tiles_indexes[i][1];
            x=funs.hexToInt(fish[0]);
            y=funs.hexToInt(fish[1]);
            xsize=funs.hexToInt(fish[2]);
            ysize=funs.hexToInt(fish[3]);
            texture.loadFromFile("assets/hud/menus.png",sf::IntRect(x*8,y*8,xsize*8,ysize*8));
            menu_tiles.push_back(texture);
        };
        
        sf::Sprite sprite;

        for (int i = 0;i<letters_index.size();i++){

            std::string fish = letters_index[i][1];

            x = funs.hexToInt(fish[0]);
            y = funs.hexToInt(fish[1]);

            texture.loadFromFile("assets/hud/font_bow.png",sf::IntRect(x*8,y*8,8,8));
            letters_bow_textures.push_back(texture);
            texture.loadFromFile("assets/hud/font_wob.png",sf::IntRect(x*8,y*8,8,8));
            letters_wob_textures.push_back(texture);
            texture.loadFromFile("assets/hud/font_debug.png",sf::IntRect(x*8,y*8,8,8));
            letters_debug_textures.push_back(texture);
            texture.loadFromFile("assets/hud/font_debug_sel.png",sf::IntRect(x*8,y*8,8,8));
            letters_debug_sel_textures.push_back(texture);
            texture.loadFromFile("assets/hud/font_debug_blu.png",sf::IntRect(x*8,y*8,8,8));
            letters_debug_blu_textures.push_back(texture);
        };

        for (int k=0;k<11;k++){
            texture.loadFromFile("assets/hud/border.png",sf::IntRect(0,0+(k*16),8,8));//TOP LEFT CORNER
            border_textures.push_back(texture);
            texture.loadFromFile("assets/hud/border.png",sf::IntRect(16,0+(k*16),8,8));//TOP RIGHT CORNER
            border_textures.push_back(texture);
            texture.loadFromFile("assets/hud/border.png",sf::IntRect(8,8+(k*16),8,8));//BOT LEFT CORNER
            border_textures.push_back(texture);
            texture.loadFromFile("assets/hud/border.png",sf::IntRect(16,8+(k*16),8,8));//BOT RIGHT CORNER
            border_textures.push_back(texture);
            texture.loadFromFile("assets/hud/border.png",sf::IntRect(8,0+(k*16),8,8));//ROW CORNER
            border_textures.push_back(texture);
            texture.loadFromFile("assets/hud/border.png",sf::IntRect(0,8+(k*16),8,8));//COLUMN CORNER
            border_textures.push_back(texture);
        };
    };

    int getMenuTileIndex(std::string tile_index){
        for(int i=0;i<menu_tiles.size();i++){
            if (tile_index==menu_tiles_indexes[i][2]){
                return i;
            };
        };
        return 0;
    };

    int showMenuTile(sf::RenderWindow& window, std::string tile_index, std::vector<int> pos){
        sf::Sprite tile;
        tile.setTexture(menu_tiles[getMenuTileIndex(tile_index)]);
        tile.setPosition(pos[0],pos[1]);
        tile.setScale(4.f,4.f);
        window.draw(tile);
        return 0;
    };

    int showMenu(sf::RenderWindow& window,std::string menu_path){
        std::ifstream menufile(menu_path);
        std::string line;
        int x=0;
        int y=0;
        while(getline(menufile,line)){
            for(int i=0;i<20;i++){
                sf::Sprite tile;
                std::string cur_index;
                cur_index=line[i];
                tile.setTexture(menu_tiles[getMenuTileIndex(cur_index)]);
                tile.setPosition(x,y);
                tile.setScale(4.f,4.f);
                window.draw(tile);
                x+=32;
            };
            y+=32;
            x=0;
        };
        return 0;
    };

    int drawSquare(sf::RenderWindow& window,std::vector<int> pos, std::vector<int> size, int border_index){
        sf::Sprite corner_tl;
        sf::Sprite corner_tr;
        sf::Sprite corner_bl;
        sf::Sprite corner_br;
        int border_offset=border_index*6;
        corner_tl.setTexture(border_textures[0+border_offset]);
        corner_tr.setTexture(border_textures[1+border_offset]);
        corner_bl.setTexture(border_textures[2+border_offset]);
        corner_br.setTexture(border_textures[3+border_offset]);
        corner_tl.setScale(4.f,4.f);
        corner_tr.setScale(4.f,4.f);
        corner_bl.setScale(4.f,4.f);
        corner_br.setScale(4.f,4.f);
        float pos0tl=pos[0];
        float pos1tl=pos[1];
        float pos0tr=pos[0]+size[0]-32;
        float pos1tr=pos[1];
        float pos0bl=pos[0];
        float pos1bl=pos[1]+size[1]-32;
        float pos0br=pos[0]+size[0]-32;
        float pos1br=pos[1]+size[1]-32;
        corner_tl.setPosition({pos0tl,pos1tl});
        corner_tr.setPosition({pos0tr,pos1tr});
        corner_bl.setPosition({pos0bl,pos1bl});
        corner_br.setPosition({pos0br,pos1br});
        window.draw(corner_tl);
        window.draw(corner_tr);
        window.draw(corner_bl);
        window.draw(corner_br);
        for(int i=0;i<(size[0]-64)/32;i++){
            sf::Sprite bar;
            bar.setTexture(border_textures[4+border_offset]);
            float bar_pos;
            bar_pos=32+(i*32)+pos[0];
            bar.setScale(4.f,4.f);
            bar.setPosition({bar_pos,pos1tl});
            window.draw(bar);
        };
        for(int i=0;i<(size[0]-64)/32;i++){
            sf::Sprite bar;
            bar.setTexture(border_textures[4+border_offset]);
            float bar_pos;
            bar_pos=32+(i*32)+pos[0];
            bar.setScale(4.f,4.f);
            bar.setPosition({bar_pos,pos1bl});
            window.draw(bar);
        };
        for(int i=0;i<(size[1]-64)/32;i++){
            sf::Sprite bar;
            bar.setTexture(border_textures[5+border_offset]);
            float bar_pos;
            bar_pos=32+(i*32)+pos[1];
            bar.setScale(4.f,4.f);
            bar.setPosition({pos0bl,bar_pos});
            window.draw(bar);
        };
        for(int i=0;i<(size[1]-64)/32;i++){
            sf::Sprite bar;
            bar.setTexture(border_textures[5+border_offset]);
            float bar_pos;
            bar_pos=32+(i*32)+pos[1];
            bar.setScale(4.f,4.f);
            bar.setPosition({pos0br,bar_pos});
            window.draw(bar);
        };
        float fx;
        fx=size[0]-64;
        float fy;
        fy=size[1]-64;
        sf::RectangleShape fill({fx,fy});
        fill.setFillColor({255,255,255});
        fill.setPosition(pos0tl+32,pos1tl+32);
        window.draw(fill);
        return 0;
    };

    int getLetterIntIndex(char index){
        std::string fish;
        fish=index;
        for (int i = 0;i<letters_index.size();i++){
            if (letters_index[i][0]==fish){
                return i;};
        };
        return letters_index.size()-1;
    };

    std::string convertText(std::string text){
        std::string res;
        res=funs.ReplaceAll(text,"é","\xed");
        return res;
    };

    int showTextBOW(std::string text, std::vector<int> text_pos, sf::RenderWindow& window){
        text=convertText(text);
        int y=text_pos[1];
        int x=text_pos[0];
        for (int i = 0; i < text.size(); i++){
            sf::Sprite sprite;
            //std::cout << text[i];
            if(text[i]=='\n'){
                y+=32;
                x=text_pos[0];
            }else{
                sprite.setPosition(x,y);
                sprite.setTexture(letters_bow_textures[getLetterIntIndex((text[i]))]);
                sprite.scale(4.f,4.f);
                window.draw(sprite);
                x+=32;
            };
        };
        //std::cout << std::endl;
        return 0;
    };
    int showTextAskBOW(std::string text, std::vector<int> text_pos, sf::RenderWindow& window){
        int y=text_pos[1];
        int x=text_pos[0];
        for (int i = 0; i < text.size(); i++){
            sf::Sprite sprite;
            if (i%18==0&&i!=0){
                y+=32;
                x=text_pos[0];
            }
            if(text[i]=='\n'){
                y+=32;
                x=text_pos[0];
            }else{
                sprite.setPosition(x,y);
                sprite.setTexture(letters_bow_textures[getLetterIntIndex((text[i]))]);
                sprite.scale(4.f,4.f);
                window.draw(sprite);
                x+=32;
            };
        };
        return 0;  
    };
    int showTextWOB(std::string text, std::vector<int> text_pos, sf::RenderWindow& window){
        int y=text_pos[1];
        int x=text_pos[0];
        for (int i = 0; i < text.size(); i++){
            sf::Sprite sprite;
            if(text[i]=='\n'){
                y+=32;
                x=text_pos[0];
            }else{
                sprite.setPosition(x,y);
                sprite.setTexture(letters_wob_textures[getLetterIntIndex((text[i]))]);
                sprite.scale(4.f,4.f);
                window.draw(sprite);
                x+=32;
            };
        };
        return 0;  
    };

    int showCursor(std::vector<int> text_pos, sf::RenderWindow& window){
        sf::Sprite sprite;
        sprite.setPosition(text_pos[0],text_pos[1]);
        sprite.setTexture(menu_tiles[1]);
        sprite.scale(4.f,4.f);
        window.draw(sprite);
        return 0;  
    };
    int showRedCursor(std::vector<int> text_pos, sf::RenderWindow& window){
        sf::Sprite sprite;
        sprite.setPosition(text_pos[0],text_pos[1]);
        sprite.setTexture(menu_tiles[31]);
        sprite.scale(4.f,4.f);
        window.draw(sprite);
        return 0;  
    };

    int showLogo(std::vector<int> text_pos, sf::RenderWindow& window){
        sf::Sprite sprite;
        sprite.setPosition(text_pos[0],text_pos[1]);
        sprite.setTexture(menu_textures[0]);
        sprite.scale(4.f,4.f);
        window.draw(sprite);
        return 0;  
    };
    int editorBG(sf::RenderWindow& window){
        sf::Sprite sprite;
        sprite.setPosition(0,0);
        sprite.setTexture(editor_background[editor_bg_index]);
        sprite.scale(4.f,4.f);
        window.draw(sprite);
        return 0;  
    };

    int showTextDEBUG(std::string text, std::vector<int> text_pos, sf::RenderWindow& window){
        int y=text_pos[1];
        int x=text_pos[0];
        for (int i = 0; i < text.size(); i++){
            sf::Sprite sprite;
            if(text[i]=='\n'){
                y+=16;
                x=text_pos[0];
            }else{
                sprite.setPosition(x,y);
                sprite.setTexture(letters_debug_textures[getLetterIntIndex((text[i]))]);
                sprite.scale(2.f,2.f);
                window.draw(sprite);
                x+=16;
            };
        };
        return 0; 
    };

    int showTextSELDEBUG(std::string text, std::vector<int> text_pos, sf::RenderWindow& window){
        for (int i = 0; i < text.size(); i++){
            sf::Sprite sprite;
            sprite.setPosition(text_pos[0]+(i*16),text_pos[1]);
            sprite.setTexture(letters_debug_sel_textures[getLetterIntIndex((text[i]))]);
            sprite.scale(2.f,2.f);
            window.draw(sprite);
        };
        return 0;  
    };

    int showTextBluDEBUG(std::string text, std::vector<int> text_pos, sf::RenderWindow& window){
        for (int i = 0; i < text.size(); i++){
            sf::Sprite sprite;
            sprite.setPosition(text_pos[0]+(i*16),text_pos[1]);
            sprite.setTexture(letters_debug_blu_textures[getLetterIntIndex((text[i]))]);
            sprite.scale(2.f,2.f);
            window.draw(sprite);
        };
        return 0;  
    };
};