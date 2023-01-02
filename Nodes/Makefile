none:
	@echo None built

FQBN=--fqbn adafruit:samd:adafruit_feather_m4_can
START=arduino-cli compile $(FQBN) --build-property "compiler.cpp.extra_flags=\"-D
END=\=1\"" Nodes.ino -e

PORT=$(shell python3 getport.py)

frontv:
	$(START)FRONT_NODE $(END)
front: frontv
	arduino-cli upload $(FQBN) -p $(PORT) Nodes.ino
screenv:
	$(START)SCREEN_NODE $(END)
screen: screenv
	arduino-cli upload $(FQBN) -p $(PORT) Nodes.ino
vdv:
	$(START)VEHICLE_DYNAMICS_NODE $(END)
vd: vdv
	arduino-cli upload $(FQBN) -p $(PORT) Nodes.ino