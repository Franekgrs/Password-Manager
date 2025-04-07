#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <locale.h>
#include <algorithm>

using namespace std;

struct PasswordEntry {
    string name;
    string password;
    string category;
    string website;
    string login;
};

class PasswordManager {
private:
    string sourceFile;
    string masterPassword;
    vector<PasswordEntry> passwords;

public:
    PasswordManager(const string& file, const string& password) {
        sourceFile = file;
        masterPassword = password;
    }

    bool loadPasswords() {
        ifstream inputFile(sourceFile);
        if (!inputFile) {
            cout << "Nie mozna otworzyc pliku zrodlowego." << endl;
            return false;
        }

        passwords.clear();
        string line;
        while (getline(inputFile, line)) {
            if (!line.empty()) {
                PasswordEntry entry = decryptEntry(line);
                passwords.push_back(entry);
            }
        }
        inputFile.close();

        return true;
    }

    void savePasswords() {
        ofstream outputFile(sourceFile);
        if (!outputFile) {
            cout << "Nie mozna zapisac pliku zrodlowego." << endl;
            return;
        }

        for (const auto& entry : passwords) {
            string encryptedEntry = encryptEntry(entry);
            outputFile << encryptedEntry << endl;
        }
        outputFile.close();
    }

    void runCommandLoop() {
        string command;
        while (true) {
            cout << "Wprowadz komende: ";
            getline(cin, command);

            if (command == "quit") {
                break;
            }
            else if (command == "list") {
                listPasswords();
            }
            else if (command == "search") {
                searchPasswords();
            }
            else if (command == "add") {
                addPassword();
            }
            else if (command == "edit") {
                editPassword();
            }
            else if (command == "remove") {
                removePassword();
            }
            else if (command == "add_category") {
                addCategory();
            }
            else if (command == "remove_category") {
                removeCategory();
            }
            else {
                cout << "Nieznana komenda." << endl;
            }
        }
    }

private:
    // Szyfrowanie wpisu
    string encryptEntry(const PasswordEntry& entry) {
        string encryptedEntry;
        string data = entry.name + ";" + entry.password + ";" + entry.category + ";" + entry.website + ";" + entry.login;

        for (size_t i = 0; i < data.length(); ++i) {
            encryptedEntry += data[i] ^ masterPassword[i % masterPassword.length()];
        }

        return encryptedEntry;
    }

    // Deszyfrowanie wpisu
    PasswordEntry decryptEntry(const string& encryptedEntry) {
        PasswordEntry entry;
        string decryptedEntry;

        for (size_t i = 0; i < encryptedEntry.length(); ++i) {
            decryptedEntry += encryptedEntry[i] ^ masterPassword[i % masterPassword.length()];
        }

        stringstream ss(decryptedEntry);
        getline(ss, entry.name, ';');
        getline(ss, entry.password, ';');
        getline(ss, entry.category, ';');
        getline(ss, entry.website, ';');
        getline(ss, entry.login, ';');

        return entry;
    }


    void listPasswords() {
        // Wyświetlanie haseł
        for (const auto& entry : passwords) {
            cout << "Nazwa: " << entry.name << endl;
            cout << "Haslo: " << entry.password << endl;
            cout << "Kategoria: " << entry.category << endl;
            cout << "Strona WWW / Serwis: " << entry.website << endl;
            cout << "Login: " << entry.login << endl;
            cout << "-----------------------" << endl;
        }
    }

    void searchPasswords() {
        // Wyszukiwanie haseł
        string searchTerm;
        cout << "Wprowadz wyszukiwane slowo: ";
        getline(cin, searchTerm);
        vector<PasswordEntry> matchingPasswords;
        for (const auto& entry : passwords) {
            if (entry.name.find(searchTerm) != string::npos ||
                entry.category.find(searchTerm) != string::npos ||
                entry.website.find(searchTerm) != string::npos ||
                entry.login.find(searchTerm) != string::npos) {
                matchingPasswords.push_back(entry);
            }
        }

        if (matchingPasswords.empty()) {
            cout << "Brak pasujacych hasel." << endl;
        }
        else {
            cout << "Pasujace hasla:" << endl;
            for (const auto& entry : matchingPasswords) {
                cout << "Nazwa: " << entry.name << endl;
                cout << "Haslo: " << entry.password << endl;
                cout << "Kategoria: " << entry.category << endl;
                cout << "Strona WWW / Serwis: " << entry.website << endl;
                cout << "Login: " << entry.login << endl;
                cout << "-----------------------" << endl;
            }
        }
    }

