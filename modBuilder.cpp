#include <filesystem>
#include <fstream>
#include <regex>

#include "functions/functions.cpp"

class Main{
    public:
    std::string version;
    std::vector<std::string> mod_names;
    std::vector<std::string> mod_classes;
    std::vector<std::string> mod_paths;
    GSC_Functions F;
    Main(){
    version="0.00";
    };
    int seekMods(){
        if (!std::filesystem::is_directory("mods/")){
            F.log("ERROR","No mods folder");
            exit(1);
        };
        if (system("g++ --help > /dev/null 2>&1")!=0){
            F.log("ERROR","g++ is not installed, please install g++ for continue (sudo apt-get install g++)");
            exit(3);
        };
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
                    mod_names.push_back(file_path.stem());
                    mod_paths.push_back(file_path_str);
                }else{
                    std::cout<<"Not adding "+file_path_str+" to mods."<<std::endl;
                };
            };
        };
        F.createMissingDir("/tmp/.gsc_o/");
        system("rm -r /tmp/.gsc_o/source -f  > /dev/null 2>&1");
        F.createMissingDir("/tmp/.gsc_o/source/");
        F.log("INFO","Getting your version source code from github...");
        std::string instruction;
        instruction="git clone -b "+version+" https://github.com/Rhubarb06150/gsc_online.git /tmp/.gsc_o/source > /dev/null 2>&1";
        if (system(instruction.c_str())==0){
            system("rm -r /tmp/.gsc_o/source/mods > /dev/null 2>&1");
            system("rm -r /tmp/.gsc_o/source/help > /dev/null 2>&1");
            system("rm -f /tmp/.gsc_o/source/make > /dev/null 2>&1");
            system("rm -r /tmp/.gsc_o/source/maps > /dev/null 2>&1");
            system("rm -f /tmp/.gsc_o/source/modBuilder > /dev/null 2>&1");
            system("rm -f /tmp/.gsc_o/source/main > /dev/null 2>&1");
            system("rm -f /tmp/.gsc_o/source/modBuilder.cpp > /dev/null 2>&1");
            system("rm -f /tmp/.gsc_o/source/help.html > /dev/null 2>&1");
            F.log("INFO","Code has been retrieved from github");
            std::ifstream file;
            file.open("/tmp/.gsc_o/source/main.cpp");
            std::string line;
            const std::regex txt_regex("class");
            while (std::getline(file, line)){
                std::cout<<std::regex_match(line,txt_regex)<<"| "<<line<<std::endl;
                //line=F.ReplaceAll(line,"        //MOD INIT","hey");
            };
        }else{
            F.log("ERROR","Could not get source code from github");
            exit(2);
        };
        return 0;
    };
};
int main(){
    Main m;
    m.seekMods();
    exit(0);
    return 0;
};