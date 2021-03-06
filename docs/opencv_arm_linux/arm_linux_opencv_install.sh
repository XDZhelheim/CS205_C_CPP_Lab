#! /bin/bash

bashrc=~/.bashrc
cv_version=4.5.4
test_file=~/test_arm_opencv

cd ~

function check_cmake() {
    echo 'Checking cmake...'
    sudo yum install -y cmake

    return 0
}

function download() {
    echo -e "\033[36m########################################"
    echo -e "#            Download Start            #"
    echo -e "########################################\033[0m"

    if [[ -n $(opencv_version 2>/dev/null) ]]; then
        echo -e "\033[33mAlready installed.\033[0m"
    else
        if [ -f "${cv_version}.zip" ]; then
            echo "Already downloaded ${cv_version}.zip"
        else
            wget "https://github.com/opencv/opencv/archive/${cv_version}.zip"
        fi
    fi

    echo -e "\033[32m########################################"
    echo -e "#            Download Finish           #"
    echo -e "########################################\033[0m"

    return 0
}

function build() {
    echo -e "\033[36m########################################"
    echo -e "#            Build Start               #"
    echo -e "#            May cost 40+ min          #"
    echo -e "########################################\033[0m"

    if [[ -n $(opencv_version 2>/dev/null) ]]; then
        echo -e "\033[33mAlready installed.\033[0m"
    else
        if [ -d "opencv-${cv_version}/" ]; then
            rm -r "opencv-${cv_version}/"
        fi

        unzip "${cv_version}.zip"
        cd "opencv-${cv_version}/"
        mkdir build
        cd build
        cmake .. -DCMAKE_BUILD_TYPE=Release -DOPENCV_GENERATE_PKGCONFIG=ON
        sudo make install -j2
    fi

    sudo chmod -R 755 /usr/local/include/opencv4

    if [[ -n $(opencv_version 2>/dev/null) ]]; then
        echo -e "\033[32m########################################"
        echo -e "#            Build Pass                #"
        echo -e "########################################\033[0m"
    else
        echo -e "\033[31m########################################"
        echo -e "#            Build Fail                #"
        echo -e "########################################\033[0m"
        exit 1
    fi

    return 0
}

function set_env() {
    echo -e "\033[36m########################################"
    echo -e "#            Setting Envs              #"
    echo -e "########################################\033[0m"

    chmod -R 755 /usr/local/lib64/cmake/

    echo 'Setting envs for root...'
    cd ~

    if [ ! -f ${bashrc} ]; then
        touch ${bashrc}
    fi
    
    if [[ -z "$(cat ${bashrc} | grep /usr/local/lib64/)" ]] || [[ -z "$(cat ${bashrc} | grep /usr/local/include/opencv4)" ]]; then
        echo '# opencv envs' >> ${bashrc}
        echo 'export C_INCLUDE_PATH="/usr/local/include/opencv4:$C_INCLUDE_PATH"' >> ${bashrc}
        echo 'export CPLUS_INCLUDE_PATH="/usr/local/include/opencv4:$CPLUS_INCLUDE_PATH"' >> ${bashrc}
        echo 'export LIBRARY_PATH="/usr/local/lib64/:$LIBRARY_PATH"' >> ${bashrc}
        echo 'export LD_LIBRARY_PATH="/usr/local/lib64/:$LD_LIBRARY_PATH"' >> ${bashrc}
    else
        echo -e "\033[33mEnvironment already satisfied.\033[0m"
    fi
    source ${bashrc}

    if [[ -n "$(ls /home)" ]]; then
        for user in $(ls /home); do
        echo "Setting envs for user ${user}..."
            cd /home/${user}

            if [ ! -f .bashrc ]; then
                touch .bashrc
            fi

            if [[ -z "$(cat .bashrc | grep /usr/local/lib64/)" ]] || [[ -z "$(cat .bashrc | grep /usr/local/include/opencv4)" ]]; then
                echo '# opencv envs' >> .bashrc
                echo 'export C_INCLUDE_PATH="/usr/local/include/opencv4:$C_INCLUDE_PATH"' >> .bashrc
                echo 'export CPLUS_INCLUDE_PATH="/usr/local/include/opencv4:$CPLUS_INCLUDE_PATH"' >> .bashrc
                echo 'export LIBRARY_PATH="/usr/local/lib64/:$LIBRARY_PATH"' >> .bashrc
                echo 'export LD_LIBRARY_PATH="/usr/local/lib64/:$LD_LIBRARY_PATH"' >> .bashrc
            else
                echo -e "\033[33mEnvironment already satisfied for user ${user}.\033[0m"
            fi

            source .bashrc
        done
    else
        echo 'No other users detected.'
    fi

    echo -e "\033[32m########################################"
    echo -e "#            Envs Set                  #"
    echo -e "########################################\033[0m"

    return 0
}

