#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "marley_utils.hh"
#include "TMarleyDecayScheme.hh"
#include "TMarleyEvent.hh"
#include "TMarleyMassTable.hh"
#include "TMarleyReaction.hh"

#ifdef USE_ROOT
#include "TFile.h"
#include "TTree.h"
#endif

double fermi_dirac_distribution(double C, bool e_flavor, bool anti, double nu_energy){
  double eta = 0;
  double T = 0;
  double N_nu;

  if(e_flavor && !anti){
    T = 3.5; // temperature in MeV
    N_nu = 2.8;  // total number of electron neutrinos expected (x10^57)
  }
  else if(e_flavor && anti) {
    T = 5.0; // temperature in MeV
    N_nu = 1.9;  // total number of electron anti-neutrinos expected (x10^57)
  }
  else { // !e_flavor
    T = 8.0; // temperature in MeV
    N_nu = 5.0; // total number of mu+tau neutrinos + anti-neutrinos expected (x10^57)
  }
  return (C/std::pow(T,3))*(std::pow(nu_energy,2)/(1+std::exp(nu_energy/(T-eta))))*N_nu;
}

int main(){

  // Get the time that the program was started
  std::chrono::system_clock::time_point start_time_point
    = std::chrono::system_clock::now();

  std::time_t start_time = std::chrono::system_clock::to_time_t(start_time_point);

  std::cout << marley_utils::marley_logo << std::endl;
  std::cout << "\"Don't worry about a thing," << std::endl;
  std::cout << "'Cause every little thing gonna be all right.\"" << std::endl;
  std::cout << "-- Bob, \"Three Little Birds\"" << std::endl << std::endl;

  std::cout << "MARLEY started on "
    << std::put_time(std::localtime(&start_time), "%c %Z")
    << std::endl;

  //for (int i = 1; i < 8; i++)
  //  TMarleyMassTable::print_separation_energies(19, 40, i);
  //std::cout << std::endl << std::endl;

  // Sample from electron flavor supernova neutrinos for C = 0.55
  std::function<double(double)> f = std::bind(fermi_dirac_distribution, 0.55,
    true, false, std::placeholders::_1);

  #ifdef USE_ROOT
  // Create a pointer to an event object. This will
  // be used to fill the event tree with the
  // events generated in the loop below
  TMarleyEvent* p_event = nullptr;

  // Create a ROOT file to store the event tree
  TFile treeFile("event_tree.root","RECREATE");

  // Create a ROOT tree to store the events
  TTree event_tree("event_tree", "A tree of TMarleyEvent objects");

  // Create a branch in this ROOT tree, and associate
  // it with the event pointer we made before
  event_tree.Branch("events", &p_event, 32000, 99);

  // The amount of data written to the ROOT file
  // represented as a string (e.g., "10 MB")
  std::string data_written;
  #endif

  // Select the isotope and ENSDF file to use for the simulation
  std::string nuc_id = marley_utils::nuc_id(19, 40); // 40K
  std::string filename = "ensdf.040";

  // Create a decay scheme object to store data
  // imported from the ENSDF file
  TMarleyDecayScheme ds(nuc_id, filename);

  TMarleyReaction r("ve40ArCC.react", &ds);

  // TODO: debug numerical errors that arise when
  // Ea = E_threshold

  // Simulate a charged current reaction
  int n_events = 1000;
  double Ea; // Incident neutrino energy

  // Display all floating-point numbers without
  // using scientific notation and using
  // one decimal digit
  std::cout << std::fixed << std::setprecision(1);

  for (int i = 1; i <= n_events; ++i) {

    // Sample a supernova neutrino energy
    Ea = marley_utils::rejection_sample(f, 4.36, 50);

    // Create an event using the charged current reaction
    TMarleyEvent e = r.create_event(Ea);

    // Print a status message showing the current number of events
    std::cout << "Event Count = " << i << "/" << n_events
      << " (" << i*100/static_cast<double>(n_events)
      << "% complete)" << std::endl;

    #ifdef USE_ROOT
    // Get the address of this event object
    p_event = new TMarleyEvent;
    *p_event = e;

    std::chrono::system_clock::time_point current_time_point
      = std::chrono::system_clock::now();
    std::cout << "Elapsed time: "
      << marley_utils::elapsed_time_string(start_time_point,
      current_time_point) << " (Estimated total run time: ";

    marley_utils::seconds<float> estimated_total_time =
      (current_time_point - start_time_point)*(static_cast<float>(n_events)/i);

    std::cout << marley_utils::duration_to_string
      <marley_utils::seconds<float>>(estimated_total_time)
      << ")" << std::endl;

    // Store this event in the ROOT tree
    event_tree.Fill();
    data_written = marley_utils::num_bytes_to_string(treeFile.GetBytesWritten(),2);
    std::cout << "Data written = " << data_written << "\033[K" << std::endl;
    // Move up one line in std::cout
    std::cout << "\033[F";
    #endif

    // Move up two lines in std::cout
    std::cout << "\033[F\033[F";
  }

  #ifdef USE_ROOT
  event_tree.Write();
  treeFile.Close();
  data_written = marley_utils::num_bytes_to_string(treeFile.GetBytesWritten());
  std::cout << "\033[E" << "Data written = " << data_written << "\033[K" << std::endl;
  #endif

  // Display the time that the program terminated
  std::chrono::system_clock::time_point end_time_point
    = std::chrono::system_clock::now();
  std::time_t end_time = std::chrono::system_clock::to_time_t(end_time_point);

  std::cout << "MARLEY terminated normally on "
    << std::put_time(std::localtime(&end_time), "%c %Z")
    << std::endl;

  std::cout << std::endl << std::endl;
  return 0;
}
