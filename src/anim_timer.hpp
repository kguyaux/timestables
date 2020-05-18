#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include <cstdlib>
#include <vector>


// this class handles, and keeps track of the time for animations.
// A bit like a stopwatch..
class Anim {
    public:
        float offset = 0.0f;
        float time = 0.0f;
        bool awaiting = true;
        bool finished = false;
        bool running = false;
        bool started = false;  // tells the actual start happened, for triggering stuff
        float duration = 0.0f;
        float start_time = -1.0f;
		std::vector<float> fLut; // a handy buffer for random offsetvalues and such.. cool for exploding-animations
        bool loop = false;
   
        Anim(float _duration, float _start_time= -1.0f) : duration(_duration), start_time(_start_time) {  } //construct
        ~Anim() { }  //destruct
        
        void run(float time) 
        {
            awaiting = false;
            offset = (start_time != -1.0f) ? start_time: time; // if start_time is given, then use that as offset, otherwise use 'now'.
            running = true;
            finished = false;
        }
        void reset() 
        {
            offset = 0.0f;
            awaiting = true;
            running = false;
            finished = false;
        }

        void stop()
        {
            running = false;
            awaiting = false;
            finished = true;
        }

        // counting up till upper value has been reached
        float countup_check(float t)
        {
            if (t >= start_time) {
                float ret = (t - offset);
                ret = ret > duration ? duration : ret;
                if (running && (ret == duration)) stop();
                return ret;
            } else {
                return 0.0f;
            }
        }
        
        // counting down until 0 is reached
        float countdown_check(float t)
        {
            //if (!running) return -1.0f;
            if (t >= start_time) {
                float ret = duration - (t - offset);
                ret = ret < 0 ? 0.0f : ret;
                if (running && (ret == 0.0f)) stop();
                return ret;
            } else {
                return duration;
            }
        }
        
        // create buffer with random values
		void init_random_LUT(size_t amount)
		// not for timing but to have random offsetts for scattering stuff
		{
			for (size_t i = 0; i < amount; i++) {
				this->fLut.push_back((float)(std::rand() % 4096) / 4096.0);
			}
		}
    private:
};

#endif
