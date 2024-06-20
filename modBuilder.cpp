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
        system("rm -r /tmp/.gsc_o/source -f > /dev/null 2>&1");
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
            std::string descs="";
            std::string displays="";
            std::string var_pass="";
            std::string acts="";
            std::string types="std::vector<std::variant<";
            std::string display="";
            std::string mod_list="mods_list={";
            std::string states="mods_states={";
            std::string header_content="";
            bool found;
            for (int i=0;i<mod_classes.size();i++){
                found=false;

                types.append(mod_classes[i]);
                if (i+1<mod_classes.size()){
                    types.append(",");
                };

                states.append("true");
                if (i+1<mod_classes.size()){
                    states.append(",");
                };

                std::ifstream modfile;
                modfile.open(mod_paths_final[i]);
                std::string line;
                while (std::getline(modfile, line)){
                    if (line.find("int passVars(") != std::string::npos){
                        getVars(line);
                    }

                };

                descs.append("mods_descriptions.push_back({mod_"+F.ReplaceAll(mod_names_final[i]," ","_")+".description");
                descs.append(",mod_"+F.ReplaceAll(mod_names_final[i]," ","_")+".author_name");
                descs.append(",mod_"+F.ReplaceAll(mod_names_final[i]," ","_")+".name");
                descs.append("});\n");

                inits.append(mod_classes[i]);
                inits.append(" ");
                inits.append("mod_");
                inits.append(F.ReplaceAll(mod_names_final[i]," ","_"));
                inits.append(";\n");
                inits.append("bool ");
                inits.append("mod_");
                inits.append(F.ReplaceAll(mod_names_final[i]," ","_"));
                inits.append("_active=true;\n");

                acts.append("if(");
                acts.append("mod_");
                acts.append(F.ReplaceAll(mod_names_final[i]," ","_"));
                acts.append("_active==true");
                acts.append("){");
                acts.append("mod_");
                acts.append(F.ReplaceAll(mod_names_final[i]," ","_"));
                acts.append(".act();};");
                acts.append("\n");

                displays.append("if(");
                displays.append("mod_");
                displays.append(F.ReplaceAll(mod_names_final[i]," ","_"));
                displays.append("_active==true");
                displays.append("){");
                displays.append("mod_");
                displays.append(F.ReplaceAll(mod_names_final[i]," ","_"));
                displays.append(".show();};");
                displays.append("\n");

                var_pass.append("if(");
                var_pass.append("mod_");
                var_pass.append(F.ReplaceAll(mod_names_final[i]," ","_"));
                var_pass.append("_active==true");
                var_pass.append("){");
                var_pass.append("mod_");
                var_pass.append(F.ReplaceAll(mod_names_final[i]," ","_"));
                var_pass.append(".passvar(");
                var_pass.append(");};");
                var_pass.append("\n");

                header_content.append("#include ");
                header_content.append("\"");
                header_content.append(F.ReplaceAll(mod_paths_final[i],"mods/",""));
                header_content.append("\"");
                header_content.append("\n");

                mod_list.append("mod_");
                mod_list.append(F.ReplaceAll(mod_names_final[i]," ","_"));
                if (i+1<mod_classes.size()){
                    mod_list.append(",");
                };
            };
            mod_list.append("};");
            states.append("};");
        types.append(">> mods_list;");

        std::ifstream main_input;
        main_input.open("/tmp/.gsc_o/source/main.cpp");
        std::ofstream main_output("/tmp/.gsc_o/source/main_bis.cpp");
        std::string line;
            while (std::getline(main_input, line)){
                line=F.ReplaceAll(line,"//MOD INIT",inits);
                line=F.ReplaceAll(line,"//MOD ACT",acts);
                line=F.ReplaceAll(line,"//MOD DISPLAY",displays);
                line=F.ReplaceAll(line,"//MODS DESC",descs);
                line=F.ReplaceAll(line,"//MODS PASSVARS",var_pass);
                line=F.ReplaceAll(line,"std::vector<std::string> mods_list;",types);
                line=F.ReplaceAll(line,"mods_list={};",mod_list);
                line=F.ReplaceAll(line,"mods_states={};",states);
                main_output<<line<<"\n";
        };
        main_input.close();
        main_output.close();
        system("rm -f /tmp/.gsc_o/source/main.cpp");
        system("mv /tmp/.gsc_o/source/main_bis.cpp /tmp/.gsc_o/source/main.cpp");
        F.log("INFO","Creating header");
        
        std::ofstream header_ouput("/tmp/.gsc_o/source/mods/header.hpp");
        header_ouput<<header_content;
        header_ouput.close();
        }else{
            F.log("ERROR","Could not get source code from github");
            exit(2);
        };
        F.log("CONFIG","Building the game...");
        std::string build_str;
        build_str="g++ -c /tmp/.gsc_o/source/main.cpp&&g++ main.o -o GSC_Online_Modded -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system&&rm -f main.o";
        if (system(build_str.c_str())==0){
            if (mod_classes.size()>1){
            F.log("SUCCESS","Game built with success! (including "+std::to_string(mod_classes.size())+" mods)");
            }else{
            F.log("SUCCESS","Game built with success! (including 1 mod)");
            };
            system("rm -r /tmp/.gsc_o/* -f > /dev/null 2>&1");
        }else{
            F.log("ERROR","Failed to build the game, verify if mods doesn't contain errors.");
            //system("rm -r /tmp/.gsc_o/* -f > /dev/null 2>&1");
            exit(5);
        };
        return 0;
    };

    std::vector<std::string> getVars(std::string function){
        std::cout << "getVars" << std::endl;
        std::string line;
        std::vector<std::string> res = {};
        std::reverse(line.begin(),line.end());
        for (int i=0;i<line.length();i++){
            std::cout << line[i] << std::endl; 
        };
        return {};
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