#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <climits>
#include <algorithm>
using namespace std;

int extract_min(vector<vector<int>> &tableau)
{
    int n = tableau.size();
    int m = tableau[0].size();
    int min = tableau[0][0];

    // replace min element with infinity
    // compare with right and below
    // exchange position with smallest
    int pos_i = 0, pos_j = 0;
    tableau[pos_i][pos_j] = INT_MAX;
    while (true)
    {
        int smallest = INT_MAX;
        bool down = false, right = false;

        if (pos_j + 1 < m && tableau[pos_i][pos_j + 1] < smallest)
        {
            smallest = tableau[pos_i][pos_j + 1];
            right = true;
        }
        if (pos_i + 1 < n && tableau[pos_i + 1][pos_j] < smallest)
        {
            smallest = tableau[pos_i + 1][pos_j];
            down = true;
        }
        if (!down && !right)
            break;

        if (down)
        {
            swap(tableau[pos_i][pos_j], tableau[pos_i + 1][pos_j]);
            pos_i += 1;
        }
        else if (right)
        {
            swap(tableau[pos_i][pos_j], tableau[pos_i][pos_j + 1]);
            pos_j += 1;
        }
    }
    return min;
}

void insert(vector<vector<int>> &tableau, int key)
{
    int n = tableau.size();
    int m = tableau[0].size();

    // find empty place for new element
    int key_i = -1, key_j = -1;
    for (int i = n - 1; i >= 0 && key_i == -1; i--)
    {
        for (int j = m - 1; j >= 0; j--)
        {
            if (tableau[i][j] == INT_MAX)
            {
                key_i = i;
                key_j = j;
                break;
            }
        }
    }
    tableau[key_i][key_j] = key;

    while (true)
    {
        int largest_i = key_i;
        int largest_j = key_j;
        int largest = tableau[key_i][key_j];

        // compare left and right
        if (key_j - 1 >= 0 && tableau[key_i][key_j - 1] > largest)
        {
            largest = tableau[key_i][key_j - 1];
            largest_i = key_i;
            largest_j = key_j - 1;
        }
        if (key_i - 1 >= 0 && tableau[key_i - 1][key_j] > largest)
        {
            largest = tableau[key_i - 1][key_j];
            largest_i = key_i - 1;
            largest_j = key_j;
        }

        // stop loop if key is already the largest
        if (largest_i == key_i && largest_j == key_j)
            break;

        // exchange position with largest
        swap(tableau[key_i][key_j], tableau[largest_i][largest_j]);
        key_i = largest_i;
        key_j = largest_j;
    }
}

int main()
{
    ifstream read("input.txt");
    ofstream write("output.txt");

    int N;
    read >> N;
    read.ignore();

    for (int i = 0; i < N; i++)
    {
        // read operation
        string line;
        getline(read, line);
        int option = stoi(line);

        // read elements to insert
        vector<int> input_insert;
        if (option == 1)
        {
            getline(read, line);
            stringstream ss(line);
            int x;
            while (ss >> x)
                input_insert.push_back(x);
        }

        // read tableau
        vector<vector<int>> tableau;
        while (getline(read, line))
        {
            if (line.empty())
                break;

            stringstream ss(line);
            vector<int> row;
            string token;
            while (ss >> token)
            {
                if (token == "x")
                    row.push_back(INT_MAX);
                else
                    row.push_back(stoi(token));
            }
            tableau.push_back(row);
        }

        // work on tableau
        switch (option)
        {
        case 1:
            write << "Insert ";
            for (int i = 0; i < input_insert.size(); i++)
            {
                write << input_insert[i];
                if (i != input_insert.size() - 1)
                    write << ' ';
                insert(tableau, input_insert[i]);
            }
            write << endl;
            break;
        case 2:
            write << "Extract-min ";
            write << extract_min(tableau) << endl;
            break;
        }

        // write tableau into file
        for (const auto &row : tableau)
        {
            for (int j = 0; j < row.size(); ++j)
            {
                if (row[j] == INT_MAX)
                    write << 'x';
                else
                    write << row[j];
                if (j + 1 < row.size())
                    write << ' ';
            }
            write << endl;
        }
        write << endl;
    }

    read.close();
    write.close();
    return 0;
}
