// -*- mode: c++; coding: utf-8 -*-

module Example {
    interface Rover {
	void forward();
	void backward();
	void left();
	void right();
    };

    interface Station {
	void notifyBatteryLevel(byte level);
    };
};
