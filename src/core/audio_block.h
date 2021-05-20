#pragma once
#include <array>

namespace algae::dsp::core{
    template<typename sample_t, size_t BLOCK_SIZE>
    class alignas(16) AudioBlock{
        private:
            std::array<sample_t,BLOCK_SIZE> samples;

        public:
            static inline const AudioBlock<sample_t,BLOCK_SIZE> empty() {
                AudioBlock<sample_t,BLOCK_SIZE> block;
                block*=0;
                return block;
            }

            inline sample_t& operator[](const size_t& index){
                return samples[index];
            }
            inline const sample_t& operator[](const size_t& index) const {
                return samples[index];
            }
            inline const AudioBlock operator*(const AudioBlock& rhs) const {
                AudioBlock output;
                for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                    output[idx] = samples[idx] * rhs[idx];
                }
                return output;
            }
            inline const AudioBlock& operator*=(const AudioBlock& rhs){
                AudioBlock output;
                for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                    output[idx] = samples[idx] * rhs[idx];
                }
                this->samples = output.samples;
                return *(this);
            }
            inline const AudioBlock operator*(const sample_t& rhs) const {
                AudioBlock output;
                for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                    output[idx] = samples[idx] * rhs;
                }
                return output;
            }
            inline const AudioBlock& operator*=(const sample_t& rhs){
                AudioBlock output;
                for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                    output[idx] = samples[idx] * rhs;
                }
                this->samples = output.samples;
                return *(this);
            }
            inline const AudioBlock operator/(const AudioBlock& rhs) const {
                AudioBlock output;
                for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                    output[idx] = samples[idx] / rhs[idx];
                }
                return output;
            }
            inline const AudioBlock& operator/=(const AudioBlock& rhs){
                AudioBlock output;
                for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                    output[idx] = samples[idx] / rhs[idx];
                }
                this->samples = output.samples;
                return *(this);
            }
            inline const AudioBlock operator/(const sample_t& rhs) const {
                AudioBlock output;
                for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                    output[idx] = samples[idx] / rhs;
                }
                return output;
            }
            inline const AudioBlock& operator/=(const sample_t& rhs){
                AudioBlock output;
                for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                    output[idx] = samples[idx] / rhs;
                }
                this->samples = output.samples;
                return *(this);
            }
            inline const AudioBlock operator+(const AudioBlock& rhs) const {
                AudioBlock output;
                for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                    output[idx] = samples[idx] + rhs[idx];
                }
                return output;
            }
            inline const AudioBlock& operator+=(const AudioBlock& rhs){
                AudioBlock output;
                for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                    output[idx] = samples[idx] + rhs[idx];
                }
                this->samples = output.samples;
                return *(this);
            }
            inline const AudioBlock operator+(const sample_t& rhs) const {
                AudioBlock output;
                for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                    output[idx] = samples[idx] + rhs;
                }
                return output;
            }
            inline const AudioBlock& operator+=(const sample_t& rhs){
                AudioBlock output;
                for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                    output[idx] = samples[idx] + rhs;
                }
                this->samples = output.samples;
                return *(this);
            }
            inline const AudioBlock operator-(const AudioBlock& rhs) const {
                AudioBlock output;
                for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                    output[idx] = samples[idx] - rhs[idx];
                }
                return output;
            }
            inline const AudioBlock& operator-=(const AudioBlock& rhs){
                AudioBlock output;
                for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                    output[idx] = samples[idx] - rhs[idx];
                }
                this->samples = output.samples;
                return *(this);
            }
            inline const AudioBlock operator-(const sample_t& rhs) const {
                AudioBlock output;
                for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                    output[idx] = samples[idx] - rhs;
                }
                return output;
            }
            inline const AudioBlock& operator-=(const sample_t& rhs){
                AudioBlock output;
                for(size_t idx=0; idx<BLOCK_SIZE; idx++){
                    output[idx] = samples[idx] - rhs;
                }
                this->samples = output.samples;
                return *(this);
            }
    };

    template<typename sample_t>
    class AudioBlock<sample_t,1>{
        private:
            sample_t sample;

        public:

            inline sample_t& operator[](const size_t& index){
                return sample;
            }

            inline const sample_t& operator[](const size_t& index) const{
                return sample;
            }
    };

    template<typename sample_t, size_t BLOCK_SIZE>
    using StereoBlock = std::array<AudioBlock<sample_t,BLOCK_SIZE>,2>;
}

