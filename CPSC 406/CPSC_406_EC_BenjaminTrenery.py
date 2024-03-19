# Import the combinations function from the itertools module
from itertools import combinations

# Global variables to store data and states
# "data" array holds input strings
data = []
# "myStates" array holds the states used in the NFA
myStates = []
# "myAlphabet" arrays holds the alphabet used in the NFA
myAlphabet = []
# "NFAStartState" strings holds the start state of the NFA
NFAStartState = ""
# "DFAStartState" array holds the start state for the DFA
DFAStartState = []
# "validAcceptStates" array holds the accept states in the NFA
validAcceptStates = []
# "NFATrasntionTable" arrays holds all the transitions for NFA (input, alphabet, output)
NFATransitionTable = []
# "DFATransitionTable" arrays holds all the transitions for DFA (input, alphabet, output)
DFATransitionTable = []
# "epsilonTransitionStates" array holds all transition outputs from a state
epsilonTransitionStates = []
# "isValidInput" holds whether the input is valid (checks if there is a blank line in the file)
isValidInput = True

# Function to read input from a file and put in data into the "data" array
def input():
    
    global isValidInput
    # Open the input file in read 
    myFile = open("input.nfa", "r")
    inputString = "-1"
    i = -1
    
    while(True):
        
        i += 1
        # Read a line from the file and remove leading/trailing whitespaces
        inputString = myFile.readline().strip()
        if(len(inputString) == 0):
            isValidInput = False
            break
        
        # Check if the line equals "END" indicating the end of the file
        elif(inputString != "END"):
            data.append(inputString)
        else:
            data.append(inputString)
            break
    
    #close file
    myFile.close()

# Function to extract information from input
def exctactInformationFromInputData():
    
    global NFAStartState
    
    # gets the states
    for character in data[0]:
        if(character.isdigit() == True):
            myStates.append(character)
    
    # gets the alphabet
    for character in data[1]:
        if(character.isalpha() == True):
            myAlphabet.append(character)
    
    # get NFA Start State       
    for character in data[2]:
        if(character.isdigit() == True):
            NFAStartState = character
            break
    
    # get accept states
    for character in data[3]:
        if(character.isdigit() == True):
            validAcceptStates.append(character)
    
    i = 5
    
    #get transitions
    while(True):
        
        dataString = data[i]
        
        if(dataString != "END"):
            
            initialState = dataString[dataString.find("{") + 1]
            alphabet = dataString[(dataString.find(",") + 1): (dataString.find("=") - 1)].strip()
            dataStringSubString = dataString[(dataString.find("=") + 1): -1]
            nextState = dataStringSubString[(dataStringSubString.find("{") + 1)]
            
            fullTransition = [initialState, alphabet, nextState]
            NFATransitionTable.append(fullTransition)
            
        else:
            break
    
        i += 1

# Function to find the start state of the DFA and its epsilon closure          
def findDFAStartState():
    
    DFAStartState.append(NFAStartState)
    epsilonTransitionStates.clear()
    findAllEpsilonTransitions(DFAStartState[0])
    
    for state in epsilonTransitionStates:
        DFAStartState.append(state)
    
    epsilonTransitionStates.clear()
    
    DFAStartState.sort()

# Function to get immediate epsilon transitions from a given state
def getEpsilonTransitionsFromState(state):
    
    newEpsilonTransitionStates = []
    
    # Iterate through NFATransitionTable to find epsilon transitions
    for transition in NFATransitionTable:
        
        if(transition[0] == state and transition[1] == "EPS"):
            
            newEpsilonTransitionStates.append(transition[2])
    
    # Iterate through the new epsilon transitions and add them to epsilonTransitionStates if it is a new transition
    for newState in newEpsilonTransitionStates:
        
        isNewState = True
        
        for foundState in epsilonTransitionStates:
            
            if(newState == foundState):
                isNewState = False
                break
        
        if(isNewState == True):
            epsilonTransitionStates.append(newState)
    
            
# Function to find all epsilon transitions from a given state
def findAllEpsilonTransitions(state):
    
    i = 0
    
    getEpsilonTransitionsFromState(state)
    
    while(i < len(epsilonTransitionStates)):
        
        state = epsilonTransitionStates[i]
        getEpsilonTransitionsFromState(state)
        i += 1

# Function to get DFA transitions from a list of NFA states
def getDFATransitionsFromStateList(inputStateList):
    
    for letter in myAlphabet:
        
        DFAStateList = []
        
        for state in inputStateList:
            
            for transition in NFATransitionTable:
                
                if(transition[0] == state and transition[1] == letter):
                    
                    isNewState = True

                    
                    for DFAState in DFAStateList:
                        
                        if(transition[2] == DFAState):
                            isNewState = False
                            
                    if(isNewState == True): 
                        DFAStateList.append(transition[2])
                    
        if not DFAStateList:
            DFAStateList.append("EM")
        else:
            
            epsilonTransitionStates.clear()
            DFAStateList.sort()
            
            for DFAState in DFAStateList:
                findAllEpsilonTransitions(DFAState)
                
            for epsilonState in epsilonTransitionStates:
                
                if epsilonState not in DFAStateList:
                    DFAStateList.append(epsilonState)

            epsilonTransitionStates.clear()
            DFAStateList.sort()
            
        DFATransition = [inputStateList, letter, DFAStateList]
        DFATransitionTable.append(DFATransition)
        
