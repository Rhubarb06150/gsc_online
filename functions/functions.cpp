///////////////////////////////////////////////////////////
//
//  Hey! Here are the usefull functions that are called sometimes
//
//  There are some functions here that I didn't write myself like ReplaceAll or takeScreenshot
//      also the currentDateTime 
//
///////////////////////////////////////////////////////////

#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

class GSC_Functions{
    public:
    GSC_Functions(){
    };
    std::string currentDateTime() {
        time_t now=time(0);
        struct tm tstruct;
        char buf[80];
        tstruct=*localtime(&now);
        strftime(buf,sizeof(buf),"%Y_%m_%d_%X",&tstruct);
        return ReplaceAll(buf,":","_");
    };
    std::string currentHour() {
        time_t now=time(0);
        struct tm tstruct;
        char buf[80];
        tstruct=*localtime(&now);
        strftime(buf,sizeof(buf),"%X",&tstruct);
        return ReplaceAll(buf,":",":");
    };

    std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    };

    std::string goParentFolder(std::string path){
        std::string res;
        res=path;
        std::cout << "going in parent folder of " << res << std::endl;
        if (res[res.length()-1]=='.')
            res.pop_back();
        if (res[res.length()-1]=='/'||res[res.length()-1]=='\\')
            res.pop_back();
        for (int i = 0;i<512;i++){
            if (res[res.length()-1]!='/'&&res[res.length()-1]!='\\'){
                res.pop_back();
            }else{
                std::cout << "parent folder is " << res << std::endl;
                std::cout << res << std::endl;
                break;
            };
        };
        return res;
    };

    int saveCurrentMap(std::vector<std::vector<std::string>> terrain_tiles, std::string cur_map){
        std::ofstream map_file("/home/rhubarb/.gsc_o/maps/"+cur_map+"_"+currentDateTime()+".lv");
        for (int i = 0; i < terrain_tiles.size() ; i++){
            for (int j = 0; j < terrain_tiles[i].size();j++){
                map_file << terrain_tiles[i][j];
            };
            if (i!=terrain_tiles.size()-1){
            map_file << "\n";
            };
        };
        map_file.close();
        std::cout << "map saved at /home/rhubarb/.gsc_o/maps/"+cur_map+"_"+currentDateTime()+".lv" << std::endl;
        return 0;
    };

    int saveMap(std::vector<std::vector<std::string>> terrain_tiles, std::string map_path){
        std::ofstream map_file(map_path);
        for (int i = 0; i < terrain_tiles.size() ; i++){
            for (int j = 0; j < terrain_tiles[i].size();j++){
                map_file << terrain_tiles[i][j];
            };
            if (i!=terrain_tiles.size()-1){
            map_file << "\n";
            };
        };
        map_file.close();
        return 0;
    };

    int hexToInt(char hexa){

        std::string res;
        res=hexa;

        res=ReplaceAll(res, "a", "10");
        res=ReplaceAll(res, "b", "11");
        res=ReplaceAll(res, "c", "12");
        res=ReplaceAll(res, "d", "13");
        res=ReplaceAll(res, "e", "14");
        res=ReplaceAll(res, "f", "15");

        return std::stoi(res);
    };

    bool compareFunction (std::string a, std::string b) {return a<b;} 

    std::string getUserPath(){
        struct passwd *pw = getpwuid(getuid());
        std::string homedir = pw->pw_dir;
        return homedir;
    }

    int takeScreenshot(sf::RenderWindow& render_window){

        sf::Texture texture;
        texture.create(render_window.getSize().x, render_window.getSize().y);
        texture.update(render_window);
        if (texture.copyToImage().saveToFile(getUserPath()+"/.gsc_o/screenshots/screenshot_"+currentDateTime()+".png"))
        {
            std::cout << "[INFO] "+currentHour()+" // screenshot saved at "+getUserPath()+"/.gsc_o/screenshots/screenshot_"+currentDateTime()+".png" << std::endl;
            return 0;
        };
        std::cout << "[ERROR] "+currentHour()+" // failed to save screenshot" << std::endl;
        return 1;
    };

    
};