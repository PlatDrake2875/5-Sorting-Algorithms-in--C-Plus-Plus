#include <bits/stdc++.h>

/// NU UITA DE LONG LONG!!!
using namespace std;

ifstream in("t.in");
ofstream out("t.out");
//ofstream out("proiect_prlong long_data.xlsx");
long long N, A_Max;
//chrono::duration<long long int, ratio<111,10011>> TIMP_STL;
double TIMP_STL;

void afis(vector<long long> A)
{
    for(int i = 1; i <= N; ++i)
    {
        out << A[i] << " ";
    }
    out << '\n';
}

int BUCKET_SIZE, RADIX_SIZE;

int get_bucket(long long x, int i)
{
    return ((x >> (i * BUCKET_SIZE)) & RADIX_SIZE);
}

void Counting_Sort_Radix(vector<long long> &A, vector<long long> &B, int bucket)
{
    int cnt[(1 << BUCKET_SIZE)];
    int poz[(1 << BUCKET_SIZE)];
    int M;

    memset(cnt, 0, sizeof(cnt));

    for(int i = 1; i <= N; ++i)
    {
        cnt[get_bucket(A[i] + A_Max, bucket)]++;
    }

    poz[0] = 0;

    for(int i = 1; i < (1 << BUCKET_SIZE); ++i)
    {
        poz[i] = poz[i - 1] + cnt[i - 1];
    }

    for(int i = 1; i <= N; ++i)
    {
        M = get_bucket(A[i] + A_Max, bucket);
        B[++poz[M]] = A[i];
    }
}

void Radix_Sort(vector<long long> &A)
{
    vector<long long> aux(N + 1);
    int BUCKET_NR;
    //cout << "Introduceti marimea bucket-ului, in biti (Acesta poate fi orice putere de-a lui 2): ";
    //cin >> BUCKET_SIZE;
    BUCKET_SIZE = 8;
    BUCKET_NR = 8 * sizeof(N) / BUCKET_SIZE;
    RADIX_SIZE  = (1 << BUCKET_SIZE) - 1;
    //out << BUCKET_NR << " " << BUCKET_SIZE << " " << bitset<16> (RADIX_SIZE) << '\n';
    for(int i = 0; i < BUCKET_NR; ++i)
    {
        if(i % 2 == 0)
        {
            Counting_Sort_Radix(A, aux, i);
        }
        else
        {
            Counting_Sort_Radix(aux, A, i);
        }
    }
}

vector<long long> temp;

void Merge_Sort(vector<long long> &A, int left, int right)
{
    if(left == right)
        return;
    int mid = (left + right) / 2;
    Merge_Sort(A, left, mid);
    Merge_Sort(A, mid + 1, right);

    for(int i = left, j = mid + 1, k = 0; i <= mid || j <= right;)
        if(j > right || (i <= mid && A[i] < A[j]))
            temp[k++] = A[i++];
        else
            temp[k++] = A[j++];
    for(int k = left; k <= right; ++k)
        A[k] = temp[k - left];
    //N_Max = 260238 (Ramane fara memorie daca declar temp-ul in stiva subprogramului)
}
/*
Fun fact: Daca generez gap-urile dupa formula Sedgewick-Knuth (1985), atunci o sa am nevoie de 10^309 elemente ca sa le sortez in O(N^1.1). Interesant, nu?
*/
void Shell_Sort(vector<long long> &A, int gap_type)
{
    vector<long long> Gaps;
    if(gap_type == 1)
    {
        //varainta O(N*log^2(N))
        int p = log2(N);
        int q = log10(N) / log10(3);
        int x, y = x = 1, curr;
        for(int i = 0; i <= p; ++i)
        {
            for(int j = 0; j <= q; ++j)
            {
                curr = x * y;
                if(curr > N)
                    break;
                Gaps.push_back(curr);
                y *= 3;
            }
            x *= 2;
            y = 1;
        }
    }
    else
    {
        for(int i = 1; i <= N; i *= 2)
            Gaps.push_back(i);
    }
    sort(Gaps.begin(), Gaps.end(), greater<int>());
    int i, j;
    for(auto gap : Gaps)
    {
        for(i = gap; i <= N; ++i)
        {
            int aux = A[i];
            for(j = i; j >= gap && A[j - gap] > aux; j -= gap)
                A[j] = A[j - gap];
            A[j] = aux;
        }
    }
}

