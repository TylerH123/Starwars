//IDENTIFIER  = AD48FB4835AF347EB0CA8009E24C3B13F8519882

#include <algorithm>
#include <getopt.h>
#include <iostream>
#include <sstream>
#include <map>
#include <math.h>
#include <queue>
#include <string>
#include <vector>
#include "P2random.h"

using namespace std;

struct Gen {
    int siths;
    int jedis; 
    int dead;  
}; 

enum class State {
    Initial, 
    SeenOne, 
    SeenBoth, 
    MaybeBetter
};

class Planet {
private:
    struct deployment { 
        int gen;  
        int forceSens; 
        mutable int troops; 
        int arrivalTime; 
    };
    class sithComp {
    public:
        bool operator()(const deployment &a, const deployment &b) const{
            if (a.forceSens == b.forceSens) 
                return a.arrivalTime > b.arrivalTime;
            return a.forceSens < b.forceSens; 
        }
    };
    class jediComp {
    public: 
        bool operator()(const deployment &a, const deployment &b) const{
            if (a.forceSens == b.forceSens) 
                return a.arrivalTime > b.arrivalTime;
            return a.forceSens > b.forceSens;
        }
    };
    int arrival = 0,
        bestAtkJediTime = -1, 
        bestAtkJediForce = -1, 
        bestAtkSithTime = -1, 
        bestAtkSithForce = -1, 
        maybeAtkJediTime = -1, 
        maybeAtkJediForce = -1, 
        bestAmbJediTime = -1,   
        bestAmbJediForce = -1,
        bestAmbSithTime = -1, 
        bestAmbSithForce = -1, 
        maybeAmbSithTime = -1, 
        maybeAmbSithForce = -1,
        planetNum;
    State atkState = State::Initial; 
    State ambState = State::Initial; 
    bool battled = false; 
    priority_queue<deployment, vector<deployment>, sithComp> siths; 
    priority_queue<deployment, vector<deployment>, jediComp> jedis; 
    priority_queue<int, vector<int>, less<int>> leftHalf; 
    priority_queue<int, vector<int>, greater<int>> rightHalf; 
    
    void rebalance() {
        if (leftHalf.size() == 1 && rightHalf.size() == 1) {
            if (leftHalf.top() > rightHalf.top()) {
                int right = rightHalf.top(); 
                rightHalf.pop();
                rightHalf.push(leftHalf.top());
                leftHalf.pop();
                leftHalf.push(right); 
            }
        }
        else if (leftHalf.size() > rightHalf.size() + 1) {
            rightHalf.push(leftHalf.top()); 
            leftHalf.pop(); 
        }
        else if (rightHalf.size() > leftHalf.size() + 1) {
            leftHalf.push(rightHalf.top()); 
            rightHalf.pop(); 
        }
    }
    
    void pushBattle(int troops) {
        if (leftHalf.empty() && rightHalf.empty()) 
            leftHalf.push(troops);
        else if (leftHalf.empty() && !rightHalf.empty()) {
            if (rightHalf.top() < troops) 
                rightHalf.push(troops);
            else 
                leftHalf.push(troops);
        }
        else if (rightHalf.empty() && !leftHalf.empty()) {
            if (leftHalf.top() > troops) 
                leftHalf.push(troops);
            else 
                rightHalf.push(troops);
        }
        else if (rightHalf.top() > troops) 
            leftHalf.push(troops);
        else 
            rightHalf.push(troops); 
        rebalance(); 
    }
    
