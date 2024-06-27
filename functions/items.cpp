#include <vector>
#include <string>

class Items{
    public:
    std::vector<std::vector<std::string>> items;
    std::vector<std::vector<std::string>> balls;
    Items(){
        items.push_back({"NONE","Item not exist."});
        balls.push_back({"FAST BALL","A BALL for \n\ncatchingfast Pokémon."});
        balls.push_back({"POKé BALL","An item for\n\ncatching Pokémon."});
        for(int i=0;i<balls.size();i++){
            items.push_back(balls[i]);
        };
    };
    int getItemIntIndexByName(std::string name){
        for(int i=0;i<items.size();i++){
            if(items[i][0]==name){
                return i;
            };
        };
        return 0;
    };
};