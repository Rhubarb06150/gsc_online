#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class MOD_MiniMAP{
    public:
    std::string name;
    std::string author_name;
    std::string description;
    bool active;
    MOD_MiniMAP(){
        name="Mini Map";
        author_name="Rhubarb";
        description="A simple mini map test\nI hope you'll enjoy :)";
        active=true;
    };
    int init(){
        return 0;
    };
    int passVars(std::vector<int> player_pos, sf::RenderWindow& window){
        return 0;
    };
    int act(){
        std::cout << "mod works!" << std::endl;
        return 0;
    };
    int show(){
        return 0;
    };
};