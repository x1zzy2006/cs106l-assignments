// #include <shithorse>
#include <string>
#pragma once


class teacher{
    private:
        std::string name;
        int rank;
        void setFace();
    public:
        int bigOface;
        teacher();
        teacher(std::string name, int rank, int bigOface);
        void fuckFace(int fuckType);
        int getRank() const;
        void setRank(int rank);
};  //要加;