#ifndef GAMESOUND_H_INCLUDED
#define GAMESOUND_H_INCLUDED
#include <map>
#include <SFML/Audio.hpp>

class GameSound
{
public:
    GameSound(const std::string filename);
    void play(const std::string soundName);

private:
    //sf::SoundBuffer _soundBonusBuffer, _soundBonusplusBuffer, _soundLoseBuffer, _soundBipBuffer, _soundBoomBuffer;
    std::vector<sf::SoundBuffer> _soundBuffers;
    std::map<const std::string, sf::Sound> _sounds;
};

#endif // GAMESOUND_H_INCLUDED
