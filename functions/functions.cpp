#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sys/stat.h>


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