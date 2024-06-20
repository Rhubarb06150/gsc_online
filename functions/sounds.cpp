#include <SFML/Audio.hpp>
#include <iostream>



class Sounds{
    public:
    sf::SoundBuffer buffer;
    sf::Sound sound;

    Sounds(){
        sf::SoundBuffer buffer;
        sf::Sound sound;
    };
    void soundEffect(std::string sound_effect){
        if (!buffer.loadFromFile("assets/sounds/"+sound_effect+".ogg")){
            std::cout << "Sound " << sound_effect << " cannot be played!" << std::endl;
            return;
        }else{
            buffer.loadFromFile("assets/sounds/"+sound_effect+".ogg");
            sound.setBuffer(buffer);
            sound.setVolume(100);
            sound.play();
        };
    };
};