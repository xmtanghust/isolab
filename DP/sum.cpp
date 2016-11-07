// sum.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int MAXN=23;
const int MAXLen = 50;
int result[MAXN+1]={0};
string str("1234567890123456789012345678901234567890");

//一般递归
unsigned long sum = 0xFFFFFFFF;
unsigned int getsum()
{
	int pos=0;
	unsigned int sum=0;
	string temp;
	for(int i=0;i<MAXN;i++)
	{
		temp = str.substr(pos, result[i]);
		if (temp.length() == 10 && temp.compare("4294967296") >= 0)//5678901234
			return 0xffffffff;
		sum+=stoul(temp);
		pos+=result[i];
	}
	return sum;
	
}
void printformula(unsigned int sum)
{
	int i;
	int pos = 0;
	for (i = 0; i<MAXN; i++)
	{
		cout << str.substr(pos, result[i]) << "+";
		pos += result[i];
	}
	cout << str.substr(pos, result[i]) << "=" << sum << endl;
}
void  step(int rem,int i)//rem 剩余多长数字串，i剩余多少个未插入的加号
{
	int temp;
	unsigned int tempsum;
	if(rem<i+1)return ;
	if(rem>(i+1)*10)return ;//假设整数最长10位十进制,可以假设数字长度不超过某更小的整数以提高效率
	if(i==0)
	{
		result[0]=rem;
		//if(rem==result[1] || rem==result[1]+1 || rem==result[1]-1)//如果数字串中没有0，可以加入这个优化条件
		{
			
			tempsum = getsum();
			if (tempsum < sum)
			{
				sum = tempsum;
				printformula(sum);
			}
		}
		return ;
	}
	for(int j=1;j<=10;j++)//此处10和上面的i*10对应
	{
		temp=rem-j;
		result[i]=j;
		if(i==MAXN-1)
			step(temp,i-1);
		else
		{
		//if(j==result[i+1] || j==result[i+1]+1 || j==result[i+1]-1)//如果数字串中没有0，可以加入这个优化条件
				step(temp,i-1);
		}
	}
}


//DP动态规划
unsigned int state[MAXLen][MAXN + 1] = { 0 };//保存状态
unsigned int getsum2(int rem,int i)//目标函数
{
	unsigned int sum = 0;
	string temp;
	while (i >= 0)
	{
		if (state[rem][i] == 0xffffffff)return 0xffffffff;
		temp = str.substr(rem - state[rem][i], state[rem][i]);
		sum += stoul(temp);
		rem -= state[rem][i];
		i--;
	}
	return sum;

}
void printformula2(int rem, int i)//根据状态打印不同阶段的决策
{
	unsigned int sum = 0;
	string temp;
	int rem1 = rem;
	int i1 = i;
	while (i1 >= 0)
	{
		result[i1] = state[rem1][i1];
		rem1 -= state[rem1][i1];
		i1--;
	}
	printformula(getsum2(rem, i));
}
void  step2(int rem, int i)//rem 剩余多长数字串，i剩余多少个未插入的加号
{
	unsigned int temp;
	unsigned int minsum=0xffffffff;
	string tempstr;
	if (rem < i + 1)
	{
		state[rem][i] = 0xffffffff;//此路不通
		return;
	}
	if (rem > (i + 1) * 9)//假设整数最长9位十进制
	{
		state[rem][i] = 0xffffffff;//此路不通
		return;
	}
	if (i == 0)
	{
		state[rem][i] = rem;
		return;
	}
	
    for (int j = 1; rem>j && j <10; j++)
	{
		if (state[rem - j][i - 1] == 0)step2(rem - j, i - 1);//未做，则做
		temp = getsum2(rem - j, i - 1);
		if (temp == 0xffffffff)continue;//此路不通
		temp += stoul(str.substr(rem - j, j));
		if (temp < minsum)
		{
			state[rem][i] = j;
			minsum = temp;
		}
	}
}

//分治法
vector <int> strpos;
int flag;

void poplast(int lastflag)
{
	vector <int>::reverse_iterator it1, it2;
	for (it1 = strpos.rbegin(); it1 != strpos.rend(); it1++)
	{
		if (*it1 == lastflag)break;
	}
	
	for (it2=it1+1; it2 != strpos.rend(); it2++)
	{
		if (*it2 == lastflag)break;
	}

	strpos.erase((++it2).base(), (++it1).base()+1);
}
void printformula3(unsigned int sum)
{
	vector <int>::iterator Iter, it;
	for (Iter = strpos.begin(); Iter != strpos.end();)
	{
		if (*Iter >(int)str.length())
		{
			Iter = strpos.erase(Iter);
		}
		else
		  Iter++;
	}
	for (Iter = strpos.begin(); Iter != strpos.end(); Iter++)
	{
		str.insert((*Iter) + 1, "+");
		for (it = Iter + 1; it != strpos.end(); it++)
			if (*it > *Iter)*it = *it + 1;
	}
	cout << str << "=" << sum << endl;
	
}
unsigned int bin_step(int from, int to,int maxn)//maxn ：how many "+" to put
{
	int wid = to - from + 1;
	unsigned int res=0xffffffff;
	unsigned int res1, res2;

	if (maxn >= wid)return 0xffffffff;
	if (maxn == 0)
	{
		if (wid < 10 || (wid == 10 && str.substr(from, wid).compare("4294967296") < 0))
		{
			return stoul(str.substr(from, wid));
		}
		else
			return 0xffffffff;
	}
	int n1 = maxn / 2;
	int n2 = maxn - n1 - 1;
	int lasti=0;
	int localflag = 0;
	int lastflag = -1;
	//middle "+" insert in diffrent pos
	for (int i = n1; i < wid-n2-1; i++)
	{
		flag++;
		localflag = flag;
		strpos.push_back(localflag);
		if ((res1 = bin_step(from, from + i, n1)) == 0xffffffff)
		{
			strpos.push_back(localflag);
			poplast(localflag);
			continue;
		}
		res2 = bin_step(from + i + 1, to, n2);
		if (res2 != 0xffffffff)
		{
			res2 += res1;
			if (res2 < res)
			{
				res = res2;
				strpos.push_back(from + i);
				strpos.push_back(localflag);
				if (lastflag!=-1)
					poplast(lastflag);
				lastflag = localflag;
				
				continue;
			}
			
		}
		strpos.push_back(localflag);
		poplast(localflag);
	}
	return res;
}
int main(int argc, char* argv[])
{
	//step(str.length(),MAXN);
	/**************************************/
	
	step2(str.length(), MAXN);
	printformula2(str.length(), MAXN);

	/***************************************/
	//strpos.clear();
	//flag = str.length() + 1;
	//printformula3(bin_step(0, str.length() - 1, 15));
	
	getchar();
	return 0;
}

