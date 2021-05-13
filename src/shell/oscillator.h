#pragma once
#include "../core/oscillator.h"
#include "dsp_graph.h"

namespace algae::dsp::shell::oscillator{

   

    template<typename sample_t, typename frequency_t>
    struct phasor:dsp_node<sample_t,frequency_t>{
        // phasor_t<frequency_t> state = phasor_t<frequency_t>();

        // static dsp_node<sample_t,frequency_t> make(){
        //     return dsp_node<sample_t,frequency_t>(
        //         make_inlets<sample_t,frequency_t>(1),
        //         make_outlets<sample_t,frequency_t>(1),
        //         phasor<sample_t,frequency_t>()
        //     );
        // }

        phasor()
        :dsp_node<sample_t,frequency_t>(
            make_inlets<sample_t,frequency_t>(1),
            make_outlets<sample_t,frequency_t>(1)
        )
        {}
        
        virtual void process( const frequency_t& sampleRate=44100.0) override{
            // std::cout<<"phasor is updating...\n";
            auto freq = this->inlets[0].getValue();
            // std::cout<<"phasor freq is... "<<freq<<" ...\n";

            // state = update_phasor<sample_t,frequency_t>(state, freq, sampleRate);
            // this->outlets[0] = state.phase;
            // std::cout<<"phasor phase is now... "<<state.phase<<" ...\n";

            // std::cout<<"phasor is updated...\n";

        }
        virtual std::shared_ptr<dsp_node<sample_t,frequency_t>> getSharedPtr() override { 
            return std::make_shared<phasor<sample_t,frequency_t>>(*this);
        }
        virtual std::unique_ptr<dsp_node<sample_t,frequency_t>> getUniquePtr() override {
            return std::make_unique<phasor<sample_t,frequency_t>>(*this);
        }
        // virtual std::unique_ptr<AbstractDSPNode<sample_t,frequency_t>> clone() const{
        //     return std::make_unique<phasor<sample_t,frequency_t>>(*this);
        // }

    };

    template<typename sample_t, typename frequency_t>
    struct osc:dsp_node<sample_t,frequency_t>{
        // phasor_t<frequency_t> state = phasor_t<frequency_t>();

        osc()
        :dsp_node<sample_t,frequency_t>(
            make_inlets<sample_t,frequency_t>(1),
            make_outlets<sample_t,frequency_t>(1)
        )
        {}
        
        virtual void process(const frequency_t& sampleRate=44100.0) override {
            // auto freq = this->inlets[0].getValue();
            // state = update_phasor<sample_t,frequency_t>(state, freq, sampleRate);
            // this->outlets[0] = sinOsc<sample_t>(state.phase);
        }
        virtual std::shared_ptr<dsp_node<sample_t,frequency_t>> getSharedPtr() override {
            return std::make_shared<osc<sample_t,frequency_t>>(*this);
        }
        virtual std::unique_ptr<dsp_node<sample_t,frequency_t>> getUniquePtr() override {
            return std::make_unique<osc<sample_t,frequency_t>>(*this);
        }


    };

    template<typename sample_t, typename frequency_t>
    struct noise:dsp_node<sample_t,frequency_t>{
        noise()
        :dsp_node<sample_t,frequency_t>(
            make_inlets<sample_t,frequency_t>(0),
            make_outlets<sample_t,frequency_t>(1)
        )
        {}
        
        virtual void process(const frequency_t& sampleRate=44100.0) override {
            this->outlets[0] = algae::dsp::core::oscillator::noise<sample_t>();
        }
        virtual std::shared_ptr<dsp_node<sample_t,frequency_t>> getSharedPtr() override {
            return std::make_shared<noise<sample_t,frequency_t>>(*this);
        }
        virtual std::unique_ptr<dsp_node<sample_t,frequency_t>> getUniquePtr() override {
            return std::make_unique<noise<sample_t,frequency_t>>(*this);
        }

    };

    


}
