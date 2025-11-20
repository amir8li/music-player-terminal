#include <iostream>
#include <bits/stdc++.h> 
using namespace std;

class Song;
class Playlist;
class Node;

struct General{
    bool welcome_shown = false;
    vector<shared_ptr<Song>> songs;
    vector<Playlist> playlists;
    queue<shared_ptr<Song>> q;
    stack<string> back_history;
    stack<string> sourcery_reserve;
    int now_playing_playlist_index = -1;
    Node *now_playing_node = nullptr;
    bool is_playing = false;
};

enum SourceType{ 
    PLAYLIST,
    QUEUE
};

void main_menu(General &general);
void show_songs_list(General &general);
void show_playlists_list(General &general);
int check_int();
void print_playbar(General &general);
void next_song(General &general);
void prev_song(General &general);
void start_playing(General &general, int playlist_index, Node *node);
bool try_parse_int(const string &s, int &out);

class Playlist{
private:
    Node *head;
public:
    enum SourceType source_type = PLAYLIST;
    string name;
    int count;
    int count_songs_played;
    map<string, bool> is_played_map;
    Node *last_song_played;
    Playlist(){
        head = nullptr;
        count = 0;
        count_songs_played = 0;
        last_song_played = nullptr;
    }
    Playlist(const Playlist& other);
    ~Playlist();
    void insert_song(shared_ptr<Song> song_ptr, int ind);
    void preppend_song(shared_ptr<Song> song_ptr);
    void append_song(shared_ptr<Song> song_ptr);
    void remove_song(shared_ptr<Song> original_song, int x);
    void print_list_of_songs();
    bool song_exist(string input_name, string input_artist);
    void menu(General &general);
};

class Song{
public:
    string name;
    string artist;
    vector<string> playlists;
    Song(){}
    Song(const Song& other) {
        name = other.name;
        artist = other.artist;
        playlists = other.playlists;
    }
    void menu(General &general);
    void write_playlists_of_song(General &general);
    void add_to_queue(General &general);
};


class Node{
public:
    shared_ptr<Song> song;
    Node *next;
    Node *prev;
};