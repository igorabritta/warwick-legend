#ifndef WLGDEventAction_h
#define WLGDEventAction_h 1

#include <algorithm>
#include <numeric>
#include <vector>

#include "WLGDCrystalHit.hh"

#include "G4UserEventAction.hh"
#include "G4GenericMessenger.hh"
#include "globals.hh"
#include <map>

/// Event action class
///

class WLGDEventAction : public G4UserEventAction
{
public:
  WLGDEventAction();
  virtual ~WLGDEventAction() = default;

  virtual void BeginOfEventAction(const G4Event* event);
  virtual void EndOfEventAction(const G4Event* event);

  // to create columns for Ntuple
  std::vector<G4int>& GetNGe77() { return nGe77; }
  std::vector<G4int>&    GetHitTID()  { return htrid; }
  std::vector<G4double>& GetHitEdep() { return edep; }
  std::vector<G4double>& GetHitEkin() { return ekin; }
  std::vector<G4double>& GetHitTime() { return thit; }
  std::vector<G4double>& GetHitWeight() { return whit; }
  std::vector<G4double>& GetHitxLoc() { return xloc; }
  std::vector<G4double>& GetHityLoc() { return yloc; }
  std::vector<G4double>& GetHitzLoc() { return zloc; }
  std::vector<G4double>& GetNeutronxLoc() { return neutronxloc; }
  std::vector<G4double>& GetNeutronyLoc() { return neutronyloc; }
  std::vector<G4double>& GetNeutronzLoc() { return neutronzloc; }
  std::vector<G4double>& GetNeutronxMom() { return neutronxmom; }
  std::vector<G4double>& GetNeutronyMom() { return neutronymom; }
  std::vector<G4double>& GetNeutronzMom() { return neutronzmom; }
  std::vector<G4int>& GetNumberOfNeutronsInEvent() { return NumberOfNeutronsProducedInEvent; }
  std::vector<G4double>& GetLArEnergyDeposition() { return TotalEnergyDepositionInLAr_prompt; }
  std::vector<G4double>& GetGeEnergyDeposition() { return TotalEnergyDepositionInGe_prompt; }
  std::vector<G4double>& GetLArEnergyDeposition_delayed() { return TotalEnergyDepositionInLAr_delayed; }
  std::vector<G4double>& GetGeEnergyDeposition_delayed() { return TotalEnergyDepositionInGe_delayed; }
  std::vector<G4int>& GetReentranceTube(){return ReentranceTube;}

  std::vector<G4double>& GetIndividualEnergyDeposition_Timing(){return IndividualEnergyDeposition_Timing;}
  std::vector<G4double>& GetIndividualEnergyDeposition_Energy(){return IndividualEnergyDeposition_Energy;}
  std::vector<G4double>& GetIndividualEnergyDeposition_Position_x(){return IndividualEnergyDeposition_Position_x;}
  std::vector<G4double>& GetIndividualEnergyDeposition_Position_y(){return IndividualEnergyDeposition_Position_y;}
  std::vector<G4double>& GetIndividualEnergyDeposition_Position_z(){return IndividualEnergyDeposition_Position_z;}
  std::vector<G4int>& GetIndividualEnergyDeposition_ReentranceTube(){return IndividualEnergyDeposition_ReentranceTube;}
  std::vector<G4int>& GetIndividualEnergyDeposition_LArOrGe(){return IndividualEnergyDeposition_LArOrGe;}

  // tajectory methods
  std::vector<G4int>&    GetTrjPDG() { return trjpdg; }
  std::vector<G4int>&    GetTrjEntries() { return trjnpts; }
  std::vector<G4int>&    GetNameID() { return nameid; }
  std::vector<G4double>& GetTrjXVtx() { return trjxvtx; }
  std::vector<G4double>& GetTrjYVtx() { return trjyvtx; }
  std::vector<G4double>& GetTrjZVtx() { return trjzvtx; }
  std::vector<G4double>& GetTrjXPos() { return trjxpos; }
  std::vector<G4double>& GetTrjYPos() { return trjypos; }
  std::vector<G4double>& GetTrjZPos() { return trjzpos; }

  void AddEkin(double kin){ekin.push_back(kin);}
  void AddNeutronxLoc(double posx){neutronxloc.push_back(posx);}
  void AddNeutronyLoc(double posy){neutronyloc.push_back(posy);}
  void AddNeutronzLoc(double posz){neutronzloc.push_back(posz);}
  void AddNeutronxMom(double posx){neutronxmom.push_back(posx);}
  void AddNeutronyMom(double posy){neutronymom.push_back(posy);}
  void AddNeutronzMom(double posz){neutronzmom.push_back(posz);}

