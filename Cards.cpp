#include <fstream>
#include "Cards.h"
#include <math.h>

Cards::Cards(){
	map<string, int> marketList;
}
pair<long int, long int> Cards::getSubsets(){
	return pair<long int, long int>(consideredSubsets, unconsideredSubsets);
}
void Cards::generateList(string marketFile){
	ifstream miFile(marketFile);
	int mNumCards = 0;
	if(miFile.is_open()){
		string line;
		getline(miFile, line);
		stringstream s1(line);
		s1 >> mNumCards;
		while(getline(miFile, line)){
			stringstream s2(line);
			string cardName;
			int cardPrice;
			s2 >> cardName;
			s2 >> cardPrice;
			marketList.insert(pair<string, int>(cardName, cardPrice));
		}
	}
	miFile.close();
}

pair<vector<string> , int> Cards::computeMax(vector<pair< string, int>> priceList, int bank){
	int max = 0;
	vector <int> sets;
	vector <string> consideredCards;
	int num = priceList.size();
	int sum = 0;
	for(int i = 0; i < num; i++){
		sum += priceList[i].second;
	}
	if(sum <= bank){
		for(int i = 0; i < num; i++){
			string cardname = priceList[i].first;
			int value = marketList[cardname];
			int cost = priceList[i].second;
			int profit = value - cost;
			max += profit;
			consideredCards.push_back(cardname);
		}
		//can buy everything in I, return I and sum I
		pair<vector<string>,int> retVal(consideredCards, max);
		return retVal;
	}
	for(int  i = 0; i < pow(2, num); i++){
		int temp = i;
		sum = 0;
		while(temp > 1){
			sets.push_back(temp % 2);
			temp = temp / 2;
		}
		sets.push_back(temp);
		for(int j = 0; j < sets.size(); j++){
			if(sets[j] == 1){
				sum += priceList[j].second;
			}
		}
		if(sum <= bank){
			int tempMax = 0;
			for(int j = 0; j < sets.size(); j++){
				if(sets[j] == 1){
					string cardname = priceList[j].first;
					int value = marketList[cardname];
					int cost = priceList[j].second;
					int profit = value - cost;
					tempMax += profit;
				}
			}
			if(tempMax > max){
				max = tempMax;
				consideredCards.clear();
				for(int j = 0; j < sets.size(); j++){
					if(sets[j] == 1){
						consideredCards.push_back(priceList[j].first);
					}
				}
			}	
		}
		sets.clear();
	}
	pair<vector<string>,int> retVal(consideredCards, max);
	return retVal;
}
vector<pair<string,int>> Cards::profitSort(vector<pair<string, int>> priceList){
	struct{
	       	bool operator()(pair<string, int> a, pair<string, int> b){
				return b.second < a.second;
		}
	}pairSorting;
	
	int num_cards = priceList.size();
	vector<pair<string, int>> weightedCards;
	map<string, int> associatedCost;
	for(int i = 0; i < num_cards; i++){
		string cardname = priceList[i].first;
		int cost = priceList[i].second;
		int value = marketList[cardname];
		pair<string, int> weightedCard;
		weightedCard.first = cardname;
		int profit = value - cost;
		int ratio = profit / cost;
		weightedCard.second = ratio;
		weightedCards.push_back(weightedCard);
		associatedCost.insert(pair<string, int>(cardname, cost));
	}
	
	sort(weightedCards.begin(), weightedCards.end(), pairSorting);
	
	for(int i = 0; i < num_cards; i++){
		string cardname = weightedCards[i].first;
		int cost = associatedCost[cardname];
		Weights.push_back(cost);
		int ratio = weightedCards[i].second;
		int profit = ratio * cost;
		Profits.push_back(profit);
	}	
	maxprofit = 0;
	numbest = 0;
	for(int i = 0; i < num_cards; i++){
		include.push_back(0);
		bestset.push_back(0);
	}	
	return weightedCards;
}	
pair<vector<string> , int> Cards::greedy1(vector<pair< string, int>> priceList, int bank){
	int sum = 0;
	vector<string> consideredCards;
	int num_cards = priceList.size();
	vector<pair<string, int>> weightedCards;
	weightedCards = profitSort(priceList);
	int tempBank = bank;
	int tempCost = Weights[0];
	int count = 0;
	while(count < num_cards  && tempBank - tempCost > 0){
		int cost = Weights[count];		
		consideredCards.push_back(weightedCards[count].first);
		sum += Profits[count];
		tempBank-= cost;
		count++;		
		tempCost = Weights[count];
	}
	//once output format is decided do it here, consideredCards contains the most profitable cards
	pair<vector<string>, int> retVal(consideredCards, sum);
	return retVal;
}

