#include <SFML/Graphics.hpp>
#include <string>

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
        // Called at end of Game()
        return 0;
    };
    int keyPress(sf::Event event){
        // Called in event loop after
        // if (event.type==sf::Event::KeyPressed)
        return 0;
    };
    int passVars(){
        // Called before your_mod.act
        return 0;
    };
    int act(){
        // Called before window.clear
        return 0;
    };
    int show(sf::RenderWindow& window){
        // Called just before window.display
        return 0;
    };
};