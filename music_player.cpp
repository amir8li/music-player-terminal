#include <iostream>
#include <bits/stdc++.h> 
using namespace std;

class Song;
class Playlist;
class Node;

struct General {
    vector<Song> songs;
    vector<Playlist> playlists;
    queue<Song> q;
    stack<string> back_history;
};

void main_menu(General &general);
void show_songs_list(General &general);
void show_playlists_list(General &general);

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
    Playlist(const Playlist& other);
    void insert_song(Node song, int ind);
    void preppend_song(Node &song, Song &original_song);
    void append_song(Node song);
    void remove_song(Node song);
    void print_list_of_songs();
    bool song_exist(string input_name, string input_artist);
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
    void write_playlists_of_song(General &general);
    void add_to_queue(General &general);
};
class Node{
public:
    Song song;
    bool is_played;
    Node *next;
    Node *prev;
};
void Playlist::menu(General &general){
    cout << "playlist name: " << this->name << endl;
    print_list_of_songs();
    cout << "-2. play \n-1. delete a song \n0. back" << endl; 
    int choice;
    cout << "your choice: ";
    cin >> choice;
    if(choice == 0){
        string last_page = general.back_history.top();
        general.back_history.pop();
        if(last_page == "show_playlists_list"){
            cout << "###############################################################\n";
            return show_playlists_list(general);
        }
        // to be continued for other pages
        else{
            for(int i = 0; i < general.songs.size(); ++i){
                if(general.songs[i].name == last_page){
                    cout << "###############################################################\n";
                    return general.songs[i].menu(general);
                }
            }
        }
    }
    // other options 
    else{
        cout << "###############################################################\n";
        general.back_history.push(this->name);
        return general.songs[choice - 1].menu(general);
    }
}
Playlist::Playlist(const Playlist& other) {
        name = other.name;
        count = other.count;
        head = nullptr;

        Node* current = other.head;
        Node* prevNewNode = nullptr;

        while (current != nullptr) {
            Node* newNode = new Node;
            newNode->song = current->song;   
            newNode->is_played = current->is_played;
            newNode->next = nullptr;
            newNode->prev = prevNewNode;

            if (prevNewNode == nullptr) {
                head = newNode;
            } else {
                prevNewNode->next = newNode;
            }

            prevNewNode = newNode;
            current = current->next;
        }
    }
bool Playlist::song_exist(string input_name, string input_artist){
    if(head == nullptr) return false;
    Node *itr = head;
    do{
        if(itr->song.name == input_name && itr->song.artist == input_artist)
            return true;
        itr = itr->next;
    }while(itr != nullptr && itr != head);
    return false;
}
void Playlist::print_list_of_songs(){
    if(head == nullptr){
        cout << "no song to show!!!" << endl;
        return;
    }
    Node *itr = head;
    int counter = 1;
    do{
        cout << counter++ << ". " << itr->song.name << ", by " << itr->song.artist << endl;
        itr = itr->next;
    }while(itr != nullptr && itr != head);
}
void Playlist::preppend_song(Node &song, Song &original_song){
    Node *newNode = new Node;
    newNode->song = song.song;
    newNode->is_played = song.is_played;
    if(count == 0){
        newNode->next = newNode;
        newNode->prev = newNode;
        head = newNode;
    }
    else{
        Node *tail = head->prev;
        newNode->next = head;
        newNode->prev = tail;
        tail->next = newNode;
        head->prev = newNode;
        head = newNode;
    }
    ++count;
    original_song.playlists[original_song.playlist_count++] = *this;
}
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
        if(last_page == "show_songs_list"){
            cout << "###############################################################\n";
            return show_songs_list(general);
        }
        // to be continued for other pages
        else{
            for(int i = 0; i < general.playlists.size(); ++i){
                if(general.playlists[i].name == last_page){
                    return general.playlists[i].menu(general);
                }
            }
        }
    }
    else if(choice == 1)
        return write_playlists_of_song(general);
    else if(choice == 2){
        bool flag = true; 
        while(flag){   
            flag = false;
            string input_name;
            cout << "enter name of the playlist to add: ";
            cin >> input_name;

            int found_index = -1;
            for(int i = 0; i < general.playlists.size(); ++i){
                if(general.playlists[i].name == input_name){
                    found_index = i;
                    break;
                }
            }

            if (found_index == -1) {
                cout << "playlist not found" << endl;
                flag = true;
                continue;
            }

            Playlist &tmp = general.playlists[found_index];

            if (tmp.song_exist(this->name, this->artist)) {
                cout << "song already exists" << endl;
                return menu(general);
            }

            tmp.print_list_of_songs();
            int x;
            while (true) {
                cout << "where do you want to add it? ";
                string line;
                if(!getline(cin >> ws, line)){
                    cout << "input error, try again" << endl;
                    continue;
                }
                try {
                    size_t pos = 0;
                    long val = stol(line, &pos);
                    if(pos != line.size()){
                        cout << "please enter a valid integer (no extra chars)" << endl;
                        continue;
                    }
                    if(val < INT_MIN || val > INT_MAX){
                        cout << "integer out of range, try again" << endl;
                        continue;
                    }
                    x = static_cast<int>(val);
                    break;
                }catch(...){
                    cout << "please enter a valid integer" << endl;
                }
            }
            Node song_node;
            song_node.song = *this;
            if (x < 1) {
                tmp.preppend_song(song_node, *this);
            }
            else if (x > tmp.count) {
                //tmp.append_song(song_node);
            }
            else {
                //tmp.insert_song(song_node, x);
            }
            cout << "song added to playlist" << endl;
            cout << "###############################################################\n";
            return menu(general);
        }
    }
    else if(choice == 3){
        // add_to_queue(general);
    }
}
void Song::write_playlists_of_song(General &general){
    cout << "Playlists containing this song:" << endl;
    for (int i = 0; i < playlist_count; ++i) {
        cout << i + 1 << ". " << playlists[i].name << endl;
    }
    cout << "0. back" << endl;
    int choice;
    cout << "your choice: "; 
    cin >> choice;
    cout << "###############################################################\n";
    if(choice == 0)
        return menu(general);
    general.back_history.push(this->name);
    return playlists[choice - 1].menu(general);
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
        return general.playlists[choice - 1].menu(general);
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