//
// Created by moritz on 3/4/21.
//


#include <iostream>

using namespace std;
#include "WLGDTrackingAction.hh"
#include "WLGDSteppingAction.hh"
#include "WLGDRunAction.hh"

#include "G4SystemOfUnits.hh"

#include "G4RunManager.hh"




WLGDSteppingAction::WLGDSteppingAction(WLGDEventAction* event,WLGDRunAction* run, WLGDDetectorConstruction* det)
{
  fEventAction = event;
  fRunAction = run;
  fDetectorConstruction = det;
  DefineCommands();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLGDSteppingAction::UserSteppingAction(const G4Step *aStep) {


  // Edit: 2021/03/05 by Moritz Neuberger
  // Adding tracking of amount of neutrons crossing the detectors
  if(aStep->GetTrack()->GetParticleDefinition()->GetParticleName() == "neutron")
  {
    if(aStep->GetTrack()->GetNextVolume())
    {
      auto physVol1 = aStep->GetTrack()->GetVolume();
      auto physVol2 = aStep->GetTrack()->GetNextVolume();
      if(physVol1->GetName() != "Ge_phys" && physVol2->GetName() == "Ge_phys"){
        if(fRunAction->getWriteOutGeneralNeutronInfo() == 1) fRunAction->increaseNumberOfCrossingNeutrons();
      }
    }
  }

  // Edit: 2021/04/07 by Moritz Neuberger
  // Adding total energy deposition inside LAr
  if(fDepositionInfo == 1)
  {
    if((fDetectorConstruction->GetGeometryName() == "hallA" && aStep->GetTrack()->GetLogicalVolumeAtVertex()->GetName() == "Lar_log") || aStep->GetTrack()->GetLogicalVolumeAtVertex()->GetName() == "ULar_log" ||  aStep->GetTrack()->GetLogicalVolumeAtVertex()->GetName() == "Ge_log" || aStep->GetTrack()->GetLogicalVolumeAtVertex()->GetName() == "Water_log")
    {
      if(aStep->GetTotalEnergyDeposit() > 0)
      {
	if(aStep->GetTrack()->GetLogicalVolumeAtVertex()->GetName() == "Water_log"){
		if(aStep->GetPostStepPoint()->GetGlobalTime() / us < 10.)
			fEventAction->IncreaseEdepWater_prompt(aStep->GetTotalEnergyDeposit() / eV);
		else if(aStep->GetPostStepPoint()->GetGlobalTime() / ms < 1.)
			fEventAction->IncreaseEdepWater_delayed(aStep->GetTotalEnergyDeposit() / eV);
	}     

	G4double tmp_x = aStep->GetPostStepPoint()->GetPosition().getX();
        G4double tmp_y = aStep->GetPostStepPoint()->GetPosition().getY();
        G4double tmp_z = aStep->GetPostStepPoint()->GetPosition().getZ();

        G4int whichReentranceTube;
        if(abs(tmp_x) > abs(tmp_y) && tmp_x > 0)
          whichReentranceTube = 0;
        if(abs(tmp_x) > abs(tmp_y) && tmp_x < 0)
          whichReentranceTube = 2;
        if(abs(tmp_x) < abs(tmp_y) && tmp_y > 0)
          whichReentranceTube = 1;
        if(abs(tmp_x) < abs(tmp_y) && tmp_y < 0)
          whichReentranceTube = 3;

        if(fDetectorConstruction->GetGeometryName() == "hallA")
          whichReentranceTube = 0;

        G4int whichVolume = -1;
        if(aStep->GetPostStepPoint()->GetTouchable()->GetVolume(0)->GetLogicalVolume()->GetName() == "ULar_log" || (fDetectorConstruction->GetGeometryName() == "hallA" && aStep->GetTrack()->GetLogicalVolumeAtVertex()->GetName() == "Lar_log"))
        {
          whichVolume = 0;
          if(aStep->GetPostStepPoint()->GetGlobalTime() / us < 10.)
          {
            fEventAction->IncreaseLArEnergyDeposition(aStep->GetTotalEnergyDeposit() / eV,
                                                      whichReentranceTube);
          }
          else
          {
            if(aStep->GetPostStepPoint()->GetGlobalTime() / ms < 1.)
            {
              fEventAction->IncreaseLArEnergyDeposition_delayed(aStep->GetTotalEnergyDeposit() / eV,
                                                               whichReentranceTube);
            }
            if(aStep->GetPostStepPoint()->GetGlobalTime() / s < 1.)
            {
              fEventAction->IncreaseLArEnergyDeposition_delayed_long(
                aStep->GetTotalEnergyDeposit() / eV, whichReentranceTube);
            }
          }
          if(aStep->GetPostStepPoint()->GetGlobalTime() / s > 1.)
          {
            fEventAction->IncreaseLArEnergyDeposition_after_delayed(
              aStep->GetTotalEnergyDeposit() / eV, whichReentranceTube);
          }
        }

        if(aStep->GetPostStepPoint()->GetTouchable()->GetVolume(0)->GetLogicalVolume()->GetName() == "Ge_log")
        {
          whichVolume = 1;
          if(aStep->GetPostStepPoint()->GetGlobalTime() / us < 10.)
          {
            if(aStep->GetPostStepPoint()->GetTouchable()->GetVolume(1)->GetLogicalVolume()->GetName() == "Layer_log")
              fEventAction->IncreaseEdepPerDetector(aStep->GetPostStepPoint()->GetTouchable()->GetVolume(1)->GetCopyNo() + whichReentranceTube*96,aStep->GetTotalEnergyDeposit() / eV);
            else
              G4cout << "Trying to access Layer_log for the prompt multiplicity but it is " << aStep->GetPostStepPoint()->GetTouchable()->GetVolume(1)->GetLogicalVolume()->GetName() << G4endl;
          }
          else {
            if(aStep->GetPostStepPoint()->GetGlobalTime() / ms < 1.)
            {
              if(aStep->GetPostStepPoint()->GetTouchable()->GetVolume(1)->GetLogicalVolume()->GetName() == "Layer_log")
                fEventAction->IncreaseEdepPerDetector_delayed(aStep->GetPostStepPoint()->GetTouchable()->GetVolume(1)->GetCopyNo() + whichReentranceTube*96,aStep->GetTotalEnergyDeposit() / eV);
              else
                G4cout << "Trying to access Layer_log for the delayed multiplicity but it is " << aStep->GetPostStepPoint()->GetTouchable()->GetVolume(1)->GetLogicalVolume()->GetName() << G4endl;
            }
            if(aStep->GetPostStepPoint()->GetGlobalTime() / s < 1.)
            {
              fEventAction->IncreaseEdepPerDetector_delayed_long(aStep->GetPostStepPoint()->GetTouchable()->GetVolume(1)->GetCopyNo() + whichReentranceTube*96,aStep->GetTotalEnergyDeposit() / eV);
            }
          }
          if(aStep->GetPostStepPoint()->GetGlobalTime() / s > 1.)
          {
            fEventAction->IncreaseGeEnergyDeposition_after_delayed(
              aStep->GetTotalEnergyDeposit() / eV, whichReentranceTube);
          }
        }

	if (aStep->GetPostStepPoint()->GetGlobalTime() / s > 1) return;

        fEventAction->AddIndividualEnergyDeposition_Timing(
          aStep->GetPostStepPoint()->GetGlobalTime() / s);
        fEventAction->AddIndividualEnergyDeposition_Energy(
          aStep->GetTotalEnergyDeposit() / eV);
        fEventAction->AddIndividualEnergyDeposition_ReentranceTube(whichReentranceTube);
        fEventAction->AddIndividualEnergyDeposition_Position_x(tmp_x / m);
        fEventAction->AddIndividualEnergyDeposition_Position_y(tmp_y / m);
        fEventAction->AddIndividualEnergyDeposition_Position_z(tmp_z / m);
        fEventAction->AddIndividualEnergyDeposition_LArOrGe(whichVolume);
      }
    }
  }
}

void WLGDSteppingAction::GetDepositionInfo(G4int answer){fDepositionInfo = answer;}

void WLGDSteppingAction::DefineCommands(){
  // Define geometry command directory using generic messenger class
  fStepMessenger = new G4GenericMessenger(this, "/WLGD/step/",
                                           "Commands for controlling stepping action");

  // switch command
  fStepMessenger->DeclareMethod("getDepositionInfo", &WLGDSteppingAction::GetDepositionInfo)
    .SetGuidance("Set whether to obtain energy deposition information inside reentrance tubes")
    .SetGuidance("0 = don't")
    .SetGuidance("1 = do")
    .SetCandidates("0 1")
    .SetDefaultValue("0");

}