    void findBestAtk(int ts, char c, int fs) {
        if (atkState == State::Initial) { 
            if (c == 'J') {
                bestAtkJediTime = ts; 
                bestAtkJediForce = fs;
                atkState = State::SeenOne; 
            }
        }
        else if (atkState == State::SeenOne) {
            if (c == 'J') {
                if (fs < bestAtkJediForce){
                    bestAtkJediTime = ts;
                    bestAtkJediForce = fs;
                }
            }
            else {
                if (fs >= bestAtkJediForce) {
                    bestAtkSithTime = ts;
                    bestAtkSithForce = fs;
                    atkState = State::SeenBoth; 
                }
            }      
        }
        else if (atkState == State::SeenBoth) {
            if (c == 'S') {
                if (fs > bestAtkSithForce) {
                    bestAtkSithTime = ts;
                    bestAtkSithForce = fs;
                }       
            }
            else {
                if (fs < bestAtkJediForce) {
                    maybeAtkJediTime = ts; 
                    maybeAtkJediForce = fs; 
                    atkState = State::MaybeBetter; 
                }
            }   
        }
        else if (atkState == State::MaybeBetter) {
            if (c == 'J') {
               if (fs < maybeAtkJediForce) {
                    maybeAtkJediTime = ts; 
                    maybeAtkJediForce = fs; 
                } 
            }
            else {
                if (fs - maybeAtkJediForce > bestAtkSithForce - bestAtkJediForce) {
                    bestAtkSithTime = ts;
                    bestAtkSithForce = fs; 
                    bestAtkJediTime = maybeAtkJediTime; 
                    bestAtkJediForce = maybeAtkJediForce; 
                    atkState = State::SeenBoth; 
                }
            }
        }
    }
    
    void findBestAmb(int ts, char c, int fs) {
        if (ambState == State::Initial) {
            if (c == 'S') {
                bestAmbSithTime = ts; 
                bestAmbSithForce = fs;
                ambState = State::SeenOne; 
            }
        }
        else if (ambState == State::SeenOne) {
            if (c == 'S') {
                if (fs > bestAmbSithForce) {
                    bestAmbSithTime = ts;
                    bestAmbSithForce = fs;
                }
            }
            else {
                if (fs <= bestAmbSithForce) {
                    bestAmbJediTime = ts;
                    bestAmbJediForce = fs;
                    ambState = State::SeenBoth; 
                }
            }      
        }
        else if (ambState == State::SeenBoth) {
            if (c == 'J') {
                if (fs < bestAmbJediForce) {
                    bestAmbJediTime = ts;
                    bestAmbJediForce = fs;
                }       
            }
            else {
                if (fs > bestAmbSithForce) {
                    maybeAmbSithTime = ts; 
                    maybeAmbSithForce = fs; 
                    ambState = State::MaybeBetter; 
                }
            }   
        }
        else if (ambState == State::MaybeBetter) {
            if (c == 'S') {
               if (fs > maybeAmbSithForce) {
                    maybeAmbSithTime = ts; 
                    maybeAmbSithForce = fs; 
                } 
            }
            else {
                if (maybeAmbSithForce - fs > bestAmbSithForce - bestAmbJediForce) {
                    bestAmbJediTime = ts;
                    bestAmbJediForce = fs; 
                    bestAmbSithTime = maybeAmbSithTime; 
                    bestAmbSithForce = maybeAmbSithForce; 
                    ambState = State::SeenBoth; 
                }
            }
        }
    }
    
public: 
    Planet(int plNum):
    planetNum(plNum) {} 
    
    void insertDeployment(int ts, char c, int g, int fs, int t, bool mv) {
        if (mv) {
            findBestAtk(ts,c,fs); 
            findBestAmb(ts,c,fs);
        }
        if (c == 'S') 
            siths.push(deployment{g,fs,t,arrival++});
        else
            jedis.push(deployment{g,fs,t,arrival++}); 
    }
    
    void battle(bool verbose, bool genEval, bool median, int &battles, vector<Gen> &generals) {
        while (!siths.empty() && !jedis.empty() && (siths.top().forceSens >= jedis.top().forceSens)) {
            int &sithTroops = siths.top().troops; 
            int &jediTroops = jedis.top().troops; 
            int troopsLost = min(sithTroops, jediTroops); 
            int sithGenId = siths.top().gen;
            int jediGenId = jedis.top().gen;
            if (verbose) {
                cout << "General " << sithGenId << "'s battalion attacked General "
                     << jediGenId << "'s battalion on planet " << planetNum  
                     << ". " << (troopsLost * 2) 
                     << " troops were lost." << "\n";
            }
            if (genEval) {
                generals[sithGenId].dead += troopsLost; 
                generals[jediGenId].dead += troopsLost; 
            }
            if (median) {
                pushBattle(troopsLost*2); 
            }
            sithTroops -= troopsLost;
            jediTroops -= troopsLost; 
            checkTroops(); 
            battled = true; 
            ++battles;
        }
    }
    
