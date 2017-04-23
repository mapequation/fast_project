

rm -r bin
mkdir bin

CPPAndreaStdPackage='standard_package/standard_include.cpp'

g++ -std=c++0x -O3 fast_project.cpp  -include $CPPAndreaStdPackage -o ./bin/fast_project -Wall



