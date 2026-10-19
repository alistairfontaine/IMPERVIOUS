#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace Impervious {

    struct EngineMetrics {
        size_t initialBytes = 0;
        size_t compressedBytes = 0;
    };

    // The data stripping core module
    void executeDataStrip(const std::string& sourcePath, const std::string& targetPath) {
        std::ifstream srcFile(sourcePath, std::ios::binary);
        std::ofstream trgFile(targetPath, std::ios::binary);

        if (!srcFile.is_open() || !trgFile.is_open()) {
            std::cerr << "[CRITICAL] File stream operational failure." << std::endl;
            return;
        }

        EngineMetrics metrics;
        std::string textLine;

        std::cout << "[SYSTEM] Initializing IMPERVIOUS engine pipeline..." << std::endl;

        while (std::getline(srcFile, textLine)) {
            metrics.initialBytes += textLine.length() + 1;

            std::string isolatedPayload = "";
            bool flagInsideTag = false;

            // First-principles string parsing routine
            for (char character : textLine) {
                if (character == '<') {
                    flagInsideTag = true;
                    continue;
                }
                if (character == '>') {
                    flagInsideTag = false;
                    continue;
                }
                if (!flagInsideTag) {
                    isolatedPayload += character;
                }
            }

            if (!isolatedPayload.empty()) {
                trgFile << isolatedPayload << "\n";
                metrics.compressedBytes += isolatedPayload.length() + 1;
            }
        }

        srcFile.close();
        trgFile.close();

        size_t absoluteSavings = metrics.initialBytes - metrics.compressedBytes;
        double compressionRatio = (static_cast<double>(absoluteSavings) / metrics.initialBytes) * 100.0;

        std::cout << "\n==========================================" << std::endl;
        std::cout << "            IMPERVIOUS METRICS            " << std::endl;
        std::cout << "==========================================" << std::endl;
        std::cout << " Raw Input Stream : " << metrics.initialBytes << " bytes" << std::endl;
        std::cout << " Compressed Text  : " << metrics.compressedBytes << " bytes" << std::endl;
        std::cout << " Data Conserved   : " << absoluteSavings << " bytes" << std::endl;
        std::cout << " Network Relief   : " << compressionRatio << "% data reduction" << std::endl;
        std::cout << "==========================================" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: ./impervious_engine <input_file> <output_file.imp>" << std::endl;
        return 1;
    }

    std::string inputSource = argv[1];
    std::string outputDestination = argv[2];

    Impervious::executeDataStrip(inputSource, outputDestination);

    return 0;
}
