// -*- mode: c++; coding: utf-8 -*-

module Example {
    interface Rover {
        void forward();
        void backward();
        void left();
        void right();
        void stop();
    };

    interface Station {
        void notifyBatteryLevel(byte level);
    };
};
