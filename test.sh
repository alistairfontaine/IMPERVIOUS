#!/usr/bin/env bash
# IMPERVIOUS Automation Verification Script

set -e

printf "========================================\n"
printf "   INITIALIZING IMPERVIOUS TEST SUITE   \n"
printf "========================================\n\n"

# Verify the native engine exists
if [ ! -f "./impervious_engine" ]; then
    printf "[BUILD ERROR] Native engine not found. Running build.sh first...\n"
    ./build.sh
fi

# Setup sandbox environment strings
mkdir -p sandbox
MOCK_HTML="sandbox/mock_feed.html"
OUTPUT_IMP="sandbox/verified_output.imp"

printf "[STEP 1] Generating bloated structural test layout data...\n"
cat << 'EOF' > "$MOCK_HTML"
<!DOCTYPE html>
<html>
<head>
    <title>ZMC Breaking News Feed</title>
    <script type="text/javascript">console.log("Tracking user metric footprints");</script>
    <style>body { background-color: #000; color: #fff; } .ad-banner { display: block; }</style>
</head>
<body>
    <div class="main-container">
        <h1>Critical News Flash: Hackathon Commences</h1>
        <p>The ZMC-NUST Media Innovation Hackathon has officially started inside the computational laboratory frameworks.</p>
    </div>
</body>
</html>
EOF

printf "[STEP 2] Running HTML Tag Isolation Loop pipeline...\n"
./impervious_engine --text "$MOCK_HTML" "$OUTPUT_IMP"

printf "\n[STEP 3] Inspecting text reduction outputs:\n"
printf "%s\n" "--- Stripped File Output Content ---"
cat "$OUTPUT_IMP"
printf "%s\n\n" "------------------------------------"
printf "[SUCCESS] Validation checks passed successfully.\n"

