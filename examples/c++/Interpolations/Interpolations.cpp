#include <ql/quantlib.hpp>
#include <vector>

using namespace QuantLib;


int main(int, char* []) {

    Rate rates[] = {0.017075, 0.024162, 0.024917, 0.026772, 0.028352, 0.030251, 0.031486, 0.033520, 0.033043, 0.037103, 0.041529};
    Date dates[] = {Date(11, Jul, 2019),
                    Date(11, Oct, 2019),
                    Date(11, Jan, 2020),
                    Date(11, Jul, 2020),
                    Date(11, Jul, 2021),
                    Date(11, Jul, 2022),
                    Date(11, Jul, 2024),
                    Date(11, Jul, 2026),
                    Date(11, Jul, 2029),
                    Date(11, Jul, 2039),
                    Date(11, Jul, 2049)};
    Actual365Fixed dc;
    NullCalendar cal;


    std::vector<Rate> refRates(std::begin(rates), std::end(rates));
    std::vector<Date> refDates(std::begin(dates), std::end(dates));

    InterpolatedZeroCurve<ForwardFlat> curve(refDates, refRates, dc, cal, ForwardFlat(), Continuous, Annual);


    for(size_t i=0; i != refDates.size(); ++i) {
        Date date = refDates[i];
        std::cout << refDates[i] << ": " << curve.zeroRate(date, dc, Continuous) << std::endl;
    }

    return 0;
}