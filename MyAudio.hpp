#pragma once
#include <fstream>
#include <cstdint>
#include <string>
#include <functional>
#include <vector>
#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>
namespace pitch{
#define A3 220.0
#define A4 440.0
#define C5 523.25 
#define A5 880.0
}

class FunctionToWav {
public:
    FunctionToWav(int sampleRate = 44100, int bitsPerSample = 16, int channels = 1)
        : sampleRate_{sampleRate}, bitsPerSample_{bitsPerSample}, channels_{channels} {}
    void setSampleRate(int SampleRate){
        this->sampleRate_=SampleRate;
    }
    void setbitsPerSample(int bitsPerSample){
        this->bitsPerSample_=bitsPerSample;
    }
    void setChannels(int Channels){
        this->channels_=Channels;
    }

    void generate(const std::string& filename, std::function<double(double)> func, double duration, int amplitude = 30000) {
        int totalSamples = static_cast<int>(duration * sampleRate_);

        // Prepare WAV header
        WAVHeader header;
        header.sampleRate_ = sampleRate_;
        header.numChannels_ = channels_;
        header.bitsPerSample_ = bitsPerSample_;
        header.byteRate_ = sampleRate_ * channels_ * bitsPerSample_ / 8;
        header.blockAlign_ = channels_ * bitsPerSample_ / 8;
        header.dataSize_ = totalSamples * header.blockAlign_;
        header.chunkSize_ = 36 + header.dataSize_;

        // Write header
        std::ofstream outFile(filename, std::ios::binary);
        if(!outFile.is_open())
        {
            throw "can not create/open file";
        }
        outFile.write(reinterpret_cast<const char*>(&header), sizeof(header));

        // Generate_and_to_file samples from user function
        for (int i = 0; i < totalSamples; ++i) {
            double t = static_cast<double>(i) / sampleRate_;
            double value = amplitude * func(t);
            int16_t sample = static_cast<int16_t>(value);
            outFile.write(reinterpret_cast<const char*>(&sample), sizeof(sample));
        }
    }
       const std::vector<char> generate_and_to_memory(std::function<double(double)> func, double duration, int amplitude = 30000) {
        int totalSamples = static_cast<int>(duration * sampleRate_);

        // Prepare WAV header
        WAVHeader header;
        header.sampleRate_ = sampleRate_;
        header.numChannels_ = channels_;
        header.bitsPerSample_ = bitsPerSample_;
        header.byteRate_ = sampleRate_ * channels_ * bitsPerSample_ / 8;
        header.blockAlign_ = channels_ * bitsPerSample_ / 8;
        header.dataSize_ = totalSamples * header.blockAlign_;
        header.chunkSize_ = 36 + header.dataSize_;

            std::vector<char> memory;
        char* header_ptr= reinterpret_cast<char*>(&header);
        for(int i=0;i<sizeof(header);i++,header_ptr++)
        {
            memory.push_back(*header_ptr);
        }

    
        for (int i = 0; i < totalSamples; ++i) {
            double t = static_cast<double>(i) / sampleRate_;
            double value = amplitude * func(t);
            int16_t sample = static_cast<int16_t>(value);
            memory.push_back(static_cast<char>(sample & 0xFF));
            memory.push_back(static_cast<char>(sample >> 8));//convert int16_t to 2 int8_t
        }
        return memory;
    }

private:
    struct WAVHeader {
        char riff_[4] = {'R', 'I', 'F', 'F'};
        uint32_t chunkSize_;
        char wave_[4] = {'W', 'A', 'V', 'E'};
        char fmt_[4] = {'f', 'm', 't', ' '};
        uint32_t subchunk1Size_ = 16;
        uint16_t audioFormat_ = 1;
        uint16_t numChannels_;
        uint32_t sampleRate_;
        uint32_t byteRate_;
        uint16_t blockAlign_;
        uint16_t bitsPerSample_;
        char data_[4] = {'d', 'a', 't', 'a'};
        uint32_t dataSize_;
    };

    int sampleRate_;
    int bitsPerSample_;
    int channels_;
};

class noises:public sf::Sound,public FunctionToWav{
    public:
            noises(std::vector<std::function<double(double)>> funclist):funclist_(funclist){
                for(auto &i:funclist)
                {
                    sf::SoundBuffer buff;
                    std::vector<char> tempMemory=generate_and_to_memory(i,2);
                     const char* start_of_memory=tempMemory.data();
                     buff.loadFromMemory(start_of_memory,tempMemory.size());
                     buffers_.push_back(buff);

                }
            }    
            void play(int a){
                 if(a<0 || a>=buffers_.size()) {throw "invalid key"; return;}
                 setBuffer(buffers_[a]);
                 sf::Sound::play();
                 std::this_thread::sleep_for(std::chrono::milliseconds(50));
                 sf::Sound::stop();
            }
    private:
       
        std::vector<std::function<double(double)>> funclist_;
        std::vector<sf::SoundBuffer> buffers_; 
};