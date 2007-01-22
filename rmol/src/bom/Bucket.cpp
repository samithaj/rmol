// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include "Bucket.hpp"

namespace RMOL {

  // //////////////////////////////////////////////////////////////////////
  Bucket::Bucket () : _yieldRange (), _demand (), 
                      _protection (0.0), _cumulatedProtection (0.0),
                      _bookingLimit (0.0), _cumulatedBookingLimit (0.0) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  Bucket::Bucket (const Bucket& iBucket) :
    _yieldRange (iBucket.getYieldRange()), 
    _demand (iBucket.getDemand()), 
    _protection (iBucket.getProtection()),
    _cumulatedProtection (iBucket.getCumulatedProtection()),
    _bookingLimit (iBucket.getBookingLimit()),
    _cumulatedBookingLimit (iBucket.getCumulatedBookingLimit()) {
  }

  // //////////////////////////////////////////////////////////////////////
  Bucket::Bucket (const FldYieldRange& iYieldRange) :
    _yieldRange (iYieldRange), _demand (), 
    _protection (0.0), _cumulatedProtection (0.0),
    _bookingLimit (0.0), _cumulatedBookingLimit (0.0) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  Bucket::Bucket (const FldYieldRange& iYieldRange, const Demand& iDemand) :
    _yieldRange (iYieldRange), _demand (iDemand), 
    _protection (0.0), _cumulatedProtection (0.0),
    _bookingLimit (0.0), _cumulatedBookingLimit (0.0) {
  }
    
  // //////////////////////////////////////////////////////////////////////
  Bucket::Bucket (const Demand& iDemand) :
    _yieldRange (iDemand.getYieldRange()), _demand (iDemand), 
    _protection (0.0), _cumulatedProtection (0.0),
    _bookingLimit (0.0), _cumulatedBookingLimit (0.0) {
  }

  // //////////////////////////////////////////////////////////////////////
  Bucket::~Bucket() {
  }

  // //////////////////////////////////////////////////////////////////////
  const FldDistributionParameters& Bucket::getDistributionParameters() const {
    return _demand.getDistributionParameters();
  }

  // //////////////////////////////////////////////////////////////////////
  const double Bucket::getMean() const {
    return _demand.getMean();
  }

  // //////////////////////////////////////////////////////////////////////
  const double Bucket::getStandardDeviation() const {
    return _demand.getStandardDeviation();
  }

  // //////////////////////////////////////////////////////////////////////
  const double Bucket::getUpperYield() const {
    return _yieldRange.getUpperYield();
  }

  // //////////////////////////////////////////////////////////////////////
  const double Bucket::getAverageYield() const {
    return _yieldRange.getAverageYield();
  }

  // //////////////////////////////////////////////////////////////////////
  const double Bucket::getLowerYield() const {
    return _yieldRange.getLowerYield();
  }

}
