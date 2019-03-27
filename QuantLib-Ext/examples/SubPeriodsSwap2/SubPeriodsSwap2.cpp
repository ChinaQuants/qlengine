#include <iostream>
#include <ql/quantlib.hpp>
#include <ql/termstructures/yield/ratehelpers.hpp>
#include <qlext/indexes/repo.hpp>
#include <qlext/termstructures/yield/ratehelpers.hpp>
#include <qlext/instruments/subperiodsswap.hpp>

using namespace std;
using namespace QuantLib;

int main() {
    /*********************
    ***  MARKET DATA  ***
    *********************/
    const NullCalendar calendar;
    Date valuationDate(15, Feb, 2019);
    valuationDate = calendar.adjust(valuationDate);
    Integer fixingDays = 1;
    Date todaysDate = calendar.advance(valuationDate, -fixingDays, Days);
    Settings::instance().evaluationDate() = todaysDate;
    boost::shared_ptr<IborIndex> index(new Repo(7 * Days));

    std::cout << "Today: " << todaysDate.weekday() << ", " << todaysDate << std::endl;
    std::cout << "Settlement date: " << valuationDate.weekday() << ", " << valuationDate << std::endl;

    const Actual365Fixed dc;
    std::vector<boost::shared_ptr<RateHelper> > subPeriodsInstruments = std::vector<boost::shared_ptr<RateHelper> >();

    // Instruments longer than 3 months swaps
    Rate s3mQuote = 0.0245875;
    Rate s6mQuote = 0.024600;
    Rate s9mQuote = 0.024600;
    Rate s1yQuote = 0.024750;
    Rate s2yQuote = 0.025150;
    Rate s3yQuote = 0.026110;
    Rate s4yQuote = 0.0272510;
    Rate s5yQuote = 0.028250;
    Rate s7yQuote = 0.029550;
    Rate s10yQuote = 0.03140;

    Size length = 10;
    Rate rs[] = {s3mQuote, s6mQuote, s9mQuote, s1yQuote, s2yQuote, s3yQuote, s4yQuote, s5yQuote, s7yQuote, s10yQuote};
    Period tenors[] = {3*Months, 6*Months, 9*Months, 1*Years, 2*Years, 3*Years, 4*Years, 5*Years, 7*Years, 10*Years};

    std::vector<boost::shared_ptr<SimpleQuote> > rates(length);

    // Deposit loans
    Rate d1DQuote = 0.0174;
    Rate d7DQuote = 0.024;

    subPeriodsInstruments.push_back(
        boost::shared_ptr<DepositRateHelper>(new DepositRateHelper(d1DQuote, 1 * Days, 1, calendar, Following, false, dc)));

    subPeriodsInstruments.push_back(
        boost::shared_ptr<DepositRateHelper>(new DepositRateHelper(d7DQuote, 7 * Days, 1, calendar, Following, false, dc)));

    for (Size i = 0; i < rates.size(); ++i)
        rates[i] = boost::shared_ptr<SimpleQuote>(new SimpleQuote(rs[i]));

    for (Size i = 0; i < rates.size(); ++i) {
        Handle<Quote> r(rates[i]);
        subPeriodsInstruments.push_back(
            boost::shared_ptr<SubPeriodsSwapRateHelper>(new SubPeriodsSwapRateHelper(r, tenors[i], Quarterly, calendar, dc, ModifiedFollowing,
                3 * Months, index, dc)));
    }

    boost::shared_ptr<YieldTermStructure> termStructure =
            boost::shared_ptr<YieldTermStructure>(new PiecewiseYieldCurve<ZeroYield, Linear, IterativeBootstrap>(
                    1, calendar, subPeriodsInstruments, dc, Linear()));

    // Term structures that will be used for pricing:
    // the one used for discounting cash flows
    RelinkableHandle<YieldTermStructure> discountingTermStructure;
    // the one used for forward rate forecasting
    RelinkableHandle<YieldTermStructure> forecastingTermStructure;

    boost::shared_ptr<IborIndex> forwardIndex = boost::shared_ptr<IborIndex>(new Repo(7 * Days, forecastingTermStructure));

    /*********************
    * SWAPS TO BE PRICED *
    **********************/

    // constant nominal 1,000,000 
    Real nominal = 1000000.0;
    Calendar cdr = China(China::IB);
    Integer lenghtInYears = 3;
    VanillaSwap::Type swapType = VanillaSwap::Payer;
    Date settlementDate(3, September, 2018);
    Date maturity = settlementDate + lenghtInYears * Years;

    // fixed leg
    Period fixedLegFrequency = 3 * Months;
    BusinessDayConvention fixedLegConvention = ModifiedFollowing;
    DayCounter fixedLegDayCounter = Actual365Fixed();
    Rate fixedRate = 0.02611;

    // floating leg
    Period floatingLegFrequency = 3 * Months;
    BusinessDayConvention floatingLegConvention = ModifiedFollowing;
    DayCounter floatingLegDayCounter = Actual365Fixed();

    //forwardIndex->addFixing(Date(30, November, 2018), 0.028);
    forwardIndex->addFixing(Date(2, December, 2018), 0.028); // new
    //forwardIndex->addFixing(Date(7, December, 2018), 0.0261);
    forwardIndex->addFixing(Date(9, December, 2018), 0.0261); // new
    //forwardIndex->addFixing(Date(14, December, 2018), 0.028);
    forwardIndex->addFixing(Date(16, December, 2018), 0.028); // new
    //forwardIndex->addFixing(Date(21, December, 2018), 0.0263);
    forwardIndex->addFixing(Date(23, December, 2018), 0.0263); // new
    forwardIndex->addFixing(Date(30, December, 2018), 0.0296);
    //forwardIndex->addFixing(Date(4, January, 2019), 0.0248);
    forwardIndex->addFixing(Date(6, January, 2019), 0.0248); // new
    //forwardIndex->addFixing(Date(11, January, 2019), 0.0253);
    forwardIndex->addFixing(Date(13, January, 2019), 0.0253); // new
    //forwardIndex->addFixing(Date(18, January, 2019), 0.026);
    forwardIndex->addFixing(Date(20, January, 2019), 0.026); // new
    //forwardIndex->addFixing(Date(25, January, 2019), 0.026);
    forwardIndex->addFixing(Date(27, January, 2019), 0.026); // new
    forwardIndex->addFixing(Date(3, February, 2019), 0.0233);
    forwardIndex->addFixing(Date(10, February, 2019), 0.0233); // new

    // Swap Object
    SubPeriodsSwap spot3YearSwap(settlementDate, nominal, lenghtInYears * Years, true,
        fixedLegFrequency, fixedRate,
        cdr, fixedLegDayCounter, fixedLegConvention,
        floatingLegFrequency, forwardIndex, floatingLegDayCounter,
        DateGeneration::Forward);

    /***************
    * SWAP PRICING *
    ****************/

    // utilities for reporting
    std::vector<std::string> headers(4);
    headers[0] = "term structure";
    headers[1] = "net present value";
    headers[2] = "fair spread";
    headers[3] = "fair fixed rate";
    std::string separator = " | ";
    Size width = headers[0].size() + separator.size() + headers[1].size() + separator.size() + headers[2].size() +
        separator.size() + headers[3].size() + separator.size() - 1;
    std::string rule(width, '-'), dblrule(width, '=');
    std::string tab(8, ' ');

    // calculations
    std::cout << dblrule << std::endl;
    std::cout << "3-year market swap-rate = " << std::setprecision(4) << io::rate(s3yQuote) << std::endl;
    std::cout << dblrule << std::endl;

    std::cout << tab << "3-years swap paying " << io::rate(fixedRate) << std::endl;
    std::cout << headers[0] << separator << headers[1] << separator << headers[2] << separator << headers[3]
        << separator << std::endl;
    std::cout << rule << std::endl;

    Real NPV;
    Rate fairRate;
    Spread fairSpread = 0.0;

    boost::shared_ptr<PricingEngine> swapEngine(new DiscountingSwapEngine(discountingTermStructure));

    forecastingTermStructure.linkTo(termStructure);
    discountingTermStructure.linkTo(termStructure);

    spot3YearSwap.setPricingEngine(swapEngine);

    NPV = spot3YearSwap.NPV();
    // fairSpread = spot3YearSwap.fairSpread();
    fairRate = spot3YearSwap.fairRate();

    std::cout << std::setw(headers[0].size()) << "depo-swap" << separator;
    std::cout << std::setw(headers[1].size()) << std::fixed << std::setprecision(4) << NPV << separator;
    std::cout << std::setw(headers[2].size()) << io::rate(fairSpread) << separator;
    std::cout << std::setw(headers[3].size()) << io::rate(fairRate) << separator;
    std::cout << std::endl;


    return 0;
}