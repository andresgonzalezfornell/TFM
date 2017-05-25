#include "Equalizer.h"

/**
 * @brief	Equalizer constructor.
 * @param   params          string of effect parameters
 */
Equalizer::Equalizer() AS_EFFECT_CONSTRUCTOR {
    for (int n = 0; n < order + 1; n++) {
        x[n] = 0;
        y[n] = 0;
    }
}

/**
 * @brief	It applies equalization effect.
 * @param 	input			input signal pointer
 * @param 	output			output signal pointer
 * @param 	samples			number of samples
 */
void Equalizer::apply(float *input, float *output, int samples) {

    const double Q = std::sqrt(2);
    // Gain-compensanting matrix
    const double C[bands][bands] = {{1.32220858046976,-0.312321455237198,0.0381733194898166,0.00472793396660785,0.000796727762724219,-0.000744140323074050,6.80405294875517e-05,7.36601688367619e-06,4.14703938689660e-06,-5.19536284602576e-07},
                                    {-0.251205632166420,1.10548831103229,-0.227386697166027,0.00171945236427240,-0.00285873237162747,0.00287233145854405,-0.000479215637480309,1.29008881586574e-05,-1.69015159464277e-05,1.66079012027255e-06},
                                    {0.000521551123389682,-0.220257367383385,1.08801423781665,-0.208891060853447,0.00100631176474711,-0.00326619885029453,0.00278935865152149,-0.000443000889171608,2.84858579080544e-05,1.68236500096876e-06},
                                    {-0.00393382064013759,0.00354692296973240,-0.209006490154809,1.08334007540533,-0.208438430620398,0.000983488771829574,-0.00323077199479425,0.00277390412538065,-0.000414252847128373,1.45598334670541e-05},
                                    {0.00338289668594477,-0.00350643054985602,0.00110361276585029,-0.208424005491719,1.08335658509289,-0.208438258947131,0.000975349984993184,-0.00322264096557444,0.00267016621873608,-0.000249776821508366},
                                    {-0.000526332664213033,0.00285529041400352,-0.00328106710446579,0.000980363688839010,-0.208442435685026,1.08336074604426,-0.208430458927100,0.00141218755297895,-0.00490585004585491,0.000506875669208193},
                                    {1.23866798423673e-05,-0.000469288856391090,0.00278974142312004,-0.00323341180843883,0.000993069350969063,-0.208447890922575,1.08335368447797,-0.209785295224579,0.00713910238777306,0.00147227948241328},
                                    {-1.67215539726301e-05,1.71012376991073e-05,-0.000439351192571039,0.00277020557181152,-0.00317997527179451,0.000925226024315485,-0.208403336941899,1.07944254655709,-0.189268015480721,0.00865146391755439},
                                    {8.21675861167529e-06,-1.33827217506060e-05,8.75424004692147e-06,-0.000438085935860196,0.00271779142944750,-0.00272026736573885,0.000212280546089003,-0.209482833335562,1.05310021990880,-0.109803397508004},
                                    {-1.04588368713643e-06,1.81124326392954e-06,3.37012587142700e-06,3.57765694705449e-05,-0.000370947922770962,0.000395303373362004,0.00218544052443248,0.0197161395759657,-0.154831224077047,1.08009144788505}};
    // Initialization
    double level[bands];
    double gain[bands];
    for (int band = 0; band < bands; band++) {
        level[band] = getDouble(params["band" + std::to_string(band)]); // [dB]
        gain[band] = 0;
    }
    // Effect
    for (int band = 0; band < bands; band++) {
        for (int i = 0; i < bands; i++) {
            gain[band] += C[band][i]*level[band]; // [dB]
        }
        if (band == 0) {
            lowShelfFilter(input, output, samples, frequency[band] / this->fs, pow(10, gain[band] / 20), order);
        } else if (band == bands) {
            highShelfFilter(output, output, samples, frequency[band] / this->fs, pow(10, gain[band] / 20), order);
        } else {
            peakingFilter(output, output, samples, frequency[band] / this->fs, pow(10, gain[band] / 20), Q, order);
        }
    }
}

/**
 * @brief   It sends some values to user interface charts.
 * @param   chart           chart id
 * @return  array of values as values[axis][sample]     axis: 0 = x (horizontal) and 1 = y (vertical)
 */
std::vector<std::vector<double>> Equalizer::plot(std::string chart) {
    return std::vector<std::vector<double>>();
}

/**
 * @brief   It applies a peaking filter.
 * @param 	input			input signal pointer
 * @param 	output			output signal pointer
 * @param 	samples			number of samples
 * @param   f_0             center frequency (real frequency / sampling frequency)
 * @param   level           peak level (amplitude)
 * @param   Q               quality factor
 */
