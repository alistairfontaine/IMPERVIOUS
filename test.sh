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
printf "[STEP 1] Building hyper-focused structural hackathon news feed...\n"
cat << 'EOF' > "$MOCK_HTML"
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>ZMC-NUST Innovation News Hub</title>
    <script type="text/javascript">
        // Tracking footprint simulation
        let trackingBeacon = "https://corporate-fluff.com";
        navigator.sendBeacon(trackingBeacon, { user_data_costs: "high" });
    </script>
    <style>
        body { font-family: monospace; background: #111; color: #ccc; }
        .expensive-ad-container { display: block; margin: 40px; border: 5px solid red; }
    </style>
</head>
<body>
    <div class="expensive-ad-container">
        <!-- Massive ad image simulated bloat -->
        <p>ADVERTISEMENT: Buy Flashy Cloud Platforms Here!</p>
    </div>

    <article class="news-content">
        <h1>BUILDING SUSTAINABLE MEDIA FOR ZIMBABWE</h1>
        <h3>Theme: Local Technology for Efficient Newsrooms, Loyal Audiences, and New Revenue</h3>

        <p><strong>EFFICIENT NEWSROOMS:</strong> Traditional media platforms are collapsing under the weight of expensive cloud licensing fees and massive processing bloat. Newsrooms need locally compiled, zero-dependency tools running on standard available desktop hardware to keep operating costs at zero.</p>

        <p><strong>LOYAL AUDIENCES:</strong> Ordinary citizens face massive data constraints. Web pages flooded with heavy layout nodes, presentation styles, and commercial ad blocks make loading essential journalism a luxury. Compressing data footprints builds deep loyalty through immediate financial relief for the consumer.</p>

        <p><strong>NEW REVENUE:</strong> Dropping transmission overhead margins to zero opens instant profitability lines. Delivering a hyper-fast, low-bandwidth, tracking-free premium feed allows media houses to monetize specialized secure channels for clients demanding data conservation.</p>
    </article>
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
