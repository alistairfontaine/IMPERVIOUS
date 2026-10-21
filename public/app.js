// Local Language Localization Registries for Track 7 Verification Passes
const localization = {
    en: {
        modATitle: "1. Low-Bandwidth Text Isolation",
        modADesc: "Strip structural document bloat, tracking scripts, and telemetry inline.",
        btnProcess: "Execute Compliance Filter",
        metricsHeader: "AUDIT METRICS:",
        privacyNotice: "*Processed entirely in browser memory. Zero network logging.",
        modBTitle: "2. Binary Source Anonymizer",
        modBDesc: "Drop EXIF parameters, device profiles, and GPS coordinates from whistleblower drops.",
        uploadPrompt: "Drop leak asset or click to browse"
    },
    sn: {
        modATitle: "1. Kuvandudza Mashoko Akaderera",
        modADesc: "Bvisa tsvina dzose dzinotevera mashoko nemakodhi anorema mukati megwaro.",
        btnProcess: "Shandisa Sefa Yemitemo",
        metricsHeader: "ZVINGANISO ZVEKUSHANDA:",
        privacyNotice: "*Zvagadziriswa mukati memusi wefoni yako. Hapana chinotumirwa paIndaneti.",
        modBTitle: "2. Kuchengetedza Kwakavanzika Kwevaburitsi Mashoko",
        modBDesc: "Bvisa ruzivo rwemidziyo nemamepu (EXIF/GPS) kubva pamifananidzo inodonhedzwa.",
        uploadPrompt: "Madonhedza mufananidzo kana baya kuti utsvage"
    },
    nd: {
        modATitle: "1. Ukuhlunga Umbhalo Olowu-Bandwidth",
        modADesc: "Susa amakhodi asindayo a-tracking le-telemetry phakathi kombhalo.",
        btnProcess: "Sebenzisa Isihlungi Somthetho",
        metricsHeader: "IMIKHOMBANDLELA YOKUSEBENZA:",
        privacyNotice: "*Kugujulwe phakathi kwememori yefoni yakho. Akukho okuthunyelwa emoyeni.",
        modBTitle: "2. Ukuvikela Imfihlo Yababiki Bezindaba",
        modBDesc: "Susa imininingwane yamathuluzi lendawo (EXIF/GPS) emifanekisweni ethunyelwayo.",
        uploadPrompt: "Phonsa umfanekiso loba chofoza ukuze ucinge"
    }
};

// Defensive DOM helper — logs instead of crashing if a node is missing
function setText(id, text) {
    const el = document.getElementById(id);
    if (el) {
        el.innerText = text;
    } else {
        console.error(`[IMPERVIOUS] Missing DOM node: #${id}`);
    }
}

function setLanguage(lang) {
    ['en', 'sn', 'nd'].forEach(l => {
        const btn = document.getElementById(`lang-${l}`);
        if (!btn) return;
        if (l === lang) {
            btn.classList.add('bg-zinc-800', 'text-white');
            btn.classList.remove('bg-zinc-900', 'text-zinc-400');
        } else {
            btn.classList.remove('bg-zinc-800', 'text-white');
            btn.classList.add('bg-zinc-900', 'text-zinc-400');
        }
    });

    const data = localization[lang];
    if (!data) return;
    setText('mod-a-title', data.modATitle);
    setText('mod-a-desc', data.modADesc);
    setText('btn-process-text', data.btnProcess);
    setText('metrics-header', data.metricsHeader);
    setText('privacy-notice', data.privacyNotice);
    setText('mod-b-title', data.modBTitle);
    setText('mod-b-desc', data.modBDesc);
    setText('upload-prompt', data.uploadPrompt);
}

// Subsystem A execution logic
function processWebText() {
    const inputEl = document.getElementById('text-input');
    if (!inputEl) return;
    const rawInput = inputEl.value;
    if (!rawInput) return;

    // Linear parsing emulation mimicking the native execution model
    let cleanText = rawInput.replace(/<script[^>]*>([\s\S]*?)<\/script>/gi, '');
    cleanText = cleanText.replace(/<style[^>]*>([\s\S]*?)<\/style>/gi, '');
    cleanText = cleanText.replace(/<[^>]+>/g, '\n');
    cleanText = cleanText.split('\n').map(line => line.trim()).filter(line => line.length > 0).join('\n');

    const originalBytes = new Blob([rawInput]).size;
    const cleanBytes = new Blob([cleanText]).size;
    const bytesSaved = originalBytes - cleanBytes;
    const reductionPct = originalBytes > 0 ? ((bytesSaved / originalBytes) * 100).toFixed(4) : 0;

    setText('raw-size', originalBytes);
    setText('clean-size', cleanBytes);
    setText('saved-size', bytesSaved);
    setText('relief-pct', reductionPct);

    const metrics = document.getElementById('text-metrics');
    if (metrics) metrics.classList.remove('hidden');
}

