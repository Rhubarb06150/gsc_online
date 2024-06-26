#include <SFML/Graphics.hpp>
#include "functions/hud.cpp"

int main(){
    sf::RenderWindow window;
    std::vector<std::vector<std::string>> tiles;
    for(int i=0;i<20;i++){
        tiles.push_back({});
        for(int j=0;j<18;j++){
            tiles[i].push_back("\x01");
        };  
    };
    window.create(sf::VideoMode(640,576), "Pokemon GSC Online Menu Editor",sf::Style::Titlebar | sf::Style::Close);
    do{
    }while(true);
    return 0;
};