echo "--------------------------------------------------------------------------------"
echo "Running complex tests"
echo "-----------------------"

echo "1 real"

echo "Input: 3.1415926535"
./check_complex 3.1415926535

echo "Input: \"3.1415926535\""
./check_complex "3.1415926535"

echo "Input: \"[3.1415926535]\""
./check_complex "[3.1415926535]"

echo "Input: \"3.1415926535 +/- 1.3e-14\""
./check_complex "3.1415926535 +/- 1.3e-14"

echo "Input: \"[3.1415926535 +/- 1.3e-14]\""
./check_complex "[3.1415926535 +/- 1.3e-14]"

echo "Input: \"[3.1415926535+/-1.3e-14\""
./check_complex "[3.1415926535+/-1.3e-14"

echo "Input: \"3.1415926535+/-1.3e-14]\""
./check_complex "3.1415926535+/-1.3e-14]"

echo "Input: \"3.1415926535+/-1.3e-14\""
./check_complex "3.1415926535+/-1.3e-14"


echo
echo "1 imag"
echo "Input: \"0 2\""
./check_complex "0 2"

echo "Input: \"0 2 +/- 1.3e-14\""
./check_complex "0 2 +/- 1.3e-14"

echo
echo "1 real 1 imag"
echo "Input: \"3.1415926535 2\""
./check_complex "3.1415926535 2"

echo "Input: \"[3.1415926535 2]\""
./check_complex "[3.1415926535 2]"

echo "Input: \"[3.1415926535] [2]\""
./check_complex "[3.1415926535] [2]"

echo "Input: \"3.1415926535 +/- 1.3e-14 2\""
./check_complex "3.1415926535 +/- 1.3e-14 2"

echo "Input: \"3.1415926535 2 +/- 1.3e-14\""
./check_complex "3.1415926535 2 +/- 1.3e-14"

echo
echo "1 real 1 imag - both have var"
echo "Input: \"3.1415926535 +/- 1.3e-14 2 +/- 4.7e-15\""
./check_complex "3.1415926535 +/- 1.3e-14 2 +/- 4.7e-15"

echo "Input: \"[3.1415926535 +/- 1.3e-14] [2 +/- 4.7e-15]\""
./check_complex "[3.1415926535 +/- 1.3e-14] [2 +/- 4.7e-15]"

echo
echo "Some invalid inputs:"
echo "Input: \"1 2 3\""
./check_complex "1 2 3"


echo "Input: \"1 2 3 4\""
./check_complex "1 2 3 4"

echo "Input: \"1 2 3 4 5\""
./check_complex "1 2 3 4 5"

echo "Input: \"1 +/- +/-\""
./check_complex "1 +/- +/-"

echo "Input: \"+/- 1\""
./check_complex "+/- 1"

echo "Input: \"1 +/-\""
./check_complex "1 +/-"

echo "Input: \"1+/-\""
./check_complex "1+/-"

