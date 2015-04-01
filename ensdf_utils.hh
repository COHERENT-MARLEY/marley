#pragma once // Nonstandard but widely-supported (see http://en.wikipedia.org/wiki/Pragma_once)
             // preprocessor directive that prevents this file from being included multiple times.
             // Consider changing to an include guard (http://en.wikipedia.org/wiki/Include_guard)
             // if desired by project collaborators.

#include <string>

namespace ensdf_utils {

  // Strings to use for latex table output of ENSDF data
  std::string latex_table_1 = "\\documentclass[12pt]{article}\n"
    "\n"
    "\\usepackage{amsmath}\n"
    "\\usepackage{booktabs}\n"
    "\\usepackage[justification=justified,\n"
    "width=\\textwidth,\n"
    "labelformat=empty]{caption}\n"
    "\\usepackage[top=1in, bottom=1in, left=0.25in, right=0.25in]{geometry}\n"
    "\\usepackage{isotope}\n"
    "\\usepackage{longtable}\n"
    "\\usepackage{multirow}\n"
    "\\usepackage{siunitx}\n"
    "\n"
    "\\newcommand{\\ExtraRowSpace}{1cm}\n"
    "\n"
    "\\begin{document}\n"
    "\\begin{center}\n"
    "\\begin{longtable}{\n"
    "S[table-number-alignment = center,\n"
    "  table-text-alignment = center]\n"
    "c\n"
    "S[table-number-alignment = center,\n"
    "  table-text-alignment = center]\n"
    "%  table-column-width = 2cm]\n"
    "S[table-number-alignment = center,\n"
    "  table-text-alignment = center]\n"
    "S[table-number-alignment = center,\n"
    "  table-text-alignment = center]\n"
    "}\n"
    "\\caption";

  std::string latex_table_2 = "\\toprule\n"
    "%{\\centering\\textbf{Level Energy (keV)}}\n"
    "%& {\\centering\\textbf{Spin-Parity}}\n"
    "%& {\\centering\\textbf{$\\boldsymbol{\\gamma}$ Energy (keV)}}\n"
    "%& {\\centering\\textbf{$\\boldsymbol{\\gamma}$ RI}}\n"
    "%& {\\centering\\textbf{Final Energy (keV)}} \\\\\n"
    "{\\multirow{3}{2cm}{\\centering\\textbf{Level Energy (keV)}}}\n"
    "& {\\multirow{3}{2cm}{\\centering\\textbf{Spin-Parity}}}\n"
    "& {\\multirow{3}{2cm}{\\centering\\textbf{$\\boldsymbol{\\gamma}$ Energy (keV)}}}\n"
    "& {\\multirow{3}{2cm}{\\centering\\textbf{$\\boldsymbol{\\gamma}$ RI}}}\n"
    "& {\\multirow{3}{2.5cm}{\\centering\\textbf{Final Level Energy (keV)}}} \\\\\n"
    "& & & & \\\\\n"
    "& & & & \\\\\n"
    "\\midrule\n"
    "\\endfirsthead\n"
    "\\caption[]";

  std::string latex_table_3 = "\\toprule\n"
    "%{\\centering\\textbf{Level Energy (keV)}}\n"
    "%& {\\centering\\textbf{Spin-Parity}}\n"
    "%& {\\centering\\textbf{$\\boldsymbol{\\gamma}$ Energy (keV)}}\n"
    "%& {\\centering\\textbf{$\\boldsymbol{\\gamma}$ RI}}\n"
    "%& {\\centering\\textbf{Final Energy (keV)}} \\\\\n"
    "{\\multirow{3}{2cm}{\\centering\\textbf{Level Energy (keV)}}}\n"
    "& {\\multirow{3}{2cm}{\\centering\\textbf{Spin-Parity}}}\n"
    "& {\\multirow{3}{2cm}{\\centering\\textbf{$\\boldsymbol{\\gamma}$ Energy (keV)}}}\n"
    "& {\\multirow{3}{2cm}{\\centering\\textbf{$\\boldsymbol{\\gamma}$ RI}}}\n"
    "& {\\multirow{3}{2.5cm}{\\centering\\textbf{Final Level Energy (keV)}}} \\\\\n"
    "& & & & \\\\\n"
    "& & & & \\\\\n"
    "\\midrule\n"
    "\\noalign{\\vspace{-\\ExtraRowSpace}}\n"
    "\\endhead\n"
    "\\bottomrule \\multicolumn{4}{r}{\\textit{Continued on next page}} \\\\\n"
    "\\endfoot\n"
    "\\bottomrule\n"
    "\\endlastfoot\n"
    "% Table data\n"
    "\n";

  std::string latex_table_4 = "\\end{longtable}\n"
    "\\end{center}\n"
    "\\end{document}";

  // Random number generator that will be used when selecting gammas for
  // cascade simulations. Seed it using the system time
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::knuth_b rand_gen(seed);
 
  // String containing all of the characters that will be
  // considered whitespace by default in the string
  // manipulation functions below 
  const std::string whitespace = " \f\n\r\t\v"; 

  // This version of std::stod will return 0 if it encounters
  // an empty string or an all-whitespace string.
  inline double str_to_double(const std::string& s) {
    size_t endpos = s.find_last_not_of(whitespace);
    if (endpos == std::string::npos) {
      return 0.0; // string was all whitespace
    }
    else {
      return std::stod(s);
    }
  }

  // Function that creates a copy of a std::string object
  // that has been converted to all lowercase
  inline std::string to_lowercase(const std::string& s) {
    std::string new_s = s;
    std::transform(new_s.begin(), new_s.end(), new_s.begin(), ::tolower);
    return new_s;
  }

  // These std::string trimming functions were taken from code
  // presented here: http://www.cplusplus.com/faq/sequences/strings/trim/
  // and here: http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
  // The first three (with the suffix copy) return a trimmed copy of
  // the string without modifying the original.
  inline std::string trim_right_copy(const std::string& s,
    const std::string& delimiters = whitespace)
  {
    size_t endpos = s.find_last_not_of(delimiters);
    return (endpos == std::string::npos) ? "" : s.substr(0, endpos + 1);
  }
  
  inline std::string trim_left_copy(const std::string& s,
    const std::string& delimiters = whitespace)
  {
    size_t startpos = s.find_first_not_of(delimiters);
    return (startpos == std::string::npos) ? "" : s.substr(startpos);
  }
  
  inline std::string trim_copy(const std::string& s,
    const std::string& delimiters = whitespace)
  {
    return trim_left_copy(trim_right_copy(s, delimiters), delimiters);
  }
  
  // The second three alter the original string, returning it
  // after it has been trimmed. 
  inline std::string& trim_right_inplace(std::string& s,
    const std::string& delimiters = whitespace)
  {
    size_t endpos = s.find_last_not_of(delimiters);
    if (endpos == std::string::npos) {
      s.clear();
    }
    else {
      s.erase(endpos + 1);
    }
    return s; 
  }
  
  inline std::string& trim_left_inplace(std::string& s,
    const std::string& delimiters = whitespace)
  {
    size_t startpos = s.find_first_not_of(delimiters);
    if (startpos == std::string::npos) {
      s.clear();
    }
    else {
      s.erase(0, startpos);
    }
    return s; 
  }
  
  inline std::string& trim_inplace(std::string& s,
    const std::string& delimiters = whitespace)
  {
    return trim_left_inplace(trim_right_inplace(s,delimiters), delimiters);
  }

}
