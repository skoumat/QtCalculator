#include <cmath>
#include <QDebug>

#include "CalcLogic.h"


CalcLogic::CalcLogic(QObject *parent)
    : QObject(parent)
{}

QString CalcLogic::getCurrentNumberString() const {
    return currentNumberString_private;
}

QString CalcLogic::getCalculationString() const {
    return calculationString_private;
}

void CalcLogic::insertDigit(int digit)
{
    if (planDeleteCalculation){
        calculationString_private = "";
        emit calculationStringChanged();
        currentNumber = 0;
        planDeleteCalculation = false;
    }

    if (!storedOperands.empty() && storedOperands.back() == ")"){
        return;
    }

    if (decimalCounter > -1){
        decimalCounter++;
        if (digit == 0){
            currentNumberString_private.append("0");
            emit currentNumberStringChanged();
        }
        else{
            currentNumber += digit / std::pow(10.0, decimalCounter);
            currentNumberChanged();
        }


    }
    else {
        currentNumber = currentNumber * 10 + digit;
        currentNumberChanged();
    }

    lastWas.push_back(DIGIT);
    justDeleted = false;
    isCurrentNumberPushed = false;
}

void CalcLogic::insertDecimal(){
    if (planDeleteCalculation){
        calculationString_private = "";
        emit calculationStringChanged();
        planDeleteCalculation = false;
    }

    if (decimalCounter == -1){
        decimalCounter = 0;
        currentNumberString_private.append(".");
        emit currentNumberStringChanged();

        lastWas.push_back(DP);
        justDeleted = false;
    }
}

void CalcLogic::currentNumberChanged(){
    if (decimalCounter >= 0){
        currentNumberString_private = QString::number(currentNumber, 'f', decimalCounter);
    }
    else{
        currentNumberString_private = QString::number(currentNumber, 'f', 0);
    }

    emit currentNumberStringChanged();
}

void CalcLogic::insertParth(const QString &par){

    if (planDeleteCalculation){
        calculationString_private = "";
        emit calculationStringChanged();
        planDeleteCalculation = false;
    }

    if (par == ")" && neededClosedPar == 0){
        return;
    }

    if(par == "("){
        if (!lastWas.empty() && storedOperands[storedOperands.size() - 1] == ")"){ // zabranuje "...) ( "
            return;
        }
        if (!lastWas.empty() && (lastWas.back() == DP || lastWas.back() == DIGIT)){ // umoznuje "(" na zacatku
            return;
        }

        neededClosedPar++;
    }

    if (par == ")"){
        if (!isCurrentNumberPushed && storedOperands[storedOperands.size() - 1] != ")"){ // zabranuje ..4) 4 +
            if (decimalCounter >= 0){
                calculationString_private.append(QString::number(currentNumber, 'f', decimalCounter));
            }
            else{
                calculationString_private.append(QString::number(currentNumber, 'f', 0));
            }

            pushAndResetCurrentNumber();

            if(lastWas.back() != DIGIT){
                lastWas.push_back(DIGIT);
            }

            isCurrentNumberPushed = true;
        }

        neededClosedPar--;
    }

    calculationString_private.append(whiteChar + par + whiteChar);
    emit calculationStringChanged();

    storedOperands.push_back(par);
    lastWas.push_back(PARTH);
}

void CalcLogic::insertOperation(const QString &op)
{
    if (planDeleteCalculation){
        calculationString_private = "";
        emit calculationStringChanged();
        planDeleteCalculation = false;
    }

    if (isDivisionByZeroFromCurrentNum()) {
        return;
    }

    if (!isCurrentNumberPushed){
        if (decimalCounter >= 0){
            calculationString_private.append(QString::number(currentNumber, 'f', decimalCounter) + whiteChar + op + whiteChar);
        }
        else{
            calculationString_private.append(QString::number(currentNumber, 'f', 0) + whiteChar + op + whiteChar);
        }

        pushAndResetCurrentNumber();
        isCurrentNumberPushed = false;

        if(lastWas.back() != DIGIT){
            lastWas.push_back(DIGIT);
        }
    }
    else {
        calculationString_private.append(whiteChar + op + whiteChar);
        isCurrentNumberPushed = false;
    }

    storedOperands.push_back(op);

    emit calculationStringChanged();

    lastWas.push_back(OPERAND);
    justDeleted = false;
}

