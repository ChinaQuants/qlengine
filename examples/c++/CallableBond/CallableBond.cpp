#include <ql/quantlib.hpp>
#include <vector>

using namespace QuantLib;

FixedRateBond bond(Date issueDate, Date maturity, Period tenor, Calendar cal,
                   BusinessDayConvention accrConv, Natural settlementDays, Real faceAmount,
                   Rate coupon, DayCounter dc) {
	Schedule sch(issueDate, maturity, tenor, cal, accrConv, accrConv, DateGeneration::Backward, false);
    std::vector<Rate> coupons(1, coupon);
    return FixedRateBond(settlementDays, faceAmount, sch, coupons, dc);
}

CallabilitySchedule callSchedule(Date issueDate, Date maturity, Calendar cal, Real callPrice) {
    Bond::Price price(callPrice, Bond::Price::Clean);
    Date callDate = cal.advance(issueDate, 1 * Years);
    CallabilitySchedule sch;
    while(callDate < maturity) {
        sch.push_back(ext::make_shared<Callability>(price, Callability::Call, callDate));
        callDate = cal.advance(callDate, 1 * Years);
    }  
    return sch;
}

CallableFixedRateBond callableBond(Date issueDate, Date maturity, Period tenor, Calendar cal,
                   BusinessDayConvention accrConv, Natural settlementDays, Real faceAmount,
                   Rate coupon, DayCounter dc, Real callPrice) {
    Schedule sch(issueDate, maturity, tenor, cal, accrConv, accrConv, DateGeneration::Backward, false);
    CallabilitySchedule callSch = callSchedule(issueDate, maturity, cal, callPrice);
    std::vector<Rate> coupons(1, coupon);
    return CallableFixedRateBond(settlementDays, faceAmount, sch, coupons, dc, accrConv, faceAmount, issueDate, callSch);
}

int main(int, char* []) {

    Date dates[] = {Date(16, Sep, 2014),
                    Date(16, Sep, 2015),
                    Date(16, Sep, 2016),
                    Date(16, Sep, 2017),
                    Date(16, Sep, 2018),
                    Date(16, Sep, 2019),
                    Date(16, Sep, 2020),
                    Date(16, Sep, 2021)};
    Rate ytms[] = {0.00, 0.040, 0.043, 0.045, 0.049, 0.039, 0.035, 0.030};

    std::vector<Date> spotDates(std::begin(dates), std::end(dates));
    std::vector<Rate> spotRates(std::begin(ytms), std::end(ytms));

    Thirty360 dc(Thirty360::BondBasis);
    NullCalendar cal;
    Linear interp;
    Compounding comp = Compounding::Compounded;
    Frequency freq = Frequency::Annual;

    Handle<YieldTermStructure> ts(ext::make_shared<InterpolatedZeroCurve<Linear> >(spotDates, spotRates, dc, cal, interp, comp, freq));

	// Bond definitions
    Date issueDate(16, Sep, 2014);
    Settings::instance().evaluationDate() = issueDate;
    Period tenor(1 * Years);
    BusinessDayConvention accrConv = BusinessDayConvention::Unadjusted;
    Natural SettlementDays = 0;
    Real faceAmount = 100.;
    Real callPrice = 100.;
    Real coupon = 0.040;
    Date maturity = spotDates[spotDates.size()-2];
    
    ext::shared_ptr<HullWhite> model = ext::make_shared<HullWhite>(ts, 0.03, 0.01);
    
    CallableFixedRateBond bond = callableBond(issueDate, maturity, tenor, cal, accrConv, SettlementDays, faceAmount, coupon, dc, callPrice);

    Natural grids[] = {1, 2, 4, 8, 16, 32, 64, 128};
    std::vector<Natural> trees(std::begin(grids), std::end(grids));
    for(size_t i=0; i != trees.size(); ++i) {
        Natural grid = trees[i];
        ext::shared_ptr<PricingEngine> engine 
                            = ext::make_shared<TreeCallableFixedRateBondEngine>(model, grid);
        bond.setPricingEngine(engine);
        std::cout << grid << ": " << bond.NPV() << std::endl;
    }

    return 0;
}
