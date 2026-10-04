#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

using namespace std;

struct TeamStats
{
    int goals_scored;
    int games_played;
};

TeamStats table[4];

atomic_flag team_locks[4] = {ATOMIC_FLAG_INIT, ATOMIC_FLAG_INIT, ATOMIC_FLAG_INIT, ATOMIC_FLAG_INIT};

void update_math_results(int team1_id, int team2_id, int team1_goals, int team2_goals)
{
    int id1 = team1_id;
    int id2 = team2_id;
    if (id1 > id2)
    {
        swap(id1, id2);
    }
    while (team_locks[id1].test_and_set(memory_order_acquire))
    {

    }
    while (team_locks[id2].test_and_set(memory_order_acquire))
    {
        
    }

    table[team1_id].goals_scored += team1_goals;
    table[team1_id].goals_played += 1;

    table[team2_id].goals_scored += team2_goals;
    table[team2_id].goals_played += 1;

    team_lock[id2].clear(memury_order_release);
    team_lock[id1].clear(memury_order_release);
}

int main()
{
    thread t1(update_math_results, 0,1,2,1);
    thread t2(update_math_results, 1,2,3,3);

    t1.join();
    t2.join();

    char team_names[4] = {'A', 'B', 'C', 'D'};
    for (int i = 0; i < 4; i++)
    {
        cout << "Team" << team_names[i] << "Goals = " << table[i].goals_scored << ", Games = " << table[i].games_played << endl;
    }

    return 0;
}