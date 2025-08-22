#include <cmath>
#include <iostream>
#include "MyAudio.hpp" 
#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>
int main() {

    FunctionToWav writer(44100, 16, 1);
    const std::vector<char> vec1=writer.generate_and_to_memory( [](double t) {
        return std::sin(2 * M_PI * 440 * t);
    }, 2.0);
    
    const std::vector<char> vec2=writer.generate_and_to_memory( [](double t) {
        return (std::sin(2 * M_PI * 440 * t) > 0) ? 1.0 : -1.0;
    }, 2.0);

    const std::vector<char> vec3=writer.generate_and_to_memory( [](double t) {
        double freq = 440;
        return 2.0 * (t * freq - std::floor(0.5 + t * freq));
    }, 2.0);


    const std::vector<char> vec=writer.generate_and_to_memory([](double t) {
        return (std::sin(2 * M_PI * 440 * t) > 0) ? 1.0 : -1.0;
    }, 2.0);

    sf::SoundBuffer buffer1;
    const char* start_of_memory=vec.data();
    buffer1.loadFromMemory(start_of_memory,vec.size());
    if (!buffer1.loadFromMemory(start_of_memory,vec.size())) {//!buffer1.loadFromFile("sine440.wav")
        std::cerr << "Error loading audio file\n";
        return -1;
    }
    sf::SoundBuffer buffer2;
    if (!buffer2.loadFromFile("square440.wav")) {
        std::cerr << "Error loading audio file\n";
        return -1;
    }
        sf::SoundBuffer buffer3;
    if (!buffer3.loadFromFile("sawtooth.wav")) {
        std::cerr << "Error loading audio file\n";
        return -1;
    }
    char ch=' ';
   while(ch!='q'){
    std::cin>>ch;
    if(ch=='a'){
    sf::Sound sound(buffer1);
    sound.play();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    sound.stop();
    }
    if(ch=='b'){
    sf::Sound sound(buffer2);

    sound.play();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    sound.stop();
    }
    if(ch=='c'){
    sf::Sound sound(buffer3);
    sound.play();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    sound.stop();
    }

   }
    return 0;
}
