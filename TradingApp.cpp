#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include<string>
#include<cmath> 

using namespace std;

//extract day from date string
int extractDay(const string& dateStr) {
    if (dateStr.length() < 10) return -1;  //check date string length
    int tensDigit = dateStr[8] - '0';
    int onesDigit = dateStr[9] - '0';
    return tensDigit * 10 + onesDigit;
}

//extract time from date string
string extractTime(const string& dateStr) {
    if (dateStr.length() < 16) return "";  //check date string length
    return dateStr.substr(11, 5);  // extract time part
}

int main() {
    ifstream dataFile("data.csv");
    if (!dataFile) {
        cerr << "Error opening data file." << endl;
        return 1;
    }

    vector<string> rowData;
    string line, cell;
    int dayCounter = 0, tradingDayCounter = 0, totalTrades = 0;
    int prevDay = 0, prevHigh = 0, prevLow = 0;
    int buyPrice = 0, targetPrice = 0, stopLoss = 0;
    int totalProfit = 0;
    bool isTradeActive = false;

    while (getline(dataFile, line)) {
        stringstream lineStream(line);
        rowData.clear();

        while (getline(lineStream, cell, ',')) {
            rowData.push_back(cell);
        }

        string date = rowData[0], openPrice = rowData[1], highPrice = rowData[2], lowPrice = rowData[3], closePrice = rowData[4];
        int day = extractDay(date);
        int openVal = stoi(openPrice), highVal = stoi(highPrice), lowVal = stoi(lowPrice), closeVal = stoi(closePrice);

        if (day != prevDay) {
            tradingDayCounter++;
            prevDay = day;
            prevHigh = max(prevHigh, highVal);
            prevLow = min(prevLow, lowVal);
        }

        if (tradingDayCounter > 1) {
            if (highVal > prevHigh && !isTradeActive) {
                isTradeActive = true;
                buyPrice = highVal;
                targetPrice = buyPrice + (prevHigh - prevLow);
                stopLoss = buyPrice - (prevHigh - prevLow) / 4;
                cout << "BUY NOW (Time = " << extractTime(date) << ")" << endl;
            }

            if ((highVal >= targetPrice || highVal <= stopLoss) && isTradeActive) {
                isTradeActive = false;
                totalTrades++;
                int tradeProfit = (highVal - buyPrice) * 10;
                totalProfit += tradeProfit;
                cout << "SELLING (Time = " << extractTime(date) << ")" << endl;
            }
        }
    }

    cout << "Total Trades: " << totalTrades << endl;
    cout << "Total Profit: " << totalProfit << endl;

    cout << "Enter any key to exit..." << endl;
    char exitChar = cin.get();
    return 0;
}
