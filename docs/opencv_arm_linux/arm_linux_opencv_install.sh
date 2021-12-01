#! /bin/bash

bashrc=~/.bashrc
cv_version=4.5.4
test_file=~/test_arm_opencv

cd ~

echo 'Checking cmake...'
sudo yum install -y cmake

echo -e "\033[36m########################################"
echo -e "#            Download Start            #"
echo -e "########################################\033[0m"

if [ -f "${cv_version}.zip" ]; then
    echo "Already downloaded ${cv_version}.zip"
else
    wget "https://github.com/opencv/opencv/archive/${cv_version}.zip"
fi

echo -e "\033[32m########################################"
echo -e "#            Download Finish           #"
echo -e "########################################\033[0m"

echo -e "\033[36m########################################"
echo -e "#            Build Start               #"
echo -e "#            May cost 1h+ time         #"
echo -e "########################################\033[0m"

unzip "${cv_version}.zip"
cd "opencv-${cv_version}/"
mkdir build
cd build
cmake .. -D CMAKE_BUILD_TYPE=Release -D OPENCV_GENERATE_PKGCONFIG=ON
sudo make install -j2

sudo chmod -R 755 /usr/local/include/opencv4

diff_res=$(diff <(echo '4.5.4') <(opencv_version))
if [[ -z "${diff_res}" ]]; then
    echo -e "\033[32m########################################"
    echo -e "#            Build Pass                 #"
    echo -e "########################################\033[0m"
else
    echo -e "\033[31m########################################"
    echo -e "#            Build Fail                 #"
    echo -e "########################################\033[0m"
fi

echo -e "\033[36m########################################"
echo -e "#            Setting Envs              #"
echo -e "########################################\033[0m"

cd ~

if [ ! -f ${bashrc} ]; then
    touch ${bashrc}
fi

echo '# opencv envs' >> ${bashrc}
echo 'export C_INCLUDE_PATH="/usr/local/include/opencv4:$C_INCLUDE_PATH"' >> ${bashrc}
echo 'export CPLUS_INCLUDE_PATH="/usr/local/include/opencv4:$CPLUS_INCLUDE_PATH"' >> ${bashrc}
echo 'export LIBRARY_PATH="/usr/local/lib64/:$LIBRARY_PATH"' >> ${bashrc}
echo 'export LD_LIBRARY_PATH="/usr/local/lib64/:$LD_LIBRARY_PATH"' >> ${bashrc}
source ${bashrc}

echo -e "\033[32m########################################"
echo -e "#            Envs Set                  #"
echo -e "########################################\033[0m"

echo -e "\033[36m########################################"
echo -e "#            Test Start                #"
echo -e "########################################\033[0m"

echo 'Compiling and running test...'

if [ -f "${test_file}.cpp" ]; then
    rm "${test_file}.cpp"
fi

touch "${test_file}.cpp"
echo '#include <opencv2/opencv.hpp>' >> "${test_file}.cpp"
echo 'int main() { cv::Mat m = (cv::Mat_<int>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9); std::cout<<m<<std::endl; cv::Mat img = cv::imread("./fake_file.jpg"); return 0; }' >> "${test_file}.cpp"
g++ "${test_file}.cpp" -o "${test_file}.out" -lopencv_core -lopencv_highgui -lopencv_imgcodecs

diff_res=$(diff <(echo -e '[1, 2, 3;\n 4, 5, 6;\n 7, 8, 9]') <(${test_file}.out 2>&1))
if [[ -z "${diff_res}" ]]; then
    echo -e "\033[32m########################################"
    echo -e "#            Test Pass                 #"
    echo -e "########################################\033[0m"
else
    echo -e "\033[31m########################################"
    echo -e "#            Test Fail                 #"
    echo -e "########################################\033[0m"
fi

rm -f "${test_file}.cpp" "${test_file}.out"
rm -f "${cv_version}.zip"

echo "----------Finished----------"
