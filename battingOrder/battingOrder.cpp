#include "battingOrder.h"
#include <random>
#include <iostream>
#include <string>
#include <vector>

namespace{
    enum BattingResult{
        OutIFG = 0,
        OutIFF,
        OutOFF,
        FourBalls,
        Single,
        Double,
        Triple,
        HR,
    };

    std::string getResultInString(BattingResult inResult)
    {
        switch(inResult){
        case OutIFG:
            return "OutIFG";
            break;
        case OutIFF:
            return "OutIFF";
            break;
        case OutOFF:
            return "OutOFF";
            break;
        case FourBalls:
            return "FourBalls";
            break;
        case Single:
            return "Single";
            break;
        case Double:
            return "Double";
            break;
        case Triple:
            return "Triple";
            break;
        case HR:
            return "HR";
            break;
        }

        return "";
    }
}

namespace{
    class Condition
    {
    public:
        std::size_t mOuts;
        std::size_t mBases;

        Condition(){
            mOuts = 0;
            mBases = 0;
        }
    };
    
    BattingResult getResult(double inRandVal, const std::vector<double>& inStats){
        std::vector<double> lProbs(8);

        double lOutProb = 1 - inStats[1];
        lProbs[0] = lOutProb / 3.0;
        lProbs[1] = 2.0 * lOutProb / 3.0;
        lProbs[2] = lOutProb;
        lProbs[3] = lProbs[2] + inStats[1] - inStats[0];
        lProbs[4] = lProbs[3] + inStats[0]*4.0/5.0 - inStats[2]*1.0/5.0;
        lProbs[5] = lProbs[4] + inStats[0]*4.0/5.0 - inStats[2]*1.0/5.0;
        lProbs[6] = lProbs[5];
        lProbs[7] = lProbs[6] - inStats[0]*3.0/5.0 + inStats[2]*2.0/5.0;
        
        for(int i=0; i<8; ++i){
            if(inRandVal < lProbs[i]){
                BattingResult lResult = static_cast<BattingResult>(i);
                return lResult;
            }
        }
    }

    double updateCondition(BattingResult inResult, Condition& inCondition){
        if(inResult == BattingResult::OutIFG || inResult == BattingResult::OutIFF){
            inCondition.mOuts += 1;
            return 0;
        }
        else if(inResult == BattingResult::OutOFF){
            if(inCondition.mOuts <= 1 && inCondition.mBases >= 4){
                inCondition.mOuts += 1;
                return 1;
            }else{
                inCondition.mOuts += 1;
                return 0;
            }
        }
        else if(inResult == BattingResult::FourBalls || inResult == BattingResult::Single){
            if(inCondition.mBases % 2 == 0){
                inCondition.mBases += 1;
                return 0;
            }else if(inCondition.mBases == 1){
                inCondition.mBases = 3;
                return 0;
            }else if(inCondition.mBases == 3 || inCondition.mBases == 5){
                inCondition.mBases = 7;
                return 0;
            }else{
                return 1;
            }
        }
        else if(inResult == BattingResult::Double){
            switch(inCondition.mBases){
            case 0:
                inCondition.mBases = 2;
                return 0;
                break;
            case 1:
                inCondition.mBases = 6;
                return 0;
                break;
            case 2:
                return 1;
                break;
            case 3:
                inCondition.mBases = 6;
                return 1;
                break;
            case 4:
                inCondition.mBases = 2;
                return 1;
                break;
            case 5:
                inCondition.mBases = 6;
                return 1;
                break;
            case 6:
                inCondition.mBases = 2;
                return 2;
                break;
            case 7:
                inCondition.mBases = 6;
                return 2;
                break;
            }
        }
        else if(inResult == BattingResult::HR){
            if(inCondition.mBases == 0){
                return 1;
            }
            else if(inCondition.mBases == 1 || inCondition.mBases == 2 || inCondition.mBases == 4){
                inCondition.mBases = 0;
                return 2;
            }else if(inCondition.mBases == 3 || inCondition.mBases == 5 || inCondition.mBases == 6){
                inCondition.mBases = 0;
                return 3;
            }else{
                inCondition.mBases = 0;
                return 4;
            }
        }
    }
} 

double BattingOrder::calcRunWithBattingOrder(int inSeed, double* inStats, std::vector<std::vector<std::size_t>>& outResults)
{
    std::vector<std::vector<double>> lStats;
    for(size_t i=0; i<9; i++){
        std::vector<double> lTmp;
        for(size_t j=0; j<4; j++){
            lTmp.push_back(inStats[4*i+j]);
        }
        lStats.push_back(lTmp);
    }

    std::mt19937 mt(inSeed);
    std::uniform_real_distribution<double> lRand(0.0, 1.0);

    //std::vector<std::vector<std::size_t>> lOutputResults;
    outResults.clear();

    double lRun = 0;
    size_t lPos = 0;
    for(std::size_t iInn = 1; iInn <= 9; ++iInn){
        Condition lCond;
        while(lCond.mOuts < 3){
            BattingResult lResult = getResult(lRand(mt), lStats[lPos]);
            lRun += updateCondition(lResult, lCond);
            lPos = lPos == 8? 0: lPos+1;
            //std::cout << iInn << "," << lCond.mOuts << "," << lCond.mBases << "," << getResultInString(lResult) << "," << lRun << std::endl;
            std::vector<std::size_t> lTmp;
            lTmp.push_back(iInn);
            lTmp.push_back(lCond.mOuts);
            lTmp.push_back(lCond.mBases);
            lTmp.push_back(static_cast<std::size_t>(lResult));
            lTmp.push_back(static_cast<std::size_t>(lRun));
            outResults.push_back(lTmp);
        }
    }

    return lRun;
}
