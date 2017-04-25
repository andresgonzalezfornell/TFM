/**
 * @name	TFM.cpp
 * @author	Andrés González Fornell
 * @brief	Test Unit file.
 */

// Classes and local files
#include "process/ProcessManager.h"
#include "effects/Effect.h"
#include "tools/Logger.h"

/**
 * @brief   It print printHelp
 * @param app
 */
void printHelp(std::string app) {
    std::string message = "\n";
    std::string separator = "";
    for (int i = 0; i < 100; i++) {
        separator += "-";
    }
    // Application information
    std::string title = "SAC EFFECTS PLATFORM";
    std::string subtitle = "Command line application to apply effects to a encoded file as Spatial Audio Coding (SAC) format.";
    std::string author = "Andrés González Fornell";
    std::string company = "Escuela Ténica Superior de Ingenieros de Telecomunicación (Universidad Politécnica de Madrid)";
    message += separator + "\n" + title + "\n" + subtitle + "\n" + "Created by " + author + " - " + company + "\n" + separator + "\n";
    // Usage
    message += "\nUSAGE\n";
    std::vector<std::vector<std::string>> usage;
    usage.push_back({"-s","filename","source file to decode and apply effects"});
    usage.push_back({"-bs","filename","bitstream file that contains all needed data to decode source file (if it is not indicated then decoding will use \"buried\" bitstream)"});
    usage.push_back({"-ds","filename","file name where decoded source file will be created"});
    usage.push_back({"-o","filename","file name where output file (including effects) will be created"});
    usage.push_back({"-ut","upmix type\t","0: normal (by default if it is not indicated)\t\t1: blind\t\t2: binaural\t\t3: stereo"});
    usage.push_back({"-dt","decoding type","0: low (by default if it is not indicated)\t\t1: high"});
    usage.push_back({"-bq","binaural quality","0: parametric (by defualt if it is not indicated)\t1: filtering"});
    usage.push_back({"-hm","HRTF model\t","0: kemar (by default if it is not indicated)\t\t1: vast\t\t2: MPS VT"});
    usage.push_back({"-fx","filename","effects parameters file"});
    usage.push_back({"-h","","it shows usage help"});
    for (int row = 0; row < usage.size(); row++) {
        for (int column = 0; column < usage[0].size(); column++) {
            message += usage[row][column] + "\t";
        }
        message += "\n";
    }
    // Example
    message += "\nEXAMPLE\n";
    message += app + " -s Folder/source.wav -bs Folder/bitstream.bs -ds Folder/input.wav -o Folder/output.wav -dt 1 -ut 3 -hm 0 -fx Folder/effects.fx\n";
    // Printing
    message += "\n";
    std::cout << message;
}

int main(int argc, char *argv[]) {
    // Arguments
    std::string app = std::string(argv[0]);
    std::string source;
    std::string bitstream = "buried";
    std::string input;
    std::string output;
    int upmixtype = 0;
    int decodingtype = 0;
    int binauralquality = 0;
    int hrtfmodel = 0;
    std::string effects;
    for(int arg = 1; arg < argc; arg++) {
        if(std::strncmp(argv[arg],"-s",2)==0) {
            arg++;
            if(std::strncmp(argv[arg],"-",1)==0) {
                printHelp(app);
                return 1;
            } else {
                source = argv[arg];
            }
        } else if(std::strncmp(argv[arg],"-bs",3)==0) {
            arg++;
            if(std::strncmp(argv[arg],"-",1)==0) {
                printHelp(app);
                return 1;
            } else {
                bitstream = argv[arg];
            }
        } else if(std::strncmp(argv[arg],"-ds",3)==0) {
            arg++;
            if(std::strncmp(argv[arg],"-",1)==0) {
                printHelp(app);
                return 1;
            } else {
                input = argv[arg];
            }
        } else if(std::strncmp(argv[arg],"-o",2)==0) {
            arg++;
            if(std::strncmp(argv[arg],"-",1)==0) {
                printHelp(app);
                return 1;
            } else {
                output = argv[arg];
            }
        } else if(std::strncmp(argv[arg],"-ut",3)==0) {
            arg++;
            if(std::strncmp(argv[arg],"-",1)==0) {
                printHelp(app);
                return 1;
            } else {
                upmixtype = std::atoi(argv[arg]);
            }
        } else if(std::strncmp(argv[arg],"-dt",3)==0) {
            arg++;
            if(std::strncmp(argv[arg],"-",1)==0) {
                printHelp(app);
                return 1;
            } else {
                decodingtype = std::atoi(argv[arg]);
            }
        } else if(std::strncmp(argv[arg],"-bq",3)==0) {
            arg++;
            if(std::strncmp(argv[arg],"-",1)==0) {
                printHelp(app);
                return 1;
            } else {
                binauralquality = std::atoi(argv[arg]);
            }
        } else if(std::strncmp(argv[arg],"-hm",3)==0) {
            arg++;
            if(std::strncmp(argv[arg],"-",1)==0) {
                printHelp(app);
                return 1;
            } else {
                hrtfmodel = std::atoi(argv[arg]);
            }
        } else if(std::strncmp(argv[arg],"-fx",3)==0) {
            arg++;
            if(std::strncmp(argv[arg],"-",1)==0) {
                printHelp(app);
                return 1;
            } else {
                effects = argv[arg];
            }
        } else if(std::strncmp(argv[arg],"-h",2)==0) {
            printHelp(app);
            return 0;
        }
    }
    std::string message = "\nPARAMETERS\n";
    message += "Source:\t\t" + source + "\n";
    message += "Bitstream:\t\t" + bitstream + "\n";
    message += "Decoded source:\t" + input + "\n";
    message += "Output:\t\t" + output + "\n";
    message += "Upmix type:\t\t" + std::to_string(upmixtype) + " ";
    switch (upmixtype) {
    case 0:
        message += "normal";
        break;
    case 1:
        message += "blind";
        break;
    case 2:
        message += "binaural";
        break;
    case 3:
        message += "stereo";
        break;
    }
    message += "\n";
    message += "Decoding type:\t" + std::to_string(decodingtype) + " ";
    switch (decodingtype) {
    case 0:
        message += "low";
        break;
    case 1:
        message += "high";
        break;
    }
    message += "\n";
    message += "Binaural quality:\t" + std::to_string(binauralquality) + " ";
    switch (binauralquality) {
    case 0:
        message += "parametric";
        break;
    case 1:
        message += "filtering";
        break;
    }
    message += "\n";
    message += "HRTF model:\t\t" + std::to_string(hrtfmodel) + " ";
    switch (hrtfmodel) {
    case 0:
        message += "kemar";
        break;
    case 1:
        message += "vast";
        break;
    case 2:
        message += "MPS VT";
        break;
    }
    message += "\n";
    message += "Effects:\t\t" + effects + "\n";
    std::cout << message << "\n";
    // Application
    consolelog("main", LogType::progress, "running application");
    ProcessManager *process = new ProcessManager(44100,0);
    // Decoding
    process->decode(source,bitstream,input,upmixtype,decodingtype,binauralquality,hrtfmodel);
    // Effects
    process->setOutput(output);
    Effect effect = Effect();
    std::vector<bool> channels = std::vector<bool>(process->channels,false);
    process->applyEffect(channels, effect);
    // End
    consolelog("main", LogType::progress, "application was completed");
    return 0;
}
