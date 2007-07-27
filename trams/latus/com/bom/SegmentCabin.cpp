// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <assert.h>
// LATUS COM
#include <latus/com/bom/SegmentCabin.hpp>

namespace LATUS {

  namespace COM {

    // //////////////////////////////////////////////////////////////////////
    SegmentCabin::SegmentCabin (const SegmentCabinKey_T& iKey)
      : _key (iKey) {
    }
    
    // //////////////////////////////////////////////////////////////////////
    SegmentCabin::~SegmentCabin () {
    }

    // //////////////////////////////////////////////////////////////////////
    const std::string SegmentCabin::describeKey() const {
      return _key.describe();
    }
    
    // //////////////////////////////////////////////////////////////////////
    const std::string SegmentCabin::describeShortKey() const {
      return _key.describeShort();
    }
    
    // //////////////////////////////////////////////////////////////////////
    void SegmentCabin::display() const {

      // Store current formatting flags of std::cout
      std::ios::fmtflags oldFlags = std::cout.flags();

      std::cout << describeKey() << std::endl;

      // Reset formatting flags of std::cout
      std::cout.flags (oldFlags);
    }
    
    // //////////////////////////////////////////////////////////////////////
    LegCabin* SegmentCabin::
    getLegCabin (const std::string& iLegCabinKey) const {
      LegCabin* resultLegCabin_ptr = NULL;
      
      LegCabinList_T::const_iterator itLegCabin =
        _legCabinList.find (iLegCabinKey);

      if (itLegCabin != _legCabinList.end()) {
        resultLegCabin_ptr = itLegCabin->second;
      }

      return resultLegCabin_ptr;
    }

  }
}
