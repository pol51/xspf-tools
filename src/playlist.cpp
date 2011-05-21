#include "playlist.h"

#include "track.h"

#include <cstdio>

// XSPFLoaderImpl...

#include <QXmlDefaultHandler>
#include <QXmlSimpleReader>

#define XSPF_INDENTATION		4

class XSPFLoaderImpl: public QXmlDefaultHandler
{
	public:
		Playlist *_master;

	protected:
		bool _location;
		bool _duration;
		Track *_tmpTrack;
	
	public:
		XSPFLoaderImpl()
		{
			_location = false;
			_duration = false;
			_tmpTrack = new Track();
		}
		
		virtual ~XSPFLoaderImpl()
		{
			delete _tmpTrack;
		}
		
	protected:
		virtual bool startDocument()
		{
			// vidage de la playlist
			//_master->clear();
			
			return true;
		}
		
		virtual bool startElement(const QString &/*namespaceURI*/,
								  const QString &/*localName*/,
								  const QString &name,
								  const QXmlAttributes &/*properties*/)
		{
			_location = (name == "location");
			_duration = (name == "duration");
			
			if (name == "track")
			{
				_tmpTrack->reset();
				_tmpTrack->setStartTime(_master->TotalTime());
			}
			
			return true;
		}
		
		virtual bool endElement(const QString &/*namespaceURI*/,
								const QString &/*localName*/,
								const QString &name)
		{
			if (name == "track")
				if (_tmpTrack->isSet())
					*(_master->addTrack()) = _tmpTrack;
			
			return true;
		}
		
		virtual bool characters(const QString &text)
		{
			if (_location)
				if (text.toStdString().substr(0, 5) == "file:")
					_tmpTrack->setLocation(text.toStdString());

			if (_duration)
			{
				int track_time;
				if (sscanf(text.toStdString().c_str(), "%d", &track_time) == 1)
					_tmpTrack->setDuration(track_time / 1000);
			}
			
			return true;
		}
};
// ...XSPFLoaderImpl

// XSPFSaverImpl...
#include <QDomDocument>

class XSPFSaverImpl
{
	public:
		Playlist *_master;

	protected:
		QDomDocument _document;
		QDomElement _trackList;
	
	public:
		XSPFSaverImpl()
		{
			QDomElement playlist = _document.createElement("playlist");
			playlist.setAttribute("version", 1);
			playlist.setAttribute("xmlns", "http://xspf.org/ns/0/");
			_trackList = _document.createElement("trackList");
			
			_document.appendChild(playlist);
			playlist.appendChild(_trackList);
			
			QDomNode xmlNode = _document.createProcessingInstruction("xml",
								"version=\"1.0\" encoding=\"UTF-8\"");
			_document.insertBefore(xmlNode, _document.firstChild());
		}
		
		bool addItem(Track *track)
		{
			if (!track || !track->isSet()) return false;
			
			QDomElement trackElement = _document.createElement("track");
			QDomElement locationElement = _document.createElement("location");
			QDomElement durationElement = _document.createElement("duration");
			QDomText location = _document.createTextNode(
									track->Location().c_str());
			QString tmpDuration;
			tmpDuration.setNum(track->Duration() * 1000);
			QDomText duration = _document.createTextNode(tmpDuration);
			
			_trackList.appendChild(trackElement);
			trackElement.appendChild(locationElement);
			trackElement.appendChild(durationElement);
			locationElement.appendChild(location);
			durationElement.appendChild(duration);
			
			return true;
		}
		
		QDomDocument *Document()
		{
			return &_document;
		}
};
// ...XSPFSaverImpl


std::string empty_str("");

Playlist::Playlist()
{
}

Playlist::~Playlist()
{
	this->clear();
}

void Playlist::clear()
{
	std::vector <Track *>::iterator i;
	for (i = _tracks.begin(); i != _tracks.end(); i++)
		delete *i;
	
	_tracks.clear();
}

Track *Playlist::getTrack(unsigned int id)
{
	if (id > _tracks.size())
		return NULL;
	
	return _tracks[id];
}

Track *Playlist::addTrack()
{
	_tracks.push_back(new Track());
	return _tracks.back();
}

void Playlist::print()
{
	std::vector <Track*>::iterator i;
	for (i = _tracks.begin(); i != _tracks.end(); i++)
		(*i)->print();
}

int Playlist::findTrackId(std::string location)
{
	for (unsigned int i = 0; i < _tracks.size(); i++)
		if (_tracks[i]->Location() == location)
			return i;
	return -1;
}

unsigned int Playlist::Nbr()
{
	return _tracks.size();
}

unsigned int Playlist::Nbr(std::string location)
{
	unsigned int total = 0;
	
	std::vector <Track*>::iterator i;
	for (i = _tracks.begin(); i != _tracks.end(); i++)
		if ((*i)->Location() == location)
			total++;
	
	return total;
}

unsigned int Playlist::TotalTime()
{
	unsigned int total = 0;
	
	std::vector <Track*>::iterator i;
	for (i = _tracks.begin(); i != _tracks.end(); i++)
		total += (*i)->Duration();
	
	return total;
}

unsigned int Playlist::TotalTime(std::string location)
{
	int trackId = findTrackId(location);
	
	if (trackId < 0) return 0;
	
	return _tracks[trackId]->Duration() * this->Nbr(location);
}

float Playlist::Percentile(std::string location)
{
	return (float)this->TotalTime(location) / (float)this->TotalTime() * 100.;
}

bool Playlist::loadXSPF(std::string filename)
{
	XSPFLoaderImpl xspfLoader;
	xspfLoader._master = this;
	
	QFile file(filename.c_str());
	QXmlInputSource inputSource(&file);
	
	QXmlSimpleReader reader;
	reader.setContentHandler(&xspfLoader);
	reader.setErrorHandler(&xspfLoader);
	
	return reader.parse(inputSource);
}

bool Playlist::saveXSPF(std::string filename)
{
	FILE* file = fopen(filename.c_str(), "w+");
	if (!file) return false;
	
	XSPFSaverImpl xspfSaver;
	xspfSaver._master = this;
	
	std::vector <Track*>::iterator i;
	for (i = _tracks.begin(); i != _tracks.end(); i++)
		xspfSaver.addItem(*i);
	
	QTextStream out(file);
	xspfSaver.Document()->save(out, XSPF_INDENTATION);
	
	return true;
}
