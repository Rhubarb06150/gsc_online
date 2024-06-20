//////////////////////////////////////
//
//  Here is a mod template, GSC Online supports mods
//  To activate a mod, just put it in a folder called "mods" at the root of the game folder
//  It will be activated by itself
//
//////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <string>

class Mod{
    public:
    std::string name;
    std::string author_name;
    bool active;
    Mod(){
        name="Demo mod";
        author_name="Rhubarb";
        active=true;
    };
    int init(){
        return 0;
    };
    int passVars(){
        return 0;
    };
    int act(){
        return 0;
    };
    int show(){
        return 0;
    };
};