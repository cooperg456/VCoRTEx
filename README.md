# VoRTEx
VoRTEx (Virtual Cosmic Ray Tracker Experiment, or Virtual CoRTEx) is a simulation engine, built on top of Geant4, for the Pitt-CoRTEx muon detector. It serves multiple purposes:

1)	To accurately model the generation and transportation of photons inside the scintillator bars to the SiPMs

2)	To interface with other software systems that run on the detector (GNN, fitting, data logging), for development and testing

3)	To serve as an educational tool that provides insight into CoRTEx's inner workings

## Usage

Functionality is provided through CLI interface, as well as Geant4 macro files

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

## Output (CSV)

The Geant4 sim outputs data for each gamma that reaches a SiPM. An example is provided below

```
x(mm),      y(mm),     z(mm),  energy(eV),  time(ns)
 0.0819763,  0.292927,  170.0,  2.42349,     19.0598
-0.4035620,  0.270916,  170.0,  2.56231,     20.2405
 0.1412000,  0.461897,  170.0,  2.50305,     10.9907
-0.0289133,  0.379356,  170.0,  2.47859,     9.42182
-0.3030230,  0.081439,  170.0,  2.48985,     18.0228
 0.3915990, -0.286169,  170.0,  2.48326,     6.64631
 0.1269430,  0.264399,  170.0,  2.55510,    44.4584
-0.0858036, -0.301851,  170.0,  2.55510,     5.43389
 0.0385735,  0.253617,  170.0,  2.60354,    44.6815
-0.3554940, -0.230052,  170.0,  2.58869,     6.58332
...
```
