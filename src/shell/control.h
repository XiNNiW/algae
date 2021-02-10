#pragma once
#include "../core/control.h"
#include "dsp_graph.h"

namespace algae::dsp::shell::control{
    using algae::dsp::shell::dsp_node;
    using algae::dsp::shell::make_inlets;
    using algae::dsp::shell::make_outlets;
    using algae::dsp::core::control::ramp_t;
    using algae::dsp::core::control::update_ramp;
    using algae::dsp::core::control::update_adsr;

    template<typename sample_t, typename frequency_t>
    struct ramp:dsp_node<sample_t, frequency_t>{
        ramp_t<sample_t> state = ramp_t<sample_t>{0,0};

        ramp()
        :dsp_node<sample_t,frequency_t>(
            make_inlets<sample_t,frequency_t>(4),
            make_outlets<sample_t,frequency_t>(1)
        )
        {}

        void process(const frequency_t& sampleRate=44100.0){

            sample_t trig = this->inlets[0].getValue(), from = this->inlets[1].getValue(), to = this->inlets[2].getValue(), time = this->inlets[3].getValue();
            if(trig&&!state.trig){state = ramp_t<sample_t>{0,0};}
            state = update_ramp<sample_t,frequency_t>(state,from,to,time);
            this->outlets[0] = state.value;
        }


        virtual std::shared_ptr<dsp_node<sample_t,frequency_t>> getSharedPtr() override {
            return std::make_shared<ramp<sample_t,frequency_t>>(*this);
        }
        virtual std::unique_ptr<dsp_node<sample_t,frequency_t>> getUniquePtr() override {
            return std::make_unique<ramp<sample_t,frequency_t>>(*this);
        }
    };

    template<typename sample_t, typename frequency_t>
    struct adsr:dsp_node<sample_t, frequency_t>{
        ramp_t<sample_t> state = ramp_t<sample_t>{0,0};

        adsr()
        :dsp_node<sample_t,frequency_t>(
            make_inlets<sample_t,frequency_t>(5),
            make_outlets<sample_t,frequency_t>(1)
        )
        {}

        virtual void process(const frequency_t& sampleRate=44100.0) override {
            dsp_node<sample_t,frequency_t>::update(sampleRate);

            sample_t trig = this->inlets[0].getValue(), a = this->inlets[1].getValue(), d = this->inlets[2].getValue(), s = this->inlets[3].getValue(), r = this->inlets[4].getValue();
            if(trig){state = ramp_t<sample_t>{0,0};}
            state = update_adsr<sample_t,frequency_t>(state,trig,a,d,s,r);
            this->outlets[0] = state.value;
        }

        virtual std::shared_ptr<dsp_node<sample_t,frequency_t>> getSharedPtr() override {
            return std::make_shared<adsr<sample_t,frequency_t>>(*this);
        }
        virtual std::unique_ptr<dsp_node<sample_t,frequency_t>> getUniquePtr() override {
            return std::make_unique<adsr<sample_t,frequency_t>>(*this);
        }
    };


    template<typename sample_t, typename frequency_t>
    struct seq:dsp_node<sample_t,frequency_t>{
        std::vector<sample_t> value;
        std::vector<long> durations;
        size_t index;
        seq()
        :dsp_node<sample_t,frequency_t>(
            make_inlets<sample_t,frequency_t>(0),
            make_outlets<sample_t,frequency_t>(1)
        )
        {}

        virtual void process(frequency_t sampleRate=44100.0) override {
            
            this->outlets[0] = value.get();
            index++;
        }
    };
}
