#ifndef DATA_HH
#define DATA_HH

#include <vector>
#include <string>

class Data {
 public:
  Data(const std::string& filename);

  unsigned int size() const { return m_data.size(); }
  double measurement(int i) const { return m_data[i]; }

  double binCenter(int i) const { return 0.5 * (m_bins[i] + m_bins[i+1]); }
  double binLow(int i) const { return m_bins[i]; }
  double binHigh(int i) const { return m_bins[i+1]; }

  double error(int i) const { return m_errors[i]; }
  int checkCompatibility(const Data& other, int n) const;
  Data average(const Data& a, const Data& b);




 private:
  Data() {}  
  void assertSizes();
  std::vector<double> m_data;
  std::vector<double> m_bins;
  std::vector<double> m_errors;  // <-- new!
};

#endif
