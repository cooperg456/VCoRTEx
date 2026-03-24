# VoRTEx
VoRTEx (Virtual Cosmic Ray Tracker Experiment, or Virtual CoRTEx) is a simulation engine, built on top of Geant4, for the Pitt-CoRTEx muon detector. It serves multiple purposes:

1)	To accurately model the generation and transportation of photons inside the scintillator bars to the SiPMs

2)	To interface with other software systems that run on the detector (GNN, fitting, data logging), for development and testing

3)	To serve as an educational tool that provides insight into CoRTEx's inner workings

### Usage

```
% ./vcortex_sim -h         
VoRTEx — Virtual Cosmic Ray Tracker Experiment


./vcortex_sim [OPTIONS] [macro]


POSITIONALS:
  macro TEXT                  Geant4 macro file (interactive if not given)

OPTIONS:
  -h,     --help              Print this help message and exit
  -o,     --output TEXT [output.csv]  
                              Output filename
  -t,     --threads INT:POSITIVE [1]  
                              Number of worker threads
  -p,     --position FLOAT [[50,50,50]]  x 3 
                              Gun position in mm (x y z)
  -d,     --direction FLOAT [[-1,-1,-1]]  x 3 
                              Gun momentum direction (x y z)
  -e,     --energy FLOAT [1000.000000]  
                              Particle energy in MeV
  -P,     --particle TEXT [mu-]  
                              Particle type (e.g. mu-, e-, proton)
```