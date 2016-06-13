#include "ofMain.h"
#include <linux/input.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <syslog.h>
#include <fcntl.h>


#define EVENT_TYPE      EV_ABS
#define EVENT_CODE_X    ABS_X
#define EVENT_CODE_Y    ABS_Y
#define EVENT_CODE_P    ABS_PRESSURE

class TFTTouch : public ofThread {
	public:

  	struct input_event ev;
	int fd, size;
	char name[256];
	char *device;
	int _minX, _maxX,_minY, _maxY;
	int _resX,_resY;
	int _rot;
        int x,y,pressur;

	int init(int rotation){ 
		size = sizeof (struct input_event);
		name[0]='U';
		name[1]='n';
		name[2]='k';
		name[3]='n';
		name[4]='o';
		name[5]='w';
		name[6]='n';
		device = "/dev/input/touchscreen";

        	if ((fd = open(device, O_RDONLY)) < 0) {
                	return 1;
		}
		
		// for valEvX and valEvY check value Min Max evtest /dev/input/event0
		_resX=320;
		_minX=260;
		_maxX=3900;
		
		_resY=480;
		_minY=150;
		_maxY=3850;
		
		_rot = rotation;
		startThread();
		return 0;
	}

	string getName(){
		ioctl (fd, EVIOCGNAME (sizeof (name)), name);
		string str(name);
		return str;
	}

	void exit(){
		stopThread();
	}

	void threadedFunction(){
		while(isThreadRunning()) {
	        	const size_t ev_size = sizeof(struct input_event);
		        ssize_t size;
		        size = read(fd, &ev, ev_size);
		        if (size < ev_size) {
	        	    ofLog()<<"Error size!\n";
		        }
		        if (ev.type == EVENT_TYPE && (ev.code == EVENT_CODE_X || ev.code == EVENT_CODE_Y || ev.code == EVENT_CODE_P)) {
			    if(ev.code == EVENT_CODE_X)
				x = ofMap(ev.value, _minX, _maxX, 0, _resX);
			    if(ev.code == EVENT_CODE_Y)
				y = ofMap(ev.value, _minY, _maxY, 0, _resY);
			    if(ev.code == EVENT_CODE_P)
				pressur = ev.value;
				
			    if (_rot == 90) pos.set(_resY-y,x,pressur);
			    else if (_rot == 180) pos.set(_resX-x,_resY-y,pressur);
			    else if (_rot == 270) pos.set(y,_resX-x,pressur);
			    else pos.set(x,y,pressur);
		        }
		}
	}

	ofVec3f pos;
	ofVec3f getCoordTouch(){
		return pos;
	}
};
