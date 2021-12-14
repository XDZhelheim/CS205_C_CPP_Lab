#ifdef NDEBUG
#define assert(condition) ((void)0)
#else
#define assert(condition) /*implementation defined*/
#endif

#include "input_parser.hpp"
#include "cnn.hpp"

int main(int argc, char const* argv[]) {
    InputParser parser(argc, argv);

    if (parser.cmdOptionExists("-h")) {
        cout << "-f\tFile path." << endl;
        cout << "-p\tPrint the result as percentage." << endl;
        return 0;
    }

    bool p_flag = false;
    if (parser.cmdOptionExists("-p")) {
        p_flag = true;
    }

    const std::string& filename = parser.getCmdOption("-f");
    if (!filename.empty()) {
        CNN::face_detection(filename.c_str(), p_flag);
    } else {
        cout << "No input file. Aborting..." << endl;
    }

    return 0;
}
