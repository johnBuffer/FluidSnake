#include "GameSound.h"
#include <fstream>
#include <iostream>

GameSound::GameSound(const std::string filename)
{
    std::ifstream file(filename);
    if (file)
    {
        int infos_count;
        file >> infos_count;
        _soundBuffers.resize(infos_count);
        for (int i(0); i<infos_count; ++i)
        {
            std::string type, soundName, soundFile;
            float volume;
            file >> type >> soundName >> soundFile >> volume;

            sf::SoundBuffer buffer;
            if (buffer.loadFromFile(soundFile))
            {
                _soundBuffers[i] = buffer;
                _sounds[soundName] = sf::Sound();
                _sounds[soundName].setBuffer(_soundBuffers[i]);
                _sounds[soundName].setVolume(volume);
            }
            else
                std::cout << "Error : cannot load '" << soundFile <<"'." << std::endl;
        }
        std::cout << "Sounds loaded." << std::endl;
    }
    else
    {
        std::cout << "Error : cannot load configuration file." << std::endl;
    }
    /*soundBonusBuffer.loadFromFile("bonus.flac");
    soundLoseBuffer.loadFromFile("lose.flac");
    soundBonusplusBuffer.loadFromFile("bonusplus.flac");
    soundBipBuffer.loadFromFile("bip.flac");
    soundBoomBuffer.loadFromFile("boom.ogg");

    sf::Sound soundBonus, soundBonusplus, soundLose, soundBip, soundBoom;
    soundBonus.setBuffer(soundBonusBuffer);
    soundBoom.setBuffer(soundBoomBuffer);
    soundBonusplus.setBuffer(soundBonusplusBuffer);
    soundBonusplus.setVolume(50);
    soundLose.setBuffer(soundLoseBuffer);
    soundBip.setBuffer(soundBipBuffer);
    soundBip.setVolume(1);

    sf::Music ambience;
    ambience.openFromFile("ambience.ogg");
    ambience.setVolume(25);
    ambience.setLoop(true);
    ambience.play();*/

}

void GameSound::play(const std::string soundName)
{
    if (_sounds.find(soundName) != _sounds.end())
        _sounds.at(soundName).play();
    else
        std::cout << "Sound not found." << std::endl;
}