    void printMWAttack() {
        if (bestAtkSithTime == -1) 
            bestAtkJediTime = -1; 
        cout << "A movie watcher would enjoy an attack on planet " 
             << planetNum << " with Jedi at time " << bestAtkJediTime 
             << " and Sith at time " << bestAtkSithTime << ".\n"; 
    }
    
    void printMWAmbush() {
        if (bestAmbJediTime == -1)
            bestAmbSithTime = -1;
        cout << "A movie watcher would enjoy an ambush on planet " 
             << planetNum << " with Sith at time " << bestAmbSithTime 
             << " and Jedi at time " << bestAmbJediTime << ".\n"; 
    }
    
    void checkTroops() {
        if (siths.top().troops <= 0) 
            siths.pop();
        if (jedis.top().troops <= 0) 
            jedis.pop(); 
    }
    
    int median() {
        if (leftHalf.empty() && rightHalf.empty()) 
            return 0;
        if (leftHalf.size() > rightHalf.size()) 
            return leftHalf.top(); 
        else if (rightHalf.size() > leftHalf.size()) 
            return rightHalf.top(); 
        else 
            return (leftHalf.top() + rightHalf.top()) / 2;
    }
    
    int getNum() {
        return planetNum; 
    }
    
    bool didBattle() {
        return battled; 
    }
}; 

class Galaxy {
private: 
	bool verbose = false,
		 median = false,
		 genEval = false,
		 watcher = false;
	char inputMode; 
	int numGens ,
        numPlanets,
        battles = 0; 
    vector<Planet> planets; 
    vector<Gen> generals; 
    
	void readInput() {
        planets_init(); 
		stringstream ss; 
		if (inputMode == 'P') {
			prInput(ss); 
		}
		istream &inputStream = inputMode == 'P' ? ss : cin; 
        string ts, s, g, p, fs, t; 
        int current_timestamp = 0;
        cout << "Deploying troops..." << '\n';
        while (inputStream >> ts >> s >> g >> p >> fs >> t) { 
            int time = stoi(ts), 
                gen = stoi(g.substr(1)), 
                pl = stoi(p.substr(1)), 
                fSens = stoi(fs.substr(1)), 
                trps = stoi(t.substr(1));
            errorCheck(time,current_timestamp,gen,pl,fSens,trps);
            if (genEval) {
                if (s[0] == 'S')
                    generals[gen].siths += trps; 
                else 
                    generals[gen].jedis += trps;
            }
            if (median) {
                if (current_timestamp != time) //has the timestamp changed 
                    printMedianData(current_timestamp); //has there been a battle at the previous timestamp (current_timestamp)
            }
            current_timestamp = time; 
            planets[pl].insertDeployment(time, s[0], gen, fSens, trps, watcher);
            planets[pl].battle(verbose, genEval, median, battles, generals); 
        } // while
        if (median) 
            printMedianData(current_timestamp);
        cout << "---End of Day---" << '\n' << "Battles: " << battles << '\n';
        if (genEval) {
            cout << "---General Evaluation---" << '\n';
            int count = 0; 
            for (auto &i : generals) {
                int total = i.jedis + i.siths;
                cout << "General " << count << " deployed " << i.jedis << " Jedi troops and " 
                     << i.siths << " Sith troops, and " << (total - i.dead) << "/" << total 
                     << " troops survived.\n";
                ++count;
            }
        }
        if (watcher) {
            cout << "---Movie Watcher---" << '\n';
            for (auto &i : planets) {
                i.printMWAmbush();
                i.printMWAttack();
            }
        }
    }
    
