# 🎥 Video-to-Face Dataset Pipeline

**Stack**: GStreamer • C++ • OpenCV • CMake • Shell scripting

> A high-throughput video-to-frame and face-extraction pipeline to accelerate ML dataset creation.

---

## 🚀 Project Overview

This project provides a two-stage pipeline for building face datasets from videos:

1. **High-throughput video-to-frame extraction** using GStreamer:  
   - Converts input videos to JPEG frames at scale, e.g., extracting 10,000+ frames in under 2 minutes.
   - Scales/resizes frames (default: 640×640) and writes them into a specified folder.

2. **Face localization & cropping** using OpenCV in C++:  
   - Detects faces in each extracted frame (≈75% precision in testing), crops them, and organizes them into per-frame folders.
   - Processes at over 90 frames per second on typical hardware, automating dataset folder structures for ML training workflows.

Together, these modules drastically reduce manual preprocessing time and streamline the ML data pipeline.

---

## 🎯 Key Features

- **High-throughput GStreamer pipeline**  
  Utilizes GStreamer (`gst-launch-1.0` or custom shell script) to read video files, scale to a target resolution (default 640×640), and output JPEG-encoded frames rapidly (10,000+ frames in < 2 min).

- **Face localization & extraction (OpenCV)**  
  C++ application that:
  - Loads each JPEG frame.
  - Runs a pre-trained OpenCV cascade (or DNN) for face detection (~75% precision in validation).
  - Crops detected faces and saves them into structured directories.
  - Processes at >90 FPS (frames per second), accelerating dataset preparation.

- **Automated dataset organization**  
  Creates a clean folder hierarchy:  
```

output/
├── frame\_000001/
│     ├── face\_01.jpg
│     ├── face\_02.jpg
│     └── ...
├── frame\_000002/
│     ├── face\_01.jpg
│     └── ...
└── ...

```
This structure suits many ML frameworks expecting per-sample organization.

- **Configurable & Extensible**  
- Modify target resolution, face detector model, detection thresholds, input/output paths via config or code constants.
- Extend detection logic (e.g., switch to DNN-based detectors, add landmarks or quality filtering).
- Adapt pipeline for multi-face videos, batch processing, or additional preprocessing steps.

- **Cross-platform build**  
- Shell script + GStreamer commands (Linux/macOS).  
- C++ face extractor built with CMake; tested on Windows (Visual Studio) and Linux.

---

## 📁 Repository Structure

