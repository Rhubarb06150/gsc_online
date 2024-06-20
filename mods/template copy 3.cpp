#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class MOD_mod3{
    public:
    std::string name;
    std::string author_name;
    bool active;
    MOD_mod3(){
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
        std::cout << "mod works!" << std::endl;
        return 0;
    };
    int show(){
        return 0;
    };
};