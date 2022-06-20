// A simple matching engine for trades
// Execute the program, then enter the number of trades followed by the trades themselves, eg:
// N 1000 APPL B 1500
// N represetns new trade, 1000 is the investment firm, APPL is the stock, B is to buy, and 1500 is the price
// The engine will then output the resulting blotter by investment firm, number of open orders, number of trades, and total paid/earned on trades

#include <iostream>
#include <vector>
#include <map>

using namespace std;

class MatchingEngine
{
private:
    vector<uint16_t> arrFirm;
    vector<string> arrSymbol;
    vector<char> arrSide;
    vector<float> arrPrice;
    map<uint16_t, vector<float>> order_book;

    void removeOrder(int i) {
        arrSymbol.erase(arrSymbol.begin()+i);              
        arrSide.erase(arrSide.begin()+i);              
        arrFirm.erase(arrFirm.begin()+i);              
        arrPrice.erase(arrPrice.begin()+i);              
        
    }

public:

    
    // use of these functions are not required
void onNewOrder(uint16_t firmId, std::string symbol, char side, float price, bool modify) {
        if (order_book.find(firmId) == order_book.end()) {
            order_book[firmId] = {0,0,0};
        } 
        float highest = -1;
        float lowest = -1;
        int chosenIndex = -1;
        bool ignore = false;
        for (int i = 0; i < arrSymbol.size(); i++) {
            if (arrSymbol[i] == symbol && arrFirm[i] == firmId && !modify) {
                ignore = true;
                i = arrSymbol.size();
            }
            if (arrSymbol[i] == symbol && arrSide[i] != side && arrFirm[i] != firmId) {
                if (side == 'S' && price <= arrPrice[i] && arrPrice[i] > highest) {
                    highest = arrPrice[i];
                    chosenIndex = i;
                } else if (side == 'B' && price >= arrPrice[i]) {
                    if (lowest == -1 || arrPrice[i] < lowest) {
                        lowest = arrPrice[i];
                        chosenIndex = i;                        
                    }
                }
            }
        }
        if (!ignore) {
            if (chosenIndex > -1) {
                order_book[firmId][1]++;
                if (modify) {
                    order_book[firmId][0]--;
                }            
                order_book[arrFirm[chosenIndex]][1]++;
                order_book[arrFirm[chosenIndex]][0]--;            
                if (highest > -1) {
                    order_book[firmId][2]+=price;                
                    order_book[arrFirm[chosenIndex]][2]-=price;                  
                } else {
                    order_book[firmId][2]-=arrPrice[chosenIndex];                                
                    order_book[arrFirm[chosenIndex]][2]+=arrPrice[chosenIndex];                
                }
                removeOrder(chosenIndex);
            } else if (!modify){
                order_book[firmId][0]++;
                arrSymbol.push_back(symbol);              
                arrSide.push_back(side);              
                arrFirm.push_back(firmId);              
                arrPrice.push_back(price);                                      
            }
        }
    }    
            
    
void onModify(uint16_t firmId, string symbol, float price) {
        for (int i = 0; i < arrSymbol.size(); i++) {
            if (arrSymbol[i] == symbol && arrFirm[i] == firmId) {
                arrPrice[i] = price;
                onNewOrder(firmId, symbol, arrSide[i], price, true);
                i = arrSymbol.size();
            }            
        }            
    }
void onCancel(uint16_t firmId, string symbol) {
        for (int i = 0; i < arrSymbol.size(); i++) {
            if (arrSymbol[i] == symbol && arrFirm[i] == firmId) {
                removeOrder(i);
                order_book[firmId][0]--;
                i = arrSymbol.size();
            }            
        }                
    }
void print() {
        for (auto x : order_book) {
            cout << x.first << ' ' << x.second[0] << ' ' << x.second[1] << ' ' << x.second[2] << endl;
        }
    }

};


int main()
{
MatchingEngine me;
uint16_t N = 0;

std::cin >> N;


char orderType;
uint16_t firmId;
std::string symbol;
char side;
float price;


for (size_t i = 0; i < N; i++)
{
 // Feel free to modify reading in from stdin
std::cin >> orderType >> firmId >> symbol;

switch (orderType)
{
case 'N':
std::cin >> side >> price;
me.onNewOrder(firmId, symbol, side, price, false);
break;
case 'M':
std::cin >> price;
me.onModify(firmId, symbol, price);
break;
case 'C':
me.onCancel(firmId, symbol);
break;
default:
break;
}
}

me.print();
}