void heapify(vector<long long> &A, int node, int N)
{
    int Nod_Max = node;
    int l = 2 * node;
    int r = 2 * node + 1;

    if(l <= N && A[l] > A[Nod_Max])
        Nod_Max = l;
    if(r <= N && A[r] > A[Nod_Max])
        Nod_Max = r;

    if(Nod_Max != node)
    {
        swap(A[node], A[Nod_Max]);
        heapify(A, Nod_Max, N);
    }
}

void Heap_Sort(vector<long long> &A)
{
    for(int i = N / 2; i >= 1; --i)
    {
        heapify(A, i, N);
    }
    for(int i = N; i >= 2; --i)
    {
        swap(A[1], A[i]);
        heapify(A, 1, i - 1);
    }
}

vector<long long> f, index, B;

void Counting_Sort(vector<long long> &A)
{
    for(int i = 1; i <= N; ++i)
        f[A[i] + A_Max]++;
    for(int i = 1; i <= 2 * A_Max; ++i)
        f[i] += f[i - 1];
    for(int i = 1; i <= N; ++i)
    {
        B[f[A[i] + A_Max]--] = A[i];
    }
    A = B;
}

void check_if_sorted(vector<long long> A)
{
    for(int i = 1; i < N; ++i)
    {
        if(A[i] > A[i + 1])
        {
            out << "Sir nesortat ";
            return;
        }
    }
    out << "Sir sortat ";
}

void sortari(int cnt, vector<long long> A)
{
    //auto t = clock();
    auto start = chrono::high_resolution_clock::now();
    switch(cnt)
    {
    case 0:
        sort(A.begin(), A.end());
        out << "Sortare STL: ";
        break;
    case 1:
        Radix_Sort(A);
        out << "Radix Sort: ";
        break;
    case 2:
        temp.assign(N + 1, 0);
        Merge_Sort(A, 1, N);
        out << "Merge Sort: ";
        break;
    case 3:
        Shell_Sort(A, 1);
        out << "Shell Sort cu gap-uri Pratt: ";
        break;
    case 4:
        Shell_Sort(A, 2);
        out << "Shell Sort cu gap-uri de forma floor(N / 2^k): ";
        break;
    case 5:
        Heap_Sort(A);
        out << "Heap Sort: ";
        break;
    case 6:
        f.assign(2 * A_Max + 1, 0);
        B.assign(N + 1, 0);
        Counting_Sort(A);
        out << "Counting Sort: ";
        break;
    }
    //t = clock() - t;
    //double time_taken = ((double)t)/CLOCKS_PER_SEC;
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    check_if_sorted(A);
    //out << time_taken << '\n';
    out << "in " << duration.count() << " de milisecunde.";
    if(!cnt)
    {
        TIMP_STL = duration.count();
        out << '\n';
    }
    else
    if(cnt)
    {
        double Timp_dif = TIMP_STL / duration.count();
        out << "\nComparat cu sort-ul din STL, aceasta sortare este de ";
        if(Timp_dif < 1)
        {
            out << 1 / Timp_dif << " ori mai lenta.\n";
        }
        else
        {
            out << Timp_dif << " ori mai rapida.\n";
        }
    }
    //afis(A);
}

int main()
{
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    int T;
    in >> T;
    while(T--)
    {
        in >> N >> A_Max;
        out << "Pentru " << N << " elemente si valoarea maxima " << A_Max << " avem urmatoarele sortari cu timpii lor de executie:\n\n";
        //out << N << " " << A_Max << '\n';
        vector<long long> nums(N + 1);
        for(int i = 1; i <= N; ++i)
        {
            nums[i] = uniform_int_distribution<long long>(-A_Max + 1, A_Max)(rng);
        }

        for(int i = 0; i <= 6; ++i)
        {
            sortari(i, nums);
        }
        out << '\n';
    }
    /*vector<pair<int, int>> A;
    int v1[17] = {10, 100, 1000, 10000, 100000, 1000000, 100000000, 100000000, 5, 50, 500, 5000, 50000, 500000, 5000000, 50000000};
    for(int i = 0; i < 15; ++i)
    {
        for(long long j = i; j < 16; ++j)
        {
            A.push_back({v1[i], v1[j]});
            //out << v1[i] << " " << v1[j] << '\n';
        }
    }
    sort(A.begin(), A.end());
    for(auto i : A)
    {
        out << i.first << " " << i.second << '\n';
    }*/
    return 0;
}
