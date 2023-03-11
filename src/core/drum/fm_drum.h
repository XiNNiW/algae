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
        sample_t carrierPhase=0;
        sample_t moddulatorPhase=0;

        inline const sample_t process(
            const sample_t trig,
            const sample_t carrierFreq,
            const sample_t modRatio,
            const sample_t fmIndex,
            const sample_t feedback,
            const sample_t amp_decay_in_samps,
            const sample_t mod_decay_in_samps,
            const sample_t pitch_env_amount,
            const sample_t pitch_decay_in_samps,
            const sample_t sampleRate
        ){
            carrierPhase    = trig>0?0:carrierPhase;
            moddulatorPhase = trig>0?0:moddulatorPhase;

            const sample_t curve = 0.005;
            
            
            sample_t indexRamp  = indexEnv.process(trig, 5, mod_decay_in_samps, curve);
            sample_t _fmIndex   = fmIndex * indexRamp;

            sample_t out        = osc1.process( carrierPhase, moddulatorPhase, _fmIndex, feedback);
            sample_t ampRamp    = ampEnv.process(trig, 50.0, amp_decay_in_samps, curve);
            out*=ampRamp;

            sample_t pitchRamp  = pitchEnv.process(trig, 5.0, pitch_decay_in_samps, curve);
            sample_t _carrierFreq = carrierFreq + pitch_env_amount*pitchRamp;
            sample_t _modFreq   = modRatio*_carrierFreq;
            carrierPhase        = update_phase_<sample_t>(carrierPhase, compute_phase_increment_<sample_t>(_carrierFreq,sampleRate,1.0), 1.0);
            moddulatorPhase     = update_phase_<sample_t>(moddulatorPhase, compute_phase_increment_<sample_t>(_modFreq,sampleRate,1.0), 1.0);

            return out;
        }
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

        // sample_t click;
        // std::tie(drum.click, click) = process<sample_t>(drum.click, trig);
        // click*=0.7;

        sample_t out;
        std::tie(drum.osc1, out) = process<sample_t>(drum.osc1, drum.carrierPhase, drum.moddulatorPhase, _fmIndex, feedback);
        std::tie(drum.ampEnv, ampRamp) = process<sample_t>(drum.ampEnv, trig, 50.0, amp_decay_in_samps, curve);
        out*=ampRamp;


        std::tie(drum.pitchEnv, pitchRamp) = process<sample_t>(drum.pitchEnv, trig, 5.0,pitch_decay_in_samps, curve);
        sample_t _carrierFreq = carrierFreq + pitch_env_amount*pitchRamp;
        sample_t _modFreq = modRatio*_carrierFreq;
        drum.carrierPhase = update_phase_<sample_t>(drum.carrierPhase, compute_phase_increment_<sample_t>(_carrierFreq,sampleRate,1.0), 1.0);
        drum.moddulatorPhase = update_phase_<sample_t>(drum.moddulatorPhase, compute_phase_increment_<sample_t>(_modFreq,sampleRate,1.0), 1.0);


        return std::pair<efm_bd_t<sample_t>, sample_t>(drum, out);
    }

    

    using algae::dsp::core::oscillator::noise;
    using algae::dsp::core::filter::hip;
    using algae::dsp::core::filter::hip_t;
    using algae::dsp::core::filter::biquad_t;
    using algae::dsp::core::filter::highpass;
    using algae::dsp::core::filter::process;
    using algae::dsp::core::filter::lowpass;

    template <typename sample_t>
    struct efm_sd_t {
        perc_t<sample_t> ampEnv;
        perc_t<sample_t> pitchEnv;
        perc_t<sample_t> noiseEnv;
        perc_t<sample_t> indexEnv;
        fbfm_t<sample_t> osc1;
        click_t<sample_t> click;
        biquad_t<sample_t> hpf;
        biquad_t<sample_t> lpf;
        sample_t carrierPhase=0;
        sample_t moddulatorPhase=0;

        const inline sample_t process(
            const sample_t& trig,
            const sample_t& carrierFreq,
            const sample_t& modRatio,
            const sample_t& fmIndex,
            const sample_t& feedback,
            const sample_t& amp_decay_in_samps,
            const sample_t& mod_decay_in_samps,
            const sample_t& noiseLevel,
            const sample_t& noise_env_decay_in_samps,
            const sample_t& hp_filter_cutoff,
            const sample_t& sampleRate
        ){

            carrierPhase = trig>0?0:carrierPhase;
            moddulatorPhase = trig>0?0:moddulatorPhase;

            const sample_t curve = 0.005;
        
            sample_t nz = noise<sample_t>();
            const sample_t noise_attack_samps = (15*(sampleRate/1000.0));
            sample_t noiseRamp = noiseEnv.process(trig, noise_attack_samps, noise_env_decay_in_samps, curve);
            nz *= noiseLevel;
            nz *= noiseRamp;

            sample_t indexRamp = indexEnv.process(trig, 1, mod_decay_in_samps, curve);
            sample_t _fmIndex = fmIndex * indexRamp;

            sample_t osc = osc1.process(carrierPhase, moddulatorPhase, _fmIndex, feedback);
            sample_t amp_attack_samps = (5*(sampleRate/1000.0));
            sample_t ampRamp = ampEnv.process(trig, amp_attack_samps, amp_decay_in_samps, curve);
            osc*=ampRamp;
            
            hpf = highpass<sample_t>(hpf, hp_filter_cutoff>0?hp_filter_cutoff:15, 0.77, sampleRate);
            hpf = algae::dsp::core::filter::process<sample_t>(hpf, osc+nz);
            sample_t out = hpf.y1;

            lpf = lowpass<sample_t, sample_t>(lpf, 17000.0, 0.7, sampleRate);
            lpf = algae::dsp::core::filter::process<sample_t>(lpf, out);
            out = lpf.y1;

            const sample_t pitch_decay_in_samps = 0.1*amp_decay_in_samps;
            sample_t pitchRamp = pitchEnv.process(trig, 0.0, pitch_decay_in_samps, curve);
            sample_t _carrierFreq = carrierFreq*(pitchRamp+1);
            sample_t _modFreq = modRatio*_carrierFreq;
            carrierPhase = update_phase_<sample_t>(carrierPhase, compute_phase_increment_<sample_t>(_carrierFreq,sampleRate,1.0), 1.0);
            moddulatorPhase = update_phase_<sample_t>(moddulatorPhase, compute_phase_increment_<sample_t>(_modFreq,sampleRate,1.0), 1.0);

            return out;
        }
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
        const sample_t& hp_filter_cutoff,

        const sample_t& sampleRate
    ){

        drum.carrierPhase = trig>0?0:drum.carrierPhase;
        drum.moddulatorPhase = trig>0?0:drum.moddulatorPhase;

        const sample_t curve = 0.005;
        sample_t indexRamp, pitchRamp, ampRamp, noiseRamp;

        sample_t nz = noise<sample_t>();
        const sample_t noise_attack_samps = (15*(sampleRate/1000.0));
        std::tie(drum.noiseEnv, noiseRamp) = process<sample_t>(drum.noiseEnv, trig, noise_attack_samps, noise_env_decay_in_samps, curve);
        nz *= noiseLevel;
        nz *= noiseRamp;

        sample_t click;
        std::tie(drum.click, click) = process<sample_t>(drum.click, trig);
        click*=0.8;

        sample_t osc;
        std::tie(drum.indexEnv, indexRamp) = process<sample_t>(drum.indexEnv, trig, 1, mod_decay_in_samps, curve);
        sample_t _fmIndex = fmIndex * indexRamp;

        std::tie(drum.osc1, osc) = process<sample_t>(drum.osc1, drum.carrierPhase, drum.moddulatorPhase, _fmIndex, feedback);
        const sample_t amp_attack_samps = (5*(sampleRate/1000.0));
        std::tie(drum.ampEnv, ampRamp) = process<sample_t>(drum.ampEnv, trig, amp_attack_samps, amp_decay_in_samps, curve);
        osc*=ampRamp;

        
        drum.hpf = highpass<sample_t>(drum.hpf, hp_filter_cutoff>0?hp_filter_cutoff:15, 0.77, sampleRate);
        drum.hpf = process<sample_t>(drum.hpf, osc+click+nz);
        sample_t out = drum.hpf.y1;

        const sample_t pitch_decay_in_samps = 0.1*amp_decay_in_samps;
        std::tie(drum.pitchEnv, pitchRamp) = process<sample_t>(drum.pitchEnv, trig, 0.0, pitch_decay_in_samps, curve);
        sample_t _carrierFreq = carrierFreq*(pitchRamp+1);
        sample_t _modFreq = modRatio*_carrierFreq;
        drum.carrierPhase = update_phase_<sample_t>(drum.carrierPhase, compute_phase_increment_<sample_t>(_carrierFreq,sampleRate,1.0), 1.0);
        drum.moddulatorPhase = update_phase_<sample_t>(drum.moddulatorPhase, compute_phase_increment_<sample_t>(_modFreq,sampleRate,1.0), 1.0);


        // out = click+nz+osc;

        return std::pair<efm_sd_t<sample_t>, sample_t>(drum,  out);
    }


    template <typename sample_t>
    struct efm_xt_t {
        perc_t<sample_t> ampEnv;
        perc_t<sample_t> pitchEnv;
        perc_t<sample_t> indexEnv;
        fbfm_t<sample_t> osc1;
        click_t<sample_t> click;
        sample_t carrierPhase=0;
        sample_t moddulatorPhase=0;
    };

    template <typename sample_t>
    const inline std::pair<efm_xt_t<sample_t>, sample_t> process(
        efm_xt_t<sample_t> drum,
        const sample_t& trig,
        const sample_t& carrierFreq,
        const sample_t& modRatio,
        const sample_t& fmIndex,
        const sample_t& feedback,
        const sample_t& amp_decay_in_samps,
        const sample_t& mod_decay_in_samps,
        const sample_t& pitch_env_amount,
        const sample_t& pitch_decay_in_samps,
        const sample_t& click_level,

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
        click*=click_level;

        sample_t out;
        std::tie(drum.osc1, out) = process<sample_t>(drum.osc1, drum.carrierPhase, drum.moddulatorPhase, _fmIndex, feedback);
        std::tie(drum.ampEnv, ampRamp) = process<sample_t>(drum.ampEnv, trig, 50.0, amp_decay_in_samps, curve);
        out*=ampRamp;


        std::tie(drum.pitchEnv, pitchRamp) = process<sample_t>(drum.pitchEnv, trig, 5.0,pitch_decay_in_samps, curve);
        sample_t _carrierFreq = carrierFreq + pitch_env_amount*pitchRamp;
        sample_t _modFreq = modRatio*_carrierFreq;
        drum.carrierPhase = update_phase_<sample_t>(drum.carrierPhase, compute_phase_increment_<sample_t>(_carrierFreq,sampleRate,1.0), 1.0);
        drum.moddulatorPhase = update_phase_<sample_t>(drum.moddulatorPhase, compute_phase_increment_<sample_t>(_modFreq,sampleRate,1.0), 1.0);


        return std::pair<efm_xt_t<sample_t>, sample_t>(drum, out+click);
    }


    template <typename sample_t>
    struct efm_clap_env_t {
        perc_t<sample_t> clap;
        sample_t phase=0;
        sample_t burstPhase=1;
        int count=0;
    };
   
    template <typename sample_t>
    inline const std::pair<efm_clap_env_t<sample_t>, sample_t> process(
        efm_clap_env_t<sample_t> env, const sample_t& trig, const sample_t& decay_in_samps, const sample_t& num_claps
    ){

        if(trig>0){
            env.phase = 0;
            env.count=int(num_claps);
            env.burstPhase = 0;
        }

        sample_t out=0;
        const sample_t burstIncrement = num_claps/decay_in_samps;

        if(env.count>0){
            
            sample_t burst=0;


            sample_t burstPhase = env.burstPhase+burstIncrement;
            burstPhase = burstPhase>=1?burstPhase-1:burstPhase;
            burst = burstPhase-env.burstPhase<0?1:0;
            env.burstPhase = burstPhase;

            if(burst>0)
                env.count--;
            

            std::tie(env.clap, out) = process<sample_t>(env.clap, burst, 3.0, 1.0/burstIncrement-3, 0.005);
        } 
        else {
            //if (env.phase <=1){
            out = 1;//env.phase;
           // env.phase += 1/50.0;
        }

        return std::pair<efm_clap_env_t<sample_t>, sample_t>(env, out);
    }

    template <typename sample_t>
    struct efm_cp_t {
        perc_t<sample_t> ampEnv;
        perc_t<sample_t> pitchEnv;
        perc_t<sample_t> indexEnv;
        efm_clap_env_t<sample_t> clapEnv;
        fbfm_t<sample_t> osc1;
        biquad_t<sample_t> hpf;
        sample_t carrierPhase=0;
        sample_t moddulatorPhase=0;
    };


    template <typename sample_t>
    const inline std::pair<efm_cp_t<sample_t>, sample_t> process(
        efm_cp_t<sample_t> drum,
        const sample_t& trig,
        const sample_t& carrierFreq,
        const sample_t& modRatio,
        const sample_t& fmIndex,
        const sample_t& feedback,
        const sample_t& amp_decay_in_samps,
        const sample_t& mod_decay_in_samps,
        const sample_t& number_of_claps,
        const sample_t& claps_env_decay_in_samps,
        const sample_t& hp_filter_cutoff,

        const sample_t& sampleRate
    ){

        drum.carrierPhase = trig>0?0:drum.carrierPhase;
        drum.moddulatorPhase = trig>0?0:drum.moddulatorPhase;

        const sample_t curve = 0.005;
        sample_t indexRamp, pitchRamp, ampRamp, clapRamp;


        std::tie(drum.clapEnv, clapRamp) = process<sample_t>(drum.clapEnv, trig, claps_env_decay_in_samps, number_of_claps);

        sample_t osc;
        std::tie(drum.indexEnv, indexRamp) = process<sample_t>(drum.indexEnv, trig, 1, mod_decay_in_samps, curve);
        indexRamp *= clapRamp;
        sample_t _fmIndex = fmIndex * indexRamp;

        std::tie(drum.osc1, osc) = process<sample_t>(drum.osc1, drum.carrierPhase, drum.moddulatorPhase, _fmIndex, feedback);
        std::tie(drum.ampEnv, ampRamp) = process<sample_t>(drum.ampEnv, trig, 50.0, amp_decay_in_samps, curve);
        ampRamp *= clapRamp;
        osc*=ampRamp;

        
        drum.hpf = highpass<sample_t>(drum.hpf, hp_filter_cutoff>0?hp_filter_cutoff:15, 0.77, sampleRate);
        drum.hpf = process<sample_t>(drum.hpf, osc);
        sample_t out = drum.hpf.y1;

        const sample_t pitch_decay_in_samps = 0.1*amp_decay_in_samps;
        std::tie(drum.pitchEnv, pitchRamp) = process<sample_t>(drum.pitchEnv, trig, 0.0, pitch_decay_in_samps, curve);
        sample_t _carrierFreq = carrierFreq*(pitchRamp+1);
        sample_t _modFreq = modRatio*_carrierFreq;
        drum.carrierPhase = update_phase_<sample_t>(drum.carrierPhase, compute_phase_increment_<sample_t>(_carrierFreq,sampleRate,1.0), 1.0);
        drum.moddulatorPhase = update_phase_<sample_t>(drum.moddulatorPhase, compute_phase_increment_<sample_t>(_modFreq,sampleRate,1.0), 1.0);


        // out = click+nz+osc;

        return std::pair<efm_cp_t<sample_t>, sample_t>(drum,  out);
    }

    using algae::dsp::core::oscillator::sin_fb_t;
    template <typename sample_t>
    struct efm_rs_t {
        perc_t<sample_t> ampEnv;
        perc_t<sample_t> snareEnv;
        sin_fb_t<sample_t> carrier1;
        sin_fb_t<sample_t> carrier2;
        sin_fb_t<sample_t> modulator1;
        sin_fb_t<sample_t> modulator2;
        biquad_t<sample_t> hpf;
    };

    template <typename sample_t>
    inline const std::pair<efm_rs_t<sample_t>,sample_t> process(
        efm_rs_t<sample_t> drum,
        const sample_t& trig,
        const sample_t& carrierFreq,
        const sample_t& snareCarrierFreq,
        const sample_t& modRatio,
        const sample_t& fmIndex,
        const sample_t& snareFmIndex,
        const sample_t& feedback,
        const sample_t& amp_decay_in_samps,
        const sample_t& snare_decay_in_samps,
        const sample_t& hp_filter_cutoff,
        const sample_t& sampleRate
    ){

        drum.carrier1.phase = trig>0?0:drum.carrier1.phase;
        drum.carrier2.phase = trig>0?0:drum.carrier2.phase;

        const sample_t curve = 0.005;
        sample_t ampRamp, snareRamp;
        const sample_t amp_attack_samps = 0.5*(sampleRate/1000.0);
        std::tie(drum.ampEnv, ampRamp) = process<sample_t>(drum.ampEnv, trig, amp_attack_samps, amp_decay_in_samps, curve);
        const sample_t snare_attack_samps = 3*amp_attack_samps;
        std::tie(drum.snareEnv, snareRamp) = process<sample_t>(drum.snareEnv, trig, snare_attack_samps, snare_decay_in_samps, curve);

        sample_t car1, car2, mod1, mod2;
        std::tie(drum.modulator1, mod1) = process<sample_t>(drum.modulator1, carrierFreq*modRatio, 0, feedback, sampleRate);
        std::tie(drum.modulator2, mod2) = process<sample_t>(drum.modulator2, snareCarrierFreq*modRatio, 0, feedback, sampleRate);
        std::tie(drum.carrier1, car1)   = process<sample_t>(drum.carrier1, carrierFreq, fmIndex*mod1, mod1, sampleRate);
        std::tie(drum.carrier2, car2)   = process<sample_t>(drum.carrier2, snareCarrierFreq, snareFmIndex*mod2, mod2, sampleRate);

        car1 *= ampRamp;
        car2 *= snareRamp;

        sample_t out = (car1+car2);

        drum.hpf = highpass<sample_t>(drum.hpf, hp_filter_cutoff>0?hp_filter_cutoff:15, 0.77, sampleRate);
        drum.hpf = process<sample_t>(drum.hpf, out);
        out = drum.hpf.y1;

        return std::pair<efm_rs_t<sample_t>,sample_t>(drum, out);

    }

    template <typename sample_t>
    struct efm_cb_t {
        perc_t<sample_t> ampEnv;
        perc_t<sample_t> snapEnv;
        perc_t<sample_t> modEnv;
        sin_fb_t<sample_t> carrier1;
        sin_fb_t<sample_t> carrier2;
        sin_fb_t<sample_t> modulator1;
    };


    template <typename sample_t>
    inline const std::pair<efm_cb_t<sample_t>,sample_t> process(
        efm_cb_t<sample_t> drum,
        const sample_t& trig,
        const sample_t& carrierFreq,
        const sample_t& modRatio,
        const sample_t& fmIndex,
        const sample_t& feedback,
        const sample_t& snap,
        const sample_t& amp_decay_in_samps,
        const sample_t& mod_decay_in_samps,
        const sample_t& sampleRate
    ){
        const sample_t curve = 0.005;

        drum.carrier1.phase = trig>0?0:drum.carrier1.phase;
        drum.carrier2.phase = trig>0?0:drum.carrier2.phase;

        sample_t ampRamp, modRamp, snapRamp;
        const sample_t amp_attack_samps = 0.5*(sampleRate/1000.0);
        std::tie(drum.ampEnv,  ampRamp)  = process<sample_t>(drum.ampEnv, trig, amp_attack_samps, amp_decay_in_samps, curve);
        std::tie(drum.modEnv,  modRamp)  = process<sample_t>(drum.modEnv, trig, amp_attack_samps, mod_decay_in_samps, curve);
        sample_t snap_decay_in_samps = amp_decay_in_samps + snap*amp_decay_in_samps*(0.125-1);
        std::tie(drum.snapEnv, snapRamp) = process<sample_t>(drum.snapEnv, trig, amp_attack_samps, snap_decay_in_samps, curve);

        sample_t car1, car2, mod1;
        std::tie(drum.modulator1, mod1) = process<sample_t>(drum.modulator1, carrierFreq*modRatio, 0, feedback, sampleRate);
        mod1 *= modRamp*fmIndex;
        sample_t fb = feedback*modRamp;
        std::tie(drum.carrier1, car1)   = process<sample_t>(drum.carrier1, carrierFreq, mod1, fb, sampleRate);
        std::tie(drum.carrier2, car2)   = process<sample_t>(drum.carrier2, 1.5*carrierFreq, mod1, fb, sampleRate);

        car1 *= ampRamp;
        car2 *= snapRamp;
        sample_t out = (car1+car2);


        return std::pair<efm_cb_t<sample_t>,sample_t>(drum, out);
    }

    template <typename sample_t>
    struct efm_cy_t {
        perc_t<sample_t> ampEnv;
        perc_t<sample_t> modEnv;
        perc_t<sample_t> modEnv2;
        click_t<sample_t> click;
        sin_fb_t<sample_t> carrier1;
        sin_fb_t<sample_t> carrier2;
        sin_fb_t<sample_t> carrier3;
        sin_fb_t<sample_t> carrier4;
        sin_fb_t<sample_t> modulator1;
        sin_fb_t<sample_t> modulator2;
        biquad_t<sample_t> hpf;
        biquad_t<sample_t> lpf;
    };

    using algae::dsp::core::filter::lowpass;

    template <typename sample_t>
    inline const std::pair<efm_cy_t<sample_t>, sample_t> process(
        efm_cy_t<sample_t> drum,
        const sample_t& trig,
        const sample_t& carrierFreq,
        const sample_t& modRatio,
        const sample_t& fmIndex,
        const sample_t& feedback,
        const sample_t& amp_decay_in_samps,
        const sample_t& mod_decay_in_samps,
        const sample_t& hpf_cutoff,
        const sample_t& sampleRate
    ){

        const sample_t curve = 0.005;

        drum.carrier1.phase = trig>0?0:drum.carrier1.phase;
        drum.carrier2.phase = trig>0?0:drum.carrier2.phase;
        drum.carrier3.phase = trig>0?0:drum.carrier3.phase;
        drum.carrier4.phase = trig>0?0:drum.carrier4.phase;

        sample_t ampRamp, modRamp1, modRamp2;
        const sample_t amp_attack_samps = 0.5*(sampleRate/1000.0);
        std::tie(drum.ampEnv,   ampRamp)   = process<sample_t>(drum.ampEnv,  trig, amp_attack_samps, amp_decay_in_samps,      curve);
        std::tie(drum.modEnv,   modRamp1)  = process<sample_t>(drum.modEnv,  trig, amp_attack_samps, mod_decay_in_samps,      curve);
        std::tie(drum.modEnv2,  modRamp2)  = process<sample_t>(drum.modEnv2, trig, amp_attack_samps, 0.25*amp_decay_in_samps, curve);

        sample_t car1, car2, car3, car4, mod1, mod2;
        std::tie(drum.modulator1, mod1) = process<sample_t>(drum.modulator1, carrierFreq*modRatio, 0, feedback, sampleRate);
        std::tie(drum.modulator2, mod2) = process<sample_t>(drum.modulator2, carrierFreq*3,        0, feedback, sampleRate);
        mod1 *= modRamp1;
        mod2 *= modRamp2;
        sample_t mods = fmIndex*(mod1 + mod2);

        std::tie(drum.carrier1, car1) = process<sample_t>(drum.carrier1, carrierFreq,           mods, feedback, sampleRate);
        std::tie(drum.carrier2, car2) = process<sample_t>(drum.carrier2, carrierFreq*1.5,       mods, feedback, sampleRate);
        std::tie(drum.carrier3, car3) = process<sample_t>(drum.carrier3, carrierFreq*modRatio,  mods, feedback, sampleRate);
        std::tie(drum.carrier4, car4) = process<sample_t>(drum.carrier4, carrierFreq*3,         mods, feedback, sampleRate);

        car1 *= 0.05;
        car2 *= 0.25;
        car3 *= 0.2;
        car4 *= 0.125;

        sample_t click;
        std::tie(drum.click, click) = process<sample_t>(drum.click, trig);
        click*=0.7;

        sample_t out = ampRamp*(car1 + car2 + car3 + car4) + click;

        drum.hpf = highpass<sample_t>(drum.hpf, hpf_cutoff>0?hpf_cutoff:15, sample_t(0.77), sampleRate);
        drum.hpf = process<sample_t>(drum.hpf, out);
        out = drum.hpf.y1;
        drum.lpf = lowpass<sample_t>(drum.lpf, sample_t(17000.0), sample_t(0.5), sampleRate);
        drum.lpf = process<sample_t>(drum.lpf, out);
        out = drum.lpf.y1;

        return std::pair<efm_cy_t<sample_t>, sample_t>(drum, out);

    }


}