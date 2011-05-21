#include "track.h"

#include <cstdio>

Track::Track()
{
	this->reset();
}

Track::~Track()
{
}

void Track::setLocation(std::string &location)
{
	_location = location;
}

void Track::setLocation(std::string location)
{
	_location = location;
}

void Track::setLocation(const char *location)
{
	if (location != NULL)
		_location = location;
}

void Track::setDuration(int duration)
{
	if (duration > 0)
		_duration = duration;
}

void Track::setStartTime(int startTime)
{
	if (startTime >= 0)
		_startTime = startTime;
}

std::string &Track::Location()
{
	return _location;
}

int Track::Duration()
{
	return _duration;
}

int Track::StartTime()
{
	return _startTime;
}

int Track::EndTime()
{
	return _duration + _startTime;
}

bool Track::isSet()
{
	return (_duration > -1 &&
			_startTime > -1 &&
			_location != "");
}

void Track::reset()
{
	_location = "";
	_duration = -1;
	_startTime = -1;
}

const Track* Track::operator=(const Track* track)
{
	if (track == NULL) return NULL;
	
	this->_location = track->_location;
	this->_duration = track->_duration;
	this->_startTime = track->_startTime;
	
	return this;
}

void Track::print()
{
	int endTime = _duration + _startTime;
	printf("[%02d:%02d:%02d] %s [%02d:%02d] -> [%02d:%02d:%02d]\n",
			_startTime / 3600, _startTime / 60 % 60, _startTime % 60,
			_location.c_str(), _duration / 60, _duration % 60,
			endTime / 3600, endTime / 60 % 60, endTime % 60);
}