function set_pkg_config() {
    echo -e "\033[36m########################################"
    echo -e "#            Setting pkg-config        #"
    echo -e "########################################\033[0m"

    echo 'Creating opencv4.pc ...'
    cd /usr/local/lib64
    if [ ! -d "pkgconfig" ]; then
        mkdir pkgconfig
    fi
    chmod 777 pkgconfig
    cd pkgconfig
    if [ ! -f "opencv4.pc" ]; then
        touch opencv4.pc

        echo '# Package Information for pkg-config' >> opencv4.pc
        echo 'prefix=/usr/local' >> opencv4.pc
        echo 'exec_prefix=${prefix}' >> opencv4.pc
        echo 'libdir=${exec_prefix}/lib64' >> opencv4.pc
        echo 'includedir=${prefix}/include/opencv4' >> opencv4.pc
        echo 'Name: OpenCV' >> opencv4.pc
        echo 'Description: Open Source Computer Vision Library' >> opencv4.pc
        echo 'Version: 4.5.4' >> opencv4.pc
        echo 'Libs: -L${exec_prefix}/lib64 -lopencv_gapi -lopencv_highgui -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_video -lopencv_calib3d -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -lopencv_core' >> opencv4.pc
        echo 'Libs.private: -ldl -lm -lpthread -lrt' >> opencv4.pc
        echo 'Cflags: -I${includedir}' >> opencv4.pc
    else
        echo -e "\033[33mopencv4.pc already exist.\033[0m"
    fi
    chmod 777 opencv4.pc

    echo 'Setting pkg-config envs for root...'
    cd ~

    if [ ! -f ${bashrc} ]; then
        touch ${bashrc}
    fi
    
    if [[ -z "$(cat ${bashrc} | grep /usr/local/lib64/pkgconfig)" ]]; then
        echo '# pkg-config envs' >> ${bashrc}
        echo 'export PKG_CONFIG_PATH="/usr/local/lib64/pkgconfig:$PKG_CONFIG_PATH"' >> ${bashrc}
        echo 'export PKG_CONFIG_ALLOW_SYSTEM_LIBS=1' >> ${bashrc}
        echo 'export PKG_CONFIG_ALLOW_SYSTEM_CFLAGS=1' >> ${bashrc}
    else
        echo -e "\033[33mpkg-config environment already satisfied.\033[0m"
    fi
    source ${bashrc}

    if [[ -n "$(ls /home)" ]]; then
        for user in $(ls /home); do
            echo "Setting pkg-config envs for user ${user}..."
            cd /home/${user}

            if [ ! -f .bashrc ]; then
                touch .bashrc
            fi

            if [[ -z "$(cat .bashrc | grep /usr/local/lib64/pkgconfig)" ]]; then
                echo '# pkg-config envs' >> .bashrc
                echo 'export PKG_CONFIG_PATH="/usr/local/lib64/pkgconfig:$PKG_CONFIG_PATH"' >> .bashrc
                echo 'export PKG_CONFIG_ALLOW_SYSTEM_LIBS=1' >> .bashrc
                echo 'export PKG_CONFIG_ALLOW_SYSTEM_CFLAGS=1' >> .bashrc
            else
                echo -e "\033[33mpkg-config environment already satisfied for user ${user}.\033[0m"
            fi

            source .bashrc
        done
    else
        echo 'No other users detected.'
    fi

    echo -e "\033[32m########################################"
    echo -e "#            pkg-config Set            #"
    echo -e "########################################\033[0m"

    return 0
}

function run_test() {
    echo -e "\033[36m########################################"
    echo -e "#            Test Start                #"
    echo -e "########################################\033[0m"

    echo 'Compiling and running test...'
    cd ~

    if [ -f "${test_file}.cpp" ]; then
        rm "${test_file}.cpp"
    fi

    touch "${test_file}.cpp"
    echo '#include <opencv2/opencv.hpp>' >> "${test_file}.cpp"
    echo 'int main() { cv::Mat m = (cv::Mat_<int>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9); std::cout<<m<<std::endl; cv::Mat img = cv::imread("./fake_file.jpg"); return 0; }' >> "${test_file}.cpp"
    g++ "${test_file}.cpp" -o "${test_file}.out" -lopencv_core -lopencv_highgui -lopencv_imgcodecs
    g++ "${test_file}.cpp" -o "${test_file}.out" $(pkg-config --cflags --libs opencv4)

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

    return 0
}

function signature() {
    echo -e "\033[34m2021-12-02 Dong Zheng 11812804 https://github.com/XDZhelheim\033[0m"

    return 0
}

# --------------------------------------------------------------------------------------------

if [[ "$(uname -m)" != 'aarch64' ]]; then
    echo -e "\033[31mPlease run this script on our lab's ARM server.\033[0m"
    signature
    exit 1
fi

if [[ "$(whoami)" != 'root' ]]; then
    echo -e "\033[31mPlease run this script on root user.\033[0m"
    signature
    exit 1
fi

check_cmake
download
build
set_env
set_pkg_config
run_test

echo 'Finished.'
signature
