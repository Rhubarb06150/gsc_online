#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

class Trainers{
    public:
    std::vector<std::vector<std::string>> trainers_index;
    std::vector<sf::Texture> trainers_textures;
    Trainers(){
        int trainers_nb=0;
        for(int i=0;i<9;i++){
            for(int j=0;j<8;j++){
                trainers_index.push_back({"trainer"+std::to_string(trainers_nb),std::to_string(j)+std::to_string(i)});
                trainers_nb++;
            };
        };
        int x;
        int y;
        for(int i=0;i<trainers_index.size();i++){
            sf::Texture texture;
            std::string fish = trainers_index[i][1];
            std::string xstr;
            xstr=fish[0];
            std::string ystr;
            ystr=fish[1];
            x=std::stoi(xstr);
            y=std::stoi(ystr);
            texture.loadFromFile("assets/player/trainers.png",sf::IntRect({x*56,y*56},{56,56}));
            trainers_textures.push_back(texture);
        };
        /* for(int i=0;i<trainers_index.size();i++){
            std::cout<<trainers_index[i][0]<<"="<<trainers_index[i][1]<<std::endl;
        }; */
    };
    int showTrainer(sf::RenderWindow& window,std::vector<int> pos, int id){
        for(int i=0;i<trainers_index.size();i++){
            if (trainers_index[i][0]=="trainer"+std::to_string(id)){
                sf::Sprite trainer;
                sf::Texture texture;
                texture=trainers_textures[i];
                trainer.setTexture(texture);
                trainer.setPosition(pos[0],pos[1]);
                trainer.setScale(4.f,4.f);
                window.draw(trainer);
                return 0;
            };
        };
        return 0;
    };
};