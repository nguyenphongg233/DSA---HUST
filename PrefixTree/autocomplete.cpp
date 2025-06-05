// 23 - 12 - 23

#include <bits/stdc++.h>
using namespace std;

#define read() ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define day() time_t now = time(0);char* x = ctime(&now);cerr<<"Right Now Is : "<<x<<"\n"

#define ii pair<int,int>
#define X first
#define Y second 

const long long MAX = (int)1e5 + 5;
const long long INF = (int)1e9;
const long long MOD = (int)1e9 + 7;

struct Node {
    Node *children[53];
    int frequency = 0;
    Node() {
        for (int i = 0; i < 53; i++) children[i] = NULL;
        frequency = 0;
    }
};

Node *root;

int getHash(char x) {
    if ((x >= 'a') && x <= 'z') return x - 'a';
    if ((x >= 'A') && x <= 'Z') return x - 'A' + 26;
    if (x == '-') return 52;
    return -1;
}

char reverseHash(int hash) {
    if (hash == 52) return '-';
    if (hash < 26) return char('a' + hash);
    return char('A' + hash - 26); 
}

void insertWord(string s) {
    int n = s.size();
    Node *cur = root;
    for (int i = 0; i < n; i++) {
        int h = getHash(s[i]);
        if (h == -1) return;
        if (cur->children[h] == NULL) {
            cur->children[h] = new Node();
        }
        cur = cur->children[h];
    }
    cur->frequency++;
}

bool searchWord(string s) {
    int n = s.size();
    Node *cur = root;
    for (int i = 0; i < n; i++) {
        int h = getHash(s[i]);
        if (h == -1 || cur->children[h] == NULL) return false;
        cur = cur->children[h];
    }
    return cur->frequency > 0;
}

void collectSuggestions(Node* cur, string path, vector<pair<int, string>> &result) {
    if (cur->frequency > 0) {
        result.push_back({-cur->frequency, path});
    }
    for (int i = 0; i < 53; i++) {
        if (cur->children[i]) {
            collectSuggestions(cur->children[i], path + reverseHash(i), result);
        }
    }
}

void suggestWords(string prefix) {
    Node* cur = root;
    for (char c : prefix) {
        int h = getHash(c);
        if (h == -1 || cur->children[h] == NULL) {
            cout << "\nNo suggestions found.\n";
            return;
        }
        cur = cur->children[h];
    }
    vector<pair<int, string>> result;
    collectSuggestions(cur, prefix, result);
    sort(result.begin(), result.end());
    for (auto it : result) {
        cout << it.second << " (" << -it.first << ")\n";
    }
}

void saveCompact(Node* cur, ofstream &fout) {
    for (int i = 0; i < 53; i++) {
        if (cur->children[i]) {
            int childCount = 0;
            for (int j = 0; j < 53; j++) {
                if (cur->children[i]->children[j]) childCount++;
            }
            fout << reverseHash(i) << " " << cur->children[i]->frequency << " " << childCount << "\n";
            saveCompact(cur->children[i], fout);
        }
    }
}

void saveTrie(string filename) {
    ofstream fout(filename);
    if (!fout) {
        cerr << "Error opening file to save.\n";
        return;
    }
    fout << "#\n0 0\n"; // root marker
    saveCompact(root, fout);
    fout.close();
}

Node* loadCompactDFS(ifstream &fin) {
    Node* cur = new Node();
    int freq, childCount;
    fin >> freq >> childCount;
    cur->frequency = freq;
    for (int i = 0; i < childCount; i++) {
        char c;
        int f, nextChild;
        fin >> c >> f >> nextChild;
        int h = getHash(c);
        cur->children[h] = loadCompactDFS(fin);
        cur->children[h]->frequency = f;
    }
    return cur;
}

void loadTrie(string filename) {
    ifstream fin(filename);
    if (!fin) {
        cerr << "Failed to open file.\n";
        return;
    }
    string marker;
    fin >> marker;
  //  cout << marker << '\n';
    root = loadCompactDFS(fin);
    fin.close();
}

signed main() {
    //read();
    root = new Node();
    string userFile = "user_trie.txt";

    cout << "Do you want to (1) Load from file or (2) Create new? ";
    int opt; cin >> opt;
    
    if (opt == 1) {
        loadTrie(userFile);
    } else {
        cout << "Enter words (end with #):\n";
        while (true) {
            string s; cin >> s;
            if (s == "#") break;
            insertWord(s);
        }
        saveTrie(userFile);
    }

    cout << "\nEnter prefix to suggest (min 2 chars, end with #):\n";
    while (true) {
        string prefix; cin >> prefix;
        if (prefix == "#") break;
        if (prefix.size() < 2) {
            cout << "Please enter at least 2 characters.\n";
            continue;
        }
        suggestWords(prefix);
    }
    return 0;
}
