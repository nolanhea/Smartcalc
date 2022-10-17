#include <math.h>

#include "gtest/gtest.h"
#include "smartcalc_model.h"
using s21::SmartcalcModel;

namespace {
TEST(SmartcalcModelTest, Functions_1) {
  SmartcalcModel my("sin(5)");
  double my_result = my.getResult();
  double correct_result = sin(5);
  EXPECT_EQ(fabs(my_result - correct_result) < 1e-6, 1);

  SmartcalcModel my_2("sin(5)+cos(5)+tan(5)+acos(0.5)+asin(0.5)+atan(0.5)");
  my_result = my_2.getResult();
  correct_result = sin(5) + cos(5) + tan(5) + acos(0.5) + asin(0.5) + atan(0.5);
  EXPECT_EQ(fabs(my_result - correct_result) < 1e-6, 1)
      << "my result: " << my_result << std::endl
      << "correct result: " << correct_result << std::endl
      << "is valid: " << my_2.getValid();
}
TEST(SmartcalcModelTest, Functions_2) {
  SmartcalcModel my_2("ln(5)+sqrt(5)+log(5)");
  double my_result = my_2.getResult();
  double correct_result = log10(5) + log(5) + sqrt(5);
  EXPECT_EQ(fabs(my_result - correct_result) < 1e-6, 1)
      << "my result: " << my_result << std::endl
      << "correct result: " << correct_result << std::endl
      << "is valid: " << my_2.getValid();
}
TEST(SmartcalcModelTest, Operators_1) {
  SmartcalcModel my_2("-(-2+4)/0.5");
  double my_result = my_2.getResult();
  double correct_result = -4;
  EXPECT_EQ(fabs(my_result - correct_result) < 1e-6, 1)
      << "my result: " << my_result << std::endl
      << "correct result: " << correct_result << std::endl
      << "is valid: " << my_2.getValid();
}
TEST(SmartcalcModelTest, Operators_2) {
  SmartcalcModel my_2("(5mod3)^-3");
  double my_result = my_2.getResult();
  double correct_result = 0.125;
  EXPECT_EQ(fabs(my_result - correct_result) < 1e-6, 1)
      << "my result: " << my_result << std::endl
      << "correct result: " << correct_result << std::endl
      << "is valid: " << my_2.getValid();
}
TEST(SmartcalcModelTest, Spaces) {
  SmartcalcModel my_2("sqrt(-5 + 5 + 16)");
  double my_result = my_2.getResult();
  double correct_result = 4;
  EXPECT_EQ(fabs(my_result - correct_result) < 1e-6, 1)
      << "my result: " << my_result << std::endl
      << "correct result: " << correct_result << std::endl
      << "is valid: " << my_2.getValid();
}
TEST(SmartcalcModelTest, SubstituteX) {
  SmartcalcModel my_2("sqrt(-5 + 5 + x)", 16);
  double my_result = my_2.getResult();
  double correct_result = 4;
  EXPECT_EQ(fabs(my_result - correct_result) < 1e-6, 1)
      << "my result: " << my_result << std::endl
      << "correct result: " << correct_result << std::endl
      << "is valid: " << my_2.getValid();
}
TEST(SmartcalcModelTest, IncorrectInput_1) {
  SmartcalcModel my_2("sqrt(-5 + 5 + x)");
  my_2.getResult();
  bool my_valid = my_2.getValid();
  EXPECT_EQ(my_valid, 0);
}
TEST(SmartcalcModelTest, IncorrectInput_2) {
  SmartcalcModel my_2("sqrt(-5 + 5 + x))");
  my_2.getResult();
  bool my_valid = my_2.getValid();
  EXPECT_EQ(my_valid, 0);
}
TEST(SmartcalcModelTest, IncorrectInput_3) {
  SmartcalcModel my_2("sqrt(-4)");
  my_2.getResult();
  bool my_valid = my_2.getValid();
  EXPECT_EQ(my_valid, 0);
}
}  // namespace
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
