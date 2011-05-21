#ifndef __PLAYLIST_H
#define __PLAYLIST_H

#include <vector>

#include <string>

class Track;

class Playlist
{
	protected:
		std::vector <Track *> _tracks;
	
	public:
		Playlist();
		virtual ~Playlist();
		void clear();
		Track *getTrack(unsigned int id = 0);
		Track *addTrack();
		void print();
		int findTrackId(std::string location);
		unsigned int Nbr();
		unsigned int Nbr(std::string location);
		unsigned int TotalTime();
		unsigned int TotalTime(std::string location);
		float Percentile(std::string location);
		bool loadXSPF(std::string filename);
		bool saveXSPF(std::string filename);
};

#endif
