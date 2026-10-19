#!/usr/bin/env bash
# IMPERVIOUS Automation Verification Script

set -e

printf "========================================\n"
printf "   INITIALIZING IMPERVIOUS TEST SUITE   \n"
printf "========================================\n\n"

# 1. Native Binary Validation Pass
if [ ! -f "./impervious_engine" ]; then
    printf "[BUILD ERROR] Native engine binary not found. Executing build script...\n"
    ./build.sh
fi

# Establish path environments
mkdir -p sandbox
MOCK_HTML="sandbox/mock_feed.html"
OUTPUT_IMP="sandbox/verified_output.imp"
TEST_IMAGE="sandbox/my_photo.jpg"
CLEANSE_IMAGE="sandbox/cleansed_photo.jpg"

# 2. Subsystem A Setup: HTML Compression Core
printf "[STEP 1] Building HTML structural test data...\n"
cat << 'EOF' > "$MOCK_HTML"
<!DOCTYPE html>
<html>
<head>
    <title>ZMC Breaking News Feed</title>
    <script type="text/javascript">console.log("Tracker active");</script>
</head>
<body>
    <h1>Critical News Flash</h1>
    <p>The ZMC-NUST Media Innovation Hackathon has officially started.</p>
</body>
</html>
EOF

printf "[STEP 2] Running HTML Tag Isolation Loop pipeline...\n"
./impervious_engine --text "$MOCK_HTML" "$OUTPUT_IMP"

printf "\n[STEP 3] Verifying text extraction payload:\n"
printf "%s\n" "--- Stripped File Output Content ---"
cat "$OUTPUT_IMP"
printf "%s\n\n" "------------------------------------"

# 3. Subsystem B Setup: Binary Metadata Sanitizer
printf "[STEP 4] Scanning host backgrounds for test JPEG media assets...\n"
WALLPAPER_SRC="/usr/share/backgrounds/ubuntu-studio/ubuntu-studio-default.jpg"

if [ -f "$WALLPAPER_SRC" ]; then
    cp "$WALLPAPER_SRC" "$TEST_IMAGE"
    printf "[SYSTEM] Found stock asset. Mounted: %s\n" "$TEST_IMAGE"
else
    # Fallback: Generate a bare minimal 3-byte layout container if wallpaper path isn't active
    printf "[SYSTEM] Host background missing. Synthesizing low-level baseline stream...\n"
    printf "\xFF\xD8\x00" > "$TEST_IMAGE"
fi

printf "[STEP 5] Launching Byte-Pattern Header Scanner...\n"
./impervious_engine --media "$TEST_IMAGE" "$CLEANSE_IMAGE"

printf "\n[SUCCESS] All IMPERVIOUS system checks completed cleanly.\n"
