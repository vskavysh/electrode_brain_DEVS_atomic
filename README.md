# Electrode–Brain DEVS model (Cadmium v2) — project + experiments

## 1) Prerequisites
- Cadmium v2 installed locally (dev-rt branch); see the following for instructions: https://devssim.carleton.ca/manuals/installation/Manual.pdf
- Environment variable `CADMIUM` set to the Cadmium root directory (.../cadmium_v2/include)

## 2) Build
From the project root:
```bash
./build_sim.sh
```
The executables will be placed in the `/bin/` folder 

## 3) Run experiments
From the bin/ folder:
```bash
./test_background_noise
./test_neuron
./test_adder
./test_electrode
./test_top
./electrode_brain_sim
```

Each experiment writes CSV logs (in `/bin/`):
- `noise_sampling_log.csv`
- `neuron_always_fire_log.csv`, `neuron_never_fire_log.csv`, `neuron_default_log.csv`
- `adder_simple_log.csv`
- `electrode_threshold_crossing_log.csv`, `electrode_high_threshold_log.csv`
- `top_threshold_45_log.csv`, `top_threshold_55_log.csv`
- `electrode_brain_log.csv`

These log files are analyzed in analyze_logs.ipynb (located in (`/analysis/`), which generates various plots and other data.

## 4) Report

The model, the experimentation framework, and the results are detailed in `electrode_brain_model_report.pdf`.
