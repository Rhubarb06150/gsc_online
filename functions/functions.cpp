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
#include <filesystem>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>
#include <pwd.h>

#include "tiles.cpp"

class GSC_Functions{
    public:
    std::vector<double> render_times;
    GSC_Functions(){};
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
        };
        return str;
    };

    std::string goParentFolder(std::string path){
        std::string res;
        res=path;
        if (res[res.length()-1]=='.')
            res.pop_back();
        if (res[res.length()-1]=='/'||res[res.length()-1]=='\\')
            res.pop_back();
        for (int i = 0;i<512;i++){
            if (res[res.length()-1]!='/'&&res[res.length()-1]!='\\'){
                res.pop_back();
            }else{
                std::cout << res << std::endl;
                break;
            };
        };
        return res;
    };

    int log(std::string type, std::string info){
        if (type=="ERROR"){//red type and red text
            std::cout << "[\033[1;31m" << type << "\033[0m] " << currentHour()  << " // \033[31m" << info << "\033[0m" << std::endl;
        }else if (type=="WARN"){//yellow type and yellow text
            std::cout << "[\033[1;33m" << type << "\033[0m] " << currentHour()  << " // \033[33m" << info << "\033[0m" << std::endl;
        }else if (type=="INFO"||type=="CONFIG"){//cyan type
            std::cout << "[\033[1;35m" << type << "\033[0m] " << currentHour()  << " // " << info << std::endl;
        }else if (type=="EDITOR"||type=="MOD"||type=="TERRAIN"||"SUCCESS"){//green type
            std::cout << "[\033[1;32m" << type << "\033[0m] " << currentHour()  << " // " << info << std::endl;
        }else if (type=="ENGINE"||type=="DEBUG"){//yellow type
            std::cout << "[\033[1;33m" << type << "\033[0m] " << currentHour()  << " // " << info << std::endl;
        }else{//basic terminal color for all
            std::cout << "\033[0m][" << type << "] " << currentHour() << " // " << info << std::endl;
        }
        return 0;
    };

    int quitGame(sf::RenderWindow& window){
        #ifdef __linux__
        log("INFO","Clearing /tmp/.gsc_o/ folder");
        system("rm -r /tmp/.gsc_o/* > /dev/null 2>&1");
        log("INFO","Temp folder cleared");
        #endif
        log("INFO","Closing game");
        window.close();
        return 0;
    };

    int createMissingDir(std::string path){
        if (!std::filesystem::is_directory(path)){
            std::string ins;
            ins="mkdir "+path;
            if (system(ins.c_str())==0){
                return 0;
            }else{
                return 2;
            };
        }else{
            return 1;
        };
    };

    int createMissingFile(std::string path,std::string content){
        if (!std::filesystem::is_regular_file(path)){
            std::ofstream ofile(path);
            ofile<<content;
            ofile.close();
            return 0;
        }else{
            return 1;
        };
        return 0;
    };

    std::vector<std::string> saveGifFrame(int index_frame, sf::RenderWindow& window, std::vector<std::string> vector){
        sf::Texture texture;
        int winx=window.getSize().x;
        int winy=window.getSize().y;
        texture.create(winx, winy);
        texture.update(window);
        if (texture.copyToImage().saveToFile("/tmp/gscogiff_"+std::to_string(index_frame)+".png"))
        {
            vector.push_back("/tmp/gscogiff_"+std::to_string(index_frame)+".png");
            return vector;
        };
        return vector;
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
        std::cout << "map saved at "+getUserPath()+"/.gsc_o/maps/"+cur_map+"_"+currentDateTime()+".lv" << std::endl;
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
    };
    int renderMap(std::vector<std::vector<std::string>> terrain_vector,TilesIndex Tiles){
        //16384x16384
        bool nope=false;
        if (terrain_vector[0].size()*16>16384){
            log("ERROR","Cant create render, width is too big!");
            nope=true;
        };
        if (terrain_vector.size()*16>16384){
            log("ERROR","Cant create render, height is too high!");
            nope=true;
        };
        if (nope){
            return 1;
        };
        log("RENDER","Starting render...");
        sf::Image render_image;
        render_image.create(terrain_vector[0].size()*16,terrain_vector.size()*16,sf::Color::Transparent);
        //sf::RenderWindow window(sf::VideoMode(terrain_vector[0].size()*16,terrain_vector.size()*16),"Render window");
        //sf::FloatRect visibleArea(0,0,terrain_vector[0].size()*16,terrain_vector.size()*16);
        //window.setView(sf::View(visibleArea));
        //window.setSize(sf::Vector2u(terrain_vector[0].size()*16,terrain_vector.size()*16));
        //window.clear();
        clock_t start, end;
        start=clock();
        sf::Color cur_pixel;
        for (int i=0;i<terrain_vector.size();i++){
            for (int j=0;j<terrain_vector[0].size();j++){
                if(Tiles.getIntIndex(terrain_vector[i][j])!=Tiles.tiles.size()-2){
                sf::Texture cur_texture;
                sf::Image image;
                    cur_texture=Tiles.day_textures[Tiles.getIntIndex(terrain_vector[i][j])];
                    image=cur_texture.copyToImage();
                    for(int ytex=0;ytex<cur_texture.getSize().y;ytex++){
                        for(int xtex=0;xtex<cur_texture.getSize().x;xtex++){
                            cur_pixel=image.getPixel(ytex,xtex);
                            //if(cur_pixel!=sf::Color::Transparent){
                            render_image.setPixel((i*16)+ytex,(j*16)+xtex,cur_pixel);
                        };
                    };
                };
            };
        };
        render_image.saveToFile(getUserPath()+"/.gsc_o/renders/render_"+currentDateTime()+".png");
        end=clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        render_times.push_back(time_taken);
        log("RENDER","Render saved in "+std::to_string(time_taken));
        //sf::Texture render;
        //render.create(window.getSize().x, window.getSize().y);
        //render.update(window);
        //if (render.copyToImage().saveToFile(getUserPath()+"/.gsc_o/renders/render_"+currentDateTime()+"_day.png")){
        //    log("INFO","Day render saved at "+getUserPath()+"/.gsc_o/renders/render_"+currentDateTime()+"_day.png");
        //}else{
        //    log("ERROR","Failed to save render");
        //};
        //window.clear();
        //for (int i=0;i<terrain_vector.size();i++){
        //    for (int j=0;j<terrain_vector[0].size();j++){
        //        sf::Sprite sprite;
        //        sprite.setPosition(j*16,i*16);
        //        sprite.setTexture(Tiles.morning_textures[Tiles.getIntIndex(terrain_vector[i][j])]);
        //        window.draw(sprite);
        //    };
        //};
        //render.create(window.getSize().x, window.getSize().y);
        //render.update(window);
        //if (render.copyToImage().saveToFile(getUserPath()+"/.gsc_o/renders/render_"+currentDateTime()+"_morning.png")){
        //    log("INFO","Morning render saved at "+getUserPath()+"/.gsc_o/renders/render_"+currentDateTime()+"_morning.png");
        //}else{
        //    log("ERROR","Failed to save render");
        //};
        //
        //window.clear();
        //for (int i=0;i<terrain_vector.size();i++){
        //    for (int j=0;j<terrain_vector[0].size();j++){
        //        sf::Sprite sprite;
        //        sprite.setPosition(j*16,i*16);
        //        sprite.setTexture(Tiles.night_textures[Tiles.getIntIndex(terrain_vector[i][j])]);
        //        window.draw(sprite);
        //    };
        //};
        //render.create(window.getSize().x, window.getSize().y);
        //render.update(window);
        //if (render.copyToImage().saveToFile(getUserPath()+"/.gsc_o/renders/render_"+currentDateTime()+"_night.png")){
        //    log("INFO","Night render saved at "+getUserPath()+"/.gsc_o/renders/render_"+currentDateTime()+"_night.png");
        //}else{
        //    log("ERROR","Failed to save render");
        //};
        return 0;
    };

    int takeScreenshot(sf::RenderWindow& render_window,int index_frame){
        sf::Texture texture;
        texture.create(render_window.getSize().x, render_window.getSize().y);
        texture.update(render_window);
        if (texture.copyToImage().saveToFile(getUserPath()+"/.gsc_o/screenshots/screenshot_"+currentDateTime()+std::to_string(index_frame)+".png"))
        {
            log("INFO","screenshot saved at "+getUserPath()+"/.gsc_o/screenshots/screenshot_"+currentDateTime()+std::to_string(index_frame)+".png");
            return 0;
        };
        log("ERROR","Failed to save screenshot");
        return 1;
    };
};