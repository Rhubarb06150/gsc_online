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

    HUD(){
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
        letters_index.push_back({"é","a3"});

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
        letters_index.push_back({"","ff"});

        // MENU TEXTURES
        sf::Texture texture;
        texture.loadFromFile("assets/hud/pause.png");
        menu_textures.push_back(texture);
        texture.loadFromFile("assets/hud/r_arrow.png");
        menu_textures.push_back(texture);
        texture.loadFromFile("assets/hud/logo.png");
        menu_textures.push_back(texture);

        //EDITOR TEXTURES
        texture.loadFromFile("assets/hud/editor_bg.png");
        editor_background.push_back(texture);
        texture.loadFromFile("assets/hud/editor_bg2.png");
        editor_background.push_back(texture);
        texture.loadFromFile("assets/hud/editor_bg3.png");
        editor_background.push_back(texture);

        int x;
        int y;
        
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

        for (int i=0;i<6;i++){
            texture.loadFromFile("assets/hud/def_border.png",sf::IntRect(0,0,8,8));
            border_textures.push_back(texture);
            texture.loadFromFile("assets/hud/def_border.png",sf::IntRect(8,0,8,8));
            border_textures.push_back(texture);
            texture.loadFromFile("assets/hud/def_border.png",sf::IntRect(0,8,8,8));
            border_textures.push_back(texture);
            texture.loadFromFile("assets/hud/def_border.png",sf::IntRect(8,8,8,8));
            border_textures.push_back(texture);
            texture.loadFromFile("assets/hud/def_border.png",sf::IntRect(16,8,8,8));
            border_textures.push_back(texture);
            texture.loadFromFile("assets/hud/def_border.png",sf::IntRect(16,0,8,8));
            border_textures.push_back(texture);
        };
    };

    int drawSquare(sf::RenderWindow& window,std::vector<int> pos, std::vector<int> size){
        sf::Sprite corner_tl;
        sf::Sprite corner_tr;
        sf::Sprite corner_bl;
        sf::Sprite corner_br;
        corner_tl.setTexture(border_textures[0]);
        corner_tr.setTexture(border_textures[1]);
        corner_bl.setTexture(border_textures[2]);
        corner_br.setTexture(border_textures[3]);
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
        for(int i=0;i<(size[0]/32)-64;i++){
            sf::Sprite bar;
            bar.setTexture(border_textures[4]);
            float bar_pos;
            bar_pos=(i*32)+pos[0];
            //bar.setPosition({bar_pos,pos[1]});
            window.draw(bar);
        };
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
    int showTextBOW(std::string text, std::vector<int> text_pos, sf::RenderWindow& window){
        int y=text_pos[1];
        int x=text_pos[0];
        for (int i = 0; i < text.size(); i++){
            sf::Sprite sprite;
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
        sprite.setTexture(menu_textures[1]);
        sprite.scale(4.f,4.f);
        window.draw(sprite);
        return 0;  
    };

    int showLogo(std::vector<int> text_pos, sf::RenderWindow& window){
        sf::Sprite sprite;
        sprite.setPosition(text_pos[0],text_pos[1]);
        sprite.setTexture(menu_textures[2]);
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

    int showPauseMenu(sf::RenderWindow& window, std::string username, int screen_width){
        sf::Sprite sprite;
        sprite.setTexture(menu_textures[0]);
        sprite.scale(4.f,4.f);
        sprite.setPosition(screen_width-320,0);
        window.draw(sprite);
        showTextBOW(username,{screen_width-256,320},window);
        return 0;
    };
};