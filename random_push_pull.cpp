#include <bits/stdc++.h>
#include <queue>
using namespace std;
using ll = long long;

/* ---------------- ITERATION RULE ---------------- */
int iterations_for_n(ll n)
{
    if (n <= 1000)
        return 1000; // 1000
    if (n <= 10000)
        return 150; // 150
    if (n <= 100000)
        return 100; // 100
    if (n <= 1000000)
        return 50; // 50
    return 10;     // 30
}

/* ---------------- CHUNG–LU GRAPH ---------------- */
vector<vector<int>> generate_chung_lu_graph(int n, double beta = 2.5)
{
    vector<double> w(n);
    double W = 0.0;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> U(0.0, 1.0);

    /* generate weights ~ power law */
    for (int i = 0; i < n; i++)
    {
        double u = U(gen);
        w[i] = pow(1 - u, -1.0 / (beta - 1));
        W += w[i];
    }

    vector<vector<int>> adj(n);

    /* Chung–Lu edge generation */
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            double p = min(w[i] * w[j] / W, 1.0);
            if (U(gen) < p)
            {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
        }
    }

    return adj;
}

/* ---------------- PUSH–PULL PROTOCOL ---------------- */
struct Result
{
    int rounds;
};

Result run_push_pull(const vector<vector<int>> &adj, double eps = 0.05)
{
    int n = adj.size();
    vector<bool> informed(n, false);

    random_device rd;
    mt19937 gen(rd());

    /* random start node */
    int start = gen() % n;
    informed[start] = true;
    int informed_cnt = 1;

    int rounds = 0;
    int target = (int)((1.0 - eps) * n);

    while (informed_cnt < target)
    {
        rounds++;
        vector<bool> next = informed;

        for (int u = 0; u < n; u++)
        {
            if (adj[u].empty())
                continue;

            uniform_int_distribution<> dis(0, adj[u].size() - 1);
            int v = adj[u][dis(gen)];

            /* pull */
            if (!informed[u] && informed[v])
            {
                next[u] = true;
            }

            /* push */
            if (informed[u] && !informed[v])
            {
                next[v] = true;
            }
        }

        for (int i = 0; i < n; i++)
        {
            if (!informed[i] && next[i])
            {
                informed[i] = true;
                informed_cnt++;
            }
        }
    }

    return {rounds};
}

/* ---------------- MAIN EXPERIMENT ---------------- */
int main()
{
    int n = 100;
    auto adj = generate_chung_lu_graph(n);
    vector<int> visited(n, 0);
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (visited[i] == 0)
        {
            visited[i] = 1;
            count++;
            queue<int> qu;
            qu.push(i);
            while (!qu.empty())
            {
                int node = qu.front();
                qu.pop();
                for (int j = 0; j < adj[node].size(); j++)
                {
                    int new_node = adj[node][j];
                    if (visited[new_node] == 0)
                    {
                        visited[new_node] = 1;
                        qu.push(new_node);
                    }
                }
            }
        }
    }
    cout << count;
    /*  ofstream file("F:\\push_pull_results1.csv");
       file << "n,iterations,avg_rounds\n";

       for (ll n = 100; n <= 1000000000; n = ll(n * 1.5))
       {
           int iters = iterations_for_n(n);
           double sum_rounds = 0;

           cout << "Running n = " << n << " (" << iters << " iterations)\n";

           for (int it = 0; it < iters; it++)
           {
               auto adj = generate_chung_lu_graph(n);
               Result r = run_push_pull(adj);
               sum_rounds += r.rounds;
           }

           file << n << "," << iters << "," << sum_rounds / iters << "\n";
       }

       file.close();
       cout << "\nResults saved to push_pull_results.csv\n";*/
    return 0;
}
