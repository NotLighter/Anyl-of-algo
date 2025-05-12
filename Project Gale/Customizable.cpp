#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include <algorithm>
using namespace std;

int getPreferenceRank(const vector<string>& preferenceList, const string& name) {
    for (int i = 0; i < preferenceList.size(); ++i) {
        if (preferenceList[i] == name)
            return i;
    }
    return preferenceList.size(); // Not found
}

int main() {
    int n;
    cout << "Enter number of teams/players: ";
    cin >> n;
    cin.ignore(); // to clear newline from buffer

    vector<string> players(n), teams(n);
    unordered_map<string, int> playerIndex, teamIndex;

    cout << "\nEnter names of players:\n";
    for (int i = 0; i < n; ++i) {
        getline(cin, players[i]);
        playerIndex[players[i]] = i;
    }

    cout << "\nEnter names of teams:\n";
    for (int i = 0; i < n; ++i) {
        getline(cin, teams[i]);
        teamIndex[teams[i]] = i;
    }

    // Player preferences
    vector<vector<string>> playerPreferences(n);
    cout << "\nEnter preferences of each player (team names in order):\n";
    for (int i = 0; i < n; ++i) {
        cout << "Preferences for " << players[i] << ": ";
        for (int j = 0; j < n; ++j) {
            string team;
            cin >> team;
            playerPreferences[i].push_back(team);
        }
    }

    cin.ignore(); // flush newline

    // Team preferences
    vector<vector<string>> teamPreferences(n);
    cout << "\nEnter preferences of each team (player names in order):\n";
    for (int i = 0; i < n; ++i) {
        cout << "Preferences for " << teams[i] << ": ";
        for (int j = 0; j < n; ++j) {
            string player;
            cin >> player;
            teamPreferences[i].push_back(player);
        }
    }

    // Gale-Shapley Algorithm
    vector<int> teamMatch(n, -1); // teamMatch[i] = player index
    vector<int> playerMatch(n, -1); // playerMatch[i] = team index
    vector<int> nextProposal(n, 0);

    queue<int> freePlayers;
    for (int i = 0; i < n; ++i)
        freePlayers.push(i);

    while (!freePlayers.empty()) {
        int p = freePlayers.front();
        freePlayers.pop();

        string preferredTeam = playerPreferences[p][nextProposal[p]];
        nextProposal[p]++;
        int t = teamIndex[preferredTeam];

        if (teamMatch[t] == -1) {
            // Team is free
            teamMatch[t] = p;
            playerMatch[p] = t;
        } else {
            int current = teamMatch[t];
            // Compare preference
            if (getPreferenceRank(teamPreferences[t], players[p]) <
                getPreferenceRank(teamPreferences[t], players[current])) {
                // Team prefers new player
                teamMatch[t] = p;
                playerMatch[p] = t;
                playerMatch[current] = -1;
                freePlayers.push(current);
            } else {
                // Rejected
                freePlayers.push(p);
            }
        }
    }

    // Output matches
    cout << "\nFinal Draft Result:\n";
    for (int i = 0; i < n; ++i) {
        cout << teams[i] << " drafted " << players[teamMatch[i]] << endl;
    }

    return 0;
}
