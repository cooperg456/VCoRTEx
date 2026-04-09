# VoRTEx
VoRTEx (Virtual Cosmic Ray Tracker Experiment, or Virtual CoRTEx) is a simulation engine, built on top of Geant4, for the Pitt-CoRTEx muon detector. It serves multiple purposes:

1)	To accurately model the generation and transport of photons inside the scintillator bars to the SiPMs

2)	To interface with other software systems running on the detector (triggering, track fitting, data logging, etc.), for development and testing

3)	To serve as an educational tool that provides insight into the Pitt-CoRTEx detector's inner workings



## Installation

VoRTEx is built using [CMake](https://cmake.org/download/) and requires both [Geant4](https://geant4.web.cern.ch/download/) and [ROOT](https://root.cern/install/). After installing these dependencies, proceed with the build

```zsh
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/geant4-install -DCMAKE_INSTALL_PREFIX=/path/to/vortex-install
make -jN
make install
```

where N is the number of threads.



## Usage

VoRTEx can be ran in interactive mode, by not specifying a macro file
```zsh
./vcortex_sim
```
or in batch mode
```zsh
./vcortex_sim batch.mac
```



## License

[MIT](https://choosealicense.com/licenses/mit/)