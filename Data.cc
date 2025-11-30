#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include<cmath>

using namespace std;

Data::Data(const std::string& filename) {
  ifstream file(filename);

  if (!file.is_open()) {
    throw std::runtime_error("Error could not open file with name: " + filename);
  }

  int size = -1;
  file >> size;

  // read bin edges
  for (int i = 0; i < size + 1; ++i) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }

  // read measurements
  for (int i = 0; i < size; ++i) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }

  // read errors  <-- NEW!
  for (int i = 0; i < size; ++i) {
    double err;
    file >> err;
    m_errors.push_back(err);
  }

  file.close();
  assertSizes();
}

void Data::assertSizes() {
  assert(m_data.size() + 1 == m_bins.size());
  assert(m_errors.size() == m_data.size());
}

int Data::checkCompatibility(const Data& other, int n) const {
    if (size() != other.size()) {
        throw std::runtime_error("Datasets have different sizes.");
    }

    int count = 0;

    for (unsigned int i = 0; i < size(); ++i) {
        double y1 = measurement(i);
        double y2 = other.measurement(i);

        double e1 = error(i);
        double e2 = other.error(i);

        double diff = std::abs(y1 - y2);
        double sigma = std::sqrt(e1*e1 + e2*e2);

        if (diff > n * sigma)
            count++;
    }

    return count;
}

Data Data:: average(const Data& a, const Data& b) {

    if (a.size() != b.size())
        throw std::runtime_error("Datasets have different sizes!");

    // Create an empty copy of a (we overwrite the contents)
    Data result = a;

    for (unsigned int i = 0; i < a.size(); ++i) {

        double y1 = a.measurement(i);
        double y2 = b.measurement(i);

        double e1 = a.error(i);
        double e2 = b.error(i);

        double w1 = 1.0 / (e1*e1);
        double w2 = 1.0 / (e2*e2);

        double y = (w1*y1 + w2*y2) / (w1 + w2);
        double e = std::sqrt(1.0 / (w1 + w2));

        result.m_data[i] = y;
        result.m_errors[i] = e;
    }

    return result;
}
