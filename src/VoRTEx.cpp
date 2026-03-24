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

#include "CLI11.hpp"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"





int main(int argc, char** argv) {

	//	cli
	CLI::App app{"VoRTEx — Virtual Cosmic Ray Tracker Experiment"};

	G4String outFile;
	G4String macFile;
	G4int nThreads;
	std::vector<double> gunPos;
	std::vector<double> gunMom;
	G4double gunEnergy;
	G4String particleName;

	app.add_option("-o,--output", outFile, "Output filename")
		->default_val("output.csv");
	app.add_option("-t,--threads", nThreads, "Number of worker threads")
		->default_val(1)
		->check(CLI::PositiveNumber);
	app.add_option("-p,--position", gunPos, "Gun position in mm (x y z)")
		->expected(3)
		->default_val(std::vector<double>{50., 50., 50.});
	app.add_option("-d,--direction", gunMom, "Gun momentum direction (x y z)")
		->expected(3)
		->default_val(std::vector<double>{-1., -1., -1.});
	app.add_option("-e,--energy", gunEnergy, "Particle energy in MeV")
		->default_val(1000.);
	app.add_option("-P,--particle", particleName, "Particle type (e.g. mu-, e-, proton)")
		->default_val("mu-");
	app.add_option("macro", macFile, "Geant4 macro file (interactive if not given)");

	CLI11_PARSE(app, argc, argv);

	bool interactive = macFile.empty();

	G4ThreeVector gunPosition(gunPos[0], gunPos[1], gunPos[2]);
	G4ThreeVector gunMomentum(gunMom[0], gunMom[1], gunMom[2]);
	




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
	runManager->SetNumberOfThreads(nThreads);
	runManager->SetUserInitialization(new PhysicsList());
	runManager->SetUserInitialization(new DetectorConstruction(outFile));
	runManager->SetUserInitialization(new ActionInitialization(gunPosition, gunMomentum, gunEnergy, particleName));
	runManager->Initialize();

	//	vis manager
	G4VisManager *visManager = new G4VisExecutive();
	visManager->Initialise();

	//	ui manager
	G4UImanager *uiManager = G4UImanager::GetUIpointer();

	//	start session
	if (interactive) {
		//	interactive mode
		G4UIExecutive *ui = new G4UIExecutive(argc, argv);
	    uiManager->ApplyCommand("/control/execute vis.mac");
		ui->SessionStart();
		delete ui;
	}
	else {
		//	batch mode
		G4String command = "/control/execute ";
		uiManager->ApplyCommand(command + macFile);
	}

	//	cleanup
	delete visManager;
	delete runManager;
	return 0;
}
