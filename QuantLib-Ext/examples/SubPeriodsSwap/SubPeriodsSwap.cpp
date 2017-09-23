#include <iostream>
#include <ql/quantlib.hpp>
#include <qlext/indexes/repo.hpp>
#include <qlext/instruments/subperiodsswap.hpp>
#include <qlext/termstructures/yield/ratehelpers.hpp>

using namespace std;
using namespace QuantLib;

int main() {

    Real tolerance = 1.e-9;
    TARGET calendar;
    Date today(18, Sep ,2017);
    Settings::instance().evaluationDate() = today;
    boost::shared_ptr<IborIndex> index(new Repo(7 * Days));

    Actual365Fixed dc;

    Settings::instance().evaluationDate() = today;
    std::vector<boost::shared_ptr<RateHelper> > subPeriodsInstruments = std::vector<boost::shared_ptr<RateHelper> >();

    // Instruments less than one year
    Size length = 10;
    Rate rs[] = {0.034625, 0.0342, 0.0343,  0.03445, 0.03525, 0.0365, 0.0366, 0.037725, 0.03835, 0.03835};
    Period tenors[] = {3*Months, 6*Months, 9*Months, 1*Years, 2*Years, 3*Years, 4*Years, 5*Years, 7*Years, 10*Years};

    std::vector<boost::shared_ptr<SimpleQuote> > rates(length);

    for (Size i = 0; i < rates.size(); ++i)
        rates[i] = boost::shared_ptr<SimpleQuote>(new SimpleQuote(rs[i]));

    for (Size i = 0; i < rates.size(); ++i) {
        Handle<Quote> r(rates[i]);
        subPeriodsInstruments.push_back(
            boost::shared_ptr<SubPeriodsSwapRateHelper>(new SubPeriodsSwapRateHelper(r, tenors[i], Quarterly, calendar, dc, ModifiedFollowing,
                3 * Months, index, dc)));
    }

    boost::shared_ptr<YieldTermStructure> termStructure =
            boost::shared_ptr<YieldTermStructure>(new PiecewiseYieldCurve<Discount, Linear, IterativeBootstrap>(
                    today, subPeriodsInstruments, dc, Linear()));

    RelinkableHandle<YieldTermStructure> curveHandle;
    curveHandle.linkTo(termStructure);

    boost::shared_ptr<IborIndex> forwardIndex = boost::shared_ptr<IborIndex>(new Repo(7 * Days, curveHandle));

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
        Date refDate = today + tenors[i];
        refDate = calendar.adjust(refDate);
        cout <<io::iso_date(refDate) << " : " << termStructure->zeroRate(refDate, dc, Continuous).rate() << ", " << termStructure->discount(refDate) << endl;
    }

    return 0;
}