#include <iostream>
#include <ql/quantlib.hpp>
#include <qlext/indexes/repo.hpp>
#include <qlext/instruments/subperiodsswap.hpp>
#include <qlext/termstructures/yield/ratehelpers.hpp>

using namespace std;
using namespace QuantLib;

int main() {

    const Real tolerance = 1.e-12;
    const NullCalendar calendar;
    const Date today(11, Oct, 2017);
    Settings::instance().evaluationDate() = today;
    boost::shared_ptr<IborIndex> index(new Repo(1 * Weeks));
    const Date settlementDate = today + 1 * Days;

    const Actual365Fixed dc;
    std::vector<boost::shared_ptr<RateHelper> > subPeriodsInstruments = std::vector<boost::shared_ptr<RateHelper> >();

    // Instruments loner than 3 months
    Size length = 10;
    Rate rs[] = {0.034625, 0.034800, 0.034940, 0.035100, 0.035900, 0.036730, 0.037430, 0.0381875, 0.039240, 0.039450};
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

    boost::shared_ptr<IborIndex> forwardIndex = boost::shared_ptr<IborIndex>(new Repo(1 * Weeks, curveHandle));

    Date spotDate = today + forwardIndex->fixingDays() * Days;
    spotDate = calendar.adjust(spotDate);

    for (Size i = 0; i < subPeriodsInstruments.size(); ++i) {
        boost::shared_ptr<SubPeriodsSwap> swap(new SubPeriodsSwap(spotDate, 100., tenors[i], true, 3 * Months, 0., calendar, dc, ModifiedFollowing,
                                                                  3 * Months, forwardIndex, dc));

        Handle<YieldTermStructure> disc = forwardIndex->forwardingTermStructure();
        bool includeSettlementDateFlows = false;
        boost::shared_ptr<PricingEngine> engine(new DiscountingSwapEngine(disc, includeSettlementDateFlows));
        swap->setPricingEngine(engine);

        Rate expectedRate = rates[i]->value();
        Rate estimatedRate = swap->fairRate();
        Spread error = std::fabs(expectedRate - estimatedRate);

    }

    Date pillars[] = {Date(12, Jan, 2018), Date(12, Apr, 2018), Date(12, July, 2018), Date(12, Oct, 2018), Date(14, Oct, 2019), Date(12, Oct, 2020),
    Date(12, Oct, 2021), Date(12, Oct, 2022), Date(14, Oct, 2024), Date(12, Oct, 2027)};

    // BBG quote the discout factor from settlement date not today
    for (Size i = 0; i < length; ++i) {
        Date refDate = pillars[i];

        Real discountFactor = termStructure->discount(refDate) / termStructure->discount(settlementDate);
        Real zeroRate = InterestRate::impliedRate(1. / discountFactor, dc, Compounded, Quarterly, settlementDate, refDate);

        cout <<io::iso_date(refDate) << " : " << zeroRate << ", " << discountFactor << endl;
    }

    return 0;
}