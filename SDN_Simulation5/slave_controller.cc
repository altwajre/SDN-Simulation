/*
 * slave_controller.cc
 *
 *  Created on: 2019年2月4日
 *      Author: qinhongbo
 */


#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <condition.h>
#include "switch_message_m.h"

using namespace omnetpp;
using namespace std;

class slave_controller : public cSimpleModule
{
    private:
        double loss[20][20], transmissionDelay[20][20], queuingDelay[20][20];
        double availableBandwidth[20][20], totalBandwidth[20][20];
        int nex[20][20];

    protected:
      virtual void initialize() override;
      virtual void handleMessage(cMessage *msg) override;
      virtual void copyCondition(condition* cond);
};

Define_Module(slave_controller);

void slave_controller::initialize()
{
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            loss[i][j] = transmissionDelay[i][j] = -1;
            queuingDelay[i][j] = availableBandwidth[i][j] = -1;
            totalBandwidth[i][j] = -1;
        }
    }
}

// this part will be updated later
void slave_controller::handleMessage(cMessage *msg)
{
    string from = msg->getSenderModule()->getName();
    if (from == "switches") {
        switch_message* tempmsg = check_and_cast<switch_message *>(msg);
        int idx = tempmsg->getSource();
        for (int i = 0; i < 20; ++i) {
            loss[idx][i] = tempmsg->getLoss(i);
            transmissionDelay[idx][i] = tempmsg->getTransmissionDelay(i);
            queuingDelay[idx][i] = tempmsg->getQueuingDelay(i);
            availableBandwidth[idx][i] = tempmsg->getAvailableBandwidth(i);
            totalBandwidth[idx][i] = tempmsg->getTotalBandwidth(i);
        }
    } else if (from == "domain") {
        // send back the network condition information
        condition* cond = new condition();
        copyCondition(cond);
        msg->addObject(cond);
        send(msg->dup(), "domain");
    }
}

void slave_controller::copyCondition(condition* cond) {
    for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 20; ++j) {
                cond->loss[i][j] = loss[i][j];
                cond->transmissionDelay[i][j] = transmissionDelay[i][j];
                cond->queuingDelay[i][j] = queuingDelay[i][j];
                cond->availableBandwidth[i][j] = availableBandwidth[i][j];
                cond->totalBandwidth[i][j] = totalBandwidth[i][j];
            }
        }
}



