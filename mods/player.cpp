#include <string>

class MOD_Player{
    public:
    std::string name;
    std::string author_name;
    bool active;
    MOD_Player(){
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