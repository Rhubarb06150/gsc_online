#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class MOD_mod{
    public:
    std::string name;
    std::string author_name;
    std::string description;
    bool active;
    MOD_mod(){
        name="Demo mod";
        author_name="Rhubarb";
        description="The basic template";
        active=true;
    };
    int init(){
        return 0;
    };
    int passVars(){
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