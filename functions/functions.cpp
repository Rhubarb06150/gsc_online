#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <fstream>

class GSC_Functions{
    public:
    GSC_Functions(){

    };
    std::string currentDateTime() {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y_%m_%d_%X", &tstruct);
        return ReplaceAll(buf,":","_");
    };

    std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
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

    int takeScreenshot(sf::RenderWindow& render_window,std::string filename){
        sf::Texture texture;
        texture.create(render_window.getSize().x, render_window.getSize().y);
        texture.update(render_window);
        if (texture.copyToImage().saveToFile(filename))
        {
            std::cout << "screenshot saved to " << filename << std::endl;
        };
        return 0;
    };

    
};