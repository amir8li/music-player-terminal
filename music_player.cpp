#include <iostream>
#include <bits/stdc++.h> 
using namespace std;

class Song;
class Playlist;

struct General {
    vector<Song> songs;
    vector<Playlist> playlists;
    queue<Song> q;
    stack<string> back_history;
};

void main_menu(General &general);
void show_songs_list(General &general);
void show_playlists_list(General &general);
class Node{
    Song song;
    bool is_played;
    Node *next;
    Node *prev;
};
class Playlist{
private:
    Node *head;
public:
    string name;
    int count;
    Playlist(){
        head = nullptr;
        count = 0;
    }
    void insert_song();
    void remove_song();
    void menu(General &general);
};

class Song{
public:
    string name;
    string artist;
    Playlist* playlists;
    int playlist_count;
    Song(){
        playlists = new Playlist[10];
        playlist_count = 0;
    }
    ~Song(){
        delete[] playlists;
    }
    void menu(General &general);
    void write_playlists_of_song();
    void add_to_playlist(Playlist playlist);
    void add_to_queue(General &general);
};

void Song::menu(General &general){
    cout << "song name: " << name << endl;
    cout << "1. list of playlists \n2. add to playlist \n3. add to queue \n0. back" << endl;
    int choice;
    cout << "your choice: ";
    cin >> choice;
    cout << "###############################################################\n";
    if(choice == 0){
        string last_page = general.back_history.top();
        general.back_history.pop();
        if(last_page == "show_songs_list")
            return show_songs_list(general);

            // to be continued for other pages
    }
    else if(choice == 1)
        return write_playlists_of_song();
    else if(choice == 2){
        // add_to_playlist();
    }
    else if(choice == 3){
        // add_to_queue(general);
    }
}
void Song::write_playlists_of_song() {
    cout << "Playlists containing this song:" << endl;
    for (int i = 0; i < playlist_count; ++i) {
        cout << i + 1 << ". " << playlists[i].name << endl;
    }
    int choice;
    cout << "your choice: "; 
    cin >> choice;
    // show the selected playlist details
}

void input_song(General &general){
    Song song;
    cout << "Enter the name of the song: ";
    cin >> song.name;
    cout << "Enter the name of the artist: ";
    cin >> song.artist;
    general.songs.push_back(song);
    cout << "Song successfully added to your songs list." << endl;
    return main_menu(general);
}

void show_songs_list(General &general){
    int counter = 1;
    for (auto& song : general.songs) {
        cout << counter++ << ". " << song.name << ", by " << song.artist << endl;
    }
    cout << "0. back" << endl;
    int choice;
    cout << "your choice: "; 
    cin >> choice;
    if(choice == 0)
        return main_menu(general);
    else{
        general.back_history.push("show_songs_list");
        cout << "###############################################################\n";
        return general.songs[choice - 1].menu(general);
    }
}
void create_playlist(General &general){
    string name;
    bool flag = true;
    while(flag){
        flag = false;
        cout << "enter a name for the new playlis: "; 
        cin >> name;
        for(auto playlist : general.playlists){
            if(name == playlist.name){
                cout << "playlist already exists" << endl;
                flag = true;
                break;
            }
        }
    }
    Playlist pl;
    pl.name = name;
    general.playlists.push_back(pl);
    cout << "new playlist added to list" << endl;
    cout << "###############################################################" << endl;
    return show_playlists_list(general);
}
void show_playlists_list(General &general){
    int counter = 1;
    for (auto& playlist : general.playlists) {
        cout << counter++ << ". " << playlist.name << endl;
    }  
    cout << "-1. add playlist" << endl << "0. back" << endl ;
    int choice;
    cout << "your choice: "; 
    cin >> choice;
    if(choice == 0)
        return main_menu(general);
    else if(choice == -1){
        create_playlist(general);
    }
    else{
        general.back_history.push("show_playlists_list");
        cout << "###############################################################\n";
        //return general.playlists[choice - 1].menu(general);
    } 
}

void main_menu(General &general){
    cout << "########################### WELCOME ###########################" << endl;
    cout << "########################## Main Page ##########################" << endl;
    cout << "1. add song \n2. songs list \n3. playlist \n4. song queue \n0. exit" << endl;
    int choice;
    cout << "your choice: ";
    cin >> choice;
    if(choice == 0){
        cout << "######################## SEE YOU SOON #########################" << endl;
        exit(0);
    }
    else{
        cout << "###############################################################\n";
        if(choice == 1){
            return input_song(general); 
        }
        else if(choice == 2){
            return show_songs_list(general);
        }
        else if(choice == 3)
            return show_playlists_list(general);
    }
}

int main(){
    General general;
    main_menu(general);
    return 0;
}