#include "battingOrder.h"
#include <vector>
#include <iostream>
#include <fstream>

int main()
{
    std::vector<double> lStats
        = {0.301, 0.352, 0.403, 23,
           0.280, 0.332, 0.412, 20,
           0.323, 0.353, 0.521, 5,
           0.313, 0.383, 0.550, 3,
           0.282, 0.331, 0.500, 3,
           0.282, 0.301, 0.421, 4,
           0.262, 0.301, 0.366, 5,
           0.258, 0.333, 0.431, 6,
           0.101, 0.101, 0.210, 0
    };
    
    BattingOrder lBO;

    double lAverage = 0.0;

    std::vector<std::vector<std::size_t>> lOutputs;
    for(size_t i=0; i<143; ++i){
        std::vector<std::vector<std::size_t>> lResults;
        double lRun = lBO.calcRunWithBattingOrder(i, lStats.data(), lResults);
        lAverage += lRun / 143.0;
        
        for(size_t j=0; j<lResults.size(); ++j){
            lOutputs.push_back(lResults[j]);
        }
    }

    std::ofstream lOfs("result.csv");
    for(size_t i=0; i<lOutputs.size(); ++i){
        lOfs << lOutputs[i][0];
        for(size_t j=1; j<lOutputs[i].size(); ++j){
            lOfs << "," << lOutputs[i][j];
        }
        lOfs << std::endl;
    }
    lOfs.close();

    
    std::cout << lAverage << std::endl;

    return 0;
}
