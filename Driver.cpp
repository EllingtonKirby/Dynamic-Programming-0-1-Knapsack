#include "Cards.h"
#include <fstream>
#include <time.h>

using namespace std;
int main(int argc, char* argv[]){
	if(argc != 8){
		cout << "Usage: ./program1 -m <market file> -p <price list file> -o <output file> [3]" << endl;
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
		cout << priceLists.size() << endl;
		for(int i = 0; i < priceLists.size(); i++){
			t1 = clock();
			int solution = card.dynamicSolution(priceLists[i], banks[i]);
			cout << solution << endl;
			t2 = clock();
			of << "Dynamic Programming: " << priceLists[i].size() << " " << solution << " " << double (t2-t1)/CLOCKS_PER_SEC << endl;
		}
		of.close();
	}

}	