void CalcLogic::pushAndResetCurrentNumber(){
    storedValues.push_back(currentNumber);
    currentNumber = 0;
    currentNumberChanged();

    decimalHistory.push_back(decimalCounter);
    decimalCounter = -1;
}

bool CalcLogic::isDivisionByZeroFromCurrentNum(){
    if (!storedOperands.empty() && !storedValues.empty() && storedOperands.back() == DIV && isDivisionByZero(currentNumber)) {
        lastWas.pop_back();

        return true;
    }

    return false;
}

bool CalcLogic::isDivisionByZero(double divisor){
    if (divisor == 0) {
        currentNumberString_private = "Error: division by zero";
        emit currentNumberStringChanged();
        return true;
    }

    return false;
}

void CalcLogic::backspace(){ // 0.000001 B -> 0
    if(lastWas.empty()){
        return;
    }

    if (lastWas.back() == Items::PARTH){
        if (storedOperands.back() == ")"){
            neededClosedPar++;
        }
        else{
            neededClosedPar--;
        }

        storedOperands.pop_back();
        calculationString_private.chop(3);
        emit calculationStringChanged();

        if(lastWas[lastWas.size()-2] == DIGIT){
            restoreCurrentNumber();
            }
    }

    if(lastWas.back() == Items::DP){
        decimalCounter = -1;

        currentNumberString_private.removeLast();
        emit currentNumberStringChanged();
    }

    if(lastWas.back() == Items::DIGIT){
        if (currentNumber == 0 && decimalCounter == -1){
            lastWas.pop_back();
            justDeleted = true;
            backspace();
            return;
        }

        if (decimalCounter > 0){
            decimalCounter--;
            currentNumber = std::trunc(currentNumber * std::pow(10.0, decimalCounter)) / std::pow(10.0, decimalCounter);

            if (currentNumber == 0){
                currentNumberString_private.removeLast();
                emit currentNumberStringChanged();

                lastWas.pop_back();
                justDeleted = true;
                return;
            }

            currentNumberString_private = QString::number(currentNumber, 'f', decimalCounter);
        }
        else{
            currentNumber = std::floor(currentNumber/10);
            currentNumberString_private = QString::number(currentNumber, 'f', 0);
        }

        emit currentNumberStringChanged();
    }

    if(lastWas.back() == Items::OPERAND){
        storedOperands.pop_back();
        calculationString_private.chop(3);
        emit calculationStringChanged();

        if (lastWas[lastWas.size() - 2] == DIGIT){
            restoreCurrentNumber();
        }
        else{
            currentNumber = 0;
            decimalCounter = -1;
        }
    }

    lastWas.pop_back();
    justDeleted = true;
}

int CalcLogic::numDigits(QString numberString)
{
    for (int i = 0; i < numberString.size() - 1; i++){
        if (numberString[i] == '.'){
            return numberString.size() - 1;
        }
    }

    return numberString.size();
}

void CalcLogic::restoreCurrentNumber(){
    currentNumber = storedValues.back();
    currentNumberChanged();
    storedValues.pop_back();

    decimalCounter = decimalHistory.back();
    decimalHistory.pop_back();

    isCurrentNumberPushed = false;

    if(decimalCounter > 0){
        calculationString_private.chop(numDigits(QString::number(currentNumber, 'f', decimalCounter)));
    }
    else{
        calculationString_private.chop(numDigits(QString::number(currentNumber, 'f', 0)));
    }



    emit calculationStringChanged();
}

