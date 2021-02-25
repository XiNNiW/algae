#pragma once
#include "../core/filter.h"
#include "dsp_graph.h"

namespace algae::dsp::shell::filter{
    using algae::dsp::core::filter::biquad_t;
    using algae::dsp::core::filter::update_biquad;
    using algae::dsp::core::filter::highpass;
    using algae::dsp::core::filter::lowpass;
    using algae::dsp::core::filter::bandpass;

    template<typename sample_t, typename frequency_t>
    struct lpf:dsp_node<sample_t, frequency_t>{
        biquad_t<sample_t,frequency_t> state = biquad_t<sample_t,frequency_t>();

        lpf()
        :dsp_node<sample_t,frequency_t>(
            make_inlets<sample_t,frequency_t>(3),
            make_outlets<sample_t,frequency_t>(1)
        )
        {}

        virtual void process(const frequency_t& sampleRate=44100.0) override {
            sample_t input = this->inlets[0].getValue(), cutoff = this->inlets[1].getValue(), q = this->inlets[2].getValue();
            state = lowpass<sample_t,frequency_t>(state, cutoff, q, sampleRate);
            state = update_biquad<sample_t,frequency_t>(state,input);
            this->outlets[0] = state.y1;
            // std::cout<<"maybe here??\n";
        }

        virtual std::shared_ptr<dsp_node<sample_t,frequency_t>> getSharedPtr() override {
            return std::make_shared<lpf<sample_t,frequency_t>>(*this);
        }

        virtual std::unique_ptr<dsp_node<sample_t,frequency_t>> getUniquePtr() override {
            return std::make_unique<lpf<sample_t,frequency_t>>(*this);
        }
    };

    template<typename sample_t, typename frequency_t>
    struct hpf:dsp_node<sample_t, frequency_t>{
        biquad_t<sample_t,frequency_t> state=biquad_t<sample_t,frequency_t>();

        hpf()
        :dsp_node<sample_t,frequency_t>(
            make_inlets<sample_t,frequency_t>(3),
            make_outlets<sample_t,frequency_t>(1)
        )
        {}

        virtual void process(const frequency_t& sampleRate=44100.0) override {
            sample_t input = this->inlets[0].getValue(), cutoff = this->inlets[1].getValue(), q = this->inlets[2].getValue();
            state = highpass<sample_t,frequency_t>(state, cutoff, q, sampleRate);
            state = update_biquad<sample_t,frequency_t>(state,input);
            this->outlets[0] = state.y1;
        }

        virtual std::shared_ptr<dsp_node<sample_t,frequency_t>> getSharedPtr() override {
            return std::make_shared<hpf<sample_t,frequency_t>>(*this);
        }

        virtual std::unique_ptr<dsp_node<sample_t,frequency_t>> getUniquePtr() override {
            return std::make_unique<hpf<sample_t,frequency_t>>(*this);
        }
    };

    template<typename sample_t, typename frequency_t>
    struct bpf:dsp_node<sample_t, frequency_t>{
        biquad_t<sample_t,frequency_t> state=biquad_t<sample_t,frequency_t>();

        bpf()
        :dsp_node<sample_t,frequency_t>(
            make_inlets<sample_t,frequency_t>(3),
            make_outlets<sample_t,frequency_t>(1)
        )
        {}

        virtual void process(const frequency_t& sampleRate=44100.0) override {
            sample_t input = this->inlets[0].getValue(), cutoff = this->inlets[1].getValue(), q = this->inlets[2].getValue();
            state = bandpass<sample_t,frequency_t>(state, cutoff, q, sampleRate);
            state = update_biquad<sample_t,frequency_t>(state,input);
            this->outlets[0] = state.y1;
        }

        virtual std::shared_ptr<dsp_node<sample_t,frequency_t>> getSharedPtr() override {
            return std::make_shared<bpf<sample_t,frequency_t>>(*this);
        }

        virtual std::unique_ptr<dsp_node<sample_t,frequency_t>> getUniquePtr() override {
            return std::make_unique<bpf<sample_t,frequency_t>>(*this);
        }
    };
}