# Function to find all DFA transitions starting at the DFA Start State
def findAllDFATransitions():
    
    i = 0
    
    getDFATransitionsFromStateList(DFAStartState)
    
    while(i < len(DFATransitionTable)):
        
        stateList = DFATransitionTable[i][2]
        
        isNewStateList = True
        
        for transition in DFATransitionTable:
            
            if(stateList == transition[0]):
                isNewStateList = False
                
        if(isNewStateList == True):
            getDFATransitionsFromStateList(stateList)
            
        i += 1      

# Function to format the output data into the corrrect format
def formatOutputData():
    
    DFAStates = getAllDFAStateCombinations()
    DFAReachableStates = getReachableStates()
    DFAAccpetStates = []
    outputData = []
    Line = ""
    
    
    for stateList in DFAReachableStates:
        
        for state in stateList:
            
            isAcceptState = False
            
            for acceptState in validAcceptStates:
                
                if(state == acceptState):
                    
                    DFAAccpetStates.append(stateList)
                    isAcceptState = True
                    break
            if(isAcceptState == True):
                break
     
    
    outputData.append(formatOutputDataStringDoubleArray(DFAStates))
    outputData.append(getAlphabetString())
    outputData.append(getDFAStartStateString())
    outputData.append(formatOutputDataStringDoubleArray(DFAAccpetStates))
    outputData.append("BEGIN")
    DFATransitionStrings = getDFATransitionStrings()
    outputData.extend(DFATransitionStrings)
    outputData.append("END")
    
    return outputData

# Function to format a list of states into a single string with curly braces and commas
def formatOutputDataStringDoubleArray(inputStatesArray):
    
    Line = ""
    
    for stateList in inputStatesArray:

        Line += "{" 
            
        for state in stateList:
            
            Line += state
            if(state != stateList[len(stateList) - 1] and state != "E" and state != "M"):
                Line += ", "

        Line += "}" + "\t"

    return Line

# Function to get a string of the alphabet separated by tabs
def getAlphabetString():
    
    Line = ""

    for letter in myAlphabet:
        Line += letter + "\t"
    
    return Line

# Function to get a string of the start state in the correct format with braces and commas
def getDFAStartStateString():
    
    Line = "{"
    
    for state in DFAStartState:
        
        if(state == DFAStartState[-1]):
            Line += state + "}" + "\t"
        else:
            Line += state + ", "
    return Line

# Function to get a string representing a transition's input and output states
def getTransitionString(stateList):
    
    Line = "{"
    
    for state in stateList:
        
        Line += state
        if(state != stateList[len(stateList) - 1] and state != "E" and state != "M"):
            Line += ", "

    Line += "}" + "\t"

    return Line
    
# Function to get a list of strings representing DFA transitions in the desired format
def getDFATransitionStrings():
    
    DFATransitionStrings = []
    
    for transition in DFATransitionTable:
         
        inputStates = getTransitionString(transition[0])
        ouptutStatesList = transition[2]
        element = transition[1]
        outputStates = getTransitionString(transition[2])
        
        DFATransitionString = inputStates.strip() + ", " + element + " = " + outputStates.strip()
        DFATransitionStrings.append(DFATransitionString)
    
    return DFATransitionStrings
    
# Function to write the formatted output data to the output file
def writeToOutputFile():
    
    myOutputFile = open("output.DFA", "w")
    outputData = formatOutputData()
        
    for line in outputData:
        
        myOutputFile.write(line)
        myOutputFile.write("\n")
    
    myOutputFile.close()

# Function to get all combinations of DFA states including the empty set            
def getAllDFAStateCombinations():
    
    possibilitiesList = []
    
    for i in range(0, len(myStates) + 1):
        
        possibilitiesList.extend(combinations(myStates, i))
    
    possibilitiesList[0] = "EM"
    
    return possibilitiesList

# Function to get reachable states from the DFATransitionTable
def getReachableStates():
    
    DFAReachableStates = []
    
    for transition in DFATransitionTable:
 
        if transition[2] not in DFAReachableStates:
                
            DFAReachableStates.append(transition[2])
    
    return DFAReachableStates
        
# Main function to execute the entire NFA to DFA conversion process
def main():
    global isValidInput
    
    # Read input from the file and check if it is valid
    input()
    if(isValidInput == True):
        # Extract Input Data Information
        exctactInformationFromInputData()
        # Find the start state of the DFA and its epsilon closure
        findDFAStartState()
        # Find all DFA transitions
        findAllDFATransitions()
        # Get all combinations of DFA states
        getAllDFAStateCombinations()
        # Write the formatted output data to the output file
        writeToOutputFile()
    else:
        print("This is not a valid input file, please check the format of your file")

main()