```

.
├── 1\_stream\_to\_frames/
│   ├── stream\_to\_frames.sh     # Shell script to run GStreamer pipeline
│   ├── README.md                 # Local instructions & details
│   └── sample\_video/            # (Optional) example MP4 files for testing
│
├── 2\_CaptureFaces/
│   ├── CMakeLists.txt            # Build configuration for CaptureFaces
│   ├── src/
│   │   ├── main.cpp              # Entry point: iterate frames, detect & crop faces
│   │   ├── FaceDetector.cpp/.hpp # Wrapper around OpenCV cascade/DNN detection
│   │   ├── ImageUtils.cpp/.hpp   # Utilities for loading/saving/resizing images
│   │   └── ...                   # Other modules: folder creation, logging, config parsing
│   ├── models/                   # (Optional) pre-trained cascade XML or DNN files
│   ├── README.md                 # Local instructions for building/running
│   └── output/                   # (After run) generated cropped-face folders
│
├── LICENSE
└── README.md                     # This file

````

> **Note**: Adjust repository name/paths as needed. For clarity, this README refers to two subdirectories:  
> - `1_stream_to_frames` for extraction  
> - `2_CaptureFaces` for face detection & cropping.

---

## 🛠️ Prerequisites

- **GStreamer** (>=1.16) installed and `gst-launch-1.0` available in PATH.  
- **C++17** compiler: e.g., `g++`, `clang++`, or MSVC on Windows.  
- **CMake** (>=3.10) for building the face extraction module.  
- **OpenCV** (>=3.4) installed and findable by CMake (for face detection). Typically includes:
  - OpenCV core, imgproc, objdetect (for cascade), or dnn modules if using DNN-based detection.
- **Shell** (bash) on Linux/macOS to run the GStreamer script. On Windows, adapt script or run in WSL.

---

## 🧪 Getting Started

### 1. Video-to-Frame Extraction (GStreamer)

1. **Navigate to** `1_stream_to_frames/`:
   ```bash
   cd 1_stream_to_frames
   ```

2. **Ensure the shell script is executable**:

   ```bash
   chmod +x stream_to_frames.sh
   ```

3. **Run the script**:

   ```bash
   ./stream_to_frames.sh <path_to_video.mp4> <output_folder>
   ```

   * `<path_to_video.mp4>`: Path to your input video (e.g., `./sample_video/sample_1.mp4`).
   * `<output_folder>`: Desired name for the folder where JPEG frames will be saved (e.g., `out_1`).
   * Example:

     ```bash
     ./stream_to_frames.sh ./sample_video/sample_1.mp4 out_1
     ```
   * **Default behavior**:

     * Scales/resizes frames to 640×640.
     * Outputs JPEG-encoded frames named sequentially (e.g., `frame_000001.jpg`, `frame_000002.jpg`, …).
     * Writes them under `1_stream_to_frames/out_1/`.

4. **Customize resolution or pipeline**:

   * Edit `stream_to_frames.sh`:

     * Adjust GStreamer pipeline caps: e.g., change `videoscale ! video/x-raw,width=640,height=640` to desired resolution.
     * Add additional elements (e.g., format conversion, cropping).
   * If using Windows, adapt to a batch script or run under WSL with GStreamer installed.

5. **Verify output**:

   * After completion, you should see N JPEG frames in `1_stream_to_frames/<output_folder>/`.
   * Use any image viewer or scripts to inspect samples.

### 2. Face Localization & Cropping (OpenCV C++)

1. **Navigate to** `2_CaptureFaces/`:

   ```bash
   cd ../2_CaptureFaces
   ```

2. **Place your frames**:

   * By default, the application reads frames from a configured input directory (e.g., `../1_stream_to_frames/out_1/`).
   * You can modify the input path in:

     * A config file (if implemented), or
     * In `main.cpp` or CMake debug arguments: e.g., adjust `VS_DEBUGGER_COMMAND_ARGUMENTS` in `CMakeLists.txt`, or pass as CLI argument if supported.

3. **Ensure face detection model is available**:

   * Place pre-trained cascade XML (e.g., `haarcascade_frontalface_default.xml`) or DNN model files under `models/`.
   * Update `FaceDetector` logic or config to point to the correct model path.

4. **Build the application**:

   ```bash
   mkdir -p build && cd build
   cmake ..
   cmake --build .
   ```

   * On Windows: open generated `.sln` in Visual Studio and build.
   * On Linux/macOS: ensure OpenCV is findable by CMake (`pkg-config` or `OpenCV_DIR`).

5. **Run the face extractor**:

   ```bash
   ./CaptureFaces <input_frames_directory> <output_root_directory>
   ```

   * If the executable supports CLI args:

     * `<input_frames_directory>`: path to folder containing JPEG frames.
     * `<output_root_directory>`: path where per-frame face folders will be created.
   * If no CLI args supported, ensure the hardcoded path in `main.cpp` or debug arguments points to correct folders.
   * Example:

     ```bash
     ./CaptureFaces ../1_stream_to_frames/out_1/ ../2_CaptureFaces/output/
     ```

6. **Inspect results**:

   * After running, `output/` should contain subfolders (e.g., `frame_000001/`) with `face_01.jpg`, etc.
   * Verify precision: random-check some frames to confirm correct face crops. Tweak detection thresholds or model if needed.

---

## ⚙️ Configuration & Tuning

* **Frame extraction pipeline** (`stream_to_frames.sh`):

  * Adjust resolution: modify `videoscale` caps.
  * Change output encoding (JPEG quality) by adjusting `jpegenc` properties if needed.
  * Add timestamp overlays or metadata if desired.

* **Face detection parameters**:

  * In `FaceDetector.cpp/.hpp`, adjust detection thresholds, minimum face size, scale factor, minNeighbors for Haar cascades.
  * For DNN: adjust confidence threshold, use faster/lightweight models for higher throughput.

* **Performance considerations**:

  * GStreamer pipeline: ensure hardware acceleration if available (e.g., `decodebin ! videoconvert ! videoscale` with hardware plugins).
  * OpenCV module: enable multi-threading (e.g., process multiple frames in parallel) if input/output IO and CPU allow. Ensure thread-safe use of OpenCV detectors.
  * I/O: Writing thousands of JPEGs can be I/O-bound; consider SSD or RAM-disk for speed.

* **Logging & Metrics**:

  * Add logging (timestamps, per-frame processing time) to measure FPS and precision.
  * Optionally produce a CSV summary: total frames processed, faces detected count, average FPS, processing duration.

---

## 📈 Performance Metrics

* **Frame extraction**: >10,000 JPEG frames in under 2 minutes on typical modern hardware (\~90+ FPS equivalent for extraction and encoding).
* **Face detection & cropping**: \~75% detection precision (depending on model) with processing speed > 90 frames/sec.
* **Dataset creation speed**: From raw video to organized face folders in minutes rather than hours.

> These numbers depend on CPU, disk speed, GStreamer plugins, and OpenCV build (e.g., with Intel/OpenVINO optimizations or GPU support).

---

## 🐛 Troubleshooting

* **GStreamer errors**:

  * “No plugin found”: ensure GStreamer and required plugins (e.g., `gst-plugins-good`, `gst-plugins-bad`, `gst-libav`) are installed.
  * Permissions: ensure the shell script is executable and input file path is correct.

* **OpenCV build issues**:

  * CMake cannot find OpenCV: set `OpenCV_DIR` or install OpenCV development package.
  * Linking errors: confirm architecture (x64 vs x86) matches between OpenCV libraries and your build.

* **Face detection misses or false positives**:

  * Adjust detection thresholds (`scaleFactor`, `minNeighbors`, `minSize`).
  * Try alternate models: DNN-based detectors (e.g., OpenCV DNN with Caffe/TensorFlow models) for higher precision.
  * Preprocess frames (e.g., histogram equalization) to improve detection in low-light.

* **I/O bottlenecks**:

  * If writing thousands of JPEGs is slow, use a faster disk or run extraction and detection on separate machines and copy intermediate frames via high-speed network or RAM-disk.
  * Consider piping frames directly between GStreamer and the face extractor to avoid disk I/O (advanced integration).

---

## 🤝 Contributing

Contributions are welcome! Possible areas:

* **Enhanced detectors**: integrate DNN-based face detectors or landmark-based cropping.
* **Parallel processing**: multi-threaded or GPU-accelerated face detection.
* **Metadata generation**: produce CSV/JSON annotations (bounding boxes, detection confidence).
* **Integration**: combine extraction + detection into a single pipeline (e.g., GStreamer + OpenCV plugin).
* **Dockerization**: containerize the pipeline for reproducible environments.
* **Tests & Benchmarks**: add unit tests for detection logic; benchmark on various hardware.

To contribute:

1. Fork the repo.
2. Create a branch: `git checkout -b feature/your-feature`.
3. Implement and test your changes.
4. Submit a PR against `main`, describing improvements and any trade-offs.
5. Ensure documentation is updated accordingly.

---

## 🧾 License

This project is released under the **MIT License**. See [LICENSE](./LICENSE) for details.

---

## 🙋‍♂️ Author

**@deepencoding** – passionate about automating ML data pipelines, high-throughput video processing, and robust computer vision tools. Feel free to open issues or suggest enhancements!
