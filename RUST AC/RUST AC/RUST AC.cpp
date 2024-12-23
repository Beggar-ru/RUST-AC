#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

using namespace std;
using namespace filesystem;

struct IpEntry {
    string ip;
    string name;
};

const string kFileName = "RUST-AC_IP.txt";

//Create a file with ip-servers and name servers in document directory
bool createOrOpenFile(const string& path){
    if (!exists(path)){
        ofstream file(path);
        if (!file){
            cerr << "Error create file: " << path << endl;
            return false;
        }
        file.close();
        return true;
    }
    return true;
}

void saveIp(const IpEntry& entry, const string path){
    ofstream file(path, ios_base::app);
    if(file.is_open()){
        file << entry.ip << " " << entry.name << "\n";
        file.close();
    }
}

void loadIps(vector<IpEntry>& entries, const string& path){
    ifstream file(path);
    if(file.is_open()){
        string line;
        while (getline(file, line)){
            size_t pos = line.find(' ');
            if (pos != string::npos) {
                IpEntry entry {line.substr(0, pos), line.substr(pos + 1)};
                entries.push_back(entry);
            }
        }
        file.close();
    }
}

int main(){
    string documentsPath = path(current_path()).parent_path().append("/Documents").string();
    string fullPath = documentsPath + "/" + kFileName;

    if (!createOrOpenFile(fullPath)){
        return 1;
    }

    vector<IpEntry> saved_ips;
    loadIps(saved_ips, fullPath);

    if (!saved_ips.empty()){
        int choice;
        cout << "Select IP adress for connect: \n";
        for (size_t i = 0; i < saved_ips.size(); i++){
            cout << i + 1 << ". " << saved_ips[i].name << ": " << saved_ips[i].ip << "\n";
        }
        cout << "Your select: ";
        cin >> choice;

        if (choice > 0 && choice <= saved_ips.size()){
            string selected_ip = saved_ips[choice - 1].ip;
            cout << "Your selected IP: " << selected_ip << '\n';
        }
        else{
            cerr << "Wrong choice.\n";
        }
    }
    else{
        string ip_adress;
        cout << "Enter IP adress server: ";
        getline(cin, ip_adress);

        char answer;
        cout << "Save this IP? (y/n): ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y'){
            string name;
            cout << "Enter name server for this IP: ";
            cin.ignore();
            getline(cin, name);

            IpEntry new_entry {ip_adress, name};
            saveIp(new_entry, fullPath);
        }
    }
    return 0;
}