pair<vector<string> , int> Cards::greedy2(vector<pair< string, int>> priceList, int bank){
	pair<vector<string>, int> greedyMax = greedy1(priceList, bank);
	vector<string> consideredCards;
	int num_cards = priceList.size();
	int max_profit = 0;
	string best_card = "";
	for(int i = 0; i < num_cards; i++){
		string cardname = priceList[i].first;
		int cost = priceList[i].second;
		int value = marketList[cardname];
		int profit = value - cost;
		if(profit <= bank){
			if(profit > max_profit){
				max_profit = profit;
				best_card = cardname;
			}
		}	
	}
	consideredCards.push_back(best_card);
	if(max_profit > greedyMax.second){
		return pair<vector<string>, int> (consideredCards, max_profit);
	}
	else{
		return greedyMax;
	}
}

int Cards::upperBound(int i, int weight, int profit, int bank, int num_cards ){
	int bound = profit;
	int *x = new int[num_cards];
	for(int j = i; j < num_cards; j++){
		x[j] = 0;
	}
	while(weight < bank && i <= num_cards){
		if(weight + Weights[i] <= bank){
			x[i] = 1;
			weight = weight + Weights[i];
			bound = bound + Profits[i];
		}
		else{
			x[i] = (bank-weight)/Weights[i];
			weight = bank;
			bound = bound + (Profits[i] * x[i]);
		}
		i++;
	}
	return bound;
}

bool Cards::promising(int i, int weight, int profit, int bank, int num_cards){
	if(weight >= bank){
		return false;
	}
	int bound = upperBound(i+1, weight, profit, bank, num_cards);
	return (bound > maxprofit);
}

void Cards::knapsack(int i, int profit, int weight, int bank, int num_cards){
	if(i == num_cards - 1){
		consideredSubsets++;
	}
	if(weight <= bank && profit > maxprofit){
		maxprofit = profit;
		numbest = i;
		bestset = include;
	}
	if(promising(i, weight, profit, bank, num_cards)){
		include[i+1] = 1;
		knapsack(i+1, profit + Profits[i+1], weight + Weights[i+1], bank, num_cards);
		include[i+1] = 0;
		knapsack(i+1, profit, weight, bank, num_cards);
	}
	else{
		unconsideredSubsets += pow(2, num_cards - i);
	}
}

pair<vector<string>, int> Cards::backtrackMethod(vector<pair<string, int>> priceList, int bank){
	vector<pair<string, int>> weightedCards;
	weightedCards = profitSort(priceList);
	consideredSubsets = 0;
	unconsideredSubsets = 0;
	//numbest, maxprofit, include, bestset all intitialized to 0
	//Profits and Weights initialized 
	//weightedCards is the sorted set of cards
	pair<vector<string>, int> greedyInit = greedy2(priceList, bank);
	for(int i = 0; i < greedyInit.first.size(); i++){
		for(int j = 0; j < weightedCards.size(); j++){
			if(greedyInit.first[i] == weightedCards[j].first){
				bestset[j] = 1;
			}
		}
	}
	maxprofit = greedyInit.second;
	knapsack(0, 0, 0, bank, weightedCards.size());
	//bestset should now be initialized to the correct value
	vector<string> cardSolutions;
	for(int i = 0; i < weightedCards.size(); i++){
		if(bestset[i] == 1){
			cardSolutions.push_back(weightedCards[i].first);
		}
	}
	
	return pair<vector<string>, int>(cardSolutions, maxprofit);
}
/*
 *
 *
 *
 * DYNAMIC PROGRAMMING CODE BEGINS HERE
 *
 *
 *
 *
 *
 */
int Cards::dynamicSolution(vector<pair<string, int>> priceList, int bank){
	vector<int*> P;
	for(int i = 0; i < 2 ; i++){
		int * arrayC = new int[bank+1];
		arrayC[0] = 0;
		P.push_back(arrayC);
	}
	profitSort(priceList);
	for(int i = 0; i <= bank; i++){
		P[0][i] = 0;
	}
	for(int i = 0; i <= priceList.size(); i++){
		P[1][0] = 0;
		for(int c = 1; c <= bank; c++){
			if(Weights[i] <= c){
				if((P[0][c - Weights[i]] + Profits[i]) > P[0][c]){
					P[1][c] = P[0][c - Weights[i]] + Profits[i];
				}
				else{
					P[1][c] = P[0][c];
				}
			}
			else{
				P[1][c] = P[0][c];
			}
		}
		for(int j = 0; j <= bank; j++){
			P[0][j] = P[1][j];
		}
	}
	int maxProfit = P[1][bank];
	return maxProfit;
}
