#ifndef SACBITSTREAM_H
#define SACBITSTREAM_H

// System libraries
#include "stdlib.h"
#include "vector"
// Classes and local files
#include "../process/File.h"
#include "../tools/Logger.h"

/**
 * @class	SACBitstream
 * @author	Andrés González Fornell
 * @brief	SAC bitstream class.
 */
class SACBitstream {
public:
    /**
     * @brief   It specifies the channel type.
     */
    struct ChannelType {
        enum channeltype {
            L = 0x0, /**< left front channel */
            Lc = 0x1, /**< left front center channel  */
            Ls = 0x2, /**< left surround channel */
            Lsr = 0x3, /**< rear surround left channel */
            R = 0x4, /**< left front channel */
            Rc = 0x5, /**< left front center channel  */
            Rs = 0x6, /**< left surround channel */
            Rsr = 0x7, /**< rear surround left channel */
            C = 0x8, /**< center front channel */
            LFE = 0x9 /**< low frequency enhancement channel */
        };
    };
    int fs; /**< signal sampling frequencye */
    std::vector<ChannelType::channeltype> channels; /**< channels order */
    double gain_surround; /**< downmix of surround channels */
    double gain_LFE; /**< downmix of LFE channels */
    double gain_downmix; /**< gain of downmix*/
    SACBitstream();
    SACBitstream(std::string filename);
    ~SACBitstream();
    long getVariable(int position, int length);
    void load();
private:
    File *bitstream;
};

#endif // SACBITSTREAM_H
