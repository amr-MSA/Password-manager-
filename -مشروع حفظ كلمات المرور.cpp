#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstring>

using namespace std;

// --- Prototypes ---
void mainMenu();
void savePassword();
void loadPassword();
void showPopularPlatforms();
string encrypt(string data);
string decrypt(string data);
bool validateInput(string data, string type);
bool isPlatformExists(string platform);
string toUpperCase(string str);

// --- Global Constants ---
const string GENERAL_FILE = "general_vault.txt";
const string PLATFORMS_FILE = "platforms.txt";
const string EMAILS_FILE = "emails_matrix.txt";
const int SHIFT = 4; // Encryption key shift

int main() {
    mainMenu();
    return 0;
}

// Simple Caesar Cipher Encryption
string encrypt(string data) {
    string output = data;
    for (int i = 0; i < output.length(); i++) {
        output[i] = output[i] + SHIFT;
    }
    return output;
}

// Simple Caesar Cipher Decryption
string decrypt(string data) {
    string output = data;
    for (int i = 0; i < output.length(); i++) {
        output[i] = output[i] - SHIFT;
    }
    return output;
}

string toUpperCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

bool validateInput(string data, string type) {
    if (data.empty()) return false;
    if (type == "digit") {
        for (char const &c : data) {
            if (isdigit(c) == 0) return false;
        }
    }
    return true;
}

void showPopularPlatforms() {
    cout << "\n--- Popular Platforms ---" << endl;
    cout << "1. GOOGLE    2. FACEBOOK  3. GITHUB" << endl;
    cout << "4. LINKEDIN  5. TWITTER   6. INSTAGRAM" << endl;
    cout << "7. NETFLIX   8. ADOBE     9. MICROSOFT" << endl;
    cout << "0. OTHER (Manual Input)" << endl;
}

bool isPlatformExists(string platform) {
    ifstream file(PLATFORMS_FILE);
    string line;
    while (getline(file, line)) {
        if (line == platform) return true;
    }
    return false;
}

void mainMenu() {
    string choice;
    while (true) {
        cout << "\n================================" << endl;
        cout << "   PASSWORD MANAGER (LIMITLESS)" << endl;
        cout << "================================" << endl;
        cout << "1. Save a password" << endl;
        cout << "2. Load a password" << endl;
        cout << "Exit (Any other key)" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (choice == "1") savePassword();
        else if (choice == "2") loadPassword();
        else break;
    }
}

void savePassword() {
    string platform, email, username, pass1, pass2, choice;
    string popular[] = {"", "GOOGLE", "FACEBOOK", "GITHUB", "LINKEDIN", "TWITTER", "INSTAGRAM", "NETFLIX", "ADOBE", "MICROSOFT"};

    showPopularPlatforms();
    cout << "Select (1-9) or 0 for Other: ";
    cin >> choice;

    if (choice == "0") {
        cout << "Enter Platform Name: ";
        cin.ignore();
        getline(cin,platform);
    } else if (stoi(choice) >= 1 && stoi(choice) <= 9) {
        platform = popular[stoi(choice)];
    } else return;

    platform = toUpperCase(platform);

    // Save platform if not exists (Dynamic Appendment)
    if (!isPlatformExists(platform)) {
        ofstream pFile(PLATFORMS_FILE, ios::app);
        pFile << platform << endl;
        pFile.close();
    }

    cout << "Enter Email: "; cin >> email;
    if (email == "0") return;

    // Emails Matrix simulation (Platform | Email)
    ofstream eFile(EMAILS_FILE, ios::app);
    eFile << platform << "\t\t" << email << endl;
    eFile.close();

    while (true) {
        cout << "Enter Password: "; cin >> pass1;
        cout << "Confirm Password: "; cin >> pass2;
        if (pass1 == pass2) break;
        cout << "Passwords do not match! Try again." << endl;
    }

    cout << "Enter Username: "; cin >> username;

    // Save to General File
    ofstream gFile(GENERAL_FILE, ios::app);
    gFile << platform << " " << username << " " << email << " " << encrypt(pass1) << endl;
    gFile.close();

    cout << "\n[Success] Data saved successfully." << endl;
}

void loadPassword() {
    ifstream pFile(PLATFORMS_FILE);
    vector<string> platforms;
    string line;
    int i = 1;

    cout << "\n--- Available Platforms ---" << endl;
    while (getline(pFile, line)) {
        platforms.push_back(line);
        cout << i++ << ". " << line << endl;
    }
    pFile.close();

    if (platforms.empty()) {
        cout << "No platforms found." << endl;
        return;
    }

    int pChoice;
    cout << "Select Platform Number (0 to back): ";
    cin >> pChoice;
    if (pChoice <= 0 || pChoice > platforms.size()) return;

    string targetPlatform = platforms[pChoice - 1];
    
    // Show accounts in that platform
    ifstream gFile(GENERAL_FILE);
    string p, u, e, pass;
    vector<pair<string, string>> accounts; // Username, Password
    
    i = 1;
    cout << "\nAccounts for " << targetPlatform << ":" << endl;
    while (gFile >> p >> u >> e >> pass) {
        if (p == targetPlatform) {
            accounts.push_back({u, pass});
            cout << i++ << ". " << u << endl;
        }
    }
    gFile.close();

    int uChoice;
    cout << "Select Account Number to see Password (0 to back): ";
    cin >> uChoice;
    if (uChoice <= 0 || uChoice > accounts.size()) return;

    cout << "\n>>> Password: " << decrypt(accounts[uChoice - 1].second) << endl;
}
