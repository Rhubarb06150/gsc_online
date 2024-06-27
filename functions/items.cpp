#include <vector>
#include <string>

class Items{
    public:
    std::vector<std::vector<std::string>> items;
    std::vector<std::vector<std::string>> balls;
    Items(){
        balls.push_back({"Fast BALL","A BALL for catching\nfast Pokémon."});
        balls.push_back({"Poké BALL","An item for catching\nPokémon."});
    };
};