#include <string>
#include <vector>
#include <utility>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;

class Cards{
	private:
		map <string, int> marketList;
		vector<int> Weights;
		vector<int> Profits;
		vector<int> include;
		vector<int> bestset;
		int maxprofit;
		int numbest;
		long int consideredSubsets;
		long int unconsideredSubsets;

	public:
		Cards();
		void generateList(string marketFile);
		pair<vector<string>,int> computeMax(vector<pair<string, int>> priceList, int bank); 	
		vector<pair<string, int>>  profitSort(vector<pair<string,int>> priceList);
		pair<vector<string>, int> greedy1(vector<pair<string, int>> priceList, int bank);
		pair<vector<string>, int> greedy2(vector<pair<string, int>> priceList, int bank);
		pair<vector<string>, int> backtrack(vector<pair<string, int>> priceList, int bank);
		int upperBound(int i, int weight, int profit, int bank, int num_cards);
		bool promising(int i, int weight, int profit, int bank, int num_cards);
		void knapsack(int i, int weight, int profit, int bank, int num_cards);
		pair<vector<string>, int> backtrackMethod(vector<pair<string, int>> priceList, int bank);
		pair<long int,long int> getSubsets();

};
