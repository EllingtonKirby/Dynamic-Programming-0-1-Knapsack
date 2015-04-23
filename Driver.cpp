#include "Cards.h"
#include <fstream>
#include <time.h>

using namespace std;
int main(int argc, char* argv[]){
	if(argc != 8){
		cout << "Usage: ./program1 -m <market file> -p <price list file> -o <output file> 0|1|2" << endl;
	}
	else{
		string marketFile(argv[2]);
		string priceFile(argv[4]);
		string outFile(argv[6]);
		int command = atoi(argv[7]);
		vector <vector <pair <string, int>>> priceLists;
		vector<int> banks;
		clock_t t1, t2;
		ifstream piFile(priceFile);
		if(piFile.is_open()){
			string line;
			while(getline(piFile, line)){
				int next = piFile.peek();
				stringstream s1(line);
				string word;
				s1 >> word;
				if(isdigit(word[0])){
					stringstream ss(line);
					int temp = 0;
					int numCards = 0;
					int cardvalue = 0;
					string cardname, newline;
					ss >> numCards;
					ss >> temp;
					banks.push_back(temp);
					vector< pair< string, int>> priceList;
					for(int i = 0; i < numCards; i++){
						getline(piFile, newline);
						stringstream s2(newline);
						s2 >> cardname;
						s2 >> cardvalue;
						priceList.push_back(pair<string, int>(cardname, cardvalue));
					}
					priceLists.push_back(priceList);
				}
			}
		}
		piFile.close();
		Cards card;
		card.generateList(marketFile);
		ofstream of(outFile);
		for(int i = 0; i < priceLists.size(); i++){
			pair<vector<string>, int> solution;
			if(command == 0){
				t1 = clock();
				solution = card.greedy1(priceLists[i], banks[i]);
				t2 = clock();
				of << "Greedy 1: " << priceLists[i].size() << " " << solution.second << " " << solution.first.size() << " " << double (t2-t1)/CLOCKS_PER_SEC << endl;
				for(int i = 0; i < solution.first.size(); i++){
					of << solution.first[i] << endl;
				}
			}
			else if(command == 1){
				t1 = clock();
				solution = card.greedy2(priceLists[i], banks[i]);
				t2 = clock();
				of << "Greedy 2: " << priceLists[i].size() << " " << solution.second << " " << solution.first.size() << " " << double (t2-t1)/CLOCKS_PER_SEC << endl;
				for(int i = 0; i < solution.first.size(); i++){
					of << solution.first[i] << endl;
				}
			}
			else{
				t1 = clock();
				solution = card.backtrackMethod(priceLists[i], banks[i]);
				t2 = clock();
				pair<int, int> subsets = card.getSubsets();
				of << "Backtracking: " << priceLists[i].size() << " " << solution.second << " " << solution.first.size() << " " << double (t2-t1)/CLOCKS_PER_SEC << endl;
				of << subsets.first << " subsets considered : " << subsets.second << " subsets avoided via backtracking" << endl;

				for(int i = 0; i < solution.first.size(); i++){
					of << solution.first[i] << endl;
				}
			}

		}	
	}
}
