#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <cstddef>
#include <string>
#include <tclap/CmdLine.h>

enum Algorithms {
    VerticalBorders,
    HorizontalBorders,
    Detail,
};

struct Config {
    Algorithms algorithm;
    bool display;
    std::string filename;
};


/******************************************************************************/
/*                          parse cmd line arguments                          */
/******************************************************************************/

class SizeConstraint : public TCLAP::Constraint<size_t> {
 public:
  [[nodiscard]] std::string description() const override {
    return "Positive non null";
  }
  [[nodiscard]] std::string shortID() const override {
    return "NonNullInteger";
  }
  [[nodiscard]] bool check(size_t const &value) const override {
    return value > 0;
  }
};

inline void parseCmdArgs(int argc, char **argv, Config &config) {
    // Parse the command line arguments
    try {
        TCLAP::CmdLine cmd("hhimg parameters", ' ', "0.1");
        SizeConstraint sc;
        TCLAP::ValueArg<std::string> algoArg("a", "algo", "Algorithm.", false, "detail", "string");
        cmd.add(algoArg);
        TCLAP::ValueArg<bool> displayArg("d", "display", "Display the image.", false, false, "bool");
        cmd.add(displayArg);
        TCLAP::ValueArg<std::string> filenameArg("i", "image", "Image path.", false, "../img/rivendell.jpg", "string");
        cmd.add(filenameArg);
        cmd.parse(argc, argv);

        if (algoArg.getValue() == "vb") {
            config.algorithm = VerticalBorders;
        } else if (algoArg.getValue() == "hb") {
            config.algorithm = HorizontalBorders;
        } else {
            config.algorithm = Detail;
        }
        config.display = displayArg.getValue();
        config.filename = filenameArg.getValue();
    } catch (TCLAP::ArgException &e)  // catch any exceptions
    { std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }
}

#endif
