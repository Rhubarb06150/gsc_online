#include <vector>
#include <string>

class Items{
    public:
    std::vector<std::vector<std::string>> items;
    std::vector<std::vector<std::string>> balls;
    Items(){
        items.push_back({"\x00","NONE","Item not exist."});
        balls.push_back({"\x01","MASTER BALL","The best Ball.\n\nIt never misses."});
        balls.push_back({"\x01","ULTRA BALL","A Ball with a high\n\nrate of success. "});
        for(int i=0;i<balls.size();i++){
            items.push_back(balls[i]);
        };
    };
    int getItemIntIndexByName(std::string name){
        for(int i=0;i<items.size();i++){
            if(items[i][1]==name){
                return i;
            };
        };
        return 0;
    };
    int getItemIntIndexByIndex(std::string name){
        for(int i=0;i<items.size();i++){
            if(items[i][0]==name){
                return i;
            };
        };
        return 0;
    };
};