#include <bits/stdc++.h>
#include <cstdlib>
#define write_data  1
#define K  1024
using namespace std;

unsigned long Cache_size,Block_size,Block_numbers,Associative;
int interval ;


unsigned long to_log2(const unsigned long n)
{
    //give n = 2^x , return  x
    if(n%2!=0) return -1;
    unsigned long sum = 0;
    int m=n;
    while(m>>=1) sum++;
    return sum;
}
string hex_to_binary(const string &hexdec)
{
    long int i = 0;
    string s="";
    while (hexdec[i]) {

        switch (hexdec[i]) {
        case '0':
            s +=  "0000";
            break;
        case '1':
            s +=  "0001";
            break;
        case '2':
            s +=  "0010";
            break;
        case '3':
            s +=  "0011";
            break;
        case '4':
            s +=  "0100";
            break;
        case '5':
            s +=  "0101";
            break;
        case '6':
            s +=  "0110";
            break;
        case '7':
            s +=  "0111";
            break;
        case '8':
            s +=  "1000";
            break;
        case '9':
            s +=  "1001";
            break;
        case 'A':
        case 'a':
            s +=  "1010";
            break;
        case 'B':
        case 'b':
            s +=  "1011";
            break;
        case 'C':
        case 'c':
            s +=  "1100";
            break;
        case 'D':
        case 'd':
            s +=  "1101";
            break;
        case 'E':
        case 'e':
            s += "1110";
            break;
        case 'F':
        case 'f':
            s +=  "1111";
            break;
        default:
            cout << "\nInvalid hexadecimal digit "
                 << hexdec[i];
        }
        i++;
    }
    for(int i=0;i<32;i++) s = "0"+s;
    reverse(s.begin(),s.end());
    return s;
}

unsigned long str_to_unsigned_long(const string &s)
{
    unsigned long sum = 0;
    for(int i=0;i<s.size();i++)
    {
        sum += (s[i]-'0');
        if(i!=s.size()-1) sum<<=1;
    }
    return sum;
}

unsigned long set_empty(unsigned long index , const vector<string> &Cache)
{
    for(unsigned long i=index;i<Cache.size();i+=interval)
    {
        if(Cache[i].empty()) return i;
    }
    return -1;
}

int in_set(unsigned long index,const string Cache_tag , const vector<string> &Cache)
{
    for(unsigned long i=index;i<Cache.size();i+=interval)
    {
        if(Cache[i]==Cache_tag)
        {
            return i;
        }
    }
    return -1;
}

void change_order(unsigned long index,int where_you_fill,vector<int> &order) //fill and change the order
{

    for(unsigned long i=index;i<order.size();i+=interval)
    {
        if(order[i]!=-1 ) // pick those has been filled ,and the order is smaller than where you fill
        {                // ex :  order[5] = { 1 3 4 2 0 }, 0 means empty -> then fill order[4] = 1 and the other non-empty +1 : {2 4 5 3 1}
            order[i]++;  // ex :  order[5] = { 1 3 4 2 5 }, LRU -> then fill order[4] = 1 : {2 4 5 3 1}
        }                // hit -> exception handle by hit_change_order
    }
    order[where_you_fill] = 1;
}

void hit_change_order(unsigned long index,int where_you_fill,vector<int> &order)
{
    for(unsigned long i=index;i<order.size();i+=interval)
    {
        if(order[i]!=-1 && order[i]<order[where_you_fill] ) // update the order since "hit" trigger the Recently Used
        {
            order[i]++;                                     //ex :  order[5] = {1 3 4 2 5}, change order[2]= 1 -> {2 4 1 3 5}
        }
    }
    order[where_you_fill] = 1;
}

void LRU_replace(const unsigned long index,string &Cache_tag,vector<string> &Cache, vector<int> &order)
{
    //if there are no empty place , do LRU replacement (notice that the maximum value of order is the least recently use)
    unsigned long biggest_order = index;
    for(unsigned long i=index;i<order.size();i+=interval)
    {
        if(order[biggest_order]<order[i])
        {
            biggest_order = i;
        }
    }
    Cache[biggest_order] = Cache_tag;
    change_order(index,biggest_order,order);
}