void Equalizer::peakingFilter(float *input, float *output, int samples, double f_0, double gain, double Q, int order) {
    // Initialization
    float *a = (float *)std::malloc((order+1)*sizeof(float));
    float *b = (float *)std::malloc((order+1)*sizeof(float));
    // Parameters
    const double pi = 3.1415926536;
    const double w_0 = 2*pi*f_0;
    // Coefficients
    a[0] = std::sqrt(gain) + std::tan(w_0/Q/2);
    a[1] = -2*std::sqrt(gain)*std::cos(w_0);
    a[2] = std::sqrt(gain) - std::tan(w_0/Q/2);
    b[0] = std::sqrt(gain) + gain*std::tan(w_0/Q/2);
    b[1] = -2*std::sqrt(gain)*std::cos(w_0);
    b[2] = std::sqrt(gain) - gain*std::tan(w_0/Q/2);
    // Filtering
    filter(input, output, samples, a, b, order);
    // Cleaning
    std::free(a);
    std::free(b);
}


/**
 * @brief   It applies a low shelf filter.
 * @param 	input			input signal pointer
 * @param 	output			output signal pointer
 * @param 	samples			number of samples
 * @param   f_0             midpoint frequency (real frequency / sampling frequency)
 * @param   level           peak level (amplitude)
 */
void Equalizer::lowShelfFilter(float *input, float *output, int samples, double f_0, double gain, int order) {
    // Initialization
    float *a = (float *)std::malloc((order+1)*sizeof(float));
    float *b = (float *)std::malloc((order+1)*sizeof(float));
    // Parameters
    const double pi = 3.1415926536;
    const double w_0 = 2*pi*f_0;
    const double w_c = w_0*1.4720;
    const double alpha = std::tan(w_c/2);
    a[0] = std::sqrt(gain) + std::sqrt(2)*alpha*std::pow(gain,0.25) + std::pow(alpha,2);
    a[1] = 2*(std::pow(alpha,2) - std::sqrt(gain));
    a[2] = std::sqrt(gain) - std::sqrt(2)*alpha*std::pow(gain,0.25) + std::pow(alpha,2);
    b[0] = std::sqrt(gain)*(std::sqrt(gain)*std::pow(alpha,2) + std::sqrt(2)*alpha*std::pow(gain,0.25) + 1);
    b[1] = std::sqrt(gain)*(2*(std::sqrt(gain)*std::pow(alpha,2) - 1));
    b[2] = std::sqrt(gain)*(std::sqrt(gain)*std::pow(alpha,2) - std::sqrt(2)*alpha*std::pow(gain,0.25) + 1);
    // Filtering
    filter(input, output, samples, a, b, order);
    // Cleaning
    std::free(a);
    std::free(b);
}

/**
 * @brief   It applies a high shelf filter.
 * @param 	input			input signal pointer
 * @param 	output			output signal pointer
 * @param 	samples			number of samples
 * @param   f_0             midpoint frequency (real frequency / sampling frequency)
 * @param   level           peak level (amplitude)
 */
void Equalizer::highShelfFilter(float *input, float *output, int samples, double f_0, double gain, int order) {
    // Initialization
    float *a = (float *)std::malloc((order+1)*sizeof(float));
    float *b = (float *)std::malloc((order+1)*sizeof(float));
    // Parameters
    const double pi = 3.1415926536;
    const double w_0 = 2*pi*f_0;
    const double w_c = w_0/1.4085;
    const double alpha = std::tan(w_c/2);
    a[0] = std::sqrt(gain)*std::pow(alpha,2) + std::sqrt(2)*alpha*std::pow(gain,0.25) + 1;
    a[1] = 2*(std::sqrt(gain)*std::pow(alpha,2) - 1);
    a[2] = std::sqrt(gain)*std::pow(alpha,2) - std::sqrt(2)*alpha*std::pow(gain,0.25) + 1;
    b[0] = std::sqrt(gain)*(std::sqrt(gain) + std::sqrt(2)*alpha*std::pow(gain,0.25) + std::pow(alpha,2));
    b[1] = std::sqrt(gain)*(2*(std::pow(alpha,2) - std::sqrt(gain)));
    b[2] = std::sqrt(gain)*(std::sqrt(gain) - std::sqrt(2)*alpha*std::pow(gain,0.25) + std::pow(alpha,2));
    // Filtering
    filter(input, output, samples, a, b, order);
    // Cleaning
    std::free(a);
    std::free(b);
}

/**
 * @brief   It applies a filter according to the transfer function H(z) = ( b[0] + b[1]·z^-1 + ... + b[order]·z^-order ) / ( a[0] + a[1]·z^-1 + ... + a[order]·z^-order ).
 * @param 	input           input signal pointer
 * @param 	output          output signal pointer
 * @param 	samples			number of samples
 * @param   a               y coefficients of transfer function
 * @param   b               x coefficients of transfer function
 * @param   order           filter order (value of the highest exponent)
 */
void Equalizer::filter(float *input, float *output, int samples, float *a, float *b, int order) {
    if (a[0] != 0) {
        for (int n = 0; n < samples; n++) {
            // Memories update
            for (int index = order; index > 0; index--) {
                x[index] = x[index - 1];
                y[index] = y[index - 1];
            }
            x[0] = input[n];
            // Filter
            y[0] = b[0] * x[0];
            for (int index = 1; index < order + 1; index++) {
                y[0] += b[index] * x[index] - a[index] * y[index];
            }
            y[0] /= a[0];
            output[n] = y[0];
        }
    }
}
