#include <SFML/Graphics.hpp>
#include "functions/hud.cpp"
#include <iostream>
#include <filesystem>
#include <fstream>

int main(){
    int selected_tile_index=0;
    sf::RenderWindow window;
    window.setFramerateLimit(60);
    sf::Event event;
    std::vector<std::vector<std::string>> tiles;
    bool tile_select=false;
    HUD HUDdisplay;
    bool stop=false;
    for(int i=0;i<18;i++){
        tiles.push_back({});
        for(int j=0;j<20;j++){
            tiles[i].push_back("\x01");
        };
    };
    window.create(sf::VideoMode(640,576), "GSC Online Menu Editor",sf::Style::Titlebar | sf::Style::Close);
    do{
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
                return 0;
            };

            if (!tile_select){
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                    std::ofstream ofile("menu.mn");
                    for(int i=0;i<tiles.size();i++){
                        for(int j=0;j<tiles[i].size();j++){
                            ofile<<tiles[i][j];
                        };
                        if(i!=17){
                            ofile<<"\n";
                        };
                    };
                };
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)){
                    int realposx=sf::Mouse::getPosition(window).x/32;
                    int realposy=sf::Mouse::getPosition(window).y/32;
                    stop=false;
                    if(realposx>=20){
                        stop=true;
                    };
                    if(realposy>=18){
                        stop=true;
                    };
                    if(realposx<=-1){
                        stop=true;
                    };
                    if(realposy<=-1){
                        stop=true;
                    };
                    //std::cout << realposx << "|" << realposy << std::endl;
                    if (!stop)
                    selected_tile_index=HUDdisplay.getMenuTileIndex(tiles[realposy][realposx]);
                };
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)){
                    std::string path;
                    std::cout << "path?\n> ";
                    std::cin >> path;
                    if(std::filesystem::is_regular_file("assets/menus/"+path)){
                        std::ifstream ifile("assets/menus/"+path);
                        tiles={};
                        std::string line;
                        while(getline(ifile,line)){
                            tiles.push_back({});
                            for(int j=0;j<line.length();j++){
                                std::string cur_tile;
                                cur_tile=line[j];
                                tiles[tiles.size()-1].push_back(cur_tile);
                            };
                        };
                        std::cout<<"Map loaded"<<std::endl;
                    }else{
                        std::cout<<"Cant find this menu!"<<std::endl;
                    }
                };
            }else{
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
                    if (selected_tile_index<HUDdisplay.menu_tiles.size()-1)
                    selected_tile_index++;
                };
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                    if (selected_tile_index>0)
                    selected_tile_index--;
                };
            };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)){
                tile_select=!tile_select;
            };
        };
        window.clear();
        for(int i=0;i<tiles.size();i++){
            for(int j=0;j<tiles[i].size();j++){
                HUDdisplay.showMenuTile(window,tiles[i][j],{j*32,i*32});
                //std::cout << tiles[i][j];
            };
            //std::cout << std::endl;
        };
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            stop=false;
            int realposx=sf::Mouse::getPosition(window).x/32;
            int realposy=sf::Mouse::getPosition(window).y/32;
            if(realposx>=20){
                stop=true;
            };
            if(realposy>=18){
                stop=true;
            };
            if(realposx<=-1){
                stop=true;
            };
            if(realposy<=-1){
                stop=true;
            };
            //std::cout << realposx << "|" << realposy << std::endl;
            if (!stop)
            tiles[realposy][realposx]=HUDdisplay.menu_tiles_indexes[selected_tile_index][2];
        };
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
            stop=false;
            int realposx=sf::Mouse::getPosition(window).x/32;
            int realposy=sf::Mouse::getPosition(window).y/32;
            if(realposx>=20){
                stop=true;
            };
            if(realposy>=18){
                stop=true;
            };
            if(realposx<=-1){
                stop=true;
            };
            if(realposy<=-1){
                stop=true;
            };
            //std::cout << realposx << "|" << realposy << std::endl;
            if (!stop)
            tiles[realposy][realposx]=HUDdisplay.menu_tiles_indexes[2][2];
        };
        if (tile_select){
            sf::RectangleShape rect({128,128});
            rect.setFillColor({0,0,0});
            rect.setPosition(0,0);
            window.draw(rect);
            sf::Sprite tile;
            tile.setPosition(0,0);
            tile.setTexture(HUDdisplay.menu_tiles[selected_tile_index]);
            tile.setScale(4.f,4.f);
            window.draw(tile);
        };
        window.display();
    }while(true);
    return 0;
};