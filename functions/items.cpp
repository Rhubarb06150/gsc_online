#include <vector>
#include <string>

class Items{
    public:
    std::vector<std::vector<std::string>> items;
    Items(){
        items.push_back({"ITEMS","\x00","NONE","Item not exist."});
        items.push_back({"BALLS","\x01","MASTER BALL","The best Ball.\n\nIt never misses."});
        items.push_back({"BALLS","\x02","ULTRA BALL","A Ball with a high\n\nrate of success."});
        items.push_back({"ITEMS","\x03","BRIGHTPOWDER","Lowers the foe's\n\naccuracy. (HOLD)"});
        items.push_back({"BALLS","\x03","GREAT BALL","A Ball with a decent\n\nsuccess rate."});
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