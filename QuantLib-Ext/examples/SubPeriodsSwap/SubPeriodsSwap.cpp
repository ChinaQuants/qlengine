#include <iostream>
#include <ql/quantlib.hpp>
#include <qlext/instruments/subperiodsswap.hpp>
#include <qlext/termstructures/yield/ratehelpers.hpp>

using namespace std;
using namespace QuantLib;

int main() {

    Real tolerance = 1.e-9;
    China calendar(China::IB);
    Date today = calendar.adjust(Date::todaysDate());
    Settings::instance().evaluationDate() = today;
    boost::shared_ptr<IborIndex> index(new Shibor(7 * Days));

    Actual365Fixed dc;

    Settings::instance().evaluationDate() = today;

    Size length = 7;
    Rate rs[] = {0.03445, 0.03525, 0.0365, 0.0366, 0.037725, 0.03835, 0.03835};
    Integer years[] = {1, 2, 3, 4, 5, 7, 10};

    std::vector<boost::shared_ptr<SimpleQuote> > rates(length);
    std::vector<Period> tenors(length);

    for (Size i = 0; i < rates.size(); ++i) {
        rates[i] = boost::shared_ptr<SimpleQuote>(new SimpleQuote(rs[i]));
        tenors[i] = years[i] * Years;
    }

    std::vector<boost::shared_ptr<RateHelper> > subPeriodsInstruments = std::vector<boost::shared_ptr<RateHelper> >(length);

    for (Size i = 0; i < rates.size(); ++i) {
        Handle<Quote> r(rates[i]);
        subPeriodsInstruments[i] =
                boost::shared_ptr<SubPeriodsSwapRateHelper>(new SubPeriodsSwapRateHelper(r, tenors[i], Quarterly, calendar, dc, ModifiedFollowing,
                                                                                         3 * Months, index, dc));
    }

    boost::shared_ptr<YieldTermStructure> termStructure =
            boost::shared_ptr<YieldTermStructure>(new PiecewiseYieldCurve<Discount, Linear, IterativeBootstrap>(
                    today, subPeriodsInstruments, Actual365Fixed(), Linear()));

    RelinkableHandle<YieldTermStructure> curveHandle;
    curveHandle.linkTo(termStructure);

    boost::shared_ptr<IborIndex> forwardIndex = boost::shared_ptr<IborIndex>(new Shibor(7 * Days, curveHandle));

    Date spotDate = today + forwardIndex->fixingDays() * Days;
    spotDate = calendar.adjust(spotDate);

    for (Size i = 0; i < subPeriodsInstruments.size(); ++i) {
        boost::shared_ptr<SubPeriodsSwap> swap(new SubPeriodsSwap(spotDate, 1., tenors[i], true, 3 * Months, 0., calendar, dc, ModifiedFollowing,
                                                                  3 * Months, forwardIndex, dc));

        Handle<YieldTermStructure> disc = forwardIndex->forwardingTermStructure();
        bool includeSettlementDateFlows = false;
        boost::shared_ptr<PricingEngine> engine(new DiscountingSwapEngine(disc, includeSettlementDateFlows));
        swap->setPricingEngine(engine);

        Rate expectedRate = rates[i]->value();
        Rate estimatedRate = swap->fairRate();
        Spread error = std::fabs(expectedRate - estimatedRate);

    }

    for (Size i = 0; i < length; ++i) {
        cout << "Year " << years[i] << " : " << termStructure->zeroRate(years[i], Continuous) << endl;
    }

    return 0;
}