#ifndef __TRACK_H
#define __TRACK_H

#include <iostream>

class Track
{
	protected:
		std::string	_location;
		int 		_duration;
		int			_startTime;
	
	public:
		Track();
		virtual ~Track();
		void setLocation(std::string &location);
		void setLocation(std::string location);
		void setLocation(const char *location);
		void setDuration(int duration = 0);
		void setStartTime(int startTime = 0);
		std::string &Location();
		int Duration();
		int StartTime();
		int EndTime();
		bool isSet();
		void reset();
		const Track* operator=(const Track* track);
		void print();
};

#endif
