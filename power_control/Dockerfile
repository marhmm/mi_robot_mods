FROM resin/rpi-raspbian:stretch-20181024

# 433Mhz Steckdose
RUN apt-get update && apt-get install git-core make build-essential

RUN git clone git://git.drogon.net/wiringPi
RUN cd wiringPi && ./build

RUN git clone --recursive https://github.com/ninjablocks/433Utils.git
RUN cd 433Utils/RPi_utils && make all
COPY steuerung.cpp 433Utils/RPi_utils/steuerung.cpp
RUN cd 433Utils/RPi_utils && g++ -DRPI ../rc-switch/RCSwitch.cpp steuerung.cpp -o steuerung -lwiringPi

# Batteriestand auslesen
RUN apt-get install wget nano python3 python3-pip

RUN pip3 install -U pip
RUN pip3 install -U requests
RUN pip3 install schedule

COPY get_batt.py /

# Start script
COPY start.sh /
RUN chmod +x /start.sh

# Run this when the container is started
CMD /start.sh