// --- Native byte-layer sanitizer (mirrors the C++ Subsystem B logic) ---

function stripJpegExif(bytes) {
    // JPEG must begin with SOI marker 0xFFD8
    if (bytes.length < 4 || bytes[0] !== 0xFF || bytes[1] !== 0xD8) {
        return { data: bytes, stripped: 0 };
    }
    const out = [bytes[0], bytes[1]];
    let stripped = 0;
    let pos = 2;
    while (pos < bytes.length - 1) {
        if (bytes[pos] !== 0xFF) { out.push(bytes[pos]); pos++; continue; }
        const marker = bytes[pos + 1];
        // Standalone markers (RST0-RST7, TEM) carry no length field
        if (marker === 0x01 || (marker >= 0xD0 && marker <= 0xD8)) {
            out.push(bytes[pos], bytes[pos + 1]); pos += 2; continue;
        }
        if (pos + 3 >= bytes.length) { out.push(bytes[pos]); pos++; continue; }
        const segLen = (bytes[pos + 2] << 8) | bytes[pos + 3];
        if (marker === 0xE1) {          // APP1: EXIF / XMP identity payload
            stripped += segLen + 2;
            pos += segLen + 2;
            continue;
        }
        if (marker === 0xDA) {          // SOS: image data begins, flush rest verbatim
            for (let i = pos; i < bytes.length; i++) out.push(bytes[i]);
            return { data: new Uint8Array(out), stripped };
        }
        for (let i = 0; i < segLen + 2; i++) out.push(bytes[pos + i]);
        pos += segLen + 2;
    }
    return { data: new Uint8Array(out), stripped };
}

function stripPngExif(bytes) {
    // PNG signature: 89 50 4E 47 0D 0A 1A 0A
    const sig = [0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A];
    if (bytes.length < 8 || sig.some((b, i) => bytes[i] !== b)) {
        return { data: bytes, stripped: 0 };
    }
    const out = Array.from(bytes.slice(0, 8));
    let stripped = 0;
    let pos = 8;
    while (pos + 8 <= bytes.length) {
        const len = (bytes[pos] << 24 | bytes[pos+1] << 16 | bytes[pos+2] << 8 | bytes[pos+3]) >>> 0;
        const type = String.fromCharCode(bytes[pos+4], bytes[pos+5], bytes[pos+6], bytes[pos+7]);
        const chunk = bytes.slice(pos, pos + 12 + len);
        if (type === 'eXIf') { stripped += chunk.length; }
        else { out.push(...chunk); }
        pos += 12 + len;
    }
    return { data: new Uint8Array(out), stripped };
}

// Subsystem B execution logic — REAL binary sanitizer
async function processMediaAsset(input) {
    if (!input || !input.files || !input.files[0]) return;

    const file = input.files[0];
    setText('web-bin-name', file.name);
    setText('web-bin-size', `${file.size} bytes`);
    setText('media-flag', 'SCANNING...');

    const metrics = document.getElementById('media-metrics');
    if (metrics) metrics.classList.remove('hidden');

    try {
        const buffer = new Uint8Array(await file.arrayBuffer());
        const isPng = file.type === 'image/png' ||
            (buffer.length > 4 && buffer[1] === 0x50 && buffer[2] === 0x4E && buffer[3] === 0x47);

        const result = isPng ? stripPngExif(buffer) : stripJpegExif(buffer);
        const outSize = result.data.length;
        const purged = result.stripped;

        setText('web-bin-stripped', `${purged} (output: ${outSize} bytes)`);
        setText('media-flag', purged > 0 ? 'SECURELY STRIPPED' : 'CLEAN / NO EXIF DETECTED');

        const link = document.getElementById('media-download');
        if (link) {
            const ext = isPng ? 'png' : 'jpg';
            const blob = new Blob([result.data], { type: isPng ? 'image/png' : 'image/jpeg' });
            if (window._cleansedUrl) URL.revokeObjectURL(window._cleansedUrl);
            window._cleansedUrl = URL.createObjectURL(blob);
            link.href = window._cleansedUrl;
            link.download = `cleansed_${file.name.replace(/\.[^.]+$/, '')}.${ext}`;
            link.classList.remove('hidden');
        }
    } catch (err) {
        console.error('[IMPERVIOUS] Sanitizer fault:', err);
        setText('media-flag', 'ENGINE FAULT');
    }
}
