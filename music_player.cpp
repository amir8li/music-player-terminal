#include <iostream>
#include <bits/stdc++.h> 
#include "music_player.hpp"
using namespace std;


void print_playbar(General &general){
    if(general.sourcery_reserve.size() > 0){
        if(general.sourcery_reserve.top() == "QUEUE"){
            // PLAY FROM SOURCE QUEUE
        }
        else{
            for(auto playlist : general.playlists){
                if(playlist.name == general.sourcery_reserve.top()){
                    general.now_playing_playlist = &playlist;
                    if(general.now_playing_playlist->last_song_played != nullptr)
                        general.now_playing_node = general.now_playing_playlist->last_song_played;
                    break;
                }
            }
            if(!general.is_playing || general.now_playing_playlist->count == general.now_playing_playlist->count_songs_played){
                general.now_playing_playlist->count_songs_played = 0;
                general.sourcery_reserve.pop();
                cout << "###############################################################" << endl;
                return;
            }
            cout << "------------------------------------------------" << endl;
            cout << "Song Playing: " << general.now_playing_node->song->name << " - " << general.now_playing_node->song->artist << endl;
            cout << ">. Next \n<. Prev" << endl;
            general.now_playing_playlist->last_song_played = general.now_playing_node;
            cout << "------------------------------------------------" << endl;
            if(!general.now_playing_playlist->is_played_map[general.now_playing_node->song]){
                general.now_playing_playlist->is_played_map[general.now_playing_node->song] = true;
                general.now_playing_playlist->count_songs_played++;
            }
            cout << "count songs played: " << general.now_playing_playlist->count_songs_played << "/count :" << general.now_playing_playlist->count << endl;
        }
    }
    else{
        general.is_playing = false;
        general.now_playing_playlist = nullptr;
        general.now_playing_node = nullptr;
    }
}
void next_song(General &general){
    if(!general.is_playing || general.now_playing_node == nullptr) return;
    general.now_playing_node = general.now_playing_node->next;
}
void prev_song(General &general){
    if(!general.is_playing || general.now_playing_node == nullptr) return;
    general.now_playing_node = general.now_playing_node->prev;
}
void start_playing(General &general, Playlist *pl, Node *node){
    if(pl == nullptr || node == nullptr) return;
    general.is_playing = true;
    general.now_playing_playlist = pl;
    general.now_playing_node = node;
}
void Playlist::menu(General &general){
    print_playbar(general);
    cout << "playlist name: " << this->name << endl;
    print_list_of_songs();
    cout << "-2. play \n-1. delete a song \n0. back" << endl; 
    string choice;
    cout << "your choice: ";
    cin >> choice;
    if(choice.find('>') != string::npos){ next_song(general); return this->menu(general); }
    if(choice.find('<') != string::npos){ prev_song(general); return this->menu(general); }
    int parsedChoice;
    if(!try_parse_int(choice, parsedChoice)){
        cout << "please enter a valid integer" << endl;
        return this->menu(general);
    }
    if(parsedChoice == 0){
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
    else if(parsedChoice == -1){
        print_playbar(general);
        if(count == 0){
            cout << "no song to remove" << endl;
            cout << "###############################################################\n";
            return this->menu(general);
        }
        print_list_of_songs();
        cout << "which song: " << endl;
        int x = check_int();
        if(x <= 1)
            x = 1;
        else if(x >= count)
            x = count;
        Node *itr = head;
        for(int i = 1; i < x; ++i){
            itr = itr->next;
        }
        // Song *real_song = nullptr;
        // for(int i = 0; i < general.songs.size(); ++i){
        //     if(general.songs[i].name == itr->song->name && general.songs[i].artist == itr->song->artist){
        //         real_song = &general.songs[i];
        //         break;
        //     }
        // }
        remove_song(itr->song, x);
        return this->menu(general);
    }
    else if(parsedChoice == -2){
        cout << "from which song do you want to play? " << endl;
        int start_index = check_int();
        if(start_index <= 1)
            start_index = 1;
        else if(start_index >= count)
            start_index = count;
        Node *itr = head;
        for(int i = 1; i < start_index; ++i){
            itr = itr->next;
        }
        cout << "###############################################################\n";
        general.sourcery_reserve.push(this->name);
        start_playing(general, this, itr);
        return this->menu(general);
    }
    else{
        cout << "###############################################################\n";
        general.back_history.push(this->name);

        if (head == nullptr) return;

        if (parsedChoice <= 0) return this->menu(general);
        if (parsedChoice > count) parsedChoice = count;
        int choice_int = parsedChoice;
        Node *itr = head;
        for (int i = 1; i < choice_int; ++i) {
            itr = itr->next;
        }

        Song *real_song = nullptr;
        for (int i = 0; i < general.songs.size(); ++i) {
            if (general.songs[i].name == itr->song->name && general.songs[i].artist == itr->song->artist) {
                real_song = &general.songs[i];
                break;
            }
        }

        if (real_song) {
            return real_song->menu(general);
        }
        else {
            return this->menu(general);
        }
    }
}
Playlist::Playlist(const Playlist& other) {
        name = other.name;
        count = other.count;
        count_songs_played = other.count_songs_played;
        is_played_map = other.is_played_map;
        last_song_played = other.last_song_played;
        head = nullptr;

        if(other.head == nullptr) return; 

        Node* current = other.head;
        Node* prevNewNode = nullptr;
        Node* firstNewNode = nullptr;

        do {
            Node* newNode = new Node;
            newNode->song = current->song;   
            newNode->next = nullptr;
            newNode->prev = prevNewNode;

            if (prevNewNode == nullptr) {
                head = newNode;
                firstNewNode = newNode;
            } else {
                prevNewNode->next = newNode;
            }

            prevNewNode = newNode;
            current = current->next;
        } while (current != other.head); 

        if(prevNewNode != nullptr) {
            prevNewNode->next = firstNewNode;
            firstNewNode->prev = prevNewNode;
        }
    }
bool Playlist::song_exist(string input_name, string input_artist){
    if(head == nullptr) return false;
    Node *itr = head;
    do{
        if(itr->song->name == input_name && itr->song->artist == input_artist)
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
        cout << counter++ << ". " << itr->song->name << ", by " << itr->song->artist << endl;
        itr = itr->next;
    }while(itr != nullptr && itr != head);
}
void Playlist::preppend_song(Song *song_ptr){
    Node *newNode = new Node;
    newNode->song = song_ptr;
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
}
void Playlist::append_song(Song *song_ptr){
    Node *newNode = new Node;
    newNode->song = song_ptr;
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
    }
    ++count;
}
void Playlist::insert_song(Song *song_ptr, int ind){
    Node *newNode = new Node;
    newNode->song = song_ptr;
    Node *itr = head;
    for(int i = 1; i < ind - 1; ++i){
        itr = itr->next;
    }

    newNode->next = itr->next;
    newNode->prev = itr;
    itr->next->prev = newNode;
    itr->next = newNode;
    ++count;
}
Playlist::~Playlist(){
    if(head == nullptr) return;
    Node *current = head;
    Node *nextNode;

    do {
        nextNode = current->next;
        delete current;
        current = nextNode;
    } while (current != head);
}
void Playlist::remove_song(Song *original_song, int x){
    if(x <= 1){
        if(count == 1){
            delete head;
            head = nullptr;
            count = 0;
        }
        else{
            Node *tail = head->prev;
            Node *toDelete = head;
            head = head->next;
            tail->next = head;
            head->prev = tail;
            delete toDelete;
            count--;
        }
    }
    else if(x >= count){
        if(count == 1){
            delete head;
            head = nullptr;
            count = 0;
        }
        else{
            Node *tail = head->prev;
            Node *toDelete = tail;
            Node *newTail = tail->prev;
            newTail->next = head;
            head->prev = newTail;
            delete toDelete;
            count--;
        }
    }
    else{
        Node *itr = head;
        for(int i = 1; i < x; ++i){
            itr = itr->next;
        }
        Node *toDelete = itr;
        itr->prev->next = itr->next;
        itr->next->prev = itr->prev;
        delete toDelete;
        count--;
    }
    cout << "song deleted from playlist" << endl;
    cout << "###############################################################\n";
    for(int i = 0; i < original_song->playlists.size(); ++i){
        if(original_song->playlists[i]->name == this->name){
            original_song->playlists.erase(original_song->playlists.begin() + i);
            break;
        }
    }
    if(is_played_map[original_song])
        count_songs_played--;
    is_played_map.erase(original_song);
}
void Song::menu(General &general){
    print_playbar(general);
    cout << "song name: " << name << endl;
    cout << "1. list of playlists \n2. add to playlist \n3. add to queue \n0. back" << endl;
    string choice;
    cout << "your choice: ";
    cin >> choice;
    if(choice.find('>') != string::npos){ next_song(general); return menu(general); }
    if(choice.find('<') != string::npos){ prev_song(general); return menu(general); }
    cout << "###############################################################\n";
    int parsedChoice;
    if(!try_parse_int(choice, parsedChoice)){
        cout << "please enter a valid integer" << endl;
        return menu(general);
    }
    if(parsedChoice == 0){
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
    else if(parsedChoice == 1)
        return write_playlists_of_song(general);
    else if(parsedChoice == 2){
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
            cout << "where do you want to add it?" << endl;
            int x = check_int();
            if (x < 1) {
                tmp.preppend_song(this);
            }
            else if (x >= tmp.count) {
                tmp.append_song(this);
            }
            else {
                tmp.insert_song(this, x);
            }
            this->playlists.push_back(&general.playlists[found_index]);
            cout << "song added to playlist" << endl;
            cout << "###############################################################\n";
            tmp.is_played_map.insert({this, false});
            return menu(general);
        }
    }
    else if(parsedChoice == 3){
        // add_to_queue(general);
    }
}
void Song::write_playlists_of_song(General &general){
    print_playbar(general);
    cout << "Playlists containing this song:" << endl;
    for (int i = 0; i < playlists.size(); ++i) {
        cout << i + 1 << ". " << playlists[i]->name << endl;
    }
    cout << "0. back" << endl;
    string choice;
    cout << "your choice: "; 
    cin >> choice;
    if(choice.find('>') != string::npos){ next_song(general); return write_playlists_of_song(general); }
    if(choice.find('<') != string::npos){ prev_song(general); return write_playlists_of_song(general); }
    cout << "###############################################################\n";
    int parsedChoice;
    if(!try_parse_int(choice, parsedChoice)){
        cout << "please enter a valid integer" << endl;
        return write_playlists_of_song(general);
    }
    if(parsedChoice == 0)
        return menu(general);
    general.back_history.push(this->name);
    return playlists[parsedChoice - 1]->menu(general);
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
    print_playbar(general);
    int counter = 1;
    for (auto& song : general.songs) {
        cout << counter++ << ". " << song.name << ", by " << song.artist << endl;
    }
    cout << "0. back" << endl;
    string choice;
    cout << "your choice: "; 
    cin >> choice;
    if(choice.find('>') != string::npos){ next_song(general); return show_songs_list(general); }
    if(choice.find('<') != string::npos){ prev_song(general); return show_songs_list(general); }
    int parsedChoice;
    if(!try_parse_int(choice, parsedChoice)){
        cout << "please enter a valid integer" << endl;
        return show_songs_list(general);
    }
    if(parsedChoice == 0)
        return main_menu(general);
    else{
        general.back_history.push("show_songs_list");
        cout << "###############################################################\n";
        return general.songs[parsedChoice - 1].menu(general);
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
    print_playbar(general);
    int counter = 1;
    for (auto& playlist : general.playlists) {
        cout << counter++ << ". " << playlist.name << endl;
    }  
    cout << "-1. add playlist" << endl << "0. back" << endl ;
    string choice;
    cout << "your choice: "; 
    cin >> choice;
    if(choice.find('>') != string::npos){ next_song(general); return show_playlists_list(general); }
    if(choice.find('<') != string::npos){ prev_song(general); return show_playlists_list(general); }
    int parsedChoice;
    if(!try_parse_int(choice, parsedChoice)){
        cout << "please enter a valid integer" << endl;
        return show_playlists_list(general);
    }
    if(parsedChoice == 0)
        return main_menu(general);
    else if(parsedChoice == -1){
        create_playlist(general);
    }
    else{
        general.back_history.push("show_playlists_list");
        cout << "###############################################################\n";
        return general.playlists[parsedChoice - 1].menu(general);
    } 
}

void main_menu(General &general){
    print_playbar(general);
    if(!general.welcome_shown){
        cout << "########################### WELCOME ###########################" << endl;
        general.welcome_shown = true;
    }
    cout << "########################## Main Page ##########################" << endl;
    cout << "1. add song \n2. songs list \n3. playlist \n4. song queue \n0. exit" << endl;
    string choice;
    cout << "your choice: ";
    cin >> choice;
    if(choice.find('>') != string::npos){ next_song(general); return main_menu(general); }
    if(choice.find('<') != string::npos){ prev_song(general); return main_menu(general); }
    int parsedChoice;
    if(!try_parse_int(choice, parsedChoice)){
        cout << "please enter a valid integer" << endl;
        return main_menu(general);
    }
    if(parsedChoice == 0){
        cout << "######################## SEE YOU SOON #########################" << endl;
        exit(0);
    }
    else{
        cout << "###############################################################\n";
        if(parsedChoice == 1){
            return input_song(general); 
        }
        else if(parsedChoice == 2){
            return show_songs_list(general);
        }
        else if(parsedChoice == 3)
            return show_playlists_list(general);
    }
}
int check_int(){
    int x;
    while (true) {
        string line;
        cout << "Enter an integer: ";
        getline(cin >> ws, line);
        try {
            size_t pos = 0;
            long val = stol(line, &pos);
            if(pos != line.size()){
                cout << "please enter a valid integer" << endl;
                continue;
            }
            x = static_cast<int>(val);
            break;
        }catch(...){
            cout << "please enter a valid integer" << endl;
        }
    }
    return x;
}

bool try_parse_int(const string &s, int &out){
    try{
        size_t pos = 0;
        long val = stol(s, &pos);
        if(pos != s.size()) return false;
        out = static_cast<int>(val);
        return true;
    }catch(...){
        return false;
    }
}
//--------------------------------------
int main(){
    General general;
    main_menu(general);
    return 0;
}