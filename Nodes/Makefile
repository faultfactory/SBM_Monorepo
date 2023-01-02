none:
	@echo None built

START=arduino-cli compile --fqbn adafruit:samd:adafruit_feather_m4_can --build-property "compiler.cpp.extra_flags=\"-D
END=\=1\"" Nodes.ino -e

front:
	$(START)FRONT_NODE $(END)
screen:
	$(START)SCREEN_NODE $(END)
vd:
	$(START)VEHICLE_DYNAMICS_NODE $(END)