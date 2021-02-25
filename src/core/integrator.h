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
        sample_t y1;
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
        frequency_t gamma = exp(M_2_PI*(cutoff/sampleRate));
        return leaky_integrator_t<sample_t>{
            i.y1,
            i.y2,
            i.x1,
            (M_PI*gamma+1.0)/2.0,
            2.0*gamma,
            gamma*gamma
        };
    }

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
        sample_t yn = a*(xn - i.x1 ) + b1*i.y1 - b2*i.y2;
        return leaky_integrator_t<sample_t>{
            yn,
            i.y1,
            xn,
            a,
            b1,
            b2
        };
    }    

}