#include <opencv2/core.hpp>
#include <iostream>
int main() {
    std::cout << cv::getBuildInformation() << std::endl;
    return 0;
}