    void planets_init() {
        //reading first 4 lines of file
        string in, junk; 
		getline(cin, junk);
        cin >> junk >> in; 
        inputMode = in[0];
        cin >> junk >> in; 
        numGens = stoi(in);
        cin >> junk >> in;
        numPlanets = stoi(in);
        planets.reserve(numPlanets); 
        for (int i = 0; i < numPlanets; ++i) {
            planets.push_back(Planet(i));
        }
        if (genEval) {
            generals.reserve(numGens); 
            for (int i = 0; i < numGens; ++i) {
                generals.push_back(Gen{0,0,0});
            }
        }
    }
    
    void errorCheck(int ts, int ct, int g, int p, int fs, int t) {
        if (ts < ct) {
            cerr << "Invalid timestamp" << '\n';
            exit(1); 
        }
        if (g < 0 || g >= numGens) {
            cerr << "Invalid general" << '\n';
            exit(1); 
        }
        if (p < 0 || p >= numPlanets) {
            cerr << "Invalid planet" << '\n';
            exit(1); 
        }
        if (fs <= 0) {
            cerr << "Invalid force sensitivity" << '\n';
            exit(1);
        }
        if (t <= 0) {
            cerr << "Invalid amount of troops" << '\n';
            exit(1);
        }
    }
    
	void prInput(stringstream &ss){
		string in, junk; 
        cin >> junk >> in; 
		uint32_t seed = static_cast<uint32_t>(stoi(in)); 
		cin >> junk >> in; 
		uint32_t numDeployments = static_cast<uint32_t>(stoi(in)); 
		cin >> junk >> in; 
		uint32_t rate = static_cast<uint32_t>(stoi(in));
		P2random::PR_init(ss, seed, numGens, numPlanets, numDeployments, rate);
	}
    
    void printMedianData(int time) {
        for (auto &i : planets) {
            if (i.didBattle()) {
                cout << "Median troops lost on planet " << i.getNum() 
                        << " at time " << time << " is " 
                        << i.median() << ".\n";
            }
        }
    }
    
public: 
    Galaxy(int argc, char *argv[]) {
        getMode(argc, argv);
        readInput(); 
    }
	// Print help for the user when requested.
    // argv[0] is the name of the currently executing program
    void printHelp(char *argv[]) {
        cout << "This program is simulate a star wars battle" << endl;
        cout << "Usage: " << argv[0] << " -v|-m|-g|-w|-h" << endl;
        cout << "Use -v for verbose ouput." << endl;
        cout << "Use -m for median output." << endl;
        cout << "Use -g for general evaluation output." << endl;
		cout << "Use -w for movie watcher output." << endl;
    } // printHelp()

    void getMode(int argc, char * argv[]) {
        // These are used with getopt_long()
        opterr = false; // Let us handle all error output for command line options
        int choice;
        int option_index = 0;
        option long_options[] = {
            // Fill in two lines, for the "mode" ('m') and
            // the "help" ('h') options.
            { "verbose", no_argument,      nullptr, 'v'},
            { "median", no_argument,       nullptr, 'm'},
            { "general-eval", no_argument, nullptr, 'g'},
			{ "watcher", no_argument,      nullptr, 'w'},
            { "help",  no_argument,        nullptr, 'h'},
            { nullptr, 0,                  nullptr, '\0'}
        };
        // cout << getopt_long(argc, argv, "m:h", long_options, &option_index) << endl; 
        // Fill in the double quotes, to match the mode and help options.
        while ((choice = getopt_long(argc, argv, "vmgwh", long_options, &option_index)) != -1) {
            switch (choice) {
            case 'h':
                printHelp(argv);
                exit(0);
            case 'v':
                verbose = true;
                break;
			case 'm':
                median = true;
                break;
            case 'g':
                genEval = true;
                break;
			case 'w':
                watcher = true;
                break;
            default:
                cerr << "Unknown command line option" << endl;
                exit(1);
            } // switch
        } // while
    } // getMode()
};

int main(int argc, char *argv[]) {
    // This should be in all of your projects, speeds up I/O
    ios_base::sync_with_stdio(false);
    Galaxy s(argc, argv); 
    return 0;
} // main()
