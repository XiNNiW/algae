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
                for(size_t samp_idx=0;samp_idx<BLOCK_SIZE;samp_idx++){
                    block[samp_idx]=0;
                }
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
            static inline const AudioBlock<sample_t,1> empty() {
                AudioBlock<sample_t,1> block;
                block*=0;
                return block;
            }

            
            inline const AudioBlock operator*(const AudioBlock& rhs) const {
                AudioBlock output;
                output.sample = sample * rhs.sample;
                return output;
            }
            inline const AudioBlock& operator*=(const AudioBlock& rhs){
        
                this->sample = sample * rhs.sample;
                return *(this);
            }
            inline const AudioBlock operator*(const sample_t& rhs) const {
                AudioBlock output;
                output.sample = sample * rhs;
                return output;
            }
            inline const AudioBlock& operator*=(const sample_t& rhs){

                this->sample = sample * rhs;
                return *(this);
            }
            inline const AudioBlock operator/(const AudioBlock& rhs) const {
                AudioBlock output;
                output.sample = sample / rhs.sample;
                return output;
            }
            inline const AudioBlock& operator/=(const AudioBlock& rhs){
                this->sample = sample / rhs.sample;
                return *(this);
            }
            inline const AudioBlock operator/(const sample_t& rhs) const {
                AudioBlock output;
                output.sample = sample / rhs;
                return output;
            }
            inline const AudioBlock& operator/=(const sample_t& rhs){
                this->sample = sample / rhs;
                return *(this);
            }
            inline const AudioBlock operator+(const AudioBlock& rhs) const {
                AudioBlock output;
                output.sample = sample + rhs.sample;
                return output;
            }
            inline const AudioBlock& operator+=(const AudioBlock& rhs){
                this->sample = sample + rhs.sample;
                return *(this);
            }
            inline const AudioBlock operator+(const sample_t& rhs) const {
                AudioBlock output;
                output.sample = sample + rhs;
                return output;
            }
            inline const AudioBlock& operator+=(const sample_t& rhs){
                this->sample = sample + rhs;
                return *(this);
            }
            inline const AudioBlock operator-(const AudioBlock& rhs) const {
                AudioBlock output;
                output.sample = sample - rhs.sample;
                return output;
            }
            inline const AudioBlock& operator-=(const AudioBlock& rhs){
                this->sample = sample - rhs.sample;
                return *(this);
            }
            inline const AudioBlock operator-(const sample_t& rhs) const {
                AudioBlock output;
                output.sample = sample - rhs;
                return output;
            }
            inline const AudioBlock& operator-=(const sample_t& rhs){
                this->sample = sample - rhs;
                return *(this);
            }
    };

    template<typename sample_t, size_t BLOCKSIZE>
    const inline AudioBlock<sample_t, BLOCKSIZE> makeBlock(const std::array<sample_t,BLOCKSIZE>& samples){
        AudioBlock<sample_t, BLOCKSIZE> block;
        for(size_t idx=0; idx<BLOCKSIZE;idx++){
            block[idx] = samples[idx];
        }
        return block;
    }
    template<typename sample_t, size_t BLOCK_SIZE>
    using StereoBlock = std::array<AudioBlock<sample_t,BLOCK_SIZE>,2>;
}

