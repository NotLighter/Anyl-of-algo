#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;

const int N = 4; // Number of players and teams

// Helper function to find the rank of a player in a team's preference list
int getTeamPreferenceRank(const vector<vector<int>>& teamPref, int team, int player) {
    for (int i = 0; i < N; ++i) {
        if (teamPref[team][i] == player)
            return i;
    }
    return N; // Should not happen
}

void galeShapley(const vector<vector<int>>& playerPref, const vector<vector<int>>& teamPref) {
    vector<int> teamMatch(N, -1);          // teamMatch[i] = player matched with team i
    vector<int> playerMatch(N, -1);        // playerMatch[i] = team matched with player i
    vector<int> nextProposal(N, 0);        // nextProposal[i] = index of next team to propose to

    queue<int> freePlayers;
    for (int i = 0; i < N; ++i) {
        freePlayers.push(i);
    }

    while (!freePlayers.empty()) {
        int player = freePlayers.front();
        freePlayers.pop();

        int team = playerPref[player][nextProposal[player]];
        nextProposal[player]++;

        if (teamMatch[team] == -1) {
            // Team is free
            teamMatch[team] = player;
            playerMatch[player] = team;
        } else {
            int currentPlayer = teamMatch[team];
            if (getTeamPreferenceRank(teamPref, team, player) < getTeamPreferenceRank(teamPref, team, currentPlayer)) {
                // Team prefers new player
                teamMatch[team] = player;
                playerMatch[player] = team;
                playerMatch[currentPlayer] = -1;
                freePlayers.push(currentPlayer);
            } else {
                // Team rejects player
                freePlayers.push(player);
            }
        }
    }

    // Output results
    cout << "\nFinal Draft Result (Team -> Player):\n";
    for (int team = 0; team < N; ++team) {
        cout << "Team " << team << " -> Player " << teamMatch[team] << endl;
    }
}

int main() {
    // Example preferences (indices represent IDs)
    vector<vector<int>> playerPref = {
        {0, 1, 2, 3}, // Player 0 prefers team 0 > 1 > 2 > 3
        {1, 0, 2, 3},
        {0, 2, 1, 3},
        {2, 1, 3, 0}
    };

    vector<vector<int>> teamPref = {
        {2, 1, 0, 3}, // Team 0 prefers player 2 > 1 > 0 > 3
        {0, 1, 2, 3},
        {1, 2, 3, 0},
        {3, 0, 1, 2}
    };

    galeShapley(playerPref, teamPref);

    return 0;
}
