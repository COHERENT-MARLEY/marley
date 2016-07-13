#pragma once
#include <cmath>
#include <complex>
#include <vector>

#include "DecayScheme.hh"
#include "MassTable.hh"
#include "marley_utils.hh"

namespace marley {

  /// @brief Nuclear optical model for fragment emission calculations
  /// @details This class implements the global optical model potential
  /// of A. J. Koning and J. P. Delarouche, <a
  /// href="http://dx.doi.org/10.1016/S0375-9474(02)01321-0">
  /// Nucl. Phys. A 713 (2003) 231-310</a>.
  /// <a href="https://en.wikipedia.org/wiki/Numerov%27s_method">Numerov's
  /// method</a> is used to integrate the Schr&ouml;dinger equation during
  /// transmission coefficient and cross section calculations.
  class SphericalOpticalModel {

    public:

      /// @param z Atomic number of the desired nuclide
      /// @param a Mass number of the desired nuclide
      SphericalOpticalModel(int z, int a);

      /// @brief Calculate the optical model potential (including the Coulomb
      /// potential)
      /// @param r Distance from nuclear center (fm)
      /// @param E Fragment kinetic energy (MeV)
      /// @param fragment_pdg PDG code for the nuclear fragment in the
      /// potential
      /// @param two_j Two times the total angular momentum of the fragment
      /// @param l Orbital angular momentum of the fragment
      /// @param two_s Two times the spin of the fragment
      /// @returns Complex-valued optical model potential (MeV)
      std::complex<double> optical_model_potential(double r, double E,
        int fragment_pdg, int two_j, int l, int two_s);

      double transmission_coefficient(double E, int fragment_pdg, int two_j,
        int l, int two_s, double h);

      double total_cross_section(double E, int fragment_pdg, int two_s,
        size_t l_max, double h);

      /// @brief Get the atomic number
      inline int Z() const;

      /// @brief Get the mass number
      inline int A() const;

    private:

      // Helper function for computing optical model transmission coefficients
      // and cross sections
      std::complex<double> s_matrix_element(double E, int fragment_pdg,
        int two_j, int l, int two_s, double h);

      // Helper functions for computing the optical model potential
      void calculate_om_parameters(double E, int fragment_pdg, int two_j,
        int l, int two_s);

      // Compute the optical model potential at radius r
      std::complex<double> omp(double r) const;

      // Computes the optical model potential minus the Coulomb potential at
      // radius r
      std::complex<double> omp_minus_Vc(double r) const;

      // Woods-Saxon shape
      double f(double r, double R, double a) const;

      inline double get_fragment_reduced_mass(int fragment_pdg) const;

      // Partial derivative with respect to r of the Woods-Saxon shape
      double dfdr(double r, double R, double a) const;

      // Coulomb potential for a point particle with charge q*e interacting
      // with a uniformly charged sphere with radius R and charge Q*e
      double Vc(double r, double R, int Q, int q) const;

      // Non-derivative radial Schrödinger equation terms to use for computing
      // transmission coefficients via the Numerov method
      std::complex<double> a(double r, double E, int fragment_pdg, int l);

      // Version of Schrodinger equation terms with the optical model potential
      // U pre-computed
      std::complex<double> a(double r, double E, int fragment_pdg, int l,
        std::complex<double> U) const;

      // Nuclear atomic and mass numbers
      int Z_, A_;
      // Neutron parameters
      double v1n, v2n, v3n, v4n, w1n, w2n, d1n, d2n, d3n, vso1n, vso2n;
      double wso1n, wso2n, Efn, Rvn, avn, Rdn, adn, Rso_n, aso_n;
      // Proton parameters
      double v1p, v2p, v3p, v4p, w1p, w2p, d1p, d2p, d3p, vso1p, vso2p;
      double wso1p, wso2p, Efp, Vcbar_p, Rvp, avp, Rdp, adp, Rso_p, aso_p;
      // Radius for nuclear Coulomb potential
      double Rc;
      // Reduced mass lookup table
      std::unordered_map<int, double> reduced_masses_;

      // Mass of a charged pion
      static constexpr double mpiplus = 139.57018; // MeV
      // Squared pion Compton wavelength
      static constexpr double lambda_piplus2 = (marley_utils::hbar_c
        / mpiplus) * (marley_utils::hbar_c / mpiplus); // fm

      // Temporary storage for optical model calculations
      double Rv, av, Rd, ad, Rso, aso; // Geometrical parameters
      double Vv, Wv, Wd, Vso, Wso; // Energy-dependent terms in the potential
      double spin_orbit_eigenvalue; // Eigenvalue of the spin-orbit operator
      int z; // Fragment atomic number

      // Threshold for abs(U - Vc) used to find a suitable matching radius for
      // computing transmission coefficients.
      // WARNING: This value should be chosen carefully.  Since the Numerov
      // method used for computing the fragment wavefunctions is only accurate
      // to order h^4 (where h is the step size used), choosing this threshold
      // to be comparable to or smaller than h^4 may cause numerical problems.
      // A value of h^3 seems to work pretty well. You may be able to get away
      // with a higher threshold, but check to make sure the transmission
      // coefficients aren't significantly affected before adopting a higher
      // value.
      static constexpr double MATCHING_RADIUS_THRESHOLD = 1e-3;
  };

  // Inline function definitions
  inline int SphericalOpticalModel::Z() const { return Z_; }

  inline int SphericalOpticalModel::A() const { return A_; }

  inline double SphericalOpticalModel::get_fragment_reduced_mass(
    int fragment_pdg) const { return reduced_masses_.at(fragment_pdg); }
}
