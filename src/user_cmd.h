// **************************************************************************
// File       [ test_cmd.h ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2012/04/10 created ]
// **************************************************************************

#ifndef _TEST_CMD_H_
#define _TEST_CMD_H_

#include "../lib/cmd.h"

class TestCmd : public CommonNs::Cmd {
public:
         TestCmd(const char * const name);
         ~TestCmd();

    bool exec(int argc, char **argv);
};

class ReadGraphCmd : public CommonNs::Cmd {
public:
         ReadGraphCmd(const char * const name);
         ~ReadGraphCmd();

    bool exec(int argc, char **argv);
};

class WriteTreeDFSCmd : public CommonNs::Cmd {
public:
        WriteTreeDFSCmd(const char * const name);
         ~WriteTreeDFSCmd();

    bool exec(int argc, char **argv);
};

class WriteTreeBFSCmd : public CommonNs::Cmd {
public:
         WriteTreeBFSCmd(const char * const name);
         ~WriteTreeBFSCmd();

    bool exec(int argc, char **argv);
};

class DijkstraCmd : public CommonNs::Cmd {
public:
         DijkstraCmd(const char * const name);
         ~DijkstraCmd();

    bool exec(int argc, char **argv);
};

#endif

