#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    string line;
    string line2;
    int minn=1e9;
    vector<string> argmins;
    int nb_lines=0;
    while(getline(cin,line))
    {
        getline(cin,line2);
        nb_lines+=2;
        if(line2[0]=='>') continue;
        int num=atoi(line2.c_str());
        if(num==0) continue;
        if(num<minn)
        {
            minn=num;
            //cerr << "best so far: " << minn << endl;
            argmins.clear();
            argmins.push_back(line);
        }
        else if(num==minn)
        {
            argmins.push_back(line);
        }
    }
    cout << minn << endl;
    for(string s : argmins)
    {
        cout << s << endl;
    }
    cerr << nb_lines << " lines were read by minPicker" << endl;
    return 0;
}
