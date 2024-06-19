#include <filesystem>
#include <fstream>

#include "functions/functions.cpp"

class Main{
    public:
    std::string version;
    GSC_Functions F;
    Main(){
    version="0.00";
    };
    int seekMods(){
        F.log("INFO","Looking for mods...");
        for (const auto & entry : std::filesystem::directory_iterator("mods/")){
            std::filesystem::path file_path;
            file_path=entry.path();
            std::string file_path_str = file_path;
            if (file_path.extension()==".cpp"){
                std::cout<<"Is "+file_path_str+" a mod? [y/n]\n>>> ";
                std::string choice;
                std::cin>>choice;
                if (choice=="y"){
                    std::cout<<"Adding "+file_path_str+" to mods."<<std::endl;
                }else{
                    std::cout<<"Not adding "+file_path_str+" to mods."<<std::endl;
                };
            };
        };
        return 0;
    };
};
int main(){
    Main m;
    m.seekMods();
    return 0;
};