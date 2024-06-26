#include <SFML/Graphics.hpp>
#include "functions/hud.cpp"
#include <iostream>
#include <fstream>

int main(){
    int selected_tile_index=0;
    std::vector<int> cursor_pos={0,0};
    sf::RenderWindow window;
    window.setFramerateLimit(60);
    sf::Event event;
    std::vector<std::vector<std::string>> tiles;
    sf::CircleShape cursor(2);
    cursor.setFillColor({255,0,0});
    bool tile_select=false;
    HUD HUDdisplay;
    for(int i=0;i<20;i++){
        tiles.push_back({});
        for(int j=0;j<18;j++){
            tiles[i].push_back("\x01");
        };  
    };
    window.create(sf::VideoMode(640,576), "Pokemon GSC Online Menu Editor",sf::Style::Titlebar | sf::Style::Close);
    do{
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
                return 0;
            }
            if (!tile_select){
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                    std::ofstream ofile("menu.mn");
                    for(int i=0;i<18;i++){
                        for(int j=0;j<20;j++){
                            ofile<<tiles[j][i];
                        };
                        if(i!=17){
                            ofile<<"\n";
                        };
                    };
                };
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
                    if(cursor_pos[0]<640)
                    cursor_pos[0]+=32;
                };
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                    if(cursor_pos[0]>0)
                    cursor_pos[0]-=32;
                };
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
                    if(cursor_pos[1]>0)
                    cursor_pos[1]-=32;
                };
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
                    if(cursor_pos[1]<576);
                    cursor_pos[1]+=32;
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
        float posx=cursor_pos[0];
        float posy=cursor_pos[1];
        cursor.setPosition({posx,posy});
        window.clear();
        for(int i=0;i<20;i++){
            for(int j=0;j<18;j++){
                tiles[i].push_back("\x01");
                HUDdisplay.showMenuTile(window,tiles[i][j],{i*32,j*32});
            };
        };
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)){
            int realposx=cursor_pos[0]/32;
            int realposy=cursor_pos[1]/32;
            tiles[realposx][realposy]=HUDdisplay.menu_tiles_indexes[selected_tile_index][2];
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
        window.draw(cursor);
        window.display();
    }while(true);
    return 0;
};