void CalcLogic::calculate()
{
    if (planDeleteCalculation || neededClosedPar > 0 || lastWas.back() == Items::DP) {
        return;
    }

    if (isDivisionByZeroFromCurrentNum()) {
        return;
    }

    planDeleteCalculation = true;

    if (lastWas.back() == Items::PARTH){
        calculationString_private.append(" =");
    }
    else{
        if(decimalCounter > 0){
            calculationString_private.append(QString::number(currentNumber, 'f', decimalCounter) + " =");
        }
        else{
            calculationString_private.append(QString::number(currentNumber, 'f', 0) + " =");
        }

        emit calculationStringChanged();
        pushAndResetCurrentNumber();
    }


    double deletedValues = calculateRec(0, storedValues.size() - 1, 0);
    if(deletedValues >= 0){
        currentNumber = storedValues[0];
        currentNumberChanged();
    }

    storedValues.clear();
    storedOperands.clear();
    lastWas.clear();
    decimalHistory.clear();
    justDeleted = false;
    isCurrentNumberPushed = false;
}


int CalcLogic::calculateRec(int startIndexVal, int endIndexVal, int startIndexOp) {
    int deletedValues = 0;
    int actualOps = 0;
    for (int i = startIndexOp; i < storedOperands.size(); i++){
        if (storedOperands[i] == "("){
            deletedValues = calculateRec(startIndexVal+actualOps, endIndexVal, i+1);
            if(deletedValues < 0){
                return deletedValues;
            }

            storedOperands.erase(storedOperands.begin() + i);
        }
        else if (storedOperands[i] == ")"){
            storedOperands.erase(storedOperands.begin() + i);
            break;
        }
        else{
            actualOps++;
        }
    }

    int newDeletedValues = calculateMultAndDiv(startIndexVal, endIndexVal - deletedValues, startIndexOp);
    if(newDeletedValues < 0){
        return newDeletedValues;
    }

    deletedValues += newDeletedValues;

    if(storedOperands.size() == 0){
        return 0;
    }

    deletedValues += calculateAddAndSub(startIndexVal, endIndexVal - deletedValues, startIndexOp);

    return deletedValues;
}

int CalcLogic::calculateMultAndDiv(int startIndexVal, int endIndexVal, int startIndexOp){
    int deletedValues = 0;

    while(startIndexVal < endIndexVal){
        if(storedOperands[startIndexOp] == MULT || storedOperands[startIndexOp] == DIV){
            double nextNumber = storedValues[startIndexVal+1];
            if(storedOperands[startIndexOp] == MULT){
                storedValues[startIndexVal] *= nextNumber;
            }
            else{
                if (isDivisionByZero(nextNumber)){
                    return -1;
                }
                storedValues[startIndexVal] /= nextNumber;
            }

            storedOperands.erase(storedOperands.begin() + startIndexOp);    // smazeme operand
            startIndexOp--;                                                 // a musime pro pristi pruchod upravit index

            storedValues.erase(storedValues.begin() + startIndexVal + 1);   // smazeme next value, ktera se prinasobila/delila
            deletedValues++;
            startIndexVal--;                                                // a pro pristi pruchod upravime indexy
            endIndexVal--;
        }
        startIndexVal++;
        startIndexOp++;
    }

    return deletedValues;
}

int CalcLogic::calculateAddAndSub(int startIndexVal, int endIndexVal, int startIndexOp){
    int deletedValues = 0;
    for (; startIndexVal < endIndexVal; endIndexVal--) {
        if (storedOperands[startIndexOp] == ADD){
            storedValues[startIndexVal] += storedValues[startIndexVal + 1];
        }
        else{
            storedValues[startIndexVal] -= storedValues[startIndexVal + 1];
        }

        storedOperands.erase(storedOperands.begin() + startIndexOp);
        storedValues.erase(storedValues.begin() + startIndexVal + 1);
        deletedValues++;
    }

    return deletedValues;
}

void CalcLogic::clear()
{
    currentNumber = 0;
    currentNumberChanged();

    decimalCounter = -1;
    decimalHistory.clear();

    storedValues.clear();
    storedOperands.clear();

    lastWas.clear();
    neededClosedPar = 0;
    justDeleted = false;
    isCurrentNumberPushed = false;

    calculationString_private = "";
    emit calculationStringChanged();
}
