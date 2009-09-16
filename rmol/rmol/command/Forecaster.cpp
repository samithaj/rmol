// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <math.h>
// RMOL
#include <rmol/bom/Bucket.hpp>
#include <rmol/bom/BucketHolder.hpp>
#include <rmol/bom/HistoricalBooking.hpp>
#include <rmol/bom/HistoricalBookingHolder.hpp>
#include <rmol/bom/HistoricalBookingHolderHolder.hpp>
#include <rmol/bom/QForecaster.hpp>
#include <rmol/command/Forecaster.hpp>
#include <rmol/command/Utilities.hpp>

namespace RMOL {

  // //////////////////////////////////////////////////////////////////////
  void Forecaster::qEquivalentBookingCalculation 
       (BucketHolder& ioBucketHolder,
        SellupFactorHolder_T& iSellupFactorHolder,
        HistoricalBookingHolderHolder& iHistoricalBookingHolderHolder,
        HolderOfQEquivalentBookingsPerSimilarFlight_T& 
                                      oQEquivalentBookingsPerSimilarFlight){

    // Get the lowest yield of the given buckets
    const double qYield = ioBucketHolder.getLowestAverageYield ();

    // Initialize a vector for sell-up probabilities
    std::vector<double> lSellupProbabilityVector;

    // Compute sell-up probability for each bucket and store it in a vector
    QForecaster::calculateSellupProbability
      (ioBucketHolder, qYield, iSellupFactorHolder, lSellupProbabilityVector);
    
    // Compute total Q-equivalent bookings
    // QEquivalentBookingCalculator::calculateQEquivalentBooking 
    //   (iHistoricalBookingHolderHolder, 
    //    lSellupProbabilityVector, 
    //    oQEquivalentBookingsPerSimilarFlight);
  }

  // //////////////////////////////////////////////////////////////////////  
  void Forecaster::demandForecastByQForecasting 
       (ForecastedDemandParameterList_T oForecastedDemandParameterList,
        HistoricalDataHolderHolder_T& iHistoricalDataHolderHolder,
        PriceHolder_T& iPriceHolder, 
        SellupFactorHolder_T& iSellupFactorHolder) {
    // Run Q-Forecasting algorithm
    // 1. Obtain forecasted Q-eqivalent demand mean and s.d.(mu_Q, sigma_Q)
    // 2. Partition to each class
    //    E(alpha x Q)=alpha x mu_Q, S.D.(alpha x Q) = |alpha| X sigma_Q

    // TODO: Find the class with the lowest yield
    const double iQYield = Utilities::minimumElementOfAVector(iPriceHolder);

    // // TODO: Initialize a vector for sell-up probabilities
    // std::vector<double> lSellupProbabilityVector;

    // // TODO: Compute sell-up probability for each class/bucket & store it in a vector
    // QForecaster::calculateSellupProbability
    //   (lSellupProbabilityVector, iQYield, iPriceHolder, iSellupFactorHolder);

    // // TODO: Q-equivalent demand calculation
    // QForecaster::calculateQEquivalentDemandParametersAndPartition 
    //   (oForecastedDemandParameterList, 
    //    iHistoricalDataHolderHolder, lSellupProbabilityVector);

  }
}