  void AddIndividualEnergyDeposition_Timing(double time){IndividualEnergyDeposition_Timing.push_back(time);}
  void AddIndividualEnergyDeposition_Energy(double energy){IndividualEnergyDeposition_Energy.push_back(energy);}
  void AddIndividualEnergyDeposition_Position_x(double posx){IndividualEnergyDeposition_Position_x.push_back(posx);}
  void AddIndividualEnergyDeposition_Position_y(double posy){IndividualEnergyDeposition_Position_y.push_back(posy);}
  void AddIndividualEnergyDeposition_Position_z(double posz){IndividualEnergyDeposition_Position_z.push_back(posz);}
  void AddIndividualEnergyDeposition_ReentranceTube(int n){IndividualEnergyDeposition_ReentranceTube.push_back(n);}
  void AddIndividualEnergyDeposition_LArOrGe(int n){IndividualEnergyDeposition_LArOrGe.push_back(n);}
  std::map<int, int>         neutronProducerMap;

  void IncreaseByOne_NeutronInEvent(){NumberOfNeutronsProducedInEvent[0] += 1;}
  void IncreaseLArEnergyDeposition(G4double Edep, G4int whichReEntranceTube){TotalEnergyDepositionInLAr_prompt[whichReEntranceTube] += Edep;}//SaveAllEvents
  void IncreaseGeEnergyDeposition(G4double Edep, G4int whichReEntranceTube){TotalEnergyDepositionInGe_prompt[whichReEntranceTube] += Edep;}//SaveAllEvents
  void IncreaseLArEnergyDeposition_delayed(G4double Edep, G4int whichReEntranceTube){TotalEnergyDepositionInLAr_delayed[whichReEntranceTube] += Edep;}//SaveAllEvents
  void IncreaseGeEnergyDeposition_delayed(G4double Edep, G4int whichReEntranceTube){TotalEnergyDepositionInGe_delayed[whichReEntranceTube] += Edep;}//SaveAllEvents
  void SaveAllEvents(G4int answer);
  void DefineCommands();
private:
  // methods
  WLGDCrystalHitsCollection*   GetHitsCollection(G4int hcID,
                                                 const G4Event* event) const;
  G4int                      GeomID(G4String name);
  void                       makeMap();
  G4GenericMessenger*        fEventMessenger;
  G4int                      fAllEvents = 0;


  //! Brief description
  /*!
   * This filter method retrieves the track history of a Germanium hit.
   * Starting with the hit id, the identical track id is found in
   * the trajectory container with vectors extracted separately,
   * here the track id and the corresponding track parent id. The parent
   * id then serves as the new id to look for, all the way back to the
   * primary particle. That way, only the relevant track history from
   * start to finish is stored.
   *
   * \param[in] item hit id to look for as starting point of history
   * \param[in] tid vector of all track id's stored in event
   * \param[in] pid corresponding vector of all parent id's for all tracks in event.
   */
  std::vector<int> FilterTrajectories(int item, const std::vector<G4int>& tid,
                                      const std::vector<G4int>& pid)
  {
    int              idx  = 0;
    int              pidx = 0;
    std::vector<int> result;
    auto             it = std::find(tid.begin(), tid.end(), item);

    while(it != tid.end())  // find all links in the chain
    {
      idx = (it - tid.begin());  // location of id
      result.push_back(idx);

      pidx = pid.at(idx);  // next to look for
      it   = std::find(tid.begin(), tid.end(), pidx);
    }

    return result;
  }

  // data members
  // hit data
  
  std::vector<G4int>    nGe77;
  G4int                 fHID    = -1;
  std::vector<G4int>    htrid;
  std::vector<G4int>    ReentranceTube;
  std::vector<G4double> edep;
  std::vector<G4double> ekin;
  std::vector<G4double> thit;
  std::vector<G4double> whit;
  std::vector<G4double> xloc;
  std::vector<G4double> yloc;
  std::vector<G4double> zloc;
  std::vector<G4double> neutronxloc;
  std::vector<G4double> neutronyloc;
  std::vector<G4double> neutronzloc;
  std::vector<G4double> neutronxmom;
  std::vector<G4double> neutronymom;
  std::vector<G4double> neutronzmom;

  // additional data for other particles
  std::vector<G4int>	NumberOfNeutronsProducedInEvent;
  std::vector<G4double>	TotalEnergyDepositionInLAr_prompt;
  std::vector<G4double>	TotalEnergyDepositionInGe_prompt;
  std::vector<G4double>	TotalEnergyDepositionInLAr_delayed;
  std::vector<G4double>	TotalEnergyDepositionInGe_delayed;
  std::vector<G4double> IndividualEnergyDeposition_Timing;
  std::vector<G4double> IndividualEnergyDeposition_Energy;
  std::vector<G4double> IndividualEnergyDeposition_Position_x;
  std::vector<G4double> IndividualEnergyDeposition_Position_y;
  std::vector<G4double> IndividualEnergyDeposition_Position_z;
  std::vector<G4int> IndividualEnergyDeposition_ReentranceTube;
  std::vector<G4int> IndividualEnergyDeposition_LArOrGe;


  // trajectory data
  std::vector<G4int>        trjpdg;
  std::vector<G4int>        trjnpts;
  std::vector<G4int>        nameid;
  std::vector<G4double>     trjxvtx;
  std::vector<G4double>     trjyvtx;
  std::vector<G4double>     trjzvtx;
  std::vector<G4double>     trjxpos;
  std::vector<G4double>     trjypos;
  std::vector<G4double>     trjzpos;
  std::map<G4String, G4int> lookup;
};

#endif
