#include "VXActionInitialization.hpp"
#include "VXPrimaryGenerator.hpp"
#include "VXRunAction.hpp"





void ActionInitialization::BuildForMaster() const {
	SetUserAction(new RunAction());
}





void ActionInitialization::Build() const {
	SetUserAction(new PrimaryGenerator());
    SetUserAction(new RunAction());
}
