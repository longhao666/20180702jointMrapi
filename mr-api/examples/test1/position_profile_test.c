// position profile test.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "MC_PositionProfile.h"
#include "mrapi.h"
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

JOINT_HANDLE joint1;
position_profile_t position_pro_strut;
volatile int stop_timer = 1;

float Pos_LinearRampPro_Init(void)
{
	int32_t initPos = 0;
	float initPos_f = 0.0f;
	float goalPos = 0;
    jointGetPosition(joint1, &initPos, 1000, NULL);
	initPos_f = (float)initPos / 65536.0f*360.0f;
    printf("\nCurrent position is %f, Please enter goal position : ", initPos_f);
    scanf("%f", &goalPos);
    position_profile_init(&position_pro_strut, goalPos, initPos_f,
		60.0f, 180.0f);
	return goalPos;
}

int TimerThreadLoop()
{
	int32_t ret;
	static int j = 0;
    JOINT_HANDLE handle = joint1;
	float realPos = 0.0f;
	float q, v;

    if (MR_ERROR_OFFLINE == jointPoll(handle, &realPos, NULL, NULL)){
//        jointDown(handle);
        printf("Joint1 offline 0x%x\n", handle);
        return 0;
    }
    printf("real position: %3.3f\t", realPos);

    if (j < position_pro_strut.steps) {

        q = position_profile_caculate(&position_pro_strut, j++);
        v = position_pro_strut.v;

        ret = jointPush(handle, q, v, 0);
        printf("target position: %3.3f\n", q);
    }
    else {
        if (fabs(realPos - position_pro_strut.qf) > 0.006) {
            ret = jointPush(handle, position_pro_strut.qf, 0, 0);
            printf("target position: %3.3f\n", q);
        }
        else {
            Pos_LinearRampPro_Init();
            j = 0;
        }
    }
}

#define maxval(a,b) ((a>b)?a:b)
void StartTimerLoop(int32_t hz, void* timer_notify)
{
    timer_t timerid;
    static struct timeval last_sig;
    struct itimerspec timerValues;
    struct sigevent sigev;
    float val;
    if (hz == 0) val = 0.0f;
    else val = 1000000.0/(float)hz;
    long tv_nsec = 1000 * (maxval((int)val,1)%1000000);
    time_t tv_sec = val/1000000;

    // Take first absolute time ref.
    if(gettimeofday(&last_sig,NULL)){
    }

    memset (&sigev, 0, sizeof (struct sigevent));
    sigev.sigev_value.sival_int = 0;
    sigev.sigev_notify = SIGEV_THREAD;
    sigev.sigev_notify_attributes = NULL;
    sigev.sigev_notify_function = timer_notify;

    if(timer_create (CLOCK_REALTIME, &sigev, &timerid)) {
        printf("timer created.\n");
    }

    timerValues.it_value.tv_sec = 1;
    timerValues.it_value.tv_nsec = 0;
    timerValues.it_interval.tv_sec = tv_sec;
    timerValues.it_interval.tv_nsec = tv_nsec;

    if(timer_settime (timerid, 0, &timerValues, NULL) == -1) {
        perror("fail to timer_settime");
        exit(-1);
    }
}

int main()
{
	float goalPos = 0;

//    int32_t errcode = startMaster("enp0s25-1", MASTER(0));
    int32_t errcode = startMaster("pcanusb1", MASTER(0));
    printf("Master Started 0x%X\n", errcode);
    if (errcode != MR_ERROR_OK) exit(-1);

    joint1 = jointUp(0x01, MASTER(0));
	if (joint1) {
        if (jointSetMode(joint1, joint_cyclesync, 5000, NULL) == MR_ERROR_ACK1) {
            printf("Set mode to MODE_CYCLESYNC.\n");
            int ret = jointSetScpInterval(joint1, 20, -1,NULL);
            printf("jointSetScpInterval ret: 0x%X\n", ret);
        }
        else {
            printf("jointSetMode timeout\n");
        }
        goalPos = Pos_LinearRampPro_Init();
        StartTimerLoop(200, TimerThreadLoop);
        joinMaster(MASTER(0));
	}
    else {
        stopMaster(MASTER(0));
    }


    return 0;
}

