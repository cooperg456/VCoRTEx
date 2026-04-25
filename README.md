# VCoRTEx
VCoRTEx (Virtual Cosmic Ray Tracker Experiment, or Virtual CoRTEx) is a simulation engine, built on top of Geant4, for the Pitt-CoRTEx muon detector. It serves multiple purposes:

1)	To accurately model the generation and transport of photons inside the scintillator bars to the SiPMs

2)	To interface with other software systems running on the detector (triggering, track fitting, data logging, etc.), for development and testing

3)	To serve as an educational tool that provides insight into the Pitt-CoRTEx detector's inner workings

This project was developed as part of the Pitt-CoRTEx research group under Dr. Pranava Teja Surukuchi and Dr. Tae Min Hong, and as a final project for PHYS 1378 Introduction to Nuclear and Particle Physics (Spring 2026) at the University of Pittsburgh.

## Installation

VCoRTEx is built using [CMake](https://cmake.org/download/) and requires both [Geant4](https://geant4.web.cern.ch/download/) and [ROOT](https://root.cern/install/). After installing these dependencies, proceed with the build

```zsh
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/geant4-install -DCMAKE_INSTALL_PREFIX=/path/to/vortex-install
make -jN
make install
```

where N is the number of system threads.



## Usage

VCoRTEx can be ran in interactive mode, by not specifying a macro file
```zsh
./vcortex_sim c
```
or in batch mode
```zsh
./vcortex_sim c batch.mac
```
Where `c` is the configuration number (c=0 for full detector stack, c=1 for a single bar)



## Output
The VCoRTEx Geant4 application write photon hits to `vcortex_output.root` in the following format, with one TTree per run.

| Item      | Description                  | Usage    |
| :-------- | :--------------------------- | :------- |
| x_mm      | x position on SiPM face (mm) | N/A      |
| y_mm      | y position on SiPM face (mm) | N/A      |
| energy_eV | photon energy (eV)           | SiPM sim |
| time_ns   | time after event start (ns)  | N/A      |
| detID     | SiPM number (copy #)         | SiPM sim |
| eventID   | event number (particle #)    | SiPM sim |

## Python

Some examples python scripts are included for simulation of SiPM response, and track fitting (WIP)



## Documentation
- [Project Proposal](docs/PHYS_1378_PROPOSAL.pdf)
- [Presentation Slides](docs/PHYS_1378_PRESENTATION.pdf)



## Notes on Implementation

The NVIDIA OptiX/Opticks optical photon transport detailed in the proposal and presentation slides is not included in this repository due to build complexity. The simulation defaults to standard multithreaded Geant4 optical physics. Contact the author for details on the full implementation.



## License

[MIT](https://choosealicense.com/licenses/mit/)