    void addPassword() {
        // Dodawanie hasła
        PasswordEntry newEntry;
        cout << "Nazwa: ";
        getline(cin, newEntry.name);
        cout << "Haslo: ";
        getline(cin, newEntry.password);
        cout << "Kategoria: ";
        getline(cin, newEntry.category);
        cout << "Strona WWW / Serwis (opcjonalne): ";
        getline(cin, newEntry.website);
        cout << "Login (opcjonalne): ";
        getline(cin, newEntry.login);

        passwords.push_back(newEntry);
        cout << "Haslo zostalo dodane." << endl;
    }

    void editPassword() {
        // Edytowanie hasła
        string name;
        cout << "Podaj nazwe hasla do edycji: ";
        getline(cin, name);

        bool found = false;
        for (auto& entry : passwords) {
            if (entry.name == name) {
                cout << "Nowe haslo: ";
                getline(cin, entry.password);
                cout << "Nowa kategoria: ";
                getline(cin, entry.category);
                cout << "Nowa strona WWW / Serwis (opcjonalne): ";
                getline(cin, entry.website);
                cout << "Nowy login (opcjonalne): ";
                getline(cin, entry.login);
                cout << "Haslo zostało zaktualizowane." << endl;
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Nie znaleziono hasla o podanej nazwie." << endl;
        }
    }

    void removePassword() {
        // Usuwanie hasła
        string name;
        cout << "Podaj nazwe hasla do usuniecia: ";
        getline(cin, name);

        int count = 0;
        for (auto it = passwords.begin(); it != passwords.end(); ) {
            if (it->name == name) {
                it = passwords.erase(it);
                count++;
            }
            else {
                ++it;
            }
        }

        if (count == 0) {
            cout << "Nie znaleziono hasla o podanej nazwie." << endl;
        }
        else {
            cout << "Usunieto " << count << " haslo(a)." << endl;
        }
    }

    void addCategory() {
        // Dodawanie kategorii
        string category;
        cout << "Podaj nazwe nowej kategorii: ";
        getline(cin, category);
        // Sprawdzenie, czy kategoria już istnieje
        auto it = find_if(passwords.begin(), passwords.end(), [&](const PasswordEntry& entry) {
            return entry.category == category;
        });

        if (it != passwords.end()) {
            cout << "Kategoria już istnieje." << endl;
        }
        else {
            cout << "Dodano nowa kategorie: " << category << endl;
        }
    }

    void removeCategory() {
        // Usuwanie kategorii wraz z powiązanymi hasłami
        string category;
        cout << "Podaj nazwe kategorii do usuniecia: ";
        getline(cin, category);

        int count = 0;
        for (auto it = passwords.begin(); it != passwords.end(); ) {
            if (it->category == category) {
                it = passwords.erase(it);
                count++;
            }
            else {
                ++it;
            }
        }

        if (count == 0) {
            cout << "Nie znaleziono kategorii o podanej nazwie." << endl;
        }
        else {
            cout << "Usunieto kategorie \"" << category << "\" wraz z " << count << " haslami." << endl;
        }
    }
};


int main() {
    setlocale(LC_ALL, "");
    string sourceFile;
    cout << "Podaj nazwe pliku zrodlowego: ";
    getline(cin, sourceFile);
    string masterPassword;
    cout << "Podaj haslo glowne: ";
    getline(cin, masterPassword);

    PasswordManager manager(sourceFile, masterPassword);
    if (manager.loadPasswords()) {
        manager.runCommandLoop();
        manager.savePasswords();
    }

    return 0;
}
