#include <iostream>
#include <ql/quantlib.hpp>

using namespace std;
using namespace QuantLib;

boost::shared_ptr<YieldTermStructure>
flatRate(const Date& today,
    const boost::shared_ptr<Quote>& forward,
    const DayCounter& dc) {
    return boost::shared_ptr<YieldTermStructure>(
        new FlatForward(today, Handle<Quote>(forward), dc));
}

int main() {

    const Size maturity = 2;
    DayCounter dc = Actual365Fixed();
    const Date today = Date::todaysDate();
    const Real upperBarrier = 1.02;
    const Real lowerBarrier = 0.86;

    Settings::instance().evaluationDate() = today;

    Handle<Quote> spot(boost::shared_ptr<Quote>(new SimpleQuote(1.)));
    boost::shared_ptr<SimpleQuote> qRate(new SimpleQuote(0.0));
    Handle<YieldTermStructure> qTS(flatRate(today, qRate, dc));
    boost::shared_ptr<SimpleQuote> rRate(new SimpleQuote(0.05));
    Handle<YieldTermStructure> rTS(flatRate(today, rRate, dc));
    boost::shared_ptr<SimpleQuote> sRate(new SimpleQuote(0.0));
    Handle<YieldTermStructure> sTS(flatRate(today, sRate, dc));
    Handle<BlackVolTermStructure> vTS(boost::shared_ptr<BlackVolTermStructure>(new BlackConstantVol(today, NullCalendar(), 0.15, dc)));

    const boost::shared_ptr<GeneralizedBlackScholesProcess> underlyingProcess(
            new GeneralizedBlackScholesProcess(spot, qTS, sTS, vTS));

    const Period monitorPeriod = Period(1, Weeks);

    Schedule schedule(today, today + Period(maturity, Years),
                      monitorPeriod, TARGET(),
                      Following, Following,
                      DateGeneration::Forward, false);

    std::vector<Time> times(schedule.size());
    std::transform(schedule.begin(), schedule.end(), times.begin(),
                   boost::bind(&Actual365Fixed::yearFraction,
                               dc, today, placeholders::_1, Date(), Date()));

    TimeGrid grid(times.begin(), times.end());

    std::vector<Real> redemption(times.size());

    for (Size i=0; i < redemption.size(); ++i) {
        redemption[i] = 1. + 0.16 * times[i];
    }

    typedef PseudoRandom::rsg_type rsg_type;
    typedef MultiPathGenerator<rsg_type>::sample_type sample_type;

    BigNatural seed = 42;
    rsg_type rsg = PseudoRandom::make_sequence_generator(grid.size()-1, seed);

    MultiPathGenerator<rsg_type> generator(underlyingProcess, grid, rsg, false);
    GeneralStatistics stat;

    Real antitheticPayoff=0;
    const Size nrTrails = 3000;
    for (Size i=0; i < nrTrails; ++i) {
        const bool antithetic = (i%2)==0 ? false : true;

        sample_type path = antithetic ? generator.antithetic()
                                      : generator.next();

        Real payoff=0;
        for (Size j=1; j < times.size(); ++j) {
            if (path.value[0][j] > upperBarrier * spot->value()) {
                payoff = redemption[j] * rTS->discount(times[j]);
                break;
            }
            else if (path.value[0][j] < lowerBarrier * spot->value())
            {
                payoff = path.value[0][j] * rTS->discount(times[j]);
                break;
            }
            else if (j == times.size() - 1) {
                payoff = redemption[j] * rTS->discount(times[j]);
            }
        }

        if (antithetic){
            stat.add(0.5*(antitheticPayoff + payoff));
        }
        else {
            antitheticPayoff = payoff;
        }
    }

    const Real calculated = stat.mean();
    const Real error = stat.errorEstimate();

    cout << "Calculated: " << calculated << "\nError esi.: " << error << endl;

    return 0;
}