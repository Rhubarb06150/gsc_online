#include <filesystem>
#include <algorithm>
#include <fstream>
#include <regex>

#include "functions/functions.cpp"

class Main{
    public:
    std::string version;
    int mods;
    int mods_final;
    std::vector<std::string> mod_names;
    std::vector<std::string> mod_names_final;
    std::vector<std::string> mod_classes;
    std::vector<std::string> mod_paths;
    std::vector<std::string> mod_paths_final;
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
                    mods++;
                }else{
                    std::cout<<"Not adding "+file_path_str+" to mods."<<std::endl;
                };
            };
        };
        if(mods==0){
            F.log("INFO","No mods to configure.");
            exit(4);
        };
        
        F.createMissingDir("/tmp/.gsc_o/");
        system("rm -r /tmp/.gsc_o/source -f  > /dev/null 2>&1");
        F.log("CONFIG","Setup of the temp source folder");
        F.createMissingDir("/tmp/.gsc_o/source/");
        F.log("INFO","Getting your version source code from github...");
        std::string instruction;
        instruction="git clone -b "+version+" https://github.com/Rhubarb06150/gsc_online.git /tmp/.gsc_o/source > /dev/null 2>&1";
        if (system(instruction.c_str())==0){
            F.log("INFO","Cleaning...");
            system("rm -r /tmp/.gsc_o/source/help > /dev/null 2>&1");
            system("rm -r /tmp/.gsc_o/source/mods > /dev/null 2>&1");
            system("rm -f /tmp/.gsc_o/source/make > /dev/null 2>&1");
            system("rm -r /tmp/.gsc_o/source/maps > /dev/null 2>&1");
            system("rm -f /tmp/.gsc_o/source/modBuilder > /dev/null 2>&1");
            system("rm -f /tmp/.gsc_o/source/main > /dev/null 2>&1");
            system("rm -f /tmp/.gsc_o/source/modBuilder.cpp > /dev/null 2>&1");
            system("rm -f /tmp/.gsc_o/source/help.html > /dev/null 2>&1");
            F.createMissingDir("/tmp/.gsc_o/source/mods");
            F.log("INFO","Code has been retrieved from github");
            F.log("MOD","Configuring following mods:");
            for(int i=0;i<mod_paths.size();i++){
                std::cout << "    -"+mod_names[i]<<std::endl;
            };
            for(int i=0;i<mod_paths.size();i++){
                bool found=false;
                F.log("MOD","Config of "+mod_names[i]);
                std::ifstream file;
                file.open(mod_paths[i]);
                std::string line;
                const std::regex regex("(class MOD_)*");
                while (std::getline(file, line)){
                    line=F.ReplaceAll(line,"\n","");
                    if (found){
                        std::string class_name;
                        class_name=F.ReplaceAll(line," ","");
                        class_name=F.ReplaceAll(class_name,"{","");
                        class_name=F.ReplaceAll(class_name,"class","");
                        F.log("INFO","Found "+mod_names[i]+" main class ("+class_name+"), adding mod to header and to main source file...");
                        if(std::find(mod_classes.begin(), mod_classes.end(), class_name) != mod_classes.end()) {
                            F.log("ERROR","A mod with the same class is already configured, "+mod_names[i]+" mod will not be included.");
                        } else {
                            std::string inst;
                            F.log("INFO","Copying "+mod_paths[i]+" in build folder...");
                            inst="cp "+F.ReplaceAll(mod_paths[i]," ","\\ ")+" /tmp/.gsc_o/source/mods > /dev/null 2>&1";
                            if (system(inst.c_str())==0){
                                F.log("INFO","Copied!");
                            }else{
                                F.log("ERROR","failed to copy "+mod_paths[i]+" in build folder");
                                break;
                            };
                            mod_classes.push_back(class_name);
                            mod_paths_final.push_back(mod_paths[i]);
                            mod_names_final.push_back(mod_names[i]);
                            F.log("INFO",mod_names[i]+" has been configured correctly!");
                        }
                        break;
                    };
                    if (regex_match(line,regex)){
                        found=true;
                    };
                };
                std::cout << std::endl;
            };
            F.log("CONFIG","Adding mods in build source main file");
            std::string inits="";
            std::string acts="";
            std::string display="";
            std::string header="";
            for (int i=0;i<mod_classes.size();i++){
                inits.append("        ");
                inits.append(mod_classes[i]);
                inits.append(" ");
                inits.append(F.ReplaceAll(mod_names_final[i]," ","_"));
                inits.append(";\n");
                
                header.append("#include "+'"'+mod_paths_final[i]+'"'+"\n");
            };
            std::cout<<inits<<std::endl;
            std::cout<<header<<std::endl;   
        }else{
            F.log("ERROR","Could not get source code from github");
            exit(2);
        };
        return 0;
    };
};
int main(){
    Main m;
    m.F.log("ENGINE","GSC Online Mod Builder for "+m.version);
    std::cout << std::endl;
    if (m.F.getUserPath()=="/home/rhubarb"){
        std::string branch_version = m.version;
        system(("git add . > /dev/null 2>&1&&git commit -m 'working on mods' > /dev/null 2>&1&&git push origin  main:"+branch_version+" > /dev/null 2>&1&").c_str());
    };
    m.seekMods();
    exit(0);
    return 0;
};