void N_way(int Associative,unsigned long Byte_select_length,int Cache_tag_length,int index_length,const vector<string>&data,vector<string>&Cache)
{
    unsigned long Hit = 0,DS = data.size();
    vector<int> order(Cache.size(),-1); // to record the replacement priority (size = number of blocks)
                                     // but Cache_size means the total bits in Cache

    bool has_print = 1;//changed

    for(unsigned long i=0;i<DS;i++)
    {

        string Cache_index = data[i].substr(Byte_select_length,index_length);//changed
        string Cache_tag = data[i].substr(Byte_select_length+index_length,Cache_tag_length);//changed

        if(!has_print)
        {
                cout<<"Block_size = "<<Block_size<<endl;
                cout<<"Byte_select_length = "<<Byte_select_length<<endl;
                cout<<"data[i] = \t"<<data[i]<<endl;
                cout<<"cache_tag  = \t"<<Cache_tag<<endl;
                cout<<"cache_index  = \t\t          "<<Cache_index<<endl;
                cout<<"data[i].size() = "<<data[i].size()<<endl;
                cout<<"cache_tag size = "<<Cache_tag.size()<<endl;
                cout<<"cache_index size = "<<Cache_index.size()<<endl;
                has_print = true;
        }//changed
        unsigned long index = str_to_unsigned_long(Cache_index)%(interval);//turn it into unsigned long int

        int enough_space = set_empty(index,Cache);//Find if there are some empty spaces or not(if exist , return index of space; if not, return -1)
        int hit_in_set = in_set(index,Cache_tag,Cache);

        if(hit_in_set!=-1)//if value string exists,then Hit++
        {
            Hit++;
            hit_change_order(index,hit_in_set,order);
        }
        else if(enough_space != -1)// if set has enough space ,then fill in Cache_tag
        {
            Cache[enough_space] = Cache_tag;
            change_order(index,enough_space,order);
        }
        else//LRU replacement
        {
            LRU_replace(index,Cache_tag,Cache,order);
        }
    }

    //***********print data*****************//
    double Hit_rate = ((double)Hit/(double)DS);
    cout<<Associative<<"-N Way\n";
    cout<<"Cache_size: "<<Cache_size/K<<'K'<<endl;
    cout<<"Block_size: "<<Block_size<<'B'<<endl;
    cout<<"Hit rate: "<<Hit_rate*100<<"% ("<<Hit<<"), Miss rate: "<<(1-Hit_rate)*100<<"% ("<<DS-Hit<<")\n";
    //***********print data*****************//

    //***********write txt*****************//
    if(write_data)
    {
        fstream output;
        output.open("answer.txt",ios::app);
        output<<(1-Hit_rate)*100<<' ';
        output.close();
    }
    //***********write txt*****************//
}

int main()
{

    Block_size = 64;//Byte select = 6;
    vector<string> data;

    //***********read data*****************//
    fstream file;
    file.open("LRU.txt",ios::in);
    if(!file) cout<<"No such File\n";
    else
    {
        while(!file.eof())
        {
            string s;
            getline(file,s);
            data.push_back(hex_to_binary(s));
        }
        file.close();
    }
    //***********read data*****************//


    //***********clean output txt*****************//
    if(write_data)
    {
        fstream output;
        output.open("answer.txt",ios::out);
        output.close();
    }
    //***********clean output txt*****************//


    //***********implementation*****************//
    unsigned long max_size = 64*K;//128KB
    for(Cache_size=1*K;Cache_size<=max_size;Cache_size*=2)
    {
        for(Associative=1;Associative<=8;Associative*=2)
        {

            //***********initialization*****************//
            Block_numbers = Cache_size/Block_size;//total blocks in Cache
            vector<string> Cache(Block_numbers,"");

            interval = (Block_numbers/Associative);

            unsigned long Byte_select_length = to_log2(Block_size);
            unsigned long index_length = to_log2(interval);
            unsigned long Cache_tag_length = 32-index_length-Byte_select_length;
            //***********initialization*****************//

            N_way(Associative,Byte_select_length,Cache_tag_length,index_length,data,Cache);
            cout<<endl;
        }
        if(write_data)
        {
            fstream output;
            output.open("answer.txt",ios::app);
            output<<'\n';
            output.close();
        }
        cout<<"--------------------------------------------"<<endl;
    }

    //***********implementation*****************//

return 0;
}
