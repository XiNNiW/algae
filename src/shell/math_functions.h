#pragma once
#include "../core/math_functions.h"
#include "../core/units.h"
#include "dsp_graph.h"
#include <iostream>

namespace algae::dsp::shell::math {

    using algae::dsp::shell::make_inlets;
    using algae::dsp::shell::make_outlets;

    template<typename sample_t, typename frequency_t>
    struct constant_node:dsp_node<sample_t,frequency_t>{//AbstractDSPNode<sample_t, frequency_t>{
        sample_t value;

        constant_node(sample_t value)
        :value(value)
        ,dsp_node<sample_t, frequency_t>(
            make_inlets<sample_t,frequency_t>(0),
            std::vector<sample_t>({value})
        )
        {}
        virtual void process(const frequency_t& sampleRate=44100.0) override {}
        virtual std::shared_ptr<dsp_node<sample_t,frequency_t>> getSharedPtr() override {
            return std::make_shared<constant_node<sample_t,frequency_t>>(*this);
        }
        virtual std::unique_ptr<dsp_node<sample_t,frequency_t>> getUniquePtr() override {
            return std::make_unique<constant_node<sample_t,frequency_t>>(*this);
        }

    };

    // template<typename sample_t, typename frequency_t, const sample_t (* Fx)(sample_t &) >
    // struct unary_op:dsp_node<sample_t, frequency_t>{
    //     unary_op()
    //     :dsp_node<sample_t,frequency_t>(
    //         make_inlets<sample_t, frequency_t>(1),
    //         make_outlets<sample_t, frequency_t>(1)
    //     )
    //     {}
    //     void update(const frequency_t& sampleRate=44100.0){
    //         this->outlets[0] = Fx(this->inlets[0].getValue());
    //     }
    // };

    template<typename sample_t, typename frequency_t, const sample_t (* Fxy)(const sample_t &, const sample_t &)>
    struct binary_op:dsp_node<sample_t, frequency_t>{
        binary_op()
        :dsp_node<sample_t, frequency_t>(
            make_inlets<sample_t, frequency_t>(2),
            make_outlets<sample_t, frequency_t>(1)
        )
        {}
        virtual void process(const frequency_t& sampleRate=44100.0) override {
            this->outlets[0] = Fxy(this->inlets[0].getValue(),this->inlets[1].getValue());
        }
        virtual std::shared_ptr<dsp_node<sample_t,frequency_t>> getSharedPtr() override {
            return std::make_shared<binary_op<sample_t,frequency_t,Fxy>>(*this);
        }
        virtual std::unique_ptr<dsp_node<sample_t,frequency_t>> getUniquePtr() override {
            return std::make_unique<binary_op<sample_t,frequency_t,Fxy>>(*this);
        }
    };

    // using algae::dsp::core::math::sin;
    // using algae::dsp::core::math::cos;
    // using algae::dsp::core::math::tan;
    // using algae::dsp::core::math::tanh;
    // using algae::dsp::core::math::sqrt;

    // using algae::dsp::core::units::mtof;

    // template<typename sample_t, typename frequency_t>
    // using sin_op = unary_op<sample_t, frequency_t,sin<sample_t>>;

    // template<typename sample_t, typename frequency_t>
    // using cos_op = unary_op<sample_t, frequency_t,cos<sample_t>>;

    // template<typename sample_t, typename frequency_t>
    // using tan_op = unary_op<sample_t, frequency_t,tan<sample_t>>;

    // template<typename sample_t, typename frequency_t>
    // using tanh_op = unary_op<sample_t, frequency_t,tanh<sample_t>>;

    // template<typename sample_t, typename frequency_t>
    // using mtof_op = unary_op<sample_t, frequency_t,mtof<sample_t>>;

    template<typename sample_t, typename frequency_t>
    using multiplication_op = binary_op<sample_t, frequency_t,algae::dsp::core::math::multiply<sample_t>>;

    template<typename sample_t, typename frequency_t>
    using division_op = binary_op<sample_t, frequency_t,algae::dsp::core::math::divide<sample_t>>;

    template<typename sample_t, typename frequency_t>
    using addition_op = binary_op<sample_t, frequency_t,algae::dsp::core::math::add<sample_t>>;

    template<typename sample_t, typename frequency_t>
    using subtraction_op = binary_op<sample_t,frequency_t,algae::dsp::core::math::subtract<sample_t>>;

    template<typename sample_t, typename frequency_t>
    using power_op = binary_op<sample_t, frequency_t,algae::dsp::core::math::power<sample_t>>;


}
