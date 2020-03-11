/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include <gtest/gtest.h>

#include <cmath>
#include <alt_math_utilities/PowFast.hpp>

using namespace std;
using namespace alt_math_utilities;

#define LENGTH(x) sizeof(x) / sizeof(x[0])

TEST(powfast, general_precision)
{
    double values[] = {-87, -25, -12, -1, -0.4234, 0, 0.4234, 1, 12, 25, 87};
    int count = LENGTH(values);
    
    double tolerance_rel = 1e-5;
    
    for (int i = 0; i < count; ++i)
    {
        double x = values[i];
        double y_expected = exp(x);
        double y_actual = POWFAST().e(x);
        
        double error_rel = fabs(y_expected - y_actual) / y_actual;
        EXPECT_LT(error_rel, tolerance_rel);
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
