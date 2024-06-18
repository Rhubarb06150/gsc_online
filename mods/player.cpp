//////////////////////////////////////
//
//  Here is a mod template, GSC Online supports mods
//  To activate a mod, just put it in a folder called "mods" at the root of the game folder
//  It will be activated by itself
//
//////////////////////////////////////
#include <string>
#include "../main.cpp"

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
    int act(){
        //Your code here
        //The act function is called just before the window.clear()
        return 0;
    };
    int show(){
        //Your code here
        //The act function is called just before the window.display()
        return 0;
    };
};