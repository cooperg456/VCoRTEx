/***************************************************************************************************************************

VoRTEx (Virtual Cosmic Ray Tracker Experiment, or Virtual CoRTEx) is a simulation engine, built on top of Geant4, for
the Pitt-CoRTEx muon detector. It serves multiple purposes:

	 1) To accurately model the generation and transportation of photons inside the scintillator bars to the SiPMs

	 2) To interface with other software systems that run on the detector (GNN, fitting, data logging), for 
		development and testing

	 3) To serve as an educational tool that provides insight into CoRTEx's inner workings

Tutorials referenced:
	https://www.youtube.com/playlist?list=PLLybgCU6QCGUYAOwtyI4U8cRL6ig0p3c7
	https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/index.html

***************************************************************************************************************************/

#include "VXPhysicsList.hpp"
#include "VXDetectorConstruction.hpp"
#include "VXActionInitialization.hpp"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "QGSP_BERT.hh"


int main(int argc, char** argv) {
	//	splash text
	std::cout << R"(
┌────────────────────────────────────────────────────────────┐
│             _    __      ____  ____________                │
│            | |  / /___  / __ \/_  __/ ____/  __            │
│            | | / / __ \/ /_/ / / / / __/ | |/_/            │
│            | |/ / /_/ / _, _/ / / / /____>  <              │
│            |___/\____/_/ |_| /_/ /_____/_/|_|              │
│                                                            │
│            VIRTUAL COSMIC RAY TRACKER EXPERIMENT           │
├────────────────────────────────────────────────────────────┤
│  VERSION  :  v0.0.1.                                       │
│  AUTHOR   :  C. Gray                                       │
│  DATE     :  2/2026                           Pitt-CoRTEx  │
└────────────────────────────────────────────────────────────┘
	)";

	//	run manager
	auto runManager = G4RunManagerFactory::CreateRunManager();
	runManager->SetUserInitialization(new PhysicsList());
	runManager->SetUserInitialization(new DetectorConstruction());
	runManager->SetUserInitialization(new ActionInitialization());
	runManager->Initialize();

	//	vis manager
	G4VisManager *visManager = new G4VisExecutive();
	visManager->Initialise();

	//	ui manager
	G4UImanager *uiManager = G4UImanager::GetUIpointer();
	//uiManager->ApplyCommand("/run/verbose 1");
	//uiManager->ApplyCommand("/event/verbose 1");
	//uiManager->ApplyCommand("/tracking/verbose 1");

	//	start session
	if (argc == 1) {
		//	interactive mode
		G4UIExecutive *ui = new G4UIExecutive(argc, argv);
	    uiManager->ApplyCommand("/control/execute vis.mac");
		ui->SessionStart();
		delete ui;
	}
	else {
		//	batch mode
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		uiManager->ApplyCommand(command + fileName);
	}

	//	cleanup
	delete visManager;
	delete runManager;
	return 0;
}