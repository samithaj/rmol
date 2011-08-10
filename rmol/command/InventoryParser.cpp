// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
#include <fstream>
#include <cassert>
// StdAir
#include <stdair/stdair_inventory_types.hpp>
#include <stdair/stdair_maths_types.hpp>
#include <stdair/stdair_exceptions.hpp>
#include <stdair/basic/BasConst_DefaultObject.hpp>
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/BomRetriever.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/SegmentDate.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/LegDate.hpp>
#include <stdair/bom/LegCabin.hpp>
#include <stdair/bom/BookingClass.hpp>
#include <stdair/bom/VirtualClassStruct.hpp>
#include <stdair/factory/FacBom.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
// RMOL
#include <rmol/command/InventoryParser.hpp>

namespace RMOL {

  // ////////////////////////////////////////////////////////////////////
  stdair::LegCabin& InventoryParser::
  getSampleLegCabin (stdair::BomRoot& ioBomRoot) {
    stdair::LegCabin* oLegCabin_ptr = NULL;

    // Retrieve the Inventory
    const stdair::Inventory* lInventory_ptr = stdair::BomRetriever::
      retrieveInventoryFromKey (ioBomRoot, stdair::DEFAULT_AIRLINE_CODE);

    if (lInventory_ptr == NULL) {
      std::ostringstream oStr;
      oStr << "The inventory corresponding to the '"
           << stdair::DEFAULT_AIRLINE_CODE << "' airline can not be found";
      throw stdair::ObjectNotFoundException (oStr.str());
    }
    
    // Retrieve the FlightDate
    const stdair::FlightDate* lFlightDate_ptr = stdair::BomRetriever::
      retrieveFlightDateFromKey (*lInventory_ptr, stdair::DEFAULT_FLIGHT_NUMBER,
                                 stdair::DEFAULT_DEPARTURE_DATE);
    
    if (lFlightDate_ptr == NULL) {
      std::ostringstream oStr;
      oStr << "The flight-date corresponding to ("
           << stdair::DEFAULT_FLIGHT_NUMBER << ", "
           << stdair::DEFAULT_DEPARTURE_DATE << ") can not be found";
      throw stdair::ObjectNotFoundException (oStr.str());
    }

    // Retrieve the LegDate
    const stdair::LegDateKey lLegDateKey (stdair::DEFAULT_ORIGIN);
    const stdair::LegDate* lLegDate_ptr =
      lFlightDate_ptr->getLegDate (lLegDateKey);

    if (lLegDate_ptr == NULL) {
      std::ostringstream oStr;
      oStr << "The leg-date corresponding to the '"
           << stdair::DEFAULT_ORIGIN << "' origin can not be found";
      throw stdair::ObjectNotFoundException (oStr.str());
    }
    
    // Retrieve the LegCabin
    const stdair::LegCabinKey lLegCabinKey (stdair::DEFAULT_CABIN_CODE);
    oLegCabin_ptr = lLegDate_ptr->getLegCabin (lLegCabinKey);

    if (oLegCabin_ptr == NULL) {
      std::ostringstream oStr;
      oStr << "The leg-cabin corresponding to the '"
           << stdair::DEFAULT_CABIN_CODE << "' cabin code can not be found";
      throw stdair::ObjectNotFoundException (oStr.str());
    }
    
    assert (oLegCabin_ptr != NULL);
    return *oLegCabin_ptr;
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::SegmentCabin& InventoryParser::
  getSampleSegmentCabin (stdair::BomRoot& ioBomRoot) {
    stdair::SegmentCabin* oSegmentCabin_ptr = NULL;

    // Retrieve the segment-cabin.
    const stdair::InventoryList_T& lInventoryList =
      stdair::BomManager::getList<stdair::Inventory> (ioBomRoot);
    stdair::InventoryList_T::const_iterator itInv = lInventoryList.begin();
    assert (itInv != lInventoryList.end());
    const stdair::Inventory* lInventory_ptr = *itInv;
    assert (lInventory_ptr != NULL);

    const stdair::FlightDateList_T& lFlightDateList =
      stdair::BomManager::getList<stdair::FlightDate> (*lInventory_ptr);
    stdair::FlightDateList_T::const_iterator itFD = lFlightDateList.begin();
    assert (itFD != lFlightDateList.end());
    const stdair::FlightDate* lFD_ptr = *itFD;
    assert (lFD_ptr != NULL);

    const stdair::SegmentDateList_T& lSegmentDateList =
      stdair::BomManager::getList<stdair::SegmentDate> (*lFD_ptr);
    stdair::SegmentDateList_T::const_iterator itSD = lSegmentDateList.begin();
    assert (itSD != lSegmentDateList.end());
    const stdair::SegmentDate* lSD_ptr = *itSD;
    assert (lSD_ptr != NULL);

    const stdair::SegmentCabinList_T& lSegmentCabinList =
      stdair::BomManager::getList<stdair::SegmentCabin> (*lSD_ptr);
    stdair::SegmentCabinList_T::const_iterator itSegmentCabin =
      lSegmentCabinList.begin();
    assert (itSegmentCabin != lSegmentCabinList.end());
    oSegmentCabin_ptr = *itSegmentCabin;

    assert (oSegmentCabin_ptr != NULL);
    
    return *oSegmentCabin_ptr;
  }

  // ////////////////////////////////////////////////////////////////////
  bool InventoryParser::
  parseInputFileAndBuildBom (const std::string& iInputFileName,
                             stdair::BomRoot& ioBomRoot) {
    bool hasReadBeenSuccessful = false;

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iInputFileName);
    if (doesExistAndIsReadable == false) {
      std::ostringstream oMessage;
      oMessage << "The input file, '" << iInputFileName
               << "', can not be retrieved on the file-system";
      throw stdair::FileNotFoundException (oMessage.str());
    }

    // Retrieve the (sample) leg-cabin
    stdair::LegCabin& lLegCabin = getSampleLegCabin (ioBomRoot);

    // Retrieve the (sample) leg-cabin
    stdair::SegmentCabin& lSegmentCabin = getSampleSegmentCabin (ioBomRoot);

    // Open the input file
    std::ifstream inputFile (iInputFileName.c_str());
    if (! inputFile) {
      STDAIR_LOG_ERROR ("Can not open input file '" << iInputFileName << "'");
      throw new stdair::FileNotFoundException ("Can not open input file '"
                                               + iInputFileName + "'");
    }
    
    char buffer[80];
    double dval;
    short i = 1;
    bool hasAllPArams = true;
    stdair::Yield_T lYield;
    stdair::MeanValue_T lMean;
    stdair::StdDevValue_T lStdDev;
    stdair::BookingClassKey lBCKey (stdair::DEFAULT_CLASS_CODE);
    
    while (inputFile.getline (buffer, sizeof (buffer), ';')) {
      std::istringstream iStringStr (buffer);

      if (i == 1) {
        hasAllPArams = true;
      }
      
      if (iStringStr >> dval) {
        if (i == 1) {
          lYield = dval;
          // std::cout << "Yield[" << i << "] = '" << dval << "'" << std::endl;

        } else if (i == 2) {
          lMean = dval;
          // std::cout << "Mean[" << i << "] = '" << dval << "'" << std::endl;

        } else if (i == 3) {
          lStdDev = dval;
          //std::cout << "stdDev[" << i << "] = '" << dval << "'" << std::endl;
          i = 0;
        }
        i++;
          
      } else {
        hasAllPArams = false;
      }

      if (hasAllPArams && i == 1) {
        stdair::BookingClass& lBookingClass =
          stdair::FacBom<stdair::BookingClass>::instance().create (lBCKey);
        stdair::FacBomManager::addToList (lSegmentCabin, lBookingClass);
        lBookingClass.setYield (lYield);
        lBookingClass.setMean (lMean);
        lBookingClass.setStdDev (lStdDev);

        stdair::VirtualClassStruct lVirtualClass (lBookingClass);
        lVirtualClass.setYield (lYield);
        lVirtualClass.setMean (lMean);
        lVirtualClass.setStdDev (lStdDev);
        lLegCabin.addVirtualClass (lVirtualClass);
      }
    }
    
    //
    if (!inputFile.eof()) {
      STDAIR_LOG_ERROR ("Problem when reading input file '" << iInputFileName
                        << "'");
      return hasReadBeenSuccessful;
    }

    //
    hasReadBeenSuccessful = true;
    return hasReadBeenSuccessful;
  }
  
}