#include <iostream>
#include <ql/time/calendars/china.hpp>
#include <ql/time/daycounters/actual365fixed.hpp>
#include <qlext/instruments/bonds/ctbzerobond.hpp>

using namespace std;
using namespace QuantLib;

int main() {

	China cal(China::SSE);
	Date issueDate(2, Feb, 2017);
	Date maturityDate(2, Feb, 2018);

	Actual365Fixed dc;

	CTBZeroBond bond(0, cal, 100., 85., issueDate, dc, maturityDate);

	cout << bond.accruedAmount() << endl;


    return 0;
}