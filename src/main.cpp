#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <fstream>
#include <iostream>
#include <string>

#include <cstdlib>

#include "playlist.h"

// Playlist
Playlist myPlaylist;

// Nom des pistes
char *track[] =
{
	"file:///mp3s/song_1.mp3",
	"file:///mp3s/song_2.mp3",
	"file:///mp3s/song_3.mp3",
	"file:///mp3s/song_4.mp3",
	"file:///mp3s/song_5.mp3"
};

void showPiste(int nb)
{
	printf("Nombre de piste \"%s\": %d\n",
		track[nb],
		myPlaylist.Nbr(track[nb]));
}

void showPisteTime(int nb)
{
	printf("Temps total de piste \"%s\": %02d:%02d:%02d (%2.2f%%)\n",
			track[nb],
			myPlaylist.TotalTime(track[nb]) / 3600,
			myPlaylist.TotalTime(track[nb]) / 60 % 60,
			myPlaylist.TotalTime(track[nb]) % 60,
			myPlaylist.Percentile(track[nb]));
}

int main(int argc, char** argv)
{
	// Chemin de la playlist à charger
	std::string filepath;
	if (argc > 1 )
		filepath = argv[1];
	else
		filepath = "1.xspf";
    
	// chargement pour plus d'une semaine
	while ((myPlaylist.TotalTime() / 3600 / 24) < 7)
		myPlaylist.loadXSPF(filepath);
	
	myPlaylist.saveXSPF("out.xspf");
	
	// Affichage du résultat
	//myPlaylist.print();
	printf("Nombre de pistes: %d\n", myPlaylist.Nbr());
	for (int i = 0; i < 5; showPiste(i++));
	printf("Temps total: %02d:%02d:%02d\n",
			myPlaylist.TotalTime() / 3600,
			myPlaylist.TotalTime() / 60 % 60,
			myPlaylist.TotalTime() % 60);
	for (int i = 0; i < 5; showPisteTime(i++));
	
	return EXIT_SUCCESS;
}

