#include <math.h>
namespace algae::dsp::core::filter{
    template<typename sample_t>
    struct integrator_t{
        sample_t y1;
    };

    template<typename sample_t>
    integrator_t<sample_t> process_integrator(const integrator_t<sample_t> i, const sample_t input){
        sample_t xn = input;
        sample_t yn = xn + i.y1;
        return integrator_t<sample_t>{yn};
    }

    template<typename sample_t>
    struct leaky_integrator_t{
        sample_t x1;
        sample_t y1=-1;
        sample_t y2;
        sample_t a1;
        sample_t b1;
        sample_t b2;
    };

    template<typename sample_t, typename frequency_t>
    leaky_integrator_t<sample_t> leaky_integrator(
        const leaky_integrator_t<sample_t> i, 
        const frequency_t cutoff, 
        const frequency_t sampleRate
    ){
        const frequency_t TWO_PI = M_PI+M_PI;
        frequency_t gamma = exp(TWO_PI*(cutoff/sampleRate));
        return leaky_integrator_t<sample_t>{
            i.x1,
            i.y1,
            i.y2,
            (gamma+1)*M_PI_2,
            2*gamma,
            gamma*gamma
        };
    }

    // derived from https://www.researchgate.net/publication/319876490_LP-BLIT_Bandlimited_Impulse_Train_Synthesis_of_Lowpass-filtered_Waveforms
    template<typename sample_t>
    leaky_integrator_t<sample_t> process_leaky_integrator(
        const leaky_integrator_t<sample_t> i, 
        const sample_t input
    )
    {
        sample_t xn = input;
        sample_t a  = i.a1;
        sample_t b1 = i.b1;
        sample_t b2 = i.b2;
        sample_t yn = a*xn - a*i.x1 + b1*i.y1 - b2*i.y2;
        return leaky_integrator_t<sample_t>{
            xn,
            yn,
            i.y1,
            a,
            b1,
            b2
        };
    }    


    template<typename sample_t>
    struct leaky_integrator_onepole_t{
        sample_t x1;
        sample_t y1=-0.5;
        sample_t a1;
        sample_t b1;
    };

    template<typename sample_t, typename frequency_t>
    leaky_integrator_onepole_t<sample_t> leaky_integrator_onepole(
        const leaky_integrator_onepole_t<sample_t> i 
        // const frequency_t cutoff, 
        // const frequency_t sampleRate
    ){
        frequency_t c = 0.9992;
        frequency_t a = M_PI_2+M_PI_2*c;
        return leaky_integrator_onepole_t<sample_t>{
            i.x1,
            i.y1,
            a,
            c
        };
    }

    template<typename sample_t>
    leaky_integrator_onepole_t<sample_t> process_leaky_integrator_onepole(
        const leaky_integrator_onepole_t<sample_t> i,
        const sample_t input
    ){
        sample_t xn = input;
        sample_t a  = i.a1;
        sample_t b1 = i.b1;
        sample_t yn = a*xn - a*i.x1 + b1*i.y1;
        return leaky_integrator_onepole_t<sample_t>{
            xn,
            yn,
            a,
            b1
        };
    }


}