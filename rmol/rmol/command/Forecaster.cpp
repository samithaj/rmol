// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
// RMOL
// #include <rmol/bom/Bucket.hpp>
// #include <rmol/bom/BucketHolder.hpp>
// #include <rmol/bom/HistoricalBooking.hpp>
// #include <rmol/bom/HistoricalBookingHolder.hpp>
// #include <rmol/bom/HistoricalBookingHolderHolder.hpp>
#include <rmol/bom/QForecaster.hpp>
#include <rmol/command/Forecaster.hpp>
#include <rmol/command/Utilities.hpp>
#include <rmol/service/Logger.hpp>

namespace RMOL {

  // // //////////////////////////////////////////////////////////////////////
  // void Forecaster::qEquivalentBookingCalculation 
  //      (BucketHolder& ioBucketHolder,
  //       SellupFactorHolder_T& iSellupFactorHolder,
  //       HistoricalBookingHolderHolder& iHistoricalBookingHolderHolder,
  //       HolderOfQEquivalentBookingsPerSimilarFlight_T& 
  //                                     oQEquivalentBookingsPerSimilarFlight){

  //   // Get the lowest yield of the given buckets
  //   const double qYield = ioBucketHolder.getLowestAverageYield ();

  //   // Initialize a vector for sell-up probabilities
  //   std::vector<double> lSellupProbabilityVector;

  //   // Compute sell-up probability for each bucket and store it in a vector
  //   QForecaster::calculateSellupProbability
  //     (ioBucketHolder, qYield, iSellupFactorHolder, lSellupProbabilityVector);
    
  //   // Compute total Q-equivalent bookings
  //   // QEquivalentBookingCalculator::calculateQEquivalentBooking 
  //   //   (iHistoricalBookingHolderHolder, 
  //   //    lSellupProbabilityVector, 
  //   //    oQEquivalentBookingsPerSimilarFlight);
  // }

  // //////////////////////////////////////////////////////////////////////  
  void Forecaster::demandForecastByQForecasting 
       (ForecastedDemandParameterList_T& ioForecastedDemandParameterList,
        HistoricalDataHolderHolder_T& iHistoricalDataHolderHolder,
        PriceHolder_T& iPriceHolder, 
        SellupFactorHolder_T& iSellupFactorHolder) {
    // Run Q-Forecasting algorithm
    // 1. Obtain forecasted Q-eqivalent demand mean and s.d.(mu_Q, sigma_Q)
    // 2. Partition to each class
    //    E(alpha x Q)=alpha x mu_Q, S.D.(alpha x Q) = |alpha| X sigma_Q

    // Debug 
    std::ostringstream oDebugStr;
    oDebugStr << "Prices are "
              << Utilities::vectorToString(iPriceHolder) << "\n"
              << "Sell up factors are "
              << Utilities::vectorToString(iSellupFactorHolder) << "\n";
              
    // Find the class with the lowest yield
    double lQYield;
    Utilities::getMinimumElement (lQYield, iPriceHolder);
    // Debug
    oDebugStr << "The minimum yield is " << lQYield << "\n";

    // Initialize a holder for sell-up probabilities
    SellupProbabilityVector_T lSellupProbabilityVector;
    // Compute sell-up probability for each class/bucket & store it in a vector
    QForecaster::calculateSellupProbability
      (lSellupProbabilityVector, lQYield, iPriceHolder, iSellupFactorHolder);
    // Debug
    oDebugStr << "Sell-up Probabilities are " 
              << Utilities::vectorToString (lSellupProbabilityVector) << "\n";

    // Initialize a holder for Q-equivalent Demand Parameters
    QEquivalentDemandParameterHolder_T lQEquivalentDemandParameterHolder;
    // Calculate Q-equivalent demand distribution parameters.
    QForecaster::calculateQEquivalentDemandParameters 
     (lQEquivalentDemandParameterHolder, iHistoricalDataHolderHolder,
      lSellupProbabilityVector);
    // Debug
    oDebugStr << "Q-equivalent demand parameters [mean, SD] = " 
              << Utilities::vectorToString (lQEquivalentDemandParameterHolder)
              << "\n";

    // Calculate Q-equivalent demand distribution parameters and 
    // partition it to each class/bucket
    QForecaster::partitionQEquivalentDemandParameters 
     (ioForecastedDemandParameterList, lQEquivalentDemandParameterHolder, 
      lSellupProbabilityVector);
    // debug
    for (unsigned int k = 0; k < ioForecastedDemandParameterList.size(); k++) {
      std::vector<double> lMeanAndSDOfAClass = ioForecastedDemandParameterList.at(k);
      oDebugStr << "Q-equivalent distribution parameters per class " 
                << Utilities::vectorToString (lMeanAndSDOfAClass);
    }
    RMOL_LOG_DEBUG(oDebugStr.str());
  }
}