// -*- mode: js; coding: utf-8 -*-

_ = console.info.bind(console);

ice = null;
rover = null;

function show_error(err) { console.error("ERROR: " + err); };
function show_ok(msg) { console.info("OK: " + msg); };

function connect() {
    var strprx = $('#rover-proxy').val();
    rover = ic.stringToProxy(strprx);
    rover = Example.RoverPrx.uncheckedCast(rover);
    rover = rover.ice_encodingVersion(Ice.Encoding_1_0);
};

function forward() {
    if (rover == null) return show_error("rover is not started!");
    rover.forward().exception(show_error);
};

function backward() {
    if (rover == null) return show_error("rover is not started!");
    rover.backward().exception(show_error);
};

function left() {
    if (rover == null) return show_error("rover is not started!");
    rover.left().exception(show_error);
};

function right() {
    if (rover == null) return show_error("rover is not started!");
    rover.right().exception(show_error);
};

function stop() {
    if (rover == null) return show_error("rover is not started!");
    rover.stop().exception(show_error);
};

$(document).ready(function() {
    ic = Ice.initialize();

    $('#arrow-up')
	.mousedown(forward)
	.mouseup(stop);
    $('#arrow-down')
	.mousedown(backward)
	.mouseup(stop);
    $('#arrow-left')
	.mousedown(left)
	.mouseup(stop);
    $('#arrow-right')
	.mousedown(right)
	.mouseup(stop);
});
