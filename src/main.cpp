#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

namespace Impervious {

    struct TextMetrics {
        size_t initialBytes = 0;
        size_t compressedBytes = 0;
    };

    struct BinaryMetrics {
        size_t inputSize = 0;
        size_t outputSize = 0;
        size_t metadataBytesStripped = 0;
    };

    // Subsystem A: Linear DOM Node Tag Isolation Loop (HTML Cleaner)
    void executeDataStrip(const std::string& sourcePath, const std::string& targetPath) {
        std::ifstream srcFile(sourcePath, std::ios::binary);
        std::ofstream trgFile(targetPath, std::ios::binary);

        if (!srcFile.is_open() || !trgFile.is_open()) {
            std::cerr << "[CRITICAL] HTML Engine stream operational failure." << std::endl;
            return;
        }

        TextMetrics metrics;
        std::string textLine;

        std::cout << "[SYSTEM] Activating IMPERVIOUS HTML Data-Stripping Core..." << std::endl;

        bool flagInsideTag = false;
        bool flagBlockScript = false;
        bool flagBlockStyle = false;
        std::string accumulatedTag = "";

        while (std::getline(srcFile, textLine)) {
            metrics.initialBytes += textLine.length() + 1;
            std::string isolatedPayload = "";

            for (size_t i = 0; i < textLine.length(); ++i) {
                char character = textLine[i];

                if (character == '<') {
                    flagInsideTag = true;
                    accumulatedTag = "";
                    continue;
                }
                if (character == '>') {
                    flagInsideTag = false;

                    // Evaluate structural telemetry and styling block wrappers
                    if (accumulatedTag.find("script") != std::string::npos) {
                        if (accumulatedTag[0] == '/') flagBlockScript = false; // Closed </script>
                        else flagBlockScript = true; // Opened <script>
                    }
                    else if (accumulatedTag.find("style") != std::string::npos) {
                        if (accumulatedTag[0] == '/') flagBlockStyle = false; // Closed </style>
                        else flagBlockStyle = true; // Opened <style>
                    }
                    continue;
                }

                if (flagInsideTag) {
                    accumulatedTag += character;
                } else {
                    // Only preserve text outside tags, script contents, and style arrays
                    if (!flagBlockScript && !flagBlockStyle) {
                        isolatedPayload += character;
                    }
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
        double compressionRatio = metrics.initialBytes > 0 ? (static_cast<double>(absoluteSavings) / metrics.initialBytes) * 100.0 : 0.0;

        std::cout << "\n==========================================" << std::endl;
        std::cout << "        IMPERVIOUS COMPRESSION ENGINE     " << std::endl;
        std::cout << "==========================================" << std::endl;
        std::cout << " Raw Input Stream : " << metrics.initialBytes << " bytes" << std::endl;
        std::cout << " Compressed Text  : " << metrics.compressedBytes << " bytes" << std::endl;
        std::cout << " Data Conserved   : " << absoluteSavings << " bytes" << std::endl;
        std::cout << " Network Relief   : " << compressionRatio << "% data reduction" << std::endl;
        std::cout << "==========================================" << std::endl;
    }

    // Subsystem B: Byte-Pattern Header Array Matcher (Binary Sanitizer)
    void sanitizeBinaryStream(const std::string& inputMedia, const std::string& outputMedia) {
        std::ifstream srcBin(inputMedia, std::ios::binary | std::ios::ate);
        if (!srcBin.is_open()) {
            std::cerr << "[CRITICAL] Binary Engine failed to read input source asset." << std::endl;
            return;
        }

        std::streamsize assetSize = srcBin.tellg();
        srcBin.seekg(0, std::ios::beg);

        std::vector<uint8_t> byteBuffer(static_cast<size_t>(assetSize));
        if (!srcBin.read(reinterpret_cast<char*>(byteBuffer.data()), assetSize)) {
            std::cerr << "[CRITICAL] Buffer allocation fault during binary ingestion loop." << std::endl;
            return;
        }
        srcBin.close();

        std::ofstream trgBin(outputMedia, std::ios::binary);
        if (!trgBin.is_open()) {
            std::cerr << "[CRITICAL] Binary Engine failed to mount target destination write stream." << std::endl;
            return;
        }

        BinaryMetrics binMetrics;
        binMetrics.inputSize = byteBuffer.size();

        std::cout << "[SYSTEM] Activating Byte-Pattern Header Scanner..." << std::endl;

        size_t cursor = 0;
        bool exifMarkerFound = false;

        // Verify standard JPEG Start of Image (SOI) marker: 0xFFD8
        if (binMetrics.inputSize > 2 && byteBuffer[0] == 0xFF && byteBuffer[1] == 0xD8) {
            trgBin.put(byteBuffer[0]);
            trgBin.put(byteBuffer[1]);
            cursor = 2;

            // Single-pass linear byte pattern verification scan
            while (cursor < binMetrics.inputSize - 1) {
                if (byteBuffer[cursor] == 0xFF) {
                    uint8_t markerType = byteBuffer[cursor + 1];

                    // Identify EXIF APP1 Segment boundary: 0xFFE1
                    if (markerType == 0xFFE1) {
                        exifMarkerFound = true;
                        size_t segmentLength = (byteBuffer[cursor + 2] << 8) | byteBuffer[cursor + 3];

                        std::cout << "[MATCH] Discovered EXIF metadata segment: " << segmentLength + 2 << " bytes." << std::endl;

                        binMetrics.metadataBytesStripped += (segmentLength + 2);
                        cursor += (segmentLength + 2);
                        continue;
                    }

                    if (markerType == 0xFFDA) { // SOS - Image data boundary
                        trgBin.put(byteBuffer[cursor]);
                        trgBin.put(byteBuffer[cursor + 1]);
                        cursor += 2;
                        break;
                    }

                }

                trgBin.put(byteBuffer[cursor]);
                cursor++;
            }
        }

        // Flush remaining payload blocks straight to target file buffer
        while (cursor < binMetrics.inputSize) {
            trgBin.put(byteBuffer[cursor]);
            cursor++;
        }

        trgBin.close();
        binMetrics.outputSize = binMetrics.inputSize - binMetrics.metadataBytesStripped;

        std::cout << "\n==========================================" << std::endl;
        std::cout << "        IMPERVIOUS BINARY SANITIZER       " << std::endl;
        std::cout << "==========================================" << std::endl;
        std::cout << " Ingested Media   : " << binMetrics.inputSize << " bytes" << std::endl;
        std::cout << " Cleansed Asset   : " << binMetrics.outputSize << " bytes" << std::endl;
        std::cout << " Identity Bloat   : " << binMetrics.metadataBytesStripped << " bytes purged" << std::endl;
        std::cout << " Status Flag      : " << (exifMarkerFound ? "SECURELY STRIPPED" : "CLEAN / NO EXIF DETECTED") << std::endl;
        std::cout << "==========================================" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "IMPERVIOUS Native Engine Framework\n" << std::endl;
        std::cout << "Usage Options:" << std::endl;
        std::cout << "  Text Extractor: ./impervious_engine --text <input_html> <output.imp>" << std::endl;
        std::cout << "  Media Purger  : ./impervious_engine --media <input_jpeg> <output_cleansed.jpg>\n" << std::endl;
        return 1;
    }

    std::string operationalMode = argv[1];
    std::string inputSource = argv[2];
    std::string outputDestination = argv[3];

    if (operationalMode == "--text") {
        Impervious::executeDataStrip(inputSource, outputDestination);
    } else if (operationalMode == "--media") {
        Impervious::sanitizeBinaryStream(inputSource, outputDestination);
    } else {
        std::cerr << "[ERROR] Invalid execution operational flag sequence provided." << std::endl;
        return 1;
    }

    return 0;
}

