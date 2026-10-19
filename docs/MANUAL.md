# IMPERVIOUS User and System Manual

This technical manual details operational parameters for the native IMPERVIOUS binary workspace.

## Module 1: Document Compression & Tag Clearance
The data engine processes input files line-by-line, tracking opening and closing operators via state flags.

### Operational Arguments
```bash
./impervious_engine <path_to_source_file> <target_destination_file.imp>
```

### Metrics Output Breakdown
Upon a successful execution path, the engine dumps the following operational analytics directly to `std::cout`:
* **Raw Input Stream:** Total footprint of data payload ingested (in bytes).
* **Compressed Text:** Total structural footprint after processing blocks have executed.
* **Data Conserved:** The exact delta metric saved on the host communication layer.
* **Network Relief:** Percentage reduction showing resource optimization.

## Error Manifest
* `[CRITICAL] File stream operational failure.` -> Check folder paths or verify storage partition mount permissions on the target directory drive.
