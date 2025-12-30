#include <iostream>
#include <vector>
#include <utility>
using namespace std;

template <typename E>
class Tree
{
public:
    void addLast(int key, const E e);
    void removeRoot();
    E getRoot() const;

private:
    void upHeapify(int position);
    void downHeapify(int position);
    vector<pair<int, E>> V;
};

int main()
{
    Tree<char> T;
    T.addLast(5, 'A');
    T.addLast(4, 'C');
    T.addLast(20, 'Z');
    cout << T.getRoot() << endl;
    return 0;
}

template <typename E>
void Tree<E>::upHeapify(int position)
{
    while (position > 0)
    {
        int parent = position / 2;
        if (V[position].first > V[parent].first)
        {
            swap(V[position], V[parent]);
            position = parent;
        }
        else
            break;
    }
}

template <typename E>
void Tree<E>::downHeapify(int position)
{
    int size = V.size();
    while (1)
    {
        int left = position * 2;
        int right = left + 1;
        int largest = position;
        if (left < size && V[left].first > V[position].first)
            largest = left;
        if (right < size && V[right].first > V[position].first)
            largest = right;

        if (largest != position)
        {
            swap(V[largest], V[position]);
            position = largest;
        }
        else
            break;
    }
}

template <typename E>
void Tree<E>::addLast(int key, const E e)
{
    V.push_back(make_pair(key, e));
    upHeapify(V.size() - 1);
}

template <typename E>
E Tree<E>::getRoot() const
{
    return V[0].second;
}

template <typename E>
void Tree<E>::removeRoot()
{
    V[0] = V.back();
    V.pop_back();
    downHeapify(0);
}
