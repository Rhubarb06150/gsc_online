#include <SFML/Graphics.hpp>
#include <iostream>

class MOD_Template{
    public:
    std::string name;
    std::string author_name;
    std::string description;
    MOD_Template(){
        name="Mod template";
        author_name="Rhubarb";
        description="A super template!";
    };
    int init(){
        return 0;
    };
    int keyPress(){
        return 0;
    };
    int passVars(){
        return 0;
    };
    int act(sf::Event event){
        return 0;
    };
    int show(sf::RenderWindow& window){
        return 0;
    };
};