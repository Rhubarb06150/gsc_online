#include <SFML/Graphics.hpp>
#include <string>

class MOD_mod{
    public:
    std::string name;
    std::string author_name;
    bool active;
    MOD_mod(){
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