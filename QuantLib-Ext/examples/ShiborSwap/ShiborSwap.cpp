#include <iostream>
#include <ql/quantlib.hpp>
#include <ql/termstructures/yield/ratehelpers.hpp>
#include <qlext/indexes/repo.hpp>
#include <qlext/termstructures/yield/ratehelpers.hpp>

using namespace std;
using namespace QuantLib;

int main() {

    const Real tolerance = 1.e-12;
    const NullCalendar calendar;
    const Date today(11, Oct, 2017);
    Settings::instance().evaluationDate() = today;
    boost::shared_ptr<Shibor> index(new Shibor(3 * Months));
    const Date settlementDate = today + 1 * Days;

    const Actual365Fixed dc;
    std::vector<boost::shared_ptr<RateHelper> > subPeriodsInstruments = std::vector<boost::shared_ptr<RateHelper> >();

    // Instruments loner than 3 months
    Size length = 10;
    Rate rs[] = {0.034625, 0.034800, 0.034940, 0.035100, 0.035900, 0.036730, 0.037430, 0.0381875, 0.039240, 0.039450};
    Period tenors[] = {3*Months, 6*Months, 9*Months, 1*Years, 2*Years, 3*Years, 4*Years, 5*Years, 7*Years, 10*Years};

    std::vector<boost::shared_ptr<SimpleQuote> > rates(length);

    // Deposit loans
    subPeriodsInstruments.push_back(
        boost::shared_ptr<DepositRateHelper>(new DepositRateHelper(0.0275, 1 * Days, 1, calendar, Following, false, dc)));

    subPeriodsInstruments.push_back(
        boost::shared_ptr<DepositRateHelper>(new DepositRateHelper(0.0344, 7 * Days, 1, calendar, Following, false, dc)));

    for (Size i = 0; i < rates.size(); ++i)
        rates[i] = boost::shared_ptr<SimpleQuote>(new SimpleQuote(rs[i]));

    for (Size i = 0; i < rates.size(); ++i) {
        Handle<Quote> r(rates[i]);
        subPeriodsInstruments.push_back(
            boost::shared_ptr<ShiborSwapRateHelper>(new ShiborSwapRateHelper(r, tenors[i], Quarterly, index)));
    }

    boost::shared_ptr<YieldTermStructure> termStructure =
            boost::shared_ptr<YieldTermStructure>(new PiecewiseYieldCurve<ZeroYield, Linear, IterativeBootstrap>(
                    1, calendar, subPeriodsInstruments, dc, Linear()));

    RelinkableHandle<YieldTermStructure> curveHandle;
    curveHandle.linkTo(termStructure);

    boost::shared_ptr<Shibor> forwardIndex = boost::shared_ptr<Shibor>(new Shibor(3 * Months));

    Date spotDate = today + forwardIndex->fixingDays() * Days;

    Date pillars[] = {Date(13, Oct, 2017), Date(19, Oct, 2017), Date(12, Jan, 2018), Date(12, Apr, 2018), Date(12, July, 2018), Date(12, Oct, 2018), Date(14, Oct, 2019), Date(12, Oct, 2020),
    Date(12, Oct, 2021), Date(12, Oct, 2022), Date(14, Oct, 2024), Date(12, Oct, 2027)};

    // BBG quote the discout factor from settlement date not today
    for (Size i = 0; i < 12; ++i) {
        Date refDate = pillars[i];

        Real discountFactor = termStructure->discount(refDate);
        Real zeroRate = 0;

        if (i < 2)
            zeroRate = termStructure->zeroRate(refDate, dc, Simple, Annual);
        else
            zeroRate = termStructure->zeroRate(refDate, dc, Compounded, Quarterly);

        cout <<io::iso_date(refDate) << " : " << zeroRate << ", " << discountFactor << endl;
    }

    return 0;
}