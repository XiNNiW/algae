#pragma once 
#include "../control.h"
#include "../oscillator.h"
namespace algae::dsp::core::drum {
    using algae::dsp::core::control::perc_t;
    using algae::dsp::core::control::process;
    using algae::dsp::core::oscillator::fbfm_t;
    using algae::dsp::core::oscillator::click_t;
    using algae::dsp::core::oscillator::process;
    using algae::dsp::core::oscillator::compute_phase_increment_;
    using algae::dsp::core::oscillator::update_phase_;

    template <typename sample_t>
    struct efm_bd_t {
        perc_t<sample_t> ampEnv;
        perc_t<sample_t> pitchEnv;
        perc_t<sample_t> indexEnv;
        fbfm_t<sample_t> osc1;
        click_t<sample_t> click;
        sample_t carrierPhase=0;
        sample_t moddulatorPhase=0;
    };

    template <typename sample_t>
    const inline std::pair<efm_bd_t<sample_t>, sample_t> process(
        efm_bd_t<sample_t> drum,
        const sample_t& trig,
        const sample_t& carrierFreq,
        const sample_t& modRatio,
        const sample_t& fmIndex,
        const sample_t& feedback,
        const sample_t& amp_decay_in_samps,
        const sample_t& mod_decay_in_samps,
        const sample_t& pitch_env_amount,
        const sample_t& pitch_decay_in_samps,
        const sample_t& sampleRate
    ){

        drum.carrierPhase = trig>0?0:drum.carrierPhase;
        drum.moddulatorPhase = trig>0?0:drum.moddulatorPhase;

        const sample_t curve = 0.005;
        sample_t indexRamp, pitchRamp, ampRamp;
        
        std::tie(drum.indexEnv, indexRamp) = process<sample_t>(drum.indexEnv, trig, 5, mod_decay_in_samps, curve);
        sample_t _fmIndex = fmIndex * indexRamp;

        sample_t click;
        std::tie(drum.click, click) = process<sample_t>(drum.click, trig);
        click*=0.7;

        sample_t out;
        std::tie(drum.osc1, out) = process<sample_t>(drum.osc1, drum.carrierPhase, drum.moddulatorPhase, _fmIndex, feedback);
        std::tie(drum.ampEnv, ampRamp) = process<sample_t>(drum.ampEnv, trig, 50.0, amp_decay_in_samps, curve);
        out*=ampRamp;


        std::tie(drum.pitchEnv, pitchRamp) = process<sample_t>(drum.pitchEnv, trig, 5.0,pitch_decay_in_samps, curve);
        sample_t _carrierFreq = carrierFreq + pitch_env_amount*pitchRamp;
        sample_t _modFreq = modRatio*_carrierFreq;
        drum.carrierPhase = update_phase_<sample_t>(drum.carrierPhase, compute_phase_increment_<sample_t>(_carrierFreq,sampleRate,1.0), 1.0);
        drum.moddulatorPhase = update_phase_<sample_t>(drum.moddulatorPhase, compute_phase_increment_<sample_t>(_modFreq,sampleRate,1.0), 1.0);


        return std::pair(drum, out+click);
    }

    using algae::dsp::core::oscillator::noise;
    using algae::dsp::core::filter::hip;
    using algae::dsp::core::filter::hip_t;
    using algae::dsp::core::filter::biquad_t;
    using algae::dsp::core::filter::highpass;
    using algae::dsp::core::filter::process;


    template <typename sample_t>
    struct efm_sd_t {
        perc_t<sample_t> ampEnv;
        perc_t<sample_t> pitchEnv;
        perc_t<sample_t> noiseEnv;
        perc_t<sample_t> indexEnv;
        fbfm_t<sample_t> osc1;
        click_t<sample_t> click;
        biquad_t<sample_t> hpf;
        sample_t carrierPhase=0;
        sample_t moddulatorPhase=0;
    };


    template <typename sample_t>
    const inline std::pair<efm_sd_t<sample_t>, sample_t> process(
        efm_sd_t<sample_t> drum,
        const sample_t& trig,
        const sample_t& carrierFreq,
        const sample_t& modRatio,
        const sample_t& fmIndex,
        const sample_t& feedback,
        const sample_t& amp_decay_in_samps,
        const sample_t& mod_decay_in_samps,
        const sample_t& noiseLevel,
        const sample_t& noise_env_decay_in_samps,
        // const sample_t& hp_filter_cutoff,

        const sample_t& sampleRate
    ){

        drum.carrierPhase = trig>0?0:drum.carrierPhase;
        drum.moddulatorPhase = trig>0?0:drum.moddulatorPhase;

        const sample_t curve = 0.005;
        sample_t indexRamp, pitchRamp, ampRamp, noiseRamp;

        sample_t nz = noise<sample_t>();
        std::tie(drum.noiseEnv, noiseRamp) = process<sample_t>(drum.noiseEnv, trig, 60.0, noise_env_decay_in_samps, curve);
        nz *= noiseLevel;
        nz *= noiseRamp;

        sample_t click;
        std::tie(drum.click, click) = process<sample_t>(drum.click, trig);
        click*=0.7;

        sample_t osc;
        std::tie(drum.indexEnv, indexRamp) = process<sample_t>(drum.indexEnv, trig, 5, mod_decay_in_samps, curve);
        sample_t _fmIndex = fmIndex * indexRamp;

        std::tie(drum.osc1, osc) = process<sample_t>(drum.osc1, drum.carrierPhase, drum.moddulatorPhase, _fmIndex, feedback);
        std::tie(drum.ampEnv, ampRamp) = process<sample_t>(drum.ampEnv, trig, 50.0, amp_decay_in_samps, curve);
        osc*=ampRamp;

        
        // drum.hpf = highpass<sample_t>(drum.hpf, hp_filter_cutoff, 1, sampleRate);
        // drum.hpf = process<sample_t>(drum.hpf, osc+click+nz);
        // sample_t out = drum.hpf.y1;

        const sample_t pitch_decay_in_samps = 0.1*amp_decay_in_samps;
        std::tie(drum.pitchEnv, pitchRamp) = process<sample_t>(drum.pitchEnv, trig, 0.0, pitch_decay_in_samps, curve);
        sample_t _carrierFreq = carrierFreq*(pitchRamp+1);
        sample_t _modFreq = modRatio*_carrierFreq;
        drum.carrierPhase = update_phase_<sample_t>(drum.carrierPhase, compute_phase_increment_<sample_t>(_carrierFreq,sampleRate,1.0), 1.0);
        drum.moddulatorPhase = update_phase_<sample_t>(drum.moddulatorPhase, compute_phase_increment_<sample_t>(_modFreq,sampleRate,1.0), 1.0);


        // out = click+nz+osc;

        return std::pair(drum,  click+nz+osc);
    }
}