#include <iostream>
#include <ql/quantlib.hpp>
#include <qlext/instruments/subperiodsswap.hpp>
#include <qlext/termstructures/yield/ratehelpers.hpp>

using namespace std;
using namespace QuantLib;

int main() {

    const Date refDate(19, Sep, 2017);
    China calendar(China::IB);
    Actual365Fixed dc;
    RelinkableHandle<YieldTermStructure> termStructure;
    termStructure.linkTo(boost::shared_ptr<YieldTermStructure>(new FlatForward(refDate, 0.05, dc)));

    boost::shared_ptr<IborIndex> index = boost::shared_ptr<IborIndex>(new Shibor(7 * Days));

    Date effectiveDate = refDate + index->fixingDays() * Days * 10;
    effectiveDate = calendar.adjust(effectiveDate);

    SubPeriodsSwap swap(effectiveDate, 1., 1 * Years, true, 3 * Months, 0.05, calendar, dc, ModifiedFollowing,
                        3 * Months, index, dc);

    swap.setPricingEngine(boost::shared_ptr<PricingEngine>(new DiscountingSwapEngine(termStructure)));
    cout << swap.NPV() << endl;

    return 0;
}