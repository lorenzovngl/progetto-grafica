#ifndef PROGETTOGRAFICA_LEADERBOARD_H
#define PROGETTOGRAFICA_LEADERBOARD_H

#define LEADERBOARD_LENGHT 20

class Leaderboard {
private:
    char mUsername[50];
    int mNumBuoys;
public:
    class LBItem {
    public:
        char name[50];
        int numBuoys;
        int time;

        LBItem(char *pname, int pnum, int ptime) {
            strcpy(name, pname);
            numBuoys = pnum;
            time = ptime;
        }
    };

    LBItem *items[LEADERBOARD_LENGHT];

    Leaderboard(char *username);

    char *getUsename();

    void setUsename(char *username);

    void setNumBuoys(int num);

    LBItem **read();

    void insert(int timeScore);
};


#endif
